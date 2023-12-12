#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include "ptrList.hpp"

/////////////////////////////////////////////////////////////////////////////

CPtrList::CPtrList()
{
    m_nCount = 0;
    m_pHead = (CNode*) NULL;
    m_pTail = (CNode*) NULL;
    m_pFree = (CNode*) NULL;
}

CPtrList::~CPtrList()
{
    CNode* pNode;
    CNode* pNext;

    pNode = m_pHead;
    while (pNode)
    {
        pNext = pNode->pNext;
        free(pNode);
        pNode = pNext;
    }

    pNode = m_pFree;
    while (pNode)
    {
        pNext = pNode->pNext;
        free(pNode);
        pNode = pNext;
    }
}

int CPtrList::GetCount() const
{
    return m_nCount;
}

bool CPtrList::IsEmpty() const
{
    if (m_nCount > 0)
        return false;

    return true;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* peek at head or tail                                                      */
/*///////////////////////////////////////////////////////////////////////////*/
NodeData& CPtrList::GetHead()
{
    ASSERT(m_pHead);
/*__
    if (!m_pHead)
        return (NodeData) NULL;
__*/

    return m_pHead->data;
}

NodeData CPtrList::GetHead() const
{
    ASSERT(m_pHead);
/*__
    if (!m_pHead)
        return (NodeData) NULL;
__*/

    return m_pHead->data;
}

NodeData& CPtrList::GetTail()
{
    ASSERT(m_pTail);
/*__
    if (!m_pTail)
        return (NodeData) NULL;
__*/

    return m_pTail->data;
}

NodeData CPtrList::GetTail() const
{
    ASSERT(m_pTail);
/*__
    if (!m_pTail)
        return (NodeData) NULL;
__*/

    return m_pTail->data;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* get head or tail (and remove it) - don't call on empty list!              */
/*///////////////////////////////////////////////////////////////////////////*/
NodeData CPtrList::RemoveHead()
{
    NodeData data;

    ASSERT(m_pHead);
/*__
    if (!m_pHead)
        return (NodeData) NULL;
__*/

    data = m_pHead->data;
    RemoveAt( m_pHead);

    return data;
}

NodeData CPtrList::RemoveTail()
{
    NodeData data;

    ASSERT(m_pTail);
/*__
    if (!m_pTail)
        return (NodeData) NULL;
__*/

    data = m_pTail->data;
    RemoveAt( m_pTail);

    return data;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* add before head or after tail                                             */
/*///////////////////////////////////////////////////////////////////////////*/
CNode* CPtrList::AddHead(NodeData newElement)
{
    CNode* pNode = m_pHead;
    CNode* pNewNode = NewNode(NULL, pNode);

    if (!pNewNode)
        return  NULL;

    pNewNode->data = newElement;

    return  pNewNode;
}

CNode* CPtrList::AddTail(NodeData newElement)
{
    CNode* pNode = m_pTail;
    CNode* pNewNode = NewNode(pNode, NULL);

    if (!pNewNode)
        return  NULL;

    pNewNode->data = newElement;

    return  pNewNode;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* remove all elements  2000.09.21 changed by jkyang                                                     */
/*///////////////////////////////////////////////////////////////////////////*/
void CPtrList::RemoveAll()
{
    CNode* pNode = m_pHead;

    while (pNode)
    {
        RemoveAt(pNode);
        pNode = m_pHead;
    }
}

/*///////////////////////////////////////////////////////////////////////////*/
/* iteration                                                                 */
/*///////////////////////////////////////////////////////////////////////////*/
CNode* CPtrList::GetHeadPosition() const
{
    return  m_pHead;
}

CNode* CPtrList::GetTailPosition() const
{
    return  m_pTail;
}

CNode* CPtrList::GetNextPosition(CNode* position)
{
    return  ( position)->pNext;
}

CNode* CPtrList::GetPrevPosition(CNode* position)
{
    return  ( position)->pPrev;
}

NodeData& CPtrList::GetNext(CNode*& rPosition)
{
    CNode* pNode =  rPosition;

    rPosition =  pNode->pNext;

    return pNode->data;
}

NodeData CPtrList::GetNext(CNode*& rPosition) const
{
    CNode* pNode =  rPosition;

    rPosition =  pNode->pNext;

    return pNode->data;
}

NodeData& CPtrList::GetPrev(CNode*& rPosition)
{
    CNode* pNode =  rPosition;

    rPosition =  pNode->pPrev;

    return pNode->data;
}

NodeData CPtrList::GetPrev(CNode*& rPosition) const
{
    CNode* pNode =  rPosition;

    rPosition =  pNode->pPrev;

    return pNode->data;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* getting/modifying an element at a given position                          */
/*///////////////////////////////////////////////////////////////////////////*/
NodeData& CPtrList::GetAt(CNode* position)
{
    CNode* pNode =  position;

    return pNode->data;
}

NodeData CPtrList::GetAt(CNode* position) const
{
    CNode* pNode =  position;

    return pNode->data;
}

void CPtrList::SetAt(CNode* pos, NodeData newElement)
{
    CNode* pNode = (CNode*) pos;

    pNode->data = newElement;
}

void CPtrList::RemoveAt(CNode* position)
{
    CNode* pNode =  position;

    if (pNode != m_pHead && pNode != m_pTail)
    {
        pNode->pPrev->pNext = pNode->pNext;
        pNode->pNext->pPrev = pNode->pPrev;
    }
	else
	{
        if (pNode == m_pHead)
		{
            m_pHead = pNode->pNext;
			if (pNode->pNext)
                pNode->pNext->pPrev = (CNode*) NULL;
		}
        if (pNode == m_pTail)
		{
            m_pTail = pNode->pPrev;
			if (pNode->pPrev)
                pNode->pPrev->pNext = (CNode*) NULL;
		}
    }

    FreeNode(pNode);
}

/*///////////////////////////////////////////////////////////////////////////*/
/* inserting before or after a given position                                */
/*///////////////////////////////////////////////////////////////////////////*/
CNode* CPtrList::InsertBefore(CNode* position, NodeData newElement)
{
    CNode* pNode =  position;
    CNode* pNewNode = NewNode(pNode->pPrev, pNode);

    if (!pNewNode)
        return  NULL;

    pNewNode->data = newElement;

    return  pNewNode;
}

CNode* CPtrList::InsertAfter(CNode* position, NodeData newElement)
{
    CNode* pNode =  position;
    CNode* pNewNode = NewNode(pNode, pNode->pNext);

    if (!pNewNode)
        return  NULL;

    pNewNode->data = newElement;

    return  pNewNode;
}

/*///////////////////////////////////////////////////////////////////////////*/
/* helper functions (note: O(n) speed)                                       */
/*///////////////////////////////////////////////////////////////////////////*/
CNode* CPtrList::Find(NodeData searchValue, CNode* startAfter) const
{
    CNode* pNode = m_pHead;

    if (startAfter)
        pNode =  startAfter;

    while (pNode)
    {
        if (pNode->data == searchValue)
            break;
        pNode = pNode->pNext;
    }

    return  pNode;
}

CNode* CPtrList::FindIndex(int nIndex) const
{
    CNode* pNode = m_pHead;

    while (pNode && nIndex--)
        pNode = pNode->pNext;

    return  pNode;
}

CPtrList::CNode* CPtrList::NewNode(CPtrList::CNode* pPrev, CPtrList::CNode* pNext)
{
    CNode* pNode;

    if (m_pFree)
    {
        pNode = m_pFree;
        m_pFree = pNode->pNext;
    }
    else
    {
        pNode = (CNode*) malloc(sizeof(CNode));
        if (!pNode)
            return (CNode*) NULL;
    }

    if (pPrev)
        pPrev->pNext = pNode;
    else
        m_pHead = pNode;
    	
    if (pNext)
        pNext->pPrev = pNode;
    else
        m_pTail = pNode;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data = (NodeData) NULL;
    m_nCount++;

    return pNode;
}

void CPtrList::FreeNode(CNode* pNode)
{
  
    pNode->pPrev = (CNode*) NULL;
    pNode->pNext = m_pFree;
	//2000.09.21 changed by jkyang
	free(pNode->data);
    m_pFree = pNode;
    m_nCount--;
}
