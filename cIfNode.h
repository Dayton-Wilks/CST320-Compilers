#include "cAstNode.h"
#include "cStmtNode.h"

class cIfNode : public cStmtNode
{
public:
    cIfNode(cExprNode * cond, cStmtsNode * ifWork, cStmtsNode * elseWork) : cStmtNode() 
    {
        AddChild(cond);
        AddChild(ifWork);
        AddChild(elseWork);
    }
    virtual string NodeType() { return string("if"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};