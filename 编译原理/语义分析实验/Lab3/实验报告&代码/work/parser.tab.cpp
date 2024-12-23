/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "example.y" /* yacc.c:339  */

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
std::vector<int> arr_dim;       //数组赋值用来传递数组维度
int arr_level = 0;              //数组赋值用


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
int offset,global_offset;
vector<int> offsetStack(1);
char tmp[128];
int s_and_p_index = 0;
bool is_fuc_define = false;


string funcName;
bool inVoidFunc = false;
bool hasReturn = false;
vector<Node*> paramList;

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







#line 474 "parser.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.hpp".  */
#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CONST = 258,
    INT = 259,
    VOID = 260,
    IF = 261,
    WHILE = 262,
    BREAK = 263,
    CONTINUE = 264,
    RETURN = 265,
    PRINTF = 266,
    SCANF = 267,
    Ident = 268,
    WrongIdent = 269,
    STRING = 270,
    INTCONST = 271,
    AND = 272,
    OR = 273,
    ERROR = 274,
    LOWER_THAN_ELSE = 275,
    ELSE = 276,
    EQ = 277,
    NEQ = 278,
    GEQ = 279,
    LEQ = 280
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 409 "example.y" /* yacc.c:355  */

    char* val;
    int num;
    int defalt;

#line 546 "parser.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 563 "parser.tab.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   422

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  129
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  253

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,     2,     2,    32,    43,     2,
      40,    41,    30,    28,    34,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    33,
      23,    35,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    24,    25,    26,
      27
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   444,   444,   449,   456,   461,   467,   472,   478,   486,
     496,   501,   509,   538,   591,   598,   608,   619,   633,   639,
     670,   710,   716,   725,   743,   753,   760,   770,   790,   814,
     841,   890,   895,   902,   909,   920,   927,   960,  1004,  1015,
    1030,  1041,  1052,  1064,  1079,  1088,  1097,  1108,  1114,  1127,
    1132,  1144,  1178,  1188,  1195,  1205,  1214,  1227,  1244,  1250,
    1258,  1268,  1279,  1296,  1304,  1315,  1321,  1326,  1331,  1338,
    1343,  1349,  1364,  1370,  1375,  1380,  1394,  1409,  1437,  1447,
    1457,  1472,  1487,  1496,  1507,  1514,  1521,  1528,  1534,  1542,
    1550,  1569,  1624,  1629,  1636,  1645,  1652,  1666,  1673,  1694,
    1755,  1774,  1794,  1802,  1810,  1829,  1852,  1862,  1877,  1886,
    1898,  1905,  1925,  1946,  1968,  1975,  1995,  2019,  2027,  2045,
    2063,  2082,  2100,  2107,  2137,  2166,  2183,  2196,  2203,  2217
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONST", "INT", "VOID", "IF", "WHILE",
  "BREAK", "CONTINUE", "RETURN", "PRINTF", "SCANF", "Ident", "WrongIdent",
  "STRING", "INTCONST", "AND", "OR", "ERROR", "LOWER_THAN_ELSE", "ELSE",
  "'>'", "'<'", "EQ", "NEQ", "GEQ", "LEQ", "'+'", "'-'", "'*'", "'/'",
  "'%'", "';'", "','", "'='", "'['", "']'", "'}'", "'{'", "'('", "')'",
  "'!'", "'&'", "$accept", "CompUnit", "OtherCompUnit", "Decl",
  "ConstDecl", "ConstDef", "OtherConstDef", "ConstArrIdex", "ConstInitVal",
  "LEFTB", "OtherConstInitVal", "VarDecl", "VarDef", "OtherVarDef",
  "InitVal", "OtherVarInitVal", "FuncDef", "FuncName",
  "BlockWithoutNewLevel", "EnterIntFuncBlock", "InsertIntFuncName",
  "EnterVoidFuncBlock", "InsertVoidFuncName", "EnterFuncBlock", "FuncEnd",
  "FuncFParams", "FuncFParam", "LExp", "Block", "EnterBlock", "ExitBlock",
  "Blocks", "BlockItem", "Stmt", "EnterWhile", "ExitWhile", "AfterElse",
  "NewLabel", "EnterStmt", "ExitStmt", "Exp", "Cond", "LVal", "PrimaryExp",
  "UnaryExp", "PointParams", "FuncRParams", "MulExp", "AddExp", "RelExp",
  "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    62,    60,   277,   278,   279,   280,    43,    45,
      42,    47,    37,    59,    44,    61,    91,    93,   125,   123,
      40,    41,    33,    38
};
# endif

#define YYPACT_NINF -211

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-211)))

