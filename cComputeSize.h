#pragma once
//**************************************
// cComputeSize.h
//
// Defines a visitor that calculates size and offset
//
// Author: Dayton Wilks
// Date: 3/1/2019
//

#include "cVisitor.h"

class cComputeSize : public cVisitor
{
    public:
        cComputeSize() : cVisitor()
        {
            m_offset = 0;
            m_size = 0;
        }

        virtual void VisitAllNodes(cAstNode *node)
        {
            VisitAllChildren(node);
        }

        virtual void Visit(cVarDeclNode* node)
        {
            node->SetOffset(m_offset);
            m_offset += node->GetSize();
            //fprintf(stderr, "test<%d><%d>\n", m_size, m_offset);
        }
        
        virtual void Visit(cDeclsNode *node)
        {
            VisitAllChildren(node);

            //fprintf(stderr, "<%d><%d>\n", m_size, m_offset);
            node->SetSize(m_offset - m_size);
        }

        virtual void Visit(cBlockNode * node)
        {
            VisitAllChildren(node);

            //fprintf(stderr, "<%d><%d>\n", m_size, m_offset);
            node->SetSize(m_offset - m_size);
        }
        
    protected:
        int m_offset = 0;
        int m_size = 0;
};

