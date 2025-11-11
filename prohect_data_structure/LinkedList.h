#ifndef _LINKEDLIST
#define _LINKEDLIST

#include <iostream>
#include "Node.h"
using namespace std;

template <typename T>
class LinkedList
{
private:
    Node<T>* Head;

public:
    LinkedList();
    ~LinkedList();

    void PrintList() const;
    void InsertBeg(const T& data);
    void InsertEnd(const T& data);
    bool Find(const T& key);
    int CountOccurance(const T& value);
    void DeleteFirst();
    void DeleteLast();
    bool DeleteNode(const T& value);
    bool DeleteNodes(const T& value);
    void Merge(const LinkedList& L);
    void Reverse();
    void DeleteAll();
};


#endif
