#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the first decl in this decls
        cVarExprNode(cSymbol * sym) : cExprNode()
        {
            AddChild(sym);
        }

        // Add a decl to the list
        void Insert(cSymbol *sym)
        {
            AddChild(sym);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};