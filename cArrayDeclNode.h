#pragma once
//**************************************
// cArrayDeclNode.h
//
// Class for declaring an array 
//
// Author: Dayton Wilks
// Date: 2/8/18

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol * type, int size,  cSymbol * name) : cDeclNode()
        {
            AddChild(type);
            m_size = size;
            AddChild(name);
            g_symbolTable.Insert(name);
        }

        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        virtual string GetName() 
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(2));
            return type->GetName();
        }

        virtual string NodeType() { return string("array_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string AttributesToString() 
        {
            return " count=\"" + std::to_string(m_size) + "\"";
        }
    protected:
        int m_size;
};