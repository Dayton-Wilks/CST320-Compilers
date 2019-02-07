#include "cAstNode.h"
#include "cStmtNode.h"

class cWhileNode : public cStmtNode
{
public:
    cWhileNode(cExprNode * cond, cStmtNode * work) : cStmtNode() 
    {
        AddChild(cond);
        AddChild(work);
    }
    virtual string NodeType() { return string("while"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};