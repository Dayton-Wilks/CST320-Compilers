#pragma once
//**************************************
// cFloatExprNode.h
//
// Defines AST node for a float constant (literal)
// Inherits from cExprNode so float constants can be used in any expression.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include "cAstNode.h"
#include "cExprNode.h"

class cFloatExprNode : public cExprNode
{
    public:
        // param is the value of the constant (literal)
        cFloatExprNode(double value)
        {
            m_value = value;
        }

        // Return the type of the expr. This is the type float
        virtual cDeclNode *GetType()
        {
            return g_SymbolTable.Find("float")->GetDecl();
        }

        // Return a string representation of the node
        virtual string AttributesToString()
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }

        virtual string NodeType() { return string("float"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        double m_value;     // value of the constant (literal)
};
