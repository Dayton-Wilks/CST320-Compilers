%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include <stdlib.h>
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"
#include "cDebugPrint.h"

%}

%locations

 /* union defines the type for lexical values */
%union{
    int             int_val;
    float           float_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cPrintNode*     stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cSymbol*        symbol;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
%}

%start  program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   EQUALS
%token <int_val>   NOT_EQUALS

%token  PROGRAM
%token  PRINT
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <ast_node> open
%type <ast_node> close
%type <ast_node> decls
%type <ast_node> decl
%type <ast_node> var_decl
%type <ast_node> struct_decl
%type <ast_node> array_decl
%type <ast_node> func_decl
%type <ast_node> func_header
%type <ast_node> func_prefix
%type <ast_node> func_call
%type <ast_node> paramsspec
%type <ast_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <ast_node> lval
%type <ast_node> params
%type <ast_node> param
%type <expr_node> expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <ast_node> varref
%type <symbol> varpart

%%

program: PROGRAM block          { $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close  { $$ = new cBlockNode((cDeclsNode*)$2, $3); }
    |   open stmts close        { $$ = new cBlockNode(nullptr, $2); }

open:   '{'                     { g_symbolTable.IncreaseScope(); }

close:  '}'                     { g_symbolTable.DecreaseScope(); }

decls:      decls decl          { DebugPrint("Adding DECLS"); $$ = $1; $$->AddChild($2); }
        |   decl                { DebugPrint("Adding DECL"); $$ = new cDeclsNode($1); }
decl:       var_decl ';'        { DebugPrint("Adding var_decl"); $$ = $1; }
        |   struct_decl ';'     { DebugPrint("Adding struct_decl"); }
        |   array_decl ';'      { DebugPrint("Adding array_decl"); }
        |   func_decl           { DebugPrint("Adding func_decl"); }
        |   error ';'           { DebugPrint("Adding error"); }

var_decl:   TYPE_ID IDENTIFIER  { DebugPrint("Creating var_decl"); $$ = new cVarDeclNode($1); $$->AddChild(yylval.symbol); }
struct_decl:  STRUCT open decls close IDENTIFIER    
                                {  }
array_decl: ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                {  }

func_decl:  func_header ';'
                                {  }
        |   func_header  '{' decls stmts '}'
                                {  }
        |   func_header  '{' stmts '}'
                                {  }
func_header: func_prefix paramsspec ')'
                                {  }
        |    func_prefix ')'    {  }
func_prefix: TYPE_ID IDENTIFIER '('
                                {  }
paramsspec: paramsspec',' paramspec 
                                {  }
        |   paramspec           {  }

paramspec:  var_decl            {  }

stmts:      stmts stmt          { DebugPrint("stmts:stmts"); $$ = $1; $$->Insert($2); }
        |   stmt                { DebugPrint("stmts:stmt"); $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                {  }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                {  }
        |   WHILE '(' expr ')' stmt 
                                {  }
        |   PRINT '(' expr ')' ';'
                                { DebugPrint("stmt:print"); $$ = new cPrintNode($3); }
        |   lval '=' expr ';'   {  }
        |   lval '=' func_call ';'   {  }
        |   func_call ';'       {  }
        |   block               {  }
        |   RETURN expr ';'     {  }
        |   error ';'           {}

func_call:  IDENTIFIER '(' params ')' {  }
        |   IDENTIFIER '(' ')'  {  }

varref:   varref '.' varpart    {  }
        | varref '[' expr ']'   {  }
        | varpart               {  }

varpart:  IDENTIFIER            {  }

lval:     varref                {  }

params:     params',' param     {  }
        |   param               {  }

param:      expr                {  }

expr:       expr EQUALS addit   { DebugPrint("expr:EQUALS"); }
        |   addit               {   }

addit:      addit '+' term      { DebugPrint("Adding ADDIT +"); $$ = new cExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term      { DebugPrint("Adding ADDIT -"); $$ = new cExprNode($1, new cOpNode('-'), $3); }
        |   term                {  }

term:       term '*' fact       { DebugPrint("Adding TERM *"); $$ = new cExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact       { DebugPrint("Adding TERM */"); $$ = new cExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact       { DebugPrint("Adding TERM *%"); $$ = new cExprNode($1, new cOpNode('%'), $3); }
        |   fact                {  }

fact:        '(' expr ')'       {  }
        |   INT_VAL             { DebugPrint("Found INT"); $$ = new cIntExprNode(yylval.int_val); }
        |   FLOAT_VAL           { DebugPrint("Found FLOAT"); $$ = new cFloatExprNode(yylval.float_val); }
        |   varref              { DebugPrint("Found VARREF"); $$ = new cVarRefNode($1); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

