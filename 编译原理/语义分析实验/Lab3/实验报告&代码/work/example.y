%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <algorithm>
    #include <cstdarg>
    #include <fstream>
    #include <map>
    #include <queue>
    #include <set>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <iostream>
using namespace std;

using pii = pair<int, int>;

    int yylex(void);
    void yyerror(const char *s);
    FILE *fDetail;
    FILE *ft;
    FILE *asm_file;
    extern FILE *yyin; 
    extern int lineno;
    // int yydebug=1;

#define MAX_NODES 19198

struct Node {
    char label[114];                // 节点标签
    int children[20];           // 子节点索引列表
    int n_children;             // 子节点数量
    int CHD_NO;                 // 画图的时候判断是父亲的第几个儿子。
    int Node_NO;                // 画图的时候判断父亲是谁。

    int value;
    bool isConst;
    bool isArr;
    bool isError;
    bool isGlobal;
    bool isAddr;                // 是否是根据地址定位的，如果是，则 offset 指向的是存储地址的指针，而非数组本身
    int offset;                 // offset to ebp
    int offsetInArray;          // offset in array
    int quad;

    std::vector<int> trueList, falseList;
    std::vector<Node*> values;
};


Node nodes[MAX_NODES];          // 全局节点数组
int cnt = 0;                    // 全局节点计数器
int treeCnt = 0;



int newNode(const char* label) {
    int idx = cnt++;
    strcpy(nodes[idx].label, label);
    nodes[idx].n_children = 0;
    nodes[idx].isError = 0;
    nodes[idx].isConst = false;
    nodes[idx].isArr = false;
    return idx;
}

void addChild(int parent_idx, int child_idx) {
    Node* parent = &nodes[parent_idx];
    if (parent->n_children < MAX_NODES) {
        parent->children[parent->n_children++] = child_idx;
    }
}

void copyNode(Node* dest, Node* src) {
    dest->value = src->value;
    strcpy(dest->label, src->label);

    dest->isConst = src->isConst;
    dest->isArr = src->isArr;
    dest->offset = src->offset;
    dest->isGlobal = src->isGlobal;
    dest->isAddr = src->isAddr;
    dest->offsetInArray = src->offsetInArray;

    dest->trueList = src->trueList;
    dest->falseList = src->falseList;
    dest->quad = src->quad;
    dest->values = src->values;

}


enum VarType { Int,
               ConstInt,
               Arr,
               ConstArr,
               FuncInt,
               FuncVoid,
               Addr };

struct Var {
    VarType type;
    int value;             // 用于常量
    int offset;            // 用于指示变量在栈中的位置
    bool isGlobal;         // 用于指示变量是否是全局变量，以区分处理方式
    vector<Node*> values;  // 用于函数参数，数组维度等
    vector<int> values2;   // 仅用于保存常量数组的值
    Var() {}
    Var(VarType type)
        : type(type) {}
    Var(VarType type, int value, int offset, bool isGlobal)
        : type(type), value(value), offset(offset), isGlobal(isGlobal) {}
    Var(VarType type, int value, int offset, bool isGlobal, vector<Node*> values)
        : type(type), value(value), offset(offset), isGlobal(isGlobal), values(values) {}
};

int getVarValue(Var var, int offset) {
    if (var.values2.size() < offset) {
        return 0;
    } else {
        return var.values2[offset];
    }
}


class SPrintfBuffer {
   public:
    vector<string> buffer;
    int line = 0;
    void append(const char* format, ...) {
        va_list args;
        va_start(args, format);
        int len = vsnprintf(nullptr, 0, format, args);
        va_end(args);
        char* str = new char[len + 1];
        va_start(args, format);
        vsnprintf(str, len + 1, format, args);
        va_end(args);
        buffer.push_back(str);
        line = buffer.size() - 1;
        delete[] str;
    }
    inline string& operator[](int i) { return buffer[i]; }
    void toFile(FILE* file) {
        for (auto& i : buffer) {
            fprintf(file, "%s", i.c_str());
        }
    }
};

class Assemble : public SPrintfBuffer {
   public:
    Assemble() {

    }
    int labelCount = 1;
    void call() {
        /*
        pushq	%rbp
        pushq	%r8
        pushq	%r9
        movq	%rsp, %rbp
        */
        buffer.push_back("\tpushq\t%rbp\n");
        buffer.push_back("\tpushq\t%r8\n");
        buffer.push_back("\tpushq\t%r9\n");
        buffer.push_back("\tmovq\t%rsp, %rbp\n");
    }

    void ret() {
        /*
        popq	%r9
        popq	%r8
        popq	%rbp
        ret
        */
        buffer.push_back("\tpopq\t%r9\n");
        buffer.push_back("\tpopq\t%r8\n");
        buffer.push_back("\tpopq\t%rbp\n");
        buffer.push_back("\tret\n");
    }

    // addr means put the address of the variable into the register, otherwise put the value
    void var2reg(Node* node, const char* reg, bool addr = false,int is_c = false) {
        if (node->isConst) {
            this->append("\tmovl\t$%d, %s\n", node->value, reg);
        } else if (node->isGlobal) {  // 全局
            if (node->isArr) {
                this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                this->append("\tcltq\n");
                this->append("\tleaq\t0(, %%rbx, 4), %%rdx\n");
                this->append("\tleaq\t%s(%%rip), %%rbx\n", node->label);
                if (!addr) {
                    this->append("\tmovl\t(%%rdx, %%rbx), %s\n", reg);
                } else {
                    this->append("\tleaq\t(%%rdx, %%rbx), %s\n", reg);
                }
            } else {
                if (!addr) {
                    this->append("\tmovl\t%s(%%rip), %s\n", node->label, reg);
                } else {
                    this->append("\tleaq\t%s(%%rip), %s\n", node->label, reg);
                }
            }
        } else {
            if (node->isArr) {
                if (node->isAddr) {
                    this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                    this->append("\tcltq\n");
                    this->append("\tmovq\t%d(%%rbp), %%r10\n", node->offset);
                    if (!addr) {
                        this->append("\tmovl\t(%%r10, %%rbx, 4), %s\n", reg);
                    } else {                       
                        if(is_c)
                            this->append("\tleaq\t(%%r10, %%rbx, 1), %s\n", reg);
                        else
                            this->append("\tleaq\t(%%r10, %%rbx, 4), %s\n", reg);

                    }
                } else {
                    this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                    this->append("\tcltq\n");
                    if (!addr) {
                        this->append("\tmovl\t%d(%%rbp, %%rbx, 4), %s\n", node->offset, reg);
                    } else {
                        this->append("\tleaq\t%d(%%rbp, %%rbx, 4), %s\n", node->offset, reg);
                    }
                }
            } else {
                if (!addr) {
                    this->append("\tmovl\t%d(%%rbp), %s\n", node->offset, reg);
                } else {
                    this->append("\tleaq\t%d(%%rbp), %s\n", node->offset, reg);
                }
            }
        }

    }

    void reg2var(const char* reg, Node* node) {
        if (node->isGlobal) {  // 全局
            if (node->isArr) {
                this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                this->append("\tcltq\n");
                this->append("\tleaq\t0(, %%rbx, 4), %%rdx\n");
                this->append("\tleaq\t%s(%%rip), %%rbx\n", node->label);
                this->append("\tmovl\t%s, (%%rdx, %%rbx)\n", reg);
            } else {
                this->append("\tmovl\t%s, %s(%%rip)\n", reg, node->label);
            }
        } else {
            if (node->isArr) {
                if (node->isAddr) {
                    this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                    this->append("\tcltq\n");
                    this->append("\tmovq\t%d(%%rbp), %%r10\n", node->offset);
                    this->append("\tmovl\t%s, (%%r10, %%rbx, 4)\n", reg);
                } else {
                    this->append("\tmovl\t%d(%%rbp), %%ebx\n", node->offsetInArray);
                    this->append("\tcltq\n");
                    this->append("\tmovl\t%s, %d(%%rbp, %rbx, 4)\n", reg, node->offset);
                }
            } else {
                this->append("\tmovl\t%s, %d(%%rbp)\n", reg, node->offset);
            }
        }

    }

    void backpatch(const vector<int>& list, int label) {
        for (auto& i : list) {
            // if (buffer[i].back() == '\n') {
            //     buffer[i] = buffer[i].substr(0, buffer[i].size() - 1);
            // }
            buffer[i] += to_string(label) + "\n";
        }
    }