#define YYTABLE_NINF -45

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     179,     2,    47,     6,   123,  -211,  -211,  -211,  -211,  -211,
      20,   134,    81,   180,  -211,   -12,  -211,    23,  -211,  -211,
      33,   181,   183,   187,  -211,    82,   380,   191,    82,   195,
    -211,   209,     1,    16,  -211,    82,   197,    82,   218,  -211,
     242,    41,    57,    63,    66,  -211,   380,   380,  -211,   380,
     380,  -211,   139,  -211,  -211,  -211,   155,   229,  -211,    49,
      82,   380,   319,  -211,  -211,   229,    82,   224,  -211,   248,
    -211,   -30,  -211,  -211,   -10,  -211,    82,  -211,    82,  -211,
      85,    90,   380,   338,    72,   380,    72,  -211,  -211,    78,
    -211,  -211,  -211,   120,   380,   380,   380,   380,   380,  -211,
    -211,    88,  -211,  -211,   122,  -211,    95,   102,   118,  -211,
     159,  -211,   118,  -211,  -211,  -211,  -211,   131,   154,   164,
    -211,  -211,    46,   380,    48,  -211,    82,  -211,  -211,  -211,
    -211,   155,   155,  -211,    82,  -211,   174,   178,   133,  -211,
    -211,  -211,   118,  -211,  -211,   118,   380,   185,  -211,   380,
    -211,   198,  -211,  -211,  -211,   201,   201,   209,   211,  -211,
     208,   230,   357,  -211,  -211,  -211,  -211,   227,   196,  -211,
    -211,   244,   240,  -211,  -211,  -211,  -211,    50,   380,    51,
    -211,  -211,    72,    72,   380,  -211,  -211,  -211,  -211,   258,
     236,  -211,  -211,  -211,   380,  -211,  -211,  -211,  -211,   238,
    -211,   252,   229,   107,   243,   277,   278,   255,  -211,  -211,
     276,   264,   380,  -211,   380,   380,   380,   380,   380,   380,
     380,   380,   380,  -211,  -211,  -211,  -211,  -211,   229,   229,
     229,   229,   107,   107,   243,   277,   257,  -211,   313,  -211,
    -211,  -211,   279,   313,  -211,  -211,  -211,  -211,  -211,   313,
    -211,  -211,  -211
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
       0,    27,    31,     0,    25,     0,    44,     0,     1,     3,
       0,     0,     0,     0,    10,     0,     0,    28,     0,    32,
      24,     0,     0,     0,     9,     0,     0,     0,     0,     8,
       0,     0,     0,    90,    92,    96,     0,     0,    21,     0,
       0,    29,     0,    95,    97,   110,   114,   129,    18,     0,
       0,     0,     0,    33,    35,    88,     0,    27,    26,     0,
      48,     0,    53,    50,     0,    12,     0,    14,     0,    11,
       0,     0,     0,     0,    91,     0,    93,   103,   104,     0,
     105,    19,    22,     0,     0,     0,     0,     0,     0,    16,
      30,     0,    36,    38,     0,    34,    55,    58,     0,    51,
       0,    48,     0,    51,    50,    13,    15,     0,     0,     0,
      98,   108,     0,     0,     0,    94,     0,    20,   111,   112,
     113,   115,   116,    17,     0,    37,     0,     0,    65,    52,
      47,    54,     0,    52,    49,     0,     0,     0,    61,     0,
      99,     0,   102,    23,    39,    56,    59,     0,     0,    82,
       0,     0,     0,    73,    46,    69,    74,     0,    65,    67,
      70,     0,    95,    41,    52,    40,    52,     0,     0,     0,
     109,    62,    57,    60,     0,    86,    78,    79,    81,     0,
      65,    45,    68,    72,     0,    43,    42,   101,   106,     0,
     100,     0,   117,   122,   125,   127,    89,     0,    80,    66,
      65,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    66,    71,   107,    86,   119,   118,
     120,   121,   123,   124,   126,   128,     0,    64,    65,    83,
      87,    85,    75,    65,    84,    87,    85,    77,    86,    65,
      87,    85,    76
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -211,  -211,   295,   172,  -211,  -211,   261,    55,   -22,   -19,
    -211,  -211,  -211,   271,   -44,  -211,  -211,   300,   -80,   199,
    -211,   192,  -211,   194,  -104,   275,   202,   -42,  -211,  -211,
      87,   125,  -160,  -194,  -211,  -211,  -211,  -163,  -210,  -205,
     -14,    91,  -131,  -211,   -21,  -211,   -82,   173,   -25,    54,
      97,   113,  -211,    -3
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,   165,     7,    23,    24,    27,    51,    52,
      93,     8,    13,    14,    63,   104,     9,    15,   139,   108,
     109,   112,   113,   140,   173,    71,    72,    84,   166,   167,
     223,   168,   169,   170,   185,   241,   246,   227,   207,   242,
     171,   201,    53,    54,    55,   179,   122,    56,    65,   203,
     204,   205,   206,    58
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,    57,    86,   124,   110,    69,    10,   172,   192,    62,
      57,   111,    57,    75,    64,    77,   100,   238,   103,    16,
      69,    20,   105,    59,   110,    87,    88,    57,    32,    90,
      92,   114,   143,    21,    22,    89,    57,   172,   249,   175,
     247,    62,    70,    62,   240,   251,    64,    62,    64,   245,
     192,    57,    64,    57,   115,   250,   116,    73,   101,   172,
      11,    12,   174,    33,   177,   176,    34,    29,   119,   121,
     195,   121,   196,   128,   129,   130,    36,    38,   243,   172,
     149,    80,   149,   248,   149,   199,    99,   150,   252,   152,
     154,   197,   200,    41,    42,    43,    44,    81,    45,    82,
     117,    57,    82,    83,   153,   118,    85,   172,   123,   151,
      46,    47,   172,   182,   183,    62,    28,    26,   172,   125,
      64,    48,    49,    18,    50,   133,     1,     2,     3,   214,
     215,   136,   121,   216,   217,   180,     1,   157,   137,   158,
     159,   160,   161,   162,    41,    42,    43,    44,   189,    45,
      41,    42,    43,    44,   126,    45,   134,   138,   127,   202,
     135,    46,    47,    69,   198,   146,   163,    46,    47,    25,
      26,   164,     6,    49,   -44,    50,     6,    91,    48,    49,
     211,    50,     1,     2,     3,    94,    95,    96,   147,   228,
     229,   230,   231,   202,   202,   202,   202,   202,   226,     1,
     157,   148,   158,   159,   160,   161,   162,    41,    42,    43,
      44,   155,    45,    30,    31,   156,    35,    26,    37,    26,
      39,    40,    67,    12,    46,    47,    60,    61,   178,   163,
      66,    61,    76,    61,   191,   181,    49,    82,    50,     1,
     157,   186,   158,   159,   160,   161,   162,    41,    42,    43,
      44,   184,    45,    78,    61,    21,    22,    97,    98,    25,
      26,   106,   107,   187,    46,    47,   190,   218,   219,   163,
     131,   132,   232,   233,   209,   194,    49,   193,    50,     1,
     157,   212,   158,   159,   160,   161,   162,    41,    42,    43,
      44,   208,    45,   213,   220,   222,   221,   225,   239,    19,
     244,    79,    68,    17,    46,    47,   145,   144,    74,   163,
     142,   237,   141,   236,   224,   210,    49,   234,    50,   158,
     159,   160,   161,   162,    41,    42,    43,    44,     0,    45,
      41,    42,    43,    44,   235,    45,     0,     0,     0,     0,
       0,    46,    47,     0,     0,     0,   163,    46,    47,    41,
      42,    43,    44,    49,    45,    50,     0,   102,    48,    49,
       0,    50,     0,     0,     0,     0,    46,    47,    41,    42,
      43,    44,     0,    45,     0,     0,     0,     0,    49,   120,
      50,     0,     0,     0,     0,    46,    47,     0,     0,     0,
     188,    41,    42,    43,    44,     0,    45,    49,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,    46,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,     0,    50
};

static const yytype_int16 yycheck[] =
{
      25,    26,    44,    85,    34,     4,     4,   138,   168,    28,
      35,    41,    37,    35,    28,    37,    60,   227,    62,    13,
       4,     1,    66,    26,    34,    46,    47,    52,    40,    50,
      52,    41,   112,    13,    14,    49,    61,   168,   248,   143,
     245,    60,    41,    62,   238,   250,    60,    66,    62,   243,
     210,    76,    66,    78,    76,   249,    78,    41,    61,   190,
      13,    14,   142,    40,   146,   145,    33,    12,    82,    83,
     174,    85,   176,    94,    95,    96,    21,    22,   241,   210,
      34,    40,    34,   246,    34,    34,    37,    41,   251,    41,
     134,    41,    41,    11,    12,    13,    14,    40,    16,    36,
      15,   126,    36,    40,   126,    15,    40,   238,    36,   123,
      28,    29,   243,   155,   156,   134,    35,    36,   249,    41,
     134,    39,    40,     0,    42,    37,     3,     4,     5,    22,
      23,    36,   146,    26,    27,   149,     3,     4,    36,     6,
       7,     8,     9,    10,    11,    12,    13,    14,   162,    16,
      11,    12,    13,    14,    34,    16,    34,    39,    38,   184,
      38,    28,    29,     4,   178,    34,    33,    28,    29,    35,
      36,    38,     0,    40,    40,    42,     4,    38,    39,    40,
     194,    42,     3,     4,     5,    30,    31,    32,    34,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   212,     3,
       4,    37,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    37,    16,    33,    34,    37,    35,    36,    35,    36,
      33,    34,    13,    14,    28,    29,    35,    36,    43,    33,
      35,    36,    35,    36,    38,    37,    40,    36,    42,     3,
       4,    33,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    40,    16,    35,    36,    13,    14,    28,    29,    35,
      36,    13,    14,    33,    28,    29,    39,    24,    25,    33,
      97,    98,   218,   219,    38,    35,    40,    33,    42,     3,
       4,    43,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    33,    16,    41,    17,    40,    18,    33,    41,     4,
      21,    40,    31,     3,    28,    29,   114,   113,    33,    33,
     111,   224,   110,   222,    38,   190,    40,   220,    42,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    16,
      11,    12,    13,    14,   221,    16,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    33,    28,    29,    11,
      12,    13,    14,    40,    16,    42,    -1,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    -1,    28,    29,    11,    12,
      13,    14,    -1,    16,    -1,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      33,    11,    12,    13,    14,    -1,    16,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    42
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    45,    46,    47,    48,    55,    60,
       4,    13,    14,    56,    57,    61,    13,    61,     0,    46,
       1,    13,    14,    49,    50,    35,    36,    51,    35,    51,
      33,    34,    40,    40,    33,    35,    51,    35,    51,    33,
      34,    11,    12,    13,    14,    16,    28,    29,    39,    40,
      42,    52,    53,    86,    87,    88,    91,    92,    97,    97,
      35,    36,    53,    58,    84,    92,    35,    13,    57,     4,
      41,    69,    70,    41,    69,    52,    35,    52,    35,    50,
      40,    40,    36,    40,    71,    40,    71,    88,    88,    84,
      88,    38,    52,    54,    30,    31,    32,    28,    29,    37,
      58,    97,    38,    58,    59,    58,    13,    14,    63,    64,
      34,    41,    65,    66,    41,    52,    52,    15,    15,    84,
      41,    84,    90,    36,    90,    41,    34,    38,    88,    88,
      88,    91,    91,    37,    34,    38,    36,    36,    39,    62,
      67,    70,    63,    62,    67,    65,    34,    34,    37,    34,
      41,    84,    41,    52,    58,    37,    37,     4,     6,     7,
       8,     9,    10,    33,    38,    47,    72,    73,    75,    76,
      77,    84,    86,    68,    62,    68,    62,    90,    43,    89,
      84,    37,    71,    71,    40,    78,    33,    33,    33,    84,
      39,    38,    76,    33,    35,    68,    68,    41,    84,    34,
      41,    85,    92,    93,    94,    95,    96,    82,    33,    38,
      75,    84,    43,    41,    22,    23,    26,    27,    24,    25,
      17,    18,    40,    74,    38,    33,    84,    81,    92,    92,
      92,    92,    93,    93,    94,    95,    85,    74,    82,    41,
      77,    79,    83,    81,    21,    77,    80,    83,    81,    82,
      77,    83,    81
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      49,    49,    50,    50,    50,    50,    51,    51,    52,    52,
      52,    53,    54,    54,    55,    56,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    58,    58,    58,    59,    59,
      60,    60,    60,    60,    61,    62,    62,    63,    64,    65,
      66,    67,    68,    69,    69,    70,    70,    70,    70,    70,
      70,    71,    71,    72,    72,    73,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    86,    86,    86,    87,    87,    87,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    90,    90,
      91,    91,    91,    91,    92,    92,    92,    93,    93,    93,
      93,    93,    94,    94,    94,    95,    95,    96,    96,    97
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     4,
       1,     3,     3,     4,     3,     4,     3,     4,     1,     2,
       3,     1,     1,     3,     3,     1,     3,     1,     2,     3,
       4,     1,     2,     3,     4,     1,     2,     3,     1,     3,
       7,     7,     8,     8,     1,     3,     2,     2,     0,     2,
       0,     0,     0,     1,     3,     2,     4,     5,     2,     4,
       5,     3,     4,     4,     5,     0,     0,     1,     2,     1,
       1,     4,     2,     1,     1,     8,    15,    10,     2,     2,
       3,     2,     0,     0,     0,     0,     0,     0,     1,     1,
       1,     2,     1,     2,     3,     1,     1,     1,     3,     4,
       6,     6,     4,     2,     2,     2,     2,     4,     1,     3,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 444 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "CompUnits -> OtherCompUnit\n");
    (yyval.defalt) = newNode("CompUnit");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 1858 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 449 "example.y" /* yacc.c:1646  */
    {    
    fprintf(fDetail, "CompUnits -> CompUnit OtherCompUnit\n");
    (yyval.defalt) = newNode("CompUnit");
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 1869 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 456 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherCompUnit -> Decl\n");
    (yyval.defalt) = newNode("OtherCompUnit");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 1879 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 461 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherCompUnit -> FuncDef\n");
    (yyval.defalt) = newNode("OtherCompUnit");
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 1889 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 467 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Decl -> ConstDecl\n");
    (yyval.defalt) = newNode("Decl");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 1899 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 472 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Decl -> VarDecl\n");
    (yyval.defalt) = newNode("Decl");
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 1909 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 478 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstDecl -> const int ConstDef ;\n");
    (yyval.defalt) = newNode("ConstDecl");
    addChild((yyval.defalt), newNode("const"));
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(";"));
    }
