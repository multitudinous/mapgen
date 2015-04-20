/**
 * Copyright 2007
 * Code Hammer Technologies, LLC
 * www.code-hammer.com
 *
 */

// +
// *****************************************************************************
//   Include File:  LinkedList.h
//   Environment :  Windows NT 4.x/Windows 98
//   Compiler    :  VISUAL C++ 6.0 (MFC)
//   Description :  This include file contains the definition
//                  of the template LinkedList class.
//
//   Notes       :  Copywrite 2000 Code Hammer Technologies, LLC
//
//   Updates     :  06/14/00 (Brian Bailey)
//                  Created initial version of class.
// *****************************************************************************
// -

#ifndef	LINKLIST_H
#define	LINKLIST_H


#include "stdlib.h"
#include "assert.h"


///////////////////////////////////////////////////////////////////////
//template LinkList Class
template<class TNodeData>
class LinkedList
{

    public:

    // Enums, Structures, etc. //////////////////////////////////////////
    public:
    private:
    protected:

        ////////////////////////////////////
        // a link list node that holds generic data
        typedef struct STListNode
        {
            TNodeData tData;
            struct STListNode *pstNext;
            struct STListNode *pstPrev;
        }STListNode;

        ////////////////////////////////////
        // enum cursor direction
        enum
        {
            I_FORWARD = 0,
            I_REVERSE
        };


        /*
    public:
        // construction/destruction /////////////////////////////////////////
        LinkedList(int iTotalCursors = 1);
        LinkedList(LinkedList &clList2);
        ~LinkedList();

        // interface ////////////////////////////////////////////////////////
        void vInsertFront(const TNodeData &tData);
        void vInsertEnd(const TNodeData &tData);
    TNodeData tInsertEnd(const TNodeData &tData);
    TNodeData* ptInsertEnd(const TNodeData &tData);
        void vInsertAscending(const TNodeData &tData);
        void vInsertDescending(const TNodeData &tData);

        bool bIsMember(TNodeData tData);
        bool bIsEmpty();

        void vRewind(int iCursor = 0);
        void vFastForward(int iCursor = 0);
    TNodeData tGetItem(int iCursor = 0);
    TNodeData tGetItemFirst();
    TNodeData* ptGetItemAccess(int iCursor=0);
    TNodeData* ptGetItemAccessNext(int iCursor=0);
    TNodeData* ptGetItemAccessFirst();
    TNodeData tGetItemLast();
    TNodeData* ptGetItemAccessLast();
    TNodeData* ptGetItemAccessNext(int iCursor=0);
        bool bGetItem(TNodeData *ptData, int iCursor = 0);
        bool bGetItemAccess(TNodeData **pptData, int iCursor = 0);
        void vCursorForward(int iCursor = 0);
        void vCursorBackward(int iCursor = 0);
        void vMoveCursorTo(int iPosition, int iCursor = 0);

        bool bMoreItems(int iCursor = 0);
        int  iGetNumItems();

    bool bHasItem(TNodeData tItem);
        bool bFindItem(TNodeData tDataIn, TNodeData *ptDataOut);

        // array type routines
        int  iFindItem(TNodeData tDataIn, TNodeData *ptDataOut);
        int  iFindLocation(TNodeData tDataIn);
        bool bFindItemAt(int iIndex, TNodeData *ptDataOut);
        bool bInsertItemAt(int iIndex, TNodeData tData);

        bool bRemoveItemAt(int iIndex);
        bool bRemoveItemAt(int iIndex, TNodeData *ptDataRemoved);

        // removal routines with no data retrival
        bool bRemoveFirst();
        bool bRemoveLast();
        bool bRemove(TNodeData tData);
        bool bRemoveCurrent(int iCursor=0);

        // removal routines with removed data retrival
        bool bRemoveFirst(TNodeData *ptDataRemoved);
        bool bRemoveLast(TNodeData *ptDataRemoved);
        bool bRemove(TNodeData tData, TNodeData *ptDataRemoved);
        bool bRemove(TNodeData tDataFrom, TNodeData tDataTo, LinkedList<TNodeData> *pclRemoveList = NULL);

        void vMakeEmpty();

        void vResizeCursors(int iNumCursors);
    void vSetCursorDir(bool bForward=true, int iCursor=0);

    void vEnableMemPool(BOOL bOn);
    void vDeleteMemPool();
    int iGetMemPoolCount();


        // Overloaded Operators /////////////////////////////////////////////
        void operator= (const LinkedList &clList2);
        void operator+= (const LinkedList &clList2);
        LinkedList operator+ (const LinkedList &clList2);

    protected:
    private:

        // methods //////////////////////////////////////////////////////////
        void vCreateNewNode(STListNode **ppstNewNode, const TNodeData &tData);
        void vRecursiveFree(STListNode *pstCurrent);
        void vDeleteList();
        void vCursorAdjust(const STListNode *pstCheck);
    void vDeleteNode(STListNode *pstNode);
*/

    private:
        // Members //////////////////////////////////////////////////////////
        STListNode *m_pstFirst;		// points to the head of the list
        STListNode *m_pstLast;		// points to the tail of the list

    STListNode *m_pstPoolFirst; // memory pool node
    int         m_iPoolCount;   // number of memory pool items
    bool        m_bPoolOn;      // memory pool on or off

        STListNode **m_ppastCursor;	// array of cursors - iterator type behavior (Note: Cursors 0,1,2.. m_iTotalCursors)
        int  *m_paiLastCursorDir;		// the last vGetItem call that the user made for (for deletes)
        int   m_iTotalCursors;			// total number of cursors for this list

