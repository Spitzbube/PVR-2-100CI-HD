

typedef struct List_49cd8c_t
{
   void* data; //0
   struct List_49cd8c_t* next; //4
   int c; //8
} List_49cd8c;



extern int List_49cd8c_add_element(List_49cd8c* list, void* data, int c);
extern void* List_49cd8c_get_element(List_49cd8c* list, int b);
extern List_49cd8c* List_49cd8c_get_element_ptr(List_49cd8c* a1, int b);
extern void List_49cd8c_clear(List_49cd8c* list);
extern int List_49cd8c_get_element_count(List_49cd8c* list);
extern void List_49cd8c_remove_element(List_49cd8c* list, void* data);
