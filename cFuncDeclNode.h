#pragma once
//**************************************
// cFuncDeclNode.h
//
// Creates a function declaration
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cFuncDeclNode(cSymbol * type, cSymbol * name) : cDeclNode()
        {
            AddChild(type);
            AddChild(name);

            g_symbolTable.Insert(name);
        }

        // Add a decl to the list
        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        virtual cDeclNode * GetType()
        {
            cSymbol * t = dynamic_cast<cSymbol*>(GetChild(0));
            return t->getDecl();
        }

        virtual string GetName()
        {
            cSymbol * sym = dynamic_cast<cSymbol*>(GetChild(1));
            return sym->GetName();
        }

        virtual bool IsFunc() { return true; }

        cParamsNode* GetParams() { return dynamic_cast<cParamsNode*>(GetChild(2)); }
        bool HasBody() { return GetChild(3) || GetChild(4); }

        void Finalize(cDeclsNode * decls, cStmtsNode* stmts) 
        {
            AddChild(decls);
            AddChild(stmts);

            cSymbol* nameSym = dynamic_cast<cSymbol*>(GetChild(1));

            cDeclNode* newDecl = GetType();
            cFuncDeclNode* oldDecl = dynamic_cast<cFuncDeclNode*>(nameSym->getDecl());

            if (oldDecl)
            {
                // Return Type
                if (newDecl->GetName() != oldDecl->GetType()->GetName())
                {
                    SemanticError(nameSym->GetName() + " previously defined with different return type ");
                    //CHECK_ERROR();
                }
                
                // Number of Params
                cParamsNode* newParams = GetParams();
                cParamsNode* oldParams = oldDecl->GetParams();
                if (newParams && oldParams)
                {
                    if (newParams->ChildCount() != oldParams->ChildCount())
                    {
                        SemanticError(nameSym->GetName() + " redeclared with a different number of parameters ");
                    }
                    else if (newParams->ChildCount())
                    {
                        for (int ii = 0; ii < newParams->ChildCount(); ++ii)
                        {
                            if (newParams->GetParam(ii)->GetType()->GetName() != oldParams->GetParam(ii)->GetType()->GetName())
                            {
                                ii = newParams->ChildCount();
                                SemanticError(nameSym->GetName() + " previously defined with different parameters ");
                            }
                        }
                    }
                }

                // check if body already defined
                if (HasBody() && oldDecl->HasBody())
                {
                    SemanticError(nameSym->GetName() + " already has a definition ");
                }
            }

            nameSym->setDecl(this); 
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};