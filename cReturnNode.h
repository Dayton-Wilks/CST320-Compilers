#include "cAstNode.h"
#include "cStmtNode.h"

class cReturnNode : public cStmtNode
{
    public:
        cReturnNode(cExprNode * child) : cStmtNode() 
        {
            AddChild(child);
        }
        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};