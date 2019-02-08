#pragma once
//**************************************
// cWhileNode.h
//
// Defines an AST node for a while node, inherits from cStmtNode 
// so a while loop can be used anywhere a stmt can.
//
// Author: Dayton Wilks
//

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