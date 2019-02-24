#pragma once
//**************************************
// cFloatExprNode.h
//
// Defines an AST node for an float constant (literals).
//
// Inherits from cExprNode so that float constants can be used anywhere 
// expressions are used.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cFloatExprNode : public cExprNode
{
    public:
        cFloatExprNode(float value) : cExprNode()
        {
            m_value = value;
        }

        virtual cDeclNode * GetType()
        {
            return g_symbolTable.Find("float")->getDecl();
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }

        virtual string NodeType() { return string("float"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        float m_value;        // value of integer constant (literal)
};