#pragma once
//**************************************
// cVarExprNode.h
//
// Defines AST node for a variable reference
//
// Inherits from cExprNode so variable refs can be used in expressions`
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include <assert.h>

#include "cSymbol.h"
#include "cAstNode.h"
#include "cExprNode.h"
#include "cIntExprNode.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the symbol for the variable
        cVarExprNode(cSymbol *name)
            : cExprNode()
        {
            cSymbol *sym = g_SymbolTable.Find(name->GetName());
            if (sym == nullptr)
            {
                string error("");
                error += "Symbol " + name->GetName() + " not defined";
                SemanticError(error);
            }

            AddChild(name);
        }

        // called for the fields in struct refs
        void AddElement(cSymbol *name)
        {
            cDeclNode *type = GetType();

            if (type->IsStruct())
            {
                cStructDeclNode *decl;
                decl = dynamic_cast<cStructDeclNode *>(type);
                assert(decl != nullptr);

                cSymbol* field = decl->GetField(name->GetName());
                if (field != nullptr)
                {
                    AddChild(field);
                }
                else
                {
                    SemanticError( name->GetName() + " is not a field of " +
                            GetName());
                }
            }
            else
            {
                SemanticError( GetName() + " is not a struct" );
            }
        }

        void AddElement(cExprNode *index)
        {
            if (!index->GetType()->IsInt())
            {
                SemanticError( "Index of " + GetName() + " is not an int");
                return ;
            }

            cDeclNode *type = GetType();
            if (!type->IsArray())
                SemanticError( GetName() + " is not an array" );
            else
                AddChild(index);
        }

        // return a string representation of the name of the var
        virtual string GetName()
        {
            string name("");
            cSymbol* sym;
            cIntExprNode* intVal;

            int index = 0;

            sym = dynamic_cast<cSymbol*>(GetChild(index));

            name += sym->GetName();

            for (index=1; index<NumChildren(); index++)
            {
                // using the result of a dynamic cast to determine the
                // type of the things is a kludge.
                intVal = dynamic_cast<cIntExprNode*>(GetChild(index));
                if (intVal != nullptr)
                {
                    name += "[]";
                }
                else
                {
                    sym = dynamic_cast<cSymbol*>(GetChild(index));
                    name += "." + sym->GetName();
                }
            }

            return name;
        }
            
        // Return the type of the var
        // This is complicated by the fact the this may be an array or struct
        virtual cDeclNode* GetType()
        {
            int index_depth = 0;
            cSymbol* sym;
            int index = NumChildren()-1;

            // search for the last non-index
            cAstNode *node = GetChild(index);
            sym = dynamic_cast<cSymbol*>(node);
            while (sym == nullptr)
            {
                index_depth++;
                index--;
                node = GetChild(index);
                sym = dynamic_cast<cSymbol*>(node);
            }

            if (sym->GetDecl()->GetType()->IsArray() && index_depth > 0)
            {
                cArrayDeclNode *arr = dynamic_cast<cArrayDeclNode*>
                    (sym->GetDecl()->GetType());
                return arr->GetType(index_depth);
            }
            else
            {
                return sym->GetDecl()->GetType();
            }
        }

        // Return the declaration of the var
        virtual cDeclNode* GetDecl()
        {
            int index_depth = 0;
            cSymbol* sym;
            cAstNode *node;
            int index = NumChildren()-1;
            node = GetChild(index);
            sym = dynamic_cast<cSymbol*>(node);
            while (sym == nullptr)
            {
                index_depth++;
                index--;
                node = GetChild(index);
                sym = dynamic_cast<cSymbol*>(node);
            }

            return sym->GetDecl();
        }

        // return a string representation of the node
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
