#pragma once
//**************************************
// cCodeGen.h
//
// Defines a visitor that calculates size and offset
//
// Author: Dayton Wilks
// Date: 3/1/2019
//

#include <string>
using std::string;
using std::to_string;

#include "cVisitor.h"
#include "emit.h"

// EmitString("\n");

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
            PostVisitAllNodes(node);
        }

        virtual void Visit(cProgramNode* node)
        {
            EmitString("main:\n");
            if (t) fprintf(stderr, "cProgramNode\n");
        }

        virtual void Visit(cIntExprNode* node)
        {
            if (t) fprintf(stderr, "cIntExprNode\n");
            EmitString("PUSH ");
            EmitInt(node->GetValue());
            EmitString("\n");
        }

        virtual void Visit(cPrintNode* node)
        {
            if (t) fprintf(stderr, "cPrintNode\n");
            VisitAllChildren(node);

            EmitString("CALL @print\n");
            EmitString("POP\n");
            EmitString("POP\n");
        }

        virtual void Visit(cBinaryExprNode* node)
        {
            if (t) fprintf(stderr, "cBinaryExprNode\n");
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
            }

            EmitString(result.c_str());
        }

        virtual void Visit(cVarDeclNode* node)
        {
            string result("ADJSP ");
            int offset = node->GetOffset();
            int size = node->GetSize();
            int mod = offset % 4;

            if (mod && size == 1) 
            {
                result += to_string(size) + "\n";
                if (t) fprintf(stderr, "cVarDeclNode ERR - %s", result.c_str());
                return;
            }
            mod = size % 4;
            if (size == 1) 
                size = 4;
            else if 
                (mod) size += 4 - mod;

            result += to_string(size) + "\n";

            if (t) fprintf(stderr, "cVarDeclNode - %s", result.c_str());

            EmitString(result);
        }

        virtual void Visit(cAssignNode* node)
        {
            string result = (node->GetLeft()->GetSize() == 1) ? "POPCVAR " : "POPVAR ";
            result += to_string(node->GetLeft()->GetOffset()) + "\n";

            if (t) fprintf(stderr, "cAssignNode %s", result.c_str());

            node->GetRight()->Visit(this);
            EmitString(result);
        }

        virtual void Visit(cVarExprNode* node)
        {
            string result = (node->GetSize() == 1) ? "PUSHCVAR " : "PUSHVAR ";
            result += to_string(node->GetOffset()) + "\n";

            if (t) fprintf(stderr, "cVarExprNode <%d> %s", node->GetSize(), result.c_str());

            EmitString(result);
        }

        virtual void Visit(cStructDeclNode* node)
        {
            
        }

    private:
        bool t = false;
};