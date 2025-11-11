#include "LinkedList.h"

template <typename T>
LinkedList<T>::LinkedList() : Head(nullptr) {}

template <typename T>
LinkedList<T>::~LinkedList() { DeleteAll(); }

template <typename T>
void LinkedList<T>::PrintList() const
{
    cout << "\nList contents:\n";
    Node<T>* p = Head;
    while (p)
    {
        cout << "[ " << p->getItem() << " ] ---> ";
        p = p->getNext();
    }
    cout << "NULL\n";
}

template <typename T>
void LinkedList<T>::InsertBeg(const T& data)
{
    Node<T>* newNode = new Node<T>(data);
    newNode->setNext(Head);
    Head = newNode;
}

template <typename T>
void LinkedList<T>::InsertEnd(const T& data)
{
    Node<T>* newNode = new Node<T>(data);
    if (!Head)
    {
        Head = newNode;
        return;
    }

    Node<T>* current = Head;
    while (current->getNext())
        current = current->getNext();

    current->setNext(newNode);
}

template <typename T>
bool LinkedList<T>::Find(const T& key)
{
    Node<T>* current = Head;
    while (current)
    {
        if (current->getItem() == key)
            return true;
        current = current->getNext();
    }
    return false;
}

template <typename T>
int LinkedList<T>::CountOccurance(const T& value)
{
    int count = 0;
    Node<T>* current = Head;
    while (current)
    {
        if (current->getItem() == value)
            count++;
        current = current->getNext();
    }
    return count;
}

template <typename T>
void LinkedList<T>::DeleteFirst()
{
    if (!Head) return;

    Node<T>* temp = Head;
    Head = Head->getNext();
    delete temp;
}

template <typename T>
void LinkedList<T>::DeleteLast()
{
    if (!Head) return;

    if (!Head->getNext())
    {
        delete Head;
        Head = nullptr;
        return;
    }

    Node<T>* prev = nullptr;
    Node<T>* current = Head;
    while (current->getNext())
    {
        prev = current;
        current = current->getNext();
    }
    prev->setNext(nullptr);
    delete current;
}

template <typename T>
bool LinkedList<T>::DeleteNode(const T& value)
{
    if (!Head) return false;

    if (Head->getItem() == value)
    {
        Node<T>* temp = Head;
        Head = Head->getNext();
        delete temp;
        return true;
    }

    Node<T>* prev = Head;
    Node<T>* current = Head->getNext();
    while (current)
    {
        if (current->getItem() == value)
        {
            prev->setNext(current->getNext());
            delete current;
            return true;
        }
        prev = current;
        current = current->getNext();
    }
    return false;
}

template <typename T>
bool LinkedList<T>::DeleteNodes(const T& value)
{
    if (!Head) return false;

    bool found = false;
    while (Head && Head->getItem() == value)
    {
        Node<T>* temp = Head;
        Head = Head->getNext();
        delete temp;
        found = true;
    }

    Node<T>* current = Head;
    while (current && current->getNext())
    {
        if (current->getNext()->getItem() == value)
        {
            Node<T>* temp = current->getNext();
            current->setNext(temp->getNext());
            delete temp;
            found = true;
        }
        else
        {
            current = current->getNext();
        }
    }

    return found;
}

template <typename T>
void LinkedList<T>::Merge(const LinkedList& L)
{
    if (!Head)
    {
        Head = L.Head;
        return;
    }

    Node<T>* current = Head;
    while (current->getNext())
        current = current->getNext();

    current->setNext(L.Head);
}

template <typename T>
void LinkedList<T>::Reverse()
{
    Node<T>* prev = nullptr;
    Node<T>* current = Head;
    Node<T>* next = nullptr;

    while (current)
    {
        next = current->getNext();
        current->setNext(prev);
        prev = current;
        current = next;
    }
    Head = prev;
}

template <typename T>
void LinkedList<T>::DeleteAll()
{
    Node<T>* current = Head;
    while (Head)
    {
        current = Head->getNext();
        delete Head;
        Head = current;
    }
}

