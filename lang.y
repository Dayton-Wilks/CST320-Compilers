%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard & Dayton Wilks
//

#include <iostream>
#include <stdlib.h>
#include <string>
using std::string;
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"
#include "cSemantic.h"

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
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cSymbol*        symbol;
    cVarExprNode*   varexpr_node;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    cFuncDeclNode*  func_decl;
    cParamsNode*    param_node;
    cParamListNode* param_list;
    }

%{
    bool g_semanticErrorHappened = false;

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
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <decl_node> array_decl
%type <func_decl> func_decl
%type <func_decl> func_header
%type <func_decl> func_prefix
%type <expr_node> func_call
%type <param_node> paramsspec
%type <param_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <varexpr_node> lval
%type <param_list> params
%type <param_list> param
%type <expr_node> expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <varexpr_node> varref
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

open:   '{'                     { g_symbolTable.IncreaseScope(); }

close:  '}'                     { g_symbolTable.DecreaseScope(); }

decls:      decls decl          { $$ = $1; $$->Insert($2); }
        |   decl                { $$ = new cDeclsNode($1); }
decl:       var_decl ';'        { $$ = $1; }
        |   struct_decl ';'     { $$ = $1; }
        |   array_decl ';'      { $$ = $1; }
        |   func_decl           { $$ = $1; }
        |   error ';'           {   }

var_decl:   TYPE_ID IDENTIFIER  { $$ = new cVarDeclNode($1, yylval.symbol); }
struct_decl:  STRUCT open decls close IDENTIFIER    
                                { $$ = new cStructDeclNode($3,$5); $5->SetType(STRUCT); }
array_decl: ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                { 
                                    $$ = new cArrayDeclNode($2, $4, $6); 
                                    $6->SetType(ARRAY); 
                                }

func_decl:  func_header ';'
                                { 
                                    $$= $1; $$->Finalize(nullptr, nullptr);
                                    g_symbolTable.DecreaseScope(); 
                                }
        |   func_header  '{' decls stmts '}'
                                { 
                                    $$ = $1; $$->Finalize($3, $4);//$$->Insert($3); $$->Insert($4); 
                                    g_symbolTable.DecreaseScope(); 
                                }
        |   func_header  '{' stmts '}'
                                { 
                                    $$ = $1; $$->Finalize(nullptr, $3); //$$->Insert(nullptr); $$->Insert($3); 
                                    g_symbolTable.DecreaseScope(); 
                                }
func_header: func_prefix paramsspec ')'
                                { $$ = $1; $1->Insert($2); }
        |    func_prefix ')'    { $$ = $1; $1->Insert(nullptr); }
func_prefix: TYPE_ID IDENTIFIER '('
                                { 
                                    $$ = new cFuncDeclNode($1, $2);
                                    //$2->SetType(TYPE_ID); 
                                    g_symbolTable.IncreaseScope(); 
                                }
paramsspec: paramsspec ',' paramspec 
                                { $$ = $1; $$->Insert($3); }
        |   paramspec           { $$ = new cParamsNode($1); }

paramspec:  var_decl            {  }

stmts:      stmts stmt          { $$ = $1; $$->Insert($2); }
        |   stmt                { $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, nullptr); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt 
                                { $$ = new cWhileNode($3, $5);}
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   lval '=' expr ';'   { $$ = new cAssignNode($1, $3); }
        |   lval '=' func_call ';'   
                                { $$ = new cAssignNode($1, $3); }
        |   func_call ';'       {  }
        |   block               {  }
        |   RETURN expr ';'     { $$ = new cReturnNode($2); }
        |   error ';'           {   }

func_call:  IDENTIFIER '(' params ')' 
                                { $$ = new cFuncExprNode($1, $3); }
        |   IDENTIFIER '(' ')'  { $$ = new cFuncExprNode($1, nullptr); }

varref:   varref '.' varpart    { $$ = $1; $$->Insert($3); }
        | varref '[' expr ']'   { $$ = $1; $$->Insert($3); }
        | varpart               { $$ = new cVarExprNode($1); }

varpart:  IDENTIFIER            { $$ = $1;}

lval:     varref                { $$ = $1; }

params:     params ',' param    { $$ = $1; $$->Insert($3); }
        |   param               { $$ = new cParamListNode($1); }

param:      expr                {  }

expr:       expr EQUALS addit   { $$ = new cBinaryExprNode($1,new cOpNode(EQUALS),$3); }
        |   addit               {   }

addit:      addit '+' term      { $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term      { $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
        |   term                {  }

term:       term '*' fact       { $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact       { $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact       { $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
        |   fact                {  }

fact:        '(' expr ')'       { $$ = $2; }
        |   INT_VAL             { $$ = new cIntExprNode(yylval.int_val); }
        |   FLOAT_VAL           { $$ = new cFloatExprNode(yylval.float_val); }
        |   varref              { $$ = $1; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;//std::cout<< "1 "<<$1->GetName()<<std::endl;
}

void SemanticError(std::string error)
{
    std::cout << "ERROR: " << error << "on line " 
            << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}