#line 1922 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 486 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstDef -> CONST INT error ;\n");
    fprintf(fDetail, "const definition error\n");
    (yyval.defalt) = newNode("ConstDecl");
    addChild((yyval.defalt), newNode("const"));
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode(";"));
}
#line 1936 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 496 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstDef -> OtherConstDef\n");
    (yyval.defalt) = newNode("ConstDef");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 1946 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 501 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstDef -> ConstDef , OtherConstDef\n");
    (yyval.defalt) = newNode("ConstDef");
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 1958 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 509 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstDef -> Ident = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode((yyvsp[-2].val)));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        if(isVarInTable((yyvsp[-2].val))){
            yyerror("Variable redefined");
            nodes[(yyval.defalt)].isError = 1;
        }else{
            if(level == 0){ // 全局变量
                assemble.append("\t.section\t.rodata\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", (yyvsp[-2].val));
                assemble.append("\t.size\t%s, 4\n", (yyvsp[-2].val));
                assemble.append("%s:\n", (yyvsp[-2].val));
                assemble.append("\t.long\t%d\n",nodes[(yyvsp[0].defalt)].value);
                assemble.append("\t.text\n");
                insertVar((yyvsp[-2].val), Var(VarType::ConstInt, nodes[(yyvsp[0].defalt)].value, 0, true));
            }else{
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t$%d, %%edi\n", nodes[(yyvsp[0].defalt)].value);
                assemble.append("\tmovl\t%%edi, %d(%rbp)\n", offset);
                insertVar((yyvsp[-2].val), Var(VarType::ConstInt, nodes[(yyvsp[0].defalt)].value, offset, false));
            }
        }
    }
#line 1992 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 538 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstDef -> Ident ConstArrIdex = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
        
        // printf("\n");
        // printf("List length is %zu\n",nodes[$4].values.size());
        // for(int i=0;i<nodes[$4].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$4].values[i]->value);
        // }
        // printf("\n");

        if(isVarInTable((yyvsp[-3].val))){
            yyerror("Variable redefined");
            nodes[(yyval.defalt)].isError = 1;
        }else{
            if(level == 0){ // 全局变量
                assemble.append("\t.section\t.rodata\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", (yyvsp[-3].val));
                assemble.append("\t.size\t%s, %d\n", (yyvsp[-3].val), nodes[(yyvsp[-2].defalt)].value*4);
                assemble.append("%s:\n", (yyvsp[-3].val));
                for(auto x: nodes[(yyvsp[0].defalt)].values){
                    assemble.append("\t.long\t%d\n", x->value);
                }
                for(int i = 0; i < nodes[(yyvsp[-2].defalt)].value - nodes[(yyvsp[0].defalt)].values.size(); i++){
                    assemble.append("\t.long\t0\n");
                }
                assemble.append("\t.text\n");
                auto var = Var(ConstArr, nodes[(yyvsp[-2].defalt)].value, 0, true, nodes[(yyvsp[-2].defalt)].values);
                for(auto v: nodes[(yyvsp[0].defalt)].values){
                    var.values2.push_back(v->value);
                }
                insertVar((yyvsp[-3].val), var);
            }else{
                offset -= 4*nodes[(yyvsp[-2].defalt)].value;
                assemble.append("\tsubq\t$%d, %%rsp\n", 4*nodes[(yyvsp[-2].defalt)].value);
                for(int i = 0; i < nodes[(yyvsp[0].defalt)].value; i++){
                    assemble.append("\tmovl\t$%d, %d(%%rbp)\n",nodes[(yyvsp[0].defalt)].values[i]->value, offset + i*4);
                }
                auto var = Var(ConstArr, nodes[(yyvsp[-2].defalt)].value, offset, false, nodes[(yyvsp[-2].defalt)].values);
                for(auto v: nodes[(yyvsp[0].defalt)].values){
                    var.values2.push_back(v->value);
                }
                insertVar((yyvsp[-3].val), var);
            }
        }

        arr_dim.clear();
    }
#line 2050 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 591 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstDef -> WrongIdent = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 2062 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 598 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstDef -> WrongIdent ConstArrIdex = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 2075 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 608 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstArrIdex -> [ ConstExp ]\n");
    (yyval.defalt) = newNode("ConstArrIdex");
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("]"));

        nodes[(yyval.defalt)].value = nodes[(yyvsp[-1].defalt)].value;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[-1].defalt)]);
        arr_dim.push_back(nodes[(yyvsp[-1].defalt)].value);
    }
#line 2091 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 619 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstArrIdex -> ConstArrIdex [ ConstExp ]\n");
    (yyval.defalt) = newNode("ConstArrIdex");
    addChild((yyval.defalt), (yyvsp[-3].defalt));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("]"));

        nodes[(yyval.defalt)].value = nodes[(yyvsp[-3].defalt)].value * nodes[(yyvsp[-1].defalt)].value;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[-1].defalt)]);
        nodes[(yyval.defalt)].values.insert(nodes[(yyval.defalt)].values.end(), nodes[(yyvsp[-3].defalt)].values.begin(), nodes[(yyvsp[-3].defalt)].values.end());
        arr_dim.push_back(nodes[(yyvsp[-1].defalt)].value);
}
#line 2109 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 633 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstInitVal -> ConstExp\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), (yyvsp[0].defalt));
        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 2120 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 639 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstInitVal -> { }\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), newNode("}"));
        nodes[(yyval.defalt)].values = vector<Node*>();
        nodes[(yyval.defalt)].value = 0;
        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];


        while(of--){
            Node* nNode = new Node();
            nNode->value = 0;
            nNode->isConst = true;
            nodes[(yyval.defalt)].values.push_back(nNode);
        }
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();

        // printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

        arr_level--;        
        

    }
