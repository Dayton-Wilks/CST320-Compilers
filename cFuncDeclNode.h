#pragma once
//**************************************
// cDeclsNode.h
//
// 
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cFuncDeclNode(cSymbol * type, cSymbol * name) : cDeclNode()
        {
            AddChild(type);
            AddChild(name);
        }

        // Add a decl to the list
        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};