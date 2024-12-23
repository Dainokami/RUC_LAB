/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "example.y"

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



/* Line 371 of yacc.c  */
#line 111 "example.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
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
   by #include "example.tab.h".  */
#ifndef YY_YY_EXAMPLE_TAB_H_INCLUDED
# define YY_YY_EXAMPLE_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONST = 258,
     INT = 259,
     VOID = 260,
     IF = 261,
     WHILE = 262,
     BREAK = 263,
     CONTINUE = 264,
     RETURN = 265,
     Ident = 266,
     WrongIdent = 267,
     INTCONST = 268,
     AND = 269,
     OR = 270,
     ERROR = 271,
     LOWER_THAN_ELSE = 272,
     ELSE = 273,
     LEQ = 274,
     GEQ = 275,
     NEQ = 276,
     EQ = 277
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 44 "example.y"

    char* val;
    int num;
    int defalt;


/* Line 387 of yacc.c  */
#line 183 "example.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_EXAMPLE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 211 "example.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   335

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNRULES -- Number of states.  */
#define YYNSTATES  234

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,     2,     2,    29,     2,     2,
      37,    38,    27,    25,    31,    26,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      20,    32,    19,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      15,    16,    17,    18,    21,    22,    23,    24
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    21,
      26,    28,    32,    36,    41,    45,    50,    54,    59,    61,
      64,    68,    70,    74,    78,    80,    84,    86,    89,    93,
      98,   100,   103,   107,   112,   116,   121,   123,   126,   130,
     132,   136,   142,   148,   155,   162,   169,   176,   182,   188,
     195,   202,   209,   216,   218,   222,   225,   230,   236,   239,
     244,   250,   254,   259,   262,   266,   268,   271,   273,   275,
     280,   283,   285,   287,   293,   301,   307,   310,   313,   317,
     320,   322,   324,   326,   329,   331,   334,   338,   340,   342,
     344,   348,   353,   357,   362,   365,   368,   371,   373,   377,
     379,   383,   387,   391,   393,   397,   401,   403,   407,   411,
     415,   419,   421,   425,   429,   431,   435,   437,   441
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    42,    -1,    41,    42,    -1,    43,    -1,
      56,    -1,    44,    -1,    50,    -1,     3,     4,    45,    30,
      -1,     3,     4,     1,    30,    -1,    46,    -1,    45,    31,
      46,    -1,    11,    32,    48,    -1,    11,    47,    32,    48,
      -1,    12,    32,    48,    -1,    12,    47,    32,    48,    -1,
      33,    76,    34,    -1,    47,    33,    76,    34,    -1,    76,
      -1,    35,    36,    -1,    35,    49,    36,    -1,    48,    -1,
      49,    31,    48,    -1,     4,    51,    30,    -1,    52,    -1,
      51,    31,    52,    -1,    11,    -1,    11,    53,    -1,    11,
      32,    54,    -1,    11,    53,    32,    54,    -1,    12,    -1,
      12,    53,    -1,    12,    32,    54,    -1,    12,    53,    32,
      54,    -1,    33,    76,    34,    -1,    53,    33,    76,    34,
      -1,    64,    -1,    35,    36,    -1,    35,    55,    36,    -1,
      54,    -1,    55,    31,    54,    -1,     5,    11,    37,    38,
      60,    -1,     4,    11,    37,    38,    60,    -1,     5,    11,
      37,    57,    38,    60,    -1,     4,    11,    37,    57,    38,
      60,    -1,     5,    11,    37,     1,    38,    60,    -1,     4,
      11,    37,     1,    38,    60,    -1,     5,    12,    37,    38,
      60,    -1,     4,    12,    37,    38,    60,    -1,     5,    12,
      37,    57,    38,    60,    -1,     4,    12,    37,    57,    38,
      60,    -1,     5,    12,    37,     1,    38,    60,    -1,     4,
      12,    37,     1,    38,    60,    -1,    58,    -1,    57,    31,
      58,    -1,     4,    11,    -1,     4,    11,    33,    34,    -1,
       4,    11,    33,    34,    59,    -1,     4,    12,    -1,     4,
      12,    33,    34,    -1,     4,    12,    33,    34,    59,    -1,
      33,    64,    34,    -1,    59,    33,    64,    34,    -1,    35,
      36,    -1,    35,    61,    36,    -1,    62,    -1,    61,    62,
      -1,    43,    -1,    63,    -1,    66,    32,    64,    30,    -1,
      64,    30,    -1,    30,    -1,    60,    -1,     6,    37,    65,
      38,    63,    -1,     6,    37,    65,    38,    63,    18,    63,
      -1,     7,    37,    65,    38,    63,    -1,     8,    30,    -1,
       9,    30,    -1,    10,    64,    30,    -1,    10,    30,    -1,
      71,    -1,    75,    -1,    11,    -1,    11,    59,    -1,    12,
      -1,    12,    59,    -1,    37,    64,    38,    -1,    66,    -1,
      13,    -1,    67,    -1,    11,    37,    38,    -1,    11,    37,
      69,    38,    -1,    12,    37,    38,    -1,    12,    37,    69,
      38,    -1,    25,    68,    -1,    26,    68,    -1,    39,    68,
      -1,    64,    -1,    69,    31,    64,    -1,    68,    -1,    70,
      27,    68,    -1,    70,    28,    68,    -1,    70,    29,    68,
      -1,    70,    -1,    71,    25,    70,    -1,    71,    26,    70,
      -1,    71,    -1,    72,    20,    71,    -1,    72,    19,    71,
      -1,    72,    22,    71,    -1,    72,    21,    71,    -1,    72,
      -1,    73,    24,    72,    -1,    73,    23,    72,    -1,    73,
      -1,    74,    14,    73,    -1,    74,    -1,    75,    15,    74,
      -1,    71,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    79,    79,    84,    91,    96,   102,   107,   113,   121,
     131,   136,   144,   151,   159,   166,   176,   183,   192,   197,
     203,   211,   216,   224,   232,   237,   245,   250,   256,   263,
     271,   276,   282,   289,   301,   308,   317,   322,   328,   336,
     341,   349,   358,   367,   377,   387,   397,   407,   416,   425,
     435,   445,   455,   467,   472,   480,   486,   494,   503,   509,
     517,   527,   534,   543,   549,   557,   562,   569,   574,   580,
     588,   594,   599,   604,   613,   624,   633,   639,   645,   652,
     659,   665,   671,   676,   682,   687,   694,   701,   706,   717,
     722,   729,   737,   744,   752,   758,   764,   771,   776,   784,
     789,   796,   803,   811,   816,   823,   831,   836,   843,   850,
     857,   865,   870,   877,   885,   890,   898,   903,   911
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONST", "INT", "VOID", "IF", "WHILE",
  "BREAK", "CONTINUE", "RETURN", "Ident", "WrongIdent", "INTCONST", "AND",
  "OR", "ERROR", "LOWER_THAN_ELSE", "ELSE", "'>'", "'<'", "LEQ", "GEQ",
  "NEQ", "EQ", "'+'", "'-'", "'*'", "'/'", "'%'", "';'", "','", "'='",
  "'['", "']'", "'{'", "'}'", "'('", "')'", "'!'", "$accept", "CompUnit",
  "OtherCompUnit", "Decl", "ConstDecl", "ConstDef", "OtherConstDef",
  "ConstArrIdex", "ConstInitVal", "OtherConstInitVal", "VarDecl", "VarDef",
  "OtherVarDef", "VarArrIdex", "InitVal", "OtherVarInitVal", "FuncDef",
  "FuncFParams", "FuncFParam", "LExp", "Block", "Blocks", "BlockItem",
  "Stmt", "Exp", "Cond", "LVal", "PrimaryExp", "UnaryExp", "FuncRParams",
  "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,    62,
      60,   274,   275,   276,   277,    43,    45,    42,    47,    37,
      59,    44,    61,    91,    93,   123,   125,    40,    41,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      45,    45,    46,    46,    46,    46,    47,    47,    48,    48,
      48,    49,    49,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    53,    53,    54,    54,    54,    55,
      55,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      64,    65,    66,    66,    66,    66,    67,    67,    67,    68,
      68,    68,    68,    68,    68,    68,    68,    69,    69,    70,
      70,    70,    70,    71,    71,    71,    72,    72,    72,    72,
      72,    73,    73,    73,    74,    74,    75,    75,    76
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     4,
       1,     3,     3,     4,     3,     4,     3,     4,     1,     2,
       3,     1,     3,     3,     1,     3,     1,     2,     3,     4,
       1,     2,     3,     4,     3,     4,     1,     2,     3,     1,
       3,     5,     5,     6,     6,     6,     6,     5,     5,     6,
       6,     6,     6,     1,     3,     2,     4,     5,     2,     4,
       5,     3,     4,     2,     3,     1,     2,     1,     1,     4,
       2,     1,     1,     5,     7,     5,     2,     2,     3,     2,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     1,
       3,     4,     3,     4,     2,     2,     2,     1,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
       0,    26,    30,     0,    24,     0,     0,     1,     3,     0,
       0,     0,     0,    10,     0,     0,     0,    27,     0,     0,
      31,    23,     0,     0,     0,     9,     0,     0,     0,     0,
       0,     8,     0,    82,    84,    88,     0,     0,     0,     0,
       0,    28,    36,    87,    89,    99,   103,    80,   118,     0,
       0,     0,     0,     0,    53,     0,     0,    32,     0,     0,
       0,     0,    26,    30,    25,     0,     0,     0,     0,     0,
       0,     0,    12,    18,     0,     0,     0,    14,     0,    11,
       0,     0,    83,     0,    85,    94,    95,    37,    39,     0,
       0,    96,     0,     0,     0,     0,     0,    34,     0,    55,
      58,     0,    42,     0,     0,    29,     0,     0,    48,     0,
      33,     0,    41,     0,     0,    47,     0,    19,    21,     0,
      16,    13,     0,    15,     0,    90,    97,     0,     0,    92,
       0,     0,    38,    86,   100,   101,   102,   104,   105,    46,
       0,     0,     0,     0,     0,     0,     0,     0,    71,    63,
      67,    72,     0,    65,    68,     0,    87,    54,    44,    35,
      52,    50,    45,    43,    51,    49,     0,    20,    17,    61,
       0,    91,     0,    93,    40,    56,    59,     0,     0,    76,
      77,    79,     0,    64,    66,    70,     0,    22,    98,    62,
      57,    60,     0,   106,   111,   114,   116,    81,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    73,   108,   107,   110,   109,   113,   112,   115,
     117,    75,     0,    74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    22,    23,    38,    82,   129,
       8,    13,    14,    27,    51,    99,     9,    63,    64,    92,
     161,   162,   163,   164,    52,   202,    53,    54,    55,   137,
      56,    57,   204,   205,   206,   207,    83
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const yytype_int16 yypact[] =
{
     155,     5,    70,    86,   123,  -116,  -116,  -116,  -116,  -116,
      39,    99,   120,    78,  -116,   -22,    -2,  -116,  -116,    24,
      97,   106,   168,  -116,   243,   284,    15,   170,   243,    28,
     172,  -116,   211,    33,    35,  -116,   246,   284,   196,   246,
     198,  -116,   224,   104,   128,  -116,   284,   284,   109,   284,
     284,  -116,  -116,  -116,  -116,  -116,   143,   216,   216,    42,
      57,   252,    71,   -21,  -116,   243,   284,  -116,    73,    71,
     -10,   243,   233,   244,  -116,    81,    71,    -7,   118,    71,
      61,   214,  -116,  -116,    90,   246,   284,  -116,   246,  -116,
     284,   249,   114,   278,   114,  -116,  -116,  -116,  -116,     2,
     138,  -116,   284,   284,   284,   284,   284,  -116,    71,   153,
     162,   171,  -116,   205,    71,  -116,   166,    71,  -116,    71,
    -116,    71,  -116,    71,    71,  -116,    71,  -116,  -116,    26,
    -116,  -116,   179,  -116,   190,  -116,  -116,    62,   284,  -116,
      63,   243,  -116,  -116,  -116,  -116,  -116,   143,   143,  -116,
     203,   212,   211,   215,   230,   202,   218,   281,  -116,  -116,
    -116,  -116,   208,  -116,  -116,   240,   241,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,   246,  -116,  -116,  -116,
     284,  -116,   245,  -116,  -116,   251,   251,   284,   284,  -116,
    -116,  -116,   272,  -116,  -116,  -116,   284,  -116,  -116,  -116,
     114,   114,   267,   216,   147,   275,   294,   299,   286,  -116,
     289,    77,   284,   284,   284,   284,   284,   284,   284,   284,
      77,  -116,   304,   216,   216,   216,   216,   147,   147,   275,
     294,  -116,    77,  -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -116,  -116,   321,  -104,  -116,  -116,   285,   305,   -36,  -116,
    -116,  -116,   296,    -4,   -23,  -116,  -116,   121,   217,   -43,
     -49,  -116,   167,  -115,   -47,   144,  -107,  -116,   -24,   238,
     195,   -25,    96,   115,   116,  -116,   -19
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      58,    94,   100,    87,   166,    67,    59,   160,    30,    10,
     113,    58,    58,   112,    58,    33,    60,   114,    84,    61,
     118,   113,    95,    96,   113,    98,   101,   122,   119,    68,
     125,   123,    61,   141,    75,    34,    78,    61,   142,    61,
      19,    58,   115,   134,   136,   128,   136,   116,   120,   131,
      20,    21,   133,    62,    35,   166,    58,   176,   160,   149,
      58,    58,   177,    58,   165,   168,    69,   132,   170,    30,
     171,    76,   172,    79,   173,   174,   107,   175,   144,   145,
     146,    11,    12,   153,   154,   155,   156,   157,    43,    44,
      45,   182,   113,   180,   180,   108,   222,    15,    16,   126,
     181,   183,    46,    47,   166,   231,   111,   158,    31,    32,
     192,   117,   111,   166,    49,   165,    50,   233,   184,   121,
      43,    44,    45,    17,   130,   166,     1,     2,     3,    36,
      37,    24,    25,   198,    46,    47,    26,    90,    39,    37,
     197,    91,   200,   201,    48,    97,    49,   138,    50,   210,
      70,    58,    28,    25,    77,    80,   124,    29,     1,     2,
       3,    90,   203,   203,   165,    93,   212,   213,   214,   215,
     102,   103,   104,   165,     1,   152,   143,   153,   154,   155,
     156,   157,    43,    44,    45,   165,   150,   223,   224,   225,
     226,   203,   203,   203,   203,   151,    46,    47,    41,    42,
     169,   158,    65,    66,    71,    66,   111,   159,    49,    61,
      50,     1,   152,   178,   153,   154,   155,   156,   157,    43,
      44,    45,    72,    73,   179,    43,    44,    45,    85,    86,
      88,    86,   189,    46,    47,    20,    21,   185,   158,    46,
      47,   105,   106,   111,   193,    49,   186,    50,   190,    81,
     127,    49,   187,    50,    43,    44,    45,    43,    44,    45,
      43,    44,    45,   109,   110,    24,    25,   188,    46,    47,
     195,    46,    47,   196,    46,    47,    28,    25,    48,   199,
      49,    81,    50,    49,    90,    50,    49,   135,    50,    43,
      44,    45,    43,    44,    45,    43,    44,    45,   216,   217,
     147,   148,   209,    46,    47,   211,    46,    47,   218,    46,
      47,   191,   227,   228,   219,    49,   139,    50,    49,   221,
      50,    49,   232,    50,   220,    18,    40,    89,    74,   194,
     167,   140,   208,   229,     0,   230
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-116)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      25,    44,    49,    39,   111,    28,    25,   111,    12,     4,
      31,    36,    37,    62,    39,    37,     1,    38,    37,     4,
      69,    31,    46,    47,    31,    48,    50,    76,    38,     1,
      79,    38,     4,    31,     1,    37,     1,     4,    36,     4,
       1,    66,    65,    90,    91,    81,    93,    66,    71,    85,
      11,    12,    88,    38,    30,   162,    81,    31,   162,   108,
      85,    86,    36,    88,   111,   114,    38,    86,   117,    73,
     119,    38,   121,    38,   123,   124,    34,   126,   102,   103,
     104,    11,    12,     6,     7,     8,     9,    10,    11,    12,
      13,   138,    31,    31,    31,    38,   211,    11,    12,    38,
      38,    38,    25,    26,   211,   220,    35,    30,    30,    31,
     157,    38,    35,   220,    37,   162,    39,   232,   141,    38,
      11,    12,    13,     0,    34,   232,     3,     4,     5,    32,
      33,    32,    33,   180,    25,    26,    37,    33,    32,    33,
     176,    37,   185,   186,    35,    36,    37,    33,    39,   196,
      29,   176,    32,    33,    33,    34,    38,    37,     3,     4,
       5,    33,   187,   188,   211,    37,    19,    20,    21,    22,
      27,    28,    29,   220,     3,     4,    38,     6,     7,     8,
       9,    10,    11,    12,    13,   232,    33,   212,   213,   214,
     215,   216,   217,   218,   219,    33,    25,    26,    30,    31,
      34,    30,    32,    33,    32,    33,    35,    36,    37,     4,
      39,     3,     4,    34,     6,     7,     8,     9,    10,    11,
      12,    13,    11,    12,    34,    11,    12,    13,    32,    33,
      32,    33,    30,    25,    26,    11,    12,    34,    30,    25,
      26,    25,    26,    35,    36,    37,    34,    39,    30,    35,
      36,    37,    37,    39,    11,    12,    13,    11,    12,    13,
      11,    12,    13,    11,    12,    32,    33,    37,    25,    26,
      30,    25,    26,    32,    25,    26,    32,    33,    35,    34,
      37,    35,    39,    37,    33,    39,    37,    38,    39,    11,
      12,    13,    11,    12,    13,    11,    12,    13,    23,    24,
     105,   106,    30,    25,    26,    38,    25,    26,    14,    25,
      26,    30,   216,   217,    15,    37,    38,    39,    37,    30,
      39,    37,    18,    39,    38,     4,    21,    42,    32,   162,
     113,    93,   188,   218,    -1,   219
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    41,    42,    43,    44,    50,    56,
       4,    11,    12,    51,    52,    11,    12,     0,    42,     1,
      11,    12,    45,    46,    32,    33,    37,    53,    32,    37,
      53,    30,    31,    37,    37,    30,    32,    33,    47,    32,
      47,    30,    31,    11,    12,    13,    25,    26,    35,    37,
      39,    54,    64,    66,    67,    68,    70,    71,    71,    76,
       1,     4,    38,    57,    58,    32,    33,    54,     1,    38,
      57,    32,    11,    12,    52,     1,    38,    57,     1,    38,
      57,    35,    48,    76,    76,    32,    33,    48,    32,    46,
      33,    37,    59,    37,    59,    68,    68,    36,    54,    55,
      64,    68,    27,    28,    29,    25,    26,    34,    38,    11,
      12,    35,    60,    31,    38,    54,    76,    38,    60,    38,
      54,    38,    60,    38,    38,    60,    38,    36,    48,    49,
      34,    48,    76,    48,    64,    38,    64,    69,    33,    38,
      69,    31,    36,    38,    68,    68,    68,    70,    70,    60,
      33,    33,     4,     6,     7,     8,     9,    10,    30,    36,
      43,    60,    61,    62,    63,    64,    66,    58,    60,    34,
      60,    60,    60,    60,    60,    60,    31,    36,    34,    34,
      31,    38,    64,    38,    54,    34,    34,    37,    37,    30,
      30,    30,    64,    36,    62,    30,    32,    48,    64,    34,
      59,    59,    65,    71,    72,    73,    74,    75,    65,    30,
      64,    38,    19,    20,    21,    22,    23,    24,    14,    15,
      38,    30,    63,    71,    71,    71,    71,    72,    72,    73,
      74,    63,    18,    63
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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
      yychar = YYLEX;
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
     `$$ = $1'.

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
/* Line 1792 of yacc.c  */
#line 79 "example.y"
    {
    fprintf(fDetail, "CompUnits -> OtherCompUnit\n");
    (yyval.defalt) = newNode("CompUnit");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 84 "example.y"
    {    
    fprintf(fDetail, "CompUnits -> CompUnit OtherCompUnit\n");
    (yyval.defalt) = newNode("CompUnit");
    addChild((yyval.defalt), (yyvsp[(1) - (2)].defalt));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 91 "example.y"
    {
    fprintf(fDetail, "OtherCompUnit -> Decl\n");
    (yyval.defalt) = newNode("OtherCompUnit");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 96 "example.y"
    {
    fprintf(fDetail, "OtherCompUnit -> FuncDef\n");
    (yyval.defalt) = newNode("OtherCompUnit");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 102 "example.y"
    {
    fprintf(fDetail, "Decl -> ConstDecl\n");
    (yyval.defalt) = newNode("Decl");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 107 "example.y"
    {
    fprintf(fDetail, "Decl -> VarDecl\n");
    (yyval.defalt) = newNode("Decl");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 113 "example.y"
    {
    fprintf(fDetail, "ConstDecl -> const int ConstDef ;\n");
    (yyval.defalt) = newNode("ConstDecl");
    addChild((yyval.defalt), newNode("const"));
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 121 "example.y"
    {
    fprintf(fDetail, "ConstDef -> CONST INT error ;\n");
    printf("const definition error\n");
    (yyval.defalt) = newNode("ConstDecl");
    addChild((yyval.defalt), newNode("const"));
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 131 "example.y"
    {
    fprintf(fDetail, "ConstDef -> OtherConstDef\n");
    (yyval.defalt) = newNode("ConstDef");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 136 "example.y"
    {
    fprintf(fDetail, "ConstDef -> ConstDef , OtherConstDef\n");
    (yyval.defalt) = newNode("ConstDef");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 144 "example.y"
    {
    fprintf(fDetail, "OtherConstDef -> Ident = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (3)].val)));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 151 "example.y"
    {
    fprintf(fDetail, "OtherConstDef -> Ident ConstArrIdex = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (4)].val)));
    addChild((yyval.defalt), (yyvsp[(2) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(4) - (4)].defalt));
    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 159 "example.y"
    {
    fprintf(fDetail, "OtherConstDef -> WrongIdent = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 166 "example.y"
    {
    fprintf(fDetail, "OtherConstDef -> WrongIdent ConstArrIdex = ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[(2) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(4) - (4)].defalt));
    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 176 "example.y"
    {
    fprintf(fDetail, "ConstArrIdex -> [ ConstExp ]\n");
    (yyval.defalt) = newNode("ConstArrIdex");
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 183 "example.y"
    {
    fprintf(fDetail, "ConstArrIdex -> ConstArrIdex [ ConstExp ]\n");
    (yyval.defalt) = newNode("ConstArrIdex");
    addChild((yyval.defalt), (yyvsp[(1) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 192 "example.y"
    {
    fprintf(fDetail, "ConstInitVal -> ConstExp\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 197 "example.y"
    {
    fprintf(fDetail, "ConstInitVal -> { }\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 203 "example.y"
    {
    fprintf(fDetail, "ConstInitVal -> { OtherConstInitVal }\n");
    (yyval.defalt) = newNode("ConstInitVal");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 211 "example.y"
    {
    fprintf(fDetail, "OtherConstInitVal -> ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstInitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 216 "example.y"
    {
    fprintf(fDetail, "OtherConstInitVal -> OtherConstInitVal , ConstInitVal\n");
    (yyval.defalt) = newNode("OtherConstInitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 224 "example.y"
    {
    fprintf(fDetail, "VarDecl -> int VarDef ;\n");
    (yyval.defalt) = newNode("VarDecl");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 232 "example.y"
    {
    fprintf(fDetail, "VarDef -> OtherVarDef\n");
    (yyval.defalt) = newNode("VarDef");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 237 "example.y"
    {
    fprintf(fDetail, "VarDef -> VarDef , OtherVarDef\n");
    (yyval.defalt) = newNode("VarDef");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 245 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> Ident\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (1)].val)));
    }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 250 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> Ident  VarArrIdex\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (2)].val)));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 256 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> Ident = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (3)].val)));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 263 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> Ident VarArrIdex = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (4)].val)));
    addChild((yyval.defalt), (yyvsp[(2) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(4) - (4)].defalt));
    }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 271 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 276 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent  VarArrIdex\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 282 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 289 "example.y"
    {
    fprintf(fDetail, "OtherVarDef -> WrongIdent VarArrIdex = InitVal\n");
    (yyval.defalt) = newNode("OtherVarDef");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[(2) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(4) - (4)].defalt));
    }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 301 "example.y"
    {
    fprintf(fDetail, "VarArrIdex -> [ ConstExp ]\n");
    (yyval.defalt) = newNode("VarArrIdex");
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 308 "example.y"
    {
    fprintf(fDetail, "VarArrIdex -> VarArrIdex [ ConstExp ]\n");
    (yyval.defalt) = newNode("VarArrIdex");
    addChild((yyval.defalt), (yyvsp[(1) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 317 "example.y"
    {
    fprintf(fDetail, "InitVal -> Exp\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 322 "example.y"
    {
    fprintf(fDetail, "InitVal -> { }\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 328 "example.y"
    {
    fprintf(fDetail, "InitVal -> { OtherVarInitVal }\n");
    (yyval.defalt) = newNode("InitVal");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 336 "example.y"
    {
    fprintf(fDetail, "OtherVarInitVal -> InitVal\n");
    (yyval.defalt) = newNode("OtherVarInitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 341 "example.y"
    {
    fprintf(fDetail, "OtherVarInitVal -> OtherVarInitVal , InitVal\n");
    (yyval.defalt) = newNode("OtherVarInitVal");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 349 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void Ident ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (5)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 358 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int Ident ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (5)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 367 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void Ident ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (6)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(4) - (6)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 377 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int Ident ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (6)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(4) - (6)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 387 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void Ident ( ERROR ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (6)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 397 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int Ident ( ERROR ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (6)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 407 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void WrongIdent ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 416 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int WrongIdent ( ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 425 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void WrongIdent ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(4) - (6)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 435 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int WrongIdent ( FuncFParams ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(4) - (6)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 445 "example.y"
    {
    fprintf(fDetail, "FuncDef -> void WrongIdent ( ERROR ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("void"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 455 "example.y"
    {
    fprintf(fDetail, "FuncDef -> int WrongIdent ( ERROR ) Block\n");
    (yyval.defalt) = newNode("FuncDef");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("ERROR"));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(6) - (6)].defalt));
    }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 467 "example.y"
    {
    fprintf(fDetail, "FuncFParams -> FuncFParam\n");
    (yyval.defalt) = newNode("FuncFParams");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 472 "example.y"
    {
    fprintf(fDetail, "FuncFParams -> FuncFParams , FuncFParam\n");
    (yyval.defalt) = newNode("FuncFParams");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt)); 
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));
    }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 480 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int Ident\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (2)].val)));
    }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 486 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int Ident [ ]\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (4)].val)));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 494 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int Ident [ ] LExp\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode((yyvsp[(2) - (5)].val)));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), newNode("\\]"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 503 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 509 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ]\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 517 "example.y"
    {
    fprintf(fDetail, "FuncFParam -> int WrongIdent [ ] LExp\n");
    (yyval.defalt) = newNode("FuncFParam");
    addChild((yyval.defalt), newNode("int"));
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), newNode("\\]"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 527 "example.y"
    {
    fprintf(fDetail, "LExp -> [ Exp ]\n");
    (yyval.defalt) = newNode("LExp");
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 534 "example.y"
    {
    fprintf(fDetail, "LExp -> LExp [ Exp ]\n");
    (yyval.defalt) = newNode("LExp");
    addChild((yyval.defalt), (yyvsp[(1) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\["));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\]"));
    }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 543 "example.y"
    {
    fprintf(fDetail, "Block -> { }\n");
    (yyval.defalt) = newNode("Block");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 549 "example.y"
    {
    fprintf(fDetail, "Block -> { Blocks }\n");
    (yyval.defalt) = newNode("Block");
    addChild((yyval.defalt), newNode("\\{"));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\}"));
    }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 557 "example.y"
    {
    fprintf(fDetail, "Blocks -> BlockItem\n");
    (yyval.defalt) = newNode("Blocks");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 562 "example.y"
    {
    fprintf(fDetail, "Blocks -> Blocks BlockItem\n");
    (yyval.defalt) = newNode("Blocks");
    addChild((yyval.defalt), (yyvsp[(1) - (2)].defalt));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 569 "example.y"
    {
    fprintf(fDetail, "BlockItem -> Decl\n");
    (yyval.defalt) = newNode("BlockItem");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 574 "example.y"
    {
    fprintf(fDetail, "BlockItem -> Stmt\n");
    (yyval.defalt) = newNode("BlockItem");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 580 "example.y"
    {
    fprintf(fDetail, "Stmt -> LVal = Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[(1) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 588 "example.y"
    {
    fprintf(fDetail, "Stmt -> Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[(1) - (2)].defalt));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 594 "example.y"
    {
    fprintf(fDetail, "Stmt -> ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 599 "example.y"
    {
    fprintf(fDetail, "Stmt -> Block\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 604 "example.y"
    {
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("if"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(3) - (5)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 613 "example.y"
    {
    fprintf(fDetail, "Stmt -> if ( Cond ) Stmt else Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("if"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(3) - (7)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (7)].defalt));
    addChild((yyval.defalt), newNode("else"));
    addChild((yyval.defalt), (yyvsp[(7) - (7)].defalt));
    }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 624 "example.y"
    {
    fprintf(fDetail, "Stmt -> while ( Cond ) Stmt\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("while"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(3) - (5)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    addChild((yyval.defalt), (yyvsp[(5) - (5)].defalt));
    }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 633 "example.y"
    {
    fprintf(fDetail, "Stmt -> break ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("break"));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 639 "example.y"
    {
    fprintf(fDetail, "Stmt -> continue ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("continue"));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 645 "example.y"
    {
    fprintf(fDetail, "Stmt -> return Exp ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("return"));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 652 "example.y"
    {
    fprintf(fDetail, "Stmt -> return ;\n");
    (yyval.defalt) = newNode("Stmt");
    addChild((yyval.defalt), newNode("return"));
    addChild((yyval.defalt), newNode("\\;"));
    }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 659 "example.y"
    {
    fprintf(fDetail, "Exp -> AddExp\n");
    (yyval.defalt) = newNode("Exp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 665 "example.y"
    {
    fprintf(fDetail, "Cond -> LOrExp\n");
    (yyval.defalt) = newNode("Cond");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 671 "example.y"
    {
    fprintf(fDetail, "LVal -> Ident\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (1)].val)));
    }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 676 "example.y"
    {
    fprintf(fDetail, "LVal -> Ident LExp\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (2)].val)));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 682 "example.y"
    {
    fprintf(fDetail, "LVal -> WrongIdent\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode("WrongIdent"));
    }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 687 "example.y"
    {
    fprintf(fDetail, "LVal -> WrongIdent LExp\n");
    (yyval.defalt) = newNode("LVal");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 694 "example.y"
    {
    fprintf(fDetail, "PrimaryExp -> ( Exp )\n");
    (yyval.defalt) = newNode("PrimaryExp");
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(2) - (3)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 701 "example.y"
    {
    fprintf(fDetail, "PrimaryExp -> LVal\n");
    (yyval.defalt) = newNode("PrimaryExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 706 "example.y"
    {
    fprintf(fDetail, "PrimaryExp -> IntConst\n");
    (yyval.defalt) = newNode("PrimaryExp");
    char* buffer = malloc(20); 
    if (buffer) {
        sprintf(buffer, "%d", (yyvsp[(1) - (1)].num));
    }
    addChild((yyval.defalt), newNode(buffer)); 
    free(buffer);                  
    }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 717 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> PrimaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 722 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> Ident ( )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (3)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 729 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> Ident ( FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode((yyvsp[(1) - (4)].val)));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 737 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> WrongIdent ( )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), newNode("\\)"));
    }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 744 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> WrongIdent ( FuncRParams )\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("WrongIdent"));
    addChild((yyval.defalt), newNode("\\("));
    addChild((yyval.defalt), (yyvsp[(3) - (4)].defalt));
    addChild((yyval.defalt), newNode("\\)"));
    }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 752 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> + UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("\\+"));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 758 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> - UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("\\-"));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 764 "example.y"
    {
    fprintf(fDetail, "UnaryExp -> ! UnaryExp\n");
    (yyval.defalt) = newNode("UnaryExp");
    addChild((yyval.defalt), newNode("\\!"));
    addChild((yyval.defalt), (yyvsp[(2) - (2)].defalt));
    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 771 "example.y"
    {
    fprintf(fDetail, "FuncRParams -> Exp\n");
    (yyval.defalt) = newNode("FuncRParams");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 776 "example.y"
    {
    fprintf(fDetail, "FuncRParams -> FuncRParams , Exp\n");
    (yyval.defalt) = newNode("FuncRParams");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\,"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt));  
    }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 784 "example.y"
    {
    fprintf(fDetail, "MulExp -> UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 789 "example.y"
    {
    fprintf(fDetail, "MulExp -> MulExp * UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\*"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 796 "example.y"
    {
    fprintf(fDetail, "MulExp -> MulExp / UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\/"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 803 "example.y"
    {
    fprintf(fDetail, "MulExp -> MulExp % UnaryExp\n");
    (yyval.defalt) = newNode("MulExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\%"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 811 "example.y"
    {
    fprintf(fDetail, "AddExp -> MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 816 "example.y"
    {
    fprintf(fDetail, "AddExp -> AddExp + MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\+"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 823 "example.y"
    {
    fprintf(fDetail, "AddExp -> AddExp - MulExp\n");
    (yyval.defalt) = newNode("AddExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\-"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 831 "example.y"
    {
    fprintf(fDetail, "RelExp -> AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 836 "example.y"
    {
    fprintf(fDetail, "RelExp -> RelExp < AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\<"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 843 "example.y"
    {
    fprintf(fDetail, "RelExp -> RelExp > AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\>"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 850 "example.y"
    {
    fprintf(fDetail, "RelExp -> RelExp <= AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\<\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 857 "example.y"
    {
    fprintf(fDetail, "RelExp -> RelExp >= AddExp\n");
    (yyval.defalt) = newNode("RelExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\>\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 865 "example.y"
    {
    fprintf(fDetail, "EqExp -> RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 870 "example.y"
    {
    fprintf(fDetail, "EqExp -> EqExp == RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\=\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 877 "example.y"
    {
    fprintf(fDetail, "EqExp -> EqExp != RelExp\n");
    (yyval.defalt) = newNode("EqExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\!\\="));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 885 "example.y"
    {
    fprintf(fDetail, "LAndExp -> EqExp\n");
    (yyval.defalt) = newNode("LAndExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
    }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 890 "example.y"
    {
    fprintf(fDetail, "LAndExp -> LAndExp && EqExp\n");
    (yyval.defalt) = newNode("LAndExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\&\\&"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 898 "example.y"
    {
    fprintf(fDetail, "LOrExp -> LAndExp\n");
    (yyval.defalt) = newNode("LOrExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));   
    }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 903 "example.y"
    {
    fprintf(fDetail, "LOrExp -> LOrExp || LAndExp\n");
    (yyval.defalt) = newNode("LOrExp");
    addChild((yyval.defalt), (yyvsp[(1) - (3)].defalt));      
    addChild((yyval.defalt), newNode("\\|\\|"));
    addChild((yyval.defalt), (yyvsp[(3) - (3)].defalt)); 
    }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 911 "example.y"
    {
    fprintf(fDetail, "ConstExp -> AddExp\n");
    (yyval.defalt) = newNode("ConstExp");
    addChild((yyval.defalt), (yyvsp[(1) - (1)].defalt));
}
    break;


/* Line 1792 of yacc.c  */
#line 3011 "example.tab.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 919 "example.y"

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