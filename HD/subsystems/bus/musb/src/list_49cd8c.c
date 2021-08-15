

#include <fapi/sys_services.h>
#include "musb/list_49cd8c.h"


/* 21ccdb14 - complete */
/* Get the data from the list at index b */
void* MUSB_ArrayFetch(MUSB_Array* pArray, unsigned short/*uint_fast16_t*/ wIndex) /*62*/
{
   void* pResult = 0;

   if ((pArray != 0) && (wIndex < pArray->num_elements))
   {
      if (wIndex < pArray->count)
      {
          pResult = (char*)(pArray->data) + (wIndex * pArray->elem_size);
      }
      else
      {
          pResult = (char*)(pArray->more_data) + ((wIndex - pArray->count) * pArray->elem_size);
      }
   }

   return pResult;
}




/* 496fa4 - complete */
/* Allocate an element in the list and set its data */
static inline uint8_t MGC_ListInsertItem(MUSB_LinkedList* pPos, void* pItem, /*209*/
        uint32_t dwCount) /*210*/
{
   uint8_t bOK = 0;
   MUSB_LinkedList* pNext = FAPI_SYS_MALLOC(sizeof(MUSB_LinkedList));

   if ((pPos != 0) && (pNext != 0))
   {
      bOK = 1;
      pNext->pNext = pPos->pNext;
      pPos->pNext = pNext;
      pNext->data = pItem;
      pNext->dwCount = dwCount;
   }
   
   return bOK;
}


/* 21ccdc94 - todo */
/* Add an element in the list and set its data */
uint8_t MUSB_ListAppendItem(MUSB_LinkedList* pList, void* pItem, /*227*/
        unsigned int dwCount) /*228*/
{   
   uint8_t bOK;
   MUSB_LinkedList* tail = pList;
   
   if (pList == 0) return 0;
   else 
   {
      if (pList->data == 0)
      {
         // First list element is free and can be used 
         bOK = 1;
         pList->data = pItem;
         pList->dwCount = dwCount;
         pList->pNext = 0;
      }
      else 
      {
         // Go to the end of the list
         while (tail->pNext != 0) tail = tail->pNext;

         // Allocate an element at the end of the list
         bOK = MGC_ListInsertItem(tail, pItem, dwCount);
      }
   
      return bOK;
   }
}


/* 21ccdc00 - complete */
void* MUSB_ListFindItem(MUSB_LinkedList* a, int b)
{
   void* elem = 0;
   MUSB_LinkedList* p = List_49cd8c_get_element_ptr(a, b);
   
   if (p != 0) 
   {
      elem = p->data;      
   }
   
   return elem;
}


/* 21ccdc54 - complete */
MUSB_LinkedList* List_49cd8c_get_element_ptr(MUSB_LinkedList* a1, int b)
{
   MUSB_LinkedList* a = a1;
   unsigned short i = 0;
   a1 = 0;
   
   while ((i < b) && (a != 0)) 
   {
      i++;
      a = a->pNext;
   }
   
   if (b == i) a1 = a;
   
   return a1;
}


/* 21ccdb98 - complete */
void MUSB_ListInit(MUSB_LinkedList* pList) /*149*/
{
   if (pList != 0)
   {
       pList->pNext = 0;
       pList->dwCount = 0;
       pList->data = 0;
   }
}


/* 49ceb4 - complete */
int List_49cd8c_insert_element(MUSB_LinkedList* list, int i, void* data, int d)
{
   MUSB_LinkedList* p;
   int res = 0;
   
   if (list == 0) return 0;
   
   if (i == 0) 
   {
      p = FAPI_SYS_MALLOC(sizeof(MUSB_LinkedList));
      if (p != 0) 
      {
         res = 1;  
         p->dwCount = list->dwCount;
         p->data = list->data;
         p->pNext = list->pNext;
         
         list->dwCount = d;
         list->data = data;
         list->pNext = p;
      }
   }
   else 
   {
      p = List_49cd8c_get_element_ptr(list, i); 
      if (p != 0) 
      {
         res = MGC_ListInsertItem(p, data, d);
      }
   }
   
   return res;
}


/* 49cf7c - todo */
int List_49cd8c_get_element_count(MUSB_LinkedList* list)
{
   MUSB_LinkedList* next = list;
   unsigned short count = 0;
   
   if ((list != 0) && (list->data != 0))
   {
      next = list->pNext;
      count = 1;
      
      while (next != 0)
      {
         count++;
         next = next->pNext;
      }
   }
   
   return count;
}


/* 49cfc0 - complete */
void List_49cd8c_remove_element(MUSB_LinkedList* list, void* data)
{
   MUSB_LinkedList* head = list;
   MUSB_LinkedList* r0 = head;
   
   if (head != 0) 
   {
      if (head->data == data) 
      {
         r0 = head->pNext;
         if (r0 != 0) 
         {
            head->data = r0->data;
            head->dwCount = r0->dwCount;
            head->pNext = r0->pNext;
            FAPI_SYS_FREE(r0);
         }
         else 
         {
            // It was the one and only element
            head->data = 0;
            head->dwCount = 0;
            head->pNext = 0;
         }
      }
      else 
      {
         MUSB_LinkedList* r3 = r0->pNext;
         while ((r3 != 0) && (r3->data != data))
         {
            r0 = r3;   
            r3 = r0->pNext;
         }

         if (r3->data == data) 
         {
            MUSB_LinkedList* r1 = r3;
            if (r1->pNext != 0)
            {
               r3->data = r1->pNext->data;
               r0->pNext->dwCount = r1->pNext->dwCount;
               r0->pNext = r1->pNext;
            }
            else 
            {
               r0->pNext = 0;
            }

            FAPI_SYS_FREE(r1);
         }
      }
   }
}


