#include "cAstNode.h"
#include "cStmtNode.h"

class cAssignNode : public cStmtNode
{
public:
    cAssignNode(cVarExprNode * left, cExprNode * right) : cStmtNode() 
    {
        AddChild(left);
        AddChild(right);
    }
    virtual string NodeType() { return string("assign"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};