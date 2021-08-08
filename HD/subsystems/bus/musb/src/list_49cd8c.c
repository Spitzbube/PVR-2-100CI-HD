

#include <fapi/sys_services.h>
#include "musb/list_49cd8c.h"



/* 496fa4 - complete */
/* Allocate an element in the list and set its data */
static int func_496fa4(List_49cd8c* list, void* data, int c)
{
   int res = 0;
   List_49cd8c* elem = FAPI_SYS_MALLOC(sizeof(List_49cd8c));

   if ((list != 0) && (elem != 0))
   {
      res = 1;
      elem->next = list->next;
      list->next = elem;
      elem->data = data;
      elem->c = c;
   }
   
   return res;
}


/* 21ccdc94 - complete */
/* Add an element in the list and set its data */
int List_49cd8c_add_element(List_49cd8c* list, void* data, int c)
{   
   int res;
   List_49cd8c* tail = list;
   
   if (list == 0) return 0;
   else 
   {
      if (list->data == 0)
      {
         // First list element is free and can be used 
         res = 1;
         list->data = data;
         list->c = c;
         list->next = 0;
      }
      else 
      {
         // Go to the end of the list
         while (tail->next != 0) tail = tail->next;

         // Allocate an element at the end of the list
         res = func_496fa4(tail, data, c);
      }
   
      return res;
   }
}


/* 21ccdc00 - complete */
void* List_49cd8c_get_element(List_49cd8c* a, int b)
{
   void* elem = 0;
   List_49cd8c* p = List_49cd8c_get_element_ptr(a, b);
   
   if (p != 0) 
   {
      elem = p->data;      
   }
   
   return elem;
}


/* 21ccdc54 - complete */
List_49cd8c* List_49cd8c_get_element_ptr(List_49cd8c* a1, int b)
{
   List_49cd8c* a = a1;
   unsigned short i = 0;
   a1 = 0;
   
   while ((i < b) && (a != 0)) 
   {
      i++;
      a = a->next;      
   }
   
   if (b == i) a1 = a;
   
   return a1;
}


/* 21ccdb98 - complete */
void List_49cd8c_clear(List_49cd8c* list)
{
   if (list != 0) 
   {
      list->next = 0;
      list->c = 0;
      list->data = 0;
   }
}


/* 49ceb4 - complete */
int List_49cd8c_insert_element(List_49cd8c* list, int i, void* data, int d)
{
   List_49cd8c* p;
   int res = 0;
   
   if (list == 0) return 0;
   
   if (i == 0) 
   {
      p = FAPI_SYS_MALLOC(sizeof(List_49cd8c));
      if (p != 0) 
      {
         res = 1;  
         p->c = list->c;
         p->data = list->data;
         p->next = list->next;
         
         list->c = d;
         list->data = data;
         list->next = p;
      }
   }
   else 
   {
      p = List_49cd8c_get_element_ptr(list, i); 
      if (p != 0) 
      {
         res = func_496fa4(p, data, d);   
      }
   }
   
   return res;
}


/* 49cf7c - todo */
int List_49cd8c_get_element_count(List_49cd8c* list)
{
   List_49cd8c* next = list;
   unsigned short count = 0;
   
   if ((list != 0) && (list->data != 0))
   {
      next = list->next;
      count = 1;
      
      while (next != 0)
      {
         count++;
         next = next->next;
      }
   }
   
   return count;
}


/* 49cfc0 - complete */
void List_49cd8c_remove_element(List_49cd8c* list, void* data)
{
   List_49cd8c* head = list;
   List_49cd8c* r0 = head;
   
   if (head != 0) 
   {
      if (head->data == data) 
      {
         r0 = head->next;
         if (r0 != 0) 
         {
            head->data = r0->data;
            head->c = r0->c;
            head->next = r0->next;
            FAPI_SYS_FREE(r0);
         }
         else 
         {
            // It was the one and only element
            head->data = 0;
            head->c = 0;
            head->next = 0;
         }
      }
      else 
      {
         List_49cd8c* r3 = r0->next;
         while ((r3 != 0) && (r3->data != data))
         {
            r0 = r3;   
            r3 = r0->next;
         }

         if (r3->data == data) 
         {
            List_49cd8c* r1 = r3;  
            if (r1->next != 0) 
            {
               r3->data = r1->next->data;
               r0->next->c = r1->next->c;
               r0->next = r1->next;
            }
            else 
            {
               r0->next = 0;
            }

            FAPI_SYS_FREE(r1);
         }
      }
   }
}


