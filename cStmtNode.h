#pragma once
//**************************************
// cStmtNode.h
//
// Defines abstract class for a single statment
//
// NOTE: all statement types must inherit from this class
//
// Author: Phil Howard & Dayton Wilks
// phil.howard@oit.edu
//

#include "cAstNode.h"

class cStmtNode : public cAstNode
{
    public:
        cStmtNode() : cAstNode() {}
        virtual string NodeType() { return string("stmt"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
