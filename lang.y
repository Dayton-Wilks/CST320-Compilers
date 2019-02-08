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
#include <string>
using std::string;
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
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cSymbol*        symbol;
    cVarExprNode*   varexpr_node;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    cFuncDeclNode*  func_decl;
    cParamListNode* param_list;
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
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <ast_node> array_decl
%type <func_decl> func_decl
%type <func_decl> func_header
%type <func_decl> func_prefix
%type <expr_node> func_call
%type <decls_node> paramsspec
%type <decl_node> paramspec
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

decls:      decls decl          { DebugPrint("Adding DECLS");           $$ = $1; $$->AddChild($2); }
        |   decl                { DebugPrint("Adding DECL");            $$ = new cDeclsNode($1); }
decl:       var_decl ';'        { DebugPrint("Adding var_decl");        $$ = $1; }
        |   struct_decl ';'     { DebugPrint("Adding struct_decl");     $$ = $1; }
        |   array_decl ';'      { DebugPrint("Adding array_decl"); }
        |   func_decl           { DebugPrint("Adding func_decl");       $$ = $1; }
        |   error ';'           { DebugPrint("Adding error"); }

var_decl:   TYPE_ID IDENTIFIER  { DebugPrint("Creating var_decl");      
                                  $$ = new cVarDeclNode($1); 
                                  string temp = yylval.symbol->GetName();
                                  if (g_symbolTable.FindLocal(temp) == nullptr) 
                                  {
                                    yylval.symbol = new cSymbol(temp);
                                    g_symbolTable.Insert(yylval.symbol);
                                  }
                                  $$->AddChild(yylval.symbol); 
                                }
struct_decl:  STRUCT open decls close IDENTIFIER    
                                { DebugPrint("Creating struct_decl"); $$ = new cStructDeclNode($3, $5); $5->SetType(STRUCT); }
array_decl: ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                {  }

func_decl:  func_header ';'
                                { DebugPrint("func_decl 0");        g_symbolTable.DecreaseScope(); }
        |   func_header  '{' decls stmts '}'
                                { DebugPrint("func_decl 1");        $$->Insert($3); $$->Insert($4); g_symbolTable.DecreaseScope(); }
        |   func_header  '{' stmts '}'
                                { DebugPrint("func_decl 2");        $$ = $1; $$->Insert($3); g_symbolTable.DecreaseScope(); }
func_header: func_prefix paramsspec ')'
                                { $$ = $1; $1->Insert($2); }
        |    func_prefix ')'    {  }
func_prefix: TYPE_ID IDENTIFIER '('
                                { DebugPrint("stmts:stmts");        $$ = new cFuncDeclNode($1, $2); g_symbolTable.IncreaseScope(); }
paramsspec: paramsspec ',' paramspec 
                                { DebugPrint("stmts:stmts");        $$ = $1; $$->Insert($3); }
        |   paramspec           { DebugPrint("stmts:stmts");        $$ = new cParamsNode($1); }

paramspec:  var_decl            {  }

stmts:      stmts stmt          { DebugPrint("stmts:stmts");        $$ = $1; $$->Insert($2); }
        |   stmt                { DebugPrint("stmts:stmt");         $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { DebugPrint("Creating IF/ENDIF");  $$ = new cIfNode($3, $5, nullptr); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { DebugPrint("Creating IF/ELSE");   $$ = new cIfNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt 
                                { DebugPrint("Creating WHILE");     $$ = new cWhileNode($3, $5);}
        |   PRINT '(' expr ')' ';'
                                { DebugPrint("stmt:print");         $$ = new cPrintNode($3); }
        |   lval '=' expr ';'   { DebugPrint("Add Assign:EXPR");    $$ = new cAssignNode($1, $3); }
        |   lval '=' func_call ';'   
                                { DebugPrint("Add Assign:FUNC");    $$ = new cAssignNode($1, $3); }
        |   func_call ';'       {  }
        |   block               {  }
        |   RETURN expr ';'     { DebugPrint("Creating RETURN");    $$ = new cReturnNode($2); }
        |   error ';'           {   }

func_call:  IDENTIFIER '(' params ')' 
                                { DebugPrint("Found FuncCall +");   $$ = new cFuncExprNode($1, $3); }
        |   IDENTIFIER '(' ')'  { DebugPrint("Found FuncCall 0");   $$ = new cFuncExprNode($1, nullptr); }

varref:   varref '.' varpart    { DebugPrint("Found VARREF .");     $$ = $1; $$->Insert($3);}
        | varref '[' expr ']'   {  }
        | varpart               { DebugPrint("Found VARREF");       $$ = new cVarExprNode($1); }

varpart:  IDENTIFIER            { DebugPrint("Found VARPART");      $$ = $1; }

lval:     varref                { DebugPrint("Found LVAL");         $$ = $1; }

params:     params ',' param    { DebugPrint("Found Params");       $$ = $1; $$->Insert($3); }
        |   param               { DebugPrint("Found Param");        $$ = new cParamListNode($1); }

param:      expr                {  }

expr:       expr EQUALS addit   { DebugPrint("expr:EQUALS"); }
        |   addit               {   }

addit:      addit '+' term      { DebugPrint("Adding ADDIT +");     $$ = new cExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term      { DebugPrint("Adding ADDIT -");     $$ = new cExprNode($1, new cOpNode('-'), $3); }
        |   term                {  }

term:       term '*' fact       { DebugPrint("Adding TERM *");      $$ = new cExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact       { DebugPrint("Adding TERM */");     $$ = new cExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact       { DebugPrint("Adding TERM *%");     $$ = new cExprNode($1, new cOpNode('%'), $3); }
        |   fact                {  }

fact:        '(' expr ')'       { DebugPrint("Adding (expr)");      $$ = $2; }
        |   INT_VAL             { DebugPrint("Found INT");          $$ = new cIntExprNode(yylval.int_val); }
        |   FLOAT_VAL           { DebugPrint("Found FLOAT");        $$ = new cFloatExprNode(yylval.float_val); }
        |   varref              { DebugPrint("Found VARREF");       $$ = $1; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

