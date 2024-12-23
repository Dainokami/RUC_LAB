import networkx as nx
import matplotlib.pyplot as plt
import time
import random
from collections import deque
from pyvis.network import Network
import matplotlib.colors as mcolors
from collections import defaultdict
import numpy as np
from networkx.algorithms.flow import edmonds_karp


class MyGraph:
    def __init__(self, file_path=None):
        self.G = nx.Graph()
        self.node_mapping = {}
        self.reverse_mapping = {}
        self.next_node_id = 0
        self.coreness = {}  
        if file_path:
            self.read(file_path)

    def _get_mapped_node(self, node):
        if node not in self.node_mapping:
            self.node_mapping[node] = self.next_node_id
            self.reverse_mapping[self.next_node_id] = node
            self.next_node_id += 1
        return self.node_mapping[node]

    def read(self, file_path):
        with open(file_path, 'r') as file:
            n, m = map(int, file.readline().strip().split())
            for line in file:
                u, v = map(int, line.strip().split())
                if u != v:  # 去除自环
                    mapped_u = self._get_mapped_node(u)
                    mapped_v = self._get_mapped_node(v)
                    self.G.add_edge(mapped_u, mapped_v)

    def save(self, output_path):
        with open(output_path, 'w') as file:
            file.write(f"node :{len(self.G.nodes())} edge : {len(self.G.edges())}\n")
            file.write(f"density is :{self.calculate_density()}\n")
            file.write(f"average_degree is :{self.calculate_average_degree()}\n")
            for u, v in self.G.edges():
                file.write(f"{self.reverse_mapping[u]} {self.reverse_mapping[v]}\n")

    def add_node(self, node):
        mapped_node = self._get_mapped_node(node)
        self.G.add_node(mapped_node)

    def remove_node(self, node):
        mapped_node = self._get_mapped_node(node)
        self.G.remove_node(mapped_node)
        del self.node_mapping[node]
        del self.reverse_mapping[mapped_node]

    def add_edge(self, u, v):
        if u != v:
            mapped_u = self._get_mapped_node(u)
            mapped_v = self._get_mapped_node(v)
            self.G.add_edge(mapped_u, mapped_v)

    def remove_edge(self, u, v):
        mapped_u = self._get_mapped_node(u)
        mapped_v = self._get_mapped_node(v)
        if self.G.has_edge(mapped_u, mapped_v):
            self.G.remove_edge(mapped_u, mapped_v)

    def calculate_density(self):
        density = nx.density(self.G)
        return density

    def calculate_average_degree(self):
        degrees = dict(self.G.degree()).values()
        avg_degree = sum(degrees) / len(degrees) if len(degrees) > 0 else 0
        return avg_degree


    ##求K—core，所有coreness
    def k_cores(self, output_path):
        start_time = time.time()
        
        # 初始化节点的度数
        degrees = dict(self.G.degree())
        k_core = {}
        
        # 初始化候选队列
        candidates = set(self.G.nodes())
        
        k = 0
        while candidates:
            k += 1
            while True:
                # 找出度数小于 k 的节点
                to_remove = [node for node in candidates if degrees[node] < k]
                if not to_remove:
                    break
                
                for node in to_remove:
                    k_core[node] = k - 1
                    candidates.remove(node)
                    for neighbor in self.G.neighbors(node):
                        if neighbor in candidates:
                            degrees[neighbor] -= 1
            
        for node in candidates:
            k_core[node] = k
        
        elapsed_time = time.time() -start_time
        self.coreness = k_core

        with open(output_path, 'w') as file:
            file.write(f"{elapsed_time:.6f}s\n")
            for node in sorted(k_core, key=lambda x: self.reverse_mapping[x]):
                file.write(f"{self.reverse_mapping[node]} {k_core[node]}\n")

    #Goldberg 算法精确算法
    def exact_densest(self, output_path):
        def construct_network(G, g, m):
            N = nx.DiGraph()
            s, t = 's', 't'
            for i in G.nodes():
                N.add_edge(s, i, capacity=m)
                N.add_edge(i, t, capacity=m + 2 * g - G.degree(i))
            for u, v in G.edges():
                N.add_edge(u, v, capacity=1)
                N.add_edge(v, u, capacity=1)
            return N, s, t

        n = len(self.G.nodes())
        m = len(self.G.edges())
        l, u = 0, m
        V1 = set()
        start_time = time.time()

        while u - l >= 1 / (n * (n - 1)):
            g = (u + l) / 2
            N, s, t = construct_network(self.G, g, m)
            cut_value, partition = nx.minimum_cut(N, s, t, flow_func=edmonds_karp)
            S, T = partition

            if S == {s}:
                u = g
            else:
                l = g
                V1 = S - {s}

        end_time = time.time()
        elapsed_time = end_time - start_time
        density =  len(self.G.subgraph(V1).edges()) / len(V1) if len(V1) > 0 else 0
        self.densest_subgraph_greedy = V1
        self.highest_density_greedy = density
        with open(output_path, 'w') as file:
            file.write(f"{elapsed_time:.6f}s\n")
            file.write(f"{density}\n")
            file.write(" ".join(str(self.reverse_mapping[node]) for node in V1))


    ##2-近似最密子图
    def approximate_densest(self, output_path):
        start_time = time.time()

        def peel_degeneracy_ordering(G):
            ordering = []
            degrees = {node: deg for node, deg in G.degree()}
            while degrees:
                min_node = min(degrees, key=degrees.get)
                ordering.append(min_node)
                del degrees[min_node]
                for neighbor in G.neighbors(min_node):
                    if neighbor in degrees:
                        degrees[neighbor] -= 1
            return ordering

        degeneracy_order = peel_degeneracy_ordering(self.G)
        densest_subgraph = None
        highest_density = 0
        S = set(self.G.nodes())
        m_S = self.G.number_of_edges()

        for u in degeneracy_order:
            density = m_S / len(S) if len(S) > 0 else 0
            if densest_subgraph is None or density > highest_density:
                highest_density = density
                densest_subgraph = S.copy()
            S.remove(u)
            for v in self.G.neighbors(u):
                if v in S:
                    m_S -= 1

        elapsed_time = time.time() - start_time
        self.densest_subgraph_greedy = densest_subgraph
        self.highest_density_greedy = highest_density
        with open(output_path, 'w') as file:
            file.write(f"{elapsed_time:.6f}s\n")
            file.write(f"{highest_density}\n")
            for node in densest_subgraph:
                file.write(f"{self.reverse_mapping[node]} ")
            file.write("\n")
    
    ##BK算法
    def bron_kerbosch(self, R, P, X, cliques):
        if len(P) == 0 and len(X) == 0:
            cliques.append(R)
            return
        for v in list(P):
            new_R = R | {v}
            new_P = P & set(self.G.neighbors(v))
            new_X = X & set(self.G.neighbors(v))
            self.bron_kerbosch(new_R, new_P, new_X, cliques)
            P.remove(v)
            X.add(v)
    
    ##使用BK的k-clique分解
    def k_clique(self, k, output_path):
        start_time = time.time()
        
        # 使用 Bron-Kerbosch 算法查找所有极大团
        cliques = []
        P = set(self.G.nodes())
        self.bron_kerbosch(set(), P, set(), cliques)
        
        # 选择 k 的团
        cliques = [clique for clique in cliques if len(clique) == k]
        
        elapsed_time = time.time() - start_time
        self.cliques = cliques
        with open(output_path, 'w') as file:
            file.write(f"{elapsed_time:.6f}s\n")
            for clique in cliques:
                file.write(" ".join(str(self.reverse_mapping[node]) for node in clique) + "\n")

    def Color(self, u, c):
        self.visited[u] = 1
        if self.color[u] == 0:
            self.V_c.add(u)
            self.color[u] = 1
        for w in self.G.neighbors(u):
            if self.visited[w] == 0 and self.coreness[w] == c:
                self.Color(w, c)

    def RecolorInsert(self, c):
        flag = 0
        for u in self.V_c:
            if self.color[u] == 1:
                X_u = 0
                for w in self.G.neighbors(u):
                    if self.color[w] == 1 or self.coreness[w] > c:
                        X_u += 1
                if X_u <= c:
                    self.color[u] = 0
                    flag = 1
        if flag == 1:
            self.RecolorInsert(c)

    def UpdateInsert(self, c):
        for w in self.V_c:
            if self.color[w] == 1:
                self.coreness[w] = c + 1

    def RecolorDelete(self, c):
        flag = 0
        for u in self.V_c:
            if self.color[u] == 1:
                X_u = 0
                for w in self.G.neighbors(u):
                    if self.color[w] == 1 or self.coreness[w] > c:
                        X_u += 1
                if X_u < c:
                    self.color[u] = 0
                    flag = 1
        if flag == 1:
            self.RecolorDelete(c)

    def UpdateDelete(self, c):
        for w in self.V_c:
            if self.color[w] == 0:
                self.coreness[w] = c - 1

    def Insertion(self, u, v):
        if self.coreness[u] > self.coreness[v]:
            c = self.coreness[v]
            self.Color(v, c)
            self.RecolorInsert(c)
            self.UpdateInsert(c)
        else:
            c = self.coreness[u]
            self.Color(u, c)
            self.RecolorInsert(c)
            self.UpdateInsert(c)

    def Deletion(self, u, v):
        if self.coreness[u] > self.coreness[v]:
            c = self.coreness[v]
            self.Color(v, c)
            self.RecolorDelete(c)
            self.UpdateDelete(c)
        if self.coreness[u] < self.coreness[v]:
            c = self.coreness[u]
            self.Color(u, c)
            self.RecolorDelete(c)
            self.UpdateDelete(c)
        if self.coreness[u] == self.coreness[v]:
            c = self.coreness[u]
            self.Color(u, c)
            if self.color[v] == 0:
                self.visited = {node: 0 for node in self.G.nodes()}
                self.Color(v, c)
                self.RecolorDelete(c)
                self.UpdateDelete(c)
            else:
                self.RecolorDelete(c)
                self.UpdateDelete(c)

    ##动态的kcore
    def Dy_kcore(self, edge, output_path):
        u, v = edge
        if u == v:
            return
        mapped_u = self._get_mapped_node(u)
        mapped_v = self._get_mapped_node(v)

        start_time = time.time()
        flag = self.G.has_edge(mapped_u, mapped_v)
        if not self.G.has_edge(mapped_u, mapped_v):
            self.G.add_edge(mapped_u, mapped_v)
            self.visited = {node: 0 for node in self.G.nodes()}
            self.color = {node: 0 for node in self.G.nodes()}
            self.V_c = set()
            self.Insertion(mapped_u, mapped_v)
        else:
            self.G.remove_edge(mapped_u, mapped_v)
            self.visited = {node: 0 for node in self.G.nodes()}
            self.color = {node: 0 for node in self.G.nodes()}
            self.V_c = set()
            self.Deletion(mapped_u, mapped_v)

        elapsed_time = time.time() - start_time

        with open(output_path, 'w') as file:
            file.write(f"{elapsed_time:.6f}s\n")
            for node in sorted(self.coreness, key=lambda x: self.reverse_mapping[x]):
                file.write(f"{self.reverse_mapping[node]} {self.coreness[node]}\n")
        return flag

    def visualize(self, node_color='skyblue', node_size=300, font_size=12, edge_color='grey'):
        pos = nx.spring_layout(self.G)  # 可以使用其他布局算法
        plt.figure(figsize=(12, 8))
        nx.draw(self.G, pos, with_labels=True, labels=self.reverse_mapping, node_color=node_color, node_size=node_size, edge_color=edge_color, font_size=font_size)
        plt.show()
    
    #整图可视化
    def interactive_visualize(self, output_path, notebook=False):
        net = Network(notebook=notebook)

        # 默认颜色和大小设置
        default_node_color = 'skyblue'
        default_node_size = 20

        # 使用默认颜色、大小和标签格式
        for node in self.G.nodes():
            title = f"The Graph"
            net.add_node(node, label=str(self.reverse_mapping[node]), title=title, color=default_node_color, size=default_node_size)
        
        for edge in self.G.edges():
            net.add_edge(edge[0], edge[1], color='grey')

        # 设置网络图的高度和宽度以填满浏览器窗口
        net.set_options("""
        var options = {
            "nodes": {
                "color": {
                    "background": "#222222",
                    "border": "#ffffff",
                    "highlight": {
                        "background": "#ff0000",
                        "border": "#00ff00"
                    }
                },
                "font": {
                    "color": "#000000"
                }
            },
            "edges": {
                "color": {
                    "color": "#ffffff",
                    "highlight": "#ff0000",
                    "hover": "#00ff00"
                }
            },
            "interaction": {
                "navigationButtons": true,
                "zoomView": true
            },
            "physics": {
                "enabled": true
            }
        }
        """)

        # 生成 HTML 文件
        net.write_html(output_path, notebook=notebook)

        # 读取生成的 HTML 文件并插入自定义 CSS 和 JavaScript
        with open(output_path, 'r') as file:
            html_content = file.read()

        custom_css_js = """
        <style>
            html, body {
                height: 100%;
                margin: 0;
                padding: 0;
            }
            #mynetwork {
                height: 90vh;  /* 将网络图高度设置为90%，以便留出空间放按钮和滑动条 */
                width: 100vw;
            }
            #controls {
                height: 10vh;
                width: 100vw;
                display: flex;
                justify-content: center;
                align-items: center;
                background-color: #333;
                color: white;
            }
            .color-button {
                margin: 0 10px;
                padding: 10px;
                cursor: pointer;
                border: none;
                background-color: white;
                color: black;
            }
            .color-button:hover {
                background-color: #ddd;
            }
        </style>
        <div id="controls">
            <button class="color-button" onclick="changeColor('red')">Red</button>
            <button class="color-button" onclick="changeColor('green')">Green</button>
            <button class="color-button" onclick="changeColor('blue')">Blue</button>
            <button class="color-button" onclick="changeColor('yellow')">yellow</button>
            <button class="color-button" onclick="changeColor('pink')">pink</button>
            <button class="color-button" onclick="changeColor('purple')">purple</button>
            <input type="range" min="10" max="100" value="20" id="sizeSlider" onchange="changeSize(this.value)">
        </div>
        <script type="text/javascript">
            function changeColor(color) {
                var nodes = network.body.data.nodes.get();
                nodes.forEach(function(node) {
                    node.color = color;
                });
                network.body.data.nodes.update(nodes);
            }
            function changeSize(size) {
                var nodes = network.body.data.nodes.get();
                nodes.forEach(function(node) {
                    node.size = size;
                });
                network.body.data.nodes.update(nodes);
            }
        </script>
        """

        # 插入自定义 CSS 和 JavaScript 到 HTML 文件中
        html_content = html_content.replace('<head>', f'<head>{custom_css_js}')

        # 写回修改后的 HTML 文件
        with open(output_path, 'w') as file:
            file.write(html_content)

    #kcore可视化
    def visualize_kcore(self, output_path, notebook=False):
        net = Network(notebook=notebook)

        # 计算 coreness 范围
        max_coreness = max(self.coreness.values())
        min_coreness = min(self.coreness.values())

        # 定义颜色映射
        colormap = plt.get_cmap('viridis', max_coreness - min_coreness + 1)

        # 根据 coreness 设置节点颜色和标签
        for node in self.G.nodes():
            coreness = self.coreness[node]
            color = mcolors.to_hex(colormap((coreness - min_coreness) / (max_coreness - min_coreness)))
            title = f"Core: {coreness}"
            net.add_node(node, label=f"{self.reverse_mapping[node]} (core: {coreness})", title=title, color=color, size=20 + 10 * coreness)
        
        for edge in self.G.edges():
            net.add_edge(edge[0], edge[1], color='grey')

        # 设置网络图的高度和宽度以填满浏览器窗口
        net.set_options("""
        var options = {
            "nodes": {
                "font": {
                    "color": "#000000"
                }
            },
            "edges": {
                "color": {
                    "color": "#ffffff",
                    "highlight": "#ff0000",
                    "hover": "#00ff00"
                }
            },
            "interaction": {
                "navigationButtons": true,
                "zoomView": true
            },
            "physics": {
                "enabled": true
            }
        }
        """)

        # 生成 HTML 文件
        net.write_html(output_path, notebook=notebook)

        # 读取生成的 HTML 文件并插入自定义 CSS 和 JavaScript
        with open(output_path, 'r') as file:
            html_content = file.read()

        custom_css_js = """
        <style>
            html, body {
                height: 100%;
                margin: 0;
                padding: 0;
            }
            #mynetwork {
                height: 90vh;  /* 将网络图高度设置为90%，以便留出空间放按钮和滑动条 */
                width: 100vw;
            }
            #controls {
                height: 10vh;
                width: 100vw;
                display: flex;
                justify-content: center;
                align-items: center;
                background-color: #333;
                color: white;
            }
            .color-button {
                margin: 0 10px;
                padding: 10px;
                cursor: pointer;
                border: none;
                background-color: white;
                color: black;
            }
            .color-button:hover {
                background-color: #ddd;
            }
        </style>
        <div id="controls">
            <input type="range" min="10" max="100" value="20" id="sizeSlider" onchange="changeSize(this.value)">
        </div>
        <script type="text/javascript">
            function changeSize(size) {
                var nodes = network.body.data.nodes.get();
                nodes.forEach(function(node) {
                    node.size = size;
                });
                network.body.data.nodes.update(nodes);
            }
        </script>
        """

        # 插入自定义 CSS 和 JavaScript 到 HTML 文件中
        html_content = html_content.replace('<head>', f'<head>{custom_css_js}')

        # 写回修改后的 HTML 文件
        with open(output_path, 'w') as file:
            file.write(html_content)

    #最密子图可视化
    def visualize_densest_subgraph(self, output_path, notebook=False):
        net = Network(notebook=notebook)

        # 根据 densest_subgraph 设置节点颜色和标签
        for node in self.G.nodes():
            if node in self.densest_subgraph_greedy:
                color = 'red'
                size = 30
                label = f"{self.reverse_mapping[node]} (densest)"
                title = f"Densest Subgraph Node"
            else:
                color = 'grey'
                size = 20
                label = f"{self.reverse_mapping[node]}"
                title = f"Node"
            net.add_node(node, label=label, title=title, color=color, size=size)
        
        for edge in self.G.edges():
            if edge[0] in self.densest_subgraph_greedy and edge[1] in self.densest_subgraph_greedy:
                net.add_edge(edge[0], edge[1], color='red')
            else:
                net.add_edge(edge[0], edge[1], color='grey')

        # 设置网络图的高度和宽度以填满浏览器窗口
        net.set_options("""
        var options = {
            "nodes": {
                "font": {
                    "color": "#000000"
                }
            },
            "edges": {
                "color": {
                    "color": "#ffffff",
                    "highlight": "#ff0000",
                    "hover": "#00ff00"
                }
            },
            "interaction": {
                "navigationButtons": true,
                "zoomView": true
            },
            "physics": {
                "enabled": true
            }
        }
        """)

        # 生成 HTML 文件
        net.write_html(output_path, notebook=notebook)

        # 读取生成的 HTML 文件并插入自定义 CSS 和 JavaScript
        with open(output_path, 'r') as file:
            html_content = file.read()

        custom_css_js = """
        <style>
            html, body {
                height: 100%;
                margin: 0;
                padding: 0;
            }
            #mynetwork {
                height: 90vh;  /* 将网络图高度设置为90%，以便留出空间放按钮和滑动条 */
                width: 100vw;
            }
            #controls {
                height: 10vh;
                width: 100vw;
                display: flex;
                justify-content: center;
                align-items: center;
                background-color: #333;
                color: white;
            }
            .color-button {
                margin: 0 10px;
                padding: 10px;
                cursor: pointer;
                border: none;
                background-color: white;
                color: black;
            }
            .color-button:hover {
                background-color: #ddd;
            }
        </style>
        <div id="controls">
            <input type="range" min="10" max="100" value="20" id="sizeSlider" onchange="changeSize(this.value)">
        </div>
        <script type="text/javascript">
            function changeSize(size) {
                var nodes = network.body.data.nodes.get();
                nodes.forEach(function(node) {
                    node.size = size;
                });
                network.body.data.nodes.update(nodes);
            }
        </script>
        """

        # 插入自定义 CSS 和 JavaScript 到 HTML 文件中
        html_content = html_content.replace('<head>', f'<head>{custom_css_js}')

        # 写回修改后的 HTML 文件
        with open(output_path, 'w') as file:
            file.write(html_content)

    #k-clique分解可视化
    def visualize_cliques(self, output_path, notebook=False):
        net = Network(notebook=notebook)

        # 定义颜色映射
        colormap = plt.get_cmap('tab20', len(self.cliques))

        # 跟踪每个节点所属的极大团
        node_cliques = defaultdict(list)
        for idx, clique in enumerate(self.cliques):
            for node in clique:
                node_cliques[node].append(idx)

        # 根据 cliques 设置节点颜色和标签
        for node, cliques in node_cliques.items():
            if len(cliques) == 1:
                color = mcolors.to_hex(colormap(cliques[0]))
            else:
                # 计算组合颜色
                colors = [colormap(c) for c in cliques]
                n_colors = len(colors)
                color = np.mean(colors, axis=0)
                color = mcolors.to_hex(color)

            title = f"Cliques: {', '.join(map(str, cliques))}"
            label = f"{self.reverse_mapping[node]} (cliques: {len(cliques)})"
            net.add_node(node, label=label, title=title, color=color, size=30)

        # 添加边
        for idx, clique in enumerate(self.cliques):
            color = mcolors.to_hex(colormap(idx))
            for node in clique:
                for neighbor in clique:
                    if node != neighbor:
                        net.add_edge(node, neighbor, color=color)

        # 设置网络图的高度和宽度以填满浏览器窗口
        net.set_options("""
        var options = {
            "nodes": {
                "font": {
                    "color": "#000000"
                }
            },
            "edges": {
                "color": {
                    "color": "#ffffff",
                    "highlight": "#ff0000",
                    "hover": "#00ff00"
                }
            },
            "interaction": {
                "navigationButtons": true,
                "zoomView": true
            },
            "physics": {
                "enabled": true
            }
        }
        """)

        # 生成 HTML 文件
        net.write_html(output_path, notebook=notebook)

        # 读取生成的 HTML 文件并插入自定义 CSS 和 JavaScript
        with open(output_path, 'r') as file:
            html_content = file.read()

        custom_css_js = """
        <style>
            html, body {
                height: 100%;
                margin: 0;
                padding: 0;
            }
            #mynetwork {
                height: 90vh;  /* 将网络图高度设置为90%，以便留出空间放按钮和滑动条 */
                width: 100vw;
            }
            #controls {
                height: 10vh;
                width: 100vw;
                display: flex;
                justify-content: center;
                align-items: center;
                background-color: #333;
                color: white;
            }
            .color-button {
                margin: 0 10px;
                padding: 10px;
                cursor: pointer;
                border: none;
                background-color: white;
                color: black;
            }
            .color-button:hover {
                background-color: #ddd;
            }
        </style>
        <div id="controls">
            <input type="range" min="10" max="100" value="20" id="sizeSlider" onchange="changeSize(this.value)">
        </div>
        <script type="text/javascript">
            function changeSize(size) {
                var nodes = network.body.data.nodes.get();
                nodes.forEach(function(node) {
                    node.size = size;
                });
                network.body.data.nodes.update(nodes);
            }
        </script>
        """

        # 插入自定义 CSS 和 JavaScript 到 HTML 文件中
        html_content = html_content.replace('<head>', f'<head>{custom_css_js}')

        # 写回修改后的 HTML 文件
        with open(output_path, 'w') as file:
            file.write(html_content)








