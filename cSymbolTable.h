#pragma once
//**************************************
// cSymbolTable.h
//
// Defines a nested symbol table.
// Individual levels of the symbol table use a std::unordered_map from the STL
//
// Author: Phil Howard & Dayton Wilks
// phil.howard@oit.edu
//


#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include "cSymbol.h"
#include "cBaseTypeNode.h"

// For brevity
typedef std::unordered_map<std::string, cSymbol *> symbolTable_t;

class cSymbolTable
{
    public:
        // Create a symbol table
        cSymbolTable() 
        {
            //_symbolTableList.insert(new symbolTable_t);
            IncreaseScope();
            cSymbol * t;

            t = new cSymbol("char");
            t->setDecl(new cBaseTypeNode(t->GetName(), 1, false));
            this->Insert(t);

            t = new cSymbol("int");
            t->setDecl(new cBaseTypeNode(t->GetName(), 4, false));
            this->Insert(new cSymbol("int"));

            t = new cSymbol("float");
            t->setDecl(new cBaseTypeNode(t->GetName(), 8, true));
            this->Insert(t);
        };

        // Increase the scope: add a level to the nested symbol table
        // Return value is the newly created scope
        symbolTable_t *IncreaseScope()
        {
            _symbolTableList.emplace_back(new symbolTable_t);
            return _symbolTableList.back();
        }

        // Decrease the scope: remove the outer-most scope.
        // Returned value is the outer-most scope AFTER the pop.
        //
        // NOTE: do NOT clean up memory after poping the table. Parts of the
        // AST will probably contain pointers to symbols in the popped table.
        symbolTable_t *DecreaseScope()
        {
            _symbolTableList.pop_back();
            return _symbolTableList.back();
        }

        // insert a symbol into the table
        // Assumes the symbol is not already in the table
        void Insert(cSymbol *sym)
        {
            //if (FindLocal(sym->m_name) == nullptr)
                _symbolTableList.back()->insert(std::pair<std::string, cSymbol*>(sym->GetName(), sym));
            //delete sym;
        }

        // Do a lookup in the nested table. 
        // Return the symbol for the outer-most match. 
        // Returns nullptr if no match is found.
        cSymbol *Find(string name)
        {
            // Create reverse iterator, from inner-most scope, find first instance of name
            for (auto revIterator = _symbolTableList.rbegin(); revIterator != _symbolTableList.rend(); ++revIterator)
            {
                symbolTable_t::iterator hashIterator = (*revIterator)->find(name); // find instance of key in map
                if (hashIterator != (*revIterator)->end()) // if found, return result
                {
                    return hashIterator->second;
                }
            }
            return nullptr;
        }

        // Find a symbol in the outer-most scope.
        // NOTE: does not search nested scopes, only the outermost scope.
        // Return the symbol if found.
        // Returns nullptr if the symbol is not found.
        cSymbol *FindLocal(string name)
        {
            // Get inner most scope from back of list
            symbolTable_t * innerTable = _symbolTableList.back();
            if (innerTable == NULL) return NULL;
            // search for instance of key
            symbolTable_t::iterator result = innerTable->find(name);

            if (result == innerTable->end())  // if not found, return nullptr 
                return nullptr;
            return result->second;
        }

    private:
        // List of hash maps : top of list is outermost scope
        // Bottom of list is innermost scope
        std::list<symbolTable_t*> _symbolTableList;
};

// Declaration for the global symbol table.
// Definition is in main.cpp
extern cSymbolTable g_symbolTable;
