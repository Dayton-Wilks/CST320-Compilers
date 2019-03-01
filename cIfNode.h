#pragma once
//**************************************
// cIfNode.h
//
// Defines AST node for an if statement
//
// Inherits from cStmtNode so that if statements can be included in lists of
// statements
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"
#include "cExprNode.h"

class cIfNode : public cStmtNode
{
    public:
        // params are the condition, the list of statments for the if part,
        // and the list of statements for the else part. The else part can
        // be nullptr
        cIfNode(cExprNode *cond, cStmtsNode *ifStmt, cStmtsNode *elseStmt)
        {
            AddChild(cond);
            AddChild(ifStmt);
            AddChild(elseStmt);
        }

        // Return a string representation of the node
        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