# 创建图对象并读取测试输入1
input_file1 = 'fb-pages-food.edges'
g1 = MyGraph(input_file1)

# 保存
g1.save(".\\output\\saved.txt")
print("全图保存至 .\output\saved.txt")
# 交互式可视化
g1.interactive_visualize(".\\output\\graph.html")
print("全图可视化保存至 .\output\graph.html")


# kcore
g1.k_cores(".\\output\\k-core.txt")
print("kcore结果保存至 .\output\k-core.txt")
#kcore可视化
g1.visualize_kcore(".\\output\\kcore_visualization_before.html")  
print("kcore可视化保存至 .\output\kcore_visualization_before.html")


#精确最密子图
g1.exact_densest(".\\output\\exact_densest.txt")
print("精确最密子图结果保存至 .\output\exact_densest.txt")
g1.visualize_densest_subgraph(".\\output\\exact_densest_visualization.html") 
print("精确最密子图可视化保存至 .\output\exact_densest_visualization.html")


# 近似最密子图
g1.approximate_densest(".\\output\\approximate_densest.txt")
print("近似最密子图结果保存至 .\output\\approximate_densest.txt")
#最密子图可视化
g1.visualize_densest_subgraph(".\\output\\approximate_densest_visualization.html") 
print("近似最密子图可视化保存至 .\output\\approximate_densest_visualization.html")


# k-clique分解
g1.k_clique(3,".\\output\\k-clique.txt")
print("k-clique分解结果保存至 .\output\k-clique.txt")
g1.visualize_cliques(".\\output\\cliques_visualization.html")
print("k-clique分解可视化保存至 .\output\cliques_visualization.html")

# 动态的kcore
edge_input = input("请输入一条边（例如：1 6）：")
u, v = map(int, edge_input.split())
result = g1.Dy_kcore((u,v),".\\output\\dynamic_Kcore.txt")
print("kcore结果保存在 .\output\dynamic_Kcore.txt")
if not result:
    print(f"添加边 ({u},{v})")
else:
    print(f"删除边 ({u},{v})")
#kcore可视化
g1.visualize_kcore(".\\output\\kcore_visualization_after.html")  
print("kcore可视化保存在  .\output\kcore_visualization_after.html")