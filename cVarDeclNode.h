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

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * sym, cSymbol * sym2) : cDeclNode()
        {
            AddChild(sym);

            string temp = sym2->GetName();
            if (g_symbolTable.FindLocal(temp) == nullptr) 
            {
                sym2 = new cSymbol(temp);
                g_symbolTable.Insert(sym2);
            }
            AddChild(sym2);
        }

        void Insert(cSymbol *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};