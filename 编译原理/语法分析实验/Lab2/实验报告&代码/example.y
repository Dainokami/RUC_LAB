%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int yylex(void);
    void yyerror(const char *s);
    FILE *fDetail;
    FILE *ft;
    extern FILE *yyin; 
    extern int lineno;
    // int yydebug=1;

#define MAX_NODES 1919810

typedef struct Node {
    char* label;                // 节点标签
    int children[20];    // 子节点索引列表
    int n_children;             // 子节点数量
    int CHD_NO;     //画图的时候判断是父亲的第几个儿子。
    int Node_NO;     //画图的时候判断父亲是谁。
} Node;

Node nodes[MAX_NODES];          // 全局节点数组
int cnt = 0;                    // 全局节点计数器
int treeCnt = 0;

int newNode(const char* label) {
    int idx = cnt++;
    nodes[idx].label = strdup(label);
    nodes[idx].n_children = 0;
    return idx;
}

void addChild(int parent_idx, int child_idx) {
    Node* parent = &nodes[parent_idx];
    if (parent->n_children < MAX_NODES) {
        parent->children[parent->n_children++] = child_idx;
    }
}


%}

%union {
    char* val;
    int num;
    int defalt;
}

%start CompUnit
%token CONST INT VOID IF WHILE BREAK CONTINUE RETURN
%token <val>Ident WrongIdent
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
%type <defalt>VarDecl VarDef OtherVarDef InitVal OtherVarInitVal VarArrIdex  
%type <defalt>FuncDef FuncFParams FuncFParam LExp
%type <defalt>Block Blocks BlockItem 
%type <defalt>Stmt LVal Cond 
%type <defalt>Exp AddExp PrimaryExp UnaryExp FuncRParams MulExp RelExp EqExp LAndExp ConstExp LOrExp





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
    addChild($$, newNode("\\;"));
    }
    | CONST INT error ';'{
    fprintf(fDetail, "ConstDef -> CONST INT error ;\n");
    printf("const definition error\n");
    $$ = newNode("ConstDecl");
    addChild($$, newNode("const"));
    addChild($$, newNode("int"));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode("\\;"));
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
    addChild($$, newNode("\\,"));
    addChild($$, $3);
    }