#line 2156 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 670 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstInitVal -> { OtherConstInitVal }\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("}"));
        nodes[(yyval.defalt)].values = nodes[(yyvsp[-1].defalt)].values;
        nodes[(yyval.defalt)].value = nodes[(yyvsp[-1].defalt)].values.size();

        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];
        arr_level--;

        if(nodes[(yyval.defalt)].value == of){
            break;
        }
        else if(of > nodes[(yyval.defalt)].value){
            of -= nodes[(yyval.defalt)].value;
            while(of--){
                Node* nNode = new Node();
                copyNode(nNode, nodes[(yyvsp[-1].defalt)].values[0]);
                nNode->value = 0;
                nNode->isConst = true;
                nodes[(yyval.defalt)].values.push_back(nNode);
            }
        }
        else{
            yyerror("Wrong arrList");
        }
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();
        //     printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

    }
#line 2200 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 710 "example.y" /* yacc.c:1646  */
    {
    arr_level++;
}
#line 2208 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 716 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstInitVal -> ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstInitVal");
    addChild((yyval.defalt), (yyvsp[0].defalt));
        if(nodes[(yyvsp[0].defalt)].values.size() == 0)
            nodes[(yyval.defalt)].values.push_back(nodes+(yyvsp[0].defalt));
        else
            nodes[(yyval.defalt)].values = nodes[(yyvsp[0].defalt)].values;
    }
#line 2222 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 725 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherConstInitVal -> OtherConstInitVal , ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstInitVal");
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        nodes[(yyval.defalt)].values = nodes[(yyvsp[-2].defalt)].values;
        if(nodes[(yyvsp[0].defalt)].values.size() == 0)
            nodes[(yyval.defalt)].values.push_back(nodes+(yyvsp[0].defalt));
        else
            nodes[(yyval.defalt)].values.insert(nodes[(yyval.defalt)].values.end(), nodes[(yyvsp[0].defalt)].values.begin(), nodes[(yyvsp[0].defalt)].values.end());
    }
#line 2240 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 743 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "VarDecl -> int VarDef ;\n");
    (yyval.defalt) = newNode("VarDecl");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(";"));

        // no need to do anything
}
#line 2254 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 753 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "VarDef -> OtherVarDef\n");
    (yyval.defalt) = newNode("VarDef");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        // no need to do anything
    }
#line 2266 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 760 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "VarDef -> VarDef , OtherVarDef\n");
    (yyval.defalt) = newNode("VarDef");
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        // no need to do anything
}
#line 2280 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 770 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> Ident\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[0].val)));

        if(level == 0){
            assemble.append("\t.globl\t%s\n", (yyvsp[0].val));
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", (yyvsp[0].val));
            assemble.append("\t.size\t%s, 4\n", (yyvsp[0].val));
            assemble.append("\t.long\t0\n");
            assemble.append("\t.text\n");
            insertVar((yyvsp[0].val), Var(Int, 0, 0, true));
        }else{
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            insertVar((yyvsp[0].val), Var(Int, 0, offset, false));
        }
    }
#line 2305 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 790 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> Ident  ConstArrIdex\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[-1].val)));
    addChild((yyval.defalt), (yyvsp[0].defalt));
        arr_dim.clear();
        if(level==0){
            assemble.append("\t.globl\t%s\n", (yyvsp[-1].val));
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", (yyvsp[-1].val));
            assemble.append("\t.size\t%s, %d\n", (yyvsp[-1].val), nodes[(yyvsp[0].defalt)].value*4);
            assemble.append("%s:\n", (yyvsp[-1].val));
            for(int i=0; i<nodes[(yyvsp[0].defalt)].value; i++){
                assemble.append("\t.long\t0\n");
            }
            assemble.append("\t.text\n");
            insertVar((yyvsp[-1].val), Var(Arr, 0, 0, true, nodes[(yyvsp[0].defalt)].values));
        }else{
            offset -= nodes[(yyvsp[0].defalt)].value*4;
            assemble.append("\tsubq\t$%d, %%rsp\n", nodes[(yyvsp[0].defalt)].value*4);
            insertVar((yyvsp[-1].val), Var(Arr, 0, offset, false, nodes[(yyvsp[0].defalt)].values));
        }
    }
#line 2334 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 814 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> Ident = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[-2].val)));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        if(level == 0){
            assemble.append("\t.globl\t%s\n", (yyvsp[-2].val));
            assemble.append("\t.data\n");
            assemble.append("\t.align\t4\n");
            assemble.append("\t.type\t%s, @object\n", (yyvsp[-2].val));
            assemble.append("\t.size\t%s, 4\n", (yyvsp[-2].val));
            assemble.append("%s:\n", (yyvsp[-2].val));
            assemble.append("\t.long\t%d\n", nodes[(yyvsp[0].defalt)].value);
            assemble.append("\t.text\n");
            insertVar((yyvsp[-2].val), Var(Int, nodes[(yyvsp[0].defalt)].value, 0, true));
        }else{
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%edi");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%edi, %d(%%rbp)\n", offset);
            insertVar((yyvsp[-2].val), Var(Int, nodes[(yyvsp[0].defalt)].value, offset, false));
        }

        arr_dim.clear();
    }
#line 2366 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 841 "example.y" /* yacc.c:1646  */
    {
    
    arr_dim.clear();
    fprintf(fDetail, "OtherVarDef -> Ident ConstArrIdex = InitValList\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
        nodes[(yyvsp[0].defalt)].value = nodes[(yyvsp[0].defalt)].values.size();
        // printf("\n ConstArrIdex is %d",nodes[$2].value);
        // printf("\n InitVal is %d",nodes[$4].value);
        // for(int i=0;i<nodes[$4].value;i++){
        //     printf("\n InitVal %d is %d",i,nodes[$4].values[i]->value);
        // }
        if(nodes[(yyvsp[0].defalt)].value > nodes[(yyvsp[-2].defalt)].value){
            yyerror("Initialize array with too many values");
        }else{
            if(level == 0){
                assemble.append("\t.globl\t%s\n", (yyvsp[-3].val));
                assemble.append("\t.data\n");
                assemble.append("\t.align\t4\n");
                assemble.append("\t.type\t%s, @object\n", (yyvsp[-3].val));
                assemble.append("\t.size\t%s, %d\n", (yyvsp[-3].val), nodes[(yyvsp[-2].defalt)].value*4);
                assemble.append("%s:\n", (yyvsp[-3].val));
                for(int i = 0; i < nodes[(yyvsp[0].defalt)].value; i++){
                    assemble.append("\t.long\t%d\n", nodes[(yyvsp[0].defalt)].values[i]->value);
                }
                for(int i = nodes[(yyvsp[0].defalt)].value; i < nodes[(yyvsp[-2].defalt)].value; i++){
                    assemble.append("\t.long\t0\n");
                }
                assemble.append("\t.text\n");
                insertVar((yyvsp[-3].val), Var(Arr, 0, 0, true, nodes[(yyvsp[-2].defalt)].values));
            } else{
                offset -= nodes[(yyvsp[-2].defalt)].value*4;
                assemble.append("\tsubq\t$%d, %%rsp\n", nodes[(yyvsp[-2].defalt)].value*4);
                for(int i = 0; i < nodes[(yyvsp[0].defalt)].value; i++){
                    assemble.var2reg(nodes[(yyvsp[0].defalt)].values[i], "%edi");
                    assemble.append("\tmovl\t%%edi, %d(%%rbp)\n", offset+i*4);
                }
                for(int i = nodes[(yyvsp[0].defalt)].value; i < nodes[(yyvsp[-2].defalt)].value; i++){
                    assemble.append("\tmovl\t$0, %d(%%rbp)\n", offset+i*4);
                }
                insertVar((yyvsp[-3].val), Var(Arr, 0, offset, false, nodes[(yyvsp[-2].defalt)].values));
            }
        }

        
    }
#line 2420 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 890 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
#line 2430 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 895 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent  ConstArrIdex\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[0].defalt));
    arr_dim.clear();
    }
