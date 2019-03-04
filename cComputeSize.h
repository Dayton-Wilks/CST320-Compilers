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
            int size = node->Sizeof();
            node->SetSize(size);

            if (size != 1) ModFix(m_offset, 4);

            node->SetOffset(m_offset);
            m_offset += size;
msg("V");
        }
        
        virtual void Visit(cDeclsNode *node)
        {
            int off = m_offset; // save start offset

            ModFix(m_offset, 4);

msg("DI", IN);
            VisitAllChildren(node);
msg("DO", OUT);

            node->SetSize(m_offset - off);
        }

        virtual void Visit(cBlockNode * node)
        {
            int off = m_offset;
            int high = m_high;
            m_high = 0;

msg("BI", IN);
            VisitAllChildren(node);
msg("BO", OUT);

            m_high = max(m_high, m_offset);
            node->SetSize(m_high - off);

            m_high = max(high, m_high);
            m_offset = off;
        }

        virtual void Visit(cFuncDeclNode* node)
        {
            int off = m_offset;
            int high = m_high;

            m_offset = 0;
            m_high = 0;

msg("FI", IN);
            VisitAllChildren(node);
msg("FO", OUT);

            ModFix(m_offset, 4);

            node->SetSize(max(m_offset, m_high));
            node->SetOffset(0);

            m_offset = off;
            m_high = high;
        }

        virtual void Visit(cStructDeclNode* node)
        {
            int off = m_offset;
            int high = m_high;

            m_offset = 0;
            m_high = 0;

msg("SI", IN);
            VisitAllChildren(node);
msg("SO", OUT);

            node->SetSize(m_offset);
            node->SetOffset(0);

            m_offset = off;
            m_high = high;
        }

        virtual void Visit(cParamsNode* node)
        {
            int temp = m_offset;
            VisitAllChildren(node);

            m_offset = temp;
            cDeclNode::iterator it;
            for (it=node->FirstChild(); it!=node->LastChild(); it++)
            {
                if ((*it) != nullptr) 
                {
                    cDeclNode* tnode = dynamic_cast<cDeclNode*>(*it);
                    tnode->SetOffset(m_offset);
                    m_offset += tnode->GetSize();
                    ModFix(m_offset, 4);
                }
            }

            //ModFix(m_offset, 4);

            node->SetSize(m_offset - temp);
        }

        void msg(string str, short tab = 0)
        {
            if (!x) return;
            if (tab == OUT) b += tab;
            string t;
            for (int ii = 0; ii < b; ++ii) t+="    ";
            str = t + str;
            str += ": off<%d> high<%d>\n";
            fprintf(stderr, str.c_str(), m_offset, m_high);
            if (tab == IN)b += tab;
        }

        static inline void ModFix(int &var, int modVal = 4)
        {
            int temp = var % modVal;
            if (temp) var += modVal - temp;
        }
        
    protected:
        int m_offset = 0;
        int m_high = 0;

        int b = 0;
        bool x = false;
        const short IN = 1;
        const short OUT = -1;
};

// cDeclNode::iterator it;
//             int size = 0;
//             for (it=node->FirstChild(); it!=node->LastChild(); it++)
//             {
//                 if ((*it) != nullptr) size += dynamic_cast<cDeclNode*>(*it)->GetSize();
//             }
//             node->SetSize(size);