        int   m_iTotalItems;				// total number of items in the list


// +
// *****************************************************************************
//   Include File:  LinkedList.h
//   Environment :  Windows NT 4.x/Windows 98
//   Compiler    :  VISUAL C++ 6.0 (MFC)
//   Description :  This include file contains the implementation
//                  of the template LinkedList class.
//
//   Notes       :  Copywrite 2000 Springhill Engineering
//
//									TO USE THIS CLASS - INCLUDE THIS CPP AND NOT THE h FILE
//									#inclued "LinkedList.cpp"
//
//   Updates     :  06/14/00 (Brian Bailey)
//                  Created initial version of class.
// *****************************************************************************
// -

public:
// construction/destruction ////////////////////////////////////////////////////////////////////

// +
// *****************************************************************************
//   Module      :  LinkedList::LinkedList(int iTotalCursors)
//   Access      :  Public
//   Input       :  iTotalCursors - total number of cursors for the list
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  Construction.
//
//   Notes       :  None
// *****************************************************************************
// -
LinkedList(int iTotalCursors=1)
{
    // initialize
    m_pstFirst  = NULL;
    m_pstLast   = NULL;

    m_iTotalItems = 0;

    // initialize cursor specifics
    m_iTotalCursors = iTotalCursors;

    m_ppastCursor = new STListNode *[iTotalCursors];
    m_paiLastCursorDir = new int[iTotalCursors];

    // initialize all defualt cursor specifics
    for(int i=0; i < iTotalCursors; i++)
    {
        m_ppastCursor[i] = NULL;
        m_paiLastCursorDir[i] = I_FORWARD;			// defualt
    }

  // memory pool items
  m_pstPoolFirst  = NULL;
  m_iPoolCount    = 0;
  m_bPoolOn       = false;
}

// +
// *****************************************************************************
//   Module      :  LinkedList::LinkedList(const LinkedList &clList2)
//   Access      :  Public
//   Input       :  clList2 - the list to copy
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  Construction - Copy constructor
//
//   Notes       :  None
// *****************************************************************************
// -
LinkedList(LinkedList &clList2)
{
        // initialize
    m_pstFirst  = NULL;
    m_pstLast   = NULL;
    m_iTotalItems = 0;

  // memory pool items
  m_pstPoolFirst  = NULL;
  m_iPoolCount    = 0;
  m_bPoolOn       = false;

    // init cursor specifics
    m_ppastCursor      = NULL;
    m_paiLastCursorDir = NULL;
    m_iTotalCursors    = 0;

    *this = clList2; // copy
}

// +
// *****************************************************************************
//   Module      :  LinkedList::~LinkedList()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  Destruction - clean up
//
//   Notes       :  None
// *****************************************************************************
// -
virtual ~LinkedList()
{
  m_bPoolOn = false; // don't want to add to the pool in delete list

    // clean up
  vMakeEmpty();

    if (m_ppastCursor)
    {
        delete [] m_ppastCursor;
        m_ppastCursor = NULL;
    }

    if (m_paiLastCursorDir)
    {
        delete [] m_paiLastCursorDir;
        m_paiLastCursorDir = NULL;
    }

  // clean up memory pool
  vDeleteMemPool();
}

// Interface ///////////////////////////////////////////////////////////////////////////////////

void vSetCursorCount(unsigned int uiTotalCursors=1)
{
  if (m_ppastCursor)
    {
        delete [] m_ppastCursor;
        m_ppastCursor = NULL;
    }

    if (m_paiLastCursorDir)
    {
        delete [] m_paiLastCursorDir;
        m_paiLastCursorDir = NULL;
    }

  if (!uiTotalCursors)
    uiTotalCursors = 1; // must have at least 1

  m_ppastCursor = new STListNode *[uiTotalCursors];
    m_paiLastCursorDir = new int[uiTotalCursors];

    // initialize all defualt cursor specifics
    for(unsigned int i=0; i < uiTotalCursors; i++)
    {
        m_ppastCursor[i] = m_pstFirst;
        m_paiLastCursorDir[i] = I_FORWARD;			// defualt
    }

  m_iTotalCursors = uiTotalCursors;
}

int iGetCursorCount()
{
  return m_iTotalCursors;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vInsertFront(const TNodeData &tData)
//   Access      :  Public
//   Input       :  tData - generic data to insert
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function inserts tData at the head(front) of the list.
//
//   Notes       :  None
// *****************************************************************************
// -
void vInsertFront(const TNodeData &tData)
{
    STListNode *pstNewNode;							// new node to insert
    vCreateNewNode(&pstNewNode, tData); // create it

    // is this the first item in the list
    if (m_pstFirst)
    {
        // insert at the front of the list
        pstNewNode->pstNext = m_pstFirst;
        m_pstFirst->pstPrev = pstNewNode;
        m_pstFirst = pstNewNode;
    }
    else
    {
        // initialize
        m_pstFirst  = pstNewNode;
        m_pstLast	  = m_pstFirst;

        // initialize all cursors
        for (int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = m_pstFirst;
    }

    m_iTotalItems++;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vInsertEnd(const TNodeData &tData)
//   Access      :  Public
//   Input       :  tData - generic data to insert
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function inserts tData at the tail(end) of the list.
//
//   Notes       :  None
// *****************************************************************************
// -
void vInsertEnd(const TNodeData &tData)
{
    STListNode *pstNewNode;							// new node to insert
    vCreateNewNode(&pstNewNode, tData); // create it

    // is this the first item in the list
    if (m_pstLast)
    {
        // insert at the end of the list
        pstNewNode->pstPrev = m_pstLast;
        m_pstLast->pstNext = pstNewNode;
        m_pstLast = pstNewNode;
    }
    else
    {
        // initialize
        m_pstLast   = pstNewNode;
        m_pstFirst  = m_pstLast;

        // initialize all cursors
        for (int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = m_pstFirst;
    }

    m_iTotalItems++;
}

TNodeData tInsertEnd(const TNodeData &tData)
{
  vInsertEnd(tData);

  return m_pstLast->tData;
}

TNodeData* ptInsertEnd(const TNodeData &tData)
{
  vInsertEnd(tData);

  return &m_pstLast->tData;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vInsertAscending(const TNodeData &tData)
//   Access      :  Public
//   Input       :  tData - generic data to insert
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function inserts tData in ascending order.
//
//   Notes       :  None
// *****************************************************************************
// -
void vInsertAscending(const TNodeData &tData)
{
    STListNode *pstCurr;								// temperary iterator
    STListNode *pstNewNode;							// new node to insert
    vCreateNewNode(&pstNewNode, tData); // create it
    bool bFound = false;

    // is this the first item in the list
    if (!m_pstFirst)
    {
                // initialize
        m_pstFirst  = pstNewNode;
        m_pstLast	  = m_pstFirst;

        // initialize all cursors
        for (int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = m_pstFirst;
    }
    else
    {
        if ( pstNewNode->tData <= m_pstFirst->tData)			// check the start of the list
        {
            // insert at the front of the list
            pstNewNode->pstNext = m_pstFirst;
            m_pstFirst->pstPrev = pstNewNode;
            m_pstFirst = pstNewNode;
        }
        else if ( pstNewNode->tData >= m_pstLast->tData)	// check the end of the list
        {
            // insert at the end of the list
            pstNewNode->pstPrev = m_pstLast;
            m_pstLast->pstNext = pstNewNode;
            m_pstLast = pstNewNode;
        }
        else																							// middle of the list
        {
            pstCurr = m_pstFirst->pstNext;
            while (pstCurr && !bFound)	// don't need to check the last item (already did)
            {
                if (pstNewNode->tData >= pstCurr->tData)
                        pstCurr = pstCurr->pstNext;						// move down the list
                else
                {
                    // insert
                    pstNewNode->pstPrev = pstCurr->pstPrev;
                    pstNewNode->pstNext = pstCurr;
                    pstCurr->pstPrev->pstNext = pstNewNode;
                    pstCurr->pstPrev = pstNewNode;

                    bFound = true;
                }
            }

            assert(&bFound);	// check
        }
    }

    m_iTotalItems++;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vInsertDescending(const TNodeData &tData)
//   Access      :  Public
//   Input       :  tData - generic data to insert
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function inserts tData in descending order.
//
//   Notes       :  None
// *****************************************************************************
// -
void vInsertDescending(const TNodeData &tData)
{
    STListNode *pstCurr;								// temperary iterator
    STListNode *pstNewNode;							// new node to insert
    vCreateNewNode(&pstNewNode, tData); // create it
    bool bFound = false;

    // is this the first item in the list
    if (!m_pstFirst)
    {
                // initialize
        m_pstFirst  = pstNewNode;
        m_pstLast	  = m_pstFirst;

        // initialize all cursors
        for (int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = m_pstFirst;
    }
  else
  {
    // check to see if the new node will be the first node
    if (pstNewNode->tData >= m_pstFirst->tData)			// check the start of the list
        {
            // insert at the front of the list
            pstNewNode->pstNext = m_pstFirst;
            m_pstFirst->pstPrev = pstNewNode;
            m_pstFirst = pstNewNode;
        }
    else
    {
      pstCurr = m_pstFirst->pstNext; // already check the first node
      while (pstCurr)
      {
        // if greater then insert
        if (pstNewNode->tData >= pstCurr->tData)
        {
          pstNewNode->pstPrev = pstCurr->pstPrev;
              pstNewNode->pstNext = pstCurr;
                pstCurr->pstPrev->pstNext = pstNewNode;
                pstCurr->pstPrev = pstNewNode;
                bFound = true;

          break;
        }

        pstCurr = pstCurr->pstNext;
      }

      // must be the last node
      if (!bFound)
      {
        // insert at the end of the list
              pstNewNode->pstPrev = m_pstLast;
              m_pstLast->pstNext = pstNewNode;
              m_pstLast = pstNewNode;

        bFound = true;
      }

    }
  }
  /*
    else
    {
        if (pstNewNode->tData >= m_pstFirst->tData)			// check the start of the list
        {
            // insert at the front of the list
            pstNewNode->pstNext = m_pstFirst;
            m_pstFirst->pstPrev = pstNewNode;
            m_pstFirst = pstNewNode;
        }
        else if ( pstNewNode->tData <= m_pstLast->tData)	// check the end of the list
        {
            // insert at the end of the list
            pstNewNode->pstPrev = m_pstLast;
            m_pstLast->pstNext = pstNewNode;
            m_pstLast = pstNewNode;
        }
        else																							// middle of the list
        {
            pstCurr = m_pstFirst->pstNext;
            while (pstCurr->pstNext && !bFound)	// don't need to check the last item (already did)
            {
                if (pstNewNode->tData >= pstCurr->tData)
                {
                    // insert
                    pstNewNode->pstPrev = pstCurr->pstPrev;
                    pstNewNode->pstNext = pstCurr;
                    pstCurr->pstPrev->pstNext = pstNewNode;
                    pstCurr->pstPrev = pstNewNode;

                    bFound = true;
                }
                else
                    pstCurr = pstCurr->pstNext;
            }

            assert(bFound);	// check
        }
    }
  */

    m_iTotalItems++;
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bIsMember(TNodeData tData)
//   Access      :  Public
//   Input       :  tData - data item to look for
//   Output      :  None
//   Return Val  :  true - the item is in the list
//									false- the item is not in the list
//   Exceptions  :  None
//   Desc        :  This function looks for tData in the list
//
//   Notes       :  None
// *****************************************************************************
// -
bool bIsMember(TNodeData tData)
{
    STListNode *pstTempNode = m_pstFirst;

    while (pstTempNode)
    {
        if (pstTempNode->tData == tData)
            return true;
        else
            pstTempNode = pstTempNode->pstNext;
    }

    return false; // not an element of this list
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bIsEmpty()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  true - the list is empty
//									false- the list is not empty
//   Exceptions  :  None
//   Desc        :  This function tells the caller if the list is empty or not
//
//   Notes       :  None
// *****************************************************************************
// -
bool bIsEmpty()
{
    if (m_iTotalItems)
        return false;

    return true;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vRewind()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function rewinds the cursor to the head of the list.
//
//   Notes       :  None
// *****************************************************************************
// -
void vRewind(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    m_ppastCursor[iCursor] = m_pstFirst;
    m_paiLastCursorDir[iCursor] = I_FORWARD;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vRewind()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function fastforwards the cursor to the tail of the list.
//
//   Notes       :  None
// *****************************************************************************
// -
void vFastForward(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    m_ppastCursor[iCursor] = m_pstLast;
    m_paiLastCursorDir[iCursor] = I_REVERSE;
}

// +
// *****************************************************************************
//   Module      :  TNodeData tGetItem(int iCursor = 0)
//   Access      :  Public
//   Input       :  iCursor - cursor to grab data from (default cursor = 0)
//   Output      :  None
//   Return Val  :  The items
//   Exceptions  :  None
//   Desc        :  This function give the user access to the list data by grabing
//									the data that iCursor points to.
//
//   Notes       :  None
// *****************************************************************************
// -
TNodeData tGetItem(int iCursor = 0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

  return m_ppastCursor[iCursor]->tData;
}

TNodeData tGetItemFirst()
{
    assert(m_pstFirst);
  return m_pstFirst->tData;
}


TNodeData* ptGetItemAccessFirst()
{
    if (!m_pstFirst)
    return NULL;

  return &m_pstFirst->tData;
}

TNodeData tGetItemLast()
{
  assert(m_pstLast);
  return m_pstLast->tData;
}

TNodeData* ptGetItemAccessLast()
{
  assert(m_pstLast);
  return &m_pstLast->tData;
}

TNodeData* ptGetItemAccessNext(int iCursor=0)
{
  assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // make sure we don't access a null pointer
    if (bMoreItems(iCursor))
  {
    TNodeData *ptData = &(m_ppastCursor[iCursor]->tData);
    if (m_paiLastCursorDir[iCursor] == I_FORWARD)
      vCursorForward(iCursor);
    else
      vCursorBackward(iCursor);

        return ptData;
  }

  return NULL;	// failure
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bGetItem(TNodeData *ptData, int iCursor)
//   Access      :  Public
//   Input       :  iCursor - cursor to grab data from (default cursor = 0)
//   Output      :  ptData  - pointer to be filled with current cursor data
//													  or unchange if no more items
//   Return Val  :  true  - success
//									false - failure
//   Exceptions  :  None
//   Desc        :  This function give the user access to the list data by grabing
//									the data that iCursor points to (Makes a copy of the data)
//
//   Notes       :  None
// *****************************************************************************
// -
bool bGetItem(TNodeData *ptData, int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // make sure we don't access a null pointer
    if ( bMoreItems(iCursor) )
    {
        *ptData = m_ppastCursor[iCursor]->tData;
        return true;
    }
    else
        return false;	// failure
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bGetItemAccess(TNodeData **pptData, int iCursor)
//   Access      :  Public
//   Input       :  iCursor - cursor to grab data from (default cursor = 0)
//   Output      :  pptData  - pointer to be filled with current cursor data
//													   address
//   Return Val  :  true  - success
//									false - failure
//   Exceptions  :  None
//   Desc        :  This function give the user access to the list data by grabing
//									the data that iCursor points to (grabs the address of the data)
//
//   Notes       :  Careful here because the pointer retured in ppt will allow the caller
//									to modify the data that is stored in the list directly
// *****************************************************************************
// -
bool bGetItemAccess(TNodeData **pptData, int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // make sure we don't access a null pointer
    if ( bMoreItems(iCursor) )
    {
        *pptData = &(m_ppastCursor[iCursor]->tData);
        return true;
    }
    else
        return false;	// failure
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bGetItemAccess(TNodeData **pptData, int iCursor)
//   Access      :  Public
//   Input       :  iCursor - cursor to grab data from (default cursor = 0)
//   Output      :  pptData  - pointer to be filled with current cursor data
//													   address
//   Return Val  :  true  - success
//									false - failure
//   Exceptions  :  None
//   Desc        :  This function give the user access to the list data by grabing
//									the data that iCursor points to (grabs the address of the data)
//
//   Notes       :  Careful here because the pointer retured in ppt will allow the caller
//									to modify the data that is stored in the list directly
// *****************************************************************************
// -
TNodeData* ptGetItemAccess(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // make sure we don't access a null pointer
    if ( bMoreItems(iCursor) )
        return &(m_ppastCursor[iCursor]->tData);
    else
        return NULL;	// failure
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vCursorForward(int iCursor = 0)
//   Access      :  Public
//   Input       :  iCursor - cursor to move forward (default cursor 0)
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function moves iCursor forward to the next item
//
//   Notes       :  None
// *****************************************************************************
// -
void vCursorForward(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // move to the next item
    if (m_ppastCursor[iCursor])
    {
        m_ppastCursor[iCursor] = m_ppastCursor[iCursor]->pstNext;
        m_paiLastCursorDir[iCursor] = I_FORWARD;
    }
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vCursorForward(int iCursor)
//   Access      :  Public
//   Input       :  iCursor - cursor to move forward (default cursor 0)
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function moves iCursor backward to the previous item
//
//   Notes       :  None
// *****************************************************************************
// -
void vCursorBackward(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // move to the next item
    if (m_ppastCursor[iCursor])
    {
        m_ppastCursor[iCursor] = m_ppastCursor[iCursor]->pstPrev;
        m_paiLastCursorDir[iCursor] = I_REVERSE;
    }
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vMoveCursorTo(int iPosition, int iCursor = 0)
//   Access      :  Public
//   Input       :  iPosition - position to move to. (Postion can be from 0 - iNumItems - 1)
//									iCursor   - cursor to move forward (default cursor 0)
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function moves cursor forward to iPosition. iPosition start2
//									at 0 and goes to iGetNumItems() - 1
//
//   Notes       :  None
// *****************************************************************************
// -
void vMoveCursorTo(int iPosition, int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );
    assert( (0 <= iPosition && iPosition < m_iTotalItems) );

    // move to the next item
    STListNode *pstTemp = m_pstFirst;
    for (int i = 0; i < iPosition; i++)
        pstTemp = pstTemp->pstNext;

    m_ppastCursor[iCursor] = pstTemp;
    m_paiLastCursorDir[iCursor] = I_FORWARD;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vDelete()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  true - there are more items for iIterator to traverse
//									false- there are no more items for iIterator to traverse
//   Exceptions  :  None
//   Desc        :  This function
//
//   Notes       :  None
// *****************************************************************************
// -
bool bMoreItems(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    if (m_ppastCursor[iCursor])
        return true;
    else
        return false;
}

// +
// *****************************************************************************
//   Module      :  int LinkedList::iGetNumItems();
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  int - total number of items currently in the list
//   Exceptions  :  None
//   Desc        :  This function returns the total number of items in the list
//
//   Notes       :  None
// *****************************************************************************
// -
int iGetNumItems()
{
    return m_iTotalItems;
}

//=======================================================================
// is the item in the list
//=======================================================================
bool bHasItem(TNodeData tItem)
{
    STListNode *pstTemp = m_pstFirst;

    // loop through all items until tDataIn is found or NULL is reached
    while (pstTemp)
    {
        if (pstTemp->tData == tItem)
          return true;

      pstTemp = pstTemp->pstNext;
    }

    return false;
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bFindItem(TNodeData tDataIn, TNodeData *ptDataOut)
//   Access      :  Public
//   Input       :  tDataIn  - generic data that you are looking for
//									tDataOut - generic data that will be filled on exit if the item was found
//   Output      :  None
//   Return Val  :  true  - the tDataIn node was found
//									false - the tDataIn node was not found
//   Exceptions  :  None
//   Desc        :  This function finds the first available node with tDataIn
//									starting at the head of the list. ptDataOut is filled
//									with the TNodeData that == tDataIn. It returns true for success
//									and false for failure.
//
//   Notes       :  None
// *****************************************************************************
// -
bool bFindItem(TNodeData tDataIn, TNodeData *ptDataOut=NULL)
{
    STListNode *pstTemp = m_pstFirst;
    bool bFound = false;

    // loop through all items until tDataIn is found or NULL is reached
    while (!bFound && pstTemp)
    {
        if (pstTemp->tData == tDataIn)
        {
      if (ptDataOut)
              *ptDataOut = pstTemp->tData;
            bFound = true;
        }
        else
            pstTemp = pstTemp->pstNext;
    }

    return bFound;
}

// +
// *****************************************************************************
//   Module      :  int LinkedList::iFindItem(TNodeData tDataIn, TNodeData *ptDataOut)
//   Access      :  Public
//   Input       :  tDataIn  - generic data that you are looking for
//									tDataOut - generic data that will be filled on exit if the item was found
//   Output      :  None
//   Return Val  :   0 based index of the items location - the tDataIn node was found
//									-1 the tDataIn node was not found
//   Exceptions  :  None
//   Desc        :  This function finds the first available node with tDataIn
//									starting at the head of the list. ptDataOut is filled
//									with the TNodeData that == tDataIn. It returns 0 based index location
//									for success and -1 for failure.
//
//   Notes       :  None
// *****************************************************************************
// -
int iFindItem(TNodeData tDataIn, TNodeData *ptDataOut)
{
    STListNode *pstTemp = m_pstFirst;
    int iIndex = 0;

    // loop through all items until tDataIn is found or NULL is reached
    while (pstTemp)
    {
        if (pstTemp->tData == tDataIn)
        {
            *ptDataOut = pstTemp->tData;
            return iIndex;
        }

        // increment
        pstTemp = pstTemp->pstNext;
        iIndex++;
    }

    return -1; // failure
}

// +
// *****************************************************************************
//   Module      :  int LinkedList::iFindLocation(TNodeData tDataIn)
//   Access      :  Public
//   Input       :  tDataIn  - generic data that you are looking for
//   Output      :  None
//   Return Val  :   0 based index of the item location on success
//									-1 for failure
//   Exceptions  :  None
//   Desc        :  This function finds the index location (simulates an array) of the item
//									in question or -1 if the item could not be found.
//
//   Notes       :  None
// *****************************************************************************
// -
int iFindLocation(TNodeData tDataIn)
{
    STListNode *pstTemp = m_pstFirst;
    int iIndex = 0;

    // loop through all items until tDataIn is found or NULL is reached
    while (pstTemp)
    {
        if (pstTemp->tData == tDataIn)
            return iIndex;

        // increment
        pstTemp = pstTemp->pstNext;
        iIndex++;
    }

    return -1;
}


// +
// *****************************************************************************
//   Module      :  bool LinkedList::bFindItemAt(int iIndex, TNodeData *ptDataOut)
//   Access      :  Public
//   Input       :  int iIndex - the index location of the item (from iFindLocation)
//									ptDataOut - generic data that will be filled on exit if the item was found
//   Output      :  None
//   Return Val  :  true  - the item was found
//									false - the item was not found
//   Exceptions  :  None
//   Desc        :  This function finds the node at location iIndex where items are
//									ordered starting at 0 (the front of the list). Or returns false for
//									failure.
//
//   Notes       :  None
// *****************************************************************************
// -
bool bFindItemAt(int iIndex, TNodeData *ptDataOut)
{
    STListNode *pstTemp = m_pstFirst;

    if (iIndex >= m_iTotalItems || iIndex < 0)
        return false;
    else
    {
        // loop through all items until the iIndex is reached
        for (int i=1; i<= iIndex; i++)
            pstTemp = pstTemp->pstNext;

        *ptDataOut = pstTemp->tData;
        return true;
    }
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bInsertItemAt(int iIndex, TNodeData tData)
//   Access      :  Public
//   Input       :  int iIndex    - the index location of the item (0 - iGetNumItems())
//									ptDataRemoved - filled with the removed data
//   Output      :  None
//   Return Val  :  true  - the item was inserted
//									false - the item was not inserted (out of bounds index
//   Exceptions  :  None
//   Desc        :  This function finds the node at location iIndex and pushes it down (forward)
//									the list and tData is then inserted at this location
//
//   Notes       :  None
// *****************************************************************************
// -
bool bInsertItemAt(int iIndex, TNodeData tData)
{
    if (iIndex > m_iTotalItems || iIndex < 0)
        return false;
    else
    {

        if (iIndex == 0)
            vInsertFront(tData);
        else if (iIndex == m_iTotalItems)
            vInsertEnd(tData);
        else
        {
            STListNode *pstNewNode;							// new node to insert
            vCreateNewNode(&pstNewNode, tData); // create it

            STListNode *pstTemp = m_pstFirst;		// to find where to put the new item
            //	loop through all items until the iIndex is reached
            for (int i=1; i<= iIndex; i++)
                pstTemp = pstTemp->pstNext;

            // insert by alway pushing the node at the iIndex location down the list
            pstNewNode->pstPrev = pstTemp->pstPrev;
            pstNewNode->pstNext = pstTemp;
            pstTemp->pstPrev->pstNext = pstNewNode;
            pstTemp->pstPrev = pstNewNode;

            m_iTotalItems++;
        }
    }

    return true;
}


// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveItemAt(int iIndex)
//   Access      :  Public
//   Input       :  int iIndex    - the index location of the item (from iFindLocation)
//   Output      :  None
//   Return Val  :  true  - the item was removed
//									false - the item was not removed
//   Exceptions  :  None
//   Desc        :  This function finds the node at location iIndex where items are
//									ordered starting at 0 and removes it.
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveItemAt(int iIndex)
{
    if (iIndex >= m_iTotalItems || iIndex < 0)
        return false;
    else
    {

        STListNode *pstTemp = m_pstFirst;

        // loop through all items until the iIndex is reached
        for (int i=1; i<= iIndex; i++)
            pstTemp = pstTemp->pstNext;

        // first item
        if (iIndex == 0)
        {
            // is it the only item
            if (m_pstFirst == m_pstLast)
            {
                vDeleteNode(m_pstFirst);
                m_pstFirst	= NULL;
                m_pstLast		= NULL;

                // reinit to null
                for (int i=0; i<m_iTotalCursors; i++)
                    m_ppastCursor[i] = NULL;
            }
            else
            {
                vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item
                m_pstFirst = m_pstFirst->pstNext;
                m_pstFirst->pstPrev = NULL;
                vDeleteNode(pstTemp);
            }
        }
        else if (iIndex == (m_iTotalItems-1)) // last item
        {
            vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

            // remove this node
            m_pstLast = pstTemp->pstPrev;
            m_pstLast->pstNext = NULL;
            vDeleteNode(pstTemp);
        }
        else				// middle item
        {
            vCursorAdjust(pstTemp); // must increment Cursors if they equal this item

            // remove this node
            pstTemp->pstPrev->pstNext = pstTemp->pstNext;
            pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
            vDeleteNode(pstTemp);
        }

        m_iTotalItems--; // one less now
        return true;
    }
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveItemAt(int iIndex, TNodeData *ptDataRemoved)
//   Access      :  Public
//   Input       :  int iIndex    - the index location of the item (from iFindLocation)
//									ptDataRemoved - filled with the removed data
//   Output      :  None
//   Return Val  :  true  - the item was removed
//									false - the item was not removed
//   Exceptions  :  None
//   Desc        :  This function finds the node at location iIndex where items are
//									ordered starting at 0 and removes it.
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveItemAt(int iIndex, TNodeData *ptDataRemoved)
{
    if (iIndex >= m_iTotalItems || iIndex < 0)
        return false;
    else
    {

        STListNode *pstTemp = m_pstFirst;

        // loop through all items until the iIndex is reached
        for (int i=1; i<= iIndex; i++)
            pstTemp = pstTemp->pstNext;

        *ptDataRemoved = pstTemp->tData;

        // first item
        if (iIndex == 0)
        {
            // is it the only item
            if (m_pstFirst == m_pstLast)
            {
                vDeleteNode(m_pstFirst);
                m_pstFirst	= NULL;
                m_pstLast		= NULL;

                // reinit to null
                for (int i=0; i<m_iTotalCursors; i++)
                    m_ppastCursor[i] = NULL;
            }
            else
            {
                vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item
                m_pstFirst = m_pstFirst->pstNext;
                m_pstFirst->pstPrev = NULL;
                vDeleteNode(pstTemp);
            }
        }
        else if (iIndex == (m_iTotalItems-1)) // last item
        {
            vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

            // remove this node
            m_pstLast = pstTemp->pstPrev;
            m_pstLast->pstNext = NULL;
            vDeleteNode(pstTemp);
        }
        else				// middle item
        {
            vCursorAdjust(pstTemp); // must increment Cursors if they equal this item

            // remove this node
            pstTemp->pstPrev->pstNext = pstTemp->pstNext;
            pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
            vDeleteNode(pstTemp);
        }

        m_iTotalItems--; // one less now
        return true;
    }
}


// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveFirst()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  true  - the first node was removed
//									false - the first node was not removed
//   Exceptions  :  None
//   Desc        :  This function attempt to remove the first node
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveFirst()
{
    if (m_pstFirst)
    {
        // is there more than a single item in the list
        if (m_pstFirst->pstNext)
        {
            vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item

            STListNode *pstTemp = m_pstFirst;
            m_pstFirst = m_pstFirst->pstNext;
            m_pstFirst->pstPrev = NULL;

            vDeleteNode(pstTemp);
            m_iTotalItems--;						// one less now
        }
        else
            vMakeEmpty();

        return true;		// success
    }
    else
        return false;
}


// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveLast();
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  true  - the last node was removed
//									false - the laste node was not removed
//   Exceptions  :  None
//   Desc        :  This function attempt to remove the last node
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveLast()
{
    if (m_pstLast)
    {
        // is there more than a single item in the list
        if (m_pstLast->pstPrev)
        {
            vCursorAdjust(m_pstLast);	// must increment Cursors if they equal this item

            STListNode *pstTemp = m_pstLast;
            m_pstLast = m_pstLast->pstPrev;
            m_pstLast->pstNext = NULL;

            vDeleteNode(pstTemp);
            m_iTotalItems--;						// one less now
        }
        else
            vMakeEmpty();

        return true;		// success
    }
    else
        return false;
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemove(TNodeData tData)
//   Access      :  Public
//   Input       :  tData         - generic data to remove
//   Output      :  None
//   Return Val  :  true  - the tData node was removed
//									false - the tData node was not removed
//   Exceptions  :  None
//   Desc        :  This function removes the first available node with tData
//									starting at the head of the list. It returns true for success
//									and false for failure
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemove(TNodeData tData)
{
    STListNode *pstTemp;
    bool bFound = false;

    // are there any items
    if (!m_pstFirst)
        return false;

    if (m_pstFirst->tData == tData)		// is it the first item
    {
        // is it the only item
        if (m_pstFirst == m_pstLast)
        {
            vDeleteNode(m_pstFirst);
            m_pstFirst	= NULL;
            m_pstLast		= NULL;

            // reinit to null
            for (int i=0; i<m_iTotalCursors; i++)
                m_ppastCursor[i] = NULL;
        }
        else
        {
            vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item
            pstTemp = m_pstFirst;
            m_pstFirst = m_pstFirst->pstNext;
            m_pstFirst->pstPrev = NULL;
            vDeleteNode(pstTemp);
        }

        bFound = true; // success
    }
    else
    {
        pstTemp = m_pstFirst->pstNext;				// already checked first
        if (!pstTemp)													// are there an items left to check
            return false;

        while (pstTemp->pstNext && !bFound)		// loop through the middle of the list
        {
            if (pstTemp->tData == tData)
            {
                vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

                // remove this node
                pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                vDeleteNode(pstTemp);

                bFound = true;	// success
            }
            else
                pstTemp = pstTemp->pstNext;
        }

        if (!bFound && (pstTemp->tData == tData) )	// check the last item
        {
            vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

            // remove this node
            m_pstLast = pstTemp->pstPrev;
            m_pstLast->pstNext = NULL;
            vDeleteNode(pstTemp);

            bFound = true;	// success
        }
    }

    if (bFound)
        m_iTotalItems--; // one less now

    return bFound;
}

// +
// *****************************************************************************
//   Module      :  bool bRemoveCurrent(int iCursor=0)
//   Access      :  Public
//   Input       :  iCursor - cursor to remove data from (default cursor = 0)
//   Output      :  None
//   Return Val  :  true  - the node pointer to by iCursor was removed
//									false - the node pointer to by iCursor was not removed
//   Exceptions  :  None
//   Desc        :  This function removes the current node pointed to by iCursor.
//									The cursor is then incremented either forward or backward from
//									the node that is being removed based on the last direction. The last
//									direction is updated anytime a call to vRewind, vFastForward, vCursorForward,
//									or vCursorBackward is called. And the initial direction is set to forward.
//
//   Notes       :  If the first item in the list is deleted the cursor will always be moved
//									forward. If the last item is deleted the cursor will always be moved
//									backward.
// *****************************************************************************
// -
bool bRemoveCurrent(int iCursor=0)
{
    assert( (0 <= iCursor && iCursor < m_iTotalCursors) );

    // make sure we don't access a null pointer
    if ( bMoreItems(iCursor) )
    {
        STListNode *pstTemp = m_ppastCursor[iCursor];
        vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

        // remove this node
        if (m_iTotalItems == 1) // only item in list
        {
            m_pstFirst = NULL;
            m_pstLast  = NULL;
        }
        else if (pstTemp == m_pstFirst) // first item in list
        {
            m_pstFirst = pstTemp->pstNext;
            m_pstFirst->pstPrev = NULL;
        }
        else if (pstTemp == m_pstLast)	// last item in list
        {
            m_pstLast = pstTemp->pstPrev;
            m_pstLast->pstNext = NULL;
        }
        else														// middle item in list
        {
            pstTemp->pstPrev->pstNext = pstTemp->pstNext;
            pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
        }

        m_iTotalItems--;
        vDeleteNode(pstTemp);

        return true;
    }
    else
        return false;	// failure
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveFirst()
//   Access      :  Public
//   Input       :  None
//   Output      :  ptDataRemoved - filled with the removed data
//   Return Val  :  true  - the first node was removed
//									false - the first node was not removed
//   Exceptions  :  None
//   Desc        :  This function attempt to remove the first node
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveFirst(TNodeData *ptDataRemoved)
{
    if (m_pstFirst)
    {
        // is there more than a single item in the list
        if (m_pstFirst->pstNext)
        {
            vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item

            STListNode *pstTemp = m_pstFirst;
            m_pstFirst = m_pstFirst->pstNext;
            m_pstFirst->pstPrev = NULL;

            *ptDataRemoved = pstTemp->tData;
            vDeleteNode(pstTemp);
            m_iTotalItems--;						// one less now
        }
        else
        {
            *ptDataRemoved = m_pstFirst->tData;
            vMakeEmpty();
        }

        return true;		// success
    }
    else
        return false;
}


// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemoveLast();
//   Access      :  Public
//   Input       :  None
//   Output      :  ptDataRemoved - filled with the removed data
//   Return Val  :  true  - the last node was removed
//									false - the laste node was not removed
//   Exceptions  :  None
//   Desc        :  This function attempt to remove the last node
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemoveLast(TNodeData *ptDataRemoved)
{
    if (m_pstLast)
    {
        // is there more than a single item in the list
        if (m_pstLast->pstPrev)
        {
            vCursorAdjust(m_pstLast);	// must increment Cursors if they equal this item

            STListNode *pstTemp = m_pstLast;
            m_pstLast = m_pstLast->pstPrev;
            m_pstLast->pstNext = NULL;

            *ptDataRemoved = pstTemp->tData;
            vDeleteNode(pstTemp);
            m_iTotalItems--;						// one less now
        }
        else
        {
            *ptDataRemoved = m_pstLast->tData;
            vMakeEmpty();
        }

        return true;		// success
    }
    else
        return false;
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemove(TNodeData tData)
//   Access      :  Public
//   Input       :  tData         - generic data to remove
//   Output      :  ptDataRemoved - filled with the removed data
//   Return Val  :  true  - the tData node was removed
//									false - the tData node was not removed
//   Exceptions  :  None
//   Desc        :  This function removes the first available node with tData
//									starting at the head of the list. It returns true for success
//									and false for failure
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemove(TNodeData tData, TNodeData *ptDataRemoved)
{
    STListNode *pstTemp;
    bool bFound = false;

    // are there any items
    if (!m_pstFirst)
        return false;

    if (m_pstFirst->tData == tData)		// is it the first item
    {
        // is it the only item
        if (m_pstFirst == m_pstLast)
        {
            *ptDataRemoved = m_pstFirst->tData;
            vDeleteNode(m_pstFirst);
            m_pstFirst	= NULL;
            m_pstLast		= NULL;

            // reinit to null
            for (int i=0; i<m_iTotalCursors; i++)
                m_ppastCursor[i] = NULL;
        }
        else
        {
            vCursorAdjust(m_pstFirst);	// must increment Cursors if they equal this item
            pstTemp = m_pstFirst;
            m_pstFirst = m_pstFirst->pstNext;
            m_pstFirst->pstPrev = NULL;
            *ptDataRemoved = pstTemp->tData;
            vDeleteNode(pstTemp);
        }

        bFound = true; // success
    }
    else
    {
        pstTemp = m_pstFirst->pstNext;				// already checked first
        if (!pstTemp)													// are there an items left to check
            return false;

        while (pstTemp->pstNext && !bFound)		// loop through the middle of the list
        {
            if (pstTemp->tData == tData)
            {
                vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

                // remove this node
                pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                *ptDataRemoved = pstTemp->tData;
                vDeleteNode(pstTemp);

                bFound = true;	// success
            }
            else
                pstTemp = pstTemp->pstNext;
        }

        if (!bFound && (pstTemp->tData == tData) )	// check the last item
        {
            vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

            // remove this node
            m_pstLast = pstTemp->pstPrev;
            m_pstLast->pstNext = NULL;
            *ptDataRemoved = pstTemp->tData;
            vDeleteNode(pstTemp);

            bFound = true;	// success
        }
    }

    if (bFound)
        m_iTotalItems--; // one less now

    return bFound;
}

// +
// *****************************************************************************
//   Module      :  bool LinkedList::bRemove(TNodeData tDataFrom, TNodeData tDataTo)
//   Access      :  Public
//   Input       :  tDataFrom - low range
//									tDataTo   - hi  range
//   Output      :  *pclRemoveList - defualts to NULL but if the caller passes in a parameter
//																	 the list will be emptied and filled with all removed
//																	 items.
//																	 Creates the ability to clean up tData of a pointer type.
//   Return Val  :  true  - data was removed
//									false - data was not removed
//   Exceptions  :  None
//   Desc        :  This function goes through the entire list and removes
//									any data that that is between tDataFrom and tDataTo inclusively
//
//   Notes       :  None
// *****************************************************************************
// -
bool bRemove(TNodeData tDataFrom, TNodeData tDataTo,
                                                 LinkedList<TNodeData> *pclRemoveList=NULL)
{
    STListNode *pstTemp, *pstNext;
    bool bRemoveOccured = false;
    bool bRemoveFirst = false;
    bool bRemoveLast = false;

    if (pclRemoveList)
        pclRemoveList->vMakeEmpty();

    // are there any items
    if (!m_pstFirst)
        return false;

    pstTemp = m_pstFirst;

    // loop through the entire list
    while(pstTemp)
    {
        // is it within range
        if ( (pstTemp->tData >= tDataFrom && pstTemp->tData <= tDataTo) ||
                 (pstTemp->tData <= tDataFrom && pstTemp->tData >= tDataTo) )
        {
            vCursorAdjust(pstTemp);	// must increment Cursors if they equal this item

            // if this is the first node adjust it
            if (pstTemp == m_pstFirst)
            {
                bRemoveFirst = true;
                m_pstFirst = pstTemp->pstNext;
            }

            // if this is the last node adjust
            if (pstTemp == m_pstLast)
            {
                bRemoveLast = true;
                m_pstLast = pstTemp->pstPrev;
            }

            // add to the remove list
            if (pclRemoveList)
                pclRemoveList->vInsertEnd(pstTemp->tData);

            // Remove the node
            if (bRemoveFirst && !bRemoveLast)					// remove the first node
            {
                // remove this node
                pstTemp->pstNext->pstPrev = NULL;
                pstNext = pstTemp->pstNext;
                vDeleteNode(pstTemp);

                bRemoveFirst = false;
                pstTemp = pstNext;
            }
            else if (bRemoveLast && !bRemoveFirst)		// remove the last node
            {
                // remove this node
                pstTemp->pstPrev->pstNext = NULL;
                vDeleteNode(pstTemp);

                bRemoveLast = false;
                pstTemp = NULL;
            }
            else if (bRemoveLast && bRemoveFirst)			// remove the only node
            {
                // remove this node
                vDeleteNode(pstTemp);
                pstTemp = NULL;
            }
            else																			// remove a middle node
            {
                // remove this node
                pstTemp->pstPrev->pstNext = pstTemp->pstNext;
                pstTemp->pstNext->pstPrev = pstTemp->pstPrev;
                pstNext = pstTemp->pstNext;
                vDeleteNode(pstTemp);

                pstTemp = pstNext;
            }


            m_iTotalItems--;				// one less now
            bRemoveOccured = true;	// yes data was removed
        }
        else
            pstTemp = pstTemp->pstNext;
    }


    // is this now the empty list
    if (m_iTotalItems == 0)
    {
        m_pstFirst	= NULL;
        m_pstLast		= NULL;

        // reinit to null
        for (int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = NULL;
    }
    else if (m_iTotalItems == 1)
        m_pstFirst = m_pstLast;

    return bRemoveOccured;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vMakeEmpty()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function deletes the entire list
//
//   Notes       :  None
// *****************************************************************************
// -
void vMakeEmpty()
{
    vDeleteList();
    /*if (m_pstFirst)
    {
        vRecursiveFree(m_pstFirst);
        m_pstFirst  = NULL;
        m_pstLast		= NULL;

        for(int i=0; i<m_iTotalCursors; i++)
            m_ppastCursor[i] = NULL;
    }

    m_iTotalItems = 0;
    */
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vResizeCursors(int iNumCursors)
//   Access      :  Private
//   Input       :  iNumCursors - total number of cursors wanted
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  Allows user to adjust the total number of cursors
//
//   Notes       :  None
// *****************************************************************************
// -
void vResizeCursors(int iNumCursors)
{
    int iShortList = 0;
  int i          = 0;

    // initialize cursor specifics
    STListNode **ppastCursor = new STListNode *[iNumCursors];
    int  *paiLastCursorDir = new int[iNumCursors];

    // initialize all defualt cursor specifics
    for(i=0; i < iNumCursors; i++)
    {
        ppastCursor[i] = NULL;
        paiLastCursorDir[i] = I_FORWARD;			// defualt
    }



    // if there is a current Cursor  try to save the old cursor state
    if (m_ppastCursor)
    {
        if (iNumCursors < m_iTotalCursors)
            iShortList = iNumCursors;
        else
            iShortList = m_iTotalCursors;

        for (i=0; i < iShortList; i++)
        {
            ppastCursor[i]			= m_ppastCursor[i];
            paiLastCursorDir[i] = m_paiLastCursorDir[i];
        }
    }

    // clean up old list
    if (m_ppastCursor)
        delete [] m_ppastCursor;

    if (m_paiLastCursorDir)
        delete [] m_paiLastCursorDir;

    // reassign new list
    m_ppastCursor			 = ppastCursor;
    m_paiLastCursorDir = paiLastCursorDir;
    m_iTotalCursors    = iNumCursors;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
void vSetCursorDir(bool bForward=true, int iCursor=0)
{
  if (iCursor < 0 || iCursor > m_iTotalCursors-1)
    return;

    // move to the next item
    if (m_ppastCursor[iCursor])
    {
    if (bForward)
          m_paiLastCursorDir[iCursor] = I_FORWARD;
    else
      m_paiLastCursorDir[iCursor] = I_REVERSE;
    }
}

// +
// *****************************************************************************
// *****************************************************************************
// -
void vEnableMemPool(bool bOn)
{
  m_bPoolOn = bOn;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
void vDeleteMemPool()
{
    STListNode *pstCur = m_pstPoolFirst;
    STListNode *pstTemp;

    while (pstCur)
    {
        pstTemp = pstCur;
        pstCur = pstCur->pstNext;
        delete pstTemp;
    }

    m_pstPoolFirst = NULL;
    m_iPoolCount = 0;
}

// +
// *****************************************************************************
// *****************************************************************************
// -
int iGetMemPoolCount()
{
  return m_iPoolCount;
}

// Overloaded Operators ////////////////////////////////////////////////////////////////////////

// +
// *****************************************************************************
//   Module      :  operator= (LinkedList &clList2)
//   Access      :  Public
//   Input       :  clList2 : the List to assingn (=) to this
//   Output      :  None
//   Return Val  :  *this - this after it has been modified
//   Exceptions  :  None
//   Desc        :  = overload
//
//   Notes       :  None
// *****************************************************************************
// -
void operator= (const LinkedList &clList2)
{
    STListNode *pstTemp = clList2.m_pstFirst;

    this->vResizeCursors(clList2.m_iTotalCursors);	// grab some cursors
    this->vMakeEmpty();															// remove any items;

    // make a copy
    while (pstTemp)
    {
        this->vInsertEnd(pstTemp->tData);
        pstTemp = pstTemp->pstNext;
    }

    /*
    if (this != &clList2)
    {
        TNodeData tData;

        clList2.vRewind();															// get to the start of the list
        this->vResizeCursors(clList2.m_iTotalCursors);	// grab some cursors
        this->vMakeEmpty();															// remove any items;

        // make a copy
        while (clList2.bMoreItems())
        {
            clList2.bGetItem(&tData);
            clList2.vCursorForward();
            this->vInsertEnd(tData);
        }
    }
    */
}

void operator= (const std::vector<TNodeData> &v)
{
    vMakeEmpty();
    for (unsigned int i=0; i<v.size(); i++)
    {
        vInsertEnd(v[i]);
    }
}

// +
// *****************************************************************************
//   Module      :  operator+ (const LinkedList &clList2)
//   Access      :  Public
//   Input       :  clList2 : the LinkList to + to this
//   Output      :  None
//   Return Val  :  clNewList - a new list after this and clList2 have been
//															joined together
//   Exceptions  :  None
//   Desc        :  + overload
//
//   Notes       :  None
// *****************************************************************************
// -
LinkedList<TNodeData> operator+ (const LinkedList &clList2)
{
    LinkedList clNewList = *this;
    STListNode *pstTemp = clList2.m_pstFirst;

    // now copy of over clList2	to the end of new list
    while (pstTemp)
    {
        clNewList.vInsertEnd(pstTemp->tData);
        pstTemp = pstTemp->pstNext;
    }

    return clNewList;

    /*
    LinkedList clNewList = *this;
    TNodeData tData;

    clList2.vRewind(); // get to the start of the list

    // now copy of over clList2	to the end of new list
    while (clList2.bMoreItems())
    {
        clList2.bGetItem(&tData);
        clList2.vCursorForward();
        clNewList.vInsertEnd(tData);
    }

    return clNewList;
    */
}

// +
// *****************************************************************************
//   Module      :  operator+= (const LinkedList &clList2)
//   Access      :  Public
//   Input       :  clList2 : the LinkList to += to this
//   Output      :  None
//   Return Val  :  *this - this after it has been modified
//   Exceptions  :  None
//   Desc        :  += overload
//
//   Notes       :  None
// *****************************************************************************
// -
void operator+= (const LinkedList &clList2)
{
    STListNode *pstTemp = clList2.m_pstFirst;

    // now copy over clList2	to the end this list
    while (pstTemp)
    {
        this->vInsertEnd(pstTemp->tData);
        pstTemp = pstTemp->pstNext;
    }

    /*
    TNodeData tData;

    clList2.vRewind(); // get to the start of the list

    // make a copy
    while (clList2.bMoreItems())
    {
        clList2.bGetItem(&tData);
        clList2.vCursorForward();
        this->vInsertEnd(tData);
    }
    */
}

/**
 * This function overloads the == operator and determines if 2 lists are
 * equal
 *
 * @param clList1 (in) First List
 * @param clList2 (in) Second List
 *
 * @return - true: list are equivalent, false: they are not
 *
 */
friend bool operator== (const LinkedList &clList1, const LinkedList &clList2)
{
  if (clList1.m_iTotalItems != clList2.m_iTotalItems)
    return false;

  STListNode *pstNode1 = clList1.m_pstFirst;
  STListNode *pstNode2 = clList2.m_pstFirst;

  while (pstNode1)
  {
    if (pstNode1->tData != pstNode2->tData)
      return false;

    pstNode1 = pstNode1->pstNext;
    pstNode2 = pstNode2->pstNext;
  }

  return true;
}


// private methods /////////////////////////////////////////////////////////////////////////////
private:

// +
// *****************************************************************************
//   Module      :  void LinkedList::vCreateNewNode(STListNode **ppstNewNode,
//																									TNodeData tData)
//   Access      :  Private
//   Input       :  ppstNewNode - the new node to be allocated
//                  tData - data to insert into the node
//   Output      :  ppstNewNode - will be allocated, filled with tData and it's
//																list pointers will be initialized to NULL
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is a helper routine to create a new node for
//									insert purposes
//
//   Notes       :  None
// *****************************************************************************
// -
void vCreateNewNode(STListNode **ppstNewNode, const TNodeData &tData)
{
  if (m_bPoolOn && m_pstPoolFirst)
  {
    // grab a node from the pool
    *ppstNewNode = m_pstPoolFirst;

    // update the pool list
    m_pstPoolFirst = m_pstPoolFirst->pstNext;
    if (m_pstPoolFirst)
      m_pstPoolFirst->pstPrev = NULL;

    m_iPoolCount--;
  }
  else
    *ppstNewNode = new STListNode;

  // init the node
  (*ppstNewNode)->pstNext = NULL;
  (*ppstNewNode)->pstPrev = NULL;
  (*ppstNewNode)->tData   = tData;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vRecursiveFree(STListNode *pstCurrent)
//   Access      :  Private
//   Input       :  pstCurrent - current node we are on (must not be NULL)
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is a helper routine to recrsively clean up
//									the link list
//
//   Notes       :  Not used because it is not efficient because of all of the calls on the
//                  stack.
// *****************************************************************************
// -
void vRecursiveFree(STListNode *pstCurrent)
{
    // recurisive call
    if (pstCurrent->pstNext != NULL)
        vRecursiveFree(pstCurrent->pstNext);

    vDeleteNode(pstCurrent);
    pstCurrent = NULL;
}

// +
// *****************************************************************************
//   Module      :  void vDeleteList()
//   Access      :  Private
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is a helper routine to clean up the link list
//
//   Notes       :  None
// *****************************************************************************
// -
void vDeleteList()
{
    STListNode *pstCur = m_pstFirst;
    STListNode *pstTemp;

    while (pstCur)
    {
        pstTemp = pstCur;
        pstCur = pstCur->pstNext;
        vDeleteNode(pstTemp);
    }

    m_pstFirst = NULL;
    m_pstLast  = NULL;
    m_iTotalItems = 0;
}

// +
// *****************************************************************************
//   Module      :  void LinkedList::vCursorAdjust(const STListNode *pstCheck)
//   Access      :  Private
//   Input       :  pstCheck - node that is going to be deleted
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is a helper routine to check if a cursor points
//									to the node to be deleted and if so increments the cursor accordingly
//
//   Notes       :  None
// *****************************************************************************
// -
void vCursorAdjust(const STListNode *pstCheck)
{
    /*
    if (pstCheck == m_pstFirst)							// first node
    {
        for(int i=0; i<m_iTotalCursors; i++)
        {
            // check it
            if(m_ppastCursor[i] == pstCheck)
                m_ppastCursor[i] = pstCheck->pstNext;
        }
    }
    else if (pstCheck == m_pstLast)					// last node
    {
        for(int i=0; i<m_iTotalCursors; i++)
        {
            // check it
            if(m_ppastCursor[i] == pstCheck)
                m_ppastCursor[i] = pstCheck->pstPrev;
        }
    }
    else																		// middle node
    {
        */
        for(int i=0; i<m_iTotalCursors; i++)
        {
            // check it
            if(m_ppastCursor[i] == pstCheck)
            {
                if (m_paiLastCursorDir[i] == I_REVERSE)
                        m_ppastCursor[i] = pstCheck->pstPrev;
                else
                        m_ppastCursor[i] = pstCheck->pstNext;
            }
        }
    //}
}

// +
// *****************************************************************************
// This function will delete a node.
// If the memory pool is on the node is added to the front of the pool list
// *****************************************************************************
// -
void vDeleteNode(STListNode *pstNode)
{
  if (!m_bPoolOn)
  {
    delete pstNode;
    return;
  }

  // add the node to the front of the memory pool
  if (m_pstPoolFirst)
    m_pstPoolFirst->pstPrev = pstNode;

  pstNode->pstPrev = NULL;
  pstNode->pstNext = m_pstPoolFirst;
  m_pstPoolFirst = pstNode;

  m_iPoolCount++;
}

};


// Basic List Types //////////////////////////////////////////////////////
typedef LinkedList<int>    CTIntList;
typedef LinkedList<long>   CTLongList;
typedef LinkedList<char>   CTCharList;
typedef LinkedList<float>  CTFloatList;
typedef LinkedList<double> CTDoubleList;

#endif