OtherConstDef : Ident '=' ConstInitVal{
    fprintf(fDetail, "OtherConstDef -> Ident = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode($1));
    addChild($$, newNode("\\="));
    addChild($$, $3);
    }
    | Ident ConstArrIdex '=' ConstInitVal {
    fprintf(fDetail, "OtherConstDef -> Ident ConstArrIdex = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode($1));
    addChild($$, $2);
    addChild($$, newNode("\\="));
    addChild($$, $4);
    }
    | WrongIdent '=' ConstInitVal{
    fprintf(fDetail, "OtherConstDef -> WrongIdent = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\="));
    addChild($$, $3);
    }
    | WrongIdent ConstArrIdex '=' ConstInitVal {
    fprintf(fDetail, "OtherConstDef -> WrongIdent ConstArrIdex = ConstInitVal\n");
    $$ = newNode("OtherConstDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    addChild($$, newNode("\\="));
    addChild($$, $4);
    }


ConstArrIdex : '[' ConstExp ']'{
    fprintf(fDetail, "ConstArrIdex -> [ ConstExp ]\n");
    $$ = newNode("ConstArrIdex");
    addChild($$, newNode("\\["));
    addChild($$, $2);
    addChild($$, newNode("\\]"));
    }
    | ConstArrIdex '[' ConstExp ']'{
    fprintf(fDetail, "ConstArrIdex -> ConstArrIdex [ ConstExp ]\n");
    $$ = newNode("ConstArrIdex");
    addChild($$, $1);
    addChild($$, newNode("\\["));
    addChild($$, $3);
    addChild($$, newNode("\\]"));
    }

ConstInitVal : ConstExp{
    fprintf(fDetail, "ConstInitVal -> ConstExp\n");
    $$ = newNode("ConstInitVal");
    addChild($$, $1);
    }
    | '{' '}'{
    fprintf(fDetail, "ConstInitVal -> { }\n");
    $$ = newNode("ConstInitVal");
    addChild($$, newNode("\\{"));
    addChild($$, newNode("\\}"));
    } 
    | '{' OtherConstInitVal '}'{
    fprintf(fDetail, "ConstInitVal -> { OtherConstInitVal }\n");
    $$ = newNode("ConstInitVal");
    addChild($$, newNode("\\{"));
    addChild($$, $2);
    addChild($$, newNode("\\}"));
    }

OtherConstInitVal : ConstInitVal{
    fprintf(fDetail, "OtherConstInitVal -> ConstInitVal\n");
    $$ = newNode("OtherConstInitVal");
    addChild($$, $1);
    }
    | OtherConstInitVal ',' ConstInitVal{
    fprintf(fDetail, "OtherConstInitVal -> OtherConstInitVal , ConstInitVal\n");
    $$ = newNode("OtherConstInitVal");
    addChild($$, $1);
    addChild($$, newNode("\\,"));
    addChild($$, $3);
    }

VarDecl : INT VarDef ';'{
    fprintf(fDetail, "VarDecl -> int VarDef ;\n");
    $$ = newNode("VarDecl");
    addChild($$, newNode("int"));
    addChild($$, $2);
    addChild($$, newNode("\\;"));
    }

VarDef : OtherVarDef{
    fprintf(fDetail, "VarDef -> OtherVarDef\n");
    $$ = newNode("VarDef");
    addChild($$, $1);
    }
    | VarDef ',' OtherVarDef{
    fprintf(fDetail, "VarDef -> VarDef , OtherVarDef\n");
    $$ = newNode("VarDef");
    addChild($$, $1);
    addChild($$, newNode("\\,"));
    addChild($$, $3);
    }

OtherVarDef : Ident{
    fprintf(fDetail, "OtherVarDef -> Ident\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    }
    | Ident VarArrIdex{
    fprintf(fDetail, "OtherVarDef -> Ident  VarArrIdex\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, $2);
    }
    | Ident '=' InitVal{
    fprintf(fDetail, "OtherVarDef -> Ident = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, newNode("\\="));
    addChild($$, $3);
    }
    | Ident VarArrIdex '=' InitVal {
    fprintf(fDetail, "OtherVarDef -> Ident VarArrIdex = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode($1));
    addChild($$, $2);
    addChild($$, newNode("\\="));
    addChild($$, $4);
    }
    | WrongIdent{
    fprintf(fDetail, "OtherVarDef -> WrongIdent\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    }
    | WrongIdent VarArrIdex{
    fprintf(fDetail, "OtherVarDef -> WrongIdent  VarArrIdex\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    }
    | WrongIdent '=' InitVal{
    fprintf(fDetail, "OtherVarDef -> WrongIdent = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\="));
    addChild($$, $3);
    }
    | WrongIdent VarArrIdex '=' InitVal {
    fprintf(fDetail, "OtherVarDef -> WrongIdent VarArrIdex = InitVal\n");
    $$ = newNode("OtherVarDef");
    addChild($$, newNode("WrongIdent"));
    addChild($$, $2);
    addChild($$, newNode("\\="));
    addChild($$, $4);
    }




VarArrIdex : '[' ConstExp ']'{
    fprintf(fDetail, "VarArrIdex -> [ ConstExp ]\n");
    $$ = newNode("VarArrIdex");
    addChild($$, newNode("\\["));
    addChild($$, $2);
    addChild($$, newNode("\\]"));
    }
    | VarArrIdex '[' ConstExp ']'{
    fprintf(fDetail, "VarArrIdex -> VarArrIdex [ ConstExp ]\n");
    $$ = newNode("VarArrIdex");
    addChild($$, $1);
    addChild($$, newNode("\\["));
    addChild($$, $3);
    addChild($$, newNode("\\]"));
    }

InitVal : Exp{
    fprintf(fDetail, "InitVal -> Exp\n");
    $$ = newNode("InitVal");
    addChild($$, $1);
    }
    | '{' '}'{
    fprintf(fDetail, "InitVal -> { }\n");
    $$ = newNode("InitVal");
    addChild($$, newNode("\\{"));
    addChild($$, newNode("\\}"));
    } 
    | '{' OtherVarInitVal '}'{
    fprintf(fDetail, "InitVal -> { OtherVarInitVal }\n");
    $$ = newNode("InitVal");
    addChild($$, newNode("\\{"));
    addChild($$, $2);
    addChild($$, newNode("\\}"));
    }

OtherVarInitVal : InitVal{
    fprintf(fDetail, "OtherVarInitVal -> InitVal\n");
    $$ = newNode("OtherVarInitVal");
    addChild($$, $1);
    }
    | OtherVarInitVal ',' InitVal{
    fprintf(fDetail, "OtherVarInitVal -> OtherVarInitVal , InitVal\n");
    $$ = newNode("OtherVarInitVal");
    addChild($$, $1);
    addChild($$, newNode("\\,"));
    addChild($$, $3);
    }

FuncDef : VOID Ident '(' ')' Block{
    fprintf(fDetail, "FuncDef -> void Ident ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    }
    | INT Ident '(' ')' Block{
    fprintf(fDetail, "FuncDef -> int Ident ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    } 
    | VOID Ident '(' FuncFParams ')' Block{
    fprintf(fDetail, "FuncDef -> void Ident ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, $4);
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | INT Ident '(' FuncFParams ')' Block{
    fprintf(fDetail, "FuncDef -> int Ident ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, $4);
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | VOID Ident '(' error ')' Block{
    fprintf(fDetail, "FuncDef -> void Ident ( ERROR ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | INT Ident '(' error ')' Block{
    fprintf(fDetail, "FuncDef -> int Ident ( ERROR ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\("));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | VOID WrongIdent '(' ')' Block{
    fprintf(fDetail, "FuncDef -> void WrongIdent ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    }
    | INT WrongIdent '(' ')' Block{
    fprintf(fDetail, "FuncDef -> int WrongIdent ( ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    } 
    | VOID WrongIdent '(' FuncFParams ')' Block{
    fprintf(fDetail, "FuncDef -> void WrongIdent ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, $4);
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | INT WrongIdent '(' FuncFParams ')' Block{
    fprintf(fDetail, "FuncDef -> int WrongIdent ( FuncFParams ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, $4);
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | VOID WrongIdent '(' error ')' Block{
    fprintf(fDetail, "FuncDef -> void WrongIdent ( ERROR ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("void"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }
    | INT WrongIdent '(' error ')' Block{
    fprintf(fDetail, "FuncDef -> int WrongIdent ( ERROR ) Block\n");
    $$ = newNode("FuncDef");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, newNode("ERROR"));
    addChild($$, newNode("\\)"));
    addChild($$, $6);
    }


FuncFParams: FuncFParam{
    fprintf(fDetail, "FuncFParams -> FuncFParam\n");
    $$ = newNode("FuncFParams");
    addChild($$, $1);
    }
    | FuncFParams ',' FuncFParam{
    fprintf(fDetail, "FuncFParams -> FuncFParams , FuncFParam\n");
    $$ = newNode("FuncFParams");
    addChild($$, $1); 
    addChild($$, newNode("\\,"));
    addChild($$, $3);
    }

FuncFParam : INT Ident{
    fprintf(fDetail, "FuncFParam -> int Ident\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    }
    | INT Ident '[' ']'{
    fprintf(fDetail, "FuncFParam -> int Ident [ ]\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\["));
    addChild($$, newNode("\\]"));
    }
    | INT Ident '[' ']' LExp{
    fprintf(fDetail, "FuncFParam -> int Ident [ ] LExp\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode($2));
    addChild($$, newNode("\\["));
    addChild($$, newNode("\\]"));
    addChild($$, $5);
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
    addChild($$, newNode("\\["));
    addChild($$, newNode("\\]"));
    }
    | INT WrongIdent '[' ']' LExp{
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ] LExp\n");
    $$ = newNode("FuncFParam");
    addChild($$, newNode("int"));
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\["));
    addChild($$, newNode("\\]"));
    addChild($$, $5);
    }

LExp : '[' Exp ']'{
    fprintf(fDetail, "LExp -> [ Exp ]\n");
    $$ = newNode("LExp");
    addChild($$, newNode("\\["));
    addChild($$, $2);
    addChild($$, newNode("\\]"));
    }
    | LExp '[' Exp ']'{
    fprintf(fDetail, "LExp -> LExp [ Exp ]\n");
    $$ = newNode("LExp");
    addChild($$, $1);
    addChild($$, newNode("\\["));
    addChild($$, $3);
    addChild($$, newNode("\\]"));
    }

Block : '{' '}'{
    fprintf(fDetail, "Block -> { }\n");
    $$ = newNode("Block");
    addChild($$, newNode("\\{"));
    addChild($$, newNode("\\}"));
    }
    | '{' Blocks '}'{
    fprintf(fDetail, "Block -> { Blocks }\n");
    $$ = newNode("Block");
    addChild($$, newNode("\\{"));
    addChild($$, $2);
    addChild($$, newNode("\\}"));
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
    addChild($$, newNode("\\="));
    addChild($$, $3);
    addChild($$, newNode("\\;"));
    }
    | Exp ';'{
    fprintf(fDetail, "Stmt -> Exp ;\n");
    $$ = newNode("Stmt");
    addChild($$, $1);
    addChild($$, newNode("\\;"));
    }
    | ';'{
    fprintf(fDetail, "Stmt -> ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("\\;"));
    }
    | Block{
    fprintf(fDetail, "Stmt -> Block\n");
    $$ = newNode("Stmt");
    addChild($$, $1);
    }
    | IF '(' Cond ')' Stmt %prec LOWER_THAN_ELSE{
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("if"));
    addChild($$, newNode("\\("));
    addChild($$, $3);
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    }
    | IF '(' Cond ')' Stmt ELSE Stmt{
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt else Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("if"));
    addChild($$, newNode("\\("));
    addChild($$, $3);
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    addChild($$, newNode("else"));
    addChild($$, $7);
    }
    | WHILE '(' Cond ')' Stmt{
    fprintf(fDetail, "Stmt -> while ( Cond ) Stmt\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("while"));
    addChild($$, newNode("\\("));
    addChild($$, $3);
    addChild($$, newNode("\\)"));
    addChild($$, $5);
    }
    | BREAK ';'{
    fprintf(fDetail, "Stmt -> break ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("break"));
    addChild($$, newNode("\\;"));
    }
    | CONTINUE ';'{
    fprintf(fDetail, "Stmt -> continue ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("continue"));
    addChild($$, newNode("\\;"));
    }
    | RETURN Exp ';'{
    fprintf(fDetail, "Stmt -> return Exp ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("return"));
    addChild($$, $2);
    addChild($$, newNode("\\;"));
    }
    | RETURN ';'{
    fprintf(fDetail, "Stmt -> return ;\n");
    $$ = newNode("Stmt");
    addChild($$, newNode("return"));
    addChild($$, newNode("\\;"));
    }

Exp : AddExp{
    fprintf(fDetail, "Exp -> AddExp\n");
    $$ = newNode("Exp");
    addChild($$, $1);
    }

Cond : LOrExp{
    fprintf(fDetail, "Cond -> LOrExp\n");
    $$ = newNode("Cond");
    addChild($$, $1);
    }

LVal : Ident{
    fprintf(fDetail, "LVal -> Ident\n");
    $$ = newNode("LVal");
    addChild($$, newNode($1));
    }
    | Ident LExp{
    fprintf(fDetail, "LVal -> Ident LExp\n");
    $$ = newNode("LVal");
    addChild($$, newNode($1));
    addChild($$, $2);
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
    addChild($$, newNode("\\("));
    addChild($$, $2);
    addChild($$, newNode("\\)"));
    }
    | LVal{
    fprintf(fDetail, "PrimaryExp -> LVal\n");
    $$ = newNode("PrimaryExp");
    addChild($$, $1);
    }
    | INTCONST{
    fprintf(fDetail, "PrimaryExp -> IntConst\n");
    $$ = newNode("PrimaryExp");
    char* buffer = malloc(20); 
    if (buffer) {
        sprintf(buffer, "%d", $1);
    }
    addChild($$, newNode(buffer)); 
    free(buffer);                  
    }

UnaryExp : PrimaryExp{
    fprintf(fDetail, "UnaryExp -> PrimaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, $1);
    }
    | Ident '(' ')'{
    fprintf(fDetail, "UnaryExp -> Ident ( )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode($1));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    }
    | Ident '(' FuncRParams ')'{
    fprintf(fDetail, "UnaryExp -> Ident ( FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode($1));
    addChild($$, newNode("\\("));
    addChild($$, $3);
    addChild($$, newNode("\\)"));
    }
    | WrongIdent '(' ')'{
    fprintf(fDetail, "UnaryExp -> WrongIdent ( )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, newNode("\\)"));
    }
    | WrongIdent '(' FuncRParams ')'{
    fprintf(fDetail, "UnaryExp -> WrongIdent ( FuncRParams )\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("WrongIdent"));
    addChild($$, newNode("\\("));
    addChild($$, $3);
    addChild($$, newNode("\\)"));
    }
    | '+' UnaryExp{
    fprintf(fDetail, "UnaryExp -> + UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("\\+"));
    addChild($$, $2);
    }
    | '-' UnaryExp{
    fprintf(fDetail, "UnaryExp -> - UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("\\-"));
    addChild($$, $2);
    }
    | '!' UnaryExp{
    fprintf(fDetail, "UnaryExp -> ! UnaryExp\n");
    $$ = newNode("UnaryExp");
    addChild($$, newNode("\\!"));
    addChild($$, $2);
    }

FuncRParams : Exp{
    fprintf(fDetail, "FuncRParams -> Exp\n");
    $$ = newNode("FuncRParams");
    addChild($$, $1);
    }
    | FuncRParams ',' Exp{
    fprintf(fDetail, "FuncRParams -> FuncRParams , Exp\n");
    $$ = newNode("FuncRParams");
    addChild($$, $1);      
    addChild($$, newNode("\\,"));
    addChild($$, $3);  
    }

MulExp : UnaryExp{
    fprintf(fDetail, "MulExp -> UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);
    }
    | MulExp '*' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp * UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("\\*"));
    addChild($$, $3); 
    }
    | MulExp '/' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp / UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("\\/"));
    addChild($$, $3); 
    }
    | MulExp '%' UnaryExp{
    fprintf(fDetail, "MulExp -> MulExp % UnaryExp\n");
    $$ = newNode("MulExp");
    addChild($$, $1);      
    addChild($$, newNode("\\%"));
    addChild($$, $3); 
    }

AddExp : MulExp {
    fprintf(fDetail, "AddExp -> MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);
    }
    | AddExp '+' MulExp{
    fprintf(fDetail, "AddExp -> AddExp + MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);      
    addChild($$, newNode("\\+"));
    addChild($$, $3); 
    }
    | AddExp '-' MulExp{
    fprintf(fDetail, "AddExp -> AddExp - MulExp\n");
    $$ = newNode("AddExp");
    addChild($$, $1);      
    addChild($$, newNode("\\-"));
    addChild($$, $3); 
    }

RelExp : AddExp{
    fprintf(fDetail, "RelExp -> AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);
    }
    | RelExp '<' AddExp{
    fprintf(fDetail, "RelExp -> RelExp < AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("\\<"));
    addChild($$, $3); 
    }
    | RelExp '>' AddExp{
    fprintf(fDetail, "RelExp -> RelExp > AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("\\>"));
    addChild($$, $3); 
    }
    | RelExp GEQ AddExp{
    fprintf(fDetail, "RelExp -> RelExp <= AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("\\<\\="));
    addChild($$, $3); 
    }
    | RelExp LEQ AddExp{
    fprintf(fDetail, "RelExp -> RelExp >= AddExp\n");
    $$ = newNode("RelExp");
    addChild($$, $1);      
    addChild($$, newNode("\\>\\="));
    addChild($$, $3); 
    }

EqExp : RelExp{
    fprintf(fDetail, "EqExp -> RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);
    }
    | EqExp EQ RelExp{
    fprintf(fDetail, "EqExp -> EqExp == RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);      
    addChild($$, newNode("\\=\\="));
    addChild($$, $3); 
    }
    | EqExp NEQ RelExp{
    fprintf(fDetail, "EqExp -> EqExp != RelExp\n");
    $$ = newNode("EqExp");
    addChild($$, $1);      
    addChild($$, newNode("\\!\\="));
    addChild($$, $3); 
    }

LAndExp : EqExp{
    fprintf(fDetail, "LAndExp -> EqExp\n");
    $$ = newNode("LAndExp");
    addChild($$, $1);
    }
    | LAndExp AND EqExp{
    fprintf(fDetail, "LAndExp -> LAndExp && EqExp\n");
    $$ = newNode("LAndExp");
    addChild($$, $1);      
    addChild($$, newNode("\\&\\&"));
    addChild($$, $3); 
    }

LOrExp : LAndExp{
    fprintf(fDetail, "LOrExp -> LAndExp\n");
    $$ = newNode("LOrExp");
    addChild($$, $1);   
    }
    | LOrExp OR LAndExp{
    fprintf(fDetail, "LOrExp -> LOrExp || LAndExp\n");
    $$ = newNode("LOrExp");
    addChild($$, $1);      
    addChild($$, newNode("\\|\\|"));
    addChild($$, $3); 
    }

ConstExp : AddExp{
    fprintf(fDetail, "ConstExp -> AddExp\n");
    $$ = newNode("ConstExp");
    addChild($$, $1);
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
        fprintf(ft, "<f%d> %s",i,nodes[nodes[tar].children[i]].label);
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
	return 0;
}