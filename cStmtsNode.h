#pragma once
//**************************************
// cStmtsNode.h
//
// Defines AST node that represents a list of statements
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"

class cStmtsNode : public cAstNode
{
    public:
        // create an empty statement list
        cStmtsNode() : cAstNode()
        {
        }

        // param is first stmt in the list of statements
        cStmtsNode(cStmtNode *stmt) : cAstNode()
        {
            AddChild(stmt);
        }

        // Add a statement to the list
        void Insert(cStmtNode *stmt)
        {
            AddChild(stmt);
        }

        virtual string NodeType() { return string("stmts"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
