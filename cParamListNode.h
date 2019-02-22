#pragma once
//**************************************
// cParamListNode.h
//
// Defines an AST node for an parameter list.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"

class cParamListNode : public cAstNode
{
    public:
        cParamListNode(cAstNode * expr) : cAstNode()
        {
            AddChild(expr);
        }

        void Insert(cAstNode * expr)
        {
            AddChild(expr);
        }

        int ChildCount() { return NumChildren(); }
        cExprNode* GetArgument(int index)
        {
            return dynamic_cast<cExprNode*>(GetChild(index));
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};