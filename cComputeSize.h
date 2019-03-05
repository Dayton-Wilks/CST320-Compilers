#pragma once
//**************************************
// cComputeSize.h
//
// Defines a visitor that calculates size and offset
//
// Author: Dayton Wilks
// Date: 3/1/2019
//

#include <string>
using std::string;
#include <algorithm> 
using std::max;
#include "cVisitor.h"

class cComputeSize : public cVisitor
{
    public:
        cComputeSize() : cVisitor()
        {}

        virtual void VisitAllNodes(cAstNode *node)
        {
            VisitAllChildren(node);
        }

        virtual void Visit(cVarDeclNode* node)
        {
            // Set size of var
            int size = node->Sizeof();
            node->SetSize(size);

            // If not char, round up
            if (size != 1) ModFix(m_offset, 4);

            // Increment offset by var size
            node->SetOffset(m_offset);
            m_offset += size;
        }
        
        virtual void Visit(cDeclsNode *node)
        {
            // Save offset
            int off = m_offset;

            // If not char, round up
            ModFix(m_offset, 4);

            VisitAllChildren(node);

            // Set size from offset diff
            node->SetSize(m_offset - off);
        }

        virtual void Visit(cBlockNode * node)
        {
            // Save off/high
            int off = m_offset;
            int high = m_high;

            m_high = 0;

            VisitAllChildren(node);

            // Set new high, calc size
            m_high = max(m_high, m_offset);
            node->SetSize(m_high - off);

            // Keep largest high, set offset
            m_high = max(high, m_high);
            m_offset = off;
        }

        virtual void Visit(cFuncDeclNode* node)
        {
            // Save off/high
            int off = m_offset;
            int high = m_high;

            m_offset = 0;
            m_high = 0;

            VisitAllChildren(node);

            ModFix(m_offset, 4);

            // Set size to high
            node->SetSize(max(m_offset, m_high));
            node->SetOffset(0); // Always 0

            // Restore off/high
            m_offset = off;
            m_high = high;
        }

        virtual void Visit(cStructDeclNode* node)
        {
            // Save off/high
            int off = m_offset;
            int high = m_high;

            m_offset = 0;
            m_high = 0;

            VisitAllChildren(node);

            // Save size
            node->SetSize(m_offset);
            node->SetOffset(0); // Always 0

            // Restore off/high
            m_offset = off;
            m_high = high;
        }

        virtual void Visit(cParamsNode* node)
        {
            // Save off
            int off = m_offset;

            VisitAllChildren(node);

            m_offset = off;
            cDeclNode::iterator it;
            for (it=node->FirstChild(); it!=node->LastChild(); it++)
            { // Calculate offset of each param arg
                if ((*it) != nullptr) 
                {
                    cDeclNode* tnode = dynamic_cast<cDeclNode*>(*it);

                    tnode->SetOffset(m_offset);
                    m_offset += tnode->GetSize();

                    ModFix(m_offset, 4);
                }
            }
            // Calculate size with new and old offset
            node->SetSize(m_offset - off);
        }

        virtual void Visit(cVarExprNode* node)
        {
            // Save off
            int varOff = 0;
            cDeclNode::iterator it;
            for (it=node->FirstChild(); it!=node->LastChild(); it++)
            { // Add offsets to get final offset
                if ((*it) != nullptr) 
                {
                    cDeclNode* tnode = dynamic_cast<cSymbol*>(*it)->GetDecl();

                    node->SetSize(tnode->GetSize());
                    varOff += tnode->GetOffset();
                }
            }
            // Set offset for varExpr
            node->SetOffset(varOff);
        }

        static inline void ModFix(int &var, int modVal = 4)
        { // If var not cleanly divisible by modVal, round up so it is.
            int temp = var % modVal;
            if (temp) var += modVal - temp;
        }
        
    protected:
        int m_offset = 0;
        int m_high = 0;
};
