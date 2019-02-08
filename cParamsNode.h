#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cParamsNode : public cDeclsNode
{
    public:
        // param is the first decl in this decls
        cParamsNode(cDeclNode * decl) : cDeclsNode(decl) { }

        // Add a decl to the list
        void Insert(cDeclNode * decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};