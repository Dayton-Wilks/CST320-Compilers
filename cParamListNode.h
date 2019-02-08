#pragma once
//**************************************
// cParamListNode.h
//
// Defines an AST node for an parameter list.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"

class cParamListNode : public cExprNode
{
    public:
        cParamListNode(cExprNode * expr) : cExprNode()
        {
            AddChild(expr);
        }

        void Insert(cExprNode * expr)
        {
            AddChild(expr);
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};