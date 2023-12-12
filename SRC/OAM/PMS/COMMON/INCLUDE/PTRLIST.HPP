#ifndef __CQIS_LIST_H__
#define __CQIS_LIST_H__
#include "intndefs.h"

//typedef struct __POSITION *POSITION;

typedef void *NodeData;
	
struct CNode
{
	CNode* pNext;
   	CNode* pPrev;
   	NodeData data;
};

///////////////////////////////////////////////////////////////////////////////
// class CPtrList

class CPtrList:public CNode
{
protected:

// Construction
public:
    CPtrList();

// Attributes
public:
    int GetCount() const;
    BOOL IsEmpty() const;

    /* peek at head or tail */
    NodeData& GetHead();
    NodeData GetHead() const;
    NodeData& GetTail();
    NodeData GetTail() const;

// Operations
public:
    /* get head or tail (and remove it) - don't call on empty list! */
    NodeData RemoveHead();
    NodeData RemoveTail();

    /* add before head or after tail */
    CNode* AddHead(NodeData newElement);
    CNode* AddTail(NodeData newElement);

    /* remove all elements */
    void RemoveAll();

    /* iteration */
    CNode* GetHeadPosition() const;
    CNode* GetTailPosition() const;
    CNode* GetNextPosition(CNode* position);
    CNode* GetPrevPosition(CNode* position);
    NodeData& GetNext(CNode*& rPosition);
    NodeData GetNext(CNode*& rPosition) const;
    NodeData& GetPrev(CNode*& rPosition);
    NodeData GetPrev(CNode*& rPosition) const;

    /* getting/modifying an element at a given position */
    NodeData& GetAt(CNode* position);
    NodeData GetAt(CNode* position) const;
    void SetAt(CNode* position, NodeData newElement);
    void RemoveAt(CNode* position);

    /* inserting before or after a given position */
    CNode* InsertBefore(CNode* position, NodeData newElement);
    CNode* InsertAfter(CNode* position, NodeData newElement);

    /* helper functions (note: O(n) speed) */
    CNode* Find(NodeData searchValue, CNode* position = NULL) const;
    CNode* FindIndex(int nIndex) const;

// Implementation
protected:
    int m_nCount;
    CNode* m_pHead;
    CNode* m_pTail;
    CNode* m_pFree;

    CNode* NewNode(CNode*, CNode*);
    void FreeNode(CNode*);

public:
    virtual ~CPtrList();
};

///////////////////////////////////////////////////////////////////////////////
// class CDataList

class CDataList
{
protected:
    struct CNode
    {
        CNode* pNext;
        CNode* pPrev;
        unsigned char data[1];
    };
// Construction
public:
    CDataList(int nLength);

// Attributes
public:
    int GetCount() const;
    BOOL IsEmpty() const;

    /* peek at head or tail */
    NodeData& GetHead();
    NodeData GetHead() const;
    NodeData& GetTail();
    NodeData GetTail() const;

// Operations
public:
    /* get head or tail (and remove it) - don't call on empty list! */
    NodeData RemoveHead();
    NodeData RemoveTail();

    /* add before head or after tail */
    CNode* AddHead(NodeData newElement);
    CNode* AddTail(NodeData newElement);

    /* remove all elements */
    void RemoveAll();

    /* iteration */
    CNode* GetHeadPosition() const;
    CNode* GetTailPosition() const;
    CNode* GetNextPosition(CNode* position);
    CNode* GetPrevPosition(CNode* position);
    NodeData& GetNext(CNode*& rPosition);
    NodeData GetNext(CNode*& rPosition) const;
    NodeData& GetPrev(CNode*& rPosition);
    NodeData GetPrev(CNode*& rPosition) const;

    /* getting/modifying an element at a given position */
    NodeData& GetAt(CNode* position);
    NodeData GetAt(CNode* position) const;
    void SetAt(CNode* position, NodeData newElement);
    void RemoveAt(CNode* position);

    /* inserting before or after a given position */
    CNode* InsertBefore(CNode* position, NodeData newElement);
    CNode* InsertAfter(CNode* position, NodeData newElement);

    /* helper functions (note: O(n) speed) */
    CNode* Find(NodeData searchValue, CNode* position = NULL) const;
    CNode* FindIndex(int nIndex) const;

// Implementation
protected:
    int m_nCount;
    int m_nLength;
    CNode* m_pHead;
    CNode* m_pTail;
    CNode* m_pFree;

    CNode* NewNode(CNode*, CNode*);
    void FreeNode(CNode*);

public:
    virtual ~CDataList();
};

#endif /* __CQIS_LIST_H__ */
