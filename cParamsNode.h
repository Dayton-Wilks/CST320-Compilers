#pragma once
//**************************************
// cParamsNode.h
//
// Defines an AST node for a functions parameters
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
// #include "cDeclsNode.h"
// #include "cSymbol.h"

class cParamsNode : public cAstNode
{
    public:
        cParamsNode(cAstNode * decl) : cAstNode() 
        {
            AddChild(decl);
        }

        void Insert(cAstNode * decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};