#pragma once
//**************************************
// cFuncExprNode.h
//
// Defines an AST node for an function.
//
// Inherits from cExprNode so that function can be used anywhere 
// expressions are used.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cExprNode.h"

class cFuncExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cFuncExprNode(cSymbol * name, cParamListNode * params) : cExprNode()
        {
            AddChild(name);
            AddChild(params);
        }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};