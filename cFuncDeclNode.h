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
//#include "cSymbol.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cFuncDeclNode(cSymbol * type, cSymbol * name) : cDeclNode()
        {
            AddChild(type);
            AddChild(name);

            // cDeclNode* newDecl = type->getDecl();
            // cFuncDeclNode* oldDecl = dynamic_cast<cFuncDeclNode*>(name->getDecl());

            // if (oldDecl && newDecl->GetName() != oldDecl->GetType()->GetName())
            // {
            //     // Return Type
            //     if (newDecl->GetName() != oldDecl->GetType()->GetName())
            //     {
            //         SemanticError(name->GetName() + " previously defined with different return type ");
            //         CHECK_ERROR();
            //     }
                
            //     // Number of Params
            //     if (newDecl->ChildCount() != oldDecl->ChildCount())
            //     {
            //         SemanticError(name->GetName() + " redeclared with different number of parameters ");
            //         CHECK_ERROR();
            //     }
            // }

            //name->setDecl(this);
            g_symbolTable.Insert(name);
        }

        // Add a decl to the list
        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        // void SetParams(cParamsNode* params) 
        // {

        //     AddChild(params);
        // }

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

        // void SetBody(cDeclsNode * decls, CStmtNode* stmts) { AddChild(decls); AddChild(stmts); FinalizeDecl();}

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
                //printf("Name <%s> - New <%d> - Old <%d>\n", GetName().c_str(), GetParams()->ChildCount(), oldDecl->GetParams()->ChildCount());
                cParamsNode* newParams = GetParams();
                cParamsNode* oldParams = oldDecl->GetParams();
                if (newParams && oldParams)
                {
                    if (newParams->ChildCount() != oldParams->ChildCount())
                    {
                        SemanticError(nameSym->GetName() + " redeclared with a different number of parameters ");
                        //CHECK_ERROR();
                    }
                    else if (newParams->ChildCount())
                    {
                        for (int ii = 0; ii < newParams->ChildCount(); ++ii)
                        {
                            if (newParams->GetParam(ii)->GetType()->GetName() != oldParams->GetParam(ii)->GetType()->GetName())
                            {
                                ii = newParams->ChildCount();
                                SemanticError(nameSym->GetName() + " previously defined with different parameters ");
                                //CHECK_ERROR();
                            }
                        }
                    }
                }

                // check if body already defined
                if (HasBody() && oldDecl->HasBody())
                {
                    SemanticError(nameSym->GetName() + " already has a definition ");
                    //CHECK_ERROR();
                }
            }

            nameSym->setDecl(this); 
        }

        //int ChildCount() { return NumChildren(); }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};