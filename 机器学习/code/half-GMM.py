import pandas as pd
import numpy as np
from sklearn.mixture import GaussianMixture
from sklearn.metrics import pairwise_distances
from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import connected_components
import statistics

# 加载特征数据
train_features = pd.read_csv('train_features1.csv', header=None)
test_features = pd.read_csv('test_features1.csv', header=None)
print("Features loaded successfully.")

# 提取ID和特征
train_ids = train_features.iloc[:, 0].values
train_data = train_features.iloc[:, 1:].values
test_ids = test_features.iloc[:, 0].values
test_data = test_features.iloc[:, 1:].values
print(f"Train data shape: {train_data.shape}")
print(f"Test data shape: {test_data.shape}")

# 加载约束条件
constraints = pd.read_csv('constraints.csv')
constraints.columns = ['id1', 'id2', 'label']
print(constraints.head())
print(f"Constraints shape: {constraints.shape}")

# 创建约束矩阵
n_samples = train_data.shape[0]
must_link = constraints[constraints['label'] == 1]
cannot_link = constraints[constraints['label'] == -1]
print(f"Must-link constraints: {must_link.shape[0]}")
print(f"Cannot-link constraints: {cannot_link.shape[0]}")

# must-link约束
must_link_matrix = csr_matrix((np.ones(must_link.shape[0]), (must_link['id1'], must_link['id2'])), shape=(n_samples, n_samples))

# cannot-link约束
cannot_link_matrix = csr_matrix((np.ones(cannot_link.shape[0]), (cannot_link['id1'], cannot_link['id2'])), shape=(n_samples, n_samples))

# 处理must-link约束，合并相连的组件
n_components, labels = connected_components(must_link_matrix, directed=False)
print(f"Number of connected components: {n_components}")

# 创建新的特征矩阵
print("Creating new train data with must-link constraints...")
new_train_data = train_data.copy()
for component in range(n_components):
    component_indices = np.where(labels == component)[0]
    if len(component_indices) > 1:
        mean_vector = np.mean(train_data[component_indices], axis=0)
        new_train_data[component_indices] = mean_vector
print("Must-link constraints processing complete.")

# 处理cannot-link约束（简单方法，通过添加噪声）
print("Processing cannot-link constraints...")
scale = 0.01 * np.std(new_train_data, axis=0)
distances = pairwise_distances(new_train_data)
discount=[]
for i, row in cannot_link.iterrows():
    id1, id2 = int(row['id1']), int(row['id2'])
    #print("cannot:",distances[id1, id2])
    discount.append(distances[id1, id2])
    if distances[id1, id2] < 27:  # 距离太近，添加噪声
        new_train_data[id1] += np.random.normal(0, 0.01, size=new_train_data[id1].shape)
        new_train_data[id2] += np.random.normal(0, 0.01, size=new_train_data[id2].shape)
print(statistics.variance(discount))
print("Cannot-link constraints processing complete.")

# 定义聚类算法
n_clusters = 4
print("Training Gaussian Mixture Model (GMM)...")
gmm = GaussianMixture(n_components=n_clusters, random_state=42)

# 训练GMM模型
gmm.fit(new_train_data)
print("GMM training complete.")

# 获取训练数据的聚类标签
train_labels = gmm.predict(new_train_data)
print("Train labels predicted.")

# 预测测试数据的聚类标签
test_labels = gmm.predict(test_data)
print("Test labels predicted.")

# 创建提交文件
submission = pd.DataFrame({'ID': test_ids, 'Class': test_labels})

# 确保按ID排序
submission = submission.sort_values(by='ID').reset_index(drop=True)
print("Submission file created and sorted by ID.")

# 保存为CSV文件
submission.to_csv('submission.csv', index=False)
print("Submission file saved as 'submission.csv'.")
