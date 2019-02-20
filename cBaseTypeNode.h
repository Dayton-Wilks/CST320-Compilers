#pragma once
//*****************************
// cBaseTypeNode.h
// Dayton Wilks
//

#include <string>
using std::string;
#include "cAstNode.h"
#include "cDeclNode.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(string n, int s, bool isFloat) : cDeclNode() 
        {
            m_name = n;
            m_size = s;
            m_isFloat = isFloat;
        }

        virtual bool IsFloat() { return m_isFloat; }
        virtual bool IsInt() { return !m_isFloat && m_size != 1; }
        virtual bool IsChar() { return m_size == 1; }
        virtual int GetSize() { return m_size; }
        virtual cDeclNode * GetType() { return this; }
        virtual string GetName() {return m_name; }
        virtual bool IsType() { return true; }

        virtual string NodeType() { return string("decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        string m_name;
        int m_size;
        bool m_isFloat;
};