#line 2442 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 902 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 2454 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 909 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent ConstArrIdex = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[0].defalt));
    arr_dim.clear();
}
#line 2468 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 920 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "InitVal -> Exp\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 2480 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 927 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "InitValList -> { }\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), newNode("}"));

        nodes[(yyval.defalt)].values = vector<Node*>();
        nodes[(yyval.defalt)].value = 0;
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
            nodes[(yyval.defalt)].values.push_back(nNode);
        }
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();

        // printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");

        arr_level--;  
    }
#line 2518 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 960 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "InitValList -> { OtherVarInitVal }\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("}"));

        nodes[(yyval.defalt)].values = nodes[(yyvsp[-1].defalt)].values;
        nodes[(yyval.defalt)].value = nodes[(yyvsp[-1].defalt)].values.size();
        
        int of = 1;
        for(int i=arr_level;i<=arr_dim.size();i++)
            of *= arr_dim[i-1];
        arr_level--;

        // printf("\n");
        // printf("of is %d\n",of);
        // printf("\n");
        
        if(nodes[(yyval.defalt)].value == of){
            break;
        }
        else if(of > nodes[(yyval.defalt)].value){
            of -= nodes[(yyval.defalt)].value;
            while(of--){
                Node* nNode = new Node();
                copyNode(nNode, nodes[(yyvsp[-1].defalt)].values[0]);
                nNode->value = 0;
                nNode->isConst = true;
                nodes[(yyval.defalt)].values.push_back(nNode);
            }
        }
        else{
            yyerror("Wrong arrList");
        }
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();
        //     printf("\n");
        // printf("List length is %zu\n",nodes[$$].values.size());
        // for(int i=0;i<nodes[$$].values.size();i++){
        //     printf("List %d is %d\n",i,nodes[$$].values[i]->value);
        // }
        // printf("\n");
}
#line 2566 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 1004 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarInitVal -> InitVal\n");
    (yyval.defalt) = newNode("OtherVarInitVal");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        if(nodes[(yyvsp[0].defalt)].values.size() == 0)
            nodes[(yyval.defalt)].values.push_back(nodes+(yyvsp[0].defalt));
        else
            nodes[(yyval.defalt)].values = nodes[(yyvsp[0].defalt)].values;
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();
    }
#line 2582 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 1015 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "OtherVarInitVal -> OtherVarInitVal , InitVal\n");
    (yyval.defalt) = newNode("OtherVarInitVal");
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        nodes[(yyval.defalt)].values = nodes[(yyvsp[-2].defalt)].values;
        if(nodes[(yyvsp[0].defalt)].values.size() == 0)
            nodes[(yyval.defalt)].values.push_back(nodes+(yyvsp[0].defalt));
        else
            nodes[(yyval.defalt)].values.insert(nodes[(yyval.defalt)].values.end(), nodes[(yyvsp[0].defalt)].values.begin(), nodes[(yyvsp[0].defalt)].values.end());
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();        
}
#line 2601 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 1030 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncDef -> void Ident ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), (yyvsp[-5].defalt));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
        inVoidFunc = false;
        // no need to do anything
    }
#line 2617 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 1041 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncDef -> int Ident ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[-5].defalt));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));

        // no need to do anything
    }
#line 2633 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 1052 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncDef -> void Ident ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), (yyvsp[-6].defalt));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-4].defalt));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
        inVoidFunc = false;
        // no need to do anything
    }
#line 2650 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 1064 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncDef -> int Ident ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[-6].defalt));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-4].defalt));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));

        // no need to do anything
    }
#line 2667 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 1079 "example.y" /* yacc.c:1646  */
    {
        fprintf(fDetail, "FuncName -> Ident\n");
        (yyval.defalt) = newNode("FuncName");
        addChild((yyval.defalt), newNode((yyvsp[0].val)));
        funcName = (yyvsp[0].val);
    }
#line 2678 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 1088 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "BlockWithoutNewLevel -> { Blocks }\n");
    (yyval.defalt) = newNode("BlockWithoutNewLevel");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("}"));

        // no need to do anything
    }
#line 2692 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 1097 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "BlockWithoutNewLevel -> { }\n");
    (yyval.defalt) = newNode("BlockWithoutNewLevel");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), newNode("}"));

        // no need to do anything
    }
#line 2705 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 1108 "example.y" /* yacc.c:1646  */
    {
        fprintf(fDetail, "EnterIntFuncBlock -> InsertIntFuncName EnterFuncBlock\n");
    }
#line 2713 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 1114 "example.y" /* yacc.c:1646  */
    {
        fprintf(fDetail, "InsertIntFuncName\n");

        if(isFuncInTable(funcName)){
            yyerror("duplicated function name");
        }else{
            insertFunc(funcName, Var(FuncInt, 0, 0, false, paramList));
        }
        
    }
#line 2728 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 1127 "example.y" /* yacc.c:1646  */
    {
    }
#line 2735 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 1132 "example.y" /* yacc.c:1646  */
    {
        fprintf(fDetail, "InsertVoidFuncName\n");
        if(isFuncInTable(funcName)){
            yyerror("duplicated function name");
        }else{
            insertFunc(funcName, Var(FuncVoid, 0, 0, false, paramList));
            inVoidFunc = true;
        }
    }
#line 2749 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 1144 "example.y" /* yacc.c:1646  */
    {
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
#line 2785 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 1178 "example.y" /* yacc.c:1646  */
    {
        exitLevel();
        if(!hasReturn){
            assemble.append("\taddq\t$%d, %%rsp\n", -offset);
            assemble.ret();
        }
        offset = 0;
    }
#line 2798 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 1188 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParams -> FuncFParam\n");
    (yyval.defalt) = newNode("FuncFParams");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        // no need to do anything
    }
#line 2810 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 1195 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParams -> FuncFParams , FuncFParam\n");
    (yyval.defalt) = newNode("FuncFParams");
    addChild((yyval.defalt), (yyvsp[-2].defalt)); 
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        // no need to do anything
}
#line 2824 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 1205 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int Ident\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[0].val)));

        strcpy(nodes[(yyval.defalt)].label, (yyvsp[0].val));
        paramList.push_back(&nodes[(yyval.defalt)]);
    }
#line 2838 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 1214 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int Ident [ ]\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[-2].val)));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), newNode("]"));

        strcpy(nodes[(yyval.defalt)].label, (yyvsp[-2].val));
        nodes[(yyval.defalt)].values.push_back(nullptr);
        nodes[(yyval.defalt)].isArr = true;
        paramList.push_back(&nodes[(yyval.defalt)]);
    }
#line 2856 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 1227 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int Ident [ ] LExp\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), newNode("]"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        strcpy(nodes[(yyval.defalt)].label, (yyvsp[-3].val));
        for(auto dim: nodes[(yyvsp[0].defalt)].values){
            nodes[(yyval.defalt)].values.push_back(dim);
        }
        nodes[(yyval.defalt)].values.push_back(nullptr);
        nodes[(yyval.defalt)].isArr = true;
        paramList.push_back(&nodes[(yyval.defalt)]);
    }
#line 2878 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 1244 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
#line 2889 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 1250 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ]\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), newNode("]"));
    }
#line 2902 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 1258 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ] LExp\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), newNode("]"));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 2916 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 1268 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LExp -> [ Exp ]\n");
    (yyval.defalt) = newNode("LExp");
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("]"));

        nodes[(yyval.defalt)].value = nodes[(yyvsp[-1].defalt)].value;
        nodes[(yyval.defalt)].isConst = nodes[(yyvsp[-1].defalt)].isConst;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[-1].defalt)]);
    }
