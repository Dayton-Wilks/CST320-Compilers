#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for a variable declaration, inherits from cDeclnNode 
// so a var delc can be used anywhere a decl can be made.
//
// Author: Dayton Wilks
//

#include <string>
using std::string;
#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
//#include "cSemantic.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * sym, cSymbol * sym2) : cDeclNode()
        {
            // Type
            AddChild(sym);

            // Name
            string temp = sym2->GetName();
            if (g_symbolTable.FindLocal(temp) == nullptr) 
            {
                if (g_symbolTable.Find(temp) != nullptr)
                {
                    sym2 = new cSymbol(temp);
                }
                sym2->setDecl(this);
                g_symbolTable.Insert(sym2);
                
                AddChild(sym2);
            }
            else
            {
               SemanticError(string("Symbol ") + temp + " already defined in current scope ");
               CHECK_ERROR();
            }
            
        }

        void Insert(cSymbol *decl)
        {
            AddChild(decl);
        }

        virtual bool IsVar() { return true; }

        virtual bool IsInt()
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(0));
            cSymbol * toReturn = g_symbolTable.Find(type->GetName());
            return toReturn->getDecl()->IsInt();
        }

        virtual bool IsChar()
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(0));
            cSymbol * toReturn = g_symbolTable.Find(type->GetName());
            return toReturn->getDecl()->IsChar();
        }

        virtual bool IsFloat()
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(0));
            cSymbol * toReturn = g_symbolTable.Find(type->GetName());
            return toReturn->getDecl()->IsFloat();
        }

        virtual string GetName() 
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(0));
            return type->GetName();
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};