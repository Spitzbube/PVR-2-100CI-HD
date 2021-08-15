


typedef struct
{
   unsigned short elem_size; //0
   unsigned short count; //2
   void* data; //4
   unsigned short num_elements; //8
   unsigned short e; //10
   void* more_data; //12
   //16
} MUSB_Array; /*50*/


typedef struct _MUSB_LinkedList /*110*/
{
   void* data; //0
   struct _MUSB_LinkedList* pNext; //4
   unsigned int dwCount; //8
} MUSB_LinkedList; /*114*/



extern void* MUSB_ArrayFetch(MUSB_Array* pArray, unsigned short wIndex);
extern uint8_t MUSB_ListAppendItem(MUSB_LinkedList* list, void* data, unsigned int c);
extern void* MUSB_ListFindItem(MUSB_LinkedList* list, int b);
extern MUSB_LinkedList* List_49cd8c_get_element_ptr(MUSB_LinkedList* a1, int b);
extern void MUSB_ListInit(MUSB_LinkedList* list);
extern int List_49cd8c_get_element_count(MUSB_LinkedList* list);
extern void List_49cd8c_remove_element(MUSB_LinkedList* list, void* data);
