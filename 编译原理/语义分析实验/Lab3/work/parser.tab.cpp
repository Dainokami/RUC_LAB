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
#line 1 "example1.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <bits/stdc++.h>
    using namespace std;
    int yylex(void);
    void yyerror(const char *s);
    FILE *f;
    extern FILE *yyin; 
    extern int lineno;
    // int yydebug=1;

enum Type {Int, Constint, Arr, Fint, Fvoid};
struct Var{
	Type type;
	int val, offset;
	vector<int> dim;
	Var() {}
	Var(Type _type, int _val) : type(_type), val(_val){}
	Var(Type _type, int _val, int _offset) : type(_type), val(_val), offset(_offset){}
	Var(Type _type, int _val, int _offset, vector<int> _dim) : type(_type), val(_val), offset(_offset), dim(_dim){}
};

int tot = 0, num = 0, offset = 0, level = 0;
bool is_func_void = false;
char tag[114514][100], funcname[114];

vector<int> p_offset, ArrInitVal;
vector<string> Assemble, Para;
vector<map<string, Var> > symbol;
vector<vector<pair <int, int> > > breaklist, continuelist; 


struct State{
	int val, offset, offset_of_arr, quad;
	bool is_const, is_arr;
	string name;
	vector<bool> para_is_const, para_is_arr;
	vector<int> truelist, falselist, dim, para_val, para_offset, para_offset_of_arr;
	vector<string> para_name;
}sta[114514];


vector<int> merge(const vector<int> &a, const vector<int> &b){
	vector<int> res;
	for(auto x : a) res.push_back(x);
	for(auto x : b) res.push_back(x);
	return res;
}

void backpatch(vector<int> &a, int pos){
	for(auto x : a) Assemble[x] += ".L" + to_string(pos) + "\n";
}

void var2reg(int x, const char* reg){
	char tmp[114];
	if(sta[x].is_const){
		sprintf(tmp, "\tmovl\t$%d, %%%s\n", sta[x].val, reg); Assemble.push_back(tmp);
	}
	else if(sta[x].offset == 1){
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].name.c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%s(%rip), %%%s\n", sta[x].name.c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].offset, reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tmovl\t%d(%rbp), %%%s\n", sta[x].offset, reg); Assemble.push_back(tmp);
		}
	}
}

void reg2var(const char* reg, int x){
	char tmp[114];
	if(sta[x].is_const){
		yyerror("Const Error");
		exit(0);
	}
	if(sta[x].offset == 1){
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].name.c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%%%s, (%rdx, %rbx)\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%%%s, %s(%rip)\n",  reg, sta[x].name.c_str()); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%%%s, %d(%rbp, %rbx, 4)\n", reg, sta[x].offset); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%%%s, %d(%rbp)\n", reg, sta[x].offset); Assemble.push_back(tmp);
		}
	}
}

void para2reg(int x, int pos, const char *reg){
	char tmp[114];
	if(sta[x].para_is_const[pos]){
		sprintf(tmp, "\tmovl\t$%d, %%%s\n", sta[x].para_val[pos], reg); Assemble.push_back(tmp);
	}
	else if(sta[x].para_offset[pos] == 1){
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].para_name[pos].c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%s(%rip), %%%s\n", sta[x].para_name[pos].c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tmovl\t%d(%rbp), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
		}
	}
}

void lea_para2reg(int x, int pos, const char *reg){
	char tmp[114];
	if(sta[x].para_is_const[pos]){
		yyerror("Const Error");
		exit(0);
	}
	if(sta[x].para_offset[pos] == 1){
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].para_name[pos].c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tleaq\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tleaq\t%s(%rip), %%%s\n", sta[x].para_name[pos].c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tleaq\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tleaq\t%d(%rbp), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
		}
	}
}

void call_func_push(){
	Assemble.push_back("\tpushq\t%rbp\n");
	Assemble.push_back("\tpushq\t%r8\n");
	Assemble.push_back("\tpushq\t%r9\n");
	Assemble.push_back("\tmovq\t%rsp, %rbp\n");
}

void ret_func_pop(){
	Assemble.push_back("\tpopq\t%r9\n");
	Assemble.push_back("\tpopq\t%r8\n");
	Assemble.push_back("\tpopq\t%rbp\n");
	Assemble.push_back("\tret\n");
}



/* Line 371 of yacc.c  */
#line 261 "parser.tab.cpp"

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
   by #include "parser.tab.hpp".  */
#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
     PRINTF = 266,
     SCANF = 267,
     MAIN = 268,
     Ident = 269,
     WrongIdent = 270,
     STRING = 271,
     INTCONST = 272,
     AND = 273,
     OR = 274,
     ERROR = 275,
     LOWER_THAN_ELSE = 276,
     ELSE = 277,
     LEQ = 278,
     GEQ = 279,
     NEQ = 280,
     EQ = 281
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 194 "example1.y"

    char* val;
    int num;
    int defalt;


