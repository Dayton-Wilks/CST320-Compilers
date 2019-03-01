%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//
#include <iostream>
#include "lex.h"
#include "astnodes.h"

%}

%locations

 /* union defines the type for lexical values */
%union{
    int             int_val;
    double          float_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cFuncDeclNode*  func_node;
    cFuncExprNode*  funccall_node;
    cSymbol*        symbol;
    cSymbolTable::symbolTable_t*  sym_table;
    cDeclsNode*     decls_node;
    cParamsNode*    params_node;
    cDeclNode*      decl_node;
    cVarExprNode*   varref_node;
    cParamListNode* paramlist_node;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;

    static bool g_semanticErrorHappened = false;

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }
%}

%start  program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL

%token  PROGRAM
%token  PRINT
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  AND OR NEQUALS EQUALS
%token  JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <sym_table> open
%type <sym_table> close
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <decl_node> array_decl
%type <func_node> func_decl
%type <func_node> func_header
%type <func_node> func_prefix
%type <funccall_node> func_call
%type <params_node> paramsspec
%type <decl_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <varref_node> lval
%type <paramlist_node> paramlist
%type <expr_node> param
%type <expr_node> expr
%type <expr_node> and
%type <expr_node> equal
%type <expr_node> add
%type <expr_node> term
%type <expr_node> fact
%type <varref_node> varref
%type <symbol> varpart

%%

program: PROGRAM block          { $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close  { $$ = new cBlockNode($2, $3); }
    |   open stmts close        { $$ = new cBlockNode(nullptr, $2); }

open:   '{'                     { $$ = g_SymbolTable.IncreaseScope(); }

close:  '}'                     { $$ = g_SymbolTable.DecreaseScope(); }

decls:      decls decl          { $$ = $1; $$->Insert($2); }
        |   decl                { $$ = new cDeclsNode($1); }
decl:       var_decl ';'        { $$ = $1; CHECK_ERROR(); }
        |   array_decl ';'      { $$ = $1; }
        |   struct_decl ';'     { $$ = $1; }
        |   func_decl           { $$ = $1; }
        |   error ';'           {}

var_decl:   TYPE_ID IDENTIFIER  { 
                                  $$ = new cVarDeclNode($1, $2);
                                  CHECK_ERROR();
                                } 
struct_decl:  STRUCT open decls close IDENTIFIER    
                                { $$ = new cStructDeclNode($2, $3, $5); 
                                  PROP_ERROR();
                                }
array_decl:  ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER 
                                { $$ = new cArrayDeclNode($2, $6, $4); 
                                  PROP_ERROR();
                                }
func_decl:  func_header ';'
                                { $$ = $1;  
                                  g_SymbolTable.DecreaseScope();  
                                }
        |   func_header  '{' decls stmts '}'
                                { $$ = $1; 
                                  $$->AddDecls($3); 
                                  $$->AddStmts($4); 
                                  g_SymbolTable.DecreaseScope();  
                                }
        |   func_header  '{' stmts '}'
                                { $$ = $1; 
                                  $$->AddDecls(nullptr); 
                                  $$->AddStmts($3); 
                                  g_SymbolTable.DecreaseScope();  
                                }
func_header: func_prefix paramsspec ')'
                                { 
                                  $$ = $1; $$->AddParams($2); 
                                }
        |    func_prefix ')'    { $$ = $1; $$->AddParams(nullptr);}
func_prefix: TYPE_ID IDENTIFIER '('
                                { 
                                  $$ = new cFuncDeclNode($1, $2); 
                                  g_SymbolTable.IncreaseScope();
                                  CHECK_ERROR();
                                }
paramsspec:  paramsspec',' paramspec 
                                { $$ = $1; $$->Insert($3); }
        |   paramspec           { $$ = new cParamsNode($1); }

paramspec:  var_decl            { $$ = $1; }

stmts:      stmts stmt          { $$ = $1; $$->Insert($2); }
        |   stmt                { $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, nullptr); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3, $5); }
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   lval '=' expr ';'   { 
                                  $$ = new cAssignNode($1, $3); 
                                  CHECK_ERROR(); 
                                }
        |   func_call ';'       { $$ = $1; } 
        |   block               { $$ = $1; }
        |   RETURN expr ';'     { $$ = new cReturnNode($2); }
        |   error ';'           {}

func_call:  IDENTIFIER '(' paramlist ')' { 
                                  $$ = new cFuncExprNode($1, $3); 
                                  CHECK_ERROR();
                                }
        |   IDENTIFIER '(' ')'  { 
                                    $$ = new cFuncExprNode($1, nullptr);
                                  CHECK_ERROR();
                                }

varref:   varref '.' varpart    { $$ = $1; $$->AddElement($3); PROP_ERROR(); }
        | varref '[' expr ']'   { $$ = $1; $$->AddElement($3); PROP_ERROR(); }
        | varpart               { $$ = new cVarExprNode($1); PROP_ERROR(); }

varpart:  IDENTIFIER            { $$ = $1; }

lval:     varref                { $$ = $1; }

paramlist:     paramlist',' param     { $$ = $1; $$->Insert($3); }
        |   param               { $$ = new cParamListNode($1); }

param:      expr                { $$ = $1; }

expr:       expr OR and         { $$ = new cBinaryExprNode($1, OR, $3); }
        |   and                 { $$ = $1; }

and:        and AND equal       { $$ = new cBinaryExprNode($1, AND, $3); }
        |   equal               { $$ = $1; }

equal:      equal EQUALS add    { $$ = new cBinaryExprNode($1, EQUALS, $3); }
        |   equal NEQUALS add    { $$ = new cBinaryExprNode($1, NEQUALS, $3); }
        |   add                 { $$ = $1; }

add:        add '+' term        { $$ = new cBinaryExprNode($1, '+', $3); }
        |   add '-' term        { $$ = new cBinaryExprNode($1, '-', $3); }
        |   term                { $$ = $1; }

term:       term '*' fact       { $$ = new cBinaryExprNode($1, '*', $3); }
        |   term '/' fact       { $$ = new cBinaryExprNode($1, '/', $3); }
        |   term '%' fact       { $$ = new cBinaryExprNode($1, '%', $3); }
        |   fact                { $$ = $1; }

fact:        '(' expr ')'       { $$ = $2; }
        |   INT_VAL             { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL           { $$ = new cFloatExprNode($1); }
        |   varref              { $$ = $1; }
        |   func_call           { $$ = $1; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

// Function that gets called when a semantic error happens
void SemanticError(std::string error)
{
    std::cout << "ERROR: " << error << " on line " << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