    int newLabel() {
        this->append(".L%d:\n", this->labelCount);
        return this->labelCount++;
    }

    void comment(const char* str) {
        this->append("\t# %s\n", str);
    }

    void comment(const string& str) {
        this->append("\t# %s\n", str.c_str());
    }
};

Assemble assemble;
int level = 0;  // 0: global
vector<map<string, Var>> varTable(1), funcTable(1);
vector<vector<pii>> breakStack(1), continueStack(1);
int offset;
vector<int> offsetStack(1);
char tmp[128];
int s_and_p_index = 0;
string funcName;
bool inVoidFunc = false;
bool hasReturn = false;
vector<Node*> paramList;
std::vector<int> arr_dim;       //数组赋值用来传递数组维度
int arr_level = 0;              //数组赋值用


template <typename T>
vector<T> merge(const vector<T>& a, const vector<T>& b) {
    vector<T> ret;
    for (auto& i : a) {
        ret.push_back(i);
    }
    for (auto& i : b) {
        ret.push_back(i);
    }
    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

void nextLevel() {
    level++;
    varTable.push_back(map<string, Var>());
    funcTable.push_back(map<string, Var>());
}

void exitLevel() {
    level--;
    varTable.pop_back();
    funcTable.pop_back();
}

void alignStack() {
    int padding = (16 - abs(offset) % 16) % 16;
    if (padding == 0)
        return;
    offset -= padding;
    assemble.append("\tsubq\t$%d, %%rsp\n", padding);
    assemble.comment("align stack");
}

void recoverStack() {
    if (offsetStack.empty())
        throw runtime_error("offsetStack is empty");
    int oldOffset = offsetStack.back();
    offsetStack.pop_back();
    assemble.append("\taddq\t$%d, %%rsp\n", oldOffset - offset);
    offset = oldOffset;
}

bool isVarInTable(const string& name) {
    for (int i = level; i >= 0; i--) {
        if (varTable[i].find(name) != varTable[i].end()) {
            return true;
        }
    }
    return false;
}

bool isFuncInTable(const string& name) {
    for (int i = level; i >= 0; i--) {
        if ( funcTable[i].find(name) != funcTable[i].end() ) {
            return true;
        }

    }
    return false;
}

void insertVar(const string& name, Var var) {
    varTable[level][name] = var;
}

void insertFunc(const string& name, Var var) {
    funcTable[level][name] = var;
}

using piv = pair<int, Var>;

piv getVar(const string& name) {
    for (int i = level; i >= 0; i--) {
        if (varTable[i].find(name) != varTable[i].end()) {
            return make_pair(i, varTable[i][name]);
        }
    }
    return make_pair(-1, Var());
}

piv getFunc(const string& name) {
    for (int i = level; i >= 0; i--) {
        if (funcTable[i].find(name) != funcTable[i].end()) {
            return make_pair(i, funcTable[i][name]);
        }
    }
    return make_pair(-1, Var());
}






%}

%union {
    char* val;
    int num;
    int defalt;
}

%start CompUnit
%token CONST INT VOID IF WHILE BREAK CONTINUE RETURN PRINTF SCANF
%token <val>Ident WrongIdent STRING 
%token <num>INTCONST
%token AND OR
%token ERROR

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%left '>' '<' EQ NEQ GEQ LEQ 
%left '+' '-'
%left '*' '/' '%'

%type <defalt>CompUnit OtherCompUnit 
%type <defalt>Decl 
%type <defalt>ConstDecl ConstDef OtherConstDef ConstInitVal OtherConstInitVal ConstArrIdex  
%type <defalt>VarDecl VarDef OtherVarDef InitVal OtherVarInitVal   
%type <defalt>FuncDef FuncFParams FuncFParam LExp 
%type <defalt>Block Blocks BlockItem 
%type <defalt>Stmt LVal Cond 
%type <defalt>Exp AddExp PrimaryExp UnaryExp FuncRParams MulExp RelExp EqExp LAndExp ConstExp LOrExp PointParams
%type <defalt> NewLabel AfterElse EnterWhile ExitWhile  EnterBlock ExitBlock
%type <defalt> FuncName EnterIntFuncBlock EnterVoidFuncBlock BlockWithoutNewLevel FuncEnd InsertIntFuncName EnterFuncBlock
//%type <defalt> //MLABEL


%%
CompUnit: OtherCompUnit{
    fprintf(fDetail, "CompUnits -> OtherCompUnit\n");
    $$ = newNode("CompUnit");
    addChild($$, $1);
    }
    | CompUnit OtherCompUnit{    
    fprintf(fDetail, "CompUnits -> CompUnit OtherCompUnit\n");
    $$ = newNode("CompUnit");
    addChild($$, $1);
    addChild($$, $2);
}   

OtherCompUnit : Decl{
    fprintf(fDetail, "OtherCompUnit -> Decl\n");
    $$ = newNode("OtherCompUnit");
    addChild($$, $1);
    }
    | FuncDef{
    fprintf(fDetail, "OtherCompUnit -> FuncDef\n");
    $$ = newNode("OtherCompUnit");
    addChild($$, $1);
}

Decl : ConstDecl{
    fprintf(fDetail, "Decl -> ConstDecl\n");
    $$ = newNode("Decl");
    addChild($$, $1);
    }
    | VarDecl{
    fprintf(fDetail, "Decl -> VarDecl\n");
    $$ = newNode("Decl");
    addChild($$, $1);
}

ConstDecl : CONST INT ConstDef ';'{
    fprintf(fDetail, "ConstDecl -> const int ConstDef ;\n");
    $$ = newNode("ConstDecl");
    addChild($$, newNode("const"));
    addChild($$, newNode("int"));
    addChild($$, $3);
    addChild($$, newNode(";"));
    }
    | CONST INT error ';'{
    fprintf(fDetail, "ConstDef -> CONST INT error ;\n");
    fprintf(fDetail, "const definition error\n");
    $$ = newNode("ConstDecl");
    addChild($$, newNode("const"));
    addChild($$, newNode("int"));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode(";"));
}

ConstDef : OtherConstDef{
    fprintf(fDetail, "ConstDef -> OtherConstDef\n");
    $$ = newNode("ConstDef");
    addChild($$, $1);
    }
    | ConstDef ',' OtherConstDef{
    fprintf(fDetail, "ConstDef -> ConstDef , OtherConstDef\n");
    $$ = newNode("ConstDef");
    addChild($$, $1);
    addChild($$, newNode(","));
    addChild($$, $3);
}

