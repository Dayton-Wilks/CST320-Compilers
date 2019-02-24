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
            AddChild(sym);

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
            }
        }

        void Insert(cSymbol *decl)
        {
            AddChild(decl);
        }

        virtual bool IsVar() { return true; }

        virtual bool IsInt()
        {
            return GetType()->getDecl()->IsInt();
        }

        virtual bool IsChar()
        {
            return GetType()->getDecl()->IsChar();
        }

        virtual bool IsFloat()
        {
            return GetType()->getDecl()->IsFloat();
        }

        virtual bool IsStruct()
        {
            return GetType()->getDecl()->IsStruct();
        }

        cSymbol* GetType() { return dynamic_cast<cSymbol*>(GetChild(0));}

        virtual string GetName() 
        {
            cSymbol * type = dynamic_cast<cSymbol*>(GetChild(1));
            return type->GetName();
        }

        virtual cSymbol* GetNameSym()
        {
            return dynamic_cast<cSymbol*>(GetChild(1));
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};