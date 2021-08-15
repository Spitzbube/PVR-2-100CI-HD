

#include <stdint.h>
#include <stddef.h>


/* 21ccf048 - todo */
void MGC_FAPI_MemSet(void* ptr, uint8_t c, size_t iSize) /*103*/
{
   uint8_t* u8_ptr = ptr;

   if ((int)u8_ptr & 0x03)
   {
      while (iSize--)
      {
          *u8_ptr++ = c;
      }
   }
   {
      memset(ptr, c, iSize);
   }
}


