#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            AddChild(decls);
            AddChild(statements);
        }

        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        virtual string AttributesToString()   
        {
            string result("");
            if (m_size != 0)
            {
                result += " size=\"";
                result += std::to_string(m_size);
                result += "\"";
            }

            return result;
        }

        int GetSize() { return m_size; }
        void SetSize(int s) { m_size = s; }

        cDeclsNode* GetDeclsNode() { return dynamic_cast<cDeclsNode*>(GetChild(0)); }
        cStmtsNode* GetStmtsNode() { return dynamic_cast<cStmtsNode*>(GetChild(1)); }
 
    protected:
        int m_size = 0;
};