#line 2932 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 1279 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LExp -> LExp [ Exp ]\n");
    (yyval.defalt) = newNode("LExp");
    addChild((yyval.defalt), (yyvsp[-3].defalt));
    addChild((yyval.defalt), newNode("["));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode("]"));

        nodes[(yyval.defalt)].value = nodes[(yyvsp[-3].defalt)].value * nodes[(yyvsp[-1].defalt)].value;
        nodes[(yyval.defalt)].isConst = nodes[(yyvsp[-3].defalt)].isConst && nodes[(yyvsp[-1].defalt)].isConst;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[-1].defalt)]);
        nodes[(yyval.defalt)].values.insert(nodes[(yyval.defalt)].values.end(), nodes[(yyvsp[-3].defalt)].values.begin(), nodes[(yyvsp[-3].defalt)].values.end());

}
#line 2951 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 1296 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Block -> { }\n");
    (yyval.defalt) = newNode("Block");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), newNode("}"));

        // no need to do anything
    }
#line 2964 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 1304 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Block -> { Blocks }\n");
    (yyval.defalt) = newNode("Block");
    addChild((yyval.defalt), newNode("{"));
    addChild((yyval.defalt), (yyvsp[-2].defalt));
    addChild((yyval.defalt), newNode("}"));

        // no need to do anything
}
#line 2978 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 1315 "example.y" /* yacc.c:1646  */
    {
        nextLevel();
        offsetStack.push_back(offset);
    }
#line 2987 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 1321 "example.y" /* yacc.c:1646  */
    {
        exitLevel();
        recoverStack();
    }
#line 2996 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 1326 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Blocks -> BlockItem\n");
    (yyval.defalt) = newNode("Blocks");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 3006 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 1331 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Blocks -> Blocks BlockItem\n");
    (yyval.defalt) = newNode("Blocks");
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 3017 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 1338 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "BlockItem -> Decl\n");
    (yyval.defalt) = newNode("BlockItem");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 3027 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 1343 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "BlockItem -> Stmt\n");
    (yyval.defalt) = newNode("BlockItem");
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 3037 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 1349 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> LVal = Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[-3].defalt));
    addChild((yyval.defalt), newNode("="));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(";"));

        if(nodes[(yyvsp[-3].defalt)].isConst){
            yyerror("assign to const");
        }else{
            assemble.var2reg(&nodes[(yyvsp[-1].defalt)], "%r9d");
            assemble.reg2var("%r9d", &nodes[(yyvsp[-3].defalt)]);
        }
    }
#line 3057 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1364 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(";"));
    }
#line 3068 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1370 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode(";"));
    }
#line 3078 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1375 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> Block\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    }
#line 3088 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 1380 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("if"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-5].defalt));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));

        assemble.backpatch(nodes[(yyvsp[-5].defalt)].trueList, nodes[(yyvsp[-3].defalt)].quad);
        int end = assemble.newLabel();
        assemble.comment("if end");
        assemble.backpatch(nodes[(yyvsp[-5].defalt)].falseList, end);
    }
#line 3107 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1394 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt else Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("if"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-12].defalt));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-8].defalt));
    addChild((yyval.defalt), newNode("else"));
    addChild((yyval.defalt), (yyvsp[-2].defalt));

        assemble.backpatch(nodes[(yyvsp[-12].defalt)].trueList, nodes[(yyvsp[-10].defalt)].quad);
        assemble.backpatch(nodes[(yyvsp[-12].defalt)].falseList, nodes[(yyvsp[-4].defalt)].quad);
        assemble.backpatch(nodes[(yyvsp[-5].defalt)].trueList, nodes[(yyvsp[0].defalt)].quad);
    }
#line 3127 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 1409 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> while ( Cond ) Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("while"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-5].defalt));
    addChild((yyval.defalt), newNode(")"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));

        assemble.backpatch(nodes[(yyvsp[-5].defalt)].trueList, nodes[(yyvsp[-2].defalt)].quad);
        assemble.append("\tjmp\t.L%d\n", nodes[(yyvsp[-8].defalt)].quad);
        int whileEnd = assemble.newLabel();
        assemble.comment("while end");
        assemble.backpatch(nodes[(yyvsp[-5].defalt)].falseList, nodes[(yyvsp[-3].defalt)].quad);
        assemble.backpatch(nodes[(yyvsp[-3].defalt)].trueList, whileEnd);
        for(auto [line, of]: breakStack.back()){
            sprintf(tmp, "\taddq\t$%d, %%rsp\n", offset - of);
            assemble[line - 1] = tmp; 
            assemble[line] += to_string(whileEnd) + "\n";
        }
        breakStack.pop_back();
        for(auto [line, of]: continueStack.back()){
            sprintf(tmp, "\taddq\t$%d, %%rsp\n", offset - of);
            assemble[line - 1] = tmp; 
            assemble[line] += to_string(nodes[(yyvsp[-8].defalt)].quad) + "\n";
        }
        continueStack.pop_back();
    }
#line 3160 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 1437 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> break ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("break"));
    addChild((yyval.defalt), newNode(";"));

        assemble.append("");
        assemble.append("\tjmp\t.L");
        breakStack.back().push_back({assemble.line, offset});
    }
#line 3175 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1447 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> continue ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("continue"));
    addChild((yyval.defalt), newNode(";"));

        assemble.append("");
        assemble.append("\tjmp\t.L");
        continueStack.back().push_back({assemble.line, offset});
    }
#line 3190 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 1457 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> return Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("return"));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(";"));

       if(inVoidFunc){
            yyerror("return in void function");
        }
        hasReturn = true;
        assemble.var2reg(&nodes[(yyvsp[-1].defalt)], "%eax");
        assemble.append("\taddq\t$%d, %%rsp\n", -offset);
        assemble.ret();
    }
#line 3210 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1472 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Stmt -> return ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("return"));
    addChild((yyval.defalt), newNode(";"));

        if(!inVoidFunc){
            yyerror("return nothing in non-void function");
        }
        hasReturn = true;
        assemble.append("\taddq\t$%d, %%rsp\n", -offset);
        assemble.ret();
}
#line 3228 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1487 "example.y" /* yacc.c:1646  */
    {
        (yyval.defalt) = newNode("EnterWhile");
        nodes[(yyval.defalt)].quad = assemble.newLabel();
        assemble.comment("enter while");
        breakStack.push_back(vector<pii>());
        continueStack.push_back(vector<pii>());
    }
#line 3240 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1496 "example.y" /* yacc.c:1646  */
    {
        (yyval.defalt) = newNode("ExitWhile");
        nodes[(yyval.defalt)].quad = assemble.newLabel();
        assemble.comment("exit while");
        int oldOffset = offsetStack.back();
        assemble.append("\taddq\t$%d, %%rsp\n", oldOffset - offset);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
    }
#line 3254 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1507 "example.y" /* yacc.c:1646  */
    {
        (yyval.defalt) = newNode("AfterElse");
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
    }
#line 3264 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1514 "example.y" /* yacc.c:1646  */
    {
        (yyval.defalt) = newNode("NewLabel");
        nodes[(yyval.defalt)].quad = assemble.newLabel();
    }
#line 3273 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1521 "example.y" /* yacc.c:1646  */
    {
        assemble.comment("enter stmt");
        offsetStack.push_back(offset);
    }
#line 3282 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1528 "example.y" /* yacc.c:1646  */
    {
        recoverStack();
    }
#line 3290 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1534 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Exp -> AddExp\n");
    (yyval.defalt) = newNode("Exp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
}
#line 3302 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1542 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "Cond -> LOrExp\n");
    (yyval.defalt) = newNode("Cond");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
}
#line 3314 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1550 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LVal -> Ident\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode((yyvsp[0].val)));

        strcpy(nodes[(yyval.defalt)].label, (yyvsp[0].val));
        if(!isVarInTable((yyvsp[0].val))){
            yyerror("Reference Undefined Variable");
        }else{
            auto [depth, var] = getVar((yyvsp[0].val));
            nodes[(yyval.defalt)].isGlobal = var.isGlobal;
            if(var.type == ConstInt){
                nodes[(yyval.defalt)].isConst = true;
                nodes[(yyval.defalt)].value = var.value;
            }else{
                nodes[(yyval.defalt)].offset = var.offset;
            }
        }
    }
