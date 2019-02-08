#pragma once
//**************************************
// cIfNode.h
//
// Defines an AST node for an if operation.
//
// Inherits from cStmtNode so that if can be used anywhere 
// statements are used.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"

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