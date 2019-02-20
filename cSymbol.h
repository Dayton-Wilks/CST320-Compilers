#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Later labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
using std::cerr;
using std::endl;
#include <string>

using std::string;
#include "cAstNode.h"
//#include "tokens.h"

// #define VAR_TYPE 0
// #define STRUCT_TYPE 1
// #define ARRAY_TYPE 2

class cSymbol : public cAstNode
{
    public:
        // param is name of symbol
        cSymbol(string name, int type = 258) : cAstNode()
        {
            m_id = ++nextId;        // get next available ID
            //cerr << name << "-" << m_id << endl;
            m_name = name;
            m_type = type;
            m_declNode = nullptr;
        }

        // return name of symbol
        string GetName() { return m_name; }
        short  GetType() { return m_type; }
        void   SetType(int v) { m_type = v; } 

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        void setDecl(cDeclNode * node)
        {
            m_declNode = node;
        }

        cDeclNode * getDecl()
        {
            return m_declNode;
        }

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        static long long nextId;        // Next avail symbol ID
        long long m_id;                 // Unique ID for this symbol
        string m_name;                  // name of symbol
        int m_type;
        cDeclNode * m_declNode;
};