OtherConstDef : Ident '=' ConstInitVal{
    fprintf(fDetail, "OtherConstDef -> Ident = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode($1));
    addChild($$, newNode("="));
    addChild($$, $3);

        if(isVarInTable($1)){
            yyerror("Variable redefined");
            nodes[$$].isError = 1;
        }else{
            if(level == 0){ // 全局变量
                assemble.append("\t.section\t.rodata\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", $1);
                assemble.append("\t.size\t%s, 4\n", $1);
                assemble.append("%s:\n", $1);
                assemble.append("\t.long\t%d\n",nodes[$3].value);
                assemble.append("\t.text\n");
                insertVar($1, Var(VarType::ConstInt, nodes[$3].value, 0, true));
            }else{
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t$%d, %%edi\n", nodes[$3].value);
                assemble.append("\tmovl\t%%edi, %d(%rbp)\n", offset);
                insertVar($1, Var(VarType::ConstInt, nodes[$3].value, offset, false));
            }
        }
    }
    | Ident ConstArrIdex '=' ConstInitVal {
    fprintf(fDetail, "OtherConstDef -> Ident ConstArrIdex = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode($1));
    addChild($$, $2);
    addChild($$, newNode("="));
    addChild($$, $4);
        
        // printf("\n");
        // printf("List length is %zu\n",nodes[$4].values.size());
        // for(int i=0;i<nodes[$4].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$4].values[i]->value);
        // }
        // printf("\n");

        if(isVarInTable($1)){
            yyerror("Variable redefined");
            nodes[$$].isError = 1;
        }else{
            if(level == 0){ // 全局变量
                assemble.append("\t.section\t.rodata\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", $1);
                assemble.append("\t.size\t%s, %d\n", $1, nodes[$2].value*4);
                assemble.append("%s:\n", $1);
                for(auto x: nodes[$4].values){
                    assemble.append("\t.long\t%d\n", x->value);
                }
                for(int i = 0; i < nodes[$2].value - nodes[$4].values.size(); i++){
                    assemble.append("\t.long\t0\n");
                }
                assemble.append("\t.text\n");
                auto var = Var(ConstArr, nodes[$2].value, 0, true, nodes[$2].values);
                for(auto v: nodes[$4].values){
                    var.values2.push_back(v->value);
                }
                insertVar($1, var);
            }else{
                offset -= 4*nodes[$2].value;
                assemble.append("\tsubq\t$%d, %%rsp\n", 4*nodes[$2].value);
                for(int i = 0; i < nodes[$4].value; i++){
                    assemble.append("\tmovl\t$%d, %d(%%rbp)\n",nodes[$4].values[i]->value, offset + i*4);
                }
                auto var = Var(ConstArr, nodes[$2].value, offset, false, nodes[$2].values);
                for(auto v: nodes[$4].values){
                    var.values2.push_back(v->value);
                }
                insertVar($1, var);
            }
        }

        arr_dim.clear();
    }
    | WrongIdent '=' ConstInitVal{
    fprintf(fDetail, "OtherConstDef -> WrongIdent = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("="));
    addChild($$, $3);
    }
    | WrongIdent ConstArrIdex '=' ConstInitVal {
    fprintf(fDetail, "OtherConstDef -> WrongIdent ConstArrIdex = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    addChild($$, newNode("="));
    addChild($$, $4);
}


ConstArrIdex : '[' ConstExp ']'{
    fprintf(fDetail, "ConstArrIdex -> [ ConstExp ]\n");
    $$ = newNode("ConstArrIdex");
    addChild($$, newNode("["));
    addChild($$, $2);
    addChild($$, newNode("]"));

        nodes[$$].value = nodes[$2].value;
        nodes[$$].values.push_back(&nodes[$2]);
        arr_dim.push_back(nodes[$2].value);
    }
    | ConstArrIdex '[' ConstExp ']'{
    fprintf(fDetail, "ConstArrIdex -> ConstArrIdex [ ConstExp ]\n");
    $$ = newNode("ConstArrIdex");
    addChild($$, $1);
    addChild($$, newNode("["));
    addChild($$, $3);
    addChild($$, newNode("]"));

        nodes[$$].value = nodes[$1].value * nodes[$3].value;
        nodes[$$].values.push_back(&nodes[$3]);
        nodes[$$].values.insert(nodes[$$].values.end(), nodes[$1].values.begin(), nodes[$1].values.end());
        arr_dim.push_back(nodes[$3].value);
}

ConstInitVal : ConstExp{
    fprintf(fDetail, "ConstInitVal -> ConstExp\n");
    $$ = newNode("ConstInitVal");
    addChild($$, $1);
        copyNode(&nodes[$$], &nodes[$1]);
    }
    | LEFTB '}'{
    fprintf(fDetail, "ConstInitVal -> { }\n");
    $$ = newNode("ConstInitVal");
    addChild($$, newNode("{"));
    addChild($$, newNode("}"));
        nodes[$$].values = vector<Node*>();
        nodes[$$].value = 0;
        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];


        while(of--){
            Node* nNode = new Node();
            nNode->value = 0;
            nNode->isConst = true;
            nodes[$$].values.push_back(nNode);
        }
        nodes[$$].value = nodes[$$].values.size();

        // printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

        arr_level--;        
        

    } 
    | LEFTB OtherConstInitVal '}'{
    fprintf(fDetail, "ConstInitVal -> { OtherConstInitVal }\n");
    $$ = newNode("ConstInitVal");
    addChild($$, newNode("{"));
    addChild($$, $2);
    addChild($$, newNode("}"));
        nodes[$$].values = nodes[$2].values;
        nodes[$$].value = nodes[$2].values.size();

        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];
        arr_level--;

        if(nodes[$$].value == of){
            break;
        }
        else if(of > nodes[$$].value){
            of -= nodes[$$].value;
            while(of--){
                Node* nNode = new Node();
                copyNode(nNode, nodes[$2].values[0]);
                nNode->value = 0;
                nNode->isConst = true;
                nodes[$$].values.push_back(nNode);
            }
        }
        else{
            yyerror("Wrong arrList");
        }
        nodes[$$].value = nodes[$$].values.size();
        //     printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

    }

LEFTB:'{'{
    arr_level++;
}



OtherConstInitVal : ConstInitVal{
    fprintf(fDetail, "OtherConstInitVal -> ConstInitVal\n");
    $$ = newNode("OtherConstInitVal");
    addChild($$, $1);
        if(nodes[$1].values.size() == 0)
            nodes[$$].values.push_back(nodes+$1);
        else
            nodes[$$].values = nodes[$1].values;
    }
    | OtherConstInitVal ',' ConstInitVal{
    fprintf(fDetail, "OtherConstInitVal -> OtherConstInitVal , ConstInitVal\n");
    $$ = newNode("OtherConstInitVal");
    addChild($$, $1);
    addChild($$, newNode(","));
    addChild($$, $3);

        nodes[$$].values = nodes[$1].values;
        if(nodes[$3].values.size() == 0)
            nodes[$$].values.push_back(nodes+$3);
        else
            nodes[$$].values.insert(nodes[$$].values.end(), nodes[$3].values.begin(), nodes[$3].values.end());
    }





VarDecl : INT VarDef ';'{
    fprintf(fDetail, "VarDecl -> int VarDef ;\n");
    $$ = newNode("VarDecl");
    addChild($$, newNode("int"));
    addChild($$, $2);
    addChild($$, newNode(";"));

        // no need to do anything
}

VarDef : OtherVarDef{
    fprintf(fDetail, "VarDef -> OtherVarDef\n");
    $$ = newNode("VarDef");
    addChild($$, $1);

        // no need to do anything
    }
    | VarDef ',' OtherVarDef{
    fprintf(fDetail, "VarDef -> VarDef , OtherVarDef\n");
    $$ = newNode("VarDef");
    addChild($$, $1);
    addChild($$, newNode(","));
    addChild($$, $3);

        // no need to do anything
}

OtherVarDef : Ident{
    fprintf(fDetail, "OtherVarDef -> Ident\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));

        if(level == 0){
            assemble.append("\t.globl\t%s\n", $1);
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", $1);
            assemble.append("\t.size\t%s, 4\n", $1);
            assemble.append("\t.long\t0\n");
            assemble.append("\t.text\n");
            insertVar($1, Var(Int, 0, 0, true));
        }else{
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            insertVar($1, Var(Int, 0, offset, false));
        }
    }
    | Ident ConstArrIdex{
    fprintf(fDetail, "OtherVarDef -> Ident  ConstArrIdex\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, $2);
        arr_dim.clear();
        if(level==0){
            assemble.append("\t.globl\t%s\n", $1);
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", $1);
            assemble.append("\t.size\t%s, %d\n", $1, nodes[$2].value*4);
            assemble.append("%s:\n", $1);
            for(int i=0; i<nodes[$2].value; i++){
                assemble.append("\t.long\t0\n");
            }
            assemble.append("\t.text\n");
            insertVar($1, Var(Arr, 0, 0, true, nodes[$2].values));
        }else{
            offset -= nodes[$2].value*4;
            assemble.append("\tsubq\t$%d, %%rsp\n", nodes[$2].value*4);
            insertVar($1, Var(Arr, 0, offset, false, nodes[$2].values));
        }
    }
    | Ident '=' ConstInitVal{
    fprintf(fDetail, "OtherVarDef -> Ident = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, newNode("="));
    addChild($$, $3);

        if(level == 0){
            assemble.append("\t.globl\t%s\n", $1);
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", $1);
            assemble.append("\t.size\t%s, 4\n", $1);
            assemble.append("%s:\n", $1);
            assemble.append("\t.long\t%d\n", nodes[$3].value);
            assemble.append("\t.text\n");
            insertVar($1, Var(Int, nodes[$3].value, 0, true));
        }else{
            assemble.var2reg(&nodes[$3], "%edi");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%edi, %d(%%rbp)\n", offset);
            insertVar($1, Var(Int, nodes[$3].value, offset, false));
        }

        arr_dim.clear();
    }
    | Ident ConstArrIdex '=' InitVal {
    
    arr_dim.clear();
    fprintf(fDetail, "OtherVarDef -> Ident ConstArrIdex = InitValList\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, $2);
    addChild($$, newNode("="));
    addChild($$, $4);
        nodes[$4].value = nodes[$4].values.size();
        // printf("\n ConstArrIdex is %d",nodes[$2].value);
        // printf("\n InitVal is %d",nodes[$4].value);
        // for(int i=0;i<nodes[$4].value;i++){
        //     printf("\n InitVal %d is %d",i,nodes[$4].values[i]->value);
        // }
        if(nodes[$4].value > nodes[$2].value){
            yyerror("Initialize array with too many values");
        }else{
            if(level == 0){
                assemble.append("\t.globl\t%s\n", $1);
                assemble.append("\t.data\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", $1);
                assemble.append("\t.size\t%s, %d\n", $1, nodes[$2].value*4);
                assemble.append("%s:\n", $1);
                for(int i = 0; i < nodes[$4].value; i++){
                    assemble.append("\t.long\t%d\n", nodes[$4].values[i]->value);
                }
                for(int i = nodes[$4].value; i < nodes[$2].value; i++){
                    assemble.append("\t.long\t0\n");
                }
                assemble.append("\t.text\n");
                insertVar($1, Var(Arr, 0, 0, true, nodes[$2].values));
            } else{
                offset -= nodes[$2].value*4;
                assemble.append("\tsubq\t$%d, %%rsp\n", nodes[$2].value*4);
                for(int i = 0; i < nodes[$4].value; i++){
                    assemble.var2reg(nodes[$4].values[i], "%edi");
                    assemble.append("\tmovl\t%%edi, %d(%%rbp)\n", offset+i*4);
                }
                for(int i = nodes[$4].value; i < nodes[$2].value; i++){
                    assemble.append("\tmovl\t$0, %d(%%rbp)\n", offset+i*4);
                }
                insertVar($1, Var(Arr, 0, offset, false, nodes[$2].values));
            }
        }

        
    }
    | WrongIdent{
    fprintf(fDetail, "OtherVarDef -> WrongIdent\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    }
    | WrongIdent ConstArrIdex{
    fprintf(fDetail, "OtherVarDef -> WrongIdent  ConstArrIdex\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    arr_dim.clear();
    }
    | WrongIdent '=' InitVal{
    fprintf(fDetail, "OtherVarDef -> WrongIdent = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("="));
    addChild($$, $3);
    }
    | WrongIdent ConstArrIdex '=' InitVal {
    fprintf(fDetail, "OtherVarDef -> WrongIdent ConstArrIdex = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    addChild($$, newNode("="));
    addChild($$, $4);
    arr_dim.clear();
}


InitVal : Exp{
    fprintf(fDetail, "InitVal -> Exp\n");
    $$ = newNode("InitVal");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
    }
    | LEFTB '}'{
    fprintf(fDetail, "InitValList -> { }\n");
    $$ = newNode("InitVal");
    addChild($$, newNode("{"));
    addChild($$, newNode("}"));

        nodes[$$].values = vector<Node*>();
        nodes[$$].value = 0;
        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];

        // printf("\n");
        // printf("of is %d\n",of);
        // printf("\n");

        while(of--){
            Node* nNode = new Node();
            nNode->value = 0;
            nNode->isConst = true;
            nodes[$$].values.push_back(nNode);
        }
        nodes[$$].value = nodes[$$].values.size();

        // printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

        arr_level--;  
    } 
    | LEFTB OtherVarInitVal '}'{
    fprintf(fDetail, "InitValList -> { OtherVarInitVal }\n");
    $$ = newNode("InitVal");
    addChild($$, newNode("{"));
    addChild($$, $2);
    addChild($$, newNode("}"));

        nodes[$$].values = nodes[$2].values;
        nodes[$$].value = nodes[$2].values.size();
        
        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];
        arr_level--;

        // printf("\n");
        // printf("of is %d\n",of);
        // printf("\n");
        
        if(nodes[$$].value == of){
            break;
        }
        else if(of > nodes[$$].value){
            of -= nodes[$$].value;
            while(of--){
                Node* nNode = new Node();
                copyNode(nNode, nodes[$2].values[0]);
                nNode->value = 0;
                nNode->isConst = true;
                nodes[$$].values.push_back(nNode);
            }
        }
        else{
            yyerror("Wrong arrList");
        }
        nodes[$$].value = nodes[$$].values.size();
        //     printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");
}

OtherVarInitVal : InitVal{
    fprintf(fDetail, "OtherVarInitVal -> InitVal\n");
    $$ = newNode("OtherVarInitVal");
    addChild($$, $1);

        if(nodes[$1].values.size() == 0)
            nodes[$$].values.push_back(nodes+$1);
        else
            nodes[$$].values = nodes[$1].values;
        nodes[$$].value = nodes[$$].values.size();
    }
    | OtherVarInitVal ',' InitVal{
    fprintf(fDetail, "OtherVarInitVal -> OtherVarInitVal , InitVal\n");
    $$ = newNode("OtherVarInitVal");
    addChild($$, $1);
    addChild($$, newNode(","));
    addChild($$, $3);

        nodes[$$].values = nodes[$1].values;
        if(nodes[$3].values.size() == 0)
            nodes[$$].values.push_back(nodes+$3);
        else
            nodes[$$].values.insert(nodes[$$].values.end(), nodes[$3].values.begin(), nodes[$3].values.end());
        nodes[$$].value = nodes[$$].values.size();        
}

FuncDef : VOID FuncName '(' ')'  EnterVoidFuncBlock BlockWithoutNewLevel FuncEnd{
    fprintf(fDetail, "FuncDef -> void Ident ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, $2);
    addChild($$, newNode("("));
    addChild($$, newNode(")"));
    addChild($$, $6);
        inVoidFunc = false;
        // no need to do anything
    }
    | INT FuncName '(' ')'  EnterIntFuncBlock BlockWithoutNewLevel FuncEnd{
    fprintf(fDetail, "FuncDef -> int Ident ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, $2);
    addChild($$, newNode("("));
    addChild($$, newNode(")"));
    addChild($$, $6);

        // no need to do anything
    } 
    | VOID FuncName '(' FuncFParams ')'  EnterVoidFuncBlock BlockWithoutNewLevel FuncEnd{
    fprintf(fDetail, "FuncDef -> void Ident ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, $2);
    addChild($$, newNode("("));
    addChild($$, $4);
    addChild($$, newNode(")"));
    addChild($$, $7);
        inVoidFunc = false;
        // no need to do anything
    }
    | INT FuncName '(' FuncFParams ')'  EnterIntFuncBlock BlockWithoutNewLevel FuncEnd{
    fprintf(fDetail, "FuncDef -> int Ident ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, $2);
    addChild($$, newNode("("));
    addChild($$, $4);
    addChild($$, newNode(")"));
    addChild($$, $7);

        // no need to do anything
    }


FuncName:
    Ident{
        fprintf(fDetail, "FuncName -> Ident\n");
        $$ = newNode("FuncName");
        addChild($$, newNode($1));
        funcName = $1;
    }
;

BlockWithoutNewLevel:
    '{' Blocks '}'{
    fprintf(fDetail, "BlockWithoutNewLevel -> { Blocks }\n");
    $$ = newNode("BlockWithoutNewLevel");
    addChild($$, newNode("{"));
    addChild($$, $2);
    addChild($$, newNode("}"));

        // no need to do anything
    }
    | '{' '}'{
    fprintf(fDetail, "BlockWithoutNewLevel -> { }\n");
    $$ = newNode("BlockWithoutNewLevel");
    addChild($$, newNode("{"));
    addChild($$, newNode("}"));

        // no need to do anything
    }
;

EnterIntFuncBlock:
    InsertIntFuncName EnterFuncBlock {
        fprintf(fDetail, "EnterIntFuncBlock -> InsertIntFuncName EnterFuncBlock\n");
    }
;

InsertIntFuncName:
    {
        fprintf(fDetail, "InsertIntFuncName\n");

        if(isFuncInTable(funcName)){
            yyerror("duplicated function name");
        }else{
            insertFunc(funcName, Var(FuncInt, 0, 0, false, paramList));
        }
        
    }
;

EnterVoidFuncBlock:
    InsertVoidFuncName EnterFuncBlock {
    }
;

InsertVoidFuncName:
    {
        fprintf(fDetail, "InsertVoidFuncName\n");
        if(isFuncInTable(funcName)){
            yyerror("duplicated function name");
        }else{
            insertFunc(funcName, Var(FuncVoid, 0, 0, false, paramList));
            inVoidFunc = true;
        }
    }
;

EnterFuncBlock:
    /* empty */ {
        fprintf(fDetail, "EnterFuncBlock\n");
        nextLevel();
        hasReturn = false;
        assemble.append("\t.globl\t%s\n", funcName.c_str());
        assemble.append("\t.type\tmain, @function\n");
        assemble.append("%s:\n", funcName.c_str());
        assemble.call();
        int curSize = 0;
        int allgi = 0;

        for(int i=0;i<paramList.size();i++){
            if(paramList[i]->isArr){
                allgi += 8;
            }else{
                allgi += 4;
            }
        }
        allgi = (16 -(allgi%16))%16;
        // printf("\nallgi is %d\n",allgi);
        for(int i=0;i<paramList.size();i++){
            if(paramList[i]->isArr){
                insertVar(paramList[i]->label, Var(Addr, 0, 32+curSize+allgi, false, paramList[i]->values));
                curSize += 8;
            }else{
                insertVar(paramList[i]->label, Var(Int, 0, 32+curSize+allgi, false));
                curSize += 4;
            }
        }
        paramList.clear();
    }
;

FuncEnd:
    {
        exitLevel();
        if(!hasReturn){
            assemble.append("\taddq\t$%d, %%rsp\n", -offset);
            assemble.ret();
        }
        offset = 0;
    }
;

FuncFParams: FuncFParam{
    fprintf(fDetail, "FuncFParams -> FuncFParam\n");
    $$ = newNode("FuncFParams");
    addChild($$, $1);

        // no need to do anything
    }
    | FuncFParams ',' FuncFParam{
    fprintf(fDetail, "FuncFParams -> FuncFParams , FuncFParam\n");
    $$ = newNode("FuncFParams");
    addChild($$, $1); 
    addChild($$, newNode(","));
    addChild($$, $3);

        // no need to do anything
}

FuncFParam : INT Ident{
    fprintf(fDetail, "FuncFParam -> int Ident\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));

        strcpy(nodes[$$].label, $2);
        paramList.push_back(&nodes[$$]);
    }
    | INT Ident '[' ']'{
    fprintf(fDetail, "FuncFParam -> int Ident [ ]\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("["));
    addChild($$, newNode("]"));

        strcpy(nodes[$$].label, $2);
        nodes[$$].values.push_back(nullptr);
        nodes[$$].isArr = true;
        paramList.push_back(&nodes[$$]);
    }
    | INT Ident '[' ']' LExp{
    fprintf(fDetail, "FuncFParam -> int Ident [ ] LExp\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("["));
    addChild($$, newNode("]"));
    addChild($$, $5);

        strcpy(nodes[$$].label, $2);
        for(auto dim: nodes[$5].values){
            nodes[$$].values.push_back(dim);
        }
        nodes[$$].values.push_back(nullptr);
        nodes[$$].isArr = true;
        paramList.push_back(&nodes[$$]);
    }
    | INT WrongIdent{
    fprintf(fDetail, "FuncFParam -> int WrongIdent\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    }
    | INT WrongIdent '[' ']'{
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ]\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("["));
    addChild($$, newNode("]"));
    }
    | INT WrongIdent '[' ']' LExp{
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ] LExp\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("["));
    addChild($$, newNode("]"));
    addChild($$, $5);
}

LExp : '[' Exp ']'{
    fprintf(fDetail, "LExp -> [ Exp ]\n");
    $$ = newNode("LExp");
    addChild($$, newNode("["));
    addChild($$, $2);
    addChild($$, newNode("]"));

        nodes[$$].value = nodes[$2].value;
        nodes[$$].isConst = nodes[$2].isConst;
        nodes[$$].values.push_back(&nodes[$2]);
    }
    | LExp '[' Exp ']'{
    fprintf(fDetail, "LExp -> LExp [ Exp ]\n");
    $$ = newNode("LExp");
    addChild($$, $1);
    addChild($$, newNode("["));
    addChild($$, $3);
    addChild($$, newNode("]"));

        nodes[$$].value = nodes[$1].value * nodes[$3].value;
        nodes[$$].isConst = nodes[$1].isConst && nodes[$3].isConst;
        nodes[$$].values.push_back(&nodes[$3]);
        nodes[$$].values.insert(nodes[$$].values.end(), nodes[$1].values.begin(), nodes[$1].values.end());

}



Block : EnterBlock '{' '}' ExitBlock{
    fprintf(fDetail, "Block -> { }\n");
    $$ = newNode("Block");
    addChild($$, newNode("{"));
    addChild($$, newNode("}"));

        // no need to do anything
    }
    | EnterBlock '{' Blocks '}' ExitBlock{
    fprintf(fDetail, "Block -> { Blocks }\n");
    $$ = newNode("Block");
    addChild($$, newNode("{"));
    addChild($$, $3);
    addChild($$, newNode("}"));

        // no need to do anything
}

EnterBlock :
    /* empty */ {
        nextLevel();
        offsetStack.push_back(offset);
    }

ExitBlock :
    /* empty */ {
        exitLevel();
        recoverStack();
    }

Blocks : BlockItem{
    fprintf(fDetail, "Blocks -> BlockItem\n");
    $$ = newNode("Blocks");
    addChild($$, $1);
    }
    | Blocks BlockItem{
    fprintf(fDetail, "Blocks -> Blocks BlockItem\n");
    $$ = newNode("Blocks");
    addChild($$, $1);
    addChild($$, $2);
}

BlockItem : Decl{
    fprintf(fDetail, "BlockItem -> Decl\n");
    $$ = newNode("BlockItem");
    addChild($$, $1);
    }
    | Stmt{
    fprintf(fDetail, "BlockItem -> Stmt\n");
    $$ = newNode("BlockItem");
    addChild($$, $1);
}

Stmt : LVal '=' Exp ';'{
    fprintf(fDetail, "Stmt -> LVal = Exp ;\n");
    $$ = newNode("Stmt");
    addChild($$, $1);
    addChild($$, newNode("="));
    addChild($$, $3);
    addChild($$, newNode(";"));

        if(nodes[$1].isConst){
            yyerror("assign to const");
        }else{
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.reg2var("%r9d", &nodes[$1]);
        }
    }
    | Exp ';'{
    fprintf(fDetail, "Stmt -> Exp ;\n");
    $$ = newNode("Stmt");
    addChild($$, $1);
    addChild($$, newNode(";"));
    }
    | ';'{
    fprintf(fDetail, "Stmt -> ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode(";"));
    }
    | Block{
    fprintf(fDetail, "Stmt -> Block\n");
    $$ = newNode("Stmt");
    addChild($$, $1);
    }
    | IF '(' Cond ')' NewLabel EnterStmt Stmt ExitStmt %prec LOWER_THAN_ELSE{
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("if"));
    addChild($$, newNode("("));
    addChild($$, $3);
    addChild($$, newNode(")"));
    addChild($$, $7);

        assemble.backpatch(nodes[$3].trueList, nodes[$5].quad);
        int end = assemble.newLabel();
        assemble.comment("if end");
        assemble.backpatch(nodes[$3].falseList, end);
    }
    | IF '(' Cond ')' NewLabel EnterStmt Stmt ExitStmt ELSE AfterElse NewLabel EnterStmt Stmt ExitStmt NewLabel{
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt else Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("if"));
    addChild($$, newNode("("));
    addChild($$, $3);
    addChild($$, newNode(")"));
    addChild($$, $7);
    addChild($$, newNode("else"));
    addChild($$, $13);

        assemble.backpatch(nodes[$3].trueList, nodes[$5].quad);
        assemble.backpatch(nodes[$3].falseList, nodes[$11].quad);
        assemble.backpatch(nodes[$10].trueList, nodes[$15].quad);
    }
    | WHILE EnterWhile EnterStmt '(' Cond ')' ExitWhile NewLabel Stmt ExitStmt{
    fprintf(fDetail, "Stmt -> while ( Cond ) Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("while"));
    addChild($$, newNode("("));
    addChild($$, $5);
    addChild($$, newNode(")"));
    addChild($$, $9);

        assemble.backpatch(nodes[$5].trueList, nodes[$8].quad);
        assemble.append("\tjmp\t.L%d\n", nodes[$2].quad);
        int whileEnd = assemble.newLabel();
        assemble.comment("while end");
        assemble.backpatch(nodes[$5].falseList, nodes[$7].quad);
        assemble.backpatch(nodes[$7].trueList, whileEnd);
        for(auto [line, of]: breakStack.back()){
            sprintf(tmp, "\taddq\t$%d, %%rsp\n", offset - of);
            assemble[line - 1] = tmp; 
            assemble[line] += to_string(whileEnd) + "\n";
        }
        breakStack.pop_back();
        for(auto [line, of]: continueStack.back()){
            sprintf(tmp, "\taddq\t$%d, %%rsp\n", offset - of);
            assemble[line - 1] = tmp; 
            assemble[line] += to_string(nodes[$2].quad) + "\n";
        }
        continueStack.pop_back();
    }
    | BREAK ';'{
    fprintf(fDetail, "Stmt -> break ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("break"));
    addChild($$, newNode(";"));

        assemble.append("");
        assemble.append("\tjmp\t.L");
        breakStack.back().push_back({assemble.line, offset});
    }
    | CONTINUE ';'{
    fprintf(fDetail, "Stmt -> continue ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("continue"));
    addChild($$, newNode(";"));

        assemble.append("");
        assemble.append("\tjmp\t.L");
        continueStack.back().push_back({assemble.line, offset});
    }
    | RETURN Exp ';'{
    fprintf(fDetail, "Stmt -> return Exp ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("return"));
    addChild($$, $2);
    addChild($$, newNode(";"));

       if(inVoidFunc){
            yyerror("return in void function");
        }
        hasReturn = true;
        assemble.var2reg(&nodes[$2], "%eax");
        assemble.append("\taddq\t$%d, %%rsp\n", -offset);
        assemble.ret();
    }
    | RETURN ';'{
    fprintf(fDetail, "Stmt -> return ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("return"));
    addChild($$, newNode(";"));

        if(!inVoidFunc){
            yyerror("return nothing in non-void function");
        }
        hasReturn = true;
        assemble.append("\taddq\t$%d, %%rsp\n", -offset);
        assemble.ret();
}

EnterWhile:
    /* empty */ {
        $$ = newNode("EnterWhile");
        nodes[$$].quad = assemble.newLabel();
        assemble.comment("enter while");
        breakStack.push_back(vector<pii>());
        continueStack.push_back(vector<pii>());
    }

ExitWhile:
    /* empty */ {
        $$ = newNode("ExitWhile");
        nodes[$$].quad = assemble.newLabel();
        assemble.comment("exit while");
        int oldOffset = offsetStack.back();
        assemble.append("\taddq\t$%d, %%rsp\n", oldOffset - offset);
        assemble.append("\tjmp\t.L");
        nodes[$$].trueList.push_back(assemble.line);
    }

AfterElse:
    /* empty */ {
        $$ = newNode("AfterElse");
        assemble.append("\tjmp\t.L");
        nodes[$$].trueList.push_back(assemble.line);
    }

NewLabel:
    /* empty */ {
        $$ = newNode("NewLabel");
        nodes[$$].quad = assemble.newLabel();
    }
;

EnterStmt:
    /* empty */ {
        assemble.comment("enter stmt");
        offsetStack.push_back(offset);
    }
;

ExitStmt:
    /* empty */ {
        recoverStack();
    }



Exp : AddExp{
    fprintf(fDetail, "Exp -> AddExp\n");
    $$ = newNode("Exp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
}

Cond : LOrExp{
    fprintf(fDetail, "Cond -> LOrExp\n");
    $$ = newNode("Cond");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
}

LVal : Ident{
    fprintf(fDetail, "LVal -> Ident\n");
    $$ = newNode("LVal");
    addChild($$, newNode($1));

        strcpy(nodes[$$].label, $1);
        if(!isVarInTable($1)){
            yyerror("Reference Undefined Variable");
        }else{
            auto [depth, var] = getVar($1);
            nodes[$$].isGlobal = var.isGlobal;
            if(var.type == ConstInt){
                nodes[$$].isConst = true;
                nodes[$$].value = var.value;
            }else{
                nodes[$$].offset = var.offset;
            }
        }
    }
    | Ident LExp{
    fprintf(fDetail, "LVal -> Ident LExp\n");
    $$ = newNode("LVal");
    addChild($$, newNode($1));
    addChild($$, $2);

        // printf("\n ex leng is  %d",nodes[$2].values.size());
        // for(int i=0;i<nodes[$2].values.size();i++)
        //     printf("\n ex is  %d",nodes[$2].values[i]->value);

        strcpy(nodes[$$].label, $1);
        if(!isVarInTable($1)){
            yyerror("Reference Undefined Variable");
        }else{
            auto [depth, var] = getVar($1);
            if(var.type != Arr && var.type != ConstArr && var.type != Addr){
                yyerror("Reference Non-Array Variable");
            }
            nodes[$$].isGlobal = var.isGlobal;
            if(var.type == ConstArr && nodes[$2].isConst){
                nodes[$$].isConst = true;
                int curSize = 1;
                int of = 0;
                int delta = var.values.size() - nodes[$2].values.size();
                for(int i = 0; i < var.values.size(); i++){
                    if(i >= delta){
                        of += curSize * nodes[$2].values[i - delta]->value;
                    }
                    curSize *= var.values[i]->value;
                }
                nodes[$$].value = getVarValue(var, of);
            } else {
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t$0, %d(%%rbp)\n", offset);
                int curSize = 1;
                int delta = var.values.size() - nodes[$2].values.size();
                for(int i = 0; i < var.values.size(); i++){
                    if(i >= delta){
                        assemble.var2reg(nodes[$2].values[i - delta], "%r8d");
                        assemble.append("\timull\t$%d, %%r8d\n", curSize);
                        assemble.append("\taddl\t%d(%%rbp), %%r8d\n", offset);
                        assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
                    }
                    if(var.values[i]){
                        curSize *= var.values[i]->value;
                    }
                }
                nodes[$$].isArr = true;
                nodes[$$].offset = var.offset;
                nodes[$$].isAddr = var.type == Addr;
                nodes[$$].offsetInArray = offset;
            }
        }
    }
    | WrongIdent{
    fprintf(fDetail, "LVal -> WrongIdent\n");
    $$ = newNode("LVal");
    addChild($$, newNode("WrongIdent"));
    }
    | WrongIdent LExp{
    fprintf(fDetail, "LVal -> WrongIdent LExp\n");
    $$ = newNode("LVal");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
}

PrimaryExp : '(' Exp ')'{
    fprintf(fDetail, "PrimaryExp -> ( Exp )\n");
    $$ = newNode("PrimaryExp");
    addChild($$, newNode("("));
    addChild($$, $2);
    addChild($$, newNode(")"));

        copyNode(&nodes[$$], &nodes[$2]);
    }
    | LVal{
    fprintf(fDetail, "PrimaryExp -> LVal\n");
    $$ = newNode("PrimaryExp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
    }
    | INTCONST{
    fprintf(fDetail, "PrimaryExp -> IntConst\n");
    $$ = newNode("PrimaryExp");
    char* buffer = new char[20];
    if (buffer) {
        sprintf(buffer, "%d", $1);
    }
    addChild($$, newNode(buffer)); 
    delete[] buffer;  

        nodes[$$].isConst = true;
        nodes[$$].value = $1;              
}

UnaryExp : PrimaryExp{
    fprintf(fDetail, "UnaryExp -> PrimaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
    }
| Ident '(' ')'{
    fprintf(fDetail, "UnaryExp -> Ident ( )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode($1));
    addChild($$, newNode("("));
    addChild($$, newNode(")"));

        if(!isFuncInTable($1)){
            yyerror("Call Undefined Function");
        }{
            alignStack();
            assemble.append("\tcall\t%s\n", $1);
            auto [depth, func] = getFunc($1);
            if(func.type == FuncInt){
                offset -= 4;
                assemble.append("\taddq\t$4, %%rsp\n");
                assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
                nodes[$$].offset = offset;
            }
        }
    }
| Ident '(' FuncRParams ')'{
    fprintf(fDetail, "UnaryExp -> Ident ( FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode($1));
    addChild($$, newNode("("));
    addChild($$, $3);
    addChild($$, newNode(")"));

        if(!isFuncInTable($1)){
            yyerror("Call Undefined Function");
        }{
            alignStack();
            //funcName
            auto [depth, func] = getFunc($1);
            auto [depth1, func1] = getFunc(funcName);
            
            for(int i = nodes[$3].value - 1; i >= 0 ; i--){
                bool flag = false;
                for(auto cur:func1.values){
                    if(!strcmp(cur->label, nodes[$3].values[i]->label)){
                        flag = true;
                        break;
                    }
                }
                
                std::string tmpstr = "main";
                if(funcName == tmpstr)
                    flag = false;
                // printf("\nname is %s and is exit %d\n",nodes[$3].values[i]->label,flag);


                if(func.values[i]->isArr && !flag){
                    assemble.var2reg(nodes[$3].values[i], "%r8", true);
                    assemble.append("\tsubq\t$8, %%rsp\n");
                    offset -= 8;
                    assemble.append("\tmovq\t%%r8, %d(%%rbp)\n", offset);

                }else if(func.values[i]->isArr &&flag){
                    assemble.append("\tmovq\t%d(%%rbp), %%r8\n", nodes[$3].values[i]->offset);
                    assemble.append("\tsubq\t$8, %%rsp\n");
                    offset -= 8;
                    assemble.append("\tmovq\t%%r8, %d(%%rbp)\n", offset);
                }else{
                    assemble.var2reg(nodes[$3].values[i], "%r8d");
                    assemble.append("\tsubq\t$4, %%rsp\n");
                    offset -= 4;
                    assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
                }
            }
            alignStack();
            assemble.append("\tcall\t%s\n", $1);
            if(func.type == FuncInt){
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
                nodes[$$].offset = offset;
            }
        }
        
    }
| SCANF'(' STRING ',' PointParams ')'{
    fprintf(fDetail, "UnaryExp -> sacnf ( \"string\" , FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("scanf"));
    addChild($$, newNode("("));
    addChild($$, newNode($3));
    addChild($$, newNode(","));
    addChild($$, $5);
    addChild($$, newNode(")"));

        alignStack();
		fprintf(asm_file, ".LC%d:\n", s_and_p_index);
		fprintf(asm_file, "\t.string\t%s\n", $3);
        assemble.var2reg(nodes[$5].values[0], "%rsi", true);
        assemble.append("\tleaq\t.LC%d(%%rip), %%rdi\n",s_and_p_index);
        assemble.append("\tmovl\t$0, %%eax\n");
        assemble.append("\tcall\t__isoc99_scanf@PLT\n");
        s_and_p_index++;
    }
| PRINTF'(' STRING ',' FuncRParams ')'{
    fprintf(fDetail, "UnaryExp -> sacnf ( \"string\" , FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("printf"));
    addChild($$, newNode("("));
    addChild($$, newNode($3));
    addChild($$, newNode(","));
    addChild($$, $5);
    addChild($$, newNode(")"));
            alignStack();
    		fprintf(asm_file, ".LC%d:\n", s_and_p_index);
		    fprintf(asm_file, "\t.string\t%s\n", $3);
            assemble.var2reg(nodes[$5].values[0], "%esi");
            if(nodes[$5].values.size()>1)
                assemble.var2reg(nodes[$5].values[1], "%ecx");
            assemble.append("\tleaq\t.LC%d(%%rip), %%rdi\n",s_and_p_index);
            assemble.append("\tmovl\t$0, %%eax\n");
            assemble.append("\tcall\tprintf@PLT\n");
            s_and_p_index++;
    }
| WrongIdent '(' FuncRParams ')'{
    fprintf(fDetail, "UnaryExp -> WrongIdent ( FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("("));
    addChild($$, $3);
    addChild($$, newNode(")"));
    }
    | '+' UnaryExp{
    fprintf(fDetail, "UnaryExp -> + UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("+"));
    addChild($$, $2);

        copyNode(&nodes[$$],&nodes[$2]);
    }
    | '-' UnaryExp{
    fprintf(fDetail, "UnaryExp -> - UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("-"));
    addChild($$, $2);

        if(nodes[$2].isConst){
            // copyNode($$, $2);
            nodes[$$].isConst = true;
            nodes[$$].value = -nodes[$2].value;
        }else{
            assemble.var2reg(&nodes[$2], "%r8d");
            assemble.append("\tneg %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq $4, %%rsp\n");
            assemble.append("\tmovl %%r8d, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }
    | '!' UnaryExp{
    fprintf(fDetail, "UnaryExp -> ! UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("!"));
    addChild($$, $2);

        if(nodes[$2].isConst){
            // copyNode($$, $2);
            nodes[$$].isConst = true;
            nodes[$$].value = !nodes[$2].value;
        }else{
            assemble.var2reg(&nodes[$2], "%eax");
            assemble.append("\ttestl %%eax, %%eax\n");
            assemble.append("\tsete %%al\n");
            assemble.append("\tmovzbl %%al, %%eax\n");
            offset -= 4;
            assemble.append("\tsubq $4, %%rsp\n");
            assemble.append("\tmovl %%eax, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
}


PointParams : '&' Exp{
    fprintf(fDetail, "PointParams -> & Exp\n");
    $$ = newNode("PointParams");
    addChild($$, newNode("&"));
    addChild($$, $2);

        nodes[$$].values = vector<Node*>();
        nodes[$$].values.push_back(&nodes[$2]);
        nodes[$$].value = 1;
    }
    | PointParams ',' '&' Exp{
    fprintf(fDetail, "PointParams -> PointParams , & Exp\n");
    $$ = newNode("PointParams");
    addChild($$, $1);
    addChild($$, newNode(","));
    addChild($$, newNode("&"));
    addChild($$, $4);

        nodes[$$].values = nodes[$1].values;
        nodes[$$].values.push_back(&nodes[$4]);
        nodes[$$].value = nodes[$$].values.size();
}



FuncRParams : Exp{
    fprintf(fDetail, "FuncRParams -> Exp\n");
    $$ = newNode("FuncRParams");
    addChild($$, $1);

        nodes[$$].values = vector<Node*>();
        nodes[$$].values.push_back(&nodes[$1]);
        nodes[$$].value = 1;
    }
    | FuncRParams ',' Exp{
    fprintf(fDetail, "FuncRParams -> FuncRParams , Exp\n");
    $$ = newNode("FuncRParams");
    addChild($$, $1);      
    addChild($$, newNode(","));
    addChild($$, $3);  

        nodes[$$].values = nodes[$1].values;
        nodes[$$].values.push_back(&nodes[$3]);
        nodes[$$].value = nodes[$$].values.size();
}

MulExp : UnaryExp{
    fprintf(fDetail, "MulExp -> UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
    }
    | MulExp '*' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp * UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("*"));
    addChild($$, $3); 

        if(nodes[$1].isConst && nodes[$3].isConst){
            nodes[$$].isConst = true;
            nodes[$$].value = nodes[$1].value * nodes[$3].value;
        }else{
            assemble.var2reg(&nodes[$1], "%r8d");
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.append("\timull\t%%r8d, %%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r9d, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }
    | MulExp '/' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp / UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("/"));
    addChild($$, $3); 

        if(nodes[$1].isConst && nodes[$3].isConst){
            nodes[$$].isConst = true;
            nodes[$$].value = nodes[$1].value / nodes[$3].value;
        }else{
            assemble.var2reg(&nodes[$1], "%eax");
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.append("\tcltd\n");
            assemble.append("\tidivl\t%%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }
    | MulExp '%' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp %% UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("%"));
    addChild($$, $3); 

        if(nodes[$1].isConst && nodes[$3].isConst){
            nodes[$$].isConst = true;
            nodes[$$].value = nodes[$1].value % nodes[$3].value;
        }else{
            assemble.var2reg(&nodes[$1], "%eax");
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.append("\tcltd\n");
            assemble.append("\tidivl %%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%edx, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }

AddExp : MulExp {
    fprintf(fDetail, "AddExp -> MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
    }
    | AddExp '+' MulExp{
    fprintf(fDetail, "AddExp -> AddExp + MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);      
    addChild($$, newNode("+"));
    addChild($$, $3); 

        if(nodes[$1].isConst && nodes[$3].isConst){
            nodes[$$].isConst = true;
            nodes[$$].value = nodes[$1].value + nodes[$3].value;
        }else{
            assemble.var2reg(&nodes[$1], "%r8d");
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.append("\taddl\t%%r9d, %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }
    | AddExp '-' MulExp{
    fprintf(fDetail, "AddExp -> AddExp - MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);      
    addChild($$, newNode("-"));
    addChild($$, $3); 

        if(nodes[$1].isConst && nodes[$3].isConst){
            nodes[$$].isConst = true;
            nodes[$$].value = nodes[$1].value - nodes[$3].value;
        }else{
            assemble.var2reg(&nodes[$1], "%r8d");
            assemble.var2reg(&nodes[$3], "%r9d");
            assemble.append("\tsubl\t%%r9d, %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
            nodes[$$].offset = offset;
        }
    }




RelExp : AddExp{
    fprintf(fDetail, "RelExp -> AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);
    
        copyNode(&nodes[$$], &nodes[$1]);
        // nodes[$$].quad = nodes[$1].quad;
    }
    |  RelExp '<' AddExp{
    fprintf(fDetail, "RelExp -> RelExp < AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("<"));
    addChild($$, $3); 

        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if <");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjl\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);
    }
    |  RelExp '>' AddExp{
    fprintf(fDetail, "RelExp -> RelExp > AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode(">"));
    addChild($$, $3); 

        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if >");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjg\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);
    }
    |  RelExp GEQ AddExp{
    fprintf(fDetail, "RelExp -> RelExp <= AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("<="));
    addChild($$, $3); 
        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if <=");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjle\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);


    }
    |  RelExp LEQ AddExp{
    fprintf(fDetail, "RelExp -> RelExp >= AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode(">="));
    addChild($$, $3); 
        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if >=");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjge\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);
    }

EqExp : RelExp{
    fprintf(fDetail, "EqExp -> RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);

    copyNode(&nodes[$$], &nodes[$1]);
    }
    |  EqExp EQ RelExp{
    fprintf(fDetail, "EqExp -> EqExp == RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);      
    addChild($$, newNode("=="));
    addChild($$, $3); 

        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        // assemble.labelCount--;
        // std::string targetLabel = ".L" + std::to_string(assemble.labelCount) + ":\n";
        // for (int i = assemble.buffer.size() - 1; i >= 0; --i) {
        //     if (assemble.buffer[i] == targetLabel) {
        //         assemble.buffer.erase(assemble.buffer.begin() + i);
        //         printf("\n has remove %d\n",assemble.labelCount);
        //         assemble.line--;
        //         break;
        //     }
        // }

        assemble.comment("if ==");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tje\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);
    
    }
    |  EqExp NEQ RelExp{
    fprintf(fDetail, "EqExp -> EqExp != RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);      
    addChild($$, newNode("!="));
    addChild($$, $3); 

        nodes[$$].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        // assemble.labelCount--;
        // std::string targetLabel = ".L" + std::to_string(assemble.labelCount) + ":\n";
        // for (int i = assemble.buffer.size() - 1; i >= 0; --i) {
        //     if (assemble.buffer[i] == targetLabel) {
        //         assemble.buffer.erase(assemble.buffer.begin() + i);
        //         printf("\n has remove %d\n",assemble.labelCount);
        //         assemble.line = assemble.buffer.size() - 1;
        //         break;
        //     }
        // }
        assemble.comment("if !=");
        assemble.var2reg(&nodes[$1], "%r8d");
        assemble.var2reg(&nodes[$3], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjne\t.L");
        nodes[$$].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[$$].falseList.push_back(assemble.line);
    }

LAndExp : EqExp{
    fprintf(fDetail, "LAndExp -> EqExp\n");
    $$ = newNode("LAndExp");
    addChild($$, $1);

        copyNode(&nodes[$$], &nodes[$1]);
        if(!(nodes[$$].quad)){
        nodes[$$].quad = assemble.newLabel();
        //    nodes[$$].quad = nodes[$1].quad;
            assemble.var2reg(&nodes[$1], "%r8d");
            assemble.append("\tcmpl\t$0, %%r8d\n");
            assemble.append("\tjne\t.L");
            nodes[$$].trueList.push_back(assemble.line);
            assemble.append("\tjmp\t.L");
            nodes[$$].falseList.push_back(assemble.line);
        }
    }
    | LAndExp AND EqExp{
    fprintf(fDetail, "LAndExp -> LAndExp && EqExp\n");
    $$ = newNode("LAndExp");
    addChild($$, $1);      
    addChild($$, newNode("&&"));
    addChild($$, $3); 

        assemble.backpatch(nodes[$1].trueList, nodes[$3].quad);
        nodes[$$].trueList = nodes[$3].trueList;
        nodes[$$].falseList = merge(nodes[$1].falseList, nodes[$3].falseList);
        nodes[$$].quad = nodes[$1].quad;
    }

LOrExp : LAndExp{
    fprintf(fDetail, "LOrExp -> LAndExp\n");
    $$ = newNode("LOrExp");
    addChild($$, $1);   
        
        copyNode(&nodes[$$], &nodes[$1]);
    }
    | LOrExp OR LAndExp{
    fprintf(fDetail, "LOrExp -> LOrExp || LAndExp\n");
    $$ = newNode("LOrExp");
    addChild($$, $1);      
    addChild($$, newNode("||"));
    addChild($$, $3); 


        assemble.backpatch(nodes[$1].falseList, nodes[$3].quad);
        nodes[$$].trueList = merge(nodes[$1].trueList, nodes[$3].trueList);
        nodes[$$].falseList = nodes[$3].falseList;
        nodes[$$].quad = nodes[$1].quad;
    }

ConstExp : AddExp{
    fprintf(fDetail, "ConstExp -> AddExp\n");
    $$ = newNode("ConstExp");
    addChild($$, $1);
    
        copyNode(&nodes[$$], &nodes[$1]);
}



%%
void yyerror(const char *s){
    fprintf(stderr, "Error at line %d: %s\n", lineno, s);
}

void drawTree(int tar){
    fprintf(ft, "node%d[label = \"", ++treeCnt);
    for(int i=0;i<nodes[tar].n_children;i++){
        nodes[nodes[tar].children[i]].CHD_NO = i;
        nodes[nodes[tar].children[i]].Node_NO = treeCnt;
        const char* label = nodes[nodes[tar].children[i]].label;

        fprintf(ft, "<f%d> ", i);
        for (int j = 0; label[j] != '\0'; j++) {
            if (label[j] == '%' || label[j] == '\\' || label[j] == '\"'|| label[j] == '|'|| label[j] == '&'|| label[j] == '!'|| label[j] == '='
                || label[j] == '<'|| label[j] == '>'|| label[j] == '+'|| label[j] == '-'|| label[j] == '/'|| label[j] == ','|| label[j] == '*'
                || label[j] == '{'|| label[j] == '}'|| label[j] == '('|| label[j] == ')'|| label[j] == '['|| label[j] == ']'|| label[j] == ';'
            ) {
                fprintf(ft, "\\");
            }
            fprintf(ft, "%c", label[j]);
        }
        
        if(i == nodes[tar].n_children - 1)
			fprintf(ft, "\"];\n");
		else 
			fprintf(ft, "|");
        
    }
    
    fprintf(ft, "\"node%d\":f%d->\"node%d\";\n",nodes[tar].Node_NO,nodes[tar].CHD_NO,treeCnt);
    
    for(int i=0;i<nodes[tar].n_children;i++){
        if(nodes[nodes[tar].children[i]].n_children > 0)
            drawTree(nodes[tar].children[i]);
    }
}



int main(int argc, char *argv[]) {

    yyin = fopen(argv[1], "r");
    asm_file = fopen("assemble.s", "w");
    fDetail = fopen("Detail.txt","w");
	ft = fopen("Tree.dot", "w");


    if (yyin == NULL) {
        perror("Error opening file");
        exit(1);
    }
    yyparse();
    fprintf(ft, "digraph \" \"{\n");
    fprintf(ft, "node [shape = record,height=.1]\n");
    if(!cnt){
        fprintf(ft, "node0[label = \"<f0> NULL\"];\n");
        fprintf(ft, "}");
    	fclose(fDetail);
        fclose(yyin);
	    return 0;
    }
    
    fprintf(ft, "node0[label = \"<f0> CompUnit\"];\n");
    nodes[cnt-1].CHD_NO = 0;
    nodes[cnt-1].Node_NO = 0;
    drawTree(cnt-1);
    fprintf(ft, "}");

	fclose(fDetail);
    fclose(yyin);
    
    assemble.toFile(asm_file); 
    fflush(asm_file);

	return 0;
}
