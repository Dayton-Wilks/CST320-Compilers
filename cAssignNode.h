#pragma once
//**************************************
// cAssignNode.h
//
// Class for an assignment operation
//
// Author: Dayton Wilks
// Date: 2/8/18
#include "cAstNode.h"

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