#line 3338 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1569 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LVal -> Ident LExp\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode((yyvsp[-1].val)));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        // printf("\n ex leng is  %d",nodes[$2].values.size());
        // for(int i=0;i<nodes[$2].values.size();i++)
        //     printf("\n ex is  %d",nodes[$2].values[i]->value);

        strcpy(nodes[(yyval.defalt)].label, (yyvsp[-1].val));
        if(!isVarInTable((yyvsp[-1].val))){
            yyerror("Reference Undefined Variable");
        }else{
            auto [depth, var] = getVar((yyvsp[-1].val));
            if(var.type != Arr && var.type != ConstArr && var.type != Addr){
                yyerror("Reference Non-Array Variable");
            }
            nodes[(yyval.defalt)].isGlobal = var.isGlobal;
            if(var.type == ConstArr && nodes[(yyvsp[0].defalt)].isConst){
                nodes[(yyval.defalt)].isConst = true;
                int curSize = 1;
                int of = 0;
                int delta = var.values.size() - nodes[(yyvsp[0].defalt)].values.size();
                for(int i = 0; i < var.values.size(); i++){
                    if(i >= delta){
                        of += curSize * nodes[(yyvsp[0].defalt)].values[i - delta]->value;
                    }
                    curSize *= var.values[i]->value;
                }
                nodes[(yyval.defalt)].value = getVarValue(var, of);
            } else {
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t$0, %d(%%rbp)\n", offset);
                int curSize = 1;
                int delta = var.values.size() - nodes[(yyvsp[0].defalt)].values.size();
                for(int i = 0; i < var.values.size(); i++){
                    if(i >= delta){
                        assemble.var2reg(nodes[(yyvsp[0].defalt)].values[i - delta], "%r8d");
                        assemble.append("\timull\t$%d, %%r8d\n", curSize);
                        assemble.append("\taddl\t%d(%%rbp), %%r8d\n", offset);
                        assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
                    }
                    if(var.values[i]){
                        curSize *= var.values[i]->value;
                    }
                }
                nodes[(yyval.defalt)].isArr = true;
                nodes[(yyval.defalt)].offset = var.offset;
                nodes[(yyval.defalt)].isAddr = var.type == Addr;
                nodes[(yyval.defalt)].offsetInArray = offset;
            }
        }
    }
#line 3398 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1624 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LVal -> WrongIdent\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
#line 3408 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1629 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LVal -> WrongIdent LExp\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[0].defalt));
}
#line 3419 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1636 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "PrimaryExp -> ( Exp )\n");
    (yyval.defalt) = newNode("PrimaryExp");
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(")"));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[-1].defalt)]);
    }
#line 3433 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1645 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "PrimaryExp -> LVal\n");
    (yyval.defalt) = newNode("PrimaryExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 3445 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1652 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "PrimaryExp -> IntConst\n");
    (yyval.defalt) = newNode("PrimaryExp");
    char* buffer = new char[20];
    if (buffer) {
        sprintf(buffer, "%d", (yyvsp[0].num));
    }
    addChild((yyval.defalt), newNode(buffer)); 
    delete[] buffer;  

        nodes[(yyval.defalt)].isConst = true;
        nodes[(yyval.defalt)].value = (yyvsp[0].num);              
}
#line 3463 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1666 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> PrimaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 3475 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1673 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> Ident ( )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode((yyvsp[-2].val)));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), newNode(")"));

        if(!isFuncInTable((yyvsp[-2].val))){
            yyerror("Call Undefined Function");
        }{
            alignStack();
            assemble.append("\tcall\t%s\n", (yyvsp[-2].val));
            auto [depth, func] = getFunc((yyvsp[-2].val));
            if(func.type == FuncInt){
                offset -= 4;
                assemble.append("\taddq\t$4, %%rsp\n");
                assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
                nodes[(yyval.defalt)].offset = offset;
            }
        }
    }
#line 3501 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1694 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> Ident ( FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(")"));

 
        if(!isFuncInTable((yyvsp[-3].val))){
            yyerror("Call Undefined Function");
        }{
            alignStack();
            //funcName
            auto [depth, func] = getFunc((yyvsp[-3].val));
            auto [depth1, func1] = getFunc(funcName);
            
            for(int i = nodes[(yyvsp[-1].defalt)].value - 1; i >= 0 ; i--){
                bool flag = false;
                for(auto cur:func1.values){
                    if(!strcmp(cur->label, nodes[(yyvsp[-1].defalt)].values[i]->label)){
                        flag = true;
                        break;
                    }
                }
                
                std::string tmpstr = "main";
                if(funcName == tmpstr)
                    flag = false;
                // printf("\nname is %s and is exit %d\n",nodes[$3].values[i]->label,flag);


                if(func.values[i]->isArr && !flag){
                    assemble.var2reg(nodes[(yyvsp[-1].defalt)].values[i], "%r8", true);
                    assemble.append("\tsubq\t$8, %%rsp\n");
                    offset -= 8;
                    assemble.append("\tmovq\t%%r8, %d(%%rbp)\n", offset);

                }else if(func.values[i]->isArr &&flag){
                    assemble.append("\tmovq\t%d(%%rbp), %%r8\n", nodes[(yyvsp[-1].defalt)].values[i]->offset);
                    assemble.append("\tsubq\t$8, %%rsp\n");
                    offset -= 8;
                    assemble.append("\tmovq\t%%r8, %d(%%rbp)\n", offset);
                }else{
                    assemble.var2reg(nodes[(yyvsp[-1].defalt)].values[i], "%r8d");
                    assemble.append("\tsubq\t$4, %%rsp\n");
                    offset -= 4;
                    assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
                }
            }
            alignStack();
            assemble.append("\tcall\t%s\n", (yyvsp[-3].val));
            if(func.type == FuncInt){
                offset -= 4;
                assemble.append("\tsubq\t$4, %%rsp\n");
                assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
                nodes[(yyval.defalt)].offset = offset;
            }
        }
        
    }
#line 3567 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1755 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> sacnf ( \"string\" , FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("scanf"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(")"));

        alignStack();
		fprintf(asm_file, ".LC%d:\n", s_and_p_index);
		fprintf(asm_file, "\t.string\t%s\n", (yyvsp[-3].val));
        assemble.var2reg(nodes[(yyvsp[-1].defalt)].values[0], "%rsi", true);
        assemble.append("\tleaq\t.LC%d(%%rip), %%rdi\n",s_and_p_index);
        assemble.append("\tmovl\t$0, %%eax\n");
        assemble.append("\tcall\t__isoc99_scanf@PLT\n");
        s_and_p_index++;
    }
#line 3591 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1774 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> sacnf ( \"string\" , FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("printf"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), newNode((yyvsp[-3].val)));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(")"));
            alignStack();
    		fprintf(asm_file, ".LC%d:\n", s_and_p_index);
		    fprintf(asm_file, "\t.string\t%s\n", (yyvsp[-3].val));
            assemble.var2reg(nodes[(yyvsp[-1].defalt)].values[0], "%esi");
            if(nodes[(yyvsp[-1].defalt)].values.size()>1)
                assemble.var2reg(nodes[(yyvsp[-1].defalt)].values[1], "%ecx");
            assemble.append("\tleaq\t.LC%d(%%rip), %%rdi\n",s_and_p_index);
            assemble.append("\tmovl\t$0, %%eax\n");
            assemble.append("\tcall\tprintf@PLT\n");
            s_and_p_index++;
    }
#line 3616 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1794 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> WrongIdent ( FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("("));
    addChild((yyval.defalt), (yyvsp[-1].defalt));
    addChild((yyval.defalt), newNode(")"));
    }
#line 3629 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1802 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> + UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("+"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)],&nodes[(yyvsp[0].defalt)]);
    }
#line 3642 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1810 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> - UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("-"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        if(nodes[(yyvsp[0].defalt)].isConst){
            // copyNode($$, $2);
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = -nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r8d");
            assemble.append("\tneg %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq $4, %%rsp\n");
            assemble.append("\tmovl %%r8d, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3666 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1829 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "UnaryExp -> ! UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("!"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        if(nodes[(yyvsp[0].defalt)].isConst){
            // copyNode($$, $2);
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = !nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%eax");
            assemble.append("\ttestl %%eax, %%eax\n");
            assemble.append("\tsete %%al\n");
            assemble.append("\tmovzbl %%al, %%eax\n");
            offset -= 4;
            assemble.append("\tsubq $4, %%rsp\n");
            assemble.append("\tmovl %%eax, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
}
#line 3692 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1852 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "PointParams -> & Exp\n");
    (yyval.defalt) = newNode("PointParams");
    addChild((yyval.defalt), newNode("&"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        nodes[(yyval.defalt)].values = vector<Node*>();
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[0].defalt)]);
        nodes[(yyval.defalt)].value = 1;
    }
#line 3707 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1862 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "PointParams -> PointParams , & Exp\n");
    (yyval.defalt) = newNode("PointParams");
    addChild((yyval.defalt), (yyvsp[-3].defalt));
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), newNode("&"));
    addChild((yyval.defalt), (yyvsp[0].defalt));

        nodes[(yyval.defalt)].values = nodes[(yyvsp[-3].defalt)].values;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[0].defalt)]);
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();
}
#line 3724 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1877 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncRParams -> Exp\n");
    (yyval.defalt) = newNode("FuncRParams");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        nodes[(yyval.defalt)].values = vector<Node*>();
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[0].defalt)]);
        nodes[(yyval.defalt)].value = 1;
    }
