#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        int NumDecls() { return NumChildren(); }

        cDeclNode *GetDecl(int index)
        {
            return (cDeclNode *)GetChild(index);
        }

        // return the XML node name
        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()   
        {
            string result("");
            if (m_size != 0)
            {
                result += " size=\"";
                result += std::to_string(m_size);
                result += "\"";
            }

            return result;
        }

        int GetSize() { return m_size; }
        void SetSize(int s) { m_size = s; }
 
    protected:
        int m_size = 0;
};
