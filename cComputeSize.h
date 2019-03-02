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
            int size = node->GetSize();
            int off;
            if (size != 1 && (off = m_offset % 4) != 0)
                m_offset += 4 - off;

            node->SetOffset(m_offset);
            m_offset += size;
            //fprintf(stderr, "test<%d><%d>\n", m_size, m_offset);
        }
        
        virtual void Visit(cDeclsNode *node)
        {
            int off = m_offset;

            int t = m_offset % 4;
            if (t != 0) m_offset += 4 - t;

            msg("DI");
            VisitAllChildren(node);
            msg("DO");

            node->SetSize(m_offset - off);
            // if (m_offset > m_high) m_high = m_offset;
        }

        virtual void Visit(cBlockNode * node)
        {
            int off = m_offset;
            int high = m_high;
            m_high = 0;

            msg("BI");
            b++;
            VisitAllChildren(node);
            b--;
            msg("BO");

            if (m_offset > m_high) m_high = m_offset;
            node->SetSize(m_high - off);

            if (high > m_high) m_high = high;
            m_offset = off;
        }

        void msg(string str)
        {
            if (!t) return;
            string t;
            for (int ii = 0; ii < b; ++ii) t+="    ";
            str = t + str;
            str += ": off<%d> high<%d>\n";
            fprintf(stderr, str.c_str(), m_offset, m_high);
        }
        
    protected:
        int m_offset = 0;
        int m_size = 0;
        int m_high = 0;

        int b = 0;
        bool t = false;
};

// cDeclNode::iterator it;
//             int size = 0;
//             for (it=node->FirstChild(); it!=node->LastChild(); it++)
//             {
//                 if ((*it) != nullptr) size += dynamic_cast<cDeclNode*>(*it)->GetSize();
//             }
//             node->SetSize(size);

