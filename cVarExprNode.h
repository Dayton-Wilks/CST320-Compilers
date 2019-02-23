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
            if (g_symbolTable.Find(sym->GetName()) == nullptr)
            {
                SemanticError(string("Symbol ") + sym->GetName() + " not defined ");
                CHECK_ERROR();
            }
            else
                AddChild(sym);
        }

        void Insert(cAstNode * node)
        {
            cSymbol* varSym = dynamic_cast<cSymbol*>(GetChild(NumChildren() - 1));
            cDeclNode* symDecl = varSym->getDecl();

            if (symDecl)
            {
                if(!symDecl->IsStruct())
                {
                    SemanticError(symDecl->GetName() + " is not a struct ");
                    CHECK_ERROR();
                }
                else
                {
                    string t = symDecl->GetName();
                    cSymbol * toReturn = g_symbolTable.Find(t);
                    string d = toReturn->GetName();
                    cDeclNode* dec = toReturn->getDecl();

                    //fprintf(stderr, "leftSym:<%i> rightSym:<%i>\n", varSym, node);

                    // symbol -> cVarDeclNode -> 

                    //cSymbol * first = dynamic_cast<cSymbol*>(dynamic_cast<cVarDeclNode*>(symDecl)->GetType());

                    //fprintf(stderr, "first <%s>\n", first->GetName().c_str());

                }
            }

            AddChild(node);
        }

        virtual cDeclNode * GetType()
        {
            cSymbol * sym = dynamic_cast<cSymbol*>(GetChild(0));
            if (sym == nullptr) 
            {
                //fprintf(stderr, "MISSING TYPE");
                return nullptr;
            }
            return sym->getDecl();
        }

        virtual bool IsVar() { return true; }
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};