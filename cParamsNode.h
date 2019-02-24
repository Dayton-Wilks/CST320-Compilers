#pragma once
//**************************************
// cParamsNode.h
//
// Defines an AST node for a functions parameters
//
// Author: Dayton Wilks
//

#include "cAstNode.h"

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

        int ChildCount() { return NumChildren(); }
        cVarDeclNode * GetParam(int index) 
        {
            return dynamic_cast<cVarDeclNode*>(GetChild(index));
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};