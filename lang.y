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
block:  open decls stmts close  {  }
    |   open stmts close        { $$ = new cBlockNode(nullptr, $2); }

open:   '{'                     { g_symbolTable.IncreaseScope(); }

close:  '}'                     { g_symbolTable.DecreaseScope(); }

decls:      decls decl          { std::cerr << "Adding DECLS" << std::endl; $$ = $1; $$->AddChild($2); }
        |   decl                { std::cerr << "Adding DECL" << std::endl; $$ = new cDeclsNode((cDeclNode*)$1); }
decl:       var_decl ';'        { std::cerr << "Adding var_decl" << std::endl; $$ = $1; }
        |   struct_decl ';'     { std::cerr << "Adding struct_decl" << std::endl; }
        |   array_decl ';'      { std::cerr << "Adding array_decl" << std::endl; }
        |   func_decl           { std::cerr << "Adding func_decl" << std::endl; }
        |   error ';'           { std::cerr << "Adding error" << std::endl; }

var_decl:   TYPE_ID IDENTIFIER  { std::cerr << "Creating var_decl" << std::endl; }
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

stmts:      stmts stmt          { std::cerr << "stmts:stmts" << std::endl; $$ = $1; $$->Insert($2); }
        |   stmt                { std::cerr << "stmts:stmt" << std::endl; $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                {  }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                {  }
        |   WHILE '(' expr ')' stmt 
                                {  }
        |   PRINT '(' expr ')' ';'
                                { std::cerr << "stmt:print" << std::endl; $$ = new cPrintNode($3); }
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

expr:       expr EQUALS addit   { std::cerr << "expr:EQUALS" << std::endl; }
        |   addit               {   }

addit:      addit '+' term      { std::cerr << "Adding ADDIT +" << std::endl; $$ = new cExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term      { std::cerr << "Adding ADDIT -" << std::endl; $$ = new cExprNode($1, new cOpNode('-'), $3); }
        |   term                {  }

term:       term '*' fact       { std::cerr << "Adding TERM *" << std::endl; $$ = new cExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact       { std::cerr << "Adding TERM */" << std::endl; $$ = new cExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact       { std::cerr << "Adding TERM *%" << std::endl; $$ = new cExprNode($1, new cOpNode('%'), $3); }
        |   fact                {  }

fact:        '(' expr ')'       {  }
        |   INT_VAL             { std::cerr << "Found INT" << std::endl; $$ = new cIntExprNode(yylval.int_val); }
        |   FLOAT_VAL           { std::cerr << "Found FLOAT" << std::endl; $$ = new cFloatExprNode(yylval.float_val); }
        |   varref              { std::cerr << "Found VARREF" << std::endl; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