/* Line 387 of yacc.c  */
#line 337 "parser.tab.cpp"
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

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 365 "parser.tab.cpp"

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
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNRULES -- Number of states.  */
#define YYNSTATES  256

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,     2,     2,    33,    44,     2,
      41,    42,    31,    29,    35,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    34,
      24,    36,    23,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,    40,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    25,    26,
      27,    28
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    21,
      26,    28,    32,    37,    42,    43,    48,    50,    53,    57,
      59,    63,    67,    69,    73,    76,    81,    84,    89,    91,
      94,    98,   100,   104,   111,   118,   126,   134,   141,   148,
     155,   161,   167,   174,   181,   188,   195,   197,   201,   203,
     204,   205,   206,   207,   208,   212,   216,   217,   221,   222,
     227,   230,   234,   236,   239,   241,   243,   248,   251,   253,
     255,   264,   280,   291,   294,   297,   301,   304,   305,   306,
     307,   308,   309,   310,   312,   314,   317,   320,   324,   326,
     328,   330,   334,   339,   346,   353,   357,   362,   365,   368,
     371,   373,   377,   380,   385,   387,   391,   395,   399,   401,
     405,   409,   411,   415,   419,   423,   427,   429,   433,   437,
     439,   443,   445,   449
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    47,    -1,    46,    47,    -1,    48,    -1,
      60,    -1,    49,    -1,    55,    -1,     3,     4,    50,    34,
      -1,     3,     4,     1,    34,    -1,    51,    -1,    50,    35,
      51,    -1,    14,    52,    36,    53,    -1,    15,    52,    36,
      53,    -1,    -1,    37,    94,    38,    52,    -1,    94,    -1,
      39,    40,    -1,    39,    54,    40,    -1,    53,    -1,    54,
      35,    53,    -1,     4,    56,    34,    -1,    57,    -1,    56,
      35,    57,    -1,    14,    52,    -1,    14,    52,    36,    58,
      -1,    15,    52,    -1,    15,    52,    36,    58,    -1,    81,
      -1,    39,    40,    -1,    39,    59,    40,    -1,    58,    -1,
      59,    35,    58,    -1,     5,    62,    41,    42,    64,    71,
      -1,     4,    62,    41,    42,    65,    71,    -1,     5,    62,
      41,    61,    42,    66,    71,    -1,     4,    62,    41,    61,
      42,    67,    71,    -1,     4,    13,    41,    42,    63,    71,
      -1,     5,    62,    41,     1,    42,    71,    -1,     4,    62,
      41,     1,    42,    71,    -1,     5,    15,    41,    42,    71,
      -1,     4,    15,    41,    42,    71,    -1,     5,    15,    41,
      61,    42,    71,    -1,     4,    15,    41,    61,    42,    71,
      -1,     5,    15,    41,     1,    42,    71,    -1,     4,    15,
      41,     1,    42,    71,    -1,    68,    -1,    61,    35,    68,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,     4,    14,
      69,    -1,     4,    15,    69,    -1,    -1,    37,    38,    70,
      -1,    -1,    37,    81,    38,    70,    -1,    39,    40,    -1,
      39,    72,    40,    -1,    73,    -1,    72,    73,    -1,    48,
      -1,    74,    -1,    83,    36,    81,    34,    -1,    81,    34,
      -1,    34,    -1,    71,    -1,     6,    41,    82,    42,    75,
      76,    74,    77,    -1,     6,    41,    82,    42,    75,    76,
      74,    77,    22,    78,    75,    76,    74,    77,    75,    -1,
       7,    79,    76,    41,    82,    42,    80,    75,    74,    77,
      -1,     8,    34,    -1,     9,    34,    -1,    10,    81,    34,
      -1,    10,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    93,    -1,    14,    70,    -1,    15,    70,    -1,
      41,    81,    42,    -1,    83,    -1,    17,    -1,    84,    -1,
      14,    41,    42,    -1,    14,    41,    86,    42,    -1,    12,
      41,    16,    35,    87,    42,    -1,    11,    41,    16,    35,
      86,    42,    -1,    15,    41,    42,    -1,    15,    41,    86,
      42,    -1,    29,    85,    -1,    30,    85,    -1,    43,    85,
      -1,    81,    -1,    86,    35,    81,    -1,    44,    81,    -1,
      87,    35,    44,    81,    -1,    85,    -1,    88,    31,    85,
      -1,    88,    32,    85,    -1,    88,    33,    85,    -1,    88,
      -1,    89,    29,    88,    -1,    89,    30,    88,    -1,    89,
      -1,    90,    24,    89,    -1,    90,    23,    89,    -1,    90,
      26,    89,    -1,    90,    25,    89,    -1,    90,    -1,    91,
      28,    90,    -1,    91,    27,    90,    -1,    91,    -1,    92,
      18,    91,    -1,    92,    -1,    93,    19,    92,    -1,    89,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   229,   229,   231,   234,   236,   239,   241,   244,   246,
     249,   251,   255,   290,   294,   295,   303,   307,   309,   312,
     314,   317,   320,   322,   325,   368,   447,   449,   454,   457,
     460,   464,   471,   479,   488,   497,   506,   515,   523,   525,
     527,   529,   531,   533,   535,   537,   542,   544,   547,   553,
     564,   583,   602,   628,   654,   657,   661,   662,   668,   669,
     691,   694,   698,   701,   705,   708,   712,   721,   724,   727,
     730,   740,   749,   772,   778,   784,   795,   805,   812,   826,
     835,   841,   851,   865,   869,   873,   932,   936,   939,   942,
     955,   958,   979,  1040,  1043,  1046,  1049,  1052,  1056,  1069,
    1083,  1095,  1108,  1111,  1114,  1117,  1132,  1148,  1165,  1169,
    1184,  1200,  1204,  1218,  1232,  1246,  1261,  1264,  1278,  1293,
    1309,  1317,  1321,  1328
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CONST", "INT", "VOID", "IF", "WHILE",
  "BREAK", "CONTINUE", "RETURN", "PRINTF", "SCANF", "MAIN", "Ident",
  "WrongIdent", "STRING", "INTCONST", "AND", "OR", "ERROR",
  "LOWER_THAN_ELSE", "ELSE", "'>'", "'<'", "LEQ", "GEQ", "NEQ", "EQ",
  "'+'", "'-'", "'*'", "'/'", "'%'", "';'", "','", "'='", "'['", "']'",
  "'{'", "'}'", "'('", "')'", "'!'", "'&'", "$accept", "CompUnit",
  "OtherCompUnit", "Decl", "ConstDecl", "ConstDef", "OtherConstDef",
  "ConstArrIdex", "ConstInitVal", "OtherConstInitVal", "VarDecl", "VarDef",
  "OtherVarDef", "InitVal", "OtherVarInitVal", "FuncDef", "FuncFParams",
  "FName", "Entry_Main", "Entry_Void", "Entry_Int", "Entry_Void_Para",
  "Entry_Int_Para", "FuncFParam", "ParaArr", "Arr", "Block", "Blocks",
  "BlockItem", "Stmt", "NewLabel", "BeforeStmt", "AfterStmt", "AfterElse",
  "WhileBegin", "WhileEnd", "Exp", "Cond", "LVal", "PrimaryExp",
  "UnaryExp", "FuncRParams", "PointParams", "MulExp", "AddExp", "RelExp",
  "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    62,    60,   278,   279,   280,   281,    43,
      45,    42,    47,    37,    59,    44,    61,    91,    93,   123,
     125,    40,    41,    33,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    46,    47,    47,    48,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    53,    54,
      54,    55,    56,    56,    57,    57,    57,    57,    58,    58,
      58,    59,    59,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    61,    62,    63,
      64,    65,    66,    67,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    83,    84,    84,    84,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      86,    86,    87,    87,    88,    88,    88,    88,    89,    89,
      89,    90,    90,    90,    90,    90,    91,    91,    91,    92,
      92,    93,    93,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     4,
       1,     3,     4,     4,     0,     4,     1,     2,     3,     1,
       3,     3,     1,     3,     2,     4,     2,     4,     1,     2,
       3,     1,     3,     6,     6,     7,     7,     6,     6,     6,
       5,     5,     6,     6,     6,     6,     1,     3,     1,     0,
       0,     0,     0,     0,     3,     3,     0,     3,     0,     4,
       2,     3,     1,     2,     1,     1,     4,     2,     1,     1,
       8,    15,    10,     2,     2,     3,     2,     0,     0,     0,
       0,     0,     0,     1,     1,     2,     2,     3,     1,     1,
       1,     3,     4,     6,     6,     3,     4,     2,     2,     2,
       1,     3,     2,     4,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
       0,     0,    14,    14,     0,    22,     0,    48,     0,     0,
       1,     3,     0,    14,    14,     0,    10,     0,     0,    24,
       0,    26,    21,     0,     0,     0,     0,     9,     0,     0,
       8,     0,    49,     0,     0,    58,    58,    89,     0,     0,
       0,     0,    88,    90,   104,   108,   123,     0,     0,     0,
       0,     0,     0,    46,     0,    14,    14,    23,     0,    51,
       0,     0,     0,     0,     0,    50,     0,     0,     0,    11,
       0,     0,     0,     0,     0,    85,     0,    86,    97,    98,
       0,    83,    99,     0,     0,     0,     0,     0,    14,     0,
      25,    28,     0,    56,    56,     0,    41,     0,     0,    27,
       0,     0,    53,     0,    40,     0,     0,     0,    52,     0,
      12,    16,    13,    37,     0,     0,     0,    91,   100,     0,
      95,     0,    87,   105,   106,   107,   109,   110,    15,    29,
      31,     0,    45,     0,    54,    55,     0,     0,    81,     0,
       0,     0,    68,    60,    64,    69,     0,    62,    65,     0,
      88,    47,    43,    39,    34,     0,    44,    42,    38,    33,
       0,    17,    19,     0,     0,     0,    58,     0,    92,    96,
       0,    30,    58,     0,    78,    73,    74,    76,     0,    61,
      63,    67,     0,    36,    35,     0,    18,     0,     0,     0,
      59,   101,    32,    57,     0,   111,   116,   119,   121,    84,
       0,    75,     0,    20,    94,   102,     0,    93,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      78,   113,   112,   115,   114,   118,   117,   120,   122,     0,
     103,     0,    82,    79,    77,    70,     0,    80,    79,    77,
      72,    78,     0,    79,    77,    71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    25,    26,    29,   120,   173,
       8,    14,    15,   100,   141,     9,    62,    16,    80,   117,
     111,   170,   165,    63,   144,    85,   155,   156,   157,   158,
     230,   210,   245,   249,   184,   244,   159,   204,    52,    53,
      54,   129,   199,    55,    91,   206,   207,   208,   209,   121
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -223
static const yytype_int16 yypact[] =
{
      95,    24,   150,    29,   131,  -223,  -223,  -223,  -223,  -223,
      18,    76,    82,    87,    75,  -223,    86,  -223,    97,    99,
    -223,  -223,   112,   121,   121,   138,  -223,   107,   363,   125,
      11,   130,  -223,   165,    13,    30,    36,  -223,   135,   151,
    -223,   168,  -223,   161,   169,   104,   106,  -223,   363,   363,
     363,   363,  -223,  -223,  -223,   137,   160,   175,   283,   179,
     201,   185,   -22,  -223,   283,   121,   121,  -223,   183,  -223,
     -12,   186,   185,     4,   187,  -223,    23,   316,   316,  -223,
     185,   214,   216,   363,    72,  -223,   323,  -223,  -223,  -223,
     191,   160,  -223,   363,   363,   363,   363,   363,   121,    65,
    -223,  -223,   185,   198,   198,   197,  -223,   230,   185,  -223,
     185,   185,  -223,   185,  -223,   185,   185,   185,  -223,   145,
    -223,  -223,  -223,  -223,   204,   208,   213,  -223,  -223,    31,
    -223,    32,  -223,  -223,  -223,  -223,   137,   137,  -223,  -223,
    -223,     7,  -223,   218,  -223,  -223,   165,   217,  -223,   220,
     223,   356,  -223,  -223,  -223,  -223,   238,  -223,  -223,   225,
     224,  -223,  -223,  -223,  -223,   185,  -223,  -223,  -223,  -223,
     185,  -223,  -223,    50,   363,   219,   227,   363,  -223,  -223,
     283,  -223,   227,   363,  -223,  -223,  -223,  -223,   228,  -223,
    -223,  -223,   363,  -223,  -223,   316,  -223,    33,   363,    46,
    -223,  -223,  -223,  -223,   229,   160,   128,   190,   243,   246,
     232,  -223,   235,  -223,  -223,  -223,   222,  -223,  -223,   363,
     363,   363,   363,   363,   363,   363,   363,   363,  -223,   363,
    -223,   160,   160,   160,   160,   128,   128,   190,   243,   233,
    -223,   276,  -223,  -223,  -223,   248,   276,  -223,  -223,  -223,
    -223,  -223,   276,  -223,  -223,  -223
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -223,  -223,   270,   -96,  -223,  -223,   239,    -2,   -74,  -223,
    -223,  -223,   256,   -58,  -223,  -223,   142,   273,  -223,  -223,
    -223,  -223,  -223,   189,   188,   -43,   -54,  -223,   143,  -134,
    -124,  -222,  -156,  -223,  -223,  -223,   -48,    74,  -104,  -223,
     -24,   -81,  -223,   123,   -28,    -1,    77,    78,  -223,   275
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -49
static const yytype_int16 yytable[] =
{
      56,   160,    90,    87,   122,   131,   109,   106,   241,   154,
     101,    31,    59,   107,    68,    60,   101,    60,   114,    22,
     108,    38,    39,   107,    88,    89,   123,    92,    10,   252,
     112,    71,    23,    24,    60,   126,   128,    74,   128,   107,
      60,   140,   180,    17,    18,   172,   115,   181,   142,    56,
      56,   101,   160,    61,   162,    69,   163,   164,   107,   166,
     154,   167,   168,   169,    31,   118,   177,   177,   177,   133,
     134,   135,    72,   178,   179,   214,    43,    44,    75,    45,
      46,   216,    47,    43,    44,   195,    45,    46,   217,    47,
     196,    56,   250,   197,    48,    49,   138,   254,     1,     2,
       3,    48,    49,   188,    99,   139,    50,   243,    51,    32,
      33,   193,   248,    50,   127,    51,   194,    27,   253,    28,
     246,   213,   202,   -48,    28,   251,   128,    34,    30,   201,
     255,    20,   101,   200,     1,     2,     3,   160,    35,   203,
      36,    83,   160,    83,   212,    84,    37,    86,   160,    42,
     215,   219,   220,   221,   222,   205,    43,    44,    28,    45,
      46,    58,    47,    11,    12,    13,    64,    56,    93,    94,
      95,    77,    40,    41,    48,    49,    70,    73,    76,    65,
      66,   240,    23,    24,   119,   171,    50,    78,    51,    96,
      97,   231,   232,   233,   234,   205,   205,   205,   205,   205,
       1,   146,    81,   147,   148,   149,   150,   151,    43,    44,
      82,    45,    46,    98,    47,   103,   104,   223,   224,   136,
     137,   102,   235,   236,   105,   110,    48,    49,   113,   116,
     124,   152,   125,   132,    60,   143,   105,   153,    50,   174,
      51,     1,   146,   175,   147,   148,   149,   150,   151,    43,
      44,   176,    45,    46,   185,    47,   182,   186,   183,   191,
     192,   225,   211,   198,    83,   226,   229,    48,    49,   228,
     247,   218,   152,   227,    21,   242,    19,   105,   189,    50,
      79,    51,   147,   148,   149,   150,   151,    43,    44,    67,
      45,    46,   145,    47,    43,    44,   161,    45,    46,   190,
      47,   239,   237,    57,   238,    48,    49,     0,     0,     0,
     152,     0,    48,    49,     0,   105,     0,    50,     0,    51,
       0,     0,    99,     0,    50,     0,    51,    43,    44,     0,
      45,    46,     0,    47,    43,    44,     0,    45,    46,     0,
      47,     0,     0,     0,     0,    48,    49,     0,     0,     0,
       0,     0,    48,    49,     0,   119,     0,    50,     0,    51,
       0,     0,     0,     0,    50,   130,    51,    43,    44,     0,
      45,    46,     0,    47,    43,    44,     0,    45,    46,     0,
      47,     0,     0,     0,     0,    48,    49,     0,     0,     0,
     187,     0,    48,    49,     0,     0,     0,    50,     0,    51,
       0,     0,     0,     0,    50,     0,    51
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-223)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      28,   105,    50,    46,    78,    86,    64,    61,   230,   105,
      58,    13,     1,    35,     1,     4,    64,     4,    72,     1,
      42,    23,    24,    35,    48,    49,    80,    51,     4,   251,
      42,     1,    14,    15,     4,    83,    84,     1,    86,    35,
       4,    99,    35,    14,    15,   119,    42,    40,   102,    77,
      78,    99,   156,    42,   108,    42,   110,   111,    35,   113,
     156,   115,   116,   117,    66,    42,    35,    35,    35,    93,
      94,    95,    42,    42,    42,    42,    11,    12,    42,    14,
      15,    35,    17,    11,    12,    35,    14,    15,    42,    17,
      40,   119,   248,   174,    29,    30,    98,   253,     3,     4,
       5,    29,    30,   151,    39,    40,    41,   241,    43,    34,
      35,   165,   246,    41,    42,    43,   170,    41,   252,    37,
     244,   195,   180,    41,    37,   249,   174,    41,    41,   177,
     254,     0,   180,   176,     3,     4,     5,   241,    41,   182,
      41,    37,   246,    37,   192,    41,    34,    41,   252,    42,
     198,    23,    24,    25,    26,   183,    11,    12,    37,    14,
      15,    36,    17,    13,    14,    15,    36,   195,    31,    32,
      33,    36,    34,    35,    29,    30,    34,    35,    36,    14,
      15,   229,    14,    15,    39,    40,    41,    36,    43,    29,
      30,   219,   220,   221,   222,   223,   224,   225,   226,   227,
       3,     4,    41,     6,     7,     8,     9,    10,    11,    12,
      41,    14,    15,    38,    17,    14,    15,    27,    28,    96,
      97,    42,   223,   224,    39,    42,    29,    30,    42,    42,
      16,    34,    16,    42,     4,    37,    39,    40,    41,    35,
      43,     3,     4,    35,     6,     7,     8,     9,    10,    11,
      12,    38,    14,    15,    34,    17,    38,    34,    41,    34,
      36,    18,    34,    44,    37,    19,    44,    29,    30,    34,
      22,    42,    34,    41,     4,    42,     3,    39,    40,    41,
      41,    43,     6,     7,     8,     9,    10,    11,    12,    33,
      14,    15,   104,    17,    11,    12,   107,    14,    15,   156,
      17,   227,   225,    28,   226,    29,    30,    -1,    -1,    -1,
      34,    -1,    29,    30,    -1,    39,    -1,    41,    -1,    43,
      -1,    -1,    39,    -1,    41,    -1,    43,    11,    12,    -1,
      14,    15,    -1,    17,    11,    12,    -1,    14,    15,    -1,
      17,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
      -1,    -1,    29,    30,    -1,    39,    -1,    41,    -1,    43,
      -1,    -1,    -1,    -1,    41,    42,    43,    11,    12,    -1,
      14,    15,    -1,    17,    11,    12,    -1,    14,    15,    -1,
      17,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
      34,    -1,    29,    30,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    -1,    -1,    41,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    46,    47,    48,    49,    55,    60,
       4,    13,    14,    15,    56,    57,    62,    14,    15,    62,
       0,    47,     1,    14,    15,    50,    51,    41,    37,    52,
      41,    52,    34,    35,    41,    41,    41,    34,    52,    52,
      34,    35,    42,    11,    12,    14,    15,    17,    29,    30,
      41,    43,    83,    84,    85,    88,    89,    94,    36,     1,
       4,    42,    61,    68,    36,    14,    15,    57,     1,    42,
      61,     1,    42,    61,     1,    42,    61,    36,    36,    51,
      63,    41,    41,    37,    41,    70,    41,    70,    85,    85,
      81,    89,    85,    31,    32,    33,    29,    30,    38,    39,
      58,    81,    42,    14,    15,    39,    71,    35,    42,    58,
      42,    65,    42,    42,    71,    42,    42,    64,    42,    39,
      53,    94,    53,    71,    16,    16,    81,    42,    81,    86,
      42,    86,    42,    85,    85,    85,    88,    88,    52,    40,
      58,    59,    71,    37,    69,    69,     4,     6,     7,     8,
       9,    10,    34,    40,    48,    71,    72,    73,    74,    81,
      83,    68,    71,    71,    71,    67,    71,    71,    71,    71,
      66,    40,    53,    54,    35,    35,    38,    35,    42,    42,
      35,    40,    38,    41,    79,    34,    34,    34,    81,    40,
      73,    34,    36,    71,    71,    35,    40,    86,    44,    87,
      70,    81,    58,    70,    82,    89,    90,    91,    92,    93,
      76,    34,    81,    53,    42,    81,    35,    42,    42,    23,
      24,    25,    26,    27,    28,    18,    19,    41,    34,    44,
      75,    89,    89,    89,    89,    90,    90,    91,    92,    82,
      81,    76,    42,    74,    80,    77,    75,    22,    74,    78,
      77,    75,    76,    74,    77,    75
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
#line 229 "example1.y"
    {
    }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 231 "example1.y"
    {
    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 234 "example1.y"
    {
    }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 236 "example1.y"
    {
    }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 239 "example1.y"
    {
    }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 241 "example1.y"
    {
    }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 244 "example1.y"
    {
    }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 246 "example1.y"
    {
    }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 249 "example1.y"
    {
    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 251 "example1.y"
    {
    }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 255 "example1.y"
    {

		if(symbol[level].find((yyvsp[(1) - (4)].val)) != symbol[level].end()){
			yyerror("Const Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!sta[(yyvsp[(4) - (4)].defalt)].is_const){
				printf("Const Definition Error\n");
				exit(0);
			}
			if (!(yyvsp[(2) - (4)].defalt)){
				Assemble.push_back("\t.section\t.rodata\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[(yyvsp[(4) - (4)].defalt)].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][(yyvsp[(1) - (4)].val)] = Var(Constint, sta[(yyvsp[(4) - (4)].defalt)].val, 1);
			}
			//else ?
		}
		else{
			if (!(yyvsp[(2) - (4)].defalt)){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t$%d, %%edi\n", sta[(yyvsp[(4) - (4)].defalt)].val); Assemble.push_back(tmp);
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][(yyvsp[(1) - (4)].val)] = Var(Constint, sta[(yyvsp[(4) - (4)].defalt)].val, offset);
			}
			//else ?
		}
    }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 290 "example1.y"
    {
    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 294 "example1.y"
    {(yyval.defalt) = 0;}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 295 "example1.y"
    {
		if((yyvsp[(4) - (4)].defalt)) 
            sta[(yyval.defalt) = (yyvsp[(4) - (4)].defalt)].val = sta[(yyvsp[(2) - (4)].defalt)].val * sta[(yyvsp[(4) - (4)].defalt)].val;
		else 
            sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(2) - (4)].defalt)].val;
		sta[(yyval.defalt)].dim.push_back(sta[(yyvsp[(2) - (4)].defalt)].val);
    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 303 "example1.y"
    {

        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 307 "example1.y"
    {
    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 309 "example1.y"
    {
    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 312 "example1.y"
    {
    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 314 "example1.y"
    {
    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 317 "example1.y"
    {
    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 320 "example1.y"
    {
    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 322 "example1.y"
    {
    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 325 "example1.y"
    {
		if(symbol[level].find((yyvsp[(1) - (2)].val)) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!(yyvsp[(2) - (2)].defalt)){
				sprintf (tmp, "\t.globl\t%s\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				Assemble.push_back("\t.long\t0\n");
				Assemble.push_back("\t.text\n");
				symbol[0][(yyvsp[(1) - (2)].val)] = Var(Int, 0, 1);
			}
			else{
				sprintf (tmp, "\t.globl\t%s\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", (yyvsp[(1) - (2)].val), sta[(yyvsp[(2) - (2)].defalt)].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", (yyvsp[(1) - (2)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.zero\t%d\n", sta[(yyvsp[(2) - (2)].defalt)].val * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][(yyvsp[(1) - (2)].val)] = Var(Arr, 0, 1, sta[(yyvsp[(2) - (2)].defalt)].dim);
			}
		}
		else{
			if(!(yyvsp[(2) - (2)].defalt)){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				symbol[level][(yyvsp[(1) - (2)].val)] = Var(Int, 0, offset);
			}
			else{
				offset -= 4 * sta[(yyvsp[(2) - (2)].defalt)].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", sta[(yyvsp[(2) - (2)].defalt)].val * 4); Assemble.push_back(tmp);
				symbol[level][(yyvsp[(1) - (2)].val)] = Var(Arr, 0, offset, sta[(yyvsp[(2) - (2)].defalt)].dim);
			}
		}
    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 368 "example1.y"
    {

		if(symbol[level].find((yyvsp[(1) - (4)].val)) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if (!(yyvsp[(2) - (4)].defalt)){
				if(!(yyvsp[(4) - (4)].defalt) || !sta[(yyvsp[(4) - (4)].defalt)].is_const){
					yyerror("Initializer Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[(yyvsp[(4) - (4)].defalt)].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][(yyvsp[(1) - (4)].val)] = Var(Int, sta[(yyvsp[(4) - (4)].defalt)].val, 1);
			}
			else {
				if((yyvsp[(4) - (4)].defalt)){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[(yyvsp[(2) - (4)].defalt)].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", (yyvsp[(1) - (4)].val), sta[(yyvsp[(2) - (4)].defalt)].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
				for(auto x : ArrInitVal) {
					sprintf (tmp, "\t.long\t%d\n", sta[x].val);  Assemble.push_back(tmp);
				}
				sprintf (tmp, "\t.zero\t%d\n", sta[(yyvsp[(2) - (4)].defalt)].val * 4 - ArrInitVal.size() * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][(yyvsp[(1) - (4)].val)] = Var(Arr, 0, 1, sta[(yyvsp[(2) - (4)].defalt)].dim);
				ArrInitVal.clear();
			}
		}
		else{
			if (!(yyvsp[(2) - (4)].defalt)){
				if(!(yyvsp[(4) - (4)].defalt)){
					yyerror("Initializer Error");
					exit(0);
				}
				var2reg((yyvsp[(4) - (4)].defalt), "edi");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][(yyvsp[(1) - (4)].val)] = Var(Int, sta[(yyvsp[(4) - (4)].defalt)].val, offset);
			}
			else {
				if((yyvsp[(4) - (4)].defalt)){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[(yyvsp[(2) - (4)].defalt)].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				offset -= 4 * sta[(yyvsp[(2) - (4)].defalt)].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", 4 * sta[(yyvsp[(2) - (4)].defalt)].val); Assemble.push_back(tmp);
				for(int i = 0; i < ArrInitVal.size(); ++i){
					var2reg(ArrInitVal[i], "edi");
					sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset + i * 4);  Assemble.push_back(tmp);
				}
				symbol[level][(yyvsp[(1) - (4)].val)] = Var(Arr, 0, offset, sta[(yyvsp[(2) - (4)].defalt)].dim);
				ArrInitVal.clear();
			} 
		}
    }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 447 "example1.y"
    {
    }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 449 "example1.y"
    {
    }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 454 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 457 "example1.y"
    {
        (yyval.defalt) = 0;
    }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 460 "example1.y"
    {
        (yyval.defalt) = 0;
    }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 464 "example1.y"
    {
		if(!level && !sta[(yyvsp[(1) - (1)].defalt)].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back((yyvsp[(1) - (1)].defalt));
    }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 471 "example1.y"
    {
		if(!level && !sta[(yyvsp[(3) - (3)].defalt)].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back((yyvsp[(3) - (3)].defalt));
    }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 479 "example1.y"
    {

		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
    }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 488 "example1.y"
    {

		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 497 "example1.y"
    {

		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 506 "example1.y"
    {

		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
    }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 515 "example1.y"
    {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0;
	}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 523 "example1.y"
    {
    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 525 "example1.y"
    {
    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 527 "example1.y"
    {
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 529 "example1.y"
    {
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 531 "example1.y"
    {
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 533 "example1.y"
    {
    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 535 "example1.y"
    {
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 537 "example1.y"
    {

    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 542 "example1.y"
    {
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 544 "example1.y"
    {
    }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 547 "example1.y"
    {
		strcpy(funcname, (yyvsp[(1) - (1)].val));
		Para.clear();
	}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 553 "example1.y"
    {
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		Assemble.push_back("\t.globl\tmain\n");
		Assemble.push_back("\t.type\tmain, @function\n");
		Assemble.push_back("main:\n");
		call_func_push();
	}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 564 "example1.y"
    {
		is_func_void = true;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fvoid, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
	}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 583 "example1.y"
    {
		is_func_void = false;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fint, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
	}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 602 "example1.y"
    {
		is_func_void = true;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fvoid, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
		for(int i = 0; i < Para.size(); ++i) {
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf(tmp, "\tmovl\t%d(%rbp), %r8d\n", 32 + i * 4); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			symbol[level][Para[i].c_str()] = Var(Int, 0, offset);
		}
	}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 628 "example1.y"
    {
		is_func_void = false;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fint, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
		for(int i = 0; i < Para.size(); ++i) {
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf(tmp, "\tmovl\t%d(%rbp), %r8d\n", 32 + i * 4); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			symbol[level][Para[i].c_str()] = Var(Int, 0, offset);
		}
	}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 654 "example1.y"
    {
        Para.push_back((yyvsp[(2) - (3)].val));
    }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 657 "example1.y"
    {
    }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 661 "example1.y"
    {}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 662 "example1.y"
    {
        if((yyvsp[(3) - (3)].defalt)) (yyval.defalt) = (yyvsp[(3) - (3)].defalt);
    
    }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 668 "example1.y"
    {(yyval.defalt) = 0;}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 669 "example1.y"
    {
		(yyval.defalt) = ((yyvsp[(4) - (4)].defalt)) ? (yyvsp[(4) - (4)].defalt) : ++tot;
		sta[(yyval.defalt)].dim.push_back((yyvsp[(2) - (4)].defalt));
	}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 691 "example1.y"
    {

    }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 694 "example1.y"
    {

    }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 698 "example1.y"
    {

    }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 701 "example1.y"
    {

    }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 705 "example1.y"
    {

    }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 708 "example1.y"
    {

    }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 712 "example1.y"
    {

		if(sta[(yyvsp[(1) - (4)].defalt)].is_const){
			yyerror("Const Error");
			exit(0);
		}
		var2reg((yyvsp[(3) - (4)].defalt), "r9d");
		reg2var("r9d", (yyvsp[(1) - (4)].defalt));
    }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 721 "example1.y"
    {

    }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 724 "example1.y"
    {

    }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 727 "example1.y"
    {

    }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 730 "example1.y"
    {

		--level;
		symbol.pop_back();
		char tmp[114];
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		for(auto x : sta[(yyvsp[(3) - (8)].defalt)].truelist) Assemble[x] += ".L" + to_string((yyvsp[(5) - (8)].defalt)) + "\n";
		for(auto x : sta[(yyvsp[(3) - (8)].defalt)].falselist) Assemble[x] += ".L" + to_string(num) + "\n";
    }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 740 "example1.y"
    {

	--level;
		symbol.pop_back();
		for(auto x : sta[(yyvsp[(3) - (15)].defalt)].truelist) Assemble[x] += ".L" + to_string((yyvsp[(5) - (15)].defalt)) + "\n";
		for(auto x : sta[(yyvsp[(3) - (15)].defalt)].falselist) Assemble[x] += ".L" + to_string((yyvsp[(11) - (15)].defalt)) + "\n";
		Assemble[sta[(yyvsp[(10) - (15)].defalt)].truelist[0]] += ".L" + to_string((yyvsp[(15) - (15)].defalt)) + "\n";

    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 749 "example1.y"
    {
		--level;
		symbol.pop_back();
		backpatch(sta[(yyvsp[(5) - (10)].defalt)].truelist, (yyvsp[(8) - (10)].defalt));
		char tmp[114];
		sprintf (tmp, "\tjmp\t.L%d\n", (yyvsp[(2) - (10)].defalt)); Assemble.push_back(tmp);
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		backpatch(sta[(yyvsp[(5) - (10)].defalt)].falselist, sta[(yyvsp[(7) - (10)].defalt)].quad);
		backpatch(sta[(yyvsp[(7) - (10)].defalt)].truelist, num);
		for(auto it : *breaklist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string(num) + "\n";
		}
        	breaklist.pop_back();
		for(auto it : *continuelist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string((yyvsp[(2) - (10)].defalt)) + "\n";
		}
        	continuelist.pop_back();
    }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 772 "example1.y"
    {
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*breaklist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));

    }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 778 "example1.y"
    {
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*continuelist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));

    }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 784 "example1.y"
    {
		if(is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		var2reg((yyvsp[(2) - (3)].defalt), "eax");
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();

    }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 795 "example1.y"
    {
		if(!is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 805 "example1.y"
    {
		(yyval.defalt) = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
	}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 812 "example1.y"
    {
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		p_offset.push_back(offset);
		if(abs(offset) % 16 != 0){
			int padding = 16 - abs(offset) % 16;
			offset -= padding;
			char tmp[114];
			sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);
		}
	}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 826 "example1.y"
    {
		char tmp[114];
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		p_offset.pop_back();
		offset = last;
	}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 835 "example1.y"
    {
		Assemble.push_back("\tjmp\t");
		sta[(yyval.defalt) = ++tot].truelist.push_back(Assemble.size() - 1);
	}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 841 "example1.y"
    {
		(yyval.defalt) = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		vector< pair<int, int> > x, y;
		breaklist.push_back(x);
		continuelist.push_back(y);
	}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 851 "example1.y"
    {
		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		Assemble.push_back("\tjmp\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
	}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 865 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 869 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 873 "example1.y"
    {
		bool used = false;
		if(!(yyvsp[(2) - (2)].defalt)){
			for(int i = level; i >= 0; --i)
				if(symbol[i].find((yyvsp[(1) - (2)].val)) != symbol[i].end()){
					used = true;
					Var cur = symbol[i][(yyvsp[(1) - (2)].val)];
					(yyval.defalt) = ++tot;
					if(cur.type == Constint) {
						sta[(yyval.defalt)].is_const = true;
						sta[(yyval.defalt)].val = cur.val;
					}
					else {
						sta[(yyval.defalt)].is_const = false;
						sta[(yyval.defalt)].offset = cur.offset;
						if(!i) sta[(yyval.defalt)].name = string((yyvsp[(1) - (2)].val));
					}
					break;
				}
		}
		else{
			char tmp[114];
			for(int i = level; i >= 0; --i)
				if(symbol[i].find((yyvsp[(1) - (2)].val)) != symbol[i].end()){
					Var cur = symbol[i][(yyvsp[(1) - (2)].val)];
					if(cur.type != Arr || sta[(yyvsp[(2) - (2)].defalt)].dim.size() != cur.dim.size()) continue;
					used = true;
					int x = 1;
					offset -= 4;
					Assemble.push_back("\tsubq\t$4, %rsp\n");
					sprintf (tmp, "\tmovl\t$0, %d(%rbp)\n", offset); Assemble.push_back(tmp);
					int z = offset;
					for(int j = 0; j < cur.dim.size(); ++j){
						var2reg(sta[(yyvsp[(2) - (2)].defalt)].dim[j], "r8d");
						sprintf (tmp, "\timull\t$%d, %r8d\n", x); Assemble.push_back(tmp);
						sprintf (tmp, "\taddl\t%d(%rbp), %r8d\n", z); Assemble.push_back(tmp);
						sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", z); Assemble.push_back(tmp);
						x *= cur.dim[j];
					}
					(yyval.defalt) = ++tot;
					sta[(yyval.defalt)].is_arr = true;
					if(cur.type == Constint){
						sta[(yyval.defalt)].is_const = true;
						sta[(yyval.defalt)].val = cur.val;
					}
					else{
						sta[(yyval.defalt)].is_const = false;
						sta[(yyval.defalt)].offset = cur.offset;
						sta[(yyval.defalt)].offset_of_arr = z;
						if(!i) sta[(yyval.defalt)].name = string((yyvsp[(1) - (2)].val));
					}
					break;
				}
		}
		if(!used){
			yyerror("Reference Undefined Variable");
			exit(0);
		}
    }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 932 "example1.y"
    {
    }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 936 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(2) - (3)].defalt);
    }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 939 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 942 "example1.y"
    {
    // fprintf(fDetail, "PrimaryExp -> IntConst\n");
    // $$ = newNode("PrimaryExp");
    // char* buffer = new char[20];
    // if (buffer) {
    //     sprintf(buffer, "%d", $1);
    // }
    // addChild($$, newNode(buffer)); 
    // delete[] buffer;       

        sta[(yyval.defalt) = ++tot].val = (yyvsp[(1) - (1)].num), sta[(yyval.defalt)].is_const = true;          
    }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 955 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 958 "example1.y"
    {

		if(symbol[0].find((yyvsp[(1) - (3)].val)) == symbol[0].end()){
			yyerror("Funciton Not Defined Error");
			exit(0);
		}
		char tmp[114];
		if(abs(offset) % 16){
			int padding = 16 - abs(offset) % 16;
			offset -= padding;
			sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
		}
		sprintf (tmp, "\tcall\t%s\n", (yyvsp[(1) - (3)].val)); Assemble.push_back(tmp);
		Var function = symbol[0][(yyvsp[(1) - (3)].val)];
		if(function.type == Fint){
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n"); 
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 979 "example1.y"
    {

		if(!strcmp((yyvsp[(1) - (4)].val), "scanf")){
			if(sta[(yyvsp[(3) - (4)].defalt)].para_name.size() != 1){
				yyerror("Scanf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}		
			lea_para2reg((yyvsp[(3) - (4)].defalt), 0, "rsi");
			Assemble.push_back("\tleaq\t.LC0(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\t__isoc99_scanf@PLT\n");
		}
		else if(!strcmp((yyvsp[(1) - (4)].val), "printf")){
			if(sta[(yyvsp[(3) - (4)].defalt)].para_name.size() != 1){
				yyerror("Printf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			para2reg((yyvsp[(3) - (4)].defalt), 0, "esi");
			Assemble.push_back("\tleaq\t.LC1(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\tprintf@PLT\n");
		}
		else{
			if(symbol[0].find((yyvsp[(1) - (4)].val)) == symbol[0].end()){
				yyerror("Funciton Undefined Error");
				exit(0);
			}
			char tmp[114];
			if((-(offset - 4 * sta[(yyvsp[(3) - (4)].defalt)].para_name.size())) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			for(int i = sta[(yyvsp[(3) - (4)].defalt)].para_name.size() - 1; i >= 0 ; --i){
				para2reg((yyvsp[(3) - (4)].defalt), i, "r8d");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			}
			sprintf (tmp, "\tcall\t%s\n", (yyvsp[(1) - (4)].val)); Assemble.push_back(tmp);
			Var function = symbol[0][(yyvsp[(1) - (4)].val)];
				if(function.type == Fint){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				sta[(yyval.defalt) = ++tot].offset = offset;
			}	
		}
    }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 1040 "example1.y"
    {

    }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1043 "example1.y"
    {

    }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1046 "example1.y"
    {

    }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1049 "example1.y"
    {

    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 1052 "example1.y"
    {

        (yyval.defalt) = (yyvsp[(2) - (2)].defalt);
    }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 1056 "example1.y"
    {

		if(sta[(yyvsp[(2) - (2)].defalt)].is_const) sta[(yyval.defalt) = (yyvsp[(2) - (2)].defalt)].val = -sta[(yyvsp[(2) - (2)].defalt)].val;
		else{
			char tmp[114];
			var2reg((yyvsp[(2) - (2)].defalt), "r8d");
			Assemble.push_back("\tnegl\t%r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 1069 "example1.y"
    {


		char tmp[114];
		var2reg((yyvsp[(2) - (2)].defalt), "eax");	
		Assemble.push_back("\ttestl\t%eax, %eax\n");
		Assemble.push_back("\tsete\t%al\n");
		Assemble.push_back("\tmovzbl\t%al, %eax\n");
		offset -= 4;
		Assemble.push_back("\tsubq\t$4, %rsp\n");
		sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
		sta[(yyval.defalt) = ++tot].offset = offset;
    }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 1083 "example1.y"
    {


		(yyval.defalt) = (yyvsp[(1) - (1)].defalt);
		sta[(yyval.defalt)].para_name.push_back(sta[(yyvsp[(1) - (1)].defalt)].name);
		sta[(yyval.defalt)].para_val.push_back(sta[(yyvsp[(1) - (1)].defalt)].val);
		sta[(yyval.defalt)].para_offset.push_back(sta[(yyvsp[(1) - (1)].defalt)].offset);
		sta[(yyval.defalt)].para_offset_of_arr.push_back(sta[(yyvsp[(1) - (1)].defalt)].offset_of_arr);
		sta[(yyval.defalt)].para_is_const.push_back(sta[(yyvsp[(1) - (1)].defalt)].is_const);
		sta[(yyval.defalt)].para_is_arr.push_back(sta[(yyvsp[(1) - (1)].defalt)].is_arr);

    }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 1095 "example1.y"
    {


		(yyval.defalt) = (yyvsp[(1) - (3)].defalt);
		sta[(yyval.defalt)].para_name.push_back(sta[(yyvsp[(3) - (3)].defalt)].name);
		sta[(yyval.defalt)].para_val.push_back(sta[(yyvsp[(3) - (3)].defalt)].val);
		sta[(yyval.defalt)].para_offset.push_back(sta[(yyvsp[(3) - (3)].defalt)].offset);
		sta[(yyval.defalt)].para_offset_of_arr.push_back(sta[(yyvsp[(3) - (3)].defalt)].offset_of_arr);
		sta[(yyval.defalt)].para_is_const.push_back(sta[(yyvsp[(3) - (3)].defalt)].is_const);
		sta[(yyval.defalt)].para_is_arr.push_back(sta[(yyvsp[(3) - (3)].defalt)].is_arr);

    }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 1108 "example1.y"
    {

    }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 1111 "example1.y"
    {
}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 1114 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 1117 "example1.y"
    {

		if(sta[(yyvsp[(1) - (3)].defalt)].is_const && sta[(yyvsp[(3) - (3)].defalt)].is_const)
			sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(1) - (3)].defalt)].val * sta[(yyvsp[(3) - (3)].defalt)].val, sta[(yyval.defalt)].is_const = 1;
		else{
			char tmp[114];
			var2reg((yyvsp[(1) - (3)].defalt), "r8d");
			var2reg((yyvsp[(3) - (3)].defalt), "r9d");
			Assemble.push_back("\timull\t%r8d, %r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r9d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 1132 "example1.y"
    {

		if(sta[(yyvsp[(1) - (3)].defalt)].is_const && sta[(yyvsp[(3) - (3)].defalt)].is_const)
			sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(1) - (3)].defalt)].val / sta[(yyvsp[(3) - (3)].defalt)].val, sta[(yyval.defalt)].is_const = 1;
		else{
			char tmp[114];
			var2reg((yyvsp[(1) - (3)].defalt), "eax");
			var2reg((yyvsp[(3) - (3)].defalt), "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 1148 "example1.y"
    {

		if(sta[(yyvsp[(1) - (3)].defalt)].is_const && sta[(yyvsp[(3) - (3)].defalt)].is_const)
			sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(1) - (3)].defalt)].val % sta[(yyvsp[(3) - (3)].defalt)].val, sta[(yyval.defalt)].is_const = 1;
		else{
			char tmp[114];
			var2reg((yyvsp[(1) - (3)].defalt), "eax");
			var2reg((yyvsp[(3) - (3)].defalt), "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%edx, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 1165 "example1.y"
    {

        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 1169 "example1.y"
    {

		if(sta[(yyvsp[(1) - (3)].defalt)].is_const && sta[(yyvsp[(3) - (3)].defalt)].is_const)
			sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(1) - (3)].defalt)].val + sta[(yyvsp[(3) - (3)].defalt)].val, sta[(yyval.defalt)].is_const = 1;
		else{
			char tmp[114];
			var2reg((yyvsp[(1) - (3)].defalt), "r8d");
			var2reg((yyvsp[(3) - (3)].defalt), "r9d");
			Assemble.push_back("\taddl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 1184 "example1.y"
    {

		if(sta[(yyvsp[(1) - (3)].defalt)].is_const && sta[(yyvsp[(3) - (3)].defalt)].is_const)
			sta[(yyval.defalt) = ++tot].val = sta[(yyvsp[(1) - (3)].defalt)].val - sta[(yyvsp[(3) - (3)].defalt)].val, sta[(yyval.defalt)].is_const = 1;
		else{
			char tmp[114];
			var2reg((yyvsp[(1) - (3)].defalt), "r8d");
			var2reg((yyvsp[(3) - (3)].defalt), "r9d");
			Assemble.push_back("\tsubl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[(yyval.defalt) = ++tot].offset = offset;
		}
    }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 1200 "example1.y"
    {

        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 1204 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjl\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjge\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 1218 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjg\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjle\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 1232 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjge\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjl\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 1246 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjle\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjg\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 1261 "example1.y"
    {
        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);
    }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 1264 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tje\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjne\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 1278 "example1.y"
    {

		char tmp[114];
		sta[(yyval.defalt) = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg((yyvsp[(1) - (3)].defalt), "r8d");
		var2reg((yyvsp[(3) - (3)].defalt), "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjne\t");
		sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tje\t");
		sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);

    }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 1293 "example1.y"
    {

		(yyval.defalt) = (yyvsp[(1) - (1)].defalt);
		if(!sta[(yyval.defalt)].quad){
			char tmp[114];
			sta[(yyval.defalt) = ++tot].quad = ++num;
			sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
			var2reg((yyvsp[(1) - (1)].defalt), "r8d");
			Assemble.push_back("\tcmpl\t$0, %r8d\n");
			Assemble.push_back("\tjne\t");
			sta[(yyval.defalt)].truelist.push_back(Assemble.size() - 1);
			Assemble.push_back("\tje\t");
			sta[(yyval.defalt)].falselist.push_back(Assemble.size() - 1);
		}

    }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 1309 "example1.y"
    {

    	backpatch(sta[(yyvsp[(1) - (3)].defalt)].truelist, sta[(yyvsp[(3) - (3)].defalt)].quad);
		sta[(yyval.defalt)].falselist = merge(sta[(yyvsp[(1) - (3)].defalt)].falselist, sta[(yyvsp[(3) - (3)].defalt)].falselist);
		sta[(yyval.defalt)].truelist = sta[(yyvsp[(3) - (3)].defalt)].truelist;
		sta[(yyval.defalt)].quad = sta[(yyvsp[(1) - (3)].defalt)].quad;
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 1317 "example1.y"
    {

        (yyval.defalt) = (yyvsp[(1) - (1)].defalt);   
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 1321 "example1.y"
    {
    	backpatch(sta[(yyvsp[(1) - (3)].defalt)].falselist, sta[(yyvsp[(3) - (3)].defalt)].quad);
		sta[(yyval.defalt)].truelist = merge(sta[(yyvsp[(1) - (3)].defalt)].truelist, sta[(yyvsp[(3) - (3)].defalt)].truelist);
		sta[(yyval.defalt)].falselist = sta[(yyvsp[(3) - (3)].defalt)].falselist;
		sta[(yyval.defalt)].quad = sta[(yyvsp[(1) - (3)].defalt)].quad;
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 1328 "example1.y"
    {

    	(yyval.defalt) = (yyvsp[(1) - (1)].defalt);
		if(sta[(yyval.defalt)].is_const == false){
			yyerror("Const Error");	
			exit(0);
		}
}
    break;


/* Line 1792 of yacc.c  */
#line 3445 "parser.tab.cpp"
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
#line 1339 "example1.y"

void yyerror(const char *s){
    fprintf(stderr, "Error at line %d: %s\n", lineno, s);
}




int main(int argc, char *argv[]) {
    yyin = fopen(argv[1], "r");
    f = fopen("assemble.s", "w");
    	
   	Assemble.push_back(".LC0:\n");
   	Assemble.push_back("\t.string\t\"\%d\"\n");
    	Assemble.push_back(".LC1:\n");
    	Assemble.push_back("\t.string\t\"\%d\\n\"\n");
    	map<string, Var> x;
    	symbol.push_back(x);
    	p_offset.push_back(0);
    	
    	yyparse();
    	
    	for(auto x : Assemble) fprintf(f, "%s", x.c_str());
    	
	fclose(f);
	return 0;
}