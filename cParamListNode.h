#include "cAstNode.h"
#include "cExprNode.h"

class cParamListNode : public cExprNode
{
    public:
        // param is the first decl in this decls
        cParamListNode(cExprNode * expr) : cExprNode()
        {
            AddChild(expr);
        }

        // Add a decl to the list
        void Insert(cExprNode * expr)
        {
            AddChild(expr);
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};