#line 3738 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1886 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "FuncRParams -> FuncRParams , Exp\n");
    (yyval.defalt) = newNode("FuncRParams");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode(","));
    addChild((yyval.defalt), (yyvsp[0].defalt));  

        nodes[(yyval.defalt)].values = nodes[(yyvsp[-2].defalt)].values;
        nodes[(yyval.defalt)].values.push_back(&nodes[(yyvsp[0].defalt)]);
        nodes[(yyval.defalt)].value = nodes[(yyval.defalt)].values.size();
}
#line 3754 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1898 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "MulExp -> UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 3766 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1905 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "MulExp -> MulExp * UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("*"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        if(nodes[(yyvsp[-2].defalt)].isConst && nodes[(yyvsp[0].defalt)].isConst){
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = nodes[(yyvsp[-2].defalt)].value * nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
            assemble.append("\timull\t%%r8d, %%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r9d, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3791 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1925 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "MulExp -> MulExp / UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("/"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        if(nodes[(yyvsp[-2].defalt)].isConst && nodes[(yyvsp[0].defalt)].isConst){
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = nodes[(yyvsp[-2].defalt)].value / nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%eax");
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
            assemble.append("\tcltd\n");
            assemble.append("\tidivl\t%%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%eax, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3817 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1946 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "MulExp -> MulExp %% UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("%"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        if(nodes[(yyvsp[-2].defalt)].isConst && nodes[(yyvsp[0].defalt)].isConst){
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = nodes[(yyvsp[-2].defalt)].value % nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%eax");
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
            assemble.append("\tcltd\n");
            assemble.append("\tidivl %%r9d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%edx, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3843 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1968 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "AddExp -> MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 3855 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1975 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "AddExp -> AddExp + MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("+"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        if(nodes[(yyvsp[-2].defalt)].isConst && nodes[(yyvsp[0].defalt)].isConst){
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = nodes[(yyvsp[-2].defalt)].value + nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
            assemble.append("\taddl\t%%r9d, %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3880 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1995 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "AddExp -> AddExp - MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("-"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        if(nodes[(yyvsp[-2].defalt)].isConst && nodes[(yyvsp[0].defalt)].isConst){
            nodes[(yyval.defalt)].isConst = true;
            nodes[(yyval.defalt)].value = nodes[(yyvsp[-2].defalt)].value - nodes[(yyvsp[0].defalt)].value;
        }else{
            assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
            assemble.append("\tsubl\t%%r9d, %%r8d\n");
            offset -= 4;
            assemble.append("\tsubq\t$4, %%rsp\n");
            assemble.append("\tmovl\t%%r8d, %d(%%rbp)\n", offset);
            nodes[(yyval.defalt)].offset = offset;
        }
    }
#line 3905 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 2019 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "RelExp -> AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    
        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
        // nodes[$$].quad = nodes[$1].quad;
    }
#line 3918 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 2027 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "RelExp -> RelExp < AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("<"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        nodes[(yyval.defalt)].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if <");
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjl\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);
    }
#line 3941 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 2045 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "RelExp -> RelExp > AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode(">"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        nodes[(yyval.defalt)].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if >");
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjg\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);
    }
#line 3964 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 2063 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "RelExp -> RelExp <= AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("<="));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 
        nodes[(yyval.defalt)].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if <=");
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjle\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);


    }
#line 3988 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 2082 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "RelExp -> RelExp >= AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode(">="));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 
        nodes[(yyval.defalt)].quad = assemble.newLabel();
        //nodes[$$].quad = nodes[$1].quad;
        assemble.comment("if >=");
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjge\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);
    }
#line 4010 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 2100 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "EqExp -> RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

    copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 4022 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 2107 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "EqExp -> EqExp == RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("=="));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        nodes[(yyval.defalt)].quad = assemble.newLabel();
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
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tje\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);
    
    }
#line 4057 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 2137 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "EqExp -> EqExp != RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("!="));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        nodes[(yyval.defalt)].quad = assemble.newLabel();
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
        assemble.var2reg(&nodes[(yyvsp[-2].defalt)], "%r8d");
        assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r9d");
        assemble.append("\tcmpl\t%%r9d, %%r8d\n");
        assemble.append("\tjne\t.L");
        nodes[(yyval.defalt)].trueList.push_back(assemble.line);
        assemble.append("\tjmp\t.L");
        nodes[(yyval.defalt)].falseList.push_back(assemble.line);
    }
#line 4090 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 2166 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LAndExp -> EqExp\n");
    (yyval.defalt) = newNode("LAndExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));

        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
        if(!(nodes[(yyval.defalt)].quad)){
        nodes[(yyval.defalt)].quad = assemble.newLabel();
        //    nodes[$$].quad = nodes[$1].quad;
            assemble.var2reg(&nodes[(yyvsp[0].defalt)], "%r8d");
            assemble.append("\tcmpl\t$0, %%r8d\n");
            assemble.append("\tjne\t.L");
            nodes[(yyval.defalt)].trueList.push_back(assemble.line);
            assemble.append("\tjmp\t.L");
            nodes[(yyval.defalt)].falseList.push_back(assemble.line);
        }
    }
#line 4112 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 2183 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LAndExp -> LAndExp && EqExp\n");
    (yyval.defalt) = newNode("LAndExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("&&"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 

        assemble.backpatch(nodes[(yyvsp[-2].defalt)].trueList, nodes[(yyvsp[0].defalt)].quad);
        nodes[(yyval.defalt)].trueList = nodes[(yyvsp[0].defalt)].trueList;
        nodes[(yyval.defalt)].falseList = merge(nodes[(yyvsp[-2].defalt)].falseList, nodes[(yyvsp[0].defalt)].falseList);
        nodes[(yyval.defalt)].quad = nodes[(yyvsp[-2].defalt)].quad;
    }
#line 4129 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 2196 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LOrExp -> LAndExp\n");
    (yyval.defalt) = newNode("LOrExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));   
        
        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
    }
#line 4141 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 2203 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "LOrExp -> LOrExp || LAndExp\n");
    (yyval.defalt) = newNode("LOrExp");
    addChild((yyval.defalt), (yyvsp[-2].defalt));      
    addChild((yyval.defalt), newNode("||"));
    addChild((yyval.defalt), (yyvsp[0].defalt)); 


        assemble.backpatch(nodes[(yyvsp[-2].defalt)].falseList, nodes[(yyvsp[0].defalt)].quad);
        nodes[(yyval.defalt)].trueList = merge(nodes[(yyvsp[-2].defalt)].trueList, nodes[(yyvsp[0].defalt)].trueList);
        nodes[(yyval.defalt)].falseList = nodes[(yyvsp[0].defalt)].falseList;
        nodes[(yyval.defalt)].quad = nodes[(yyvsp[-2].defalt)].quad;
    }
#line 4159 "parser.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 2217 "example.y" /* yacc.c:1646  */
    {
    fprintf(fDetail, "ConstExp -> AddExp\n");
    (yyval.defalt) = newNode("ConstExp");
    addChild((yyval.defalt), (yyvsp[0].defalt));
    
        copyNode(&nodes[(yyval.defalt)], &nodes[(yyvsp[0].defalt)]);
}
#line 4171 "parser.tab.cpp" /* yacc.c:1646  */
    break;


#line 4175 "parser.tab.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 2227 "example.y" /* yacc.c:1906  */

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
