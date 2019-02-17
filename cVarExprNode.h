#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for a var expr, inherits from cExprNode 
// so a var expr can be used anywhere a expr can.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cExprNode.h"
//#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol * sym) : cExprNode()
        {
            AddChild(sym);
        }

        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};