#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cAstNode *decl) : cDeclNode()
        {
            AddChild(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            //fprintf(stderr, "DeclInsert<%i>\n", decl);
            AddChild(decl);
        }

        virtual bool HasChild(cSymbol* sym)
        {
            int count = NumChildren();
            for (int ii = 0; ii < count; ++ii)
            {
                string a = sym->GetName();
                cDeclNode* child = dynamic_cast<cDeclNode*>(GetChild(ii));
                string b = child->GetName();
                if (a == b)
                    return true;
            }
            return false;
        }

        virtual cSymbol* GetChildSym(cSymbol* sym)
        {
            int count = NumChildren();
            for (int ii = 0; ii < count; ++ii)
            {
                cDeclNode* child = dynamic_cast<cDeclNode*>(GetChild(ii));
                if (sym->GetName() == child->GetName())
                {
                    return child->GetNameSym();
                }
            }
            return nullptr;
        }

        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string GetName() { return "delcsNode"; }
};
