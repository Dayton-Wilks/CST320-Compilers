#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"

class cVarRefNode : public cExprNode
{
    public:
        // param is the first decl in this decls
        cVarRefNode(cAstNode * sym) : cExprNode()
        {
            AddChild(sym);
        }

        // Add a decl to the list
        void Insert(cAstNode *sym)
        {
            AddChild(sym);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};