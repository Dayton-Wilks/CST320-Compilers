#pragma once
//**************************************
// cCodeGen.h
//
// Defines a visitor that generates stackl code
//
// Author: Dayton Wilks
// Date: 3/9/2019
//

#include <string>
using std::string;
using std::to_string;

#include "cVisitor.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(string filename) : cVisitor()
        {
            InitOutput(filename);
        }

        ~cCodeGen()
        {
            FinalizeOutput();
        }

        virtual void VisitAllNodes(cAstNode *node)
        {
            node->Visit(this);
        }

        virtual void Visit(cProgramNode* node)
        {
            cBlockNode* block = node->GetBlock();
            cDeclsNode* decls = block->GetDeclsNode();
            cStmtsNode* stmts = block->GetStmtsNode();
            
            int size = block->GetSize();
            size = valMod(size);

            if (decls) decls->Visit(this);

            EmitString("main:\n");
            EmitString("ADJSP " + to_string(size) + "\n");

            if (stmts) stmts->Visit(this);
        }

        virtual void Visit(cIntExprNode* node)
        {
            EmitString("PUSH " + to_string(node->GetValue()) + "\n");
        }

        virtual void Visit(cPrintNode* node)
        {
            // Evaluate Stmt
            VisitAllChildren(node);

            EmitString("CALL @print\n");
            EmitString("POP\n");
            EmitString("POP\n");
        }

        virtual void Visit(cBinaryExprNode* node)
        { // Change Visit Order
            node->GetLeft()->Visit(this);
            node->GetRight()->Visit(this);
            node->GetOp()->Visit(this);
        }

        virtual void Visit(cOpNode* node)
        {
            int op = node->GetOpCode();
            string result("");

            switch (op)
            {
                case NEQUALS:
                    result = "NE\n";
                    break;
                case EQUALS:
                    result = "EQ\n";
                    break;
                case OR:
                    result = "OR\n";
                    break;
                case AND:
                    result = "AND\n";
                    break;
                case '+':
                    result = "PLUS\n";
                    break;
                case '-':
                    result = "MINUS\n";
                    break;
                case '*':
                    result = "TIMES\n";
                    break;
                case '/':
                    result = "DIVIDE\n";
                    break;
                case '%':
                    result = "MOD\n";
                    break;
            }

            EmitString(result.c_str());
        }

        virtual void Visit(cAssignNode* node)
        {
            string result = (node->GetLeft()->GetSize() == 1) ? "POPCVAR " : "POPVAR ";
            result += to_string(node->GetLeft()->GetOffset()) + "\n";

            node->GetRight()->Visit(this);
            EmitString(result);
        }

        virtual void Visit(cVarExprNode* node)
        {
            string result = (node->GetSize() == 1) ? "PUSHCVAR " : "PUSHVAR ";
            result += to_string(node->GetOffset()) + "\n";
            EmitString(result);
        }

        virtual void Visit(cStructDeclNode* node)
        {  /* We do not need to visit children */ }
        

        virtual void Visit(cIfNode* node)
        {
            cStmtsNode* ifChild = node->GetIfStmts();
            cStmtsNode* elseChild = node->GetElseStmts();
            string label1 = GenerateLabel();
            string label2;

            // Calc condition
            node->GetCond()->Visit(this);

            // Create Check
            EmitString("JUMPE @" + label1 + "\n");

            // Insert if stmt code
            ifChild->Visit(this);

            if (elseChild) 
            { // jump over else
                label2 = GenerateLabel();
                EmitString("JUMP @" + label2 + "\n");
            }

            // else or end label
            EmitString(label1 + ":\n");
            
            if (elseChild)
            { // else code and end label
                elseChild->Visit(this);
                EmitString(label2 + ":\n");
            }
        }

        virtual void Visit(cWhileNode* node)
        {
            string label1 = GenerateLabel();
            string label2 = GenerateLabel();

            // start label
            EmitString(label1 + ":\n");

            // cond calc
            node->GetCond()->Visit(this);

            // cond check, jump tp label2
            EmitString("JUMPE @" + label2 + "\n");

            // while code
            node->GetStmts()->Visit(this);

            // restart loop
            EmitString("JUMP @" + label1 + "\n");

            // end label
            EmitString(label2 + ":\n");
        }

        virtual void Visit(cFuncDeclNode* node)
        {
            cStmtsNode * stmts = node->GetStmts();
            cDeclsNode* decls = node->GetLocals();
            cDeclsNode* params = node->GetParams();
            if (stmts || decls) // Is valid Function
            {
                // Print function label
                EmitString("$" + node->GetName() + ":\n");

                // Load Params, Adj sp, add stmt code
                if (params) params->Visit(this); 
                if (decls) EmitString("ADJSP " + to_string(decls->GetSize()) + "\n");
                if (stmts) stmts->Visit(this);
            }
        }

        virtual void Visit(cParamsNode* node)
        {
            cDeclNode* arg;
            for (int ii = 0; ii < node->NumArgs(); ++ii)
            {
                arg = node->GetArg(ii);
                EmitString("PUSHVAR " + to_string(-1 * (arg->GetOffset() + 12)) + "\n");
            }
        }

        virtual void Visit(cFuncExprNode* node)
        {
            VisitAllChildren(node);
            EmitString("CALL @$" + node->GetName() + "\n");
            int num = node->ArgSize();
            if (num != 0)
                EmitString("POPARGS " + to_string(num) + "\n");
        }

        virtual void Visit(cReturnNode* node)
        {
            VisitAllChildren(node);
            EmitString("RETURNV\n");
        }

    private:
        inline int valMod(int val)
        {
            int mod = val % 4;
            if (mod) val += 4 - mod;
            return val;
        }
};
