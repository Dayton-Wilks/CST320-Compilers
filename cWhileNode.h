#pragma once
//**************************************
// cWhileNode.h
//
// Defines AST node for a while statement
//
// Inherits from cStmtNode because this is a statement
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:
        // params are the condition and the statement
        cWhileNode(cExprNode *cond, cStmtNode *stmt)
            : cStmtNode()
        {
            AddChild(cond);
            AddChild(stmt);
        }

        // return a string representation of the node
        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
