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
                //CHECK_ERROR();
            }
            else
                AddChild(sym);
        }

        void Insert(cAstNode * node)
        {
            cSymbol* varSym = dynamic_cast<cSymbol*>(GetChild(NumChildren() - 1));
            cSymbol* symNode = dynamic_cast<cSymbol*>(node);

            // if (symNode->GetName() == "a")
            //     SemanticError(string(" check "));

            cDeclNode* symDecl = varSym->getDecl();

            //SemanticError(varSym->GetName() + " test ");

            //if (symDecl)
            {
                if(!symDecl || !symDecl->IsStruct())
                {
                    string err;
                    int count = NumChildren();
                    for (int ii = 0; ii < count; ++ii)
                    {
                        err += dynamic_cast<cSymbol*>(GetChild(ii))->GetName();
                        if (ii != count - 1) err += ".";
                    }
                    //err += varSym->GetName();

                    SemanticError(err + " is not a struct ");
                    //CHECK_ERROR();
                }
                else 
                {
                    cSymbol * test = dynamic_cast<cVarDeclNode*>(symDecl)->GetType();
                    cDeclNode* test2 = test->getDecl(); // struct decl made here

                    if (!test2->HasChild(dynamic_cast<cSymbol*>(node)))
                    {
                        SemanticError(symNode->GetName() + " is not a field of " + varSym->GetName() + " ");
                        //CHECK_ERROR();
                    }
                    else
                    {
                        //SemanticError(string("Insert <" + std::to_string((long)test2->GetChildSym(dynamic_cast<cSymbol*>(node))) + ">"));
                        //fprintf(stderr, "Insert <%i>\n", test2->GetChildSym(dynamic_cast<cSymbol*>(node)));
                        cSymbol* t = test2->GetChildSym(dynamic_cast<cSymbol*>(node));
                        //if (t->getDecl()->IsStruct())
                        {
                            //SemanticError(string(" Alt Insert <") + t->GetName() + "> ");
                            AddChild(t);
                            return;
                        }
                    }
                }
            }
            //else
            {
                AddChild(node);
            }
        }

        virtual cDeclNode * GetType()
        {
            int count = NumChildren();

            cDeclNode* node = (count) ? 
                dynamic_cast<cSymbol*>(GetChild(count - 1))->getDecl() 
                : nullptr;
            return node;

            // cSymbol * sym;
            // if (count == 0) 
            // {
            //     return nullptr;
            // }
            // sym = dynamic_cast<cSymbol*>(GetChild(count - 1));
            // return sym->getDecl();
        }



        virtual bool IsVar() { return true; }
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};