#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cExprNode.h"

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        // return the type of the expr. This will either be int or char 
        // base on the value.
        virtual cDeclNode *GetType()
        {
            if (m_value >= -128 && m_value <= 127)
                return g_SymbolTable.Find("char")->GetDecl();
            else
                return g_SymbolTable.Find("int")->GetDecl();
        }

        // return a string representation of the integer constant
        virtual string NodeType() { return string("int"); }
        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        int m_value;        // value of integer constant (literal)
};
