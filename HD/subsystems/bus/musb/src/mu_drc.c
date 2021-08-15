
#include <string.h>
#include <fapi/sys_services.h>
#include <fapi/reg_dma.h>
#include <fapi/drv_dma.h>
#include <fapi/drv_usb.h>
#include "musb/usb_ch9.h"
#include "musb/musbhdrc.h"
#include "musb/musbdefs.h"
#include "musb/list_49cd8c.h"
#include "musb/musb_dma.h"
#include "musb/musb.h"


extern uint8_t MGC_Read8(uint8_t* baseAddr, uint16_t offset);
extern uint16_t MGC_Read16(uint8_t* baseAddr, uint16_t offset);
extern void MGC_Write8(uint8_t* baseAddr, uint16_t offset, uint8_t);
extern MUSB_Array* MUSB_ArrayInit(MUSB_Array* pArray,
   uint16_t wItemSize, uint16_t wStaticItemCount, void* pStaticBuffer);
extern uint8_t MUSB_ArrayAppend(MUSB_Array* pArray, void* pItem);


/* 21cc8000 - complete */
uint8_t MGC_DrcInit(MGC_Port* pPort/*fp128*/) /*120*/
{
    uint8_t bEnd; /*fp26*/
    MGC_EndpointResource end; /*fp124*/ /*123*/
    uint8_t bFifoSize; /*fp25*/
    MGC_Controller* pImplController = pPort->pController; /*fp24*/
    uint8_t* pBase = pImplController->pControllerAddressIst; /*fp20*/
    uint8_t bOk = 0; /*fp13*/

   /* At least Endpoint 0 */
   pPort->bEndCount = 1;
   pPort->wEndMask = (1 << 0);

   for (bEnd = 1; bEnd < MUSB_C_NUM_EPS; bEnd++)
   {
      MGC_SelectEnd(pBase, bEnd);

      bFifoSize = MGC_ReadCsr8(pBase, MGC_O_HDRC_FIFOSIZE, bEnd);

      if (bFifoSize == 0)
      {
         /* 0's returned when no more endpoints */
         break;
      }

      pPort->bEndCount++;
      pPort->wEndMask |= (1 << bEnd);
   }

   pPort->pPrivateData = FAPI_SYS_MALLOC(pPort->bEndCount * sizeof(MGC_EndpointResource));

   if (pPort->pPrivateData != 0)
   {
      // Add pPort->c to pPort->Data_12
      if (0 != MUSB_ArrayInit(&pPort->LocalEnds,
         sizeof(MGC_EndpointResource), pPort->bEndCount, pPort->pPrivateData))
      {
          bOk = 1;

         MGC_FAPI_MemSet(&end, 0, sizeof(MGC_EndpointResource));

         end.bIsFifoShared = 1;
         end.bTrafficType = end.bRxTrafficType = 0;
         end.wMaxPacketSizeTx = 64;
         end.wMaxPacketSizeRx = 64;
         end.bIsClaimed = end.bRxClaimed = 1;

         // Add ep0 to pPort->ep_list
         MUSB_ArrayAppend(&pPort->LocalEnds, &end);

         for (bEnd = 1; bEnd < pPort->bEndCount; bEnd++)
         {
            MGC_SelectEnd(pBase, bEnd);

            bFifoSize = MGC_ReadCsr8(pBase, MGC_O_HDRC_FIFOSIZE, bEnd);

            MGC_FAPI_MemSet(&end, 0, sizeof(MGC_EndpointResource));

            end.bLocalEnd = bEnd;
            end.wMaxPacketSizeTx = 1 << (bFifoSize & 0x0F);
            /* shared TX/RX FIFO? */
            if ((bFifoSize & 0xF0) == 0xF0)
            {
               end.wMaxPacketSizeRx = 1 << (bFifoSize & 0x0F);
               end.bIsFifoShared = 1;
            }
            else
            {
               end.wMaxPacketSizeRx = 1 << ((bFifoSize & 0xF0) >> 4);
               end.bIsFifoShared = 0;
            }

            // Add ep to pPort->list_12
            MUSB_ArrayAppend(&pPort->LocalEnds, &end);
         }
      }
      else
      {
          //2f0
         (FAPI_SYS_Services.freeFunc)(pPort->pPrivateData);
      }
   }
   //308

   MGC_Write8(pBase, 14, 0);

   return bOk;
}




#if 0

mu_drc-g.o:     file format elf32-littlearm
rw-rw-rw- 1007/513  55232 Nov 17 16:29 2010 mu_drc-g.o
architecture: arm, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x00000000
private flags = 204: [interworking enabled] [APCS-32] [FPA float format] [software FP]

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         0000427c  00000000  00000000  00000034  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000058  00000000  00000000  000042b0  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
  2 .bss          00000000  00000000  00000000  00004308  2**0
                  ALLOC
  3 .debug_abbrev 0000026d  00000000  00000000  00004308  2**0
                  CONTENTS, READONLY, DEBUGGING
  4 .debug_info   00005720  00000000  00000000  00004575  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  5 .debug_line   0000054f  00000000  00000000  00009c95  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  6 .rodata       0000029c  00000000  00000000  0000a1e4  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .debug_frame  00000454  00000000  00000000  0000a480  2**2
                  CONTENTS, RELOC, READONLY, DEBUGGING
  8 .debug_loc    00000594  00000000  00000000  0000a8d4  2**0
                  CONTENTS, READONLY, DEBUGGING
  9 .debug_pubnames 000001a9  00000000  00000000  0000ae68  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 10 .debug_aranges 00000020  00000000  00000000  0000b011  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 11 .debug_str    000001e9  00000000  00000000  0000b031  2**0
                  CONTENTS, READONLY, DEBUGGING
 12 .comment      00000012  00000000  00000000  0000b21a  2**0
                  CONTENTS, READONLY
 13 .ARM.attributes 00000010  00000000  00000000  0000b22c  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00000000 l    df *ABS*  00000000 mu_drc.c
00000000 l    d  .text  00000000 .text
00000000 l    d  .data  00000000 .data
00000000 l    d  .bss   00000000 .bss
00000000 l    d  .debug_abbrev  00000000 .debug_abbrev
00000000 l    d  .debug_info    00000000 .debug_info
00000000 l    d  .debug_line    00000000 .debug_line
00000000 l    d  .rodata    00000000 .rodata
00000000 l     O .rodata    00000008 MGC_aSetHnpSupportData
00000008 l     O .rodata    00000008 MGC_aSetHnpEnableData
00000000 l     O .data  0000002c MGC_SetHnpSupportIrp
00001f00 l     F .text  00000020 MGC_SetHnpSupportIrpComplete
0000002c l     O .data  0000002c MGC_SetHnpEnableIrp
00001f20 l     F .text  000000a4 MGC_SetHnpEnableIrpComplete
000013cc l     F .text  0000022c MGC_DrcFlushAll
0000172c l     F .text  00000090 MGC_ExitingHost
000017bc l     F .text  00000054 MGC_ExitActiveUsbMode
00001810 l     F .text  00000024 MGC_SetPowerSave
00001834 l     F .text  00000018 MGC_StartPeripheralMode
0000184c l     F .text  000000e4 MGC_AidlBdisTimeout
00001930 l     F .text  000000e4 MGC_WaitSessReqTimeout
00001a14 l     F .text  000000d4 MGC_HostResetStart
00001ae8 l     F .text  000001a4 MGC_HostResetComplete
00001c8c l     F .text  00000188 MGC_BSrpFailed
00001e14 l     F .text  000000ec MGC_BAase0BrstTimeout
00001fc4 l     F .text  00000054 MGC_RemoveSuspend
00002634 l     F .text  00000110 MGC_DrcCompleteResume
00003ed8 l     F .text  000000c0 MGC_OtgStateGetId
00000000 l    d  .debug_frame   00000000 .debug_frame
00000000 l    d  .debug_loc 00000000 .debug_loc
00000000 l    d  .debug_pubnames    00000000 .debug_pubnames
00000000 l    d  .debug_aranges 00000000 .debug_aranges
00000000 l    d  .debug_str 00000000 .debug_str
00000000 l    d  .comment   00000000 .comment
00000000 l    d  .ARM.attributes    00000000 .ARM.attributes
00000000 g     F .text  0000032c MGC_DrcInit
00000000         *UND*  00000000 MGC_FAPI_WriteReg8
00000000         *UND*  00000000 MGC_FAPI_ReadReg8
00000000         *UND*  00000000 MUSB_ArrayInit
00000000         *UND*  00000000 MGC_FAPI_MemSet
00000000         *UND*  00000000 MUSB_ArrayAppend
00000000         *UND*  00000000 FAPI_SYS_Services
0000032c g     F .text  00000478 MGC_DrcIsr
00002744 g     F .text  000004d8 MGC_DrcUsbIsr
00000000         *UND*  00000000 MGC_DiagMsg
00000000         *UND*  00000000 MGC_bDiagLevel
000007a4 g     F .text  000003d0 MGC_DrcBindEndpoint
00000000         *UND*  00000000 MUSB_ArrayLength
00000000         *UND*  00000000 MUSB_ArrayFetch
00000b74 g     F .text  0000074c MGC_DrcServiceDefaultEnd
00000000         *UND*  00000000 MGC_CompleteIrp
00000000         *UND*  00000000 MGC_StartNextControlTransfer
00003688 g     F .text  000004e4 MGC_DrcServiceHostDefault
00000000         *UND*  00000000 MGC_FunctionParseSetup
000012c0 g     F .text  0000010c MGC_DrcResetPort
000025bc g     F .text  00000078 MGC_CompleteOtgTransition
00000000         *UND*  00000000 MUSB_ListLength
00000000         *UND*  00000000 MUSB_ListFindItem
00000000         *UND*  00000000 MUSB_ListRemoveItem
000015f8 g     F .text  00000134 MGC_DrcOtgTimer
00002018 g     F .text  000005a4 MGC_DrcChangeOtgState
00000000         *UND*  00000000 MGC_FunctionChangeState
00000000         *UND*  00000000 MGC_AllocateAddress
00000000         *UND*  00000000 MGC_EnumerateDevice
00000000         *UND*  00000000 MGC_FindDescriptor
00000000         *UND*  00000000 MUSB_StartControlTransfer
00000000         *UND*  00000000 MUSB_SuspendBus
00000000         *UND*  00000000 FAPI_USB_KeepVbusFlag
00000000         *UND*  00000000 MGC_FunctionSpeedSet
00000000         *UND*  00000000 MGC_RunScheduledTransfers
00002c1c g     F .text  00000a6c MGC_DrcBsr
00000000         *UND*  00000000 MGC_HostDestroy
00000000         *UND*  00000000 MUSB_DeviceDisconnected
00000000         *UND*  00000000 FAPI_DMA_GetRequestList
00000000         *UND*  00000000 FAPI_DMA_GetRequestStatus
00000000         *UND*  00000000 FAPI_DMA_CancelRequest
00000000         *UND*  00000000 MGC_StartNextIrp
00003b6c g     F .text  0000024c MGC_DrcAcceptDevice
000040fc g     F .text  000000d0 MUSB_RelinquishHost
00003db8 g     F .text  0000008c MGC_DrcFinishResume
00003e44 g     F .text  00000094 MGC_DrcSetHostPower
00000000         *UND*  00000000 MGC_HostSetMaxPower
00003f98 g     F .text  00000164 MUSB_RegisterOtgClient
00000000         *UND*  00000000 MGC_FunctionRegisterClient
000041cc g     F .text  00000088 MUSB_RequestBus
00004254 g     F .text  00000028 MGC_DrcDumpPipe


Contents of the .debug_abbrev section:

  Number TAG
   1      DW_TAG_compile_unit    [has children]
    DW_AT_producer     DW_FORM_string
    DW_AT_language     DW_FORM_data1
    DW_AT_name         DW_FORM_string
    DW_AT_comp_dir     DW_FORM_string
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_stmt_list    DW_FORM_data4
   2      DW_TAG_typedef    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
   3      DW_TAG_base_type    [no children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_encoding     DW_FORM_data1
    DW_AT_name         DW_FORM_string
   4      DW_TAG_base_type    [no children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_encoding     DW_FORM_data1
   5      DW_TAG_pointer_type    [no children]
    DW_AT_byte_size    DW_FORM_data1
   6      DW_TAG_subroutine_type    [no children]
    DW_AT_prototyped   DW_FORM_flag
   7      DW_TAG_pointer_type    [no children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
   8      DW_TAG_const_type    [no children]
    DW_AT_type         DW_FORM_ref4
   9      DW_TAG_enumeration_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   10      DW_TAG_enumerator    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_const_value  DW_FORM_sdata
   11      DW_TAG_structure_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   12      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   13      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   14      DW_TAG_subroutine_type    [no children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
   15      DW_TAG_subroutine_type    [has children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_sibling      DW_FORM_ref4
   16      DW_TAG_formal_parameter    [no children]
    DW_AT_type         DW_FORM_ref4
   17      DW_TAG_typedef    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
   18      DW_TAG_structure_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_sibling      DW_FORM_ref4
   19      DW_TAG_subroutine_type    [has children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
    DW_AT_sibling      DW_FORM_ref4
   20      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   21      DW_TAG_const_type    [no children]
   22      DW_TAG_structure_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_sibling      DW_FORM_ref4
   23      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   24      DW_TAG_structure_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   25      DW_TAG_enumeration_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   26      DW_TAG_structure_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data2
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   27      DW_TAG_structure_type    [has children]
    DW_AT_byte_size    DW_FORM_data2
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_sibling      DW_FORM_ref4
   28      DW_TAG_array_type    [has children]
    DW_AT_type         DW_FORM_ref4
    DW_AT_sibling      DW_FORM_ref4
   29      DW_TAG_subrange_type    [no children]
    DW_AT_type         DW_FORM_ref4
    DW_AT_upper_bound  DW_FORM_data1
   30      DW_TAG_unspecified_parameters    [no children]
   31      DW_TAG_subprogram    [has children]
    DW_AT_external     DW_FORM_flag
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   32      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   33      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   34      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   35      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   36      DW_TAG_subprogram    [has children]
    DW_AT_external     DW_FORM_flag
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   37      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   38      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   39      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   40      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   41      DW_TAG_subprogram    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   42      DW_TAG_subprogram    [has children]
    DW_AT_external     DW_FORM_flag
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   43      DW_TAG_subprogram    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   44      DW_TAG_lexical_block    [has children]
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
   45      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_external     DW_FORM_flag
    DW_AT_declaration  DW_FORM_flag
   46      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_external     DW_FORM_flag
    DW_AT_declaration  DW_FORM_flag

The section .debug_info contains:

  Compilation Unit @ offset 0x0:
   Length:        22300
   Version:       2
   Abbrev Offset: 0
   Pointer Size:  4
 <0><b>: Abbrev Number: 1 (DW_TAG_compile_unit)
  < c>     DW_AT_producer    : GNU C 4.2.2
  <18>     DW_AT_language    : 1    (ANSI C)
  <19>     DW_AT_name        : ../cd/mu_drc.c
  <28>     DW_AT_comp_dir    : /cygdrive/d/PROJECTS/MB86H60/software/HDTV_branch/subsystems/bus/musb/src/systems
  <7a>     DW_AT_low_pc      : 0
  <7e>     DW_AT_high_pc     : 0x427c
  <82>     DW_AT_stmt_list   : 0
 <1><86>: Abbrev Number: 2 (DW_TAG_typedef)
  <87>     DW_AT_name        : size_t
  <8e>     DW_AT_decl_file   : 2
  <8f>     DW_AT_decl_line   : 214
  <90>     DW_AT_type        : <94>
 <1><94>: Abbrev Number: 3 (DW_TAG_base_type)
  <95>     DW_AT_byte_size   : 4
  <96>     DW_AT_encoding    : 7    (unsigned)
  <97>     DW_AT_name        : long unsigned int
 <1><a9>: Abbrev Number: 3 (DW_TAG_base_type)
  <aa>     DW_AT_byte_size   : 4
  <ab>     DW_AT_encoding    : 5    (signed)
  <ac>     DW_AT_name        : int
 <1><b0>: Abbrev Number: 3 (DW_TAG_base_type)
  <b1>     DW_AT_byte_size   : 4
  <b2>     DW_AT_encoding    : 5    (signed)
  <b3>     DW_AT_name        : long int
 <1><bc>: Abbrev Number: 3 (DW_TAG_base_type)
  <bd>     DW_AT_byte_size   : 8
  <be>     DW_AT_encoding    : 5    (signed)
  <bf>     DW_AT_name        : long long int
 <1><cd>: Abbrev Number: 3 (DW_TAG_base_type)
  <ce>     DW_AT_byte_size   : 4
  <cf>     DW_AT_encoding    : 7    (unsigned)
  <d0>     DW_AT_name        : unsigned int
 <1><dd>: Abbrev Number: 4 (DW_TAG_base_type)
  <de>     DW_AT_byte_size   : 4
  <df>     DW_AT_encoding    : 7    (unsigned)
 <1><e0>: Abbrev Number: 3 (DW_TAG_base_type)
  <e1>     DW_AT_byte_size   : 1
  <e2>     DW_AT_encoding    : 8    (unsigned char)
  <e3>     DW_AT_name        : unsigned char
 <1><f1>: Abbrev Number: 5 (DW_TAG_pointer_type)
  <f2>     DW_AT_byte_size   : 4
 <1><f3>: Abbrev Number: 6 (DW_TAG_subroutine_type)
  <f4>     DW_AT_prototyped  : 1
 <1><f5>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f6>     DW_AT_byte_size   : 4
  <f7>     DW_AT_type        : <f3>
 <1><fb>: Abbrev Number: 3 (DW_TAG_base_type)
  <fc>     DW_AT_byte_size   : 2
  <fd>     DW_AT_encoding    : 5    (signed)
  <fe>     DW_AT_name        : short int
 <1><108>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <109>     DW_AT_byte_size   : 4
  <10a>     DW_AT_type        : <10e>
 <1><10e>: Abbrev Number: 3 (DW_TAG_base_type)
  <10f>     DW_AT_byte_size   : 1
  <110>     DW_AT_encoding    : 8   (unsigned char)
  <111>     DW_AT_name        : char
 <1><116>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <117>     DW_AT_byte_size   : 4
  <118>     DW_AT_type        : <11c>
 <1><11c>: Abbrev Number: 8 (DW_TAG_const_type)
  <11d>     DW_AT_type        : <10e>
 <1><121>: Abbrev Number: 3 (DW_TAG_base_type)
  <122>     DW_AT_byte_size   : 2
  <123>     DW_AT_encoding    : 7   (unsigned)
  <124>     DW_AT_name        : short unsigned int
 <1><137>: Abbrev Number: 3 (DW_TAG_base_type)
  <138>     DW_AT_byte_size   : 8
  <139>     DW_AT_encoding    : 7   (unsigned)
  <13a>     DW_AT_name        : long long unsigned int
 <1><151>: Abbrev Number: 3 (DW_TAG_base_type)
  <152>     DW_AT_byte_size   : 1
  <153>     DW_AT_encoding    : 6   (signed char)
  <154>     DW_AT_name        : signed char
 <1><160>: Abbrev Number: 2 (DW_TAG_typedef)
  <161>     DW_AT_name        : uint8_t
  <169>     DW_AT_decl_file   : 3
  <16a>     DW_AT_decl_line   : 46
  <16b>     DW_AT_type        : <e0>
 <1><16f>: Abbrev Number: 2 (DW_TAG_typedef)
  <170>     DW_AT_name        : uint16_t
  <179>     DW_AT_decl_file   : 3
  <17a>     DW_AT_decl_line   : 58
  <17b>     DW_AT_type        : <121>
 <1><17f>: Abbrev Number: 2 (DW_TAG_typedef)
  <180>     DW_AT_name        : int32_t
  <188>     DW_AT_decl_file   : 3
  <189>     DW_AT_decl_line   : 83
  <18a>     DW_AT_type        : <b0>
 <1><18e>: Abbrev Number: 2 (DW_TAG_typedef)
  <18f>     DW_AT_name        : uint32_t
  <198>     DW_AT_decl_file   : 3
  <199>     DW_AT_decl_line   : 84
  <19a>     DW_AT_type        : <94>
 <1><19e>: Abbrev Number: 2 (DW_TAG_typedef)
  <19f>     DW_AT_name        : uint_fast16_t
  <1ad>     DW_AT_decl_file   : 3
  <1ae>     DW_AT_decl_line   : 170
  <1af>     DW_AT_type        : <cd>
 <1><1b3>: Abbrev Number: 2 (DW_TAG_typedef)
  <1b4>     DW_AT_name        : FAPI_SYS_HandleT
  <1c5>     DW_AT_decl_file   : 4
  <1c6>     DW_AT_decl_line   : 71
  <1c7>     DW_AT_type        : <f1>
 <1><1cb>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <1cc>     DW_AT_byte_size   : 4
  <1cd>     DW_AT_decl_file   : 4
  <1ce>     DW_AT_decl_line   : 83
  <1cf>     DW_AT_sibling     : <225>
 <2><1d3>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1d4>     DW_AT_name        : FAPI_SYS_BSR_EXEC_UNDEFINED
  <1f0>     DW_AT_const_value : 0
 <2><1f1>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1f2>     DW_AT_name        : FAPI_SYS_BSR_EXEC_THREAD
  <20b>     DW_AT_const_value : 1
 <2><20c>: Abbrev Number: 10 (DW_TAG_enumerator)
  <20d>     DW_AT_name        : FAPI_SYS_BSR_EXEC_IRQ
  <223>     DW_AT_const_value : 2
 <1><225>: Abbrev Number: 2 (DW_TAG_typedef)
  <226>     DW_AT_name        : FAPI_SYS_BsrExecutionEnumT
  <241>     DW_AT_decl_file   : 4
  <242>     DW_AT_decl_line   : 125
  <243>     DW_AT_type        : <1cb>
 <1><247>: Abbrev Number: 11 (DW_TAG_structure_type)
  <248>     DW_AT_byte_size   : 36
  <249>     DW_AT_decl_file   : 4
  <24a>     DW_AT_decl_line   : 139
  <24b>     DW_AT_sibling     : <317>
 <2><24f>: Abbrev Number: 12 (DW_TAG_member)
  <250>     DW_AT_name        : driverNamePtr
  <25e>     DW_AT_decl_file   : 4
  <25f>     DW_AT_decl_line   : 150
  <260>     DW_AT_type        : <116>
  <264>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><267>: Abbrev Number: 12 (DW_TAG_member)
  <268>     DW_AT_name        : irqNo
  <26e>     DW_AT_decl_file   : 4
  <26f>     DW_AT_decl_line   : 163
  <270>     DW_AT_type        : <17f>
  <274>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><277>: Abbrev Number: 12 (DW_TAG_member)
  <278>     DW_AT_name        : bsrExecution
  <285>     DW_AT_decl_file   : 4
  <286>     DW_AT_decl_line   : 177
  <287>     DW_AT_type        : <225>
  <28b>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><28e>: Abbrev Number: 12 (DW_TAG_member)
  <28f>     DW_AT_name        : initDriverFunc
  <29e>     DW_AT_decl_file   : 4
  <29f>     DW_AT_decl_line   : 206
  <2a0>     DW_AT_type        : <31d>
  <2a4>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><2a7>: Abbrev Number: 12 (DW_TAG_member)
  <2a8>     DW_AT_name        : exitDriverFunc
  <2b7>     DW_AT_decl_file   : 4
  <2b8>     DW_AT_decl_line   : 222
  <2b9>     DW_AT_type        : <f5>
  <2bd>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><2c0>: Abbrev Number: 13 (DW_TAG_member)
  <2c1>     DW_AT_name        : isrFunc
  <2c9>     DW_AT_decl_file   : 4
  <2ca>     DW_AT_decl_line   : 257
  <2cc>     DW_AT_type        : <329>
  <2d0>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><2d3>: Abbrev Number: 13 (DW_TAG_member)
  <2d4>     DW_AT_name        : bsrFunc
  <2dc>     DW_AT_decl_file   : 4
  <2dd>     DW_AT_decl_line   : 291
  <2df>     DW_AT_type        : <33b>
  <2e3>     DW_AT_data_member_location: 2 byte block: 23 18     (DW_OP_plus_uconst: 24)
 <2><2e6>: Abbrev Number: 13 (DW_TAG_member)
  <2e7>     DW_AT_name        : bsrStackSize
  <2f4>     DW_AT_decl_file   : 4
  <2f5>     DW_AT_decl_line   : 302
  <2f7>     DW_AT_type        : <18e>
  <2fb>     DW_AT_data_member_location: 2 byte block: 23 1c     (DW_OP_plus_uconst: 28)
 <2><2fe>: Abbrev Number: 13 (DW_TAG_member)
  <2ff>     DW_AT_name        : bsrFifoDepth
  <30c>     DW_AT_decl_file   : 4
  <30d>     DW_AT_decl_line   : 312
  <30f>     DW_AT_type        : <18e>
  <313>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <1><317>: Abbrev Number: 14 (DW_TAG_subroutine_type)
  <318>     DW_AT_prototyped  : 1
  <319>     DW_AT_type        : <17f>
 <1><31d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <31e>     DW_AT_byte_size   : 4
  <31f>     DW_AT_type        : <317>
 <1><323>: Abbrev Number: 14 (DW_TAG_subroutine_type)
  <324>     DW_AT_prototyped  : 1
  <325>     DW_AT_type        : <18e>
 <1><329>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <32a>     DW_AT_byte_size   : 4
  <32b>     DW_AT_type        : <323>
 <1><32f>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <330>     DW_AT_prototyped  : 1
  <331>     DW_AT_sibling     : <33b>
 <2><335>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <336>     DW_AT_type        : <18e>
 <1><33b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <33c>     DW_AT_byte_size   : 4
  <33d>     DW_AT_type        : <32f>
 <1><341>: Abbrev Number: 17 (DW_TAG_typedef)
  <342>     DW_AT_name        : FAPI_SYS_DriverT
  <353>     DW_AT_decl_file   : 4
  <354>     DW_AT_decl_line   : 313
  <356>     DW_AT_type        : <247>
 <1><35a>: Abbrev Number: 11 (DW_TAG_structure_type)
  <35b>     DW_AT_byte_size   : 76
  <35c>     DW_AT_decl_file   : 5
  <35d>     DW_AT_decl_line   : 247
  <35e>     DW_AT_sibling     : <59f>
 <2><362>: Abbrev Number: 12 (DW_TAG_member)
  <363>     DW_AT_name        : chConfig
  <36c>     DW_AT_decl_file   : 5
  <36d>     DW_AT_decl_line   : 248
  <36e>     DW_AT_type        : <18e>
  <372>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><375>: Abbrev Number: 12 (DW_TAG_member)
  <376>     DW_AT_name        : chLength
  <37f>     DW_AT_decl_file   : 5
  <380>     DW_AT_decl_line   : 249
  <381>     DW_AT_type        : <18e>
  <385>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><388>: Abbrev Number: 12 (DW_TAG_member)
  <389>     DW_AT_name        : chLladdr
  <392>     DW_AT_decl_file   : 5
  <393>     DW_AT_decl_line   : 250
  <394>     DW_AT_type        : <18e>
  <398>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><39b>: Abbrev Number: 12 (DW_TAG_member)
  <39c>     DW_AT_name        : chRdaddr_EndianSwap
  <3b0>     DW_AT_decl_file   : 5
  <3b1>     DW_AT_decl_line   : 251
  <3b2>     DW_AT_type        : <18e>
  <3b6>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><3b9>: Abbrev Number: 12 (DW_TAG_member)
  <3ba>     DW_AT_name        : chRdaddr_PeripheralAddress
  <3d5>     DW_AT_decl_file   : 5
  <3d6>     DW_AT_decl_line   : 252
  <3d7>     DW_AT_type        : <18e>
  <3db>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><3de>: Abbrev Number: 12 (DW_TAG_member)
  <3df>     DW_AT_name        : chRdaddr_OffsetAddress
  <3f6>     DW_AT_decl_file   : 5
  <3f7>     DW_AT_decl_line   : 253
  <3f8>     DW_AT_type        : <18e>
  <3fc>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><3ff>: Abbrev Number: 12 (DW_TAG_member)
  <400>     DW_AT_name        : chRdline_Lines
  <40f>     DW_AT_decl_file   : 5
  <410>     DW_AT_decl_line   : 254
  <411>     DW_AT_type        : <18e>
  <415>     DW_AT_data_member_location: 2 byte block: 23 18     (DW_OP_plus_uconst: 24)
 <2><418>: Abbrev Number: 12 (DW_TAG_member)
  <419>     DW_AT_name        : chRdline_LoopIncrement
  <430>     DW_AT_decl_file   : 5
  <431>     DW_AT_decl_line   : 255
  <432>     DW_AT_type        : <18e>
  <436>     DW_AT_data_member_location: 2 byte block: 23 1c     (DW_OP_plus_uconst: 28)
 <2><439>: Abbrev Number: 13 (DW_TAG_member)
  <43a>     DW_AT_name        : chRdinc_LineLength
  <44d>     DW_AT_decl_file   : 5
  <44e>     DW_AT_decl_line   : 256
  <450>     DW_AT_type        : <18e>
  <454>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <2><457>: Abbrev Number: 13 (DW_TAG_member)
  <458>     DW_AT_name        : chRdinc_LineIncrement
  <46e>     DW_AT_decl_file   : 5
  <46f>     DW_AT_decl_line   : 257
  <471>     DW_AT_type        : <18e>
  <475>     DW_AT_data_member_location: 2 byte block: 23 24     (DW_OP_plus_uconst: 36)
 <2><478>: Abbrev Number: 13 (DW_TAG_member)
  <479>     DW_AT_name        : chRdlpaddr_LoopAddress
  <490>     DW_AT_decl_file   : 5
  <491>     DW_AT_decl_line   : 258
  <493>     DW_AT_type        : <18e>
  <497>     DW_AT_data_member_location: 2 byte block: 23 28     (DW_OP_plus_uconst: 40)
 <2><49a>: Abbrev Number: 13 (DW_TAG_member)
  <49b>     DW_AT_name        : chWraddr_EndianSwap
  <4af>     DW_AT_decl_file   : 5
  <4b0>     DW_AT_decl_line   : 259
  <4b2>     DW_AT_type        : <18e>
  <4b6>     DW_AT_data_member_location: 2 byte block: 23 2c     (DW_OP_plus_uconst: 44)
 <2><4b9>: Abbrev Number: 13 (DW_TAG_member)
  <4ba>     DW_AT_name        : chWraddr_PeripheralAddress
  <4d5>     DW_AT_decl_file   : 5
  <4d6>     DW_AT_decl_line   : 260
  <4d8>     DW_AT_type        : <18e>
  <4dc>     DW_AT_data_member_location: 2 byte block: 23 30     (DW_OP_plus_uconst: 48)
 <2><4df>: Abbrev Number: 13 (DW_TAG_member)
  <4e0>     DW_AT_name        : chWraddr_OffsetAddress
  <4f7>     DW_AT_decl_file   : 5
  <4f8>     DW_AT_decl_line   : 261
  <4fa>     DW_AT_type        : <18e>
  <4fe>     DW_AT_data_member_location: 2 byte block: 23 34     (DW_OP_plus_uconst: 52)
 <2><501>: Abbrev Number: 13 (DW_TAG_member)
  <502>     DW_AT_name        : chWrline_Lines
  <511>     DW_AT_decl_file   : 5
  <512>     DW_AT_decl_line   : 262
  <514>     DW_AT_type        : <18e>
  <518>     DW_AT_data_member_location: 2 byte block: 23 38     (DW_OP_plus_uconst: 56)
 <2><51b>: Abbrev Number: 13 (DW_TAG_member)
  <51c>     DW_AT_name        : chWrline_LoopIncrement
  <533>     DW_AT_decl_file   : 5
  <534>     DW_AT_decl_line   : 263
  <536>     DW_AT_type        : <18e>
  <53a>     DW_AT_data_member_location: 2 byte block: 23 3c     (DW_OP_plus_uconst: 60)
 <2><53d>: Abbrev Number: 13 (DW_TAG_member)
  <53e>     DW_AT_name        : chWrinc_LineLength
  <551>     DW_AT_decl_file   : 5
  <552>     DW_AT_decl_line   : 264
  <554>     DW_AT_type        : <18e>
  <558>     DW_AT_data_member_location: 2 byte block: 23 40     (DW_OP_plus_uconst: 64)
 <2><55b>: Abbrev Number: 13 (DW_TAG_member)
  <55c>     DW_AT_name        : chWrinc_LineIncrement
  <572>     DW_AT_decl_file   : 5
  <573>     DW_AT_decl_line   : 265
  <575>     DW_AT_type        : <18e>
  <579>     DW_AT_data_member_location: 2 byte block: 23 44     (DW_OP_plus_uconst: 68)
 <2><57c>: Abbrev Number: 13 (DW_TAG_member)
  <57d>     DW_AT_name        : chWrlpaddr_LoopAddress
  <594>     DW_AT_decl_file   : 5
  <595>     DW_AT_decl_line   : 266
  <597>     DW_AT_type        : <18e>
  <59b>     DW_AT_data_member_location: 2 byte block: 23 48     (DW_OP_plus_uconst: 72)
 <1><59f>: Abbrev Number: 17 (DW_TAG_typedef)
  <5a0>     DW_AT_name        : FAPI_DMA_ChannelConfigT
  <5b8>     DW_AT_decl_file   : 5
  <5b9>     DW_AT_decl_line   : 268
  <5bb>     DW_AT_type        : <35a>
 <1><5bf>: Abbrev Number: 17 (DW_TAG_typedef)
  <5c0>     DW_AT_name        : FAPI_DMA_RequestT
  <5d2>     DW_AT_decl_file   : 5
  <5d3>     DW_AT_decl_line   : 286
  <5d5>     DW_AT_type        : <5d9>
 <1><5d9>: Abbrev Number: 18 (DW_TAG_structure_type)
  <5da>     DW_AT_name        : FAPI_DMA_RequestS
  <5ec>     DW_AT_byte_size   : 140
  <5ed>     DW_AT_decl_file   : 5
  <5ee>     DW_AT_decl_line   : 285
  <5f0>     DW_AT_sibling     : <797>
 <2><5f4>: Abbrev Number: 13 (DW_TAG_member)
  <5f5>     DW_AT_name        : feature
  <5fd>     DW_AT_decl_file   : 5
  <5fe>     DW_AT_decl_line   : 331
  <600>     DW_AT_type        : <18e>
  <604>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><607>: Abbrev Number: 13 (DW_TAG_member)
  <608>     DW_AT_name        : status
  <60f>     DW_AT_decl_file   : 5
  <610>     DW_AT_decl_line   : 339
  <612>     DW_AT_type        : <18e>
  <616>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><619>: Abbrev Number: 13 (DW_TAG_member)
  <61a>     DW_AT_name        : channel
  <622>     DW_AT_decl_file   : 5
  <623>     DW_AT_decl_line   : 346
  <625>     DW_AT_type        : <18e>
  <629>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><62c>: Abbrev Number: 13 (DW_TAG_member)
  <62d>     DW_AT_name        : result
  <634>     DW_AT_decl_file   : 5
  <635>     DW_AT_decl_line   : 356
  <637>     DW_AT_type        : <17f>
  <63b>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><63e>: Abbrev Number: 13 (DW_TAG_member)
  <63f>     DW_AT_name        : selfDestroy
  <64b>     DW_AT_decl_file   : 5
  <64c>     DW_AT_decl_line   : 366
  <64e>     DW_AT_type        : <18e>
  <652>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><655>: Abbrev Number: 13 (DW_TAG_member)
  <656>     DW_AT_name        : channelConfig
  <664>     DW_AT_decl_file   : 5
  <665>     DW_AT_decl_line   : 371
  <667>     DW_AT_type        : <59f>
  <66b>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><66e>: Abbrev Number: 13 (DW_TAG_member)
  <66f>     DW_AT_name        : preEnableCallback
  <681>     DW_AT_decl_file   : 5
  <682>     DW_AT_decl_line   : 380
  <684>     DW_AT_type        : <797>
  <688>     DW_AT_data_member_location: 2 byte block: 23 60     (DW_OP_plus_uconst: 96)
 <2><68b>: Abbrev Number: 13 (DW_TAG_member)
  <68c>     DW_AT_name        : postEnableCallback
  <69f>     DW_AT_decl_file   : 5
  <6a0>     DW_AT_decl_line   : 389
  <6a2>     DW_AT_type        : <797>
  <6a6>     DW_AT_data_member_location: 2 byte block: 23 64     (DW_OP_plus_uconst: 100)
 <2><6a9>: Abbrev Number: 13 (DW_TAG_member)
  <6aa>     DW_AT_name        : postFinishCallback
  <6bd>     DW_AT_decl_file   : 5
  <6be>     DW_AT_decl_line   : 397
  <6c0>     DW_AT_type        : <797>
  <6c4>     DW_AT_data_member_location: 2 byte block: 23 68     (DW_OP_plus_uconst: 104)
 <2><6c7>: Abbrev Number: 13 (DW_TAG_member)
  <6c8>     DW_AT_name        : optHandle
  <6d2>     DW_AT_decl_file   : 5
  <6d3>     DW_AT_decl_line   : 402
  <6d5>     DW_AT_type        : <1b3>
  <6d9>     DW_AT_data_member_location: 2 byte block: 23 6c     (DW_OP_plus_uconst: 108)
 <2><6dc>: Abbrev Number: 13 (DW_TAG_member)
  <6dd>     DW_AT_name        : optData
  <6e5>     DW_AT_decl_file   : 5
  <6e6>     DW_AT_decl_line   : 410
  <6e8>     DW_AT_type        : <f1>
  <6ec>     DW_AT_data_member_location: 2 byte block: 23 70     (DW_OP_plus_uconst: 112)
 <2><6ef>: Abbrev Number: 13 (DW_TAG_member)
  <6f0>     DW_AT_name        : optFinishCallback
  <702>     DW_AT_decl_file   : 5
  <703>     DW_AT_decl_line   : 417
  <705>     DW_AT_type        : <7ca>
  <709>     DW_AT_data_member_location: 2 byte block: 23 74     (DW_OP_plus_uconst: 116)
 <2><70c>: Abbrev Number: 13 (DW_TAG_member)
  <70d>     DW_AT_name        : optChangeCallback
  <71f>     DW_AT_decl_file   : 5
  <720>     DW_AT_decl_line   : 426
  <722>     DW_AT_type        : <7ca>
  <726>     DW_AT_data_member_location: 2 byte block: 23 78     (DW_OP_plus_uconst: 120)
 <2><729>: Abbrev Number: 13 (DW_TAG_member)
  <72a>     DW_AT_name        : autoRestart
  <736>     DW_AT_decl_file   : 5
  <737>     DW_AT_decl_line   : 441
  <739>     DW_AT_type        : <18e>
  <73d>     DW_AT_data_member_location: 2 byte block: 23 7c     (DW_OP_plus_uconst: 124)
 <2><740>: Abbrev Number: 13 (DW_TAG_member)
  <741>     DW_AT_name        : optSplitCount
  <74f>     DW_AT_decl_file   : 5
  <750>     DW_AT_decl_line   : 447
  <752>     DW_AT_type        : <18e>
  <756>     DW_AT_data_member_location: 3 byte block: 23 80 1   (DW_OP_plus_uconst: 128)
 <2><75a>: Abbrev Number: 13 (DW_TAG_member)
  <75b>     DW_AT_name        : completeSplitCount
  <76e>     DW_AT_decl_file   : 5
  <76f>     DW_AT_decl_line   : 454
  <771>     DW_AT_type        : <18e>
  <775>     DW_AT_data_member_location: 3 byte block: 23 84 1   (DW_OP_plus_uconst: 132)
 <2><779>: Abbrev Number: 13 (DW_TAG_member)
  <77a>     DW_AT_name        : optSplitCallback
  <78b>     DW_AT_decl_file   : 5
  <78c>     DW_AT_decl_line   : 462
  <78e>     DW_AT_type        : <7ca>
  <792>     DW_AT_data_member_location: 3 byte block: 23 88 1   (DW_OP_plus_uconst: 136)
 <1><797>: Abbrev Number: 17 (DW_TAG_typedef)
  <798>     DW_AT_name        : FAPI_DMA_CallbackT
  <7ab>     DW_AT_decl_file   : 5
  <7ac>     DW_AT_decl_line   : 287
  <7ae>     DW_AT_type        : <7b2>
 <1><7b2>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <7b3>     DW_AT_byte_size   : 4
  <7b4>     DW_AT_type        : <7b8>
 <1><7b8>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <7b9>     DW_AT_prototyped  : 1
  <7ba>     DW_AT_sibling     : <7c4>
 <2><7be>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <7bf>     DW_AT_type        : <7c4>
 <1><7c4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <7c5>     DW_AT_byte_size   : 4
  <7c6>     DW_AT_type        : <5bf>
 <1><7ca>: Abbrev Number: 17 (DW_TAG_typedef)
  <7cb>     DW_AT_name        : FAPI_DMA_OptCallbackT
  <7e1>     DW_AT_decl_file   : 5
  <7e2>     DW_AT_decl_line   : 305
  <7e4>     DW_AT_type        : <7e8>
 <1><7e8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <7e9>     DW_AT_byte_size   : 4
  <7ea>     DW_AT_type        : <7ee>
 <1><7ee>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <7ef>     DW_AT_prototyped  : 1
  <7f0>     DW_AT_sibling     : <7ff>
 <2><7f4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <7f5>     DW_AT_type        : <17f>
 <2><7f9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <7fa>     DW_AT_type        : <18e>
 <1><7ff>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <800>     DW_AT_byte_size   : 4
  <801>     DW_AT_type        : <805>
 <1><805>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <806>     DW_AT_prototyped  : 1
  <807>     DW_AT_sibling     : <811>
 <2><80b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <80c>     DW_AT_type        : <f1>
 <1><811>: Abbrev Number: 2 (DW_TAG_typedef)
  <812>     DW_AT_name        : MUSB_pfTimerExpired
  <826>     DW_AT_decl_file   : 6
  <827>     DW_AT_decl_line   : 88
  <828>     DW_AT_type        : <82c>
 <1><82c>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <82d>     DW_AT_byte_size   : 4
  <82e>     DW_AT_type        : <832>
 <1><832>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <833>     DW_AT_prototyped  : 1
  <834>     DW_AT_sibling     : <843>
 <2><838>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <839>     DW_AT_type        : <f1>
 <2><83d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <83e>     DW_AT_type        : <16f>
 <1><843>: Abbrev Number: 2 (DW_TAG_typedef)
  <844>     DW_AT_name        : MUSB_pfMessageString
  <859>     DW_AT_decl_file   : 6
  <85a>     DW_AT_decl_line   : 154
  <85b>     DW_AT_type        : <85f>
 <1><85f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <860>     DW_AT_byte_size   : 4
  <861>     DW_AT_type        : <865>
 <1><865>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <866>     DW_AT_prototyped  : 1
  <867>     DW_AT_type        : <160>
  <86b>     DW_AT_sibling     : <87f>
 <2><86f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <870>     DW_AT_type        : <108>
 <2><874>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <875>     DW_AT_type        : <16f>
 <2><879>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <87a>     DW_AT_type        : <116>
 <1><87f>: Abbrev Number: 2 (DW_TAG_typedef)
  <880>     DW_AT_name        : MUSB_pfMessageNumber
  <895>     DW_AT_decl_file   : 6
  <896>     DW_AT_decl_line   : 174
  <897>     DW_AT_type        : <89b>
 <1><89b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <89c>     DW_AT_byte_size   : 4
  <89d>     DW_AT_type        : <8a1>
 <1><8a1>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <8a2>     DW_AT_prototyped  : 1
  <8a3>     DW_AT_type        : <160>
  <8a7>     DW_AT_sibling     : <8c5>
 <2><8ab>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <8ac>     DW_AT_type        : <108>
 <2><8b0>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <8b1>     DW_AT_type        : <16f>
 <2><8b5>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <8b6>     DW_AT_type        : <18e>
 <2><8ba>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <8bb>     DW_AT_type        : <160>
 <2><8bf>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <8c0>     DW_AT_type        : <160>
 <1><8c5>: Abbrev Number: 2 (DW_TAG_typedef)
  <8c6>     DW_AT_name        : MUSB_pfGetTime
  <8d5>     DW_AT_decl_file   : 6
  <8d6>     DW_AT_decl_line   : 184
  <8d7>     DW_AT_type        : <329>
 <1><8db>: Abbrev Number: 11 (DW_TAG_structure_type)
  <8dc>     DW_AT_byte_size   : 16
  <8dd>     DW_AT_decl_file   : 6
  <8de>     DW_AT_decl_line   : 208
  <8df>     DW_AT_sibling     : <93a>
 <2><8e3>: Abbrev Number: 20 (DW_TAG_member)
  <8e4>     DW_AT_name        : (indirect string, offset: 0x134): wVersion
  <8e8>     DW_AT_decl_file   : 6
  <8e9>     DW_AT_decl_line   : 209
  <8ea>     DW_AT_type        : <16f>
  <8ee>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><8f1>: Abbrev Number: 12 (DW_TAG_member)
  <8f2>     DW_AT_name        : pfMessageString
  <902>     DW_AT_decl_file   : 6
  <903>     DW_AT_decl_line   : 210
  <904>     DW_AT_type        : <843>
  <908>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><90b>: Abbrev Number: 12 (DW_TAG_member)
  <90c>     DW_AT_name        : pfMessageNumber
  <91c>     DW_AT_decl_file   : 6
  <91d>     DW_AT_decl_line   : 211
  <91e>     DW_AT_type        : <87f>
  <922>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><925>: Abbrev Number: 12 (DW_TAG_member)
  <926>     DW_AT_name        : pfGetTime
  <930>     DW_AT_decl_file   : 6
  <931>     DW_AT_decl_line   : 212
  <932>     DW_AT_type        : <8c5>
  <936>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <1><93a>: Abbrev Number: 2 (DW_TAG_typedef)
  <93b>     DW_AT_name        : MUSB_SystemUtils
  <94c>     DW_AT_decl_file   : 6
  <94d>     DW_AT_decl_line   : 213
  <94e>     DW_AT_type        : <8db>
 <1><952>: Abbrev Number: 2 (DW_TAG_typedef)
  <953>     DW_AT_name        : MUSB_pfSystemToBusAddress
  <96d>     DW_AT_decl_file   : 6
  <96e>     DW_AT_decl_line   : 225
  <96f>     DW_AT_type        : <973>
 <1><973>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <974>     DW_AT_byte_size   : 4
  <975>     DW_AT_type        : <979>
 <1><979>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <97a>     DW_AT_prototyped  : 1
  <97b>     DW_AT_type        : <f1>
  <97f>     DW_AT_sibling     : <98e>
 <2><983>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <984>     DW_AT_type        : <f1>
 <2><988>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <989>     DW_AT_type        : <98e>
 <1><98e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <98f>     DW_AT_byte_size   : 4
  <990>     DW_AT_type        : <994>
 <1><994>: Abbrev Number: 21 (DW_TAG_const_type)
 <1><995>: Abbrev Number: 2 (DW_TAG_typedef)
  <996>     DW_AT_name        : MUSB_pfQueueBackgroundItem
  <9b1>     DW_AT_decl_file   : 6
  <9b2>     DW_AT_decl_line   : 238
  <9b3>     DW_AT_type        : <9b7>
 <1><9b7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <9b8>     DW_AT_byte_size   : 4
  <9b9>     DW_AT_type        : <9bd>
 <1><9bd>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <9be>     DW_AT_prototyped  : 1
  <9bf>     DW_AT_type        : <160>
  <9c3>     DW_AT_sibling     : <9d2>
 <2><9c7>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <9c8>     DW_AT_type        : <f1>
 <2><9cc>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <9cd>     DW_AT_type        : <98e>
 <1><9d2>: Abbrev Number: 2 (DW_TAG_typedef)
  <9d3>     DW_AT_name        : MUSB_pfDequeueBackgroundItem
  <9f0>     DW_AT_decl_file   : 6
  <9f1>     DW_AT_decl_line   : 251
  <9f2>     DW_AT_type        : <9f6>
 <1><9f6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <9f7>     DW_AT_byte_size   : 4
  <9f8>     DW_AT_type        : <9fc>
 <1><9fc>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <9fd>     DW_AT_prototyped  : 1
  <9fe>     DW_AT_type        : <160>
  <a02>     DW_AT_sibling     : <a11>
 <2><a06>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a07>     DW_AT_type        : <f1>
 <2><a0b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a0c>     DW_AT_type        : <f1>
 <1><a11>: Abbrev Number: 17 (DW_TAG_typedef)
  <a12>     DW_AT_name        : MUSB_pfFlushBackgroundQueue
  <a2e>     DW_AT_decl_file   : 6
  <a2f>     DW_AT_decl_line   : 259
  <a31>     DW_AT_type        : <7ff>
 <1><a35>: Abbrev Number: 17 (DW_TAG_typedef)
  <a36>     DW_AT_name        : MUSB_pfArmTimer
  <a46>     DW_AT_decl_file   : 6
  <a47>     DW_AT_decl_line   : 274
  <a49>     DW_AT_type        : <a4d>
 <1><a4d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <a4e>     DW_AT_byte_size   : 4
  <a4f>     DW_AT_type        : <a53>
 <1><a53>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <a54>     DW_AT_prototyped  : 1
  <a55>     DW_AT_type        : <160>
  <a59>     DW_AT_sibling     : <a77>
 <2><a5d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a5e>     DW_AT_type        : <f1>
 <2><a62>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a63>     DW_AT_type        : <16f>
 <2><a67>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a68>     DW_AT_type        : <18e>
 <2><a6c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a6d>     DW_AT_type        : <160>
 <2><a71>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <a72>     DW_AT_type        : <811>
 <1><a77>: Abbrev Number: 17 (DW_TAG_typedef)
  <a78>     DW_AT_name        : MUSB_pfCancelTimer
  <a8b>     DW_AT_decl_file   : 6
  <a8c>     DW_AT_decl_line   : 285
  <a8e>     DW_AT_type        : <a92>
 <1><a92>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <a93>     DW_AT_byte_size   : 4
  <a94>     DW_AT_type        : <a98>
 <1><a98>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <a99>     DW_AT_prototyped  : 1
  <a9a>     DW_AT_type        : <160>
  <a9e>     DW_AT_sibling     : <aad>
 <2><aa2>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <aa3>     DW_AT_type        : <f1>
 <2><aa7>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <aa8>     DW_AT_type        : <16f>
 <1><aad>: Abbrev Number: 17 (DW_TAG_typedef)
  <aae>     DW_AT_name        : MUSB_pfLock
  <aba>     DW_AT_decl_file   : 6
  <abb>     DW_AT_decl_line   : 296
  <abd>     DW_AT_type        : <a92>
 <1><ac1>: Abbrev Number: 17 (DW_TAG_typedef)
  <ac2>     DW_AT_name        : MUSB_pfUnlock
  <ad0>     DW_AT_decl_file   : 6
  <ad1>     DW_AT_decl_line   : 307
  <ad3>     DW_AT_type        : <a92>
 <1><ad7>: Abbrev Number: 17 (DW_TAG_typedef)
  <ad8>     DW_AT_name        : MUSB_pfPrintDiag
  <ae9>     DW_AT_decl_file   : 6
  <aea>     DW_AT_decl_line   : 318
  <aec>     DW_AT_type        : <af0>
 <1><af0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <af1>     DW_AT_byte_size   : 4
  <af2>     DW_AT_type        : <af6>
 <1><af6>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <af7>     DW_AT_prototyped  : 1
  <af8>     DW_AT_type        : <160>
  <afc>     DW_AT_sibling     : <b0b>
 <2><b00>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b01>     DW_AT_type        : <f1>
 <2><b05>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b06>     DW_AT_type        : <116>
 <1><b0b>: Abbrev Number: 17 (DW_TAG_typedef)
  <b0c>     DW_AT_name        : MUSB_pfNewPowerLoad
  <b20>     DW_AT_decl_file   : 6
  <b21>     DW_AT_decl_line   : 346
  <b23>     DW_AT_type        : <b27>
 <1><b27>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <b28>     DW_AT_byte_size   : 4
  <b29>     DW_AT_type        : <b2d>
 <1><b2d>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <b2e>     DW_AT_prototyped  : 1
  <b2f>     DW_AT_type        : <160>
  <b33>     DW_AT_sibling     : <b51>
 <2><b37>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b38>     DW_AT_type        : <f1>
 <2><b3c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b3d>     DW_AT_type        : <16f>
 <2><b41>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b42>     DW_AT_type        : <160>
 <2><b46>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b47>     DW_AT_type        : <b51>
 <2><b4b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <b4c>     DW_AT_type        : <160>
 <1><b51>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <b52>     DW_AT_byte_size   : 4
  <b53>     DW_AT_type        : <b57>
 <1><b57>: Abbrev Number: 8 (DW_TAG_const_type)
  <b58>     DW_AT_type        : <160>
 <1><b5c>: Abbrev Number: 17 (DW_TAG_typedef)
  <b5d>     DW_AT_name        : MUSB_pfRemovePowerLoad
  <b74>     DW_AT_decl_file   : 6
  <b75>     DW_AT_decl_line   : 366
  <b77>     DW_AT_type        : <b27>
 <1><b7b>: Abbrev Number: 22 (DW_TAG_structure_type)
  <b7c>     DW_AT_byte_size   : 52
  <b7d>     DW_AT_decl_file   : 6
  <b7e>     DW_AT_decl_line   : 403
  <b80>     DW_AT_sibling     : <ccc>
 <2><b84>: Abbrev Number: 23 (DW_TAG_member)
  <b85>     DW_AT_name        : (indirect string, offset: 0x134): wVersion
  <b89>     DW_AT_decl_file   : 6
  <b8a>     DW_AT_decl_line   : 404
  <b8c>     DW_AT_type        : <16f>
  <b90>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><b93>: Abbrev Number: 23 (DW_TAG_member)
  <b94>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <b98>     DW_AT_decl_file   : 6
  <b99>     DW_AT_decl_line   : 405
  <b9b>     DW_AT_type        : <f1>
  <b9f>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><ba2>: Abbrev Number: 13 (DW_TAG_member)
  <ba3>     DW_AT_name        : pfSystemToBusAddress
  <bb8>     DW_AT_decl_file   : 6
  <bb9>     DW_AT_decl_line   : 406
  <bbb>     DW_AT_type        : <952>
  <bbf>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><bc2>: Abbrev Number: 13 (DW_TAG_member)
  <bc3>     DW_AT_name        : pfQueueBackgroundItem
  <bd9>     DW_AT_decl_file   : 6
  <bda>     DW_AT_decl_line   : 407
  <bdc>     DW_AT_type        : <995>
  <be0>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><be3>: Abbrev Number: 13 (DW_TAG_member)
  <be4>     DW_AT_name        : pfDequeueBackgroundItem
  <bfc>     DW_AT_decl_file   : 6
  <bfd>     DW_AT_decl_line   : 408
  <bff>     DW_AT_type        : <9d2>
  <c03>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><c06>: Abbrev Number: 13 (DW_TAG_member)
  <c07>     DW_AT_name        : pfFlushBackgroundQueue
  <c1e>     DW_AT_decl_file   : 6
  <c1f>     DW_AT_decl_line   : 409
  <c21>     DW_AT_type        : <a11>
  <c25>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><c28>: Abbrev Number: 13 (DW_TAG_member)
  <c29>     DW_AT_name        : pfArmTimer
  <c34>     DW_AT_decl_file   : 6
  <c35>     DW_AT_decl_line   : 410
  <c37>     DW_AT_type        : <a35>
  <c3b>     DW_AT_data_member_location: 2 byte block: 23 18     (DW_OP_plus_uconst: 24)
 <2><c3e>: Abbrev Number: 13 (DW_TAG_member)
  <c3f>     DW_AT_name        : pfCancelTimer
  <c4d>     DW_AT_decl_file   : 6
  <c4e>     DW_AT_decl_line   : 411
  <c50>     DW_AT_type        : <a77>
  <c54>     DW_AT_data_member_location: 2 byte block: 23 1c     (DW_OP_plus_uconst: 28)
 <2><c57>: Abbrev Number: 13 (DW_TAG_member)
  <c58>     DW_AT_name        : pfLock
  <c5f>     DW_AT_decl_file   : 6
  <c60>     DW_AT_decl_line   : 412
  <c62>     DW_AT_type        : <aad>
  <c66>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <2><c69>: Abbrev Number: 13 (DW_TAG_member)
  <c6a>     DW_AT_name        : pfUnlock
  <c73>     DW_AT_decl_file   : 6
  <c74>     DW_AT_decl_line   : 413
  <c76>     DW_AT_type        : <ac1>
  <c7a>     DW_AT_data_member_location: 2 byte block: 23 24     (DW_OP_plus_uconst: 36)
 <2><c7d>: Abbrev Number: 13 (DW_TAG_member)
  <c7e>     DW_AT_name        : pfPrintDiag
  <c8a>     DW_AT_decl_file   : 6
  <c8b>     DW_AT_decl_line   : 414
  <c8d>     DW_AT_type        : <ad7>
  <c91>     DW_AT_data_member_location: 2 byte block: 23 28     (DW_OP_plus_uconst: 40)
 <2><c94>: Abbrev Number: 13 (DW_TAG_member)
  <c95>     DW_AT_name        : pfNewPowerLoad
  <ca4>     DW_AT_decl_file   : 6
  <ca5>     DW_AT_decl_line   : 415
  <ca7>     DW_AT_type        : <b0b>
  <cab>     DW_AT_data_member_location: 2 byte block: 23 2c     (DW_OP_plus_uconst: 44)
 <2><cae>: Abbrev Number: 13 (DW_TAG_member)
  <caf>     DW_AT_name        : pfRemovePowerLoad
  <cc1>     DW_AT_decl_file   : 6
  <cc2>     DW_AT_decl_line   : 416
  <cc4>     DW_AT_type        : <b5c>
  <cc8>     DW_AT_data_member_location: 2 byte block: 23 30     (DW_OP_plus_uconst: 48)
 <1><ccc>: Abbrev Number: 17 (DW_TAG_typedef)
  <ccd>     DW_AT_name        : MUSB_SystemServices
  <ce1>     DW_AT_decl_file   : 6
  <ce2>     DW_AT_decl_line   : 417
  <ce4>     DW_AT_type        : <b7b>
 <1><ce8>: Abbrev Number: 11 (DW_TAG_structure_type)
  <ce9>     DW_AT_byte_size   : 8
  <cea>     DW_AT_decl_file   : 7
  <ceb>     DW_AT_decl_line   : 173
  <cec>     DW_AT_sibling     : <d4d>
 <2><cf0>: Abbrev Number: 12 (DW_TAG_member)
  <cf1>     DW_AT_name        : bmRequestType
  <cff>     DW_AT_decl_file   : 7
  <d00>     DW_AT_decl_line   : 174
  <d01>     DW_AT_type        : <160>
  <d05>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><d08>: Abbrev Number: 12 (DW_TAG_member)
  <d09>     DW_AT_name        : bRequest
  <d12>     DW_AT_decl_file   : 7
  <d13>     DW_AT_decl_line   : 175
  <d14>     DW_AT_type        : <160>
  <d18>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><d1b>: Abbrev Number: 12 (DW_TAG_member)
  <d1c>     DW_AT_name        : wValue
  <d23>     DW_AT_decl_file   : 7
  <d24>     DW_AT_decl_line   : 176
  <d25>     DW_AT_type        : <16f>
  <d29>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><d2c>: Abbrev Number: 20 (DW_TAG_member)
  <d2d>     DW_AT_name        : (indirect string, offset: 0xba): wIndex
  <d31>     DW_AT_decl_file   : 7
  <d32>     DW_AT_decl_line   : 177
  <d33>     DW_AT_type        : <16f>
  <d37>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><d3a>: Abbrev Number: 12 (DW_TAG_member)
  <d3b>     DW_AT_name        : wLength
  <d43>     DW_AT_decl_file   : 7
  <d44>     DW_AT_decl_line   : 178
  <d45>     DW_AT_type        : <16f>
  <d49>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <1><d4d>: Abbrev Number: 2 (DW_TAG_typedef)
  <d4e>     DW_AT_name        : MUSB_DeviceRequest
  <d61>     DW_AT_decl_file   : 7
  <d62>     DW_AT_decl_line   : 179
  <d63>     DW_AT_type        : <ce8>
 <1><d67>: Abbrev Number: 11 (DW_TAG_structure_type)
  <d68>     DW_AT_byte_size   : 20
  <d69>     DW_AT_decl_file   : 7
  <d6a>     DW_AT_decl_line   : 194
  <d6b>     DW_AT_sibling     : <e9d>
 <2><d6f>: Abbrev Number: 20 (DW_TAG_member)
  <d70>     DW_AT_name        : (indirect string, offset: 0x12c): bLength
  <d74>     DW_AT_decl_file   : 7
  <d75>     DW_AT_decl_line   : 195
  <d76>     DW_AT_type        : <160>
  <d7a>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><d7d>: Abbrev Number: 20 (DW_TAG_member)
  <d7e>     DW_AT_name        : (indirect string, offset: 0x146): bDescriptorType
  <d82>     DW_AT_decl_file   : 7
  <d83>     DW_AT_decl_line   : 196
  <d84>     DW_AT_type        : <160>
  <d88>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><d8b>: Abbrev Number: 12 (DW_TAG_member)
  <d8c>     DW_AT_name        : bcdUSB
  <d93>     DW_AT_decl_file   : 7
  <d94>     DW_AT_decl_line   : 197
  <d95>     DW_AT_type        : <16f>
  <d99>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><d9c>: Abbrev Number: 12 (DW_TAG_member)
  <d9d>     DW_AT_name        : bDeviceClass
  <daa>     DW_AT_decl_file   : 7
  <dab>     DW_AT_decl_line   : 198
  <dac>     DW_AT_type        : <160>
  <db0>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><db3>: Abbrev Number: 12 (DW_TAG_member)
  <db4>     DW_AT_name        : bDeviceSubClass
  <dc4>     DW_AT_decl_file   : 7
  <dc5>     DW_AT_decl_line   : 199
  <dc6>     DW_AT_type        : <160>
  <dca>     DW_AT_data_member_location: 2 byte block: 23 5  (DW_OP_plus_uconst: 5)
 <2><dcd>: Abbrev Number: 12 (DW_TAG_member)
  <dce>     DW_AT_name        : bDeviceProtocol
  <dde>     DW_AT_decl_file   : 7
  <ddf>     DW_AT_decl_line   : 200
  <de0>     DW_AT_type        : <160>
  <de4>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <2><de7>: Abbrev Number: 12 (DW_TAG_member)
  <de8>     DW_AT_name        : bMaxPacketSize0
  <df8>     DW_AT_decl_file   : 7
  <df9>     DW_AT_decl_line   : 201
  <dfa>     DW_AT_type        : <160>
  <dfe>     DW_AT_data_member_location: 2 byte block: 23 7  (DW_OP_plus_uconst: 7)
 <2><e01>: Abbrev Number: 12 (DW_TAG_member)
  <e02>     DW_AT_name        : idVendor
  <e0b>     DW_AT_decl_file   : 7
  <e0c>     DW_AT_decl_line   : 202
  <e0d>     DW_AT_type        : <16f>
  <e11>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><e14>: Abbrev Number: 12 (DW_TAG_member)
  <e15>     DW_AT_name        : idProduct
  <e1f>     DW_AT_decl_file   : 7
  <e20>     DW_AT_decl_line   : 203
  <e21>     DW_AT_type        : <16f>
  <e25>     DW_AT_data_member_location: 2 byte block: 23 a  (DW_OP_plus_uconst: 10)
 <2><e28>: Abbrev Number: 12 (DW_TAG_member)
  <e29>     DW_AT_name        : bcdDevice
  <e33>     DW_AT_decl_file   : 7
  <e34>     DW_AT_decl_line   : 204
  <e35>     DW_AT_type        : <16f>
  <e39>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><e3c>: Abbrev Number: 12 (DW_TAG_member)
  <e3d>     DW_AT_name        : iManufacturer
  <e4b>     DW_AT_decl_file   : 7
  <e4c>     DW_AT_decl_line   : 205
  <e4d>     DW_AT_type        : <160>
  <e51>     DW_AT_data_member_location: 2 byte block: 23 e  (DW_OP_plus_uconst: 14)
 <2><e54>: Abbrev Number: 12 (DW_TAG_member)
  <e55>     DW_AT_name        : iProduct
  <e5e>     DW_AT_decl_file   : 7
  <e5f>     DW_AT_decl_line   : 206
  <e60>     DW_AT_type        : <160>
  <e64>     DW_AT_data_member_location: 2 byte block: 23 f  (DW_OP_plus_uconst: 15)
 <2><e67>: Abbrev Number: 12 (DW_TAG_member)
  <e68>     DW_AT_name        : iSerialNumber
  <e76>     DW_AT_decl_file   : 7
  <e77>     DW_AT_decl_line   : 207
  <e78>     DW_AT_type        : <160>
  <e7c>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><e7f>: Abbrev Number: 12 (DW_TAG_member)
  <e80>     DW_AT_name        : bNumConfigurations
  <e93>     DW_AT_decl_file   : 7
  <e94>     DW_AT_decl_line   : 208
  <e95>     DW_AT_type        : <160>
  <e99>     DW_AT_data_member_location: 2 byte block: 23 11     (DW_OP_plus_uconst: 17)
 <1><e9d>: Abbrev Number: 2 (DW_TAG_typedef)
  <e9e>     DW_AT_name        : MUSB_DeviceDescriptor
  <eb4>     DW_AT_decl_file   : 7
  <eb5>     DW_AT_decl_line   : 209
  <eb6>     DW_AT_type        : <d67>
 <1><eba>: Abbrev Number: 11 (DW_TAG_structure_type)
  <ebb>     DW_AT_byte_size   : 8
  <ebc>     DW_AT_decl_file   : 7
  <ebd>     DW_AT_decl_line   : 215
  <ebe>     DW_AT_sibling     : <f24>
 <2><ec2>: Abbrev Number: 20 (DW_TAG_member)
  <ec3>     DW_AT_name        : (indirect string, offset: 0x12c): bLength
  <ec7>     DW_AT_decl_file   : 7
  <ec8>     DW_AT_decl_line   : 216
  <ec9>     DW_AT_type        : <160>
  <ecd>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><ed0>: Abbrev Number: 20 (DW_TAG_member)
  <ed1>     DW_AT_name        : (indirect string, offset: 0x146): bDescriptorType
  <ed5>     DW_AT_decl_file   : 7
  <ed6>     DW_AT_decl_line   : 217
  <ed7>     DW_AT_type        : <160>
  <edb>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><ede>: Abbrev Number: 12 (DW_TAG_member)
  <edf>     DW_AT_name        : bEndpointAddress
  <ef0>     DW_AT_decl_file   : 7
  <ef1>     DW_AT_decl_line   : 218
  <ef2>     DW_AT_type        : <160>
  <ef6>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><ef9>: Abbrev Number: 20 (DW_TAG_member)
  <efa>     DW_AT_name        : (indirect string, offset: 0x1b1): bmAttributes
  <efe>     DW_AT_decl_file   : 7
  <eff>     DW_AT_decl_line   : 219
  <f00>     DW_AT_type        : <160>
  <f04>     DW_AT_data_member_location: 2 byte block: 23 3  (DW_OP_plus_uconst: 3)
 <2><f07>: Abbrev Number: 20 (DW_TAG_member)
  <f08>     DW_AT_name        : (indirect string, offset: 0x11d): wMaxPacketSize
  <f0c>     DW_AT_decl_file   : 7
  <f0d>     DW_AT_decl_line   : 220
  <f0e>     DW_AT_type        : <16f>
  <f12>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><f15>: Abbrev Number: 20 (DW_TAG_member)
  <f16>     DW_AT_name        : (indirect string, offset: 0x39): bInterval
  <f1a>     DW_AT_decl_file   : 7
  <f1b>     DW_AT_decl_line   : 221
  <f1c>     DW_AT_type        : <160>
  <f20>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <1><f24>: Abbrev Number: 2 (DW_TAG_typedef)
  <f25>     DW_AT_name        : MUSB_EndpointDescriptor
  <f3d>     DW_AT_decl_file   : 7
  <f3e>     DW_AT_decl_line   : 222
  <f3f>     DW_AT_type        : <eba>
 <1><f43>: Abbrev Number: 11 (DW_TAG_structure_type)
  <f44>     DW_AT_byte_size   : 12
  <f45>     DW_AT_decl_file   : 7
  <f46>     DW_AT_decl_line   : 244
  <f47>     DW_AT_sibling     : <ff1>
 <2><f4b>: Abbrev Number: 20 (DW_TAG_member)
  <f4c>     DW_AT_name        : (indirect string, offset: 0x12c): bLength
  <f50>     DW_AT_decl_file   : 7
  <f51>     DW_AT_decl_line   : 245
  <f52>     DW_AT_type        : <160>
  <f56>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><f59>: Abbrev Number: 20 (DW_TAG_member)
  <f5a>     DW_AT_name        : (indirect string, offset: 0x146): bDescriptorType
  <f5e>     DW_AT_decl_file   : 7
  <f5f>     DW_AT_decl_line   : 246
  <f60>     DW_AT_type        : <160>
  <f64>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><f67>: Abbrev Number: 12 (DW_TAG_member)
  <f68>     DW_AT_name        : wTotalLength
  <f75>     DW_AT_decl_file   : 7
  <f76>     DW_AT_decl_line   : 247
  <f77>     DW_AT_type        : <16f>
  <f7b>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><f7e>: Abbrev Number: 12 (DW_TAG_member)
  <f7f>     DW_AT_name        : bNumInterfaces
  <f8e>     DW_AT_decl_file   : 7
  <f8f>     DW_AT_decl_line   : 248
  <f90>     DW_AT_type        : <160>
  <f94>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><f97>: Abbrev Number: 12 (DW_TAG_member)
  <f98>     DW_AT_name        : bConfigurationValue
  <fac>     DW_AT_decl_file   : 7
  <fad>     DW_AT_decl_line   : 249
  <fae>     DW_AT_type        : <160>
  <fb2>     DW_AT_data_member_location: 2 byte block: 23 5  (DW_OP_plus_uconst: 5)
 <2><fb5>: Abbrev Number: 12 (DW_TAG_member)
  <fb6>     DW_AT_name        : iConfiguration
  <fc5>     DW_AT_decl_file   : 7
  <fc6>     DW_AT_decl_line   : 250
  <fc7>     DW_AT_type        : <160>
  <fcb>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <2><fce>: Abbrev Number: 20 (DW_TAG_member)
  <fcf>     DW_AT_name        : (indirect string, offset: 0x1b1): bmAttributes
  <fd3>     DW_AT_decl_file   : 7
  <fd4>     DW_AT_decl_line   : 251
  <fd5>     DW_AT_type        : <160>
  <fd9>     DW_AT_data_member_location: 2 byte block: 23 7  (DW_OP_plus_uconst: 7)
 <2><fdc>: Abbrev Number: 12 (DW_TAG_member)
  <fdd>     DW_AT_name        : bMaxPower
  <fe7>     DW_AT_decl_file   : 7
  <fe8>     DW_AT_decl_line   : 252
  <fe9>     DW_AT_type        : <160>
  <fed>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <1><ff1>: Abbrev Number: 2 (DW_TAG_typedef)
  <ff2>     DW_AT_name        : MUSB_ConfigurationDescriptor
  <100f>     DW_AT_decl_file   : 7
  <1010>     DW_AT_decl_line   : 253
  <1011>     DW_AT_type        : <f43>
 <1><1015>: Abbrev Number: 22 (DW_TAG_structure_type)
  <1016>     DW_AT_byte_size   : 4
  <1017>     DW_AT_decl_file   : 7
  <1018>     DW_AT_decl_line   : 269
  <101a>     DW_AT_sibling     : <104c>
 <2><101e>: Abbrev Number: 23 (DW_TAG_member)
  <101f>     DW_AT_name        : (indirect string, offset: 0x12c): bLength
  <1023>     DW_AT_decl_file   : 7
  <1024>     DW_AT_decl_line   : 270
  <1026>     DW_AT_type        : <160>
  <102a>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><102d>: Abbrev Number: 23 (DW_TAG_member)
  <102e>     DW_AT_name        : (indirect string, offset: 0x146): bDescriptorType
  <1032>     DW_AT_decl_file   : 7
  <1033>     DW_AT_decl_line   : 271
  <1035>     DW_AT_type        : <160>
  <1039>     DW_AT_data_member_location: 2 byte block: 23 1     (DW_OP_plus_uconst: 1)
 <2><103c>: Abbrev Number: 23 (DW_TAG_member)
  <103d>     DW_AT_name        : (indirect string, offset: 0x1b1): bmAttributes
  <1041>     DW_AT_decl_file   : 7
  <1042>     DW_AT_decl_line   : 272
  <1044>     DW_AT_type        : <160>
  <1048>     DW_AT_data_member_location: 2 byte block: 23 2     (DW_OP_plus_uconst: 2)
 <1><104c>: Abbrev Number: 17 (DW_TAG_typedef)
  <104d>     DW_AT_name        : MUSB_OtgDescriptor
  <1060>     DW_AT_decl_file   : 7
  <1061>     DW_AT_decl_line   : 273
  <1063>     DW_AT_type        : <1015>
 <1><1067>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <1068>     DW_AT_byte_size   : 4
  <1069>     DW_AT_decl_file   : 8
  <106a>     DW_AT_decl_line   : 36
  <106b>     DW_AT_sibling     : <10c9>
 <2><106f>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1070>     DW_AT_name        : MUSB_PORT_UNKNOWN
  <1082>     DW_AT_const_value : 0
 <2><1083>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1084>     DW_AT_name        : MUSB_PORT_TYPE_FUNCTION
  <109c>     DW_AT_const_value : 1
 <2><109d>: Abbrev Number: 10 (DW_TAG_enumerator)
  <109e>     DW_AT_name        : MUSB_PORT_TYPE_HOST
  <10b2>     DW_AT_const_value : 2
 <2><10b3>: Abbrev Number: 10 (DW_TAG_enumerator)
  <10b4>     DW_AT_name        : MUSB_PORT_TYPE_OTG
  <10c7>     DW_AT_const_value : 3
 <1><10c9>: Abbrev Number: 2 (DW_TAG_typedef)
  <10ca>     DW_AT_name        : MUSB_PortType
  <10d8>     DW_AT_decl_file   : 8
  <10d9>     DW_AT_decl_line   : 45
  <10da>     DW_AT_type        : <1067>
 <1><10de>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <10df>     DW_AT_byte_size   : 4
  <10e0>     DW_AT_decl_file   : 8
  <10e1>     DW_AT_decl_line   : 52
  <10e2>     DW_AT_sibling     : <1145>
 <2><10e6>: Abbrev Number: 10 (DW_TAG_enumerator)
  <10e7>     DW_AT_name        : MUSB_PORT_SPEED_UNKNOWN
  <10ff>     DW_AT_const_value : 0
 <2><1100>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1101>     DW_AT_name        : MUSB_PORT_SPEED_LOW
  <1115>     DW_AT_const_value : 1
 <2><1116>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1117>     DW_AT_name        : MUSB_PORT_SPEED_FULL
  <112c>     DW_AT_const_value : 2
 <2><112d>: Abbrev Number: 10 (DW_TAG_enumerator)
  <112e>     DW_AT_name        : MUSB_PORT_SPEED_HIGH
  <1143>     DW_AT_const_value : 3
 <1><1145>: Abbrev Number: 2 (DW_TAG_typedef)
  <1146>     DW_AT_name        : MUSB_PortSpeed
  <1155>     DW_AT_decl_file   : 8
  <1156>     DW_AT_decl_line   : 61
  <1157>     DW_AT_type        : <10de>
 <1><115b>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <115c>     DW_AT_byte_size   : 4
  <115d>     DW_AT_decl_file   : 8
  <115e>     DW_AT_decl_line   : 68
  <115f>     DW_AT_sibling     : <11c5>
 <2><1163>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1164>     DW_AT_name        : MUSB_POWER_OFF
  <1173>     DW_AT_const_value : 0
 <2><1174>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1175>     DW_AT_name        : MUSB_POWERED
  <1182>     DW_AT_const_value : 1
 <2><1183>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1184>     DW_AT_name        : MUSB_DEFAULT
  <1191>     DW_AT_const_value : 2
 <2><1192>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1193>     DW_AT_name        : MUSB_ADDRESS
  <11a0>     DW_AT_const_value : 3
 <2><11a1>: Abbrev Number: 10 (DW_TAG_enumerator)
  <11a2>     DW_AT_name        : MUSB_CONFIGURED
  <11b2>     DW_AT_const_value : 4
 <2><11b3>: Abbrev Number: 10 (DW_TAG_enumerator)
  <11b4>     DW_AT_name        : MUSB_SUSPENDED
  <11c3>     DW_AT_const_value : 5
 <1><11c5>: Abbrev Number: 2 (DW_TAG_typedef)
  <11c6>     DW_AT_name        : MUSB_State
  <11d1>     DW_AT_decl_file   : 8
  <11d2>     DW_AT_decl_line   : 81
  <11d3>     DW_AT_type        : <115b>
 <1><11d7>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <11d8>     DW_AT_byte_size   : 4
  <11d9>     DW_AT_decl_file   : 8
  <11da>     DW_AT_decl_line   : 88
  <11db>     DW_AT_sibling     : <12a2>
 <2><11df>: Abbrev Number: 10 (DW_TAG_enumerator)
  <11e0>     DW_AT_name        : MUSB_AB_IDLE
  <11ed>     DW_AT_const_value : 0
 <2><11ee>: Abbrev Number: 10 (DW_TAG_enumerator)
  <11ef>     DW_AT_name        : MUSB_WAIT_VRISE
  <11ff>     DW_AT_const_value : 1
 <2><1200>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1201>     DW_AT_name        : MUSB_B_PERIPHERAL
  <1213>     DW_AT_const_value : 17
 <2><1214>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1215>     DW_AT_name        : MUSB_B_WAIT_ACON
  <1226>     DW_AT_const_value : 18
 <2><1227>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1228>     DW_AT_name        : MUSB_B_HOST
  <1234>     DW_AT_const_value : 19
 <2><1235>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1236>     DW_AT_name        : MUSB_B_SRP_INIT
  <1246>     DW_AT_const_value : 20
 <2><1247>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1248>     DW_AT_name        : MUSB_A_PERIPHERAL
  <125a>     DW_AT_const_value : 33
 <2><125b>: Abbrev Number: 10 (DW_TAG_enumerator)
  <125c>     DW_AT_name        : MUSB_A_WAIT_BCON
  <126d>     DW_AT_const_value : 34
 <2><126e>: Abbrev Number: 10 (DW_TAG_enumerator)
  <126f>     DW_AT_name        : MUSB_A_HOST
  <127b>     DW_AT_const_value : 35
 <2><127c>: Abbrev Number: 10 (DW_TAG_enumerator)
  <127d>     DW_AT_name        : MUSB_A_SUSPEND
  <128c>     DW_AT_const_value : 36
 <2><128d>: Abbrev Number: 10 (DW_TAG_enumerator)
  <128e>     DW_AT_name        : MUSB_S_TRANSITION
  <12a0>     DW_AT_const_value : 48
 <1><12a2>: Abbrev Number: 2 (DW_TAG_typedef)
  <12a3>     DW_AT_name        : MUSB_OtgState
  <12b1>     DW_AT_decl_file   : 8
  <12b2>     DW_AT_decl_line   : 111
  <12b3>     DW_AT_type        : <11d7>
 <1><12b7>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <12b8>     DW_AT_byte_size   : 4
  <12b9>     DW_AT_decl_file   : 8
  <12ba>     DW_AT_decl_line   : 118
  <12bb>     DW_AT_sibling     : <1336>
 <2><12bf>: Abbrev Number: 10 (DW_TAG_enumerator)
  <12c0>     DW_AT_name        : MUSB_CONNECTION_SPEED_UNKNOWN
  <12de>     DW_AT_const_value : 0
 <2><12df>: Abbrev Number: 10 (DW_TAG_enumerator)
  <12e0>     DW_AT_name        : MUSB_CONNECTION_SPEED_LOW
  <12fa>     DW_AT_const_value : 1
 <2><12fb>: Abbrev Number: 10 (DW_TAG_enumerator)
  <12fc>     DW_AT_name        : MUSB_CONNECTION_SPEED_FULL
  <1317>     DW_AT_const_value : 2
 <2><1318>: Abbrev Number: 10 (DW_TAG_enumerator)
  <1319>     DW_AT_name        : MUSB_CONNECTION_SPEED_HIGH
  <1334>     DW_AT_const_value : 3
 <1><1336>: Abbrev Number: 2 (DW_TAG_typedef)
  <1337>     DW_AT_name        : MUSB_ConnectionSpeed
  <134c>     DW_AT_decl_file   : 8
  <134d>     DW_AT_decl_line   : 127
  <134e>     DW_AT_type        : <12b7>
 <1><1352>: Abbrev Number: 11 (DW_TAG_structure_type)
  <1353>     DW_AT_byte_size   : 16
  <1354>     DW_AT_decl_file   : 8
  <1355>     DW_AT_decl_line   : 228
  <1356>     DW_AT_sibling     : <139c>
 <2><135a>: Abbrev Number: 20 (DW_TAG_member)
  <135b>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <135f>     DW_AT_decl_file   : 8
  <1360>     DW_AT_decl_line   : 229
  <1361>     DW_AT_type        : <f1>
  <1365>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1368>: Abbrev Number: 12 (DW_TAG_member)
  <1369>     DW_AT_name        : bcdCdiRev
  <1373>     DW_AT_decl_file   : 8
  <1374>     DW_AT_decl_line   : 230
  <1375>     DW_AT_type        : <16f>
  <1379>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><137c>: Abbrev Number: 12 (DW_TAG_member)
  <137d>     DW_AT_name        : Type
  <1382>     DW_AT_decl_file   : 8
  <1383>     DW_AT_decl_line   : 231
  <1384>     DW_AT_type        : <10c9>
  <1388>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><138b>: Abbrev Number: 12 (DW_TAG_member)
  <138c>     DW_AT_name        : Speed
  <1392>     DW_AT_decl_file   : 8
  <1393>     DW_AT_decl_line   : 232
  <1394>     DW_AT_type        : <1145>
  <1398>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><139c>: Abbrev Number: 2 (DW_TAG_typedef)
  <139d>     DW_AT_name        : MUSB_Port
  <13a7>     DW_AT_decl_file   : 8
  <13a8>     DW_AT_decl_line   : 233
  <13a9>     DW_AT_type        : <1352>
 <1><13ad>: Abbrev Number: 24 (DW_TAG_structure_type)
  <13ae>     DW_AT_name        : _MUSB_Device
  <13bb>     DW_AT_byte_size   : 64
  <13bc>     DW_AT_decl_file   : 8
  <13bd>     DW_AT_decl_line   : 235
  <13be>     DW_AT_sibling     : <14d1>
 <2><13c2>: Abbrev Number: 23 (DW_TAG_member)
  <13c3>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <13c7>     DW_AT_decl_file   : 8
  <13c8>     DW_AT_decl_line   : 270
  <13ca>     DW_AT_type        : <14d1>
  <13ce>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><13d1>: Abbrev Number: 23 (DW_TAG_member)
  <13d2>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <13d6>     DW_AT_decl_file   : 8
  <13d7>     DW_AT_decl_line   : 271
  <13d9>     DW_AT_type        : <f1>
  <13dd>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><13e0>: Abbrev Number: 13 (DW_TAG_member)
  <13e1>     DW_AT_name        : pDriverPrivateData
  <13f4>     DW_AT_decl_file   : 8
  <13f5>     DW_AT_decl_line   : 272
  <13f7>     DW_AT_type        : <f1>
  <13fb>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><13fe>: Abbrev Number: 13 (DW_TAG_member)
  <13ff>     DW_AT_name        : DeviceDescriptor
  <1410>     DW_AT_decl_file   : 8
  <1411>     DW_AT_decl_line   : 273
  <1413>     DW_AT_type        : <e9d>
  <1417>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><141a>: Abbrev Number: 13 (DW_TAG_member)
  <141b>     DW_AT_name        : pCurrentConfiguration
  <1431>     DW_AT_decl_file   : 8
  <1432>     DW_AT_decl_line   : 274
  <1434>     DW_AT_type        : <14d7>
  <1438>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><143b>: Abbrev Number: 23 (DW_TAG_member)
  <143c>     DW_AT_name        : (indirect string, offset: 0x9b): apConfigDescriptors
  <1440>     DW_AT_decl_file   : 8
  <1441>     DW_AT_decl_line   : 275
  <1443>     DW_AT_type        : <14dd>
  <1447>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><144a>: Abbrev Number: 13 (DW_TAG_member)
  <144b>     DW_AT_name        : pDescriptorBuffer
  <145d>     DW_AT_decl_file   : 8
  <145e>     DW_AT_decl_line   : 276
  <1460>     DW_AT_type        : <14e3>
  <1464>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><1467>: Abbrev Number: 23 (DW_TAG_member)
  <1468>     DW_AT_name        : (indirect string, offset: 0x191): wDescriptorBufferLength
  <146c>     DW_AT_decl_file   : 8
  <146d>     DW_AT_decl_line   : 277
  <146f>     DW_AT_type        : <16f>
  <1473>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><1476>: Abbrev Number: 13 (DW_TAG_member)
  <1477>     DW_AT_name        : ConnectionSpeed
  <1487>     DW_AT_decl_file   : 8
  <1488>     DW_AT_decl_line   : 278
  <148a>     DW_AT_type        : <1336>
  <148e>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><1491>: Abbrev Number: 23 (DW_TAG_member)
  <1492>     DW_AT_name        : (indirect string, offset: 0x24): bBusAddress
  <1496>     DW_AT_decl_file   : 8
  <1497>     DW_AT_decl_line   : 279
  <1499>     DW_AT_type        : <160>
  <149d>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><14a0>: Abbrev Number: 13 (DW_TAG_member)
  <14a1>     DW_AT_name        : pParentUsbDevice
  <14b2>     DW_AT_decl_file   : 8
  <14b3>     DW_AT_decl_line   : 280
  <14b5>     DW_AT_type        : <14e9>
  <14b9>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <2><14bc>: Abbrev Number: 13 (DW_TAG_member)
  <14bd>     DW_AT_name        : bHubPort
  <14c6>     DW_AT_decl_file   : 8
  <14c7>     DW_AT_decl_line   : 281
  <14c9>     DW_AT_type        : <160>
  <14cd>     DW_AT_data_member_location: 2 byte block: 23 3c    (DW_OP_plus_uconst: 60)
 <1><14d1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14d2>     DW_AT_byte_size   : 4
  <14d3>     DW_AT_type        : <139c>
 <1><14d7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14d8>     DW_AT_byte_size   : 4
  <14d9>     DW_AT_type        : <ff1>
 <1><14dd>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14de>     DW_AT_byte_size   : 4
  <14df>     DW_AT_type        : <14d7>
 <1><14e3>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14e4>     DW_AT_byte_size   : 4
  <14e5>     DW_AT_type        : <160>
 <1><14e9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14ea>     DW_AT_byte_size   : 4
  <14eb>     DW_AT_type        : <13ad>
 <1><14ef>: Abbrev Number: 17 (DW_TAG_typedef)
  <14f0>     DW_AT_name        : MUSB_Device
  <14fc>     DW_AT_decl_file   : 8
  <14fd>     DW_AT_decl_line   : 282
  <14ff>     DW_AT_type        : <13ad>
 <1><1503>: Abbrev Number: 17 (DW_TAG_typedef)
  <1504>     DW_AT_name        : MUSB_BusHandle
  <1513>     DW_AT_decl_file   : 8
  <1514>     DW_AT_decl_line   : 285
  <1516>     DW_AT_type        : <f1>
 <1><151a>: Abbrev Number: 22 (DW_TAG_structure_type)
  <151b>     DW_AT_byte_size   : 16
  <151c>     DW_AT_decl_file   : 8
  <151d>     DW_AT_decl_line   : 301
  <151f>     DW_AT_sibling     : <1561>
 <2><1523>: Abbrev Number: 23 (DW_TAG_member)
  <1524>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <1528>     DW_AT_decl_file   : 8
  <1529>     DW_AT_decl_line   : 302
  <152b>     DW_AT_type        : <1561>
  <152f>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1532>: Abbrev Number: 13 (DW_TAG_member)
  <1533>     DW_AT_name        : UsbDescriptor
  <1541>     DW_AT_decl_file   : 8
  <1542>     DW_AT_decl_line   : 303
  <1544>     DW_AT_type        : <f24>
  <1548>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><154b>: Abbrev Number: 13 (DW_TAG_member)
  <154c>     DW_AT_name        : wNakLimit
  <1556>     DW_AT_decl_file   : 8
  <1557>     DW_AT_decl_line   : 304
  <1559>     DW_AT_type        : <16f>
  <155d>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><1561>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1562>     DW_AT_byte_size   : 4
  <1563>     DW_AT_type        : <1567>
 <1><1567>: Abbrev Number: 8 (DW_TAG_const_type)
  <1568>     DW_AT_type        : <14ef>
 <1><156c>: Abbrev Number: 17 (DW_TAG_typedef)
  <156d>     DW_AT_name        : MUSB_DeviceEndpoint
  <1581>     DW_AT_decl_file   : 8
  <1582>     DW_AT_decl_line   : 305
  <1584>     DW_AT_type        : <151a>
 <1><1588>: Abbrev Number: 22 (DW_TAG_structure_type)
  <1589>     DW_AT_byte_size   : 12
  <158a>     DW_AT_decl_file   : 8
  <158b>     DW_AT_decl_line   : 321
  <158d>     DW_AT_sibling     : <15cc>
 <2><1591>: Abbrev Number: 13 (DW_TAG_member)
  <1592>     DW_AT_name        : dwBufferSize
  <159f>     DW_AT_decl_file   : 8
  <15a0>     DW_AT_decl_line   : 322
  <15a2>     DW_AT_type        : <18e>
  <15a6>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><15a9>: Abbrev Number: 13 (DW_TAG_member)
  <15aa>     DW_AT_name        : bmFlags
  <15b2>     DW_AT_decl_file   : 8
  <15b3>     DW_AT_decl_line   : 323
  <15b5>     DW_AT_type        : <18e>
  <15b9>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><15bc>: Abbrev Number: 23 (DW_TAG_member)
  <15bd>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <15c1>     DW_AT_decl_file   : 8
  <15c2>     DW_AT_decl_line   : 324
  <15c4>     DW_AT_type        : <f1>
  <15c8>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><15cc>: Abbrev Number: 17 (DW_TAG_typedef)
  <15cd>     DW_AT_name        : MUSB_EndpointResource
  <15e3>     DW_AT_decl_file   : 8
  <15e4>     DW_AT_decl_line   : 325
  <15e6>     DW_AT_type        : <1588>
 <1><15ea>: Abbrev Number: 17 (DW_TAG_typedef)
  <15eb>     DW_AT_name        : MUSB_Pipe
  <15f5>     DW_AT_decl_file   : 8
  <15f6>     DW_AT_decl_line   : 338
  <15f8>     DW_AT_type        : <f1>
 <1><15fc>: Abbrev Number: 17 (DW_TAG_typedef)
  <15fd>     DW_AT_name        : MUSB_pfControlIrpComplete
  <1617>     DW_AT_decl_file   : 8
  <1618>     DW_AT_decl_line   : 348
  <161a>     DW_AT_type        : <161e>
 <1><161e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <161f>     DW_AT_byte_size   : 4
  <1620>     DW_AT_type        : <1624>
 <1><1624>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1625>     DW_AT_prototyped  : 1
  <1626>     DW_AT_sibling     : <1635>
 <2><162a>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <162b>     DW_AT_type        : <f1>
 <2><162f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1630>     DW_AT_type        : <1635>
 <1><1635>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1636>     DW_AT_byte_size   : 4
  <1637>     DW_AT_type        : <163b>
 <1><163b>: Abbrev Number: 18 (DW_TAG_structure_type)
  <163c>     DW_AT_name        : _MUSB_ControlIrp
  <164d>     DW_AT_byte_size   : 44
  <164e>     DW_AT_decl_file   : 8
  <164f>     DW_AT_decl_line   : 340
  <1651>     DW_AT_sibling     : <1732>
 <2><1655>: Abbrev Number: 23 (DW_TAG_member)
  <1656>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <165a>     DW_AT_decl_file   : 8
  <165b>     DW_AT_decl_line   : 379
  <165d>     DW_AT_type        : <1561>
  <1661>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1664>: Abbrev Number: 13 (DW_TAG_member)
  <1665>     DW_AT_name        : pOutBuffer
  <1670>     DW_AT_decl_file   : 8
  <1671>     DW_AT_decl_line   : 380
  <1673>     DW_AT_type        : <b51>
  <1677>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><167a>: Abbrev Number: 13 (DW_TAG_member)
  <167b>     DW_AT_name        : dwOutLength
  <1687>     DW_AT_decl_file   : 8
  <1688>     DW_AT_decl_line   : 381
  <168a>     DW_AT_type        : <18e>
  <168e>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1691>: Abbrev Number: 13 (DW_TAG_member)
  <1692>     DW_AT_name        : pInBuffer
  <169c>     DW_AT_decl_file   : 8
  <169d>     DW_AT_decl_line   : 382
  <169f>     DW_AT_type        : <14e3>
  <16a3>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><16a6>: Abbrev Number: 13 (DW_TAG_member)
  <16a7>     DW_AT_name        : dwInLength
  <16b2>     DW_AT_decl_file   : 8
  <16b3>     DW_AT_decl_line   : 383
  <16b5>     DW_AT_type        : <18e>
  <16b9>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><16bc>: Abbrev Number: 23 (DW_TAG_member)
  <16bd>     DW_AT_name        : (indirect string, offset: 0x30): dwStatus
  <16c1>     DW_AT_decl_file   : 8
  <16c2>     DW_AT_decl_line   : 384
  <16c4>     DW_AT_type        : <18e>
  <16c8>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><16cb>: Abbrev Number: 13 (DW_TAG_member)
  <16cc>     DW_AT_name        : dwActualOutLength
  <16de>     DW_AT_decl_file   : 8
  <16df>     DW_AT_decl_line   : 385
  <16e1>     DW_AT_type        : <18e>
  <16e5>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><16e8>: Abbrev Number: 13 (DW_TAG_member)
  <16e9>     DW_AT_name        : dwActualInLength
  <16fa>     DW_AT_decl_file   : 8
  <16fb>     DW_AT_decl_line   : 386
  <16fd>     DW_AT_type        : <18e>
  <1701>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1704>: Abbrev Number: 23 (DW_TAG_member)
  <1705>     DW_AT_name        : (indirect string, offset: 0x1c6): pfIrpComplete
  <1709>     DW_AT_decl_file   : 8
  <170a>     DW_AT_decl_line   : 387
  <170c>     DW_AT_type        : <15fc>
  <1710>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><1713>: Abbrev Number: 23 (DW_TAG_member)
  <1714>     DW_AT_name        : (indirect string, offset: 0xc): pCompleteParam
  <1718>     DW_AT_decl_file   : 8
  <1719>     DW_AT_decl_line   : 388
  <171b>     DW_AT_type        : <f1>
  <171f>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><1722>: Abbrev Number: 23 (DW_TAG_member)
  <1723>     DW_AT_name        : (indirect string, offset: 0xf0): bAllowDma
  <1727>     DW_AT_decl_file   : 8
  <1728>     DW_AT_decl_line   : 389
  <172a>     DW_AT_type        : <160>
  <172e>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <1><1732>: Abbrev Number: 17 (DW_TAG_typedef)
  <1733>     DW_AT_name        : MUSB_ControlIrp
  <1743>     DW_AT_decl_file   : 8
  <1744>     DW_AT_decl_line   : 390
  <1746>     DW_AT_type        : <163b>
 <1><174a>: Abbrev Number: 17 (DW_TAG_typedef)
  <174b>     DW_AT_name        : MUSB_pfIrpComplete
  <175e>     DW_AT_decl_file   : 8
  <175f>     DW_AT_decl_line   : 400
  <1761>     DW_AT_type        : <1765>
 <1><1765>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1766>     DW_AT_byte_size   : 4
  <1767>     DW_AT_type        : <176b>
 <1><176b>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <176c>     DW_AT_prototyped  : 1
  <176d>     DW_AT_sibling     : <177c>
 <2><1771>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1772>     DW_AT_type        : <f1>
 <2><1776>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1777>     DW_AT_type        : <177c>
 <1><177c>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <177d>     DW_AT_byte_size   : 4
  <177e>     DW_AT_type        : <1782>
 <1><1782>: Abbrev Number: 18 (DW_TAG_structure_type)
  <1783>     DW_AT_name        : _MUSB_Irp
  <178d>     DW_AT_byte_size   : 32
  <178e>     DW_AT_decl_file   : 8
  <178f>     DW_AT_decl_line   : 392
  <1791>     DW_AT_sibling     : <1847>
 <2><1795>: Abbrev Number: 13 (DW_TAG_member)
  <1796>     DW_AT_name        : hPipe
  <179c>     DW_AT_decl_file   : 8
  <179d>     DW_AT_decl_line   : 431
  <179f>     DW_AT_type        : <15ea>
  <17a3>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><17a6>: Abbrev Number: 13 (DW_TAG_member)
  <17a7>     DW_AT_name        : pBuffer
  <17af>     DW_AT_decl_file   : 8
  <17b0>     DW_AT_decl_line   : 432
  <17b2>     DW_AT_type        : <14e3>
  <17b6>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><17b9>: Abbrev Number: 13 (DW_TAG_member)
  <17ba>     DW_AT_name        : dwLength
  <17c3>     DW_AT_decl_file   : 8
  <17c4>     DW_AT_decl_line   : 433
  <17c6>     DW_AT_type        : <18e>
  <17ca>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><17cd>: Abbrev Number: 23 (DW_TAG_member)
  <17ce>     DW_AT_name        : (indirect string, offset: 0x30): dwStatus
  <17d2>     DW_AT_decl_file   : 8
  <17d3>     DW_AT_decl_line   : 434
  <17d5>     DW_AT_type        : <18e>
  <17d9>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><17dc>: Abbrev Number: 23 (DW_TAG_member)
  <17dd>     DW_AT_name        : (indirect string, offset: 0x156): dwActualLength
  <17e1>     DW_AT_decl_file   : 8
  <17e2>     DW_AT_decl_line   : 435
  <17e4>     DW_AT_type        : <18e>
  <17e8>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><17eb>: Abbrev Number: 23 (DW_TAG_member)
  <17ec>     DW_AT_name        : (indirect string, offset: 0x1c6): pfIrpComplete
  <17f0>     DW_AT_decl_file   : 8
  <17f1>     DW_AT_decl_line   : 436
  <17f3>     DW_AT_type        : <174a>
  <17f7>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><17fa>: Abbrev Number: 23 (DW_TAG_member)
  <17fb>     DW_AT_name        : (indirect string, offset: 0xc): pCompleteParam
  <17ff>     DW_AT_decl_file   : 8
  <1800>     DW_AT_decl_line   : 437
  <1802>     DW_AT_type        : <f1>
  <1806>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><1809>: Abbrev Number: 13 (DW_TAG_member)
  <180a>     DW_AT_name        : bAllowShortTransfer
  <181e>     DW_AT_decl_file   : 8
  <181f>     DW_AT_decl_line   : 438
  <1821>     DW_AT_type        : <160>
  <1825>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1828>: Abbrev Number: 23 (DW_TAG_member)
  <1829>     DW_AT_name        : (indirect string, offset: 0x43): bIsrCallback
  <182d>     DW_AT_decl_file   : 8
  <182e>     DW_AT_decl_line   : 439
  <1830>     DW_AT_type        : <160>
  <1834>     DW_AT_data_member_location: 2 byte block: 23 1d    (DW_OP_plus_uconst: 29)
 <2><1837>: Abbrev Number: 23 (DW_TAG_member)
  <1838>     DW_AT_name        : (indirect string, offset: 0xf0): bAllowDma
  <183c>     DW_AT_decl_file   : 8
  <183d>     DW_AT_decl_line   : 440
  <183f>     DW_AT_type        : <160>
  <1843>     DW_AT_data_member_location: 2 byte block: 23 1e    (DW_OP_plus_uconst: 30)
 <1><1847>: Abbrev Number: 17 (DW_TAG_typedef)
  <1848>     DW_AT_name        : MUSB_Irp
  <1851>     DW_AT_decl_file   : 8
  <1852>     DW_AT_decl_line   : 441
  <1854>     DW_AT_type        : <1782>
 <1><1858>: Abbrev Number: 17 (DW_TAG_typedef)
  <1859>     DW_AT_name        : MUSB_pfIsochIrpComplete
  <1871>     DW_AT_decl_file   : 8
  <1872>     DW_AT_decl_line   : 451
  <1874>     DW_AT_type        : <1878>
 <1><1878>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1879>     DW_AT_byte_size   : 4
  <187a>     DW_AT_type        : <187e>
 <1><187e>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <187f>     DW_AT_prototyped  : 1
  <1880>     DW_AT_sibling     : <188f>
 <2><1884>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1885>     DW_AT_type        : <f1>
 <2><1889>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <188a>     DW_AT_type        : <188f>
 <1><188f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1890>     DW_AT_byte_size   : 4
  <1891>     DW_AT_type        : <1895>
 <1><1895>: Abbrev Number: 18 (DW_TAG_structure_type)
  <1896>     DW_AT_name        : _MUSB_IsochIrp
  <18a5>     DW_AT_byte_size   : 36
  <18a6>     DW_AT_decl_file   : 8
  <18a7>     DW_AT_decl_line   : 443
  <18a9>     DW_AT_sibling     : <19a0>
 <2><18ad>: Abbrev Number: 13 (DW_TAG_member)
  <18ae>     DW_AT_name        : hPipe
  <18b4>     DW_AT_decl_file   : 8
  <18b5>     DW_AT_decl_line   : 488
  <18b7>     DW_AT_type        : <15ea>
  <18bb>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><18be>: Abbrev Number: 13 (DW_TAG_member)
  <18bf>     DW_AT_name        : pBuffer
  <18c7>     DW_AT_decl_file   : 8
  <18c8>     DW_AT_decl_line   : 489
  <18ca>     DW_AT_type        : <14e3>
  <18ce>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><18d1>: Abbrev Number: 13 (DW_TAG_member)
  <18d2>     DW_AT_name        : adwLength
  <18dc>     DW_AT_decl_file   : 8
  <18dd>     DW_AT_decl_line   : 490
  <18df>     DW_AT_type        : <19a0>
  <18e3>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><18e6>: Abbrev Number: 13 (DW_TAG_member)
  <18e7>     DW_AT_name        : adwStatus
  <18f1>     DW_AT_decl_file   : 8
  <18f2>     DW_AT_decl_line   : 491
  <18f4>     DW_AT_type        : <19a0>
  <18f8>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><18fb>: Abbrev Number: 13 (DW_TAG_member)
  <18fc>     DW_AT_name        : adwActualLength
  <190c>     DW_AT_decl_file   : 8
  <190d>     DW_AT_decl_line   : 492
  <190f>     DW_AT_type        : <19a0>
  <1913>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1916>: Abbrev Number: 23 (DW_TAG_member)
  <1917>     DW_AT_name        : (indirect string, offset: 0x1c6): pfIrpComplete
  <191b>     DW_AT_decl_file   : 8
  <191c>     DW_AT_decl_line   : 493
  <191e>     DW_AT_type        : <1858>
  <1922>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1925>: Abbrev Number: 23 (DW_TAG_member)
  <1926>     DW_AT_name        : (indirect string, offset: 0xc): pCompleteParam
  <192a>     DW_AT_decl_file   : 8
  <192b>     DW_AT_decl_line   : 494
  <192d>     DW_AT_type        : <f1>
  <1931>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><1934>: Abbrev Number: 13 (DW_TAG_member)
  <1935>     DW_AT_name        : wFrameCount
  <1941>     DW_AT_decl_file   : 8
  <1942>     DW_AT_decl_line   : 495
  <1944>     DW_AT_type        : <16f>
  <1948>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><194b>: Abbrev Number: 13 (DW_TAG_member)
  <194c>     DW_AT_name        : wCurrentFrame
  <195a>     DW_AT_decl_file   : 8
  <195b>     DW_AT_decl_line   : 496
  <195d>     DW_AT_type        : <16f>
  <1961>     DW_AT_data_member_location: 2 byte block: 23 1e    (DW_OP_plus_uconst: 30)
 <2><1964>: Abbrev Number: 13 (DW_TAG_member)
  <1965>     DW_AT_name        : wCallbackInterval
  <1977>     DW_AT_decl_file   : 8
  <1978>     DW_AT_decl_line   : 497
  <197a>     DW_AT_type        : <16f>
  <197e>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><1981>: Abbrev Number: 23 (DW_TAG_member)
  <1982>     DW_AT_name        : (indirect string, offset: 0x43): bIsrCallback
  <1986>     DW_AT_decl_file   : 8
  <1987>     DW_AT_decl_line   : 498
  <1989>     DW_AT_type        : <160>
  <198d>     DW_AT_data_member_location: 2 byte block: 23 22    (DW_OP_plus_uconst: 34)
 <2><1990>: Abbrev Number: 23 (DW_TAG_member)
  <1991>     DW_AT_name        : (indirect string, offset: 0xf0): bAllowDma
  <1995>     DW_AT_decl_file   : 8
  <1996>     DW_AT_decl_line   : 499
  <1998>     DW_AT_type        : <160>
  <199c>     DW_AT_data_member_location: 2 byte block: 23 23    (DW_OP_plus_uconst: 35)
 <1><19a0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <19a1>     DW_AT_byte_size   : 4
  <19a2>     DW_AT_type        : <18e>
 <1><19a6>: Abbrev Number: 17 (DW_TAG_typedef)
  <19a7>     DW_AT_name        : MUSB_IsochIrp
  <19b5>     DW_AT_decl_file   : 8
  <19b6>     DW_AT_decl_line   : 500
  <19b8>     DW_AT_type        : <1895>
 <1><19bc>: Abbrev Number: 17 (DW_TAG_typedef)
  <19bd>     DW_AT_name        : MUSB_pfNewUsbState
  <19d0>     DW_AT_decl_file   : 8
  <19d1>     DW_AT_decl_line   : 515
  <19d3>     DW_AT_type        : <19d7>
 <1><19d7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <19d8>     DW_AT_byte_size   : 4
  <19d9>     DW_AT_type        : <19dd>
 <1><19dd>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <19de>     DW_AT_prototyped  : 1
  <19df>     DW_AT_sibling     : <19f3>
 <2><19e3>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <19e4>     DW_AT_type        : <f1>
 <2><19e8>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <19e9>     DW_AT_type        : <1503>
 <2><19ed>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <19ee>     DW_AT_type        : <11c5>
 <1><19f3>: Abbrev Number: 17 (DW_TAG_typedef)
  <19f4>     DW_AT_name        : MUSB_pfDeviceRequest
  <1a09>     DW_AT_decl_file   : 8
  <1a0a>     DW_AT_decl_line   : 537
  <1a0c>     DW_AT_type        : <1a10>
 <1><1a10>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1a11>     DW_AT_byte_size   : 4
  <1a12>     DW_AT_type        : <1a16>
 <1><1a16>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <1a17>     DW_AT_prototyped  : 1
  <1a18>     DW_AT_type        : <160>
  <1a1c>     DW_AT_sibling     : <1a3a>
 <2><1a20>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a21>     DW_AT_type        : <f1>
 <2><1a25>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a26>     DW_AT_type        : <1503>
 <2><1a2a>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a2b>     DW_AT_type        : <18e>
 <2><1a2f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a30>     DW_AT_type        : <b51>
 <2><1a34>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a35>     DW_AT_type        : <16f>
 <1><1a3a>: Abbrev Number: 17 (DW_TAG_typedef)
  <1a3b>     DW_AT_name        : MUSB_pfDeviceConfigSelected
  <1a57>     DW_AT_decl_file   : 8
  <1a58>     DW_AT_decl_line   : 567
  <1a5a>     DW_AT_type        : <1a5e>
 <1><1a5e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1a5f>     DW_AT_byte_size   : 4
  <1a60>     DW_AT_type        : <1a64>
 <1><1a64>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <1a65>     DW_AT_prototyped  : 1
  <1a66>     DW_AT_type        : <160>
  <1a6a>     DW_AT_sibling     : <1a83>
 <2><1a6e>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a6f>     DW_AT_type        : <f1>
 <2><1a73>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a74>     DW_AT_type        : <1503>
 <2><1a78>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a79>     DW_AT_type        : <160>
 <2><1a7d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1a7e>     DW_AT_type        : <1a83>
 <1><1a83>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1a84>     DW_AT_byte_size   : 4
  <1a85>     DW_AT_type        : <15ea>
 <1><1a89>: Abbrev Number: 17 (DW_TAG_typedef)
  <1a8a>     DW_AT_name        : MUSB_pfInterfaceSet
  <1a9e>     DW_AT_decl_file   : 8
  <1a9f>     DW_AT_decl_line   : 598
  <1aa1>     DW_AT_type        : <1aa5>
 <1><1aa5>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1aa6>     DW_AT_byte_size   : 4
  <1aa7>     DW_AT_type        : <1aab>
 <1><1aab>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <1aac>     DW_AT_prototyped  : 1
  <1aad>     DW_AT_type        : <160>
  <1ab1>     DW_AT_sibling     : <1acf>
 <2><1ab5>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1ab6>     DW_AT_type        : <f1>
 <2><1aba>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1abb>     DW_AT_type        : <1503>
 <2><1abf>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1ac0>     DW_AT_type        : <160>
 <2><1ac4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1ac5>     DW_AT_type        : <160>
 <2><1ac9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1aca>     DW_AT_type        : <1a83>
 <1><1acf>: Abbrev Number: 22 (DW_TAG_structure_type)
  <1ad0>     DW_AT_byte_size   : 44
  <1ad1>     DW_AT_decl_file   : 8
  <1ad2>     DW_AT_decl_line   : 654
  <1ad4>     DW_AT_sibling     : <1c40>
 <2><1ad8>: Abbrev Number: 23 (DW_TAG_member)
  <1ad9>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <1add>     DW_AT_decl_file   : 8
  <1ade>     DW_AT_decl_line   : 655
  <1ae0>     DW_AT_type        : <f1>
  <1ae4>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1ae7>: Abbrev Number: 13 (DW_TAG_member)
  <1ae8>     DW_AT_name        : pStandardDescriptors
  <1afd>     DW_AT_decl_file   : 8
  <1afe>     DW_AT_decl_line   : 656
  <1b00>     DW_AT_type        : <b51>
  <1b04>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1b07>: Abbrev Number: 23 (DW_TAG_member)
  <1b08>     DW_AT_name        : (indirect string, offset: 0x191): wDescriptorBufferLength
  <1b0c>     DW_AT_decl_file   : 8
  <1b0d>     DW_AT_decl_line   : 657
  <1b0f>     DW_AT_type        : <16f>
  <1b13>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1b16>: Abbrev Number: 13 (DW_TAG_member)
  <1b17>     DW_AT_name        : wStringDescriptorCount
  <1b2e>     DW_AT_decl_file   : 8
  <1b2f>     DW_AT_decl_line   : 658
  <1b31>     DW_AT_type        : <16f>
  <1b35>     DW_AT_data_member_location: 2 byte block: 23 a     (DW_OP_plus_uconst: 10)
 <2><1b38>: Abbrev Number: 13 (DW_TAG_member)
  <1b39>     DW_AT_name        : pHighSpeedDescriptors
  <1b4f>     DW_AT_decl_file   : 8
  <1b50>     DW_AT_decl_line   : 659
  <1b52>     DW_AT_type        : <b51>
  <1b56>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1b59>: Abbrev Number: 13 (DW_TAG_member)
  <1b5a>     DW_AT_name        : wHighSpeedDescriptorLength
  <1b75>     DW_AT_decl_file   : 8
  <1b76>     DW_AT_decl_line   : 660
  <1b78>     DW_AT_type        : <16f>
  <1b7c>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1b7f>: Abbrev Number: 13 (DW_TAG_member)
  <1b80>     DW_AT_name        : wControlBufferLength
  <1b95>     DW_AT_decl_file   : 8
  <1b96>     DW_AT_decl_line   : 661
  <1b98>     DW_AT_type        : <16f>
  <1b9c>     DW_AT_data_member_location: 2 byte block: 23 12    (DW_OP_plus_uconst: 18)
 <2><1b9f>: Abbrev Number: 13 (DW_TAG_member)
  <1ba0>     DW_AT_name        : pControlBuffer
  <1baf>     DW_AT_decl_file   : 8
  <1bb0>     DW_AT_decl_line   : 662
  <1bb2>     DW_AT_type        : <14e3>
  <1bb6>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1bb9>: Abbrev Number: 13 (DW_TAG_member)
  <1bba>     DW_AT_name        : pbSelfPowered
  <1bc8>     DW_AT_decl_file   : 8
  <1bc9>     DW_AT_decl_line   : 663
  <1bcb>     DW_AT_type        : <14e3>
  <1bcf>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><1bd2>: Abbrev Number: 13 (DW_TAG_member)
  <1bd3>     DW_AT_name        : pfDeviceRequest
  <1be3>     DW_AT_decl_file   : 8
  <1be4>     DW_AT_decl_line   : 664
  <1be6>     DW_AT_type        : <19f3>
  <1bea>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1bed>: Abbrev Number: 13 (DW_TAG_member)
  <1bee>     DW_AT_name        : pfDeviceConfigSelected
  <1c05>     DW_AT_decl_file   : 8
  <1c06>     DW_AT_decl_line   : 665
  <1c08>     DW_AT_type        : <1a3a>
  <1c0c>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><1c0f>: Abbrev Number: 13 (DW_TAG_member)
  <1c10>     DW_AT_name        : pfInterfaceSet
  <1c1f>     DW_AT_decl_file   : 8
  <1c20>     DW_AT_decl_line   : 666
  <1c22>     DW_AT_type        : <1a89>
  <1c26>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><1c29>: Abbrev Number: 13 (DW_TAG_member)
  <1c2a>     DW_AT_name        : pfUsbState
  <1c35>     DW_AT_decl_file   : 8
  <1c36>     DW_AT_decl_line   : 667
  <1c38>     DW_AT_type        : <19bc>
  <1c3c>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <1><1c40>: Abbrev Number: 17 (DW_TAG_typedef)
  <1c41>     DW_AT_name        : MUSB_FunctionClient
  <1c55>     DW_AT_decl_file   : 8
  <1c56>     DW_AT_decl_line   : 668
  <1c58>     DW_AT_type        : <1acf>
 <1><1c5c>: Abbrev Number: 17 (DW_TAG_typedef)
  <1c5d>     DW_AT_name        : MUSB_pfDeviceConnected
  <1c74>     DW_AT_decl_file   : 8
  <1c75>     DW_AT_decl_line   : 684
  <1c77>     DW_AT_type        : <1c7b>
 <1><1c7b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1c7c>     DW_AT_byte_size   : 4
  <1c7d>     DW_AT_type        : <1c81>
 <1><1c81>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <1c82>     DW_AT_prototyped  : 1
  <1c83>     DW_AT_type        : <160>
  <1c87>     DW_AT_sibling     : <1ca0>
 <2><1c8b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1c8c>     DW_AT_type        : <f1>
 <2><1c90>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1c91>     DW_AT_type        : <1503>
 <2><1c95>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1c96>     DW_AT_type        : <1ca0>
 <2><1c9a>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1c9b>     DW_AT_type        : <b51>
 <1><1ca0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1ca1>     DW_AT_byte_size   : 4
  <1ca2>     DW_AT_type        : <14ef>
 <1><1ca6>: Abbrev Number: 17 (DW_TAG_typedef)
  <1ca7>     DW_AT_name        : MUSB_pfDeviceDisconnected
  <1cc1>     DW_AT_decl_file   : 8
  <1cc2>     DW_AT_decl_line   : 694
  <1cc4>     DW_AT_type        : <1cc8>
 <1><1cc8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1cc9>     DW_AT_byte_size   : 4
  <1cca>     DW_AT_type        : <1cce>
 <1><1cce>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1ccf>     DW_AT_prototyped  : 1
  <1cd0>     DW_AT_sibling     : <1ce4>
 <2><1cd4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1cd5>     DW_AT_type        : <f1>
 <2><1cd9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1cda>     DW_AT_type        : <1503>
 <2><1cde>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1cdf>     DW_AT_type        : <1ca0>
 <1><1ce4>: Abbrev Number: 17 (DW_TAG_typedef)
  <1ce5>     DW_AT_name        : MUSB_pfDriverTimerExpired
  <1cff>     DW_AT_decl_file   : 8
  <1d00>     DW_AT_decl_line   : 702
  <1d02>     DW_AT_type        : <1d06>
 <1><1d06>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1d07>     DW_AT_byte_size   : 4
  <1d08>     DW_AT_type        : <1d0c>
 <1><1d0c>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1d0d>     DW_AT_prototyped  : 1
  <1d0e>     DW_AT_sibling     : <1d1d>
 <2><1d12>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1d13>     DW_AT_type        : <f1>
 <2><1d17>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1d18>     DW_AT_type        : <1503>
 <1><1d1d>: Abbrev Number: 17 (DW_TAG_typedef)
  <1d1e>     DW_AT_name        : MUSB_pfBusSuspended
  <1d32>     DW_AT_decl_file   : 8
  <1d33>     DW_AT_decl_line   : 710
  <1d35>     DW_AT_type        : <1d06>
 <1><1d39>: Abbrev Number: 17 (DW_TAG_typedef)
  <1d3a>     DW_AT_name        : MUSB_pfBusResumed
  <1d4c>     DW_AT_decl_file   : 8
  <1d4d>     DW_AT_decl_line   : 718
  <1d4f>     DW_AT_type        : <1d06>
 <1><1d53>: Abbrev Number: 18 (DW_TAG_structure_type)
  <1d54>     DW_AT_name        : _MUSB_DeviceDriver
  <1d67>     DW_AT_byte_size   : 28
  <1d68>     DW_AT_decl_file   : 8
  <1d69>     DW_AT_decl_line   : 740
  <1d6b>     DW_AT_sibling     : <1e24>
 <2><1d6f>: Abbrev Number: 23 (DW_TAG_member)
  <1d70>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <1d74>     DW_AT_decl_file   : 8
  <1d75>     DW_AT_decl_line   : 741
  <1d77>     DW_AT_type        : <f1>
  <1d7b>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1d7e>: Abbrev Number: 13 (DW_TAG_member)
  <1d7f>     DW_AT_name        : bTimerCount
  <1d8b>     DW_AT_decl_file   : 8
  <1d8c>     DW_AT_decl_line   : 742
  <1d8e>     DW_AT_type        : <160>
  <1d92>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1d95>: Abbrev Number: 13 (DW_TAG_member)
  <1d96>     DW_AT_name        : adwTimerResolutions
  <1daa>     DW_AT_decl_file   : 8
  <1dab>     DW_AT_decl_line   : 743
  <1dad>     DW_AT_type        : <19a0>
  <1db1>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1db4>: Abbrev Number: 13 (DW_TAG_member)
  <1db5>     DW_AT_name        : pfDeviceConnected
  <1dc7>     DW_AT_decl_file   : 8
  <1dc8>     DW_AT_decl_line   : 744
  <1dca>     DW_AT_type        : <1c5c>
  <1dce>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1dd1>: Abbrev Number: 13 (DW_TAG_member)
  <1dd2>     DW_AT_name        : pfDeviceDisconnected
  <1de7>     DW_AT_decl_file   : 8
  <1de8>     DW_AT_decl_line   : 745
  <1dea>     DW_AT_type        : <1ca6>
  <1dee>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1df1>: Abbrev Number: 13 (DW_TAG_member)
  <1df2>     DW_AT_name        : pfBusSuspended
  <1e01>     DW_AT_decl_file   : 8
  <1e02>     DW_AT_decl_line   : 746
  <1e04>     DW_AT_type        : <1d1d>
  <1e08>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1e0b>: Abbrev Number: 13 (DW_TAG_member)
  <1e0c>     DW_AT_name        : pfBusResumed
  <1e19>     DW_AT_decl_file   : 8
  <1e1a>     DW_AT_decl_line   : 747
  <1e1c>     DW_AT_type        : <1d39>
  <1e20>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <1><1e24>: Abbrev Number: 17 (DW_TAG_typedef)
  <1e25>     DW_AT_name        : MUSB_DeviceDriver
  <1e37>     DW_AT_decl_file   : 8
  <1e38>     DW_AT_decl_line   : 748
  <1e3a>     DW_AT_type        : <1d53>
 <1><1e3e>: Abbrev Number: 22 (DW_TAG_structure_type)
  <1e3f>     DW_AT_byte_size   : 16
  <1e40>     DW_AT_decl_file   : 8
  <1e41>     DW_AT_decl_line   : 765
  <1e43>     DW_AT_sibling     : <1ec4>
 <2><1e47>: Abbrev Number: 13 (DW_TAG_member)
  <1e48>     DW_AT_name        : pPeripheralList
  <1e58>     DW_AT_decl_file   : 8
  <1e59>     DW_AT_decl_line   : 766
  <1e5b>     DW_AT_type        : <b51>
  <1e5f>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1e62>: Abbrev Number: 13 (DW_TAG_member)
  <1e63>     DW_AT_name        : wPeripheralListLength
  <1e79>     DW_AT_decl_file   : 8
  <1e7a>     DW_AT_decl_line   : 767
  <1e7c>     DW_AT_type        : <16f>
  <1e80>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1e83>: Abbrev Number: 13 (DW_TAG_member)
  <1e84>     DW_AT_name        : aDeviceDriverList
  <1e96>     DW_AT_decl_file   : 8
  <1e97>     DW_AT_decl_line   : 768
  <1e99>     DW_AT_type        : <1ec4>
  <1e9d>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1ea0>: Abbrev Number: 13 (DW_TAG_member)
  <1ea1>     DW_AT_name        : bDeviceDriverListLength
  <1eb9>     DW_AT_decl_file   : 8
  <1eba>     DW_AT_decl_line   : 769
  <1ebc>     DW_AT_type        : <160>
  <1ec0>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><1ec4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1ec5>     DW_AT_byte_size   : 4
  <1ec6>     DW_AT_type        : <1e24>
 <1><1eca>: Abbrev Number: 17 (DW_TAG_typedef)
  <1ecb>     DW_AT_name        : MUSB_HostClient
  <1edb>     DW_AT_decl_file   : 8
  <1edc>     DW_AT_decl_line   : 770
  <1ede>     DW_AT_type        : <1e3e>
 <1><1ee2>: Abbrev Number: 17 (DW_TAG_typedef)
  <1ee3>     DW_AT_name        : MUSB_pfNewOtgState
  <1ef6>     DW_AT_decl_file   : 8
  <1ef7>     DW_AT_decl_line   : 780
  <1ef9>     DW_AT_type        : <1efd>
 <1><1efd>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1efe>     DW_AT_byte_size   : 4
  <1eff>     DW_AT_type        : <1f03>
 <1><1f03>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1f04>     DW_AT_prototyped  : 1
  <1f05>     DW_AT_sibling     : <1f19>
 <2><1f09>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f0a>     DW_AT_type        : <f1>
 <2><1f0e>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f0f>     DW_AT_type        : <1503>
 <2><1f13>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f14>     DW_AT_type        : <12a2>
 <1><1f19>: Abbrev Number: 17 (DW_TAG_typedef)
  <1f1a>     DW_AT_name        : MUSB_pfOtgError
  <1f2a>     DW_AT_decl_file   : 8
  <1f2b>     DW_AT_decl_line   : 791
  <1f2d>     DW_AT_type        : <1f31>
 <1><1f31>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1f32>     DW_AT_byte_size   : 4
  <1f33>     DW_AT_type        : <1f37>
 <1><1f37>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1f38>     DW_AT_prototyped  : 1
  <1f39>     DW_AT_sibling     : <1f4d>
 <2><1f3d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f3e>     DW_AT_type        : <f1>
 <2><1f42>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f43>     DW_AT_type        : <1503>
 <2><1f47>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1f48>     DW_AT_type        : <18e>
 <1><1f4d>: Abbrev Number: 22 (DW_TAG_structure_type)
  <1f4e>     DW_AT_byte_size   : 16
  <1f4f>     DW_AT_decl_file   : 8
  <1f50>     DW_AT_decl_line   : 809
  <1f52>     DW_AT_sibling     : <1fae>
 <2><1f56>: Abbrev Number: 23 (DW_TAG_member)
  <1f57>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <1f5b>     DW_AT_decl_file   : 8
  <1f5c>     DW_AT_decl_line   : 810
  <1f5e>     DW_AT_type        : <f1>
  <1f62>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1f65>: Abbrev Number: 13 (DW_TAG_member)
  <1f66>     DW_AT_name        : pbDesireHostRole
  <1f77>     DW_AT_decl_file   : 8
  <1f78>     DW_AT_decl_line   : 811
  <1f7a>     DW_AT_type        : <14e3>
  <1f7e>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1f81>: Abbrev Number: 13 (DW_TAG_member)
  <1f82>     DW_AT_name        : pfOtgState
  <1f8d>     DW_AT_decl_file   : 8
  <1f8e>     DW_AT_decl_line   : 812
  <1f90>     DW_AT_type        : <1ee2>
  <1f94>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1f97>: Abbrev Number: 13 (DW_TAG_member)
  <1f98>     DW_AT_name        : pfOtgError
  <1fa3>     DW_AT_decl_file   : 8
  <1fa4>     DW_AT_decl_line   : 813
  <1fa6>     DW_AT_type        : <1f19>
  <1faa>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><1fae>: Abbrev Number: 17 (DW_TAG_typedef)
  <1faf>     DW_AT_name        : MUSB_OtgClient
  <1fbe>     DW_AT_decl_file   : 8
  <1fbf>     DW_AT_decl_line   : 814
  <1fc1>     DW_AT_type        : <1f4d>
 <1><1fc5>: Abbrev Number: 17 (DW_TAG_typedef)
  <1fc6>     DW_AT_name        : MUSB_pfHubEnumerationComplete
  <1fe4>     DW_AT_decl_file   : 8
  <1fe5>     DW_AT_decl_line   : 825
  <1fe7>     DW_AT_type        : <1feb>
 <1><1feb>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1fec>     DW_AT_byte_size   : 4
  <1fed>     DW_AT_type        : <1ff1>
 <1><1ff1>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <1ff2>     DW_AT_prototyped  : 1
  <1ff3>     DW_AT_sibling     : <2002>
 <2><1ff7>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1ff8>     DW_AT_type        : <1ca0>
 <2><1ffc>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <1ffd>     DW_AT_type        : <1ca0>
 <1><2002>: Abbrev Number: 11 (DW_TAG_structure_type)
  <2003>     DW_AT_byte_size   : 24
  <2004>     DW_AT_decl_file   : 9
  <2005>     DW_AT_decl_line   : 43
  <2006>     DW_AT_sibling     : <209c>
 <2><200a>: Abbrev Number: 12 (DW_TAG_member)
  <200b>     DW_AT_name        : wItemSize
  <2015>     DW_AT_decl_file   : 9
  <2016>     DW_AT_decl_line   : 44
  <2017>     DW_AT_type        : <19e>
  <201b>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><201e>: Abbrev Number: 12 (DW_TAG_member)
  <201f>     DW_AT_name        : wStaticItemCount
  <2030>     DW_AT_decl_file   : 9
  <2031>     DW_AT_decl_line   : 45
  <2032>     DW_AT_type        : <19e>
  <2036>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2039>: Abbrev Number: 12 (DW_TAG_member)
  <203a>     DW_AT_name        : pStaticBuffer
  <2048>     DW_AT_decl_file   : 9
  <2049>     DW_AT_decl_line   : 46
  <204a>     DW_AT_type        : <f1>
  <204e>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><2051>: Abbrev Number: 12 (DW_TAG_member)
  <2052>     DW_AT_name        : wItemCount
  <205d>     DW_AT_decl_file   : 9
  <205e>     DW_AT_decl_line   : 47
  <205f>     DW_AT_type        : <19e>
  <2063>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2066>: Abbrev Number: 12 (DW_TAG_member)
  <2067>     DW_AT_name        : wDynamicItemCount
  <2079>     DW_AT_decl_file   : 9
  <207a>     DW_AT_decl_line   : 48
  <207b>     DW_AT_type        : <19e>
  <207f>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><2082>: Abbrev Number: 12 (DW_TAG_member)
  <2083>     DW_AT_name        : pDynamicBuffer
  <2092>     DW_AT_decl_file   : 9
  <2093>     DW_AT_decl_line   : 49
  <2094>     DW_AT_type        : <f1>
  <2098>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <1><209c>: Abbrev Number: 2 (DW_TAG_typedef)
  <209d>     DW_AT_name        : MUSB_Array
  <20a8>     DW_AT_decl_file   : 9
  <20a9>     DW_AT_decl_line   : 50
  <20aa>     DW_AT_type        : <2002>
 <1><20ae>: Abbrev Number: 24 (DW_TAG_structure_type)
  <20af>     DW_AT_name        : _MUSB_LinkedList
  <20c0>     DW_AT_byte_size   : 12
  <20c1>     DW_AT_decl_file   : 9
  <20c2>     DW_AT_decl_line   : 110
  <20c3>     DW_AT_sibling     : <20fa>
 <2><20c7>: Abbrev Number: 12 (DW_TAG_member)
  <20c8>     DW_AT_name        : pItem
  <20ce>     DW_AT_decl_file   : 9
  <20cf>     DW_AT_decl_line   : 111
  <20d0>     DW_AT_type        : <f1>
  <20d4>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><20d7>: Abbrev Number: 12 (DW_TAG_member)
  <20d8>     DW_AT_name        : pNext
  <20de>     DW_AT_decl_file   : 9
  <20df>     DW_AT_decl_line   : 112
  <20e0>     DW_AT_type        : <20fa>
  <20e4>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><20e7>: Abbrev Number: 12 (DW_TAG_member)
  <20e8>     DW_AT_name        : dwCount
  <20f0>     DW_AT_decl_file   : 9
  <20f1>     DW_AT_decl_line   : 113
  <20f2>     DW_AT_type        : <94>
  <20f6>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><20fa>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <20fb>     DW_AT_byte_size   : 4
  <20fc>     DW_AT_type        : <20ae>
 <1><2100>: Abbrev Number: 2 (DW_TAG_typedef)
  <2101>     DW_AT_name        : MUSB_LinkedList
  <2111>     DW_AT_decl_file   : 9
  <2112>     DW_AT_decl_line   : 114
  <2113>     DW_AT_type        : <20ae>
 <1><2117>: Abbrev Number: 9 (DW_TAG_enumeration_type)
  <2118>     DW_AT_byte_size   : 4
  <2119>     DW_AT_decl_file   : 10
  <211a>     DW_AT_decl_line   : 30
  <211b>     DW_AT_sibling     : <223e>
 <2><211f>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2120>     DW_AT_name        : MUSB_HSET_PORT_NONE
  <2134>     DW_AT_const_value : 0
 <2><2135>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2136>     DW_AT_name        : MUSB_HSET_PORT_TEST_J
  <214c>     DW_AT_const_value : 1
 <2><214d>: Abbrev Number: 10 (DW_TAG_enumerator)
  <214e>     DW_AT_name        : MUSB_HSET_PORT_TEST_K
  <2164>     DW_AT_const_value : 2
 <2><2165>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2166>     DW_AT_name        : MUSB_HSET_PORT_TEST_SE0_NAK
  <2182>     DW_AT_const_value : 3
 <2><2183>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2184>     DW_AT_name        : MUSB_HSET_PORT_TEST_PACKET
  <219f>     DW_AT_const_value : 4
 <2><21a0>: Abbrev Number: 10 (DW_TAG_enumerator)
  <21a1>     DW_AT_name        : MUSB_HSET_PORT_RESET
  <21b6>     DW_AT_const_value : 5
 <2><21b7>: Abbrev Number: 10 (DW_TAG_enumerator)
  <21b8>     DW_AT_name        : MUSB_HSET_PORT_SUSPEND
  <21cf>     DW_AT_const_value : 6
 <2><21d0>: Abbrev Number: 10 (DW_TAG_enumerator)
  <21d1>     DW_AT_name        : MUSB_HSET_PORT_RESUME
  <21e7>     DW_AT_const_value : 7
 <2><21e8>: Abbrev Number: 10 (DW_TAG_enumerator)
  <21e9>     DW_AT_name        : MUSB_HSET_PORT_SETUP_START
  <2204>     DW_AT_const_value : 8
 <2><2205>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2206>     DW_AT_name        : MUSB_HSET_PORT_SETUP_IN
  <221e>     DW_AT_const_value : 9
 <2><221f>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2220>     DW_AT_name        : MUSB_HSET_PORT_SETUP_STATUS
  <223c>     DW_AT_const_value : 10
 <1><223e>: Abbrev Number: 2 (DW_TAG_typedef)
  <223f>     DW_AT_name        : MUSB_HsetPortMode
  <2251>     DW_AT_decl_file   : 10
  <2252>     DW_AT_decl_line   : 53
  <2253>     DW_AT_type        : <2117>
 <1><2257>: Abbrev Number: 25 (DW_TAG_enumeration_type)
  <2258>     DW_AT_name        : _MUSB_DmaChannelStatus
  <226f>     DW_AT_byte_size   : 4
  <2270>     DW_AT_decl_file   : 11
  <2271>     DW_AT_decl_line   : 57
  <2272>     DW_AT_sibling     : <22f8>
 <2><2276>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2277>     DW_AT_name        : MUSB_DMA_STATUS_UNKNOWN
  <228f>     DW_AT_const_value : 0
 <2><2290>: Abbrev Number: 10 (DW_TAG_enumerator)
  <2291>     DW_AT_name        : MUSB_DMA_STATUS_FREE
  <22a6>     DW_AT_const_value : 1
 <2><22a7>: Abbrev Number: 10 (DW_TAG_enumerator)
  <22a8>     DW_AT_name        : MUSB_DMA_STATUS_BUSY
  <22bd>     DW_AT_const_value : 2
 <2><22be>: Abbrev Number: 10 (DW_TAG_enumerator)
  <22bf>     DW_AT_name        : MUSB_DMA_STATUS_BUS_ABORT
  <22d9>     DW_AT_const_value : 3
 <2><22da>: Abbrev Number: 10 (DW_TAG_enumerator)
  <22db>     DW_AT_name        : MUSB_DMA_STATUS_CORE_ABORT
  <22f6>     DW_AT_const_value : 4
 <1><22f8>: Abbrev Number: 2 (DW_TAG_typedef)
  <22f9>     DW_AT_name        : MUSB_DmaChannelStatus
  <230f>     DW_AT_decl_file   : 11
  <2310>     DW_AT_decl_line   : 68
  <2311>     DW_AT_type        : <2257>
 <1><2315>: Abbrev Number: 11 (DW_TAG_structure_type)
  <2316>     DW_AT_byte_size   : 20
  <2317>     DW_AT_decl_file   : 11
  <2318>     DW_AT_decl_line   : 83
  <2319>     DW_AT_sibling     : <2375>
 <2><231d>: Abbrev Number: 20 (DW_TAG_member)
  <231e>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <2322>     DW_AT_decl_file   : 11
  <2323>     DW_AT_decl_line   : 84
  <2324>     DW_AT_type        : <f1>
  <2328>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><232b>: Abbrev Number: 12 (DW_TAG_member)
  <232c>     DW_AT_name        : dwMaxLength
  <2338>     DW_AT_decl_file   : 11
  <2339>     DW_AT_decl_line   : 85
  <233a>     DW_AT_type        : <18e>
  <233e>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2341>: Abbrev Number: 20 (DW_TAG_member)
  <2342>     DW_AT_name        : (indirect string, offset: 0x156): dwActualLength
  <2346>     DW_AT_decl_file   : 11
  <2347>     DW_AT_decl_line   : 86
  <2348>     DW_AT_type        : <18e>
  <234c>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><234f>: Abbrev Number: 20 (DW_TAG_member)
  <2350>     DW_AT_name        : (indirect string, offset: 0x1be): bStatus
  <2354>     DW_AT_decl_file   : 11
  <2355>     DW_AT_decl_line   : 87
  <2356>     DW_AT_type        : <22f8>
  <235a>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><235d>: Abbrev Number: 12 (DW_TAG_member)
  <235e>     DW_AT_name        : bDesiredMode
  <236b>     DW_AT_decl_file   : 11
  <236c>     DW_AT_decl_line   : 88
  <236d>     DW_AT_type        : <160>
  <2371>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <1><2375>: Abbrev Number: 2 (DW_TAG_typedef)
  <2376>     DW_AT_name        : MUSB_DmaChannel
  <2386>     DW_AT_decl_file   : 11
  <2387>     DW_AT_decl_line   : 89
  <2388>     DW_AT_type        : <2315>
 <1><238c>: Abbrev Number: 2 (DW_TAG_typedef)
  <238d>     DW_AT_name        : MUSB_pfDmaStartController
  <23a7>     DW_AT_decl_file   : 11
  <23a8>     DW_AT_decl_line   : 98
  <23a9>     DW_AT_type        : <23ad>
 <1><23ad>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <23ae>     DW_AT_byte_size   : 4
  <23af>     DW_AT_type        : <23b3>
 <1><23b3>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <23b4>     DW_AT_prototyped  : 1
  <23b5>     DW_AT_type        : <160>
  <23b9>     DW_AT_sibling     : <23c3>
 <2><23bd>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <23be>     DW_AT_type        : <f1>
 <1><23c3>: Abbrev Number: 2 (DW_TAG_typedef)
  <23c4>     DW_AT_name        : MUSB_pfDmaStopController
  <23dd>     DW_AT_decl_file   : 11
  <23de>     DW_AT_decl_line   : 107
  <23df>     DW_AT_type        : <23ad>
 <1><23e3>: Abbrev Number: 2 (DW_TAG_typedef)
  <23e4>     DW_AT_name        : MUSB_pfDmaAllocateChannel
  <23fe>     DW_AT_decl_file   : 11
  <23ff>     DW_AT_decl_line   : 123
  <2400>     DW_AT_type        : <2404>
 <1><2404>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2405>     DW_AT_byte_size   : 4
  <2406>     DW_AT_type        : <240a>
 <1><240a>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <240b>     DW_AT_prototyped  : 1
  <240c>     DW_AT_type        : <242e>
  <2410>     DW_AT_sibling     : <242e>
 <2><2414>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2415>     DW_AT_type        : <f1>
 <2><2419>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <241a>     DW_AT_type        : <160>
 <2><241e>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <241f>     DW_AT_type        : <160>
 <2><2423>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2424>     DW_AT_type        : <160>
 <2><2428>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2429>     DW_AT_type        : <16f>
 <1><242e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <242f>     DW_AT_byte_size   : 4
  <2430>     DW_AT_type        : <2375>
 <1><2434>: Abbrev Number: 2 (DW_TAG_typedef)
  <2435>     DW_AT_name        : MUSB_pfDmaReleaseChannel
  <244e>     DW_AT_decl_file   : 11
  <244f>     DW_AT_decl_line   : 132
  <2450>     DW_AT_type        : <2454>
 <1><2454>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2455>     DW_AT_byte_size   : 4
  <2456>     DW_AT_type        : <245a>
 <1><245a>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <245b>     DW_AT_prototyped  : 1
  <245c>     DW_AT_sibling     : <2466>
 <2><2460>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2461>     DW_AT_type        : <242e>
 <1><2466>: Abbrev Number: 2 (DW_TAG_typedef)
  <2467>     DW_AT_name        : MUSB_pfDmaAllocateBuffer
  <2480>     DW_AT_decl_file   : 11
  <2481>     DW_AT_decl_line   : 145
  <2482>     DW_AT_type        : <2486>
 <1><2486>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2487>     DW_AT_byte_size   : 4
  <2488>     DW_AT_type        : <248c>
 <1><248c>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <248d>     DW_AT_prototyped  : 1
  <248e>     DW_AT_type        : <14e3>
  <2492>     DW_AT_sibling     : <24a1>
 <2><2496>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2497>     DW_AT_type        : <242e>
 <2><249b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <249c>     DW_AT_type        : <18e>
 <1><24a1>: Abbrev Number: 2 (DW_TAG_typedef)
  <24a2>     DW_AT_name        : MUSB_pfDmaReleaseBuffer
  <24ba>     DW_AT_decl_file   : 11
  <24bb>     DW_AT_decl_line   : 158
  <24bc>     DW_AT_type        : <24c0>
 <1><24c0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <24c1>     DW_AT_byte_size   : 4
  <24c2>     DW_AT_type        : <24c6>
 <1><24c6>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <24c7>     DW_AT_prototyped  : 1
  <24c8>     DW_AT_type        : <160>
  <24cc>     DW_AT_sibling     : <24db>
 <2><24d0>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <24d1>     DW_AT_type        : <242e>
 <2><24d5>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <24d6>     DW_AT_type        : <14e3>
 <1><24db>: Abbrev Number: 2 (DW_TAG_typedef)
  <24dc>     DW_AT_name        : MUSB_pfDmaProgramChannel
  <24f5>     DW_AT_decl_file   : 11
  <24f6>     DW_AT_decl_line   : 178
  <24f7>     DW_AT_type        : <24fb>
 <1><24fb>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <24fc>     DW_AT_byte_size   : 4
  <24fd>     DW_AT_type        : <2501>
 <1><2501>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <2502>     DW_AT_prototyped  : 1
  <2503>     DW_AT_type        : <160>
  <2507>     DW_AT_sibling     : <2525>
 <2><250b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <250c>     DW_AT_type        : <242e>
 <2><2510>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2511>     DW_AT_type        : <16f>
 <2><2515>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2516>     DW_AT_type        : <160>
 <2><251a>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <251b>     DW_AT_type        : <b51>
 <2><251f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2520>     DW_AT_type        : <18e>
 <1><2525>: Abbrev Number: 2 (DW_TAG_typedef)
  <2526>     DW_AT_name        : MUSB_pfDmaGetChannelStatus
  <2541>     DW_AT_decl_file   : 11
  <2542>     DW_AT_decl_line   : 189
  <2543>     DW_AT_type        : <2547>
 <1><2547>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2548>     DW_AT_byte_size   : 4
  <2549>     DW_AT_type        : <254d>
 <1><254d>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <254e>     DW_AT_prototyped  : 1
  <254f>     DW_AT_type        : <22f8>
  <2553>     DW_AT_sibling     : <255d>
 <2><2557>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2558>     DW_AT_type        : <242e>
 <1><255d>: Abbrev Number: 2 (DW_TAG_typedef)
  <255e>     DW_AT_name        : MUSB_pfDmaControllerIsr
  <2576>     DW_AT_decl_file   : 11
  <2577>     DW_AT_decl_line   : 209
  <2578>     DW_AT_type        : <23ad>
 <1><257c>: Abbrev Number: 11 (DW_TAG_structure_type)
  <257d>     DW_AT_byte_size   : 40
  <257e>     DW_AT_decl_file   : 11
  <257f>     DW_AT_decl_line   : 230
  <2580>     DW_AT_sibling     : <26a3>
 <2><2584>: Abbrev Number: 20 (DW_TAG_member)
  <2585>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <2589>     DW_AT_decl_file   : 11
  <258a>     DW_AT_decl_line   : 231
  <258b>     DW_AT_type        : <f1>
  <258f>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><2592>: Abbrev Number: 12 (DW_TAG_member)
  <2593>     DW_AT_name        : pfDmaStartController
  <25a8>     DW_AT_decl_file   : 11
  <25a9>     DW_AT_decl_line   : 232
  <25aa>     DW_AT_type        : <238c>
  <25ae>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><25b1>: Abbrev Number: 12 (DW_TAG_member)
  <25b2>     DW_AT_name        : pfDmaStopController
  <25c6>     DW_AT_decl_file   : 11
  <25c7>     DW_AT_decl_line   : 233
  <25c8>     DW_AT_type        : <23c3>
  <25cc>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><25cf>: Abbrev Number: 12 (DW_TAG_member)
  <25d0>     DW_AT_name        : pfDmaAllocateChannel
  <25e5>     DW_AT_decl_file   : 11
  <25e6>     DW_AT_decl_line   : 234
  <25e7>     DW_AT_type        : <23e3>
  <25eb>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><25ee>: Abbrev Number: 12 (DW_TAG_member)
  <25ef>     DW_AT_name        : pfDmaReleaseChannel
  <2603>     DW_AT_decl_file   : 11
  <2604>     DW_AT_decl_line   : 235
  <2605>     DW_AT_type        : <2434>
  <2609>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><260c>: Abbrev Number: 12 (DW_TAG_member)
  <260d>     DW_AT_name        : pfDmaAllocateBuffer
  <2621>     DW_AT_decl_file   : 11
  <2622>     DW_AT_decl_line   : 236
  <2623>     DW_AT_type        : <2466>
  <2627>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><262a>: Abbrev Number: 12 (DW_TAG_member)
  <262b>     DW_AT_name        : pfDmaReleaseBuffer
  <263e>     DW_AT_decl_file   : 11
  <263f>     DW_AT_decl_line   : 237
  <2640>     DW_AT_type        : <24a1>
  <2644>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><2647>: Abbrev Number: 12 (DW_TAG_member)
  <2648>     DW_AT_name        : pfDmaProgramChannel
  <265c>     DW_AT_decl_file   : 11
  <265d>     DW_AT_decl_line   : 238
  <265e>     DW_AT_type        : <24db>
  <2662>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><2665>: Abbrev Number: 12 (DW_TAG_member)
  <2666>     DW_AT_name        : pfDmaGetChannelStatus
  <267c>     DW_AT_decl_file   : 11
  <267d>     DW_AT_decl_line   : 239
  <267e>     DW_AT_type        : <2525>
  <2682>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><2685>: Abbrev Number: 12 (DW_TAG_member)
  <2686>     DW_AT_name        : pfDmaControllerIsr
  <2699>     DW_AT_decl_file   : 11
  <269a>     DW_AT_decl_line   : 240
  <269b>     DW_AT_type        : <255d>
  <269f>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <1><26a3>: Abbrev Number: 2 (DW_TAG_typedef)
  <26a4>     DW_AT_name        : MUSB_DmaController
  <26b7>     DW_AT_decl_file   : 11
  <26b8>     DW_AT_decl_line   : 241
  <26b9>     DW_AT_type        : <257c>
 <1><26bd>: Abbrev Number: 17 (DW_TAG_typedef)
  <26be>     DW_AT_name        : MUSB_pfDmaChannelStatusChanged
  <26dd>     DW_AT_decl_file   : 11
  <26de>     DW_AT_decl_line   : 257
  <26e0>     DW_AT_type        : <26e4>
 <1><26e4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <26e5>     DW_AT_byte_size   : 4
  <26e6>     DW_AT_type        : <26ea>
 <1><26ea>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <26eb>     DW_AT_prototyped  : 1
  <26ec>     DW_AT_type        : <160>
  <26f0>     DW_AT_sibling     : <2704>
 <2><26f4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <26f5>     DW_AT_type        : <f1>
 <2><26f9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <26fa>     DW_AT_type        : <160>
 <2><26fe>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <26ff>     DW_AT_type        : <160>
 <1><2704>: Abbrev Number: 17 (DW_TAG_typedef)
  <2705>     DW_AT_name        : MUSB_pfNewDmaController
  <271d>     DW_AT_decl_file   : 11
  <271e>     DW_AT_decl_line   : 290
  <2720>     DW_AT_type        : <2724>
 <1><2724>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2725>     DW_AT_byte_size   : 4
  <2726>     DW_AT_type        : <272a>
 <1><272a>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <272b>     DW_AT_prototyped  : 1
  <272c>     DW_AT_type        : <2753>
  <2730>     DW_AT_sibling     : <2753>
 <2><2734>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2735>     DW_AT_type        : <26bd>
 <2><2739>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <273a>     DW_AT_type        : <f1>
 <2><273e>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <273f>     DW_AT_type        : <952>
 <2><2743>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2744>     DW_AT_type        : <f1>
 <2><2748>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <2749>     DW_AT_type        : <14e3>
 <2><274d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <274e>     DW_AT_type        : <14e3>
 <1><2753>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2754>     DW_AT_byte_size   : 4
  <2755>     DW_AT_type        : <26a3>
 <1><2759>: Abbrev Number: 17 (DW_TAG_typedef)
  <275a>     DW_AT_name        : MUSB_pfDestroyDmaController
  <2776>     DW_AT_decl_file   : 11
  <2777>     DW_AT_decl_line   : 296
  <2779>     DW_AT_type        : <277d>
 <1><277d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <277e>     DW_AT_byte_size   : 4
  <277f>     DW_AT_type        : <2783>
 <1><2783>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <2784>     DW_AT_prototyped  : 1
  <2785>     DW_AT_sibling     : <278f>
 <2><2789>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <278a>     DW_AT_type        : <2753>
 <1><278f>: Abbrev Number: 22 (DW_TAG_structure_type)
  <2790>     DW_AT_byte_size   : 12
  <2791>     DW_AT_decl_file   : 11
  <2792>     DW_AT_decl_line   : 313
  <2794>     DW_AT_sibling     : <27f9>
 <2><2798>: Abbrev Number: 13 (DW_TAG_member)
  <2799>     DW_AT_name        : wCoreRegistersExtent
  <27ae>     DW_AT_decl_file   : 11
  <27af>     DW_AT_decl_line   : 314
  <27b1>     DW_AT_type        : <16f>
  <27b5>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><27b8>: Abbrev Number: 13 (DW_TAG_member)
  <27b9>     DW_AT_name        : pfNewDmaController
  <27cc>     DW_AT_decl_file   : 11
  <27cd>     DW_AT_decl_line   : 315
  <27cf>     DW_AT_type        : <2704>
  <27d3>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><27d6>: Abbrev Number: 13 (DW_TAG_member)
  <27d7>     DW_AT_name        : pfDestroyDmaController
  <27ee>     DW_AT_decl_file   : 11
  <27ef>     DW_AT_decl_line   : 316
  <27f1>     DW_AT_type        : <2759>
  <27f5>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><27f9>: Abbrev Number: 17 (DW_TAG_typedef)
  <27fa>     DW_AT_name        : MUSB_DmaControllerFactory
  <2814>     DW_AT_decl_file   : 11
  <2815>     DW_AT_decl_line   : 317
  <2817>     DW_AT_type        : <278f>
 <1><281b>: Abbrev Number: 11 (DW_TAG_structure_type)
  <281c>     DW_AT_byte_size   : 8
  <281d>     DW_AT_decl_file   : 12
  <281e>     DW_AT_decl_line   : 148
  <281f>     DW_AT_sibling     : <2861>
 <2><2823>: Abbrev Number: 12 (DW_TAG_member)
  <2824>     DW_AT_name        : pData
  <282a>     DW_AT_decl_file   : 12
  <282b>     DW_AT_decl_line   : 152
  <282c>     DW_AT_type        : <f1>
  <2830>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><2833>: Abbrev Number: 12 (DW_TAG_member)
  <2834>     DW_AT_name        : bCause
  <283b>     DW_AT_decl_file   : 12
  <283c>     DW_AT_decl_line   : 153
  <283d>     DW_AT_type        : <160>
  <2841>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2844>: Abbrev Number: 20 (DW_TAG_member)
  <2845>     DW_AT_name        : (indirect string, offset: 0x170): bLocalEnd
  <2849>     DW_AT_decl_file   : 12
  <284a>     DW_AT_decl_line   : 154
  <284b>     DW_AT_type        : <160>
  <284f>     DW_AT_data_member_location: 2 byte block: 23 5     (DW_OP_plus_uconst: 5)
 <2><2852>: Abbrev Number: 20 (DW_TAG_member)
  <2853>     DW_AT_name        : (indirect string, offset: 0x1be): bStatus
  <2857>     DW_AT_decl_file   : 12
  <2858>     DW_AT_decl_line   : 155
  <2859>     DW_AT_type        : <160>
  <285d>     DW_AT_data_member_location: 2 byte block: 23 6     (DW_OP_plus_uconst: 6)
 <1><2861>: Abbrev Number: 2 (DW_TAG_typedef)
  <2862>     DW_AT_name        : MGC_BsrItem
  <286e>     DW_AT_decl_file   : 12
  <286f>     DW_AT_decl_line   : 156
  <2870>     DW_AT_type        : <281b>
 <1><2874>: Abbrev Number: 22 (DW_TAG_structure_type)
  <2875>     DW_AT_byte_size   : 96
  <2876>     DW_AT_decl_file   : 12
  <2877>     DW_AT_decl_line   : 269
  <2879>     DW_AT_sibling     : <2bc3>
 <2><287d>: Abbrev Number: 23 (DW_TAG_member)
  <287e>     DW_AT_name        : (indirect string, offset: 0x170): bLocalEnd
  <2882>     DW_AT_decl_file   : 12
  <2883>     DW_AT_decl_line   : 273
  <2885>     DW_AT_type        : <160>
  <2889>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><288c>: Abbrev Number: 13 (DW_TAG_member)
  <288d>     DW_AT_name        : bIsFifoShared
  <289b>     DW_AT_decl_file   : 12
  <289c>     DW_AT_decl_line   : 274
  <289e>     DW_AT_type        : <160>
  <28a2>     DW_AT_data_member_location: 2 byte block: 23 1     (DW_OP_plus_uconst: 1)
 <2><28a5>: Abbrev Number: 13 (DW_TAG_member)
  <28a6>     DW_AT_name        : wMaxPacketSizeTx
  <28b7>     DW_AT_decl_file   : 12
  <28b8>     DW_AT_decl_line   : 275
  <28ba>     DW_AT_type        : <16f>
  <28be>     DW_AT_data_member_location: 2 byte block: 23 2     (DW_OP_plus_uconst: 2)
 <2><28c1>: Abbrev Number: 13 (DW_TAG_member)
  <28c2>     DW_AT_name        : wMaxPacketSizeRx
  <28d3>     DW_AT_decl_file   : 12
  <28d4>     DW_AT_decl_line   : 276
  <28d6>     DW_AT_type        : <16f>
  <28da>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><28dd>: Abbrev Number: 23 (DW_TAG_member)
  <28de>     DW_AT_name        : (indirect string, offset: 0x24): bBusAddress
  <28e2>     DW_AT_decl_file   : 12
  <28e3>     DW_AT_decl_line   : 279
  <28e5>     DW_AT_type        : <160>
  <28e9>     DW_AT_data_member_location: 2 byte block: 23 6     (DW_OP_plus_uconst: 6)
 <2><28ec>: Abbrev Number: 13 (DW_TAG_member)
  <28ed>     DW_AT_name        : bBusEnd
  <28f5>     DW_AT_decl_file   : 12
  <28f6>     DW_AT_decl_line   : 280
  <28f8>     DW_AT_type        : <160>
  <28fc>     DW_AT_data_member_location: 2 byte block: 23 7     (DW_OP_plus_uconst: 7)
 <2><28ff>: Abbrev Number: 23 (DW_TAG_member)
  <2900>     DW_AT_name        : (indirect string, offset: 0x39): bInterval
  <2904>     DW_AT_decl_file   : 12
  <2905>     DW_AT_decl_line   : 281
  <2907>     DW_AT_type        : <160>
  <290b>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><290e>: Abbrev Number: 23 (DW_TAG_member)
  <290f>     DW_AT_name        : (indirect string, offset: 0x5a): bTrafficType
  <2913>     DW_AT_decl_file   : 12
  <2914>     DW_AT_decl_line   : 282
  <2916>     DW_AT_type        : <160>
  <291a>     DW_AT_data_member_location: 2 byte block: 23 9     (DW_OP_plus_uconst: 9)
 <2><291d>: Abbrev Number: 13 (DW_TAG_member)
  <291e>     DW_AT_name        : bRxBusAddress
  <292c>     DW_AT_decl_file   : 12
  <292d>     DW_AT_decl_line   : 283
  <292f>     DW_AT_type        : <160>
  <2933>     DW_AT_data_member_location: 2 byte block: 23 a     (DW_OP_plus_uconst: 10)
 <2><2936>: Abbrev Number: 13 (DW_TAG_member)
  <2937>     DW_AT_name        : bRxBusEnd
  <2941>     DW_AT_decl_file   : 12
  <2942>     DW_AT_decl_line   : 284
  <2944>     DW_AT_type        : <160>
  <2948>     DW_AT_data_member_location: 2 byte block: 23 b     (DW_OP_plus_uconst: 11)
 <2><294b>: Abbrev Number: 13 (DW_TAG_member)
  <294c>     DW_AT_name        : bRxInterval
  <2958>     DW_AT_decl_file   : 12
  <2959>     DW_AT_decl_line   : 285
  <295b>     DW_AT_type        : <160>
  <295f>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2962>: Abbrev Number: 13 (DW_TAG_member)
  <2963>     DW_AT_name        : bRxTrafficType
  <2972>     DW_AT_decl_file   : 12
  <2973>     DW_AT_decl_line   : 286
  <2975>     DW_AT_type        : <160>
  <2979>     DW_AT_data_member_location: 2 byte block: 23 d     (DW_OP_plus_uconst: 13)
 <2><297c>: Abbrev Number: 13 (DW_TAG_member)
  <297d>     DW_AT_name        : bIsTx
  <2983>     DW_AT_decl_file   : 12
  <2984>     DW_AT_decl_line   : 287
  <2986>     DW_AT_type        : <160>
  <298a>     DW_AT_data_member_location: 2 byte block: 23 e     (DW_OP_plus_uconst: 14)
 <2><298d>: Abbrev Number: 13 (DW_TAG_member)
  <298e>     DW_AT_name        : bIsHalted
  <2998>     DW_AT_decl_file   : 12
  <2999>     DW_AT_decl_line   : 288
  <299b>     DW_AT_type        : <160>
  <299f>     DW_AT_data_member_location: 2 byte block: 23 f     (DW_OP_plus_uconst: 15)
 <2><29a2>: Abbrev Number: 23 (DW_TAG_member)
  <29a3>     DW_AT_name        : (indirect string, offset: 0xaf): bIsClaimed
  <29a7>     DW_AT_decl_file   : 12
  <29a8>     DW_AT_decl_line   : 289
  <29aa>     DW_AT_type        : <160>
  <29ae>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><29b1>: Abbrev Number: 13 (DW_TAG_member)
  <29b2>     DW_AT_name        : bRxClaimed
  <29bd>     DW_AT_decl_file   : 12
  <29be>     DW_AT_decl_line   : 290
  <29c0>     DW_AT_type        : <160>
  <29c4>     DW_AT_data_member_location: 2 byte block: 23 11    (DW_OP_plus_uconst: 17)
 <2><29c7>: Abbrev Number: 13 (DW_TAG_member)
  <29c8>     DW_AT_name        : bStopTx
  <29d0>     DW_AT_decl_file   : 12
  <29d1>     DW_AT_decl_line   : 291
  <29d3>     DW_AT_type        : <160>
  <29d7>     DW_AT_data_member_location: 2 byte block: 23 12    (DW_OP_plus_uconst: 18)
 <2><29da>: Abbrev Number: 13 (DW_TAG_member)
  <29db>     DW_AT_name        : bIsRxHalted
  <29e7>     DW_AT_decl_file   : 12
  <29e8>     DW_AT_decl_line   : 292
  <29ea>     DW_AT_type        : <160>
  <29ee>     DW_AT_data_member_location: 2 byte block: 23 13    (DW_OP_plus_uconst: 19)
 <2><29f1>: Abbrev Number: 23 (DW_TAG_member)
  <29f2>     DW_AT_name        : (indirect string, offset: 0x8f): wPacketSize
  <29f6>     DW_AT_decl_file   : 12
  <29f7>     DW_AT_decl_line   : 293
  <29f9>     DW_AT_type        : <16f>
  <29fd>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><2a00>: Abbrev Number: 13 (DW_TAG_member)
  <2a01>     DW_AT_name        : wRxPacketSize
  <2a0f>     DW_AT_decl_file   : 12
  <2a10>     DW_AT_decl_line   : 294
  <2a12>     DW_AT_type        : <16f>
  <2a16>     DW_AT_data_member_location: 2 byte block: 23 16    (DW_OP_plus_uconst: 22)
 <2><2a19>: Abbrev Number: 13 (DW_TAG_member)
  <2a1a>     DW_AT_name        : dwTxSize
  <2a23>     DW_AT_decl_file   : 12
  <2a24>     DW_AT_decl_line   : 295
  <2a26>     DW_AT_type        : <18e>
  <2a2a>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><2a2d>: Abbrev Number: 13 (DW_TAG_member)
  <2a2e>     DW_AT_name        : pTxIrp
  <2a35>     DW_AT_decl_file   : 12
  <2a36>     DW_AT_decl_line   : 297
  <2a38>     DW_AT_type        : <f1>
  <2a3c>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><2a3f>: Abbrev Number: 13 (DW_TAG_member)
  <2a40>     DW_AT_name        : TxIrpList
  <2a4a>     DW_AT_decl_file   : 12
  <2a4b>     DW_AT_decl_line   : 298
  <2a4d>     DW_AT_type        : <2100>
  <2a51>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><2a54>: Abbrev Number: 13 (DW_TAG_member)
  <2a55>     DW_AT_name        : dwTxOffset
  <2a60>     DW_AT_decl_file   : 12
  <2a61>     DW_AT_decl_line   : 299
  <2a63>     DW_AT_type        : <18e>
  <2a67>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><2a6a>: Abbrev Number: 13 (DW_TAG_member)
  <2a6b>     DW_AT_name        : pRxIrp
  <2a72>     DW_AT_decl_file   : 12
  <2a73>     DW_AT_decl_line   : 300
  <2a75>     DW_AT_type        : <f1>
  <2a79>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><2a7c>: Abbrev Number: 13 (DW_TAG_member)
  <2a7d>     DW_AT_name        : RxIrpList
  <2a87>     DW_AT_decl_file   : 12
  <2a88>     DW_AT_decl_line   : 301
  <2a8a>     DW_AT_type        : <2100>
  <2a8e>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><2a91>: Abbrev Number: 13 (DW_TAG_member)
  <2a92>     DW_AT_name        : dwRxOffset
  <2a9d>     DW_AT_decl_file   : 12
  <2a9e>     DW_AT_decl_line   : 302
  <2aa0>     DW_AT_type        : <18e>
  <2aa4>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><2aa7>: Abbrev Number: 13 (DW_TAG_member)
  <2aa8>     DW_AT_name        : pDmaChannel
  <2ab4>     DW_AT_decl_file   : 12
  <2ab5>     DW_AT_decl_line   : 308
  <2ab7>     DW_AT_type        : <242e>
  <2abb>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><2abe>: Abbrev Number: 13 (DW_TAG_member)
  <2abf>     DW_AT_name        : pRxDmaChannel
  <2acd>     DW_AT_decl_file   : 12
  <2ace>     DW_AT_decl_line   : 309
  <2ad0>     DW_AT_type        : <242e>
  <2ad4>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><2ad7>: Abbrev Number: 13 (DW_TAG_member)
  <2ad8>     DW_AT_name        : wDmaBufferCount
  <2ae8>     DW_AT_decl_file   : 12
  <2ae9>     DW_AT_decl_line   : 310
  <2aeb>     DW_AT_type        : <16f>
  <2aef>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><2af2>: Abbrev Number: 13 (DW_TAG_member)
  <2af3>     DW_AT_name        : wRxDmaBufferCount
  <2b05>     DW_AT_decl_file   : 12
  <2b06>     DW_AT_decl_line   : 311
  <2b08>     DW_AT_type        : <16f>
  <2b0c>     DW_AT_data_member_location: 2 byte block: 23 4e    (DW_OP_plus_uconst: 78)
 <2><2b0f>: Abbrev Number: 13 (DW_TAG_member)
  <2b10>     DW_AT_name        : bDmaTx
  <2b17>     DW_AT_decl_file   : 12
  <2b18>     DW_AT_decl_line   : 312
  <2b1a>     DW_AT_type        : <160>
  <2b1e>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><2b21>: Abbrev Number: 13 (DW_TAG_member)
  <2b22>     DW_AT_name        : bDmaResidual
  <2b2f>     DW_AT_decl_file   : 12
  <2b30>     DW_AT_decl_line   : 313
  <2b32>     DW_AT_type        : <160>
  <2b36>     DW_AT_data_member_location: 2 byte block: 23 51    (DW_OP_plus_uconst: 81)
 <2><2b39>: Abbrev Number: 23 (DW_TAG_member)
  <2b3a>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <2b3e>     DW_AT_decl_file   : 12
  <2b3f>     DW_AT_decl_line   : 316
  <2b41>     DW_AT_type        : <f1>
  <2b45>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <2><2b48>: Abbrev Number: 13 (DW_TAG_member)
  <2b49>     DW_AT_name        : dwWaitFrameCount
  <2b5a>     DW_AT_decl_file   : 12
  <2b5b>     DW_AT_decl_line   : 317
  <2b5d>     DW_AT_type        : <18e>
  <2b61>     DW_AT_data_member_location: 2 byte block: 23 58    (DW_OP_plus_uconst: 88)
 <2><2b64>: Abbrev Number: 13 (DW_TAG_member)
  <2b65>     DW_AT_name        : bTxFifoSize
  <2b71>     DW_AT_decl_file   : 12
  <2b72>     DW_AT_decl_line   : 320
  <2b74>     DW_AT_type        : <160>
  <2b78>     DW_AT_data_member_location: 2 byte block: 23 5c    (DW_OP_plus_uconst: 92)
 <2><2b7b>: Abbrev Number: 13 (DW_TAG_member)
  <2b7c>     DW_AT_name        : bRxFifoSize
  <2b88>     DW_AT_decl_file   : 12
  <2b89>     DW_AT_decl_line   : 321
  <2b8b>     DW_AT_type        : <160>
  <2b8f>     DW_AT_data_member_location: 2 byte block: 23 5d    (DW_OP_plus_uconst: 93)
 <2><2b92>: Abbrev Number: 13 (DW_TAG_member)
  <2b93>     DW_AT_name        : bIsrResidual
  <2ba0>     DW_AT_decl_file   : 12
  <2ba1>     DW_AT_decl_line   : 323
  <2ba3>     DW_AT_type        : <160>
  <2ba7>     DW_AT_data_member_location: 2 byte block: 23 5e    (DW_OP_plus_uconst: 94)
 <2><2baa>: Abbrev Number: 13 (DW_TAG_member)
  <2bab>     DW_AT_name        : bBsrResidual
  <2bb8>     DW_AT_decl_file   : 12
  <2bb9>     DW_AT_decl_line   : 324
  <2bbb>     DW_AT_type        : <160>
  <2bbf>     DW_AT_data_member_location: 2 byte block: 23 5f    (DW_OP_plus_uconst: 95)
 <1><2bc3>: Abbrev Number: 17 (DW_TAG_typedef)
  <2bc4>     DW_AT_name        : MGC_EndpointResource
  <2bd9>     DW_AT_decl_file   : 12
  <2bda>     DW_AT_decl_line   : 325
  <2bdc>     DW_AT_type        : <2874>
 <1><2be0>: Abbrev Number: 18 (DW_TAG_structure_type)
  <2be1>     DW_AT_name        : _MGC_Pipe
  <2beb>     DW_AT_byte_size   : 36
  <2bec>     DW_AT_decl_file   : 12
  <2bed>     DW_AT_decl_line   : 344
  <2bef>     DW_AT_sibling     : <2ca4>
 <2><2bf3>: Abbrev Number: 13 (DW_TAG_member)
  <2bf4>     DW_AT_name        : hSession
  <2bfd>     DW_AT_decl_file   : 12
  <2bfe>     DW_AT_decl_line   : 345
  <2c00>     DW_AT_type        : <f1>
  <2c04>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><2c07>: Abbrev Number: 23 (DW_TAG_member)
  <2c08>     DW_AT_name        : (indirect string, offset: 0x85): pLocalEnd
  <2c0c>     DW_AT_decl_file   : 12
  <2c0d>     DW_AT_decl_line   : 346
  <2c0f>     DW_AT_type        : <2ca4>
  <2c13>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2c16>: Abbrev Number: 23 (DW_TAG_member)
  <2c17>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <2c1b>     DW_AT_decl_file   : 12
  <2c1c>     DW_AT_decl_line   : 347
  <2c1e>     DW_AT_type        : <3559>
  <2c22>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><2c25>: Abbrev Number: 13 (DW_TAG_member)
  <2c26>     DW_AT_name        : pSlot
  <2c2c>     DW_AT_decl_file   : 12
  <2c2d>     DW_AT_decl_line   : 348
  <2c2f>     DW_AT_type        : <3589>
  <2c33>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2c36>: Abbrev Number: 23 (DW_TAG_member)
  <2c37>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <2c3b>     DW_AT_decl_file   : 12
  <2c3c>     DW_AT_decl_line   : 349
  <2c3e>     DW_AT_type        : <1561>
  <2c42>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><2c45>: Abbrev Number: 23 (DW_TAG_member)
  <2c46>     DW_AT_name        : (indirect string, offset: 0x11d): wMaxPacketSize
  <2c4a>     DW_AT_decl_file   : 12
  <2c4b>     DW_AT_decl_line   : 350
  <2c4d>     DW_AT_type        : <16f>
  <2c51>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><2c54>: Abbrev Number: 13 (DW_TAG_member)
  <2c55>     DW_AT_name        : wInterval
  <2c5f>     DW_AT_decl_file   : 12
  <2c60>     DW_AT_decl_line   : 351
  <2c62>     DW_AT_type        : <16f>
  <2c66>     DW_AT_data_member_location: 2 byte block: 23 16    (DW_OP_plus_uconst: 22)
 <2><2c69>: Abbrev Number: 13 (DW_TAG_member)
  <2c6a>     DW_AT_name        : dwMaxBusTime
  <2c77>     DW_AT_decl_file   : 12
  <2c78>     DW_AT_decl_line   : 352
  <2c7a>     DW_AT_type        : <18e>
  <2c7e>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><2c81>: Abbrev Number: 13 (DW_TAG_member)
  <2c82>     DW_AT_name        : bmFlags
  <2c8a>     DW_AT_decl_file   : 12
  <2c8b>     DW_AT_decl_line   : 353
  <2c8d>     DW_AT_type        : <18e>
  <2c91>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><2c94>: Abbrev Number: 23 (DW_TAG_member)
  <2c95>     DW_AT_name        : (indirect string, offset: 0x5a): bTrafficType
  <2c99>     DW_AT_decl_file   : 12
  <2c9a>     DW_AT_decl_line   : 354
  <2c9c>     DW_AT_type        : <160>
  <2ca0>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <1><2ca4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2ca5>     DW_AT_byte_size   : 4
  <2ca6>     DW_AT_type        : <2bc3>
 <1><2caa>: Abbrev Number: 26 (DW_TAG_structure_type)
  <2cab>     DW_AT_name        : _MGC_Port
  <2cb5>     DW_AT_byte_size   : 608
  <2cb7>     DW_AT_decl_file   : 12
  <2cb8>     DW_AT_decl_line   : 241
  <2cb9>     DW_AT_sibling     : <3559>
 <2><2cbd>: Abbrev Number: 23 (DW_TAG_member)
  <2cbe>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <2cc2>     DW_AT_decl_file   : 12
  <2cc3>     DW_AT_decl_line   : 866
  <2cc5>     DW_AT_type        : <3f67>
  <2cc9>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><2ccc>: Abbrev Number: 13 (DW_TAG_member)
  <2ccd>     DW_AT_name        : pInterfacePort
  <2cdc>     DW_AT_decl_file   : 12
  <2cdd>     DW_AT_decl_line   : 867
  <2cdf>     DW_AT_type        : <14d1>
  <2ce3>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2ce6>: Abbrev Number: 23 (DW_TAG_member)
  <2ce7>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <2ceb>     DW_AT_decl_file   : 12
  <2cec>     DW_AT_decl_line   : 868
  <2cee>     DW_AT_type        : <f1>
  <2cf2>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><2cf5>: Abbrev Number: 13 (DW_TAG_member)
  <2cf6>     DW_AT_name        : LocalEnds
  <2d00>     DW_AT_decl_file   : 12
  <2d01>     DW_AT_decl_line   : 869
  <2d03>     DW_AT_type        : <209c>
  <2d07>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2d0a>: Abbrev Number: 13 (DW_TAG_member)
  <2d0b>     DW_AT_name        : bWantHighSpeed
  <2d1a>     DW_AT_decl_file   : 12
  <2d1b>     DW_AT_decl_line   : 872
  <2d1d>     DW_AT_type        : <160>
  <2d21>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><2d24>: Abbrev Number: 13 (DW_TAG_member)
  <2d25>     DW_AT_name        : bWantReset
  <2d30>     DW_AT_decl_file   : 12
  <2d31>     DW_AT_decl_line   : 873
  <2d33>     DW_AT_type        : <160>
  <2d37>     DW_AT_data_member_location: 2 byte block: 23 25    (DW_OP_plus_uconst: 37)
 <2><2d3a>: Abbrev Number: 13 (DW_TAG_member)
  <2d3b>     DW_AT_name        : bWantSuspend
  <2d48>     DW_AT_decl_file   : 12
  <2d49>     DW_AT_decl_line   : 874
  <2d4b>     DW_AT_type        : <160>
  <2d4f>     DW_AT_data_member_location: 2 byte block: 23 26    (DW_OP_plus_uconst: 38)
 <2><2d52>: Abbrev Number: 13 (DW_TAG_member)
  <2d53>     DW_AT_name        : bWantResume
  <2d5f>     DW_AT_decl_file   : 12
  <2d60>     DW_AT_decl_line   : 875
  <2d62>     DW_AT_type        : <160>
  <2d66>     DW_AT_data_member_location: 2 byte block: 23 27    (DW_OP_plus_uconst: 39)
 <2><2d69>: Abbrev Number: 13 (DW_TAG_member)
  <2d6a>     DW_AT_name        : bFuncAddr
  <2d74>     DW_AT_decl_file   : 12
  <2d75>     DW_AT_decl_line   : 876
  <2d77>     DW_AT_type        : <160>
  <2d7b>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><2d7e>: Abbrev Number: 13 (DW_TAG_member)
  <2d7f>     DW_AT_name        : bWantSession
  <2d8c>     DW_AT_decl_file   : 12
  <2d8d>     DW_AT_decl_line   : 877
  <2d8f>     DW_AT_type        : <160>
  <2d93>     DW_AT_data_member_location: 2 byte block: 23 29    (DW_OP_plus_uconst: 41)
 <2><2d96>: Abbrev Number: 13 (DW_TAG_member)
  <2d97>     DW_AT_name        : bWantHost
  <2da1>     DW_AT_decl_file   : 12
  <2da2>     DW_AT_decl_line   : 878
  <2da4>     DW_AT_type        : <160>
  <2da8>     DW_AT_data_member_location: 2 byte block: 23 2a    (DW_OP_plus_uconst: 42)
 <2><2dab>: Abbrev Number: 13 (DW_TAG_member)
  <2dac>     DW_AT_name        : bWantTestMode
  <2dba>     DW_AT_decl_file   : 12
  <2dbb>     DW_AT_decl_line   : 879
  <2dbd>     DW_AT_type        : <160>
  <2dc1>     DW_AT_data_member_location: 2 byte block: 23 2b    (DW_OP_plus_uconst: 43)
 <2><2dc4>: Abbrev Number: 13 (DW_TAG_member)
  <2dc5>     DW_AT_name        : bTestMode
  <2dcf>     DW_AT_decl_file   : 12
  <2dd0>     DW_AT_decl_line   : 880
  <2dd2>     DW_AT_type        : <160>
  <2dd6>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><2dd9>: Abbrev Number: 13 (DW_TAG_member)
  <2dda>     DW_AT_name        : bSessionActive
  <2de9>     DW_AT_decl_file   : 12
  <2dea>     DW_AT_decl_line   : 883
  <2dec>     DW_AT_type        : <160>
  <2df0>     DW_AT_data_member_location: 2 byte block: 23 2d    (DW_OP_plus_uconst: 45)
 <2><2df3>: Abbrev Number: 13 (DW_TAG_member)
  <2df4>     DW_AT_name        : bHasBulkSplit
  <2e02>     DW_AT_decl_file   : 12
  <2e03>     DW_AT_decl_line   : 886
  <2e05>     DW_AT_type        : <160>
  <2e09>     DW_AT_data_member_location: 2 byte block: 23 2e    (DW_OP_plus_uconst: 46)
 <2><2e0c>: Abbrev Number: 13 (DW_TAG_member)
  <2e0d>     DW_AT_name        : bHasBulkCombine
  <2e1d>     DW_AT_decl_file   : 12
  <2e1e>     DW_AT_decl_line   : 887
  <2e20>     DW_AT_type        : <160>
  <2e24>     DW_AT_data_member_location: 2 byte block: 23 2f    (DW_OP_plus_uconst: 47)
 <2><2e27>: Abbrev Number: 13 (DW_TAG_member)
  <2e28>     DW_AT_name        : bHasHbIsoTx
  <2e34>     DW_AT_decl_file   : 12
  <2e35>     DW_AT_decl_line   : 888
  <2e37>     DW_AT_type        : <160>
  <2e3b>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><2e3e>: Abbrev Number: 13 (DW_TAG_member)
  <2e3f>     DW_AT_name        : bHasHbIsoRx
  <2e4b>     DW_AT_decl_file   : 12
  <2e4c>     DW_AT_decl_line   : 889
  <2e4e>     DW_AT_type        : <160>
  <2e52>     DW_AT_data_member_location: 2 byte block: 23 31    (DW_OP_plus_uconst: 49)
 <2><2e55>: Abbrev Number: 13 (DW_TAG_member)
  <2e56>     DW_AT_name        : bIsMultipoint
  <2e64>     DW_AT_decl_file   : 12
  <2e65>     DW_AT_decl_line   : 890
  <2e67>     DW_AT_type        : <160>
  <2e6b>     DW_AT_data_member_location: 2 byte block: 23 32    (DW_OP_plus_uconst: 50)
 <2><2e6e>: Abbrev Number: 13 (DW_TAG_member)
  <2e6f>     DW_AT_name        : bEndCount
  <2e79>     DW_AT_decl_file   : 12
  <2e7a>     DW_AT_decl_line   : 891
  <2e7c>     DW_AT_type        : <160>
  <2e80>     DW_AT_data_member_location: 2 byte block: 23 33    (DW_OP_plus_uconst: 51)
 <2><2e83>: Abbrev Number: 13 (DW_TAG_member)
  <2e84>     DW_AT_name        : dwFrame
  <2e8c>     DW_AT_decl_file   : 12
  <2e8d>     DW_AT_decl_line   : 894
  <2e8f>     DW_AT_type        : <18e>
  <2e93>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><2e96>: Abbrev Number: 13 (DW_TAG_member)
  <2e97>     DW_AT_name        : bIsHighSpeed
  <2ea4>     DW_AT_decl_file   : 12
  <2ea5>     DW_AT_decl_line   : 895
  <2ea7>     DW_AT_type        : <160>
  <2eab>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <2><2eae>: Abbrev Number: 13 (DW_TAG_member)
  <2eaf>     DW_AT_name        : bIsLowSpeed
  <2ebb>     DW_AT_decl_file   : 12
  <2ebc>     DW_AT_decl_line   : 896
  <2ebe>     DW_AT_type        : <160>
  <2ec2>     DW_AT_data_member_location: 2 byte block: 23 39    (DW_OP_plus_uconst: 57)
 <2><2ec5>: Abbrev Number: 23 (DW_TAG_member)
  <2ec6>     DW_AT_name        : (indirect string, offset: 0x13d): bIsReset
  <2eca>     DW_AT_decl_file   : 12
  <2ecb>     DW_AT_decl_line   : 897
  <2ecd>     DW_AT_type        : <160>
  <2ed1>     DW_AT_data_member_location: 2 byte block: 23 3a    (DW_OP_plus_uconst: 58)
 <2><2ed4>: Abbrev Number: 13 (DW_TAG_member)
  <2ed5>     DW_AT_name        : bIsSuspend
  <2ee0>     DW_AT_decl_file   : 12
  <2ee1>     DW_AT_decl_line   : 898
  <2ee3>     DW_AT_type        : <160>
  <2ee7>     DW_AT_data_member_location: 2 byte block: 23 3b    (DW_OP_plus_uconst: 59)
 <2><2eea>: Abbrev Number: 13 (DW_TAG_member)
  <2eeb>     DW_AT_name        : bIsResume
  <2ef5>     DW_AT_decl_file   : 12
  <2ef6>     DW_AT_decl_line   : 899
  <2ef8>     DW_AT_type        : <160>
  <2efc>     DW_AT_data_member_location: 2 byte block: 23 3c    (DW_OP_plus_uconst: 60)
 <2><2eff>: Abbrev Number: 13 (DW_TAG_member)
  <2f00>     DW_AT_name        : bIsSession
  <2f0b>     DW_AT_decl_file   : 12
  <2f0c>     DW_AT_decl_line   : 900
  <2f0e>     DW_AT_type        : <160>
  <2f12>     DW_AT_data_member_location: 2 byte block: 23 3d    (DW_OP_plus_uconst: 61)
 <2><2f15>: Abbrev Number: 13 (DW_TAG_member)
  <2f16>     DW_AT_name        : bIsHost
  <2f1e>     DW_AT_decl_file   : 12
  <2f1f>     DW_AT_decl_line   : 901
  <2f21>     DW_AT_type        : <160>
  <2f25>     DW_AT_data_member_location: 2 byte block: 23 3e    (DW_OP_plus_uconst: 62)
 <2><2f28>: Abbrev Number: 13 (DW_TAG_member)
  <2f29>     DW_AT_name        : bConnectorId
  <2f36>     DW_AT_decl_file   : 12
  <2f37>     DW_AT_decl_line   : 902
  <2f39>     DW_AT_type        : <160>
  <2f3d>     DW_AT_data_member_location: 2 byte block: 23 3f    (DW_OP_plus_uconst: 63)
 <2><2f40>: Abbrev Number: 13 (DW_TAG_member)
  <2f41>     DW_AT_name        : bBusVoltage
  <2f4d>     DW_AT_decl_file   : 12
  <2f4e>     DW_AT_decl_line   : 903
  <2f50>     DW_AT_type        : <160>
  <2f54>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><2f57>: Abbrev Number: 13 (DW_TAG_member)
  <2f58>     DW_AT_name        : bEnd0Status
  <2f64>     DW_AT_decl_file   : 12
  <2f65>     DW_AT_decl_line   : 906
  <2f67>     DW_AT_type        : <160>
  <2f6b>     DW_AT_data_member_location: 2 byte block: 23 41    (DW_OP_plus_uconst: 65)
 <2><2f6e>: Abbrev Number: 13 (DW_TAG_member)
  <2f6f>     DW_AT_name        : wEndMask
  <2f78>     DW_AT_decl_file   : 12
  <2f79>     DW_AT_decl_line   : 907
  <2f7b>     DW_AT_type        : <16f>
  <2f7f>     DW_AT_data_member_location: 2 byte block: 23 42    (DW_OP_plus_uconst: 66)
 <2><2f82>: Abbrev Number: 13 (DW_TAG_member)
  <2f83>     DW_AT_name        : bDmaCompleted
  <2f91>     DW_AT_decl_file   : 12
  <2f92>     DW_AT_decl_line   : 908
  <2f94>     DW_AT_type        : <160>
  <2f98>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><2f9b>: Abbrev Number: 13 (DW_TAG_member)
  <2f9c>     DW_AT_name        : bVbusErrorRetries
  <2fae>     DW_AT_decl_file   : 12
  <2faf>     DW_AT_decl_line   : 909
  <2fb1>     DW_AT_type        : <160>
  <2fb5>     DW_AT_data_member_location: 2 byte block: 23 45    (DW_OP_plus_uconst: 69)
 <2><2fb8>: Abbrev Number: 13 (DW_TAG_member)
  <2fb9>     DW_AT_name        : bHasDisablePing
  <2fc9>     DW_AT_decl_file   : 12
  <2fca>     DW_AT_decl_line   : 910
  <2fcc>     DW_AT_type        : <160>
  <2fd0>     DW_AT_data_member_location: 2 byte block: 23 46    (DW_OP_plus_uconst: 70)
 <2><2fd3>: Abbrev Number: 13 (DW_TAG_member)
  <2fd4>     DW_AT_name        : bBabbleRetries
  <2fe3>     DW_AT_decl_file   : 12
  <2fe4>     DW_AT_decl_line   : 911
  <2fe6>     DW_AT_type        : <160>
  <2fea>     DW_AT_data_member_location: 2 byte block: 23 47    (DW_OP_plus_uconst: 71)
 <2><2fed>: Abbrev Number: 13 (DW_TAG_member)
  <2fee>     DW_AT_name        : bHasRqPktCount
  <2ffd>     DW_AT_decl_file   : 12
  <2ffe>     DW_AT_decl_line   : 912
  <3000>     DW_AT_type        : <160>
  <3004>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><3007>: Abbrev Number: 13 (DW_TAG_member)
  <3008>     DW_AT_name        : bPad1
  <300e>     DW_AT_decl_file   : 12
  <300f>     DW_AT_decl_line   : 913
  <3011>     DW_AT_type        : <160>
  <3015>     DW_AT_data_member_location: 2 byte block: 23 49    (DW_OP_plus_uconst: 73)
 <2><3018>: Abbrev Number: 13 (DW_TAG_member)
  <3019>     DW_AT_name        : bPad2
  <301f>     DW_AT_decl_file   : 12
  <3020>     DW_AT_decl_line   : 914
  <3022>     DW_AT_type        : <160>
  <3026>     DW_AT_data_member_location: 2 byte block: 23 4a    (DW_OP_plus_uconst: 74)
 <2><3029>: Abbrev Number: 13 (DW_TAG_member)
  <302a>     DW_AT_name        : bPad3
  <3030>     DW_AT_decl_file   : 12
  <3031>     DW_AT_decl_line   : 915
  <3033>     DW_AT_type        : <160>
  <3037>     DW_AT_data_member_location: 2 byte block: 23 4b    (DW_OP_plus_uconst: 75)
 <2><303a>: Abbrev Number: 13 (DW_TAG_member)
  <303b>     DW_AT_name        : bOtgState
  <3045>     DW_AT_decl_file   : 12
  <3046>     DW_AT_decl_line   : 918
  <3048>     DW_AT_type        : <160>
  <304c>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><304f>: Abbrev Number: 13 (DW_TAG_member)
  <3050>     DW_AT_name        : bRelinquish
  <305c>     DW_AT_decl_file   : 12
  <305d>     DW_AT_decl_line   : 919
  <305f>     DW_AT_type        : <160>
  <3063>     DW_AT_data_member_location: 2 byte block: 23 4d    (DW_OP_plus_uconst: 77)
 <2><3066>: Abbrev Number: 13 (DW_TAG_member)
  <3067>     DW_AT_name        : bIsHnpSupported
  <3077>     DW_AT_decl_file   : 12
  <3078>     DW_AT_decl_line   : 920
  <307a>     DW_AT_type        : <160>
  <307e>     DW_AT_data_member_location: 2 byte block: 23 4e    (DW_OP_plus_uconst: 78)
 <2><3081>: Abbrev Number: 13 (DW_TAG_member)
  <3082>     DW_AT_name        : bIsHnpAllowed
  <3090>     DW_AT_decl_file   : 12
  <3091>     DW_AT_decl_line   : 921
  <3093>     DW_AT_type        : <160>
  <3097>     DW_AT_data_member_location: 2 byte block: 23 4f    (DW_OP_plus_uconst: 79)
 <2><309a>: Abbrev Number: 23 (DW_TAG_member)
  <309b>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <309f>     DW_AT_decl_file   : 12
  <30a0>     DW_AT_decl_line   : 922
  <30a2>     DW_AT_type        : <3f6d>
  <30a6>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><30a9>: Abbrev Number: 13 (DW_TAG_member)
  <30aa>     DW_AT_name        : EnumerationData
  <30ba>     DW_AT_decl_file   : 12
  <30bb>     DW_AT_decl_line   : 926
  <30bd>     DW_AT_type        : <3f4b>
  <30c1>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <2><30c4>: Abbrev Number: 23 (DW_TAG_member)
  <30c5>     DW_AT_name        : (indirect string, offset: 0x0): pHostClient
  <30c9>     DW_AT_decl_file   : 12
  <30ca>     DW_AT_decl_line   : 927
  <30cc>     DW_AT_type        : <3f73>
  <30d0>     DW_AT_data_member_location: 3 byte block: 23 ac 3  (DW_OP_plus_uconst: 428)
 <2><30d4>: Abbrev Number: 13 (DW_TAG_member)
  <30d5>     DW_AT_name        : pfDriverTimerExpired
  <30ea>     DW_AT_decl_file   : 12
  <30eb>     DW_AT_decl_line   : 928
  <30ed>     DW_AT_type        : <1ce4>
  <30f1>     DW_AT_data_member_location: 3 byte block: 23 b0 3  (DW_OP_plus_uconst: 432)
 <2><30f5>: Abbrev Number: 13 (DW_TAG_member)
  <30f6>     DW_AT_name        : pDriverTimerData
  <3107>     DW_AT_decl_file   : 12
  <3108>     DW_AT_decl_line   : 929
  <310a>     DW_AT_type        : <f1>
  <310e>     DW_AT_data_member_location: 3 byte block: 23 b4 3  (DW_OP_plus_uconst: 436)
 <2><3112>: Abbrev Number: 13 (DW_TAG_member)
  <3113>     DW_AT_name        : pRootDevice
  <311f>     DW_AT_decl_file   : 12
  <3120>     DW_AT_decl_line   : 930
  <3122>     DW_AT_type        : <1ca0>
  <3126>     DW_AT_data_member_location: 3 byte block: 23 b8 3  (DW_OP_plus_uconst: 440)
 <2><312a>: Abbrev Number: 13 (DW_TAG_member)
  <312b>     DW_AT_name        : ConnectedDeviceList
  <313f>     DW_AT_decl_file   : 12
  <3140>     DW_AT_decl_line   : 931
  <3142>     DW_AT_type        : <2100>
  <3146>     DW_AT_data_member_location: 3 byte block: 23 bc 3  (DW_OP_plus_uconst: 444)
 <2><314a>: Abbrev Number: 13 (DW_TAG_member)
  <314b>     DW_AT_name        : Schedule
  <3154>     DW_AT_decl_file   : 12
  <3155>     DW_AT_decl_line   : 932
  <3157>     DW_AT_type        : <35f0>
  <315b>     DW_AT_data_member_location: 3 byte block: 23 c8 3  (DW_OP_plus_uconst: 456)
 <2><315f>: Abbrev Number: 13 (DW_TAG_member)
  <3160>     DW_AT_name        : wHostMaxPower
  <316e>     DW_AT_decl_file   : 12
  <316f>     DW_AT_decl_line   : 933
  <3171>     DW_AT_type        : <16f>
  <3175>     DW_AT_data_member_location: 3 byte block: 23 dc 3  (DW_OP_plus_uconst: 476)
 <2><3179>: Abbrev Number: 13 (DW_TAG_member)
  <317a>     DW_AT_name        : wHostUsedPower
  <3189>     DW_AT_decl_file   : 12
  <318a>     DW_AT_decl_line   : 934
  <318c>     DW_AT_type        : <16f>
  <3190>     DW_AT_data_member_location: 3 byte block: 23 de 3  (DW_OP_plus_uconst: 478)
 <2><3194>: Abbrev Number: 23 (DW_TAG_member)
  <3195>     DW_AT_name        : (indirect string, offset: 0x6e): pFunctionClient
  <3199>     DW_AT_decl_file   : 12
  <319a>     DW_AT_decl_line   : 938
  <319c>     DW_AT_type        : <3f79>
  <31a0>     DW_AT_data_member_location: 3 byte block: 23 e0 3  (DW_OP_plus_uconst: 480)
 <2><31a4>: Abbrev Number: 23 (DW_TAG_member)
  <31a5>     DW_AT_name        : (indirect string, offset: 0x9b): apConfigDescriptors
  <31a9>     DW_AT_decl_file   : 12
  <31aa>     DW_AT_decl_line   : 939
  <31ac>     DW_AT_type        : <3f7f>
  <31b0>     DW_AT_data_member_location: 3 byte block: 23 e4 3  (DW_OP_plus_uconst: 484)
 <2><31b4>: Abbrev Number: 13 (DW_TAG_member)
  <31b5>     DW_AT_name        : pCurrentConfig
  <31c4>     DW_AT_decl_file   : 12
  <31c5>     DW_AT_decl_line   : 940
  <31c7>     DW_AT_type        : <3f85>
  <31cb>     DW_AT_data_member_location: 3 byte block: 23 e8 3  (DW_OP_plus_uconst: 488)
 <2><31cf>: Abbrev Number: 13 (DW_TAG_member)
  <31d0>     DW_AT_name        : apPipe
  <31d7>     DW_AT_decl_file   : 12
  <31d8>     DW_AT_decl_line   : 941
  <31da>     DW_AT_type        : <3f90>
  <31de>     DW_AT_data_member_location: 3 byte block: 23 ec 3  (DW_OP_plus_uconst: 492)
 <2><31e2>: Abbrev Number: 13 (DW_TAG_member)
  <31e3>     DW_AT_name        : bUsbState
  <31ed>     DW_AT_decl_file   : 12
  <31ee>     DW_AT_decl_line   : 942
  <31f0>     DW_AT_type        : <160>
  <31f4>     DW_AT_data_member_location: 3 byte block: 23 f0 3  (DW_OP_plus_uconst: 496)
 <2><31f8>: Abbrev Number: 13 (DW_TAG_member)
  <31f9>     DW_AT_name        : bMaxFunctionEnds
  <320a>     DW_AT_decl_file   : 12
  <320b>     DW_AT_decl_line   : 943
  <320d>     DW_AT_type        : <160>
  <3211>     DW_AT_data_member_location: 3 byte block: 23 f1 3  (DW_OP_plus_uconst: 497)
 <2><3215>: Abbrev Number: 13 (DW_TAG_member)
  <3216>     DW_AT_name        : bCanWakeup
  <3221>     DW_AT_decl_file   : 12
  <3222>     DW_AT_decl_line   : 944
  <3224>     DW_AT_type        : <160>
  <3228>     DW_AT_data_member_location: 3 byte block: 23 f2 3  (DW_OP_plus_uconst: 498)
 <2><322c>: Abbrev Number: 13 (DW_TAG_member)
  <322d>     DW_AT_name        : bSetAddress
  <3239>     DW_AT_decl_file   : 12
  <323a>     DW_AT_decl_line   : 945
  <323c>     DW_AT_type        : <160>
  <3240>     DW_AT_data_member_location: 3 byte block: 23 f3 3  (DW_OP_plus_uconst: 499)
 <2><3244>: Abbrev Number: 13 (DW_TAG_member)
  <3245>     DW_AT_name        : bConfigValue
  <3252>     DW_AT_decl_file   : 12
  <3253>     DW_AT_decl_line   : 946
  <3255>     DW_AT_type        : <160>
  <3259>     DW_AT_data_member_location: 3 byte block: 23 f4 3  (DW_OP_plus_uconst: 500)
 <2><325d>: Abbrev Number: 13 (DW_TAG_member)
  <325e>     DW_AT_name        : bParse
  <3265>     DW_AT_decl_file   : 12
  <3266>     DW_AT_decl_line   : 947
  <3268>     DW_AT_type        : <160>
  <326c>     DW_AT_data_member_location: 3 byte block: 23 f5 3  (DW_OP_plus_uconst: 501)
 <2><3270>: Abbrev Number: 13 (DW_TAG_member)
  <3271>     DW_AT_name        : wSetupDataIndex
  <3281>     DW_AT_decl_file   : 12
  <3282>     DW_AT_decl_line   : 948
  <3284>     DW_AT_type        : <16f>
  <3288>     DW_AT_data_member_location: 3 byte block: 23 f6 3  (DW_OP_plus_uconst: 502)
 <2><328c>: Abbrev Number: 13 (DW_TAG_member)
  <328d>     DW_AT_name        : wSetupRxIndex
  <329b>     DW_AT_decl_file   : 12
  <329c>     DW_AT_decl_line   : 949
  <329e>     DW_AT_type        : <16f>
  <32a2>     DW_AT_data_member_location: 3 byte block: 23 f8 3  (DW_OP_plus_uconst: 504)
 <2><32a6>: Abbrev Number: 13 (DW_TAG_member)
  <32a7>     DW_AT_name        : wSetupDataSize
  <32b6>     DW_AT_decl_file   : 12
  <32b7>     DW_AT_decl_line   : 950
  <32b9>     DW_AT_type        : <16f>
  <32bd>     DW_AT_data_member_location: 3 byte block: 23 fa 3  (DW_OP_plus_uconst: 506)
 <2><32c1>: Abbrev Number: 13 (DW_TAG_member)
  <32c2>     DW_AT_name        : pSetupData
  <32cd>     DW_AT_decl_file   : 12
  <32ce>     DW_AT_decl_line   : 951
  <32d0>     DW_AT_type        : <14e3>
  <32d4>     DW_AT_data_member_location: 3 byte block: 23 fc 3  (DW_OP_plus_uconst: 508)
 <2><32d8>: Abbrev Number: 13 (DW_TAG_member)
  <32d9>     DW_AT_name        : dwSequenceNumber
  <32ea>     DW_AT_decl_file   : 12
  <32eb>     DW_AT_decl_line   : 952
  <32ed>     DW_AT_type        : <18e>
  <32f1>     DW_AT_data_member_location: 3 byte block: 23 80 4  (DW_OP_plus_uconst: 512)
 <2><32f5>: Abbrev Number: 13 (DW_TAG_member)
  <32f6>     DW_AT_name        : abIfaceOffsets
  <3305>     DW_AT_decl_file   : 12
  <3306>     DW_AT_decl_line   : 953
  <3308>     DW_AT_type        : <3f3b>
  <330c>     DW_AT_data_member_location: 3 byte block: 23 84 4  (DW_OP_plus_uconst: 516)
 <2><3310>: Abbrev Number: 13 (DW_TAG_member)
  <3311>     DW_AT_name        : pDmaController
  <3320>     DW_AT_decl_file   : 12
  <3321>     DW_AT_decl_line   : 969
  <3323>     DW_AT_type        : <2753>
  <3327>     DW_AT_data_member_location: 3 byte block: 23 94 4  (DW_OP_plus_uconst: 532)
 <2><332b>: Abbrev Number: 13 (DW_TAG_member)
  <332c>     DW_AT_name        : pfReadBusState
  <333b>     DW_AT_decl_file   : 12
  <333c>     DW_AT_decl_line   : 972
  <333e>     DW_AT_type        : <3a1c>
  <3342>     DW_AT_data_member_location: 3 byte block: 23 98 4  (DW_OP_plus_uconst: 536)
 <2><3346>: Abbrev Number: 13 (DW_TAG_member)
  <3347>     DW_AT_name        : pfProgramBusState
  <3359>     DW_AT_decl_file   : 12
  <335a>     DW_AT_decl_line   : 973
  <335c>     DW_AT_type        : <3a4d>
  <3360>     DW_AT_data_member_location: 3 byte block: 23 9c 4  (DW_OP_plus_uconst: 540)
 <2><3364>: Abbrev Number: 13 (DW_TAG_member)
  <3365>     DW_AT_name        : pfResetPort
  <3371>     DW_AT_decl_file   : 12
  <3372>     DW_AT_decl_line   : 974
  <3374>     DW_AT_type        : <3a6b>
  <3378>     DW_AT_data_member_location: 3 byte block: 23 a0 4  (DW_OP_plus_uconst: 544)
 <2><337c>: Abbrev Number: 13 (DW_TAG_member)
  <337d>     DW_AT_name        : pfBindEndpoint
  <338c>     DW_AT_decl_file   : 12
  <338d>     DW_AT_decl_line   : 975
  <338f>     DW_AT_type        : <3a83>
  <3393>     DW_AT_data_member_location: 3 byte block: 23 a4 4  (DW_OP_plus_uconst: 548)
 <2><3397>: Abbrev Number: 13 (DW_TAG_member)
  <3398>     DW_AT_name        : pfProgramStartReceive
  <33ae>     DW_AT_decl_file   : 12
  <33af>     DW_AT_decl_line   : 976
  <33b1>     DW_AT_type        : <3ace>
  <33b5>     DW_AT_data_member_location: 3 byte block: 23 a8 4  (DW_OP_plus_uconst: 552)
 <2><33b9>: Abbrev Number: 13 (DW_TAG_member)
  <33ba>     DW_AT_name        : pfProgramStartTransmit
  <33d1>     DW_AT_decl_file   : 12
  <33d2>     DW_AT_decl_line   : 977
  <33d4>     DW_AT_type        : <3b1f>
  <33d8>     DW_AT_data_member_location: 3 byte block: 23 ac 4  (DW_OP_plus_uconst: 556)
 <2><33dc>: Abbrev Number: 13 (DW_TAG_member)
  <33dd>     DW_AT_name        : pfProgramFlushEndpoint
  <33f4>     DW_AT_decl_file   : 12
  <33f5>     DW_AT_decl_line   : 978
  <33f7>     DW_AT_type        : <3b6c>
  <33fb>     DW_AT_data_member_location: 3 byte block: 23 b0 4  (DW_OP_plus_uconst: 560)
 <2><33ff>: Abbrev Number: 13 (DW_TAG_member)
  <3400>     DW_AT_name        : pfProgramHaltEndpoint
  <3416>     DW_AT_decl_file   : 12
  <3417>     DW_AT_decl_line   : 979
  <3419>     DW_AT_type        : <3bb4>
  <341d>     DW_AT_data_member_location: 3 byte block: 23 b4 4  (DW_OP_plus_uconst: 564)
 <2><3421>: Abbrev Number: 13 (DW_TAG_member)
  <3422>     DW_AT_name        : pfDefaultEndResponse
  <3437>     DW_AT_decl_file   : 12
  <3438>     DW_AT_decl_line   : 980
  <343a>     DW_AT_type        : <3bd6>
  <343e>     DW_AT_data_member_location: 3 byte block: 23 b8 4  (DW_OP_plus_uconst: 568)
 <2><3442>: Abbrev Number: 13 (DW_TAG_member)
  <3443>     DW_AT_name        : pfServiceDefaultEnd
  <3457>     DW_AT_decl_file   : 12
  <3458>     DW_AT_decl_line   : 981
  <345a>     DW_AT_type        : <3c12>
  <345e>     DW_AT_data_member_location: 3 byte block: 23 bc 4  (DW_OP_plus_uconst: 572)
 <2><3462>: Abbrev Number: 13 (DW_TAG_member)
  <3463>     DW_AT_name        : pfServiceTransmitAvail
  <347a>     DW_AT_decl_file   : 12
  <347b>     DW_AT_decl_line   : 982
  <347d>     DW_AT_type        : <3c53>
  <3481>     DW_AT_data_member_location: 3 byte block: 23 c0 4  (DW_OP_plus_uconst: 576)
 <2><3485>: Abbrev Number: 13 (DW_TAG_member)
  <3486>     DW_AT_name        : pfServiceReceiveReady
  <349c>     DW_AT_decl_file   : 12
  <349d>     DW_AT_decl_line   : 983
  <349f>     DW_AT_type        : <3c96>
  <34a3>     DW_AT_data_member_location: 3 byte block: 23 c4 4  (DW_OP_plus_uconst: 580)
 <2><34a7>: Abbrev Number: 13 (DW_TAG_member)
  <34a8>     DW_AT_name        : pfLoadFifo
  <34b3>     DW_AT_decl_file   : 12
  <34b4>     DW_AT_decl_line   : 984
  <34b6>     DW_AT_type        : <3cb8>
  <34ba>     DW_AT_data_member_location: 3 byte block: 23 c8 4  (DW_OP_plus_uconst: 584)
 <2><34be>: Abbrev Number: 13 (DW_TAG_member)
  <34bf>     DW_AT_name        : pfUnloadFifo
  <34cc>     DW_AT_decl_file   : 12
  <34cd>     DW_AT_decl_line   : 985
  <34cf>     DW_AT_type        : <3cf4>
  <34d3>     DW_AT_data_member_location: 3 byte block: 23 cc 4  (DW_OP_plus_uconst: 588)
 <2><34d7>: Abbrev Number: 13 (DW_TAG_member)
  <34d8>     DW_AT_name        : pfAcceptDevice
  <34e7>     DW_AT_decl_file   : 12
  <34e8>     DW_AT_decl_line   : 986
  <34ea>     DW_AT_type        : <3d32>
  <34ee>     DW_AT_data_member_location: 3 byte block: 23 d0 4  (DW_OP_plus_uconst: 592)
 <2><34f2>: Abbrev Number: 13 (DW_TAG_member)
  <34f3>     DW_AT_name        : pfDmaChannelStatusChanged
  <350d>     DW_AT_decl_file   : 12
  <350e>     DW_AT_decl_line   : 987
  <3510>     DW_AT_type        : <26bd>
  <3514>     DW_AT_data_member_location: 3 byte block: 23 d4 4  (DW_OP_plus_uconst: 596)
 <2><3518>: Abbrev Number: 13 (DW_TAG_member)
  <3519>     DW_AT_name        : pfSetPortTestMode
  <352b>     DW_AT_decl_file   : 12
  <352c>     DW_AT_decl_line   : 988
  <352e>     DW_AT_type        : <3d6d>
  <3532>     DW_AT_data_member_location: 3 byte block: 23 d8 4  (DW_OP_plus_uconst: 600)
 <2><3536>: Abbrev Number: 13 (DW_TAG_member)
  <3537>     DW_AT_name        : pfDynamicFifoLocation
  <354d>     DW_AT_decl_file   : 12
  <354e>     DW_AT_decl_line   : 989
  <3550>     DW_AT_type        : <3da2>
  <3554>     DW_AT_data_member_location: 3 byte block: 23 dc 4  (DW_OP_plus_uconst: 604)
 <1><3559>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <355a>     DW_AT_byte_size   : 4
  <355b>     DW_AT_type        : <2caa>
 <1><355f>: Abbrev Number: 24 (DW_TAG_structure_type)
  <3560>     DW_AT_name        : _MGC_ScheduleSlot
  <3572>     DW_AT_byte_size   : 12
  <3573>     DW_AT_decl_file   : 12
  <3574>     DW_AT_decl_line   : 242
  <3575>     DW_AT_sibling     : <3589>
 <2><3579>: Abbrev Number: 23 (DW_TAG_member)
  <357a>     DW_AT_name        : (indirect string, offset: 0x188): PipeList
  <357e>     DW_AT_decl_file   : 12
  <357f>     DW_AT_decl_line   : 378
  <3581>     DW_AT_type        : <2100>
  <3585>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <1><3589>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <358a>     DW_AT_byte_size   : 4
  <358b>     DW_AT_type        : <355f>
 <1><358f>: Abbrev Number: 17 (DW_TAG_typedef)
  <3590>     DW_AT_name        : MGC_Pipe
  <3599>     DW_AT_decl_file   : 12
  <359a>     DW_AT_decl_line   : 355
  <359c>     DW_AT_type        : <2be0>
 <1><35a0>: Abbrev Number: 22 (DW_TAG_structure_type)
  <35a1>     DW_AT_byte_size   : 20
  <35a2>     DW_AT_decl_file   : 12
  <35a3>     DW_AT_decl_line   : 395
  <35a5>     DW_AT_sibling     : <35f0>
 <2><35a9>: Abbrev Number: 13 (DW_TAG_member)
  <35aa>     DW_AT_name        : dwTotalTime
  <35b6>     DW_AT_decl_file   : 12
  <35b7>     DW_AT_decl_line   : 396
  <35b9>     DW_AT_type        : <18e>
  <35bd>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><35c0>: Abbrev Number: 13 (DW_TAG_member)
  <35c1>     DW_AT_name        : ScheduleSlots
  <35cf>     DW_AT_decl_file   : 12
  <35d0>     DW_AT_decl_line   : 397
  <35d2>     DW_AT_type        : <2100>
  <35d6>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><35d9>: Abbrev Number: 13 (DW_TAG_member)
  <35da>     DW_AT_name        : wSlotCount
  <35e5>     DW_AT_decl_file   : 12
  <35e6>     DW_AT_decl_line   : 398
  <35e8>     DW_AT_type        : <16f>
  <35ec>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <1><35f0>: Abbrev Number: 17 (DW_TAG_typedef)
  <35f1>     DW_AT_name        : MGC_Schedule
  <35fe>     DW_AT_decl_file   : 12
  <35ff>     DW_AT_decl_line   : 399
  <3601>     DW_AT_type        : <35a0>
 <1><3605>: Abbrev Number: 22 (DW_TAG_structure_type)
  <3606>     DW_AT_byte_size   : 92
  <3607>     DW_AT_decl_file   : 12
  <3608>     DW_AT_decl_line   : 411
  <360a>     DW_AT_sibling     : <3698>
 <2><360e>: Abbrev Number: 23 (DW_TAG_member)
  <360f>     DW_AT_name        : (indirect string, offset: 0x1a9): pDriver
  <3613>     DW_AT_decl_file   : 12
  <3614>     DW_AT_decl_line   : 412
  <3616>     DW_AT_type        : <1ec4>
  <361a>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><361d>: Abbrev Number: 13 (DW_TAG_member)
  <361e>     DW_AT_name        : pSchedule
  <3628>     DW_AT_decl_file   : 12
  <3629>     DW_AT_decl_line   : 413
  <362b>     DW_AT_type        : <3698>
  <362f>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><3632>: Abbrev Number: 23 (DW_TAG_member)
  <3633>     DW_AT_name        : (indirect string, offset: 0x188): PipeList
  <3637>     DW_AT_decl_file   : 12
  <3638>     DW_AT_decl_line   : 414
  <363a>     DW_AT_type        : <2100>
  <363e>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><3641>: Abbrev Number: 13 (DW_TAG_member)
  <3642>     DW_AT_name        : Device
  <3649>     DW_AT_decl_file   : 12
  <364a>     DW_AT_decl_line   : 415
  <364c>     DW_AT_type        : <14ef>
  <3650>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><3653>: Abbrev Number: 13 (DW_TAG_member)
  <3654>     DW_AT_name        : wbmTxToggle
  <3660>     DW_AT_decl_file   : 12
  <3661>     DW_AT_decl_line   : 417
  <3663>     DW_AT_type        : <16f>
  <3667>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <2><366a>: Abbrev Number: 13 (DW_TAG_member)
  <366b>     DW_AT_name        : wbmRxToggle
  <3677>     DW_AT_decl_file   : 12
  <3678>     DW_AT_decl_line   : 418
  <367a>     DW_AT_type        : <16f>
  <367e>     DW_AT_data_member_location: 2 byte block: 23 56    (DW_OP_plus_uconst: 86)
 <2><3681>: Abbrev Number: 13 (DW_TAG_member)
  <3682>     DW_AT_name        : bIsMultiTt
  <368d>     DW_AT_decl_file   : 12
  <368e>     DW_AT_decl_line   : 419
  <3690>     DW_AT_type        : <160>
  <3694>     DW_AT_data_member_location: 2 byte block: 23 58    (DW_OP_plus_uconst: 88)
 <1><3698>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3699>     DW_AT_byte_size   : 4
  <369a>     DW_AT_type        : <35f0>
 <1><369e>: Abbrev Number: 17 (DW_TAG_typedef)
  <369f>     DW_AT_name        : MGC_Device
  <36aa>     DW_AT_decl_file   : 12
  <36ab>     DW_AT_decl_line   : 421
  <36ad>     DW_AT_type        : <3605>
 <1><36b1>: Abbrev Number: 17 (DW_TAG_typedef)
  <36b2>     DW_AT_name        : MGC_pfProgramStartController
  <36cf>     DW_AT_decl_file   : 12
  <36d0>     DW_AT_decl_line   : 435
  <36d2>     DW_AT_type        : <36d6>
 <1><36d6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <36d7>     DW_AT_byte_size   : 4
  <36d8>     DW_AT_type        : <36dc>
 <1><36dc>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <36dd>     DW_AT_prototyped  : 1
  <36de>     DW_AT_type        : <18e>
  <36e2>     DW_AT_sibling     : <36ec>
 <2><36e6>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <36e7>     DW_AT_type        : <36ec>
 <1><36ec>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <36ed>     DW_AT_byte_size   : 4
  <36ee>     DW_AT_type        : <36f2>
 <1><36f2>: Abbrev Number: 24 (DW_TAG_structure_type)
  <36f3>     DW_AT_name        : _MGC_Controller
  <3703>     DW_AT_byte_size   : 60
  <3704>     DW_AT_decl_file   : 12
  <3705>     DW_AT_decl_line   : 240
  <3706>     DW_AT_sibling     : <38b9>
 <2><370a>: Abbrev Number: 13 (DW_TAG_member)
  <370b>     DW_AT_name        : pControllerAddressIsr
  <3721>     DW_AT_decl_file   : 12
  <3722>     DW_AT_decl_line   : 512
  <3724>     DW_AT_type        : <f1>
  <3728>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><372b>: Abbrev Number: 13 (DW_TAG_member)
  <372c>     DW_AT_name        : pControllerAddressIst
  <3742>     DW_AT_decl_file   : 12
  <3743>     DW_AT_decl_line   : 513
  <3745>     DW_AT_type        : <f1>
  <3749>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><374c>: Abbrev Number: 13 (DW_TAG_member)
  <374d>     DW_AT_name        : pUtils
  <3754>     DW_AT_decl_file   : 12
  <3755>     DW_AT_decl_line   : 514
  <3757>     DW_AT_type        : <39f3>
  <375b>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><375e>: Abbrev Number: 13 (DW_TAG_member)
  <375f>     DW_AT_name        : pSystemServices
  <376f>     DW_AT_decl_file   : 12
  <3770>     DW_AT_decl_line   : 515
  <3772>     DW_AT_type        : <39f9>
  <3776>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><3779>: Abbrev Number: 23 (DW_TAG_member)
  <377a>     DW_AT_name        : (indirect string, offset: 0x104): pPrivateData
  <377e>     DW_AT_decl_file   : 12
  <377f>     DW_AT_decl_line   : 516
  <3781>     DW_AT_type        : <f1>
  <3785>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><3788>: Abbrev Number: 23 (DW_TAG_member)
  <3789>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <378d>     DW_AT_decl_file   : 12
  <378e>     DW_AT_decl_line   : 517
  <3790>     DW_AT_type        : <3559>
  <3794>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><3797>: Abbrev Number: 13 (DW_TAG_member)
  <3798>     DW_AT_name        : pDmaControllerFactory
  <37ae>     DW_AT_decl_file   : 12
  <37af>     DW_AT_decl_line   : 519
  <37b1>     DW_AT_type        : <39ff>
  <37b5>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><37b8>: Abbrev Number: 13 (DW_TAG_member)
  <37b9>     DW_AT_name        : pfProgramStartController
  <37d2>     DW_AT_decl_file   : 12
  <37d3>     DW_AT_decl_line   : 522
  <37d5>     DW_AT_type        : <36b1>
  <37d9>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><37dc>: Abbrev Number: 13 (DW_TAG_member)
  <37dd>     DW_AT_name        : pfProgramStopController
  <37f5>     DW_AT_decl_file   : 12
  <37f6>     DW_AT_decl_line   : 523
  <37f8>     DW_AT_type        : <38b9>
  <37fc>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><37ff>: Abbrev Number: 13 (DW_TAG_member)
  <3800>     DW_AT_name        : pfDestroyController
  <3814>     DW_AT_decl_file   : 12
  <3815>     DW_AT_decl_line   : 524
  <3817>     DW_AT_type        : <38dd>
  <381b>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><381e>: Abbrev Number: 13 (DW_TAG_member)
  <381f>     DW_AT_name        : pfSetControllerHostPower
  <3838>     DW_AT_decl_file   : 12
  <3839>     DW_AT_decl_line   : 525
  <383b>     DW_AT_type        : <38fd>
  <383f>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><3842>: Abbrev Number: 13 (DW_TAG_member)
  <3843>     DW_AT_name        : pfDumpControllerState
  <3859>     DW_AT_decl_file   : 12
  <385a>     DW_AT_decl_line   : 528
  <385c>     DW_AT_type        : <393d>
  <3860>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><3863>: Abbrev Number: 13 (DW_TAG_member)
  <3864>     DW_AT_name        : pfDumpPipeState
  <3874>     DW_AT_decl_file   : 12
  <3875>     DW_AT_decl_line   : 529
  <3877>     DW_AT_type        : <3975>
  <387b>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><387e>: Abbrev Number: 13 (DW_TAG_member)
  <387f>     DW_AT_name        : pfDumpLocalEndState
  <3893>     DW_AT_decl_file   : 12
  <3894>     DW_AT_decl_line   : 530
  <3896>     DW_AT_type        : <39b2>
  <389a>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><389d>: Abbrev Number: 13 (DW_TAG_member)
  <389e>     DW_AT_name        : wControllerType
  <38ae>     DW_AT_decl_file   : 12
  <38af>     DW_AT_decl_line   : 532
  <38b1>     DW_AT_type        : <16f>
  <38b5>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <1><38b9>: Abbrev Number: 17 (DW_TAG_typedef)
  <38ba>     DW_AT_name        : MGC_pfProgramStopController
  <38d6>     DW_AT_decl_file   : 12
  <38d7>     DW_AT_decl_line   : 444
  <38d9>     DW_AT_type        : <36d6>
 <1><38dd>: Abbrev Number: 17 (DW_TAG_typedef)
  <38de>     DW_AT_name        : MGC_pfDestroyController
  <38f6>     DW_AT_decl_file   : 12
  <38f7>     DW_AT_decl_line   : 452
  <38f9>     DW_AT_type        : <36d6>
 <1><38fd>: Abbrev Number: 17 (DW_TAG_typedef)
  <38fe>     DW_AT_name        : MGC_pfSetControllerHostPower
  <391b>     DW_AT_decl_file   : 12
  <391c>     DW_AT_decl_line   : 460
  <391e>     DW_AT_type        : <3922>
 <1><3922>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3923>     DW_AT_byte_size   : 4
  <3924>     DW_AT_type        : <3928>
 <1><3928>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3929>     DW_AT_prototyped  : 1
  <392a>     DW_AT_type        : <18e>
  <392e>     DW_AT_sibling     : <393d>
 <2><3932>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3933>     DW_AT_type        : <36ec>
 <2><3937>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3938>     DW_AT_type        : <16f>
 <1><393d>: Abbrev Number: 17 (DW_TAG_typedef)
  <393e>     DW_AT_name        : MGC_pfDumpControllerState
  <3958>     DW_AT_decl_file   : 12
  <3959>     DW_AT_decl_line   : 468
  <395b>     DW_AT_type        : <395f>
 <1><395f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3960>     DW_AT_byte_size   : 4
  <3961>     DW_AT_type        : <3965>
 <1><3965>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3966>     DW_AT_prototyped  : 1
  <3967>     DW_AT_type        : <a9>
  <396b>     DW_AT_sibling     : <3975>
 <2><396f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3970>     DW_AT_type        : <36ec>
 <1><3975>: Abbrev Number: 17 (DW_TAG_typedef)
  <3976>     DW_AT_name        : MGC_pfDumpPipeState
  <398a>     DW_AT_decl_file   : 12
  <398b>     DW_AT_decl_line   : 477
  <398d>     DW_AT_type        : <3991>
 <1><3991>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3992>     DW_AT_byte_size   : 4
  <3993>     DW_AT_type        : <3997>
 <1><3997>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3998>     DW_AT_prototyped  : 1
  <3999>     DW_AT_type        : <a9>
  <399d>     DW_AT_sibling     : <39ac>
 <2><39a1>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <39a2>     DW_AT_type        : <36ec>
 <2><39a6>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <39a7>     DW_AT_type        : <39ac>
 <1><39ac>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <39ad>     DW_AT_byte_size   : 4
  <39ae>     DW_AT_type        : <358f>
 <1><39b2>: Abbrev Number: 17 (DW_TAG_typedef)
  <39b3>     DW_AT_name        : MGC_pfDumpLocalEndState
  <39cb>     DW_AT_decl_file   : 12
  <39cc>     DW_AT_decl_line   : 487
  <39ce>     DW_AT_type        : <39d2>
 <1><39d2>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <39d3>     DW_AT_byte_size   : 4
  <39d4>     DW_AT_type        : <39d8>
 <1><39d8>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <39d9>     DW_AT_prototyped  : 1
  <39da>     DW_AT_type        : <a9>
  <39de>     DW_AT_sibling     : <39ed>
 <2><39e2>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <39e3>     DW_AT_type        : <36ec>
 <2><39e7>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <39e8>     DW_AT_type        : <39ed>
 <1><39ed>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <39ee>     DW_AT_byte_size   : 4
  <39ef>     DW_AT_type        : <15cc>
 <1><39f3>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <39f4>     DW_AT_byte_size   : 4
  <39f5>     DW_AT_type        : <93a>
 <1><39f9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <39fa>     DW_AT_byte_size   : 4
  <39fb>     DW_AT_type        : <ccc>
 <1><39ff>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3a00>     DW_AT_byte_size   : 4
  <3a01>     DW_AT_type        : <27f9>
 <1><3a05>: Abbrev Number: 17 (DW_TAG_typedef)
  <3a06>     DW_AT_name        : MGC_Controller
  <3a15>     DW_AT_decl_file   : 12
  <3a16>     DW_AT_decl_line   : 533
  <3a18>     DW_AT_type        : <36f2>
 <1><3a1c>: Abbrev Number: 17 (DW_TAG_typedef)
  <3a1d>     DW_AT_name        : MGC_pfReadBusState
  <3a30>     DW_AT_decl_file   : 12
  <3a31>     DW_AT_decl_line   : 547
  <3a33>     DW_AT_type        : <3a37>
 <1><3a37>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3a38>     DW_AT_byte_size   : 4
  <3a39>     DW_AT_type        : <3a3d>
 <1><3a3d>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3a3e>     DW_AT_prototyped  : 1
  <3a3f>     DW_AT_type        : <18e>
  <3a43>     DW_AT_sibling     : <3a4d>
 <2><3a47>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3a48>     DW_AT_type        : <3559>
 <1><3a4d>: Abbrev Number: 17 (DW_TAG_typedef)
  <3a4e>     DW_AT_name        : MGC_pfProgramBusState
  <3a64>     DW_AT_decl_file   : 12
  <3a65>     DW_AT_decl_line   : 556
  <3a67>     DW_AT_type        : <3a37>
 <1><3a6b>: Abbrev Number: 17 (DW_TAG_typedef)
  <3a6c>     DW_AT_name        : MGC_pfResetPort
  <3a7c>     DW_AT_decl_file   : 12
  <3a7d>     DW_AT_decl_line   : 565
  <3a7f>     DW_AT_type        : <3a37>
 <1><3a83>: Abbrev Number: 17 (DW_TAG_typedef)
  <3a84>     DW_AT_name        : MGC_pfBindEndpoint
  <3a97>     DW_AT_decl_file   : 12
  <3a98>     DW_AT_decl_line   : 581
  <3a9a>     DW_AT_type        : <3a9e>
 <1><3a9e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3a9f>     DW_AT_byte_size   : 4
  <3aa0>     DW_AT_type        : <3aa4>
 <1><3aa4>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3aa5>     DW_AT_prototyped  : 1
  <3aa6>     DW_AT_type        : <2ca4>
  <3aaa>     DW_AT_sibling     : <3ac3>
 <2><3aae>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3aaf>     DW_AT_type        : <3559>
 <2><3ab3>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ab4>     DW_AT_type        : <3ac3>
 <2><3ab8>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ab9>     DW_AT_type        : <39ed>
 <2><3abd>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3abe>     DW_AT_type        : <160>
 <1><3ac3>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3ac4>     DW_AT_byte_size   : 4
  <3ac5>     DW_AT_type        : <3ac9>
 <1><3ac9>: Abbrev Number: 8 (DW_TAG_const_type)
  <3aca>     DW_AT_type        : <156c>
 <1><3ace>: Abbrev Number: 17 (DW_TAG_typedef)
  <3acf>     DW_AT_name        : MGC_pfProgramStartReceive
  <3ae9>     DW_AT_decl_file   : 12
  <3aea>     DW_AT_decl_line   : 600
  <3aec>     DW_AT_type        : <3af0>
 <1><3af0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3af1>     DW_AT_byte_size   : 4
  <3af2>     DW_AT_type        : <3af6>
 <1><3af6>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3af7>     DW_AT_prototyped  : 1
  <3af8>     DW_AT_type        : <18e>
  <3afc>     DW_AT_sibling     : <3b1f>
 <2><3b00>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b01>     DW_AT_type        : <3559>
 <2><3b05>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b06>     DW_AT_type        : <2ca4>
 <2><3b0a>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b0b>     DW_AT_type        : <14e3>
 <2><3b0f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b10>     DW_AT_type        : <18e>
 <2><3b14>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b15>     DW_AT_type        : <f1>
 <2><3b19>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b1a>     DW_AT_type        : <160>
 <1><3b1f>: Abbrev Number: 17 (DW_TAG_typedef)
  <3b20>     DW_AT_name        : MGC_pfProgramStartTransmit
  <3b3b>     DW_AT_decl_file   : 12
  <3b3c>     DW_AT_decl_line   : 614
  <3b3e>     DW_AT_type        : <3b42>
 <1><3b42>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3b43>     DW_AT_byte_size   : 4
  <3b44>     DW_AT_type        : <3b48>
 <1><3b48>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3b49>     DW_AT_prototyped  : 1
  <3b4a>     DW_AT_type        : <18e>
  <3b4e>     DW_AT_sibling     : <3b6c>
 <2><3b52>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b53>     DW_AT_type        : <3559>
 <2><3b57>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b58>     DW_AT_type        : <2ca4>
 <2><3b5c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b5d>     DW_AT_type        : <b51>
 <2><3b61>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b62>     DW_AT_type        : <18e>
 <2><3b66>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3b67>     DW_AT_type        : <f1>
 <1><3b6c>: Abbrev Number: 17 (DW_TAG_typedef)
  <3b6d>     DW_AT_name        : MGC_pfProgramFlushEndpoint
  <3b88>     DW_AT_decl_file   : 12
  <3b89>     DW_AT_decl_line   : 629
  <3b8b>     DW_AT_type        : <3b8f>
 <1><3b8f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3b90>     DW_AT_byte_size   : 4
  <3b91>     DW_AT_type        : <3b95>
 <1><3b95>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3b96>     DW_AT_prototyped  : 1
  <3b97>     DW_AT_type        : <18e>
  <3b9b>     DW_AT_sibling     : <3bb4>
 <2><3b9f>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ba0>     DW_AT_type        : <3559>
 <2><3ba4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ba5>     DW_AT_type        : <2ca4>
 <2><3ba9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3baa>     DW_AT_type        : <160>
 <2><3bae>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3baf>     DW_AT_type        : <160>
 <1><3bb4>: Abbrev Number: 17 (DW_TAG_typedef)
  <3bb5>     DW_AT_name        : MGC_pfProgramHaltEndpoint
  <3bcf>     DW_AT_decl_file   : 12
  <3bd0>     DW_AT_decl_line   : 646
  <3bd2>     DW_AT_type        : <3b8f>
 <1><3bd6>: Abbrev Number: 17 (DW_TAG_typedef)
  <3bd7>     DW_AT_name        : MGC_pfDefaultEndResponse
  <3bf0>     DW_AT_decl_file   : 12
  <3bf1>     DW_AT_decl_line   : 656
  <3bf3>     DW_AT_type        : <3bf7>
 <1><3bf7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3bf8>     DW_AT_byte_size   : 4
  <3bf9>     DW_AT_type        : <3bfd>
 <1><3bfd>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3bfe>     DW_AT_prototyped  : 1
  <3bff>     DW_AT_type        : <18e>
  <3c03>     DW_AT_sibling     : <3c12>
 <2><3c07>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c08>     DW_AT_type        : <3559>
 <2><3c0c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c0d>     DW_AT_type        : <160>
 <1><3c12>: Abbrev Number: 17 (DW_TAG_typedef)
  <3c13>     DW_AT_name        : MGC_pfServiceDefaultEnd
  <3c2b>     DW_AT_decl_file   : 12
  <3c2c>     DW_AT_decl_line   : 665
  <3c2e>     DW_AT_type        : <3c32>
 <1><3c32>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3c33>     DW_AT_byte_size   : 4
  <3c34>     DW_AT_type        : <3c38>
 <1><3c38>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3c39>     DW_AT_prototyped  : 1
  <3c3a>     DW_AT_type        : <160>
  <3c3e>     DW_AT_sibling     : <3c4d>
 <2><3c42>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c43>     DW_AT_type        : <3559>
 <2><3c47>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c48>     DW_AT_type        : <3c4d>
 <1><3c4d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3c4e>     DW_AT_byte_size   : 4
  <3c4f>     DW_AT_type        : <2861>
 <1><3c53>: Abbrev Number: 17 (DW_TAG_typedef)
  <3c54>     DW_AT_name        : MGC_pfServiceTransmitAvail
  <3c6f>     DW_AT_decl_file   : 12
  <3c70>     DW_AT_decl_line   : 675
  <3c72>     DW_AT_type        : <3c76>
 <1><3c76>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3c77>     DW_AT_byte_size   : 4
  <3c78>     DW_AT_type        : <3c7c>
 <1><3c7c>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3c7d>     DW_AT_prototyped  : 1
  <3c7e>     DW_AT_type        : <160>
  <3c82>     DW_AT_sibling     : <3c96>
 <2><3c86>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c87>     DW_AT_type        : <3559>
 <2><3c8b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c8c>     DW_AT_type        : <16f>
 <2><3c90>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3c91>     DW_AT_type        : <3c4d>
 <1><3c96>: Abbrev Number: 17 (DW_TAG_typedef)
  <3c97>     DW_AT_name        : MGC_pfServiceReceiveReady
  <3cb1>     DW_AT_decl_file   : 12
  <3cb2>     DW_AT_decl_line   : 685
  <3cb4>     DW_AT_type        : <3c76>
 <1><3cb8>: Abbrev Number: 17 (DW_TAG_typedef)
  <3cb9>     DW_AT_name        : MGC_pfLoadFifo
  <3cc8>     DW_AT_decl_file   : 12
  <3cc9>     DW_AT_decl_line   : 695
  <3ccb>     DW_AT_type        : <3ccf>
 <1><3ccf>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3cd0>     DW_AT_byte_size   : 4
  <3cd1>     DW_AT_type        : <3cd5>
 <1><3cd5>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3cd6>     DW_AT_prototyped  : 1
  <3cd7>     DW_AT_type        : <160>
  <3cdb>     DW_AT_sibling     : <3cf4>
 <2><3cdf>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ce0>     DW_AT_type        : <3559>
 <2><3ce4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ce5>     DW_AT_type        : <160>
 <2><3ce9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3cea>     DW_AT_type        : <18e>
 <2><3cee>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3cef>     DW_AT_type        : <b51>
 <1><3cf4>: Abbrev Number: 17 (DW_TAG_typedef)
  <3cf5>     DW_AT_name        : MGC_pfUnloadFifo
  <3d06>     DW_AT_decl_file   : 12
  <3d07>     DW_AT_decl_line   : 705
  <3d09>     DW_AT_type        : <3d0d>
 <1><3d0d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3d0e>     DW_AT_byte_size   : 4
  <3d0f>     DW_AT_type        : <3d13>
 <1><3d13>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3d14>     DW_AT_prototyped  : 1
  <3d15>     DW_AT_type        : <160>
  <3d19>     DW_AT_sibling     : <3d32>
 <2><3d1d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d1e>     DW_AT_type        : <3559>
 <2><3d22>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d23>     DW_AT_type        : <160>
 <2><3d27>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d28>     DW_AT_type        : <18e>
 <2><3d2c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d2d>     DW_AT_type        : <14e3>
 <1><3d32>: Abbrev Number: 17 (DW_TAG_typedef)
  <3d33>     DW_AT_name        : MGC_pfAcceptDevice
  <3d46>     DW_AT_decl_file   : 12
  <3d47>     DW_AT_decl_line   : 718
  <3d49>     DW_AT_type        : <3d4d>
 <1><3d4d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3d4e>     DW_AT_byte_size   : 4
  <3d4f>     DW_AT_type        : <3d53>
 <1><3d53>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <3d54>     DW_AT_prototyped  : 1
  <3d55>     DW_AT_type        : <160>
  <3d59>     DW_AT_sibling     : <3d6d>
 <2><3d5d>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d5e>     DW_AT_type        : <3559>
 <2><3d62>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d63>     DW_AT_type        : <1ca0>
 <2><3d67>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d68>     DW_AT_type        : <1ec4>
 <1><3d6d>: Abbrev Number: 17 (DW_TAG_typedef)
  <3d6e>     DW_AT_name        : MGC_pfSetPortTestMode
  <3d84>     DW_AT_decl_file   : 12
  <3d85>     DW_AT_decl_line   : 725
  <3d87>     DW_AT_type        : <3d8b>
 <1><3d8b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3d8c>     DW_AT_byte_size   : 4
  <3d8d>     DW_AT_type        : <3d91>
 <1><3d91>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <3d92>     DW_AT_prototyped  : 1
  <3d93>     DW_AT_sibling     : <3da2>
 <2><3d97>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d98>     DW_AT_type        : <3559>
 <2><3d9c>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3d9d>     DW_AT_type        : <223e>
 <1><3da2>: Abbrev Number: 17 (DW_TAG_typedef)
  <3da3>     DW_AT_name        : MGC_pfDynamicFifoLocation
  <3dbd>     DW_AT_decl_file   : 12
  <3dbe>     DW_AT_decl_line   : 737
  <3dc0>     DW_AT_type        : <3dc4>
 <1><3dc4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3dc5>     DW_AT_byte_size   : 4
  <3dc6>     DW_AT_type        : <3dca>
 <1><3dca>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <3dcb>     DW_AT_prototyped  : 1
  <3dcc>     DW_AT_sibling     : <3df4>
 <2><3dd0>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3dd1>     DW_AT_type        : <3559>
 <2><3dd5>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3dd6>     DW_AT_type        : <160>
 <2><3dda>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3ddb>     DW_AT_type        : <160>
 <2><3ddf>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3de0>     DW_AT_type        : <160>
 <2><3de4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3de5>     DW_AT_type        : <19a0>
 <2><3de9>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3dea>     DW_AT_type        : <14e3>
 <2><3dee>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <3def>     DW_AT_type        : <14e3>
 <1><3df4>: Abbrev Number: 27 (DW_TAG_structure_type)
  <3df5>     DW_AT_byte_size   : 344
  <3df7>     DW_AT_decl_file   : 12
  <3df8>     DW_AT_decl_line   : 784
  <3dfa>     DW_AT_sibling     : <3f1b>
 <2><3dfe>: Abbrev Number: 13 (DW_TAG_member)
  <3dff>     DW_AT_name        : pfEnumerationComplete
  <3e15>     DW_AT_decl_file   : 12
  <3e16>     DW_AT_decl_line   : 785
  <3e18>     DW_AT_type        : <1fc5>
  <3e1c>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><3e1f>: Abbrev Number: 23 (DW_TAG_member)
  <3e20>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <3e24>     DW_AT_decl_file   : 12
  <3e25>     DW_AT_decl_line   : 786
  <3e27>     DW_AT_type        : <1ca0>
  <3e2b>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><3e2e>: Abbrev Number: 13 (DW_TAG_member)
  <3e2f>     DW_AT_name        : aSetupTx
  <3e38>     DW_AT_decl_file   : 12
  <3e39>     DW_AT_decl_line   : 787
  <3e3b>     DW_AT_type        : <3f1b>
  <3e3f>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><3e42>: Abbrev Number: 13 (DW_TAG_member)
  <3e43>     DW_AT_name        : aSetupRx
  <3e4c>     DW_AT_decl_file   : 12
  <3e4d>     DW_AT_decl_line   : 788
  <3e4f>     DW_AT_type        : <3f2b>
  <3e53>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><3e56>: Abbrev Number: 13 (DW_TAG_member)
  <3e57>     DW_AT_name        : abmAddress
  <3e62>     DW_AT_decl_file   : 12
  <3e63>     DW_AT_decl_line   : 789
  <3e65>     DW_AT_type        : <3f3b>
  <3e69>     DW_AT_data_member_location: 3 byte block: 23 90 2  (DW_OP_plus_uconst: 272)
 <2><3e6d>: Abbrev Number: 13 (DW_TAG_member)
  <3e6e>     DW_AT_name        : Irp
  <3e72>     DW_AT_decl_file   : 12
  <3e73>     DW_AT_decl_line   : 790
  <3e75>     DW_AT_type        : <1732>
  <3e79>     DW_AT_data_member_location: 3 byte block: 23 a0 2  (DW_OP_plus_uconst: 288)
 <2><3e7d>: Abbrev Number: 13 (DW_TAG_member)
  <3e7e>     DW_AT_name        : dwData
  <3e85>     DW_AT_decl_file   : 12
  <3e86>     DW_AT_decl_line   : 791
  <3e88>     DW_AT_type        : <18e>
  <3e8c>     DW_AT_data_member_location: 3 byte block: 23 cc 2  (DW_OP_plus_uconst: 332)
 <2><3e90>: Abbrev Number: 13 (DW_TAG_member)
  <3e91>     DW_AT_name        : wData
  <3e97>     DW_AT_decl_file   : 12
  <3e98>     DW_AT_decl_line   : 792
  <3e9a>     DW_AT_type        : <16f>
  <3e9e>     DW_AT_data_member_location: 3 byte block: 23 d0 2  (DW_OP_plus_uconst: 336)
 <2><3ea2>: Abbrev Number: 23 (DW_TAG_member)
  <3ea3>     DW_AT_name        : (indirect string, offset: 0xd5): bAddress
  <3ea7>     DW_AT_decl_file   : 12
  <3ea8>     DW_AT_decl_line   : 793
  <3eaa>     DW_AT_type        : <160>
  <3eae>     DW_AT_data_member_location: 3 byte block: 23 d2 2  (DW_OP_plus_uconst: 338)
 <2><3eb2>: Abbrev Number: 13 (DW_TAG_member)
  <3eb3>     DW_AT_name        : bState
  <3eba>     DW_AT_decl_file   : 12
  <3ebb>     DW_AT_decl_line   : 794
  <3ebd>     DW_AT_type        : <160>
  <3ec1>     DW_AT_data_member_location: 3 byte block: 23 d3 2  (DW_OP_plus_uconst: 339)
 <2><3ec5>: Abbrev Number: 13 (DW_TAG_member)
  <3ec6>     DW_AT_name        : bIndex
  <3ecd>     DW_AT_decl_file   : 12
  <3ece>     DW_AT_decl_line   : 795
  <3ed0>     DW_AT_type        : <160>
  <3ed4>     DW_AT_data_member_location: 3 byte block: 23 d4 2  (DW_OP_plus_uconst: 340)
 <2><3ed8>: Abbrev Number: 13 (DW_TAG_member)
  <3ed9>     DW_AT_name        : bCount
  <3ee0>     DW_AT_decl_file   : 12
  <3ee1>     DW_AT_decl_line   : 796
  <3ee3>     DW_AT_type        : <160>
  <3ee7>     DW_AT_data_member_location: 3 byte block: 23 d5 2  (DW_OP_plus_uconst: 341)
 <2><3eeb>: Abbrev Number: 13 (DW_TAG_member)
  <3eec>     DW_AT_name        : bRetries
  <3ef5>     DW_AT_decl_file   : 12
  <3ef6>     DW_AT_decl_line   : 797
  <3ef8>     DW_AT_type        : <160>
  <3efc>     DW_AT_data_member_location: 3 byte block: 23 d6 2  (DW_OP_plus_uconst: 342)
 <2><3f00>: Abbrev Number: 13 (DW_TAG_member)
  <3f01>     DW_AT_name        : bFatalRetries
  <3f0f>     DW_AT_decl_file   : 12
  <3f10>     DW_AT_decl_line   : 798
  <3f12>     DW_AT_type        : <160>
  <3f16>     DW_AT_data_member_location: 3 byte block: 23 d7 2  (DW_OP_plus_uconst: 343)
 <1><3f1b>: Abbrev Number: 28 (DW_TAG_array_type)
  <3f1c>     DW_AT_type        : <160>
  <3f20>     DW_AT_sibling     : <3f2b>
 <2><3f24>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <3f25>     DW_AT_type        : <dd>
  <3f29>     DW_AT_upper_bound : 7
 <1><3f2b>: Abbrev Number: 28 (DW_TAG_array_type)
  <3f2c>     DW_AT_type        : <160>
  <3f30>     DW_AT_sibling     : <3f3b>
 <2><3f34>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <3f35>     DW_AT_type        : <dd>
  <3f39>     DW_AT_upper_bound : 255
 <1><3f3b>: Abbrev Number: 28 (DW_TAG_array_type)
  <3f3c>     DW_AT_type        : <160>
  <3f40>     DW_AT_sibling     : <3f4b>
 <2><3f44>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <3f45>     DW_AT_type        : <dd>
  <3f49>     DW_AT_upper_bound : 15
 <1><3f4b>: Abbrev Number: 17 (DW_TAG_typedef)
  <3f4c>     DW_AT_name        : MGC_EnumerationData
  <3f60>     DW_AT_decl_file   : 12
  <3f61>     DW_AT_decl_line   : 799
  <3f63>     DW_AT_type        : <3df4>
 <1><3f67>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f68>     DW_AT_byte_size   : 4
  <3f69>     DW_AT_type        : <3a05>
 <1><3f6d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f6e>     DW_AT_byte_size   : 4
  <3f6f>     DW_AT_type        : <1fae>
 <1><3f73>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f74>     DW_AT_byte_size   : 4
  <3f75>     DW_AT_type        : <1eca>
 <1><3f79>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f7a>     DW_AT_byte_size   : 4
  <3f7b>     DW_AT_type        : <1c40>
 <1><3f7f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f80>     DW_AT_byte_size   : 4
  <3f81>     DW_AT_type        : <3f85>
 <1><3f85>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f86>     DW_AT_byte_size   : 4
  <3f87>     DW_AT_type        : <3f8b>
 <1><3f8b>: Abbrev Number: 8 (DW_TAG_const_type)
  <3f8c>     DW_AT_type        : <ff1>
 <1><3f90>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f91>     DW_AT_byte_size   : 4
  <3f92>     DW_AT_type        : <39ac>
 <1><3f96>: Abbrev Number: 17 (DW_TAG_typedef)
  <3f97>     DW_AT_name        : MGC_Port
  <3fa0>     DW_AT_decl_file   : 12
  <3fa1>     DW_AT_decl_line   : 990
  <3fa3>     DW_AT_type        : <2caa>
 <1><3fa7>: Abbrev Number: 17 (DW_TAG_typedef)
  <3fa8>     DW_AT_name        : FAPI_SYS_SemaphoreT
  <3fbc>     DW_AT_decl_file   : 13
  <3fbd>     DW_AT_decl_line   : 965
  <3fbf>     DW_AT_type        : <18e>
 <1><3fc3>: Abbrev Number: 17 (DW_TAG_typedef)
  <3fc4>     DW_AT_name        : FAPI_SYS_MailqueueT
  <3fd8>     DW_AT_decl_file   : 13
  <3fd9>     DW_AT_decl_line   : 975
  <3fdb>     DW_AT_type        : <f1>
 <1><3fdf>: Abbrev Number: 22 (DW_TAG_structure_type)
  <3fe0>     DW_AT_byte_size   : 88
  <3fe1>     DW_AT_decl_file   : 13
  <3fe2>     DW_AT_decl_line   : 1029
  <3fe4>     DW_AT_sibling     : <4249>
 <2><3fe8>: Abbrev Number: 13 (DW_TAG_member)
  <3fe9>     DW_AT_name        : lockSchedulerFunc
  <3ffb>     DW_AT_decl_file   : 13
  <3ffc>     DW_AT_decl_line   : 1044
  <3ffe>     DW_AT_type        : <f5>
  <4002>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><4005>: Abbrev Number: 13 (DW_TAG_member)
  <4006>     DW_AT_name        : unlockSchedulerFunc
  <401a>     DW_AT_decl_file   : 13
  <401b>     DW_AT_decl_line   : 1060
  <401d>     DW_AT_type        : <f5>
  <4021>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><4024>: Abbrev Number: 13 (DW_TAG_member)
  <4025>     DW_AT_name        : sleepFunc
  <402f>     DW_AT_decl_file   : 13
  <4030>     DW_AT_decl_line   : 1073
  <4032>     DW_AT_type        : <33b>
  <4036>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><4039>: Abbrev Number: 13 (DW_TAG_member)
  <403a>     DW_AT_name        : disableIrqFunc
  <4049>     DW_AT_decl_file   : 13
  <404a>     DW_AT_decl_line   : 1092
  <404c>     DW_AT_type        : <329>
  <4050>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><4053>: Abbrev Number: 13 (DW_TAG_member)
  <4054>     DW_AT_name        : enableIrqFunc
  <4062>     DW_AT_decl_file   : 13
  <4063>     DW_AT_decl_line   : 1109
  <4065>     DW_AT_type        : <33b>
  <4069>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><406c>: Abbrev Number: 13 (DW_TAG_member)
  <406d>     DW_AT_name        : createSemaphoreFunc
  <4081>     DW_AT_decl_file   : 13
  <4082>     DW_AT_decl_line   : 1133
  <4084>     DW_AT_type        : <4259>
  <4088>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><408b>: Abbrev Number: 13 (DW_TAG_member)
  <408c>     DW_AT_name        : destroySemaphoreFunc
  <40a1>     DW_AT_decl_file   : 13
  <40a2>     DW_AT_decl_line   : 1154
  <40a4>     DW_AT_type        : <426b>
  <40a8>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><40ab>: Abbrev Number: 13 (DW_TAG_member)
  <40ac>     DW_AT_name        : getSemaphoreFunc
  <40bd>     DW_AT_decl_file   : 13
  <40be>     DW_AT_decl_line   : 1194
  <40c0>     DW_AT_type        : <4286>
  <40c4>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><40c7>: Abbrev Number: 13 (DW_TAG_member)
  <40c8>     DW_AT_name        : setSemaphoreFunc
  <40d9>     DW_AT_decl_file   : 13
  <40da>     DW_AT_decl_line   : 1223
  <40dc>     DW_AT_type        : <429d>
  <40e0>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><40e3>: Abbrev Number: 13 (DW_TAG_member)
  <40e4>     DW_AT_name        : createMailqueueFunc
  <40f8>     DW_AT_decl_file   : 13
  <40f9>     DW_AT_decl_line   : 1249
  <40fb>     DW_AT_type        : <42b8>
  <40ff>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><4102>: Abbrev Number: 13 (DW_TAG_member)
  <4103>     DW_AT_name        : destroyMailqueueFunc
  <4118>     DW_AT_decl_file   : 13
  <4119>     DW_AT_decl_line   : 1270
  <411b>     DW_AT_type        : <42ca>
  <411f>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><4122>: Abbrev Number: 13 (DW_TAG_member)
  <4123>     DW_AT_name        : getMailqueueFunc
  <4134>     DW_AT_decl_file   : 13
  <4135>     DW_AT_decl_line   : 1316
  <4137>     DW_AT_type        : <42ea>
  <413b>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><413e>: Abbrev Number: 13 (DW_TAG_member)
  <413f>     DW_AT_name        : setMailqueueFunc
  <4150>     DW_AT_decl_file   : 13
  <4151>     DW_AT_decl_line   : 1358
  <4153>     DW_AT_type        : <42ea>
  <4157>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><415a>: Abbrev Number: 13 (DW_TAG_member)
  <415b>     DW_AT_name        : getPhysAddressFunc
  <416e>     DW_AT_decl_file   : 13
  <416f>     DW_AT_decl_line   : 1375
  <4171>     DW_AT_type        : <4300>
  <4175>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><4178>: Abbrev Number: 13 (DW_TAG_member)
  <4179>     DW_AT_name        : getVirtAddressFunc
  <418c>     DW_AT_decl_file   : 13
  <418d>     DW_AT_decl_line   : 1392
  <418f>     DW_AT_type        : <4300>
  <4193>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <2><4196>: Abbrev Number: 13 (DW_TAG_member)
  <4197>     DW_AT_name        : mallocFunc
  <41a2>     DW_AT_decl_file   : 13
  <41a3>     DW_AT_decl_line   : 1412
  <41a5>     DW_AT_type        : <4316>
  <41a9>     DW_AT_data_member_location: 2 byte block: 23 3c    (DW_OP_plus_uconst: 60)
 <2><41ac>: Abbrev Number: 13 (DW_TAG_member)
  <41ad>     DW_AT_name        : mallocUncachedFunc
  <41c0>     DW_AT_decl_file   : 13
  <41c1>     DW_AT_decl_line   : 1432
  <41c3>     DW_AT_type        : <4316>
  <41c7>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><41ca>: Abbrev Number: 13 (DW_TAG_member)
  <41cb>     DW_AT_name        : mallocCachedFunc
  <41dc>     DW_AT_decl_file   : 13
  <41dd>     DW_AT_decl_line   : 1452
  <41df>     DW_AT_type        : <4316>
  <41e3>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><41e6>: Abbrev Number: 13 (DW_TAG_member)
  <41e7>     DW_AT_name        : mallocSegmentFunc
  <41f9>     DW_AT_decl_file   : 13
  <41fa>     DW_AT_decl_line   : 1473
  <41fc>     DW_AT_type        : <4331>
  <4200>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><4203>: Abbrev Number: 13 (DW_TAG_member)
  <4204>     DW_AT_name        : freeFunc
  <420d>     DW_AT_decl_file   : 13
  <420e>     DW_AT_decl_line   : 1491
  <4210>     DW_AT_type        : <7ff>
  <4214>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><4217>: Abbrev Number: 13 (DW_TAG_member)
  <4218>     DW_AT_name        : printfFunc
  <4223>     DW_AT_decl_file   : 13
  <4224>     DW_AT_decl_line   : 1512
  <4226>     DW_AT_type        : <4348>
  <422a>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><422d>: Abbrev Number: 13 (DW_TAG_member)
  <422e>     DW_AT_name        : initDriversFunc
  <423e>     DW_AT_decl_file   : 13
  <423f>     DW_AT_decl_line   : 1545
  <4241>     DW_AT_type        : <436a>
  <4245>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <1><4249>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <424a>     DW_AT_prototyped  : 1
  <424b>     DW_AT_type        : <3fa7>
  <424f>     DW_AT_sibling     : <4259>
 <2><4253>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4254>     DW_AT_type        : <17f>
 <1><4259>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <425a>     DW_AT_byte_size   : 4
  <425b>     DW_AT_type        : <4249>
 <1><425f>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <4260>     DW_AT_prototyped  : 1
  <4261>     DW_AT_sibling     : <426b>
 <2><4265>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4266>     DW_AT_type        : <3fa7>
 <1><426b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <426c>     DW_AT_byte_size   : 4
  <426d>     DW_AT_type        : <425f>
 <1><4271>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <4272>     DW_AT_prototyped  : 1
  <4273>     DW_AT_type        : <17f>
  <4277>     DW_AT_sibling     : <4286>
 <2><427b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <427c>     DW_AT_type        : <3fa7>
 <2><4280>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4281>     DW_AT_type        : <18e>
 <1><4286>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4287>     DW_AT_byte_size   : 4
  <4288>     DW_AT_type        : <4271>
 <1><428c>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <428d>     DW_AT_prototyped  : 1
  <428e>     DW_AT_sibling     : <429d>
 <2><4292>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4293>     DW_AT_type        : <3fa7>
 <2><4297>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4298>     DW_AT_type        : <18e>
 <1><429d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <429e>     DW_AT_byte_size   : 4
  <429f>     DW_AT_type        : <428c>
 <1><42a3>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <42a4>     DW_AT_prototyped  : 1
  <42a5>     DW_AT_type        : <3fc3>
  <42a9>     DW_AT_sibling     : <42b8>
 <2><42ad>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42ae>     DW_AT_type        : <18e>
 <2><42b2>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42b3>     DW_AT_type        : <18e>
 <1><42b8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <42b9>     DW_AT_byte_size   : 4
  <42ba>     DW_AT_type        : <42a3>
 <1><42be>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <42bf>     DW_AT_prototyped  : 1
  <42c0>     DW_AT_sibling     : <42ca>
 <2><42c4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42c5>     DW_AT_type        : <3fc3>
 <1><42ca>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <42cb>     DW_AT_byte_size   : 4
  <42cc>     DW_AT_type        : <42be>
 <1><42d0>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <42d1>     DW_AT_prototyped  : 1
  <42d2>     DW_AT_type        : <17f>
  <42d6>     DW_AT_sibling     : <42ea>
 <2><42da>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42db>     DW_AT_type        : <3fc3>
 <2><42df>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42e0>     DW_AT_type        : <f1>
 <2><42e4>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42e5>     DW_AT_type        : <18e>
 <1><42ea>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <42eb>     DW_AT_byte_size   : 4
  <42ec>     DW_AT_type        : <42d0>
 <1><42f0>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <42f1>     DW_AT_prototyped  : 1
  <42f2>     DW_AT_type        : <18e>
  <42f6>     DW_AT_sibling     : <4300>
 <2><42fa>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <42fb>     DW_AT_type        : <18e>
 <1><4300>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4301>     DW_AT_byte_size   : 4
  <4302>     DW_AT_type        : <42f0>
 <1><4306>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <4307>     DW_AT_prototyped  : 1
  <4308>     DW_AT_type        : <f1>
  <430c>     DW_AT_sibling     : <4316>
 <2><4310>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4311>     DW_AT_type        : <86>
 <1><4316>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4317>     DW_AT_byte_size   : 4
  <4318>     DW_AT_type        : <4306>
 <1><431c>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <431d>     DW_AT_prototyped  : 1
  <431e>     DW_AT_type        : <f1>
  <4322>     DW_AT_sibling     : <4331>
 <2><4326>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4327>     DW_AT_type        : <1b3>
 <2><432b>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <432c>     DW_AT_type        : <86>
 <1><4331>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4332>     DW_AT_byte_size   : 4
  <4333>     DW_AT_type        : <431c>
 <1><4337>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <4338>     DW_AT_prototyped  : 1
  <4339>     DW_AT_type        : <17f>
  <433d>     DW_AT_sibling     : <4348>
 <2><4341>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4342>     DW_AT_type        : <116>
 <2><4346>: Abbrev Number: 30 (DW_TAG_unspecified_parameters)
 <1><4348>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4349>     DW_AT_byte_size   : 4
  <434a>     DW_AT_type        : <4337>
 <1><434e>: Abbrev Number: 19 (DW_TAG_subroutine_type)
  <434f>     DW_AT_prototyped  : 1
  <4350>     DW_AT_type        : <17f>
  <4354>     DW_AT_sibling     : <435e>
 <2><4358>: Abbrev Number: 16 (DW_TAG_formal_parameter)
  <4359>     DW_AT_type        : <435e>
 <1><435e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <435f>     DW_AT_byte_size   : 4
  <4360>     DW_AT_type        : <4364>
 <1><4364>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4365>     DW_AT_byte_size   : 4
  <4366>     DW_AT_type        : <341>
 <1><436a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <436b>     DW_AT_byte_size   : 4
  <436c>     DW_AT_type        : <434e>
 <1><4370>: Abbrev Number: 17 (DW_TAG_typedef)
  <4371>     DW_AT_name        : FAPI_SYS_ServicesT
  <4384>     DW_AT_decl_file   : 13
  <4385>     DW_AT_decl_line   : 1547
  <4387>     DW_AT_type        : <3fdf>
 <1><438b>: Abbrev Number: 31 (DW_TAG_subprogram)
  <438c>     DW_AT_external    : 1
  <438d>     DW_AT_name        : MGC_DrcInit
  <4399>     DW_AT_decl_file   : 1
  <439a>     DW_AT_decl_line   : 121
  <439b>     DW_AT_prototyped  : 1
  <439c>     DW_AT_type        : <160>
  <43a0>     DW_AT_low_pc      : 0
  <43a4>     DW_AT_high_pc     : 0x32c
  <43a8>     DW_AT_frame_base  : 0  (location list)
  <43ac>     DW_AT_sibling     : <442a>
 <2><43b0>: Abbrev Number: 32 (DW_TAG_formal_parameter)
  <43b1>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <43b5>     DW_AT_decl_file   : 1
  <43b6>     DW_AT_decl_line   : 120
  <43b7>     DW_AT_type        : <442a>
  <43bb>     DW_AT_location    : 3 byte block: 91 fc 7e     (DW_OP_fbreg: -132)
 <2><43bf>: Abbrev Number: 33 (DW_TAG_variable)
  <43c0>     DW_AT_name        : bEnd
  <43c5>     DW_AT_decl_file   : 1
  <43c6>     DW_AT_decl_line   : 122
  <43c7>     DW_AT_type        : <160>
  <43cb>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><43ce>: Abbrev Number: 33 (DW_TAG_variable)
  <43cf>     DW_AT_name        : end
  <43d3>     DW_AT_decl_file   : 1
  <43d4>     DW_AT_decl_line   : 123
  <43d5>     DW_AT_type        : <2bc3>
  <43d9>     DW_AT_location    : 3 byte block: 91 80 7f     (DW_OP_fbreg: -128)
 <2><43dd>: Abbrev Number: 33 (DW_TAG_variable)
  <43de>     DW_AT_name        : bFifoSize
  <43e8>     DW_AT_decl_file   : 1
  <43e9>     DW_AT_decl_line   : 124
  <43ea>     DW_AT_type        : <160>
  <43ee>     DW_AT_location    : 2 byte block: 91 63    (DW_OP_fbreg: -29)
 <2><43f1>: Abbrev Number: 33 (DW_TAG_variable)
  <43f2>     DW_AT_name        : pImplController
  <4402>     DW_AT_decl_file   : 1
  <4403>     DW_AT_decl_line   : 125
  <4404>     DW_AT_type        : <3f67>
  <4408>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><440b>: Abbrev Number: 33 (DW_TAG_variable)
  <440c>     DW_AT_name        : pBase
  <4412>     DW_AT_decl_file   : 1
  <4413>     DW_AT_decl_line   : 126
  <4414>     DW_AT_type        : <14e3>
  <4418>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><441b>: Abbrev Number: 33 (DW_TAG_variable)
  <441c>     DW_AT_name        : bOk
  <4420>     DW_AT_decl_file   : 1
  <4421>     DW_AT_decl_line   : 127
  <4422>     DW_AT_type        : <160>
  <4426>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><442a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <442b>     DW_AT_byte_size   : 4
  <442c>     DW_AT_type        : <3f96>
 <1><4430>: Abbrev Number: 31 (DW_TAG_subprogram)
  <4431>     DW_AT_external    : 1
  <4432>     DW_AT_name        : MGC_DrcIsr
  <443d>     DW_AT_decl_file   : 1
  <443e>     DW_AT_decl_line   : 202
  <443f>     DW_AT_prototyped  : 1
  <4440>     DW_AT_type        : <a9>
  <4444>     DW_AT_low_pc      : 0x32c
  <4448>     DW_AT_high_pc     : 0x7a4
  <444c>     DW_AT_frame_base  : 0x2a   (location list)
  <4450>     DW_AT_sibling     : <451c>
 <2><4454>: Abbrev Number: 32 (DW_TAG_formal_parameter)
  <4455>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4459>     DW_AT_decl_file   : 1
  <445a>     DW_AT_decl_line   : 200
  <445b>     DW_AT_type        : <3f67>
  <445f>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4462>: Abbrev Number: 34 (DW_TAG_formal_parameter)
  <4463>     DW_AT_name        : bIntrUsbValue
  <4471>     DW_AT_decl_file   : 1
  <4472>     DW_AT_decl_line   : 200
  <4473>     DW_AT_type        : <160>
  <4477>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><447a>: Abbrev Number: 34 (DW_TAG_formal_parameter)
  <447b>     DW_AT_name        : wIntrTxValue
  <4488>     DW_AT_decl_file   : 1
  <4489>     DW_AT_decl_line   : 201
  <448a>     DW_AT_type        : <16f>
  <448e>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4491>: Abbrev Number: 34 (DW_TAG_formal_parameter)
  <4492>     DW_AT_name        : wIntrRxValue
  <449f>     DW_AT_decl_file   : 1
  <44a0>     DW_AT_decl_line   : 201
  <44a1>     DW_AT_type        : <16f>
  <44a5>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><44a8>: Abbrev Number: 33 (DW_TAG_variable)
  <44a9>     DW_AT_name        : qItem
  <44af>     DW_AT_decl_file   : 1
  <44b0>     DW_AT_decl_line   : 203
  <44b1>     DW_AT_type        : <2861>
  <44b5>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><44b8>: Abbrev Number: 33 (DW_TAG_variable)
  <44b9>     DW_AT_name        : dwRegVal
  <44c2>     DW_AT_decl_file   : 1
  <44c3>     DW_AT_decl_line   : 204
  <44c4>     DW_AT_type        : <18e>
  <44c8>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><44cb>: Abbrev Number: 33 (DW_TAG_variable)
  <44cc>     DW_AT_name        : bQueue
  <44d3>     DW_AT_decl_file   : 1
  <44d4>     DW_AT_decl_line   : 205
  <44d5>     DW_AT_type        : <160>
  <44d9>     DW_AT_location    : 2 byte block: 91 5f    (DW_OP_fbreg: -33)
 <2><44dc>: Abbrev Number: 33 (DW_TAG_variable)
  <44dd>     DW_AT_name        : iShift
  <44e4>     DW_AT_decl_file   : 1
  <44e5>     DW_AT_decl_line   : 206
  <44e6>     DW_AT_type        : <cd>
  <44ea>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><44ed>: Abbrev Number: 33 (DW_TAG_variable)
  <44ee>     DW_AT_name        : iResult
  <44f6>     DW_AT_decl_file   : 1
  <44f7>     DW_AT_decl_line   : 207
  <44f8>     DW_AT_type        : <a9>
  <44fc>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><44ff>: Abbrev Number: 35 (DW_TAG_variable)
  <4500>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <4504>     DW_AT_decl_file   : 1
  <4505>     DW_AT_decl_line   : 208
  <4506>     DW_AT_type        : <39f9>
  <450a>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><450d>: Abbrev Number: 35 (DW_TAG_variable)
  <450e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4512>     DW_AT_decl_file   : 1
  <4513>     DW_AT_decl_line   : 209
  <4514>     DW_AT_type        : <442a>
  <4518>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><451c>: Abbrev Number: 36 (DW_TAG_subprogram)
  <451d>     DW_AT_external    : 1
  <451e>     DW_AT_name        : MGC_DrcBindEndpoint
  <4532>     DW_AT_decl_file   : 1
  <4533>     DW_AT_decl_line   : 312
  <4535>     DW_AT_prototyped  : 1
  <4536>     DW_AT_type        : <2ca4>
  <453a>     DW_AT_low_pc      : 0x7a4
  <453e>     DW_AT_high_pc     : 0xb74
  <4542>     DW_AT_frame_base  : 0x54   (location list)
  <4546>     DW_AT_sibling     : <46ae>
 <2><454a>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <454b>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <454f>     DW_AT_decl_file   : 1
  <4550>     DW_AT_decl_line   : 308
  <4552>     DW_AT_type        : <442a>
  <4556>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><4559>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <455a>     DW_AT_name        : pUsbEnd
  <4562>     DW_AT_decl_file   : 1
  <4563>     DW_AT_decl_line   : 309
  <4565>     DW_AT_type        : <3ac3>
  <4569>     DW_AT_location    : 3 byte block: 91 bc 7f     (DW_OP_fbreg: -68)
 <2><456d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <456e>     DW_AT_name        : (indirect string, offset: 0xc1): pRequest
  <4572>     DW_AT_decl_file   : 1
  <4573>     DW_AT_decl_line   : 310
  <4575>     DW_AT_type        : <39ed>
  <4579>     DW_AT_location    : 3 byte block: 91 b8 7f     (DW_OP_fbreg: -72)
 <2><457d>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <457e>     DW_AT_name        : bBind
  <4584>     DW_AT_decl_file   : 1
  <4585>     DW_AT_decl_line   : 311
  <4587>     DW_AT_type        : <160>
  <458b>     DW_AT_location    : 3 byte block: 91 b4 7f     (DW_OP_fbreg: -76)
 <2><458f>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4590>     DW_AT_name        : pbIsTx
  <4597>     DW_AT_decl_file   : 1
  <4598>     DW_AT_decl_line   : 311
  <459a>     DW_AT_type        : <14e3>
  <459e>     DW_AT_location    : 2 byte block: 91 0     (DW_OP_fbreg: 0)
 <2><45a1>: Abbrev Number: 39 (DW_TAG_variable)
  <45a2>     DW_AT_name        : (indirect string, offset: 0xca): dwEndCount
  <45a6>     DW_AT_decl_file   : 1
  <45a7>     DW_AT_decl_line   : 313
  <45a9>     DW_AT_type        : <18e>
  <45ad>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><45b0>: Abbrev Number: 39 (DW_TAG_variable)
  <45b1>     DW_AT_name        : (indirect string, offset: 0x1de): dwEndIndex
  <45b5>     DW_AT_decl_file   : 1
  <45b6>     DW_AT_decl_line   : 314
  <45b8>     DW_AT_type        : <18e>
  <45bc>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><45bf>: Abbrev Number: 40 (DW_TAG_variable)
  <45c0>     DW_AT_name        : bIsIn
  <45c6>     DW_AT_decl_file   : 1
  <45c7>     DW_AT_decl_line   : 315
  <45c9>     DW_AT_type        : <160>
  <45cd>     DW_AT_location    : 2 byte block: 91 4e    (DW_OP_fbreg: -50)
 <2><45d0>: Abbrev Number: 40 (DW_TAG_variable)
  <45d1>     DW_AT_name        : bIsTx
  <45d7>     DW_AT_decl_file   : 1
  <45d8>     DW_AT_decl_line   : 316
  <45da>     DW_AT_type        : <160>
  <45de>     DW_AT_location    : 2 byte block: 91 4f    (DW_OP_fbreg: -49)
 <2><45e1>: Abbrev Number: 40 (DW_TAG_variable)
  <45e2>     DW_AT_name        : bEnd
  <45e7>     DW_AT_decl_file   : 1
  <45e8>     DW_AT_decl_line   : 317
  <45ea>     DW_AT_type        : <160>
  <45ee>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><45f1>: Abbrev Number: 39 (DW_TAG_variable)
  <45f2>     DW_AT_name        : (indirect string, offset: 0xaf): bIsClaimed
  <45f6>     DW_AT_decl_file   : 1
  <45f7>     DW_AT_decl_line   : 318
  <45f9>     DW_AT_type        : <160>
  <45fd>     DW_AT_location    : 2 byte block: 91 51    (DW_OP_fbreg: -47)
 <2><4600>: Abbrev Number: 39 (DW_TAG_variable)
  <4601>     DW_AT_name        : (indirect string, offset: 0x8f): wPacketSize
  <4605>     DW_AT_decl_file   : 1
  <4606>     DW_AT_decl_line   : 319
  <4608>     DW_AT_type        : <16f>
  <460c>     DW_AT_location    : 2 byte block: 91 52    (DW_OP_fbreg: -46)
 <2><460f>: Abbrev Number: 40 (DW_TAG_variable)
  <4610>     DW_AT_name        : pEnd
  <4615>     DW_AT_decl_file   : 1
  <4616>     DW_AT_decl_line   : 320
  <4618>     DW_AT_type        : <2ca4>
  <461c>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><461f>: Abbrev Number: 40 (DW_TAG_variable)
  <4620>     DW_AT_name        : dwDiff
  <4627>     DW_AT_decl_file   : 1
  <4628>     DW_AT_decl_line   : 321
  <462a>     DW_AT_type        : <17f>
  <462e>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4631>: Abbrev Number: 40 (DW_TAG_variable)
  <4632>     DW_AT_name        : dwReqDiff
  <463c>     DW_AT_decl_file   : 1
  <463d>     DW_AT_decl_line   : 322
  <463f>     DW_AT_type        : <17f>
  <4643>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4646>: Abbrev Number: 39 (DW_TAG_variable)
  <4647>     DW_AT_name        : (indirect string, offset: 0x11d): wMaxPacketSize
  <464b>     DW_AT_decl_file   : 1
  <464c>     DW_AT_decl_line   : 323
  <464e>     DW_AT_type        : <16f>
  <4652>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><4655>: Abbrev Number: 40 (DW_TAG_variable)
  <4656>     DW_AT_name        : pBestEnd
  <465f>     DW_AT_decl_file   : 1
  <4660>     DW_AT_decl_line   : 324
  <4662>     DW_AT_type        : <2ca4>
  <4666>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4669>: Abbrev Number: 40 (DW_TAG_variable)
  <466a>     DW_AT_name        : pBestReqEnd
  <4676>     DW_AT_decl_file   : 1
  <4677>     DW_AT_decl_line   : 325
  <4679>     DW_AT_type        : <2ca4>
  <467d>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4680>: Abbrev Number: 40 (DW_TAG_variable)
  <4681>     DW_AT_name        : wBestDiff
  <468b>     DW_AT_decl_file   : 1
  <468c>     DW_AT_decl_line   : 326
  <468e>     DW_AT_type        : <16f>
  <4692>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><4695>: Abbrev Number: 40 (DW_TAG_variable)
  <4696>     DW_AT_name        : wBestReqDiff
  <46a3>     DW_AT_decl_file   : 1
  <46a4>     DW_AT_decl_line   : 327
  <46a6>     DW_AT_type        : <16f>
  <46aa>     DW_AT_location    : 2 byte block: 91 6e    (DW_OP_fbreg: -18)
 <1><46ae>: Abbrev Number: 36 (DW_TAG_subprogram)
  <46af>     DW_AT_external    : 1
  <46b0>     DW_AT_name        : MGC_DrcServiceDefaultEnd
  <46c9>     DW_AT_decl_file   : 1
  <46ca>     DW_AT_decl_line   : 426
  <46cc>     DW_AT_prototyped  : 1
  <46cd>     DW_AT_type        : <160>
  <46d1>     DW_AT_low_pc      : 0xb74
  <46d5>     DW_AT_high_pc     : 0x12c0
  <46d9>     DW_AT_frame_base  : 0x7e   (location list)
  <46dd>     DW_AT_sibling     : <47b9>
 <2><46e1>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <46e2>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <46e6>     DW_AT_decl_file   : 1
  <46e7>     DW_AT_decl_line   : 424
  <46e9>     DW_AT_type        : <442a>
  <46ed>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><46f0>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <46f1>     DW_AT_name        : pItem
  <46f7>     DW_AT_decl_file   : 1
  <46f8>     DW_AT_decl_line   : 424
  <46fa>     DW_AT_type        : <3c4d>
  <46fe>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4701>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4702>     DW_AT_name        : pwCsr
  <4708>     DW_AT_decl_file   : 1
  <4709>     DW_AT_decl_line   : 425
  <470b>     DW_AT_type        : <47b9>
  <470f>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4712>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4713>     DW_AT_name        : (indirect string, offset: 0x67): wCount
  <4717>     DW_AT_decl_file   : 1
  <4718>     DW_AT_decl_line   : 425
  <471a>     DW_AT_type        : <16f>
  <471e>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4721>: Abbrev Number: 39 (DW_TAG_variable)
  <4722>     DW_AT_name        : (indirect string, offset: 0x165): wFifoCount
  <4726>     DW_AT_decl_file   : 1
  <4727>     DW_AT_decl_line   : 427
  <4729>     DW_AT_type        : <16f>
  <472d>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4730>: Abbrev Number: 40 (DW_TAG_variable)
  <4731>     DW_AT_name        : wSpace
  <4738>     DW_AT_decl_file   : 1
  <4739>     DW_AT_decl_line   : 427
  <473b>     DW_AT_type        : <16f>
  <473f>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><4742>: Abbrev Number: 39 (DW_TAG_variable)
  <4743>     DW_AT_name        : (indirect string, offset: 0xfa): pFifoDest
  <4747>     DW_AT_decl_file   : 1
  <4748>     DW_AT_decl_line   : 428
  <474a>     DW_AT_type        : <14e3>
  <474e>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4751>: Abbrev Number: 39 (DW_TAG_variable)
  <4752>     DW_AT_name        : (indirect string, offset: 0xc1): pRequest
  <4756>     DW_AT_decl_file   : 1
  <4757>     DW_AT_decl_line   : 429
  <4759>     DW_AT_type        : <47bf>
  <475d>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4760>: Abbrev Number: 40 (DW_TAG_variable)
  <4761>     DW_AT_name        : bVal
  <4766>     DW_AT_decl_file   : 1
  <4767>     DW_AT_decl_line   : 430
  <4769>     DW_AT_type        : <160>
  <476d>     DW_AT_location    : 2 byte block: 91 66    (DW_OP_fbreg: -26)
 <2><4770>: Abbrev Number: 40 (DW_TAG_variable)
  <4771>     DW_AT_name        : bOutVal
  <4779>     DW_AT_decl_file   : 1
  <477a>     DW_AT_decl_line   : 431
  <477c>     DW_AT_type        : <160>
  <4780>     DW_AT_location    : 2 byte block: 91 57    (DW_OP_fbreg: -41)
 <2><4783>: Abbrev Number: 40 (DW_TAG_variable)
  <4784>     DW_AT_name        : bResult
  <478c>     DW_AT_decl_file   : 1
  <478d>     DW_AT_decl_line   : 432
  <478f>     DW_AT_type        : <160>
  <4793>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4796>: Abbrev Number: 40 (DW_TAG_variable)
  <4797>     DW_AT_name        : status
  <479e>     DW_AT_decl_file   : 1
  <479f>     DW_AT_decl_line   : 433
  <47a1>     DW_AT_type        : <160>
  <47a5>     DW_AT_location    : 2 byte block: 91 56    (DW_OP_fbreg: -42)
 <2><47a8>: Abbrev Number: 40 (DW_TAG_variable)
  <47a9>     DW_AT_name        : pEnd
  <47ae>     DW_AT_decl_file   : 1
  <47af>     DW_AT_decl_line   : 434
  <47b1>     DW_AT_type        : <2ca4>
  <47b5>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><47b9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <47ba>     DW_AT_byte_size   : 4
  <47bb>     DW_AT_type        : <16f>
 <1><47bf>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <47c0>     DW_AT_byte_size   : 4
  <47c1>     DW_AT_type        : <d4d>
 <1><47c5>: Abbrev Number: 36 (DW_TAG_subprogram)
  <47c6>     DW_AT_external    : 1
  <47c7>     DW_AT_name        : MGC_DrcResetPort
  <47d8>     DW_AT_decl_file   : 1
  <47d9>     DW_AT_decl_line   : 602
  <47db>     DW_AT_prototyped  : 1
  <47dc>     DW_AT_type        : <18e>
  <47e0>     DW_AT_low_pc      : 0x12c0
  <47e4>     DW_AT_high_pc     : 0x13cc
  <47e8>     DW_AT_frame_base  : 0xa8   (location list)
  <47ec>     DW_AT_sibling     : <480f>
 <2><47f0>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <47f1>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <47f5>     DW_AT_decl_file   : 1
  <47f6>     DW_AT_decl_line   : 601
  <47f8>     DW_AT_type        : <442a>
  <47fc>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><47ff>: Abbrev Number: 39 (DW_TAG_variable)
  <4800>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4804>     DW_AT_decl_file   : 1
  <4805>     DW_AT_decl_line   : 603
  <4807>     DW_AT_type        : <3f67>
  <480b>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><480f>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4810>     DW_AT_name        : MGC_DrcFlushAll
  <4820>     DW_AT_decl_file   : 1
  <4821>     DW_AT_decl_line   : 644
  <4823>     DW_AT_prototyped  : 1
  <4824>     DW_AT_low_pc      : 0x13cc
  <4828>     DW_AT_high_pc     : 0x15f8
  <482c>     DW_AT_frame_base  : 0xd2   (location list)
  <4830>     DW_AT_sibling     : <48b1>
 <2><4834>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4835>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4839>     DW_AT_decl_file   : 1
  <483a>     DW_AT_decl_line   : 643
  <483c>     DW_AT_type        : <442a>
  <4840>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4843>: Abbrev Number: 39 (DW_TAG_variable)
  <4844>     DW_AT_name        : (indirect string, offset: 0xca): dwEndCount
  <4848>     DW_AT_decl_file   : 1
  <4849>     DW_AT_decl_line   : 645
  <484b>     DW_AT_type        : <18e>
  <484f>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4852>: Abbrev Number: 39 (DW_TAG_variable)
  <4853>     DW_AT_name        : (indirect string, offset: 0x1de): dwEndIndex
  <4857>     DW_AT_decl_file   : 1
  <4858>     DW_AT_decl_line   : 646
  <485a>     DW_AT_type        : <18e>
  <485e>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4861>: Abbrev Number: 39 (DW_TAG_variable)
  <4862>     DW_AT_name        : (indirect string, offset: 0x67): wCount
  <4866>     DW_AT_decl_file   : 1
  <4867>     DW_AT_decl_line   : 647
  <4869>     DW_AT_type        : <16f>
  <486d>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4870>: Abbrev Number: 39 (DW_TAG_variable)
  <4871>     DW_AT_name        : (indirect string, offset: 0xba): wIndex
  <4875>     DW_AT_decl_file   : 1
  <4876>     DW_AT_decl_line   : 648
  <4878>     DW_AT_type        : <16f>
  <487c>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><487f>: Abbrev Number: 40 (DW_TAG_variable)
  <4880>     DW_AT_name        : pIrp
  <4885>     DW_AT_decl_file   : 1
  <4886>     DW_AT_decl_line   : 649
  <4888>     DW_AT_type        : <f1>
  <488c>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><488f>: Abbrev Number: 40 (DW_TAG_variable)
  <4890>     DW_AT_name        : pEnd
  <4895>     DW_AT_decl_file   : 1
  <4896>     DW_AT_decl_line   : 650
  <4898>     DW_AT_type        : <2ca4>
  <489c>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><489f>: Abbrev Number: 40 (DW_TAG_variable)
  <48a0>     DW_AT_name        : pBase
  <48a6>     DW_AT_decl_file   : 1
  <48a7>     DW_AT_decl_line   : 651
  <48a9>     DW_AT_type        : <14e3>
  <48ad>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><48b1>: Abbrev Number: 42 (DW_TAG_subprogram)
  <48b2>     DW_AT_external    : 1
  <48b3>     DW_AT_name        : MGC_DrcOtgTimer
  <48c3>     DW_AT_decl_file   : 1
  <48c4>     DW_AT_decl_line   : 690
  <48c6>     DW_AT_prototyped  : 1
  <48c7>     DW_AT_low_pc      : 0x15f8
  <48cb>     DW_AT_high_pc     : 0x172c
  <48cf>     DW_AT_frame_base  : 0xfc   (location list)
  <48d3>     DW_AT_sibling     : <4923>
 <2><48d7>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <48d8>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <48dc>     DW_AT_decl_file   : 1
  <48dd>     DW_AT_decl_line   : 689
  <48df>     DW_AT_type        : <f1>
  <48e3>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><48e6>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <48e7>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <48eb>     DW_AT_decl_file   : 1
  <48ec>     DW_AT_decl_line   : 689
  <48ee>     DW_AT_type        : <16f>
  <48f2>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><48f5>: Abbrev Number: 39 (DW_TAG_variable)
  <48f6>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <48fa>     DW_AT_decl_file   : 1
  <48fb>     DW_AT_decl_line   : 691
  <48fd>     DW_AT_type        : <3f67>
  <4901>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4904>: Abbrev Number: 39 (DW_TAG_variable)
  <4905>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4909>     DW_AT_decl_file   : 1
  <490a>     DW_AT_decl_line   : 692
  <490c>     DW_AT_type        : <442a>
  <4910>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4913>: Abbrev Number: 39 (DW_TAG_variable)
  <4914>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4918>     DW_AT_decl_file   : 1
  <4919>     DW_AT_decl_line   : 693
  <491b>     DW_AT_type        : <3f6d>
  <491f>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4923>: Abbrev Number: 43 (DW_TAG_subprogram)
  <4924>     DW_AT_name        : MGC_ExitingHost
  <4934>     DW_AT_decl_file   : 1
  <4935>     DW_AT_decl_line   : 722
  <4937>     DW_AT_prototyped  : 1
  <4938>     DW_AT_type        : <160>
  <493c>     DW_AT_low_pc      : 0x172c
  <4940>     DW_AT_high_pc     : 0x17bc
  <4944>     DW_AT_frame_base  : 0x126  (location list)
  <4948>     DW_AT_sibling     : <496d>
 <2><494c>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <494d>     DW_AT_name        : toOtg
  <4953>     DW_AT_decl_file   : 1
  <4954>     DW_AT_decl_line   : 721
  <4956>     DW_AT_type        : <160>
  <495a>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><495d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <495e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4962>     DW_AT_decl_file   : 1
  <4963>     DW_AT_decl_line   : 721
  <4965>     DW_AT_type        : <442a>
  <4969>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><496d>: Abbrev Number: 41 (DW_TAG_subprogram)
  <496e>     DW_AT_name        : MGC_ExitActiveUsbMode
  <4984>     DW_AT_decl_file   : 1
  <4985>     DW_AT_decl_line   : 748
  <4987>     DW_AT_prototyped  : 1
  <4988>     DW_AT_low_pc      : 0x17bc
  <498c>     DW_AT_high_pc     : 0x1810
  <4990>     DW_AT_frame_base  : 0x150  (location list)
  <4994>     DW_AT_sibling     : <49b9>
 <2><4998>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4999>     DW_AT_name        : toOtg
  <499f>     DW_AT_decl_file   : 1
  <49a0>     DW_AT_decl_line   : 747
  <49a2>     DW_AT_type        : <160>
  <49a6>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><49a9>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <49aa>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <49ae>     DW_AT_decl_file   : 1
  <49af>     DW_AT_decl_line   : 747
  <49b1>     DW_AT_type        : <442a>
  <49b5>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><49b9>: Abbrev Number: 41 (DW_TAG_subprogram)
  <49ba>     DW_AT_name        : MGC_SetPowerSave
  <49cb>     DW_AT_decl_file   : 1
  <49cc>     DW_AT_decl_line   : 760
  <49ce>     DW_AT_prototyped  : 1
  <49cf>     DW_AT_low_pc      : 0x1810
  <49d3>     DW_AT_high_pc     : 0x1834
  <49d7>     DW_AT_frame_base  : 0x17a  (location list)
  <49db>     DW_AT_sibling     : <4a00>
 <2><49df>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <49e0>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <49e4>     DW_AT_decl_file   : 1
  <49e5>     DW_AT_decl_line   : 759
  <49e7>     DW_AT_type        : <442a>
  <49eb>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><49ee>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <49ef>     DW_AT_name        : bSave
  <49f5>     DW_AT_decl_file   : 1
  <49f6>     DW_AT_decl_line   : 759
  <49f8>     DW_AT_type        : <160>
  <49fc>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><4a00>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4a01>     DW_AT_name        : MGC_StartPeripheralMode
  <4a19>     DW_AT_decl_file   : 1
  <4a1a>     DW_AT_decl_line   : 774
  <4a1c>     DW_AT_prototyped  : 1
  <4a1d>     DW_AT_low_pc      : 0x1834
  <4a21>     DW_AT_high_pc     : 0x184c
  <4a25>     DW_AT_frame_base  : 0x1a4  (location list)
  <4a29>     DW_AT_sibling     : <4a3d>
 <2><4a2d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4a2e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4a32>     DW_AT_decl_file   : 1
  <4a33>     DW_AT_decl_line   : 773
  <4a35>     DW_AT_type        : <442a>
  <4a39>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4a3d>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4a3e>     DW_AT_name        : MGC_AidlBdisTimeout
  <4a52>     DW_AT_decl_file   : 1
  <4a53>     DW_AT_decl_line   : 791
  <4a55>     DW_AT_prototyped  : 1
  <4a56>     DW_AT_low_pc      : 0x184c
  <4a5a>     DW_AT_high_pc     : 0x1930
  <4a5e>     DW_AT_frame_base  : 0x1ce  (location list)
  <4a62>     DW_AT_sibling     : <4ab2>
 <2><4a66>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4a67>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4a6b>     DW_AT_decl_file   : 1
  <4a6c>     DW_AT_decl_line   : 790
  <4a6e>     DW_AT_type        : <f1>
  <4a72>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4a75>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4a76>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4a7a>     DW_AT_decl_file   : 1
  <4a7b>     DW_AT_decl_line   : 790
  <4a7d>     DW_AT_type        : <16f>
  <4a81>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4a84>: Abbrev Number: 39 (DW_TAG_variable)
  <4a85>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4a89>     DW_AT_decl_file   : 1
  <4a8a>     DW_AT_decl_line   : 792
  <4a8c>     DW_AT_type        : <3f67>
  <4a90>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4a93>: Abbrev Number: 39 (DW_TAG_variable)
  <4a94>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4a98>     DW_AT_decl_file   : 1
  <4a99>     DW_AT_decl_line   : 793
  <4a9b>     DW_AT_type        : <442a>
  <4a9f>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4aa2>: Abbrev Number: 39 (DW_TAG_variable)
  <4aa3>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4aa7>     DW_AT_decl_file   : 1
  <4aa8>     DW_AT_decl_line   : 794
  <4aaa>     DW_AT_type        : <3f6d>
  <4aae>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4ab2>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4ab3>     DW_AT_name        : MGC_WaitSessReqTimeout
  <4aca>     DW_AT_decl_file   : 1
  <4acb>     DW_AT_decl_line   : 817
  <4acd>     DW_AT_prototyped  : 1
  <4ace>     DW_AT_low_pc      : 0x1930
  <4ad2>     DW_AT_high_pc     : 0x1a14
  <4ad6>     DW_AT_frame_base  : 0x1f8  (location list)
  <4ada>     DW_AT_sibling     : <4b2a>
 <2><4ade>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4adf>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4ae3>     DW_AT_decl_file   : 1
  <4ae4>     DW_AT_decl_line   : 816
  <4ae6>     DW_AT_type        : <f1>
  <4aea>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4aed>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4aee>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4af2>     DW_AT_decl_file   : 1
  <4af3>     DW_AT_decl_line   : 816
  <4af5>     DW_AT_type        : <16f>
  <4af9>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4afc>: Abbrev Number: 39 (DW_TAG_variable)
  <4afd>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4b01>     DW_AT_decl_file   : 1
  <4b02>     DW_AT_decl_line   : 818
  <4b04>     DW_AT_type        : <3f67>
  <4b08>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4b0b>: Abbrev Number: 39 (DW_TAG_variable)
  <4b0c>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4b10>     DW_AT_decl_file   : 1
  <4b11>     DW_AT_decl_line   : 819
  <4b13>     DW_AT_type        : <442a>
  <4b17>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4b1a>: Abbrev Number: 39 (DW_TAG_variable)
  <4b1b>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4b1f>     DW_AT_decl_file   : 1
  <4b20>     DW_AT_decl_line   : 820
  <4b22>     DW_AT_type        : <3f6d>
  <4b26>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4b2a>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4b2b>     DW_AT_name        : MGC_HostResetStart
  <4b3e>     DW_AT_decl_file   : 1
  <4b3f>     DW_AT_decl_line   : 843
  <4b41>     DW_AT_prototyped  : 1
  <4b42>     DW_AT_low_pc      : 0x1a14
  <4b46>     DW_AT_high_pc     : 0x1ae8
  <4b4a>     DW_AT_frame_base  : 0x222  (location list)
  <4b4e>     DW_AT_sibling     : <4b8f>
 <2><4b52>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4b53>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4b57>     DW_AT_decl_file   : 1
  <4b58>     DW_AT_decl_line   : 842
  <4b5a>     DW_AT_type        : <f1>
  <4b5e>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4b61>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4b62>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4b66>     DW_AT_decl_file   : 1
  <4b67>     DW_AT_decl_line   : 842
  <4b69>     DW_AT_type        : <16f>
  <4b6d>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4b70>: Abbrev Number: 39 (DW_TAG_variable)
  <4b71>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4b75>     DW_AT_decl_file   : 1
  <4b76>     DW_AT_decl_line   : 844
  <4b78>     DW_AT_type        : <3f67>
  <4b7c>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4b7f>: Abbrev Number: 39 (DW_TAG_variable)
  <4b80>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4b84>     DW_AT_decl_file   : 1
  <4b85>     DW_AT_decl_line   : 845
  <4b87>     DW_AT_type        : <442a>
  <4b8b>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4b8f>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4b90>     DW_AT_name        : MGC_HostResetComplete
  <4ba6>     DW_AT_decl_file   : 1
  <4ba7>     DW_AT_decl_line   : 863
  <4ba9>     DW_AT_prototyped  : 1
  <4baa>     DW_AT_low_pc      : 0x1ae8
  <4bae>     DW_AT_high_pc     : 0x1c8c
  <4bb2>     DW_AT_frame_base  : 0x24c  (location list)
  <4bb6>     DW_AT_sibling     : <4c18>
 <2><4bba>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4bbb>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4bbf>     DW_AT_decl_file   : 1
  <4bc0>     DW_AT_decl_line   : 862
  <4bc2>     DW_AT_type        : <f1>
  <4bc6>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4bc9>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4bca>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4bce>     DW_AT_decl_file   : 1
  <4bcf>     DW_AT_decl_line   : 862
  <4bd1>     DW_AT_type        : <16f>
  <4bd5>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4bd8>: Abbrev Number: 39 (DW_TAG_variable)
  <4bd9>     DW_AT_name        : (indirect string, offset: 0xd5): bAddress
  <4bdd>     DW_AT_decl_file   : 1
  <4bde>     DW_AT_decl_line   : 864
  <4be0>     DW_AT_type        : <160>
  <4be4>     DW_AT_location    : 2 byte block: 91 66    (DW_OP_fbreg: -26)
 <2><4be7>: Abbrev Number: 40 (DW_TAG_variable)
  <4be8>     DW_AT_name        : bSpeed
  <4bef>     DW_AT_decl_file   : 1
  <4bf0>     DW_AT_decl_line   : 865
  <4bf2>     DW_AT_type        : <160>
  <4bf6>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4bf9>: Abbrev Number: 39 (DW_TAG_variable)
  <4bfa>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4bfe>     DW_AT_decl_file   : 1
  <4bff>     DW_AT_decl_line   : 866
  <4c01>     DW_AT_type        : <3f67>
  <4c05>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4c08>: Abbrev Number: 39 (DW_TAG_variable)
  <4c09>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4c0d>     DW_AT_decl_file   : 1
  <4c0e>     DW_AT_decl_line   : 867
  <4c10>     DW_AT_type        : <442a>
  <4c14>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4c18>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4c19>     DW_AT_name        : MGC_BSrpFailed
  <4c28>     DW_AT_decl_file   : 1
  <4c29>     DW_AT_decl_line   : 906
  <4c2b>     DW_AT_prototyped  : 1
  <4c2c>     DW_AT_low_pc      : 0x1c8c
  <4c30>     DW_AT_high_pc     : 0x1e14
  <4c34>     DW_AT_frame_base  : 0x276  (location list)
  <4c38>     DW_AT_sibling     : <4c88>
 <2><4c3c>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4c3d>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4c41>     DW_AT_decl_file   : 1
  <4c42>     DW_AT_decl_line   : 905
  <4c44>     DW_AT_type        : <f1>
  <4c48>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4c4b>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4c4c>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4c50>     DW_AT_decl_file   : 1
  <4c51>     DW_AT_decl_line   : 905
  <4c53>     DW_AT_type        : <16f>
  <4c57>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4c5a>: Abbrev Number: 39 (DW_TAG_variable)
  <4c5b>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4c5f>     DW_AT_decl_file   : 1
  <4c60>     DW_AT_decl_line   : 907
  <4c62>     DW_AT_type        : <3f67>
  <4c66>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4c69>: Abbrev Number: 39 (DW_TAG_variable)
  <4c6a>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4c6e>     DW_AT_decl_file   : 1
  <4c6f>     DW_AT_decl_line   : 908
  <4c71>     DW_AT_type        : <442a>
  <4c75>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4c78>: Abbrev Number: 39 (DW_TAG_variable)
  <4c79>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4c7d>     DW_AT_decl_file   : 1
  <4c7e>     DW_AT_decl_line   : 909
  <4c80>     DW_AT_type        : <3f6d>
  <4c84>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4c88>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4c89>     DW_AT_name        : MGC_BAase0BrstTimeout
  <4c9f>     DW_AT_decl_file   : 1
  <4ca0>     DW_AT_decl_line   : 947
  <4ca2>     DW_AT_prototyped  : 1
  <4ca3>     DW_AT_low_pc      : 0x1e14
  <4ca7>     DW_AT_high_pc     : 0x1f00
  <4cab>     DW_AT_frame_base  : 0x2a0  (location list)
  <4caf>     DW_AT_sibling     : <4cff>
 <2><4cb3>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4cb4>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4cb8>     DW_AT_decl_file   : 1
  <4cb9>     DW_AT_decl_line   : 946
  <4cbb>     DW_AT_type        : <f1>
  <4cbf>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4cc2>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4cc3>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4cc7>     DW_AT_decl_file   : 1
  <4cc8>     DW_AT_decl_line   : 946
  <4cca>     DW_AT_type        : <16f>
  <4cce>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4cd1>: Abbrev Number: 39 (DW_TAG_variable)
  <4cd2>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4cd6>     DW_AT_decl_file   : 1
  <4cd7>     DW_AT_decl_line   : 948
  <4cd9>     DW_AT_type        : <3f67>
  <4cdd>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4ce0>: Abbrev Number: 39 (DW_TAG_variable)
  <4ce1>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4ce5>     DW_AT_decl_file   : 1
  <4ce6>     DW_AT_decl_line   : 949
  <4ce8>     DW_AT_type        : <442a>
  <4cec>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4cef>: Abbrev Number: 39 (DW_TAG_variable)
  <4cf0>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4cf4>     DW_AT_decl_file   : 1
  <4cf5>     DW_AT_decl_line   : 950
  <4cf7>     DW_AT_type        : <3f6d>
  <4cfb>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4cff>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4d00>     DW_AT_name        : MGC_SetHnpSupportIrpComplete
  <4d1d>     DW_AT_decl_file   : 1
  <4d1e>     DW_AT_decl_line   : 971
  <4d20>     DW_AT_prototyped  : 1
  <4d21>     DW_AT_low_pc      : 0x1f00
  <4d25>     DW_AT_high_pc     : 0x1f20
  <4d29>     DW_AT_frame_base  : 0x2ca  (location list)
  <4d2d>     DW_AT_sibling     : <4d51>
 <2><4d31>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4d32>     DW_AT_name        : (indirect string, offset: 0xc): pCompleteParam
  <4d36>     DW_AT_decl_file   : 1
  <4d37>     DW_AT_decl_line   : 970
  <4d39>     DW_AT_type        : <f1>
  <4d3d>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><4d40>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4d41>     DW_AT_name        : pIrp
  <4d46>     DW_AT_decl_file   : 1
  <4d47>     DW_AT_decl_line   : 970
  <4d49>     DW_AT_type        : <4d51>
  <4d4d>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><4d51>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4d52>     DW_AT_byte_size   : 4
  <4d53>     DW_AT_type        : <1732>
 <1><4d57>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4d58>     DW_AT_name        : MGC_SetHnpEnableIrpComplete
  <4d74>     DW_AT_decl_file   : 1
  <4d75>     DW_AT_decl_line   : 977
  <4d77>     DW_AT_prototyped  : 1
  <4d78>     DW_AT_low_pc      : 0x1f20
  <4d7c>     DW_AT_high_pc     : 0x1fc4
  <4d80>     DW_AT_frame_base  : 0x2f4  (location list)
  <4d84>     DW_AT_sibling     : <4dc6>
 <2><4d88>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4d89>     DW_AT_name        : (indirect string, offset: 0xc): pCompleteParam
  <4d8d>     DW_AT_decl_file   : 1
  <4d8e>     DW_AT_decl_line   : 976
  <4d90>     DW_AT_type        : <f1>
  <4d94>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4d97>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4d98>     DW_AT_name        : pIrp
  <4d9d>     DW_AT_decl_file   : 1
  <4d9e>     DW_AT_decl_line   : 976
  <4da0>     DW_AT_type        : <4d51>
  <4da4>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4da7>: Abbrev Number: 39 (DW_TAG_variable)
  <4da8>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4dac>     DW_AT_decl_file   : 1
  <4dad>     DW_AT_decl_line   : 978
  <4daf>     DW_AT_type        : <442a>
  <4db3>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4db6>: Abbrev Number: 39 (DW_TAG_variable)
  <4db7>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <4dbb>     DW_AT_decl_file   : 1
  <4dbc>     DW_AT_decl_line   : 979
  <4dbe>     DW_AT_type        : <39f9>
  <4dc2>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4dc6>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4dc7>     DW_AT_name        : MGC_RemoveSuspend
  <4dd9>     DW_AT_decl_file   : 1
  <4dda>     DW_AT_decl_line   : 995
  <4ddc>     DW_AT_prototyped  : 1
  <4ddd>     DW_AT_low_pc      : 0x1fc4
  <4de1>     DW_AT_high_pc     : 0x2018
  <4de5>     DW_AT_frame_base  : 0x31e  (location list)
  <4de9>     DW_AT_sibling     : <4e2a>
 <2><4ded>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4dee>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4df2>     DW_AT_decl_file   : 1
  <4df3>     DW_AT_decl_line   : 994
  <4df5>     DW_AT_type        : <f1>
  <4df9>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4dfc>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4dfd>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4e01>     DW_AT_decl_file   : 1
  <4e02>     DW_AT_decl_line   : 994
  <4e04>     DW_AT_type        : <16f>
  <4e08>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4e0b>: Abbrev Number: 39 (DW_TAG_variable)
  <4e0c>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4e10>     DW_AT_decl_file   : 1
  <4e11>     DW_AT_decl_line   : 996
  <4e13>     DW_AT_type        : <3f67>
  <4e17>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4e1a>: Abbrev Number: 39 (DW_TAG_variable)
  <4e1b>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4e1f>     DW_AT_decl_file   : 1
  <4e20>     DW_AT_decl_line   : 997
  <4e22>     DW_AT_type        : <442a>
  <4e26>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4e2a>: Abbrev Number: 42 (DW_TAG_subprogram)
  <4e2b>     DW_AT_external    : 1
  <4e2c>     DW_AT_name        : MGC_DrcChangeOtgState
  <4e42>     DW_AT_decl_file   : 1
  <4e43>     DW_AT_decl_line   : 1007
  <4e45>     DW_AT_prototyped  : 1
  <4e46>     DW_AT_low_pc      : 0x2018
  <4e4a>     DW_AT_high_pc     : 0x25bc
  <4e4e>     DW_AT_frame_base  : 0x348  (location list)
  <4e52>     DW_AT_sibling     : <4e95>
 <2><4e56>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4e57>     DW_AT_name        : toOtg
  <4e5d>     DW_AT_decl_file   : 1
  <4e5e>     DW_AT_decl_line   : 1006
  <4e60>     DW_AT_type        : <160>
  <4e64>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4e67>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4e68>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4e6c>     DW_AT_decl_file   : 1
  <4e6d>     DW_AT_decl_line   : 1006
  <4e6f>     DW_AT_type        : <442a>
  <4e73>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4e76>: Abbrev Number: 39 (DW_TAG_variable)
  <4e77>     DW_AT_name        : (indirect string, offset: 0x1b): pOtgDesc
  <4e7b>     DW_AT_decl_file   : 1
  <4e7c>     DW_AT_decl_line   : 1008
  <4e7e>     DW_AT_type        : <4e95>
  <4e82>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4e85>: Abbrev Number: 39 (DW_TAG_variable)
  <4e86>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <4e8a>     DW_AT_decl_file   : 1
  <4e8b>     DW_AT_decl_line   : 1009
  <4e8d>     DW_AT_type        : <39f9>
  <4e91>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4e95>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4e96>     DW_AT_byte_size   : 4
  <4e97>     DW_AT_type        : <4e9b>
 <1><4e9b>: Abbrev Number: 8 (DW_TAG_const_type)
  <4e9c>     DW_AT_type        : <104c>
 <1><4ea0>: Abbrev Number: 42 (DW_TAG_subprogram)
  <4ea1>     DW_AT_external    : 1
  <4ea2>     DW_AT_name        : MGC_CompleteOtgTransition
  <4ebc>     DW_AT_decl_file   : 1
  <4ebd>     DW_AT_decl_line   : 1130
  <4ebf>     DW_AT_prototyped  : 1
  <4ec0>     DW_AT_low_pc      : 0x25bc
  <4ec4>     DW_AT_high_pc     : 0x2634
  <4ec8>     DW_AT_frame_base  : 0x372  (location list)
  <4ecc>     DW_AT_sibling     : <4eef>
 <2><4ed0>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4ed1>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4ed5>     DW_AT_decl_file   : 1
  <4ed6>     DW_AT_decl_line   : 1129
  <4ed8>     DW_AT_type        : <442a>
  <4edc>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4edf>: Abbrev Number: 39 (DW_TAG_variable)
  <4ee0>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <4ee4>     DW_AT_decl_file   : 1
  <4ee5>     DW_AT_decl_line   : 1131
  <4ee7>     DW_AT_type        : <3f6d>
  <4eeb>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4eef>: Abbrev Number: 41 (DW_TAG_subprogram)
  <4ef0>     DW_AT_name        : MGC_DrcCompleteResume
  <4f06>     DW_AT_decl_file   : 1
  <4f07>     DW_AT_decl_line   : 1148
  <4f09>     DW_AT_prototyped  : 1
  <4f0a>     DW_AT_low_pc      : 0x2634
  <4f0e>     DW_AT_high_pc     : 0x2744
  <4f12>     DW_AT_frame_base  : 0x39c  (location list)
  <4f16>     DW_AT_sibling     : <4faa>
 <2><4f1a>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4f1b>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <4f1f>     DW_AT_decl_file   : 1
  <4f20>     DW_AT_decl_line   : 1147
  <4f22>     DW_AT_type        : <f1>
  <4f26>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4f29>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4f2a>     DW_AT_name        : (indirect string, offset: 0xde): wTimer
  <4f2e>     DW_AT_decl_file   : 1
  <4f2f>     DW_AT_decl_line   : 1147
  <4f31>     DW_AT_type        : <16f>
  <4f35>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4f38>: Abbrev Number: 39 (DW_TAG_variable)
  <4f39>     DW_AT_name        : (indirect string, offset: 0x67): wCount
  <4f3d>     DW_AT_decl_file   : 1
  <4f3e>     DW_AT_decl_line   : 1149
  <4f40>     DW_AT_type        : <16f>
  <4f44>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4f47>: Abbrev Number: 39 (DW_TAG_variable)
  <4f48>     DW_AT_name        : (indirect string, offset: 0xba): wIndex
  <4f4c>     DW_AT_decl_file   : 1
  <4f4d>     DW_AT_decl_line   : 1150
  <4f4f>     DW_AT_type        : <16f>
  <4f53>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><4f56>: Abbrev Number: 39 (DW_TAG_variable)
  <4f57>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <4f5b>     DW_AT_decl_file   : 1
  <4f5c>     DW_AT_decl_line   : 1151
  <4f5e>     DW_AT_type        : <1ca0>
  <4f62>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4f65>: Abbrev Number: 40 (DW_TAG_variable)
  <4f66>     DW_AT_name        : pImplDevice
  <4f72>     DW_AT_decl_file   : 1
  <4f73>     DW_AT_decl_line   : 1152
  <4f75>     DW_AT_type        : <4faa>
  <4f79>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4f7c>: Abbrev Number: 39 (DW_TAG_variable)
  <4f7d>     DW_AT_name        : (indirect string, offset: 0x1a9): pDriver
  <4f81>     DW_AT_decl_file   : 1
  <4f82>     DW_AT_decl_line   : 1153
  <4f84>     DW_AT_type        : <1ec4>
  <4f88>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4f8b>: Abbrev Number: 39 (DW_TAG_variable)
  <4f8c>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <4f90>     DW_AT_decl_file   : 1
  <4f91>     DW_AT_decl_line   : 1154
  <4f93>     DW_AT_type        : <3f67>
  <4f97>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4f9a>: Abbrev Number: 39 (DW_TAG_variable)
  <4f9b>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4f9f>     DW_AT_decl_file   : 1
  <4fa0>     DW_AT_decl_line   : 1155
  <4fa2>     DW_AT_type        : <442a>
  <4fa6>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4faa>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4fab>     DW_AT_byte_size   : 4
  <4fac>     DW_AT_type        : <369e>
 <1><4fb0>: Abbrev Number: 36 (DW_TAG_subprogram)
  <4fb1>     DW_AT_external    : 1
  <4fb2>     DW_AT_name        : MGC_DrcUsbIsr
  <4fc0>     DW_AT_decl_file   : 1
  <4fc1>     DW_AT_decl_line   : 1188
  <4fc3>     DW_AT_prototyped  : 1
  <4fc4>     DW_AT_type        : <160>
  <4fc8>     DW_AT_low_pc      : 0x2744
  <4fcc>     DW_AT_high_pc     : 0x2c1c
  <4fd0>     DW_AT_frame_base  : 0x3c6  (location list)
  <4fd4>     DW_AT_sibling     : <508c>
 <2><4fd8>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <4fd9>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <4fdd>     DW_AT_decl_file   : 1
  <4fde>     DW_AT_decl_line   : 1187
  <4fe0>     DW_AT_type        : <442a>
  <4fe4>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4fe7>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4fe8>     DW_AT_name        : bIntrUsbVal
  <4ff4>     DW_AT_decl_file   : 1
  <4ff5>     DW_AT_decl_line   : 1187
  <4ff7>     DW_AT_type        : <160>
  <4ffb>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><4ffe>: Abbrev Number: 40 (DW_TAG_variable)
  <4fff>     DW_AT_name        : qItem
  <5005>     DW_AT_decl_file   : 1
  <5006>     DW_AT_decl_line   : 1189
  <5008>     DW_AT_type        : <2861>
  <500c>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><500f>: Abbrev Number: 39 (DW_TAG_variable)
  <5010>     DW_AT_name        : (indirect string, offset: 0xca): dwEndCount
  <5014>     DW_AT_decl_file   : 1
  <5015>     DW_AT_decl_line   : 1190
  <5017>     DW_AT_type        : <18e>
  <501b>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><501e>: Abbrev Number: 39 (DW_TAG_variable)
  <501f>     DW_AT_name        : (indirect string, offset: 0x1de): dwEndIndex
  <5023>     DW_AT_decl_file   : 1
  <5024>     DW_AT_decl_line   : 1191
  <5026>     DW_AT_type        : <18e>
  <502a>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><502d>: Abbrev Number: 40 (DW_TAG_variable)
  <502e>     DW_AT_name        : pEnd
  <5033>     DW_AT_decl_file   : 1
  <5034>     DW_AT_decl_line   : 1192
  <5036>     DW_AT_type        : <2ca4>
  <503a>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><503d>: Abbrev Number: 39 (DW_TAG_variable)
  <503e>     DW_AT_name        : (indirect string, offset: 0x50): pIsochIrp
  <5042>     DW_AT_decl_file   : 1
  <5043>     DW_AT_decl_line   : 1193
  <5045>     DW_AT_type        : <508c>
  <5049>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><504c>: Abbrev Number: 40 (DW_TAG_variable)
  <504d>     DW_AT_name        : bResult
  <5055>     DW_AT_decl_file   : 1
  <5056>     DW_AT_decl_line   : 1194
  <5058>     DW_AT_type        : <160>
  <505c>     DW_AT_location    : 2 byte block: 91 63    (DW_OP_fbreg: -29)
 <2><505f>: Abbrev Number: 39 (DW_TAG_variable)
  <5060>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <5064>     DW_AT_decl_file   : 1
  <5065>     DW_AT_decl_line   : 1195
  <5067>     DW_AT_type        : <39f9>
  <506b>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><506e>: Abbrev Number: 44 (DW_TAG_lexical_block)
  <506f>     DW_AT_low_pc      : 0x296c
  <5073>     DW_AT_high_pc     : 0x2ae0
 <3><5077>: Abbrev Number: 40 (DW_TAG_variable)
  <5078>     DW_AT_name        : wasHost
  <5080>     DW_AT_decl_file   : 1
  <5081>     DW_AT_decl_line   : 1250
  <5083>     DW_AT_type        : <160>
  <5087>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <1><508c>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <508d>     DW_AT_byte_size   : 4
  <508e>     DW_AT_type        : <19a6>
 <1><5092>: Abbrev Number: 42 (DW_TAG_subprogram)
  <5093>     DW_AT_external    : 1
  <5094>     DW_AT_name        : MGC_DrcBsr
  <509f>     DW_AT_decl_file   : 1
  <50a0>     DW_AT_decl_line   : 1339
  <50a2>     DW_AT_prototyped  : 1
  <50a3>     DW_AT_low_pc      : 0x2c1c
  <50a7>     DW_AT_high_pc     : 0x3688
  <50ab>     DW_AT_frame_base  : 0x3f0  (location list)
  <50af>     DW_AT_sibling     : <51c3>
 <2><50b3>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <50b4>     DW_AT_name        : (indirect string, offset: 0x7e): pParam
  <50b8>     DW_AT_decl_file   : 1
  <50b9>     DW_AT_decl_line   : 1338
  <50bb>     DW_AT_type        : <f1>
  <50bf>     DW_AT_location    : 3 byte block: 91 90 7f     (DW_OP_fbreg: -112)
 <2><50c3>: Abbrev Number: 40 (DW_TAG_variable)
  <50c4>     DW_AT_name        : item
  <50c9>     DW_AT_decl_file   : 1
  <50ca>     DW_AT_decl_line   : 1344
  <50cc>     DW_AT_type        : <2861>
  <50d0>     DW_AT_location    : 3 byte block: 91 bc 7f     (DW_OP_fbreg: -68)
 <2><50d4>: Abbrev Number: 40 (DW_TAG_variable)
  <50d5>     DW_AT_name        : bOk
  <50d9>     DW_AT_decl_file   : 1
  <50da>     DW_AT_decl_line   : 1345
  <50dc>     DW_AT_type        : <160>
  <50e0>     DW_AT_location    : 2 byte block: 91 46    (DW_OP_fbreg: -58)
 <2><50e3>: Abbrev Number: 39 (DW_TAG_variable)
  <50e4>     DW_AT_name        : (indirect string, offset: 0x5a): bTrafficType
  <50e8>     DW_AT_decl_file   : 1
  <50e9>     DW_AT_decl_line   : 1346
  <50eb>     DW_AT_type        : <160>
  <50ef>     DW_AT_location    : 2 byte block: 91 47    (DW_OP_fbreg: -57)
 <2><50f2>: Abbrev Number: 39 (DW_TAG_variable)
  <50f3>     DW_AT_name        : (indirect string, offset: 0x85): pLocalEnd
  <50f7>     DW_AT_decl_file   : 1
  <50f8>     DW_AT_decl_line   : 1347
  <50fa>     DW_AT_type        : <2ca4>
  <50fe>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><5101>: Abbrev Number: 40 (DW_TAG_variable)
  <5102>     DW_AT_name        : pIrp
  <5107>     DW_AT_decl_file   : 1
  <5108>     DW_AT_decl_line   : 1348
  <510a>     DW_AT_type        : <51c3>
  <510e>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><5111>: Abbrev Number: 40 (DW_TAG_variable)
  <5112>     DW_AT_name        : pControlIrp
  <511e>     DW_AT_decl_file   : 1
  <511f>     DW_AT_decl_line   : 1349
  <5121>     DW_AT_type        : <4d51>
  <5125>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><5128>: Abbrev Number: 39 (DW_TAG_variable)
  <5129>     DW_AT_name        : (indirect string, offset: 0x50): pIsochIrp
  <512d>     DW_AT_decl_file   : 1
  <512e>     DW_AT_decl_line   : 1350
  <5130>     DW_AT_type        : <508c>
  <5134>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><5137>: Abbrev Number: 40 (DW_TAG_variable)
  <5138>     DW_AT_name        : pIfacePort
  <5143>     DW_AT_decl_file   : 1
  <5144>     DW_AT_decl_line   : 1351
  <5146>     DW_AT_type        : <14d1>
  <514a>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><514d>: Abbrev Number: 39 (DW_TAG_variable)
  <514e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5152>     DW_AT_decl_file   : 1
  <5153>     DW_AT_decl_line   : 1352
  <5155>     DW_AT_type        : <442a>
  <5159>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><515c>: Abbrev Number: 39 (DW_TAG_variable)
  <515d>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <5161>     DW_AT_decl_file   : 1
  <5162>     DW_AT_decl_line   : 1353
  <5164>     DW_AT_type        : <3f67>
  <5168>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><516b>: Abbrev Number: 39 (DW_TAG_variable)
  <516c>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <5170>     DW_AT_decl_file   : 1
  <5171>     DW_AT_decl_line   : 1354
  <5173>     DW_AT_type        : <39f9>
  <5177>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><517a>: Abbrev Number: 39 (DW_TAG_variable)
  <517b>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <517f>     DW_AT_decl_file   : 1
  <5180>     DW_AT_decl_line   : 1355
  <5182>     DW_AT_type        : <3f6d>
  <5186>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><5189>: Abbrev Number: 40 (DW_TAG_variable)
  <518a>     DW_AT_name        : cnt
  <518e>     DW_AT_decl_file   : 1
  <518f>     DW_AT_decl_line   : 1357
  <5191>     DW_AT_type        : <160>
  <5195>     DW_AT_location    : 2 byte block: 91 6e    (DW_OP_fbreg: -18)
 <2><5198>: Abbrev Number: 40 (DW_TAG_variable)
  <5199>     DW_AT_name        : ii
  <519c>     DW_AT_decl_file   : 1
  <519d>     DW_AT_decl_line   : 1357
  <519f>     DW_AT_type        : <160>
  <51a3>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <2><51a6>: Abbrev Number: 40 (DW_TAG_variable)
  <51a7>     DW_AT_name        : dma_request_arr
  <51b7>     DW_AT_decl_file   : 1
  <51b8>     DW_AT_decl_line   : 1358
  <51ba>     DW_AT_type        : <51c9>
  <51be>     DW_AT_location    : 3 byte block: 91 94 7f     (DW_OP_fbreg: -108)
 <1><51c3>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <51c4>     DW_AT_byte_size   : 4
  <51c5>     DW_AT_type        : <1847>
 <1><51c9>: Abbrev Number: 28 (DW_TAG_array_type)
  <51ca>     DW_AT_type        : <7c4>
  <51ce>     DW_AT_sibling     : <51d9>
 <2><51d2>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <51d3>     DW_AT_type        : <dd>
  <51d7>     DW_AT_upper_bound : 9
 <1><51d9>: Abbrev Number: 36 (DW_TAG_subprogram)
  <51da>     DW_AT_external    : 1
  <51db>     DW_AT_name        : MGC_DrcServiceHostDefault
  <51f5>     DW_AT_decl_file   : 1
  <51f6>     DW_AT_decl_line   : 1631
  <51f8>     DW_AT_prototyped  : 1
  <51f9>     DW_AT_type        : <160>
  <51fd>     DW_AT_low_pc      : 0x3688
  <5201>     DW_AT_high_pc     : 0x3b6c
  <5205>     DW_AT_frame_base  : 0x41a  (location list)
  <5209>     DW_AT_sibling     : <52ad>
 <2><520d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <520e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5212>     DW_AT_decl_file   : 1
  <5213>     DW_AT_decl_line   : 1629
  <5215>     DW_AT_type        : <442a>
  <5219>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><521c>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <521d>     DW_AT_name        : (indirect string, offset: 0x67): wCount
  <5221>     DW_AT_decl_file   : 1
  <5222>     DW_AT_decl_line   : 1629
  <5224>     DW_AT_type        : <16f>
  <5228>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><522b>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <522c>     DW_AT_name        : (indirect string, offset: 0x1be): bStatus
  <5230>     DW_AT_decl_file   : 1
  <5231>     DW_AT_decl_line   : 1630
  <5233>     DW_AT_type        : <160>
  <5237>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><523a>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <523b>     DW_AT_name        : pbCsrVal
  <5244>     DW_AT_decl_file   : 1
  <5245>     DW_AT_decl_line   : 1630
  <5247>     DW_AT_type        : <14e3>
  <524b>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><524e>: Abbrev Number: 40 (DW_TAG_variable)
  <524f>     DW_AT_name        : bMore
  <5255>     DW_AT_decl_file   : 1
  <5256>     DW_AT_decl_line   : 1632
  <5258>     DW_AT_type        : <160>
  <525c>     DW_AT_location    : 2 byte block: 91 53    (DW_OP_fbreg: -45)
 <2><525f>: Abbrev Number: 39 (DW_TAG_variable)
  <5260>     DW_AT_name        : (indirect string, offset: 0xfa): pFifoDest
  <5264>     DW_AT_decl_file   : 1
  <5265>     DW_AT_decl_line   : 1633
  <5267>     DW_AT_type        : <14e3>
  <526b>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><526e>: Abbrev Number: 39 (DW_TAG_variable)
  <526f>     DW_AT_name        : (indirect string, offset: 0x165): wFifoCount
  <5273>     DW_AT_decl_file   : 1
  <5274>     DW_AT_decl_line   : 1634
  <5276>     DW_AT_type        : <16f>
  <527a>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><527d>: Abbrev Number: 40 (DW_TAG_variable)
  <527e>     DW_AT_name        : pEnd
  <5283>     DW_AT_decl_file   : 1
  <5284>     DW_AT_decl_line   : 1635
  <5286>     DW_AT_type        : <2ca4>
  <528a>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><528d>: Abbrev Number: 40 (DW_TAG_variable)
  <528e>     DW_AT_name        : pIrp
  <5293>     DW_AT_decl_file   : 1
  <5294>     DW_AT_decl_line   : 1636
  <5296>     DW_AT_type        : <4d51>
  <529a>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><529d>: Abbrev Number: 39 (DW_TAG_variable)
  <529e>     DW_AT_name        : (indirect string, offset: 0xc1): pRequest
  <52a2>     DW_AT_decl_file   : 1
  <52a3>     DW_AT_decl_line   : 1637
  <52a5>     DW_AT_type        : <47bf>
  <52a9>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <1><52ad>: Abbrev Number: 36 (DW_TAG_subprogram)
  <52ae>     DW_AT_external    : 1
  <52af>     DW_AT_name        : MGC_DrcAcceptDevice
  <52c3>     DW_AT_decl_file   : 1
  <52c4>     DW_AT_decl_line   : 1746
  <52c6>     DW_AT_prototyped  : 1
  <52c7>     DW_AT_type        : <160>
  <52cb>     DW_AT_low_pc      : 0x3b6c
  <52cf>     DW_AT_high_pc     : 0x3db8
  <52d3>     DW_AT_frame_base  : 0x444  (location list)
  <52d7>     DW_AT_sibling     : <5336>
 <2><52db>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <52dc>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <52e0>     DW_AT_decl_file   : 1
  <52e1>     DW_AT_decl_line   : 1744
  <52e3>     DW_AT_type        : <442a>
  <52e7>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><52ea>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <52eb>     DW_AT_name        : (indirect string, offset: 0x180): pDevice
  <52ef>     DW_AT_decl_file   : 1
  <52f0>     DW_AT_decl_line   : 1744
  <52f2>     DW_AT_type        : <1ca0>
  <52f6>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><52f9>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <52fa>     DW_AT_name        : (indirect string, offset: 0x1a9): pDriver
  <52fe>     DW_AT_decl_file   : 1
  <52ff>     DW_AT_decl_line   : 1745
  <5301>     DW_AT_type        : <1ec4>
  <5305>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><5308>: Abbrev Number: 39 (DW_TAG_variable)
  <5309>     DW_AT_name        : (indirect string, offset: 0x1b): pOtgDesc
  <530d>     DW_AT_decl_file   : 1
  <530e>     DW_AT_decl_line   : 1747
  <5310>     DW_AT_type        : <4e95>
  <5314>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><5317>: Abbrev Number: 40 (DW_TAG_variable)
  <5318>     DW_AT_name        : bOk
  <531c>     DW_AT_decl_file   : 1
  <531d>     DW_AT_decl_line   : 1748
  <531f>     DW_AT_type        : <160>
  <5323>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <2><5326>: Abbrev Number: 39 (DW_TAG_variable)
  <5327>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <532b>     DW_AT_decl_file   : 1
  <532c>     DW_AT_decl_line   : 1749
  <532e>     DW_AT_type        : <3f6d>
  <5332>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><5336>: Abbrev Number: 42 (DW_TAG_subprogram)
  <5337>     DW_AT_external    : 1
  <5338>     DW_AT_name        : MGC_DrcFinishResume
  <534c>     DW_AT_decl_file   : 1
  <534d>     DW_AT_decl_line   : 1806
  <534f>     DW_AT_prototyped  : 1
  <5350>     DW_AT_low_pc      : 0x3db8
  <5354>     DW_AT_high_pc     : 0x3e44
  <5358>     DW_AT_frame_base  : 0x46e  (location list)
  <535c>     DW_AT_sibling     : <5385>
 <2><5360>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <5361>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5365>     DW_AT_decl_file   : 1
  <5366>     DW_AT_decl_line   : 1805
  <5368>     DW_AT_type        : <442a>
  <536c>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><536f>: Abbrev Number: 40 (DW_TAG_variable)
  <5370>     DW_AT_name        : dwTimeout
  <537a>     DW_AT_decl_file   : 1
  <537b>     DW_AT_decl_line   : 1807
  <537d>     DW_AT_type        : <18e>
  <5381>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><5385>: Abbrev Number: 36 (DW_TAG_subprogram)
  <5386>     DW_AT_external    : 1
  <5387>     DW_AT_name        : MGC_DrcSetHostPower
  <539b>     DW_AT_decl_file   : 1
  <539c>     DW_AT_decl_line   : 1825
  <539e>     DW_AT_prototyped  : 1
  <539f>     DW_AT_type        : <18e>
  <53a3>     DW_AT_low_pc      : 0x3e44
  <53a7>     DW_AT_high_pc     : 0x3ed8
  <53ab>     DW_AT_frame_base  : 0x498  (location list)
  <53af>     DW_AT_sibling     : <53f6>
 <2><53b3>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <53b4>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <53b8>     DW_AT_decl_file   : 1
  <53b9>     DW_AT_decl_line   : 1823
  <53bb>     DW_AT_type        : <3f67>
  <53bf>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><53c2>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <53c3>     DW_AT_name        : wPower
  <53ca>     DW_AT_decl_file   : 1
  <53cb>     DW_AT_decl_line   : 1824
  <53cd>     DW_AT_type        : <16f>
  <53d1>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><53d4>: Abbrev Number: 40 (DW_TAG_variable)
  <53d5>     DW_AT_name        : result
  <53dc>     DW_AT_decl_file   : 1
  <53dd>     DW_AT_decl_line   : 1826
  <53df>     DW_AT_type        : <18e>
  <53e3>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><53e6>: Abbrev Number: 39 (DW_TAG_variable)
  <53e7>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <53eb>     DW_AT_decl_file   : 1
  <53ec>     DW_AT_decl_line   : 1827
  <53ee>     DW_AT_type        : <442a>
  <53f2>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><53f6>: Abbrev Number: 41 (DW_TAG_subprogram)
  <53f7>     DW_AT_name        : MGC_OtgStateGetId
  <5409>     DW_AT_decl_file   : 1
  <540a>     DW_AT_decl_line   : 1847
  <540c>     DW_AT_prototyped  : 1
  <540d>     DW_AT_low_pc      : 0x3ed8
  <5411>     DW_AT_high_pc     : 0x3f98
  <5415>     DW_AT_frame_base  : 0x4c2  (location list)
  <5419>     DW_AT_sibling     : <543c>
 <2><541d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <541e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5422>     DW_AT_decl_file   : 1
  <5423>     DW_AT_decl_line   : 1846
  <5425>     DW_AT_type        : <442a>
  <5429>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><542c>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <542d>     DW_AT_name        : (indirect string, offset: 0x13d): bIsReset
  <5431>     DW_AT_decl_file   : 1
  <5432>     DW_AT_decl_line   : 1846
  <5434>     DW_AT_type        : <160>
  <5438>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><543c>: Abbrev Number: 36 (DW_TAG_subprogram)
  <543d>     DW_AT_external    : 1
  <543e>     DW_AT_name        : MUSB_RegisterOtgClient
  <5455>     DW_AT_decl_file   : 1
  <5456>     DW_AT_decl_line   : 1890
  <5458>     DW_AT_prototyped  : 1
  <5459>     DW_AT_type        : <1503>
  <545d>     DW_AT_low_pc      : 0x3f98
  <5461>     DW_AT_high_pc     : 0x40fc
  <5465>     DW_AT_frame_base  : 0x4ec  (location list)
  <5469>     DW_AT_sibling     : <54f0>
 <2><546d>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <546e>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5472>     DW_AT_decl_file   : 1
  <5473>     DW_AT_decl_line   : 1886
  <5475>     DW_AT_type        : <14d1>
  <5479>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><547c>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <547d>     DW_AT_name        : (indirect string, offset: 0x6e): pFunctionClient
  <5481>     DW_AT_decl_file   : 1
  <5482>     DW_AT_decl_line   : 1887
  <5484>     DW_AT_type        : <3f79>
  <5488>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><548b>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <548c>     DW_AT_name        : (indirect string, offset: 0x0): pHostClient
  <5490>     DW_AT_decl_file   : 1
  <5491>     DW_AT_decl_line   : 1888
  <5493>     DW_AT_type        : <3f73>
  <5497>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><549a>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <549b>     DW_AT_name        : (indirect string, offset: 0xe5): pOtgClient
  <549f>     DW_AT_decl_file   : 1
  <54a0>     DW_AT_decl_line   : 1889
  <54a2>     DW_AT_type        : <3f6d>
  <54a6>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><54a9>: Abbrev Number: 39 (DW_TAG_variable)
  <54aa>     DW_AT_name        : (indirect string, offset: 0x1b): pOtgDesc
  <54ae>     DW_AT_decl_file   : 1
  <54af>     DW_AT_decl_line   : 1891
  <54b1>     DW_AT_type        : <54f0>
  <54b5>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><54b8>: Abbrev Number: 39 (DW_TAG_variable)
  <54b9>     DW_AT_name        : (indirect string, offset: 0x30): dwStatus
  <54bd>     DW_AT_decl_file   : 1
  <54be>     DW_AT_decl_line   : 1892
  <54c0>     DW_AT_type        : <18e>
  <54c4>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><54c7>: Abbrev Number: 40 (DW_TAG_variable)
  <54c8>     DW_AT_name        : pResult
  <54d0>     DW_AT_decl_file   : 1
  <54d1>     DW_AT_decl_line   : 1893
  <54d3>     DW_AT_type        : <f1>
  <54d7>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><54da>: Abbrev Number: 40 (DW_TAG_variable)
  <54db>     DW_AT_name        : pImplPort
  <54e5>     DW_AT_decl_file   : 1
  <54e6>     DW_AT_decl_line   : 1894
  <54e8>     DW_AT_type        : <442a>
  <54ec>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><54f0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <54f1>     DW_AT_byte_size   : 4
  <54f2>     DW_AT_type        : <104c>
 <1><54f6>: Abbrev Number: 42 (DW_TAG_subprogram)
  <54f7>     DW_AT_external    : 1
  <54f8>     DW_AT_name        : MUSB_RelinquishHost
  <550c>     DW_AT_decl_file   : 1
  <550d>     DW_AT_decl_line   : 1947
  <550f>     DW_AT_prototyped  : 1
  <5510>     DW_AT_low_pc      : 0x40fc
  <5514>     DW_AT_high_pc     : 0x41cc
  <5518>     DW_AT_frame_base  : 0x516  (location list)
  <551c>     DW_AT_sibling     : <555e>
 <2><5520>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <5521>     DW_AT_name        : hBus
  <5526>     DW_AT_decl_file   : 1
  <5527>     DW_AT_decl_line   : 1946
  <5529>     DW_AT_type        : <1503>
  <552d>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><5530>: Abbrev Number: 39 (DW_TAG_variable)
  <5531>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <5535>     DW_AT_decl_file   : 1
  <5536>     DW_AT_decl_line   : 1948
  <5538>     DW_AT_type        : <442a>
  <553c>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><553f>: Abbrev Number: 39 (DW_TAG_variable)
  <5540>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <5544>     DW_AT_decl_file   : 1
  <5545>     DW_AT_decl_line   : 1949
  <5547>     DW_AT_type        : <3f67>
  <554b>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><554e>: Abbrev Number: 39 (DW_TAG_variable)
  <554f>     DW_AT_name        : (indirect string, offset: 0x1d4): pServices
  <5553>     DW_AT_decl_file   : 1
  <5554>     DW_AT_decl_line   : 1950
  <5556>     DW_AT_type        : <39f9>
  <555a>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><555e>: Abbrev Number: 36 (DW_TAG_subprogram)
  <555f>     DW_AT_external    : 1
  <5560>     DW_AT_name        : MUSB_RequestBus
  <5570>     DW_AT_decl_file   : 1
  <5571>     DW_AT_decl_line   : 1972
  <5573>     DW_AT_prototyped  : 1
  <5574>     DW_AT_type        : <160>
  <5578>     DW_AT_low_pc      : 0x41cc
  <557c>     DW_AT_high_pc     : 0x4254
  <5580>     DW_AT_frame_base  : 0x540  (location list)
  <5584>     DW_AT_sibling     : <55a8>
 <2><5588>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <5589>     DW_AT_name        : hBus
  <558e>     DW_AT_decl_file   : 1
  <558f>     DW_AT_decl_line   : 1971
  <5591>     DW_AT_type        : <1503>
  <5595>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><5598>: Abbrev Number: 39 (DW_TAG_variable)
  <5599>     DW_AT_name        : (indirect string, offset: 0x17a): pPort
  <559d>     DW_AT_decl_file   : 1
  <559e>     DW_AT_decl_line   : 1973
  <55a0>     DW_AT_type        : <442a>
  <55a4>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><55a8>: Abbrev Number: 36 (DW_TAG_subprogram)
  <55a9>     DW_AT_external    : 1
  <55aa>     DW_AT_name        : MGC_DrcDumpPipe
  <55ba>     DW_AT_decl_file   : 1
  <55bb>     DW_AT_decl_line   : 1995
  <55bd>     DW_AT_prototyped  : 1
  <55be>     DW_AT_type        : <a9>
  <55c2>     DW_AT_low_pc      : 0x4254
  <55c6>     DW_AT_high_pc     : 0x427c
  <55ca>     DW_AT_frame_base  : 0x56a  (location list)
  <55ce>     DW_AT_sibling     : <55f3>
 <2><55d2>: Abbrev Number: 37 (DW_TAG_formal_parameter)
  <55d3>     DW_AT_name        : (indirect string, offset: 0x111): pController
  <55d7>     DW_AT_decl_file   : 1
  <55d8>     DW_AT_decl_line   : 1994
  <55da>     DW_AT_type        : <3f67>
  <55de>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><55e1>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <55e2>     DW_AT_name        : pPipe
  <55e8>     DW_AT_decl_file   : 1
  <55e9>     DW_AT_decl_line   : 1994
  <55eb>     DW_AT_type        : <39ac>
  <55ef>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><55f3>: Abbrev Number: 28 (DW_TAG_array_type)
  <55f4>     DW_AT_type        : <160>
  <55f8>     DW_AT_sibling     : <5603>
 <2><55fc>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <55fd>     DW_AT_type        : <dd>
  <5601>     DW_AT_upper_bound : 7
 <1><5603>: Abbrev Number: 33 (DW_TAG_variable)
  <5604>     DW_AT_name        : MGC_aSetHnpSupportData
  <561b>     DW_AT_decl_file   : 1
  <561c>     DW_AT_decl_line   : 58
  <561d>     DW_AT_type        : <5627>
  <5621>     DW_AT_location    : 5 byte block: 3 0 0 0 0    (DW_OP_addr: 0)
 <1><5627>: Abbrev Number: 8 (DW_TAG_const_type)
  <5628>     DW_AT_type        : <55f3>
 <1><562c>: Abbrev Number: 28 (DW_TAG_array_type)
  <562d>     DW_AT_type        : <160>
  <5631>     DW_AT_sibling     : <563c>
 <2><5635>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <5636>     DW_AT_type        : <dd>
  <563a>     DW_AT_upper_bound : 7
 <1><563c>: Abbrev Number: 33 (DW_TAG_variable)
  <563d>     DW_AT_name        : MGC_aSetHnpEnableData
  <5653>     DW_AT_decl_file   : 1
  <5654>     DW_AT_decl_line   : 68
  <5655>     DW_AT_type        : <565f>
  <5659>     DW_AT_location    : 5 byte block: 3 8 0 0 0    (DW_OP_addr: 8)
 <1><565f>: Abbrev Number: 8 (DW_TAG_const_type)
  <5660>     DW_AT_type        : <562c>
 <1><5664>: Abbrev Number: 33 (DW_TAG_variable)
  <5665>     DW_AT_name        : MGC_SetHnpSupportIrp
  <567a>     DW_AT_decl_file   : 1
  <567b>     DW_AT_decl_line   : 82
  <567c>     DW_AT_type        : <1732>
  <5680>     DW_AT_location    : 5 byte block: 3 0 0 0 0    (DW_OP_addr: 0)
 <1><5686>: Abbrev Number: 33 (DW_TAG_variable)
  <5687>     DW_AT_name        : MGC_SetHnpEnableIrp
  <569b>     DW_AT_decl_file   : 1
  <569c>     DW_AT_decl_line   : 101
  <569d>     DW_AT_type        : <1732>
  <56a1>     DW_AT_location    : 5 byte block: 3 2c 0 0 0   (DW_OP_addr: 2c)
 <1><56a7>: Abbrev Number: 45 (DW_TAG_variable)
  <56a8>     DW_AT_name        : FAPI_USB_KeepVbusFlag
  <56be>     DW_AT_decl_file   : 14
  <56bf>     DW_AT_decl_line   : 35
  <56c0>     DW_AT_type        : <18e>
  <56c4>     DW_AT_external    : 1
  <56c5>     DW_AT_declaration : 1
 <1><56c6>: Abbrev Number: 28 (DW_TAG_array_type)
  <56c7>     DW_AT_type        : <10e>
  <56cb>     DW_AT_sibling     : <56d6>
 <2><56cf>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <56d0>     DW_AT_type        : <dd>
  <56d4>     DW_AT_upper_bound : 255
 <1><56d6>: Abbrev Number: 45 (DW_TAG_variable)
  <56d7>     DW_AT_name        : MGC_DiagMsg
  <56e3>     DW_AT_decl_file   : 12
  <56e4>     DW_AT_decl_line   : 66
  <56e5>     DW_AT_type        : <56c6>
  <56e9>     DW_AT_external    : 1
  <56ea>     DW_AT_declaration : 1
 <1><56eb>: Abbrev Number: 45 (DW_TAG_variable)
  <56ec>     DW_AT_name        : MGC_bDiagLevel
  <56fb>     DW_AT_decl_file   : 12
  <56fc>     DW_AT_decl_line   : 69
  <56fd>     DW_AT_type        : <160>
  <5701>     DW_AT_external    : 1
  <5702>     DW_AT_declaration : 1
 <1><5703>: Abbrev Number: 46 (DW_TAG_variable)
  <5704>     DW_AT_name        : FAPI_SYS_Services
  <5716>     DW_AT_decl_file   : 13
  <5717>     DW_AT_decl_line   : 1555
  <5719>     DW_AT_type        : <4370>
  <571d>     DW_AT_external    : 1
  <571e>     DW_AT_declaration : 1


Dump of debug contents of section .debug_line:

  Length:                      1355
  DWARF Version:               2
  Prologue Length:             351
  Minimum Instruction Length:  2
  Initial value of 'is_stmt':  1
  Line Base:                   -5
  Line Range:                  14
  Opcode Base:                 13

 Opcodes:
  Opcode 1 has 0 args
  Opcode 2 has 1 args
  Opcode 3 has 1 args
  Opcode 4 has 1 args
  Opcode 5 has 1 args
  Opcode 6 has 0 args
  Opcode 7 has 0 args
  Opcode 8 has 0 args
  Opcode 9 has 1 args
  Opcode 10 has 0 args
  Opcode 11 has 0 args
  Opcode 12 has 1 args

 The Directory Table:
  ../cd
  c:/PROGRA~1/GNUARM/lib/gcc/arm-elf/4.2.2/include
  c:/PROGRA~1/GNUARM/arm-elf/include
  ../../../../../install/MB86H60/include/fapi
  ../../include

 The File Name Table:
  Entry Dir Time    Size    Name
  1 1   0   0   mu_drc.c
  2 2   0   0   stddef.h
  3 3   0   0   stdint.h
  4 4   0   0   sys_driver.h
  5 4   0   0   drv_dma.h
  6 5   0   0   mu_dsi.h
  7 5   0   0   mu_dev.h
  8 5   0   0   mu_cdi.h
  9 5   0   0   mu_list.h
  10    5   0   0   mu_hset.h
  11    5   0   0   mu_dci.h
  12    1   0   0   mu_impl.h
  13    4   0   0   sys_services.h
  14    5   0   0   plat_cnf.h

 Line Number Statements:
  Extended opcode 2: set Address to 0x0
  Advance Line by 120 to 121
  Copy
  Special opcode 149: advance Address by 20 to 0x14 and Line by 4 to 125
  Special opcode 90: advance Address by 12 to 0x20 and Line by 1 to 126
  Special opcode 90: advance Address by 12 to 0x2c and Line by 1 to 127
  Special opcode 64: advance Address by 8 to 0x34 and Line by 3 to 130
  Special opcode 90: advance Address by 12 to 0x40 and Line by 1 to 131
  Special opcode 90: advance Address by 12 to 0x4c and Line by 1 to 132
  Special opcode 91: advance Address by 12 to 0x58 and Line by 2 to 134
  Special opcode 146: advance Address by 20 to 0x6c and Line by 1 to 135
  Special opcode 146: advance Address by 20 to 0x80 and Line by 1 to 136
  Special opcode 94: advance Address by 12 to 0x8c and Line by 5 to 141
  Special opcode 174: advance Address by 24 to 0xa4 and Line by 1 to 142
  Advance Line by -10 to 132
  Advance PC by constant 34 to 0xc6
  Special opcode 131: advance Address by 18 to 0xd8 and Line by 0 to 132
  Advance Line by 13 to 145
  Special opcode 173: advance Address by 24 to 0xf0 and Line by 0 to 145
  Advance PC by 84 to 0x144
  Special opcode 7: advance Address by 0 to 0x144 and Line by 2 to 147
  Special opcode 119: advance Address by 16 to 0x154 and Line by 2 to 149
  Advance PC by constant 34 to 0x176
  Special opcode 134: advance Address by 18 to 0x188 and Line by 3 to 152
  Special opcode 64: advance Address by 8 to 0x190 and Line by 3 to 155
  Special opcode 146: advance Address by 20 to 0x1a4 and Line by 1 to 156
  Special opcode 62: advance Address by 8 to 0x1ac and Line by 1 to 157
  Special opcode 118: advance Address by 16 to 0x1bc and Line by 1 to 158
  Special opcode 62: advance Address by 8 to 0x1c4 and Line by 1 to 159
  Special opcode 62: advance Address by 8 to 0x1cc and Line by 1 to 160
  Special opcode 118: advance Address by 16 to 0x1dc and Line by 1 to 161
  Special opcode 176: advance Address by 24 to 0x1f4 and Line by 3 to 164
  Special opcode 91: advance Address by 12 to 0x200 and Line by 2 to 166
  Special opcode 146: advance Address by 20 to 0x214 and Line by 1 to 167
  Special opcode 146: advance Address by 20 to 0x228 and Line by 1 to 168
  Special opcode 146: advance Address by 20 to 0x23c and Line by 1 to 169
  Special opcode 62: advance Address by 8 to 0x244 and Line by 1 to 170
  Special opcode 175: advance Address by 24 to 0x25c and Line by 2 to 172
  Special opcode 119: advance Address by 16 to 0x26c and Line by 2 to 174
  Special opcode 174: advance Address by 24 to 0x284 and Line by 1 to 175
  Special opcode 93: advance Address by 12 to 0x290 and Line by 4 to 179
  Special opcode 202: advance Address by 28 to 0x2ac and Line by 1 to 180
  Special opcode 63: advance Address by 8 to 0x2b4 and Line by 2 to 182
  Advance Line by -18 to 164
  Special opcode 173: advance Address by 24 to 0x2cc and Line by 0 to 164
  Advance Line by 23 to 187
  Advance PC by constant 34 to 0x2ee
  Special opcode 19: advance Address by 2 to 0x2f0 and Line by 0 to 187
  Special opcode 178: advance Address by 24 to 0x308 and Line by 5 to 192
  Special opcode 119: advance Address by 16 to 0x318 and Line by 2 to 194
  Special opcode 34: advance Address by 4 to 0x31c and Line by 1 to 195
  Special opcode 124: advance Address by 16 to 0x32c and Line by 7 to 202
  Advance PC by constant 34 to 0x34e
  Special opcode 52: advance Address by 6 to 0x354 and Line by 5 to 207
  Special opcode 62: advance Address by 8 to 0x35c and Line by 1 to 208
  Special opcode 90: advance Address by 12 to 0x368 and Line by 1 to 209
  Special opcode 92: advance Address by 12 to 0x374 and Line by 3 to 212
  Special opcode 90: advance Address by 12 to 0x380 and Line by 1 to 213
  Advance PC by constant 34 to 0x3a2
  Special opcode 21: advance Address by 2 to 0x3a4 and Line by 2 to 215
  Advance PC by constant 34 to 0x3c6
  Special opcode 78: advance Address by 10 to 0x3d0 and Line by 3 to 218
  Special opcode 94: advance Address by 12 to 0x3dc and Line by 5 to 223
  Special opcode 91: advance Address by 12 to 0x3e8 and Line by 2 to 225
  Advance PC by 132 to 0x46c
  Special opcode 6: advance Address by 0 to 0x46c and Line by 1 to 226
  Special opcode 175: advance Address by 24 to 0x484 and Line by 2 to 228
  Advance PC by 200 to 0x54c
  Special opcode 9: advance Address by 0 to 0x54c and Line by 4 to 232
  Special opcode 62: advance Address by 8 to 0x554 and Line by 1 to 233
  Special opcode 175: advance Address by 24 to 0x56c and Line by 2 to 235
  Special opcode 64: advance Address by 8 to 0x574 and Line by 3 to 238
  Special opcode 147: advance Address by 20 to 0x588 and Line by 2 to 240
  Special opcode 230: advance Address by 32 to 0x5a8 and Line by 1 to 241
  Special opcode 91: advance Address by 12 to 0x5b4 and Line by 2 to 243
  Special opcode 93: advance Address by 12 to 0x5c0 and Line by 4 to 247
  Special opcode 234: advance Address by 32 to 0x5e0 and Line by 5 to 252
  Special opcode 62: advance Address by 8 to 0x5e8 and Line by 1 to 253
  Special opcode 90: advance Address by 12 to 0x5f4 and Line by 1 to 254
  Special opcode 35: advance Address by 4 to 0x5f8 and Line by 2 to 256
  Special opcode 147: advance Address by 20 to 0x60c and Line by 2 to 258
  Advance PC by constant 34 to 0x62e
  Special opcode 48: advance Address by 6 to 0x634 and Line by 1 to 259
  Special opcode 91: advance Address by 12 to 0x640 and Line by 2 to 261
  Special opcode 93: advance Address by 12 to 0x64c and Line by 4 to 265
  Special opcode 90: advance Address by 12 to 0x658 and Line by 1 to 266
  Special opcode 232: advance Address by 32 to 0x678 and Line by 3 to 269
  Special opcode 90: advance Address by 12 to 0x684 and Line by 1 to 270
  Advance Line by -16 to 254
  Special opcode 89: advance Address by 12 to 0x690 and Line by 0 to 254
  Advance Line by 20 to 274
  Special opcode 89: advance Address by 12 to 0x69c and Line by 0 to 274
  Special opcode 62: advance Address by 8 to 0x6a4 and Line by 1 to 275
  Special opcode 175: advance Address by 24 to 0x6bc and Line by 2 to 277
  Special opcode 63: advance Address by 8 to 0x6c4 and Line by 2 to 279
  Special opcode 62: advance Address by 8 to 0x6cc and Line by 1 to 280
  Special opcode 90: advance Address by 12 to 0x6d8 and Line by 1 to 281
  Special opcode 35: advance Address by 4 to 0x6dc and Line by 2 to 283
  Special opcode 147: advance Address by 20 to 0x6f0 and Line by 2 to 285
  Advance PC by constant 34 to 0x712
  Special opcode 48: advance Address by 6 to 0x718 and Line by 1 to 286
  Special opcode 91: advance Address by 12 to 0x724 and Line by 2 to 288
  Special opcode 93: advance Address by 12 to 0x730 and Line by 4 to 292
  Special opcode 90: advance Address by 12 to 0x73c and Line by 1 to 293
  Special opcode 232: advance Address by 32 to 0x75c and Line by 3 to 296
  Special opcode 90: advance Address by 12 to 0x768 and Line by 1 to 297
  Advance Line by -16 to 281
  Special opcode 89: advance Address by 12 to 0x774 and Line by 0 to 281
  Advance Line by 19 to 300
  Special opcode 89: advance Address by 12 to 0x780 and Line by 0 to 300
  Special opcode 34: advance Address by 4 to 0x784 and Line by 1 to 301
  Advance Line by 11 to 312
  Special opcode 229: advance Address by 32 to 0x7a4 and Line by 0 to 312
  Advance Line by 12 to 324
  Special opcode 229: advance Address by 32 to 0x7c4 and Line by 0 to 324
  Special opcode 62: advance Address by 8 to 0x7cc and Line by 1 to 325
  Special opcode 62: advance Address by 8 to 0x7d4 and Line by 1 to 326
  Special opcode 62: advance Address by 8 to 0x7dc and Line by 1 to 327
  Special opcode 63: advance Address by 8 to 0x7e4 and Line by 2 to 329
  Advance PC by constant 34 to 0x806
  Special opcode 104: advance Address by 14 to 0x814 and Line by 1 to 330
  Special opcode 118: advance Address by 16 to 0x824 and Line by 1 to 331
  Advance PC by constant 34 to 0x846
  Special opcode 188: advance Address by 26 to 0x860 and Line by 1 to 332
  Special opcode 174: advance Address by 24 to 0x878 and Line by 1 to 333
  Special opcode 91: advance Address by 12 to 0x884 and Line by 2 to 335
  Special opcode 202: advance Address by 28 to 0x8a0 and Line by 1 to 336
  Advance PC by 92 to 0x8fc
  Special opcode 7: advance Address by 0 to 0x8fc and Line by 2 to 338
  Special opcode 175: advance Address by 24 to 0x914 and Line by 2 to 340
  Advance PC by constant 34 to 0x936
  Special opcode 104: advance Address by 14 to 0x944 and Line by 1 to 341
  Special opcode 120: advance Address by 16 to 0x954 and Line by 3 to 344
  Special opcode 118: advance Address by 16 to 0x964 and Line by 1 to 345
  Special opcode 146: advance Address by 20 to 0x978 and Line by 1 to 346
  Special opcode 92: advance Address by 12 to 0x984 and Line by 3 to 349
  Special opcode 148: advance Address by 20 to 0x998 and Line by 3 to 352
  Special opcode 62: advance Address by 8 to 0x9a0 and Line by 1 to 353
  Special opcode 64: advance Address by 8 to 0x9a8 and Line by 3 to 356
  Special opcode 148: advance Address by 20 to 0x9bc and Line by 3 to 359
  Special opcode 62: advance Address by 8 to 0x9c4 and Line by 1 to 360
  Special opcode 96: advance Address by 12 to 0x9d0 and Line by 7 to 367
  Special opcode 118: advance Address by 16 to 0x9e0 and Line by 1 to 368
  Advance PC by constant 34 to 0xa02
  Special opcode 23: advance Address by 2 to 0xa04 and Line by 4 to 372
  Special opcode 62: advance Address by 8 to 0xa0c and Line by 1 to 373
  Advance Line by -40 to 333
  Special opcode 33: advance Address by 4 to 0xa10 and Line by 0 to 333
  Advance Line by 47 to 380
  Special opcode 201: advance Address by 28 to 0xa2c and Line by 0 to 380
  Special opcode 91: advance Address by 12 to 0xa38 and Line by 2 to 382
  Special opcode 65: advance Address by 8 to 0xa40 and Line by 4 to 386
  Special opcode 175: advance Address by 24 to 0xa58 and Line by 2 to 388
  Special opcode 203: advance Address by 28 to 0xa74 and Line by 2 to 390
  Special opcode 87: advance Address by 12 to 0xa80 and Line by -2 to 388
  Special opcode 39: advance Address by 4 to 0xa84 and Line by 6 to 394
  Special opcode 91: advance Address by 12 to 0xa90 and Line by 2 to 396
  Special opcode 90: advance Address by 12 to 0xa9c and Line by 1 to 397
  Special opcode 90: advance Address by 12 to 0xaa8 and Line by 1 to 398
  Special opcode 91: advance Address by 12 to 0xab4 and Line by 2 to 400
  Special opcode 146: advance Address by 20 to 0xac8 and Line by 1 to 401
  Special opcode 118: advance Address by 16 to 0xad8 and Line by 1 to 402
  Special opcode 118: advance Address by 16 to 0xae8 and Line by 1 to 403
  Special opcode 118: advance Address by 16 to 0xaf8 and Line by 1 to 404
  Special opcode 121: advance Address by 16 to 0xb08 and Line by 4 to 408
  Special opcode 146: advance Address by 20 to 0xb1c and Line by 1 to 409
  Special opcode 118: advance Address by 16 to 0xb2c and Line by 1 to 410
  Special opcode 118: advance Address by 16 to 0xb3c and Line by 1 to 411
  Special opcode 118: advance Address by 16 to 0xb4c and Line by 1 to 412
  Special opcode 93: advance Address by 12 to 0xb58 and Line by 4 to 416
  Special opcode 90: advance Address by 12 to 0xb64 and Line by 1 to 417
  Special opcode 35: advance Address by 4 to 0xb68 and Line by 2 to 419
  Special opcode 96: advance Address by 12 to 0xb74 and Line by 7 to 426
  Special opcode 233: advance Address by 32 to 0xb94 and Line by 4 to 430
  Special opcode 90: advance Address by 12 to 0xba0 and Line by 1 to 431
  Special opcode 62: advance Address by 8 to 0xba8 and Line by 1 to 432
  Special opcode 62: advance Address by 8 to 0xbb0 and Line by 1 to 433
  Special opcode 62: advance Address by 8 to 0xbb8 and Line by 1 to 434
  Special opcode 203: advance Address by 28 to 0xbd4 and Line by 2 to 436
  Special opcode 119: advance Address by 16 to 0xbe4 and Line by 2 to 438
  Advance PC by 72 to 0xc2c
  Special opcode 8: advance Address by 0 to 0xc2c and Line by 3 to 441
  Special opcode 90: advance Address by 12 to 0xc38 and Line by 1 to 442
  Advance PC by constant 34 to 0xc5a
  Special opcode 21: advance Address by 2 to 0xc5c and Line by 2 to 444
  Special opcode 62: advance Address by 8 to 0xc64 and Line by 1 to 445
  Special opcode 93: advance Address by 12 to 0xc70 and Line by 4 to 449
  Advance PC by constant 34 to 0xc92
  Special opcode 78: advance Address by 10 to 0xc9c and Line by 3 to 452
  Special opcode 62: advance Address by 8 to 0xca4 and Line by 1 to 453
  Special opcode 119: advance Address by 16 to 0xcb4 and Line by 2 to 455
  Special opcode 90: advance Address by 12 to 0xcc0 and Line by 1 to 456
  Special opcode 149: advance Address by 20 to 0xcd4 and Line by 4 to 460
  Special opcode 90: advance Address by 12 to 0xce0 and Line by 1 to 461
  Special opcode 90: advance Address by 12 to 0xcec and Line by 1 to 462
  Special opcode 152: advance Address by 20 to 0xd00 and Line by 7 to 469
  Special opcode 147: advance Address by 20 to 0xd14 and Line by 2 to 471
  Special opcode 91: advance Address by 12 to 0xd20 and Line by 2 to 473
  Advance PC by 72 to 0xd68
  Special opcode 7: advance Address by 0 to 0xd68 and Line by 2 to 475
  Special opcode 147: advance Address by 20 to 0xd7c and Line by 2 to 477
  Special opcode 232: advance Address by 32 to 0xd9c and Line by 3 to 480
  Special opcode 114: advance Address by 16 to 0xdac and Line by -3 to 477
  Special opcode 41: advance Address by 4 to 0xdb0 and Line by 8 to 485
  Special opcode 231: advance Address by 32 to 0xdd0 and Line by 2 to 487
  Advance PC by 124 to 0xe4c
  Special opcode 11: advance Address by 0 to 0xe4c and Line by 6 to 493
  Special opcode 90: advance Address by 12 to 0xe58 and Line by 1 to 494
  Special opcode 92: advance Address by 12 to 0xe64 and Line by 3 to 497
  Special opcode 230: advance Address by 32 to 0xe84 and Line by 1 to 498
  Advance PC by constant 34 to 0xea6
  Special opcode 21: advance Address by 2 to 0xea8 and Line by 2 to 500
  Special opcode 119: advance Address by 16 to 0xeb8 and Line by 2 to 502
  Advance PC by 80 to 0xf08
  Special opcode 8: advance Address by 0 to 0xf08 and Line by 3 to 505
  Special opcode 146: advance Address by 20 to 0xf1c and Line by 1 to 506
  Special opcode 174: advance Address by 24 to 0xf34 and Line by 1 to 507
  Special opcode 118: advance Address by 16 to 0xf44 and Line by 1 to 508
  Special opcode 63: advance Address by 8 to 0xf4c and Line by 2 to 510
  Special opcode 120: advance Address by 16 to 0xf5c and Line by 3 to 513
  Special opcode 204: advance Address by 28 to 0xf78 and Line by 3 to 516
  Advance PC by 92 to 0xfd4
  Special opcode 11: advance Address by 0 to 0xfd4 and Line by 6 to 522
  Special opcode 179: advance Address by 24 to 0xfec and Line by 6 to 528
  Special opcode 91: advance Address by 12 to 0xff8 and Line by 2 to 530
  Special opcode 118: advance Address by 16 to 0x1008 and Line by 1 to 531
  Special opcode 118: advance Address by 16 to 0x1018 and Line by 1 to 532
  Special opcode 177: advance Address by 24 to 0x1030 and Line by 4 to 536
  Special opcode 180: advance Address by 24 to 0x1048 and Line by 7 to 543
  Advance PC by 92 to 0x10a4
  Special opcode 11: advance Address by 0 to 0x10a4 and Line by 6 to 549
  Advance Line by -47 to 502
  Special opcode 145: advance Address by 20 to 0x10b8 and Line by 0 to 502
  Advance Line by 54 to 556
  Special opcode 33: advance Address by 4 to 0x10bc and Line by 0 to 556
  Special opcode 93: advance Address by 12 to 0x10c8 and Line by 4 to 560
  Special opcode 120: advance Address by 16 to 0x10d8 and Line by 3 to 563
  Advance PC by constant 34 to 0x10fa
  Special opcode 76: advance Address by 10 to 0x1104 and Line by 1 to 564
  Special opcode 92: advance Address by 12 to 0x1110 and Line by 3 to 567
  Special opcode 118: advance Address by 16 to 0x1120 and Line by 1 to 568
  Special opcode 118: advance Address by 16 to 0x1130 and Line by 1 to 569
  Special opcode 91: advance Address by 12 to 0x113c and Line by 2 to 571
  Special opcode 203: advance Address by 28 to 0x1158 and Line by 2 to 573
  Advance PC by 124 to 0x11d4
  Special opcode 11: advance Address by 0 to 0x11d4 and Line by 6 to 579
  Special opcode 90: advance Address by 12 to 0x11e0 and Line by 1 to 580
  Special opcode 93: advance Address by 12 to 0x11ec and Line by 4 to 584
  Special opcode 230: advance Address by 32 to 0x120c and Line by 1 to 585
  Advance PC by constant 34 to 0x122e
  Special opcode 20: advance Address by 2 to 0x1230 and Line by 1 to 586
  Special opcode 118: advance Address by 16 to 0x1240 and Line by 1 to 587
  Special opcode 231: advance Address by 32 to 0x1260 and Line by 2 to 589
  Special opcode 118: advance Address by 16 to 0x1270 and Line by 1 to 590
  Special opcode 118: advance Address by 16 to 0x1280 and Line by 1 to 591
  Special opcode 122: advance Address by 16 to 0x1290 and Line by 5 to 596
  Special opcode 118: advance Address by 16 to 0x12a0 and Line by 1 to 597
  Special opcode 90: advance Address by 12 to 0x12ac and Line by 1 to 598
  Special opcode 149: advance Address by 20 to 0x12c0 and Line by 4 to 602
  Special opcode 146: advance Address by 20 to 0x12d4 and Line by 1 to 603
  Special opcode 92: advance Address by 12 to 0x12e0 and Line by 3 to 606
  Advance PC by constant 34 to 0x1302
  Special opcode 23: advance Address by 2 to 0x1304 and Line by 4 to 610
  Special opcode 90: advance Address by 12 to 0x1310 and Line by 1 to 611
  Special opcode 90: advance Address by 12 to 0x131c and Line by 1 to 612
  Special opcode 120: advance Address by 16 to 0x132c and Line by 3 to 615
  Special opcode 120: advance Address by 16 to 0x133c and Line by 3 to 618
  Special opcode 232: advance Address by 32 to 0x135c and Line by 3 to 621
  Special opcode 63: advance Address by 8 to 0x1364 and Line by 2 to 623
  Special opcode 120: advance Address by 16 to 0x1374 and Line by 3 to 626
  Special opcode 119: advance Address by 16 to 0x1384 and Line by 2 to 628
  Special opcode 90: advance Address by 12 to 0x1390 and Line by 1 to 629
  Special opcode 90: advance Address by 12 to 0x139c and Line by 1 to 630
  Special opcode 91: advance Address by 12 to 0x13a8 and Line by 2 to 632
  Special opcode 90: advance Address by 12 to 0x13b4 and Line by 1 to 633
  Special opcode 63: advance Address by 8 to 0x13bc and Line by 2 to 635
  Special opcode 34: advance Address by 4 to 0x13c0 and Line by 1 to 636
  Special opcode 97: advance Address by 12 to 0x13cc and Line by 8 to 644
  Special opcode 152: advance Address by 20 to 0x13e0 and Line by 7 to 651
  Special opcode 120: advance Address by 16 to 0x13f0 and Line by 3 to 654
  Special opcode 174: advance Address by 24 to 0x1408 and Line by 1 to 655
  Special opcode 91: advance Address by 12 to 0x1414 and Line by 2 to 657
  Special opcode 202: advance Address by 28 to 0x1430 and Line by 1 to 658
  Special opcode 91: advance Address by 12 to 0x143c and Line by 2 to 660
  Special opcode 202: advance Address by 28 to 0x1458 and Line by 1 to 661
  Special opcode 202: advance Address by 28 to 0x1474 and Line by 1 to 662
  Special opcode 203: advance Address by 28 to 0x1490 and Line by 2 to 664
  Special opcode 203: advance Address by 28 to 0x14ac and Line by 2 to 666
  Special opcode 202: advance Address by 28 to 0x14c8 and Line by 1 to 667
  Special opcode 202: advance Address by 28 to 0x14e4 and Line by 1 to 668
  Special opcode 174: advance Address by 24 to 0x14fc and Line by 1 to 669
  Special opcode 91: advance Address by 12 to 0x1508 and Line by 2 to 671
  Special opcode 202: advance Address by 28 to 0x1524 and Line by 1 to 672
  Special opcode 142: advance Address by 20 to 0x1538 and Line by -3 to 669
  Special opcode 206: advance Address by 28 to 0x1554 and Line by 5 to 674
  Special opcode 174: advance Address by 24 to 0x156c and Line by 1 to 675
  Special opcode 91: advance Address by 12 to 0x1578 and Line by 2 to 677
  Special opcode 202: advance Address by 28 to 0x1594 and Line by 1 to 678
  Special opcode 142: advance Address by 20 to 0x15a8 and Line by -3 to 675
  Advance Line by -20 to 655
  Special opcode 201: advance Address by 28 to 0x15c4 and Line by 0 to 655
  Advance Line by 29 to 684
  Special opcode 201: advance Address by 28 to 0x15e0 and Line by 0 to 684
  Special opcode 119: advance Address by 16 to 0x15f0 and Line by 2 to 686
  Special opcode 65: advance Address by 8 to 0x15f8 and Line by 4 to 690
  Special opcode 202: advance Address by 28 to 0x1614 and Line by 1 to 691
  Special opcode 62: advance Address by 8 to 0x161c and Line by 1 to 692
  Special opcode 90: advance Address by 12 to 0x1628 and Line by 1 to 693
  Special opcode 91: advance Address by 12 to 0x1634 and Line by 2 to 695
  Special opcode 118: advance Address by 16 to 0x1644 and Line by 1 to 696
  Advance PC by constant 34 to 0x1666
  Special opcode 134: advance Address by 18 to 0x1678 and Line by 3 to 699
  Special opcode 119: advance Address by 16 to 0x1688 and Line by 2 to 701
  Advance PC by constant 34 to 0x16aa
  Special opcode 189: advance Address by 26 to 0x16c4 and Line by 2 to 703
  Advance PC by constant 34 to 0x16e6
  Special opcode 78: advance Address by 10 to 0x16f0 and Line by 3 to 706
  Special opcode 91: advance Address by 12 to 0x16fc and Line by 2 to 708
  Special opcode 37: advance Address by 4 to 0x1700 and Line by 4 to 712
  Special opcode 118: advance Address by 16 to 0x1710 and Line by 1 to 713
  Special opcode 92: advance Address by 12 to 0x171c and Line by 3 to 716
  Special opcode 123: advance Address by 16 to 0x172c and Line by 6 to 722
  Special opcode 202: advance Address by 28 to 0x1748 and Line by 1 to 723
  Special opcode 92: advance Address by 12 to 0x1754 and Line by 3 to 726
  Special opcode 91: advance Address by 12 to 0x1760 and Line by 2 to 728
  Special opcode 92: advance Address by 12 to 0x176c and Line by 3 to 731
  Special opcode 91: advance Address by 12 to 0x1778 and Line by 2 to 733
  Special opcode 231: advance Address by 32 to 0x1798 and Line by 2 to 735
  Special opcode 91: advance Address by 12 to 0x17a4 and Line by 2 to 737
  Special opcode 90: advance Address by 12 to 0x17b0 and Line by 1 to 738
  Advance Line by 10 to 748
  Special opcode 89: advance Address by 12 to 0x17bc and Line by 0 to 748
  Special opcode 202: advance Address by 28 to 0x17d8 and Line by 1 to 749
  Special opcode 91: advance Address by 12 to 0x17e4 and Line by 2 to 751
  Special opcode 64: advance Address by 8 to 0x17ec and Line by 3 to 754
  Special opcode 90: advance Address by 12 to 0x17f8 and Line by 1 to 755
  Special opcode 119: advance Address by 16 to 0x1808 and Line by 2 to 757
  Special opcode 64: advance Address by 8 to 0x1810 and Line by 3 to 760
  Special opcode 208: advance Address by 28 to 0x182c and Line by 7 to 767
  Special opcode 68: advance Address by 8 to 0x1834 and Line by 7 to 774
  Advance Line by 11 to 785
  Special opcode 145: advance Address by 20 to 0x1848 and Line by 0 to 785
  Special opcode 39: advance Address by 4 to 0x184c and Line by 6 to 791
  Special opcode 202: advance Address by 28 to 0x1868 and Line by 1 to 792
  Special opcode 62: advance Address by 8 to 0x1870 and Line by 1 to 793
  Special opcode 90: advance Address by 12 to 0x187c and Line by 1 to 794
  Special opcode 91: advance Address by 12 to 0x1888 and Line by 2 to 796
  Special opcode 231: advance Address by 32 to 0x18a8 and Line by 2 to 798
  Special opcode 35: advance Address by 4 to 0x18ac and Line by 2 to 800
  Advance PC by constant 34 to 0x18ce
  Special opcode 190: advance Address by 26 to 0x18e8 and Line by 3 to 803
  Advance PC by constant 34 to 0x190a
  Special opcode 79: advance Address by 10 to 0x1914 and Line by 4 to 807
  Special opcode 91: advance Address by 12 to 0x1920 and Line by 2 to 809
  Special opcode 125: advance Address by 16 to 0x1930 and Line by 8 to 817
  Special opcode 202: advance Address by 28 to 0x194c and Line by 1 to 818
  Special opcode 62: advance Address by 8 to 0x1954 and Line by 1 to 819
  Special opcode 90: advance Address by 12 to 0x1960 and Line by 1 to 820
  Special opcode 91: advance Address by 12 to 0x196c and Line by 2 to 822
  Special opcode 231: advance Address by 32 to 0x198c and Line by 2 to 824
  Special opcode 35: advance Address by 4 to 0x1990 and Line by 2 to 826
  Advance PC by constant 34 to 0x19b2
  Special opcode 190: advance Address by 26 to 0x19cc and Line by 3 to 829
  Advance PC by constant 34 to 0x19ee
  Special opcode 79: advance Address by 10 to 0x19f8 and Line by 4 to 833
  Special opcode 91: advance Address by 12 to 0x1a04 and Line by 2 to 835
  Special opcode 125: advance Address by 16 to 0x1a14 and Line by 8 to 843
  Special opcode 202: advance Address by 28 to 0x1a30 and Line by 1 to 844
  Special opcode 62: advance Address by 8 to 0x1a38 and Line by 1 to 845
  Special opcode 91: advance Address by 12 to 0x1a44 and Line by 2 to 847
  Advance PC by constant 34 to 0x1a66
  Special opcode 133: advance Address by 18 to 0x1a78 and Line by 2 to 849
  Special opcode 90: advance Address by 12 to 0x1a84 and Line by 1 to 850
  Special opcode 90: advance Address by 12 to 0x1a90 and Line by 1 to 851
  Special opcode 119: advance Address by 16 to 0x1aa0 and Line by 2 to 853
  Advance PC by constant 34 to 0x1ac2
  Special opcode 135: advance Address by 18 to 0x1ad4 and Line by 4 to 857
  Special opcode 151: advance Address by 20 to 0x1ae8 and Line by 6 to 863
  Special opcode 203: advance Address by 28 to 0x1b04 and Line by 2 to 865
  Special opcode 62: advance Address by 8 to 0x1b0c and Line by 1 to 866
  Special opcode 62: advance Address by 8 to 0x1b14 and Line by 1 to 867
  Special opcode 91: advance Address by 12 to 0x1b20 and Line by 2 to 869
  Advance PC by constant 34 to 0x1b42
  Special opcode 133: advance Address by 18 to 0x1b54 and Line by 2 to 871
  Special opcode 90: advance Address by 12 to 0x1b60 and Line by 1 to 872
  Special opcode 119: advance Address by 16 to 0x1b70 and Line by 2 to 874
  Special opcode 118: advance Address by 16 to 0x1b80 and Line by 1 to 875
  Special opcode 119: advance Address by 16 to 0x1b90 and Line by 2 to 877
  Special opcode 91: advance Address by 12 to 0x1b9c and Line by 2 to 879
  Special opcode 119: advance Address by 16 to 0x1bac and Line by 2 to 881
  Special opcode 65: advance Address by 8 to 0x1bb4 and Line by 4 to 885
  Special opcode 174: advance Address by 24 to 0x1bcc and Line by 1 to 886
  Special opcode 91: advance Address by 12 to 0x1bd8 and Line by 2 to 888
  Advance PC by constant 34 to 0x1bfa
  Special opcode 105: advance Address by 14 to 0x1c08 and Line by 2 to 890
  Advance PC by constant 34 to 0x1c2a
  Special opcode 164: advance Address by 22 to 0x1c40 and Line by 5 to 895
  Advance PC by constant 34 to 0x1c62
  Special opcode 134: advance Address by 18 to 0x1c74 and Line by 3 to 898
  Special opcode 181: advance Address by 24 to 0x1c8c and Line by 8 to 906
  Special opcode 202: advance Address by 28 to 0x1ca8 and Line by 1 to 907
  Special opcode 62: advance Address by 8 to 0x1cb0 and Line by 1 to 908
  Special opcode 90: advance Address by 12 to 0x1cbc and Line by 1 to 909
  Special opcode 91: advance Address by 12 to 0x1cc8 and Line by 2 to 911
  Special opcode 235: advance Address by 32 to 0x1ce8 and Line by 6 to 917
  Special opcode 119: advance Address by 16 to 0x1cf8 and Line by 2 to 919
  Advance PC by constant 34 to 0x1d1a
  Special opcode 191: advance Address by 26 to 0x1d34 and Line by 4 to 923
  Advance PC by constant 34 to 0x1d56
  Special opcode 109: advance Address by 14 to 0x1d64 and Line by 6 to 929
  Special opcode 90: advance Address by 12 to 0x1d70 and Line by 1 to 930
  Special opcode 90: advance Address by 12 to 0x1d7c and Line by 1 to 931
  Special opcode 119: advance Address by 16 to 0x1d8c and Line by 2 to 933
  Advance PC by constant 34 to 0x1dae
  Special opcode 190: advance Address by 26 to 0x1dc8 and Line by 3 to 936
  Advance PC by constant 34 to 0x1dea
  Special opcode 79: advance Address by 10 to 0x1df4 and Line by 4 to 940
  Special opcode 92: advance Address by 12 to 0x1e00 and Line by 3 to 943
  Special opcode 149: advance Address by 20 to 0x1e14 and Line by 4 to 947
  Special opcode 202: advance Address by 28 to 0x1e30 and Line by 1 to 948
  Special opcode 62: advance Address by 8 to 0x1e38 and Line by 1 to 949
  Special opcode 90: advance Address by 12 to 0x1e44 and Line by 1 to 950
  Special opcode 91: advance Address by 12 to 0x1e50 and Line by 2 to 952
  Special opcode 119: advance Address by 16 to 0x1e60 and Line by 2 to 954
  Advance PC by constant 34 to 0x1e82
  Special opcode 190: advance Address by 26 to 0x1e9c and Line by 3 to 957
  Advance PC by constant 34 to 0x1ebe
  Special opcode 79: advance Address by 10 to 0x1ec8 and Line by 4 to 961
  Special opcode 90: advance Address by 12 to 0x1ed4 and Line by 1 to 962
  Special opcode 119: advance Address by 16 to 0x1ee4 and Line by 2 to 964
  Special opcode 92: advance Address by 12 to 0x1ef0 and Line by 3 to 967
  Special opcode 121: advance Address by 16 to 0x1f00 and Line by 4 to 971
  Special opcode 175: advance Address by 24 to 0x1f18 and Line by 2 to 973
  Special opcode 65: advance Address by 8 to 0x1f20 and Line by 4 to 977
  Special opcode 174: advance Address by 24 to 0x1f38 and Line by 1 to 978
  Special opcode 62: advance Address by 8 to 0x1f40 and Line by 1 to 979
  Special opcode 119: advance Address by 16 to 0x1f50 and Line by 2 to 981
  Special opcode 119: advance Address by 16 to 0x1f60 and Line by 2 to 983
  Special opcode 90: advance Address by 12 to 0x1f6c and Line by 1 to 984
  Special opcode 118: advance Address by 16 to 0x1f7c and Line by 1 to 985
  Advance PC by constant 34 to 0x1f9e
  Special opcode 108: advance Address by 14 to 0x1fac and Line by 5 to 990
  Special opcode 91: advance Address by 12 to 0x1fb8 and Line by 2 to 992
  Special opcode 92: advance Address by 12 to 0x1fc4 and Line by 3 to 995
  Special opcode 202: advance Address by 28 to 0x1fe0 and Line by 1 to 996
  Special opcode 62: advance Address by 8 to 0x1fe8 and Line by 1 to 997
  Special opcode 91: advance Address by 12 to 0x1ff4 and Line by 2 to 999
  Special opcode 90: advance Address by 12 to 0x2000 and Line by 1 to 1000
  Special opcode 118: advance Address by 16 to 0x2010 and Line by 1 to 1001
  Special opcode 67: advance Address by 8 to 0x2018 and Line by 6 to 1007
  Special opcode 203: advance Address by 28 to 0x2034 and Line by 2 to 1009
  Special opcode 120: advance Address by 16 to 0x2044 and Line by 3 to 1012
  Special opcode 204: advance Address by 28 to 0x2060 and Line by 3 to 1015
  Special opcode 120: advance Address by 16 to 0x2070 and Line by 3 to 1018
  Special opcode 206: advance Address by 28 to 0x208c and Line by 5 to 1023
  Advance PC by constant 34 to 0x20ae
  Special opcode 104: advance Address by 14 to 0x20bc and Line by 1 to 1024
  Special opcode 119: advance Address by 16 to 0x20cc and Line by 2 to 1026
  Advance PC by 164 to 0x2170
  Special opcode 9: advance Address by 0 to 0x2170 and Line by 4 to 1030
  Special opcode 90: advance Address by 12 to 0x217c and Line by 1 to 1031
  Special opcode 90: advance Address by 12 to 0x2188 and Line by 1 to 1032
  Special opcode 118: advance Address by 16 to 0x2198 and Line by 1 to 1033
  Special opcode 36: advance Address by 4 to 0x219c and Line by 3 to 1036
  Special opcode 90: advance Address by 12 to 0x21a8 and Line by 1 to 1037
  Special opcode 119: advance Address by 16 to 0x21b8 and Line by 2 to 1039
  Special opcode 119: advance Address by 16 to 0x21c8 and Line by 2 to 1041
  Advance PC by constant 34 to 0x21ea
  Special opcode 77: advance Address by 10 to 0x21f4 and Line by 2 to 1043
  Special opcode 36: advance Address by 4 to 0x21f8 and Line by 3 to 1046
  Special opcode 90: advance Address by 12 to 0x2204 and Line by 1 to 1047
  Special opcode 119: advance Address by 16 to 0x2214 and Line by 2 to 1049
  Special opcode 119: advance Address by 16 to 0x2224 and Line by 2 to 1051
  Advance PC by constant 34 to 0x2246
  Special opcode 77: advance Address by 10 to 0x2250 and Line by 2 to 1053
  Special opcode 41: advance Address by 4 to 0x2254 and Line by 8 to 1061
  Advance PC by constant 34 to 0x2276
  Special opcode 77: advance Address by 10 to 0x2280 and Line by 2 to 1063
  Advance Line by 9 to 1072
  Special opcode 33: advance Address by 4 to 0x2284 and Line by 0 to 1072
  Special opcode 119: advance Address by 16 to 0x2294 and Line by 2 to 1074
  Advance PC by constant 34 to 0x22b6
  Special opcode 78: advance Address by 10 to 0x22c0 and Line by 3 to 1077
  Special opcode 41: advance Address by 4 to 0x22c4 and Line by 8 to 1085
  Special opcode 90: advance Address by 12 to 0x22d0 and Line by 1 to 1086
  Special opcode 118: advance Address by 16 to 0x22e0 and Line by 1 to 1087
  Advance PC by constant 34 to 0x2302
  Special opcode 77: advance Address by 10 to 0x230c and Line by 2 to 1089
  Special opcode 36: advance Address by 4 to 0x2310 and Line by 3 to 1092
  Special opcode 119: advance Address by 16 to 0x2320 and Line by 2 to 1094
  Advance PC by 108 to 0x238c
  Special opcode 9: advance Address by 0 to 0x238c and Line by 4 to 1098
  Special opcode 231: advance Address by 32 to 0x23ac and Line by 2 to 1100
  Special opcode 118: advance Address by 16 to 0x23bc and Line by 1 to 1101
  Special opcode 90: advance Address by 12 to 0x23c8 and Line by 1 to 1102
  Special opcode 141: advance Address by 20 to 0x23dc and Line by -4 to 1098
  Special opcode 41: advance Address by 4 to 0x23e0 and Line by 8 to 1106
  Special opcode 64: advance Address by 8 to 0x23e8 and Line by 3 to 1109
  Special opcode 36: advance Address by 4 to 0x23ec and Line by 3 to 1112
  Advance PC by 148 to 0x2480
  Special opcode 6: advance Address by 0 to 0x2480 and Line by 1 to 1113
  Special opcode 36: advance Address by 4 to 0x2484 and Line by 3 to 1116
  Advance PC by 232 to 0x256c
  Special opcode 8: advance Address by 0 to 0x256c and Line by 3 to 1119
  Special opcode 91: advance Address by 12 to 0x2578 and Line by 2 to 1121
  Special opcode 63: advance Address by 8 to 0x2580 and Line by 2 to 1123
  Advance PC by constant 34 to 0x25a2
  Special opcode 194: advance Address by 26 to 0x25bc and Line by 7 to 1130
  Special opcode 146: advance Address by 20 to 0x25d0 and Line by 1 to 1131
  Special opcode 91: advance Address by 12 to 0x25dc and Line by 2 to 1133
  Special opcode 147: advance Address by 20 to 0x25f0 and Line by 2 to 1135
  Special opcode 64: advance Address by 8 to 0x25f8 and Line by 3 to 1138
  Special opcode 91: advance Address by 12 to 0x2604 and Line by 2 to 1140
  Advance PC by constant 34 to 0x2626
  Special opcode 51: advance Address by 6 to 0x262c and Line by 4 to 1144
  Special opcode 65: advance Address by 8 to 0x2634 and Line by 4 to 1148
  Special opcode 207: advance Address by 28 to 0x2650 and Line by 6 to 1154
  Special opcode 62: advance Address by 8 to 0x2658 and Line by 1 to 1155
  Special opcode 91: advance Address by 12 to 0x2664 and Line by 2 to 1157
  Special opcode 90: advance Address by 12 to 0x2670 and Line by 1 to 1158
  Special opcode 120: advance Address by 16 to 0x2680 and Line by 3 to 1161
  Special opcode 174: advance Address by 24 to 0x2698 and Line by 1 to 1162
  Special opcode 91: advance Address by 12 to 0x26a4 and Line by 2 to 1164
  Special opcode 230: advance Address by 32 to 0x26c4 and Line by 1 to 1165
  Special opcode 91: advance Address by 12 to 0x26d0 and Line by 2 to 1167
  Special opcode 90: advance Address by 12 to 0x26dc and Line by 1 to 1168
  Special opcode 91: advance Address by 12 to 0x26e8 and Line by 2 to 1170
  Special opcode 90: advance Address by 12 to 0x26f4 and Line by 1 to 1171
  Special opcode 119: advance Address by 16 to 0x2704 and Line by 2 to 1173
  Advance Line by -11 to 1162
  Special opcode 201: advance Address by 28 to 0x2720 and Line by 0 to 1162
  Advance Line by 17 to 1179
  Special opcode 201: advance Address by 28 to 0x273c and Line by 0 to 1179
  Advance Line by 9 to 1188
  Special opcode 61: advance Address by 8 to 0x2744 and Line by 0 to 1188
  Special opcode 207: advance Address by 28 to 0x2760 and Line by 6 to 1194
  Special opcode 62: advance Address by 8 to 0x2768 and Line by 1 to 1195
  Special opcode 119: advance Address by 16 to 0x2778 and Line by 2 to 1197
  Special opcode 91: advance Address by 12 to 0x2784 and Line by 2 to 1199
  Special opcode 119: advance Address by 16 to 0x2794 and Line by 2 to 1201
  Special opcode 90: advance Address by 12 to 0x27a0 and Line by 1 to 1202
  Special opcode 90: advance Address by 12 to 0x27ac and Line by 1 to 1203
  Special opcode 62: advance Address by 8 to 0x27b4 and Line by 1 to 1204
  Special opcode 231: advance Address by 32 to 0x27d4 and Line by 2 to 1206
  Special opcode 119: advance Address by 16 to 0x27e4 and Line by 2 to 1208
  Special opcode 90: advance Address by 12 to 0x27f0 and Line by 1 to 1209
  Special opcode 62: advance Address by 8 to 0x27f8 and Line by 1 to 1210
  Special opcode 231: advance Address by 32 to 0x2818 and Line by 2 to 1212
  Special opcode 119: advance Address by 16 to 0x2828 and Line by 2 to 1214
  Special opcode 90: advance Address by 12 to 0x2834 and Line by 1 to 1215
  Special opcode 62: advance Address by 8 to 0x283c and Line by 1 to 1216
  Special opcode 231: advance Address by 32 to 0x285c and Line by 2 to 1218
  Special opcode 147: advance Address by 20 to 0x2870 and Line by 2 to 1220
  Special opcode 90: advance Address by 12 to 0x287c and Line by 1 to 1221
  Special opcode 62: advance Address by 8 to 0x2884 and Line by 1 to 1222
  Special opcode 231: advance Address by 32 to 0x28a4 and Line by 2 to 1224
  Special opcode 119: advance Address by 16 to 0x28b4 and Line by 2 to 1226
  Special opcode 90: advance Address by 12 to 0x28c0 and Line by 1 to 1227
  Special opcode 90: advance Address by 12 to 0x28cc and Line by 1 to 1228
  Special opcode 62: advance Address by 8 to 0x28d4 and Line by 1 to 1229
  Special opcode 231: advance Address by 32 to 0x28f4 and Line by 2 to 1231
  Special opcode 120: advance Address by 16 to 0x2904 and Line by 3 to 1234
  Special opcode 119: advance Address by 16 to 0x2914 and Line by 2 to 1236
  Special opcode 62: advance Address by 8 to 0x291c and Line by 1 to 1237
  Special opcode 90: advance Address by 12 to 0x2928 and Line by 1 to 1238
  Special opcode 90: advance Address by 12 to 0x2934 and Line by 1 to 1239
  Special opcode 62: advance Address by 8 to 0x293c and Line by 1 to 1240
  Special opcode 232: advance Address by 32 to 0x295c and Line by 3 to 1243
  Special opcode 124: advance Address by 16 to 0x296c and Line by 7 to 1250
  Special opcode 93: advance Address by 12 to 0x2978 and Line by 4 to 1254
  Special opcode 118: advance Address by 16 to 0x2988 and Line by 1 to 1255
  Special opcode 120: advance Address by 16 to 0x2998 and Line by 3 to 1258
  Special opcode 90: advance Address by 12 to 0x29a4 and Line by 1 to 1259
  Special opcode 62: advance Address by 8 to 0x29ac and Line by 1 to 1260
  Advance PC by constant 34 to 0x29ce
  Special opcode 22: advance Address by 2 to 0x29d0 and Line by 3 to 1263
  Special opcode 92: advance Address by 12 to 0x29dc and Line by 3 to 1266
  Special opcode 120: advance Address by 16 to 0x29ec and Line by 3 to 1269
  Special opcode 62: advance Address by 8 to 0x29f4 and Line by 1 to 1270
  Special opcode 90: advance Address by 12 to 0x2a00 and Line by 1 to 1271
  Special opcode 90: advance Address by 12 to 0x2a0c and Line by 1 to 1272
  Special opcode 62: advance Address by 8 to 0x2a14 and Line by 1 to 1273
  Advance PC by constant 34 to 0x2a36
  Special opcode 27: advance Address by 2 to 0x2a38 and Line by 8 to 1281
  Special opcode 118: advance Address by 16 to 0x2a48 and Line by 1 to 1282
  Special opcode 90: advance Address by 12 to 0x2a54 and Line by 1 to 1283
  Special opcode 90: advance Address by 12 to 0x2a60 and Line by 1 to 1284
  Special opcode 118: advance Address by 16 to 0x2a70 and Line by 1 to 1285
  Special opcode 118: advance Address by 16 to 0x2a80 and Line by 1 to 1286
  Special opcode 62: advance Address by 8 to 0x2a88 and Line by 1 to 1287
  Special opcode 90: advance Address by 12 to 0x2a94 and Line by 1 to 1288
  Special opcode 62: advance Address by 8 to 0x2a9c and Line by 1 to 1289
  Advance PC by constant 34 to 0x2abe
  Special opcode 50: advance Address by 6 to 0x2ac4 and Line by 3 to 1292
  Special opcode 90: advance Address by 12 to 0x2ad0 and Line by 1 to 1293
  Special opcode 35: advance Address by 4 to 0x2ad4 and Line by 2 to 1295
  Special opcode 94: advance Address by 12 to 0x2ae0 and Line by 5 to 1300
  Special opcode 120: advance Address by 16 to 0x2af0 and Line by 3 to 1303
  Special opcode 120: advance Address by 16 to 0x2b00 and Line by 3 to 1306
  Special opcode 174: advance Address by 24 to 0x2b18 and Line by 1 to 1307
  Special opcode 91: advance Address by 12 to 0x2b24 and Line by 2 to 1309
  Special opcode 203: advance Address by 28 to 0x2b40 and Line by 2 to 1311
  Advance PC by constant 34 to 0x2b62
  Special opcode 78: advance Address by 10 to 0x2b6c and Line by 3 to 1314
  Special opcode 90: advance Address by 12 to 0x2b78 and Line by 1 to 1315
  Advance PC by constant 34 to 0x2b9a
  Special opcode 21: advance Address by 2 to 0x2b9c and Line by 2 to 1317
  Advance Line by -10 to 1307
  Advance PC by constant 34 to 0x2bbe
  Special opcode 131: advance Address by 18 to 0x2bd0 and Line by 0 to 1307
  Advance Line by 17 to 1324
  Special opcode 201: advance Address by 28 to 0x2bec and Line by 0 to 1324
  Special opcode 119: advance Address by 16 to 0x2bfc and Line by 2 to 1326
  Special opcode 66: advance Address by 8 to 0x2c04 and Line by 5 to 1331
  Special opcode 34: advance Address by 4 to 0x2c08 and Line by 1 to 1332
  Special opcode 152: advance Address by 20 to 0x2c1c and Line by 7 to 1339
  Advance Line by 12 to 1351
  Special opcode 145: advance Address by 20 to 0x2c30 and Line by 0 to 1351
  Special opcode 62: advance Address by 8 to 0x2c38 and Line by 1 to 1352
  Special opcode 90: advance Address by 12 to 0x2c44 and Line by 1 to 1353
  Special opcode 90: advance Address by 12 to 0x2c50 and Line by 1 to 1354
  Special opcode 90: advance Address by 12 to 0x2c5c and Line by 1 to 1355
  Special opcode 95: advance Address by 12 to 0x2c68 and Line by 6 to 1361
  Advance PC by constant 34 to 0x2c8a
  Special opcode 48: advance Address by 6 to 0x2c90 and Line by 1 to 1362
  Special opcode 35: advance Address by 4 to 0x2c94 and Line by 2 to 1364
  Advance PC by 164 to 0x2d38
  Special opcode 12: advance Address by 0 to 0x2d38 and Line by 7 to 1371
  Advance PC by constant 34 to 0x2d5a
  Special opcode 132: advance Address by 18 to 0x2d6c and Line by 1 to 1372
  Special opcode 119: advance Address by 16 to 0x2d7c and Line by 2 to 1374
  Advance PC by constant 34 to 0x2d9e
  Special opcode 106: advance Address by 14 to 0x2dac and Line by 3 to 1377
  Advance PC by constant 34 to 0x2dce
  Special opcode 51: advance Address by 6 to 0x2dd4 and Line by 4 to 1381
  Special opcode 90: advance Address by 12 to 0x2de0 and Line by 1 to 1382
  Special opcode 37: advance Address by 4 to 0x2de4 and Line by 4 to 1386
  Special opcode 91: advance Address by 12 to 0x2df0 and Line by 2 to 1388
  Special opcode 90: advance Address by 12 to 0x2dfc and Line by 1 to 1389
  Special opcode 118: advance Address by 16 to 0x2e0c and Line by 1 to 1390
  Advance PC by constant 34 to 0x2e2e
  Special opcode 136: advance Address by 18 to 0x2e40 and Line by 5 to 1395
  Special opcode 36: advance Address by 4 to 0x2e44 and Line by 3 to 1398
  Advance PC by constant 34 to 0x2e66
  Special opcode 133: advance Address by 18 to 0x2e78 and Line by 2 to 1400
  Special opcode 120: advance Address by 16 to 0x2e88 and Line by 3 to 1403
  Special opcode 90: advance Address by 12 to 0x2e94 and Line by 1 to 1404
  Special opcode 90: advance Address by 12 to 0x2ea0 and Line by 1 to 1405
  Special opcode 119: advance Address by 16 to 0x2eb0 and Line by 2 to 1407
  Special opcode 37: advance Address by 4 to 0x2eb4 and Line by 4 to 1411
  Advance PC by constant 34 to 0x2ed6
  Special opcode 132: advance Address by 18 to 0x2ee8 and Line by 1 to 1412
  Special opcode 119: advance Address by 16 to 0x2ef8 and Line by 2 to 1414
  Special opcode 174: advance Address by 24 to 0x2f10 and Line by 1 to 1415
  Advance PC by constant 34 to 0x2f32
  Special opcode 132: advance Address by 18 to 0x2f44 and Line by 1 to 1416
  Special opcode 90: advance Address by 12 to 0x2f50 and Line by 1 to 1417
  Special opcode 121: advance Address by 16 to 0x2f60 and Line by 4 to 1421
  Advance PC by constant 34 to 0x2f82
  Special opcode 77: advance Address by 10 to 0x2f8c and Line by 2 to 1423
  Special opcode 91: advance Address by 12 to 0x2f98 and Line by 2 to 1425
  Special opcode 39: advance Address by 4 to 0x2f9c and Line by 6 to 1431
  Advance PC by constant 34 to 0x2fbe
  Special opcode 133: advance Address by 18 to 0x2fd0 and Line by 2 to 1433
  Special opcode 119: advance Address by 16 to 0x2fe0 and Line by 2 to 1435
  Advance PC by constant 34 to 0x3002
  Special opcode 50: advance Address by 6 to 0x3008 and Line by 3 to 1438
  Advance PC by constant 34 to 0x302a
  Special opcode 134: advance Address by 18 to 0x303c and Line by 3 to 1441
  Special opcode 90: advance Address by 12 to 0x3048 and Line by 1 to 1442
  Special opcode 119: advance Address by 16 to 0x3058 and Line by 2 to 1444
  Special opcode 91: advance Address by 12 to 0x3064 and Line by 2 to 1446
  Special opcode 36: advance Address by 4 to 0x3068 and Line by 3 to 1449
  Special opcode 90: advance Address by 12 to 0x3074 and Line by 1 to 1450
  Special opcode 90: advance Address by 12 to 0x3080 and Line by 1 to 1451
  Special opcode 36: advance Address by 4 to 0x3084 and Line by 3 to 1454
  Special opcode 91: advance Address by 12 to 0x3090 and Line by 2 to 1456
  Special opcode 36: advance Address by 4 to 0x3094 and Line by 3 to 1459
  Advance PC by constant 34 to 0x30b6
  Special opcode 132: advance Address by 18 to 0x30c8 and Line by 1 to 1460
  Special opcode 119: advance Address by 16 to 0x30d8 and Line by 2 to 1462
  Special opcode 90: advance Address by 12 to 0x30e4 and Line by 1 to 1463
  Special opcode 232: advance Address by 32 to 0x3104 and Line by 3 to 1466
  Special opcode 86: advance Address by 12 to 0x3110 and Line by -3 to 1463
  Special opcode 40: advance Address by 4 to 0x3114 and Line by 7 to 1470
  Special opcode 91: advance Address by 12 to 0x3120 and Line by 2 to 1472
  Special opcode 36: advance Address by 4 to 0x3124 and Line by 3 to 1475
  Advance PC by constant 34 to 0x3146
  Special opcode 133: advance Address by 18 to 0x3158 and Line by 2 to 1477
  Special opcode 92: advance Address by 12 to 0x3164 and Line by 3 to 1480
  Special opcode 63: advance Address by 8 to 0x316c and Line by 2 to 1482
  Advance PC by constant 34 to 0x318e
  Special opcode 134: advance Address by 18 to 0x31a0 and Line by 3 to 1485
  Special opcode 118: advance Address by 16 to 0x31b0 and Line by 1 to 1486
  Special opcode 90: advance Address by 12 to 0x31bc and Line by 1 to 1487
  Special opcode 36: advance Address by 4 to 0x31c0 and Line by 3 to 1490
  Special opcode 231: advance Address by 32 to 0x31e0 and Line by 2 to 1492
  Special opcode 119: advance Address by 16 to 0x31f0 and Line by 2 to 1494
  Special opcode 85: advance Address by 12 to 0x31fc and Line by -4 to 1490
  Special opcode 41: advance Address by 4 to 0x3200 and Line by 8 to 1498
  Special opcode 91: advance Address by 12 to 0x320c and Line by 2 to 1500
  Special opcode 36: advance Address by 4 to 0x3210 and Line by 3 to 1503
  Special opcode 118: advance Address by 16 to 0x3220 and Line by 1 to 1504
  Special opcode 92: advance Address by 12 to 0x322c and Line by 3 to 1507
  Special opcode 93: advance Address by 12 to 0x3238 and Line by 4 to 1511
  Special opcode 205: advance Address by 28 to 0x3254 and Line by 4 to 1515
  Special opcode 90: advance Address by 12 to 0x3260 and Line by 1 to 1516
  Advance PC by constant 34 to 0x3282
  Special opcode 104: advance Address by 14 to 0x3290 and Line by 1 to 1517
  Special opcode 230: advance Address by 32 to 0x32b0 and Line by 1 to 1518
  Advance PC by 72 to 0x32f8
  Special opcode 2: advance Address by 0 to 0x32f8 and Line by -3 to 1515
  Special opcode 208: advance Address by 28 to 0x3314 and Line by 7 to 1522
  Special opcode 36: advance Address by 4 to 0x3318 and Line by 3 to 1525
  Advance PC by constant 34 to 0x333a
  Special opcode 132: advance Address by 18 to 0x334c and Line by 1 to 1526
  Special opcode 119: advance Address by 16 to 0x335c and Line by 2 to 1528
  Special opcode 120: advance Address by 16 to 0x336c and Line by 3 to 1531
  Special opcode 119: advance Address by 16 to 0x337c and Line by 2 to 1533
  Special opcode 174: advance Address by 24 to 0x3394 and Line by 1 to 1534
  Advance PC by constant 34 to 0x33b6
  Special opcode 132: advance Address by 18 to 0x33c8 and Line by 1 to 1535
  Special opcode 90: advance Address by 12 to 0x33d4 and Line by 1 to 1536
  Special opcode 121: advance Address by 16 to 0x33e4 and Line by 4 to 1540
  Advance PC by constant 34 to 0x3406
  Special opcode 132: advance Address by 18 to 0x3418 and Line by 1 to 1541
  Special opcode 118: advance Address by 16 to 0x3428 and Line by 1 to 1542
  Special opcode 118: advance Address by 16 to 0x3438 and Line by 1 to 1543
  Special opcode 92: advance Address by 12 to 0x3444 and Line by 3 to 1546
  Special opcode 37: advance Address by 4 to 0x3448 and Line by 4 to 1550
  Advance Line by 17 to 1567
  Special opcode 229: advance Address by 32 to 0x3468 and Line by 0 to 1567
  Special opcode 91: advance Address by 12 to 0x3474 and Line by 2 to 1569
  Advance PC by constant 34 to 0x3496
  Special opcode 133: advance Address by 18 to 0x34a8 and Line by 2 to 1571
  Special opcode 233: advance Address by 32 to 0x34c8 and Line by 4 to 1575
  Special opcode 62: advance Address by 8 to 0x34d0 and Line by 1 to 1576
  Special opcode 203: advance Address by 28 to 0x34ec and Line by 2 to 1578
  Special opcode 203: advance Address by 28 to 0x3508 and Line by 2 to 1580
  Special opcode 36: advance Address by 4 to 0x350c and Line by 3 to 1583
  Special opcode 62: advance Address by 8 to 0x3514 and Line by 1 to 1584
  Special opcode 203: advance Address by 28 to 0x3530 and Line by 2 to 1586
  Special opcode 204: advance Address by 28 to 0x354c and Line by 3 to 1589
  Special opcode 36: advance Address by 4 to 0x3550 and Line by 3 to 1592
  Special opcode 62: advance Address by 8 to 0x3558 and Line by 1 to 1593
  Special opcode 203: advance Address by 28 to 0x3574 and Line by 2 to 1595
  Special opcode 207: advance Address by 28 to 0x3590 and Line by 6 to 1601
  Special opcode 91: advance Address by 12 to 0x359c and Line by 2 to 1603
  Special opcode 231: advance Address by 32 to 0x35bc and Line by 2 to 1605
  Special opcode 150: advance Address by 20 to 0x35d0 and Line by 5 to 1610
  Advance PC by constant 34 to 0x35f2
  Special opcode 106: advance Address by 14 to 0x3600 and Line by 3 to 1613
  Special opcode 124: advance Address by 16 to 0x3610 and Line by 7 to 1620
  Advance Line by -258 to 1362
  Advance PC by constant 34 to 0x3632
  Special opcode 47: advance Address by 6 to 0x3638 and Line by 0 to 1362
  Advance Line by 261 to 1623
  Special opcode 89: advance Address by 12 to 0x3644 and Line by 0 to 1623
  Advance PC by 68 to 0x3688
  Special opcode 13: advance Address by 0 to 0x3688 and Line by 8 to 1631
  Advance PC by constant 34 to 0x36aa
  Special opcode 20: advance Address by 2 to 0x36ac and Line by 1 to 1632
  Special opcode 62: advance Address by 8 to 0x36b4 and Line by 1 to 1633
  Special opcode 62: advance Address by 8 to 0x36bc and Line by 1 to 1634
  Special opcode 62: advance Address by 8 to 0x36c4 and Line by 1 to 1635
  Special opcode 202: advance Address by 28 to 0x36e0 and Line by 1 to 1636
  Special opcode 90: advance Address by 12 to 0x36ec and Line by 1 to 1637
  Special opcode 92: advance Address by 12 to 0x36f8 and Line by 3 to 1640
  Special opcode 91: advance Address by 12 to 0x3704 and Line by 2 to 1642
  Special opcode 90: advance Address by 12 to 0x3710 and Line by 1 to 1643
  Special opcode 92: advance Address by 12 to 0x371c and Line by 3 to 1646
  Special opcode 120: advance Address by 16 to 0x372c and Line by 3 to 1649
  Special opcode 174: advance Address by 24 to 0x3744 and Line by 1 to 1650
  Advance PC by constant 34 to 0x3766
  Special opcode 21: advance Address by 2 to 0x3768 and Line by 2 to 1652
  Advance PC by 124 to 0x37e4
  Special opcode 11: advance Address by 0 to 0x37e4 and Line by 6 to 1658
  Special opcode 90: advance Address by 12 to 0x37f0 and Line by 1 to 1659
  Special opcode 93: advance Address by 12 to 0x37fc and Line by 4 to 1663
  Special opcode 230: advance Address by 32 to 0x381c and Line by 1 to 1664
  Special opcode 174: advance Address by 24 to 0x3834 and Line by 1 to 1665
  Advance PC by constant 34 to 0x3856
  Special opcode 77: advance Address by 10 to 0x3860 and Line by 2 to 1667
  Special opcode 95: advance Address by 12 to 0x386c and Line by 6 to 1673
  Special opcode 202: advance Address by 28 to 0x3888 and Line by 1 to 1674
  Special opcode 90: advance Address by 12 to 0x3894 and Line by 1 to 1675
  Special opcode 148: advance Address by 20 to 0x38a8 and Line by 3 to 1678
  Special opcode 91: advance Address by 12 to 0x38b4 and Line by 2 to 1680
  Special opcode 119: advance Address by 16 to 0x38c4 and Line by 2 to 1682
  Special opcode 202: advance Address by 28 to 0x38e0 and Line by 1 to 1683
  Advance PC by 68 to 0x3924
  Special opcode 8: advance Address by 0 to 0x3924 and Line by 3 to 1686
  Special opcode 119: advance Address by 16 to 0x3934 and Line by 2 to 1688
  Special opcode 118: advance Address by 16 to 0x3944 and Line by 1 to 1689
  Advance PC by 120 to 0x39bc
  Special opcode 10: advance Address by 0 to 0x39bc and Line by 5 to 1694
  Special opcode 202: advance Address by 28 to 0x39d8 and Line by 1 to 1695
  Special opcode 94: advance Address by 12 to 0x39e4 and Line by 5 to 1700
  Special opcode 93: advance Address by 12 to 0x39f0 and Line by 4 to 1704
  Special opcode 174: advance Address by 24 to 0x3a08 and Line by 1 to 1705
  Advance PC by constant 34 to 0x3a2a
  Special opcode 50: advance Address by 6 to 0x3a30 and Line by 3 to 1708
  Advance PC by 124 to 0x3aac
  Special opcode 11: advance Address by 0 to 0x3aac and Line by 6 to 1714
  Special opcode 90: advance Address by 12 to 0x3ab8 and Line by 1 to 1715
  Special opcode 93: advance Address by 12 to 0x3ac4 and Line by 4 to 1719
  Special opcode 91: advance Address by 12 to 0x3ad0 and Line by 2 to 1721
  Special opcode 230: advance Address by 32 to 0x3af0 and Line by 1 to 1722
  Special opcode 90: advance Address by 12 to 0x3afc and Line by 1 to 1723
  Special opcode 66: advance Address by 8 to 0x3b04 and Line by 5 to 1728
  Special opcode 91: advance Address by 12 to 0x3b10 and Line by 2 to 1730
  Advance PC by constant 34 to 0x3b32
  Special opcode 163: advance Address by 22 to 0x3b48 and Line by 4 to 1734
  Special opcode 91: advance Address by 12 to 0x3b54 and Line by 2 to 1736
  Special opcode 90: advance Address by 12 to 0x3b60 and Line by 1 to 1737
  Advance Line by 9 to 1746
  Special opcode 89: advance Address by 12 to 0x3b6c and Line by 0 to 1746
  Special opcode 203: advance Address by 28 to 0x3b88 and Line by 2 to 1748
  Special opcode 62: advance Address by 8 to 0x3b90 and Line by 1 to 1749
  Special opcode 92: advance Address by 12 to 0x3b9c and Line by 3 to 1752
  Advance PC by constant 34 to 0x3bbe
  Special opcode 51: advance Address by 6 to 0x3bc4 and Line by 4 to 1756
  Advance PC by constant 34 to 0x3be6
  Special opcode 77: advance Address by 10 to 0x3bf0 and Line by 2 to 1758
  Special opcode 65: advance Address by 8 to 0x3bf8 and Line by 4 to 1762
  Special opcode 231: advance Address by 32 to 0x3c18 and Line by 2 to 1764
  Advance PC by constant 34 to 0x3c3a
  Special opcode 77: advance Address by 10 to 0x3c44 and Line by 2 to 1766
  Special opcode 65: advance Address by 8 to 0x3c4c and Line by 4 to 1770
  Special opcode 175: advance Address by 24 to 0x3c64 and Line by 2 to 1772
  Advance PC by constant 34 to 0x3c86
  Special opcode 77: advance Address by 10 to 0x3c90 and Line by 2 to 1774
  Special opcode 64: advance Address by 8 to 0x3c98 and Line by 3 to 1777
  Special opcode 92: advance Address by 12 to 0x3ca4 and Line by 3 to 1780
  Special opcode 231: advance Address by 32 to 0x3cc4 and Line by 2 to 1782
  Advance PC by 108 to 0x3d30
  Special opcode 9: advance Address by 0 to 0x3d30 and Line by 4 to 1786
  Special opcode 231: advance Address by 32 to 0x3d50 and Line by 2 to 1788
  Special opcode 118: advance Address by 16 to 0x3d60 and Line by 1 to 1789
  Special opcode 90: advance Address by 12 to 0x3d6c and Line by 1 to 1790
  Special opcode 179: advance Address by 24 to 0x3d84 and Line by 6 to 1796
  Special opcode 120: advance Address by 16 to 0x3d94 and Line by 3 to 1799
  Special opcode 64: advance Address by 8 to 0x3d9c and Line by 3 to 1802
  Special opcode 34: advance Address by 4 to 0x3da0 and Line by 1 to 1803
  Special opcode 176: advance Address by 24 to 0x3db8 and Line by 3 to 1806
  Special opcode 146: advance Address by 20 to 0x3dcc and Line by 1 to 1807
  Special opcode 63: advance Address by 8 to 0x3dd4 and Line by 2 to 1809
  Special opcode 118: advance Address by 16 to 0x3de4 and Line by 1 to 1810
  Special opcode 120: advance Address by 16 to 0x3df4 and Line by 3 to 1813
  Special opcode 63: advance Address by 8 to 0x3dfc and Line by 2 to 1815
  Advance PC by constant 34 to 0x3e1e
  Special opcode 190: advance Address by 26 to 0x3e38 and Line by 3 to 1818
  Special opcode 96: advance Address by 12 to 0x3e44 and Line by 7 to 1825
  Special opcode 202: advance Address by 28 to 0x3e60 and Line by 1 to 1826
  Special opcode 62: advance Address by 8 to 0x3e68 and Line by 1 to 1827
  Special opcode 91: advance Address by 12 to 0x3e74 and Line by 2 to 1829
  Special opcode 118: advance Address by 16 to 0x3e84 and Line by 1 to 1830
  Special opcode 119: advance Address by 16 to 0x3e94 and Line by 2 to 1832
  Special opcode 119: advance Address by 16 to 0x3ea4 and Line by 2 to 1834
  Special opcode 93: advance Address by 12 to 0x3eb0 and Line by 4 to 1838
  Special opcode 175: advance Address by 24 to 0x3ec8 and Line by 2 to 1840
  Special opcode 34: advance Address by 4 to 0x3ecc and Line by 1 to 1841
  Special opcode 95: advance Address by 12 to 0x3ed8 and Line by 6 to 1847
  Special opcode 202: advance Address by 28 to 0x3ef4 and Line by 1 to 1848
  Special opcode 90: advance Address by 12 to 0x3f00 and Line by 1 to 1849
  Special opcode 90: advance Address by 12 to 0x3f0c and Line by 1 to 1850
  Advance Line by 9 to 1859
  Special opcode 117: advance Address by 16 to 0x3f1c and Line by 0 to 1859
  Special opcode 119: advance Address by 16 to 0x3f2c and Line by 2 to 1861
  Special opcode 119: advance Address by 16 to 0x3f3c and Line by 2 to 1863
  Special opcode 119: advance Address by 16 to 0x3f4c and Line by 2 to 1865
  Special opcode 91: advance Address by 12 to 0x3f58 and Line by 2 to 1867
  Special opcode 121: advance Address by 16 to 0x3f68 and Line by 4 to 1871
  Special opcode 123: advance Address by 16 to 0x3f78 and Line by 6 to 1877
  Special opcode 90: advance Address by 12 to 0x3f84 and Line by 1 to 1878
  Special opcode 92: advance Address by 12 to 0x3f90 and Line by 3 to 1881
  Advance Line by 9 to 1890
  Special opcode 61: advance Address by 8 to 0x3f98 and Line by 0 to 1890
  Special opcode 231: advance Address by 32 to 0x3fb8 and Line by 2 to 1892
  Special opcode 62: advance Address by 8 to 0x3fc0 and Line by 1 to 1893
  Special opcode 62: advance Address by 8 to 0x3fc8 and Line by 1 to 1894
  Special opcode 92: advance Address by 12 to 0x3fd4 and Line by 3 to 1897
  Special opcode 120: advance Address by 16 to 0x3fe4 and Line by 3 to 1900
  Special opcode 91: advance Address by 12 to 0x3ff0 and Line by 2 to 1902
  Special opcode 92: advance Address by 12 to 0x3ffc and Line by 3 to 1905
  Special opcode 147: advance Address by 20 to 0x4010 and Line by 2 to 1907
  Special opcode 92: advance Address by 12 to 0x401c and Line by 3 to 1910
  Special opcode 90: advance Address by 12 to 0x4028 and Line by 1 to 1911
  Special opcode 91: advance Address by 12 to 0x4034 and Line by 2 to 1913
  Advance PC by constant 34 to 0x4056
  Special opcode 79: advance Address by 10 to 0x4060 and Line by 4 to 1917
  Special opcode 91: advance Address by 12 to 0x406c and Line by 2 to 1919
  Special opcode 232: advance Address by 32 to 0x408c and Line by 3 to 1922
  Special opcode 90: advance Address by 12 to 0x4098 and Line by 1 to 1923
  Special opcode 92: advance Address by 12 to 0x40a4 and Line by 3 to 1926
  Special opcode 120: advance Address by 16 to 0x40b4 and Line by 3 to 1929
  Special opcode 90: advance Address by 12 to 0x40c0 and Line by 1 to 1930
  Special opcode 90: advance Address by 12 to 0x40cc and Line by 1 to 1931
  Special opcode 91: advance Address by 12 to 0x40d8 and Line by 2 to 1933
  Special opcode 92: advance Address by 12 to 0x40e4 and Line by 3 to 1936
  Special opcode 65: advance Address by 8 to 0x40ec and Line by 4 to 1940
  Special opcode 34: advance Address by 4 to 0x40f0 and Line by 1 to 1941
  Special opcode 95: advance Address by 12 to 0x40fc and Line by 6 to 1947
  Special opcode 146: advance Address by 20 to 0x4110 and Line by 1 to 1948
  Special opcode 62: advance Address by 8 to 0x4118 and Line by 1 to 1949
  Special opcode 90: advance Address by 12 to 0x4124 and Line by 1 to 1950
  Special opcode 91: advance Address by 12 to 0x4130 and Line by 2 to 1952
  Special opcode 90: advance Address by 12 to 0x413c and Line by 1 to 1953
  Special opcode 119: advance Address by 16 to 0x414c and Line by 2 to 1955
  Special opcode 122: advance Address by 16 to 0x415c and Line by 5 to 1960
  Special opcode 118: advance Address by 16 to 0x416c and Line by 1 to 1961
  Special opcode 90: advance Address by 12 to 0x4178 and Line by 1 to 1962
  Special opcode 119: advance Address by 16 to 0x4188 and Line by 2 to 1964
  Special opcode 91: advance Address by 12 to 0x4194 and Line by 2 to 1966
  Advance PC by constant 34 to 0x41b6
  Special opcode 78: advance Address by 10 to 0x41c0 and Line by 3 to 1969
  Special opcode 92: advance Address by 12 to 0x41cc and Line by 3 to 1972
  Special opcode 146: advance Address by 20 to 0x41e0 and Line by 1 to 1973
  Special opcode 63: advance Address by 8 to 0x41e8 and Line by 2 to 1975
  Special opcode 148: advance Address by 20 to 0x41fc and Line by 3 to 1978
  Special opcode 90: advance Address by 12 to 0x4208 and Line by 1 to 1979
  Special opcode 91: advance Address by 12 to 0x4214 and Line by 2 to 1981
  Special opcode 119: advance Address by 16 to 0x4224 and Line by 2 to 1983
  Special opcode 90: advance Address by 12 to 0x4230 and Line by 1 to 1984
  Special opcode 91: advance Address by 12 to 0x423c and Line by 2 to 1986
  Special opcode 90: advance Address by 12 to 0x4248 and Line by 1 to 1987
  Special opcode 97: advance Address by 12 to 0x4254 and Line by 8 to 1995
  Special opcode 175: advance Address by 24 to 0x426c and Line by 2 to 1997
  Special opcode 34: advance Address by 4 to 0x4270 and Line by 1 to 1998
  Advance PC by 12 to 0x427c
  Extended opcode 1: End of Sequence


The section .debug_frame contains:

00000000 0000000c ffffffff CIE
  Version:               1
  Augmentation:          ""
  Code alignment factor: 1
  Data alignment factor: -4
  Return address column: 14

  DW_CFA_def_cfa: r13 ofs 0

00000010 0000001c 00000000 FDE cie=00000000 pc=00000000..0000032c
  DW_CFA_advance_loc: 4 to 00000004
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000008
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 0000000c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000030 0000001c 00000000 FDE cie=00000000 pc=0000032c..000007a4
  DW_CFA_advance_loc: 4 to 00000330
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000334
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00000338
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000050 0000001c 00000000 FDE cie=00000000 pc=000007a4..00000b74
  DW_CFA_advance_loc: 4 to 000007a8
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000007ac
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000007b0
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000070 0000001c 00000000 FDE cie=00000000 pc=00000b74..000012c0
  DW_CFA_advance_loc: 4 to 00000b78
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000b7c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 00000b80
  DW_CFA_def_cfa: r11 ofs 4

00000090 0000001c 00000000 FDE cie=00000000 pc=000012c0..000013cc
  DW_CFA_advance_loc: 4 to 000012c4
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000012c8
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000012cc
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000b0 0000001c 00000000 FDE cie=00000000 pc=000013cc..000015f8
  DW_CFA_advance_loc: 4 to 000013d0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000013d4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000013d8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000d0 0000001c 00000000 FDE cie=00000000 pc=000015f8..0000172c
  DW_CFA_advance_loc: 4 to 000015fc
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001600
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001604
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000f0 0000001c 00000000 FDE cie=00000000 pc=0000172c..000017bc
  DW_CFA_advance_loc: 4 to 00001730
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001734
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001738
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000110 0000001c 00000000 FDE cie=00000000 pc=000017bc..00001810
  DW_CFA_advance_loc: 4 to 000017c0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000017c4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000017c8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000130 0000001c 00000000 FDE cie=00000000 pc=00001810..00001834
  DW_CFA_advance_loc: 4 to 00001814
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001818
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 0000181c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000150 0000001c 00000000 FDE cie=00000000 pc=00001834..0000184c
  DW_CFA_advance_loc: 4 to 00001838
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000183c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001840
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000170 0000001c 00000000 FDE cie=00000000 pc=0000184c..00001930
  DW_CFA_advance_loc: 4 to 00001850
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001854
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001858
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000190 0000001c 00000000 FDE cie=00000000 pc=00001930..00001a14
  DW_CFA_advance_loc: 4 to 00001934
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001938
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 0000193c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001b0 0000001c 00000000 FDE cie=00000000 pc=00001a14..00001ae8
  DW_CFA_advance_loc: 4 to 00001a18
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001a1c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001a20
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001d0 0000001c 00000000 FDE cie=00000000 pc=00001ae8..00001c8c
  DW_CFA_advance_loc: 4 to 00001aec
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001af0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001af4
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001f0 0000001c 00000000 FDE cie=00000000 pc=00001c8c..00001e14
  DW_CFA_advance_loc: 4 to 00001c90
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001c94
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001c98
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000210 0000001c 00000000 FDE cie=00000000 pc=00001e14..00001f00
  DW_CFA_advance_loc: 4 to 00001e18
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001e1c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001e20
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000230 0000001c 00000000 FDE cie=00000000 pc=00001f00..00001f20
  DW_CFA_advance_loc: 4 to 00001f04
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001f08
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001f0c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000250 0000001c 00000000 FDE cie=00000000 pc=00001f20..00001fc4
  DW_CFA_advance_loc: 4 to 00001f24
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001f28
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001f2c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000270 0000001c 00000000 FDE cie=00000000 pc=00001fc4..00002018
  DW_CFA_advance_loc: 4 to 00001fc8
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001fcc
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001fd0
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000290 0000001c 00000000 FDE cie=00000000 pc=00002018..000025bc
  DW_CFA_advance_loc: 4 to 0000201c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00002020
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002024
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000002b0 0000001c 00000000 FDE cie=00000000 pc=000025bc..00002634
  DW_CFA_advance_loc: 4 to 000025c0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000025c4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000025c8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000002d0 0000001c 00000000 FDE cie=00000000 pc=00002634..00002744
  DW_CFA_advance_loc: 4 to 00002638
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000263c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002640
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000002f0 0000001c 00000000 FDE cie=00000000 pc=00002744..00002c1c
  DW_CFA_advance_loc: 4 to 00002748
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000274c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 00002750
  DW_CFA_def_cfa: r11 ofs 4

00000310 0000001c 00000000 FDE cie=00000000 pc=00002c1c..00003688
  DW_CFA_advance_loc: 4 to 00002c20
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00002c24
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002c28
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000330 00000020 00000000 FDE cie=00000000 pc=00003688..00003b6c
  DW_CFA_advance_loc: 4 to 0000368c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003690
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r5 at cfa-20
  DW_CFA_offset: r4 at cfa-24
  DW_CFA_advance_loc: 4 to 00003694
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000354 0000001c 00000000 FDE cie=00000000 pc=00003b6c..00003db8
  DW_CFA_advance_loc: 4 to 00003b70
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003b74
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003b78
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000374 0000001c 00000000 FDE cie=00000000 pc=00003db8..00003e44
  DW_CFA_advance_loc: 4 to 00003dbc
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003dc0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003dc4
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000394 0000001c 00000000 FDE cie=00000000 pc=00003e44..00003ed8
  DW_CFA_advance_loc: 4 to 00003e48
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003e4c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003e50
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000003b4 0000001c 00000000 FDE cie=00000000 pc=00003ed8..00003f98
  DW_CFA_advance_loc: 4 to 00003edc
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003ee0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003ee4
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000003d4 0000001c 00000000 FDE cie=00000000 pc=00003f98..000040fc
  DW_CFA_advance_loc: 4 to 00003f9c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003fa0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003fa4
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000003f4 0000001c 00000000 FDE cie=00000000 pc=000040fc..000041cc
  DW_CFA_advance_loc: 4 to 00004100
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00004104
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00004108
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000414 0000001c 00000000 FDE cie=00000000 pc=000041cc..00004254
  DW_CFA_advance_loc: 4 to 000041d0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000041d4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000041d8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000434 0000001c 00000000 FDE cie=00000000 pc=00004254..0000427c
  DW_CFA_advance_loc: 4 to 00004258
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000425c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00004260
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

Contents of the .debug_loc section:

    Offset   Begin    End      Expression
    00000000 00000000 00000004 (DW_OP_reg13)
    00000000 00000004 0000000c (DW_OP_reg12)
    00000000 0000000c 0000032c (DW_OP_breg11: 4)
    00000000 <End of list>
    0000002a 0000032c 00000330 (DW_OP_reg13)
    0000002a 00000330 00000338 (DW_OP_reg12)
    0000002a 00000338 000007a4 (DW_OP_breg11: 4)
    0000002a <End of list>
    00000054 000007a4 000007a8 (DW_OP_reg13)
    00000054 000007a8 000007b0 (DW_OP_reg12)
    00000054 000007b0 00000b74 (DW_OP_breg11: 4)
    00000054 <End of list>
    0000007e 00000b74 00000b78 (DW_OP_reg13)
    0000007e 00000b78 00000b80 (DW_OP_reg12)
    0000007e 00000b80 000012c0 (DW_OP_breg11: 4)
    0000007e <End of list>
    000000a8 000012c0 000012c4 (DW_OP_reg13)
    000000a8 000012c4 000012cc (DW_OP_reg12)
    000000a8 000012cc 000013cc (DW_OP_breg11: 4)
    000000a8 <End of list>
    000000d2 000013cc 000013d0 (DW_OP_reg13)
    000000d2 000013d0 000013d8 (DW_OP_reg12)
    000000d2 000013d8 000015f8 (DW_OP_breg11: 4)
    000000d2 <End of list>
    000000fc 000015f8 000015fc (DW_OP_reg13)
    000000fc 000015fc 00001604 (DW_OP_reg12)
    000000fc 00001604 0000172c (DW_OP_breg11: 4)
    000000fc <End of list>
    00000126 0000172c 00001730 (DW_OP_reg13)
    00000126 00001730 00001738 (DW_OP_reg12)
    00000126 00001738 000017bc (DW_OP_breg11: 4)
    00000126 <End of list>
    00000150 000017bc 000017c0 (DW_OP_reg13)
    00000150 000017c0 000017c8 (DW_OP_reg12)
    00000150 000017c8 00001810 (DW_OP_breg11: 4)
    00000150 <End of list>
    0000017a 00001810 00001814 (DW_OP_reg13)
    0000017a 00001814 0000181c (DW_OP_reg12)
    0000017a 0000181c 00001834 (DW_OP_breg11: 4)
    0000017a <End of list>
    000001a4 00001834 00001838 (DW_OP_reg13)
    000001a4 00001838 00001840 (DW_OP_reg12)
    000001a4 00001840 0000184c (DW_OP_breg11: 4)
    000001a4 <End of list>
    000001ce 0000184c 00001850 (DW_OP_reg13)
    000001ce 00001850 00001858 (DW_OP_reg12)
    000001ce 00001858 00001930 (DW_OP_breg11: 4)
    000001ce <End of list>
    000001f8 00001930 00001934 (DW_OP_reg13)
    000001f8 00001934 0000193c (DW_OP_reg12)
    000001f8 0000193c 00001a14 (DW_OP_breg11: 4)
    000001f8 <End of list>
    00000222 00001a14 00001a18 (DW_OP_reg13)
    00000222 00001a18 00001a20 (DW_OP_reg12)
    00000222 00001a20 00001ae8 (DW_OP_breg11: 4)
    00000222 <End of list>
    0000024c 00001ae8 00001aec (DW_OP_reg13)
    0000024c 00001aec 00001af4 (DW_OP_reg12)
    0000024c 00001af4 00001c8c (DW_OP_breg11: 4)
    0000024c <End of list>
    00000276 00001c8c 00001c90 (DW_OP_reg13)
    00000276 00001c90 00001c98 (DW_OP_reg12)
    00000276 00001c98 00001e14 (DW_OP_breg11: 4)
    00000276 <End of list>
    000002a0 00001e14 00001e18 (DW_OP_reg13)
    000002a0 00001e18 00001e20 (DW_OP_reg12)
    000002a0 00001e20 00001f00 (DW_OP_breg11: 4)
    000002a0 <End of list>
    000002ca 00001f00 00001f04 (DW_OP_reg13)
    000002ca 00001f04 00001f0c (DW_OP_reg12)
    000002ca 00001f0c 00001f20 (DW_OP_breg11: 4)
    000002ca <End of list>
    000002f4 00001f20 00001f24 (DW_OP_reg13)
    000002f4 00001f24 00001f2c (DW_OP_reg12)
    000002f4 00001f2c 00001fc4 (DW_OP_breg11: 4)
    000002f4 <End of list>
    0000031e 00001fc4 00001fc8 (DW_OP_reg13)
    0000031e 00001fc8 00001fd0 (DW_OP_reg12)
    0000031e 00001fd0 00002018 (DW_OP_breg11: 4)
    0000031e <End of list>
    00000348 00002018 0000201c (DW_OP_reg13)
    00000348 0000201c 00002024 (DW_OP_reg12)
    00000348 00002024 000025bc (DW_OP_breg11: 4)
    00000348 <End of list>
    00000372 000025bc 000025c0 (DW_OP_reg13)
    00000372 000025c0 000025c8 (DW_OP_reg12)
    00000372 000025c8 00002634 (DW_OP_breg11: 4)
    00000372 <End of list>
    0000039c 00002634 00002638 (DW_OP_reg13)
    0000039c 00002638 00002640 (DW_OP_reg12)
    0000039c 00002640 00002744 (DW_OP_breg11: 4)
    0000039c <End of list>
    000003c6 00002744 00002748 (DW_OP_reg13)
    000003c6 00002748 00002750 (DW_OP_reg12)
    000003c6 00002750 00002c1c (DW_OP_breg11: 4)
    000003c6 <End of list>
    000003f0 00002c1c 00002c20 (DW_OP_reg13)
    000003f0 00002c20 00002c28 (DW_OP_reg12)
    000003f0 00002c28 00003688 (DW_OP_breg11: 4)
    000003f0 <End of list>
    0000041a 00003688 0000368c (DW_OP_reg13)
    0000041a 0000368c 00003694 (DW_OP_reg12)
    0000041a 00003694 00003b6c (DW_OP_breg11: 4)
    0000041a <End of list>
    00000444 00003b6c 00003b70 (DW_OP_reg13)
    00000444 00003b70 00003b78 (DW_OP_reg12)
    00000444 00003b78 00003db8 (DW_OP_breg11: 4)
    00000444 <End of list>
    0000046e 00003db8 00003dbc (DW_OP_reg13)
    0000046e 00003dbc 00003dc4 (DW_OP_reg12)
    0000046e 00003dc4 00003e44 (DW_OP_breg11: 4)
    0000046e <End of list>
    00000498 00003e44 00003e48 (DW_OP_reg13)
    00000498 00003e48 00003e50 (DW_OP_reg12)
    00000498 00003e50 00003ed8 (DW_OP_breg11: 4)
    00000498 <End of list>
    000004c2 00003ed8 00003edc (DW_OP_reg13)
    000004c2 00003edc 00003ee4 (DW_OP_reg12)
    000004c2 00003ee4 00003f98 (DW_OP_breg11: 4)
    000004c2 <End of list>
    000004ec 00003f98 00003f9c (DW_OP_reg13)
    000004ec 00003f9c 00003fa4 (DW_OP_reg12)
    000004ec 00003fa4 000040fc (DW_OP_breg11: 4)
    000004ec <End of list>
    00000516 000040fc 00004100 (DW_OP_reg13)
    00000516 00004100 00004108 (DW_OP_reg12)
    00000516 00004108 000041cc (DW_OP_breg11: 4)
    00000516 <End of list>
    00000540 000041cc 000041d0 (DW_OP_reg13)
    00000540 000041d0 000041d8 (DW_OP_reg12)
    00000540 000041d8 00004254 (DW_OP_breg11: 4)
    00000540 <End of list>
    0000056a 00004254 00004258 (DW_OP_reg13)
    0000056a 00004258 00004260 (DW_OP_reg12)
    0000056a 00004260 0000427c (DW_OP_breg11: 4)
    0000056a <End of list>
Contents of the .debug_pubnames section:

  Length:                              421
  Version:                             2
  Offset into .debug_info section:     0
  Size of area in .debug_info section: 22304

    Offset  Name
    17291       MGC_DrcInit
    17456       MGC_DrcIsr
    17692       MGC_DrcBindEndpoint
    18094       MGC_DrcServiceDefaultEnd
    18373       MGC_DrcResetPort
    18609       MGC_DrcOtgTimer
    20010       MGC_DrcChangeOtgState
    20128       MGC_CompleteOtgTransition
    20400       MGC_DrcUsbIsr
    20626       MGC_DrcBsr
    20953       MGC_DrcServiceHostDefault
    21165       MGC_DrcAcceptDevice
    21302       MGC_DrcFinishResume
    21381       MGC_DrcSetHostPower
    21564       MUSB_RegisterOtgClient
    21750       MUSB_RelinquishHost
    21854       MUSB_RequestBus
    21928       MGC_DrcDumpPipe

The section .debug_aranges contains:

  Length:                   28
  Version:                  2
  Offset into .debug_info:  0
  Pointer Size:             4
  Segment Size:             0

    Address    Length
    0x00000000 0x427c
    0x00000000 0x0

Contents of the .debug_str section:

  0x00000000 70486f73 74436c69 656e7400 70436f6d pHostClient.pCom
  0x00000010 706c6574 65506172 616d0070 4f746744 pleteParam.pOtgD
  0x00000020 65736300 62427573 41646472 65737300 esc.bBusAddress.
  0x00000030 64775374 61747573 0062496e 74657276 dwStatus.bInterv
  0x00000040 616c0062 49737243 616c6c62 61636b00 al.bIsrCallback.
  0x00000050 7049736f 63684972 70006254 72616666 pIsochIrp.bTraff
  0x00000060 69635479 70650077 436f756e 74007046 icType.wCount.pF
  0x00000070 756e6374 696f6e43 6c69656e 74007050 unctionClient.pP
  0x00000080 6172616d 00704c6f 63616c45 6e640077 aram.pLocalEnd.w
  0x00000090 5061636b 65745369 7a650061 70436f6e PacketSize.apCon
  0x000000a0 66696744 65736372 6970746f 72730062 figDescriptors.b
  0x000000b0 4973436c 61696d65 64007749 6e646578 IsClaimed.wIndex
  0x000000c0 00705265 71756573 74006477 456e6443 .pRequest.dwEndC
  0x000000d0 6f756e74 00624164 64726573 73007754 ount.bAddress.wT
  0x000000e0 696d6572 00704f74 67436c69 656e7400 imer.pOtgClient.
  0x000000f0 62416c6c 6f77446d 61007046 69666f44 bAllowDma.pFifoD
  0x00000100 65737400 70507269 76617465 44617461 est.pPrivateData
  0x00000110 0070436f 6e74726f 6c6c6572 00774d61 .pController.wMa
  0x00000120 78506163 6b657453 697a6500 624c656e xPacketSize.bLen
  0x00000130 67746800 77566572 73696f6e 00624973 gth.wVersion.bIs
  0x00000140 52657365 74006244 65736372 6970746f Reset.bDescripto
  0x00000150 72547970 65006477 41637475 616c4c65 rType.dwActualLe
  0x00000160 6e677468 00774669 666f436f 756e7400 ngth.wFifoCount.
  0x00000170 624c6f63 616c456e 64007050 6f727400 bLocalEnd.pPort.
  0x00000180 70446576 69636500 50697065 4c697374 pDevice.PipeList
  0x00000190 00774465 73637269 70746f72 42756666 .wDescriptorBuff
  0x000001a0 65724c65 6e677468 00704472 69766572 erLength.pDriver
  0x000001b0 00626d41 74747269 62757465 73006253 .bmAttributes.bS
  0x000001c0 74617475 73007066 49727043 6f6d706c tatus.pfIrpCompl
  0x000001d0 65746500 70536572 76696365 73006477 ete.pServices.dw
  0x000001e0 456e6449 6e646578 00                EndIndex.

Disassembly of section .text:

00000000 <MGC_DrcInit>:
       0:   e1a0c00d    mov ip, sp
       4:   e92dd800    push    {fp, ip, lr, pc}
       8:   e24cb004    sub fp, ip, #4  ; 0x4
       c:   e24dd078    sub sp, sp, #120    ; 0x78
      10:   e50b0080    str r0, [fp, #-128]
      14:   e51b3080    ldr r3, [fp, #-128]
      18:   e5933000    ldr r3, [r3]
      1c:   e50b3018    str r3, [fp, #-24]
      20:   e51b3018    ldr r3, [fp, #-24]
      24:   e5933004    ldr r3, [r3, #4]
      28:   e50b3014    str r3, [fp, #-20]
      2c:   e3a03000    mov r3, #0  ; 0x0
      30:   e54b300d    strb    r3, [fp, #-13]
      34:   e51b2080    ldr r2, [fp, #-128]
      38:   e3a03001    mov r3, #1  ; 0x1
      3c:   e5c23033    strb    r3, [r2, #51]
      40:   e51b2080    ldr r2, [fp, #-128]
      44:   e3a03001    mov r3, #1  ; 0x1
      48:   e1c234b2    strh    r3, [r2, #66]
      4c:   e3a03001    mov r3, #1  ; 0x1
      50:   e54b301a    strb    r3, [fp, #-26]
      54:   ea000022    b   e4 <MGC_DrcInit+0xe4>
      58:   e55b301a    ldrb    r3, [fp, #-26]
      5c:   e51b0014    ldr r0, [fp, #-20]
      60:   e3a0100e    mov r1, #14 ; 0xe
      64:   e1a02003    mov r2, r3
      68:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            68: R_ARM_PC24  MGC_FAPI_WriteReg8
      6c:   e51b0014    ldr r0, [fp, #-20]
      70:   e3a0101f    mov r1, #31 ; 0x1f
      74:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            74: R_ARM_PC24  MGC_FAPI_ReadReg8
      78:   e1a03000    mov r3, r0
      7c:   e54b3019    strb    r3, [fp, #-25]
      80:   e55b3019    ldrb    r3, [fp, #-25]
      84:   e3530000    cmp r3, #0  ; 0x0
      88:   0a000018    beq f0 <MGC_DrcInit+0xf0>
      8c:   e51b3080    ldr r3, [fp, #-128]
      90:   e5d33033    ldrb    r3, [r3, #51]
      94:   e2833001    add r3, r3, #1  ; 0x1
      98:   e6ef3073    uxtb    r3, r3
      9c:   e51b2080    ldr r2, [fp, #-128]
      a0:   e5c23033    strb    r3, [r2, #51]
      a4:   e51b3080    ldr r3, [fp, #-128]
      a8:   e1d334b2    ldrh    r3, [r3, #66]
      ac:   e1a01003    mov r1, r3
      b0:   e55b201a    ldrb    r2, [fp, #-26]
      b4:   e3a03001    mov r3, #1  ; 0x1
      b8:   e1a03213    lsl r3, r3, r2
      bc:   e6ff3073    uxth    r3, r3
      c0:   e1a02001    mov r2, r1
      c4:   e1823003    orr r3, r2, r3
      c8:   e6ff3073    uxth    r3, r3
      cc:   e6ff2073    uxth    r2, r3
      d0:   e51b3080    ldr r3, [fp, #-128]
      d4:   e1c324b2    strh    r2, [r3, #66]
      d8:   e55b301a    ldrb    r3, [fp, #-26]
      dc:   e2833001    add r3, r3, #1  ; 0x1
      e0:   e54b301a    strb    r3, [fp, #-26]
      e4:   e55b301a    ldrb    r3, [fp, #-26]
      e8:   e353000f    cmp r3, #15 ; 0xf
      ec:   9affffd9    bls 58 <MGC_DrcInit+0x58>
      f0:   e59f3230    ldr r3, [pc, #560]  ; 328 <MGC_DrcInit+0x328>
      f4:   e593303c    ldr r3, [r3, #60]
      f8:   e3530000    cmp r3, #0  ; 0x0
      fc:   0a00000b    beq 130 <MGC_DrcInit+0x130>
     100:   e59f3220    ldr r3, [pc, #544]  ; 328 <MGC_DrcInit+0x328>
     104:   e593103c    ldr r1, [r3, #60]
     108:   e51b3080    ldr r3, [fp, #-128]
     10c:   e5d33033    ldrb    r3, [r3, #51]
     110:   e1a02283    lsl r2, r3, #5
     114:   e1a03102    lsl r3, r2, #2
     118:   e0623003    rsb r3, r2, r3
     11c:   e1a00003    mov r0, r3
     120:   e12fff31    blx r1
     124:   e1a03000    mov r3, r0
     128:   e50b3084    str r3, [fp, #-132]
     12c:   ea000001    b   138 <MGC_DrcInit+0x138>
     130:   e3a02000    mov r2, #0  ; 0x0
     134:   e50b2084    str r2, [fp, #-132]
     138:   e51b3080    ldr r3, [fp, #-128]
     13c:   e51b2084    ldr r2, [fp, #-132]
     140:   e5832008    str r2, [r3, #8]
     144:   e51b3080    ldr r3, [fp, #-128]
     148:   e5933008    ldr r3, [r3, #8]
     14c:   e3530000    cmp r3, #0  ; 0x0
     150:   0a00006c    beq 308 <MGC_DrcInit+0x308>
     154:   e51b3080    ldr r3, [fp, #-128]
     158:   e283100c    add r1, r3, #12 ; 0xc
     15c:   e51b3080    ldr r3, [fp, #-128]
     160:   e5d33033    ldrb    r3, [r3, #51]
     164:   e1a02003    mov r2, r3
     168:   e51b3080    ldr r3, [fp, #-128]
     16c:   e5933008    ldr r3, [r3, #8]
     170:   e1a00001    mov r0, r1
     174:   e3a01060    mov r1, #96 ; 0x60
     178:   ebfffffe    bl  0 <MUSB_ArrayInit>
            178: R_ARM_PC24 MUSB_ArrayInit
     17c:   e1a03000    mov r3, r0
     180:   e3530000    cmp r3, #0  ; 0x0
     184:   0a000059    beq 2f0 <MGC_DrcInit+0x2f0>
     188:   e3a03001    mov r3, #1  ; 0x1
     18c:   e54b300d    strb    r3, [fp, #-13]
     190:   e24b307c    sub r3, fp, #124    ; 0x7c
     194:   e1a00003    mov r0, r3
     198:   e3a01000    mov r1, #0  ; 0x0
     19c:   e3a02060    mov r2, #96 ; 0x60
     1a0:   ebfffffe    bl  0 <MGC_FAPI_MemSet>
            1a0: R_ARM_PC24 MGC_FAPI_MemSet
     1a4:   e3a03001    mov r3, #1  ; 0x1
     1a8:   e54b307b    strb    r3, [fp, #-123]
     1ac:   e3a03000    mov r3, #0  ; 0x0
     1b0:   e54b306f    strb    r3, [fp, #-111]
     1b4:   e55b306f    ldrb    r3, [fp, #-111]
     1b8:   e54b3073    strb    r3, [fp, #-115]
     1bc:   e3a03040    mov r3, #64 ; 0x40
     1c0:   e14b37ba    strh    r3, [fp, #-122]
     1c4:   e3a03040    mov r3, #64 ; 0x40
     1c8:   e14b37b8    strh    r3, [fp, #-120]
     1cc:   e3a03001    mov r3, #1  ; 0x1
     1d0:   e54b306b    strb    r3, [fp, #-107]
     1d4:   e55b306b    ldrb    r3, [fp, #-107]
     1d8:   e54b306c    strb    r3, [fp, #-108]
     1dc:   e51b3080    ldr r3, [fp, #-128]
     1e0:   e283300c    add r3, r3, #12 ; 0xc
     1e4:   e24b207c    sub r2, fp, #124    ; 0x7c
     1e8:   e1a00003    mov r0, r3
     1ec:   e1a01002    mov r1, r2
     1f0:   ebfffffe    bl  0 <MUSB_ArrayAppend>
            1f0: R_ARM_PC24 MUSB_ArrayAppend
     1f4:   e3a03001    mov r3, #1  ; 0x1
     1f8:   e54b301a    strb    r3, [fp, #-26]
     1fc:   ea000035    b   2d8 <MGC_DrcInit+0x2d8>
     200:   e55b301a    ldrb    r3, [fp, #-26]
     204:   e51b0014    ldr r0, [fp, #-20]
     208:   e3a0100e    mov r1, #14 ; 0xe
     20c:   e1a02003    mov r2, r3
     210:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            210: R_ARM_PC24 MGC_FAPI_WriteReg8
     214:   e51b0014    ldr r0, [fp, #-20]
     218:   e3a0101f    mov r1, #31 ; 0x1f
     21c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            21c: R_ARM_PC24 MGC_FAPI_ReadReg8
     220:   e1a03000    mov r3, r0
     224:   e54b3019    strb    r3, [fp, #-25]
     228:   e24b307c    sub r3, fp, #124    ; 0x7c
     22c:   e1a00003    mov r0, r3
     230:   e3a01000    mov r1, #0  ; 0x0
     234:   e3a02060    mov r2, #96 ; 0x60
     238:   ebfffffe    bl  0 <MGC_FAPI_MemSet>
            238: R_ARM_PC24 MGC_FAPI_MemSet
     23c:   e55b301a    ldrb    r3, [fp, #-26]
     240:   e54b307c    strb    r3, [fp, #-124]
     244:   e55b3019    ldrb    r3, [fp, #-25]
     248:   e203200f    and r2, r3, #15 ; 0xf
     24c:   e3a03001    mov r3, #1  ; 0x1
     250:   e1a03213    lsl r3, r3, r2
     254:   e6ff3073    uxth    r3, r3
     258:   e14b37ba    strh    r3, [fp, #-122]
     25c:   e55b3019    ldrb    r3, [fp, #-25]
     260:   e20330f0    and r3, r3, #240    ; 0xf0
     264:   e35300f0    cmp r3, #240    ; 0xf0
     268:   1a000008    bne 290 <MGC_DrcInit+0x290>
     26c:   e55b3019    ldrb    r3, [fp, #-25]
     270:   e203200f    and r2, r3, #15 ; 0xf
     274:   e3a03001    mov r3, #1  ; 0x1
     278:   e1a03213    lsl r3, r3, r2
     27c:   e6ff3073    uxth    r3, r3
     280:   e14b37b8    strh    r3, [fp, #-120]
     284:   e3a03001    mov r3, #1  ; 0x1
     288:   e54b307b    strb    r3, [fp, #-123]
     28c:   ea000008    b   2b4 <MGC_DrcInit+0x2b4>
     290:   e55b3019    ldrb    r3, [fp, #-25]
     294:   e20330f0    and r3, r3, #240    ; 0xf0
     298:   e1a02243    asr r2, r3, #4
     29c:   e3a03001    mov r3, #1  ; 0x1
     2a0:   e1a03213    lsl r3, r3, r2
     2a4:   e6ff3073    uxth    r3, r3
     2a8:   e14b37b8    strh    r3, [fp, #-120]
     2ac:   e3a03000    mov r3, #0  ; 0x0
     2b0:   e54b307b    strb    r3, [fp, #-123]
     2b4:   e51b3080    ldr r3, [fp, #-128]
     2b8:   e283300c    add r3, r3, #12 ; 0xc
     2bc:   e24b207c    sub r2, fp, #124    ; 0x7c
     2c0:   e1a00003    mov r0, r3
     2c4:   e1a01002    mov r1, r2
     2c8:   ebfffffe    bl  0 <MUSB_ArrayAppend>
            2c8: R_ARM_PC24 MUSB_ArrayAppend
     2cc:   e55b301a    ldrb    r3, [fp, #-26]
     2d0:   e2833001    add r3, r3, #1  ; 0x1
     2d4:   e54b301a    strb    r3, [fp, #-26]
     2d8:   e51b3080    ldr r3, [fp, #-128]
     2dc:   e5d32033    ldrb    r2, [r3, #51]
     2e0:   e55b301a    ldrb    r3, [fp, #-26]
     2e4:   e1530002    cmp r3, r2
     2e8:   3affffc4    bcc 200 <MGC_DrcInit+0x200>
     2ec:   ea000005    b   308 <MGC_DrcInit+0x308>
     2f0:   e59f3030    ldr r3, [pc, #48]   ; 328 <MGC_DrcInit+0x328>
     2f4:   e593204c    ldr r2, [r3, #76]
     2f8:   e51b3080    ldr r3, [fp, #-128]
     2fc:   e5933008    ldr r3, [r3, #8]
     300:   e1a00003    mov r0, r3
     304:   e12fff32    blx r2
     308:   e51b0014    ldr r0, [fp, #-20]
     30c:   e3a0100e    mov r1, #14 ; 0xe
     310:   e3a02000    mov r2, #0  ; 0x0
     314:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            314: R_ARM_PC24 MGC_FAPI_WriteReg8
     318:   e55b300d    ldrb    r3, [fp, #-13]
     31c:   e1a00003    mov r0, r3
     320:   e24bd00c    sub sp, fp, #12 ; 0xc
     324:   e89da800    ldm sp, {fp, sp, pc}
     328:   00000000    .word   0x00000000
            328: R_ARM_ABS32    FAPI_SYS_Services

0000032c <MGC_DrcIsr>:
     32c:   e1a0c00d    mov ip, sp
     330:   e92dd800    push    {fp, ip, lr, pc}
     334:   e24cb004    sub fp, ip, #4  ; 0x4
     338:   e24dd034    sub sp, sp, #52 ; 0x34
     33c:   e50b0030    str r0, [fp, #-48]
     340:   e1a00003    mov r0, r3
     344:   e1a03001    mov r3, r1
     348:   e54b3034    strb    r3, [fp, #-52]
     34c:   e14b23b8    strh    r2, [fp, #-56]
     350:   e14b03bc    strh    r0, [fp, #-60]
     354:   e3e03000    mvn r3, #0  ; 0x0
     358:   e50b3018    str r3, [fp, #-24]
     35c:   e51b3030    ldr r3, [fp, #-48]
     360:   e593300c    ldr r3, [r3, #12]
     364:   e50b3014    str r3, [fp, #-20]
     368:   e51b3030    ldr r3, [fp, #-48]
     36c:   e5933014    ldr r3, [r3, #20]
     370:   e50b3010    str r3, [fp, #-16]
     374:   e51b2010    ldr r2, [fp, #-16]
     378:   e3a03000    mov r3, #0  ; 0x0
     37c:   e5c23044    strb    r3, [r2, #68]
     380:   e51b3010    ldr r3, [fp, #-16]
     384:   e5933214    ldr r3, [r3, #532]
     388:   e3530000    cmp r3, #0  ; 0x0
     38c:   0a000012    beq 3dc <MGC_DrcIsr+0xb0>
     390:   e51b3010    ldr r3, [fp, #-16]
     394:   e5933214    ldr r3, [r3, #532]
     398:   e5933024    ldr r3, [r3, #36]
     39c:   e3530000    cmp r3, #0  ; 0x0
     3a0:   0a00000d    beq 3dc <MGC_DrcIsr+0xb0>
     3a4:   e51b3010    ldr r3, [fp, #-16]
     3a8:   e5933214    ldr r3, [r3, #532]
     3ac:   e5932024    ldr r2, [r3, #36]
     3b0:   e51b3010    ldr r3, [fp, #-16]
     3b4:   e5933214    ldr r3, [r3, #532]
     3b8:   e5933000    ldr r3, [r3]
     3bc:   e1a00003    mov r0, r3
     3c0:   e12fff32    blx r2
     3c4:   e1a03000    mov r3, r0
     3c8:   e3530000    cmp r3, #0  ; 0x0
     3cc:   0a000002    beq 3dc <MGC_DrcIsr+0xb0>
     3d0:   e51b3010    ldr r3, [fp, #-16]
     3d4:   e5d33044    ldrb    r3, [r3, #68]
     3d8:   e50b3018    str r3, [fp, #-24]
     3dc:   e55b3034    ldrb    r3, [fp, #-52]
     3e0:   e3530000    cmp r3, #0  ; 0x0
     3e4:   0a000026    beq 484 <MGC_DrcIsr+0x158>
     3e8:   e59f23a0    ldr r2, [pc, #928]  ; 790 <MGC_DrcIsr+0x464>
     3ec:   e3a03000    mov r3, #0  ; 0x0
     3f0:   e5c23000    strb    r3, [r2]
     3f4:   e51b3030    ldr r3, [fp, #-48]
     3f8:   e5933008    ldr r3, [r3, #8]
     3fc:   e5933004    ldr r3, [r3, #4]
     400:   e59f0388    ldr r0, [pc, #904]  ; 790 <MGC_DrcIsr+0x464>
     404:   e3a01c01    mov r1, #256    ; 0x100
     408:   e59f2384    ldr r2, [pc, #900]  ; 794 <MGC_DrcIsr+0x468>
     40c:   e12fff33    blx r3
     410:   e51b3030    ldr r3, [fp, #-48]
     414:   e5933008    ldr r3, [r3, #8]
     418:   e593c008    ldr ip, [r3, #8]
     41c:   e55b2034    ldrb    r2, [fp, #-52]
     420:   e3a03002    mov r3, #2  ; 0x2
     424:   e58d3000    str r3, [sp]
     428:   e59f0360    ldr r0, [pc, #864]  ; 790 <MGC_DrcIsr+0x464>
     42c:   e3a01c01    mov r1, #256    ; 0x100
     430:   e3a03010    mov r3, #16 ; 0x10
     434:   e12fff3c    blx ip
     438:   e59f3358    ldr r3, [pc, #856]  ; 798 <MGC_DrcIsr+0x46c>
     43c:   e5d33000    ldrb    r3, [r3]
     440:   e3530002    cmp r3, #2  ; 0x2
     444:   9a000008    bls 46c <MGC_DrcIsr+0x140>
     448:   e51b3030    ldr r3, [fp, #-48]
     44c:   e593300c    ldr r3, [r3, #12]
     450:   e5932028    ldr r2, [r3, #40]
     454:   e51b3030    ldr r3, [fp, #-48]
     458:   e593300c    ldr r3, [r3, #12]
     45c:   e5933004    ldr r3, [r3, #4]
     460:   e1a00003    mov r0, r3
     464:   e59f1324    ldr r1, [pc, #804]  ; 790 <MGC_DrcIsr+0x464>
     468:   e12fff32    blx r2
     46c:   e55b3034    ldrb    r3, [fp, #-52]
     470:   e51b0010    ldr r0, [fp, #-16]
     474:   e1a01003    mov r1, r3
     478:   ebfffffe    bl  2744 <MGC_DrcUsbIsr>
            478: R_ARM_PC24 MGC_DrcUsbIsr
     47c:   e1a03000    mov r3, r0
     480:   e50b3018    str r3, [fp, #-24]
     484:   e59f2304    ldr r2, [pc, #772]  ; 790 <MGC_DrcIsr+0x464>
     488:   e3a03000    mov r3, #0  ; 0x0
     48c:   e5c23000    strb    r3, [r2]
     490:   e51b3030    ldr r3, [fp, #-48]
     494:   e5933008    ldr r3, [r3, #8]
     498:   e5933004    ldr r3, [r3, #4]
     49c:   e59f02ec    ldr r0, [pc, #748]  ; 790 <MGC_DrcIsr+0x464>
     4a0:   e3a01c01    mov r1, #256    ; 0x100
     4a4:   e59f22f0    ldr r2, [pc, #752]  ; 79c <MGC_DrcIsr+0x470>
     4a8:   e12fff33    blx r3
     4ac:   e51b3030    ldr r3, [fp, #-48]
     4b0:   e5933008    ldr r3, [r3, #8]
     4b4:   e593c008    ldr ip, [r3, #8]
     4b8:   e15b23b8    ldrh    r2, [fp, #-56]
     4bc:   e3a03004    mov r3, #4  ; 0x4
     4c0:   e58d3000    str r3, [sp]
     4c4:   e59f02c4    ldr r0, [pc, #708]  ; 790 <MGC_DrcIsr+0x464>
     4c8:   e3a01c01    mov r1, #256    ; 0x100
     4cc:   e3a03010    mov r3, #16 ; 0x10
     4d0:   e12fff3c    blx ip
     4d4:   e51b3030    ldr r3, [fp, #-48]
     4d8:   e5933008    ldr r3, [r3, #8]
     4dc:   e5933004    ldr r3, [r3, #4]
     4e0:   e59f02a8    ldr r0, [pc, #680]  ; 790 <MGC_DrcIsr+0x464>
     4e4:   e3a01c01    mov r1, #256    ; 0x100
     4e8:   e59f22b0    ldr r2, [pc, #688]  ; 7a0 <MGC_DrcIsr+0x474>
     4ec:   e12fff33    blx r3
     4f0:   e51b3030    ldr r3, [fp, #-48]
     4f4:   e5933008    ldr r3, [r3, #8]
     4f8:   e593c008    ldr ip, [r3, #8]
     4fc:   e15b23bc    ldrh    r2, [fp, #-60]
     500:   e3a03004    mov r3, #4  ; 0x4
     504:   e58d3000    str r3, [sp]
     508:   e59f0280    ldr r0, [pc, #640]  ; 790 <MGC_DrcIsr+0x464>
     50c:   e3a01c01    mov r1, #256    ; 0x100
     510:   e3a03010    mov r3, #16 ; 0x10
     514:   e12fff3c    blx ip
     518:   e59f3278    ldr r3, [pc, #632]  ; 798 <MGC_DrcIsr+0x46c>
     51c:   e5d33000    ldrb    r3, [r3]
     520:   e3530002    cmp r3, #2  ; 0x2
     524:   9a000008    bls 54c <MGC_DrcIsr+0x220>
     528:   e51b3030    ldr r3, [fp, #-48]
     52c:   e593300c    ldr r3, [r3, #12]
     530:   e5932028    ldr r2, [r3, #40]
     534:   e51b3030    ldr r3, [fp, #-48]
     538:   e593300c    ldr r3, [r3, #12]
     53c:   e5933004    ldr r3, [r3, #4]
     540:   e1a00003    mov r0, r3
     544:   e59f1244    ldr r1, [pc, #580]  ; 790 <MGC_DrcIsr+0x464>
     548:   e12fff32    blx r2
     54c:   e15b33b8    ldrh    r3, [fp, #-56]
     550:   e50b3024    str r3, [fp, #-36]
     554:   e51b3024    ldr r3, [fp, #-36]
     558:   e3530000    cmp r3, #0  ; 0x0
     55c:   0a000004    beq 574 <MGC_DrcIsr+0x248>
     560:   e51b3018    ldr r3, [fp, #-24]
     564:   e3530000    cmp r3, #0  ; 0x0
     568:   aa000001    bge 574 <MGC_DrcIsr+0x248>
     56c:   e3a03000    mov r3, #0  ; 0x0
     570:   e50b3018    str r3, [fp, #-24]
     574:   e51b3024    ldr r3, [fp, #-36]
     578:   e2033001    and r3, r3, #1  ; 0x1
     57c:   e6ef3073    uxtb    r3, r3
     580:   e3530000    cmp r3, #0  ; 0x0
     584:   0a000015    beq 5e0 <MGC_DrcIsr+0x2b4>
     588:   e51b3010    ldr r3, [fp, #-16]
     58c:   e593223c    ldr r2, [r3, #572]
     590:   e24b302c    sub r3, fp, #44 ; 0x2c
     594:   e51b0010    ldr r0, [fp, #-16]
     598:   e1a01003    mov r1, r3
     59c:   e12fff32    blx r2
     5a0:   e1a03000    mov r3, r0
     5a4:   e54b301d    strb    r3, [fp, #-29]
     5a8:   e55b301d    ldrb    r3, [fp, #-29]
     5ac:   e3530000    cmp r3, #0  ; 0x0
     5b0:   0a00000a    beq 5e0 <MGC_DrcIsr+0x2b4>
     5b4:   e51b3018    ldr r3, [fp, #-24]
     5b8:   e2833001    add r3, r3, #1  ; 0x1
     5bc:   e50b3018    str r3, [fp, #-24]
     5c0:   e51b3014    ldr r3, [fp, #-20]
     5c4:   e593c00c    ldr ip, [r3, #12]
     5c8:   e51b3014    ldr r3, [fp, #-20]
     5cc:   e5933004    ldr r3, [r3, #4]
     5d0:   e24b202c    sub r2, fp, #44 ; 0x2c
     5d4:   e1a00003    mov r0, r3
     5d8:   e1a01002    mov r1, r2
     5dc:   e12fff3c    blx ip
     5e0:   e3a03001    mov r3, #1  ; 0x1
     5e4:   e50b301c    str r3, [fp, #-28]
     5e8:   e51b3024    ldr r3, [fp, #-36]
     5ec:   e1a030a3    lsr r3, r3, #1
     5f0:   e50b3024    str r3, [fp, #-36]
     5f4:   ea000025    b   690 <MGC_DrcIsr+0x364>
     5f8:   e51b3024    ldr r3, [fp, #-36]
     5fc:   e2033001    and r3, r3, #1  ; 0x1
     600:   e6ef3073    uxtb    r3, r3
     604:   e3530000    cmp r3, #0  ; 0x0
     608:   0a00001a    beq 678 <MGC_DrcIsr+0x34c>
     60c:   e51b3010    ldr r3, [fp, #-16]
     610:   e593c240    ldr ip, [r3, #576]
     614:   e51b301c    ldr r3, [fp, #-28]
     618:   e6ff3073    uxth    r3, r3
     61c:   e24b202c    sub r2, fp, #44 ; 0x2c
     620:   e51b0010    ldr r0, [fp, #-16]
     624:   e1a01003    mov r1, r3
     628:   e12fff3c    blx ip
     62c:   e1a03000    mov r3, r0
     630:   e54b301d    strb    r3, [fp, #-29]
     634:   e55b301d    ldrb    r3, [fp, #-29]
     638:   e3530000    cmp r3, #0  ; 0x0
     63c:   0a00000d    beq 678 <MGC_DrcIsr+0x34c>
     640:   e51b3018    ldr r3, [fp, #-24]
     644:   e2833001    add r3, r3, #1  ; 0x1
     648:   e50b3018    str r3, [fp, #-24]
     64c:   e51b301c    ldr r3, [fp, #-28]
     650:   e6ef3073    uxtb    r3, r3
     654:   e54b3027    strb    r3, [fp, #-39]
     658:   e51b3014    ldr r3, [fp, #-20]
     65c:   e593c00c    ldr ip, [r3, #12]
     660:   e51b3014    ldr r3, [fp, #-20]
     664:   e5933004    ldr r3, [r3, #4]
     668:   e24b202c    sub r2, fp, #44 ; 0x2c
     66c:   e1a00003    mov r0, r3
     670:   e1a01002    mov r1, r2
     674:   e12fff3c    blx ip
     678:   e51b3024    ldr r3, [fp, #-36]
     67c:   e1a030a3    lsr r3, r3, #1
     680:   e50b3024    str r3, [fp, #-36]
     684:   e51b301c    ldr r3, [fp, #-28]
     688:   e2833001    add r3, r3, #1  ; 0x1
     68c:   e50b301c    str r3, [fp, #-28]
     690:   e51b3024    ldr r3, [fp, #-36]
     694:   e3530000    cmp r3, #0  ; 0x0
     698:   1affffd6    bne 5f8 <MGC_DrcIsr+0x2cc>
     69c:   e15b33bc    ldrh    r3, [fp, #-60]
     6a0:   e50b3024    str r3, [fp, #-36]
     6a4:   e51b3024    ldr r3, [fp, #-36]
     6a8:   e3530000    cmp r3, #0  ; 0x0
     6ac:   0a000004    beq 6c4 <MGC_DrcIsr+0x398>
     6b0:   e51b3018    ldr r3, [fp, #-24]
     6b4:   e3530000    cmp r3, #0  ; 0x0
     6b8:   aa000001    bge 6c4 <MGC_DrcIsr+0x398>
     6bc:   e3a03000    mov r3, #0  ; 0x0
     6c0:   e50b3018    str r3, [fp, #-24]
     6c4:   e3a03001    mov r3, #1  ; 0x1
     6c8:   e50b301c    str r3, [fp, #-28]
     6cc:   e51b3024    ldr r3, [fp, #-36]
     6d0:   e1a030a3    lsr r3, r3, #1
     6d4:   e50b3024    str r3, [fp, #-36]
     6d8:   ea000025    b   774 <MGC_DrcIsr+0x448>
     6dc:   e51b3024    ldr r3, [fp, #-36]
     6e0:   e2033001    and r3, r3, #1  ; 0x1
     6e4:   e6ef3073    uxtb    r3, r3
     6e8:   e3530000    cmp r3, #0  ; 0x0
     6ec:   0a00001a    beq 75c <MGC_DrcIsr+0x430>
     6f0:   e51b3010    ldr r3, [fp, #-16]
     6f4:   e593c244    ldr ip, [r3, #580]
     6f8:   e51b301c    ldr r3, [fp, #-28]
     6fc:   e6ff3073    uxth    r3, r3
     700:   e24b202c    sub r2, fp, #44 ; 0x2c
     704:   e51b0010    ldr r0, [fp, #-16]
     708:   e1a01003    mov r1, r3
     70c:   e12fff3c    blx ip
     710:   e1a03000    mov r3, r0
     714:   e54b301d    strb    r3, [fp, #-29]
     718:   e55b301d    ldrb    r3, [fp, #-29]
     71c:   e3530000    cmp r3, #0  ; 0x0
     720:   0a00000d    beq 75c <MGC_DrcIsr+0x430>
     724:   e51b3018    ldr r3, [fp, #-24]
     728:   e2833001    add r3, r3, #1  ; 0x1
     72c:   e50b3018    str r3, [fp, #-24]
     730:   e51b301c    ldr r3, [fp, #-28]
     734:   e6ef3073    uxtb    r3, r3
     738:   e54b3027    strb    r3, [fp, #-39]
     73c:   e51b3014    ldr r3, [fp, #-20]
     740:   e593c00c    ldr ip, [r3, #12]
     744:   e51b3014    ldr r3, [fp, #-20]
     748:   e5933004    ldr r3, [r3, #4]
     74c:   e24b202c    sub r2, fp, #44 ; 0x2c
     750:   e1a00003    mov r0, r3
     754:   e1a01002    mov r1, r2
     758:   e12fff3c    blx ip
     75c:   e51b3024    ldr r3, [fp, #-36]
     760:   e1a030a3    lsr r3, r3, #1
     764:   e50b3024    str r3, [fp, #-36]
     768:   e51b301c    ldr r3, [fp, #-28]
     76c:   e2833001    add r3, r3, #1  ; 0x1
     770:   e50b301c    str r3, [fp, #-28]
     774:   e51b3024    ldr r3, [fp, #-36]
     778:   e3530000    cmp r3, #0  ; 0x0
     77c:   1affffd6    bne 6dc <MGC_DrcIsr+0x3b0>
     780:   e51b3018    ldr r3, [fp, #-24]
     784:   e1a00003    mov r0, r3
     788:   e24bd00c    sub sp, fp, #12 ; 0xc
     78c:   e89da800    ldm sp, {fp, sp, pc}
     790:   00000000    .word   0x00000000
            790: R_ARM_ABS32    MGC_DiagMsg
     794:   00000010    .word   0x00000010
            794: R_ARM_ABS32    .rodata
     798:   00000000    .word   0x00000000
            798: R_ARM_ABS32    MGC_bDiagLevel
     79c:   0000001c    .word   0x0000001c
            79c: R_ARM_ABS32    .rodata
     7a0:   00000024    .word   0x00000024
            7a0: R_ARM_ABS32    .rodata

000007a4 <MGC_DrcBindEndpoint>:
     7a4:   e1a0c00d    mov ip, sp
     7a8:   e92dd800    push    {fp, ip, lr, pc}
     7ac:   e24cb004    sub fp, ip, #4  ; 0x4
     7b0:   e24dd04c    sub sp, sp, #76 ; 0x4c
     7b4:   e50b003c    str r0, [fp, #-60]
     7b8:   e50b1040    str r1, [fp, #-64]
     7bc:   e50b2044    str r2, [fp, #-68]
     7c0:   e54b3048    strb    r3, [fp, #-72]
     7c4:   e3a03000    mov r3, #0  ; 0x0
     7c8:   e50b3018    str r3, [fp, #-24]
     7cc:   e3a03000    mov r3, #0  ; 0x0
     7d0:   e50b3014    str r3, [fp, #-20]
     7d4:   e3e03000    mvn r3, #0  ; 0x0
     7d8:   e14b31b0    strh    r3, [fp, #-16]
     7dc:   e3e03000    mvn r3, #0  ; 0x0
     7e0:   e14b30be    strh    r3, [fp, #-14]
     7e4:   e51b3040    ldr r3, [fp, #-64]
     7e8:   e2833008    add r3, r3, #8  ; 0x8
     7ec:   e2833001    add r3, r3, #1  ; 0x1
     7f0:   e5d33000    ldrb    r3, [r3]
     7f4:   e1a03403    lsl r3, r3, #8
     7f8:   e6ff2073    uxth    r2, r3
     7fc:   e51b3040    ldr r3, [fp, #-64]
     800:   e2833008    add r3, r3, #8  ; 0x8
     804:   e5d33000    ldrb    r3, [r3]
     808:   e1823003    orr r3, r2, r3
     80c:   e6ff3073    uxth    r3, r3
     810:   e14b32ba    strh    r3, [fp, #-42]
     814:   e51b3040    ldr r3, [fp, #-64]
     818:   e5d33006    ldrb    r3, [r3, #6]
     81c:   e1a033a3    lsr r3, r3, #7
     820:   e54b302e    strb    r3, [fp, #-46]
     824:   e51b303c    ldr r3, [fp, #-60]
     828:   e5d3303e    ldrb    r3, [r3, #62]
     82c:   e3530000    cmp r3, #0  ; 0x0
     830:   0a000005    beq 84c <MGC_DrcBindEndpoint+0xa8>
     834:   e55b302e    ldrb    r3, [fp, #-46]
     838:   e3530000    cmp r3, #0  ; 0x0
     83c:   13a02000    movne   r2, #0  ; 0x0
     840:   03a02001    moveq   r2, #1  ; 0x1
     844:   e50b2058    str r2, [fp, #-88]
     848:   ea000001    b   854 <MGC_DrcBindEndpoint+0xb0>
     84c:   e55b302e    ldrb    r3, [fp, #-46]
     850:   e50b3058    str r3, [fp, #-88]
     854:   e51b2058    ldr r2, [fp, #-88]
     858:   e1a03002    mov r3, r2
     85c:   e54b302d    strb    r3, [fp, #-45]
     860:   e51b303c    ldr r3, [fp, #-60]
     864:   e283300c    add r3, r3, #12 ; 0xc
     868:   e1a00003    mov r0, r3
     86c:   ebfffffe    bl  0 <MUSB_ArrayLength>
            86c: R_ARM_PC24 MUSB_ArrayLength
     870:   e1a03000    mov r3, r0
     874:   e50b3038    str r3, [fp, #-56]
     878:   e3a03000    mov r3, #0  ; 0x0
     87c:   e50b3034    str r3, [fp, #-52]
     880:   ea000065    b   a1c <MGC_DrcBindEndpoint+0x278>
     884:   e51b303c    ldr r3, [fp, #-60]
     888:   e283300c    add r3, r3, #12 ; 0xc
     88c:   e1a00003    mov r0, r3
     890:   e51b1034    ldr r1, [fp, #-52]
     894:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            894: R_ARM_PC24 MUSB_ArrayFetch
     898:   e1a03000    mov r3, r0
     89c:   e50b3028    str r3, [fp, #-40]
     8a0:   e51b3028    ldr r3, [fp, #-40]
     8a4:   e5d33001    ldrb    r3, [r3, #1]
     8a8:   e3530000    cmp r3, #0  ; 0x0
     8ac:   0a000003    beq 8c0 <MGC_DrcBindEndpoint+0x11c>
     8b0:   e51b3028    ldr r3, [fp, #-40]
     8b4:   e5d33010    ldrb    r3, [r3, #16]
     8b8:   e50b3054    str r3, [fp, #-84]
     8bc:   ea00000b    b   8f0 <MGC_DrcBindEndpoint+0x14c>
     8c0:   e55b302d    ldrb    r3, [fp, #-45]
     8c4:   e3530000    cmp r3, #0  ; 0x0
     8c8:   0a000003    beq 8dc <MGC_DrcBindEndpoint+0x138>
     8cc:   e51b3028    ldr r3, [fp, #-40]
     8d0:   e5d33010    ldrb    r3, [r3, #16]
     8d4:   e50b3050    str r3, [fp, #-80]
     8d8:   ea000002    b   8e8 <MGC_DrcBindEndpoint+0x144>
     8dc:   e51b3028    ldr r3, [fp, #-40]
     8e0:   e5d33011    ldrb    r3, [r3, #17]
     8e4:   e50b3050    str r3, [fp, #-80]
     8e8:   e51b3050    ldr r3, [fp, #-80]
     8ec:   e50b3054    str r3, [fp, #-84]
     8f0:   e51b2054    ldr r2, [fp, #-84]
     8f4:   e1a03002    mov r3, r2
     8f8:   e54b302b    strb    r3, [fp, #-43]
     8fc:   e51b3028    ldr r3, [fp, #-40]
     900:   e3530000    cmp r3, #0  ; 0x0
     904:   0a000041    beq a10 <MGC_DrcBindEndpoint+0x26c>
     908:   e55b302b    ldrb    r3, [fp, #-43]
     90c:   e3530000    cmp r3, #0  ; 0x0
     910:   1a00003e    bne a10 <MGC_DrcBindEndpoint+0x26c>
     914:   e55b302d    ldrb    r3, [fp, #-45]
     918:   e3530000    cmp r3, #0  ; 0x0
     91c:   0a000003    beq 930 <MGC_DrcBindEndpoint+0x18c>
     920:   e51b3028    ldr r3, [fp, #-40]
     924:   e1d330b2    ldrh    r3, [r3, #2]
     928:   e50b304c    str r3, [fp, #-76]
     92c:   ea000002    b   93c <MGC_DrcBindEndpoint+0x198>
     930:   e51b3028    ldr r3, [fp, #-40]
     934:   e1d330b4    ldrh    r3, [r3, #4]
     938:   e50b304c    str r3, [fp, #-76]
     93c:   e51b304c    ldr r3, [fp, #-76]
     940:   e14b31ba    strh    r3, [fp, #-26]
     944:   e51b303c    ldr r3, [fp, #-60]
     948:   e5d3303e    ldrb    r3, [r3, #62]
     94c:   e3530000    cmp r3, #0  ; 0x0
     950:   0a00001e    beq 9d0 <MGC_DrcBindEndpoint+0x22c>
     954:   e15b21ba    ldrh    r2, [fp, #-26]
     958:   e15b32ba    ldrh    r3, [fp, #-42]
     95c:   e0633002    rsb r3, r3, r2
     960:   e50b3024    str r3, [fp, #-36]
     964:   e15b21ba    ldrh    r2, [fp, #-26]
     968:   e51b3044    ldr r3, [fp, #-68]
     96c:   e5933000    ldr r3, [r3]
     970:   e0633002    rsb r3, r3, r2
     974:   e50b3020    str r3, [fp, #-32]
     978:   e51b3024    ldr r3, [fp, #-36]
     97c:   e3530000    cmp r3, #0  ; 0x0
     980:   ba000022    blt a10 <MGC_DrcBindEndpoint+0x26c>
     984:   e51b3024    ldr r3, [fp, #-36]
     988:   e6ff2073    uxth    r2, r3
     98c:   e15b31b0    ldrh    r3, [fp, #-16]
     990:   e1530002    cmp r3, r2
     994:   9a000003    bls 9a8 <MGC_DrcBindEndpoint+0x204>
     998:   e51b3024    ldr r3, [fp, #-36]
     99c:   e14b31b0    strh    r3, [fp, #-16]
     9a0:   e51b3028    ldr r3, [fp, #-40]
     9a4:   e50b3018    str r3, [fp, #-24]
     9a8:   e51b3020    ldr r3, [fp, #-32]
     9ac:   e6ff2073    uxth    r2, r3
     9b0:   e15b30be    ldrh    r3, [fp, #-14]
     9b4:   e1530002    cmp r3, r2
     9b8:   9a000014    bls a10 <MGC_DrcBindEndpoint+0x26c>
     9bc:   e51b3020    ldr r3, [fp, #-32]
     9c0:   e14b30be    strh    r3, [fp, #-14]
     9c4:   e51b3028    ldr r3, [fp, #-40]
     9c8:   e50b3014    str r3, [fp, #-20]
     9cc:   ea00000f    b   a10 <MGC_DrcBindEndpoint+0x26c>
     9d0:   e51b3040    ldr r3, [fp, #-64]
     9d4:   e5d33006    ldrb    r3, [r3, #6]
     9d8:   e203300f    and r3, r3, #15 ; 0xf
     9dc:   e54b302c    strb    r3, [fp, #-44]
     9e0:   e15b21ba    ldrh    r2, [fp, #-26]
     9e4:   e15b32ba    ldrh    r3, [fp, #-42]
     9e8:   e1520003    cmp r2, r3
     9ec:   3a000007    bcc a10 <MGC_DrcBindEndpoint+0x26c>
     9f0:   e51b3028    ldr r3, [fp, #-40]
     9f4:   e5d32000    ldrb    r2, [r3]
     9f8:   e55b302c    ldrb    r3, [fp, #-44]
     9fc:   e1530002    cmp r3, r2
     a00:   1a000002    bne a10 <MGC_DrcBindEndpoint+0x26c>
     a04:   e51b3028    ldr r3, [fp, #-40]
     a08:   e50b3018    str r3, [fp, #-24]
     a0c:   ea000006    b   a2c <MGC_DrcBindEndpoint+0x288>
     a10:   e51b3034    ldr r3, [fp, #-52]
     a14:   e2833001    add r3, r3, #1  ; 0x1
     a18:   e50b3034    str r3, [fp, #-52]
     a1c:   e51b2034    ldr r2, [fp, #-52]
     a20:   e51b3038    ldr r3, [fp, #-56]
     a24:   e1520003    cmp r2, r3
     a28:   3affff95    bcc 884 <MGC_DrcBindEndpoint+0xe0>
     a2c:   e51b3014    ldr r3, [fp, #-20]
     a30:   e3530000    cmp r3, #0  ; 0x0
     a34:   0a000001    beq a40 <MGC_DrcBindEndpoint+0x29c>
     a38:   e51b3014    ldr r3, [fp, #-20]
     a3c:   e50b3018    str r3, [fp, #-24]
     a40:   e51b3018    ldr r3, [fp, #-24]
     a44:   e3530000    cmp r3, #0  ; 0x0
     a48:   0a000042    beq b58 <MGC_DrcBindEndpoint+0x3b4>
     a4c:   e55b3048    ldrb    r3, [fp, #-72]
     a50:   e3530000    cmp r3, #0  ; 0x0
     a54:   0a00003f    beq b58 <MGC_DrcBindEndpoint+0x3b4>
     a58:   e51b3018    ldr r3, [fp, #-24]
     a5c:   e5d33001    ldrb    r3, [r3, #1]
     a60:   e3530000    cmp r3, #0  ; 0x0
     a64:   1a000002    bne a74 <MGC_DrcBindEndpoint+0x2d0>
     a68:   e55b302d    ldrb    r3, [fp, #-45]
     a6c:   e3530000    cmp r3, #0  ; 0x0
     a70:   0a000003    beq a84 <MGC_DrcBindEndpoint+0x2e0>
     a74:   e51b3018    ldr r3, [fp, #-24]
     a78:   e3a02001    mov r2, #1  ; 0x1
     a7c:   e5c32010    strb    r2, [r3, #16]
     a80:   ea000002    b   a90 <MGC_DrcBindEndpoint+0x2ec>
     a84:   e51b3018    ldr r3, [fp, #-24]
     a88:   e3a02001    mov r2, #1  ; 0x1
     a8c:   e5c32011    strb    r2, [r3, #17]
     a90:   e51b2018    ldr r2, [fp, #-24]
     a94:   e55b302d    ldrb    r3, [fp, #-45]
     a98:   e5c2300e    strb    r3, [r2, #14]
     a9c:   e51b2018    ldr r2, [fp, #-24]
     aa0:   e3a03000    mov r3, #0  ; 0x0
     aa4:   e5c2300f    strb    r3, [r2, #15]
     aa8:   e55b302d    ldrb    r3, [fp, #-45]
     aac:   e3530000    cmp r3, #0  ; 0x0
     ab0:   0a000014    beq b08 <MGC_DrcBindEndpoint+0x364>
     ab4:   e51b3040    ldr r3, [fp, #-64]
     ab8:   e5933000    ldr r3, [r3]
     abc:   e5d33034    ldrb    r3, [r3, #52]
     ac0:   e51b2018    ldr r2, [fp, #-24]
     ac4:   e5c23006    strb    r3, [r2, #6]
     ac8:   e51b3040    ldr r3, [fp, #-64]
     acc:   e5d33006    ldrb    r3, [r3, #6]
     ad0:   e51b2018    ldr r2, [fp, #-24]
     ad4:   e5c23007    strb    r3, [r2, #7]
     ad8:   e51b3040    ldr r3, [fp, #-64]
     adc:   e5d3300a    ldrb    r3, [r3, #10]
     ae0:   e51b2018    ldr r2, [fp, #-24]
     ae4:   e5c23008    strb    r3, [r2, #8]
     ae8:   e51b3040    ldr r3, [fp, #-64]
     aec:   e5d33007    ldrb    r3, [r3, #7]
     af0:   e51b2018    ldr r2, [fp, #-24]
     af4:   e5c23009    strb    r3, [r2, #9]
     af8:   e51b2018    ldr r2, [fp, #-24]
     afc:   e15b32ba    ldrh    r3, [fp, #-42]
     b00:   e1c231b4    strh    r3, [r2, #20]
     b04:   ea000013    b   b58 <MGC_DrcBindEndpoint+0x3b4>
     b08:   e51b3040    ldr r3, [fp, #-64]
     b0c:   e5933000    ldr r3, [r3]
     b10:   e5d33034    ldrb    r3, [r3, #52]
     b14:   e51b2018    ldr r2, [fp, #-24]
     b18:   e5c2300a    strb    r3, [r2, #10]
     b1c:   e51b3040    ldr r3, [fp, #-64]
     b20:   e5d33006    ldrb    r3, [r3, #6]
     b24:   e51b2018    ldr r2, [fp, #-24]
     b28:   e5c2300b    strb    r3, [r2, #11]
     b2c:   e51b3040    ldr r3, [fp, #-64]
     b30:   e5d3300a    ldrb    r3, [r3, #10]
     b34:   e51b2018    ldr r2, [fp, #-24]
     b38:   e5c2300c    strb    r3, [r2, #12]
     b3c:   e51b3040    ldr r3, [fp, #-64]
     b40:   e5d33007    ldrb    r3, [r3, #7]
     b44:   e51b2018    ldr r2, [fp, #-24]
     b48:   e5c2300d    strb    r3, [r2, #13]
     b4c:   e51b2018    ldr r2, [fp, #-24]
     b50:   e15b32ba    ldrh    r3, [fp, #-42]
     b54:   e1c231b6    strh    r3, [r2, #22]
     b58:   e59b2004    ldr r2, [fp, #4]
     b5c:   e55b302d    ldrb    r3, [fp, #-45]
     b60:   e5c23000    strb    r3, [r2]
     b64:   e51b3018    ldr r3, [fp, #-24]
     b68:   e1a00003    mov r0, r3
     b6c:   e24bd00c    sub sp, fp, #12 ; 0xc
     b70:   e89da800    ldm sp, {fp, sp, pc}

00000b74 <MGC_DrcServiceDefaultEnd>:
     b74:   e1a0c00d    mov ip, sp
     b78:   e92dd810    push    {r4, fp, ip, lr, pc}
     b7c:   e24cb004    sub fp, ip, #4  ; 0x4
     b80:   e24dd034    sub sp, sp, #52 ; 0x34
     b84:   e50b002c    str r0, [fp, #-44]
     b88:   e50b1030    str r1, [fp, #-48]
     b8c:   e50b2034    str r2, [fp, #-52]
     b90:   e14b33b8    strh    r3, [fp, #-56]
     b94:   e51b3034    ldr r3, [fp, #-52]
     b98:   e1d330b0    ldrh    r3, [r3]
     b9c:   e54b3016    strb    r3, [fp, #-22]
     ba0:   e3a03000    mov r3, #0  ; 0x0
     ba4:   e54b3025    strb    r3, [fp, #-37]
     ba8:   e3a03000    mov r3, #0  ; 0x0
     bac:   e54b3015    strb    r3, [fp, #-21]
     bb0:   e3a03000    mov r3, #0  ; 0x0
     bb4:   e54b3026    strb    r3, [fp, #-38]
     bb8:   e51b302c    ldr r3, [fp, #-44]
     bbc:   e283300c    add r3, r3, #12 ; 0xc
     bc0:   e1a00003    mov r0, r3
     bc4:   e3a01000    mov r1, #0  ; 0x0
     bc8:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            bc8: R_ARM_PC24 MUSB_ArrayFetch
     bcc:   e1a03000    mov r3, r0
     bd0:   e50b3014    str r3, [fp, #-20]
     bd4:   e51b302c    ldr r3, [fp, #-44]
     bd8:   e5d3303e    ldrb    r3, [r3, #62]
     bdc:   e3530000    cmp r3, #0  ; 0x0
     be0:   0a000046    beq d00 <MGC_DrcServiceDefaultEnd+0x18c>
     be4:   e55b3026    ldrb    r3, [fp, #-38]
     be8:   e3530000    cmp r3, #0  ; 0x0
     bec:   1a00000e    bne c2c <MGC_DrcServiceDefaultEnd+0xb8>
     bf0:   e51b3014    ldr r3, [fp, #-20]
     bf4:   e5d3300e    ldrb    r3, [r3, #14]
     bf8:   e3530000    cmp r3, #0  ; 0x0
     bfc:   0a000003    beq c10 <MGC_DrcServiceDefaultEnd+0x9c>
     c00:   e51b3014    ldr r3, [fp, #-20]
     c04:   e5933018    ldr r3, [r3, #24]
     c08:   e3530000    cmp r3, #0  ; 0x0
     c0c:   0a000006    beq c2c <MGC_DrcServiceDefaultEnd+0xb8>
     c10:   e51b3014    ldr r3, [fp, #-20]
     c14:   e5d3300e    ldrb    r3, [r3, #14]
     c18:   e3530000    cmp r3, #0  ; 0x0
     c1c:   1a000013    bne c70 <MGC_DrcServiceDefaultEnd+0xfc>
     c20:   e15b33b8    ldrh    r3, [fp, #-56]
     c24:   e3530000    cmp r3, #0  ; 0x0
     c28:   1a000010    bne c70 <MGC_DrcServiceDefaultEnd+0xfc>
     c2c:   e51b2034    ldr r2, [fp, #-52]
     c30:   e3a03000    mov r3, #0  ; 0x0
     c34:   e1c230b0    strh    r3, [r2]
     c38:   e55b3026    ldrb    r3, [fp, #-38]
     c3c:   e1a02003    mov r2, r3
     c40:   e51b3014    ldr r3, [fp, #-20]
     c44:   e593301c    ldr r3, [r3, #28]
     c48:   e58d3000    str r3, [sp]
     c4c:   e51b0030    ldr r0, [fp, #-48]
     c50:   e51b1014    ldr r1, [fp, #-20]
     c54:   e3a03001    mov r3, #1  ; 0x1
     c58:   ebfffffe    bl  0 <MGC_CompleteIrp>
            c58: R_ARM_PC24 MGC_CompleteIrp
     c5c:   e51b002c    ldr r0, [fp, #-44]
     c60:   ebfffffe    bl  0 <MGC_StartNextControlTransfer>
            c60: R_ARM_PC24 MGC_StartNextControlTransfer
     c64:   e3a03001    mov r3, #1  ; 0x1
     c68:   e50b3040    str r3, [fp, #-64]
     c6c:   ea00018d    b   12a8 <MGC_DrcServiceDefaultEnd+0x734>
     c70:   e15b23b8    ldrh    r2, [fp, #-56]
     c74:   e55b3026    ldrb    r3, [fp, #-38]
     c78:   e24bc025    sub ip, fp, #37 ; 0x25
     c7c:   e51b002c    ldr r0, [fp, #-44]
     c80:   e1a01002    mov r1, r2
     c84:   e1a02003    mov r2, r3
     c88:   e1a0300c    mov r3, ip
     c8c:   ebfffffe    bl  3688 <MGC_DrcServiceHostDefault>
            c8c: R_ARM_PC24 MGC_DrcServiceHostDefault
     c90:   e1a03000    mov r3, r0
     c94:   e3530000    cmp r3, #0  ; 0x0
     c98:   1a00017c    bne 1290 <MGC_DrcServiceDefaultEnd+0x71c>
     c9c:   e3a03040    mov r3, #64 ; 0x40
     ca0:   e54b3025    strb    r3, [fp, #-37]
     ca4:   e51b3014    ldr r3, [fp, #-20]
     ca8:   e5d3300e    ldrb    r3, [r3, #14]
     cac:   e3530000    cmp r3, #0  ; 0x0
     cb0:   0a000007    beq cd4 <MGC_DrcServiceDefaultEnd+0x160>
     cb4:   e51b2014    ldr r2, [fp, #-20]
     cb8:   e3a03000    mov r3, #0  ; 0x0
     cbc:   e5c2300e    strb    r3, [r2, #14]
     cc0:   e55b3025    ldrb    r3, [fp, #-37]
     cc4:   e3833020    orr r3, r3, #32 ; 0x20
     cc8:   e6ef3073    uxtb    r3, r3
     ccc:   e54b3025    strb    r3, [fp, #-37]
     cd0:   ea00016e    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
     cd4:   e51b2014    ldr r2, [fp, #-20]
     cd8:   e3a03001    mov r3, #1  ; 0x1
     cdc:   e5c2300e    strb    r3, [r2, #14]
     ce0:   e51b2014    ldr r2, [fp, #-20]
     ce4:   e3a03000    mov r3, #0  ; 0x0
     ce8:   e5823018    str r3, [r2, #24]
     cec:   e55b3025    ldrb    r3, [fp, #-37]
     cf0:   e3833002    orr r3, r3, #2  ; 0x2
     cf4:   e6ef3073    uxtb    r3, r3
     cf8:   e54b3025    strb    r3, [fp, #-37]
     cfc:   ea000163    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
     d00:   e55b3016    ldrb    r3, [fp, #-22]
     d04:   e2033001    and r3, r3, #1  ; 0x1
     d08:   e6ef3073    uxtb    r3, r3
     d0c:   e3530000    cmp r3, #0  ; 0x0
     d10:   0a0000ec    beq 10c8 <MGC_DrcServiceDefaultEnd+0x554>
     d14:   e51b202c    ldr r2, [fp, #-44]
     d18:   e3a03000    mov r3, #0  ; 0x0
     d1c:   e5c23041    strb    r3, [r2, #65]
     d20:   e51b302c    ldr r3, [fp, #-44]
     d24:   e59331e0    ldr r3, [r3, #480]
     d28:   e3530000    cmp r3, #0  ; 0x0
     d2c:   0a000009    beq d58 <MGC_DrcServiceDefaultEnd+0x1e4>
     d30:   e51b302c    ldr r3, [fp, #-44]
     d34:   e59331e0    ldr r3, [r3, #480]
     d38:   e1d311b2    ldrh    r1, [r3, #18]
     d3c:   e51b202c    ldr r2, [fp, #-44]
     d40:   e59f3570    ldr r3, [pc, #1392] ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     d44:   e19230b3    ldrh    r3, [r2, r3]
     d48:   e0633001    rsb r3, r3, r1
     d4c:   e6ff3073    uxth    r3, r3
     d50:   e50b303c    str r3, [fp, #-60]
     d54:   ea000001    b   d60 <MGC_DrcServiceDefaultEnd+0x1ec>
     d58:   e3a03000    mov r3, #0  ; 0x0
     d5c:   e50b303c    str r3, [fp, #-60]
     d60:   e51b303c    ldr r3, [fp, #-60]
     d64:   e14b32b2    strh    r3, [fp, #-34]
     d68:   e15b22b2    ldrh    r2, [fp, #-34]
     d6c:   e15b33b8    ldrh    r3, [fp, #-56]
     d70:   e1520003    cmp r2, r3
     d74:   31a03002    movcc   r3, r2
     d78:   e14b32b4    strh    r3, [fp, #-36]
     d7c:   e51b302c    ldr r3, [fp, #-44]
     d80:   e59331e0    ldr r3, [r3, #480]
     d84:   e3530000    cmp r3, #0  ; 0x0
     d88:   0a000003    beq d9c <MGC_DrcServiceDefaultEnd+0x228>
     d8c:   e15b23b8    ldrh    r2, [fp, #-56]
     d90:   e15b32b4    ldrh    r3, [fp, #-36]
     d94:   e1520003    cmp r2, r3
     d98:   9a000004    bls db0 <MGC_DrcServiceDefaultEnd+0x23c>
     d9c:   e55b3025    ldrb    r3, [fp, #-37]
     da0:   e3833020    orr r3, r3, #32 ; 0x20
     da4:   e6ef3073    uxtb    r3, r3
     da8:   e54b3025    strb    r3, [fp, #-37]
     dac:   ea000137    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
     db0:   e51b302c    ldr r3, [fp, #-44]
     db4:   e59331e0    ldr r3, [r3, #480]
     db8:   e5931014    ldr r1, [r3, #20]
     dbc:   e51b202c    ldr r2, [fp, #-44]
     dc0:   e59f34f0    ldr r3, [pc, #1264] ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     dc4:   e19230b3    ldrh    r3, [r2, r3]
     dc8:   e0813003    add r3, r1, r3
     dcc:   e50b3020    str r3, [fp, #-32]
     dd0:   e51b3014    ldr r3, [fp, #-20]
     dd4:   e5933044    ldr r3, [r3, #68]
     dd8:   e3530000    cmp r3, #0  ; 0x0
     ddc:   0a000020    beq e64 <MGC_DrcServiceDefaultEnd+0x2f0>
     de0:   e51b302c    ldr r3, [fp, #-44]
     de4:   e5933214    ldr r3, [r3, #532]
     de8:   e593401c    ldr r4, [r3, #28]
     dec:   e51b3014    ldr r3, [fp, #-20]
     df0:   e5931044    ldr r1, [r3, #68]
     df4:   e51b3014    ldr r3, [fp, #-20]
     df8:   e1d331b4    ldrh    r3, [r3, #20]
     dfc:   e1a0e003    mov lr, r3
     e00:   e51b3014    ldr r3, [fp, #-20]
     e04:   e5933044    ldr r3, [r3, #68]
     e08:   e5d33010    ldrb    r3, [r3, #16]
     e0c:   e1a0c003    mov ip, r3
     e10:   e15b22b4    ldrh    r2, [fp, #-36]
     e14:   e51b3014    ldr r3, [fp, #-20]
     e18:   e5933044    ldr r3, [r3, #68]
     e1c:   e5933004    ldr r3, [r3, #4]
     e20:   e1520003    cmp r2, r3
     e24:   31a03002    movcc   r3, r2
     e28:   e58d3000    str r3, [sp]
     e2c:   e1a00001    mov r0, r1
     e30:   e1a0100e    mov r1, lr
     e34:   e1a0200c    mov r2, ip
     e38:   e51b3020    ldr r3, [fp, #-32]
     e3c:   e12fff34    blx r4
     e40:   e1a03000    mov r3, r0
     e44:   e3530000    cmp r3, #0  ; 0x0
     e48:   0a000005    beq e64 <MGC_DrcServiceDefaultEnd+0x2f0>
     e4c:   e51b3034    ldr r3, [fp, #-52]
     e50:   e3a02000    mov r2, #0  ; 0x0
     e54:   e1c320b0    strh    r2, [r3]
     e58:   e3a03001    mov r3, #1  ; 0x1
     e5c:   e50b3040    str r3, [fp, #-64]
     e60:   ea000110    b   12a8 <MGC_DrcServiceDefaultEnd+0x734>
     e64:   e51b302c    ldr r3, [fp, #-44]
     e68:   e593c24c    ldr ip, [r3, #588]
     e6c:   e15b32b4    ldrh    r3, [fp, #-36]
     e70:   e51b002c    ldr r0, [fp, #-44]
     e74:   e3a01000    mov r1, #0  ; 0x0
     e78:   e1a02003    mov r2, r3
     e7c:   e51b3020    ldr r3, [fp, #-32]
     e80:   e12fff3c    blx ip
     e84:   e51b202c    ldr r2, [fp, #-44]
     e88:   e59f3428    ldr r3, [pc, #1064] ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     e8c:   e19220b3    ldrh    r2, [r2, r3]
     e90:   e15b32b4    ldrh    r3, [fp, #-36]
     e94:   e0823003    add r3, r2, r3
     e98:   e6ff1073    uxth    r1, r3
     e9c:   e51b202c    ldr r2, [fp, #-44]
     ea0:   e59f3410    ldr r3, [pc, #1040] ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     ea4:   e18210b3    strh    r1, [r2, r3]
     ea8:   e51b302c    ldr r3, [fp, #-44]
     eac:   e59331e0    ldr r3, [r3, #480]
     eb0:   e5933014    ldr r3, [r3, #20]
     eb4:   e50b301c    str r3, [fp, #-28]
     eb8:   e51b301c    ldr r3, [fp, #-28]
     ebc:   e5d33000    ldrb    r3, [r3]
     ec0:   e6af3073    sxtb    r3, r3
     ec4:   e3530000    cmp r3, #0  ; 0x0
     ec8:   ba00000e    blt f08 <MGC_DrcServiceDefaultEnd+0x394>
     ecc:   e51b202c    ldr r2, [fp, #-44]
     ed0:   e59f33e0    ldr r3, [pc, #992]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     ed4:   e19230b3    ldrh    r3, [r2, r3]
     ed8:   e2431008    sub r1, r3, #8  ; 0x8
     edc:   e51b301c    ldr r3, [fp, #-28]
     ee0:   e2833006    add r3, r3, #6  ; 0x6
     ee4:   e2833001    add r3, r3, #1  ; 0x1
     ee8:   e5d33000    ldrb    r3, [r3]
     eec:   e1a02403    lsl r2, r3, #8
     ef0:   e51b301c    ldr r3, [fp, #-28]
     ef4:   e2833006    add r3, r3, #6  ; 0x6
     ef8:   e5d33000    ldrb    r3, [r3]
     efc:   e1823003    orr r3, r2, r3
     f00:   e1510003    cmp r1, r3
     f04:   ba00006c    blt 10bc <MGC_DrcServiceDefaultEnd+0x548>
     f08:   e51b302c    ldr r3, [fp, #-44]
     f0c:   e5933200    ldr r3, [r3, #512]
     f10:   e2832001    add r2, r3, #1  ; 0x1
     f14:   e51b302c    ldr r3, [fp, #-44]
     f18:   e5832200    str r2, [r3, #512]
     f1c:   e51b202c    ldr r2, [fp, #-44]
     f20:   e59f3390    ldr r3, [pc, #912]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     f24:   e19210b3    ldrh    r1, [r2, r3]
     f28:   e51b202c    ldr r2, [fp, #-44]
     f2c:   e59f3388    ldr r3, [pc, #904]  ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
     f30:   e18210b3    strh    r1, [r2, r3]
     f34:   e51b102c    ldr r1, [fp, #-44]
     f38:   e59f2378    ldr r2, [pc, #888]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     f3c:   e3a03000    mov r3, #0  ; 0x0
     f40:   e18130b2    strh    r3, [r1, r2]
     f44:   e3a03000    mov r3, #0  ; 0x0
     f48:   e54b3015    strb    r3, [fp, #-21]
     f4c:   e51b302c    ldr r3, [fp, #-44]
     f50:   e5d331f5    ldrb    r3, [r3, #501]
     f54:   e3530000    cmp r3, #0  ; 0x0
     f58:   0a00003a    beq 1048 <MGC_DrcServiceDefaultEnd+0x4d4>
     f5c:   e24b3026    sub r3, fp, #38 ; 0x26
     f60:   e51b002c    ldr r0, [fp, #-44]
     f64:   e1a01003    mov r1, r3
     f68:   ebfffffe    bl  0 <MGC_FunctionParseSetup>
            f68: R_ARM_PC24 MGC_FunctionParseSetup
     f6c:   e1a03000    mov r3, r0
     f70:   e3530000    cmp r3, #0  ; 0x0
     f74:   1a00001c    bne fec <MGC_DrcServiceDefaultEnd+0x478>
     f78:   e51b302c    ldr r3, [fp, #-44]
     f7c:   e59331e0    ldr r3, [r3, #480]
     f80:   e593401c    ldr r4, [r3, #28]
     f84:   e51b302c    ldr r3, [fp, #-44]
     f88:   e59331e0    ldr r3, [r3, #480]
     f8c:   e5931000    ldr r1, [r3]
     f90:   e51b302c    ldr r3, [fp, #-44]
     f94:   e593e200    ldr lr, [r3, #512]
     f98:   e51b302c    ldr r3, [fp, #-44]
     f9c:   e59331e0    ldr r3, [r3, #480]
     fa0:   e593c014    ldr ip, [r3, #20]
     fa4:   e51b202c    ldr r2, [fp, #-44]
     fa8:   e59f3308    ldr r3, [pc, #776]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
     fac:   e19230b3    ldrh    r3, [r2, r3]
     fb0:   e58d3000    str r3, [sp]
     fb4:   e1a00001    mov r0, r1
     fb8:   e51b102c    ldr r1, [fp, #-44]
     fbc:   e1a0200e    mov r2, lr
     fc0:   e1a0300c    mov r3, ip
     fc4:   e12fff34    blx r4
     fc8:   e1a03000    mov r3, r0
     fcc:   e3530000    cmp r3, #0  ; 0x0
     fd0:   1a0000ae    bne 1290 <MGC_DrcServiceDefaultEnd+0x71c>
     fd4:   e51b302c    ldr r3, [fp, #-44]
     fd8:   e5933238    ldr r3, [r3, #568]
     fdc:   e51b002c    ldr r0, [fp, #-44]
     fe0:   e3a01001    mov r1, #1  ; 0x1
     fe4:   e12fff33    blx r3
     fe8:   ea0000a8    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
     fec:   e55b3026    ldrb    r3, [fp, #-38]
     ff0:   e3530080    cmp r3, #128    ; 0x80
     ff4:   1a00000d    bne 1030 <MGC_DrcServiceDefaultEnd+0x4bc>
     ff8:   e51b102c    ldr r1, [fp, #-44]
     ffc:   e59f22b4    ldr r2, [pc, #692]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    1000:   e3a03000    mov r3, #0  ; 0x0
    1004:   e18130b2    strh    r3, [r1, r2]
    1008:   e51b102c    ldr r1, [fp, #-44]
    100c:   e59f22a8    ldr r2, [pc, #680]  ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
    1010:   e3a03000    mov r3, #0  ; 0x0
    1014:   e18130b2    strh    r3, [r1, r2]
    1018:   e51b302c    ldr r3, [fp, #-44]
    101c:   e5933238    ldr r3, [r3, #568]
    1020:   e51b002c    ldr r0, [fp, #-44]
    1024:   e3a01001    mov r1, #1  ; 0x1
    1028:   e12fff33    blx r3
    102c:   ea000097    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
    1030:   e51b302c    ldr r3, [fp, #-44]
    1034:   e5933238    ldr r3, [r3, #568]
    1038:   e51b002c    ldr r0, [fp, #-44]
    103c:   e3a01000    mov r1, #0  ; 0x0
    1040:   e12fff33    blx r3
    1044:   ea000091    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
    1048:   e51b302c    ldr r3, [fp, #-44]
    104c:   e59331e0    ldr r3, [r3, #480]
    1050:   e593401c    ldr r4, [r3, #28]
    1054:   e51b302c    ldr r3, [fp, #-44]
    1058:   e59331e0    ldr r3, [r3, #480]
    105c:   e5931000    ldr r1, [r3]
    1060:   e51b302c    ldr r3, [fp, #-44]
    1064:   e593e200    ldr lr, [r3, #512]
    1068:   e51b302c    ldr r3, [fp, #-44]
    106c:   e59331e0    ldr r3, [r3, #480]
    1070:   e593c014    ldr ip, [r3, #20]
    1074:   e51b202c    ldr r2, [fp, #-44]
    1078:   e59f3238    ldr r3, [pc, #568]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    107c:   e19230b3    ldrh    r3, [r2, r3]
    1080:   e58d3000    str r3, [sp]
    1084:   e1a00001    mov r0, r1
    1088:   e51b102c    ldr r1, [fp, #-44]
    108c:   e1a0200e    mov r2, lr
    1090:   e1a0300c    mov r3, ip
    1094:   e12fff34    blx r4
    1098:   e1a03000    mov r3, r0
    109c:   e3530000    cmp r3, #0  ; 0x0
    10a0:   1a00007a    bne 1290 <MGC_DrcServiceDefaultEnd+0x71c>
    10a4:   e51b302c    ldr r3, [fp, #-44]
    10a8:   e5933238    ldr r3, [r3, #568]
    10ac:   e51b002c    ldr r0, [fp, #-44]
    10b0:   e3a01001    mov r1, #1  ; 0x1
    10b4:   e12fff33    blx r3
    10b8:   ea000074    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
    10bc:   e3a03040    mov r3, #64 ; 0x40
    10c0:   e54b3025    strb    r3, [fp, #-37]
    10c4:   ea000071    b   1290 <MGC_DrcServiceDefaultEnd+0x71c>
    10c8:   e55b3016    ldrb    r3, [fp, #-22]
    10cc:   e2033002    and r3, r3, #2  ; 0x2
    10d0:   e3530000    cmp r3, #0  ; 0x0
    10d4:   1a00006d    bne 1290 <MGC_DrcServiceDefaultEnd+0x71c>
    10d8:   e51b202c    ldr r2, [fp, #-44]
    10dc:   e59f31d8    ldr r3, [pc, #472]  ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
    10e0:   e19230b3    ldrh    r3, [r2, r3]
    10e4:   e1a01003    mov r1, r3
    10e8:   e51b202c    ldr r2, [fp, #-44]
    10ec:   e59f31c4    ldr r3, [pc, #452]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    10f0:   e19230b3    ldrh    r3, [r2, r3]
    10f4:   e0633001    rsb r3, r3, r1
    10f8:   e3530040    cmp r3, #64 ; 0x40
    10fc:   a3a03040    movge   r3, #64 ; 0x40
    1100:   e14b32b4    strh    r3, [fp, #-36]
    1104:   e15b32b4    ldrh    r3, [fp, #-36]
    1108:   e3530000    cmp r3, #0  ; 0x0
    110c:   1a00000a    bne 113c <MGC_DrcServiceDefaultEnd+0x5c8>
    1110:   e51b102c    ldr r1, [fp, #-44]
    1114:   e59f219c    ldr r2, [pc, #412]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    1118:   e3a03000    mov r3, #0  ; 0x0
    111c:   e18130b2    strh    r3, [r1, r2]
    1120:   e51b102c    ldr r1, [fp, #-44]
    1124:   e59f2190    ldr r2, [pc, #400]  ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
    1128:   e3a03000    mov r3, #0  ; 0x0
    112c:   e18130b2    strh    r3, [r1, r2]
    1130:   e3a03000    mov r3, #0  ; 0x0
    1134:   e50b3040    str r3, [fp, #-64]
    1138:   ea00005a    b   12a8 <MGC_DrcServiceDefaultEnd+0x734>
    113c:   e51b302c    ldr r3, [fp, #-44]
    1140:   e59311fc    ldr r1, [r3, #508]
    1144:   e51b202c    ldr r2, [fp, #-44]
    1148:   e59f3168    ldr r3, [pc, #360]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    114c:   e19230b3    ldrh    r3, [r2, r3]
    1150:   e0813003    add r3, r1, r3
    1154:   e50b3020    str r3, [fp, #-32]
    1158:   e51b3014    ldr r3, [fp, #-20]
    115c:   e5933044    ldr r3, [r3, #68]
    1160:   e3530000    cmp r3, #0  ; 0x0
    1164:   0a000020    beq 11ec <MGC_DrcServiceDefaultEnd+0x678>
    1168:   e51b302c    ldr r3, [fp, #-44]
    116c:   e5933214    ldr r3, [r3, #532]
    1170:   e593401c    ldr r4, [r3, #28]
    1174:   e51b3014    ldr r3, [fp, #-20]
    1178:   e5931044    ldr r1, [r3, #68]
    117c:   e51b3014    ldr r3, [fp, #-20]
    1180:   e1d331b4    ldrh    r3, [r3, #20]
    1184:   e1a0e003    mov lr, r3
    1188:   e51b3014    ldr r3, [fp, #-20]
    118c:   e5933044    ldr r3, [r3, #68]
    1190:   e5d33010    ldrb    r3, [r3, #16]
    1194:   e1a0c003    mov ip, r3
    1198:   e15b22b4    ldrh    r2, [fp, #-36]
    119c:   e51b3014    ldr r3, [fp, #-20]
    11a0:   e5933044    ldr r3, [r3, #68]
    11a4:   e5933004    ldr r3, [r3, #4]
    11a8:   e1520003    cmp r2, r3
    11ac:   31a03002    movcc   r3, r2
    11b0:   e58d3000    str r3, [sp]
    11b4:   e1a00001    mov r0, r1
    11b8:   e1a0100e    mov r1, lr
    11bc:   e1a0200c    mov r2, ip
    11c0:   e51b3020    ldr r3, [fp, #-32]
    11c4:   e12fff34    blx r4
    11c8:   e1a03000    mov r3, r0
    11cc:   e3530000    cmp r3, #0  ; 0x0
    11d0:   0a000005    beq 11ec <MGC_DrcServiceDefaultEnd+0x678>
    11d4:   e51b3034    ldr r3, [fp, #-52]
    11d8:   e3a02000    mov r2, #0  ; 0x0
    11dc:   e1c320b0    strh    r2, [r3]
    11e0:   e3a03001    mov r3, #1  ; 0x1
    11e4:   e50b3040    str r3, [fp, #-64]
    11e8:   ea00002e    b   12a8 <MGC_DrcServiceDefaultEnd+0x734>
    11ec:   e51b302c    ldr r3, [fp, #-44]
    11f0:   e593c248    ldr ip, [r3, #584]
    11f4:   e15b32b4    ldrh    r3, [fp, #-36]
    11f8:   e51b002c    ldr r0, [fp, #-44]
    11fc:   e3a01000    mov r1, #0  ; 0x0
    1200:   e1a02003    mov r2, r3
    1204:   e51b3020    ldr r3, [fp, #-32]
    1208:   e12fff3c    blx ip
    120c:   e51b202c    ldr r2, [fp, #-44]
    1210:   e59f30a0    ldr r3, [pc, #160]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    1214:   e19220b3    ldrh    r2, [r2, r3]
    1218:   e15b32b4    ldrh    r3, [fp, #-36]
    121c:   e0823003    add r3, r2, r3
    1220:   e6ff1073    uxth    r1, r3
    1224:   e51b202c    ldr r2, [fp, #-44]
    1228:   e59f3088    ldr r3, [pc, #136]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    122c:   e18210b3    strh    r1, [r2, r3]
    1230:   e55b3025    ldrb    r3, [fp, #-37]
    1234:   e3833002    orr r3, r3, #2  ; 0x2
    1238:   e6ef3073    uxtb    r3, r3
    123c:   e54b3025    strb    r3, [fp, #-37]
    1240:   e51b202c    ldr r2, [fp, #-44]
    1244:   e59f306c    ldr r3, [pc, #108]  ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    1248:   e19210b3    ldrh    r1, [r2, r3]
    124c:   e51b202c    ldr r2, [fp, #-44]
    1250:   e59f3064    ldr r3, [pc, #100]  ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
    1254:   e19230b3    ldrh    r3, [r2, r3]
    1258:   e1510003    cmp r1, r3
    125c:   3a00000b    bcc 1290 <MGC_DrcServiceDefaultEnd+0x71c>
    1260:   e55b3025    ldrb    r3, [fp, #-37]
    1264:   e3833008    orr r3, r3, #8  ; 0x8
    1268:   e6ef3073    uxtb    r3, r3
    126c:   e54b3025    strb    r3, [fp, #-37]
    1270:   e51b102c    ldr r1, [fp, #-44]
    1274:   e59f203c    ldr r2, [pc, #60]   ; 12b8 <MGC_DrcServiceDefaultEnd+0x744>
    1278:   e3a03000    mov r3, #0  ; 0x0
    127c:   e18130b2    strh    r3, [r1, r2]
    1280:   e51b102c    ldr r1, [fp, #-44]
    1284:   e59f2030    ldr r2, [pc, #48]   ; 12bc <MGC_DrcServiceDefaultEnd+0x748>
    1288:   e3a03000    mov r3, #0  ; 0x0
    128c:   e18130b2    strh    r3, [r1, r2]
    1290:   e55b3025    ldrb    r3, [fp, #-37]
    1294:   e1a02003    mov r2, r3
    1298:   e51b3034    ldr r3, [fp, #-52]
    129c:   e1c320b0    strh    r2, [r3]
    12a0:   e55b3015    ldrb    r3, [fp, #-21]
    12a4:   e50b3040    str r3, [fp, #-64]
    12a8:   e51b3040    ldr r3, [fp, #-64]
    12ac:   e1a00003    mov r0, r3
    12b0:   e24bd010    sub sp, fp, #16 ; 0x10
    12b4:   e89da810    ldm sp, {r4, fp, sp, pc}
    12b8:   000001f6    .word   0x000001f6
    12bc:   000001fa    .word   0x000001fa

000012c0 <MGC_DrcResetPort>:
    12c0:   e1a0c00d    mov ip, sp
    12c4:   e92dd800    push    {fp, ip, lr, pc}
    12c8:   e24cb004    sub fp, ip, #4  ; 0x4
    12cc:   e24dd008    sub sp, sp, #8  ; 0x8
    12d0:   e50b0014    str r0, [fp, #-20]
    12d4:   e51b3014    ldr r3, [fp, #-20]
    12d8:   e5933000    ldr r3, [r3]
    12dc:   e50b3010    str r3, [fp, #-16]
    12e0:   e51b3010    ldr r3, [fp, #-16]
    12e4:   e593300c    ldr r3, [r3, #12]
    12e8:   e593201c    ldr r2, [r3, #28]
    12ec:   e51b3010    ldr r3, [fp, #-16]
    12f0:   e593300c    ldr r3, [r3, #12]
    12f4:   e5933004    ldr r3, [r3, #4]
    12f8:   e1a00003    mov r0, r3
    12fc:   e3a01000    mov r1, #0  ; 0x0
    1300:   e12fff32    blx r2
    1304:   e51b2014    ldr r2, [fp, #-20]
    1308:   e3a03000    mov r3, #0  ; 0x0
    130c:   e5c23029    strb    r3, [r2, #41]
    1310:   e51b2014    ldr r2, [fp, #-20]
    1314:   e3a03000    mov r3, #0  ; 0x0
    1318:   e5c23028    strb    r3, [r2, #40]
    131c:   e51b3014    ldr r3, [fp, #-20]
    1320:   e593321c    ldr r3, [r3, #540]
    1324:   e51b0014    ldr r0, [fp, #-20]
    1328:   e12fff33    blx r3
    132c:   e51b3010    ldr r3, [fp, #-16]
    1330:   e5933020    ldr r3, [r3, #32]
    1334:   e51b0010    ldr r0, [fp, #-16]
    1338:   e12fff33    blx r3
    133c:   e51b3010    ldr r3, [fp, #-16]
    1340:   e593300c    ldr r3, [r3, #12]
    1344:   e5932014    ldr r2, [r3, #20]
    1348:   e51b3010    ldr r3, [fp, #-16]
    134c:   e593300c    ldr r3, [r3, #12]
    1350:   e5933004    ldr r3, [r3, #4]
    1354:   e1a00003    mov r0, r3
    1358:   e12fff32    blx r2
    135c:   e51b0014    ldr r0, [fp, #-20]
    1360:   eb000019    bl  13cc <MGC_DrcFlushAll>
    1364:   e51b3010    ldr r3, [fp, #-16]
    1368:   e593301c    ldr r3, [r3, #28]
    136c:   e51b0010    ldr r0, [fp, #-16]
    1370:   e12fff33    blx r3
    1374:   e51b3014    ldr r3, [fp, #-20]
    1378:   e5933218    ldr r3, [r3, #536]
    137c:   e51b0014    ldr r0, [fp, #-20]
    1380:   e12fff33    blx r3
    1384:   e51b2014    ldr r2, [fp, #-20]
    1388:   e3a03000    mov r3, #0  ; 0x0
    138c:   e5c23025    strb    r3, [r2, #37]
    1390:   e51b2014    ldr r2, [fp, #-20]
    1394:   e3a03000    mov r3, #0  ; 0x0
    1398:   e5c231f0    strb    r3, [r2, #496]
    139c:   e51b2014    ldr r2, [fp, #-20]
    13a0:   e3a03000    mov r3, #0  ; 0x0
    13a4:   e5c2302a    strb    r3, [r2, #42]
    13a8:   e51b2014    ldr r2, [fp, #-20]
    13ac:   e3a03000    mov r3, #0  ; 0x0
    13b0:   e5c2304c    strb    r3, [r2, #76]
    13b4:   e51b0014    ldr r0, [fp, #-20]
    13b8:   ebfffffe    bl  25bc <MGC_CompleteOtgTransition>
            13b8: R_ARM_PC24    MGC_CompleteOtgTransition
    13bc:   e3a03000    mov r3, #0  ; 0x0
    13c0:   e1a00003    mov r0, r3
    13c4:   e24bd00c    sub sp, fp, #12 ; 0xc
    13c8:   e89da800    ldm sp, {fp, sp, pc}

000013cc <MGC_DrcFlushAll>:
    13cc:   e1a0c00d    mov ip, sp
    13d0:   e92dd800    push    {fp, ip, lr, pc}
    13d4:   e24cb004    sub fp, ip, #4  ; 0x4
    13d8:   e24dd01c    sub sp, sp, #28 ; 0x1c
    13dc:   e50b0028    str r0, [fp, #-40]
    13e0:   e51b3028    ldr r3, [fp, #-40]
    13e4:   e5933000    ldr r3, [r3]
    13e8:   e5933004    ldr r3, [r3, #4]
    13ec:   e50b3010    str r3, [fp, #-16]
    13f0:   e51b3028    ldr r3, [fp, #-40]
    13f4:   e283300c    add r3, r3, #12 ; 0xc
    13f8:   e1a00003    mov r0, r3
    13fc:   ebfffffe    bl  0 <MUSB_ArrayLength>
            13fc: R_ARM_PC24    MUSB_ArrayLength
    1400:   e1a03000    mov r3, r0
    1404:   e50b3024    str r3, [fp, #-36]
    1408:   e3a03001    mov r3, #1  ; 0x1
    140c:   e50b3020    str r3, [fp, #-32]
    1410:   ea00006e    b   15d0 <MGC_DrcFlushAll+0x204>
    1414:   e51b3028    ldr r3, [fp, #-40]
    1418:   e283300c    add r3, r3, #12 ; 0xc
    141c:   e1a00003    mov r0, r3
    1420:   e51b1020    ldr r1, [fp, #-32]
    1424:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            1424: R_ARM_PC24    MUSB_ArrayFetch
    1428:   e1a03000    mov r3, r0
    142c:   e50b3014    str r3, [fp, #-20]
    1430:   e51b3014    ldr r3, [fp, #-20]
    1434:   e3530000    cmp r3, #0  ; 0x0
    1438:   0a000061    beq 15c4 <MGC_DrcFlushAll+0x1f8>
    143c:   e51b2014    ldr r2, [fp, #-20]
    1440:   e3a03000    mov r3, #0  ; 0x0
    1444:   e5c23011    strb    r3, [r2, #17]
    1448:   e51b3014    ldr r3, [fp, #-20]
    144c:   e5d33011    ldrb    r3, [r3, #17]
    1450:   e51b2014    ldr r2, [fp, #-20]
    1454:   e5c23010    strb    r3, [r2, #16]
    1458:   e51b2014    ldr r2, [fp, #-20]
    145c:   e3a03000    mov r3, #0  ; 0x0
    1460:   e5c23013    strb    r3, [r2, #19]
    1464:   e51b3014    ldr r3, [fp, #-20]
    1468:   e5d33013    ldrb    r3, [r3, #19]
    146c:   e51b2014    ldr r2, [fp, #-20]
    1470:   e5c2300f    strb    r3, [r2, #15]
    1474:   e51b2014    ldr r2, [fp, #-20]
    1478:   e3a03000    mov r3, #0  ; 0x0
    147c:   e5823030    str r3, [r2, #48]
    1480:   e51b3014    ldr r3, [fp, #-20]
    1484:   e5932030    ldr r2, [r3, #48]
    1488:   e51b3014    ldr r3, [fp, #-20]
    148c:   e583201c    str r2, [r3, #28]
    1490:   e51b2014    ldr r2, [fp, #-20]
    1494:   e3a03000    mov r3, #0  ; 0x0
    1498:   e5c23051    strb    r3, [r2, #81]
    149c:   e51b3014    ldr r3, [fp, #-20]
    14a0:   e5d33051    ldrb    r3, [r3, #81]
    14a4:   e51b2014    ldr r2, [fp, #-20]
    14a8:   e5c23050    strb    r3, [r2, #80]
    14ac:   e51b3028    ldr r3, [fp, #-40]
    14b0:   e593c230    ldr ip, [r3, #560]
    14b4:   e51b0028    ldr r0, [fp, #-40]
    14b8:   e51b1014    ldr r1, [fp, #-20]
    14bc:   e3a02000    mov r2, #0  ; 0x0
    14c0:   e3a03001    mov r3, #1  ; 0x1
    14c4:   e12fff3c    blx ip
    14c8:   e51b3028    ldr r3, [fp, #-40]
    14cc:   e593c230    ldr ip, [r3, #560]
    14d0:   e51b0028    ldr r0, [fp, #-40]
    14d4:   e51b1014    ldr r1, [fp, #-20]
    14d8:   e3a02080    mov r2, #128    ; 0x80
    14dc:   e3a03001    mov r3, #1  ; 0x1
    14e0:   e12fff3c    blx ip
    14e4:   e51b3014    ldr r3, [fp, #-20]
    14e8:   e2833020    add r3, r3, #32 ; 0x20
    14ec:   e1a00003    mov r0, r3
    14f0:   ebfffffe    bl  0 <MUSB_ListLength>
            14f0: R_ARM_PC24    MUSB_ListLength
    14f4:   e1a03000    mov r3, r0
    14f8:   e14b31bc    strh    r3, [fp, #-28]
    14fc:   e3a03000    mov r3, #0  ; 0x0
    1500:   e14b31ba    strh    r3, [fp, #-26]
    1504:   ea00000e    b   1544 <MGC_DrcFlushAll+0x178>
    1508:   e51b3014    ldr r3, [fp, #-20]
    150c:   e2833020    add r3, r3, #32 ; 0x20
    1510:   e1a00003    mov r0, r3
    1514:   e3a01000    mov r1, #0  ; 0x0
    1518:   ebfffffe    bl  0 <MUSB_ListFindItem>
            1518: R_ARM_PC24    MUSB_ListFindItem
    151c:   e1a03000    mov r3, r0
    1520:   e50b3018    str r3, [fp, #-24]
    1524:   e51b3014    ldr r3, [fp, #-20]
    1528:   e2833020    add r3, r3, #32 ; 0x20
    152c:   e1a00003    mov r0, r3
    1530:   e51b1018    ldr r1, [fp, #-24]
    1534:   ebfffffe    bl  0 <MUSB_ListRemoveItem>
            1534: R_ARM_PC24    MUSB_ListRemoveItem
    1538:   e15b31ba    ldrh    r3, [fp, #-26]
    153c:   e2833001    add r3, r3, #1  ; 0x1
    1540:   e14b31ba    strh    r3, [fp, #-26]
    1544:   e15b21ba    ldrh    r2, [fp, #-26]
    1548:   e15b31bc    ldrh    r3, [fp, #-28]
    154c:   e1520003    cmp r2, r3
    1550:   3affffec    bcc 1508 <MGC_DrcFlushAll+0x13c>
    1554:   e51b3014    ldr r3, [fp, #-20]
    1558:   e2833034    add r3, r3, #52 ; 0x34
    155c:   e1a00003    mov r0, r3
    1560:   ebfffffe    bl  0 <MUSB_ListLength>
            1560: R_ARM_PC24    MUSB_ListLength
    1564:   e1a03000    mov r3, r0
    1568:   e14b31bc    strh    r3, [fp, #-28]
    156c:   e3a03000    mov r3, #0  ; 0x0
    1570:   e14b31ba    strh    r3, [fp, #-26]
    1574:   ea00000e    b   15b4 <MGC_DrcFlushAll+0x1e8>
    1578:   e51b3014    ldr r3, [fp, #-20]
    157c:   e2833034    add r3, r3, #52 ; 0x34
    1580:   e1a00003    mov r0, r3
    1584:   e3a01000    mov r1, #0  ; 0x0
    1588:   ebfffffe    bl  0 <MUSB_ListFindItem>
            1588: R_ARM_PC24    MUSB_ListFindItem
    158c:   e1a03000    mov r3, r0
    1590:   e50b3018    str r3, [fp, #-24]
    1594:   e51b3014    ldr r3, [fp, #-20]
    1598:   e2833034    add r3, r3, #52 ; 0x34
    159c:   e1a00003    mov r0, r3
    15a0:   e51b1018    ldr r1, [fp, #-24]
    15a4:   ebfffffe    bl  0 <MUSB_ListRemoveItem>
            15a4: R_ARM_PC24    MUSB_ListRemoveItem
    15a8:   e15b31ba    ldrh    r3, [fp, #-26]
    15ac:   e2833001    add r3, r3, #1  ; 0x1
    15b0:   e14b31ba    strh    r3, [fp, #-26]
    15b4:   e15b21ba    ldrh    r2, [fp, #-26]
    15b8:   e15b31bc    ldrh    r3, [fp, #-28]
    15bc:   e1520003    cmp r2, r3
    15c0:   3affffec    bcc 1578 <MGC_DrcFlushAll+0x1ac>
    15c4:   e51b3020    ldr r3, [fp, #-32]
    15c8:   e2833001    add r3, r3, #1  ; 0x1
    15cc:   e50b3020    str r3, [fp, #-32]
    15d0:   e51b2020    ldr r2, [fp, #-32]
    15d4:   e51b3024    ldr r3, [fp, #-36]
    15d8:   e1520003    cmp r2, r3
    15dc:   3affff8c    bcc 1414 <MGC_DrcFlushAll+0x48>
    15e0:   e51b0010    ldr r0, [fp, #-16]
    15e4:   e3a0100e    mov r1, #14 ; 0xe
    15e8:   e3a02000    mov r2, #0  ; 0x0
    15ec:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            15ec: R_ARM_PC24    MGC_FAPI_WriteReg8
    15f0:   e24bd00c    sub sp, fp, #12 ; 0xc
    15f4:   e89da800    ldm sp, {fp, sp, pc}

000015f8 <MGC_DrcOtgTimer>:
    15f8:   e1a0c00d    mov ip, sp
    15fc:   e92dd800    push    {fp, ip, lr, pc}
    1600:   e24cb004    sub fp, ip, #4  ; 0x4
    1604:   e24dd018    sub sp, sp, #24 ; 0x18
    1608:   e50b001c    str r0, [fp, #-28]
    160c:   e1a03001    mov r3, r1
    1610:   e14b32b0    strh    r3, [fp, #-32]
    1614:   e51b301c    ldr r3, [fp, #-28]
    1618:   e50b3018    str r3, [fp, #-24]
    161c:   e51b3018    ldr r3, [fp, #-24]
    1620:   e5933014    ldr r3, [r3, #20]
    1624:   e50b3014    str r3, [fp, #-20]
    1628:   e51b3014    ldr r3, [fp, #-20]
    162c:   e5933050    ldr r3, [r3, #80]
    1630:   e50b3010    str r3, [fp, #-16]
    1634:   e51b3014    ldr r3, [fp, #-20]
    1638:   e5933218    ldr r3, [r3, #536]
    163c:   e51b0014    ldr r0, [fp, #-20]
    1640:   e12fff33    blx r3
    1644:   e51b3014    ldr r3, [fp, #-20]
    1648:   e5d3304c    ldrb    r3, [r3, #76]
    164c:   e50b3024    str r3, [fp, #-36]
    1650:   e51b3024    ldr r3, [fp, #-36]
    1654:   e3530012    cmp r3, #18 ; 0x12
    1658:   0a000006    beq 1678 <MGC_DrcOtgTimer+0x80>
    165c:   e51b3024    ldr r3, [fp, #-36]
    1660:   e3530013    cmp r3, #19 ; 0x13
    1664:   0a000025    beq 1700 <MGC_DrcOtgTimer+0x108>
    1668:   e51b3024    ldr r3, [fp, #-36]
    166c:   e3530011    cmp r3, #17 ; 0x11
    1670:   0a000022    beq 1700 <MGC_DrcOtgTimer+0x108>
    1674:   ea000028    b   171c <MGC_DrcOtgTimer+0x124>
    1678:   e51b3014    ldr r3, [fp, #-20]
    167c:   e5d33040    ldrb    r3, [r3, #64]
    1680:   e3530001    cmp r3, #1  ; 0x1
    1684:   8a000024    bhi 171c <MGC_DrcOtgTimer+0x124>
    1688:   e59f3094    ldr r3, [pc, #148]  ; 1724 <MGC_DrcOtgTimer+0x12c>
    168c:   e5d33000    ldrb    r3, [r3]
    1690:   e3530000    cmp r3, #0  ; 0x0
    1694:   0a00000a    beq 16c4 <MGC_DrcOtgTimer+0xcc>
    1698:   e51b3014    ldr r3, [fp, #-20]
    169c:   e5933000    ldr r3, [r3]
    16a0:   e593300c    ldr r3, [r3, #12]
    16a4:   e5932028    ldr r2, [r3, #40]
    16a8:   e51b3014    ldr r3, [fp, #-20]
    16ac:   e5933000    ldr r3, [r3]
    16b0:   e593300c    ldr r3, [r3, #12]
    16b4:   e5933004    ldr r3, [r3, #4]
    16b8:   e1a00003    mov r0, r3
    16bc:   e59f1064    ldr r1, [pc, #100]  ; 1728 <MGC_DrcOtgTimer+0x130>
    16c0:   e12fff32    blx r2
    16c4:   e51b3010    ldr r3, [fp, #-16]
    16c8:   e3530000    cmp r3, #0  ; 0x0
    16cc:   0a000007    beq 16f0 <MGC_DrcOtgTimer+0xf8>
    16d0:   e51b3010    ldr r3, [fp, #-16]
    16d4:   e593c00c    ldr ip, [r3, #12]
    16d8:   e51b3010    ldr r3, [fp, #-16]
    16dc:   e5933000    ldr r3, [r3]
    16e0:   e1a00003    mov r0, r3
    16e4:   e51b1010    ldr r1, [fp, #-16]
    16e8:   e3a020b1    mov r2, #177    ; 0xb1
    16ec:   e12fff3c    blx ip
    16f0:   e3a00000    mov r0, #0  ; 0x0
    16f4:   e51b1014    ldr r1, [fp, #-20]
    16f8:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            16f8: R_ARM_PC24    MGC_DrcChangeOtgState
    16fc:   ea000006    b   171c <MGC_DrcOtgTimer+0x124>
    1700:   e51b3014    ldr r3, [fp, #-20]
    1704:   e5d33040    ldrb    r3, [r3, #64]
    1708:   e3530001    cmp r3, #1  ; 0x1
    170c:   8a000002    bhi 171c <MGC_DrcOtgTimer+0x124>
    1710:   e3a00000    mov r0, #0  ; 0x0
    1714:   e51b1014    ldr r1, [fp, #-20]
    1718:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            1718: R_ARM_PC24    MGC_DrcChangeOtgState
    171c:   e24bd00c    sub sp, fp, #12 ; 0xc
    1720:   e89da800    ldm sp, {fp, sp, pc}
    1724:   00000000    .word   0x00000000
            1724: R_ARM_ABS32   MGC_bDiagLevel
    1728:   00000030    .word   0x00000030
            1728: R_ARM_ABS32   .rodata

0000172c <MGC_ExitingHost>:
    172c:   e1a0c00d    mov ip, sp
    1730:   e92dd800    push    {fp, ip, lr, pc}
    1734:   e24cb004    sub fp, ip, #4  ; 0x4
    1738:   e24dd00c    sub sp, sp, #12 ; 0xc
    173c:   e1a03000    mov r3, r0
    1740:   e50b1014    str r1, [fp, #-20]
    1744:   e54b3010    strb    r3, [fp, #-16]
    1748:   e55b3010    ldrb    r3, [fp, #-16]
    174c:   e3530024    cmp r3, #36 ; 0x24
    1750:   1a000002    bne 1760 <MGC_ExitingHost+0x34>
    1754:   e3a03000    mov r3, #0  ; 0x0
    1758:   e50b3018    str r3, [fp, #-24]
    175c:   ea000012    b   17ac <MGC_ExitingHost+0x80>
    1760:   e55b3010    ldrb    r3, [fp, #-16]
    1764:   e3530021    cmp r3, #33 ; 0x21
    1768:   1a000002    bne 1778 <MGC_ExitingHost+0x4c>
    176c:   e3a03001    mov r3, #1  ; 0x1
    1770:   e50b3018    str r3, [fp, #-24]
    1774:   ea00000c    b   17ac <MGC_ExitingHost+0x80>
    1778:   e51b3014    ldr r3, [fp, #-20]
    177c:   e5d3304c    ldrb    r3, [r3, #76]
    1780:   e3530023    cmp r3, #35 ; 0x23
    1784:   0a000003    beq 1798 <MGC_ExitingHost+0x6c>
    1788:   e51b3014    ldr r3, [fp, #-20]
    178c:   e5d3304c    ldrb    r3, [r3, #76]
    1790:   e3530013    cmp r3, #19 ; 0x13
    1794:   1a000002    bne 17a4 <MGC_ExitingHost+0x78>
    1798:   e3a03001    mov r3, #1  ; 0x1
    179c:   e50b3018    str r3, [fp, #-24]
    17a0:   ea000001    b   17ac <MGC_ExitingHost+0x80>
    17a4:   e3a03000    mov r3, #0  ; 0x0
    17a8:   e50b3018    str r3, [fp, #-24]
    17ac:   e51b3018    ldr r3, [fp, #-24]
    17b0:   e1a00003    mov r0, r3
    17b4:   e24bd00c    sub sp, fp, #12 ; 0xc
    17b8:   e89da800    ldm sp, {fp, sp, pc}

000017bc <MGC_ExitActiveUsbMode>:
    17bc:   e1a0c00d    mov ip, sp
    17c0:   e92dd800    push    {fp, ip, lr, pc}
    17c4:   e24cb004    sub fp, ip, #4  ; 0x4
    17c8:   e24dd008    sub sp, sp, #8  ; 0x8
    17cc:   e1a03000    mov r3, r0
    17d0:   e50b1014    str r1, [fp, #-20]
    17d4:   e54b3010    strb    r3, [fp, #-16]
    17d8:   e51b0014    ldr r0, [fp, #-20]
    17dc:   e3a01001    mov r1, #1  ; 0x1
    17e0:   ebfffffe    bl  0 <MGC_FunctionChangeState>
            17e0: R_ARM_PC24    MGC_FunctionChangeState
    17e4:   e51b0014    ldr r0, [fp, #-20]
    17e8:   ebfffef7    bl  13cc <MGC_DrcFlushAll>
    17ec:   e51b2014    ldr r2, [fp, #-20]
    17f0:   e3a03000    mov r3, #0  ; 0x0
    17f4:   e5c23028    strb    r3, [r2, #40]
    17f8:   e51b3014    ldr r3, [fp, #-20]
    17fc:   e593321c    ldr r3, [r3, #540]
    1800:   e51b0014    ldr r0, [fp, #-20]
    1804:   e12fff33    blx r3
    1808:   e24bd00c    sub sp, fp, #12 ; 0xc
    180c:   e89da800    ldm sp, {fp, sp, pc}

00001810 <MGC_SetPowerSave>:
    1810:   e1a0c00d    mov ip, sp
    1814:   e92dd800    push    {fp, ip, lr, pc}
    1818:   e24cb004    sub fp, ip, #4  ; 0x4
    181c:   e24dd008    sub sp, sp, #8  ; 0x8
    1820:   e50b0010    str r0, [fp, #-16]
    1824:   e1a03001    mov r3, r1
    1828:   e54b3014    strb    r3, [fp, #-20]
    182c:   e24bd00c    sub sp, fp, #12 ; 0xc
    1830:   e89da800    ldm sp, {fp, sp, pc}

00001834 <MGC_StartPeripheralMode>:
    1834:   e1a0c00d    mov ip, sp
    1838:   e92dd800    push    {fp, ip, lr, pc}
    183c:   e24cb004    sub fp, ip, #4  ; 0x4
    1840:   e24dd004    sub sp, sp, #4  ; 0x4
    1844:   e50b0010    str r0, [fp, #-16]
    1848:   e99da800    ldmib   sp, {fp, sp, pc}

0000184c <MGC_AidlBdisTimeout>:
    184c:   e1a0c00d    mov ip, sp
    1850:   e92dd800    push    {fp, ip, lr, pc}
    1854:   e24cb004    sub fp, ip, #4  ; 0x4
    1858:   e24dd014    sub sp, sp, #20 ; 0x14
    185c:   e50b001c    str r0, [fp, #-28]
    1860:   e1a03001    mov r3, r1
    1864:   e14b32b0    strh    r3, [fp, #-32]
    1868:   e51b301c    ldr r3, [fp, #-28]
    186c:   e50b3018    str r3, [fp, #-24]
    1870:   e51b3018    ldr r3, [fp, #-24]
    1874:   e5933014    ldr r3, [r3, #20]
    1878:   e50b3014    str r3, [fp, #-20]
    187c:   e51b3014    ldr r3, [fp, #-20]
    1880:   e5933050    ldr r3, [r3, #80]
    1884:   e50b3010    str r3, [fp, #-16]
    1888:   e51b3014    ldr r3, [fp, #-20]
    188c:   e5d3303b    ldrb    r3, [r3, #59]
    1890:   e3530000    cmp r3, #0  ; 0x0
    1894:   1a000021    bne 1920 <MGC_AidlBdisTimeout+0xd4>
    1898:   e51b3014    ldr r3, [fp, #-20]
    189c:   e5d3303a    ldrb    r3, [r3, #58]
    18a0:   e3530000    cmp r3, #0  ; 0x0
    18a4:   0a000000    beq 18ac <MGC_AidlBdisTimeout+0x60>
    18a8:   ea00001c    b   1920 <MGC_AidlBdisTimeout+0xd4>
    18ac:   e59f3074    ldr r3, [pc, #116]  ; 1928 <MGC_AidlBdisTimeout+0xdc>
    18b0:   e5d33000    ldrb    r3, [r3]
    18b4:   e3530000    cmp r3, #0  ; 0x0
    18b8:   0a00000a    beq 18e8 <MGC_AidlBdisTimeout+0x9c>
    18bc:   e51b3014    ldr r3, [fp, #-20]
    18c0:   e5933000    ldr r3, [r3]
    18c4:   e593300c    ldr r3, [r3, #12]
    18c8:   e5932028    ldr r2, [r3, #40]
    18cc:   e51b3014    ldr r3, [fp, #-20]
    18d0:   e5933000    ldr r3, [r3]
    18d4:   e593300c    ldr r3, [r3, #12]
    18d8:   e5933004    ldr r3, [r3, #4]
    18dc:   e1a00003    mov r0, r3
    18e0:   e59f1044    ldr r1, [pc, #68]   ; 192c <MGC_AidlBdisTimeout+0xe0>
    18e4:   e12fff32    blx r2
    18e8:   e51b3010    ldr r3, [fp, #-16]
    18ec:   e3530000    cmp r3, #0  ; 0x0
    18f0:   0a000007    beq 1914 <MGC_AidlBdisTimeout+0xc8>
    18f4:   e51b3010    ldr r3, [fp, #-16]
    18f8:   e593c00c    ldr ip, [r3, #12]
    18fc:   e51b3010    ldr r3, [fp, #-16]
    1900:   e5933000    ldr r3, [r3]
    1904:   e1a00003    mov r0, r3
    1908:   e51b1010    ldr r1, [fp, #-16]
    190c:   e3a020b1    mov r2, #177    ; 0xb1
    1910:   e12fff3c    blx ip
    1914:   e3a00000    mov r0, #0  ; 0x0
    1918:   e51b1014    ldr r1, [fp, #-20]
    191c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            191c: R_ARM_PC24    MGC_DrcChangeOtgState
    1920:   e24bd00c    sub sp, fp, #12 ; 0xc
    1924:   e89da800    ldm sp, {fp, sp, pc}
    1928:   00000000    .word   0x00000000
            1928: R_ARM_ABS32   MGC_bDiagLevel
    192c:   00000060    .word   0x00000060
            192c: R_ARM_ABS32   .rodata

00001930 <MGC_WaitSessReqTimeout>:
    1930:   e1a0c00d    mov ip, sp
    1934:   e92dd800    push    {fp, ip, lr, pc}
    1938:   e24cb004    sub fp, ip, #4  ; 0x4
    193c:   e24dd014    sub sp, sp, #20 ; 0x14
    1940:   e50b001c    str r0, [fp, #-28]
    1944:   e1a03001    mov r3, r1
    1948:   e14b32b0    strh    r3, [fp, #-32]
    194c:   e51b301c    ldr r3, [fp, #-28]
    1950:   e50b3018    str r3, [fp, #-24]
    1954:   e51b3018    ldr r3, [fp, #-24]
    1958:   e5933014    ldr r3, [r3, #20]
    195c:   e50b3014    str r3, [fp, #-20]
    1960:   e51b3014    ldr r3, [fp, #-20]
    1964:   e5933050    ldr r3, [r3, #80]
    1968:   e50b3010    str r3, [fp, #-16]
    196c:   e51b3014    ldr r3, [fp, #-20]
    1970:   e5d3303b    ldrb    r3, [r3, #59]
    1974:   e3530000    cmp r3, #0  ; 0x0
    1978:   1a000021    bne 1a04 <MGC_WaitSessReqTimeout+0xd4>
    197c:   e51b3014    ldr r3, [fp, #-20]
    1980:   e5d3303a    ldrb    r3, [r3, #58]
    1984:   e3530000    cmp r3, #0  ; 0x0
    1988:   0a000000    beq 1990 <MGC_WaitSessReqTimeout+0x60>
    198c:   ea00001c    b   1a04 <MGC_WaitSessReqTimeout+0xd4>
    1990:   e59f3074    ldr r3, [pc, #116]  ; 1a0c <MGC_WaitSessReqTimeout+0xdc>
    1994:   e5d33000    ldrb    r3, [r3]
    1998:   e3530000    cmp r3, #0  ; 0x0
    199c:   0a00000a    beq 19cc <MGC_WaitSessReqTimeout+0x9c>
    19a0:   e51b3014    ldr r3, [fp, #-20]
    19a4:   e5933000    ldr r3, [r3]
    19a8:   e593300c    ldr r3, [r3, #12]
    19ac:   e5932028    ldr r2, [r3, #40]
    19b0:   e51b3014    ldr r3, [fp, #-20]
    19b4:   e5933000    ldr r3, [r3]
    19b8:   e593300c    ldr r3, [r3, #12]
    19bc:   e5933004    ldr r3, [r3, #4]
    19c0:   e1a00003    mov r0, r3
    19c4:   e59f1044    ldr r1, [pc, #68]   ; 1a10 <MGC_WaitSessReqTimeout+0xe0>
    19c8:   e12fff32    blx r2
    19cc:   e51b3010    ldr r3, [fp, #-16]
    19d0:   e3530000    cmp r3, #0  ; 0x0
    19d4:   0a000007    beq 19f8 <MGC_WaitSessReqTimeout+0xc8>
    19d8:   e51b3010    ldr r3, [fp, #-16]
    19dc:   e593c00c    ldr ip, [r3, #12]
    19e0:   e51b3010    ldr r3, [fp, #-16]
    19e4:   e5933000    ldr r3, [r3]
    19e8:   e1a00003    mov r0, r3
    19ec:   e51b1010    ldr r1, [fp, #-16]
    19f0:   e3a020b1    mov r2, #177    ; 0xb1
    19f4:   e12fff3c    blx ip
    19f8:   e3a00000    mov r0, #0  ; 0x0
    19fc:   e51b1014    ldr r1, [fp, #-20]
    1a00:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            1a00: R_ARM_PC24    MGC_DrcChangeOtgState
    1a04:   e24bd00c    sub sp, fp, #12 ; 0xc
    1a08:   e89da800    ldm sp, {fp, sp, pc}
    1a0c:   00000000    .word   0x00000000
            1a0c: R_ARM_ABS32   MGC_bDiagLevel
    1a10:   00000084    .word   0x00000084
            1a10: R_ARM_ABS32   .rodata

00001a14 <MGC_HostResetStart>:
    1a14:   e1a0c00d    mov ip, sp
    1a18:   e92dd800    push    {fp, ip, lr, pc}
    1a1c:   e24cb004    sub fp, ip, #4  ; 0x4
    1a20:   e24dd014    sub sp, sp, #20 ; 0x14
    1a24:   e50b0018    str r0, [fp, #-24]
    1a28:   e1a03001    mov r3, r1
    1a2c:   e14b31bc    strh    r3, [fp, #-28]
    1a30:   e51b3018    ldr r3, [fp, #-24]
    1a34:   e50b3014    str r3, [fp, #-20]
    1a38:   e51b3014    ldr r3, [fp, #-20]
    1a3c:   e5933014    ldr r3, [r3, #20]
    1a40:   e50b3010    str r3, [fp, #-16]
    1a44:   e59f3090    ldr r3, [pc, #144]  ; 1adc <MGC_HostResetStart+0xc8>
    1a48:   e5d33000    ldrb    r3, [r3]
    1a4c:   e3530001    cmp r3, #1  ; 0x1
    1a50:   9a000008    bls 1a78 <MGC_HostResetStart+0x64>
    1a54:   e51b3014    ldr r3, [fp, #-20]
    1a58:   e593300c    ldr r3, [r3, #12]
    1a5c:   e5932028    ldr r2, [r3, #40]
    1a60:   e51b3014    ldr r3, [fp, #-20]
    1a64:   e593300c    ldr r3, [r3, #12]
    1a68:   e5933004    ldr r3, [r3, #4]
    1a6c:   e1a00003    mov r0, r3
    1a70:   e59f1068    ldr r1, [pc, #104]  ; 1ae0 <MGC_HostResetStart+0xcc>
    1a74:   e12fff32    blx r2
    1a78:   e51b2010    ldr r2, [fp, #-16]
    1a7c:   e3a03001    mov r3, #1  ; 0x1
    1a80:   e5c23025    strb    r3, [r2, #37]
    1a84:   e51b2010    ldr r2, [fp, #-16]
    1a88:   e3a03000    mov r3, #0  ; 0x0
    1a8c:   e5c23026    strb    r3, [r2, #38]
    1a90:   e51b3010    ldr r3, [fp, #-16]
    1a94:   e593321c    ldr r3, [r3, #540]
    1a98:   e51b0010    ldr r0, [fp, #-16]
    1a9c:   e12fff33    blx r3
    1aa0:   e51b3014    ldr r3, [fp, #-20]
    1aa4:   e593300c    ldr r3, [r3, #12]
    1aa8:   e593c018    ldr ip, [r3, #24]
    1aac:   e51b3014    ldr r3, [fp, #-20]
    1ab0:   e593300c    ldr r3, [r3, #12]
    1ab4:   e5932004    ldr r2, [r3, #4]
    1ab8:   e59f3024    ldr r3, [pc, #36]   ; 1ae4 <MGC_HostResetStart+0xd0>
    1abc:   e58d3000    str r3, [sp]
    1ac0:   e1a00002    mov r0, r2
    1ac4:   e3a01000    mov r1, #0  ; 0x0
    1ac8:   e3a0203c    mov r2, #60 ; 0x3c
    1acc:   e3a03000    mov r3, #0  ; 0x0
    1ad0:   e12fff3c    blx ip
    1ad4:   e24bd00c    sub sp, fp, #12 ; 0xc
    1ad8:   e89da800    ldm sp, {fp, sp, pc}
    1adc:   00000000    .word   0x00000000
            1adc: R_ARM_ABS32   MGC_bDiagLevel
    1ae0:   0000009c    .word   0x0000009c
            1ae0: R_ARM_ABS32   .rodata
    1ae4:   00000000    .word   0x00000000
            1ae4: R_ARM_ABS32   MGC_HostResetComplete

00001ae8 <MGC_HostResetComplete>:
    1ae8:   e1a0c00d    mov ip, sp
    1aec:   e92dd800    push    {fp, ip, lr, pc}
    1af0:   e24cb004    sub fp, ip, #4  ; 0x4
    1af4:   e24dd01c    sub sp, sp, #28 ; 0x1c
    1af8:   e50b001c    str r0, [fp, #-28]
    1afc:   e1a03001    mov r3, r1
    1b00:   e14b32b0    strh    r3, [fp, #-32]
    1b04:   e3a03002    mov r3, #2  ; 0x2
    1b08:   e54b3015    strb    r3, [fp, #-21]
    1b0c:   e51b301c    ldr r3, [fp, #-28]
    1b10:   e50b3014    str r3, [fp, #-20]
    1b14:   e51b3014    ldr r3, [fp, #-20]
    1b18:   e5933014    ldr r3, [r3, #20]
    1b1c:   e50b3010    str r3, [fp, #-16]
    1b20:   e59f3154    ldr r3, [pc, #340]  ; 1c7c <MGC_HostResetComplete+0x194>
    1b24:   e5d33000    ldrb    r3, [r3]
    1b28:   e3530001    cmp r3, #1  ; 0x1
    1b2c:   9a000008    bls 1b54 <MGC_HostResetComplete+0x6c>
    1b30:   e51b3014    ldr r3, [fp, #-20]
    1b34:   e593300c    ldr r3, [r3, #12]
    1b38:   e5932028    ldr r2, [r3, #40]
    1b3c:   e51b3014    ldr r3, [fp, #-20]
    1b40:   e593300c    ldr r3, [r3, #12]
    1b44:   e5933004    ldr r3, [r3, #4]
    1b48:   e1a00003    mov r0, r3
    1b4c:   e59f112c    ldr r1, [pc, #300]  ; 1c80 <MGC_HostResetComplete+0x198>
    1b50:   e12fff32    blx r2
    1b54:   e51b2010    ldr r2, [fp, #-16]
    1b58:   e3a03000    mov r3, #0  ; 0x0
    1b5c:   e5c23025    strb    r3, [r2, #37]
    1b60:   e51b3010    ldr r3, [fp, #-16]
    1b64:   e593321c    ldr r3, [r3, #540]
    1b68:   e51b0010    ldr r0, [fp, #-16]
    1b6c:   e12fff33    blx r3
    1b70:   e51b3010    ldr r3, [fp, #-16]
    1b74:   e5933218    ldr r3, [r3, #536]
    1b78:   e51b0010    ldr r0, [fp, #-16]
    1b7c:   e12fff33    blx r3
    1b80:   e51b3010    ldr r3, [fp, #-16]
    1b84:   e5d33038    ldrb    r3, [r3, #56]
    1b88:   e3530000    cmp r3, #0  ; 0x0
    1b8c:   0a000002    beq 1b9c <MGC_HostResetComplete+0xb4>
    1b90:   e3a03003    mov r3, #3  ; 0x3
    1b94:   e54b3015    strb    r3, [fp, #-21]
    1b98:   ea000005    b   1bb4 <MGC_HostResetComplete+0xcc>
    1b9c:   e51b3010    ldr r3, [fp, #-16]
    1ba0:   e5d33039    ldrb    r3, [r3, #57]
    1ba4:   e3530000    cmp r3, #0  ; 0x0
    1ba8:   0a000001    beq 1bb4 <MGC_HostResetComplete+0xcc>
    1bac:   e3a03001    mov r3, #1  ; 0x1
    1bb0:   e54b3015    strb    r3, [fp, #-21]
    1bb4:   e51b3010    ldr r3, [fp, #-16]
    1bb8:   e2833054    add r3, r3, #84 ; 0x54
    1bbc:   e1a00003    mov r0, r3
    1bc0:   ebfffffe    bl  0 <MGC_AllocateAddress>
            1bc0: R_ARM_PC24    MGC_AllocateAddress
    1bc4:   e1a03000    mov r3, r0
    1bc8:   e54b3016    strb    r3, [fp, #-22]
    1bcc:   e55b3016    ldrb    r3, [fp, #-22]
    1bd0:   e3530000    cmp r3, #0  ; 0x0
    1bd4:   0a000019    beq 1c40 <MGC_HostResetComplete+0x158>
    1bd8:   e55b2016    ldrb    r2, [fp, #-22]
    1bdc:   e55b3015    ldrb    r3, [fp, #-21]
    1be0:   e58d3000    str r3, [sp]
    1be4:   e3a03000    mov r3, #0  ; 0x0
    1be8:   e58d3004    str r3, [sp, #4]
    1bec:   e51b0010    ldr r0, [fp, #-16]
    1bf0:   e3a01000    mov r1, #0  ; 0x0
    1bf4:   e3a03000    mov r3, #0  ; 0x0
    1bf8:   ebfffffe    bl  0 <MGC_EnumerateDevice>
            1bf8: R_ARM_PC24    MGC_EnumerateDevice
    1bfc:   e1a03000    mov r3, r0
    1c00:   e3530000    cmp r3, #0  ; 0x0
    1c04:   1a00001a    bne 1c74 <MGC_HostResetComplete+0x18c>
    1c08:   e59f306c    ldr r3, [pc, #108]  ; 1c7c <MGC_HostResetComplete+0x194>
    1c0c:   e5d33000    ldrb    r3, [r3]
    1c10:   e3530000    cmp r3, #0  ; 0x0
    1c14:   0a000016    beq 1c74 <MGC_HostResetComplete+0x18c>
    1c18:   e51b3014    ldr r3, [fp, #-20]
    1c1c:   e593300c    ldr r3, [r3, #12]
    1c20:   e5932028    ldr r2, [r3, #40]
    1c24:   e51b3014    ldr r3, [fp, #-20]
    1c28:   e593300c    ldr r3, [r3, #12]
    1c2c:   e5933004    ldr r3, [r3, #4]
    1c30:   e1a00003    mov r0, r3
    1c34:   e59f1048    ldr r1, [pc, #72]   ; 1c84 <MGC_HostResetComplete+0x19c>
    1c38:   e12fff32    blx r2
    1c3c:   ea00000c    b   1c74 <MGC_HostResetComplete+0x18c>
    1c40:   e59f3034    ldr r3, [pc, #52]   ; 1c7c <MGC_HostResetComplete+0x194>
    1c44:   e5d33000    ldrb    r3, [r3]
    1c48:   e3530000    cmp r3, #0  ; 0x0
    1c4c:   0a000008    beq 1c74 <MGC_HostResetComplete+0x18c>
    1c50:   e51b3014    ldr r3, [fp, #-20]
    1c54:   e593300c    ldr r3, [r3, #12]
    1c58:   e5932028    ldr r2, [r3, #40]
    1c5c:   e51b3014    ldr r3, [fp, #-20]
    1c60:   e593300c    ldr r3, [r3, #12]
    1c64:   e5933004    ldr r3, [r3, #4]
    1c68:   e1a00003    mov r0, r3
    1c6c:   e59f1014    ldr r1, [pc, #20]   ; 1c88 <MGC_HostResetComplete+0x1a0>
    1c70:   e12fff32    blx r2
    1c74:   e24bd00c    sub sp, fp, #12 ; 0xc
    1c78:   e89da800    ldm sp, {fp, sp, pc}
    1c7c:   00000000    .word   0x00000000
            1c7c: R_ARM_ABS32   MGC_bDiagLevel
    1c80:   000000ac    .word   0x000000ac
            1c80: R_ARM_ABS32   .rodata
    1c84:   000000bc    .word   0x000000bc
            1c84: R_ARM_ABS32   .rodata
    1c88:   000000e0    .word   0x000000e0
            1c88: R_ARM_ABS32   .rodata

00001c8c <MGC_BSrpFailed>:
    1c8c:   e1a0c00d    mov ip, sp
    1c90:   e92dd800    push    {fp, ip, lr, pc}
    1c94:   e24cb004    sub fp, ip, #4  ; 0x4
    1c98:   e24dd014    sub sp, sp, #20 ; 0x14
    1c9c:   e50b001c    str r0, [fp, #-28]
    1ca0:   e1a03001    mov r3, r1
    1ca4:   e14b32b0    strh    r3, [fp, #-32]
    1ca8:   e51b301c    ldr r3, [fp, #-28]
    1cac:   e50b3018    str r3, [fp, #-24]
    1cb0:   e51b3018    ldr r3, [fp, #-24]
    1cb4:   e5933014    ldr r3, [r3, #20]
    1cb8:   e50b3014    str r3, [fp, #-20]
    1cbc:   e51b3014    ldr r3, [fp, #-20]
    1cc0:   e5933050    ldr r3, [r3, #80]
    1cc4:   e50b3010    str r3, [fp, #-16]
    1cc8:   e51b3014    ldr r3, [fp, #-20]
    1ccc:   e5d3304c    ldrb    r3, [r3, #76]
    1cd0:   e3530014    cmp r3, #20 ; 0x14
    1cd4:   0a000003    beq 1ce8 <MGC_BSrpFailed+0x5c>
    1cd8:   e51b3014    ldr r3, [fp, #-20]
    1cdc:   e5d3304c    ldrb    r3, [r3, #76]
    1ce0:   e3530000    cmp r3, #0  ; 0x0
    1ce4:   1a000045    bne 1e00 <MGC_BSrpFailed+0x174>
    1ce8:   e51b3014    ldr r3, [fp, #-20]
    1cec:   e5d33040    ldrb    r3, [r3, #64]
    1cf0:   e3530001    cmp r3, #1  ; 0x1
    1cf4:   9a00001a    bls 1d64 <MGC_BSrpFailed+0xd8>
    1cf8:   e59f3108    ldr r3, [pc, #264]  ; 1e08 <MGC_BSrpFailed+0x17c>
    1cfc:   e5d33000    ldrb    r3, [r3]
    1d00:   e3530000    cmp r3, #0  ; 0x0
    1d04:   0a00000a    beq 1d34 <MGC_BSrpFailed+0xa8>
    1d08:   e51b3014    ldr r3, [fp, #-20]
    1d0c:   e5933000    ldr r3, [r3]
    1d10:   e593300c    ldr r3, [r3, #12]
    1d14:   e5932028    ldr r2, [r3, #40]
    1d18:   e51b3014    ldr r3, [fp, #-20]
    1d1c:   e5933000    ldr r3, [r3]
    1d20:   e593300c    ldr r3, [r3, #12]
    1d24:   e5933004    ldr r3, [r3, #4]
    1d28:   e1a00003    mov r0, r3
    1d2c:   e59f10d8    ldr r1, [pc, #216]  ; 1e0c <MGC_BSrpFailed+0x180>
    1d30:   e12fff32    blx r2
    1d34:   e51b3010    ldr r3, [fp, #-16]
    1d38:   e3530000    cmp r3, #0  ; 0x0
    1d3c:   0a00002f    beq 1e00 <MGC_BSrpFailed+0x174>
    1d40:   e51b3010    ldr r3, [fp, #-16]
    1d44:   e593c00c    ldr ip, [r3, #12]
    1d48:   e51b3010    ldr r3, [fp, #-16]
    1d4c:   e5933000    ldr r3, [r3]
    1d50:   e1a00003    mov r0, r3
    1d54:   e51b1010    ldr r1, [fp, #-16]
    1d58:   e3a020b2    mov r2, #178    ; 0xb2
    1d5c:   e12fff3c    blx ip
    1d60:   ea000026    b   1e00 <MGC_BSrpFailed+0x174>
    1d64:   e51b2014    ldr r2, [fp, #-20]
    1d68:   e3a03000    mov r3, #0  ; 0x0
    1d6c:   e5c23029    strb    r3, [r2, #41]
    1d70:   e51b2014    ldr r2, [fp, #-20]
    1d74:   e3a03000    mov r3, #0  ; 0x0
    1d78:   e5c2302a    strb    r3, [r2, #42]
    1d7c:   e51b3014    ldr r3, [fp, #-20]
    1d80:   e593321c    ldr r3, [r3, #540]
    1d84:   e51b0014    ldr r0, [fp, #-20]
    1d88:   e12fff33    blx r3
    1d8c:   e59f3074    ldr r3, [pc, #116]  ; 1e08 <MGC_BSrpFailed+0x17c>
    1d90:   e5d33000    ldrb    r3, [r3]
    1d94:   e3530000    cmp r3, #0  ; 0x0
    1d98:   0a00000a    beq 1dc8 <MGC_BSrpFailed+0x13c>
    1d9c:   e51b3014    ldr r3, [fp, #-20]
    1da0:   e5933000    ldr r3, [r3]
    1da4:   e593300c    ldr r3, [r3, #12]
    1da8:   e5932028    ldr r2, [r3, #40]
    1dac:   e51b3014    ldr r3, [fp, #-20]
    1db0:   e5933000    ldr r3, [r3]
    1db4:   e593300c    ldr r3, [r3, #12]
    1db8:   e5933004    ldr r3, [r3, #4]
    1dbc:   e1a00003    mov r0, r3
    1dc0:   e59f1048    ldr r1, [pc, #72]   ; 1e10 <MGC_BSrpFailed+0x184>
    1dc4:   e12fff32    blx r2
    1dc8:   e51b3010    ldr r3, [fp, #-16]
    1dcc:   e3530000    cmp r3, #0  ; 0x0
    1dd0:   0a000007    beq 1df4 <MGC_BSrpFailed+0x168>
    1dd4:   e51b3010    ldr r3, [fp, #-16]
    1dd8:   e593c00c    ldr ip, [r3, #12]
    1ddc:   e51b3010    ldr r3, [fp, #-16]
    1de0:   e5933000    ldr r3, [r3]
    1de4:   e1a00003    mov r0, r3
    1de8:   e51b1010    ldr r1, [fp, #-16]
    1dec:   e3a020b1    mov r2, #177    ; 0xb1
    1df0:   e12fff3c    blx ip
    1df4:   e3a00000    mov r0, #0  ; 0x0
    1df8:   e51b1014    ldr r1, [fp, #-20]
    1dfc:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            1dfc: R_ARM_PC24    MGC_DrcChangeOtgState
    1e00:   e24bd00c    sub sp, fp, #12 ; 0xc
    1e04:   e89da800    ldm sp, {fp, sp, pc}
    1e08:   00000000    .word   0x00000000
            1e08: R_ARM_ABS32   MGC_bDiagLevel
    1e0c:   00000108    .word   0x00000108
            1e0c: R_ARM_ABS32   .rodata
    1e10:   00000130    .word   0x00000130
            1e10: R_ARM_ABS32   .rodata

00001e14 <MGC_BAase0BrstTimeout>:
    1e14:   e1a0c00d    mov ip, sp
    1e18:   e92dd800    push    {fp, ip, lr, pc}
    1e1c:   e24cb004    sub fp, ip, #4  ; 0x4
    1e20:   e24dd014    sub sp, sp, #20 ; 0x14
    1e24:   e50b001c    str r0, [fp, #-28]
    1e28:   e1a03001    mov r3, r1
    1e2c:   e14b32b0    strh    r3, [fp, #-32]
    1e30:   e51b301c    ldr r3, [fp, #-28]
    1e34:   e50b3018    str r3, [fp, #-24]
    1e38:   e51b3018    ldr r3, [fp, #-24]
    1e3c:   e5933014    ldr r3, [r3, #20]
    1e40:   e50b3014    str r3, [fp, #-20]
    1e44:   e51b3014    ldr r3, [fp, #-20]
    1e48:   e5933050    ldr r3, [r3, #80]
    1e4c:   e50b3010    str r3, [fp, #-16]
    1e50:   e51b3014    ldr r3, [fp, #-20]
    1e54:   e5d3304c    ldrb    r3, [r3, #76]
    1e58:   e3530012    cmp r3, #18 ; 0x12
    1e5c:   1a000023    bne 1ef0 <MGC_BAase0BrstTimeout+0xdc>
    1e60:   e59f3090    ldr r3, [pc, #144]  ; 1ef8 <MGC_BAase0BrstTimeout+0xe4>
    1e64:   e5d33000    ldrb    r3, [r3]
    1e68:   e3530000    cmp r3, #0  ; 0x0
    1e6c:   0a00000a    beq 1e9c <MGC_BAase0BrstTimeout+0x88>
    1e70:   e51b3014    ldr r3, [fp, #-20]
    1e74:   e5933000    ldr r3, [r3]
    1e78:   e593300c    ldr r3, [r3, #12]
    1e7c:   e5932028    ldr r2, [r3, #40]
    1e80:   e51b3014    ldr r3, [fp, #-20]
    1e84:   e5933000    ldr r3, [r3]
    1e88:   e593300c    ldr r3, [r3, #12]
    1e8c:   e5933004    ldr r3, [r3, #4]
    1e90:   e1a00003    mov r0, r3
    1e94:   e59f1060    ldr r1, [pc, #96]   ; 1efc <MGC_BAase0BrstTimeout+0xe8>
    1e98:   e12fff32    blx r2
    1e9c:   e51b3010    ldr r3, [fp, #-16]
    1ea0:   e3530000    cmp r3, #0  ; 0x0
    1ea4:   0a000007    beq 1ec8 <MGC_BAase0BrstTimeout+0xb4>
    1ea8:   e51b3010    ldr r3, [fp, #-16]
    1eac:   e593c00c    ldr ip, [r3, #12]
    1eb0:   e51b3010    ldr r3, [fp, #-16]
    1eb4:   e5933000    ldr r3, [r3]
    1eb8:   e1a00003    mov r0, r3
    1ebc:   e51b1010    ldr r1, [fp, #-16]
    1ec0:   e3a020b1    mov r2, #177    ; 0xb1
    1ec4:   e12fff3c    blx ip
    1ec8:   e51b2014    ldr r2, [fp, #-20]
    1ecc:   e3a03000    mov r3, #0  ; 0x0
    1ed0:   e5c2302a    strb    r3, [r2, #42]
    1ed4:   e51b3014    ldr r3, [fp, #-20]
    1ed8:   e593321c    ldr r3, [r3, #540]
    1edc:   e51b0014    ldr r0, [fp, #-20]
    1ee0:   e12fff33    blx r3
    1ee4:   e3a00011    mov r0, #17 ; 0x11
    1ee8:   e51b1014    ldr r1, [fp, #-20]
    1eec:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            1eec: R_ARM_PC24    MGC_DrcChangeOtgState
    1ef0:   e24bd00c    sub sp, fp, #12 ; 0xc
    1ef4:   e89da800    ldm sp, {fp, sp, pc}
    1ef8:   00000000    .word   0x00000000
            1ef8: R_ARM_ABS32   MGC_bDiagLevel
    1efc:   00000030    .word   0x00000030
            1efc: R_ARM_ABS32   .rodata

00001f00 <MGC_SetHnpSupportIrpComplete>:
    1f00:   e1a0c00d    mov ip, sp
    1f04:   e92dd800    push    {fp, ip, lr, pc}
    1f08:   e24cb004    sub fp, ip, #4  ; 0x4
    1f0c:   e24dd008    sub sp, sp, #8  ; 0x8
    1f10:   e50b0010    str r0, [fp, #-16]
    1f14:   e50b1014    str r1, [fp, #-20]
    1f18:   e24bd00c    sub sp, fp, #12 ; 0xc
    1f1c:   e89da800    ldm sp, {fp, sp, pc}

00001f20 <MGC_SetHnpEnableIrpComplete>:
    1f20:   e1a0c00d    mov ip, sp
    1f24:   e92dd800    push    {fp, ip, lr, pc}
    1f28:   e24cb004    sub fp, ip, #4  ; 0x4
    1f2c:   e24dd014    sub sp, sp, #20 ; 0x14
    1f30:   e50b0018    str r0, [fp, #-24]
    1f34:   e50b101c    str r1, [fp, #-28]
    1f38:   e51b3018    ldr r3, [fp, #-24]
    1f3c:   e50b3014    str r3, [fp, #-20]
    1f40:   e51b3014    ldr r3, [fp, #-20]
    1f44:   e5933000    ldr r3, [r3]
    1f48:   e593300c    ldr r3, [r3, #12]
    1f4c:   e50b3010    str r3, [fp, #-16]
    1f50:   e51b301c    ldr r3, [fp, #-28]
    1f54:   e5933014    ldr r3, [r3, #20]
    1f58:   e3530000    cmp r3, #0  ; 0x0
    1f5c:   1a000012    bne 1fac <MGC_SetHnpEnableIrpComplete+0x8c>
    1f60:   e51b2014    ldr r2, [fp, #-20]
    1f64:   e3a03001    mov r3, #1  ; 0x1
    1f68:   e5c23026    strb    r3, [r2, #38]
    1f6c:   e51b3014    ldr r3, [fp, #-20]
    1f70:   e593321c    ldr r3, [r3, #540]
    1f74:   e51b0014    ldr r0, [fp, #-20]
    1f78:   e12fff33    blx r3
    1f7c:   e51b3010    ldr r3, [fp, #-16]
    1f80:   e593c018    ldr ip, [r3, #24]
    1f84:   e51b3010    ldr r3, [fp, #-16]
    1f88:   e5932004    ldr r2, [r3, #4]
    1f8c:   e59f302c    ldr r3, [pc, #44]   ; 1fc0 <MGC_SetHnpEnableIrpComplete+0xa0>
    1f90:   e58d3000    str r3, [sp]
    1f94:   e1a00002    mov r0, r2
    1f98:   e3a01000    mov r1, #0  ; 0x0
    1f9c:   e3a020fa    mov r2, #250    ; 0xfa
    1fa0:   e3a03000    mov r3, #0  ; 0x0
    1fa4:   e12fff3c    blx ip
    1fa8:   ea000002    b   1fb8 <MGC_SetHnpEnableIrpComplete+0x98>
    1fac:   e51b0014    ldr r0, [fp, #-20]
    1fb0:   e3a01001    mov r1, #1  ; 0x1
    1fb4:   ebfffe15    bl  1810 <MGC_SetPowerSave>
    1fb8:   e24bd00c    sub sp, fp, #12 ; 0xc
    1fbc:   e89da800    ldm sp, {fp, sp, pc}
    1fc0:   00000000    .word   0x00000000
            1fc0: R_ARM_ABS32   MGC_AidlBdisTimeout

00001fc4 <MGC_RemoveSuspend>:
    1fc4:   e1a0c00d    mov ip, sp
    1fc8:   e92dd800    push    {fp, ip, lr, pc}
    1fcc:   e24cb004    sub fp, ip, #4  ; 0x4
    1fd0:   e24dd010    sub sp, sp, #16 ; 0x10
    1fd4:   e50b0018    str r0, [fp, #-24]
    1fd8:   e1a03001    mov r3, r1
    1fdc:   e14b31bc    strh    r3, [fp, #-28]
    1fe0:   e51b3018    ldr r3, [fp, #-24]
    1fe4:   e50b3014    str r3, [fp, #-20]
    1fe8:   e51b3014    ldr r3, [fp, #-20]
    1fec:   e5933014    ldr r3, [r3, #20]
    1ff0:   e50b3010    str r3, [fp, #-16]
    1ff4:   e51b2010    ldr r2, [fp, #-16]
    1ff8:   e3a03000    mov r3, #0  ; 0x0
    1ffc:   e5c23026    strb    r3, [r2, #38]
    2000:   e51b3010    ldr r3, [fp, #-16]
    2004:   e593321c    ldr r3, [r3, #540]
    2008:   e51b0010    ldr r0, [fp, #-16]
    200c:   e12fff33    blx r3
    2010:   e24bd00c    sub sp, fp, #12 ; 0xc
    2014:   e89da800    ldm sp, {fp, sp, pc}

00002018 <MGC_DrcChangeOtgState>:
    2018:   e1a0c00d    mov ip, sp
    201c:   e92dd800    push    {fp, ip, lr, pc}
    2020:   e24cb004    sub fp, ip, #4  ; 0x4
    2024:   e24dd014    sub sp, sp, #20 ; 0x14
    2028:   e1a03000    mov r3, r0
    202c:   e50b101c    str r1, [fp, #-28]
    2030:   e54b3018    strb    r3, [fp, #-24]
    2034:   e51b301c    ldr r3, [fp, #-28]
    2038:   e5933000    ldr r3, [r3]
    203c:   e593300c    ldr r3, [r3, #12]
    2040:   e50b3010    str r3, [fp, #-16]
    2044:   e51b301c    ldr r3, [fp, #-28]
    2048:   e5933000    ldr r3, [r3]
    204c:   e593202c    ldr r2, [r3, #44]
    2050:   e51b301c    ldr r3, [fp, #-28]
    2054:   e5933000    ldr r3, [r3]
    2058:   e1a00003    mov r0, r3
    205c:   e12fff32    blx r2
    2060:   e51b301c    ldr r3, [fp, #-28]
    2064:   e5933218    ldr r3, [r3, #536]
    2068:   e51b001c    ldr r0, [fp, #-28]
    206c:   e12fff33    blx r3
    2070:   e51b3010    ldr r3, [fp, #-16]
    2074:   e593201c    ldr r2, [r3, #28]
    2078:   e51b3010    ldr r3, [fp, #-16]
    207c:   e5933004    ldr r3, [r3, #4]
    2080:   e1a00003    mov r0, r3
    2084:   e3a01000    mov r1, #0  ; 0x0
    2088:   e12fff32    blx r2
    208c:   e55b3018    ldrb    r3, [fp, #-24]
    2090:   e1a00003    mov r0, r3
    2094:   e51b101c    ldr r1, [fp, #-28]
    2098:   ebfffda3    bl  172c <MGC_ExitingHost>
    209c:   e1a03000    mov r3, r0
    20a0:   e3530000    cmp r3, #0  ; 0x0
    20a4:   1a000004    bne 20bc <MGC_DrcChangeOtgState+0xa4>
    20a8:   e51b301c    ldr r3, [fp, #-28]
    20ac:   e5d3304c    ldrb    r3, [r3, #76]
    20b0:   e203300f    and r3, r3, #15 ; 0xf
    20b4:   e3530001    cmp r3, #1  ; 0x1
    20b8:   1a000003    bne 20cc <MGC_DrcChangeOtgState+0xb4>
    20bc:   e55b3018    ldrb    r3, [fp, #-24]
    20c0:   e1a00003    mov r0, r3
    20c4:   e51b101c    ldr r1, [fp, #-28]
    20c8:   ebfffdbb    bl  17bc <MGC_ExitActiveUsbMode>
    20cc:   e55b3018    ldrb    r3, [fp, #-24]
    20d0:   e3530024    cmp r3, #36 ; 0x24
    20d4:   979ff103    ldrls   pc, [pc, r3, lsl #2]
    20d8:   ea0000c3    b   23ec <MGC_DrcChangeOtgState+0x3d4>
    20dc:   00002170    .word   0x00002170
            20dc: R_ARM_ABS32   .text
    20e0:   000023ec    .word   0x000023ec
            20e0: R_ARM_ABS32   .text
    20e4:   000023ec    .word   0x000023ec
            20e4: R_ARM_ABS32   .text
    20e8:   000023ec    .word   0x000023ec
            20e8: R_ARM_ABS32   .text
    20ec:   000023ec    .word   0x000023ec
            20ec: R_ARM_ABS32   .text
    20f0:   000023ec    .word   0x000023ec
            20f0: R_ARM_ABS32   .text
    20f4:   000023ec    .word   0x000023ec
            20f4: R_ARM_ABS32   .text
    20f8:   000023ec    .word   0x000023ec
            20f8: R_ARM_ABS32   .text
    20fc:   000023ec    .word   0x000023ec
            20fc: R_ARM_ABS32   .text
    2100:   000023ec    .word   0x000023ec
            2100: R_ARM_ABS32   .text
    2104:   000023ec    .word   0x000023ec
            2104: R_ARM_ABS32   .text
    2108:   000023ec    .word   0x000023ec
            2108: R_ARM_ABS32   .text
    210c:   000023ec    .word   0x000023ec
            210c: R_ARM_ABS32   .text
    2110:   000023ec    .word   0x000023ec
            2110: R_ARM_ABS32   .text
    2114:   000023ec    .word   0x000023ec
            2114: R_ARM_ABS32   .text
    2118:   000023ec    .word   0x000023ec
            2118: R_ARM_ABS32   .text
    211c:   000023ec    .word   0x000023ec
            211c: R_ARM_ABS32   .text
    2120:   00002484    .word   0x00002484
            2120: R_ARM_ABS32   .text
    2124:   00002254    .word   0x00002254
            2124: R_ARM_ABS32   .text
    2128:   000022c4    .word   0x000022c4
            2128: R_ARM_ABS32   .text
    212c:   0000219c    .word   0x0000219c
            212c: R_ARM_ABS32   .text
    2130:   000023ec    .word   0x000023ec
            2130: R_ARM_ABS32   .text
    2134:   000023ec    .word   0x000023ec
            2134: R_ARM_ABS32   .text
    2138:   000023ec    .word   0x000023ec
            2138: R_ARM_ABS32   .text
    213c:   000023ec    .word   0x000023ec
            213c: R_ARM_ABS32   .text
    2140:   000023ec    .word   0x000023ec
            2140: R_ARM_ABS32   .text
    2144:   000023ec    .word   0x000023ec
            2144: R_ARM_ABS32   .text
    2148:   000023ec    .word   0x000023ec
            2148: R_ARM_ABS32   .text
    214c:   000023ec    .word   0x000023ec
            214c: R_ARM_ABS32   .text
    2150:   000023ec    .word   0x000023ec
            2150: R_ARM_ABS32   .text
    2154:   000023ec    .word   0x000023ec
            2154: R_ARM_ABS32   .text
    2158:   000023ec    .word   0x000023ec
            2158: R_ARM_ABS32   .text
    215c:   000023ec    .word   0x000023ec
            215c: R_ARM_ABS32   .text
    2160:   00002484    .word   0x00002484
            2160: R_ARM_ABS32   .text
    2164:   000021f8    .word   0x000021f8
            2164: R_ARM_ABS32   .text
    2168:   00002284    .word   0x00002284
            2168: R_ARM_ABS32   .text
    216c:   00002310    .word   0x00002310
            216c: R_ARM_ABS32   .text
    2170:   e51b201c    ldr r2, [fp, #-28]
    2174:   e3a03000    mov r3, #0  ; 0x0
    2178:   e5c2302a    strb    r3, [r2, #42]
    217c:   e51b201c    ldr r2, [fp, #-28]
    2180:   e3a03000    mov r3, #0  ; 0x0
    2184:   e5c23029    strb    r3, [r2, #41]
    2188:   e51b301c    ldr r3, [fp, #-28]
    218c:   e593321c    ldr r3, [r3, #540]
    2190:   e51b001c    ldr r0, [fp, #-28]
    2194:   e12fff33    blx r3
    2198:   ea0000b9    b   2484 <MGC_DrcChangeOtgState+0x46c>
    219c:   e51b201c    ldr r2, [fp, #-28]
    21a0:   e3a03001    mov r3, #1  ; 0x1
    21a4:   e5c23029    strb    r3, [r2, #41]
    21a8:   e51b301c    ldr r3, [fp, #-28]
    21ac:   e593321c    ldr r3, [r3, #540]
    21b0:   e51b001c    ldr r0, [fp, #-28]
    21b4:   e12fff33    blx r3
    21b8:   e59f33c8    ldr r3, [pc, #968]  ; 2588 <MGC_DrcChangeOtgState+0x570>
    21bc:   e5933000    ldr r3, [r3]
    21c0:   e3530000    cmp r3, #0  ; 0x0
    21c4:   1a0000ae    bne 2484 <MGC_DrcChangeOtgState+0x46c>
    21c8:   e51b3010    ldr r3, [fp, #-16]
    21cc:   e593c018    ldr ip, [r3, #24]
    21d0:   e51b3010    ldr r3, [fp, #-16]
    21d4:   e5932004    ldr r2, [r3, #4]
    21d8:   e59f33ac    ldr r3, [pc, #940]  ; 258c <MGC_DrcChangeOtgState+0x574>
    21dc:   e58d3000    str r3, [sp]
    21e0:   e1a00002    mov r0, r2
    21e4:   e3a01000    mov r1, #0  ; 0x0
    21e8:   e59f23a0    ldr r2, [pc, #928]  ; 2590 <MGC_DrcChangeOtgState+0x578>
    21ec:   e3a03000    mov r3, #0  ; 0x0
    21f0:   e12fff3c    blx ip
    21f4:   ea0000a2    b   2484 <MGC_DrcChangeOtgState+0x46c>
    21f8:   e51b201c    ldr r2, [fp, #-28]
    21fc:   e3a03001    mov r3, #1  ; 0x1
    2200:   e5c23029    strb    r3, [r2, #41]
    2204:   e51b301c    ldr r3, [fp, #-28]
    2208:   e593321c    ldr r3, [r3, #540]
    220c:   e51b001c    ldr r0, [fp, #-28]
    2210:   e12fff33    blx r3
    2214:   e59f336c    ldr r3, [pc, #876]  ; 2588 <MGC_DrcChangeOtgState+0x570>
    2218:   e5933000    ldr r3, [r3]
    221c:   e3530000    cmp r3, #0  ; 0x0
    2220:   1a000097    bne 2484 <MGC_DrcChangeOtgState+0x46c>
    2224:   e51b3010    ldr r3, [fp, #-16]
    2228:   e593c018    ldr ip, [r3, #24]
    222c:   e51b3010    ldr r3, [fp, #-16]
    2230:   e5932004    ldr r2, [r3, #4]
    2234:   e59f3358    ldr r3, [pc, #856]  ; 2594 <MGC_DrcChangeOtgState+0x57c>
    2238:   e58d3000    str r3, [sp]
    223c:   e1a00002    mov r0, r2
    2240:   e3a01000    mov r1, #0  ; 0x0
    2244:   e3a02d7d    mov r2, #8000   ; 0x1f40
    2248:   e3a03000    mov r3, #0  ; 0x0
    224c:   e12fff3c    blx ip
    2250:   ea00008b    b   2484 <MGC_DrcChangeOtgState+0x46c>
    2254:   e51b3010    ldr r3, [fp, #-16]
    2258:   e593c018    ldr ip, [r3, #24]
    225c:   e51b3010    ldr r3, [fp, #-16]
    2260:   e5932004    ldr r2, [r3, #4]
    2264:   e59f332c    ldr r3, [pc, #812]  ; 2598 <MGC_DrcChangeOtgState+0x580>
    2268:   e58d3000    str r3, [sp]
    226c:   e1a00002    mov r0, r2
    2270:   e3a01000    mov r1, #0  ; 0x0
    2274:   e3a020c8    mov r2, #200    ; 0xc8
    2278:   e3a03000    mov r3, #0  ; 0x0
    227c:   e12fff3c    blx ip
    2280:   ea00007f    b   2484 <MGC_DrcChangeOtgState+0x46c>
    2284:   e51b301c    ldr r3, [fp, #-28]
    2288:   e5d3304c    ldrb    r3, [r3, #76]
    228c:   e3530024    cmp r3, #36 ; 0x24
    2290:   0a00007b    beq 2484 <MGC_DrcChangeOtgState+0x46c>
    2294:   e51b3010    ldr r3, [fp, #-16]
    2298:   e593c018    ldr ip, [r3, #24]
    229c:   e51b3010    ldr r3, [fp, #-16]
    22a0:   e5932004    ldr r2, [r3, #4]
    22a4:   e59f32f0    ldr r3, [pc, #752]  ; 259c <MGC_DrcChangeOtgState+0x584>
    22a8:   e58d3000    str r3, [sp]
    22ac:   e1a00002    mov r0, r2
    22b0:   e3a01000    mov r1, #0  ; 0x0
    22b4:   e3a0200a    mov r2, #10 ; 0xa
    22b8:   e3a03000    mov r3, #0  ; 0x0
    22bc:   e12fff3c    blx ip
    22c0:   ea00006f    b   2484 <MGC_DrcChangeOtgState+0x46c>
    22c4:   e51b201c    ldr r2, [fp, #-28]
    22c8:   e3a03000    mov r3, #0  ; 0x0
    22cc:   e5c2302a    strb    r3, [r2, #42]
    22d0:   e51b301c    ldr r3, [fp, #-28]
    22d4:   e593321c    ldr r3, [r3, #540]
    22d8:   e51b001c    ldr r0, [fp, #-28]
    22dc:   e12fff33    blx r3
    22e0:   e51b3010    ldr r3, [fp, #-16]
    22e4:   e593c018    ldr ip, [r3, #24]
    22e8:   e51b3010    ldr r3, [fp, #-16]
    22ec:   e5932004    ldr r2, [r3, #4]
    22f0:   e59f32a8    ldr r3, [pc, #680]  ; 25a0 <MGC_DrcChangeOtgState+0x588>
    22f4:   e58d3000    str r3, [sp]
    22f8:   e1a00002    mov r0, r2
    22fc:   e3a01000    mov r1, #0  ; 0x0
    2300:   e3a02064    mov r2, #100    ; 0x64
    2304:   e3a03000    mov r3, #0  ; 0x0
    2308:   e12fff3c    blx ip
    230c:   ea00005c    b   2484 <MGC_DrcChangeOtgState+0x46c>
    2310:   e51b301c    ldr r3, [fp, #-28]
    2314:   e59331b8    ldr r3, [r3, #440]
    2318:   e3530000    cmp r3, #0  ; 0x0
    231c:   0a000058    beq 2484 <MGC_DrcChangeOtgState+0x46c>
    2320:   e51b301c    ldr r3, [fp, #-28]
    2324:   e59331b8    ldr r3, [r3, #440]
    2328:   e5933020    ldr r3, [r3, #32]
    232c:   e1a01003    mov r1, r3
    2330:   e51b301c    ldr r3, [fp, #-28]
    2334:   e59331b8    ldr r3, [r3, #440]
    2338:   e5933020    ldr r3, [r3, #32]
    233c:   e2833002    add r3, r3, #2  ; 0x2
    2340:   e2833001    add r3, r3, #1  ; 0x1
    2344:   e5d33000    ldrb    r3, [r3]
    2348:   e1a03403    lsl r3, r3, #8
    234c:   e6ff2073    uxth    r2, r3
    2350:   e51b301c    ldr r3, [fp, #-28]
    2354:   e59331b8    ldr r3, [r3, #440]
    2358:   e5933020    ldr r3, [r3, #32]
    235c:   e2833002    add r3, r3, #2  ; 0x2
    2360:   e5d33000    ldrb    r3, [r3]
    2364:   e1823003    orr r3, r2, r3
    2368:   e6ff3073    uxth    r3, r3
    236c:   e6ff3073    uxth    r3, r3
    2370:   e1a00001    mov r0, r1
    2374:   e1a01003    mov r1, r3
    2378:   e3a02009    mov r2, #9  ; 0x9
    237c:   e3a03000    mov r3, #0  ; 0x0
    2380:   ebfffffe    bl  0 <MGC_FindDescriptor>
            2380: R_ARM_PC24    MGC_FindDescriptor
    2384:   e1a03000    mov r3, r0
    2388:   e50b3014    str r3, [fp, #-20]
    238c:   e51b3014    ldr r3, [fp, #-20]
    2390:   e3530000    cmp r3, #0  ; 0x0
    2394:   0a000011    beq 23e0 <MGC_DrcChangeOtgState+0x3c8>
    2398:   e51b3014    ldr r3, [fp, #-20]
    239c:   e5d33002    ldrb    r3, [r3, #2]
    23a0:   e2033002    and r3, r3, #2  ; 0x2
    23a4:   e3530000    cmp r3, #0  ; 0x0
    23a8:   0a00000c    beq 23e0 <MGC_DrcChangeOtgState+0x3c8>
    23ac:   e51b301c    ldr r3, [fp, #-28]
    23b0:   e59321b8    ldr r2, [r3, #440]
    23b4:   e59f31e8    ldr r3, [pc, #488]  ; 25a4 <MGC_DrcChangeOtgState+0x58c>
    23b8:   e5832000    str r2, [r3]
    23bc:   e59f21e0    ldr r2, [pc, #480]  ; 25a4 <MGC_DrcChangeOtgState+0x58c>
    23c0:   e51b301c    ldr r3, [fp, #-28]
    23c4:   e5823024    str r3, [r2, #36]
    23c8:   e51b301c    ldr r3, [fp, #-28]
    23cc:   e5933004    ldr r3, [r3, #4]
    23d0:   e1a00003    mov r0, r3
    23d4:   e59f11c8    ldr r1, [pc, #456]  ; 25a4 <MGC_DrcChangeOtgState+0x58c>
    23d8:   ebfffffe    bl  0 <MUSB_StartControlTransfer>
            23d8: R_ARM_PC24    MUSB_StartControlTransfer
    23dc:   ea000028    b   2484 <MGC_DrcChangeOtgState+0x46c>
    23e0:   e51b001c    ldr r0, [fp, #-28]
    23e4:   ebfffffe    bl  0 <MUSB_SuspendBus>
            23e4: R_ARM_PC24    MUSB_SuspendBus
    23e8:   ea000025    b   2484 <MGC_DrcChangeOtgState+0x46c>
    23ec:   e59f21b4    ldr r2, [pc, #436]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    23f0:   e3a03000    mov r3, #0  ; 0x0
    23f4:   e5c23000    strb    r3, [r2]
    23f8:   e51b301c    ldr r3, [fp, #-28]
    23fc:   e5933000    ldr r3, [r3]
    2400:   e5933008    ldr r3, [r3, #8]
    2404:   e5933004    ldr r3, [r3, #4]
    2408:   e59f0198    ldr r0, [pc, #408]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    240c:   e3a01c01    mov r1, #256    ; 0x100
    2410:   e59f2194    ldr r2, [pc, #404]  ; 25ac <MGC_DrcChangeOtgState+0x594>
    2414:   e12fff33    blx r3
    2418:   e51b301c    ldr r3, [fp, #-28]
    241c:   e5933000    ldr r3, [r3]
    2420:   e5933008    ldr r3, [r3, #8]
    2424:   e593c008    ldr ip, [r3, #8]
    2428:   e55b2018    ldrb    r2, [fp, #-24]
    242c:   e3a03000    mov r3, #0  ; 0x0
    2430:   e58d3000    str r3, [sp]
    2434:   e59f016c    ldr r0, [pc, #364]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    2438:   e3a01c01    mov r1, #256    ; 0x100
    243c:   e3a03010    mov r3, #16 ; 0x10
    2440:   e12fff3c    blx ip
    2444:   e59f3164    ldr r3, [pc, #356]  ; 25b0 <MGC_DrcChangeOtgState+0x598>
    2448:   e5d33000    ldrb    r3, [r3]
    244c:   e3530000    cmp r3, #0  ; 0x0
    2450:   0a00004a    beq 2580 <MGC_DrcChangeOtgState+0x568>
    2454:   e51b301c    ldr r3, [fp, #-28]
    2458:   e5933000    ldr r3, [r3]
    245c:   e593300c    ldr r3, [r3, #12]
    2460:   e5932028    ldr r2, [r3, #40]
    2464:   e51b301c    ldr r3, [fp, #-28]
    2468:   e5933000    ldr r3, [r3]
    246c:   e593300c    ldr r3, [r3, #12]
    2470:   e5933004    ldr r3, [r3, #4]
    2474:   e1a00003    mov r0, r3
    2478:   e59f1128    ldr r1, [pc, #296]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    247c:   e12fff32    blx r2
    2480:   ea00003e    b   2580 <MGC_DrcChangeOtgState+0x568>
    2484:   e59f211c    ldr r2, [pc, #284]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    2488:   e3a03000    mov r3, #0  ; 0x0
    248c:   e5c23000    strb    r3, [r2]
    2490:   e51b301c    ldr r3, [fp, #-28]
    2494:   e5933000    ldr r3, [r3]
    2498:   e5933008    ldr r3, [r3, #8]
    249c:   e5933004    ldr r3, [r3, #4]
    24a0:   e59f0100    ldr r0, [pc, #256]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    24a4:   e3a01c01    mov r1, #256    ; 0x100
    24a8:   e59f2104    ldr r2, [pc, #260]  ; 25b4 <MGC_DrcChangeOtgState+0x59c>
    24ac:   e12fff33    blx r3
    24b0:   e51b301c    ldr r3, [fp, #-28]
    24b4:   e5933000    ldr r3, [r3]
    24b8:   e5933008    ldr r3, [r3, #8]
    24bc:   e593c008    ldr ip, [r3, #8]
    24c0:   e51b301c    ldr r3, [fp, #-28]
    24c4:   e5d3304c    ldrb    r3, [r3, #76]
    24c8:   e1a02003    mov r2, r3
    24cc:   e3a03000    mov r3, #0  ; 0x0
    24d0:   e58d3000    str r3, [sp]
    24d4:   e59f00cc    ldr r0, [pc, #204]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    24d8:   e3a01c01    mov r1, #256    ; 0x100
    24dc:   e3a0300a    mov r3, #10 ; 0xa
    24e0:   e12fff3c    blx ip
    24e4:   e51b301c    ldr r3, [fp, #-28]
    24e8:   e5933000    ldr r3, [r3]
    24ec:   e5933008    ldr r3, [r3, #8]
    24f0:   e5933004    ldr r3, [r3, #4]
    24f4:   e59f00ac    ldr r0, [pc, #172]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    24f8:   e3a01c01    mov r1, #256    ; 0x100
    24fc:   e59f20b4    ldr r2, [pc, #180]  ; 25b8 <MGC_DrcChangeOtgState+0x5a0>
    2500:   e12fff33    blx r3
    2504:   e51b301c    ldr r3, [fp, #-28]
    2508:   e5933000    ldr r3, [r3]
    250c:   e5933008    ldr r3, [r3, #8]
    2510:   e593c008    ldr ip, [r3, #8]
    2514:   e55b2018    ldrb    r2, [fp, #-24]
    2518:   e3a03000    mov r3, #0  ; 0x0
    251c:   e58d3000    str r3, [sp]
    2520:   e59f0080    ldr r0, [pc, #128]  ; 25a8 <MGC_DrcChangeOtgState+0x590>
    2524:   e3a01c01    mov r1, #256    ; 0x100
    2528:   e3a0300a    mov r3, #10 ; 0xa
    252c:   e12fff3c    blx ip
    2530:   e59f3078    ldr r3, [pc, #120]  ; 25b0 <MGC_DrcChangeOtgState+0x598>
    2534:   e5d33000    ldrb    r3, [r3]
    2538:   e3530001    cmp r3, #1  ; 0x1
    253c:   9a00000a    bls 256c <MGC_DrcChangeOtgState+0x554>
    2540:   e51b301c    ldr r3, [fp, #-28]
    2544:   e5933000    ldr r3, [r3]
    2548:   e593300c    ldr r3, [r3, #12]
    254c:   e5932028    ldr r2, [r3, #40]
    2550:   e51b301c    ldr r3, [fp, #-28]
    2554:   e5933000    ldr r3, [r3]
    2558:   e593300c    ldr r3, [r3, #12]
    255c:   e5933004    ldr r3, [r3, #4]
    2560:   e1a00003    mov r0, r3
    2564:   e59f103c    ldr r1, [pc, #60]   ; 25a8 <MGC_DrcChangeOtgState+0x590>
    2568:   e12fff32    blx r2
    256c:   e51b201c    ldr r2, [fp, #-28]
    2570:   e55b3018    ldrb    r3, [fp, #-24]
    2574:   e5c2304c    strb    r3, [r2, #76]
    2578:   e51b001c    ldr r0, [fp, #-28]
    257c:   ebfffffe    bl  25bc <MGC_CompleteOtgTransition>
            257c: R_ARM_PC24    MGC_CompleteOtgTransition
    2580:   e24bd00c    sub sp, fp, #12 ; 0xc
    2584:   e89da800    ldm sp, {fp, sp, pc}
    ...
            2588: R_ARM_ABS32   FAPI_USB_KeepVbusFlag
            258c: R_ARM_ABS32   MGC_BSrpFailed
    2590:   00001388    .word   0x00001388
    ...
            2594: R_ARM_ABS32   MGC_WaitSessReqTimeout
            2598: R_ARM_ABS32   MGC_BAase0BrstTimeout
            259c: R_ARM_ABS32   MGC_HostResetStart
            25a0: R_ARM_ABS32   MGC_HostResetComplete
    25a4:   0000002c    .word   0x0000002c
            25a4: R_ARM_ABS32   .data
    25a8:   00000000    .word   0x00000000
            25a8: R_ARM_ABS32   MGC_DiagMsg
    25ac:   0000014c    .word   0x0000014c
            25ac: R_ARM_ABS32   .rodata
    25b0:   00000000    .word   0x00000000
            25b0: R_ARM_ABS32   MGC_bDiagLevel
    25b4:   0000016c    .word   0x0000016c
            25b4: R_ARM_ABS32   .rodata
    25b8:   00000184    .word   0x00000184
            25b8: R_ARM_ABS32   .rodata

000025bc <MGC_CompleteOtgTransition>:
    25bc:   e1a0c00d    mov ip, sp
    25c0:   e92dd800    push    {fp, ip, lr, pc}
    25c4:   e24cb004    sub fp, ip, #4  ; 0x4
    25c8:   e24dd008    sub sp, sp, #8  ; 0x8
    25cc:   e50b0014    str r0, [fp, #-20]
    25d0:   e51b3014    ldr r3, [fp, #-20]
    25d4:   e5933050    ldr r3, [r3, #80]
    25d8:   e50b3010    str r3, [fp, #-16]
    25dc:   e51b3014    ldr r3, [fp, #-20]
    25e0:   e5d3304c    ldrb    r3, [r3, #76]
    25e4:   e203300f    and r3, r3, #15 ; 0xf
    25e8:   e3530001    cmp r3, #1  ; 0x1
    25ec:   1a000001    bne 25f8 <MGC_CompleteOtgTransition+0x3c>
    25f0:   e51b0014    ldr r0, [fp, #-20]
    25f4:   ebfffc8e    bl  1834 <MGC_StartPeripheralMode>
    25f8:   e51b3010    ldr r3, [fp, #-16]
    25fc:   e3530000    cmp r3, #0  ; 0x0
    2600:   0a000009    beq 262c <MGC_CompleteOtgTransition+0x70>
    2604:   e51b3010    ldr r3, [fp, #-16]
    2608:   e593c008    ldr ip, [r3, #8]
    260c:   e51b3010    ldr r3, [fp, #-16]
    2610:   e5932000    ldr r2, [r3]
    2614:   e51b3014    ldr r3, [fp, #-20]
    2618:   e5d3304c    ldrb    r3, [r3, #76]
    261c:   e1a00002    mov r0, r2
    2620:   e51b1014    ldr r1, [fp, #-20]
    2624:   e1a02003    mov r2, r3
    2628:   e12fff3c    blx ip
    262c:   e24bd00c    sub sp, fp, #12 ; 0xc
    2630:   e89da800    ldm sp, {fp, sp, pc}

00002634 <MGC_DrcCompleteResume>:
    2634:   e1a0c00d    mov ip, sp
    2638:   e92dd800    push    {fp, ip, lr, pc}
    263c:   e24cb004    sub fp, ip, #4  ; 0x4
    2640:   e24dd020    sub sp, sp, #32 ; 0x20
    2644:   e50b0028    str r0, [fp, #-40]
    2648:   e1a03001    mov r3, r1
    264c:   e14b32bc    strh    r3, [fp, #-44]
    2650:   e51b3028    ldr r3, [fp, #-40]
    2654:   e50b3014    str r3, [fp, #-20]
    2658:   e51b3014    ldr r3, [fp, #-20]
    265c:   e5933014    ldr r3, [r3, #20]
    2660:   e50b3010    str r3, [fp, #-16]
    2664:   e51b2010    ldr r2, [fp, #-16]
    2668:   e3a03000    mov r3, #0  ; 0x0
    266c:   e5c23027    strb    r3, [r2, #39]
    2670:   e51b3010    ldr r3, [fp, #-16]
    2674:   e593321c    ldr r3, [r3, #540]
    2678:   e51b0010    ldr r0, [fp, #-16]
    267c:   e12fff33    blx r3
    2680:   e51b3010    ldr r3, [fp, #-16]
    2684:   e2833f6f    add r3, r3, #444    ; 0x1bc
    2688:   e1a00003    mov r0, r3
    268c:   ebfffffe    bl  0 <MUSB_ListLength>
            268c: R_ARM_PC24    MUSB_ListLength
    2690:   e1a03000    mov r3, r0
    2694:   e14b32b4    strh    r3, [fp, #-36]
    2698:   e3a03000    mov r3, #0  ; 0x0
    269c:   e14b32b2    strh    r3, [fp, #-34]
    26a0:   ea000021    b   272c <MGC_DrcCompleteResume+0xf8>
    26a4:   e51b3010    ldr r3, [fp, #-16]
    26a8:   e2833f6f    add r3, r3, #444    ; 0x1bc
    26ac:   e15b22b2    ldrh    r2, [fp, #-34]
    26b0:   e1a00003    mov r0, r3
    26b4:   e1a01002    mov r1, r2
    26b8:   ebfffffe    bl  0 <MUSB_ListFindItem>
            26b8: R_ARM_PC24    MUSB_ListFindItem
    26bc:   e1a03000    mov r3, r0
    26c0:   e50b3020    str r3, [fp, #-32]
    26c4:   e51b3020    ldr r3, [fp, #-32]
    26c8:   e3530000    cmp r3, #0  ; 0x0
    26cc:   0a000013    beq 2720 <MGC_DrcCompleteResume+0xec>
    26d0:   e51b3020    ldr r3, [fp, #-32]
    26d4:   e5933004    ldr r3, [r3, #4]
    26d8:   e50b301c    str r3, [fp, #-28]
    26dc:   e51b301c    ldr r3, [fp, #-28]
    26e0:   e3530000    cmp r3, #0  ; 0x0
    26e4:   0a00000d    beq 2720 <MGC_DrcCompleteResume+0xec>
    26e8:   e51b301c    ldr r3, [fp, #-28]
    26ec:   e5933000    ldr r3, [r3]
    26f0:   e50b3018    str r3, [fp, #-24]
    26f4:   e51b3018    ldr r3, [fp, #-24]
    26f8:   e5933014    ldr r3, [r3, #20]
    26fc:   e3530000    cmp r3, #0  ; 0x0
    2700:   0a000006    beq 2720 <MGC_DrcCompleteResume+0xec>
    2704:   e51b3018    ldr r3, [fp, #-24]
    2708:   e5932018    ldr r2, [r3, #24]
    270c:   e51b3018    ldr r3, [fp, #-24]
    2710:   e5933000    ldr r3, [r3]
    2714:   e1a00003    mov r0, r3
    2718:   e51b1010    ldr r1, [fp, #-16]
    271c:   e12fff32    blx r2
    2720:   e15b32b2    ldrh    r3, [fp, #-34]
    2724:   e2833001    add r3, r3, #1  ; 0x1
    2728:   e14b32b2    strh    r3, [fp, #-34]
    272c:   e15b22b2    ldrh    r2, [fp, #-34]
    2730:   e15b32b4    ldrh    r3, [fp, #-36]
    2734:   e1520003    cmp r2, r3
    2738:   3affffd9    bcc 26a4 <MGC_DrcCompleteResume+0x70>
    273c:   e24bd00c    sub sp, fp, #12 ; 0xc
    2740:   e89da800    ldm sp, {fp, sp, pc}

00002744 <MGC_DrcUsbIsr>:
    2744:   e1a0c00d    mov ip, sp
    2748:   e92dd810    push    {r4, fp, ip, lr, pc}
    274c:   e24cb004    sub fp, ip, #4  ; 0x4
    2750:   e24dd034    sub sp, sp, #52 ; 0x34
    2754:   e50b0038    str r0, [fp, #-56]
    2758:   e1a03001    mov r3, r1
    275c:   e54b303c    strb    r3, [fp, #-60]
    2760:   e3a03000    mov r3, #0  ; 0x0
    2764:   e54b3019    strb    r3, [fp, #-25]
    2768:   e51b3038    ldr r3, [fp, #-56]
    276c:   e5933000    ldr r3, [r3]
    2770:   e593300c    ldr r3, [r3, #12]
    2774:   e50b3018    str r3, [fp, #-24]
    2778:   e55b303c    ldrb    r3, [fp, #-60]
    277c:   e3530000    cmp r3, #0  ; 0x0
    2780:   0a00011f    beq 2c04 <MGC_DrcUsbIsr+0x4c0>
    2784:   e55b303c    ldrb    r3, [fp, #-60]
    2788:   e2033002    and r3, r3, #2  ; 0x2
    278c:   e3530000    cmp r3, #0  ; 0x0
    2790:   0a00000f    beq 27d4 <MGC_DrcUsbIsr+0x90>
    2794:   e51b0038    ldr r0, [fp, #-56]
    2798:   e3a01000    mov r1, #0  ; 0x0
    279c:   ebfffc1b    bl  1810 <MGC_SetPowerSave>
    27a0:   e55b3019    ldrb    r3, [fp, #-25]
    27a4:   e2833001    add r3, r3, #1  ; 0x1
    27a8:   e54b3019    strb    r3, [fp, #-25]
    27ac:   e3a03007    mov r3, #7  ; 0x7
    27b0:   e54b3030    strb    r3, [fp, #-48]
    27b4:   e51b3018    ldr r3, [fp, #-24]
    27b8:   e593c00c    ldr ip, [r3, #12]
    27bc:   e51b3018    ldr r3, [fp, #-24]
    27c0:   e5933004    ldr r3, [r3, #4]
    27c4:   e24b2034    sub r2, fp, #52 ; 0x34
    27c8:   e1a00003    mov r0, r3
    27cc:   e1a01002    mov r1, r2
    27d0:   e12fff3c    blx ip
    27d4:   e55b303c    ldrb    r3, [fp, #-60]
    27d8:   e2033040    and r3, r3, #64 ; 0x40
    27dc:   e3530000    cmp r3, #0  ; 0x0
    27e0:   0a00000c    beq 2818 <MGC_DrcUsbIsr+0xd4>
    27e4:   e55b3019    ldrb    r3, [fp, #-25]
    27e8:   e2833001    add r3, r3, #1  ; 0x1
    27ec:   e54b3019    strb    r3, [fp, #-25]
    27f0:   e3a03001    mov r3, #1  ; 0x1
    27f4:   e54b3030    strb    r3, [fp, #-48]
    27f8:   e51b3018    ldr r3, [fp, #-24]
    27fc:   e593c00c    ldr ip, [r3, #12]
    2800:   e51b3018    ldr r3, [fp, #-24]
    2804:   e5933004    ldr r3, [r3, #4]
    2808:   e24b2034    sub r2, fp, #52 ; 0x34
    280c:   e1a00003    mov r0, r3
    2810:   e1a01002    mov r1, r2
    2814:   e12fff3c    blx ip
    2818:   e55b303c    ldrb    r3, [fp, #-60]
    281c:   e6af3073    sxtb    r3, r3
    2820:   e3530000    cmp r3, #0  ; 0x0
    2824:   aa00000c    bge 285c <MGC_DrcUsbIsr+0x118>
    2828:   e55b3019    ldrb    r3, [fp, #-25]
    282c:   e2833001    add r3, r3, #1  ; 0x1
    2830:   e54b3019    strb    r3, [fp, #-25]
    2834:   e3a03008    mov r3, #8  ; 0x8
    2838:   e54b3030    strb    r3, [fp, #-48]
    283c:   e51b3018    ldr r3, [fp, #-24]
    2840:   e593c00c    ldr ip, [r3, #12]
    2844:   e51b3018    ldr r3, [fp, #-24]
    2848:   e5933004    ldr r3, [r3, #4]
    284c:   e24b2034    sub r2, fp, #52 ; 0x34
    2850:   e1a00003    mov r0, r3
    2854:   e1a01002    mov r1, r2
    2858:   e12fff3c    blx ip
    285c:   e55b303c    ldrb    r3, [fp, #-60]
    2860:   e2033001    and r3, r3, #1  ; 0x1
    2864:   e6ef3073    uxtb    r3, r3
    2868:   e3530000    cmp r3, #0  ; 0x0
    286c:   0a00000c    beq 28a4 <MGC_DrcUsbIsr+0x160>
    2870:   e55b3019    ldrb    r3, [fp, #-25]
    2874:   e2833001    add r3, r3, #1  ; 0x1
    2878:   e54b3019    strb    r3, [fp, #-25]
    287c:   e3a03006    mov r3, #6  ; 0x6
    2880:   e54b3030    strb    r3, [fp, #-48]
    2884:   e51b3018    ldr r3, [fp, #-24]
    2888:   e593c00c    ldr ip, [r3, #12]
    288c:   e51b3018    ldr r3, [fp, #-24]
    2890:   e5933004    ldr r3, [r3, #4]
    2894:   e24b2034    sub r2, fp, #52 ; 0x34
    2898:   e1a00003    mov r0, r3
    289c:   e1a01002    mov r1, r2
    28a0:   e12fff3c    blx ip
    28a4:   e55b303c    ldrb    r3, [fp, #-60]
    28a8:   e2033010    and r3, r3, #16 ; 0x10
    28ac:   e3530000    cmp r3, #0  ; 0x0
    28b0:   0a00000f    beq 28f4 <MGC_DrcUsbIsr+0x1b0>
    28b4:   e51b2038    ldr r2, [fp, #-56]
    28b8:   e3a03001    mov r3, #1  ; 0x1
    28bc:   e5c2303e    strb    r3, [r2, #62]
    28c0:   e55b3019    ldrb    r3, [fp, #-25]
    28c4:   e2833001    add r3, r3, #1  ; 0x1
    28c8:   e54b3019    strb    r3, [fp, #-25]
    28cc:   e3a03002    mov r3, #2  ; 0x2
    28d0:   e54b3030    strb    r3, [fp, #-48]
    28d4:   e51b3018    ldr r3, [fp, #-24]
    28d8:   e593c00c    ldr ip, [r3, #12]
    28dc:   e51b3018    ldr r3, [fp, #-24]
    28e0:   e5933004    ldr r3, [r3, #4]
    28e4:   e24b2034    sub r2, fp, #52 ; 0x34
    28e8:   e1a00003    mov r0, r3
    28ec:   e1a01002    mov r1, r2
    28f0:   e12fff3c    blx ip
    28f4:   e55b303c    ldrb    r3, [fp, #-60]
    28f8:   e2033020    and r3, r3, #32 ; 0x20
    28fc:   e3530000    cmp r3, #0  ; 0x0
    2900:   0a000015    beq 295c <MGC_DrcUsbIsr+0x218>
    2904:   e51b3038    ldr r3, [fp, #-56]
    2908:   e5d33025    ldrb    r3, [r3, #37]
    290c:   e3530000    cmp r3, #0  ; 0x0
    2910:   1a000011    bne 295c <MGC_DrcUsbIsr+0x218>
    2914:   e51b0038    ldr r0, [fp, #-56]
    2918:   ebfffaab    bl  13cc <MGC_DrcFlushAll>
    291c:   e51b0038    ldr r0, [fp, #-56]
    2920:   e3a01002    mov r1, #2  ; 0x2
    2924:   ebfffffe    bl  0 <MGC_FunctionChangeState>
            2924: R_ARM_PC24    MGC_FunctionChangeState
    2928:   e55b3019    ldrb    r3, [fp, #-25]
    292c:   e2833001    add r3, r3, #1  ; 0x1
    2930:   e54b3019    strb    r3, [fp, #-25]
    2934:   e3a03003    mov r3, #3  ; 0x3
    2938:   e54b3030    strb    r3, [fp, #-48]
    293c:   e51b3018    ldr r3, [fp, #-24]
    2940:   e593c00c    ldr ip, [r3, #12]
    2944:   e51b3018    ldr r3, [fp, #-24]
    2948:   e5933004    ldr r3, [r3, #4]
    294c:   e24b2034    sub r2, fp, #52 ; 0x34
    2950:   e1a00003    mov r0, r3
    2954:   e1a01002    mov r1, r2
    2958:   e12fff3c    blx ip
    295c:   e55b303c    ldrb    r3, [fp, #-60]
    2960:   e2033004    and r3, r3, #4  ; 0x4
    2964:   e3530000    cmp r3, #0  ; 0x0
    2968:   0a00005c    beq 2ae0 <MGC_DrcUsbIsr+0x39c>
    296c:   e51b3038    ldr r3, [fp, #-56]
    2970:   e5d3303e    ldrb    r3, [r3, #62]
    2974:   e54b3011    strb    r3, [fp, #-17]
    2978:   e51b3038    ldr r3, [fp, #-56]
    297c:   e5933218    ldr r3, [r3, #536]
    2980:   e51b0038    ldr r0, [fp, #-56]
    2984:   e12fff33    blx r3
    2988:   e51b3038    ldr r3, [fp, #-56]
    298c:   e5d3303e    ldrb    r3, [r3, #62]
    2990:   e3530000    cmp r3, #0  ; 0x0
    2994:   0a00000d    beq 29d0 <MGC_DrcUsbIsr+0x28c>
    2998:   e55b3019    ldrb    r3, [fp, #-25]
    299c:   e2833001    add r3, r3, #1  ; 0x1
    29a0:   e54b3019    strb    r3, [fp, #-25]
    29a4:   e3a03005    mov r3, #5  ; 0x5
    29a8:   e54b3030    strb    r3, [fp, #-48]
    29ac:   e51b3018    ldr r3, [fp, #-24]
    29b0:   e593c00c    ldr ip, [r3, #12]
    29b4:   e51b3018    ldr r3, [fp, #-24]
    29b8:   e5933004    ldr r3, [r3, #4]
    29bc:   e24b2034    sub r2, fp, #52 ; 0x34
    29c0:   e1a00003    mov r0, r3
    29c4:   e1a01002    mov r1, r2
    29c8:   e12fff3c    blx ip
    29cc:   ea000043    b   2ae0 <MGC_DrcUsbIsr+0x39c>
    29d0:   e55b3011    ldrb    r3, [fp, #-17]
    29d4:   e3530000    cmp r3, #0  ; 0x0
    29d8:   0a000016    beq 2a38 <MGC_DrcUsbIsr+0x2f4>
    29dc:   e51b3038    ldr r3, [fp, #-56]
    29e0:   e5d33025    ldrb    r3, [r3, #37]
    29e4:   e3530000    cmp r3, #0  ; 0x0
    29e8:   1a00003c    bne 2ae0 <MGC_DrcUsbIsr+0x39c>
    29ec:   e51b0038    ldr r0, [fp, #-56]
    29f0:   ebfffa75    bl  13cc <MGC_DrcFlushAll>
    29f4:   e51b0038    ldr r0, [fp, #-56]
    29f8:   e3a01002    mov r1, #2  ; 0x2
    29fc:   ebfffffe    bl  0 <MGC_FunctionChangeState>
            29fc: R_ARM_PC24    MGC_FunctionChangeState
    2a00:   e55b3019    ldrb    r3, [fp, #-25]
    2a04:   e2833001    add r3, r3, #1  ; 0x1
    2a08:   e54b3019    strb    r3, [fp, #-25]
    2a0c:   e3a03003    mov r3, #3  ; 0x3
    2a10:   e54b3030    strb    r3, [fp, #-48]
    2a14:   e51b3018    ldr r3, [fp, #-24]
    2a18:   e593c00c    ldr ip, [r3, #12]
    2a1c:   e51b3018    ldr r3, [fp, #-24]
    2a20:   e5933004    ldr r3, [r3, #4]
    2a24:   e24b2034    sub r2, fp, #52 ; 0x34
    2a28:   e1a00003    mov r0, r3
    2a2c:   e1a01002    mov r1, r2
    2a30:   e12fff3c    blx ip
    2a34:   ea000029    b   2ae0 <MGC_DrcUsbIsr+0x39c>
    2a38:   e51b3038    ldr r3, [fp, #-56]
    2a3c:   e5d3303d    ldrb    r3, [r3, #61]
    2a40:   e51b2038    ldr r2, [fp, #-56]
    2a44:   e5c23029    strb    r3, [r2, #41]
    2a48:   e51b2038    ldr r2, [fp, #-56]
    2a4c:   e3a03000    mov r3, #0  ; 0x0
    2a50:   e5c2302a    strb    r3, [r2, #42]
    2a54:   e51b2038    ldr r2, [fp, #-56]
    2a58:   e3a03000    mov r3, #0  ; 0x0
    2a5c:   e5c23028    strb    r3, [r2, #40]
    2a60:   e51b1038    ldr r1, [fp, #-56]
    2a64:   e59f21a8    ldr r2, [pc, #424]  ; 2c14 <MGC_DrcUsbIsr+0x4d0>
    2a68:   e3a03000    mov r3, #0  ; 0x0
    2a6c:   e18130b2    strh    r3, [r1, r2]
    2a70:   e51b1038    ldr r1, [fp, #-56]
    2a74:   e59f219c    ldr r2, [pc, #412]  ; 2c18 <MGC_DrcUsbIsr+0x4d4>
    2a78:   e3a03000    mov r3, #0  ; 0x0
    2a7c:   e18130b2    strh    r3, [r1, r2]
    2a80:   e51b0038    ldr r0, [fp, #-56]
    2a84:   ebfffa50    bl  13cc <MGC_DrcFlushAll>
    2a88:   e51b0038    ldr r0, [fp, #-56]
    2a8c:   e3a01002    mov r1, #2  ; 0x2
    2a90:   ebfffffe    bl  0 <MGC_FunctionChangeState>
            2a90: R_ARM_PC24    MGC_FunctionChangeState
    2a94:   e51b0038    ldr r0, [fp, #-56]
    2a98:   ebfffffe    bl  0 <MGC_FunctionSpeedSet>
            2a98: R_ARM_PC24    MGC_FunctionSpeedSet
    2a9c:   e51b3038    ldr r3, [fp, #-56]
    2aa0:   e5d3304c    ldrb    r3, [r3, #76]
    2aa4:   e50b3040    str r3, [fp, #-64]
    2aa8:   e51b3040    ldr r3, [fp, #-64]
    2aac:   e3530000    cmp r3, #0  ; 0x0
    2ab0:   0a000003    beq 2ac4 <MGC_DrcUsbIsr+0x380>
    2ab4:   e51b3040    ldr r3, [fp, #-64]
    2ab8:   e3530014    cmp r3, #20 ; 0x14
    2abc:   0a000004    beq 2ad4 <MGC_DrcUsbIsr+0x390>
    2ac0:   ea000006    b   2ae0 <MGC_DrcUsbIsr+0x39c>
    2ac4:   e51b0038    ldr r0, [fp, #-56]
    2ac8:   e3a01001    mov r1, #1  ; 0x1
    2acc:   eb000501    bl  3ed8 <MGC_OtgStateGetId>
    2ad0:   ea000002    b   2ae0 <MGC_DrcUsbIsr+0x39c>
    2ad4:   e3a00011    mov r0, #17 ; 0x11
    2ad8:   e51b1038    ldr r1, [fp, #-56]
    2adc:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2adc: R_ARM_PC24    MGC_DrcChangeOtgState
    2ae0:   e55b303c    ldrb    r3, [fp, #-60]
    2ae4:   e2033008    and r3, r3, #8  ; 0x8
    2ae8:   e3530000    cmp r3, #0  ; 0x0
    2aec:   0a000044    beq 2c04 <MGC_DrcUsbIsr+0x4c0>
    2af0:   e51b3038    ldr r3, [fp, #-56]
    2af4:   e5933218    ldr r3, [r3, #536]
    2af8:   e51b0038    ldr r0, [fp, #-56]
    2afc:   e12fff33    blx r3
    2b00:   e51b3038    ldr r3, [fp, #-56]
    2b04:   e283300c    add r3, r3, #12 ; 0xc
    2b08:   e1a00003    mov r0, r3
    2b0c:   ebfffffe    bl  0 <MUSB_ArrayLength>
            2b0c: R_ARM_PC24    MUSB_ArrayLength
    2b10:   e1a03000    mov r3, r0
    2b14:   e50b302c    str r3, [fp, #-44]
    2b18:   e3a03001    mov r3, #1  ; 0x1
    2b1c:   e50b3028    str r3, [fp, #-40]
    2b20:   ea00002d    b   2bdc <MGC_DrcUsbIsr+0x498>
    2b24:   e51b3038    ldr r3, [fp, #-56]
    2b28:   e283300c    add r3, r3, #12 ; 0xc
    2b2c:   e1a00003    mov r0, r3
    2b30:   e51b1028    ldr r1, [fp, #-40]
    2b34:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            2b34: R_ARM_PC24    MUSB_ArrayFetch
    2b38:   e1a03000    mov r3, r0
    2b3c:   e50b3024    str r3, [fp, #-36]
    2b40:   e51b3024    ldr r3, [fp, #-36]
    2b44:   e3530000    cmp r3, #0  ; 0x0
    2b48:   0a000020    beq 2bd0 <MGC_DrcUsbIsr+0x48c>
    2b4c:   e51b3024    ldr r3, [fp, #-36]
    2b50:   e5d33009    ldrb    r3, [r3, #9]
    2b54:   e3530001    cmp r3, #1  ; 0x1
    2b58:   1a00001c    bne 2bd0 <MGC_DrcUsbIsr+0x48c>
    2b5c:   e51b3024    ldr r3, [fp, #-36]
    2b60:   e5933058    ldr r3, [r3, #88]
    2b64:   e3530000    cmp r3, #0  ; 0x0
    2b68:   0a000018    beq 2bd0 <MGC_DrcUsbIsr+0x48c>
    2b6c:   e51b3024    ldr r3, [fp, #-36]
    2b70:   e593301c    ldr r3, [r3, #28]
    2b74:   e50b3020    str r3, [fp, #-32]
    2b78:   e51b3024    ldr r3, [fp, #-36]
    2b7c:   e5933058    ldr r3, [r3, #88]
    2b80:   e2432001    sub r2, r3, #1  ; 0x1
    2b84:   e51b3024    ldr r3, [fp, #-36]
    2b88:   e5832058    str r2, [r3, #88]
    2b8c:   e51b3024    ldr r3, [fp, #-36]
    2b90:   e5933058    ldr r3, [r3, #88]
    2b94:   e3530000    cmp r3, #0  ; 0x0
    2b98:   1a00000c    bne 2bd0 <MGC_DrcUsbIsr+0x48c>
    2b9c:   e51b3038    ldr r3, [fp, #-56]
    2ba0:   e593422c    ldr r4, [r3, #556]
    2ba4:   e51b3020    ldr r3, [fp, #-32]
    2ba8:   e5932004    ldr r2, [r3, #4]
    2bac:   e51b3020    ldr r3, [fp, #-32]
    2bb0:   e5933008    ldr r3, [r3, #8]
    2bb4:   e593c000    ldr ip, [r3]
    2bb8:   e51b3020    ldr r3, [fp, #-32]
    2bbc:   e58d3000    str r3, [sp]
    2bc0:   e51b0038    ldr r0, [fp, #-56]
    2bc4:   e51b1024    ldr r1, [fp, #-36]
    2bc8:   e1a0300c    mov r3, ip
    2bcc:   e12fff34    blx r4
    2bd0:   e51b3028    ldr r3, [fp, #-40]
    2bd4:   e2833001    add r3, r3, #1  ; 0x1
    2bd8:   e50b3028    str r3, [fp, #-40]
    2bdc:   e51b2028    ldr r2, [fp, #-40]
    2be0:   e51b302c    ldr r3, [fp, #-44]
    2be4:   e1520003    cmp r2, r3
    2be8:   3affffcd    bcc 2b24 <MGC_DrcUsbIsr+0x3e0>
    2bec:   e51b3038    ldr r3, [fp, #-56]
    2bf0:   e5d3303e    ldrb    r3, [r3, #62]
    2bf4:   e3530000    cmp r3, #0  ; 0x0
    2bf8:   0a000001    beq 2c04 <MGC_DrcUsbIsr+0x4c0>
    2bfc:   e51b0038    ldr r0, [fp, #-56]
    2c00:   ebfffffe    bl  0 <MGC_RunScheduledTransfers>
            2c00: R_ARM_PC24    MGC_RunScheduledTransfers
    2c04:   e55b3019    ldrb    r3, [fp, #-25]
    2c08:   e1a00003    mov r0, r3
    2c0c:   e24bd010    sub sp, fp, #16 ; 0x10
    2c10:   e89da810    ldm sp, {r4, fp, sp, pc}
    2c14:   000001f6    .word   0x000001f6
    2c18:   000001fa    .word   0x000001fa

00002c1c <MGC_DrcBsr>:
    2c1c:   e1a0c00d    mov ip, sp
    2c20:   e92dd800    push    {fp, ip, lr, pc}
    2c24:   e24cb004    sub fp, ip, #4  ; 0x4
    2c28:   e24dd074    sub sp, sp, #116    ; 0x74
    2c2c:   e50b006c    str r0, [fp, #-108]
    2c30:   e51b306c    ldr r3, [fp, #-108]
    2c34:   e50b3024    str r3, [fp, #-36]
    2c38:   e51b3024    ldr r3, [fp, #-36]
    2c3c:   e5933000    ldr r3, [r3]
    2c40:   e50b3020    str r3, [fp, #-32]
    2c44:   e51b3020    ldr r3, [fp, #-32]
    2c48:   e5933000    ldr r3, [r3]
    2c4c:   e50b301c    str r3, [fp, #-28]
    2c50:   e51b301c    ldr r3, [fp, #-28]
    2c54:   e593300c    ldr r3, [r3, #12]
    2c58:   e50b3018    str r3, [fp, #-24]
    2c5c:   e51b3020    ldr r3, [fp, #-32]
    2c60:   e5933050    ldr r3, [r3, #80]
    2c64:   e50b3014    str r3, [fp, #-20]
    2c68:   e51b3018    ldr r3, [fp, #-24]
    2c6c:   e593c010    ldr ip, [r3, #16]
    2c70:   e51b3018    ldr r3, [fp, #-24]
    2c74:   e5933004    ldr r3, [r3, #4]
    2c78:   e24b2040    sub r2, fp, #64 ; 0x40
    2c7c:   e1a00003    mov r0, r3
    2c80:   e1a01002    mov r1, r2
    2c84:   e12fff3c    blx ip
    2c88:   e1a03000    mov r3, r0
    2c8c:   e54b3036    strb    r3, [fp, #-54]
    2c90:   ea000268    b   3638 <MGC_DrcBsr+0xa1c>
    2c94:   e55b303c    ldrb    r3, [fp, #-60]
    2c98:   e2433001    sub r3, r3, #1  ; 0x1
    2c9c:   e3530023    cmp r3, #35 ; 0x23
    2ca0:   979ff103    ldrls   pc, [pc, r3, lsl #2]
    2ca4:   ea000259    b   3610 <MGC_DrcBsr+0x9f4>
    2ca8:   00002e44    .word   0x00002e44
            2ca8: R_ARM_ABS32   .text
    2cac:   00003094    .word   0x00003094
            2cac: R_ARM_ABS32   .text
    2cb0:   00003124    .word   0x00003124
            2cb0: R_ARM_ABS32   .text
    2cb4:   00003610    .word   0x00003610
            2cb4: R_ARM_ABS32   .text
    2cb8:   00003318    .word   0x00003318
            2cb8: R_ARM_ABS32   .text
    2cbc:   00002f9c    .word   0x00002f9c
            2cbc: R_ARM_ABS32   .text
    2cc0:   00002d38    .word   0x00002d38
            2cc0: R_ARM_ABS32   .text
    2cc4:   00002eb4    .word   0x00002eb4
            2cc4: R_ARM_ABS32   .text
    2cc8:   00003610    .word   0x00003610
            2cc8: R_ARM_ABS32   .text
    2ccc:   00003610    .word   0x00003610
            2ccc: R_ARM_ABS32   .text
    2cd0:   00003610    .word   0x00003610
            2cd0: R_ARM_ABS32   .text
    2cd4:   00003610    .word   0x00003610
            2cd4: R_ARM_ABS32   .text
    2cd8:   00003610    .word   0x00003610
            2cd8: R_ARM_ABS32   .text
    2cdc:   00003610    .word   0x00003610
            2cdc: R_ARM_ABS32   .text
    2ce0:   00003610    .word   0x00003610
            2ce0: R_ARM_ABS32   .text
    2ce4:   00003610    .word   0x00003610
            2ce4: R_ARM_ABS32   .text
    2ce8:   00003610    .word   0x00003610
            2ce8: R_ARM_ABS32   .text
    2cec:   00003610    .word   0x00003610
            2cec: R_ARM_ABS32   .text
    2cf0:   00003610    .word   0x00003610
            2cf0: R_ARM_ABS32   .text
    2cf4:   00003610    .word   0x00003610
            2cf4: R_ARM_ABS32   .text
    2cf8:   00003610    .word   0x00003610
            2cf8: R_ARM_ABS32   .text
    2cfc:   00003610    .word   0x00003610
            2cfc: R_ARM_ABS32   .text
    2d00:   00003610    .word   0x00003610
            2d00: R_ARM_ABS32   .text
    2d04:   00003610    .word   0x00003610
            2d04: R_ARM_ABS32   .text
    2d08:   00003610    .word   0x00003610
            2d08: R_ARM_ABS32   .text
    2d0c:   00003610    .word   0x00003610
            2d0c: R_ARM_ABS32   .text
    2d10:   00003610    .word   0x00003610
            2d10: R_ARM_ABS32   .text
    2d14:   00003610    .word   0x00003610
            2d14: R_ARM_ABS32   .text
    2d18:   00003610    .word   0x00003610
            2d18: R_ARM_ABS32   .text
    2d1c:   00003610    .word   0x00003610
            2d1c: R_ARM_ABS32   .text
    2d20:   00003610    .word   0x00003610
            2d20: R_ARM_ABS32   .text
    2d24:   00003610    .word   0x00003610
            2d24: R_ARM_ABS32   .text
    2d28:   00003610    .word   0x00003610
            2d28: R_ARM_ABS32   .text
    2d2c:   00003610    .word   0x00003610
            2d2c: R_ARM_ABS32   .text
    2d30:   00003448    .word   0x00003448
            2d30: R_ARM_ABS32   .text
    2d34:   00003448    .word   0x00003448
            2d34: R_ARM_ABS32   .text
    2d38:   e59f390c    ldr r3, [pc, #2316] ; 364c <MGC_DrcBsr+0xa30>
    2d3c:   e5d33000    ldrb    r3, [r3]
    2d40:   e3530001    cmp r3, #1  ; 0x1
    2d44:   9a000008    bls 2d6c <MGC_DrcBsr+0x150>
    2d48:   e51b301c    ldr r3, [fp, #-28]
    2d4c:   e593300c    ldr r3, [r3, #12]
    2d50:   e5932028    ldr r2, [r3, #40]
    2d54:   e51b301c    ldr r3, [fp, #-28]
    2d58:   e593300c    ldr r3, [r3, #12]
    2d5c:   e5933004    ldr r3, [r3, #4]
    2d60:   e1a00003    mov r0, r3
    2d64:   e59f18e4    ldr r1, [pc, #2276] ; 3650 <MGC_DrcBsr+0xa34>
    2d68:   e12fff32    blx r2
    2d6c:   e51b3020    ldr r3, [fp, #-32]
    2d70:   e59331e0    ldr r3, [r3, #480]
    2d74:   e3530000    cmp r3, #0  ; 0x0
    2d78:   0a00000b    beq 2dac <MGC_DrcBsr+0x190>
    2d7c:   e51b3020    ldr r3, [fp, #-32]
    2d80:   e59331e0    ldr r3, [r3, #480]
    2d84:   e593c028    ldr ip, [r3, #40]
    2d88:   e51b3020    ldr r3, [fp, #-32]
    2d8c:   e59331e0    ldr r3, [r3, #480]
    2d90:   e5932000    ldr r2, [r3]
    2d94:   e51b3020    ldr r3, [fp, #-32]
    2d98:   e5d331f0    ldrb    r3, [r3, #496]
    2d9c:   e1a00002    mov r0, r2
    2da0:   e51b1020    ldr r1, [fp, #-32]
    2da4:   e1a02003    mov r2, r3
    2da8:   e12fff3c    blx ip
    2dac:   e51b3020    ldr r3, [fp, #-32]
    2db0:   e5d3304c    ldrb    r3, [r3, #76]
    2db4:   e50b307c    str r3, [fp, #-124]
    2db8:   e51b007c    ldr r0, [fp, #-124]
    2dbc:   e3500012    cmp r0, #18 ; 0x12
    2dc0:   0a000003    beq 2dd4 <MGC_DrcBsr+0x1b8>
    2dc4:   e51b107c    ldr r1, [fp, #-124]
    2dc8:   e3510024    cmp r1, #36 ; 0x24
    2dcc:   0a000004    beq 2de4 <MGC_DrcBsr+0x1c8>
    2dd0:   ea00020e    b   3610 <MGC_DrcBsr+0x9f4>
    2dd4:   e3a00011    mov r0, #17 ; 0x11
    2dd8:   e51b1020    ldr r1, [fp, #-32]
    2ddc:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2ddc: R_ARM_PC24    MGC_DrcChangeOtgState
    2de0:   ea00020a    b   3610 <MGC_DrcBsr+0x9f4>
    2de4:   e3a00023    mov r0, #35 ; 0x23
    2de8:   e51b1020    ldr r1, [fp, #-32]
    2dec:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2dec: R_ARM_PC24    MGC_DrcChangeOtgState
    2df0:   e51b2020    ldr r2, [fp, #-32]
    2df4:   e3a03001    mov r3, #1  ; 0x1
    2df8:   e5c23027    strb    r3, [r2, #39]
    2dfc:   e51b3020    ldr r3, [fp, #-32]
    2e00:   e593321c    ldr r3, [r3, #540]
    2e04:   e51b0020    ldr r0, [fp, #-32]
    2e08:   e12fff33    blx r3
    2e0c:   e51b301c    ldr r3, [fp, #-28]
    2e10:   e593300c    ldr r3, [r3, #12]
    2e14:   e593c018    ldr ip, [r3, #24]
    2e18:   e51b301c    ldr r3, [fp, #-28]
    2e1c:   e593300c    ldr r3, [r3, #12]
    2e20:   e5932004    ldr r2, [r3, #4]
    2e24:   e59f3828    ldr r3, [pc, #2088] ; 3654 <MGC_DrcBsr+0xa38>
    2e28:   e58d3000    str r3, [sp]
    2e2c:   e1a00002    mov r0, r2
    2e30:   e3a01000    mov r1, #0  ; 0x0
    2e34:   e3a02014    mov r2, #20 ; 0x14
    2e38:   e3a03000    mov r3, #0  ; 0x0
    2e3c:   e12fff3c    blx ip
    2e40:   ea0001f2    b   3610 <MGC_DrcBsr+0x9f4>
    2e44:   e59f3800    ldr r3, [pc, #2048] ; 364c <MGC_DrcBsr+0xa30>
    2e48:   e5d33000    ldrb    r3, [r3]
    2e4c:   e3530001    cmp r3, #1  ; 0x1
    2e50:   9a000008    bls 2e78 <MGC_DrcBsr+0x25c>
    2e54:   e51b301c    ldr r3, [fp, #-28]
    2e58:   e593300c    ldr r3, [r3, #12]
    2e5c:   e5932028    ldr r2, [r3, #40]
    2e60:   e51b301c    ldr r3, [fp, #-28]
    2e64:   e593300c    ldr r3, [r3, #12]
    2e68:   e5933004    ldr r3, [r3, #4]
    2e6c:   e1a00003    mov r0, r3
    2e70:   e59f17e0    ldr r1, [pc, #2016] ; 3658 <MGC_DrcBsr+0xa3c>
    2e74:   e12fff32    blx r2
    2e78:   e51b3020    ldr r3, [fp, #-32]
    2e7c:   e5d3304c    ldrb    r3, [r3, #76]
    2e80:   e3530000    cmp r3, #0  ; 0x0
    2e84:   1a0001e1    bne 3610 <MGC_DrcBsr+0x9f4>
    2e88:   e3a00022    mov r0, #34 ; 0x22
    2e8c:   e51b1020    ldr r1, [fp, #-32]
    2e90:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2e90: R_ARM_PC24    MGC_DrcChangeOtgState
    2e94:   e51b2020    ldr r2, [fp, #-32]
    2e98:   e3a03001    mov r3, #1  ; 0x1
    2e9c:   e5c23029    strb    r3, [r2, #41]
    2ea0:   e51b3020    ldr r3, [fp, #-32]
    2ea4:   e593321c    ldr r3, [r3, #540]
    2ea8:   e51b0020    ldr r0, [fp, #-32]
    2eac:   e12fff33    blx r3
    2eb0:   ea0001d6    b   3610 <MGC_DrcBsr+0x9f4>
    2eb4:   e59f3790    ldr r3, [pc, #1936] ; 364c <MGC_DrcBsr+0xa30>
    2eb8:   e5d33000    ldrb    r3, [r3]
    2ebc:   e3530001    cmp r3, #1  ; 0x1
    2ec0:   9a000008    bls 2ee8 <MGC_DrcBsr+0x2cc>
    2ec4:   e51b301c    ldr r3, [fp, #-28]
    2ec8:   e593300c    ldr r3, [r3, #12]
    2ecc:   e5932028    ldr r2, [r3, #40]
    2ed0:   e51b301c    ldr r3, [fp, #-28]
    2ed4:   e593300c    ldr r3, [r3, #12]
    2ed8:   e5933004    ldr r3, [r3, #4]
    2edc:   e1a00003    mov r0, r3
    2ee0:   e59f1774    ldr r1, [pc, #1908] ; 365c <MGC_DrcBsr+0xa40>
    2ee4:   e12fff32    blx r2
    2ee8:   e51b3020    ldr r3, [fp, #-32]
    2eec:   e5d33045    ldrb    r3, [r3, #69]
    2ef0:   e3530000    cmp r3, #0  ; 0x0
    2ef4:   0a000019    beq 2f60 <MGC_DrcBsr+0x344>
    2ef8:   e51b3020    ldr r3, [fp, #-32]
    2efc:   e5d33045    ldrb    r3, [r3, #69]
    2f00:   e2433001    sub r3, r3, #1  ; 0x1
    2f04:   e6ef3073    uxtb    r3, r3
    2f08:   e51b2020    ldr r2, [fp, #-32]
    2f0c:   e5c23045    strb    r3, [r2, #69]
    2f10:   e59f3734    ldr r3, [pc, #1844] ; 364c <MGC_DrcBsr+0xa30>
    2f14:   e5d33000    ldrb    r3, [r3]
    2f18:   e3530001    cmp r3, #1  ; 0x1
    2f1c:   9a000008    bls 2f44 <MGC_DrcBsr+0x328>
    2f20:   e51b301c    ldr r3, [fp, #-28]
    2f24:   e593300c    ldr r3, [r3, #12]
    2f28:   e5932028    ldr r2, [r3, #40]
    2f2c:   e51b301c    ldr r3, [fp, #-28]
    2f30:   e593300c    ldr r3, [r3, #12]
    2f34:   e5933004    ldr r3, [r3, #4]
    2f38:   e1a00003    mov r0, r3
    2f3c:   e59f171c    ldr r1, [pc, #1820] ; 3660 <MGC_DrcBsr+0xa44>
    2f40:   e12fff32    blx r2
    2f44:   e3a00000    mov r0, #0  ; 0x0
    2f48:   e51b1020    ldr r1, [fp, #-32]
    2f4c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2f4c: R_ARM_PC24    MGC_DrcChangeOtgState
    2f50:   e51b0020    ldr r0, [fp, #-32]
    2f54:   e3a01000    mov r1, #0  ; 0x0
    2f58:   eb0003de    bl  3ed8 <MGC_OtgStateGetId>
    2f5c:   ea0001ab    b   3610 <MGC_DrcBsr+0x9f4>
    2f60:   e51b3014    ldr r3, [fp, #-20]
    2f64:   e3530000    cmp r3, #0  ; 0x0
    2f68:   0a000007    beq 2f8c <MGC_DrcBsr+0x370>
    2f6c:   e51b3014    ldr r3, [fp, #-20]
    2f70:   e593c00c    ldr ip, [r3, #12]
    2f74:   e51b3014    ldr r3, [fp, #-20]
    2f78:   e5933000    ldr r3, [r3]
    2f7c:   e1a00003    mov r0, r3
    2f80:   e51b1014    ldr r1, [fp, #-20]
    2f84:   e3a020b0    mov r2, #176    ; 0xb0
    2f88:   e12fff3c    blx ip
    2f8c:   e3a00000    mov r0, #0  ; 0x0
    2f90:   e51b1020    ldr r1, [fp, #-32]
    2f94:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            2f94: R_ARM_PC24    MGC_DrcChangeOtgState
    2f98:   ea00019c    b   3610 <MGC_DrcBsr+0x9f4>
    2f9c:   e59f36a8    ldr r3, [pc, #1704] ; 364c <MGC_DrcBsr+0xa30>
    2fa0:   e5d33000    ldrb    r3, [r3]
    2fa4:   e3530001    cmp r3, #1  ; 0x1
    2fa8:   9a000008    bls 2fd0 <MGC_DrcBsr+0x3b4>
    2fac:   e51b301c    ldr r3, [fp, #-28]
    2fb0:   e593300c    ldr r3, [r3, #12]
    2fb4:   e5932028    ldr r2, [r3, #40]
    2fb8:   e51b301c    ldr r3, [fp, #-28]
    2fbc:   e593300c    ldr r3, [r3, #12]
    2fc0:   e5933004    ldr r3, [r3, #4]
    2fc4:   e1a00003    mov r0, r3
    2fc8:   e59f1694    ldr r1, [pc, #1684] ; 3664 <MGC_DrcBsr+0xa48>
    2fcc:   e12fff32    blx r2
    2fd0:   e51b3020    ldr r3, [fp, #-32]
    2fd4:   e59331e0    ldr r3, [r3, #480]
    2fd8:   e3530000    cmp r3, #0  ; 0x0
    2fdc:   0a000009    beq 3008 <MGC_DrcBsr+0x3ec>
    2fe0:   e51b3020    ldr r3, [fp, #-32]
    2fe4:   e59331e0    ldr r3, [r3, #480]
    2fe8:   e593c028    ldr ip, [r3, #40]
    2fec:   e51b3020    ldr r3, [fp, #-32]
    2ff0:   e59331e0    ldr r3, [r3, #480]
    2ff4:   e5933000    ldr r3, [r3]
    2ff8:   e1a00003    mov r0, r3
    2ffc:   e51b1020    ldr r1, [fp, #-32]
    3000:   e3a02005    mov r2, #5  ; 0x5
    3004:   e12fff3c    blx ip
    3008:   e51b3020    ldr r3, [fp, #-32]
    300c:   e5d3304c    ldrb    r3, [r3, #76]
    3010:   e50b3078    str r3, [fp, #-120]
    3014:   e51b2078    ldr r2, [fp, #-120]
    3018:   e3520021    cmp r2, #33 ; 0x21
    301c:   0a000018    beq 3084 <MGC_DrcBsr+0x468>
    3020:   e51b3078    ldr r3, [fp, #-120]
    3024:   e3530023    cmp r3, #35 ; 0x23
    3028:   0a00000e    beq 3068 <MGC_DrcBsr+0x44c>
    302c:   e51b0078    ldr r0, [fp, #-120]
    3030:   e3500011    cmp r0, #17 ; 0x11
    3034:   0a000000    beq 303c <MGC_DrcBsr+0x420>
    3038:   ea000174    b   3610 <MGC_DrcBsr+0x9f4>
    303c:   e51b2020    ldr r2, [fp, #-32]
    3040:   e3a03001    mov r3, #1  ; 0x1
    3044:   e5c2303b    strb    r3, [r2, #59]
    3048:   e51b3020    ldr r3, [fp, #-32]
    304c:   e5d3303e    ldrb    r3, [r3, #62]
    3050:   e3530000    cmp r3, #0  ; 0x0
    3054:   0a00016d    beq 3610 <MGC_DrcBsr+0x9f4>
    3058:   e3a00012    mov r0, #18 ; 0x12
    305c:   e51b1020    ldr r1, [fp, #-32]
    3060:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3060: R_ARM_PC24    MGC_DrcChangeOtgState
    3064:   ea000169    b   3610 <MGC_DrcBsr+0x9f4>
    3068:   e51b3020    ldr r3, [fp, #-32]
    306c:   e3a02001    mov r2, #1  ; 0x1
    3070:   e5c3203b    strb    r2, [r3, #59]
    3074:   e3a00024    mov r0, #36 ; 0x24
    3078:   e51b1020    ldr r1, [fp, #-32]
    307c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            307c: R_ARM_PC24    MGC_DrcChangeOtgState
    3080:   ea000162    b   3610 <MGC_DrcBsr+0x9f4>
    3084:   e3a00022    mov r0, #34 ; 0x22
    3088:   e51b1020    ldr r1, [fp, #-32]
    308c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            308c: R_ARM_PC24    MGC_DrcChangeOtgState
    3090:   ea00015e    b   3610 <MGC_DrcBsr+0x9f4>
    3094:   e59f35b0    ldr r3, [pc, #1456] ; 364c <MGC_DrcBsr+0xa30>
    3098:   e5d33000    ldrb    r3, [r3]
    309c:   e3530001    cmp r3, #1  ; 0x1
    30a0:   9a000008    bls 30c8 <MGC_DrcBsr+0x4ac>
    30a4:   e51b301c    ldr r3, [fp, #-28]
    30a8:   e593300c    ldr r3, [r3, #12]
    30ac:   e5932028    ldr r2, [r3, #40]
    30b0:   e51b301c    ldr r3, [fp, #-28]
    30b4:   e593300c    ldr r3, [r3, #12]
    30b8:   e5933004    ldr r3, [r3, #4]
    30bc:   e1a00003    mov r0, r3
    30c0:   e59f15a0    ldr r1, [pc, #1440] ; 3668 <MGC_DrcBsr+0xa4c>
    30c4:   e12fff32    blx r2
    30c8:   e51b3020    ldr r3, [fp, #-32]
    30cc:   e5933218    ldr r3, [r3, #536]
    30d0:   e51b0020    ldr r0, [fp, #-32]
    30d4:   e12fff33    blx r3
    30d8:   e51b2020    ldr r2, [fp, #-32]
    30dc:   e3a03000    mov r3, #0  ; 0x0
    30e0:   e58231b8    str r3, [r2, #440]
    30e4:   e51b3020    ldr r3, [fp, #-32]
    30e8:   e5d3304c    ldrb    r3, [r3, #76]
    30ec:   e3530012    cmp r3, #18 ; 0x12
    30f0:   0a000003    beq 3104 <MGC_DrcBsr+0x4e8>
    30f4:   e51b3020    ldr r3, [fp, #-32]
    30f8:   e5d3304c    ldrb    r3, [r3, #76]
    30fc:   e3530011    cmp r3, #17 ; 0x11
    3100:   1a000003    bne 3114 <MGC_DrcBsr+0x4f8>
    3104:   e3a00013    mov r0, #19 ; 0x13
    3108:   e51b1020    ldr r1, [fp, #-32]
    310c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            310c: R_ARM_PC24    MGC_DrcChangeOtgState
    3110:   ea00013e    b   3610 <MGC_DrcBsr+0x9f4>
    3114:   e3a00023    mov r0, #35 ; 0x23
    3118:   e51b1020    ldr r1, [fp, #-32]
    311c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            311c: R_ARM_PC24    MGC_DrcChangeOtgState
    3120:   ea00013a    b   3610 <MGC_DrcBsr+0x9f4>
    3124:   e59f3520    ldr r3, [pc, #1312] ; 364c <MGC_DrcBsr+0xa30>
    3128:   e5d33000    ldrb    r3, [r3]
    312c:   e3530001    cmp r3, #1  ; 0x1
    3130:   9a000008    bls 3158 <MGC_DrcBsr+0x53c>
    3134:   e51b301c    ldr r3, [fp, #-28]
    3138:   e593300c    ldr r3, [r3, #12]
    313c:   e5932028    ldr r2, [r3, #40]
    3140:   e51b301c    ldr r3, [fp, #-28]
    3144:   e593300c    ldr r3, [r3, #12]
    3148:   e5933004    ldr r3, [r3, #4]
    314c:   e1a00003    mov r0, r3
    3150:   e59f1514    ldr r1, [pc, #1300] ; 366c <MGC_DrcBsr+0xa50>
    3154:   e12fff32    blx r2
    3158:   e51b2020    ldr r2, [fp, #-32]
    315c:   e3a03000    mov r3, #0  ; 0x0
    3160:   e5c2303b    strb    r3, [r2, #59]
    3164:   e51b0020    ldr r0, [fp, #-32]
    3168:   ebfffffe    bl  0 <MGC_HostDestroy>
            3168: R_ARM_PC24    MGC_HostDestroy
    316c:   e51b3020    ldr r3, [fp, #-32]
    3170:   e5d3304c    ldrb    r3, [r3, #76]
    3174:   e50b3074    str r3, [fp, #-116]
    3178:   e51b1074    ldr r1, [fp, #-116]
    317c:   e3510023    cmp r1, #35 ; 0x23
    3180:   0a000006    beq 31a0 <MGC_DrcBsr+0x584>
    3184:   e51b2074    ldr r2, [fp, #-116]
    3188:   e3520024    cmp r2, #36 ; 0x24
    318c:   0a00000b    beq 31c0 <MGC_DrcBsr+0x5a4>
    3190:   e51b3074    ldr r3, [fp, #-116]
    3194:   e3530013    cmp r3, #19 ; 0x13
    3198:   0a00001c    beq 3210 <MGC_DrcBsr+0x5f4>
    319c:   ea000022    b   322c <MGC_DrcBsr+0x610>
    31a0:   e51b3020    ldr r3, [fp, #-32]
    31a4:   e59331b8    ldr r3, [r3, #440]
    31a8:   e1a00003    mov r0, r3
    31ac:   ebfffffe    bl  0 <MUSB_DeviceDisconnected>
            31ac: R_ARM_PC24    MUSB_DeviceDisconnected
    31b0:   e3a00022    mov r0, #34 ; 0x22
    31b4:   e51b1020    ldr r1, [fp, #-32]
    31b8:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            31b8: R_ARM_PC24    MGC_DrcChangeOtgState
    31bc:   ea00001a    b   322c <MGC_DrcBsr+0x610>
    31c0:   e51b3020    ldr r3, [fp, #-32]
    31c4:   e5d3304d    ldrb    r3, [r3, #77]
    31c8:   e3530000    cmp r3, #0  ; 0x0
    31cc:   0a00000b    beq 3200 <MGC_DrcBsr+0x5e4>
    31d0:   e59f3498    ldr r3, [pc, #1176] ; 3670 <MGC_DrcBsr+0xa54>
    31d4:   e5933014    ldr r3, [r3, #20]
    31d8:   e3530000    cmp r3, #0  ; 0x0
    31dc:   1a000007    bne 3200 <MGC_DrcBsr+0x5e4>
    31e0:   e51b3020    ldr r3, [fp, #-32]
    31e4:   e59331b8    ldr r3, [r3, #440]
    31e8:   e1a00003    mov r0, r3
    31ec:   ebfffffe    bl  0 <MUSB_DeviceDisconnected>
            31ec: R_ARM_PC24    MUSB_DeviceDisconnected
    31f0:   e3a00021    mov r0, #33 ; 0x21
    31f4:   e51b1020    ldr r1, [fp, #-32]
    31f8:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            31f8: R_ARM_PC24    MGC_DrcChangeOtgState
    31fc:   ea00000a    b   322c <MGC_DrcBsr+0x610>
    3200:   e3a00022    mov r0, #34 ; 0x22
    3204:   e51b1020    ldr r1, [fp, #-32]
    3208:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3208: R_ARM_PC24    MGC_DrcChangeOtgState
    320c:   ea000006    b   322c <MGC_DrcBsr+0x610>
    3210:   e51b3020    ldr r3, [fp, #-32]
    3214:   e59331b8    ldr r3, [r3, #440]
    3218:   e1a00003    mov r0, r3
    321c:   ebfffffe    bl  0 <MUSB_DeviceDisconnected>
            321c: R_ARM_PC24    MUSB_DeviceDisconnected
    3220:   e3a00011    mov r0, #17 ; 0x11
    3224:   e51b1020    ldr r1, [fp, #-32]
    3228:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3228: R_ARM_PC24    MGC_DrcChangeOtgState
    322c:   e51b2020    ldr r2, [fp, #-32]
    3230:   e3a03000    mov r3, #0  ; 0x0
    3234:   e58231b8    str r3, [r2, #440]
    3238:   e24b3068    sub r3, fp, #104    ; 0x68
    323c:   e3a00080    mov r0, #128    ; 0x80
    3240:   e1a01003    mov r1, r3
    3244:   e3a0200a    mov r2, #10 ; 0xa
    3248:   ebfffffe    bl  0 <FAPI_DMA_GetRequestList>
            3248: R_ARM_PC24    FAPI_DMA_GetRequestList
    324c:   e1a03000    mov r3, r0
    3250:   e54b300e    strb    r3, [fp, #-14]
    3254:   e3a03000    mov r3, #0  ; 0x0
    3258:   e54b300d    strb    r3, [fp, #-13]
    325c:   ea000028    b   3304 <MGC_DrcBsr+0x6e8>
    3260:   e55b300d    ldrb    r3, [fp, #-13]
    3264:   e3e0205b    mvn r2, #91 ; 0x5b
    3268:   e1a03103    lsl r3, r3, #2
    326c:   e24b000c    sub r0, fp, #12 ; 0xc
    3270:   e0833000    add r3, r3, r0
    3274:   e0833002    add r3, r3, r2
    3278:   e5933000    ldr r3, [r3]
    327c:   e1a00003    mov r0, r3
    3280:   ebfffffe    bl  0 <FAPI_DMA_GetRequestStatus>
            3280: R_ARM_PC24    FAPI_DMA_GetRequestStatus
    3284:   e1a03000    mov r3, r0
    3288:   e3530004    cmp r3, #4  ; 0x4
    328c:   1a000019    bne 32f8 <MGC_DrcBsr+0x6dc>
    3290:   e59f33dc    ldr r3, [pc, #988]  ; 3674 <MGC_DrcBsr+0xa58>
    3294:   e5933050    ldr r3, [r3, #80]
    3298:   e3530000    cmp r3, #0  ; 0x0
    329c:   0a000003    beq 32b0 <MGC_DrcBsr+0x694>
    32a0:   e59f33cc    ldr r3, [pc, #972]  ; 3674 <MGC_DrcBsr+0xa58>
    32a4:   e5933050    ldr r3, [r3, #80]
    32a8:   e59f03c8    ldr r0, [pc, #968]  ; 3678 <MGC_DrcBsr+0xa5c>
    32ac:   e12fff33    blx r3
    32b0:   e55b300d    ldrb    r3, [fp, #-13]
    32b4:   e3e0205b    mvn r2, #91 ; 0x5b
    32b8:   e1a03103    lsl r3, r3, #2
    32bc:   e24b100c    sub r1, fp, #12 ; 0xc
    32c0:   e0833001    add r3, r3, r1
    32c4:   e0833002    add r3, r3, r2
    32c8:   e5931000    ldr r1, [r3]
    32cc:   e55b300d    ldrb    r3, [fp, #-13]
    32d0:   e3e0205b    mvn r2, #91 ; 0x5b
    32d4:   e1a03103    lsl r3, r3, #2
    32d8:   e24b000c    sub r0, fp, #12 ; 0xc
    32dc:   e0833000    add r3, r3, r0
    32e0:   e0833002    add r3, r3, r2
    32e4:   e5933000    ldr r3, [r3]
    32e8:   e5933000    ldr r3, [r3]
    32ec:   e1a00001    mov r0, r1
    32f0:   e1a01003    mov r1, r3
    32f4:   ebfffffe    bl  0 <FAPI_DMA_CancelRequest>
            32f4: R_ARM_PC24    FAPI_DMA_CancelRequest
    32f8:   e55b300d    ldrb    r3, [fp, #-13]
    32fc:   e2833001    add r3, r3, #1  ; 0x1
    3300:   e54b300d    strb    r3, [fp, #-13]
    3304:   e55b200d    ldrb    r2, [fp, #-13]
    3308:   e55b300e    ldrb    r3, [fp, #-14]
    330c:   e1520003    cmp r2, r3
    3310:   3affffd2    bcc 3260 <MGC_DrcBsr+0x644>
    3314:   ea0000bd    b   3610 <MGC_DrcBsr+0x9f4>
    3318:   e59f332c    ldr r3, [pc, #812]  ; 364c <MGC_DrcBsr+0xa30>
    331c:   e5d33000    ldrb    r3, [r3]
    3320:   e3530001    cmp r3, #1  ; 0x1
    3324:   9a000008    bls 334c <MGC_DrcBsr+0x730>
    3328:   e51b301c    ldr r3, [fp, #-28]
    332c:   e593300c    ldr r3, [r3, #12]
    3330:   e5932028    ldr r2, [r3, #40]
    3334:   e51b301c    ldr r3, [fp, #-28]
    3338:   e593300c    ldr r3, [r3, #12]
    333c:   e5933004    ldr r3, [r3, #4]
    3340:   e1a00003    mov r0, r3
    3344:   e59f1330    ldr r1, [pc, #816]  ; 367c <MGC_DrcBsr+0xa60>
    3348:   e12fff32    blx r2
    334c:   e51b3020    ldr r3, [fp, #-32]
    3350:   e5933218    ldr r3, [r3, #536]
    3354:   e51b0020    ldr r0, [fp, #-32]
    3358:   e12fff33    blx r3
    335c:   e51b3020    ldr r3, [fp, #-32]
    3360:   e5d3303d    ldrb    r3, [r3, #61]
    3364:   e3530000    cmp r3, #0  ; 0x0
    3368:   1a0000a8    bne 3610 <MGC_DrcBsr+0x9f4>
    336c:   e51b3020    ldr r3, [fp, #-32]
    3370:   e5d33047    ldrb    r3, [r3, #71]
    3374:   e3530000    cmp r3, #0  ; 0x0
    3378:   0a000019    beq 33e4 <MGC_DrcBsr+0x7c8>
    337c:   e51b3020    ldr r3, [fp, #-32]
    3380:   e5d33047    ldrb    r3, [r3, #71]
    3384:   e2433001    sub r3, r3, #1  ; 0x1
    3388:   e6ef3073    uxtb    r3, r3
    338c:   e51b2020    ldr r2, [fp, #-32]
    3390:   e5c23047    strb    r3, [r2, #71]
    3394:   e59f32b0    ldr r3, [pc, #688]  ; 364c <MGC_DrcBsr+0xa30>
    3398:   e5d33000    ldrb    r3, [r3]
    339c:   e3530001    cmp r3, #1  ; 0x1
    33a0:   9a000008    bls 33c8 <MGC_DrcBsr+0x7ac>
    33a4:   e51b301c    ldr r3, [fp, #-28]
    33a8:   e593300c    ldr r3, [r3, #12]
    33ac:   e5932028    ldr r2, [r3, #40]
    33b0:   e51b301c    ldr r3, [fp, #-28]
    33b4:   e593300c    ldr r3, [r3, #12]
    33b8:   e5933004    ldr r3, [r3, #4]
    33bc:   e1a00003    mov r0, r3
    33c0:   e59f12b8    ldr r1, [pc, #696]  ; 3680 <MGC_DrcBsr+0xa64>
    33c4:   e12fff32    blx r2
    33c8:   e3a00000    mov r0, #0  ; 0x0
    33cc:   e51b1020    ldr r1, [fp, #-32]
    33d0:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            33d0: R_ARM_PC24    MGC_DrcChangeOtgState
    33d4:   e51b0020    ldr r0, [fp, #-32]
    33d8:   e3a01000    mov r1, #0  ; 0x0
    33dc:   eb0002bd    bl  3ed8 <MGC_OtgStateGetId>
    33e0:   ea00008a    b   3610 <MGC_DrcBsr+0x9f4>
    33e4:   e59f3260    ldr r3, [pc, #608]  ; 364c <MGC_DrcBsr+0xa30>
    33e8:   e5d33000    ldrb    r3, [r3]
    33ec:   e3530000    cmp r3, #0  ; 0x0
    33f0:   0a000008    beq 3418 <MGC_DrcBsr+0x7fc>
    33f4:   e51b301c    ldr r3, [fp, #-28]
    33f8:   e593300c    ldr r3, [r3, #12]
    33fc:   e5932028    ldr r2, [r3, #40]
    3400:   e51b301c    ldr r3, [fp, #-28]
    3404:   e593300c    ldr r3, [r3, #12]
    3408:   e5933004    ldr r3, [r3, #4]
    340c:   e1a00003    mov r0, r3
    3410:   e59f126c    ldr r1, [pc, #620]  ; 3684 <MGC_DrcBsr+0xa68>
    3414:   e12fff32    blx r2
    3418:   e51b3020    ldr r3, [fp, #-32]
    341c:   e5933220    ldr r3, [r3, #544]
    3420:   e51b0020    ldr r0, [fp, #-32]
    3424:   e12fff33    blx r3
    3428:   e51b3020    ldr r3, [fp, #-32]
    342c:   e593321c    ldr r3, [r3, #540]
    3430:   e51b0020    ldr r0, [fp, #-32]
    3434:   e12fff33    blx r3
    3438:   e3a00000    mov r0, #0  ; 0x0
    343c:   e51b1020    ldr r1, [fp, #-32]
    3440:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3440: R_ARM_PC24    MGC_DrcChangeOtgState
    3444:   ea000071    b   3610 <MGC_DrcBsr+0x9f4>
    3448:   e51b3020    ldr r3, [fp, #-32]
    344c:   e283200c    add r2, r3, #12 ; 0xc
    3450:   e55b303b    ldrb    r3, [fp, #-59]
    3454:   e1a00002    mov r0, r2
    3458:   e1a01003    mov r1, r3
    345c:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            345c: R_ARM_PC24    MUSB_ArrayFetch
    3460:   e1a03000    mov r3, r0
    3464:   e50b3034    str r3, [fp, #-52]
    3468:   e51b3034    ldr r3, [fp, #-52]
    346c:   e3530000    cmp r3, #0  ; 0x0
    3470:   0a000066    beq 3610 <MGC_DrcBsr+0x9f4>
    3474:   e55b303c    ldrb    r3, [fp, #-60]
    3478:   e3530023    cmp r3, #35 ; 0x23
    347c:   1a000003    bne 3490 <MGC_DrcBsr+0x874>
    3480:   e51b3034    ldr r3, [fp, #-52]
    3484:   e5d33009    ldrb    r3, [r3, #9]
    3488:   e50b3070    str r3, [fp, #-112]
    348c:   ea000002    b   349c <MGC_DrcBsr+0x880>
    3490:   e51b3034    ldr r3, [fp, #-52]
    3494:   e5d3300d    ldrb    r3, [r3, #13]
    3498:   e50b3070    str r3, [fp, #-112]
    349c:   e51b1070    ldr r1, [fp, #-112]
    34a0:   e1a03001    mov r3, r1
    34a4:   e54b3035    strb    r3, [fp, #-53]
    34a8:   e55b3035    ldrb    r3, [fp, #-53]
    34ac:   e3530003    cmp r3, #3  ; 0x3
    34b0:   979ff103    ldrls   pc, [pc, r3, lsl #2]
    34b4:   ea000035    b   3590 <MGC_DrcBsr+0x974>
    34b8:   0000350c    .word   0x0000350c
            34b8: R_ARM_ABS32   .text
    34bc:   00003550    .word   0x00003550
            34bc: R_ARM_ABS32   .text
    34c0:   000034c8    .word   0x000034c8
            34c0: R_ARM_ABS32   .text
    34c4:   000034c8    .word   0x000034c8
            34c4: R_ARM_ABS32   .text
    34c8:   e51b3040    ldr r3, [fp, #-64]
    34cc:   e50b3030    str r3, [fp, #-48]
    34d0:   e51b3030    ldr r3, [fp, #-48]
    34d4:   e3530000    cmp r3, #0  ; 0x0
    34d8:   0a00002c    beq 3590 <MGC_DrcBsr+0x974>
    34dc:   e51b3030    ldr r3, [fp, #-48]
    34e0:   e5933014    ldr r3, [r3, #20]
    34e4:   e3530000    cmp r3, #0  ; 0x0
    34e8:   0a000028    beq 3590 <MGC_DrcBsr+0x974>
    34ec:   e51b3030    ldr r3, [fp, #-48]
    34f0:   e5932014    ldr r2, [r3, #20]
    34f4:   e51b3030    ldr r3, [fp, #-48]
    34f8:   e5933018    ldr r3, [r3, #24]
    34fc:   e1a00003    mov r0, r3
    3500:   e51b1030    ldr r1, [fp, #-48]
    3504:   e12fff32    blx r2
    3508:   ea000020    b   3590 <MGC_DrcBsr+0x974>
    350c:   e51b3040    ldr r3, [fp, #-64]
    3510:   e50b302c    str r3, [fp, #-44]
    3514:   e51b302c    ldr r3, [fp, #-44]
    3518:   e3530000    cmp r3, #0  ; 0x0
    351c:   0a00001b    beq 3590 <MGC_DrcBsr+0x974>
    3520:   e51b302c    ldr r3, [fp, #-44]
    3524:   e5933020    ldr r3, [r3, #32]
    3528:   e3530000    cmp r3, #0  ; 0x0
    352c:   0a000017    beq 3590 <MGC_DrcBsr+0x974>
    3530:   e51b302c    ldr r3, [fp, #-44]
    3534:   e5932020    ldr r2, [r3, #32]
    3538:   e51b302c    ldr r3, [fp, #-44]
    353c:   e5933024    ldr r3, [r3, #36]
    3540:   e1a00003    mov r0, r3
    3544:   e51b102c    ldr r1, [fp, #-44]
    3548:   e12fff32    blx r2
    354c:   ea00000f    b   3590 <MGC_DrcBsr+0x974>
    3550:   e51b3040    ldr r3, [fp, #-64]
    3554:   e50b3028    str r3, [fp, #-40]
    3558:   e51b3028    ldr r3, [fp, #-40]
    355c:   e3530000    cmp r3, #0  ; 0x0
    3560:   0a00000a    beq 3590 <MGC_DrcBsr+0x974>
    3564:   e51b3028    ldr r3, [fp, #-40]
    3568:   e5933014    ldr r3, [r3, #20]
    356c:   e3530000    cmp r3, #0  ; 0x0
    3570:   0a000006    beq 3590 <MGC_DrcBsr+0x974>
    3574:   e51b3028    ldr r3, [fp, #-40]
    3578:   e5932014    ldr r2, [r3, #20]
    357c:   e51b3028    ldr r3, [fp, #-40]
    3580:   e5933018    ldr r3, [r3, #24]
    3584:   e1a00003    mov r0, r3
    3588:   e51b1028    ldr r1, [fp, #-40]
    358c:   e12fff32    blx r2
    3590:   e55b303c    ldrb    r3, [fp, #-60]
    3594:   e3530023    cmp r3, #35 ; 0x23
    3598:   1a00000c    bne 35d0 <MGC_DrcBsr+0x9b4>
    359c:   e51b3034    ldr r3, [fp, #-52]
    35a0:   e593301c    ldr r3, [r3, #28]
    35a4:   e3530000    cmp r3, #0  ; 0x0
    35a8:   1a000018    bne 3610 <MGC_DrcBsr+0x9f4>
    35ac:   e51b3034    ldr r3, [fp, #-52]
    35b0:   e5d33012    ldrb    r3, [r3, #18]
    35b4:   e3530000    cmp r3, #0  ; 0x0
    35b8:   1a000014    bne 3610 <MGC_DrcBsr+0x9f4>
    35bc:   e51b0020    ldr r0, [fp, #-32]
    35c0:   e51b1034    ldr r1, [fp, #-52]
    35c4:   e3a02001    mov r2, #1  ; 0x1
    35c8:   ebfffffe    bl  0 <MGC_StartNextIrp>
            35c8: R_ARM_PC24    MGC_StartNextIrp
    35cc:   ea00000f    b   3610 <MGC_DrcBsr+0x9f4>
    35d0:   e51b3034    ldr r3, [fp, #-52]
    35d4:   e5933030    ldr r3, [r3, #48]
    35d8:   e3530000    cmp r3, #0  ; 0x0
    35dc:   0a000003    beq 35f0 <MGC_DrcBsr+0x9d4>
    35e0:   e51b3034    ldr r3, [fp, #-52]
    35e4:   e5d3300d    ldrb    r3, [r3, #13]
    35e8:   e3530003    cmp r3, #3  ; 0x3
    35ec:   1a000007    bne 3610 <MGC_DrcBsr+0x9f4>
    35f0:   e51b3034    ldr r3, [fp, #-52]
    35f4:   e5d33013    ldrb    r3, [r3, #19]
    35f8:   e3530000    cmp r3, #0  ; 0x0
    35fc:   1a000003    bne 3610 <MGC_DrcBsr+0x9f4>
    3600:   e51b0020    ldr r0, [fp, #-32]
    3604:   e51b1034    ldr r1, [fp, #-52]
    3608:   e3a02000    mov r2, #0  ; 0x0
    360c:   ebfffffe    bl  0 <MGC_StartNextIrp>
            360c: R_ARM_PC24    MGC_StartNextIrp
    3610:   e51b3018    ldr r3, [fp, #-24]
    3614:   e593c010    ldr ip, [r3, #16]
    3618:   e51b3018    ldr r3, [fp, #-24]
    361c:   e5933004    ldr r3, [r3, #4]
    3620:   e24b2040    sub r2, fp, #64 ; 0x40
    3624:   e1a00003    mov r0, r3
    3628:   e1a01002    mov r1, r2
    362c:   e12fff3c    blx ip
    3630:   e1a03000    mov r3, r0
    3634:   e54b3036    strb    r3, [fp, #-54]
    3638:   e55b3036    ldrb    r3, [fp, #-54]
    363c:   e3530000    cmp r3, #0  ; 0x0
    3640:   1afffd93    bne 2c94 <MGC_DrcBsr+0x78>
    3644:   e24bd00c    sub sp, fp, #12 ; 0xc
    3648:   e89da800    ldm sp, {fp, sp, pc}
    364c:   00000000    .word   0x00000000
            364c: R_ARM_ABS32   MGC_bDiagLevel
    3650:   0000018c    .word   0x0000018c
            3650: R_ARM_ABS32   .rodata
    3654:   00000000    .word   0x00000000
            3654: R_ARM_ABS32   MGC_DrcCompleteResume
    3658:   000001a0    .word   0x000001a0
            3658: R_ARM_ABS32   .rodata
    365c:   000001bc    .word   0x000001bc
            365c: R_ARM_ABS32   .rodata
    3660:   000001d4    .word   0x000001d4
            3660: R_ARM_ABS32   .rodata
    3664:   000001f0    .word   0x000001f0
            3664: R_ARM_ABS32   .rodata
    3668:   00000204    .word   0x00000204
            3668: R_ARM_ABS32   .rodata
    366c:   00000218    .word   0x00000218
            366c: R_ARM_ABS32   .rodata
    3670:   0000002c    .word   0x0000002c
            3670: R_ARM_ABS32   .data
    3674:   00000000    .word   0x00000000
            3674: R_ARM_ABS32   FAPI_SYS_Services
    3678:   00000230    .word   0x00000230
            3678: R_ARM_ABS32   .rodata
    367c:   0000025c    .word   0x0000025c
            367c: R_ARM_ABS32   .rodata
    3680:   0000026c    .word   0x0000026c
            3680: R_ARM_ABS32   .rodata
    3684:   00000284    .word   0x00000284
            3684: R_ARM_ABS32   .rodata

00003688 <MGC_DrcServiceHostDefault>:
    3688:   e1a0c00d    mov ip, sp
    368c:   e92dd830    push    {r4, r5, fp, ip, lr, pc}
    3690:   e24cb004    sub fp, ip, #4  ; 0x4
    3694:   e24dd034    sub sp, sp, #52 ; 0x34
    3698:   e50b0030    str r0, [fp, #-48]
    369c:   e50b303c    str r3, [fp, #-60]
    36a0:   e14b13b4    strh    r1, [fp, #-52]
    36a4:   e1a03002    mov r3, r2
    36a8:   e54b3038    strb    r3, [fp, #-56]
    36ac:   e3a03000    mov r3, #0  ; 0x0
    36b0:   e54b3029    strb    r3, [fp, #-41]
    36b4:   e3a03000    mov r3, #0  ; 0x0
    36b8:   e50b3028    str r3, [fp, #-40]
    36bc:   e3a03000    mov r3, #0  ; 0x0
    36c0:   e14b32b2    strh    r3, [fp, #-34]
    36c4:   e51b3030    ldr r3, [fp, #-48]
    36c8:   e283300c    add r3, r3, #12 ; 0xc
    36cc:   e1a00003    mov r0, r3
    36d0:   e3a01000    mov r1, #0  ; 0x0
    36d4:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            36d4: R_ARM_PC24    MUSB_ArrayFetch
    36d8:   e1a03000    mov r3, r0
    36dc:   e50b3020    str r3, [fp, #-32]
    36e0:   e51b3020    ldr r3, [fp, #-32]
    36e4:   e593301c    ldr r3, [r3, #28]
    36e8:   e50b301c    str r3, [fp, #-28]
    36ec:   e51b301c    ldr r3, [fp, #-28]
    36f0:   e5933004    ldr r3, [r3, #4]
    36f4:   e50b3018    str r3, [fp, #-24]
    36f8:   e55b3038    ldrb    r3, [fp, #-56]
    36fc:   e3530000    cmp r3, #0  ; 0x0
    3700:   0a000005    beq 371c <MGC_DrcServiceHostDefault+0x94>
    3704:   e55b2038    ldrb    r2, [fp, #-56]
    3708:   e51b301c    ldr r3, [fp, #-28]
    370c:   e5832014    str r2, [r3, #20]
    3710:   e3a01000    mov r1, #0  ; 0x0
    3714:   e50b1044    str r1, [fp, #-68]
    3718:   ea00010f    b   3b5c <MGC_DrcServiceHostDefault+0x4d4>
    371c:   e51b3020    ldr r3, [fp, #-32]
    3720:   e5d3300e    ldrb    r3, [r3, #14]
    3724:   e3530000    cmp r3, #0  ; 0x0
    3728:   1a00004f    bne 386c <MGC_DrcServiceHostDefault+0x1e4>
    372c:   e51b301c    ldr r3, [fp, #-28]
    3730:   e593200c    ldr r2, [r3, #12]
    3734:   e51b301c    ldr r3, [fp, #-28]
    3738:   e593301c    ldr r3, [r3, #28]
    373c:   e0823003    add r3, r2, r3
    3740:   e50b3028    str r3, [fp, #-40]
    3744:   e51b301c    ldr r3, [fp, #-28]
    3748:   e5932010    ldr r2, [r3, #16]
    374c:   e51b301c    ldr r3, [fp, #-28]
    3750:   e593301c    ldr r3, [r3, #28]
    3754:   e0632002    rsb r2, r3, r2
    3758:   e15b33b4    ldrh    r3, [fp, #-52]
    375c:   e1520003    cmp r2, r3
    3760:   31a03002    movcc   r3, r2
    3764:   e14b32b2    strh    r3, [fp, #-34]
    3768:   e51b3020    ldr r3, [fp, #-32]
    376c:   e5933044    ldr r3, [r3, #68]
    3770:   e3530000    cmp r3, #0  ; 0x0
    3774:   0a000020    beq 37fc <MGC_DrcServiceHostDefault+0x174>
    3778:   e51b3030    ldr r3, [fp, #-48]
    377c:   e5933214    ldr r3, [r3, #532]
    3780:   e593401c    ldr r4, [r3, #28]
    3784:   e51b3020    ldr r3, [fp, #-32]
    3788:   e5931044    ldr r1, [r3, #68]
    378c:   e51b3020    ldr r3, [fp, #-32]
    3790:   e1d331b4    ldrh    r3, [r3, #20]
    3794:   e1a0e003    mov lr, r3
    3798:   e51b3020    ldr r3, [fp, #-32]
    379c:   e5933044    ldr r3, [r3, #68]
    37a0:   e5d33010    ldrb    r3, [r3, #16]
    37a4:   e1a0c003    mov ip, r3
    37a8:   e15b22b2    ldrh    r2, [fp, #-34]
    37ac:   e51b3020    ldr r3, [fp, #-32]
    37b0:   e5933044    ldr r3, [r3, #68]
    37b4:   e5933004    ldr r3, [r3, #4]
    37b8:   e1520003    cmp r2, r3
    37bc:   31a03002    movcc   r3, r2
    37c0:   e58d3000    str r3, [sp]
    37c4:   e1a00001    mov r0, r1
    37c8:   e1a0100e    mov r1, lr
    37cc:   e1a0200c    mov r2, ip
    37d0:   e51b3028    ldr r3, [fp, #-40]
    37d4:   e12fff34    blx r4
    37d8:   e1a03000    mov r3, r0
    37dc:   e3530000    cmp r3, #0  ; 0x0
    37e0:   0a000005    beq 37fc <MGC_DrcServiceHostDefault+0x174>
    37e4:   e51b303c    ldr r3, [fp, #-60]
    37e8:   e3a02000    mov r2, #0  ; 0x0
    37ec:   e5c32000    strb    r2, [r3]
    37f0:   e3a03001    mov r3, #1  ; 0x1
    37f4:   e50b3044    str r3, [fp, #-68]
    37f8:   ea0000d7    b   3b5c <MGC_DrcServiceHostDefault+0x4d4>
    37fc:   e51b3030    ldr r3, [fp, #-48]
    3800:   e593c24c    ldr ip, [r3, #588]
    3804:   e15b32b2    ldrh    r3, [fp, #-34]
    3808:   e51b0030    ldr r0, [fp, #-48]
    380c:   e3a01000    mov r1, #0  ; 0x0
    3810:   e1a02003    mov r2, r3
    3814:   e51b3028    ldr r3, [fp, #-40]
    3818:   e12fff3c    blx ip
    381c:   e51b301c    ldr r3, [fp, #-28]
    3820:   e593201c    ldr r2, [r3, #28]
    3824:   e15b32b2    ldrh    r3, [fp, #-34]
    3828:   e0822003    add r2, r2, r3
    382c:   e51b301c    ldr r3, [fp, #-28]
    3830:   e583201c    str r2, [r3, #28]
    3834:   e51b301c    ldr r3, [fp, #-28]
    3838:   e593201c    ldr r2, [r3, #28]
    383c:   e51b301c    ldr r3, [fp, #-28]
    3840:   e5933010    ldr r3, [r3, #16]
    3844:   e1520003    cmp r2, r3
    3848:   2a0000ad    bcs 3b04 <MGC_DrcServiceHostDefault+0x47c>
    384c:   e51b3020    ldr r3, [fp, #-32]
    3850:   e1d321b4    ldrh    r2, [r3, #20]
    3854:   e15b33b4    ldrh    r3, [fp, #-52]
    3858:   e1530002    cmp r3, r2
    385c:   1a0000a8    bne 3b04 <MGC_DrcServiceHostDefault+0x47c>
    3860:   e3a03001    mov r3, #1  ; 0x1
    3864:   e54b3029    strb    r3, [fp, #-41]
    3868:   ea0000a5    b   3b04 <MGC_DrcServiceHostDefault+0x47c>
    386c:   e51b301c    ldr r3, [fp, #-28]
    3870:   e5932018    ldr r2, [r3, #24]
    3874:   e51b3020    ldr r3, [fp, #-32]
    3878:   e5933018    ldr r3, [r3, #24]
    387c:   e0822003    add r2, r2, r3
    3880:   e51b301c    ldr r3, [fp, #-28]
    3884:   e5832018    str r2, [r3, #24]
    3888:   e51b2020    ldr r2, [fp, #-32]
    388c:   e3a03000    mov r3, #0  ; 0x0
    3890:   e5823018    str r3, [r2, #24]
    3894:   e51b3018    ldr r3, [fp, #-24]
    3898:   e5d33000    ldrb    r3, [r3]
    389c:   e6af3073    sxtb    r3, r3
    38a0:   e3530000    cmp r3, #0  ; 0x0
    38a4:   aa000051    bge 39f0 <MGC_DrcServiceHostDefault+0x368>
    38a8:   e51b2020    ldr r2, [fp, #-32]
    38ac:   e3a03000    mov r3, #0  ; 0x0
    38b0:   e5c2300e    strb    r3, [r2, #14]
    38b4:   e51b3020    ldr r3, [fp, #-32]
    38b8:   e5933044    ldr r3, [r3, #68]
    38bc:   e3530000    cmp r3, #0  ; 0x0
    38c0:   0a000047    beq 39e4 <MGC_DrcServiceHostDefault+0x35c>
    38c4:   e51b3030    ldr r3, [fp, #-48]
    38c8:   e5933214    ldr r3, [r3, #532]
    38cc:   e5932010    ldr r2, [r3, #16]
    38d0:   e51b3020    ldr r3, [fp, #-32]
    38d4:   e5933044    ldr r3, [r3, #68]
    38d8:   e1a00003    mov r0, r3
    38dc:   e12fff32    blx r2
    38e0:   e51b3030    ldr r3, [fp, #-48]
    38e4:   e5933214    ldr r3, [r3, #532]
    38e8:   e593c00c    ldr ip, [r3, #12]
    38ec:   e51b3030    ldr r3, [fp, #-48]
    38f0:   e5933214    ldr r3, [r3, #532]
    38f4:   e5932000    ldr r2, [r3]
    38f8:   e51b3020    ldr r3, [fp, #-32]
    38fc:   e1d331b4    ldrh    r3, [r3, #20]
    3900:   e58d3000    str r3, [sp]
    3904:   e1a00002    mov r0, r2
    3908:   e3a01000    mov r1, #0  ; 0x0
    390c:   e3a02000    mov r2, #0  ; 0x0
    3910:   e3a03000    mov r3, #0  ; 0x0
    3914:   e12fff3c    blx ip
    3918:   e1a02000    mov r2, r0
    391c:   e51b3020    ldr r3, [fp, #-32]
    3920:   e5832044    str r2, [r3, #68]
    3924:   e51b3020    ldr r3, [fp, #-32]
    3928:   e5933044    ldr r3, [r3, #68]
    392c:   e3530000    cmp r3, #0  ; 0x0
    3930:   0a00002b    beq 39e4 <MGC_DrcServiceHostDefault+0x35c>
    3934:   e51b3020    ldr r3, [fp, #-32]
    3938:   e5932044    ldr r2, [r3, #68]
    393c:   e3a03000    mov r3, #0  ; 0x0
    3940:   e5823008    str r3, [r2, #8]
    3944:   e51b3030    ldr r3, [fp, #-48]
    3948:   e5933214    ldr r3, [r3, #532]
    394c:   e593c01c    ldr ip, [r3, #28]
    3950:   e51b3020    ldr r3, [fp, #-32]
    3954:   e5931044    ldr r1, [r3, #68]
    3958:   e51b3020    ldr r3, [fp, #-32]
    395c:   e1d331b4    ldrh    r3, [r3, #20]
    3960:   e1a0e003    mov lr, r3
    3964:   e51b3020    ldr r3, [fp, #-32]
    3968:   e5933044    ldr r3, [r3, #68]
    396c:   e5d33010    ldrb    r3, [r3, #16]
    3970:   e1a04003    mov r4, r3
    3974:   e51b301c    ldr r3, [fp, #-28]
    3978:   e593500c    ldr r5, [r3, #12]
    397c:   e51b301c    ldr r3, [fp, #-28]
    3980:   e5932010    ldr r2, [r3, #16]
    3984:   e51b3020    ldr r3, [fp, #-32]
    3988:   e5933044    ldr r3, [r3, #68]
    398c:   e5933004    ldr r3, [r3, #4]
    3990:   e1520003    cmp r2, r3
    3994:   31a03002    movcc   r3, r2
    3998:   e58d3000    str r3, [sp]
    399c:   e1a00001    mov r0, r1
    39a0:   e1a0100e    mov r1, lr
    39a4:   e1a02004    mov r2, r4
    39a8:   e1a03005    mov r3, r5
    39ac:   e12fff3c    blx ip
    39b0:   e1a03000    mov r3, r0
    39b4:   e3530000    cmp r3, #0  ; 0x0
    39b8:   1a000009    bne 39e4 <MGC_DrcServiceHostDefault+0x35c>
    39bc:   e51b3030    ldr r3, [fp, #-48]
    39c0:   e5933214    ldr r3, [r3, #532]
    39c4:   e5932010    ldr r2, [r3, #16]
    39c8:   e51b3020    ldr r3, [fp, #-32]
    39cc:   e5933044    ldr r3, [r3, #68]
    39d0:   e1a00003    mov r0, r3
    39d4:   e12fff32    blx r2
    39d8:   e51b2020    ldr r2, [fp, #-32]
    39dc:   e3a03000    mov r3, #0  ; 0x0
    39e0:   e5823044    str r3, [r2, #68]
    39e4:   e3a03001    mov r3, #1  ; 0x1
    39e8:   e54b3029    strb    r3, [fp, #-41]
    39ec:   ea000044    b   3b04 <MGC_DrcServiceHostDefault+0x47c>
    39f0:   e51b301c    ldr r3, [fp, #-28]
    39f4:   e5932004    ldr r2, [r3, #4]
    39f8:   e51b301c    ldr r3, [fp, #-28]
    39fc:   e5933018    ldr r3, [r3, #24]
    3a00:   e0823003    add r3, r2, r3
    3a04:   e50b3028    str r3, [fp, #-40]
    3a08:   e51b301c    ldr r3, [fp, #-28]
    3a0c:   e5932008    ldr r2, [r3, #8]
    3a10:   e51b301c    ldr r3, [fp, #-28]
    3a14:   e5933018    ldr r3, [r3, #24]
    3a18:   e0632002    rsb r2, r3, r2
    3a1c:   e51b3020    ldr r3, [fp, #-32]
    3a20:   e1d331b4    ldrh    r3, [r3, #20]
    3a24:   e1520003    cmp r2, r3
    3a28:   31a03002    movcc   r3, r2
    3a2c:   e14b32b2    strh    r3, [fp, #-34]
    3a30:   e51b3020    ldr r3, [fp, #-32]
    3a34:   e5933044    ldr r3, [r3, #68]
    3a38:   e3530000    cmp r3, #0  ; 0x0
    3a3c:   0a000020    beq 3ac4 <MGC_DrcServiceHostDefault+0x43c>
    3a40:   e51b3030    ldr r3, [fp, #-48]
    3a44:   e5933214    ldr r3, [r3, #532]
    3a48:   e593401c    ldr r4, [r3, #28]
    3a4c:   e51b3020    ldr r3, [fp, #-32]
    3a50:   e5931044    ldr r1, [r3, #68]
    3a54:   e51b3020    ldr r3, [fp, #-32]
    3a58:   e1d331b4    ldrh    r3, [r3, #20]
    3a5c:   e1a0e003    mov lr, r3
    3a60:   e51b3020    ldr r3, [fp, #-32]
    3a64:   e5933044    ldr r3, [r3, #68]
    3a68:   e5d33010    ldrb    r3, [r3, #16]
    3a6c:   e1a0c003    mov ip, r3
    3a70:   e15b22b2    ldrh    r2, [fp, #-34]
    3a74:   e51b3020    ldr r3, [fp, #-32]
    3a78:   e5933044    ldr r3, [r3, #68]
    3a7c:   e5933004    ldr r3, [r3, #4]
    3a80:   e1520003    cmp r2, r3
    3a84:   31a03002    movcc   r3, r2
    3a88:   e58d3000    str r3, [sp]
    3a8c:   e1a00001    mov r0, r1
    3a90:   e1a0100e    mov r1, lr
    3a94:   e1a0200c    mov r2, ip
    3a98:   e51b3028    ldr r3, [fp, #-40]
    3a9c:   e12fff34    blx r4
    3aa0:   e1a03000    mov r3, r0
    3aa4:   e3530000    cmp r3, #0  ; 0x0
    3aa8:   0a000005    beq 3ac4 <MGC_DrcServiceHostDefault+0x43c>
    3aac:   e51b303c    ldr r3, [fp, #-60]
    3ab0:   e3a02000    mov r2, #0  ; 0x0
    3ab4:   e5c32000    strb    r2, [r3]
    3ab8:   e3a01001    mov r1, #1  ; 0x1
    3abc:   e50b1044    str r1, [fp, #-68]
    3ac0:   ea000025    b   3b5c <MGC_DrcServiceHostDefault+0x4d4>
    3ac4:   e15b32b2    ldrh    r3, [fp, #-34]
    3ac8:   e3530000    cmp r3, #0  ; 0x0
    3acc:   0a00000c    beq 3b04 <MGC_DrcServiceHostDefault+0x47c>
    3ad0:   e51b3030    ldr r3, [fp, #-48]
    3ad4:   e593c248    ldr ip, [r3, #584]
    3ad8:   e15b32b2    ldrh    r3, [fp, #-34]
    3adc:   e51b0030    ldr r0, [fp, #-48]
    3ae0:   e3a01000    mov r1, #0  ; 0x0
    3ae4:   e1a02003    mov r2, r3
    3ae8:   e51b3028    ldr r3, [fp, #-40]
    3aec:   e12fff3c    blx ip
    3af0:   e15b22b2    ldrh    r2, [fp, #-34]
    3af4:   e51b3020    ldr r3, [fp, #-32]
    3af8:   e5832018    str r2, [r3, #24]
    3afc:   e3a03001    mov r3, #1  ; 0x1
    3b00:   e54b3029    strb    r3, [fp, #-41]
    3b04:   e55b3029    ldrb    r3, [fp, #-41]
    3b08:   e3530000    cmp r3, #0  ; 0x0
    3b0c:   0a00000d    beq 3b48 <MGC_DrcServiceHostDefault+0x4c0>
    3b10:   e51b3020    ldr r3, [fp, #-32]
    3b14:   e5d3300e    ldrb    r3, [r3, #14]
    3b18:   e3530000    cmp r3, #0  ; 0x0
    3b1c:   0a000002    beq 3b2c <MGC_DrcServiceHostDefault+0x4a4>
    3b20:   e3a03002    mov r3, #2  ; 0x2
    3b24:   e50b3040    str r3, [fp, #-64]
    3b28:   ea000001    b   3b34 <MGC_DrcServiceHostDefault+0x4ac>
    3b2c:   e3a01020    mov r1, #32 ; 0x20
    3b30:   e50b1040    str r1, [fp, #-64]
    3b34:   e51b203c    ldr r2, [fp, #-60]
    3b38:   e51b1040    ldr r1, [fp, #-64]
    3b3c:   e1a03001    mov r3, r1
    3b40:   e5c23000    strb    r3, [r2]
    3b44:   ea000002    b   3b54 <MGC_DrcServiceHostDefault+0x4cc>
    3b48:   e51b303c    ldr r3, [fp, #-60]
    3b4c:   e3a02000    mov r2, #0  ; 0x0
    3b50:   e5c32000    strb    r2, [r3]
    3b54:   e55b3029    ldrb    r3, [fp, #-41]
    3b58:   e50b3044    str r3, [fp, #-68]
    3b5c:   e51b3044    ldr r3, [fp, #-68]
    3b60:   e1a00003    mov r0, r3
    3b64:   e24bd014    sub sp, fp, #20 ; 0x14
    3b68:   e89da830    ldm sp, {r4, r5, fp, sp, pc}

00003b6c <MGC_DrcAcceptDevice>:
    3b6c:   e1a0c00d    mov ip, sp
    3b70:   e92dd800    push    {fp, ip, lr, pc}
    3b74:   e24cb004    sub fp, ip, #4  ; 0x4
    3b78:   e24dd018    sub sp, sp, #24 ; 0x18
    3b7c:   e50b001c    str r0, [fp, #-28]
    3b80:   e50b1020    str r1, [fp, #-32]
    3b84:   e50b2024    str r2, [fp, #-36]
    3b88:   e3a03001    mov r3, #1  ; 0x1
    3b8c:   e54b3011    strb    r3, [fp, #-17]
    3b90:   e51b301c    ldr r3, [fp, #-28]
    3b94:   e5933050    ldr r3, [r3, #80]
    3b98:   e50b3010    str r3, [fp, #-16]
    3b9c:   e51b3020    ldr r3, [fp, #-32]
    3ba0:   e1d321b4    ldrh    r2, [r3, #20]
    3ba4:   e59f3200    ldr r3, [pc, #512]  ; 3dac <MGC_DrcAcceptDevice+0x240>
    3ba8:   e1520003    cmp r2, r3
    3bac:   1a000011    bne 3bf8 <MGC_DrcAcceptDevice+0x8c>
    3bb0:   e51b3020    ldr r3, [fp, #-32]
    3bb4:   e1d321b6    ldrh    r2, [r3, #22]
    3bb8:   e59f31f0    ldr r3, [pc, #496]  ; 3db0 <MGC_DrcAcceptDevice+0x244>
    3bbc:   e1520003    cmp r2, r3
    3bc0:   1a00000c    bne 3bf8 <MGC_DrcAcceptDevice+0x8c>
    3bc4:   e51b3010    ldr r3, [fp, #-16]
    3bc8:   e3530000    cmp r3, #0  ; 0x0
    3bcc:   0a000007    beq 3bf0 <MGC_DrcAcceptDevice+0x84>
    3bd0:   e51b3010    ldr r3, [fp, #-16]
    3bd4:   e593c00c    ldr ip, [r3, #12]
    3bd8:   e51b3010    ldr r3, [fp, #-16]
    3bdc:   e5933000    ldr r3, [r3]
    3be0:   e1a00003    mov r0, r3
    3be4:   e51b101c    ldr r1, [fp, #-28]
    3be8:   e3a02041    mov r2, #65 ; 0x41
    3bec:   e12fff3c    blx ip
    3bf0:   e3a03000    mov r3, #0  ; 0x0
    3bf4:   e54b3011    strb    r3, [fp, #-17]
    3bf8:   e51b301c    ldr r3, [fp, #-28]
    3bfc:   e5d33032    ldrb    r3, [r3, #50]
    3c00:   e3530000    cmp r3, #0  ; 0x0
    3c04:   1a000010    bne 3c4c <MGC_DrcAcceptDevice+0xe0>
    3c08:   e51b3020    ldr r3, [fp, #-32]
    3c0c:   e5d33010    ldrb    r3, [r3, #16]
    3c10:   e3530009    cmp r3, #9  ; 0x9
    3c14:   1a00000c    bne 3c4c <MGC_DrcAcceptDevice+0xe0>
    3c18:   e51b3010    ldr r3, [fp, #-16]
    3c1c:   e3530000    cmp r3, #0  ; 0x0
    3c20:   0a000007    beq 3c44 <MGC_DrcAcceptDevice+0xd8>
    3c24:   e51b3010    ldr r3, [fp, #-16]
    3c28:   e593c00c    ldr ip, [r3, #12]
    3c2c:   e51b3010    ldr r3, [fp, #-16]
    3c30:   e5933000    ldr r3, [r3]
    3c34:   e1a00003    mov r0, r3
    3c38:   e51b101c    ldr r1, [fp, #-28]
    3c3c:   e3a02042    mov r2, #66 ; 0x42
    3c40:   e12fff3c    blx ip
    3c44:   e3a03000    mov r3, #0  ; 0x0
    3c48:   e54b3011    strb    r3, [fp, #-17]
    3c4c:   e55b3011    ldrb    r3, [fp, #-17]
    3c50:   e3530000    cmp r3, #0  ; 0x0
    3c54:   0a00000f    beq 3c98 <MGC_DrcAcceptDevice+0x12c>
    3c58:   e51b3024    ldr r3, [fp, #-36]
    3c5c:   e3530000    cmp r3, #0  ; 0x0
    3c60:   1a00000c    bne 3c98 <MGC_DrcAcceptDevice+0x12c>
    3c64:   e51b3010    ldr r3, [fp, #-16]
    3c68:   e3530000    cmp r3, #0  ; 0x0
    3c6c:   0a000007    beq 3c90 <MGC_DrcAcceptDevice+0x124>
    3c70:   e51b3010    ldr r3, [fp, #-16]
    3c74:   e593c00c    ldr ip, [r3, #12]
    3c78:   e51b3010    ldr r3, [fp, #-16]
    3c7c:   e5933000    ldr r3, [r3]
    3c80:   e1a00003    mov r0, r3
    3c84:   e51b101c    ldr r1, [fp, #-28]
    3c88:   e3a02041    mov r2, #65 ; 0x41
    3c8c:   e12fff3c    blx ip
    3c90:   e3a03000    mov r3, #0  ; 0x0
    3c94:   e54b3011    strb    r3, [fp, #-17]
    3c98:   e55b3011    ldrb    r3, [fp, #-17]
    3c9c:   e3530000    cmp r3, #0  ; 0x0
    3ca0:   0a000037    beq 3d84 <MGC_DrcAcceptDevice+0x218>
    3ca4:   e51b301c    ldr r3, [fp, #-28]
    3ca8:   e5d3303f    ldrb    r3, [r3, #63]
    3cac:   e3530000    cmp r3, #0  ; 0x0
    3cb0:   1a000039    bne 3d9c <MGC_DrcAcceptDevice+0x230>
    3cb4:   e51b301c    ldr r3, [fp, #-28]
    3cb8:   e59331b8    ldr r3, [r3, #440]
    3cbc:   e3530000    cmp r3, #0  ; 0x0
    3cc0:   0a000035    beq 3d9c <MGC_DrcAcceptDevice+0x230>
    3cc4:   e51b301c    ldr r3, [fp, #-28]
    3cc8:   e59331b8    ldr r3, [r3, #440]
    3ccc:   e5933020    ldr r3, [r3, #32]
    3cd0:   e1a01003    mov r1, r3
    3cd4:   e51b301c    ldr r3, [fp, #-28]
    3cd8:   e59331b8    ldr r3, [r3, #440]
    3cdc:   e5933020    ldr r3, [r3, #32]
    3ce0:   e2833002    add r3, r3, #2  ; 0x2
    3ce4:   e2833001    add r3, r3, #1  ; 0x1
    3ce8:   e5d33000    ldrb    r3, [r3]
    3cec:   e1a03403    lsl r3, r3, #8
    3cf0:   e6ff2073    uxth    r2, r3
    3cf4:   e51b301c    ldr r3, [fp, #-28]
    3cf8:   e59331b8    ldr r3, [r3, #440]
    3cfc:   e5933020    ldr r3, [r3, #32]
    3d00:   e2833002    add r3, r3, #2  ; 0x2
    3d04:   e5d33000    ldrb    r3, [r3]
    3d08:   e1823003    orr r3, r2, r3
    3d0c:   e6ff3073    uxth    r3, r3
    3d10:   e6ff3073    uxth    r3, r3
    3d14:   e1a00001    mov r0, r1
    3d18:   e1a01003    mov r1, r3
    3d1c:   e3a02009    mov r2, #9  ; 0x9
    3d20:   e3a03000    mov r3, #0  ; 0x0
    3d24:   ebfffffe    bl  0 <MGC_FindDescriptor>
            3d24: R_ARM_PC24    MGC_FindDescriptor
    3d28:   e1a03000    mov r3, r0
    3d2c:   e50b3018    str r3, [fp, #-24]
    3d30:   e51b3018    ldr r3, [fp, #-24]
    3d34:   e3530000    cmp r3, #0  ; 0x0
    3d38:   0a000017    beq 3d9c <MGC_DrcAcceptDevice+0x230>
    3d3c:   e51b3018    ldr r3, [fp, #-24]
    3d40:   e5d33002    ldrb    r3, [r3, #2]
    3d44:   e2033002    and r3, r3, #2  ; 0x2
    3d48:   e3530000    cmp r3, #0  ; 0x0
    3d4c:   0a000012    beq 3d9c <MGC_DrcAcceptDevice+0x230>
    3d50:   e51b301c    ldr r3, [fp, #-28]
    3d54:   e59321b8    ldr r2, [r3, #440]
    3d58:   e59f3054    ldr r3, [pc, #84]   ; 3db4 <MGC_DrcAcceptDevice+0x248>
    3d5c:   e5832000    str r2, [r3]
    3d60:   e59f204c    ldr r2, [pc, #76]   ; 3db4 <MGC_DrcAcceptDevice+0x248>
    3d64:   e51b301c    ldr r3, [fp, #-28]
    3d68:   e5823024    str r3, [r2, #36]
    3d6c:   e51b301c    ldr r3, [fp, #-28]
    3d70:   e5933004    ldr r3, [r3, #4]
    3d74:   e1a00003    mov r0, r3
    3d78:   e59f1034    ldr r1, [pc, #52]   ; 3db4 <MGC_DrcAcceptDevice+0x248>
    3d7c:   ebfffffe    bl  0 <MUSB_StartControlTransfer>
            3d7c: R_ARM_PC24    MUSB_StartControlTransfer
    3d80:   ea000005    b   3d9c <MGC_DrcAcceptDevice+0x230>
    3d84:   e51b301c    ldr r3, [fp, #-28]
    3d88:   e59331b8    ldr r3, [r3, #440]
    3d8c:   e3530000    cmp r3, #0  ; 0x0
    3d90:   1a000001    bne 3d9c <MGC_DrcAcceptDevice+0x230>
    3d94:   e51b001c    ldr r0, [fp, #-28]
    3d98:   ebfffffe    bl  40fc <MUSB_RelinquishHost>
            3d98: R_ARM_PC24    MUSB_RelinquishHost
    3d9c:   e55b3011    ldrb    r3, [fp, #-17]
    3da0:   e1a00003    mov r0, r3
    3da4:   e24bd00c    sub sp, fp, #12 ; 0xc
    3da8:   e89da800    ldm sp, {fp, sp, pc}
    3dac:   00001a0a    .word   0x00001a0a
    3db0:   0000badd    .word   0x0000badd
    3db4:   00000000    .word   0x00000000
            3db4: R_ARM_ABS32   .data

00003db8 <MGC_DrcFinishResume>:
    3db8:   e1a0c00d    mov ip, sp
    3dbc:   e92dd800    push    {fp, ip, lr, pc}
    3dc0:   e24cb004    sub fp, ip, #4  ; 0x4
    3dc4:   e24dd00c    sub sp, sp, #12 ; 0xc
    3dc8:   e50b0014    str r0, [fp, #-20]
    3dcc:   e3a0300a    mov r3, #10 ; 0xa
    3dd0:   e50b3010    str r3, [fp, #-16]
    3dd4:   e51b3014    ldr r3, [fp, #-20]
    3dd8:   e5933218    ldr r3, [r3, #536]
    3ddc:   e51b0014    ldr r0, [fp, #-20]
    3de0:   e12fff33    blx r3
    3de4:   e51b3014    ldr r3, [fp, #-20]
    3de8:   e5d3303e    ldrb    r3, [r3, #62]
    3dec:   e3530000    cmp r3, #0  ; 0x0
    3df0:   0a000001    beq 3dfc <MGC_DrcFinishResume+0x44>
    3df4:   e3a03014    mov r3, #20 ; 0x14
    3df8:   e50b3010    str r3, [fp, #-16]
    3dfc:   e51b3014    ldr r3, [fp, #-20]
    3e00:   e5933000    ldr r3, [r3]
    3e04:   e593300c    ldr r3, [r3, #12]
    3e08:   e593c018    ldr ip, [r3, #24]
    3e0c:   e51b3014    ldr r3, [fp, #-20]
    3e10:   e5933000    ldr r3, [r3]
    3e14:   e593300c    ldr r3, [r3, #12]
    3e18:   e5932004    ldr r2, [r3, #4]
    3e1c:   e59f301c    ldr r3, [pc, #28]   ; 3e40 <MGC_DrcFinishResume+0x88>
    3e20:   e58d3000    str r3, [sp]
    3e24:   e1a00002    mov r0, r2
    3e28:   e3a01000    mov r1, #0  ; 0x0
    3e2c:   e51b2010    ldr r2, [fp, #-16]
    3e30:   e3a03000    mov r3, #0  ; 0x0
    3e34:   e12fff3c    blx ip
    3e38:   e24bd00c    sub sp, fp, #12 ; 0xc
    3e3c:   e89da800    ldm sp, {fp, sp, pc}
    3e40:   00000000    .word   0x00000000
            3e40: R_ARM_ABS32   MGC_DrcCompleteResume

00003e44 <MGC_DrcSetHostPower>:
    3e44:   e1a0c00d    mov ip, sp
    3e48:   e92dd800    push    {fp, ip, lr, pc}
    3e4c:   e24cb004    sub fp, ip, #4  ; 0x4
    3e50:   e24dd010    sub sp, sp, #16 ; 0x10
    3e54:   e50b0018    str r0, [fp, #-24]
    3e58:   e1a03001    mov r3, r1
    3e5c:   e14b31bc    strh    r3, [fp, #-28]
    3e60:   e3a030a8    mov r3, #168    ; 0xa8
    3e64:   e50b3014    str r3, [fp, #-20]
    3e68:   e51b3018    ldr r3, [fp, #-24]
    3e6c:   e5933014    ldr r3, [r3, #20]
    3e70:   e50b3010    str r3, [fp, #-16]
    3e74:   e51b1010    ldr r1, [fp, #-16]
    3e78:   e3a02f77    mov r2, #476    ; 0x1dc
    3e7c:   e15b31bc    ldrh    r3, [fp, #-28]
    3e80:   e18130b2    strh    r3, [r1, r2]
    3e84:   e51b3010    ldr r3, [fp, #-16]
    3e88:   e5933218    ldr r3, [r3, #536]
    3e8c:   e51b0010    ldr r0, [fp, #-16]
    3e90:   e12fff33    blx r3
    3e94:   e51b3010    ldr r3, [fp, #-16]
    3e98:   e5d3303f    ldrb    r3, [r3, #63]
    3e9c:   e3530000    cmp r3, #0  ; 0x0
    3ea0:   0a000002    beq 3eb0 <MGC_DrcSetHostPower+0x6c>
    3ea4:   e3a03000    mov r3, #0  ; 0x0
    3ea8:   e50b3014    str r3, [fp, #-20]
    3eac:   ea000005    b   3ec8 <MGC_DrcSetHostPower+0x84>
    3eb0:   e15b31bc    ldrh    r3, [fp, #-28]
    3eb4:   e51b0010    ldr r0, [fp, #-16]
    3eb8:   e1a01003    mov r1, r3
    3ebc:   ebfffffe    bl  0 <MGC_HostSetMaxPower>
            3ebc: R_ARM_PC24    MGC_HostSetMaxPower
    3ec0:   e1a03000    mov r3, r0
    3ec4:   e50b3014    str r3, [fp, #-20]
    3ec8:   e51b3014    ldr r3, [fp, #-20]
    3ecc:   e1a00003    mov r0, r3
    3ed0:   e24bd00c    sub sp, fp, #12 ; 0xc
    3ed4:   e89da800    ldm sp, {fp, sp, pc}

00003ed8 <MGC_OtgStateGetId>:
    3ed8:   e1a0c00d    mov ip, sp
    3edc:   e92dd800    push    {fp, ip, lr, pc}
    3ee0:   e24cb004    sub fp, ip, #4  ; 0x4
    3ee4:   e24dd008    sub sp, sp, #8  ; 0x8
    3ee8:   e50b0010    str r0, [fp, #-16]
    3eec:   e1a03001    mov r3, r1
    3ef0:   e54b3014    strb    r3, [fp, #-20]
    3ef4:   e51b2010    ldr r2, [fp, #-16]
    3ef8:   e3a03000    mov r3, #0  ; 0x0
    3efc:   e5c23026    strb    r3, [r2, #38]
    3f00:   e51b2010    ldr r2, [fp, #-16]
    3f04:   e3a03001    mov r3, #1  ; 0x1
    3f08:   e5c23029    strb    r3, [r2, #41]
    3f0c:   e51b3010    ldr r3, [fp, #-16]
    3f10:   e593321c    ldr r3, [r3, #540]
    3f14:   e51b0010    ldr r0, [fp, #-16]
    3f18:   e12fff33    blx r3
    3f1c:   e51b3010    ldr r3, [fp, #-16]
    3f20:   e5933218    ldr r3, [r3, #536]
    3f24:   e51b0010    ldr r0, [fp, #-16]
    3f28:   e12fff33    blx r3
    3f2c:   e51b3010    ldr r3, [fp, #-16]
    3f30:   e5d3303e    ldrb    r3, [r3, #62]
    3f34:   e3530000    cmp r3, #0  ; 0x0
    3f38:   1a000014    bne 3f90 <MGC_OtgStateGetId+0xb8>
    3f3c:   e51b3010    ldr r3, [fp, #-16]
    3f40:   e5d3303f    ldrb    r3, [r3, #63]
    3f44:   e3530000    cmp r3, #0  ; 0x0
    3f48:   0a00000a    beq 3f78 <MGC_OtgStateGetId+0xa0>
    3f4c:   e55b3014    ldrb    r3, [fp, #-20]
    3f50:   e3530000    cmp r3, #0  ; 0x0
    3f54:   0a000003    beq 3f68 <MGC_OtgStateGetId+0x90>
    3f58:   e3a00011    mov r0, #17 ; 0x11
    3f5c:   e51b1010    ldr r1, [fp, #-16]
    3f60:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3f60: R_ARM_PC24    MGC_DrcChangeOtgState
    3f64:   ea000009    b   3f90 <MGC_OtgStateGetId+0xb8>
    3f68:   e3a00014    mov r0, #20 ; 0x14
    3f6c:   e51b1010    ldr r1, [fp, #-16]
    3f70:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3f70: R_ARM_PC24    MGC_DrcChangeOtgState
    3f74:   ea000005    b   3f90 <MGC_OtgStateGetId+0xb8>
    3f78:   e51b3010    ldr r3, [fp, #-16]
    3f7c:   e3a02001    mov r2, #1  ; 0x1
    3f80:   e5c3204e    strb    r2, [r3, #78]
    3f84:   e3a00022    mov r0, #34 ; 0x22
    3f88:   e51b1010    ldr r1, [fp, #-16]
    3f8c:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            3f8c: R_ARM_PC24    MGC_DrcChangeOtgState
    3f90:   e24bd00c    sub sp, fp, #12 ; 0xc
    3f94:   e89da800    ldm sp, {fp, sp, pc}

00003f98 <MUSB_RegisterOtgClient>:
    3f98:   e1a0c00d    mov ip, sp
    3f9c:   e92dd800    push    {fp, ip, lr, pc}
    3fa0:   e24cb004    sub fp, ip, #4  ; 0x4
    3fa4:   e24dd020    sub sp, sp, #32 ; 0x20
    3fa8:   e50b0020    str r0, [fp, #-32]
    3fac:   e50b1024    str r1, [fp, #-36]
    3fb0:   e50b2028    str r2, [fp, #-40]
    3fb4:   e50b302c    str r3, [fp, #-44]
    3fb8:   e3a03000    mov r3, #0  ; 0x0
    3fbc:   e50b3018    str r3, [fp, #-24]
    3fc0:   e3a03000    mov r3, #0  ; 0x0
    3fc4:   e50b3014    str r3, [fp, #-20]
    3fc8:   e51b3020    ldr r3, [fp, #-32]
    3fcc:   e5933000    ldr r3, [r3]
    3fd0:   e50b3010    str r3, [fp, #-16]
    3fd4:   e51b3010    ldr r3, [fp, #-16]
    3fd8:   e5d3302d    ldrb    r3, [r3, #45]
    3fdc:   e3530000    cmp r3, #0  ; 0x0
    3fe0:   1a000041    bne 40ec <MUSB_RegisterOtgClient+0x154>
    3fe4:   e51b3010    ldr r3, [fp, #-16]
    3fe8:   e51b2028    ldr r2, [fp, #-40]
    3fec:   e58321ac    str r2, [r3, #428]
    3ff0:   e51b3024    ldr r3, [fp, #-36]
    3ff4:   e3530000    cmp r3, #0  ; 0x0
    3ff8:   0a000004    beq 4010 <MUSB_RegisterOtgClient+0x78>
    3ffc:   e51b0010    ldr r0, [fp, #-16]
    4000:   e51b1024    ldr r1, [fp, #-36]
    4004:   ebfffffe    bl  0 <MGC_FunctionRegisterClient>
            4004: R_ARM_PC24    MGC_FunctionRegisterClient
    4008:   e1a03000    mov r3, r0
    400c:   e50b3018    str r3, [fp, #-24]
    4010:   e51b3018    ldr r3, [fp, #-24]
    4014:   e3530000    cmp r3, #0  ; 0x0
    4018:   1a000033    bne 40ec <MUSB_RegisterOtgClient+0x154>
    401c:   e51b2010    ldr r2, [fp, #-16]
    4020:   e3a03000    mov r3, #0  ; 0x0
    4024:   e5c2304e    strb    r3, [r2, #78]
    4028:   e51b3024    ldr r3, [fp, #-36]
    402c:   e3530000    cmp r3, #0  ; 0x0
    4030:   0a00000a    beq 4060 <MUSB_RegisterOtgClient+0xc8>
    4034:   e51b3024    ldr r3, [fp, #-36]
    4038:   e5932004    ldr r2, [r3, #4]
    403c:   e51b3024    ldr r3, [fp, #-36]
    4040:   e1d330b8    ldrh    r3, [r3, #8]
    4044:   e1a00002    mov r0, r2
    4048:   e1a01003    mov r1, r3
    404c:   e3a02009    mov r2, #9  ; 0x9
    4050:   e3a03000    mov r3, #0  ; 0x0
    4054:   ebfffffe    bl  0 <MGC_FindDescriptor>
            4054: R_ARM_PC24    MGC_FindDescriptor
    4058:   e1a03000    mov r3, r0
    405c:   e50b301c    str r3, [fp, #-28]
    4060:   e51b301c    ldr r3, [fp, #-28]
    4064:   e3530000    cmp r3, #0  ; 0x0
    4068:   0a000007    beq 408c <MUSB_RegisterOtgClient+0xf4>
    406c:   e51b301c    ldr r3, [fp, #-28]
    4070:   e5d33002    ldrb    r3, [r3, #2]
    4074:   e2033002    and r3, r3, #2  ; 0x2
    4078:   e3530000    cmp r3, #0  ; 0x0
    407c:   03a03000    moveq   r3, #0  ; 0x0
    4080:   13a03001    movne   r3, #1  ; 0x1
    4084:   e51b2010    ldr r2, [fp, #-16]
    4088:   e5c2304e    strb    r3, [r2, #78]
    408c:   e51b2010    ldr r2, [fp, #-16]
    4090:   e3a03004    mov r3, #4  ; 0x4
    4094:   e5c23045    strb    r3, [r2, #69]
    4098:   e51b2010    ldr r2, [fp, #-16]
    409c:   e3a03004    mov r3, #4  ; 0x4
    40a0:   e5c23047    strb    r3, [r2, #71]
    40a4:   e51b3010    ldr r3, [fp, #-16]
    40a8:   e5933220    ldr r3, [r3, #544]
    40ac:   e51b0010    ldr r0, [fp, #-16]
    40b0:   e12fff33    blx r3
    40b4:   e51b2010    ldr r2, [fp, #-16]
    40b8:   e51b3024    ldr r3, [fp, #-36]
    40bc:   e58231e0    str r3, [r2, #480]
    40c0:   e51b2010    ldr r2, [fp, #-16]
    40c4:   e51b302c    ldr r3, [fp, #-44]
    40c8:   e5823050    str r3, [r2, #80]
    40cc:   e51b2010    ldr r2, [fp, #-16]
    40d0:   e3a03001    mov r3, #1  ; 0x1
    40d4:   e5c2302d    strb    r3, [r2, #45]
    40d8:   e51b0010    ldr r0, [fp, #-16]
    40dc:   e3a01000    mov r1, #0  ; 0x0
    40e0:   ebffff7c    bl  3ed8 <MGC_OtgStateGetId>
    40e4:   e51b3010    ldr r3, [fp, #-16]
    40e8:   e50b3014    str r3, [fp, #-20]
    40ec:   e51b3014    ldr r3, [fp, #-20]
    40f0:   e1a00003    mov r0, r3
    40f4:   e24bd00c    sub sp, fp, #12 ; 0xc
    40f8:   e89da800    ldm sp, {fp, sp, pc}

000040fc <MUSB_RelinquishHost>:
    40fc:   e1a0c00d    mov ip, sp
    4100:   e92dd800    push    {fp, ip, lr, pc}
    4104:   e24cb004    sub fp, ip, #4  ; 0x4
    4108:   e24dd014    sub sp, sp, #20 ; 0x14
    410c:   e50b001c    str r0, [fp, #-28]
    4110:   e51b301c    ldr r3, [fp, #-28]
    4114:   e50b3018    str r3, [fp, #-24]
    4118:   e51b3018    ldr r3, [fp, #-24]
    411c:   e5933000    ldr r3, [r3]
    4120:   e50b3014    str r3, [fp, #-20]
    4124:   e51b3014    ldr r3, [fp, #-20]
    4128:   e593300c    ldr r3, [r3, #12]
    412c:   e50b3010    str r3, [fp, #-16]
    4130:   e51b2018    ldr r2, [fp, #-24]
    4134:   e3a03001    mov r3, #1  ; 0x1
    4138:   e5c2304d    strb    r3, [r2, #77]
    413c:   e51b3018    ldr r3, [fp, #-24]
    4140:   e5d3303f    ldrb    r3, [r3, #63]
    4144:   e3530000    cmp r3, #0  ; 0x0
    4148:   1a000003    bne 415c <MUSB_RelinquishHost+0x60>
    414c:   e3a00024    mov r0, #36 ; 0x24
    4150:   e51b1018    ldr r1, [fp, #-24]
    4154:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            4154: R_ARM_PC24    MGC_DrcChangeOtgState
    4158:   ea000018    b   41c0 <MUSB_RelinquishHost+0xc4>
    415c:   e51b3018    ldr r3, [fp, #-24]
    4160:   e5933218    ldr r3, [r3, #536]
    4164:   e51b0018    ldr r0, [fp, #-24]
    4168:   e12fff33    blx r3
    416c:   e51b2018    ldr r2, [fp, #-24]
    4170:   e3a03001    mov r3, #1  ; 0x1
    4174:   e5c23026    strb    r3, [r2, #38]
    4178:   e51b3018    ldr r3, [fp, #-24]
    417c:   e593321c    ldr r3, [r3, #540]
    4180:   e51b0018    ldr r0, [fp, #-24]
    4184:   e12fff33    blx r3
    4188:   e3a00011    mov r0, #17 ; 0x11
    418c:   e51b1018    ldr r1, [fp, #-24]
    4190:   ebfffffe    bl  2018 <MGC_DrcChangeOtgState>
            4190: R_ARM_PC24    MGC_DrcChangeOtgState
    4194:   e51b3010    ldr r3, [fp, #-16]
    4198:   e593c018    ldr ip, [r3, #24]
    419c:   e51b3010    ldr r3, [fp, #-16]
    41a0:   e5932004    ldr r2, [r3, #4]
    41a4:   e59f301c    ldr r3, [pc, #28]   ; 41c8 <MUSB_RelinquishHost+0xcc>
    41a8:   e58d3000    str r3, [sp]
    41ac:   e1a00002    mov r0, r2
    41b0:   e3a01000    mov r1, #0  ; 0x0
    41b4:   e3a0200a    mov r2, #10 ; 0xa
    41b8:   e3a03000    mov r3, #0  ; 0x0
    41bc:   e12fff3c    blx ip
    41c0:   e24bd00c    sub sp, fp, #12 ; 0xc
    41c4:   e89da800    ldm sp, {fp, sp, pc}
    41c8:   00000000    .word   0x00000000
            41c8: R_ARM_ABS32   MGC_RemoveSuspend

000041cc <MUSB_RequestBus>:
    41cc:   e1a0c00d    mov ip, sp
    41d0:   e92dd800    push    {fp, ip, lr, pc}
    41d4:   e24cb004    sub fp, ip, #4  ; 0x4
    41d8:   e24dd00c    sub sp, sp, #12 ; 0xc
    41dc:   e50b0014    str r0, [fp, #-20]
    41e0:   e51b3014    ldr r3, [fp, #-20]
    41e4:   e50b3010    str r3, [fp, #-16]
    41e8:   e51b3010    ldr r3, [fp, #-16]
    41ec:   e5d3304c    ldrb    r3, [r3, #76]
    41f0:   e3530000    cmp r3, #0  ; 0x0
    41f4:   0a000000    beq 61c0 <MGC_DrcDumpPipe+0x1f6c>
    41f8:   ea000005    b   61d8 <MGC_DrcDumpPipe+0x1f84>
    41fc:   e51b0010    ldr r0, [fp, #-16]
    4200:   e3a01000    mov r1, #0  ; 0x0
    4204:   ebffff33    bl  5e9c <MGC_DrcDumpPipe+0x1c48>
    4208:   e3a03001    mov r3, #1  ; 0x1
    420c:   e50b3018    str r3, [fp, #-24]
    4210:   ea00000b    b   6208 <MGC_DrcDumpPipe+0x1fb4>
    4214:   e51b3010    ldr r3, [fp, #-16]
    4218:   e5d3303d    ldrb    r3, [r3, #61]
    421c:   e3530000    cmp r3, #0  ; 0x0
    4220:   1a000005    bne 6200 <MGC_DrcDumpPipe+0x1fac>
    4224:   e51b0010    ldr r0, [fp, #-16]
    4228:   e3a01000    mov r1, #0  ; 0x0
    422c:   ebffff29    bl  5e9c <MGC_DrcDumpPipe+0x1c48>
    4230:   e3a03001    mov r3, #1  ; 0x1
    4234:   e50b3018    str r3, [fp, #-24]
    4238:   ea000001    b   6208 <MGC_DrcDumpPipe+0x1fb4>
    423c:   e3a03000    mov r3, #0  ; 0x0
    4240:   e50b3018    str r3, [fp, #-24]
    4244:   e51b3018    ldr r3, [fp, #-24]
    4248:   e1a00003    mov r0, r3
    424c:   e24bd00c    sub sp, fp, #12 ; 0xc
    4250:   e89da800    ldm sp, {fp, sp, pc}

00004254 <MGC_DrcDumpPipe>:
    4254:   e1a0c00d    mov ip, sp
    4258:   e92dd800    push    {fp, ip, lr, pc}
    425c:   e24cb004    sub fp, ip, #4  ; 0x4
    4260:   e24dd008    sub sp, sp, #8  ; 0x8
    4264:   e50b0010    str r0, [fp, #-16]
    4268:   e50b1014    str r1, [fp, #-20]
    426c:   e3a03000    mov r3, #0  ; 0x0
    4270:   e1a00003    mov r0, r3
    4274:   e24bd00c    sub sp, fp, #12 ; 0xc
    4278:   e89da800    ldm sp, {fp, sp, pc}
Disassembly of section .data:

00000000 <MGC_SetHnpSupportIrp>:
    ...
            4: R_ARM_ABS32  .rodata
   8:   00000008    andeq   r0, r0, r8
    ...
            20: R_ARM_ABS32 MGC_SetHnpSupportIrpComplete

0000002c <MGC_SetHnpEnableIrp>:
  2c:   00000000    andeq   r0, r0, r0
  30:   00000008    .word   0x00000008
            30: R_ARM_ABS32 .rodata
  34:   00000008    andeq   r0, r0, r8
    ...
            4c: R_ARM_ABS32 MGC_SetHnpEnableIrpComplete
Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:   25011101    strcs   r1, [r1, #-257]
   4:   030b1308    movweq  r1, #45832  ; 0xb308
   8:   11081b08    tstne   r8, r8, lsl #22
   c:   10011201    andne   r1, r1, r1, lsl #4
  10:   02000006    andeq   r0, r0, #6  ; 0x6
  14:   08030016    stmdaeq r3, {r1, r2, r4}
  18:   0b3b0b3a    bleq    ec2d08 <MGC_DrcDumpPipe+0xebeab4>
  1c:   00001349    andeq   r1, r0, r9, asr #6
  20:   0b002403    bleq    9034 <MGC_DrcDumpPipe+0x4de0>
  24:   030b3e0b    movweq  r3, #48651  ; 0xbe0b
  28:   04000008    streq   r0, [r0], #-8
  2c:   0b0b0024    bleq    2c00c4 <MGC_DrcDumpPipe+0x2bbe70>
  30:   00000b3e    andeq   r0, r0, lr, lsr fp
  34:   0b000f05    bleq    3c50 <MGC_DrcAcceptDevice+0xe4>
  38:   0600000b    streq   r0, [r0], -fp
  3c:   0c270015    stceq   0, cr0, [r7], #-84
  40:   0f070000    svceq   0x00070000
  44:   490b0b00    stmdbmi fp, {r8, r9, fp}
  48:   08000013    stmdaeq r0, {r0, r1, r4}
  4c:   13490026    movtne  r0, #36902  ; 0x9026
  50:   04090000    streq   r0, [r9]
  54:   3a0b0b01    bcc 2c2c60 <MGC_DrcDumpPipe+0x2bea0c>
  58:   010b3b0b    tsteq   fp, fp, lsl #22
  5c:   0a000013    beq b0 <.debug_abbrev+0xb0>
  60:   08030028    stmdaeq r3, {r3, r5}
  64:   00000d1c    andeq   r0, r0, ip, lsl sp
  68:   0b01130b    bleq    44c9c <MGC_DrcDumpPipe+0x40a48>
  6c:   3b0b3a0b    blcc    2ce8a0 <MGC_DrcDumpPipe+0x2ca64c>
  70:   0013010b    andseq  r0, r3, fp, lsl #2
  74:   000d0c00    andeq   r0, sp, r0, lsl #24
  78:   0b3a0803    bleq    e8208c <MGC_DrcDumpPipe+0xe7de38>
  7c:   13490b3b    movtne  r0, #39739  ; 0x9b3b
  80:   00000a38    andeq   r0, r0, r8, lsr sl
  84:   03000d0d    movweq  r0, #3341   ; 0xd0d
  88:   3b0b3a08    blcc    2ce8b0 <MGC_DrcDumpPipe+0x2ca65c>
  8c:   38134905    ldmdacc r3, {r0, r2, r8, fp, lr}
  90:   0e00000a    cdpeq   0, 0, cr0, cr0, cr10, {0}
  94:   0c270015    stceq   0, cr0, [r7], #-84
  98:   00001349    andeq   r1, r0, r9, asr #6
  9c:   2701150f    strcs   r1, [r1, -pc, lsl #10]
  a0:   0013010c    andseq  r0, r3, ip, lsl #2
  a4:   00051000    andeq   r1, r5, r0
  a8:   00001349    andeq   r1, r0, r9, asr #6
  ac:   03001611    movweq  r1, #1553   ; 0x611
  b0:   3b0b3a08    blcc    2ce8d8 <MGC_DrcDumpPipe+0x2ca684>
  b4:   00134905    andseq  r4, r3, r5, lsl #18
  b8:   01131200    tsteq   r3, r0, lsl #4
  bc:   0b0b0803    bleq    2c20d0 <MGC_DrcDumpPipe+0x2bde7c>
  c0:   053b0b3a    ldreq   r0, [fp, #-2874]!
  c4:   00001301    andeq   r1, r0, r1, lsl #6
  c8:   27011513    smladcs r1, r3, r5, r1
  cc:   0113490c    tsteq   r3, ip, lsl #18
  d0:   14000013    strne   r0, [r0], #-19
  d4:   0e03000d    cdpeq   0, 0, cr0, cr3, cr13, {0}
  d8:   0b3b0b3a    bleq    ec2dc8 <MGC_DrcDumpPipe+0xebeb74>
  dc:   0a381349    beq e04e08 <MGC_DrcDumpPipe+0xe00bb4>
  e0:   26150000    ldrcs   r0, [r5], -r0
  e4:   16000000    strne   r0, [r0], -r0
  e8:   0b0b0113    bleq    2c053c <MGC_DrcDumpPipe+0x2bc2e8>
  ec:   053b0b3a    ldreq   r0, [fp, #-2874]!
  f0:   00001301    andeq   r1, r0, r1, lsl #6
  f4:   03000d17    movweq  r0, #3351   ; 0xd17
  f8:   3b0b3a0e    blcc    2ce938 <MGC_DrcDumpPipe+0x2ca6e4>
  fc:   38134905    ldmdacc r3, {r0, r2, r8, fp, lr}
 100:   1800000a    stmdane r0, {r1, r3}
 104:   08030113    stmdaeq r3, {r0, r1, r4, r8}
 108:   0b3a0b0b    bleq    e82d3c <MGC_DrcDumpPipe+0xe7eae8>
 10c:   13010b3b    movwne  r0, #6971   ; 0x1b3b
 110:   04190000    ldreq   r0, [r9]
 114:   0b080301    bleq    200d20 <MGC_DrcDumpPipe+0x1fcacc>
 118:   3b0b3a0b    blcc    2ce94c <MGC_DrcDumpPipe+0x2ca6f8>
 11c:   0013010b    andseq  r0, r3, fp, lsl #2
 120:   01131a00    tsteq   r3, r0, lsl #20
 124:   050b0803    streq   r0, [fp, #-2051]
 128:   0b3b0b3a    bleq    ec2e18 <MGC_DrcDumpPipe+0xebebc4>
 12c:   00001301    andeq   r1, r0, r1, lsl #6
 130:   0b01131b    bleq    44da4 <MGC_DrcDumpPipe+0x40b50>
 134:   3b0b3a05    blcc    2ce950 <MGC_DrcDumpPipe+0x2ca6fc>
 138:   00130105    andseq  r0, r3, r5, lsl #2
 13c:   01011c00    tsteq   r1, r0, lsl #24
 140:   13011349    movwne  r1, #4937   ; 0x1349
 144:   211d0000    tstcs   sp, r0
 148:   2f134900    svccs   0x00134900
 14c:   1e00000b    cdpne   0, 0, cr0, cr0, cr11, {0}
 150:   00000018    andeq   r0, r0, r8, lsl r0
 154:   3f012e1f    svccc   0x00012e1f
 158:   3a08030c    bcc 200d90 <MGC_DrcDumpPipe+0x1fcb3c>
 15c:   270b3b0b    strcs   r3, [fp, -fp, lsl #22]
 160:   1113490c    tstne   r3, ip, lsl #18
 164:   40011201    andmi   r1, r1, r1, lsl #4
 168:   00130106    andseq  r0, r3, r6, lsl #2
 16c:   00052000    andeq   r2, r5, r0
 170:   0b3a0e03    bleq    e83984 <MGC_DrcDumpPipe+0xe7f730>
 174:   13490b3b    movtne  r0, #39739  ; 0x9b3b
 178:   00000a02    andeq   r0, r0, r2, lsl #20
 17c:   03003421    movweq  r3, #1057   ; 0x421
 180:   3b0b3a08    blcc    2ce9a8 <MGC_DrcDumpPipe+0x2ca754>
 184:   0213490b    andseq  r4, r3, #180224 ; 0x2c000
 188:   2200000a    andcs   r0, r0, #10 ; 0xa
 18c:   08030005    stmdaeq r3, {r0, r2}
 190:   0b3b0b3a    bleq    ec2e80 <MGC_DrcDumpPipe+0xebec2c>
 194:   0a021349    beq 84ec0 <MGC_DrcDumpPipe+0x80c6c>
 198:   34230000    strtcc  r0, [r3]
 19c:   3a0e0300    bcc 380da4 <MGC_DrcDumpPipe+0x37cb50>
 1a0:   490b3b0b    stmdbmi fp, {r0, r1, r3, r8, r9, fp, ip, sp}
 1a4:   000a0213    andeq   r0, sl, r3, lsl r2
 1a8:   012e2400    teqeq   lr, r0, lsl #8
 1ac:   08030c3f    stmdaeq r3, {r0, r1, r2, r3, r4, r5, sl, fp}
 1b0:   053b0b3a    ldreq   r0, [fp, #-2874]!
 1b4:   13490c27    movtne  r0, #39975  ; 0x9c27
 1b8:   01120111    tsteq   r2, r1, lsl r1
 1bc:   13010640    movwne  r0, #5696   ; 0x1640
 1c0:   05250000    streq   r0, [r5]!
 1c4:   3a0e0300    bcc 380dcc <MGC_DrcDumpPipe+0x37cb78>
 1c8:   49053b0b    stmdbmi r5, {r0, r1, r3, r8, r9, fp, ip, sp}
 1cc:   000a0213    andeq   r0, sl, r3, lsl r2
 1d0:   00052600    andeq   r2, r5, r0, lsl #12
 1d4:   0b3a0803    bleq    e821e8 <MGC_DrcDumpPipe+0xe7df94>
 1d8:   1349053b    movtne  r0, #38203  ; 0x953b
 1dc:   00000a02    andeq   r0, r0, r2, lsl #20
 1e0:   03003427    movweq  r3, #1063   ; 0x427
 1e4:   3b0b3a0e    blcc    2cea24 <MGC_DrcDumpPipe+0x2ca7d0>
 1e8:   02134905    andseq  r4, r3, #81920  ; 0x14000
 1ec:   2800000a    stmdacs r0, {r1, r3}
 1f0:   08030034    stmdaeq r3, {r2, r4, r5}
 1f4:   053b0b3a    ldreq   r0, [fp, #-2874]!
 1f8:   0a021349    beq 84f24 <MGC_DrcDumpPipe+0x80cd0>
 1fc:   2e290000    cdpcs   0, 2, cr0, cr9, cr0, {0}
 200:   3a080301    bcc 200e0c <MGC_DrcDumpPipe+0x1fcbb8>
 204:   27053b0b    strcs   r3, [r5, -fp, lsl #22]
 208:   1201110c    andne   r1, r1, #3  ; 0x3
 20c:   01064001    tsteq   r6, r1
 210:   2a000013    bcs 264 <.debug_abbrev+0x264>
 214:   0c3f012e    ldfeqs  f0, [pc], #-184
 218:   0b3a0803    bleq    e8222c <MGC_DrcDumpPipe+0xe7dfd8>
 21c:   0c27053b    cfstr32eq   mvfx0, [r7], #-236
 220:   01120111    tsteq   r2, r1, lsl r1
 224:   13010640    movwne  r0, #5696   ; 0x1640
 228:   2e2b0000    cdpcs   0, 2, cr0, cr11, cr0, {0}
 22c:   3a080301    bcc 200e38 <MGC_DrcDumpPipe+0x1fcbe4>
 230:   27053b0b    strcs   r3, [r5, -fp, lsl #22]
 234:   1113490c    tstne   r3, ip, lsl #18
 238:   40011201    andmi   r1, r1, r1, lsl #4
 23c:   00130106    andseq  r0, r3, r6, lsl #2
 240:   010b2c00    tsteq   fp, r0, lsl #24
 244:   01120111    tsteq   r2, r1, lsl r1
 248:   342d0000    strtcc  r0, [sp]
 24c:   3a080300    bcc 200e54 <MGC_DrcDumpPipe+0x1fcc00>
 250:   490b3b0b    stmdbmi fp, {r0, r1, r3, r8, r9, fp, ip, sp}
 254:   3c0c3f13    stccc   15, cr3, [ip], {19}
 258:   2e00000c    cdpcs   0, 0, cr0, cr0, cr12, {0}
 25c:   08030034    stmdaeq r3, {r2, r4, r5}
 260:   053b0b3a    ldreq   r0, [fp, #-2874]!
 264:   0c3f1349    ldceq   3, cr1, [pc], #-292
 268:   00000c3c    andeq   r0, r0, ip, lsr ip
    ...
Disassembly of section .debug_info:

00000000 <.debug_info>:
       0:   0000571c    andeq   r5, r0, ip, lsl r7
       4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_abbrev
       8:   01040000    tsteq   r4, r0
       c:   20554e47    subscs  r4, r5, r7, asr #28
      10:   2e342043    cdpcs   0, 3, cr2, cr4, cr3, {2}
      14:   00322e32    eorseq  r2, r2, r2, lsr lr
      18:   2f2e2e01    svccs   0x002e2e01
      1c:   6d2f6463    cfstrsvs    mvf6, [pc, #-396]!
      20:   72645f75    rsbvc   r5, r4, #468    ; 0x1d4
      24:   00632e63    rsbeq   r2, r3, r3, ror #28
      28:   6779632f    ldrbvs  r6, [r9, -pc, lsr #6]!
      2c:   76697264    strbtvc r7, [r9], -r4, ror #4
      30:   2f642f65    svccs   0x00642f65
      34:   4a4f5250    bmi 13d497c <MGC_DrcDumpPipe+0x13d0728>
      38:   53544345    cmppl   r4, #335544321  ; 0x14000001
      3c:   38424d2f    stmdacc r2, {r0, r1, r2, r3, r5, r8, sl, fp, lr}^
      40:   30364836    eorscc  r4, r6, r6, lsr r8
      44:   666f732f    strbtvs r7, [pc], -pc, lsr #6
      48:   72617774    rsbvc   r7, r1, #30408704   ; 0x1d00000
      4c:   44482f65    strbmi  r2, [r8], #-3941
      50:   625f5654    subsvs  r5, pc, #88080384   ; 0x5400000
      54:   636e6172    cmnvs   lr, #-2147483620    ; 0x8000001c
      58:   75732f68    ldrbvc  r2, [r3, #-3944]!
      5c:   73797362    cmnvc   r9, #-2013265919    ; 0x88000001
      60:   736d6574    cmnvc   sp, #486539264  ; 0x1d000000
      64:   7375622f    cmnvc   r5, #-268435454 ; 0xf0000002
      68:   73756d2f    cmnvc   r5, #3008   ; 0xbc0
      6c:   72732f62    rsbsvc  r2, r3, #392    ; 0x188
      70:   79732f63    ldmdbvc r3!, {r0, r1, r5, r6, r8, r9, sl, fp, sp}^
      74:   6d657473    cfstrdvs    mvd7, [r5, #-460]!
      78:   00000073    andeq   r0, r0, r3, ror r0
            7a: R_ARM_ABS32 .text
      7c:   427c0000    rsbsmi  r0, ip, #0  ; 0x0
            7e: R_ARM_ABS32 .text
      80:   00000000    andeq   r0, r0, r0
            82: R_ARM_ABS32 .debug_line
      84:   73020000    movwvc  r0, #8192   ; 0x2000
      88:   5f657a69    svcpl   0x00657a69
      8c:   d6020074    undefined
      90:   00000094    muleq   r0, r4, r0
      94:   6c070403    cfstrsvs    mvf0, [r7], {3}
      98:   20676e6f    rsbcs   r6, r7, pc, ror #28
      9c:   69736e75    ldmdbvs r3!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
      a0:   64656e67    strbtvs r6, [r5], #-3687
      a4:   746e6920    strbtvc r6, [lr], #-2336
      a8:   05040300    streq   r0, [r4, #-768]
      ac:   00746e69    rsbseq  r6, r4, r9, ror #28
      b0:   6c050403    cfstrsvs    mvf0, [r5], {3}
      b4:   20676e6f    rsbcs   r6, r7, pc, ror #28
      b8:   00746e69    rsbseq  r6, r4, r9, ror #28
      bc:   6c050803    stcvs   8, cr0, [r5], {3}
      c0:   20676e6f    rsbcs   r6, r7, pc, ror #28
      c4:   676e6f6c    strbvs  r6, [lr, -ip, ror #30]!
      c8:   746e6920    strbtvc r6, [lr], #-2336
      cc:   07040300    streq   r0, [r4, -r0, lsl #6]
      d0:   69736e75    ldmdbvs r3!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
      d4:   64656e67    strbtvs r6, [r5], #-3687
      d8:   746e6920    strbtvc r6, [lr], #-2336
      dc:   07040400    streq   r0, [r4, -r0, lsl #8]
      e0:   75080103    strvc   r0, [r8, #-259]
      e4:   6769736e    strbvs  r7, [r9, -lr, ror #6]!
      e8:   2064656e    rsbcs   r6, r4, lr, ror #10
      ec:   72616863    rsbvc   r6, r1, #6488064    ; 0x630000
      f0:   06040500    streq   r0, [r4], -r0, lsl #10
      f4:   f3040701    vabd.u8 d0, d4, d1
      f8:   03000000    movweq  r0, #0  ; 0x0
      fc:   68730502    ldmdavs r3!, {r1, r8, sl}^
     100:   2074726f    rsbscs  r7, r4, pc, ror #4
     104:   00746e69    rsbseq  r6, r4, r9, ror #28
     108:   010e0407    tsteq   lr, r7, lsl #8
     10c:   01030000    tsteq   r3, r0
     110:   61686308    cmnvs   r8, r8, lsl #6
     114:   04070072    streq   r0, [r7], #-114
     118:   0000011c    andeq   r0, r0, ip, lsl r1
     11c:   00010e08    andeq   r0, r1, r8, lsl #28
     120:   07020300    streq   r0, [r2, -r0, lsl #6]
     124:   726f6873    rsbvc   r6, pc, #7536640    ; 0x730000
     128:   6e752074    mrcvs   0, 3, r2, cr5, cr4, {3}
     12c:   6e676973    mcrvs   9, 3, r6, cr7, cr3, {3}
     130:   69206465    stmdbvs r0!, {r0, r2, r5, r6, sl, sp, lr}
     134:   0300746e    movweq  r7, #1134   ; 0x46e
     138:   6f6c0708    svcvs   0x006c0708
     13c:   6c20676e    stcvs   7, cr6, [r0], #-440
     140:   20676e6f    rsbcs   r6, r7, pc, ror #28
     144:   69736e75    ldmdbvs r3!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
     148:   64656e67    strbtvs r6, [r5], #-3687
     14c:   746e6920    strbtvc r6, [lr], #-2336
     150:   06010300    streq   r0, [r1], -r0, lsl #6
     154:   6e676973    mcrvs   9, 3, r6, cr7, cr3, {3}
     158:   63206465    teqvs   r0, #1694498816 ; 0x65000000
     15c:   00726168    rsbseq  r6, r2, r8, ror #2
     160:   6e697502    cdpvs   5, 6, cr7, cr9, cr2, {0}
     164:   745f3874    ldrbvc  r3, [pc], #2164 ; 16c <.debug_info+0x16c>
     168:   e02e0300    eor r0, lr, r0, lsl #6
     16c:   02000000    andeq   r0, r0, #0  ; 0x0
     170:   746e6975    strbtvc r6, [lr], #-2421
     174:   745f3631    ldrbvc  r3, [pc], #1585 ; 17c <.debug_info+0x17c>
     178:   213a0300    teqcs   sl, r0, lsl #6
     17c:   02000001    andeq   r0, r0, #1  ; 0x1
     180:   33746e69    cmncc   r4, #1680   ; 0x690
     184:   00745f32    rsbseq  r5, r4, r2, lsr pc
     188:   00b05303    adcseq  r5, r0, r3, lsl #6
     18c:   75020000    strvc   r0, [r2]
     190:   33746e69    cmncc   r4, #1680   ; 0x690
     194:   00745f32    rsbseq  r5, r4, r2, lsr pc
     198:   00945403    addseq  r5, r4, r3, lsl #8
     19c:   75020000    strvc   r0, [r2]
     1a0:   5f746e69    svcpl   0x00746e69
     1a4:   74736166    ldrbtvc r6, [r3], #-358
     1a8:   745f3631    ldrbvc  r3, [pc], #1585 ; 1b0 <.debug_info+0x1b0>
     1ac:   cdaa0300    stcgt   3, cr0, [sl]
     1b0:   02000000    andeq   r0, r0, #0  ; 0x0
     1b4:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
     1b8:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
     1bc:   6e61485f    mcrvs   8, 3, r4, cr1, cr15, {2}
     1c0:   54656c64    strbtpl r6, [r5], #-3172
     1c4:   f1470400    undefined instruction 0xf1470400
     1c8:   09000000    stmdbeq r0, {}
     1cc:   25530404    ldrbcs  r0, [r3, #-1028]
     1d0:   0a000002    beq 1e0 <.debug_info+0x1e0>
     1d4:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
     1d8:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
     1dc:   5253425f    subspl  r4, r3, #-268435451 ; 0xf0000005
     1e0:   4558455f    ldrbmi  r4, [r8, #-1375]
     1e4:   4e555f43    cdpmi   15, 5, cr5, cr5, cr3, {2}
     1e8:   49464544    stmdbmi r6, {r2, r6, r8, sl, lr}^
     1ec:   0044454e    subeq   r4, r4, lr, asr #10
     1f0:   41460a00    cmpmi   r6, r0, lsl #20
     1f4:   535f4950    cmppl   pc, #1310720    ; 0x140000
     1f8:   425f5359    subsmi  r5, pc, #1677721601 ; 0x64000001
     1fc:   455f5253    ldrbmi  r5, [pc, #-595] ; ffffffb1 <MGC_DrcDumpPipe+0xffffbd5d>
     200:   5f434558    svcpl   0x00434558
     204:   45524854    ldrbmi  r4, [r2, #-2132]
     208:   01004441    tsteq   r0, r1, asr #8
     20c:   5041460a    subpl   r4, r1, sl, lsl #12
     210:   59535f49    ldmdbpl r3, {r0, r3, r6, r8, r9, sl, fp, ip, lr}^
     214:   53425f53    movtpl  r5, #12115  ; 0x2f53
     218:   58455f52    stmdapl r5, {r1, r4, r6, r8, r9, sl, fp, ip, lr}^
     21c:   495f4345    ldmdbmi pc, {r0, r2, r6, r8, r9, lr}^
     220:   02005152    andeq   r5, r0, #-2147483628    ; 0x80000014
     224:   41460200    cmpmi   r6, r0, lsl #4
     228:   535f4950    cmppl   pc, #1310720    ; 0x140000
     22c:   425f5359    subsmi  r5, pc, #1677721601 ; 0x64000001
     230:   78457273    stmdavc r5, {r0, r1, r4, r5, r6, r9, ip, sp, lr}^
     234:   74756365    ldrbtvc r6, [r5], #-869
     238:   456e6f69    strbmi  r6, [lr, #-3945]!
     23c:   546d756e    strbtpl r7, [sp], #-1390
     240:   cb7d0400    blgt    1f41248 <MGC_DrcDumpPipe+0x1f3cff4>
     244:   0b000001    bleq    250 <.debug_info+0x250>
     248:   178b0424    strne   r0, [fp, r4, lsr #8]
     24c:   0c000003    stceq   0, cr0, [r0], {3}
     250:   76697264    strbtvc r7, [r9], -r4, ror #4
     254:   614e7265    cmpvs   lr, r5, ror #4
     258:   7450656d    ldrbvc  r6, [r0], #-1389
     25c:   96040072    undefined
     260:   00000116    andeq   r0, r0, r6, lsl r1
     264:   0c002302    stceq   3, cr2, [r0], {2}
     268:   4e717269    cdpmi   2, 7, cr7, cr1, cr9, {3}
     26c:   a304006f    movwge  r0, #16495  ; 0x406f
     270:   0000017f    andeq   r0, r0, pc, ror r1
     274:   0c042302    stceq   3, cr2, [r4], {2}
     278:   45727362    ldrbmi  r7, [r2, #-866]!
     27c:   75636578    strbvc  r6, [r3, #-1400]!
     280:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
     284:   25b10400    ldrcs   r0, [r1, #1024]!
     288:   02000002    andeq   r0, r0, #2  ; 0x2
     28c:   690c0823    stmdbvs ip, {r0, r1, r5, fp}
     290:   4474696e    ldrbtmi r6, [r4], #-2414
     294:   65766972    ldrbvs  r6, [r6, #-2418]!
     298:   6e754672    mrcvs   6, 3, r4, cr5, cr2, {3}
     29c:   ce040063    cdpgt   0, 0, cr0, cr4, cr3, {3}
     2a0:   0000031d    andeq   r0, r0, sp, lsl r3
     2a4:   0c0c2302    stceq   3, cr2, [ip], {2}
     2a8:   74697865    strbtvc r7, [r9], #-2149
     2ac:   76697244    strbtvc r7, [r9], -r4, asr #4
     2b0:   75467265    strbvc  r7, [r6, #-613]
     2b4:   0400636e    streq   r6, [r0], #-878
     2b8:   0000f5de    ldrdeq  pc, [r0], -lr
     2bc:   10230200    eorne   r0, r3, r0, lsl #4
     2c0:   7273690d    rsbsvc  r6, r3, #212992 ; 0x34000
     2c4:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
     2c8:   01010400    tsteq   r1, r0, lsl #8
     2cc:   00000329    andeq   r0, r0, r9, lsr #6
     2d0:   0d142302    ldceq   3, cr2, [r4, #-8]
     2d4:   46727362    ldrbtmi r7, [r2], -r2, ror #6
     2d8:   00636e75    rsbeq   r6, r3, r5, ror lr
     2dc:   3b012304    blcc    48ef4 <MGC_DrcDumpPipe+0x44ca0>
     2e0:   02000003    andeq   r0, r0, #3  ; 0x3
     2e4:   620d1823    andvs   r1, sp, #2293760    ; 0x230000
     2e8:   74537273    ldrbvc  r7, [r3], #-627
     2ec:   536b6361    cmnpl   fp, #-2080374783    ; 0x84000001
     2f0:   00657a69    rsbeq   r7, r5, r9, ror #20
     2f4:   8e012e04    cdphi   14, 0, cr2, cr1, cr4, {0}
     2f8:   02000001    andeq   r0, r0, #1  ; 0x1
     2fc:   620d1c23    andvs   r1, sp, #8960   ; 0x2300
     300:   69467273    stmdbvs r6, {r0, r1, r4, r5, r6, r9, ip, sp, lr}^
     304:   65446f66    strbvs  r6, [r4, #-3942]
     308:   00687470    rsbeq   r7, r8, r0, ror r4
     30c:   8e013804    cdphi   8, 0, cr3, cr1, cr4, {0}
     310:   02000001    andeq   r0, r0, #1  ; 0x1
     314:   0e002023    cdpeq   0, 0, cr2, cr0, cr3, {1}
     318:   00017f01    andeq   r7, r1, r1, lsl #30
     31c:   17040700    strne   r0, [r4, -r0, lsl #14]
     320:   0e000003    cdpeq   0, 0, cr0, cr0, cr3, {0}
     324:   00018e01    andeq   r8, r1, r1, lsl #28
     328:   23040700    movwcs  r0, #18176  ; 0x4700
     32c:   0f000003    svceq   0x00000003
     330:   00033b01    andeq   r3, r3, r1, lsl #22
     334:   018e1000    orreq   r1, lr, r0
     338:   07000000    streq   r0, [r0, -r0]
     33c:   00032f04    andeq   r2, r3, r4, lsl #30
     340:   41461100    cmpmi   r6, r0, lsl #2
     344:   535f4950    cmppl   pc, #1310720    ; 0x140000
     348:   445f5359    ldrbmi  r5, [pc], #857  ; 350 <.debug_info+0x350>
     34c:   65766972    ldrbvs  r6, [r6, #-2418]!
     350:   04005472    streq   r5, [r0], #-1138
     354:   02470139    subeq   r0, r7, #1073741838 ; 0x4000000e
     358:   4c0b0000    stcmi   0, cr0, [fp], {0}
     35c:   059ff705    ldreq   pc, [pc, #1797] ; a69 <.debug_info+0xa69>
     360:   630c0000    movwvs  r0, #49152  ; 0xc000
     364:   6e6f4368    cdpvs   3, 6, cr4, cr15, cr8, {3}
     368:   00676966    rsbeq   r6, r7, r6, ror #18
     36c:   018ef805    orreq   pc, lr, r5, lsl #16
     370:   23020000    movwcs  r0, #8192   ; 0x2000
     374:   68630c00    stmdavs r3!, {sl, fp}^
     378:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
     37c:   05006874    streq   r6, [r0, #-2164]
     380:   00018ef9    strdeq  r8, [r1], -r9
     384:   04230200    strteq  r0, [r3], #-512
     388:   4c68630c    stclmi  3, cr6, [r8], #-48
     38c:   6464616c    strbtvs r6, [r4], #-364
     390:   fa050072    blx 140560 <MGC_DrcDumpPipe+0x13c30c>
     394:   0000018e    andeq   r0, r0, lr, lsl #3
     398:   0c082302    stceq   3, cr2, [r8], {2}
     39c:   64526863    ldrbvs  r6, [r2], #-2147
     3a0:   72646461    rsbvc   r6, r4, #1627389952 ; 0x61000000
     3a4:   646e455f    strbtvs r4, [lr], #-1375
     3a8:   536e6169    cmnpl   lr, #1073741850 ; 0x4000001a
     3ac:   00706177    rsbseq  r6, r0, r7, ror r1
     3b0:   018efb05    orreq   pc, lr, r5, lsl #22
     3b4:   23020000    movwcs  r0, #8192   ; 0x2000
     3b8:   68630c0c    stmdavs r3!, {r2, r3, sl, fp}^
     3bc:   64616452    strbtvs r6, [r1], #-1106
     3c0:   505f7264    subspl  r7, pc, r4, ror #4
     3c4:   70697265    rsbvc   r7, r9, r5, ror #4
     3c8:   61726568    cmnvs   r2, r8, ror #10
     3cc:   6464416c    strbtvs r4, [r4], #-364
     3d0:   73736572    cmnvc   r3, #478150656  ; 0x1c800000
     3d4:   8efc0500    cdphi   5, 15, cr0, cr12, cr0, {0}
     3d8:   02000001    andeq   r0, r0, #1  ; 0x1
     3dc:   630c1023    movwvs  r1, #49187  ; 0xc023
     3e0:   61645268    cmnvs   r4, r8, ror #4
     3e4:   5f726464    svcpl   0x00726464
     3e8:   7366664f    cmnvc   r6, #82837504   ; 0x4f00000
     3ec:   64417465    strbvs  r7, [r1], #-1125
     3f0:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
     3f4:   fd050073    stc2    0, cr0, [r5, #-460]
     3f8:   0000018e    andeq   r0, r0, lr, lsl #3
     3fc:   0c142302    ldceq   3, cr2, [r4], {2}
     400:   64526863    ldrbvs  r6, [r2], #-2147
     404:   656e696c    strbvs  r6, [lr, #-2412]!
     408:   6e694c5f    mcrvs   12, 3, r4, cr9, cr15, {2}
     40c:   05007365    streq   r7, [r0, #-869]
     410:   00018efe    strdeq  r8, [r1], -lr
     414:   18230200    stmdane r3!, {r9}
     418:   5268630c    rsbpl   r6, r8, #805306368  ; 0x30000000
     41c:   6e696c64    cdpvs   12, 6, cr6, cr9, cr4, {3}
     420:   6f4c5f65    svcvs   0x004c5f65
     424:   6e49706f    cdpvs   0, 4, cr7, cr9, cr15, {3}
     428:   6d657263    sfmvs   f7, 2, [r5, #-396]!
     42c:   00746e65    rsbseq  r6, r4, r5, ror #28
     430:   018eff05    orreq   pc, lr, r5, lsl #30
     434:   23020000    movwcs  r0, #8192   ; 0x2000
     438:   68630d1c    stmdavs r3!, {r2, r3, r4, r8, sl, fp}^
     43c:   6e696452    mcrvs   4, 3, r6, cr9, cr2, {2}
     440:   694c5f63    stmdbvs ip, {r0, r1, r5, r6, r8, r9, sl, fp, ip, lr}^
     444:   654c656e    strbvs  r6, [ip, #-1390]
     448:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
     44c:   01000500    tsteq   r0, r0, lsl #10
     450:   0000018e    andeq   r0, r0, lr, lsl #3
     454:   0d202302    stceq   3, cr2, [r0, #-8]!
     458:   64526863    ldrbvs  r6, [r2], #-2147
     45c:   5f636e69    svcpl   0x00636e69
     460:   656e694c    strbvs  r6, [lr, #-2380]!
     464:   72636e49    rsbvc   r6, r3, #1168   ; 0x490
     468:   6e656d65    cdpvs   13, 6, cr6, cr5, cr5, {3}
     46c:   01050074    tsteq   r5, r4, ror r0
     470:   00018e01    andeq   r8, r1, r1, lsl #28
     474:   24230200    strtcs  r0, [r3], #-512
     478:   5268630d    rsbpl   r6, r8, #872415232  ; 0x34000000
     47c:   61706c64    cmnvs   r0, r4, ror #24
     480:   5f726464    svcpl   0x00726464
     484:   706f6f4c    rsbvc   r6, pc, ip, asr #30
     488:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
     48c:   00737365    rsbseq  r7, r3, r5, ror #6
     490:   8e010205    cdphi   2, 0, cr0, cr1, cr5, {0}
     494:   02000001    andeq   r0, r0, #1  ; 0x1
     498:   630d2823    movwvs  r2, #55331  ; 0xd823
     49c:   61725768    cmnvs   r2, r8, ror #14
     4a0:   5f726464    svcpl   0x00726464
     4a4:   69646e45    stmdbvs r4!, {r0, r2, r6, r9, sl, fp, sp, lr}^
     4a8:   77536e61    ldrbvc  r6, [r3, -r1, ror #28]
     4ac:   05007061    streq   r7, [r0, #-97]
     4b0:   018e0103    orreq   r0, lr, r3, lsl #2
     4b4:   23020000    movwcs  r0, #8192   ; 0x2000
     4b8:   68630d2c    stmdavs r3!, {r2, r3, r5, r8, sl, fp}^
     4bc:   64617257    strbtvs r7, [r1], #-599
     4c0:   505f7264    subspl  r7, pc, r4, ror #4
     4c4:   70697265    rsbvc   r7, r9, r5, ror #4
     4c8:   61726568    cmnvs   r2, r8, ror #10
     4cc:   6464416c    strbtvs r4, [r4], #-364
     4d0:   73736572    cmnvc   r3, #478150656  ; 0x1c800000
     4d4:   01040500    tsteq   r4, r0, lsl #10
     4d8:   0000018e    andeq   r0, r0, lr, lsl #3
     4dc:   0d302302    ldceq   3, cr2, [r0, #-8]!
     4e0:   72576863    subsvc  r6, r7, #6488064    ; 0x630000
     4e4:   72646461    rsbvc   r6, r4, #1627389952 ; 0x61000000
     4e8:   66664f5f    uqsubaddxvs r4, r6, pc
     4ec:   41746573    cmnmi   r4, r3, ror r5
     4f0:   65726464    ldrbvs  r6, [r2, #-1124]!
     4f4:   05007373    streq   r7, [r0, #-883]
     4f8:   018e0105    orreq   r0, lr, r5, lsl #2
     4fc:   23020000    movwcs  r0, #8192   ; 0x2000
     500:   68630d34    stmdavs r3!, {r2, r4, r5, r8, sl, fp}^
     504:   696c7257    stmdbvs ip!, {r0, r1, r2, r4, r6, r9, ip, sp, lr}^
     508:   4c5f656e    cfldr64mi   mvdx6, [pc], {110}
     50c:   73656e69    cmnvc   r5, #1680   ; 0x690
     510:   01060500    tsteq   r6, r0, lsl #10
     514:   0000018e    andeq   r0, r0, lr, lsl #3
     518:   0d382302    ldceq   3, cr2, [r8, #-8]!
     51c:   72576863    subsvc  r6, r7, #6488064    ; 0x630000
     520:   656e696c    strbvs  r6, [lr, #-2412]!
     524:   6f6f4c5f    svcvs   0x006f4c5f
     528:   636e4970    cmnvs   lr, #1835008    ; 0x1c0000
     52c:   656d6572    strbvs  r6, [sp, #-1394]!
     530:   0500746e    streq   r7, [r0, #-1134]
     534:   018e0107    orreq   r0, lr, r7, lsl #2
     538:   23020000    movwcs  r0, #8192   ; 0x2000
     53c:   68630d3c    stmdavs r3!, {r2, r3, r4, r5, r8, sl, fp}^
     540:   6e697257    mcrvs   2, 3, r7, cr9, cr7, {2}
     544:   694c5f63    stmdbvs ip, {r0, r1, r5, r6, r8, r9, sl, fp, ip, lr}^
     548:   654c656e    strbvs  r6, [ip, #-1390]
     54c:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
     550:   01080500    tsteq   r8, r0, lsl #10
     554:   0000018e    andeq   r0, r0, lr, lsl #3
     558:   0d402302    stcleq  3, cr2, [r0, #-8]
     55c:   72576863    subsvc  r6, r7, #6488064    ; 0x630000
     560:   5f636e69    svcpl   0x00636e69
     564:   656e694c    strbvs  r6, [lr, #-2380]!
     568:   72636e49    rsbvc   r6, r3, #1168   ; 0x490
     56c:   6e656d65    cdpvs   13, 6, cr6, cr5, cr5, {3}
     570:   09050074    stmdbeq r5, {r2, r4, r5, r6}
     574:   00018e01    andeq   r8, r1, r1, lsl #28
     578:   44230200    strtmi  r0, [r3], #-512
     57c:   5768630d    strbpl  r6, [r8, -sp, lsl #6]!
     580:   61706c72    cmnvs   r0, r2, ror ip
     584:   5f726464    svcpl   0x00726464
     588:   706f6f4c    rsbvc   r6, pc, ip, asr #30
     58c:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
     590:   00737365    rsbseq  r7, r3, r5, ror #6
     594:   8e010a05    fmacshi s0, s2, s10
     598:   02000001    andeq   r0, r0, #1  ; 0x1
     59c:   11004823    tstne   r0, r3, lsr #16
     5a0:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
     5a4:   414d445f    cmpmi   sp, pc, asr r4
     5a8:   6168435f    cmnvs   r8, pc, asr r3
     5ac:   6c656e6e    stclvs  14, cr6, [r5], #-440
     5b0:   666e6f43    strbtvs r6, [lr], -r3, asr #30
     5b4:   00546769    subseq  r6, r4, r9, ror #14
     5b8:   5a010c05    bpl 435d4 <MGC_DrcDumpPipe+0x3f380>
     5bc:   11000003    tstne   r0, r3
     5c0:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
     5c4:   414d445f    cmpmi   sp, pc, asr r4
     5c8:   7165525f    cmnvc   r5, pc, asr r2
     5cc:   74736575    ldrbtvc r6, [r3], #-1397
     5d0:   1e050054    mcrne   0, 0, r0, cr5, cr4, {2}
     5d4:   0005d901    andeq   sp, r5, r1, lsl #18
     5d8:   41461200    cmpmi   r6, r0, lsl #4
     5dc:   445f4950    ldrbmi  r4, [pc], #2384 ; 5e4 <.debug_info+0x5e4>
     5e0:   525f414d    subspl  r4, pc, #1073741843 ; 0x40000013
     5e4:   65757165    ldrbvs  r7, [r5, #-357]!
     5e8:   00537473    subseq  r7, r3, r3, ror r4
     5ec:   011d058c    tsteq   sp, ip, lsl #11
     5f0:   00000797    muleq   r0, r7, r7
     5f4:   6165660d    cmnvs   r5, sp, lsl #12
     5f8:   65727574    ldrbvs  r7, [r2, #-1396]!
     5fc:   014b0500    cmpeq   fp, r0, lsl #10
     600:   0000018e    andeq   r0, r0, lr, lsl #3
     604:   0d002302    stceq   3, cr2, [r0, #-8]
     608:   74617473    strbtvc r7, [r1], #-1139
     60c:   05007375    streq   r7, [r0, #-885]
     610:   018e0153    orreq   r0, lr, r3, asr r1
     614:   23020000    movwcs  r0, #8192   ; 0x2000
     618:   68630d04    stmdavs r3!, {r2, r8, sl, fp}^
     61c:   656e6e61    strbvs  r6, [lr, #-3681]!
     620:   5a05006c    bpl 1407d8 <MGC_DrcDumpPipe+0x13c584>
     624:   00018e01    andeq   r8, r1, r1, lsl #28
     628:   08230200    stmdaeq r3!, {r9}
     62c:   7365720d    cmnvc   r5, #-805306368 ; 0xd0000000
     630:   00746c75    rsbseq  r6, r4, r5, ror ip
     634:   7f016405    svcvc   0x00016405
     638:   02000001    andeq   r0, r0, #1  ; 0x1
     63c:   730d0c23    movwvc  r0, #56355  ; 0xdc23
     640:   44666c65    strbtmi r6, [r6], #-3173
     644:   72747365    rsbsvc  r7, r4, #-1811939327    ; 0x94000001
     648:   0500796f    streq   r7, [r0, #-2415]
     64c:   018e016e    orreq   r0, lr, lr, ror #2
     650:   23020000    movwcs  r0, #8192   ; 0x2000
     654:   68630d10    stmdavs r3!, {r4, r8, sl, fp}^
     658:   656e6e61    strbvs  r6, [lr, #-3681]!
     65c:   6e6f436c    cdpvs   3, 6, cr4, cr15, cr12, {3}
     660:   00676966    rsbeq   r6, r7, r6, ror #18
     664:   9f017305    svcls   0x00017305
     668:   02000005    andeq   r0, r0, #5  ; 0x5
     66c:   700d1423    andvc   r1, sp, r3, lsr #8
     670:   6e456572    mcrvs   5, 2, r6, cr5, cr2, {3}
     674:   656c6261    strbvs  r6, [ip, #-609]!
     678:   6c6c6143    stfvse  f6, [ip], #-268
     67c:   6b636162    blvs    18d8c0c <MGC_DrcDumpPipe+0x18d49b8>
     680:   017c0500    cmneq   ip, r0, lsl #10
     684:   00000797    muleq   r0, r7, r7
     688:   0d602302    stcleq  3, cr2, [r0, #-8]!
     68c:   74736f70    ldrbtvc r6, [r3], #-3952
     690:   62616e45    rsbvs   r6, r1, #1104   ; 0x450
     694:   6143656c    cmpvs   r3, ip, ror #10
     698:   61626c6c    cmnvs   r2, ip, ror #24
     69c:   05006b63    streq   r6, [r0, #-2915]
     6a0:   07970185    ldreq   r0, [r7, r5, lsl #3]
     6a4:   23020000    movwcs  r0, #8192   ; 0x2000
     6a8:   6f700d64    svcvs   0x00700d64
     6ac:   69467473    stmdbvs r6, {r0, r1, r4, r5, r6, sl, ip, sp, lr}^
     6b0:   6873696e    ldmdavs r3!, {r1, r2, r3, r5, r6, r8, fp, sp, lr}^
     6b4:   6c6c6143    stfvse  f6, [ip], #-268
     6b8:   6b636162    blvs    18d8c48 <MGC_DrcDumpPipe+0x18d49f4>
     6bc:   018d0500    orreq   r0, sp, r0, lsl #10
     6c0:   00000797    muleq   r0, r7, r7
     6c4:   0d682302    stcleq  3, cr2, [r8, #-8]!
     6c8:   4874706f    ldmdami r4!, {r0, r1, r2, r3, r5, r6, ip, sp, lr}^
     6cc:   6c646e61    stclvs  14, cr6, [r4], #-388
     6d0:   92050065    andls   r0, r5, #101    ; 0x65
     6d4:   0001b301    andeq   fp, r1, r1, lsl #6
     6d8:   6c230200    sfmvs   f0, 4, [r3]
     6dc:   74706f0d    ldrbtvc r6, [r0], #-3853
     6e0:   61746144    cmnvs   r4, r4, asr #2
     6e4:   019a0500    orrseq  r0, sl, r0, lsl #10
     6e8:   000000f1    strdeq  r0, [r0], -r1
     6ec:   0d702302    ldcleq  3, cr2, [r0, #-8]!
     6f0:   4674706f    ldrbtmi r7, [r4], -pc, rrx
     6f4:   73696e69    cmnvc   r9, #1680   ; 0x690
     6f8:   6c614368    stclvs  3, cr4, [r1], #-416
     6fc:   6361626c    cmnvs   r1, #-1073741818    ; 0xc0000006
     700:   a105006b    tstge   r5, fp, rrx
     704:   0007ca01    andeq   ip, r7, r1, lsl #20
     708:   74230200    strtvc  r0, [r3], #-512
     70c:   74706f0d    ldrbtvc r6, [r0], #-3853
     710:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
     714:   61436567    cmpvs   r3, r7, ror #10
     718:   61626c6c    cmnvs   r2, ip, ror #24
     71c:   05006b63    streq   r6, [r0, #-2915]
     720:   07ca01aa    strbeq  r0, [sl, sl, lsr #3]
     724:   23020000    movwcs  r0, #8192   ; 0x2000
     728:   75610d78    strbvc  r0, [r1, #-3448]!
     72c:   65526f74    ldrbvs  r6, [r2, #-3956]
     730:   72617473    rsbvc   r7, r1, #1929379840 ; 0x73000000
     734:   b9050074    stmdblt r5, {r2, r4, r5, r6}
     738:   00018e01    andeq   r8, r1, r1, lsl #28
     73c:   7c230200    sfmvc   f0, 4, [r3]
     740:   74706f0d    ldrbtvc r6, [r0], #-3853
     744:   696c7053    stmdbvs ip!, {r0, r1, r4, r6, ip, sp, lr}^
     748:   756f4374    strbvc  r4, [pc, #-884]!    ; 3dc <.debug_info+0x3dc>
     74c:   0500746e    streq   r7, [r0, #-1134]
     750:   018e01bf    strheq  r0, [lr, pc]
     754:   23030000    movwcs  r0, #12288  ; 0x3000
     758:   630d0180    movwvs  r0, #53632  ; 0xd180
     75c:   6c706d6f    ldclvs  13, cr6, [r0], #-444
     760:   53657465    cmnpl   r5, #1694498816 ; 0x65000000
     764:   74696c70    strbtvc r6, [r9], #-3184
     768:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
     76c:   c6050074    undefined
     770:   00018e01    andeq   r8, r1, r1, lsl #28
     774:   84230300    strthi  r0, [r3], #-768
     778:   706f0d01    rsbvc   r0, pc, r1, lsl #26
     77c:   6c705374    ldclvs  3, cr5, [r0], #-464
     780:   61437469    cmpvs   r3, r9, ror #8
     784:   61626c6c    cmnvs   r2, ip, ror #24
     788:   05006b63    streq   r6, [r0, #-2915]
     78c:   07ca01ce    strbeq  r0, [sl, lr, asr #3]
     790:   23030000    movwcs  r0, #12288  ; 0x3000
     794:   11000188    smlabbne    r0, r8, r1, r0
     798:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
     79c:   414d445f    cmpmi   sp, pc, asr r4
     7a0:   6c61435f    stclvs  3, cr4, [r1], #-380
     7a4:   6361626c    cmnvs   r1, #-1073741818    ; 0xc0000006
     7a8:   0500546b    streq   r5, [r0, #-1131]
     7ac:   07b2011f    undefined
     7b0:   04070000    streq   r0, [r7]
     7b4:   000007b8    strheq  r0, [r0], -r8
     7b8:   07c4010f    strbeq  r0, [r4, pc, lsl #2]
     7bc:   c4100000    ldrgt   r0, [r0]
     7c0:   00000007    andeq   r0, r0, r7
     7c4:   05bf0407    ldreq   r0, [pc, #1031]!    ; bd3 <.debug_info+0xbd3>
     7c8:   46110000    ldrmi   r0, [r1], -r0
     7cc:   5f495041    svcpl   0x00495041
     7d0:   5f414d44    svcpl   0x00414d44
     7d4:   4374704f    cmnmi   r4, #79 ; 0x4f
     7d8:   626c6c61    rsbvs   r6, ip, #24832  ; 0x6100
     7dc:   546b6361    strbtpl r6, [fp], #-865
     7e0:   01310500    teqeq   r1, r0, lsl #10
     7e4:   000007e8    andeq   r0, r0, r8, ror #15
     7e8:   07ee0407    strbeq  r0, [lr, r7, lsl #8]!
     7ec:   010f0000    mrseq   r0, CPSR
     7f0:   000007ff    strdeq  r0, [r0], -pc
     7f4:   00017f10    andeq   r7, r1, r0, lsl pc
     7f8:   018e1000    orreq   r1, lr, r0
     7fc:   07000000    streq   r0, [r0, -r0]
     800:   00080504    andeq   r0, r8, r4, lsl #10
     804:   11010f00    tstne   r1, r0, lsl #30
     808:   10000008    andne   r0, r0, r8
     80c:   000000f1    strdeq  r0, [r0], -r1
     810:   554d0200    strbpl  r0, [sp, #-512]
     814:   705f4253    subsvc  r4, pc, r3, asr r2
     818:   6d695466    cfstrdvs    mvd5, [r9, #-408]!
     81c:   78457265    stmdavc r5, {r0, r2, r5, r6, r9, ip, sp, lr}^
     820:   65726970    ldrbvs  r6, [r2, #-2416]!
     824:   58060064    stmdapl r6, {r2, r5, r6}
     828:   0000082c    andeq   r0, r0, ip, lsr #16
     82c:   08320407    ldmdaeq r2!, {r0, r1, r2, sl}
     830:   010f0000    mrseq   r0, CPSR
     834:   00000843    andeq   r0, r0, r3, asr #16
     838:   0000f110    andeq   pc, r0, r0, lsl r1
     83c:   016f1000    cmneq   pc, r0
     840:   02000000    andeq   r0, r0, #0  ; 0x0
     844:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     848:   4d66705f    stclmi  0, cr7, [r6, #-380]!
     84c:   61737365    cmnvs   r3, r5, ror #6
     850:   74536567    ldrbvc  r6, [r3], #-1383
     854:   676e6972    undefined
     858:   5f9a0600    svcpl   0x009a0600
     85c:   07000008    streq   r0, [r0, -r8]
     860:   00086504    andeq   r6, r8, r4, lsl #10
     864:   60011300    andvs   r1, r1, r0, lsl #6
     868:   7f000001    svcvc   0x00000001
     86c:   10000008    andne   r0, r0, r8
     870:   00000108    andeq   r0, r0, r8, lsl #2
     874:   00016f10    andeq   r6, r1, r0, lsl pc
     878:   01161000    tsteq   r6, r0
     87c:   02000000    andeq   r0, r0, #0  ; 0x0
     880:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     884:   4d66705f    stclmi  0, cr7, [r6, #-380]!
     888:   61737365    cmnvs   r3, r5, ror #6
     88c:   754e6567    strbvc  r6, [lr, #-1383]
     890:   7265626d    rsbvc   r6, r5, #-805306362 ; 0xd0000006
     894:   9bae0600    blls    feb8209c <MGC_DrcDumpPipe+0xfeb7de48>
     898:   07000008    streq   r0, [r0, -r8]
     89c:   0008a104    andeq   sl, r8, r4, lsl #2
     8a0:   60011300    andvs   r1, r1, r0, lsl #6
     8a4:   c5000001    strgt   r0, [r0, #-1]
     8a8:   10000008    andne   r0, r0, r8
     8ac:   00000108    andeq   r0, r0, r8, lsl #2
     8b0:   00016f10    andeq   r6, r1, r0, lsl pc
     8b4:   018e1000    orreq   r1, lr, r0
     8b8:   60100000    andsvs  r0, r0, r0
     8bc:   10000001    andne   r0, r0, r1
     8c0:   00000160    andeq   r0, r0, r0, ror #2
     8c4:   554d0200    strbpl  r0, [sp, #-512]
     8c8:   705f4253    subsvc  r4, pc, r3, asr r2
     8cc:   74654766    strbtvc r4, [r5], #-1894
     8d0:   656d6954    strbvs  r6, [sp, #-2388]!
     8d4:   29b80600    ldmibcs r8!, {r9, sl}
     8d8:   0b000003    bleq    8ec <.debug_info+0x8ec>
     8dc:   3ad00610    bcc ff402124 <MGC_DrcDumpPipe+0xff3fded0>
     8e0:   14000009    strne   r0, [r0], #-9
     8e4:   00000134    andeq   r0, r0, r4, lsr r1
            8e4: R_ARM_ABS32    .debug_str
     8e8:   016fd106    cmneq   pc, r6, lsl #2
     8ec:   23020000    movwcs  r0, #8192   ; 0x2000
     8f0:   66700c00    ldrbtvs r0, [r0], -r0, lsl #24
     8f4:   7373654d    cmnvc   r3, #322961408  ; 0x13400000
     8f8:   53656761    cmnpl   r5, #25427968   ; 0x1840000
     8fc:   6e697274    mcrvs   2, 3, r7, cr9, cr4, {3}
     900:   d2060067    andle   r0, r6, #103    ; 0x67
     904:   00000843    andeq   r0, r0, r3, asr #16
     908:   0c042302    stceq   3, cr2, [r4], {2}
     90c:   654d6670    strbvs  r6, [sp, #-1648]
     910:   67617373    undefined
     914:   6d754e65    ldclvs  14, cr4, [r5, #-404]!
     918:   00726562    rsbseq  r6, r2, r2, ror #10
     91c:   087fd306    ldmdaeq pc!, {r1, r2, r8, r9, ip, lr, pc}^
     920:   23020000    movwcs  r0, #8192   ; 0x2000
     924:   66700c08    ldrbtvs r0, [r0], -r8, lsl #24
     928:   54746547    ldrbtpl r6, [r4], #-1351
     92c:   00656d69    rsbeq   r6, r5, r9, ror #26
     930:   08c5d406    stmiaeq r5, {r1, r2, sl, ip, lr, pc}^
     934:   23020000    movwcs  r0, #8192   ; 0x2000
     938:   4d02000c    stcmi   0, cr0, [r2, #-48]
     93c:   5f425355    svcpl   0x00425355
     940:   74737953    ldrbtvc r7, [r3], #-2387
     944:   74556d65    ldrbvc  r6, [r5], #-3429
     948:   00736c69    rsbseq  r6, r3, r9, ror #24
     94c:   08dbd506    ldmeq   fp, {r1, r2, r8, sl, ip, lr, pc}^
     950:   4d020000    stcmi   0, cr0, [r2]
     954:   5f425355    svcpl   0x00425355
     958:   79536670    ldmdbvc r3, {r4, r5, r6, r9, sl, sp, lr}^
     95c:   6d657473    cfstrdvs    mvd7, [r5, #-460]!
     960:   75426f54    strbvc  r6, [r2, #-3924]
     964:   64644173    strbtvs r4, [r4], #-371
     968:   73736572    cmnvc   r3, #478150656  ; 0x1c800000
     96c:   73e10600    mvnvc   r0, #0  ; 0x0
     970:   07000009    streq   r0, [r0, -r9]
     974:   00097904    andeq   r7, r9, r4, lsl #18
     978:   f1011300    undefined instruction 0xf1011300
     97c:   8e000000    cdphi   0, 0, cr0, cr0, cr0, {0}
     980:   10000009    andne   r0, r0, r9
     984:   000000f1    strdeq  r0, [r0], -r1
     988:   00098e10    andeq   r8, r9, r0, lsl lr
     98c:   04070000    streq   r0, [r7]
     990:   00000994    muleq   r0, r4, r9
     994:   554d0215    strbpl  r0, [sp, #-533]
     998:   705f4253    subsvc  r4, pc, r3, asr r2
     99c:   65755166    ldrbvs  r5, [r5, #-358]!
     9a0:   61426575    cmpvs   r2, r5, ror r5
     9a4:   72676b63    rsbvc   r6, r7, #101376 ; 0x18c00
     9a8:   646e756f    strbtvs r7, [lr], #-1391
     9ac:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
     9b0:   b7ee0600    strblt  r0, [lr, r0, lsl #12]!
     9b4:   07000009    streq   r0, [r0, -r9]
     9b8:   0009bd04    andeq   fp, r9, r4, lsl #26
     9bc:   60011300    andvs   r1, r1, r0, lsl #6
     9c0:   d2000001    andle   r0, r0, #1  ; 0x1
     9c4:   10000009    andne   r0, r0, r9
     9c8:   000000f1    strdeq  r0, [r0], -r1
     9cc:   00098e10    andeq   r8, r9, r0, lsl lr
     9d0:   4d020000    stcmi   0, cr0, [r2]
     9d4:   5f425355    svcpl   0x00425355
     9d8:   65446670    strbvs  r6, [r4, #-1648]
     9dc:   75657571    strbvc  r7, [r5, #-1393]!
     9e0:   63614265    cmnvs   r1, #1342177286 ; 0x50000006
     9e4:   6f72676b    svcvs   0x0072676b
     9e8:   49646e75    stmdbmi r4!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
     9ec:   006d6574    rsbeq   r6, sp, r4, ror r5
     9f0:   09f6fb06    ldmibeq r6!, {r1, r2, r8, r9, fp, ip, sp, lr, pc}^
     9f4:   04070000    streq   r0, [r7]
     9f8:   000009fc    strdeq  r0, [r0], -ip
     9fc:   01600113    cmneq   r0, r3, lsl r1
     a00:   0a110000    beq 440a08 <MGC_DrcDumpPipe+0x43c7b4>
     a04:   f1100000    undefined instruction 0xf1100000
     a08:   10000000    andne   r0, r0, r0
     a0c:   000000f1    strdeq  r0, [r0], -r1
     a10:   554d1100    strbpl  r1, [sp, #-256]
     a14:   705f4253    subsvc  r4, pc, r3, asr r2
     a18:   756c4666    strbvc  r4, [ip, #-1638]!
     a1c:   61426873    cmpvs   r2, r3, ror r8
     a20:   72676b63    rsbvc   r6, r7, #101376 ; 0x18c00
     a24:   646e756f    strbtvs r7, [lr], #-1391
     a28:   75657551    strbvc  r7, [r5, #-1361]!
     a2c:   03060065    movweq  r0, #24677  ; 0x6065
     a30:   0007ff01    andeq   pc, r7, r1, lsl #30
     a34:   554d1100    strbpl  r1, [sp, #-256]
     a38:   705f4253    subsvc  r4, pc, r3, asr r2
     a3c:   6d724166    ldfvse  f4, [r2, #-408]!
     a40:   656d6954    strbvs  r6, [sp, #-2388]!
     a44:   12060072    andne   r0, r6, #114    ; 0x72
     a48:   000a4d01    andeq   r4, sl, r1, lsl #26
     a4c:   53040700    movwpl  r0, #18176  ; 0x4700
     a50:   1300000a    movwne  r0, #10 ; 0xa
     a54:   00016001    andeq   r6, r1, r1
     a58:   000a7700    andeq   r7, sl, r0, lsl #14
     a5c:   00f11000    rscseq  r1, r1, r0
     a60:   6f100000    svcvs   0x00100000
     a64:   10000001    andne   r0, r0, r1
     a68:   0000018e    andeq   r0, r0, lr, lsl #3
     a6c:   00016010    andeq   r6, r1, r0, lsl r0
     a70:   08111000    ldmdaeq r1, {ip}
     a74:   11000000    tstne   r0, r0
     a78:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     a7c:   4366705f    cmnmi   r6, #95 ; 0x5f
     a80:   65636e61    strbvs  r6, [r3, #-3681]!
     a84:   6d69546c    cfstrdvs    mvd5, [r9, #-432]!
     a88:   06007265    streq   r7, [r0], -r5, ror #4
     a8c:   0a92011d    beq fe480f08 <MGC_DrcDumpPipe+0xfe47ccb4>
     a90:   04070000    streq   r0, [r7]
     a94:   00000a98    muleq   r0, r8, sl
     a98:   01600113    cmneq   r0, r3, lsl r1
     a9c:   0aad0000    beq feb40aa4 <MGC_DrcDumpPipe+0xfeb3c850>
     aa0:   f1100000    undefined instruction 0xf1100000
     aa4:   10000000    andne   r0, r0, r0
     aa8:   0000016f    andeq   r0, r0, pc, ror #2
     aac:   554d1100    strbpl  r1, [sp, #-256]
     ab0:   705f4253    subsvc  r4, pc, r3, asr r2
     ab4:   636f4c66    cmnvs   pc, #26112  ; 0x6600
     ab8:   2806006b    stmdacs r6, {r0, r1, r3, r5, r6}
     abc:   000a9201    andeq   r9, sl, r1, lsl #4
     ac0:   554d1100    strbpl  r1, [sp, #-256]
     ac4:   705f4253    subsvc  r4, pc, r3, asr r2
     ac8:   6c6e5566    cfstr64vs   mvdx5, [lr], #-408
     acc:   006b636f    rsbeq   r6, fp, pc, ror #6
     ad0:   92013306    andls   r3, r1, #402653184  ; 0x18000000
     ad4:   1100000a    tstne   r0, sl
     ad8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     adc:   5066705f    rsbpl   r7, r6, pc, asr r0
     ae0:   746e6972    strbtvc r6, [lr], #-2418
     ae4:   67616944    strbvs  r6, [r1, -r4, asr #18]!
     ae8:   013e0600    teqeq   lr, r0, lsl #12
     aec:   00000af0    strdeq  r0, [r0], -r0
     af0:   0af60407    beq ffd81b14 <MGC_DrcDumpPipe+0xffd7d8c0>
     af4:   01130000    tsteq   r3, r0
     af8:   00000160    andeq   r0, r0, r0, ror #2
     afc:   00000b0b    andeq   r0, r0, fp, lsl #22
     b00:   0000f110    andeq   pc, r0, r0, lsl r1
     b04:   01161000    tsteq   r6, r0
     b08:   11000000    tstne   r0, r0
     b0c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     b10:   4e66705f    mcrmi   0, 3, r7, cr6, cr15, {2}
     b14:   6f507765    svcvs   0x00507765
     b18:   4c726577    cfldr64mi   mvdx6, [r2], #-476
     b1c:   0064616f    rsbeq   r6, r4, pc, ror #2
     b20:   27015a06    strcs   r5, [r1, -r6, lsl #20]
     b24:   0700000b    streq   r0, [r0, -fp]
     b28:   000b2d04    andeq   r2, fp, r4, lsl #26
     b2c:   60011300    andvs   r1, r1, r0, lsl #6
     b30:   51000001    tstpl   r0, r1
     b34:   1000000b    andne   r0, r0, fp
     b38:   000000f1    strdeq  r0, [r0], -r1
     b3c:   00016f10    andeq   r6, r1, r0, lsl pc
     b40:   01601000    cmneq   r0, r0
     b44:   51100000    tstpl   r0, r0
     b48:   1000000b    andne   r0, r0, fp
     b4c:   00000160    andeq   r0, r0, r0, ror #2
     b50:   57040700    strpl   r0, [r4, -r0, lsl #14]
     b54:   0800000b    stmdaeq r0, {r0, r1, r3}
     b58:   00000160    andeq   r0, r0, r0, ror #2
     b5c:   53554d11    cmppl   r5, #1088   ; 0x440
     b60:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
     b64:   6f6d6552    svcvs   0x006d6552
     b68:   6f506576    svcvs   0x00506576
     b6c:   4c726577    cfldr64mi   mvdx6, [r2], #-476
     b70:   0064616f    rsbeq   r6, r4, pc, ror #2
     b74:   27016e06    strcs   r6, [r1, -r6, lsl #28]
     b78:   1600000b    strne   r0, [r0], -fp
     b7c:   01930634    orrseq  r0, r3, r4, lsr r6
     b80:   00000ccc    andeq   r0, r0, ip, asr #25
     b84:   00013417    andeq   r3, r1, r7, lsl r4
            b85: R_ARM_ABS32    .debug_str
     b88:   01940600    orrseq  r0, r4, r0, lsl #12
     b8c:   0000016f    andeq   r0, r0, pc, ror #2
     b90:   17002302    strne   r2, [r0, -r2, lsl #6]
     b94:   00000104    andeq   r0, r0, r4, lsl #2
            b94: R_ARM_ABS32    .debug_str
     b98:   f1019506    undefined instruction 0xf1019506
     b9c:   02000000    andeq   r0, r0, #0  ; 0x0
     ba0:   700d0423    andvc   r0, sp, r3, lsr #8
     ba4:   73795366    cmnvc   r9, #-1744830463    ; 0x98000001
     ba8:   546d6574    strbtpl r6, [sp], #-1396
     bac:   7375426f    cmnvc   r5, #-268435450 ; 0xf0000006
     bb0:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
     bb4:   00737365    rsbseq  r7, r3, r5, ror #6
     bb8:   52019606    andpl   r9, r1, #6291456    ; 0x600000
     bbc:   02000009    andeq   r0, r0, #9  ; 0x9
     bc0:   700d0823    andvc   r0, sp, r3, lsr #16
     bc4:   65755166    ldrbvs  r5, [r5, #-358]!
     bc8:   61426575    cmpvs   r2, r5, ror r5
     bcc:   72676b63    rsbvc   r6, r7, #101376 ; 0x18c00
     bd0:   646e756f    strbtvs r7, [lr], #-1391
     bd4:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
     bd8:   01970600    orrseq  r0, r7, r0, lsl #12
     bdc:   00000995    muleq   r0, r5, r9
     be0:   0d0c2302    stceq   3, cr2, [ip, #-8]
     be4:   65446670    strbvs  r6, [r4, #-1648]
     be8:   75657571    strbvc  r7, [r5, #-1393]!
     bec:   63614265    cmnvs   r1, #1342177286 ; 0x50000006
     bf0:   6f72676b    svcvs   0x0072676b
     bf4:   49646e75    stmdbmi r4!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
     bf8:   006d6574    rsbeq   r6, sp, r4, ror r5
     bfc:   d2019806    andle   r9, r1, #393216 ; 0x60000
     c00:   02000009    andeq   r0, r0, #9  ; 0x9
     c04:   700d1023    andvc   r1, sp, r3, lsr #32
     c08:   756c4666    strbvc  r4, [ip, #-1638]!
     c0c:   61426873    cmpvs   r2, r3, ror r8
     c10:   72676b63    rsbvc   r6, r7, #101376 ; 0x18c00
     c14:   646e756f    strbtvs r7, [lr], #-1391
     c18:   75657551    strbvc  r7, [r5, #-1361]!
     c1c:   99060065    stmdbls r6, {r0, r2, r5, r6}
     c20:   000a1101    andeq   r1, sl, r1, lsl #2
     c24:   14230200    strtne  r0, [r3], #-512
     c28:   4166700d    cmnmi   r6, sp
     c2c:   69546d72    ldmdbvs r4, {r1, r4, r5, r6, r8, sl, fp, sp, lr}^
     c30:   0072656d    rsbseq  r6, r2, sp, ror #10
     c34:   35019a06    strcc   r9, [r1, #-2566]
     c38:   0200000a    andeq   r0, r0, #10 ; 0xa
     c3c:   700d1823    andvc   r1, sp, r3, lsr #16
     c40:   6e614366    cdpvs   3, 6, cr4, cr1, cr6, {3}
     c44:   546c6563    strbtpl r6, [ip], #-1379
     c48:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
     c4c:   019b0600    orrseq  r0, fp, r0, lsl #12
     c50:   00000a77    andeq   r0, r0, r7, ror sl
     c54:   0d1c2302    ldceq   3, cr2, [ip, #-8]
     c58:   6f4c6670    svcvs   0x004c6670
     c5c:   06006b63    streq   r6, [r0], -r3, ror #22
     c60:   0aad019c    beq feb412d8 <MGC_DrcDumpPipe+0xfeb3d084>
     c64:   23020000    movwcs  r0, #8192   ; 0x2000
     c68:   66700d20    ldrbtvs r0, [r0], -r0, lsr #26
     c6c:   6f6c6e55    svcvs   0x006c6e55
     c70:   06006b63    streq   r6, [r0], -r3, ror #22
     c74:   0ac1019d    beq ff0412f0 <MGC_DrcDumpPipe+0xff03d09c>
     c78:   23020000    movwcs  r0, #8192   ; 0x2000
     c7c:   66700d24    ldrbtvs r0, [r0], -r4, lsr #26
     c80:   6e697250    mcrvs   2, 3, r7, cr9, cr0, {2}
     c84:   61694474    smcvs   37956
     c88:   9e060067    cdpls   0, 0, cr0, cr6, cr7, {3}
     c8c:   000ad701    andeq   sp, sl, r1, lsl #14
     c90:   28230200    stmdacs r3!, {r9}
     c94:   4e66700d    cdpmi   0, 6, cr7, cr6, cr13, {0}
     c98:   6f507765    svcvs   0x00507765
     c9c:   4c726577    cfldr64mi   mvdx6, [r2], #-476
     ca0:   0064616f    rsbeq   r6, r4, pc, ror #2
     ca4:   0b019f06    bleq    688c4 <MGC_DrcDumpPipe+0x64670>
     ca8:   0200000b    andeq   r0, r0, #11 ; 0xb
     cac:   700d2c23    andvc   r2, sp, r3, lsr #24
     cb0:   6d655266    sfmvs   f5, 2, [r5, #-408]!
     cb4:   5065766f    rsbpl   r7, r5, pc, ror #12
     cb8:   7265776f    rsbvc   r7, r5, #29097984   ; 0x1bc0000
     cbc:   64616f4c    strbtvs r6, [r1], #-3916
     cc0:   01a00600    lsleq   r0, r0, #12
     cc4:   00000b5c    andeq   r0, r0, ip, asr fp
     cc8:   00302302    eorseq  r2, r0, r2, lsl #6
     ccc:   53554d11    cmppl   r5, #1088   ; 0x440
     cd0:   79535f42    ldmdbvc r3, {r1, r6, r8, r9, sl, fp, ip, lr}^
     cd4:   6d657473    cfstrdvs    mvd7, [r5, #-460]!
     cd8:   76726553    undefined
     cdc:   73656369    cmnvc   r5, #-1543503871    ; 0xa4000001
     ce0:   01a10600    undefined instruction 0x01a10600
     ce4:   00000b7b    andeq   r0, r0, fp, ror fp
     ce8:   ad07080b    stcge   8, cr0, [r7, #-44]
     cec:   00000d4d    andeq   r0, r0, sp, asr #26
     cf0:   526d620c    rsbpl   r6, sp, #-1073741824    ; 0xc0000000
     cf4:   65757165    ldrbvs  r7, [r5, #-357]!
     cf8:   79547473    ldmdbvc r4, {r0, r1, r4, r5, r6, sl, ip, sp, lr}^
     cfc:   07006570    smlsdxeq    r0, r0, r5, r6
     d00:   000160ae    andeq   r6, r1, lr, lsr #1
     d04:   00230200    eoreq   r0, r3, r0, lsl #4
     d08:   6552620c    ldrbvs  r6, [r2, #-524]
     d0c:   73657571    cmnvc   r5, #473956352  ; 0x1c400000
     d10:   af070074    svcge   0x00070074
     d14:   00000160    andeq   r0, r0, r0, ror #2
     d18:   0c012302    stceq   3, cr2, [r1], {2}
     d1c:   6c615677    stclvs  6, cr5, [r1], #-476
     d20:   07006575    smlsdxeq    r0, r5, r5, r6
     d24:   00016fb0    strheq  r6, [r1], -r0
     d28:   02230200    eoreq   r0, r3, #0  ; 0x0
     d2c:   0000ba14    andeq   fp, r0, r4, lsl sl
            d2d: R_ARM_ABS32    .debug_str
     d30:   6fb10700    svcvs   0x00b10700
     d34:   02000001    andeq   r0, r0, #1  ; 0x1
     d38:   770c0423    strvc   r0, [ip, -r3, lsr #8]
     d3c:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
     d40:   07006874    smlsdxeq    r0, r4, r8, r6
     d44:   00016fb2    strheq  r6, [r1], -r2
     d48:   06230200    strteq  r0, [r3], -r0, lsl #4
     d4c:   554d0200    strbpl  r0, [sp, #-512]
     d50:   445f4253    ldrbmi  r4, [pc], #595  ; d58 <.debug_info+0xd58>
     d54:   63697665    cmnvs   r9, #105906176  ; 0x6500000
     d58:   71655265    cmnvc   r5, r5, ror #4
     d5c:   74736575    ldrbtvc r6, [r3], #-1397
     d60:   e8b30700    ldm r3!, {r8, r9, sl}
     d64:   0b00000c    bleq    d9c <.debug_info+0xd9c>
     d68:   9dc20714    stclls  7, cr0, [r2, #80]
     d6c:   1400000e    strne   r0, [r0], #-14
     d70:   0000012c    andeq   r0, r0, ip, lsr #2
            d70: R_ARM_ABS32    .debug_str
     d74:   0160c307    cmneq   r0, r7, lsl #6
     d78:   23020000    movwcs  r0, #8192   ; 0x2000
     d7c:   01461400    cmpeq   r6, r0, lsl #8
            d7e: R_ARM_ABS32    .debug_str
     d80:   c4070000    strgt   r0, [r7]
     d84:   00000160    andeq   r0, r0, r0, ror #2
     d88:   0c012302    stceq   3, cr2, [r1], {2}
     d8c:   55646362    strbpl  r6, [r4, #-866]!
     d90:   07004253    smlsdeq r0, r3, r2, r4
     d94:   00016fc5    andeq   r6, r1, r5, asr #31
     d98:   02230200    eoreq   r0, r3, #0  ; 0x0
     d9c:   6544620c    strbvs  r6, [r4, #-524]
     da0:   65636976    strbvs  r6, [r3, #-2422]!
     da4:   73616c43    cmnvc   r1, #17152  ; 0x4300
     da8:   c6070073    undefined
     dac:   00000160    andeq   r0, r0, r0, ror #2
     db0:   0c042302    stceq   3, cr2, [r4], {2}
     db4:   76654462    strbtvc r4, [r5], -r2, ror #8
     db8:   53656369    cmnpl   r5, #-1543503871    ; 0xa4000001
     dbc:   6c436275    sfmvs   f6, 2, [r3], {117}
     dc0:   00737361    rsbseq  r7, r3, r1, ror #6
     dc4:   0160c707    cmneq   r0, r7, lsl #14
     dc8:   23020000    movwcs  r0, #8192   ; 0x2000
     dcc:   44620c05    strbtmi r0, [r2], #-3077
     dd0:   63697665    cmnvs   r9, #105906176  ; 0x6500000
     dd4:   6f725065    svcvs   0x00725065
     dd8:   6f636f74    svcvs   0x00636f74
     ddc:   c807006c    stmdagt r7, {r2, r3, r5, r6}
     de0:   00000160    andeq   r0, r0, r0, ror #2
     de4:   0c062302    stceq   3, cr2, [r6], {2}
     de8:   78614d62    stmdavc r1!, {r1, r5, r6, r8, sl, fp, lr}^
     dec:   6b636150    blvs    18d9334 <MGC_DrcDumpPipe+0x18d50e0>
     df0:   69537465    ldmdbvs r3, {r0, r2, r5, r6, sl, ip, sp, lr}^
     df4:   0030657a    eorseq  r6, r0, sl, ror r5
     df8:   0160c907    cmneq   r0, r7, lsl #18
     dfc:   23020000    movwcs  r0, #8192   ; 0x2000
     e00:   64690c07    strbtvs r0, [r9], #-3079
     e04:   646e6556    strbtvs r6, [lr], #-1366
     e08:   0700726f    streq   r7, [r0, -pc, ror #4]
     e0c:   00016fca    andeq   r6, r1, sl, asr #31
     e10:   08230200    stmdaeq r3!, {r9}
     e14:   5064690c    rsbpl   r6, r4, ip, lsl #18
     e18:   75646f72    strbvc  r6, [r4, #-3954]!
     e1c:   07007463    streq   r7, [r0, -r3, ror #8]
     e20:   00016fcb    andeq   r6, r1, fp, asr #31
     e24:   0a230200    beq 8c162c <MGC_DrcDumpPipe+0x8bd3d8>
     e28:   6463620c    strbtvs r6, [r3], #-524
     e2c:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
     e30:   07006563    streq   r6, [r0, -r3, ror #10]
     e34:   00016fcc    andeq   r6, r1, ip, asr #31
     e38:   0c230200    sfmeq   f0, 4, [r3]
     e3c:   614d690c    cmpvs   sp, ip, lsl #18
     e40:   6166756e    cmnvs   r6, lr, ror #10
     e44:   72757463    rsbsvc  r7, r5, #1660944384 ; 0x63000000
     e48:   07007265    streq   r7, [r0, -r5, ror #4]
     e4c:   000160cd    andeq   r6, r1, sp, asr #1
     e50:   0e230200    cdpeq   2, 2, cr0, cr3, cr0, {0}
     e54:   7250690c    subsvc  r6, r0, #196608 ; 0x30000
     e58:   6375646f    cmnvs   r5, #1862270976 ; 0x6f000000
     e5c:   ce070074    mcrgt   0, 0, r0, cr7, cr4, {3}
     e60:   00000160    andeq   r0, r0, r0, ror #2
     e64:   0c0f2302    stceq   3, cr2, [pc], {2}
     e68:   72655369    rsbvc   r5, r5, #-1543503871    ; 0xa4000001
     e6c:   4e6c6169    powmiez f6, f4, #1.0
     e70:   65626d75    strbvs  r6, [r2, #-3445]!
     e74:   cf070072    svcgt   0x00070072
     e78:   00000160    andeq   r0, r0, r0, ror #2
     e7c:   0c102302    ldceq   3, cr2, [r0], {2}
     e80:   6d754e62    ldclvs  14, cr4, [r5, #-392]!
     e84:   666e6f43    strbtvs r6, [lr], -r3, asr #30
     e88:   72756769    rsbsvc  r6, r5, #27525120   ; 0x1a40000
     e8c:   6f697461    svcvs   0x00697461
     e90:   0700736e    streq   r7, [r0, -lr, ror #6]
     e94:   000160d0    ldrdeq  r6, [r1], -r0
     e98:   11230200    teqne   r3, r0, lsl #4
     e9c:   554d0200    strbpl  r0, [sp, #-512]
     ea0:   445f4253    ldrbmi  r4, [pc], #595  ; ea8 <.debug_info+0xea8>
     ea4:   63697665    cmnvs   r9, #105906176  ; 0x6500000
     ea8:   73654465    cmnvc   r5, #1694498816 ; 0x65000000
     eac:   70697263    rsbvc   r7, r9, r3, ror #4
     eb0:   00726f74    rsbseq  r6, r2, r4, ror pc
     eb4:   0d67d107    stfeqp  f5, [r7, #-28]!
     eb8:   080b0000    stmdaeq fp, {}
     ebc:   0f24d707    svceq   0x0024d707
     ec0:   2c140000    ldccs   0, cr0, [r4], {0}
            ec3: R_ARM_ABS32    .debug_str
     ec4:   07000001    streq   r0, [r0, -r1]
     ec8:   000160d8    ldrdeq  r6, [r1], -r8
     ecc:   00230200    eoreq   r0, r3, r0, lsl #4
     ed0:   00014614    andeq   r4, r1, r4, lsl r6
            ed1: R_ARM_ABS32    .debug_str
     ed4:   60d90700    sbcsvs  r0, r9, r0, lsl #14
     ed8:   02000001    andeq   r0, r0, #1  ; 0x1
     edc:   620c0123    andvs   r0, ip, #-1073741816    ; 0xc0000008
     ee0:   70646e45    rsbvc   r6, r4, r5, asr #28
     ee4:   746e696f    strbtvc r6, [lr], #-2415
     ee8:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
     eec:   00737365    rsbseq  r7, r3, r5, ror #6
     ef0:   0160da07    cmneq   r0, r7, lsl #20
     ef4:   23020000    movwcs  r0, #8192   ; 0x2000
     ef8:   01b11402    undefined instruction 0x01b11402
            efa: R_ARM_ABS32    .debug_str
     efc:   db070000    blle    1c0f04 <MGC_DrcDumpPipe+0x1bccb0>
     f00:   00000160    andeq   r0, r0, r0, ror #2
     f04:   14032302    strne   r2, [r3], #-770
     f08:   0000011d    andeq   r0, r0, sp, lsl r1
            f08: R_ARM_ABS32    .debug_str
     f0c:   016fdc07    cmneq   pc, r7, lsl #24
     f10:   23020000    movwcs  r0, #8192   ; 0x2000
     f14:   00391404    eorseq  r1, r9, r4, lsl #8
            f16: R_ARM_ABS32    .debug_str
     f18:   dd070000    stcle   0, cr0, [r7]
     f1c:   00000160    andeq   r0, r0, r0, ror #2
     f20:   00062302    andeq   r2, r6, r2, lsl #6
     f24:   53554d02    cmppl   r5, #128    ; 0x80
     f28:   6e455f42    cdpvs   15, 4, cr5, cr5, cr2, {2}
     f2c:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
     f30:   6544746e    strbvs  r7, [r4, #-1134]
     f34:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
     f38:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
     f3c:   bade0700    blt ff782b44 <MGC_DrcDumpPipe+0xff77e8f0>
     f40:   0b00000e    bleq    f80 <.debug_info+0xf80>
     f44:   f1f4070c    undefined instruction 0xf1f4070c
     f48:   1400000f    strne   r0, [r0], #-15
     f4c:   0000012c    andeq   r0, r0, ip, lsr #2
            f4c: R_ARM_ABS32    .debug_str
     f50:   0160f507    msreq   SPSR_, r7, lsl #10
     f54:   23020000    movwcs  r0, #8192   ; 0x2000
     f58:   01461400    cmpeq   r6, r0, lsl #8
            f5a: R_ARM_ABS32    .debug_str
     f5c:   f6070000    undefined instruction 0xf6070000
     f60:   00000160    andeq   r0, r0, r0, ror #2
     f64:   0c012302    stceq   3, cr2, [r1], {2}
     f68:   746f5477    strbtvc r5, [pc], #1143 ; f70 <.debug_info+0xf70>
     f6c:   654c6c61    strbvs  r6, [ip, #-3169]
     f70:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
     f74:   6ff70700    svcvs   0x00f70700
     f78:   02000001    andeq   r0, r0, #1  ; 0x1
     f7c:   620c0223    andvs   r0, ip, #805306370  ; 0x30000002
     f80:   496d754e    stmdbmi sp!, {r1, r2, r3, r6, r8, sl, ip, sp, lr}^
     f84:   7265746e    rsbvc   r7, r5, #1845493760 ; 0x6e000000
     f88:   65636166    strbvs  r6, [r3, #-358]!
     f8c:   f8070073    undefined instruction 0xf8070073
     f90:   00000160    andeq   r0, r0, r0, ror #2
     f94:   0c042302    stceq   3, cr2, [r4], {2}
     f98:   6e6f4362    cdpvs   3, 6, cr4, cr15, cr2, {3}
     f9c:   75676966    strbvc  r6, [r7, #-2406]!
     fa0:   69746172    ldmdbvs r4!, {r1, r4, r5, r6, r8, sp, lr}^
     fa4:   61566e6f    cmpvs   r6, pc, ror #28
     fa8:   0065756c    rsbeq   r7, r5, ip, ror #10
     fac:   0160f907    msreq   SPSR_, r7, lsl #18
     fb0:   23020000    movwcs  r0, #8192   ; 0x2000
     fb4:   43690c05    cmnmi   r9, #1280   ; 0x500
     fb8:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
     fbc:   61727567    cmnvs   r2, r7, ror #10
     fc0:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
     fc4:   60fa0700    rscsvs  r0, sl, r0, lsl #14
     fc8:   02000001    andeq   r0, r0, #1  ; 0x1
     fcc:   b1140623    tstlt   r4, r3, lsr #12
            fcf: R_ARM_ABS32    .debug_str
     fd0:   07000001    streq   r0, [r0, -r1]
     fd4:   000160fb    strdeq  r6, [r1], -fp
     fd8:   07230200    streq   r0, [r3, -r0, lsl #4]!
     fdc:   614d620c    cmpvs   sp, ip, lsl #4
     fe0:   776f5078    undefined
     fe4:   07007265    streq   r7, [r0, -r5, ror #4]
     fe8:   000160fc    strdeq  r6, [r1], -ip
     fec:   08230200    stmdaeq r3!, {r9}
     ff0:   554d0200    strbpl  r0, [sp, #-512]
     ff4:   435f4253    cmpmi   pc, #805306373  ; 0x30000005
     ff8:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
     ffc:   61727567    cmnvs   r2, r7, ror #10
    1000:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    1004:   63736544    cmnvs   r3, #285212672  ; 0x11000000
    1008:   74706972    ldrbtvc r6, [r0], #-2418
    100c:   0700726f    streq   r7, [r0, -pc, ror #4]
    1010:   000f43fd    strdeq  r4, [pc], -sp
    1014:   07041600    streq   r1, [r4, -r0, lsl #12]
    1018:   104c010d    subne   r0, ip, sp, lsl #2
    101c:   2c170000    ldccs   0, cr0, [r7], {0}
            101f: R_ARM_ABS32   .debug_str
    1020:   07000001    streq   r0, [r0, -r1]
    1024:   0160010e    cmneq   r0, lr, lsl #2
    1028:   23020000    movwcs  r0, #8192   ; 0x2000
    102c:   01461700    cmpeq   r6, r0, lsl #14
            102e: R_ARM_ABS32   .debug_str
    1030:   0f070000    svceq   0x00070000
    1034:   00016001    andeq   r6, r1, r1
    1038:   01230200    teqeq   r3, r0, lsl #4
    103c:   0001b117    andeq   fp, r1, r7, lsl r1
            103d: R_ARM_ABS32   .debug_str
    1040:   01100700    tsteq   r0, r0, lsl #14
    1044:   00000160    andeq   r0, r0, r0, ror #2
    1048:   00022302    andeq   r2, r2, r2, lsl #6
    104c:   53554d11    cmppl   r5, #1088   ; 0x440
    1050:   744f5f42    strbvc  r5, [pc], #3906 ; 1058 <.debug_info+0x1058>
    1054:   73654467    cmnvc   r5, #1728053248 ; 0x67000000
    1058:   70697263    rsbvc   r7, r9, r3, ror #4
    105c:   00726f74    rsbseq  r6, r2, r4, ror pc
    1060:   15011107    strne   r1, [r1, #-263]
    1064:   09000010    stmdbeq r0, {r4}
    1068:   c9240804    stmdbgt r4!, {r2, fp}
    106c:   0a000010    beq 10b4 <.debug_info+0x10b4>
    1070:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1074:   524f505f    subpl   r5, pc, #95 ; 0x5f
    1078:   4e555f54    mrcmi   15, 2, r5, cr5, cr4, {2}
    107c:   574f4e4b    strbpl  r4, [pc, -fp, asr #28]
    1080:   0a00004e    beq 11c0 <.debug_info+0x11c0>
    1084:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1088:   524f505f    subpl   r5, pc, #95 ; 0x5f
    108c:   59545f54    ldmdbpl r4, {r2, r4, r6, r8, r9, sl, fp, ip, lr}^
    1090:   465f4550    undefined
    1094:   54434e55    strbpl  r4, [r3], #-3669
    1098:   004e4f49    subeq   r4, lr, r9, asr #30
    109c:   554d0a01    strbpl  r0, [sp, #-2561]
    10a0:   505f4253    subspl  r4, pc, r3, asr r2
    10a4:   5f54524f    svcpl   0x0054524f
    10a8:   45505954    ldrbmi  r5, [r0, #-2388]
    10ac:   534f485f    movtpl  r4, #63583  ; 0xf85f
    10b0:   0a020054    beq 81208 <MGC_DrcDumpPipe+0x7cfb4>
    10b4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    10b8:   524f505f    subpl   r5, pc, #95 ; 0x5f
    10bc:   59545f54    ldmdbpl r4, {r2, r4, r6, r8, r9, sl, fp, ip, lr}^
    10c0:   4f5f4550    svcmi   0x005f4550
    10c4:   03004754    movweq  r4, #1876   ; 0x754
    10c8:   554d0200    strbpl  r0, [sp, #-512]
    10cc:   505f4253    subspl  r4, pc, r3, asr r2
    10d0:   5474726f    ldrbtpl r7, [r4], #-623
    10d4:   00657079    rsbeq   r7, r5, r9, ror r0
    10d8:   10672d08    rsbne   r2, r7, r8, lsl #26
    10dc:   04090000    streq   r0, [r9]
    10e0:   11453408    cmpne   r5, r8, lsl #8
    10e4:   4d0a0000    stcmi   0, cr0, [sl]
    10e8:   5f425355    svcpl   0x00425355
    10ec:   54524f50    ldrbpl  r4, [r2], #-3920
    10f0:   4550535f    ldrbmi  r5, [r0, #-863]
    10f4:   555f4445    ldrbpl  r4, [pc, #-1093]    ; cb7 <.debug_info+0xcb7>
    10f8:   4f4e4b4e    svcmi   0x004e4b4e
    10fc:   00004e57    andeq   r4, r0, r7, asr lr
    1100:   53554d0a    cmppl   r5, #640    ; 0x280
    1104:   4f505f42    svcmi   0x00505f42
    1108:   535f5452    cmppl   pc, #1375731712 ; 0x52000000
    110c:   44454550    strbmi  r4, [r5], #-1360
    1110:   574f4c5f    smlsldpl    r4, pc, pc, ip
    1114:   4d0a0100    stfmis  f0, [sl]
    1118:   5f425355    svcpl   0x00425355
    111c:   54524f50    ldrbpl  r4, [r2], #-3920
    1120:   4550535f    ldrbmi  r5, [r0, #-863]
    1124:   465f4445    ldrbmi  r4, [pc], -r5, asr #8
    1128:   004c4c55    subeq   r4, ip, r5, asr ip
    112c:   554d0a02    strbpl  r0, [sp, #-2562]
    1130:   505f4253    subspl  r4, pc, r3, asr r2
    1134:   5f54524f    svcpl   0x0054524f
    1138:   45455053    strbmi  r5, [r5, #-83]
    113c:   49485f44    stmdbmi r8, {r2, r6, r8, r9, sl, fp, ip, lr}^
    1140:   03004847    movweq  r4, #2119   ; 0x847
    1144:   554d0200    strbpl  r0, [sp, #-512]
    1148:   505f4253    subspl  r4, pc, r3, asr r2
    114c:   5374726f    cmnpl   r4, #-268435450 ; 0xf0000006
    1150:   64656570    strbtvs r6, [r5], #-1392
    1154:   de3d0800    cdple   8, 3, cr0, cr13, cr0, {0}
    1158:   09000010    stmdbeq r0, {r4}
    115c:   c5440804    strbgt  r0, [r4, #-2052]
    1160:   0a000011    beq 11ac <.debug_info+0x11ac>
    1164:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1168:   574f505f    smlsldpl    r5, pc, pc, r0
    116c:   4f5f5245    svcmi   0x005f5245
    1170:   00004646    andeq   r4, r0, r6, asr #12
    1174:   53554d0a    cmppl   r5, #640    ; 0x280
    1178:   4f505f42    svcmi   0x00505f42
    117c:   45524557    ldrbmi  r4, [r2, #-1367]
    1180:   0a010044    beq 41298 <MGC_DrcDumpPipe+0x3d044>
    1184:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1188:   4645445f    undefined
    118c:   544c5541    strbpl  r5, [ip], #-1345
    1190:   4d0a0200    sfmmi   f0, 4, [sl]
    1194:   5f425355    svcpl   0x00425355
    1198:   52444441    subpl   r4, r4, #1090519040 ; 0x41000000
    119c:   00535345    subseq  r5, r3, r5, asr #6
    11a0:   554d0a03    strbpl  r0, [sp, #-2563]
    11a4:   435f4253    cmpmi   pc, #805306373  ; 0x30000005
    11a8:   49464e4f    stmdbmi r6, {r0, r1, r2, r3, r6, r9, sl, fp, lr}^
    11ac:   45525547    ldrbmi  r5, [r2, #-1351]
    11b0:   0a040044    beq 1012c8 <MGC_DrcDumpPipe+0xfd074>
    11b4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    11b8:   5355535f    cmppl   r5, #2080374785 ; 0x7c000001
    11bc:   444e4550    strbmi  r4, [lr], #-1360
    11c0:   05004445    streq   r4, [r0, #-1093]
    11c4:   554d0200    strbpl  r0, [sp, #-512]
    11c8:   535f4253    cmppl   pc, #805306373  ; 0x30000005
    11cc:   65746174    ldrbvs  r6, [r4, #-372]!
    11d0:   5b510800    blpl    14431d8 <MGC_DrcDumpPipe+0x143ef84>
    11d4:   09000011    stmdbeq r0, {r0, r4}
    11d8:   a2580804    subsge  r0, r8, #262144 ; 0x40000
    11dc:   0a000012    beq 122c <.debug_info+0x122c>
    11e0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    11e4:   5f42415f    svcpl   0x0042415f
    11e8:   454c4449    strbmi  r4, [ip, #-1097]
    11ec:   4d0a0000    stcmi   0, cr0, [sl]
    11f0:   5f425355    svcpl   0x00425355
    11f4:   54494157    strbpl  r4, [r9], #-343
    11f8:   4952565f    ldmdbmi r2, {r0, r1, r2, r3, r4, r6, r9, sl, ip, lr}^
    11fc:   01004553    tsteq   r0, r3, asr r5
    1200:   53554d0a    cmppl   r5, #640    ; 0x280
    1204:   5f425f42    svcpl   0x00425f42
    1208:   49524550    ldmdbmi r2, {r4, r6, r8, sl, lr}^
    120c:   52454850    subpl   r4, r5, #5242880    ; 0x500000
    1210:   11004c41    tstne   r0, r1, asr #24
    1214:   53554d0a    cmppl   r5, #640    ; 0x280
    1218:   5f425f42    svcpl   0x00425f42
    121c:   54494157    strbpl  r4, [r9], #-343
    1220:   4f43415f    svcmi   0x0043415f
    1224:   0a12004e    beq 481364 <MGC_DrcDumpPipe+0x47d110>
    1228:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    122c:   485f425f    ldmdami pc, {r0, r1, r2, r3, r4, r6, r9, lr}^
    1230:   0054534f    subseq  r5, r4, pc, asr #6
    1234:   554d0a13    strbpl  r0, [sp, #-2579]
    1238:   425f4253    subsmi  r4, pc, #805306373  ; 0x30000005
    123c:   5052535f    subspl  r5, r2, pc, asr r3
    1240:   494e495f    stmdbmi lr, {r0, r1, r2, r3, r4, r6, r8, fp, lr}^
    1244:   0a140054    beq 50139c <MGC_DrcDumpPipe+0x4fd148>
    1248:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    124c:   505f415f    subspl  r4, pc, pc, asr r1
    1250:   50495245    subpl   r5, r9, r5, asr #4
    1254:   41524548    cmpmi   r2, r8, asr #10
    1258:   0a21004c    beq 841390 <MGC_DrcDumpPipe+0x83d13c>
    125c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1260:   575f415f    undefined
    1264:   5f544941    svcpl   0x00544941
    1268:   4e4f4342    cdpmi   3, 4, cr4, cr15, cr2, {2}
    126c:   4d0a2200    sfmmi   f2, 4, [sl]
    1270:   5f425355    svcpl   0x00425355
    1274:   4f485f41    svcmi   0x00485f41
    1278:   23005453    movwcs  r5, #1107   ; 0x453
    127c:   53554d0a    cmppl   r5, #640    ; 0x280
    1280:   5f415f42    svcpl   0x00415f42
    1284:   50535553    subspl  r5, r3, r3, asr r5
    1288:   00444e45    subeq   r4, r4, r5, asr #28
    128c:   554d0a24    strbpl  r0, [sp, #-2596]
    1290:   535f4253    cmppl   pc, #805306373  ; 0x30000005
    1294:   4152545f    cmpmi   r2, pc, asr r4
    1298:   5449534e    strbpl  r5, [r9], #-846
    129c:   004e4f49    subeq   r4, lr, r9, asr #30
    12a0:   4d020030    stcmi   0, cr0, [r2, #-192]
    12a4:   5f425355    svcpl   0x00425355
    12a8:   5367744f    cmnpl   r7, #1325400064 ; 0x4f000000
    12ac:   65746174    ldrbvs  r6, [r4, #-372]!
    12b0:   d76f0800    strble  r0, [pc, -r0, lsl #16]!
    12b4:   09000011    stmdbeq r0, {r0, r4}
    12b8:   36760804    ldrbtcc r0, [r6], -r4, lsl #16
    12bc:   0a000013    beq 1310 <.debug_info+0x1310>
    12c0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    12c4:   4e4f435f    mcrmi   3, 2, r4, cr15, cr15, {2}
    12c8:   5443454e    strbpl  r4, [r3], #-1358
    12cc:   5f4e4f49    svcpl   0x004e4f49
    12d0:   45455053    strbmi  r5, [r5, #-83]
    12d4:   4e555f44    cdpmi   15, 5, cr5, cr5, cr4, {2}
    12d8:   574f4e4b    strbpl  r4, [pc, -fp, asr #28]
    12dc:   0a00004e    beq 141c <.debug_info+0x141c>
    12e0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    12e4:   4e4f435f    mcrmi   3, 2, r4, cr15, cr15, {2}
    12e8:   5443454e    strbpl  r4, [r3], #-1358
    12ec:   5f4e4f49    svcpl   0x004e4f49
    12f0:   45455053    strbmi  r5, [r5, #-83]
    12f4:   4f4c5f44    svcmi   0x004c5f44
    12f8:   0a010057    beq 4145c <MGC_DrcDumpPipe+0x3d208>
    12fc:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1300:   4e4f435f    mcrmi   3, 2, r4, cr15, cr15, {2}
    1304:   5443454e    strbpl  r4, [r3], #-1358
    1308:   5f4e4f49    svcpl   0x004e4f49
    130c:   45455053    strbmi  r5, [r5, #-83]
    1310:   55465f44    strbpl  r5, [r6, #-3908]
    1314:   02004c4c    andeq   r4, r0, #19456  ; 0x4c00
    1318:   53554d0a    cmppl   r5, #640    ; 0x280
    131c:   4f435f42    svcmi   0x00435f42
    1320:   43454e4e    movtmi  r4, #24142  ; 0x5e4e
    1324:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
    1328:   4550535f    ldrbmi  r5, [r0, #-863]
    132c:   485f4445    ldmdami pc, {r0, r2, r6, sl, lr}^
    1330:   00484749    subeq   r4, r8, r9, asr #14
    1334:   4d020003    stcmi   0, cr0, [r2, #-12]
    1338:   5f425355    svcpl   0x00425355
    133c:   6e6e6f43    cdpvs   15, 6, cr6, cr14, cr3, {2}
    1340:   69746365    ldmdbvs r4!, {r0, r2, r5, r6, r8, r9, sp, lr}^
    1344:   70536e6f    subsvc  r6, r3, pc, ror #28
    1348:   00646565    rsbeq   r6, r4, r5, ror #10
    134c:   12b77f08    adcsne  r7, r7, #32 ; 0x20
    1350:   100b0000    andne   r0, fp, r0
    1354:   139ce408    orrsne  lr, ip, #134217728  ; 0x8000000
    1358:   04140000    ldreq   r0, [r4]
            135b: R_ARM_ABS32   .debug_str
    135c:   08000001    stmdaeq r0, {r0}
    1360:   0000f1e5    andeq   pc, r0, r5, ror #3
    1364:   00230200    eoreq   r0, r3, r0, lsl #4
    1368:   6463620c    strbtvs r6, [r3], #-524
    136c:   52696443    rsbpl   r6, r9, #1124073472 ; 0x43000000
    1370:   08007665    stmdaeq r0, {r0, r2, r5, r6, r9, sl, ip, sp, lr}
    1374:   00016fe6    andeq   r6, r1, r6, ror #31
    1378:   04230200    strteq  r0, [r3], #-512
    137c:   7079540c    rsbsvc  r5, r9, ip, lsl #8
    1380:   e7080065    str r0, [r8, -r5, rrx]
    1384:   000010c9    andeq   r1, r0, r9, asr #1
    1388:   0c082302    stceq   3, cr2, [r8], {2}
    138c:   65657053    strbvs  r7, [r5, #-83]!
    1390:   e8080064    stmda   r8, {r2, r5, r6}
    1394:   00001145    andeq   r1, r0, r5, asr #2
    1398:   000c2302    andeq   r2, ip, r2, lsl #6
    139c:   53554d02    cmppl   r5, #128    ; 0x80
    13a0:   6f505f42    svcvs   0x00505f42
    13a4:   08007472    stmdaeq r0, {r1, r4, r5, r6, sl, ip, sp, lr}
    13a8:   001352e9    andseq  r5, r3, r9, ror #5
    13ac:   4d5f1800    ldclmi  8, cr1, [pc]
    13b0:   5f425355    svcpl   0x00425355
    13b4:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    13b8:   40006563    andmi   r6, r0, r3, ror #10
    13bc:   14d1eb08    ldrbne  lr, [r1], #2824
    13c0:   7a170000    bvc 5c0008 <MGC_DrcDumpPipe+0x5bbdb4>
            13c3: R_ARM_ABS32   .debug_str
    13c4:   08000001    stmdaeq r0, {r0}
    13c8:   14d1010e    ldrbne  r0, [r1], #270
    13cc:   23020000    movwcs  r0, #8192   ; 0x2000
    13d0:   01041700    tsteq   r4, r0, lsl #14
            13d2: R_ARM_ABS32   .debug_str
    13d4:   0f080000    svceq   0x00080000
    13d8:   0000f101    andeq   pc, r0, r1, lsl #2
    13dc:   04230200    strteq  r0, [r3], #-512
    13e0:   7244700d    subvc   r7, r4, #13 ; 0xd
    13e4:   72657669    rsbvc   r7, r5, #110100480  ; 0x6900000
    13e8:   76697250    undefined
    13ec:   44657461    strbtmi r7, [r5], #-1121
    13f0:   00617461    rsbeq   r7, r1, r1, ror #8
    13f4:   f1011008    undefined instruction 0xf1011008
    13f8:   02000000    andeq   r0, r0, #0  ; 0x0
    13fc:   440d0823    strmi   r0, [sp], #-2083
    1400:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    1404:   73654465    cmnvc   r5, #1694498816 ; 0x65000000
    1408:   70697263    rsbvc   r7, r9, r3, ror #4
    140c:   00726f74    rsbseq  r6, r2, r4, ror pc
    1410:   9d011108    stflss  f1, [r1, #-32]
    1414:   0200000e    andeq   r0, r0, #14 ; 0xe
    1418:   700d0c23    andvc   r0, sp, r3, lsr #24
    141c:   72727543    rsbsvc  r7, r2, #281018368  ; 0x10c00000
    1420:   43746e65    cmnmi   r4, #1616   ; 0x650
    1424:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
    1428:   61727567    cmnvs   r2, r7, ror #10
    142c:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    1430:   01120800    tsteq   r2, r0, lsl #16
    1434:   000014d7    ldrdeq  r1, [r0], -r7
    1438:   17202302    strne   r2, [r0, -r2, lsl #6]!
    143c:   0000009b    muleq   r0, fp, r0
            143c: R_ARM_ABS32   .debug_str
    1440:   dd011308    stcle   3, cr1, [r1, #-32]
    1444:   02000014    andeq   r0, r0, #20 ; 0x14
    1448:   700d2423    andvc   r2, sp, r3, lsr #8
    144c:   63736544    cmnvs   r3, #285212672  ; 0x11000000
    1450:   74706972    ldrbtvc r6, [r0], #-2418
    1454:   7542726f    strbvc  r7, [r2, #-623]
    1458:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    145c:   01140800    tsteq   r4, r0, lsl #16
    1460:   000014e3    andeq   r1, r0, r3, ror #9
    1464:   17282302    strne   r2, [r8, -r2, lsl #6]!
    1468:   00000191    muleq   r0, r1, r1
            1468: R_ARM_ABS32   .debug_str
    146c:   6f011508    svcvs   0x00011508
    1470:   02000001    andeq   r0, r0, #1  ; 0x1
    1474:   430d2c23    movwmi  r2, #56355  ; 0xdc23
    1478:   656e6e6f    strbvs  r6, [lr, #-3695]!
    147c:   6f697463    svcvs   0x00697463
    1480:   6570536e    ldrbvs  r5, [r0, #-878]!
    1484:   08006465    stmdaeq r0, {r0, r2, r5, r6, sl, sp, lr}
    1488:   13360116    teqne   r6, #-2147483643    ; 0x80000005
    148c:   23020000    movwcs  r0, #8192   ; 0x2000
    1490:   00241730    eoreq   r1, r4, r0, lsr r7
            1492: R_ARM_ABS32   .debug_str
    1494:   17080000    strne   r0, [r8, -r0]
    1498:   00016001    andeq   r6, r1, r1
    149c:   34230200    strtcc  r0, [r3], #-512
    14a0:   6150700d    cmpvs   r0, sp
    14a4:   746e6572    strbtvc r6, [lr], #-1394
    14a8:   44627355    strbtmi r7, [r2], #-853
    14ac:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    14b0:   18080065    stmdane r8, {r0, r2, r5, r6}
    14b4:   0014e901    andseq  lr, r4, r1, lsl #18
    14b8:   38230200    stmdacc r3!, {r9}
    14bc:   7548620d    strbvc  r6, [r8, #-525]
    14c0:   726f5062    rsbvc   r5, pc, #98 ; 0x62
    14c4:   19080074    stmdbne r8, {r2, r4, r5, r6}
    14c8:   00016001    andeq   r6, r1, r1
    14cc:   3c230200    sfmcc   f0, 4, [r3]
    14d0:   9c040700    stcls   7, cr0, [r4], {0}
    14d4:   07000013    smladeq r0, r3, r0, r0
    14d8:   000ff104    andeq   pc, pc, r4, lsl #2
    14dc:   d7040700    strle   r0, [r4, -r0, lsl #14]
    14e0:   07000014    smladeq r0, r4, r0, r0
    14e4:   00016004    andeq   r6, r1, r4
    14e8:   ad040700    stcge   7, cr0, [r4]
    14ec:   11000013    tstne   r0, r3, lsl r0
    14f0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    14f4:   7665445f    undefined
    14f8:   00656369    rsbeq   r6, r5, r9, ror #6
    14fc:   ad011a08    fstsge  s2, [r1, #-32]
    1500:   11000013    tstne   r0, r3, lsl r0
    1504:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1508:   7375425f    cmnvc   r5, #-268435451 ; 0xf0000005
    150c:   646e6148    strbtvs r6, [lr], #-328
    1510:   0800656c    stmdaeq r0, {r2, r3, r5, r6, r8, sl, sp, lr}
    1514:   00f1011d    rscseq  r0, r1, sp, lsl r1
    1518:   10160000    andsne  r0, r6, r0
    151c:   61012d08    tstvs   r1, r8, lsl #26
    1520:   17000015    smladne r0, r5, r0, r0
    1524:   00000180    andeq   r0, r0, r0, lsl #3
            1524: R_ARM_ABS32   .debug_str
    1528:   61012e08    tstvs   r1, r8, lsl #28
    152c:   02000015    andeq   r0, r0, #21 ; 0x15
    1530:   550d0023    strpl   r0, [sp, #-35]
    1534:   65446273    strbvs  r6, [r4, #-627]
    1538:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
    153c:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
    1540:   012f0800    teqeq   pc, r0, lsl #16
    1544:   00000f24    andeq   r0, r0, r4, lsr #30
    1548:   0d042302    stceq   3, cr2, [r4, #-8]
    154c:   6b614e77    blvs    1854f30 <MGC_DrcDumpPipe+0x1850cdc>
    1550:   696d694c    stmdbvs sp!, {r2, r3, r6, r8, fp, sp, lr}^
    1554:   30080074    andcc   r0, r8, r4, ror r0
    1558:   00016f01    andeq   r6, r1, r1, lsl #30
    155c:   0c230200    sfmeq   f0, 4, [r3]
    1560:   67040700    strvs   r0, [r4, -r0, lsl #14]
    1564:   08000015    stmdaeq r0, {r0, r2, r4}
    1568:   000014ef    andeq   r1, r0, pc, ror #9
    156c:   53554d11    cmppl   r5, #1088   ; 0x440
    1570:   65445f42    strbvs  r5, [r4, #-3906]
    1574:   65636976    strbvs  r6, [r3, #-2422]!
    1578:   70646e45    rsbvc   r6, r4, r5, asr #28
    157c:   746e696f    strbtvc r6, [lr], #-2415
    1580:   01310800    teqeq   r1, r0, lsl #16
    1584:   0000151a    andeq   r1, r0, sl, lsl r5
    1588:   41080c16    tstmi   r8, r6, lsl ip
    158c:   0015cc01    andseq  ip, r5, r1, lsl #24
    1590:   77640d00    strbvc  r0, [r4, -r0, lsl #26]!
    1594:   66667542    strbtvs r7, [r6], -r2, asr #10
    1598:   69537265    ldmdbvs r3, {r0, r2, r5, r6, r9, ip, sp, lr}^
    159c:   0800657a    stmdaeq r0, {r1, r3, r4, r5, r6, r8, sl, sp, lr}
    15a0:   018e0142    orreq   r0, lr, r2, asr #2
    15a4:   23020000    movwcs  r0, #8192   ; 0x2000
    15a8:   6d620d00    stclvs  13, cr0, [r2]
    15ac:   67616c46    strbvs  r6, [r1, -r6, asr #24]!
    15b0:   43080073    movwmi  r0, #32883  ; 0x8073
    15b4:   00018e01    andeq   r8, r1, r1, lsl #28
    15b8:   04230200    strteq  r0, [r3], #-512
    15bc:   00010417    andeq   r0, r1, r7, lsl r4
            15bd: R_ARM_ABS32   .debug_str
    15c0:   01440800    cmpeq   r4, r0, lsl #16
    15c4:   000000f1    strdeq  r0, [r0], -r1
    15c8:   00082302    andeq   r2, r8, r2, lsl #6
    15cc:   53554d11    cmppl   r5, #1088   ; 0x440
    15d0:   6e455f42    cdpvs   15, 4, cr5, cr5, cr2, {2}
    15d4:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    15d8:   6552746e    ldrbvs  r7, [r2, #-1134]
    15dc:   72756f73    rsbsvc  r6, r5, #460    ; 0x1cc
    15e0:   08006563    stmdaeq r0, {r0, r1, r5, r6, r8, sl, sp, lr}
    15e4:   15880145    strne   r0, [r8, #325]
    15e8:   4d110000    ldcmi   0, cr0, [r1]
    15ec:   5f425355    svcpl   0x00425355
    15f0:   65706950    ldrbvs  r6, [r0, #-2384]!
    15f4:   01520800    cmpeq   r2, r0, lsl #16
    15f8:   000000f1    strdeq  r0, [r0], -r1
    15fc:   53554d11    cmppl   r5, #1088   ; 0x440
    1600:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1604:   746e6f43    strbtvc r6, [lr], #-3907
    1608:   496c6f72    stmdbmi ip!, {r1, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
    160c:   6f437072    svcvs   0x00437072
    1610:   656c706d    strbvs  r7, [ip, #-109]!
    1614:   08006574    stmdaeq r0, {r2, r4, r5, r6, r8, sl, sp, lr}
    1618:   161e015c    undefined
    161c:   04070000    streq   r0, [r7]
    1620:   00001624    andeq   r1, r0, r4, lsr #12
    1624:   1635010f    ldrtne  r0, [r5], -pc, lsl #2
    1628:   f1100000    undefined instruction 0xf1100000
    162c:   10000000    andne   r0, r0, r0
    1630:   00001635    andeq   r1, r0, r5, lsr r6
    1634:   3b040700    blcc    10323c <MGC_DrcDumpPipe+0xfefe8>
    1638:   12000016    andne   r0, r0, #22 ; 0x16
    163c:   53554d5f    cmppl   r5, #6080   ; 0x17c0
    1640:   6f435f42    svcvs   0x00435f42
    1644:   6f72746e    svcvs   0x0072746e
    1648:   7072496c    rsbsvc  r4, r2, ip, ror #18
    164c:   54082c00    strpl   r2, [r8], #-3072
    1650:   00173201    andseq  r3, r7, r1, lsl #4
    1654:   01801700    orreq   r1, r0, r0, lsl #14
            1656: R_ARM_ABS32   .debug_str
    1658:   7b080000    blvc    201660 <MGC_DrcDumpPipe+0x1fd40c>
    165c:   00156101    andseq  r6, r5, r1, lsl #2
    1660:   00230200    eoreq   r0, r3, r0, lsl #4
    1664:   754f700d    strbvc  r7, [pc, #-13]  ; 165f <.debug_info+0x165f>
    1668:   66754274    undefined
    166c:   00726566    rsbseq  r6, r2, r6, ror #10
    1670:   51017c08    tstpl   r1, r8, lsl #24
    1674:   0200000b    andeq   r0, r0, #11 ; 0xb
    1678:   640d0423    strvs   r0, [sp], #-1059
    167c:   74754f77    ldrbtvc r4, [r5], #-3959
    1680:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    1684:   08006874    stmdaeq r0, {r2, r4, r5, r6, fp, sp, lr}
    1688:   018e017d    orreq   r0, lr, sp, ror r1
    168c:   23020000    movwcs  r0, #8192   ; 0x2000
    1690:   49700d08    ldmdbmi r0!, {r3, r8, sl, fp}^
    1694:   6675426e    ldrbtvs r4, [r5], -lr, ror #4
    1698:   00726566    rsbseq  r6, r2, r6, ror #10
    169c:   e3017e08    movw    r7, #7688   ; 0x1e08
    16a0:   02000014    andeq   r0, r0, #20 ; 0x14
    16a4:   640d0c23    strvs   r0, [sp], #-3107
    16a8:   4c6e4977    stclmi  9, cr4, [lr], #-476
    16ac:   74676e65    strbtvc r6, [r7], #-3685
    16b0:   7f080068    svcvc   0x00080068
    16b4:   00018e01    andeq   r8, r1, r1, lsl #28
    16b8:   10230200    eorne   r0, r3, r0, lsl #4
    16bc:   00003017    andeq   r3, r0, r7, lsl r0
            16bd: R_ARM_ABS32   .debug_str
    16c0:   01800800    orreq   r0, r0, r0, lsl #16
    16c4:   0000018e    andeq   r0, r0, lr, lsl #3
    16c8:   0d142302    ldceq   3, cr2, [r4, #-8]
    16cc:   63417764    movtvs  r7, #5988   ; 0x1764
    16d0:   6c617574    cfstr64vs   mvdx7, [r1], #-464
    16d4:   4c74754f    cfldr64mi   mvdx7, [r4], #-316
    16d8:   74676e65    strbtvc r6, [r7], #-3685
    16dc:   81080068    tsthi   r8, r8, rrx
    16e0:   00018e01    andeq   r8, r1, r1, lsl #28
    16e4:   18230200    stmdane r3!, {r9}
    16e8:   4177640d    cmnmi   r7, sp, lsl #8
    16ec:   61757463    cmnvs   r5, r3, ror #8
    16f0:   4c6e496c    stclmi  9, cr4, [lr], #-432
    16f4:   74676e65    strbtvc r6, [r7], #-3685
    16f8:   82080068    andhi   r0, r8, #104    ; 0x68
    16fc:   00018e01    andeq   r8, r1, r1, lsl #28
    1700:   1c230200    sfmne   f0, 4, [r3]
    1704:   0001c617    andeq   ip, r1, r7, lsl r6
            1705: R_ARM_ABS32   .debug_str
    1708:   01830800    orreq   r0, r3, r0, lsl #16
    170c:   000015fc    strdeq  r1, [r0], -ip
    1710:   17202302    strne   r2, [r0, -r2, lsl #6]!
    1714:   0000000c    andeq   r0, r0, ip
            1714: R_ARM_ABS32   .debug_str
    1718:   f1018408    undefined instruction 0xf1018408
    171c:   02000000    andeq   r0, r0, #0  ; 0x0
    1720:   f0172423    undefined instruction 0xf0172423
            1723: R_ARM_ABS32   .debug_str
    1724:   08000000    stmdaeq r0, {}
    1728:   01600185    smulbbeq    r0, r5, r1
    172c:   23020000    movwcs  r0, #8192   ; 0x2000
    1730:   4d110028    ldcmi   0, cr0, [r1, #-160]
    1734:   5f425355    svcpl   0x00425355
    1738:   746e6f43    strbtvc r6, [lr], #-3907
    173c:   496c6f72    stmdbmi ip!, {r1, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
    1740:   08007072    stmdaeq r0, {r1, r4, r5, r6, ip, sp, lr}
    1744:   163b0186    ldrtne  r0, [fp], -r6, lsl #3
    1748:   4d110000    ldcmi   0, cr0, [r1]
    174c:   5f425355    svcpl   0x00425355
    1750:   72496670    subvc   r6, r9, #117440512  ; 0x7000000
    1754:   6d6f4370    stclvs  3, cr4, [pc, #-448]!
    1758:   74656c70    strbtvc r6, [r5], #-3184
    175c:   90080065    andls   r0, r8, r5, rrx
    1760:   00176501    andseq  r6, r7, r1, lsl #10
    1764:   6b040700    blvs    10336c <MGC_DrcDumpPipe+0xff118>
    1768:   0f000017    svceq   0x00000017
    176c:   00177c01    andseq  r7, r7, r1, lsl #24
    1770:   00f11000    rscseq  r1, r1, r0
    1774:   7c100000    ldcvc   0, cr0, [r0], {0}
    1778:   00000017    andeq   r0, r0, r7, lsl r0
    177c:   17820407    strne   r0, [r2, r7, lsl #8]
    1780:   5f120000    svcpl   0x00120000
    1784:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1788:   7072495f    rsbsvc  r4, r2, pc, asr r9
    178c:   88082000    stmdahi r8, {sp}
    1790:   00184701    andseq  r4, r8, r1, lsl #14
    1794:   50680d00    rsbpl   r0, r8, r0, lsl #26
    1798:   00657069    rsbeq   r7, r5, r9, rrx
    179c:   ea01af08    b   6d3c4 <MGC_DrcDumpPipe+0x69170>
    17a0:   02000015    andeq   r0, r0, #21 ; 0x15
    17a4:   700d0023    andvc   r0, sp, r3, lsr #32
    17a8:   66667542    strbtvs r7, [r6], -r2, asr #10
    17ac:   08007265    stmdaeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    17b0:   14e301b0    strbtne r0, [r3], #432
    17b4:   23020000    movwcs  r0, #8192   ; 0x2000
    17b8:   77640d04    strbvc  r0, [r4, -r4, lsl #26]!
    17bc:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    17c0:   08006874    stmdaeq r0, {r2, r4, r5, r6, fp, sp, lr}
    17c4:   018e01b1    strheq  r0, [lr, r1]
    17c8:   23020000    movwcs  r0, #8192   ; 0x2000
    17cc:   00301708    eorseq  r1, r0, r8, lsl #14
            17ce: R_ARM_ABS32   .debug_str
    17d0:   b2080000    andlt   r0, r8, #0  ; 0x0
    17d4:   00018e01    andeq   r8, r1, r1, lsl #28
    17d8:   0c230200    sfmeq   f0, 4, [r3]
    17dc:   00015617    andeq   r5, r1, r7, lsl r6
            17dd: R_ARM_ABS32   .debug_str
    17e0:   01b30800    undefined instruction 0x01b30800
    17e4:   0000018e    andeq   r0, r0, lr, lsl #3
    17e8:   17102302    ldrne   r2, [r0, -r2, lsl #6]
    17ec:   000001c6    andeq   r0, r0, r6, asr #3
            17ec: R_ARM_ABS32   .debug_str
    17f0:   4a01b408    bmi 6e818 <MGC_DrcDumpPipe+0x6a5c4>
    17f4:   02000017    andeq   r0, r0, #23 ; 0x17
    17f8:   0c171423    cfldrseq    mvf1, [r7], {35}
            17fb: R_ARM_ABS32   .debug_str
    17fc:   08000000    stmdaeq r0, {}
    1800:   00f101b5    ldrhteq r0, [r1], #21
    1804:   23020000    movwcs  r0, #8192   ; 0x2000
    1808:   41620d18    cmnmi   r2, r8, lsl sp
    180c:   776f6c6c    strbvc  r6, [pc, -ip, ror #24]!
    1810:   726f6853    rsbvc   r6, pc, #5439488    ; 0x530000
    1814:   61725474    cmnvs   r2, r4, ror r4
    1818:   6566736e    strbvs  r7, [r6, #-878]!
    181c:   b6080072    undefined
    1820:   00016001    andeq   r6, r1, r1
    1824:   1c230200    sfmne   f0, 4, [r3]
    1828:   00004317    andeq   r4, r0, r7, lsl r3
            1829: R_ARM_ABS32   .debug_str
    182c:   01b70800    undefined instruction 0x01b70800
    1830:   00000160    andeq   r0, r0, r0, ror #2
    1834:   171d2302    ldrne   r2, [sp, -r2, lsl #6]
    1838:   000000f0    strdeq  r0, [r0], -r0
            1838: R_ARM_ABS32   .debug_str
    183c:   6001b808    andvs   fp, r1, r8, lsl #16
    1840:   02000001    andeq   r0, r0, #1  ; 0x1
    1844:   11001e23    tstne   r0, r3, lsr #28
    1848:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    184c:   7072495f    rsbsvc  r4, r2, pc, asr r9
    1850:   01b90800    undefined instruction 0x01b90800
    1854:   00001782    andeq   r1, r0, r2, lsl #15
    1858:   53554d11    cmppl   r5, #1088   ; 0x440
    185c:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1860:   636f7349    cmnvs   pc, #603979777  ; 0x24000001
    1864:   70724968    rsbsvc  r4, r2, r8, ror #18
    1868:   706d6f43    rsbvc   r6, sp, r3, asr #30
    186c:   6574656c    ldrbvs  r6, [r4, #-1388]!
    1870:   01c30800    biceq   r0, r3, r0, lsl #16
    1874:   00001878    andeq   r1, r0, r8, ror r8
    1878:   187e0407    ldmdane lr!, {r0, r1, r2, sl}^
    187c:   010f0000    mrseq   r0, CPSR
    1880:   0000188f    andeq   r1, r0, pc, lsl #17
    1884:   0000f110    andeq   pc, r0, r0, lsl r1
    1888:   188f1000    stmne   pc, {ip}
    188c:   07000000    streq   r0, [r0, -r0]
    1890:   00189504    andseq  r9, r8, r4, lsl #10
    1894:   4d5f1200    lfmmi   f1, 2, [pc]
    1898:   5f425355    svcpl   0x00425355
    189c:   636f7349    cmnvs   pc, #603979777  ; 0x24000001
    18a0:   70724968    rsbsvc  r4, r2, r8, ror #18
    18a4:   bb082400    bllt    20a8ac <MGC_DrcDumpPipe+0x206658>
    18a8:   0019a001    andseq  sl, r9, r1
    18ac:   50680d00    rsbpl   r0, r8, r0, lsl #26
    18b0:   00657069    rsbeq   r7, r5, r9, rrx
    18b4:   ea01e808    b   7b8dc <MGC_DrcDumpPipe+0x77688>
    18b8:   02000015    andeq   r0, r0, #21 ; 0x15
    18bc:   700d0023    andvc   r0, sp, r3, lsr #32
    18c0:   66667542    strbtvs r7, [r6], -r2, asr #10
    18c4:   08007265    stmdaeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    18c8:   14e301e9    strbtne r0, [r3], #489
    18cc:   23020000    movwcs  r0, #8192   ; 0x2000
    18d0:   64610d04    strbtvs r0, [r1], #-3332
    18d4:   6e654c77    mcrvs   12, 3, r4, cr5, cr7, {3}
    18d8:   00687467    rsbeq   r7, r8, r7, ror #8
    18dc:   a001ea08    andge   lr, r1, r8, lsl #20
    18e0:   02000019    andeq   r0, r0, #25 ; 0x19
    18e4:   610d0823    tstvs   sp, r3, lsr #16
    18e8:   74537764    ldrbvc  r7, [r3], #-1892
    18ec:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
    18f0:   01eb0800    mvneq   r0, r0, lsl #16
    18f4:   000019a0    andeq   r1, r0, r0, lsr #19
    18f8:   0d0c2302    stceq   3, cr2, [ip, #-8]
    18fc:   41776461    cmnmi   r7, r1, ror #8
    1900:   61757463    cmnvs   r5, r3, ror #8
    1904:   6e654c6c    cdpvs   12, 6, cr4, cr5, cr12, {3}
    1908:   00687467    rsbeq   r7, r8, r7, ror #8
    190c:   a001ec08    andge   lr, r1, r8, lsl #24
    1910:   02000019    andeq   r0, r0, #25 ; 0x19
    1914:   c6171023    ldrgt   r1, [r7], -r3, lsr #32
            1917: R_ARM_ABS32   .debug_str
    1918:   08000001    stmdaeq r0, {r0}
    191c:   185801ed    ldmdane r8, {r0, r2, r3, r5, r6, r7, r8}^
    1920:   23020000    movwcs  r0, #8192   ; 0x2000
    1924:   000c1714    andeq   r1, ip, r4, lsl r7
            1926: R_ARM_ABS32   .debug_str
    1928:   ee080000    cdp 0, 0, cr0, cr8, cr0, {0}
    192c:   0000f101    andeq   pc, r0, r1, lsl #2
    1930:   18230200    stmdane r3!, {r9}
    1934:   7246770d    subvc   r7, r6, #3407872    ; 0x340000
    1938:   43656d61    cmnmi   r5, #6208   ; 0x1840
    193c:   746e756f    strbtvc r7, [lr], #-1391
    1940:   01ef0800    mvneq   r0, r0, lsl #16
    1944:   0000016f    andeq   r0, r0, pc, ror #2
    1948:   0d1c2302    ldceq   3, cr2, [ip, #-8]
    194c:   72754377    rsbsvc  r4, r5, #-603979775 ; 0xdc000001
    1950:   746e6572    strbtvc r6, [lr], #-1394
    1954:   6d617246    sfmvs   f7, 2, [r1, #-280]!
    1958:   f0080065    undefined instruction 0xf0080065
    195c:   00016f01    andeq   r6, r1, r1, lsl #30
    1960:   1e230200    cdpne   2, 2, cr0, cr3, cr0, {0}
    1964:   6143770d    cmpvs   r3, sp, lsl #14
    1968:   61626c6c    cmnvs   r2, ip, ror #24
    196c:   6e496b63    fnmacdvs    d22, d9, d19
    1970:   76726574    undefined
    1974:   08006c61    stmdaeq r0, {r0, r5, r6, sl, fp, sp, lr}
    1978:   016f01f1    strdeq  r0, [pc, #-17]  ; 196f <.debug_info+0x196f>
    197c:   23020000    movwcs  r0, #8192   ; 0x2000
    1980:   00431720    subeq   r1, r3, r0, lsr #14
            1982: R_ARM_ABS32   .debug_str
    1984:   f2080000    vhadd.s8    d0, d8, d0
    1988:   00016001    andeq   r6, r1, r1
    198c:   22230200    eorcs   r0, r3, #0  ; 0x0
    1990:   0000f017    andeq   pc, r0, r7, lsl r0
            1991: R_ARM_ABS32   .debug_str
    1994:   01f30800    mvnseq  r0, r0, lsl #16
    1998:   00000160    andeq   r0, r0, r0, ror #2
    199c:   00232302    eoreq   r2, r3, r2, lsl #6
    19a0:   018e0407    orreq   r0, lr, r7, lsl #8
    19a4:   4d110000    ldcmi   0, cr0, [r1]
    19a8:   5f425355    svcpl   0x00425355
    19ac:   636f7349    cmnvs   pc, #603979777  ; 0x24000001
    19b0:   70724968    rsbsvc  r4, r2, r8, ror #18
    19b4:   01f40800    mvnseq  r0, r0, lsl #16
    19b8:   00001895    muleq   r0, r5, r8
    19bc:   53554d11    cmppl   r5, #1088   ; 0x440
    19c0:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    19c4:   5577654e    ldrbpl  r6, [r7, #-1358]!
    19c8:   74536273    ldrbvc  r6, [r3], #-627
    19cc:   00657461    rsbeq   r7, r5, r1, ror #8
    19d0:   d7020308    strle   r0, [r2, -r8, lsl #6]
    19d4:   07000019    smladeq r0, r9, r0, r0
    19d8:   0019dd04    andseq  sp, r9, r4, lsl #26
    19dc:   f3010f00    vpmax.f32   d0, d1, d0
    19e0:   10000019    andne   r0, r0, r9, lsl r0
    19e4:   000000f1    strdeq  r0, [r0], -r1
    19e8:   00150310    andseq  r0, r5, r0, lsl r3
    19ec:   11c51000    bicne   r1, r5, r0
    19f0:   11000000    tstne   r0, r0
    19f4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    19f8:   4466705f    strbtmi r7, [r6], #-95
    19fc:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    1a00:   71655265    cmnvc   r5, r5, ror #4
    1a04:   74736575    ldrbtvc r6, [r3], #-1397
    1a08:   02190800    andseq  r0, r9, #0  ; 0x0
    1a0c:   00001a10    andeq   r1, r0, r0, lsl sl
    1a10:   1a160407    bne 582a34 <MGC_DrcDumpPipe+0x57e7e0>
    1a14:   01130000    tsteq   r3, r0
    1a18:   00000160    andeq   r0, r0, r0, ror #2
    1a1c:   00001a3a    andeq   r1, r0, sl, lsr sl
    1a20:   0000f110    andeq   pc, r0, r0, lsl r1
    1a24:   15031000    strne   r1, [r3]
    1a28:   8e100000    wxorhi  wr0, wr0, wr0
    1a2c:   10000001    andne   r0, r0, r1
    1a30:   00000b51    andeq   r0, r0, r1, asr fp
    1a34:   00016f10    andeq   r6, r1, r0, lsl pc
    1a38:   4d110000    ldcmi   0, cr0, [r1]
    1a3c:   5f425355    svcpl   0x00425355
    1a40:   65446670    strbvs  r6, [r4, #-1648]
    1a44:   65636976    strbvs  r6, [r3, #-2422]!
    1a48:   666e6f43    strbtvs r6, [lr], -r3, asr #30
    1a4c:   65536769    ldrbvs  r6, [r3, #-1897]
    1a50:   7463656c    strbtvc r6, [r3], #-1388
    1a54:   08006465    stmdaeq r0, {r0, r2, r5, r6, sl, sp, lr}
    1a58:   1a5e0237    bne 178233c <MGC_DrcDumpPipe+0x177e0e8>
    1a5c:   04070000    streq   r0, [r7]
    1a60:   00001a64    andeq   r1, r0, r4, ror #20
    1a64:   01600113    cmneq   r0, r3, lsl r1
    1a68:   1a830000    bne fe0c1a70 <MGC_DrcDumpPipe+0xfe0bd81c>
    1a6c:   f1100000    undefined instruction 0xf1100000
    1a70:   10000000    andne   r0, r0, r0
    1a74:   00001503    andeq   r1, r0, r3, lsl #10
    1a78:   00016010    andeq   r6, r1, r0, lsl r0
    1a7c:   1a831000    bne fe0c5a84 <MGC_DrcDumpPipe+0xfe0c1830>
    1a80:   07000000    streq   r0, [r0, -r0]
    1a84:   0015ea04    andseq  lr, r5, r4, lsl #20
    1a88:   554d1100    strbpl  r1, [sp, #-256]
    1a8c:   705f4253    subsvc  r4, pc, r3, asr r2
    1a90:   746e4966    strbtvc r4, [lr], #-2406
    1a94:   61667265    cmnvs   r6, r5, ror #4
    1a98:   65536563    ldrbvs  r6, [r3, #-1379]
    1a9c:   56080074    undefined
    1aa0:   001aa502    andseq  sl, sl, r2, lsl #10
    1aa4:   ab040700    blge    1036ac <MGC_DrcDumpPipe+0xff458>
    1aa8:   1300001a    movwne  r0, #26 ; 0x1a
    1aac:   00016001    andeq   r6, r1, r1
    1ab0:   001acf00    andseq  ip, sl, r0, lsl #30
    1ab4:   00f11000    rscseq  r1, r1, r0
    1ab8:   03100000    tsteq   r0, #0  ; 0x0
    1abc:   10000015    andne   r0, r0, r5, lsl r0
    1ac0:   00000160    andeq   r0, r0, r0, ror #2
    1ac4:   00016010    andeq   r6, r1, r0, lsl r0
    1ac8:   1a831000    bne fe0c5ad0 <MGC_DrcDumpPipe+0xfe0c187c>
    1acc:   16000000    strne   r0, [r0], -r0
    1ad0:   028e082c    addeq   r0, lr, #2883584    ; 0x2c0000
    1ad4:   00001c40    andeq   r1, r0, r0, asr #24
    1ad8:   00010417    andeq   r0, r1, r7, lsl r4
            1ad9: R_ARM_ABS32   .debug_str
    1adc:   028f0800    addeq   r0, pc, #0  ; 0x0
    1ae0:   000000f1    strdeq  r0, [r0], -r1
    1ae4:   0d002302    stceq   3, cr2, [r0, #-8]
    1ae8:   61745370    cmnvs   r4, r0, ror r3
    1aec:   7261646e    rsbvc   r6, r1, #1845493760 ; 0x6e000000
    1af0:   73654464    cmnvc   r5, #1677721600 ; 0x64000000
    1af4:   70697263    rsbvc   r7, r9, r3, ror #4
    1af8:   73726f74    cmnvc   r2, #464    ; 0x1d0
    1afc:   02900800    addseq  r0, r0, #0  ; 0x0
    1b00:   00000b51    andeq   r0, r0, r1, asr fp
    1b04:   17042302    strne   r2, [r4, -r2, lsl #6]
    1b08:   00000191    muleq   r0, r1, r1
            1b08: R_ARM_ABS32   .debug_str
    1b0c:   6f029108    svcvs   0x00029108
    1b10:   02000001    andeq   r0, r0, #1  ; 0x1
    1b14:   770d0823    strvc   r0, [sp, -r3, lsr #16]
    1b18:   69727453    ldmdbvs r2!, {r0, r1, r4, r6, sl, ip, sp, lr}^
    1b1c:   6544676e    strbvs  r6, [r4, #-1902]
    1b20:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
    1b24:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
    1b28:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    1b2c:   92080074    andls   r0, r8, #116    ; 0x74
    1b30:   00016f02    andeq   r6, r1, r2, lsl #30
    1b34:   0a230200    beq 8c233c <MGC_DrcDumpPipe+0x8be0e8>
    1b38:   6948700d    stmdbvs r8, {r0, r2, r3, ip, sp, lr}^
    1b3c:   70536867    subsvc  r6, r3, r7, ror #16
    1b40:   44646565    strbtmi r6, [r4], #-1381
    1b44:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
    1b48:   6f747069    svcvs   0x00747069
    1b4c:   08007372    stmdaeq r0, {r1, r4, r5, r6, r8, r9, ip, sp, lr}
    1b50:   0b510293    bleq    14425a4 <MGC_DrcDumpPipe+0x143e350>
    1b54:   23020000    movwcs  r0, #8192   ; 0x2000
    1b58:   48770d0c    ldmdami r7!, {r2, r3, r8, sl, fp}^
    1b5c:   53686769    cmnpl   r8, #27525120   ; 0x1a40000
    1b60:   64656570    strbtvs r6, [r5], #-1392
    1b64:   63736544    cmnvs   r3, #285212672  ; 0x11000000
    1b68:   74706972    ldrbtvc r6, [r0], #-2418
    1b6c:   654c726f    strbvs  r7, [ip, #-623]
    1b70:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1b74:   02940800    addseq  r0, r4, #0  ; 0x0
    1b78:   0000016f    andeq   r0, r0, pc, ror #2
    1b7c:   0d102302    ldceq   3, cr2, [r0, #-8]
    1b80:   6e6f4377    mcrvs   3, 3, r4, cr15, cr7, {3}
    1b84:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    1b88:   66667542    strbtvs r7, [r6], -r2, asr #10
    1b8c:   654c7265    strbvs  r7, [ip, #-613]
    1b90:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1b94:   02950800    addseq  r0, r5, #0  ; 0x0
    1b98:   0000016f    andeq   r0, r0, pc, ror #2
    1b9c:   0d122302    ldceq   3, cr2, [r2, #-8]
    1ba0:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    1ba4:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    1ba8:   66667542    strbtvs r7, [r6], -r2, asr #10
    1bac:   08007265    stmdaeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    1bb0:   14e30296    strbtne r0, [r3], #662
    1bb4:   23020000    movwcs  r0, #8192   ; 0x2000
    1bb8:   62700d14    rsbsvs  r0, r0, #1280   ; 0x500
    1bbc:   666c6553    undefined
    1bc0:   65776f50    ldrbvs  r6, [r7, #-3920]!
    1bc4:   00646572    rsbeq   r6, r4, r2, ror r5
    1bc8:   e3029708    movw    r9, #9992   ; 0x2708
    1bcc:   02000014    andeq   r0, r0, #20 ; 0x14
    1bd0:   700d1823    andvc   r1, sp, r3, lsr #16
    1bd4:   76654466    strbtvc r4, [r5], -r6, ror #8
    1bd8:   52656369    rsbpl   r6, r5, #-1543503871    ; 0xa4000001
    1bdc:   65757165    ldrbvs  r7, [r5, #-357]!
    1be0:   08007473    stmdaeq r0, {r0, r1, r4, r5, r6, sl, ip, sp, lr}
    1be4:   19f30298    ldmibne r3!, {r3, r4, r7, r9}^
    1be8:   23020000    movwcs  r0, #8192   ; 0x2000
    1bec:   66700d1c    undefined
    1bf0:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    1bf4:   6f436563    svcvs   0x00436563
    1bf8:   6769666e    strbvs  r6, [r9, -lr, ror #12]!
    1bfc:   656c6553    strbvs  r6, [ip, #-1363]!
    1c00:   64657463    strbtvs r7, [r5], #-1123
    1c04:   02990800    addseq  r0, r9, #0  ; 0x0
    1c08:   00001a3a    andeq   r1, r0, sl, lsr sl
    1c0c:   0d202302    stceq   3, cr2, [r0, #-8]!
    1c10:   6e496670    mcrvs   6, 2, r6, cr9, cr0, {3}
    1c14:   66726574    undefined
    1c18:   53656361    cmnpl   r5, #-2080374783    ; 0x84000001
    1c1c:   08007465    stmdaeq r0, {r0, r2, r5, r6, sl, ip, sp, lr}
    1c20:   1a89029a    bne fe242690 <MGC_DrcDumpPipe+0xfe23e43c>
    1c24:   23020000    movwcs  r0, #8192   ; 0x2000
    1c28:   66700d24    ldrbtvs r0, [r0], -r4, lsr #26
    1c2c:   53627355    cmnpl   r2, #1409286145 ; 0x54000001
    1c30:   65746174    ldrbvs  r6, [r4, #-372]!
    1c34:   029b0800    addseq  r0, fp, #0  ; 0x0
    1c38:   000019bc    strheq  r1, [r0], -ip
    1c3c:   00282302    eoreq   r2, r8, r2, lsl #6
    1c40:   53554d11    cmppl   r5, #1088   ; 0x440
    1c44:   75465f42    strbvc  r5, [r6, #-3906]
    1c48:   6974636e    ldmdbvs r4!, {r1, r2, r3, r5, r6, r8, r9, sp, lr}^
    1c4c:   6c436e6f    mcrrvs  14, 6, r6, r3, cr15
    1c50:   746e6569    strbtvc r6, [lr], #-1385
    1c54:   029c0800    addseq  r0, ip, #0  ; 0x0
    1c58:   00001acf    andeq   r1, r0, pc, asr #21
    1c5c:   53554d11    cmppl   r5, #1088   ; 0x440
    1c60:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1c64:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    1c68:   6f436563    svcvs   0x00436563
    1c6c:   63656e6e    cmnvs   r5, #1760   ; 0x6e0
    1c70:   00646574    rsbeq   r6, r4, r4, ror r5
    1c74:   7b02ac08    blvc    acc9c <MGC_DrcDumpPipe+0xa8a48>
    1c78:   0700001c    smladeq r0, ip, r0, r0
    1c7c:   001c8104    andseq  r8, ip, r4, lsl #2
    1c80:   60011300    andvs   r1, r1, r0, lsl #6
    1c84:   a0000001    andge   r0, r0, r1
    1c88:   1000001c    andne   r0, r0, ip, lsl r0
    1c8c:   000000f1    strdeq  r0, [r0], -r1
    1c90:   00150310    andseq  r0, r5, r0, lsl r3
    1c94:   1ca01000    stcne   0, cr1, [r0]
    1c98:   51100000    tstpl   r0, r0
    1c9c:   0000000b    andeq   r0, r0, fp
    1ca0:   14ef0407    strbtne r0, [pc], #1031 ; 1ca8 <.debug_info+0x1ca8>
    1ca4:   4d110000    ldcmi   0, cr0, [r1]
    1ca8:   5f425355    svcpl   0x00425355
    1cac:   65446670    strbvs  r6, [r4, #-1648]
    1cb0:   65636976    strbvs  r6, [r3, #-2422]!
    1cb4:   63736944    cmnvs   r3, #1114112    ; 0x110000
    1cb8:   656e6e6f    strbvs  r6, [lr, #-3695]!
    1cbc:   64657463    strbtvs r7, [r5], #-1123
    1cc0:   02b60800    adcseq  r0, r6, #0  ; 0x0
    1cc4:   00001cc8    andeq   r1, r0, r8, asr #25
    1cc8:   1cce0407    cfstrdne    mvd0, [lr], {7}
    1ccc:   010f0000    mrseq   r0, CPSR
    1cd0:   00001ce4    andeq   r1, r0, r4, ror #25
    1cd4:   0000f110    andeq   pc, r0, r0, lsl r1
    1cd8:   15031000    strne   r1, [r3]
    1cdc:   a0100000    andsge  r0, r0, r0
    1ce0:   0000001c    andeq   r0, r0, ip, lsl r0
    1ce4:   53554d11    cmppl   r5, #1088   ; 0x440
    1ce8:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1cec:   76697244    strbtvc r7, [r9], -r4, asr #4
    1cf0:   69547265    ldmdbvs r4, {r0, r2, r5, r6, r9, ip, sp, lr}^
    1cf4:   4572656d    ldrbmi  r6, [r2, #-1389]!
    1cf8:   72697078    rsbvc   r7, r9, #120    ; 0x78
    1cfc:   08006465    stmdaeq r0, {r0, r2, r5, r6, sl, sp, lr}
    1d00:   1d0602be    sfmne   f0, 4, [r6, #-760]
    1d04:   04070000    streq   r0, [r7]
    1d08:   00001d0c    andeq   r1, r0, ip, lsl #26
    1d0c:   1d1d010f    ldfnes  f0, [sp, #-60]
    1d10:   f1100000    undefined instruction 0xf1100000
    1d14:   10000000    andne   r0, r0, r0
    1d18:   00001503    andeq   r1, r0, r3, lsl #10
    1d1c:   554d1100    strbpl  r1, [sp, #-256]
    1d20:   705f4253    subsvc  r4, pc, r3, asr r2
    1d24:   73754266    cmnvc   r5, #1610612742 ; 0x60000006
    1d28:   70737553    rsbsvc  r7, r3, r3, asr r5
    1d2c:   65646e65    strbvs  r6, [r4, #-3685]!
    1d30:   c6080064    strgt   r0, [r8], -r4, rrx
    1d34:   001d0602    andseq  r0, sp, r2, lsl #12
    1d38:   554d1100    strbpl  r1, [sp, #-256]
    1d3c:   705f4253    subsvc  r4, pc, r3, asr r2
    1d40:   73754266    cmnvc   r5, #1610612742 ; 0x60000006
    1d44:   75736552    ldrbvc  r6, [r3, #-1362]!
    1d48:   0064656d    rsbeq   r6, r4, sp, ror #10
    1d4c:   0602ce08    streq   ip, [r2], -r8, lsl #28
    1d50:   1200001d    andne   r0, r0, #29 ; 0x1d
    1d54:   53554d5f    cmppl   r5, #6080   ; 0x17c0
    1d58:   65445f42    strbvs  r5, [r4, #-3906]
    1d5c:   65636976    strbvs  r6, [r3, #-2422]!
    1d60:   76697244    strbtvc r7, [r9], -r4, asr #4
    1d64:   1c007265    sfmne   f7, 4, [r0], {101}
    1d68:   2402e408    strcs   lr, [r2], #-1032
    1d6c:   1700001e    smladne r0, lr, r0, r0
    1d70:   00000104    andeq   r0, r0, r4, lsl #2
            1d70: R_ARM_ABS32   .debug_str
    1d74:   f102e508    undefined instruction 0xf102e508
    1d78:   02000000    andeq   r0, r0, #0  ; 0x0
    1d7c:   620d0023    andvs   r0, sp, #35 ; 0x23
    1d80:   656d6954    strbvs  r6, [sp, #-2388]!
    1d84:   756f4372    strbvc  r4, [pc, #-882]!    ; 1a1a <.debug_info+0x1a1a>
    1d88:   0800746e    stmdaeq r0, {r1, r2, r3, r5, r6, sl, ip, sp, lr}
    1d8c:   016002e6    smultteq    r0, r6, r2
    1d90:   23020000    movwcs  r0, #8192   ; 0x2000
    1d94:   64610d04    strbtvs r0, [r1], #-3332
    1d98:   6d695477    cfstrdvs    mvd5, [r9, #-476]!
    1d9c:   65527265    ldrbvs  r7, [r2, #-613]
    1da0:   756c6f73    strbvc  r6, [ip, #-3955]!
    1da4:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    1da8:   e7080073    smlsdx  r8, r3, r0, r0
    1dac:   0019a002    andseq  sl, r9, r2
    1db0:   08230200    stmdaeq r3!, {r9}
    1db4:   4466700d    strbtmi r7, [r6], #-13
    1db8:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    1dbc:   6e6f4365    cdpvs   3, 6, cr4, cr15, cr5, {3}
    1dc0:   7463656e    strbtvc r6, [r3], #-1390
    1dc4:   08006465    stmdaeq r0, {r0, r2, r5, r6, sl, sp, lr}
    1dc8:   1c5c02e8    lfmne   f0, 2, [ip], {232}
    1dcc:   23020000    movwcs  r0, #8192   ; 0x2000
    1dd0:   66700d0c    ldrbtvs r0, [r0], -ip, lsl #26
    1dd4:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    1dd8:   69446563    stmdbvs r4, {r0, r1, r5, r6, r8, sl, sp, lr}^
    1ddc:   6e6f6373    mcrvs   3, 3, r6, cr15, cr3, {3}
    1de0:   7463656e    strbtvc r6, [r3], #-1390
    1de4:   08006465    stmdaeq r0, {r0, r2, r5, r6, sl, sp, lr}
    1de8:   1ca602e9    sfmne   f0, 4, [r6], #932
    1dec:   23020000    movwcs  r0, #8192   ; 0x2000
    1df0:   66700d10    undefined
    1df4:   53737542    cmnpl   r3, #276824064  ; 0x10800000
    1df8:   65707375    ldrbvs  r7, [r0, #-885]!
    1dfc:   6465646e    strbtvs r6, [r5], #-1134
    1e00:   02ea0800    rsceq   r0, sl, #0  ; 0x0
    1e04:   00001d1d    andeq   r1, r0, sp, lsl sp
    1e08:   0d142302    ldceq   3, cr2, [r4, #-8]
    1e0c:   75426670    strbvc  r6, [r2, #-1648]
    1e10:   73655273    cmnvc   r5, #805306375  ; 0x30000007
    1e14:   64656d75    strbtvs r6, [r5], #-3445
    1e18:   02eb0800    rsceq   r0, fp, #0  ; 0x0
    1e1c:   00001d39    andeq   r1, r0, r9, lsr sp
    1e20:   00182302    andseq  r2, r8, r2, lsl #6
    1e24:   53554d11    cmppl   r5, #1088   ; 0x440
    1e28:   65445f42    strbvs  r5, [r4, #-3906]
    1e2c:   65636976    strbvs  r6, [r3, #-2422]!
    1e30:   76697244    strbtvc r7, [r9], -r4, asr #4
    1e34:   08007265    stmdaeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    1e38:   1d5302ec    lfmne   f0, 2, [r3, #-944]
    1e3c:   10160000    andsne  r0, r6, r0
    1e40:   c402fd08    strgt   pc, [r2], #-3336
    1e44:   0d00001e    stceq   0, cr0, [r0, #-120]
    1e48:   72655070    rsbvc   r5, r5, #112    ; 0x70
    1e4c:   65687069    strbvs  r7, [r8, #-105]!
    1e50:   4c6c6172    stfmie  f6, [ip], #-456
    1e54:   00747369    rsbseq  r7, r4, r9, ror #6
    1e58:   5102fe08    tstppl  r2, r8, lsl #28
    1e5c:   0200000b    andeq   r0, r0, #11 ; 0xb
    1e60:   770d0023    strvc   r0, [sp, -r3, lsr #32]
    1e64:   69726550    ldmdbvs r2!, {r4, r6, r8, sl, sp, lr}^
    1e68:   72656870    rsbvc   r6, r5, #7340032    ; 0x700000
    1e6c:   694c6c61    stmdbvs ip, {r0, r5, r6, sl, fp, sp, lr}^
    1e70:   654c7473    strbvs  r7, [ip, #-1139]
    1e74:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1e78:   02ff0800    rscseq  r0, pc, #0  ; 0x0
    1e7c:   0000016f    andeq   r0, r0, pc, ror #2
    1e80:   0d042302    stceq   3, cr2, [r4, #-8]
    1e84:   76654461    strbtvc r4, [r5], -r1, ror #8
    1e88:   44656369    strbtmi r6, [r5], #-873
    1e8c:   65766972    ldrbvs  r6, [r6, #-2418]!
    1e90:   73694c72    cmnvc   r9, #29184  ; 0x7200
    1e94:   00080074    andeq   r0, r8, r4, ror r0
    1e98:   001ec403    andseq  ip, lr, r3, lsl #8
    1e9c:   08230200    stmdaeq r3!, {r9}
    1ea0:   6544620d    strbvs  r6, [r4, #-525]
    1ea4:   65636976    strbvs  r6, [r3, #-2422]!
    1ea8:   76697244    strbtvc r7, [r9], -r4, asr #4
    1eac:   694c7265    stmdbvs ip, {r0, r2, r5, r6, r9, ip, sp, lr}^
    1eb0:   654c7473    strbvs  r7, [ip, #-1139]
    1eb4:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1eb8:   03010800    movweq  r0, #6144   ; 0x1800
    1ebc:   00000160    andeq   r0, r0, r0, ror #2
    1ec0:   000c2302    andeq   r2, ip, r2, lsl #6
    1ec4:   1e240407    cdpne   4, 2, cr0, cr4, cr7, {0}
    1ec8:   4d110000    ldcmi   0, cr0, [r1]
    1ecc:   5f425355    svcpl   0x00425355
    1ed0:   74736f48    ldrbtvc r6, [r3], #-3912
    1ed4:   65696c43    strbvs  r6, [r9, #-3139]!
    1ed8:   0800746e    stmdaeq r0, {r1, r2, r3, r5, r6, sl, ip, sp, lr}
    1edc:   1e3e0302    cdpne   3, 3, cr0, cr14, cr2, {0}
    1ee0:   4d110000    ldcmi   0, cr0, [r1]
    1ee4:   5f425355    svcpl   0x00425355
    1ee8:   654e6670    strbvs  r6, [lr, #-1648]
    1eec:   67744f77    undefined
    1ef0:   74617453    strbtvc r7, [r1], #-1107
    1ef4:   0c080065    stceq   0, cr0, [r8], {101}
    1ef8:   001efd03    andseq  pc, lr, r3, lsl #26
    1efc:   03040700    movweq  r0, #18176  ; 0x4700
    1f00:   0f00001f    svceq   0x0000001f
    1f04:   001f1901    andseq  r1, pc, r1, lsl #18
    1f08:   00f11000    rscseq  r1, r1, r0
    1f0c:   03100000    tsteq   r0, #0  ; 0x0
    1f10:   10000015    andne   r0, r0, r5, lsl r0
    1f14:   000012a2    andeq   r1, r0, r2, lsr #5
    1f18:   554d1100    strbpl  r1, [sp, #-256]
    1f1c:   705f4253    subsvc  r4, pc, r3, asr r2
    1f20:   67744f66    ldrbvs  r4, [r4, -r6, ror #30]!
    1f24:   6f727245    svcvs   0x00727245
    1f28:   17080072    smlsdxne    r8, r2, r0, r0
    1f2c:   001f3103    andseq  r3, pc, r3, lsl #2
    1f30:   37040700    strcc   r0, [r4, -r0, lsl #14]
    1f34:   0f00001f    svceq   0x0000001f
    1f38:   001f4d01    andseq  r4, pc, r1, lsl #26
    1f3c:   00f11000    rscseq  r1, r1, r0
    1f40:   03100000    tsteq   r0, #0  ; 0x0
    1f44:   10000015    andne   r0, r0, r5, lsl r0
    1f48:   0000018e    andeq   r0, r0, lr, lsl #3
    1f4c:   08101600    ldmdaeq r0, {r9, sl, ip}
    1f50:   1fae0329    svcne   0x00ae0329
    1f54:   04170000    ldreq   r0, [r7]
            1f57: R_ARM_ABS32   .debug_str
    1f58:   08000001    stmdaeq r0, {r0}
    1f5c:   00f1032a    rscseq  r0, r1, sl, lsr #6
    1f60:   23020000    movwcs  r0, #8192   ; 0x2000
    1f64:   62700d00    rsbsvs  r0, r0, #0  ; 0x0
    1f68:   69736544    ldmdbvs r3!, {r2, r6, r8, sl, sp, lr}^
    1f6c:   6f486572    svcvs   0x00486572
    1f70:   6f527473    svcvs   0x00527473
    1f74:   0800656c    stmdaeq r0, {r2, r3, r5, r6, r8, sl, sp, lr}
    1f78:   14e3032b    strbtne r0, [r3], #811
    1f7c:   23020000    movwcs  r0, #8192   ; 0x2000
    1f80:   66700d04    ldrbtvs r0, [r0], -r4, lsl #26
    1f84:   5367744f    cmnpl   r7, #1325400064 ; 0x4f000000
    1f88:   65746174    ldrbvs  r6, [r4, #-372]!
    1f8c:   032c0800    teqeq   ip, #0  ; 0x0
    1f90:   00001ee2    andeq   r1, r0, r2, ror #29
    1f94:   0d082302    stceq   3, cr2, [r8, #-8]
    1f98:   744f6670    strbvc  r6, [pc], #1648 ; 1fa0 <.debug_info+0x1fa0>
    1f9c:   72724567    rsbsvc  r4, r2, #432013312  ; 0x19c00000
    1fa0:   0800726f    stmdaeq r0, {r0, r1, r2, r3, r5, r6, r9, ip, sp, lr}
    1fa4:   1f19032d    svcne   0x0019032d
    1fa8:   23020000    movwcs  r0, #8192   ; 0x2000
    1fac:   4d11000c    ldcmi   0, cr0, [r1, #-48]
    1fb0:   5f425355    svcpl   0x00425355
    1fb4:   4367744f    cmnmi   r7, #1325400064 ; 0x4f000000
    1fb8:   6e65696c    cdpvs   9, 6, cr6, cr5, cr12, {3}
    1fbc:   2e080074    mcrcs   0, 0, r0, cr8, cr4, {3}
    1fc0:   001f4d03    andseq  r4, pc, r3, lsl #26
    1fc4:   554d1100    strbpl  r1, [sp, #-256]
    1fc8:   705f4253    subsvc  r4, pc, r3, asr r2
    1fcc:   62754866    rsbsvs  r4, r5, #6684672    ; 0x660000
    1fd0:   6d756e45    ldclvs  14, cr6, [r5, #-276]!
    1fd4:   74617265    strbtvc r7, [r1], #-613
    1fd8:   436e6f69    cmnmi   lr, #420    ; 0x1a4
    1fdc:   6c706d6f    ldclvs  13, cr6, [r0], #-444
    1fe0:   00657465    rsbeq   r7, r5, r5, ror #8
    1fe4:   eb033908    bl  d040c <MGC_DrcDumpPipe+0xcc1b8>
    1fe8:   0700001f    smladeq r0, pc, r0, r0
    1fec:   001ff104    andseq  pc, pc, r4, lsl #2
    1ff0:   02010f00    andeq   r0, r1, #0  ; 0x0
    1ff4:   10000020    andne   r0, r0, r0, lsr #32
    1ff8:   00001ca0    andeq   r1, r0, r0, lsr #25
    1ffc:   001ca010    andseq  sl, ip, r0, lsl r0
    2000:   180b0000    stmdane fp, {}
    2004:   209c2b09    addscs  r2, ip, r9, lsl #22
    2008:   770c0000    strvc   r0, [ip, -r0]
    200c:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
    2010:   657a6953    ldrbvs  r6, [sl, #-2387]!
    2014:   9e2c0900    cdpls   9, 2, cr0, cr12, cr0, {0}
    2018:   02000001    andeq   r0, r0, #1  ; 0x1
    201c:   770c0023    strvc   r0, [ip, -r3, lsr #32]
    2020:   74617453    strbtvc r7, [r1], #-1107
    2024:   74496369    strbvc  r6, [r9], #-873
    2028:   6f436d65    svcvs   0x00436d65
    202c:   00746e75    rsbseq  r6, r4, r5, ror lr
    2030:   019e2d09    orrseq  r2, lr, r9, lsl #26
    2034:   23020000    movwcs  r0, #8192   ; 0x2000
    2038:   53700c04    cmnpl   r0, #1024   ; 0x400
    203c:   69746174    ldmdbvs r4!, {r2, r4, r5, r6, r8, sp, lr}^
    2040:   66754263    ldrbtvs r4, [r5], -r3, ror #4
    2044:   00726566    rsbseq  r6, r2, r6, ror #10
    2048:   00f12e09    rscseq  r2, r1, r9, lsl #28
    204c:   23020000    movwcs  r0, #8192   ; 0x2000
    2050:   49770c08    ldmdbmi r7!, {r3, sl, fp}^
    2054:   436d6574    cmnmi   sp, #486539264  ; 0x1d000000
    2058:   746e756f    strbtvc r7, [lr], #-1391
    205c:   9e2f0900    cdpls   9, 2, cr0, cr15, cr0, {0}
    2060:   02000001    andeq   r0, r0, #1  ; 0x1
    2064:   770c0c23    strvc   r0, [ip, -r3, lsr #24]
    2068:   616e7944    cmnvs   lr, r4, asr #18
    206c:   4963696d    stmdbmi r3!, {r0, r2, r3, r5, r6, r8, fp, sp, lr}^
    2070:   436d6574    cmnmi   sp, #486539264  ; 0x1d000000
    2074:   746e756f    strbtvc r7, [lr], #-1391
    2078:   9e300900    cdpls   9, 3, cr0, cr0, cr0, {0}
    207c:   02000001    andeq   r0, r0, #1  ; 0x1
    2080:   700c1023    andvc   r1, ip, r3, lsr #32
    2084:   616e7944    cmnvs   lr, r4, asr #18
    2088:   4263696d    rsbmi   r6, r3, #1785856    ; 0x1b4000
    208c:   65666675    strbvs  r6, [r6, #-1653]!
    2090:   31090072    tstcc   r9, r2, ror r0
    2094:   000000f1    strdeq  r0, [r0], -r1
    2098:   00142302    andseq  r2, r4, r2, lsl #6
    209c:   53554d02    cmppl   r5, #128    ; 0x80
    20a0:   72415f42    subvc   r5, r1, #264    ; 0x108
    20a4:   00796172    rsbseq  r6, r9, r2, ror r1
    20a8:   20023209    andcs   r3, r2, r9, lsl #4
    20ac:   5f180000    svcpl   0x00180000
    20b0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    20b4:   6e694c5f    mcrvs   12, 3, r4, cr9, cr15, {2}
    20b8:   4c64656b    cfstr64mi   mvdx6, [r4], #-428
    20bc:   00747369    rsbseq  r7, r4, r9, ror #6
    20c0:   fa6e090c    blx 1b844f8 <MGC_DrcDumpPipe+0x1b802a4>
    20c4:   0c000020    stceq   0, cr0, [r0], {32}
    20c8:   65744970    ldrbvs  r4, [r4, #-2416]!
    20cc:   6f09006d    svcvs   0x0009006d
    20d0:   000000f1    strdeq  r0, [r0], -r1
    20d4:   0c002302    stceq   3, cr2, [r0], {2}
    20d8:   78654e70    stmdavc r5!, {r4, r5, r6, r9, sl, fp, lr}^
    20dc:   70090074    andvc   r0, r9, r4, ror r0
    20e0:   000020fa    strdeq  r2, [r0], -sl
    20e4:   0c042302    stceq   3, cr2, [r4], {2}
    20e8:   6f437764    svcvs   0x00437764
    20ec:   00746e75    rsbseq  r6, r4, r5, ror lr
    20f0:   00947109    addseq  r7, r4, r9, lsl #2
    20f4:   23020000    movwcs  r0, #8192   ; 0x2000
    20f8:   04070008    streq   r0, [r7], #-8
    20fc:   000020ae    andeq   r2, r0, lr, lsr #1
    2100:   53554d02    cmppl   r5, #128    ; 0x80
    2104:   694c5f42    stmdbvs ip, {r1, r6, r8, r9, sl, fp, ip, lr}^
    2108:   64656b6e    strbtvs r6, [r5], #-2926
    210c:   7473694c    ldrbtvc r6, [r3], #-2380
    2110:   ae720900    cdpge   9, 7, cr0, cr2, cr0, {0}
    2114:   09000020    stmdbeq r0, {r5}
    2118:   3e1e0a04    fmscscc s0, s28, s8
    211c:   0a000022    beq 21ac <.debug_info+0x21ac>
    2120:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    2124:   4553485f    ldrbmi  r4, [r3, #-2143]
    2128:   4f505f54    svcmi   0x00505f54
    212c:   4e5f5452    mrcmi   4, 2, r5, cr15, cr2, {2}
    2130:   00454e4f    subeq   r4, r5, pc, asr #28
    2134:   554d0a00    strbpl  r0, [sp, #-2560]
    2138:   485f4253    ldmdami pc, {r0, r1, r4, r6, r9, lr}^
    213c:   5f544553    svcpl   0x00544553
    2140:   54524f50    ldrbpl  r4, [r2], #-3920
    2144:   5345545f    movtpl  r5, #21599  ; 0x545f
    2148:   004a5f54    subeq   r5, sl, r4, asr pc
    214c:   554d0a01    strbpl  r0, [sp, #-2561]
    2150:   485f4253    ldmdami pc, {r0, r1, r4, r6, r9, lr}^
    2154:   5f544553    svcpl   0x00544553
    2158:   54524f50    ldrbpl  r4, [r2], #-3920
    215c:   5345545f    movtpl  r5, #21599  ; 0x545f
    2160:   004b5f54    subeq   r5, fp, r4, asr pc
    2164:   554d0a02    strbpl  r0, [sp, #-2562]
    2168:   485f4253    ldmdami pc, {r0, r1, r4, r6, r9, lr}^
    216c:   5f544553    svcpl   0x00544553
    2170:   54524f50    ldrbpl  r4, [r2], #-3920
    2174:   5345545f    movtpl  r5, #21599  ; 0x545f
    2178:   45535f54    ldrbmi  r5, [r3, #-3924]
    217c:   414e5f30    cmpmi   lr, r0, lsr pc
    2180:   0a03004b    beq c22b4 <MGC_DrcDumpPipe+0xbe060>
    2184:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    2188:   4553485f    ldrbmi  r4, [r3, #-2143]
    218c:   4f505f54    svcmi   0x00505f54
    2190:   545f5452    ldrbpl  r5, [pc], #1106 ; 2198 <.debug_info+0x2198>
    2194:   5f545345    svcpl   0x00545345
    2198:   4b434150    blmi    10d26e0 <MGC_DrcDumpPipe+0x10ce48c>
    219c:   04005445    streq   r5, [r0], #-1093
    21a0:   53554d0a    cmppl   r5, #640    ; 0x280
    21a4:   53485f42    movtpl  r5, #36674  ; 0x8f42
    21a8:   505f5445    subspl  r5, pc, r5, asr #8
    21ac:   5f54524f    svcpl   0x0054524f
    21b0:   45534552    ldrbmi  r4, [r3, #-1362]
    21b4:   0a050054    beq 14230c <MGC_DrcDumpPipe+0x13e0b8>
    21b8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    21bc:   4553485f    ldrbmi  r4, [r3, #-2143]
    21c0:   4f505f54    svcmi   0x00505f54
    21c4:   535f5452    cmppl   pc, #1375731712 ; 0x52000000
    21c8:   45505355    ldrbmi  r5, [r0, #-853]
    21cc:   0600444e    streq   r4, [r0], -lr, asr #8
    21d0:   53554d0a    cmppl   r5, #640    ; 0x280
    21d4:   53485f42    movtpl  r5, #36674  ; 0x8f42
    21d8:   505f5445    subspl  r5, pc, r5, asr #8
    21dc:   5f54524f    svcpl   0x0054524f
    21e0:   55534552    ldrbpl  r4, [r3, #-1362]
    21e4:   0700454d    streq   r4, [r0, -sp, asr #10]
    21e8:   53554d0a    cmppl   r5, #640    ; 0x280
    21ec:   53485f42    movtpl  r5, #36674  ; 0x8f42
    21f0:   505f5445    subspl  r5, pc, r5, asr #8
    21f4:   5f54524f    svcpl   0x0054524f
    21f8:   55544553    ldrbpl  r4, [r4, #-1363]
    21fc:   54535f50    ldrbpl  r5, [r3], #-3920
    2200:   00545241    subseq  r5, r4, r1, asr #4
    2204:   554d0a08    strbpl  r0, [sp, #-2568]
    2208:   485f4253    ldmdami pc, {r0, r1, r4, r6, r9, lr}^
    220c:   5f544553    svcpl   0x00544553
    2210:   54524f50    ldrbpl  r4, [r2], #-3920
    2214:   5445535f    strbpl  r5, [r5], #-863
    2218:   495f5055    ldmdbmi pc, {r0, r2, r4, r6, ip, lr}^
    221c:   0a09004e    beq 24235c <MGC_DrcDumpPipe+0x23e108>
    2220:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    2224:   4553485f    ldrbmi  r4, [r3, #-2143]
    2228:   4f505f54    svcmi   0x00505f54
    222c:   535f5452    cmppl   pc, #1375731712 ; 0x52000000
    2230:   50555445    subspl  r5, r5, r5, asr #8
    2234:   4154535f    cmpmi   r4, pc, asr r3
    2238:   00535554    subseq  r5, r3, r4, asr r5
    223c:   4d02000a    stcmi   0, cr0, [r2, #-40]
    2240:   5f425355    svcpl   0x00425355
    2244:   74657348    strbtvc r7, [r5], #-840
    2248:   74726f50    ldrbtvc r6, [r2], #-3920
    224c:   65646f4d    strbvs  r6, [r4, #-3917]!
    2250:   17350a00    ldrne   r0, [r5, -r0, lsl #20]!
    2254:   19000021    stmdbne r0, {r0, r5}
    2258:   53554d5f    cmppl   r5, #6080   ; 0x17c0
    225c:   6d445f42    stclvs  15, cr5, [r4, #-264]
    2260:   61684361    cmnvs   r8, r1, ror #6
    2264:   6c656e6e    stclvs  14, cr6, [r5], #-440
    2268:   74617453    strbtvc r7, [r1], #-1107
    226c:   04007375    streq   r7, [r0], #-885
    2270:   22f8390b    rscscs  r3, r8, #180224 ; 0x2c000
    2274:   4d0a0000    stcmi   0, cr0, [sl]
    2278:   5f425355    svcpl   0x00425355
    227c:   5f414d44    svcpl   0x00414d44
    2280:   54415453    strbpl  r5, [r1], #-1107
    2284:   555f5355    ldrbpl  r5, [pc, #-853] ; 1f37 <.debug_info+0x1f37>
    2288:   4f4e4b4e    svcmi   0x004e4b4e
    228c:   00004e57    andeq   r4, r0, r7, asr lr
    2290:   53554d0a    cmppl   r5, #640    ; 0x280
    2294:   4d445f42    stclmi  15, cr5, [r4, #-264]
    2298:   54535f41    ldrbpl  r5, [r3], #-3905
    229c:   53555441    cmppl   r5, #1090519040 ; 0x41000000
    22a0:   4552465f    ldrbmi  r4, [r2, #-1631]
    22a4:   0a010045    beq 423c0 <MGC_DrcDumpPipe+0x3e16c>
    22a8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    22ac:   414d445f    cmpmi   sp, pc, asr r4
    22b0:   4154535f    cmpmi   r4, pc, asr r3
    22b4:   5f535554    svcpl   0x00535554
    22b8:   59535542    ldmdbpl r3, {r1, r6, r8, sl, ip, lr}^
    22bc:   4d0a0200    sfmmi   f0, 4, [sl]
    22c0:   5f425355    svcpl   0x00425355
    22c4:   5f414d44    svcpl   0x00414d44
    22c8:   54415453    strbpl  r5, [r1], #-1107
    22cc:   425f5355    subsmi  r5, pc, #1409286145 ; 0x54000001
    22d0:   415f5355    cmpmi   pc, r5, asr r3
    22d4:   54524f42    ldrbpl  r4, [r2], #-3906
    22d8:   4d0a0300    stcmi   3, cr0, [sl]
    22dc:   5f425355    svcpl   0x00425355
    22e0:   5f414d44    svcpl   0x00414d44
    22e4:   54415453    strbpl  r5, [r1], #-1107
    22e8:   435f5355    cmpmi   pc, #1409286145 ; 0x54000001
    22ec:   5f45524f    svcpl   0x0045524f
    22f0:   524f4241    subpl   r4, pc, #268435460  ; 0x10000004
    22f4:   00040054    andeq   r0, r4, r4, asr r0
    22f8:   53554d02    cmppl   r5, #128    ; 0x80
    22fc:   6d445f42    stclvs  15, cr5, [r4, #-264]
    2300:   61684361    cmnvs   r8, r1, ror #6
    2304:   6c656e6e    stclvs  14, cr6, [r5], #-440
    2308:   74617453    strbtvc r7, [r1], #-1107
    230c:   0b007375    bleq    1f0e8 <MGC_DrcDumpPipe+0x1ae94>
    2310:   00225744    eoreq   r5, r2, r4, asr #14
    2314:   0b140b00    bleq    504f1c <MGC_DrcDumpPipe+0x500cc8>
    2318:   00237553    eoreq   r7, r3, r3, asr r5
    231c:   01041400    tsteq   r4, r0, lsl #8
            231e: R_ARM_ABS32   .debug_str
    2320:   540b0000    strpl   r0, [fp]
    2324:   000000f1    strdeq  r0, [r0], -r1
    2328:   0c002302    stceq   3, cr2, [r0], {2}
    232c:   614d7764    cmpvs   sp, r4, ror #14
    2330:   6e654c78    mcrvs   12, 3, r4, cr5, cr8, {3}
    2334:   00687467    rsbeq   r7, r8, r7, ror #8
    2338:   018e550b    orreq   r5, lr, fp, lsl #10
    233c:   23020000    movwcs  r0, #8192   ; 0x2000
    2340:   01561404    cmpeq   r6, r4, lsl #8
            2342: R_ARM_ABS32   .debug_str
    2344:   560b0000    strpl   r0, [fp], -r0
    2348:   0000018e    andeq   r0, r0, lr, lsl #3
    234c:   14082302    strne   r2, [r8], #-770
    2350:   000001be    strheq  r0, [r0], -lr
            2350: R_ARM_ABS32   .debug_str
    2354:   22f8570b    rscscs  r5, r8, #2883584    ; 0x2c0000
    2358:   23020000    movwcs  r0, #8192   ; 0x2000
    235c:   44620c0c    strbtmi r0, [r2], #-3084
    2360:   72697365    rsbvc   r7, r9, #-1811939327    ; 0x94000001
    2364:   6f4d6465    svcvs   0x004d6465
    2368:   0b006564    bleq    1b900 <MGC_DrcDumpPipe+0x176ac>
    236c:   00016058    andeq   r6, r1, r8, asr r0
    2370:   10230200    eorne   r0, r3, r0, lsl #4
    2374:   554d0200    strbpl  r0, [sp, #-512]
    2378:   445f4253    ldrbmi  r4, [pc], #595  ; 2380 <.debug_info+0x2380>
    237c:   6843616d    stmdavs r3, {r0, r2, r3, r5, r6, r8, sp, lr}^
    2380:   656e6e61    strbvs  r6, [lr, #-3681]!
    2384:   590b006c    stmdbpl fp, {r2, r3, r5, r6}
    2388:   00002315    andeq   r2, r0, r5, lsl r3
    238c:   53554d02    cmppl   r5, #128    ; 0x80
    2390:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    2394:   53616d44    cmnpl   r1, #4352   ; 0x1100
    2398:   74726174    ldrbtvc r6, [r2], #-372
    239c:   746e6f43    strbtvc r6, [lr], #-3907
    23a0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    23a4:   0b007265    bleq    1ed40 <MGC_DrcDumpPipe+0x1aaec>
    23a8:   0023ad62    eoreq   sl, r3, r2, ror #26
    23ac:   b3040700    movwlt  r0, #18176  ; 0x4700
    23b0:   13000023    movwne  r0, #35 ; 0x23
    23b4:   00016001    andeq   r6, r1, r1
    23b8:   0023c300    eoreq   ip, r3, r0, lsl #6
    23bc:   00f11000    rscseq  r1, r1, r0
    23c0:   02000000    andeq   r0, r0, #0  ; 0x0
    23c4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    23c8:   4466705f    strbtmi r7, [r6], #-95
    23cc:   7453616d    ldrbvc  r6, [r3], #-365
    23d0:   6f43706f    svcvs   0x0043706f
    23d4:   6f72746e    svcvs   0x0072746e
    23d8:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    23dc:   ad6b0b00    vstmdbge    fp!, {d16-d15}
    23e0:   02000023    andeq   r0, r0, #35 ; 0x23
    23e4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    23e8:   4466705f    strbtmi r7, [r6], #-95
    23ec:   6c41616d    stfvse  f6, [r1], {109}
    23f0:   61636f6c    cmnvs   r3, ip, ror #30
    23f4:   68436574    stmdavs r3, {r2, r4, r5, r6, r8, sl, sp, lr}^
    23f8:   656e6e61    strbvs  r6, [lr, #-3681]!
    23fc:   7b0b006c    blvc    2c25b4 <MGC_DrcDumpPipe+0x2be360>
    2400:   00002404    andeq   r2, r0, r4, lsl #8
    2404:   240a0407    strcs   r0, [sl], #-1031
    2408:   01130000    tsteq   r3, r0
    240c:   0000242e    andeq   r2, r0, lr, lsr #8
    2410:   0000242e    andeq   r2, r0, lr, lsr #8
    2414:   0000f110    andeq   pc, r0, r0, lsl r1
    2418:   01601000    cmneq   r0, r0
    241c:   60100000    andsvs  r0, r0, r0
    2420:   10000001    andne   r0, r0, r1
    2424:   00000160    andeq   r0, r0, r0, ror #2
    2428:   00016f10    andeq   r6, r1, r0, lsl pc
    242c:   04070000    streq   r0, [r7]
    2430:   00002375    andeq   r2, r0, r5, ror r3
    2434:   53554d02    cmppl   r5, #128    ; 0x80
    2438:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    243c:   52616d44    rsbpl   r6, r1, #4352   ; 0x1100
    2440:   61656c65    cmnvs   r5, r5, ror #24
    2444:   68436573    stmdavs r3, {r0, r1, r4, r5, r6, r8, sl, sp, lr}^
    2448:   656e6e61    strbvs  r6, [lr, #-3681]!
    244c:   840b006c    strhi   r0, [fp], #-108
    2450:   00002454    andeq   r2, r0, r4, asr r4
    2454:   245a0407    ldrbcs  r0, [sl], #-1031
    2458:   010f0000    mrseq   r0, CPSR
    245c:   00002466    andeq   r2, r0, r6, ror #8
    2460:   00242e10    eoreq   r2, r4, r0, lsl lr
    2464:   4d020000    stcmi   0, cr0, [r2]
    2468:   5f425355    svcpl   0x00425355
    246c:   6d446670    stclvs  6, cr6, [r4, #-448]
    2470:   6c6c4161    stfvse  f4, [ip], #-388
    2474:   7461636f    strbtvc r6, [r1], #-879
    2478:   66754265    ldrbtvs r4, [r5], -r5, ror #4
    247c:   00726566    rsbseq  r6, r2, r6, ror #10
    2480:   2486910b    strcs   r9, [r6], #267
    2484:   04070000    streq   r0, [r7]
    2488:   0000248c    andeq   r2, r0, ip, lsl #9
    248c:   14e30113    strbtne r0, [r3], #275
    2490:   24a10000    strtcs  r0, [r1]
    2494:   2e100000    wxorcs  wr0, wr0, wr0
    2498:   10000024    andne   r0, r0, r4, lsr #32
    249c:   0000018e    andeq   r0, r0, lr, lsl #3
    24a0:   554d0200    strbpl  r0, [sp, #-512]
    24a4:   705f4253    subsvc  r4, pc, r3, asr r2
    24a8:   616d4466    cmnvs   sp, r6, ror #8
    24ac:   656c6552    strbvs  r6, [ip, #-1362]!
    24b0:   42657361    rsbmi   r7, r5, #-2080374783    ; 0x84000001
    24b4:   65666675    strbvs  r6, [r6, #-1653]!
    24b8:   9e0b0072    mcrls   0, 0, r0, cr11, cr2, {3}
    24bc:   000024c0    andeq   r2, r0, r0, asr #9
    24c0:   24c60407    strbcs  r0, [r6], #1031
    24c4:   01130000    tsteq   r3, r0
    24c8:   00000160    andeq   r0, r0, r0, ror #2
    24cc:   000024db    ldrdeq  r2, [r0], -fp
    24d0:   00242e10    eoreq   r2, r4, r0, lsl lr
    24d4:   14e31000    strbtne r1, [r3]
    24d8:   02000000    andeq   r0, r0, #0  ; 0x0
    24dc:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    24e0:   4466705f    strbtmi r7, [r6], #-95
    24e4:   7250616d    subsvc  r6, r0, #1073741851 ; 0x4000001b
    24e8:   6172676f    cmnvs   r2, pc, ror #14
    24ec:   6168436d    cmnvs   r8, sp, ror #6
    24f0:   6c656e6e    stclvs  14, cr6, [r5], #-440
    24f4:   fbb20b00    blx fec850fe <MGC_DrcDumpPipe+0xfec80eaa>
    24f8:   07000024    streq   r0, [r0, -r4, lsr #32]
    24fc:   00250104    eoreq   r0, r5, r4, lsl #2
    2500:   60011300    andvs   r1, r1, r0, lsl #6
    2504:   25000001    strcs   r0, [r0, #-1]
    2508:   10000025    andne   r0, r0, r5, lsr #32
    250c:   0000242e    andeq   r2, r0, lr, lsr #8
    2510:   00016f10    andeq   r6, r1, r0, lsl pc
    2514:   01601000    cmneq   r0, r0
    2518:   51100000    tstpl   r0, r0
    251c:   1000000b    andne   r0, r0, fp
    2520:   0000018e    andeq   r0, r0, lr, lsl #3
    2524:   554d0200    strbpl  r0, [sp, #-512]
    2528:   705f4253    subsvc  r4, pc, r3, asr r2
    252c:   616d4466    cmnvs   sp, r6, ror #8
    2530:   43746547    cmnmi   r4, #297795584  ; 0x11c00000
    2534:   6e6e6168    powvsez f6, f6, #0.0
    2538:   74536c65    ldrbvc  r6, [r3], #-3173
    253c:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
    2540:   47bd0b00    ldrmi   r0, [sp, r0, lsl #22]!
    2544:   07000025    streq   r0, [r0, -r5, lsr #32]
    2548:   00254d04    eoreq   r4, r5, r4, lsl #26
    254c:   f8011300    undefined instruction 0xf8011300
    2550:   5d000022    stcpl   0, cr0, [r0, #-136]
    2554:   10000025    andne   r0, r0, r5, lsr #32
    2558:   0000242e    andeq   r2, r0, lr, lsr #8
    255c:   554d0200    strbpl  r0, [sp, #-512]
    2560:   705f4253    subsvc  r4, pc, r3, asr r2
    2564:   616d4466    cmnvs   sp, r6, ror #8
    2568:   746e6f43    strbtvc r6, [lr], #-3907
    256c:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    2570:   73497265    movtvc  r7, #37477  ; 0x9265
    2574:   d10b0072    tstle   fp, r2, ror r0
    2578:   000023ad    andeq   r2, r0, sp, lsr #7
    257c:   e60b280b    str r2, [fp], -fp, lsl #16
    2580:   000026a3    andeq   r2, r0, r3, lsr #13
    2584:   00010414    andeq   r0, r1, r4, lsl r4
            2585: R_ARM_ABS32   .debug_str
    2588:   f1e70b00    undefined instruction 0xf1e70b00
    258c:   02000000    andeq   r0, r0, #0  ; 0x0
    2590:   700c0023    andvc   r0, ip, r3, lsr #32
    2594:   616d4466    cmnvs   sp, r6, ror #8
    2598:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
    259c:   6e6f4374    mcrvs   3, 3, r4, cr15, cr4, {3}
    25a0:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    25a4:   0072656c    rsbseq  r6, r2, ip, ror #10
    25a8:   238ce80b    orrcs   lr, ip, #720896 ; 0xb0000
    25ac:   23020000    movwcs  r0, #8192   ; 0x2000
    25b0:   66700c04    ldrbtvs r0, [r0], -r4, lsl #24
    25b4:   53616d44    cmnpl   r1, #4352   ; 0x1100
    25b8:   43706f74    cmnmi   r0, #464    ; 0x1d0
    25bc:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    25c0:   656c6c6f    strbvs  r6, [ip, #-3183]!
    25c4:   e90b0072    stmdb   fp, {r1, r4, r5, r6}
    25c8:   000023c3    andeq   r2, r0, r3, asr #7
    25cc:   0c082302    stceq   3, cr2, [r8], {2}
    25d0:   6d446670    stclvs  6, cr6, [r4, #-448]
    25d4:   6c6c4161    stfvse  f4, [ip], #-388
    25d8:   7461636f    strbtvc r6, [r1], #-879
    25dc:   61684365    cmnvs   r8, r5, ror #6
    25e0:   6c656e6e    stclvs  14, cr6, [r5], #-440
    25e4:   e3ea0b00    mvn r0, #0  ; 0x0
    25e8:   02000023    andeq   r0, r0, #35 ; 0x23
    25ec:   700c0c23    andvc   r0, ip, r3, lsr #24
    25f0:   616d4466    cmnvs   sp, r6, ror #8
    25f4:   656c6552    strbvs  r6, [ip, #-1362]!
    25f8:   43657361    cmnmi   r5, #-2080374783    ; 0x84000001
    25fc:   6e6e6168    powvsez f6, f6, #0.0
    2600:   0b006c65    bleq    1d79c <MGC_DrcDumpPipe+0x19548>
    2604:   002434eb    eoreq   r3, r4, fp, ror #9
    2608:   10230200    eorne   r0, r3, r0, lsl #4
    260c:   4466700c    strbtmi r7, [r6], #-12
    2610:   6c41616d    stfvse  f6, [r1], {109}
    2614:   61636f6c    cmnvs   r3, ip, ror #30
    2618:   75426574    strbvc  r6, [r2, #-1396]
    261c:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    2620:   66ec0b00    strbtvs r0, [ip], r0, lsl #22
    2624:   02000024    andeq   r0, r0, #36 ; 0x24
    2628:   700c1423    andvc   r1, ip, r3, lsr #8
    262c:   616d4466    cmnvs   sp, r6, ror #8
    2630:   656c6552    strbvs  r6, [ip, #-1362]!
    2634:   42657361    rsbmi   r7, r5, #-2080374783    ; 0x84000001
    2638:   65666675    strbvs  r6, [r6, #-1653]!
    263c:   ed0b0072    stc 0, cr0, [fp, #-456]
    2640:   000024a1    andeq   r2, r0, r1, lsr #9
    2644:   0c182302    ldceq   3, cr2, [r8], {2}
    2648:   6d446670    stclvs  6, cr6, [r4, #-448]
    264c:   6f725061    svcvs   0x00725061
    2650:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    2654:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    2658:   006c656e    rsbeq   r6, ip, lr, ror #10
    265c:   24dbee0b    ldrbcs  lr, [fp], #3595
    2660:   23020000    movwcs  r0, #8192   ; 0x2000
    2664:   66700c1c    undefined
    2668:   47616d44    strbmi  r6, [r1, -r4, asr #26]!
    266c:   68437465    stmdavs r3, {r0, r2, r5, r6, sl, ip, sp, lr}^
    2670:   656e6e61    strbvs  r6, [lr, #-3681]!
    2674:   6174536c    cmnvs   r4, ip, ror #6
    2678:   00737574    rsbseq  r7, r3, r4, ror r5
    267c:   2525ef0b    strcs   lr, [r5, #-3851]!
    2680:   23020000    movwcs  r0, #8192   ; 0x2000
    2684:   66700c20    ldrbtvs r0, [r0], -r0, lsr #24
    2688:   43616d44    cmnmi   r1, #4352   ; 0x1100
    268c:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    2690:   656c6c6f    strbvs  r6, [ip, #-3183]!
    2694:   72734972    rsbsvc  r4, r3, #1867776    ; 0x1c8000
    2698:   5df00b00    ldclpl  11, cr0, [r0]
    269c:   02000025    andeq   r0, r0, #37 ; 0x25
    26a0:   02002423    andeq   r2, r0, #587202560  ; 0x23000000
    26a4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    26a8:   616d445f    cmnvs   sp, pc, asr r4
    26ac:   746e6f43    strbtvc r6, [lr], #-3907
    26b0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    26b4:   0b007265    bleq    1f050 <MGC_DrcDumpPipe+0x1adfc>
    26b8:   00257cf1    strdeq  r7, [r5], -r1
    26bc:   554d1100    strbpl  r1, [sp, #-256]
    26c0:   705f4253    subsvc  r4, pc, r3, asr r2
    26c4:   616d4466    cmnvs   sp, r6, ror #8
    26c8:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    26cc:   536c656e    cmnpl   ip, #461373440  ; 0x1b800000
    26d0:   75746174    ldrbvc  r6, [r4, #-372]!
    26d4:   61684373    smcvs   33843
    26d8:   6465676e    strbtvs r6, [r5], #-1902
    26dc:   01010b00    tsteq   r1, r0, lsl #22
    26e0:   000026e4    andeq   r2, r0, r4, ror #13
    26e4:   26ea0407    strbtcs r0, [sl], r7, lsl #8
    26e8:   01130000    tsteq   r3, r0
    26ec:   00000160    andeq   r0, r0, r0, ror #2
    26f0:   00002704    andeq   r2, r0, r4, lsl #14
    26f4:   0000f110    andeq   pc, r0, r0, lsl r1
    26f8:   01601000    cmneq   r0, r0
    26fc:   60100000    andsvs  r0, r0, r0
    2700:   00000001    andeq   r0, r0, r1
    2704:   53554d11    cmppl   r5, #1088   ; 0x440
    2708:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    270c:   4477654e    ldrbtmi r6, [r7], #-1358
    2710:   6f43616d    svcvs   0x0043616d
    2714:   6f72746e    svcvs   0x0072746e
    2718:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    271c:   01220b00    teqeq   r2, r0, lsl #22
    2720:   00002724    andeq   r2, r0, r4, lsr #14
    2724:   272a0407    strcs   r0, [sl, -r7, lsl #8]!
    2728:   01130000    tsteq   r3, r0
    272c:   00002753    andeq   r2, r0, r3, asr r7
    2730:   00002753    andeq   r2, r0, r3, asr r7
    2734:   0026bd10    eoreq   fp, r6, r0, lsl sp
    2738:   00f11000    rscseq  r1, r1, r0
    273c:   52100000    andspl  r0, r0, #0  ; 0x0
    2740:   10000009    andne   r0, r0, r9
    2744:   000000f1    strdeq  r0, [r0], -r1
    2748:   0014e310    andseq  lr, r4, r0, lsl r3
    274c:   14e31000    strbtne r1, [r3]
    2750:   07000000    streq   r0, [r0, -r0]
    2754:   0026a304    eoreq   sl, r6, r4, lsl #6
    2758:   554d1100    strbpl  r1, [sp, #-256]
    275c:   705f4253    subsvc  r4, pc, r3, asr r2
    2760:   73654466    cmnvc   r5, #1711276032 ; 0x66000000
    2764:   796f7274    stmdbvc pc!, {r2, r4, r5, r6, r9, ip, sp, lr}^
    2768:   43616d44    cmnmi   r1, #4352   ; 0x1100
    276c:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    2770:   656c6c6f    strbvs  r6, [ip, #-3183]!
    2774:   280b0072    stmdacs fp, {r1, r4, r5, r6}
    2778:   00277d01    eoreq   r7, r7, r1, lsl #26
    277c:   83040700    movwhi  r0, #18176  ; 0x4700
    2780:   0f000027    svceq   0x00000027
    2784:   00278f01    eoreq   r8, r7, r1, lsl #30
    2788:   27531000    ldrbcs  r1, [r3, -r0]
    278c:   16000000    strne   r0, [r0], -r0
    2790:   01390b0c    teqeq   r9, ip, lsl #22
    2794:   000027f9    strdeq  r2, [r0], -r9
    2798:   6f43770d    svcvs   0x0043770d
    279c:   65526572    ldrbvs  r6, [r2, #-1394]
    27a0:   74736967    ldrbtvc r6, [r3], #-2407
    27a4:   45737265    ldrbmi  r7, [r3, #-613]!
    27a8:   6e657478    mcrvs   4, 3, r7, cr5, cr8, {3}
    27ac:   3a0b0074    bcc 2c2984 <MGC_DrcDumpPipe+0x2be730>
    27b0:   00016f01    andeq   r6, r1, r1, lsl #30
    27b4:   00230200    eoreq   r0, r3, r0, lsl #4
    27b8:   4e66700d    cdpmi   0, 6, cr7, cr6, cr13, {0}
    27bc:   6d447765    stclvs  7, cr7, [r4, #-404]
    27c0:   6e6f4361    cdpvs   3, 6, cr4, cr15, cr1, {3}
    27c4:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    27c8:   0072656c    rsbseq  r6, r2, ip, ror #10
    27cc:   04013b0b    streq   r3, [r1], #-2827
    27d0:   02000027    andeq   r0, r0, #39 ; 0x27
    27d4:   700d0423    andvc   r0, sp, r3, lsr #8
    27d8:   73654466    cmnvc   r5, #1711276032 ; 0x66000000
    27dc:   796f7274    stmdbvc pc!, {r2, r4, r5, r6, r9, ip, sp, lr}^
    27e0:   43616d44    cmnmi   r1, #4352   ; 0x1100
    27e4:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    27e8:   656c6c6f    strbvs  r6, [ip, #-3183]!
    27ec:   3c0b0072    stccc   0, cr0, [fp], {114}
    27f0:   00275901    eoreq   r5, r7, r1, lsl #18
    27f4:   08230200    stmdaeq r3!, {r9}
    27f8:   554d1100    strbpl  r1, [sp, #-256]
    27fc:   445f4253    ldrbmi  r4, [pc], #595  ; 2804 <.debug_info+0x2804>
    2800:   6f43616d    svcvs   0x0043616d
    2804:   6f72746e    svcvs   0x0072746e
    2808:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    280c:   74636146    strbtvc r6, [r3], #-326
    2810:   0079726f    rsbseq  r7, r9, pc, ror #4
    2814:   8f013d0b    svchi   0x00013d0b
    2818:   0b000027    bleq    28bc <.debug_info+0x28bc>
    281c:   61940c08    orrsvs  r0, r4, r8, lsl #24
    2820:   0c000028    stceq   0, cr0, [r0], {40}
    2824:   74614470    strbtvc r4, [r1], #-1136
    2828:   980c0061    stmdals ip, {r0, r5, r6}
    282c:   000000f1    strdeq  r0, [r0], -r1
    2830:   0c002302    stceq   3, cr2, [r0], {2}
    2834:   75614362    strbvc  r4, [r1, #-866]!
    2838:   0c006573    cfstr32eq   mvfx6, [r0], {115}
    283c:   00016099    muleq   r1, r9, r0
    2840:   04230200    strteq  r0, [r3], #-512
    2844:   00017014    andeq   r7, r1, r4, lsl r0
            2845: R_ARM_ABS32   .debug_str
    2848:   609a0c00    addsvs  r0, sl, r0, lsl #24
    284c:   02000001    andeq   r0, r0, #1  ; 0x1
    2850:   be140523    cfmul64lt   mvdx0, mvdx4, mvdx3
            2853: R_ARM_ABS32   .debug_str
    2854:   0c000001    stceq   0, cr0, [r0], {1}
    2858:   0001609b    muleq   r1, fp, r0
    285c:   06230200    strteq  r0, [r3], -r0, lsl #4
    2860:   474d0200    strbmi  r0, [sp, -r0, lsl #4]
    2864:   73425f43    movtvc  r5, #12099  ; 0x2f43
    2868:   65744972    ldrbvs  r4, [r4, #-2418]!
    286c:   9c0c006d    stcls   0, cr0, [ip], {109}
    2870:   0000281b    andeq   r2, r0, fp, lsl r8
    2874:   0d0c6016    stceq   0, cr6, [ip, #-88]
    2878:   002bc301    eoreq   ip, fp, r1, lsl #6
    287c:   01701700    cmneq   r0, r0, lsl #14
            287e: R_ARM_ABS32   .debug_str
    2880:   110c0000    tstne   ip, r0
    2884:   00016001    andeq   r6, r1, r1
    2888:   00230200    eoreq   r0, r3, r0, lsl #4
    288c:   7349620d    movtvc  r6, #37389  ; 0x920d
    2890:   6f666946    svcvs   0x00666946
    2894:   72616853    rsbvc   r6, r1, #5439488    ; 0x530000
    2898:   0c006465    cfstrseq    mvf6, [r0], {101}
    289c:   01600112    cmneq   r0, r2, lsl r1
    28a0:   23020000    movwcs  r0, #8192   ; 0x2000
    28a4:   4d770d01    ldclmi  13, cr0, [r7, #-4]!
    28a8:   61507861    cmpvs   r0, r1, ror #16
    28ac:   74656b63    strbtvc r6, [r5], #-2915
    28b0:   657a6953    ldrbvs  r6, [sl, #-2387]!
    28b4:   0c007854    stceq   8, cr7, [r0], {84}
    28b8:   016f0113    cmneq   pc, r3, lsl r1
    28bc:   23020000    movwcs  r0, #8192   ; 0x2000
    28c0:   4d770d02    ldclmi  13, cr0, [r7, #-8]!
    28c4:   61507861    cmpvs   r0, r1, ror #16
    28c8:   74656b63    strbtvc r6, [r5], #-2915
    28cc:   657a6953    ldrbvs  r6, [sl, #-2387]!
    28d0:   0c007852    stceq   8, cr7, [r0], {82}
    28d4:   016f0114    cmneq   pc, r4, lsl r1
    28d8:   23020000    movwcs  r0, #8192   ; 0x2000
    28dc:   00241704    eoreq   r1, r4, r4, lsl #14
            28de: R_ARM_ABS32   .debug_str
    28e0:   170c0000    strne   r0, [ip, -r0]
    28e4:   00016001    andeq   r6, r1, r1
    28e8:   06230200    strteq  r0, [r3], -r0, lsl #4
    28ec:   7542620d    strbvc  r6, [r2, #-525]
    28f0:   646e4573    strbtvs r4, [lr], #-1395
    28f4:   01180c00    tsteq   r8, r0, lsl #24
    28f8:   00000160    andeq   r0, r0, r0, ror #2
    28fc:   17072302    strne   r2, [r7, -r2, lsl #6]
    2900:   00000039    andeq   r0, r0, r9, lsr r0
            2900: R_ARM_ABS32   .debug_str
    2904:   6001190c    andvs   r1, r1, ip, lsl #18
    2908:   02000001    andeq   r0, r0, #1  ; 0x1
    290c:   5a170823    bpl 5c2094 <MGC_DrcDumpPipe+0x5bde40>
            290f: R_ARM_ABS32   .debug_str
    2910:   0c000000    stceq   0, cr0, [r0], {0}
    2914:   0160011a    cmneq   r0, sl, lsl r1
    2918:   23020000    movwcs  r0, #8192   ; 0x2000
    291c:   52620d09    rsbpl   r0, r2, #576    ; 0x240
    2920:   73754278    cmnvc   r5, #-2147483641    ; 0x80000007
    2924:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
    2928:   00737365    rsbseq  r7, r3, r5, ror #6
    292c:   60011b0c    andvs   r1, r1, ip, lsl #22
    2930:   02000001    andeq   r0, r0, #1  ; 0x1
    2934:   620d0a23    andvs   r0, sp, #143360 ; 0x23000
    2938:   75427852    strbvc  r7, [r2, #-2130]
    293c:   646e4573    strbtvs r4, [lr], #-1395
    2940:   011c0c00    tsteq   ip, r0, lsl #24
    2944:   00000160    andeq   r0, r0, r0, ror #2
    2948:   0d0b2302    stceq   3, cr2, [fp, #-8]
    294c:   49785262    ldmdbmi r8!, {r1, r5, r6, r9, ip, lr}^
    2950:   7265746e    rsbvc   r7, r5, #1845493760 ; 0x6e000000
    2954:   006c6176    rsbeq   r6, ip, r6, ror r1
    2958:   60011d0c    andvs   r1, r1, ip, lsl #26
    295c:   02000001    andeq   r0, r0, #1  ; 0x1
    2960:   620d0c23    andvs   r0, sp, #8960   ; 0x2300
    2964:   72547852    subsvc  r7, r4, #5373952    ; 0x520000
    2968:   69666661    stmdbvs r6!, {r0, r5, r6, r9, sl, sp, lr}^
    296c:   70795463    rsbsvc  r5, r9, r3, ror #8
    2970:   1e0c0065    cdpne   0, 0, cr0, cr12, cr5, {3}
    2974:   00016001    andeq   r6, r1, r1
    2978:   0d230200    sfmeq   f0, 4, [r3]
    297c:   7349620d    movtvc  r6, #37389  ; 0x920d
    2980:   0c007854    stceq   8, cr7, [r0], {84}
    2984:   0160011f    cmneq   r0, pc, lsl r1
    2988:   23020000    movwcs  r0, #8192   ; 0x2000
    298c:   49620d0e    stmdbmi r2!, {r1, r2, r3, r8, sl, fp}^
    2990:   6c614873    stclvs  8, cr4, [r1], #-460
    2994:   00646574    rsbeq   r6, r4, r4, ror r5
    2998:   6001200c    andvs   r2, r1, ip
    299c:   02000001    andeq   r0, r0, #1  ; 0x1
    29a0:   af170f23    svcge   0x00170f23
            29a3: R_ARM_ABS32   .debug_str
    29a4:   0c000000    stceq   0, cr0, [r0], {0}
    29a8:   01600121    cmneq   r0, r1, lsr #2
    29ac:   23020000    movwcs  r0, #8192   ; 0x2000
    29b0:   52620d10    rsbpl   r0, r2, #1024   ; 0x400
    29b4:   616c4378    smcvs   50232
    29b8:   64656d69    strbtvs r6, [r5], #-3433
    29bc:   01220c00    teqeq   r2, r0, lsl #24
    29c0:   00000160    andeq   r0, r0, r0, ror #2
    29c4:   0d112302    ldceq   3, cr2, [r1, #-8]
    29c8:   6f745362    svcvs   0x00745362
    29cc:   00785470    rsbseq  r5, r8, r0, ror r4
    29d0:   6001230c    andvs   r2, r1, ip, lsl #6
    29d4:   02000001    andeq   r0, r0, #1  ; 0x1
    29d8:   620d1223    andvs   r1, sp, #805306370  ; 0x30000002
    29dc:   78527349    ldmdavc r2, {r0, r3, r6, r8, r9, ip, sp, lr}^
    29e0:   746c6148    strbtvc r6, [ip], #-328
    29e4:   0c006465    cfstrseq    mvf6, [r0], {101}
    29e8:   01600124    cmneq   r0, r4, lsr #2
    29ec:   23020000    movwcs  r0, #8192   ; 0x2000
    29f0:   008f1713    addeq   r1, pc, r3, lsl r7
            29f2: R_ARM_ABS32   .debug_str
    29f4:   250c0000    strcs   r0, [ip]
    29f8:   00016f01    andeq   r6, r1, r1, lsl #30
    29fc:   14230200    strtne  r0, [r3], #-512
    2a00:   7852770d    ldmdavc r2, {r0, r2, r3, r8, r9, sl, ip, sp, lr}^
    2a04:   6b636150    blvs    18daf4c <MGC_DrcDumpPipe+0x18d6cf8>
    2a08:   69537465    ldmdbvs r3, {r0, r2, r5, r6, sl, ip, sp, lr}^
    2a0c:   0c00657a    cfstr32eq   mvfx6, [r0], {122}
    2a10:   016f0126    cmneq   pc, r6, lsr #2
    2a14:   23020000    movwcs  r0, #8192   ; 0x2000
    2a18:   77640d16    undefined
    2a1c:   69537854    ldmdbvs r3, {r2, r4, r6, fp, ip, sp, lr}^
    2a20:   0c00657a    cfstr32eq   mvfx6, [r0], {122}
    2a24:   018e0127    orreq   r0, lr, r7, lsr #2
    2a28:   23020000    movwcs  r0, #8192   ; 0x2000
    2a2c:   54700d18    ldrbtpl r0, [r0], #-3352
    2a30:   70724978    rsbsvc  r4, r2, r8, ror r9
    2a34:   01290c00    teqeq   r9, r0, lsl #24
    2a38:   000000f1    strdeq  r0, [r0], -r1
    2a3c:   0d1c2302    ldceq   3, cr2, [ip, #-8]
    2a40:   72497854    subvc   r7, r9, #5505024    ; 0x540000
    2a44:   73694c70    cmnvc   r9, #28672  ; 0x7000
    2a48:   2a0c0074    bcs 302c20 <MGC_DrcDumpPipe+0x2fe9cc>
    2a4c:   00210001    eoreq   r0, r1, r1
    2a50:   20230200    eorcs   r0, r3, r0, lsl #4
    2a54:   5477640d    ldrbtpl r6, [r7], #-1037
    2a58:   66664f78    uqsub16vs   r4, r6, r8
    2a5c:   00746573    rsbseq  r6, r4, r3, ror r5
    2a60:   8e012b0c    fmacdhi d2, d1, d12
    2a64:   02000001    andeq   r0, r0, #1  ; 0x1
    2a68:   700d2c23    andvc   r2, sp, r3, lsr #24
    2a6c:   72497852    subvc   r7, r9, #5373952    ; 0x520000
    2a70:   2c0c0070    stccs   0, cr0, [ip], {112}
    2a74:   0000f101    andeq   pc, r0, r1, lsl #2
    2a78:   30230200    eorcc   r0, r3, r0, lsl #4
    2a7c:   4978520d    ldmdbmi r8!, {r0, r2, r3, r9, ip, lr}^
    2a80:   694c7072    stmdbvs ip, {r1, r4, r5, r6, ip, sp, lr}^
    2a84:   0c007473    cfstrseq    mvf7, [r0], {115}
    2a88:   2100012d    tstcs   r0, sp, lsr #2
    2a8c:   23020000    movwcs  r0, #8192   ; 0x2000
    2a90:   77640d34    undefined
    2a94:   664f7852    undefined
    2a98:   74657366    strbtvc r7, [r5], #-870
    2a9c:   012e0c00    teqeq   lr, r0, lsl #24
    2aa0:   0000018e    andeq   r0, r0, lr, lsl #3
    2aa4:   0d402302    stcleq  3, cr2, [r0, #-8]
    2aa8:   616d4470    smcvs   54336
    2aac:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    2ab0:   006c656e    rsbeq   r6, ip, lr, ror #10
    2ab4:   2e01340c    cdpcs   4, 0, cr3, cr1, cr12, {0}
    2ab8:   02000024    andeq   r0, r0, #36 ; 0x24
    2abc:   700d4423    andvc   r4, sp, r3, lsr #8
    2ac0:   6d447852    stclvs  8, cr7, [r4, #-328]
    2ac4:   61684361    cmnvs   r8, r1, ror #6
    2ac8:   6c656e6e    stclvs  14, cr6, [r5], #-440
    2acc:   01350c00    teqeq   r5, r0, lsl #24
    2ad0:   0000242e    andeq   r2, r0, lr, lsr #8
    2ad4:   0d482302    stcleq  3, cr2, [r8, #-8]
    2ad8:   616d4477    smcvs   54343
    2adc:   66667542    strbtvs r7, [r6], -r2, asr #10
    2ae0:   6f437265    svcvs   0x00437265
    2ae4:   00746e75    rsbseq  r6, r4, r5, ror lr
    2ae8:   6f01360c    svcvs   0x0001360c
    2aec:   02000001    andeq   r0, r0, #1  ; 0x1
    2af0:   770d4c23    strvc   r4, [sp, -r3, lsr #24]
    2af4:   6d447852    stclvs  8, cr7, [r4, #-328]
    2af8:   66754261    ldrbtvs r4, [r5], -r1, ror #4
    2afc:   43726566    cmnmi   r2, #427819008  ; 0x19800000
    2b00:   746e756f    strbtvc r7, [lr], #-1391
    2b04:   01370c00    teqeq   r7, r0, lsl #24
    2b08:   0000016f    andeq   r0, r0, pc, ror #2
    2b0c:   0d4e2302    stcleq  3, cr2, [lr, #-8]
    2b10:   616d4462    cmnvs   sp, r2, ror #8
    2b14:   0c007854    stceq   8, cr7, [r0], {84}
    2b18:   01600138    cmneq   r0, r8, lsr r1
    2b1c:   23020000    movwcs  r0, #8192   ; 0x2000
    2b20:   44620d50    strbtmi r0, [r2], #-3408
    2b24:   6552616d    ldrbvs  r6, [r2, #-365]
    2b28:   75646973    strbvc  r6, [r4, #-2419]!
    2b2c:   0c006c61    stceq   12, cr6, [r0], {97}
    2b30:   01600139    cmneq   r0, r9, lsr r1
    2b34:   23020000    movwcs  r0, #8192   ; 0x2000
    2b38:   01041751    tsteq   r4, r1, asr r7
            2b3a: R_ARM_ABS32   .debug_str
    2b3c:   3c0c0000    stccc   0, cr0, [ip], {0}
    2b40:   0000f101    andeq   pc, r0, r1, lsl #2
    2b44:   54230200    strtpl  r0, [r3], #-512
    2b48:   5777640d    ldrbpl  r6, [r7, -sp, lsl #8]!
    2b4c:   46746961    ldrbtmi r6, [r4], -r1, ror #18
    2b50:   656d6172    strbvs  r6, [sp, #-370]!
    2b54:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    2b58:   3d0c0074    stccc   0, cr0, [ip, #-464]
    2b5c:   00018e01    andeq   r8, r1, r1, lsl #28
    2b60:   58230200    stmdapl r3!, {r9}
    2b64:   7854620d    ldmdavc r4, {r0, r2, r3, r9, sp, lr}^
    2b68:   6f666946    svcvs   0x00666946
    2b6c:   657a6953    ldrbvs  r6, [sl, #-2387]!
    2b70:   01400c00    cmpeq   r0, r0, lsl #24
    2b74:   00000160    andeq   r0, r0, r0, ror #2
    2b78:   0d5c2302    ldcleq  3, cr2, [ip, #-8]
    2b7c:   46785262    ldrbtmi r5, [r8], -r2, ror #4
    2b80:   536f6669    cmnpl   pc, #110100480  ; 0x6900000
    2b84:   00657a69    rsbeq   r7, r5, r9, ror #20
    2b88:   6001410c    andvs   r4, r1, ip, lsl #2
    2b8c:   02000001    andeq   r0, r0, #1  ; 0x1
    2b90:   620d5d23    andvs   r5, sp, #2240   ; 0x8c0
    2b94:   52727349    rsbspl  r7, r2, #603979777  ; 0x24000001
    2b98:   64697365    strbtvs r7, [r9], #-869
    2b9c:   006c6175    rsbeq   r6, ip, r5, ror r1
    2ba0:   6001430c    andvs   r4, r1, ip, lsl #6
    2ba4:   02000001    andeq   r0, r0, #1  ; 0x1
    2ba8:   620d5e23    andvs   r5, sp, #560    ; 0x230
    2bac:   52727342    rsbspl  r7, r2, #134217729  ; 0x8000001
    2bb0:   64697365    strbtvs r7, [r9], #-869
    2bb4:   006c6175    rsbeq   r6, ip, r5, ror r1
    2bb8:   6001440c    andvs   r4, r1, ip, lsl #8
    2bbc:   02000001    andeq   r0, r0, #1  ; 0x1
    2bc0:   11005f23    tstne   r0, r3, lsr #30
    2bc4:   5f43474d    svcpl   0x0043474d
    2bc8:   70646e45    rsbvc   r6, r4, r5, asr #28
    2bcc:   746e696f    strbtvc r6, [lr], #-2415
    2bd0:   6f736552    svcvs   0x00736552
    2bd4:   65637275    strbvs  r7, [r3, #-629]!
    2bd8:   01450c00    cmpeq   r5, r0, lsl #24
    2bdc:   00002874    andeq   r2, r0, r4, ror r8
    2be0:   474d5f12    smlaldmi    r5, sp, r2, pc
    2be4:   69505f43    ldmdbvs r0, {r0, r1, r6, r8, r9, sl, fp, ip, lr}^
    2be8:   24006570    strcs   r6, [r0], #-1392
    2bec:   a401580c    strge   r5, [r1], #-2060
    2bf0:   0d00002c    stceq   0, cr0, [r0, #-176]
    2bf4:   73655368    cmnvc   r5, #-1610612735    ; 0xa0000001
    2bf8:   6e6f6973    mcrvs   9, 3, r6, cr15, cr3, {3}
    2bfc:   01590c00    cmpeq   r9, r0, lsl #24
    2c00:   000000f1    strdeq  r0, [r0], -r1
    2c04:   17002302    strne   r2, [r0, -r2, lsl #6]
    2c08:   00000085    andeq   r0, r0, r5, lsl #1
            2c08: R_ARM_ABS32   .debug_str
    2c0c:   a4015a0c    strge   r5, [r1], #-2572
    2c10:   0200002c    andeq   r0, r0, #44 ; 0x2c
    2c14:   7a170423    bvc 5c1094 <MGC_DrcDumpPipe+0x5bce40>
            2c17: R_ARM_ABS32   .debug_str
    2c18:   0c000001    stceq   0, cr0, [r0], {1}
    2c1c:   3559015b    ldrbcc  r0, [r9, #-347]
    2c20:   23020000    movwcs  r0, #8192   ; 0x2000
    2c24:   53700d08    cmnpl   r0, #512    ; 0x200
    2c28:   00746f6c    rsbseq  r6, r4, ip, ror #30
    2c2c:   89015c0c    stmdbhi r1, {r2, r3, sl, fp, ip, lr}
    2c30:   02000035    andeq   r0, r0, #53 ; 0x35
    2c34:   80170c23    andshi  r0, r7, r3, lsr #24
            2c37: R_ARM_ABS32   .debug_str
    2c38:   0c000001    stceq   0, cr0, [r0], {1}
    2c3c:   1561015d    strbne  r0, [r1, #-349]!
    2c40:   23020000    movwcs  r0, #8192   ; 0x2000
    2c44:   011d1710    tsteq   sp, r0, lsl r7
            2c46: R_ARM_ABS32   .debug_str
    2c48:   5e0c0000    cdppl   0, 0, cr0, cr12, cr0, {0}
    2c4c:   00016f01    andeq   r6, r1, r1, lsl #30
    2c50:   14230200    strtne  r0, [r3], #-512
    2c54:   6e49770d    cdpvs   7, 4, cr7, cr9, cr13, {0}
    2c58:   76726574    undefined
    2c5c:   0c006c61    stceq   12, cr6, [r0], {97}
    2c60:   016f015f    cmneq   pc, pc, asr r1
    2c64:   23020000    movwcs  r0, #8192   ; 0x2000
    2c68:   77640d16    undefined
    2c6c:   4278614d    rsbsmi  r6, r8, #1073741843 ; 0x40000013
    2c70:   69547375    ldmdbvs r4, {r0, r2, r4, r5, r6, r8, r9, ip, sp, lr}^
    2c74:   0c00656d    cfstr32eq   mvfx6, [r0], {109}
    2c78:   018e0160    orreq   r0, lr, r0, ror #2
    2c7c:   23020000    movwcs  r0, #8192   ; 0x2000
    2c80:   6d620d18    stclvs  13, cr0, [r2, #-96]!
    2c84:   67616c46    strbvs  r6, [r1, -r6, asr #24]!
    2c88:   610c0073    tstvs   ip, r3, ror r0
    2c8c:   00018e01    andeq   r8, r1, r1, lsl #28
    2c90:   1c230200    sfmne   f0, 4, [r3]
    2c94:   00005a17    andeq   r5, r0, r7, lsl sl
            2c95: R_ARM_ABS32   .debug_str
    2c98:   01620c00    cmneq   r2, r0, lsl #24
    2c9c:   00000160    andeq   r0, r0, r0, ror #2
    2ca0:   00202302    eoreq   r2, r0, r2, lsl #6
    2ca4:   2bc30407    blcs    ff0c3cc8 <MGC_DrcDumpPipe+0xff0bfa74>
    2ca8:   5f1a0000    svcpl   0x001a0000
    2cac:   5f43474d    svcpl   0x0043474d
    2cb0:   74726f50    ldrbtvc r6, [r2], #-3920
    2cb4:   0c026000    stceq   0, cr6, [r2], {0}
    2cb8:   003559f1    ldrshteq    r5, [r5], -r1
    2cbc:   01111700    tsteq   r1, r0, lsl #14
            2cbe: R_ARM_ABS32   .debug_str
    2cc0:   620c0000    andvs   r0, ip, #0  ; 0x0
    2cc4:   003f6703    eorseq  r6, pc, r3, lsl #14
    2cc8:   00230200    eoreq   r0, r3, r0, lsl #4
    2ccc:   6e49700d    cdpvs   0, 4, cr7, cr9, cr13, {0}
    2cd0:   66726574    undefined
    2cd4:   50656361    rsbpl   r6, r5, r1, ror #6
    2cd8:   0074726f    rsbseq  r7, r4, pc, ror #4
    2cdc:   d103630c    tstle   r3, ip, lsl #6
    2ce0:   02000014    andeq   r0, r0, #20 ; 0x14
    2ce4:   04170423    ldreq   r0, [r7], #-1059
            2ce7: R_ARM_ABS32   .debug_str
    2ce8:   0c000001    stceq   0, cr0, [r0], {1}
    2cec:   00f10364    rscseq  r0, r1, r4, ror #6
    2cf0:   23020000    movwcs  r0, #8192   ; 0x2000
    2cf4:   6f4c0d08    svcvs   0x004c0d08
    2cf8:   456c6163    strbmi  r6, [ip, #-355]!
    2cfc:   0073646e    rsbseq  r6, r3, lr, ror #8
    2d00:   9c03650c    cfstr32ls   mvfx6, [r3], {12}
    2d04:   02000020    andeq   r0, r0, #32 ; 0x20
    2d08:   620d0c23    andvs   r0, sp, #8960   ; 0x2300
    2d0c:   746e6157    strbtvc r6, [lr], #-343
    2d10:   68676948    stmdavs r7!, {r3, r6, r8, fp, sp, lr}^
    2d14:   65657053    strbvs  r7, [r5, #-83]!
    2d18:   680c0064    stmdavs ip, {r2, r5, r6}
    2d1c:   00016003    andeq   r6, r1, r3
    2d20:   24230200    strtcs  r0, [r3], #-512
    2d24:   6157620d    cmpvs   r7, sp, lsl #4
    2d28:   6552746e    ldrbvs  r7, [r2, #-1134]
    2d2c:   00746573    rsbseq  r6, r4, r3, ror r5
    2d30:   6003690c    andvs   r6, r3, ip, lsl #18
    2d34:   02000001    andeq   r0, r0, #1  ; 0x1
    2d38:   620d2523    andvs   r2, sp, #146800640  ; 0x8c00000
    2d3c:   746e6157    strbtvc r6, [lr], #-343
    2d40:   70737553    rsbsvc  r7, r3, r3, asr r5
    2d44:   00646e65    rsbeq   r6, r4, r5, ror #28
    2d48:   60036a0c    andvs   r6, r3, ip, lsl #20
    2d4c:   02000001    andeq   r0, r0, #1  ; 0x1
    2d50:   620d2623    andvs   r2, sp, #36700160   ; 0x2300000
    2d54:   746e6157    strbtvc r6, [lr], #-343
    2d58:   75736552    ldrbvc  r6, [r3, #-1362]!
    2d5c:   0c00656d    cfstr32eq   mvfx6, [r0], {109}
    2d60:   0160036b    cmneq   r0, fp, ror #6
    2d64:   23020000    movwcs  r0, #8192   ; 0x2000
    2d68:   46620d27    strbtmi r0, [r2], -r7, lsr #26
    2d6c:   41636e75    smcmi   14053
    2d70:   00726464    rsbseq  r6, r2, r4, ror #8
    2d74:   60036c0c    andvs   r6, r3, ip, lsl #24
    2d78:   02000001    andeq   r0, r0, #1  ; 0x1
    2d7c:   620d2823    andvs   r2, sp, #2293760    ; 0x230000
    2d80:   746e6157    strbtvc r6, [lr], #-343
    2d84:   73736553    cmnvc   r3, #348127232  ; 0x14c00000
    2d88:   006e6f69    rsbeq   r6, lr, r9, ror #30
    2d8c:   60036d0c    andvs   r6, r3, ip, lsl #26
    2d90:   02000001    andeq   r0, r0, #1  ; 0x1
    2d94:   620d2923    andvs   r2, sp, #573440 ; 0x8c000
    2d98:   746e6157    strbtvc r6, [lr], #-343
    2d9c:   74736f48    ldrbtvc r6, [r3], #-3912
    2da0:   036e0c00    cmneq   lr, #0  ; 0x0
    2da4:   00000160    andeq   r0, r0, r0, ror #2
    2da8:   0d2a2302    stceq   3, cr2, [sl, #-8]!
    2dac:   6e615762    cdpvs   7, 6, cr5, cr1, cr2, {3}
    2db0:   73655474    cmnvc   r5, #1946157056 ; 0x74000000
    2db4:   646f4d74    strbtvs r4, [pc], #3444 ; 2dbc <.debug_info+0x2dbc>
    2db8:   6f0c0065    svcvs   0x000c0065
    2dbc:   00016003    andeq   r6, r1, r3
    2dc0:   2b230200    blcs    8c35c8 <MGC_DrcDumpPipe+0x8bf374>
    2dc4:   6554620d    ldrbvs  r6, [r4, #-525]
    2dc8:   6f4d7473    svcvs   0x004d7473
    2dcc:   0c006564    cfstr32eq   mvfx6, [r0], {100}
    2dd0:   01600370    smceq   48
    2dd4:   23020000    movwcs  r0, #8192   ; 0x2000
    2dd8:   53620d2c    cmnpl   r2, #2816   ; 0xb00
    2ddc:   69737365    ldmdbvs r3!, {r0, r2, r5, r6, r8, r9, ip, sp, lr}^
    2de0:   63416e6f    movtvs  r6, #7791   ; 0x1e6f
    2de4:   65766974    ldrbvs  r6, [r6, #-2420]!
    2de8:   03730c00    cmneq   r3, #0  ; 0x0
    2dec:   00000160    andeq   r0, r0, r0, ror #2
    2df0:   0d2d2302    stceq   3, cr2, [sp, #-8]!
    2df4:   73614862    cmnvc   r1, #6422528    ; 0x620000
    2df8:   6b6c7542    blvs    1b20308 <MGC_DrcDumpPipe+0x1b1c0b4>
    2dfc:   696c7053    stmdbvs ip!, {r0, r1, r4, r6, ip, sp, lr}^
    2e00:   760c0074    undefined
    2e04:   00016003    andeq   r6, r1, r3
    2e08:   2e230200    cdpcs   2, 2, cr0, cr3, cr0, {0}
    2e0c:   6148620d    cmpvs   r8, sp, lsl #4
    2e10:   6c754273    lfmvs   f4, 2, [r5], #-460
    2e14:   6d6f436b    stclvs  3, cr4, [pc, #-428]!
    2e18:   656e6962    strbvs  r6, [lr, #-2402]!
    2e1c:   03770c00    cmneq   r7, #0  ; 0x0
    2e20:   00000160    andeq   r0, r0, r0, ror #2
    2e24:   0d2f2302    stceq   3, cr2, [pc, #-8]!
    2e28:   73614862    cmnvc   r1, #6422528    ; 0x620000
    2e2c:   73496248    movtvc  r6, #37448  ; 0x9248
    2e30:   0078546f    rsbseq  r5, r8, pc, ror #8
    2e34:   6003780c    andvs   r7, r3, ip, lsl #16
    2e38:   02000001    andeq   r0, r0, #1  ; 0x1
    2e3c:   620d3023    andvs   r3, sp, #35 ; 0x23
    2e40:   48736148    ldmdami r3!, {r3, r6, r8, sp, lr}^
    2e44:   6f734962    svcvs   0x00734962
    2e48:   0c007852    stceq   8, cr7, [r0], {82}
    2e4c:   01600379    smceq   57
    2e50:   23020000    movwcs  r0, #8192   ; 0x2000
    2e54:   49620d31    stmdbmi r2!, {r0, r4, r5, r8, sl, fp}^
    2e58:   6c754d73    ldclvs  13, cr4, [r5], #-460
    2e5c:   6f706974    svcvs   0x00706974
    2e60:   00746e69    rsbseq  r6, r4, r9, ror #28
    2e64:   60037a0c    andvs   r7, r3, ip, lsl #20
    2e68:   02000001    andeq   r0, r0, #1  ; 0x1
    2e6c:   620d3223    andvs   r3, sp, #805306370  ; 0x30000002
    2e70:   43646e45    cmnmi   r4, #1104   ; 0x450
    2e74:   746e756f    strbtvc r7, [lr], #-1391
    2e78:   037b0c00    cmneq   fp, #0  ; 0x0
    2e7c:   00000160    andeq   r0, r0, r0, ror #2
    2e80:   0d332302    ldceq   3, cr2, [r3, #-8]!
    2e84:   72467764    subvc   r7, r6, #26214400   ; 0x1900000
    2e88:   00656d61    rsbeq   r6, r5, r1, ror #26
    2e8c:   8e037e0c    cdphi   14, 0, cr7, cr3, cr12, {0}
    2e90:   02000001    andeq   r0, r0, #1  ; 0x1
    2e94:   620d3423    andvs   r3, sp, #587202560  ; 0x23000000
    2e98:   69487349    stmdbvs r8, {r0, r3, r6, r8, r9, ip, sp, lr}^
    2e9c:   70536867    subsvc  r6, r3, r7, ror #16
    2ea0:   00646565    rsbeq   r6, r4, r5, ror #10
    2ea4:   60037f0c    andvs   r7, r3, ip, lsl #30
    2ea8:   02000001    andeq   r0, r0, #1  ; 0x1
    2eac:   620d3823    andvs   r3, sp, #2293760    ; 0x230000
    2eb0:   6f4c7349    svcvs   0x004c7349
    2eb4:   65705377    ldrbvs  r5, [r0, #-887]!
    2eb8:   0c006465    cfstrseq    mvf6, [r0], {101}
    2ebc:   01600380    smulbbeq    r0, r0, r3
    2ec0:   23020000    movwcs  r0, #8192   ; 0x2000
    2ec4:   013d1739    teqeq   sp, r9, lsr r7
            2ec6: R_ARM_ABS32   .debug_str
    2ec8:   810c0000    tsthi   ip, r0
    2ecc:   00016003    andeq   r6, r1, r3
    2ed0:   3a230200    bcc 8c36d8 <MGC_DrcDumpPipe+0x8bf484>
    2ed4:   7349620d    movtvc  r6, #37389  ; 0x920d
    2ed8:   70737553    rsbsvc  r7, r3, r3, asr r5
    2edc:   00646e65    rsbeq   r6, r4, r5, ror #28
    2ee0:   6003820c    andvs   r8, r3, ip, lsl #4
    2ee4:   02000001    andeq   r0, r0, #1  ; 0x1
    2ee8:   620d3b23    andvs   r3, sp, #35840  ; 0x8c00
    2eec:   65527349    ldrbvs  r7, [r2, #-841]
    2ef0:   656d7573    strbvs  r7, [sp, #-1395]!
    2ef4:   03830c00    orreq   r0, r3, #0  ; 0x0
    2ef8:   00000160    andeq   r0, r0, r0, ror #2
    2efc:   0d3c2302    ldceq   3, cr2, [ip, #-8]!
    2f00:   53734962    cmnpl   r3, #1605632    ; 0x188000
    2f04:   69737365    ldmdbvs r3!, {r0, r2, r5, r6, r8, r9, ip, sp, lr}^
    2f08:   0c006e6f    stceq   14, cr6, [r0], {111}
    2f0c:   01600384    smulbbeq    r0, r4, r3
    2f10:   23020000    movwcs  r0, #8192   ; 0x2000
    2f14:   49620d3d    stmdbmi r2!, {r0, r2, r3, r4, r5, r8, sl, fp}^
    2f18:   736f4873    cmnvc   pc, #7536640    ; 0x730000
    2f1c:   850c0074    strhi   r0, [ip, #-116]
    2f20:   00016003    andeq   r6, r1, r3
    2f24:   3e230200    cdpcc   2, 2, cr0, cr3, cr0, {0}
    2f28:   6f43620d    svcvs   0x0043620d
    2f2c:   63656e6e    cmnvs   r5, #1760   ; 0x6e0
    2f30:   49726f74    ldmdbmi r2!, {r2, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
    2f34:   860c0064    strhi   r0, [ip], -r4, rrx
    2f38:   00016003    andeq   r6, r1, r3
    2f3c:   3f230200    svccc   0x00230200
    2f40:   7542620d    strbvc  r6, [r2, #-525]
    2f44:   6c6f5673    stclvs  6, cr5, [pc], #-460
    2f48:   65676174    strbvs  r6, [r7, #-372]!
    2f4c:   03870c00    orreq   r0, r7, #0  ; 0x0
    2f50:   00000160    andeq   r0, r0, r0, ror #2
    2f54:   0d402302    stcleq  3, cr2, [r0, #-8]
    2f58:   646e4562    strbtvs r4, [lr], #-1378
    2f5c:   61745330    cmnvs   r4, r0, lsr r3
    2f60:   00737574    rsbseq  r7, r3, r4, ror r5
    2f64:   60038a0c    andvs   r8, r3, ip, lsl #20
    2f68:   02000001    andeq   r0, r0, #1  ; 0x1
    2f6c:   770d4123    strvc   r4, [sp, -r3, lsr #2]
    2f70:   4d646e45    stclmi  14, cr6, [r4, #-276]!
    2f74:   006b7361    rsbeq   r7, fp, r1, ror #6
    2f78:   6f038b0c    svcvs   0x00038b0c
    2f7c:   02000001    andeq   r0, r0, #1  ; 0x1
    2f80:   620d4223    andvs   r4, sp, #805306370  ; 0x30000002
    2f84:   43616d44    cmnmi   r1, #4352   ; 0x1100
    2f88:   6c706d6f    ldclvs  13, cr6, [r0], #-444
    2f8c:   64657465    strbtvs r7, [r5], #-1125
    2f90:   038c0c00    orreq   r0, ip, #0  ; 0x0
    2f94:   00000160    andeq   r0, r0, r0, ror #2
    2f98:   0d442302    stcleq  3, cr2, [r4, #-8]
    2f9c:   75625662    strbvc  r5, [r2, #-1634]!
    2fa0:   72724573    rsbsvc  r4, r2, #482344960  ; 0x1cc00000
    2fa4:   6552726f    ldrbvs  r7, [r2, #-623]
    2fa8:   65697274    strbvs  r7, [r9, #-628]!
    2fac:   8d0c0073    stchi   0, cr0, [ip, #-460]
    2fb0:   00016003    andeq   r6, r1, r3
    2fb4:   45230200    strmi   r0, [r3, #-512]!
    2fb8:   6148620d    cmpvs   r8, sp, lsl #4
    2fbc:   73694473    cmnvc   r9, #1929379840 ; 0x73000000
    2fc0:   656c6261    strbvs  r6, [ip, #-609]!
    2fc4:   676e6950    undefined
    2fc8:   038e0c00    orreq   r0, lr, #0  ; 0x0
    2fcc:   00000160    andeq   r0, r0, r0, ror #2
    2fd0:   0d462302    stcleq  3, cr2, [r6, #-8]
    2fd4:   62614262    rsbvs   r4, r1, #536870918  ; 0x20000006
    2fd8:   52656c62    rsbpl   r6, r5, #25088  ; 0x6200
    2fdc:   69727465    ldmdbvs r2!, {r0, r2, r5, r6, sl, ip, sp, lr}^
    2fe0:   0c007365    stceq   3, cr7, [r0], {101}
    2fe4:   0160038f    smulbbeq    r0, pc, r3
    2fe8:   23020000    movwcs  r0, #8192   ; 0x2000
    2fec:   48620d47    stmdami r2!, {r0, r1, r2, r6, r8, sl, fp}^
    2ff0:   71527361    cmpvc   r2, r1, ror #6
    2ff4:   43746b50    cmnmi   r4, #81920  ; 0x14000
    2ff8:   746e756f    strbtvc r7, [lr], #-1391
    2ffc:   03900c00    orrseq  r0, r0, #0  ; 0x0
    3000:   00000160    andeq   r0, r0, r0, ror #2
    3004:   0d482302    stcleq  3, cr2, [r8, #-8]
    3008:   64615062    strbtvs r5, [r1], #-98
    300c:   910c0031    tstls   ip, r1, lsr r0
    3010:   00016003    andeq   r6, r1, r3
    3014:   49230200    stmdbmi r3!, {r9}
    3018:   6150620d    cmpvs   r0, sp, lsl #4
    301c:   0c003264    sfmeq   f3, 4, [r0], {100}
    3020:   01600392    strbeq  r0, [r0, #-50]!
    3024:   23020000    movwcs  r0, #8192   ; 0x2000
    3028:   50620d4a    rsbpl   r0, r2, sl, asr #26
    302c:   00336461    eorseq  r6, r3, r1, ror #8
    3030:   6003930c    andvs   r9, r3, ip, lsl #6
    3034:   02000001    andeq   r0, r0, #1  ; 0x1
    3038:   620d4b23    andvs   r4, sp, #35840  ; 0x8c00
    303c:   5367744f    cmnpl   r7, #1325400064 ; 0x4f000000
    3040:   65746174    ldrbvs  r6, [r4, #-372]!
    3044:   03960c00    orrseq  r0, r6, #0  ; 0x0
    3048:   00000160    andeq   r0, r0, r0, ror #2
    304c:   0d4c2302    stcleq  3, cr2, [ip, #-8]
    3050:   6c655262    sfmvs   f5, 2, [r5], #-392
    3054:   75716e69    ldrbvc  r6, [r1, #-3689]!
    3058:   00687369    rsbeq   r7, r8, r9, ror #6
    305c:   6003970c    andvs   r9, r3, ip, lsl #14
    3060:   02000001    andeq   r0, r0, #1  ; 0x1
    3064:   620d4d23    andvs   r4, sp, #2240   ; 0x8c0
    3068:   6e487349    cdpvs   3, 4, cr7, cr8, cr9, {2}
    306c:   70755370    rsbsvc  r5, r5, r0, ror r3
    3070:   74726f70    ldrbtvc r6, [r2], #-3952
    3074:   0c006465    cfstrseq    mvf6, [r0], {101}
    3078:   01600398    strbeq  r0, [r0, #-56]!
    307c:   23020000    movwcs  r0, #8192   ; 0x2000
    3080:   49620d4e    stmdbmi r2!, {r1, r2, r3, r6, r8, sl, fp}^
    3084:   706e4873    rsbvc   r4, lr, r3, ror r8
    3088:   6f6c6c41    svcvs   0x006c6c41
    308c:   00646577    rsbeq   r6, r4, r7, ror r5
    3090:   6003990c    andvs   r9, r3, ip, lsl #18
    3094:   02000001    andeq   r0, r0, #1  ; 0x1
    3098:   e5174f23    ldr r4, [r7, #-3875]
            309b: R_ARM_ABS32   .debug_str
    309c:   0c000000    stceq   0, cr0, [r0], {0}
    30a0:   3f6d039a    svccc   0x006d039a
    30a4:   23020000    movwcs  r0, #8192   ; 0x2000
    30a8:   6e450d50    mcrvs   13, 2, r0, cr5, cr0, {2}
    30ac:   72656d75    rsbvc   r6, r5, #7488   ; 0x1d40
    30b0:   6f697461    svcvs   0x00697461
    30b4:   7461446e    strbtvc r4, [r1], #-1134
    30b8:   9e0c0061    cdpls   0, 0, cr0, cr12, cr1, {3}
    30bc:   003f4b03    eorseq  r4, pc, r3, lsl #22
    30c0:   54230200    strtpl  r0, [r3], #-512
    30c4:   00000017    andeq   r0, r0, r7, lsl r0
            30c5: R_ARM_ABS32   .debug_str
    30c8:   039f0c00    orrseq  r0, pc, #0  ; 0x0
    30cc:   00003f73    andeq   r3, r0, r3, ror pc
    30d0:   03ac2303    undefined instruction 0x03ac2303
    30d4:   4466700d    strbtmi r7, [r6], #-13
    30d8:   65766972    ldrbvs  r6, [r6, #-2418]!
    30dc:   6d695472    cfstrdvs    mvd5, [r9, #-456]!
    30e0:   78457265    stmdavc r5, {r0, r2, r5, r6, r9, ip, sp, lr}^
    30e4:   65726970    ldrbvs  r6, [r2, #-2416]!
    30e8:   a00c0064    andge   r0, ip, r4, rrx
    30ec:   001ce403    andseq  lr, ip, r3, lsl #8
    30f0:   b0230300    eorlt   r0, r3, r0, lsl #6
    30f4:   44700d03    ldrbtmi r0, [r0], #-3331
    30f8:   65766972    ldrbvs  r6, [r6, #-2418]!
    30fc:   6d695472    cfstrdvs    mvd5, [r9, #-456]!
    3100:   61447265    cmpvs   r4, r5, ror #4
    3104:   0c006174    stfeqs  f6, [r0], {116}
    3108:   00f103a1    rscseq  r0, r1, r1, lsr #7
    310c:   23030000    movwcs  r0, #12288  ; 0x3000
    3110:   700d03b4    strhvc  r0, [sp], -r4
    3114:   746f6f52    strbtvc r6, [pc], #3922 ; 311c <.debug_info+0x311c>
    3118:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    311c:   0c006563    cfstr32eq   mvfx6, [r0], {99}
    3120:   1ca003a2    stcne   3, cr0, [r0], #648
    3124:   23030000    movwcs  r0, #12288  ; 0x3000
    3128:   430d03b8    movwmi  r0, #54200  ; 0xd3b8
    312c:   656e6e6f    strbvs  r6, [lr, #-3695]!
    3130:   64657463    strbtvs r7, [r5], #-1123
    3134:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    3138:   694c6563    stmdbvs ip, {r0, r1, r5, r6, r8, sl, sp, lr}^
    313c:   0c007473    cfstrseq    mvf7, [r0], {115}
    3140:   210003a3    smlatbcs    r0, r3, r3, r0
    3144:   23030000    movwcs  r0, #12288  ; 0x3000
    3148:   530d03bc    movwpl  r0, #54204  ; 0xd3bc
    314c:   64656863    strbtvs r6, [r5], #-2147
    3150:   00656c75    rsbeq   r6, r5, r5, ror ip
    3154:   f003a40c    undefined instruction 0xf003a40c
    3158:   03000035    movweq  r0, #53 ; 0x35
    315c:   0d03c823    stceq   8, cr12, [r3, #-140]
    3160:   736f4877    cmnvc   pc, #7798784    ; 0x770000
    3164:   78614d74    stmdavc r1!, {r2, r4, r5, r6, r8, sl, fp, lr}^
    3168:   65776f50    ldrbvs  r6, [r7, #-3920]!
    316c:   a50c0072    strge   r0, [ip, #-114]
    3170:   00016f03    andeq   r6, r1, r3, lsl #30
    3174:   dc230300    stcle   3, cr0, [r3]
    3178:   48770d03    ldmdami r7!, {r0, r1, r8, sl, fp}^
    317c:   5574736f    ldrbpl  r7, [r4, #-879]!
    3180:   50646573    rsbpl   r6, r4, r3, ror r5
    3184:   7265776f    rsbvc   r7, r5, #29097984   ; 0x1bc0000
    3188:   03a60c00    undefined instruction 0x03a60c00
    318c:   0000016f    andeq   r0, r0, pc, ror #2
    3190:   03de2303    bicseq  r2, lr, #201326592  ; 0xc000000
    3194:   00006e17    andeq   r6, r0, r7, lsl lr
            3195: R_ARM_ABS32   .debug_str
    3198:   03aa0c00    undefined instruction 0x03aa0c00
    319c:   00003f79    andeq   r3, r0, r9, ror pc
    31a0:   03e02303    mvneq   r2, #201326592  ; 0xc000000
    31a4:   00009b17    andeq   r9, r0, r7, lsl fp
            31a5: R_ARM_ABS32   .debug_str
    31a8:   03ab0c00    undefined instruction 0x03ab0c00
    31ac:   00003f7f    andeq   r3, r0, pc, ror pc
    31b0:   03e42303    mvneq   r2, #201326592  ; 0xc000000
    31b4:   7543700d    strbvc  r7, [r3, #-13]
    31b8:   6e657272    mcrvs   2, 3, r7, cr5, cr2, {3}
    31bc:   6e6f4374    mcrvs   3, 3, r4, cr15, cr4, {3}
    31c0:   00676966    rsbeq   r6, r7, r6, ror #18
    31c4:   8503ac0c    strhi   sl, [r3, #-3084]
    31c8:   0300003f    movweq  r0, #63 ; 0x3f
    31cc:   0d03e823    stceq   8, cr14, [r3, #-140]
    31d0:   69507061    ldmdbvs r0, {r0, r5, r6, ip, sp, lr}^
    31d4:   0c006570    cfstr32eq   mvfx6, [r0], {112}
    31d8:   3f9003ad    svccc   0x009003ad
    31dc:   23030000    movwcs  r0, #12288  ; 0x3000
    31e0:   620d03ec    andvs   r0, sp, #-1342177277    ; 0xb0000003
    31e4:   53627355    cmnpl   r2, #1409286145 ; 0x54000001
    31e8:   65746174    ldrbvs  r6, [r4, #-372]!
    31ec:   03ae0c00    undefined instruction 0x03ae0c00
    31f0:   00000160    andeq   r0, r0, r0, ror #2
    31f4:   03f02303    mvnseq  r2, #201326592  ; 0xc000000
    31f8:   614d620d    cmpvs   sp, sp, lsl #4
    31fc:   6e754678    mrcvs   6, 3, r4, cr5, cr8, {3}
    3200:   6f697463    svcvs   0x00697463
    3204:   646e456e    strbtvs r4, [lr], #-1390
    3208:   af0c0073    svcge   0x000c0073
    320c:   00016003    andeq   r6, r1, r3
    3210:   f1230300    undefined instruction 0xf1230300
    3214:   43620d03    cmnmi   r2, #192    ; 0xc0
    3218:   61576e61    cmpvs   r7, r1, ror #28
    321c:   7075656b    rsbsvc  r6, r5, fp, ror #10
    3220:   03b00c00    movseq  r0, #0  ; 0x0
    3224:   00000160    andeq   r0, r0, r0, ror #2
    3228:   03f22303    mvnseq  r2, #201326592  ; 0xc000000
    322c:   6553620d    ldrbvs  r6, [r3, #-525]
    3230:   64644174    strbtvs r4, [r4], #-372
    3234:   73736572    cmnvc   r3, #478150656  ; 0x1c800000
    3238:   03b10c00    undefined instruction 0x03b10c00
    323c:   00000160    andeq   r0, r0, r0, ror #2
    3240:   03f32303    mvnseq  r2, #201326592  ; 0xc000000
    3244:   6f43620d    svcvs   0x0043620d
    3248:   6769666e    strbvs  r6, [r9, -lr, ror #12]!
    324c:   756c6156    strbvc  r6, [ip, #-342]!
    3250:   b20c0065    andlt   r0, ip, #101    ; 0x65
    3254:   00016003    andeq   r6, r1, r3
    3258:   f4230300    vld2.8  {d0-d3}, [r3], r0
    325c:   50620d03    rsbpl   r0, r2, r3, lsl #26
    3260:   65737261    ldrbvs  r7, [r3, #-609]!
    3264:   03b30c00    undefined instruction 0x03b30c00
    3268:   00000160    andeq   r0, r0, r0, ror #2
    326c:   03f52303    mvnseq  r2, #201326592  ; 0xc000000
    3270:   6553770d    ldrbvs  r7, [r3, #-1805]
    3274:   44707574    ldrbtmi r7, [r0], #-1396
    3278:   49617461    stmdbmi r1!, {r0, r5, r6, sl, ip, sp, lr}^
    327c:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
    3280:   03b40c00    undefined instruction 0x03b40c00
    3284:   0000016f    andeq   r0, r0, pc, ror #2
    3288:   03f62303    mvnseq  r2, #201326592  ; 0xc000000
    328c:   6553770d    ldrbvs  r7, [r3, #-1805]
    3290:   52707574    rsbspl  r7, r0, #486539264  ; 0x1d000000
    3294:   646e4978    strbtvs r4, [lr], #-2424
    3298:   0c007865    stceq   8, cr7, [r0], {101}
    329c:   016f03b5    strheq  r0, [pc, #-53]  ; 326f <.debug_info+0x326f>
    32a0:   23030000    movwcs  r0, #12288  ; 0x3000
    32a4:   770d03f8    undefined
    32a8:   75746553    ldrbvc  r6, [r4, #-1363]!
    32ac:   74614470    strbtvc r4, [r1], #-1136
    32b0:   7a695361    bvc 1a5803c <MGC_DrcDumpPipe+0x1a53de8>
    32b4:   b60c0065    strlt   r0, [ip], -r5, rrx
    32b8:   00016f03    andeq   r6, r1, r3, lsl #30
    32bc:   fa230300    blx 8c3ec4 <MGC_DrcDumpPipe+0x8bfc70>
    32c0:   53700d03    cmnpl   r0, #192    ; 0xc0
    32c4:   70757465    rsbsvc  r7, r5, r5, ror #8
    32c8:   61746144    cmnvs   r4, r4, asr #2
    32cc:   03b70c00    undefined instruction 0x03b70c00
    32d0:   000014e3    andeq   r1, r0, r3, ror #9
    32d4:   03fc2303    mvnseq  r2, #201326592  ; 0xc000000
    32d8:   5377640d    cmnpl   r7, #218103808  ; 0xd000000
    32dc:   65757165    ldrbvs  r7, [r5, #-357]!
    32e0:   4e65636e    cdpmi   3, 6, cr6, cr5, cr14, {3}
    32e4:   65626d75    strbvs  r6, [r2, #-3445]!
    32e8:   b80c0072    stmdalt ip, {r1, r4, r5, r6}
    32ec:   00018e03    andeq   r8, r1, r3, lsl #28
    32f0:   80230300    eorhi   r0, r3, r0, lsl #6
    32f4:   62610d04    rsbvs   r0, r1, #256    ; 0x100
    32f8:   63616649    cmnvs   r1, #76546048   ; 0x4900000
    32fc:   66664f65    strbtvs r4, [r6], -r5, ror #30
    3300:   73746573    cmnvc   r4, #482344960  ; 0x1cc00000
    3304:   03b90c00    undefined instruction 0x03b90c00
    3308:   00003f3b    andeq   r3, r0, fp, lsr pc
    330c:   04842303    streq   r2, [r4], #771
    3310:   6d44700d    stclvs  0, cr7, [r4, #-52]
    3314:   6e6f4361    cdpvs   3, 6, cr4, cr15, cr1, {3}
    3318:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    331c:   0072656c    rsbseq  r6, r2, ip, ror #10
    3320:   5303c90c    movwpl  ip, #14604  ; 0x390c
    3324:   03000027    movweq  r0, #39 ; 0x27
    3328:   0d049423    cfstrseq    mvf9, [r4, #-140]
    332c:   65526670    ldrbvs  r6, [r2, #-1648]
    3330:   75426461    strbvc  r6, [r2, #-1121]
    3334:   61745373    cmnvs   r4, r3, ror r3
    3338:   0c006574    cfstr32eq   mvfx6, [r0], {116}
    333c:   3a1c03cc    bcc 704274 <MGC_DrcDumpPipe+0x700020>
    3340:   23030000    movwcs  r0, #12288  ; 0x3000
    3344:   700d0498    mulvc   sp, r8, r4
    3348:   6f725066    svcvs   0x00725066
    334c:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    3350:   53737542    cmnpl   r3, #276824064  ; 0x10800000
    3354:   65746174    ldrbvs  r6, [r4, #-372]!
    3358:   03cd0c00    biceq   r0, sp, #0  ; 0x0
    335c:   00003a4d    andeq   r3, r0, sp, asr #20
    3360:   049c2303    ldreq   r2, [ip], #771
    3364:   5266700d    rsbpl   r7, r6, #13 ; 0xd
    3368:   74657365    strbtvc r7, [r5], #-869
    336c:   74726f50    ldrbtvc r6, [r2], #-3920
    3370:   03ce0c00    biceq   r0, lr, #0  ; 0x0
    3374:   00003a6b    andeq   r3, r0, fp, ror #20
    3378:   04a02303    strteq  r2, [r0], #771
    337c:   4266700d    rsbmi   r7, r6, #13 ; 0xd
    3380:   45646e69    strbmi  r6, [r4, #-3689]!
    3384:   6f70646e    svcvs   0x0070646e
    3388:   00746e69    rsbseq  r6, r4, r9, ror #28
    338c:   8303cf0c    movwhi  ip, #16140  ; 0x3f0c
    3390:   0300003a    movweq  r0, #58 ; 0x3a
    3394:   0d04a423    cfstrseq    mvf10, [r4, #-140]
    3398:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
    339c:   6172676f    cmnvs   r2, pc, ror #14
    33a0:   6174536d    cmnvs   r4, sp, ror #6
    33a4:   65527472    ldrbvs  r7, [r2, #-1138]
    33a8:   76696563    strbtvc r6, [r9], -r3, ror #10
    33ac:   d00c0065    andle   r0, ip, r5, rrx
    33b0:   003ace03    eorseq  ip, sl, r3, lsl #28
    33b4:   a8230300    stmdage r3!, {r8, r9}
    33b8:   66700d04    ldrbtvs r0, [r0], -r4, lsl #26
    33bc:   676f7250    undefined
    33c0:   536d6172    cmnpl   sp, #-2147483620    ; 0x8000001c
    33c4:   74726174    ldrbtvc r6, [r2], #-372
    33c8:   6e617254    mcrvs   2, 3, r7, cr1, cr4, {2}
    33cc:   74696d73    strbtvc r6, [r9], #-3443
    33d0:   03d10c00    bicseq  r0, r1, #0  ; 0x0
    33d4:   00003b1f    andeq   r3, r0, pc, lsl fp
    33d8:   04ac2303    strteq  r2, [ip], #771
    33dc:   5066700d    rsbpl   r7, r6, sp
    33e0:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    33e4:   6c466d61    mcrrvs  13, 6, r6, r6, cr1
    33e8:   45687375    strbmi  r7, [r8, #-885]!
    33ec:   6f70646e    svcvs   0x0070646e
    33f0:   00746e69    rsbseq  r6, r4, r9, ror #28
    33f4:   6c03d20c    sfmvs   f5, 1, [r3], {12}
    33f8:   0300003b    movweq  r0, #59 ; 0x3b
    33fc:   0d04b023    stceq   0, cr11, [r4, #-140]
    3400:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
    3404:   6172676f    cmnvs   r2, pc, ror #14
    3408:   6c61486d    stclvs  8, cr4, [r1], #-436
    340c:   646e4574    strbtvs r4, [lr], #-1396
    3410:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    3414:   d30c0074    movwle  r0, #49268  ; 0xc074
    3418:   003bb403    eorseq  fp, fp, r3, lsl #8
    341c:   b4230300    strtlt  r0, [r3], #-768
    3420:   66700d04    ldrbtvs r0, [r0], -r4, lsl #26
    3424:   61666544    cmnvs   r6, r4, asr #10
    3428:   45746c75    ldrbmi  r6, [r4, #-3189]!
    342c:   6552646e    ldrbvs  r6, [r2, #-1134]
    3430:   6e6f7073    mcrvs   0, 3, r7, cr15, cr3, {3}
    3434:   0c006573    cfstr32eq   mvfx6, [r0], {115}
    3438:   3bd603d4    blcc    ff584390 <MGC_DrcDumpPipe+0xff58013c>
    343c:   23030000    movwcs  r0, #12288  ; 0x3000
    3440:   700d04b8    strhvc  r0, [sp], -r8
    3444:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    3448:   65636976    strbvs  r6, [r3, #-2422]!
    344c:   61666544    cmnvs   r6, r4, asr #10
    3450:   45746c75    ldrbmi  r6, [r4, #-3189]!
    3454:   0c00646e    cfstrseq    mvf6, [r0], {110}
    3458:   3c1203d5    ldccc   3, cr0, [r2], {213}
    345c:   23030000    movwcs  r0, #12288  ; 0x3000
    3460:   700d04bc    strhvc  r0, [sp], -ip
    3464:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    3468:   65636976    strbvs  r6, [r3, #-2422]!
    346c:   6e617254    mcrvs   2, 3, r7, cr1, cr4, {2}
    3470:   74696d73    strbtvc r6, [r9], #-3443
    3474:   69617641    stmdbvs r1!, {r0, r6, r9, sl, ip, sp, lr}^
    3478:   d60c006c    strle   r0, [ip], -ip, rrx
    347c:   003c5303    eorseq  r5, ip, r3, lsl #6
    3480:   c0230300    eorgt   r0, r3, r0, lsl #6
    3484:   66700d04    ldrbtvs r0, [r0], -r4, lsl #26
    3488:   76726553    undefined
    348c:   52656369    rsbpl   r6, r5, #-1543503871    ; 0xa4000001
    3490:   69656365    stmdbvs r5!, {r0, r2, r5, r6, r8, r9, sp, lr}^
    3494:   65526576    ldrbvs  r6, [r2, #-1398]
    3498:   00796461    rsbseq  r6, r9, r1, ror #8
    349c:   9603d70c    strls   sp, [r3], -ip, lsl #14
    34a0:   0300003c    movweq  r0, #60 ; 0x3c
    34a4:   0d04c423    cfstrseq    mvf12, [r4, #-140]
    34a8:   6f4c6670    svcvs   0x004c6670
    34ac:   69466461    stmdbvs r6, {r0, r5, r6, sl, sp, lr}^
    34b0:   0c006f66    stceq   15, cr6, [r0], {102}
    34b4:   3cb803d8    ldccc   3, cr0, [r8], #864
    34b8:   23030000    movwcs  r0, #12288  ; 0x3000
    34bc:   700d04c8    andvc   r0, sp, r8, asr #9
    34c0:   6c6e5566    cfstr64vs   mvdx5, [lr], #-408
    34c4:   4664616f    strbtmi r6, [r4], -pc, ror #2
    34c8:   006f6669    rsbeq   r6, pc, r9, ror #12
    34cc:   f403d90c    vst2.8  {d13,d15}, [r3], ip
    34d0:   0300003c    movweq  r0, #60 ; 0x3c
    34d4:   0d04cc23    stceq   12, cr12, [r4, #-140]
    34d8:   63416670    movtvs  r6, #5744   ; 0x1670
    34dc:   74706563    ldrbtvc r6, [r0], #-1379
    34e0:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    34e4:   0c006563    cfstr32eq   mvfx6, [r0], {99}
    34e8:   3d3203da    ldccc   3, cr0, [r2, #-872]!
    34ec:   23030000    movwcs  r0, #12288  ; 0x3000
    34f0:   700d04d0    ldrdvc  r0, [sp], -r0
    34f4:   616d4466    cmnvs   sp, r6, ror #8
    34f8:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    34fc:   536c656e    cmnpl   ip, #461373440  ; 0x1b800000
    3500:   75746174    ldrbvc  r6, [r4, #-372]!
    3504:   61684373    smcvs   33843
    3508:   6465676e    strbtvs r6, [r5], #-1902
    350c:   03db0c00    bicseq  r0, fp, #0  ; 0x0
    3510:   000026bd    strheq  r2, [r0], -sp
    3514:   04d42303    ldrbeq  r2, [r4], #771
    3518:   5366700d    cmnpl   r6, #13 ; 0xd
    351c:   6f507465    svcvs   0x00507465
    3520:   65547472    ldrbvs  r7, [r4, #-1138]
    3524:   6f4d7473    svcvs   0x004d7473
    3528:   0c006564    cfstr32eq   mvfx6, [r0], {100}
    352c:   3d6d03dc    stclcc  3, cr0, [sp, #-880]!
    3530:   23030000    movwcs  r0, #12288  ; 0x3000
    3534:   700d04d8    ldrdvc  r0, [sp], -r8
    3538:   6e794466    cdpvs   4, 7, cr4, cr9, cr6, {3}
    353c:   63696d61    cmnvs   r9, #6208   ; 0x1840
    3540:   6f666946    svcvs   0x00666946
    3544:   61636f4c    cmnvs   r3, ip, asr #30
    3548:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    354c:   03dd0c00    bicseq  r0, sp, #0  ; 0x0
    3550:   00003da2    andeq   r3, r0, r2, lsr #27
    3554:   04dc2303    ldrbeq  r2, [ip], #771
    3558:   aa040700    bge 105160 <MGC_DrcDumpPipe+0x100f0c>
    355c:   1800002c    stmdane r0, {r2, r3, r5}
    3560:   43474d5f    movtmi  r4, #32095  ; 0x7d5f
    3564:   6863535f    stmdavs r3!, {r0, r1, r2, r3, r4, r6, r8, r9, ip, lr}^
    3568:   6c756465    cfldrdvs    mvd6, [r5], #-404
    356c:   6f6c5365    svcvs   0x006c5365
    3570:   0c0c0074    stceq   0, cr0, [ip], {116}
    3574:   003589f2    ldrshteq    r8, [r5], -r2
    3578:   01881700    orreq   r1, r8, r0, lsl #14
            357a: R_ARM_ABS32   .debug_str
    357c:   7a0c0000    bvc 303584 <MGC_DrcDumpPipe+0x2ff330>
    3580:   00210001    eoreq   r0, r1, r1
    3584:   00230200    eoreq   r0, r3, r0, lsl #4
    3588:   5f040700    svcpl   0x00040700
    358c:   11000035    tstne   r0, r5, lsr r0
    3590:   5f43474d    svcpl   0x0043474d
    3594:   65706950    ldrbvs  r6, [r0, #-2384]!
    3598:   01630c00    cmneq   r3, r0, lsl #24
    359c:   00002be0    andeq   r2, r0, r0, ror #23
    35a0:   8b0c1416    blhi    308600 <MGC_DrcDumpPipe+0x3043ac>
    35a4:   0035f001    eorseq  pc, r5, r1
    35a8:   77640d00    strbvc  r0, [r4, -r0, lsl #26]!
    35ac:   61746f54    cmnvs   r4, r4, asr pc
    35b0:   6d69546c    cfstrdvs    mvd5, [r9, #-432]!
    35b4:   8c0c0065    stchi   0, cr0, [ip], {101}
    35b8:   00018e01    andeq   r8, r1, r1, lsl #28
    35bc:   00230200    eoreq   r0, r3, r0, lsl #4
    35c0:   6863530d    stmdavs r3!, {r0, r2, r3, r8, r9, ip, lr}^
    35c4:   6c756465    cfldrdvs    mvd6, [r5], #-404
    35c8:   6f6c5365    svcvs   0x006c5365
    35cc:   0c007374    stceq   3, cr7, [r0], {116}
    35d0:   2100018d    smlabbcs    r0, sp, r1, r0
    35d4:   23020000    movwcs  r0, #8192   ; 0x2000
    35d8:   53770d04    cmnpl   r7, #256    ; 0x100
    35dc:   43746f6c    cmnmi   r4, #432    ; 0x1b0
    35e0:   746e756f    strbtvc r7, [lr], #-1391
    35e4:   018e0c00    orreq   r0, lr, r0, lsl #24
    35e8:   0000016f    andeq   r0, r0, pc, ror #2
    35ec:   00102302    andseq  r2, r0, r2, lsl #6
    35f0:   43474d11    movtmi  r4, #32017  ; 0x7d11
    35f4:   6863535f    stmdavs r3!, {r0, r1, r2, r3, r4, r6, r8, r9, ip, lr}^
    35f8:   6c756465    cfldrdvs    mvd6, [r5], #-404
    35fc:   8f0c0065    svchi   0x000c0065
    3600:   0035a001    eorseq  sl, r5, r1
    3604:   0c5c1600    mrrceq  6, 0, r1, ip, cr0
    3608:   3698019b    undefined
    360c:   a9170000    ldmdbge r7, {}
            360f: R_ARM_ABS32   .debug_str
    3610:   0c000001    stceq   0, cr0, [r0], {1}
    3614:   1ec4019c    mcrne   1, 6, r0, cr4, cr12, {4}
    3618:   23020000    movwcs  r0, #8192   ; 0x2000
    361c:   53700d00    cmnpl   r0, #0  ; 0x0
    3620:   64656863    strbtvs r6, [r5], #-2147
    3624:   00656c75    rsbeq   r6, r5, r5, ror ip
    3628:   98019d0c    stmdals r1, {r2, r3, r8, sl, fp, ip, pc}
    362c:   02000036    andeq   r0, r0, #54 ; 0x36
    3630:   88170423    ldmdahi r7, {r0, r1, r5, sl}
            3633: R_ARM_ABS32   .debug_str
    3634:   0c000001    stceq   0, cr0, [r0], {1}
    3638:   2100019e    strbcs  r0, [r0, -lr]
    363c:   23020000    movwcs  r0, #8192   ; 0x2000
    3640:   65440d08    strbvs  r0, [r4, #-3336]
    3644:   65636976    strbvs  r6, [r3, #-2422]!
    3648:   019f0c00    orrseq  r0, pc, r0, lsl #24
    364c:   000014ef    andeq   r1, r0, pc, ror #9
    3650:   0d142302    ldceq   3, cr2, [r4, #-8]
    3654:   546d6277    strbtpl r6, [sp], #-631
    3658:   676f5478    undefined
    365c:   00656c67    rsbeq   r6, r5, r7, ror #24
    3660:   6f01a10c    svcvs   0x0001a10c
    3664:   02000001    andeq   r0, r0, #1  ; 0x1
    3668:   770d5423    strvc   r5, [sp, -r3, lsr #8]
    366c:   78526d62    ldmdavc r2, {r1, r5, r6, r8, sl, fp, sp, lr}^
    3670:   67676f54    undefined
    3674:   0c00656c    cfstr32eq   mvfx6, [r0], {108}
    3678:   016f01a2    smultbeq    pc, r2, r1
    367c:   23020000    movwcs  r0, #8192   ; 0x2000
    3680:   49620d56    stmdbmi r2!, {r1, r2, r4, r6, r8, sl, fp}^
    3684:   6c754d73    ldclvs  13, cr4, [r5], #-460
    3688:   74546974    ldrbvc  r6, [r4], #-2420
    368c:   01a30c00    undefined instruction 0x01a30c00
    3690:   00000160    andeq   r0, r0, r0, ror #2
    3694:   00582302    subseq  r2, r8, r2, lsl #6
    3698:   35f00407    ldrbcc  r0, [r0, #1031]!
    369c:   4d110000    ldcmi   0, cr0, [r1]
    36a0:   445f4347    ldrbmi  r4, [pc], #839  ; 36a8 <.debug_info+0x36a8>
    36a4:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    36a8:   a50c0065    strge   r0, [ip, #-101]
    36ac:   00360501    eorseq  r0, r6, r1, lsl #10
    36b0:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    36b4:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    36b8:   676f7250    undefined
    36bc:   536d6172    cmnpl   sp, #-2147483620    ; 0x8000001c
    36c0:   74726174    ldrbtvc r6, [r2], #-372
    36c4:   746e6f43    strbtvc r6, [lr], #-3907
    36c8:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    36cc:   0c007265    sfmeq   f7, 4, [r0], {101}
    36d0:   36d601b3    undefined
    36d4:   04070000    streq   r0, [r7]
    36d8:   000036dc    ldrdeq  r3, [r0], -ip
    36dc:   018e0113    orreq   r0, lr, r3, lsl r1
    36e0:   36ec0000    strbtcc r0, [ip], r0
    36e4:   ec100000    ldc 0, cr0, [r0], {0}
    36e8:   00000036    andeq   r0, r0, r6, lsr r0
    36ec:   36f20407    ldrbtcc r0, [r2], r7, lsl #8
    36f0:   5f180000    svcpl   0x00180000
    36f4:   5f43474d    svcpl   0x0043474d
    36f8:   746e6f43    strbtvc r6, [lr], #-3907
    36fc:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    3700:   3c007265    sfmcc   f7, 4, [r0], {101}
    3704:   38b9f00c    ldmcc   r9!, {r2, r3, ip, sp, lr, pc}
    3708:   700d0000    andvc   r0, sp, r0
    370c:   746e6f43    strbtvc r6, [lr], #-3907
    3710:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    3714:   64417265    strbvs  r7, [r1], #-613
    3718:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
    371c:   72734973    rsbsvc  r4, r3, #1884160    ; 0x1cc000
    3720:   02000c00    andeq   r0, r0, #0  ; 0x0
    3724:   000000f1    strdeq  r0, [r0], -r1
    3728:   0d002302    stceq   3, cr2, [r0, #-8]
    372c:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    3730:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    3734:   4172656c    cmnmi   r2, ip, ror #10
    3738:   65726464    ldrbvs  r6, [r2, #-1124]!
    373c:   73497373    movtvc  r7, #37747  ; 0x9373
    3740:   010c0074    tsteq   ip, r4, ror r0
    3744:   0000f102    andeq   pc, r0, r2, lsl #2
    3748:   04230200    strteq  r0, [r3], #-512
    374c:   7455700d    ldrbvc  r7, [r5], #-13
    3750:   00736c69    rsbseq  r6, r3, r9, ror #24
    3754:   f302020c    vhsub.u8    d0, d2, d12
    3758:   02000039    andeq   r0, r0, #57 ; 0x39
    375c:   700d0823    andvc   r0, sp, r3, lsr #16
    3760:   74737953    ldrbtvc r7, [r3], #-2387
    3764:   65536d65    ldrbvs  r6, [r3, #-3429]
    3768:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    376c:   0c007365    stceq   3, cr7, [r0], {101}
    3770:   39f90203    ldmibcc r9!, {r0, r1, r9}^
    3774:   23020000    movwcs  r0, #8192   ; 0x2000
    3778:   0104170c    tsteq   r4, ip, lsl #14
            377a: R_ARM_ABS32   .debug_str
    377c:   040c0000    streq   r0, [ip]
    3780:   0000f102    andeq   pc, r0, r2, lsl #2
    3784:   10230200    eorne   r0, r3, r0, lsl #4
    3788:   00017a17    andeq   r7, r1, r7, lsl sl
            3789: R_ARM_ABS32   .debug_str
    378c:   02050c00    andeq   r0, r5, #0  ; 0x0
    3790:   00003559    andeq   r3, r0, r9, asr r5
    3794:   0d142302    ldceq   3, cr2, [r4, #-8]
    3798:   616d4470    smcvs   54336
    379c:   746e6f43    strbtvc r6, [lr], #-3907
    37a0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    37a4:   61467265    cmpvs   r6, r5, ror #4
    37a8:   726f7463    rsbvc   r7, pc, #1660944384 ; 0x63000000
    37ac:   070c0079    smlsdxeq    ip, r9, r0, r0
    37b0:   0039ff02    eorseq  pc, r9, r2, lsl #30
    37b4:   18230200    stmdane r3!, {r9}
    37b8:   5066700d    rsbpl   r7, r6, sp
    37bc:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    37c0:   74536d61    ldrbvc  r6, [r3], #-3425
    37c4:   43747261    cmnmi   r4, #268435462  ; 0x10000006
    37c8:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    37cc:   656c6c6f    strbvs  r6, [ip, #-3183]!
    37d0:   0a0c0072    beq 3039a0 <MGC_DrcDumpPipe+0x2ff74c>
    37d4:   0036b102    eorseq  fp, r6, r2, lsl #2
    37d8:   1c230200    sfmne   f0, 4, [r3]
    37dc:   5066700d    rsbpl   r7, r6, sp
    37e0:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    37e4:   74536d61    ldrbvc  r6, [r3], #-3425
    37e8:   6f43706f    svcvs   0x0043706f
    37ec:   6f72746e    svcvs   0x0072746e
    37f0:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    37f4:   020b0c00    andeq   r0, fp, #0  ; 0x0
    37f8:   000038b9    strheq  r3, [r0], -r9
    37fc:   0d202302    stceq   3, cr2, [r0, #-8]!
    3800:   65446670    strbvs  r6, [r4, #-1648]
    3804:   6f727473    svcvs   0x00727473
    3808:   6e6f4379    mcrvs   3, 3, r4, cr15, cr9, {3}
    380c:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    3810:   0072656c    rsbseq  r6, r2, ip, ror #10
    3814:   dd020c0c    stcle   12, cr0, [r2, #-48]
    3818:   02000038    andeq   r0, r0, #56 ; 0x38
    381c:   700d2423    andvc   r2, sp, r3, lsr #8
    3820:   74655366    strbtvc r5, [r5], #-870
    3824:   746e6f43    strbtvc r6, [lr], #-3907
    3828:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    382c:   6f487265    svcvs   0x00487265
    3830:   6f507473    svcvs   0x00507473
    3834:   00726577    rsbseq  r6, r2, r7, ror r5
    3838:   fd020d0c    stc2    13, cr0, [r2, #-48]
    383c:   02000038    andeq   r0, r0, #56 ; 0x38
    3840:   700d2823    andvc   r2, sp, r3, lsr #16
    3844:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    3848:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    384c:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    3850:   5372656c    cmnpl   r2, #452984832  ; 0x1b000000
    3854:   65746174    ldrbvs  r6, [r4, #-372]!
    3858:   02100c00    andseq  r0, r0, #0  ; 0x0
    385c:   0000393d    andeq   r3, r0, sp, lsr r9
    3860:   0d2c2302    stceq   3, cr2, [ip, #-8]!
    3864:   75446670    strbvc  r6, [r4, #-1648]
    3868:   6950706d    ldmdbvs r0, {r0, r2, r3, r5, r6, ip, sp, lr}^
    386c:   74536570    ldrbvc  r6, [r3], #-1392
    3870:   00657461    rsbeq   r7, r5, r1, ror #8
    3874:   7502110c    strvc   r1, [r2, #-268]
    3878:   02000039    andeq   r0, r0, #57 ; 0x39
    387c:   700d3023    andvc   r3, sp, r3, lsr #32
    3880:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    3884:   636f4c70    cmnvs   pc, #28672  ; 0x7000
    3888:   6e456c61    cdpvs   12, 4, cr6, cr5, cr1, {3}
    388c:   61745364    cmnvs   r4, r4, ror #6
    3890:   0c006574    cfstr32eq   mvfx6, [r0], {116}
    3894:   39b20212    ldmibcc r2!, {r1, r4, r9}
    3898:   23020000    movwcs  r0, #8192   ; 0x2000
    389c:   43770d34    cmnmi   r7, #3328   ; 0xd00
    38a0:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    38a4:   656c6c6f    strbvs  r6, [ip, #-3183]!
    38a8:   70795472    rsbsvc  r5, r9, r2, ror r4
    38ac:   140c0065    strne   r0, [ip], #-101
    38b0:   00016f02    andeq   r6, r1, r2, lsl #30
    38b4:   38230200    stmdacc r3!, {r9}
    38b8:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    38bc:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    38c0:   676f7250    undefined
    38c4:   536d6172    cmnpl   sp, #-2147483620    ; 0x8000001c
    38c8:   43706f74    cmnmi   r0, #464    ; 0x1d0
    38cc:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    38d0:   656c6c6f    strbvs  r6, [ip, #-3183]!
    38d4:   bc0c0072    stclt   0, cr0, [ip], {114}
    38d8:   0036d601    eorseq  sp, r6, r1, lsl #12
    38dc:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    38e0:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    38e4:   74736544    ldrbtvc r6, [r3], #-1348
    38e8:   43796f72    cmnmi   r9, #456    ; 0x1c8
    38ec:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    38f0:   656c6c6f    strbvs  r6, [ip, #-3183]!
    38f4:   c40c0072    strgt   r0, [ip], #-114
    38f8:   0036d601    eorseq  sp, r6, r1, lsl #12
    38fc:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3900:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3904:   43746553    cmnmi   r4, #348127232  ; 0x14c00000
    3908:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    390c:   656c6c6f    strbvs  r6, [ip, #-3183]!
    3910:   736f4872    cmnvc   pc, #7471104    ; 0x720000
    3914:   776f5074    undefined
    3918:   0c007265    sfmeq   f7, 4, [r0], {101}
    391c:   392201cc    stmdbcc r2!, {r2, r3, r6, r7, r8}
    3920:   04070000    streq   r0, [r7]
    3924:   00003928    andeq   r3, r0, r8, lsr #18
    3928:   018e0113    orreq   r0, lr, r3, lsl r1
    392c:   393d0000    ldmdbcc sp!, {}
    3930:   ec100000    ldc 0, cr0, [r0], {0}
    3934:   10000036    andne   r0, r0, r6, lsr r0
    3938:   0000016f    andeq   r0, r0, pc, ror #2
    393c:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3940:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3944:   706d7544    rsbvc   r7, sp, r4, asr #10
    3948:   746e6f43    strbtvc r6, [lr], #-3907
    394c:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    3950:   74537265    ldrbvc  r7, [r3], #-613
    3954:   00657461    rsbeq   r7, r5, r1, ror #8
    3958:   5f01d40c    svcpl   0x0001d40c
    395c:   07000039    smladxeq    r0, r9, r0, r0
    3960:   00396504    eorseq  r6, r9, r4, lsl #10
    3964:   a9011300    stmdbge r1, {r8, r9, ip}
    3968:   75000000    strvc   r0, [r0]
    396c:   10000039    andne   r0, r0, r9, lsr r0
    3970:   000036ec    andeq   r3, r0, ip, ror #13
    3974:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3978:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    397c:   706d7544    rsbvc   r7, sp, r4, asr #10
    3980:   65706950    ldrbvs  r6, [r0, #-2384]!
    3984:   74617453    strbtvc r7, [r1], #-1107
    3988:   dd0c0065    stcle   0, cr0, [ip, #-404]
    398c:   00399101    eorseq  r9, r9, r1, lsl #2
    3990:   97040700    strls   r0, [r4, -r0, lsl #14]
    3994:   13000039    movwne  r0, #57 ; 0x39
    3998:   0000a901    andeq   sl, r0, r1, lsl #18
    399c:   0039ac00    eorseq  sl, r9, r0, lsl #24
    39a0:   36ec1000    strbtcc r1, [ip], r0
    39a4:   ac100000    ldcge   0, cr0, [r0], {0}
    39a8:   00000039    andeq   r0, r0, r9, lsr r0
    39ac:   358f0407    strcc   r0, [pc, #1031] ; 3dbb <.debug_info+0x3dbb>
    39b0:   4d110000    ldcmi   0, cr0, [r1]
    39b4:   705f4347    subsvc  r4, pc, r7, asr #6
    39b8:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    39bc:   636f4c70    cmnvs   pc, #28672  ; 0x7000
    39c0:   6e456c61    cdpvs   12, 4, cr6, cr5, cr1, {3}
    39c4:   61745364    cmnvs   r4, r4, ror #6
    39c8:   0c006574    cfstr32eq   mvfx6, [r0], {116}
    39cc:   39d201e7    ldmibcc r2, {r0, r1, r2, r5, r6, r7, r8}^
    39d0:   04070000    streq   r0, [r7]
    39d4:   000039d8    ldrdeq  r3, [r0], -r8
    39d8:   00a90113    adceq   r0, r9, r3, lsl r1
    39dc:   39ed0000    stmibcc sp!, {}^
    39e0:   ec100000    ldc 0, cr0, [r0], {0}
    39e4:   10000036    andne   r0, r0, r6, lsr r0
    39e8:   000039ed    andeq   r3, r0, sp, ror #19
    39ec:   cc040700    stcgt   7, cr0, [r4], {0}
    39f0:   07000015    smladeq r0, r5, r0, r0
    39f4:   00093a04    andeq   r3, r9, r4, lsl #20
    39f8:   cc040700    stcgt   7, cr0, [r4], {0}
    39fc:   0700000c    streq   r0, [r0, -ip]
    3a00:   0027f904    eoreq   pc, r7, r4, lsl #18
    3a04:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3a08:   6f435f43    svcvs   0x00435f43
    3a0c:   6f72746e    svcvs   0x0072746e
    3a10:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    3a14:   02150c00    andseq  r0, r5, #0  ; 0x0
    3a18:   000036f2    strdeq  r3, [r0], -r2
    3a1c:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3a20:   5266705f    rsbpl   r7, r6, #95 ; 0x5f
    3a24:   42646165    rsbmi   r6, r4, #1073741849 ; 0x40000019
    3a28:   74537375    ldrbvc  r7, [r3], #-885
    3a2c:   00657461    rsbeq   r7, r5, r1, ror #8
    3a30:   3702230c    strcc   r2, [r2, -ip, lsl #6]
    3a34:   0700003a    smladxeq    r0, sl, r0, r0
    3a38:   003a3d04    eorseq  r3, sl, r4, lsl #26
    3a3c:   8e011300    cdphi   3, 0, cr1, cr1, cr0, {0}
    3a40:   4d000001    stcmi   0, cr0, [r0, #-4]
    3a44:   1000003a    andne   r0, r0, sl, lsr r0
    3a48:   00003559    andeq   r3, r0, r9, asr r5
    3a4c:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3a50:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3a54:   676f7250    undefined
    3a58:   426d6172    rsbmi   r6, sp, #-2147483620    ; 0x8000001c
    3a5c:   74537375    ldrbvc  r7, [r3], #-885
    3a60:   00657461    rsbeq   r7, r5, r1, ror #8
    3a64:   37022c0c    strcc   r2, [r2, -ip, lsl #24]
    3a68:   1100003a    tstne   r0, sl, lsr r0
    3a6c:   5f43474d    svcpl   0x0043474d
    3a70:   65526670    ldrbvs  r6, [r2, #-1648]
    3a74:   50746573    rsbspl  r6, r4, r3, ror r5
    3a78:   0074726f    rsbseq  r7, r4, pc, ror #4
    3a7c:   3702350c    strcc   r3, [r2, -ip, lsl #10]
    3a80:   1100003a    tstne   r0, sl, lsr r0
    3a84:   5f43474d    svcpl   0x0043474d
    3a88:   69426670    stmdbvs r2, {r4, r5, r6, r9, sl, sp, lr}^
    3a8c:   6e45646e    cdpvs   4, 4, cr6, cr5, cr14, {3}
    3a90:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    3a94:   0c00746e    cfstrseq    mvf7, [r0], {110}
    3a98:   3a9e0245    bcc fe7843b4 <MGC_DrcDumpPipe+0xfe780160>
    3a9c:   04070000    streq   r0, [r7]
    3aa0:   00003aa4    andeq   r3, r0, r4, lsr #21
    3aa4:   2ca40113    stfcss  f0, [r4], #76
    3aa8:   3ac30000    bcc ff0c3ab0 <MGC_DrcDumpPipe+0xff0bf85c>
    3aac:   59100000    ldmdbpl r0, {}
    3ab0:   10000035    andne   r0, r0, r5, lsr r0
    3ab4:   00003ac3    andeq   r3, r0, r3, asr #21
    3ab8:   0039ed10    eorseq  lr, r9, r0, lsl sp
    3abc:   01601000    cmneq   r0, r0
    3ac0:   07000000    streq   r0, [r0, -r0]
    3ac4:   003ac904    eorseq  ip, sl, r4, lsl #18
    3ac8:   156c0800    strbne  r0, [ip, #-2048]!
    3acc:   4d110000    ldcmi   0, cr0, [r1]
    3ad0:   705f4347    subsvc  r4, pc, r7, asr #6
    3ad4:   6f725066    svcvs   0x00725066
    3ad8:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    3adc:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
    3ae0:   63655274    cmnvs   r5, #1073741831 ; 0x40000007
    3ae4:   65766965    ldrbvs  r6, [r6, #-2405]!
    3ae8:   02580c00    subseq  r0, r8, #0  ; 0x0
    3aec:   00003af0    strdeq  r3, [r0], -r0
    3af0:   3af60407    bcc ffd84b14 <MGC_DrcDumpPipe+0xffd808c0>
    3af4:   01130000    tsteq   r3, r0
    3af8:   0000018e    andeq   r0, r0, lr, lsl #3
    3afc:   00003b1f    andeq   r3, r0, pc, lsl fp
    3b00:   00355910    eorseq  r5, r5, r0, lsl r9
    3b04:   2ca41000    stccs   0, cr1, [r4]
    3b08:   e3100000    tst r0, #0  ; 0x0
    3b0c:   10000014    andne   r0, r0, r4, lsl r0
    3b10:   0000018e    andeq   r0, r0, lr, lsl #3
    3b14:   0000f110    andeq   pc, r0, r0, lsl r1
    3b18:   01601000    cmneq   r0, r0
    3b1c:   11000000    tstne   r0, r0
    3b20:   5f43474d    svcpl   0x0043474d
    3b24:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
    3b28:   6172676f    cmnvs   r2, pc, ror #14
    3b2c:   6174536d    cmnvs   r4, sp, ror #6
    3b30:   72547472    subsvc  r7, r4, #1912602624 ; 0x72000000
    3b34:   6d736e61    ldclvs  14, cr6, [r3, #-388]!
    3b38:   0c007469    cfstrseq    mvf7, [r0], {105}
    3b3c:   3b420266    blcc    10844dc <MGC_DrcDumpPipe+0x1080288>
    3b40:   04070000    streq   r0, [r7]
    3b44:   00003b48    andeq   r3, r0, r8, asr #22
    3b48:   018e0113    orreq   r0, lr, r3, lsl r1
    3b4c:   3b6c0000    blcc    1b03b54 <MGC_DrcDumpPipe+0x1aff900>
    3b50:   59100000    ldmdbpl r0, {}
    3b54:   10000035    andne   r0, r0, r5, lsr r0
    3b58:   00002ca4    andeq   r2, r0, r4, lsr #25
    3b5c:   000b5110    andeq   r5, fp, r0, lsl r1
    3b60:   018e1000    orreq   r1, lr, r0
    3b64:   f1100000    undefined instruction 0xf1100000
    3b68:   00000000    andeq   r0, r0, r0
    3b6c:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3b70:   5066705f    rsbpl   r7, r6, pc, asr r0
    3b74:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    3b78:   6c466d61    mcrrvs  13, 6, r6, r6, cr1
    3b7c:   45687375    strbmi  r7, [r8, #-885]!
    3b80:   6f70646e    svcvs   0x0070646e
    3b84:   00746e69    rsbseq  r6, r4, r9, ror #28
    3b88:   8f02750c    svchi   0x0002750c
    3b8c:   0700003b    smladxeq    r0, fp, r0, r0
    3b90:   003b9504    eorseq  r9, fp, r4, lsl #10
    3b94:   8e011300    cdphi   3, 0, cr1, cr1, cr0, {0}
    3b98:   b4000001    strlt   r0, [r0], #-1
    3b9c:   1000003b    andne   r0, r0, fp, lsr r0
    3ba0:   00003559    andeq   r3, r0, r9, asr r5
    3ba4:   002ca410    eoreq   sl, ip, r0, lsl r4
    3ba8:   01601000    cmneq   r0, r0
    3bac:   60100000    andsvs  r0, r0, r0
    3bb0:   00000001    andeq   r0, r0, r1
    3bb4:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3bb8:   5066705f    rsbpl   r7, r6, pc, asr r0
    3bbc:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    3bc0:   61486d61    cmpvs   r8, r1, ror #26
    3bc4:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
    3bc8:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    3bcc:   0c00746e    cfstrseq    mvf7, [r0], {110}
    3bd0:   3b8f0286    blcc    fe3c45f0 <MGC_DrcDumpPipe+0xfe3c039c>
    3bd4:   4d110000    ldcmi   0, cr0, [r1]
    3bd8:   705f4347    subsvc  r4, pc, r7, asr #6
    3bdc:   66654466    strbtvs r4, [r5], -r6, ror #8
    3be0:   746c7561    strbtvc r7, [ip], #-1377
    3be4:   52646e45    rsbpl   r6, r4, #1104   ; 0x450
    3be8:   6f707365    svcvs   0x00707365
    3bec:   0065736e    rsbeq   r7, r5, lr, ror #6
    3bf0:   f702900c    undefined instruction 0xf702900c
    3bf4:   0700003b    smladxeq    r0, fp, r0, r0
    3bf8:   003bfd04    eorseq  pc, fp, r4, lsl #26
    3bfc:   8e011300    cdphi   3, 0, cr1, cr1, cr0, {0}
    3c00:   12000001    andne   r0, r0, #1  ; 0x1
    3c04:   1000003c    andne   r0, r0, ip, lsr r0
    3c08:   00003559    andeq   r3, r0, r9, asr r5
    3c0c:   00016010    andeq   r6, r1, r0, lsl r0
    3c10:   4d110000    ldcmi   0, cr0, [r1]
    3c14:   705f4347    subsvc  r4, pc, r7, asr #6
    3c18:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    3c1c:   65636976    strbvs  r6, [r3, #-2422]!
    3c20:   61666544    cmnvs   r6, r4, asr #10
    3c24:   45746c75    ldrbmi  r6, [r4, #-3189]!
    3c28:   0c00646e    cfstrseq    mvf6, [r0], {110}
    3c2c:   3c320299    lfmcc   f0, 4, [r2], #-612
    3c30:   04070000    streq   r0, [r7]
    3c34:   00003c38    andeq   r3, r0, r8, lsr ip
    3c38:   01600113    cmneq   r0, r3, lsl r1
    3c3c:   3c4d0000    marcc   acc0, r0, sp
    3c40:   59100000    ldmdbpl r0, {}
    3c44:   10000035    andne   r0, r0, r5, lsr r0
    3c48:   00003c4d    andeq   r3, r0, sp, asr #24
    3c4c:   61040700    tstvs   r4, r0, lsl #14
    3c50:   11000028    tstne   r0, r8, lsr #32
    3c54:   5f43474d    svcpl   0x0043474d
    3c58:   65536670    ldrbvs  r6, [r3, #-1648]
    3c5c:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    3c60:   61725465    cmnvs   r2, r5, ror #8
    3c64:   696d736e    stmdbvs sp!, {r1, r2, r3, r5, r6, r8, r9, ip, sp, lr}^
    3c68:   61764174    cmnvs   r6, r4, ror r1
    3c6c:   0c006c69    stceq   12, cr6, [r0], {105}
    3c70:   3c7602a3    lfmcc   f0, 2, [r6], #-652
    3c74:   04070000    streq   r0, [r7]
    3c78:   00003c7c    andeq   r3, r0, ip, ror ip
    3c7c:   01600113    cmneq   r0, r3, lsl r1
    3c80:   3c960000    ldccc   0, cr0, [r6], {0}
    3c84:   59100000    ldmdbpl r0, {}
    3c88:   10000035    andne   r0, r0, r5, lsr r0
    3c8c:   0000016f    andeq   r0, r0, pc, ror #2
    3c90:   003c4d10    eorseq  r4, ip, r0, lsl sp
    3c94:   4d110000    ldcmi   0, cr0, [r1]
    3c98:   705f4347    subsvc  r4, pc, r7, asr #6
    3c9c:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    3ca0:   65636976    strbvs  r6, [r3, #-2422]!
    3ca4:   65636552    strbvs  r6, [r3, #-1362]!
    3ca8:   52657669    rsbpl   r7, r5, #110100480  ; 0x6900000
    3cac:   79646165    stmdbvc r4!, {r0, r2, r5, r6, r8, sp, lr}^
    3cb0:   02ad0c00    adceq   r0, sp, #0  ; 0x0
    3cb4:   00003c76    andeq   r3, r0, r6, ror ip
    3cb8:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3cbc:   4c66705f    stclmi  0, cr7, [r6], #-380
    3cc0:   4664616f    strbtmi r6, [r4], -pc, ror #2
    3cc4:   006f6669    rsbeq   r6, pc, r9, ror #12
    3cc8:   cf02b70c    svcgt   0x0002b70c
    3ccc:   0700003c    smladxeq    r0, ip, r0, r0
    3cd0:   003cd504    eorseq  sp, ip, r4, lsl #10
    3cd4:   60011300    andvs   r1, r1, r0, lsl #6
    3cd8:   f4000001    vst4.8  {d0-d3}, [r0], r1
    3cdc:   1000003c    andne   r0, r0, ip, lsr r0
    3ce0:   00003559    andeq   r3, r0, r9, asr r5
    3ce4:   00016010    andeq   r6, r1, r0, lsl r0
    3ce8:   018e1000    orreq   r1, lr, r0
    3cec:   51100000    tstpl   r0, r0
    3cf0:   0000000b    andeq   r0, r0, fp
    3cf4:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3cf8:   5566705f    strbpl  r7, [r6, #-95]!
    3cfc:   616f6c6e    cmnvs   pc, lr, ror #24
    3d00:   66694664    strbtvs r4, [r9], -r4, ror #12
    3d04:   c10c006f    tstgt   ip, pc, rrx
    3d08:   003d0d02    eorseq  r0, sp, r2, lsl #26
    3d0c:   13040700    movwne  r0, #18176  ; 0x4700
    3d10:   1300003d    movwne  r0, #61 ; 0x3d
    3d14:   00016001    andeq   r6, r1, r1
    3d18:   003d3200    eorseq  r3, sp, r0, lsl #4
    3d1c:   35591000    ldrbcc  r1, [r9]
    3d20:   60100000    andsvs  r0, r0, r0
    3d24:   10000001    andne   r0, r0, r1
    3d28:   0000018e    andeq   r0, r0, lr, lsl #3
    3d2c:   0014e310    andseq  lr, r4, r0, lsl r3
    3d30:   4d110000    ldcmi   0, cr0, [r1]
    3d34:   705f4347    subsvc  r4, pc, r7, asr #6
    3d38:   63634166    cmnvs   r3, #-2147483623    ; 0x80000019
    3d3c:   44747065    ldrbtmi r7, [r4], #-101
    3d40:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    3d44:   ce0c0065    cdpgt   0, 0, cr0, cr12, cr5, {3}
    3d48:   003d4d02    eorseq  r4, sp, r2, lsl #26
    3d4c:   53040700    movwpl  r0, #18176  ; 0x4700
    3d50:   1300003d    movwne  r0, #61 ; 0x3d
    3d54:   00016001    andeq   r6, r1, r1
    3d58:   003d6d00    eorseq  r6, sp, r0, lsl #26
    3d5c:   35591000    ldrbcc  r1, [r9]
    3d60:   a0100000    andsge  r0, r0, r0
    3d64:   1000001c    andne   r0, r0, ip, lsl r0
    3d68:   00001ec4    andeq   r1, r0, r4, asr #29
    3d6c:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3d70:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3d74:   50746553    rsbspl  r6, r4, r3, asr r5
    3d78:   5474726f    ldrbtpl r7, [r4], #-623
    3d7c:   4d747365    ldclmi  3, cr7, [r4, #-404]!
    3d80:   0065646f    rsbeq   r6, r5, pc, ror #8
    3d84:   8b02d50c    blhi    b91bc <MGC_DrcDumpPipe+0xb4f68>
    3d88:   0700003d    smladxeq    r0, sp, r0, r0
    3d8c:   003d9104    eorseq  r9, sp, r4, lsl #2
    3d90:   a2010f00    andge   r0, r1, #0  ; 0x0
    3d94:   1000003d    andne   r0, r0, sp, lsr r0
    3d98:   00003559    andeq   r3, r0, r9, asr r5
    3d9c:   00223e10    eoreq   r3, r2, r0, lsl lr
    3da0:   4d110000    ldcmi   0, cr0, [r1]
    3da4:   705f4347    subsvc  r4, pc, r7, asr #6
    3da8:   6e794466    cdpvs   4, 7, cr4, cr9, cr6, {3}
    3dac:   63696d61    cmnvs   r9, #6208   ; 0x1840
    3db0:   6f666946    svcvs   0x00666946
    3db4:   61636f4c    cmnvs   r3, ip, asr #30
    3db8:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    3dbc:   02e10c00    rsceq   r0, r1, #0  ; 0x0
    3dc0:   00003dc4    andeq   r3, r0, r4, asr #27
    3dc4:   3dca0407    cfstrdcc    mvd0, [sl, #28]
    3dc8:   010f0000    mrseq   r0, CPSR
    3dcc:   00003df4    strdeq  r3, [r0], -r4
    3dd0:   00355910    eorseq  r5, r5, r0, lsl r9
    3dd4:   01601000    cmneq   r0, r0
    3dd8:   60100000    andsvs  r0, r0, r0
    3ddc:   10000001    andne   r0, r0, r1
    3de0:   00000160    andeq   r0, r0, r0, ror #2
    3de4:   0019a010    andseq  sl, r9, r0, lsl r0
    3de8:   14e31000    strbtne r1, [r3]
    3dec:   e3100000    tst r0, #0  ; 0x0
    3df0:   00000014    andeq   r0, r0, r4, lsl r0
    3df4:   0c01581b    stceq   8, cr5, [r1], {27}
    3df8:   3f1b0310    svccc   0x001b0310
    3dfc:   700d0000    andvc   r0, sp, r0
    3e00:   756e4566    strbvc  r4, [lr, #-1382]!
    3e04:   6172656d    cmnvs   r2, sp, ror #10
    3e08:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    3e0c:   706d6f43    rsbvc   r6, sp, r3, asr #30
    3e10:   6574656c    ldrbvs  r6, [r4, #-1388]!
    3e14:   03110c00    tsteq   r1, #0  ; 0x0
    3e18:   00001fc5    andeq   r1, r0, r5, asr #31
    3e1c:   17002302    strne   r2, [r0, -r2, lsl #6]
    3e20:   00000180    andeq   r0, r0, r0, lsl #3
            3e20: R_ARM_ABS32   .debug_str
    3e24:   a003120c    andge   r1, r3, ip, lsl #4
    3e28:   0200001c    andeq   r0, r0, #28 ; 0x1c
    3e2c:   610d0423    tstvs   sp, r3, lsr #8
    3e30:   75746553    ldrbvc  r6, [r4, #-1363]!
    3e34:   00785470    rsbseq  r5, r8, r0, ror r4
    3e38:   1b03130c    blne    c8a70 <MGC_DrcDumpPipe+0xc481c>
    3e3c:   0200003f    andeq   r0, r0, #63 ; 0x3f
    3e40:   610d0823    tstvs   sp, r3, lsr #16
    3e44:   75746553    ldrbvc  r6, [r4, #-1363]!
    3e48:   00785270    rsbseq  r5, r8, r0, ror r2
    3e4c:   2b03140c    blcs    c8e84 <MGC_DrcDumpPipe+0xc4c30>
    3e50:   0200003f    andeq   r0, r0, #63 ; 0x3f
    3e54:   610d1023    tstvs   sp, r3, lsr #32
    3e58:   64416d62    strbvs  r6, [r1], #-3426
    3e5c:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
    3e60:   150c0073    strne   r0, [ip, #-115]
    3e64:   003f3b03    eorseq  r3, pc, r3, lsl #22
    3e68:   90230300    eorls   r0, r3, r0, lsl #6
    3e6c:   72490d02    subvc   r0, r9, #128    ; 0x80
    3e70:   160c0070    undefined
    3e74:   00173203    andseq  r3, r7, r3, lsl #4
    3e78:   a0230300    eorge   r0, r3, r0, lsl #6
    3e7c:   77640d02    strbvc  r0, [r4, -r2, lsl #26]!
    3e80:   61746144    cmnvs   r4, r4, asr #2
    3e84:   03170c00    tsteq   r7, #0  ; 0x0
    3e88:   0000018e    andeq   r0, r0, lr, lsl #3
    3e8c:   02cc2303    sbceq   r2, ip, #201326592  ; 0xc000000
    3e90:   6144770d    cmpvs   r4, sp, lsl #14
    3e94:   0c006174    stfeqs  f6, [r0], {116}
    3e98:   016f0318    cmneq   pc, r8, lsl r3
    3e9c:   23030000    movwcs  r0, #12288  ; 0x3000
    3ea0:   d51702d0    ldrle   r0, [r7, #-720]
            3ea3: R_ARM_ABS32   .debug_str
    3ea4:   0c000000    stceq   0, cr0, [r0], {0}
    3ea8:   01600319    cmneq   r0, r9, lsl r3
    3eac:   23030000    movwcs  r0, #12288  ; 0x3000
    3eb0:   620d02d2    andvs   r0, sp, #536870925  ; 0x2000000d
    3eb4:   74617453    strbtvc r7, [r1], #-1107
    3eb8:   1a0c0065    bne 304054 <MGC_DrcDumpPipe+0x2ffe00>
    3ebc:   00016003    andeq   r6, r1, r3
    3ec0:   d3230300    teqle   r3, #0  ; 0x0
    3ec4:   49620d02    stmdbmi r2!, {r1, r8, sl, fp}^
    3ec8:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
    3ecc:   031b0c00    tsteq   fp, #0  ; 0x0
    3ed0:   00000160    andeq   r0, r0, r0, ror #2
    3ed4:   02d42303    sbcseq  r2, r4, #201326592  ; 0xc000000
    3ed8:   6f43620d    svcvs   0x0043620d
    3edc:   00746e75    rsbseq  r6, r4, r5, ror lr
    3ee0:   60031c0c    andvs   r1, r3, ip, lsl #24
    3ee4:   03000001    movweq  r0, #1  ; 0x1
    3ee8:   0d02d523    cfstr32eq   mvfx13, [r2, #-140]
    3eec:   74655262    strbtvc r5, [r5], #-610
    3ef0:   73656972    cmnvc   r5, #1867776    ; 0x1c8000
    3ef4:   031d0c00    tsteq   sp, #0  ; 0x0
    3ef8:   00000160    andeq   r0, r0, r0, ror #2
    3efc:   02d62303    sbcseq  r2, r6, #201326592  ; 0xc000000
    3f00:   6146620d    cmpvs   r6, sp, lsl #4
    3f04:   526c6174    rsbpl   r6, ip, #29 ; 0x1d
    3f08:   69727465    ldmdbvs r2!, {r0, r2, r5, r6, sl, ip, sp, lr}^
    3f0c:   0c007365    stceq   3, cr7, [r0], {101}
    3f10:   0160031e    cmneq   r0, lr, lsl r3
    3f14:   23030000    movwcs  r0, #12288  ; 0x3000
    3f18:   1c0002d7    sfmne   f0, 4, [r0], {215}
    3f1c:   00000160    andeq   r0, r0, r0, ror #2
    3f20:   00003f2b    andeq   r3, r0, fp, lsr #30
    3f24:   0000dd1d    andeq   sp, r0, sp, lsl sp
    3f28:   1c000700    stcne   7, cr0, [r0], {0}
    3f2c:   00000160    andeq   r0, r0, r0, ror #2
    3f30:   00003f3b    andeq   r3, r0, fp, lsr pc
    3f34:   0000dd1d    andeq   sp, r0, sp, lsl sp
    3f38:   1c00ff00    stcne   15, cr15, [r0], {0}
    3f3c:   00000160    andeq   r0, r0, r0, ror #2
    3f40:   00003f4b    andeq   r3, r0, fp, asr #30
    3f44:   0000dd1d    andeq   sp, r0, sp, lsl sp
    3f48:   11000f00    tstne   r0, r0, lsl #30
    3f4c:   5f43474d    svcpl   0x0043474d
    3f50:   6d756e45    ldclvs  14, cr6, [r5, #-276]!
    3f54:   74617265    strbtvc r7, [r1], #-613
    3f58:   446e6f69    strbtmi r6, [lr], #-3945
    3f5c:   00617461    rsbeq   r7, r1, r1, ror #8
    3f60:   f4031f0c    undefined instruction 0xf4031f0c
    3f64:   0700003d    smladxeq    r0, sp, r0, r0
    3f68:   003a0504    eorseq  r0, sl, r4, lsl #10
    3f6c:   ae040700    cdpge   7, 0, cr0, cr4, cr0, {0}
    3f70:   0700001f    smladeq r0, pc, r0, r0
    3f74:   001eca04    andseq  ip, lr, r4, lsl #20
    3f78:   40040700    andmi   r0, r4, r0, lsl #14
    3f7c:   0700001c    smladeq r0, ip, r0, r0
    3f80:   003f8504    eorseq  r8, pc, r4, lsl #10
    3f84:   8b040700    blhi    105b8c <MGC_DrcDumpPipe+0x101938>
    3f88:   0800003f    stmdaeq r0, {r0, r1, r2, r3, r4, r5}
    3f8c:   00000ff1    strdeq  r0, [r0], -r1
    3f90:   39ac0407    stmibcc ip!, {r0, r1, r2, sl}
    3f94:   4d110000    ldcmi   0, cr0, [r1]
    3f98:   505f4347    subspl  r4, pc, r7, asr #6
    3f9c:   0074726f    rsbseq  r7, r4, pc, ror #4
    3fa0:   aa03de0c    bge fb7d8 <MGC_DrcDumpPipe+0xf7584>
    3fa4:   1100002c    tstne   r0, ip, lsr #32
    3fa8:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    3fac:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    3fb0:   6d65535f    stclvs  3, cr5, [r5, #-380]!
    3fb4:   6f687061    svcvs   0x00687061
    3fb8:   00546572    subseq  r6, r4, r2, ror r5
    3fbc:   8e03c50d    cfsh32hi    mvfx12, mvfx3, #13
    3fc0:   11000001    tstne   r0, r1
    3fc4:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    3fc8:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    3fcc:   69614d5f    stmdbvs r1!, {r0, r1, r2, r3, r4, r6, r8, sl, fp, lr}^
    3fd0:   6575716c    ldrbvs  r7, [r5, #-364]!
    3fd4:   00546575    subseq  r6, r4, r5, ror r5
    3fd8:   f103cf0d    undefined instruction 0xf103cf0d
    3fdc:   16000000    strne   r0, [r0], -r0
    3fe0:   04050d58    streq   r0, [r5], #-3416
    3fe4:   00004249    andeq   r4, r0, r9, asr #4
    3fe8:   636f6c0d    cmnvs   pc, #3328   ; 0xd00
    3fec:   6863536b    stmdavs r3!, {r0, r1, r3, r5, r6, r8, r9, ip, lr}^
    3ff0:   6c756465    cfldrdvs    mvd6, [r5], #-404
    3ff4:   75467265    strbvc  r7, [r6, #-613]
    3ff8:   0d00636e    stceq   3, cr6, [r0, #-440]
    3ffc:   00f50414    rscseq  r0, r5, r4, lsl r4
    4000:   23020000    movwcs  r0, #8192   ; 0x2000
    4004:   6e750d00    cdpvs   13, 7, cr0, cr5, cr0, {0}
    4008:   6b636f6c    blvs    18dfdc0 <MGC_DrcDumpPipe+0x18dbb6c>
    400c:   65686353    strbvs  r6, [r8, #-851]!
    4010:   656c7564    strbvs  r7, [ip, #-1380]!
    4014:   6e754672    mrcvs   6, 3, r4, cr5, cr2, {3}
    4018:   240d0063    strcs   r0, [sp], #-99
    401c:   0000f504    andeq   pc, r0, r4, lsl #10
    4020:   04230200    strteq  r0, [r3], #-512
    4024:   656c730d    strbvs  r7, [ip, #-781]!
    4028:   75467065    strbvc  r7, [r6, #-101]
    402c:   0d00636e    stceq   3, cr6, [r0, #-440]
    4030:   033b0431    teqeq   fp, #822083584  ; 0x31000000
    4034:   23020000    movwcs  r0, #8192   ; 0x2000
    4038:   69640d08    stmdbvs r4!, {r3, r8, sl, fp}^
    403c:   6c626173    stfvse  f6, [r2], #-460
    4040:   71724965    cmnvc   r2, r5, ror #18
    4044:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    4048:   04440d00    strbeq  r0, [r4], #-3328
    404c:   00000329    andeq   r0, r0, r9, lsr #6
    4050:   0d0c2302    stceq   3, cr2, [ip, #-8]
    4054:   62616e65    rsbvs   r6, r1, #1616   ; 0x650
    4058:   7249656c    subvc   r6, r9, #452984832  ; 0x1b000000
    405c:   6e754671    mrcvs   6, 3, r4, cr5, cr1, {3}
    4060:   550d0063    strpl   r0, [sp, #-99]
    4064:   00033b04    andeq   r3, r3, r4, lsl #22
    4068:   10230200    eorne   r0, r3, r0, lsl #4
    406c:   6572630d    ldrbvs  r6, [r2, #-781]!
    4070:   53657461    cmnpl   r5, #1627389952 ; 0x61000000
    4074:   70616d65    rsbvc   r6, r1, r5, ror #26
    4078:   65726f68    ldrbvs  r6, [r2, #-3944]!
    407c:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    4080:   046d0d00    strbteq r0, [sp], #-3328
    4084:   00004259    andeq   r4, r0, r9, asr r2
    4088:   0d142302    ldceq   3, cr2, [r4, #-8]
    408c:   74736564    ldrbtvc r6, [r3], #-1380
    4090:   53796f72    cmnpl   r9, #456    ; 0x1c8
    4094:   70616d65    rsbvc   r6, r1, r5, ror #26
    4098:   65726f68    ldrbvs  r6, [r2, #-3944]!
    409c:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    40a0:   04820d00    streq   r0, [r2], #3328
    40a4:   0000426b    andeq   r4, r0, fp, ror #4
    40a8:   0d182302    ldceq   3, cr2, [r8, #-8]
    40ac:   53746567    cmnpl   r4, #432013312  ; 0x19c00000
    40b0:   70616d65    rsbvc   r6, r1, r5, ror #26
    40b4:   65726f68    ldrbvs  r6, [r2, #-3944]!
    40b8:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    40bc:   04aa0d00    strteq  r0, [sl], #3328
    40c0:   00004286    andeq   r4, r0, r6, lsl #5
    40c4:   0d1c2302    ldceq   3, cr2, [ip, #-8]
    40c8:   53746573    cmnpl   r4, #482344960  ; 0x1cc00000
    40cc:   70616d65    rsbvc   r6, r1, r5, ror #26
    40d0:   65726f68    ldrbvs  r6, [r2, #-3944]!
    40d4:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    40d8:   04c70d00    strbeq  r0, [r7], #3328
    40dc:   0000429d    muleq   r0, sp, r2
    40e0:   0d202302    stceq   3, cr2, [r0, #-8]!
    40e4:   61657263    cmnvs   r5, r3, ror #4
    40e8:   614d6574    cmpvs   sp, r4, ror r5
    40ec:   75716c69    ldrbvc  r6, [r1, #-3177]!
    40f0:   46657565    strbtmi r7, [r5], -r5, ror #10
    40f4:   00636e75    rsbeq   r6, r3, r5, ror lr
    40f8:   b804e10d    stmdalt r4, {r0, r2, r3, r8, sp, lr, pc}
    40fc:   02000042    andeq   r0, r0, #66 ; 0x42
    4100:   640d2423    strvs   r2, [sp], #-1059
    4104:   72747365    rsbsvc  r7, r4, #-1811939327    ; 0x94000001
    4108:   614d796f    cmpvs   sp, pc, ror #18
    410c:   75716c69    ldrbvc  r6, [r1, #-3177]!
    4110:   46657565    strbtmi r7, [r5], -r5, ror #10
    4114:   00636e75    rsbeq   r6, r3, r5, ror lr
    4118:   ca04f60d    bgt 141954 <MGC_DrcDumpPipe+0x13d700>
    411c:   02000042    andeq   r0, r0, #66 ; 0x42
    4120:   670d2823    strvs   r2, [sp, -r3, lsr #16]
    4124:   614d7465    cmpvs   sp, r5, ror #8
    4128:   75716c69    ldrbvc  r6, [r1, #-3177]!
    412c:   46657565    strbtmi r7, [r5], -r5, ror #10
    4130:   00636e75    rsbeq   r6, r3, r5, ror lr
    4134:   ea05240d    b   14d170 <MGC_DrcDumpPipe+0x148f1c>
    4138:   02000042    andeq   r0, r0, #66 ; 0x42
    413c:   730d2c23    movwvc  r2, #56355  ; 0xdc23
    4140:   614d7465    cmpvs   sp, r5, ror #8
    4144:   75716c69    ldrbvc  r6, [r1, #-3177]!
    4148:   46657565    strbtmi r7, [r5], -r5, ror #10
    414c:   00636e75    rsbeq   r6, r3, r5, ror lr
    4150:   ea054e0d    b   15798c <MGC_DrcDumpPipe+0x153738>
    4154:   02000042    andeq   r0, r0, #66 ; 0x42
    4158:   670d3023    strvs   r3, [sp, -r3, lsr #32]
    415c:   68507465    ldmdavs r0, {r0, r2, r5, r6, sl, ip, sp, lr}^
    4160:   64417379    strbvs  r7, [r1], #-889
    4164:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
    4168:   6e754673    mrcvs   6, 3, r4, cr5, cr3, {3}
    416c:   5f0d0063    svcpl   0x000d0063
    4170:   00430005    subeq   r0, r3, r5
    4174:   34230200    strtcc  r0, [r3], #-512
    4178:   7465670d    strbtvc r6, [r5], #-1805
    417c:   74726956    ldrbtvc r6, [r2], #-2390
    4180:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
    4184:   46737365    ldrbtmi r7, [r3], -r5, ror #6
    4188:   00636e75    rsbeq   r6, r3, r5, ror lr
    418c:   0005700d    andeq   r7, r5, sp
    4190:   02000043    andeq   r0, r0, #67 ; 0x43
    4194:   6d0d3823    stcvs   8, cr3, [sp, #-140]
    4198:   6f6c6c61    svcvs   0x006c6c61
    419c:   6e754663    cdpvs   6, 7, cr4, cr5, cr3, {3}
    41a0:   840d0063    strhi   r0, [sp], #-99
    41a4:   00431605    subeq   r1, r3, r5, lsl #12
    41a8:   3c230200    sfmcc   f0, 4, [r3]
    41ac:   6c616d0d    stclvs  13, cr6, [r1], #-52
    41b0:   55636f6c    strbpl  r6, [r3, #-3948]!
    41b4:   6361636e    cmnvs   r1, #-1207959551    ; 0xb8000001
    41b8:   46646568    strbtmi r6, [r4], -r8, ror #10
    41bc:   00636e75    rsbeq   r6, r3, r5, ror lr
    41c0:   1605980d    strne   r9, [r5], -sp, lsl #16
    41c4:   02000043    andeq   r0, r0, #67 ; 0x43
    41c8:   6d0d4023    stcvs   0, cr4, [sp, #-140]
    41cc:   6f6c6c61    svcvs   0x006c6c61
    41d0:   63614363    cmnvs   r1, #-1946157055    ; 0x8c000001
    41d4:   46646568    strbtmi r6, [r4], -r8, ror #10
    41d8:   00636e75    rsbeq   r6, r3, r5, ror lr
    41dc:   1605ac0d    strne   sl, [r5], -sp, lsl #24
    41e0:   02000043    andeq   r0, r0, #67 ; 0x43
    41e4:   6d0d4423    cfstrsvs    mvf4, [sp, #-140]
    41e8:   6f6c6c61    svcvs   0x006c6c61
    41ec:   67655363    strbvs  r5, [r5, -r3, ror #6]!
    41f0:   746e656d    strbtvc r6, [lr], #-1389
    41f4:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    41f8:   05c10d00    strbeq  r0, [r1, #3328]
    41fc:   00004331    andeq   r4, r0, r1, lsr r3
    4200:   0d482302    stcleq  3, cr2, [r8, #-8]
    4204:   65657266    strbvs  r7, [r5, #-614]!
    4208:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    420c:   05d30d00    ldrbeq  r0, [r3, #3328]
    4210:   000007ff    strdeq  r0, [r0], -pc
    4214:   0d4c2302    stcleq  3, cr2, [ip, #-8]
    4218:   6e697270    mcrvs   2, 3, r7, cr9, cr0, {3}
    421c:   75466674    strbvc  r6, [r6, #-1652]
    4220:   0d00636e    stceq   3, cr6, [r0, #-440]
    4224:   434805e8    movtmi  r0, #34280  ; 0x85e8
    4228:   23020000    movwcs  r0, #8192   ; 0x2000
    422c:   6e690d50    mcrvs   13, 3, r0, cr9, cr0, {2}
    4230:   72447469    subvc   r7, r4, #1761607680 ; 0x69000000
    4234:   72657669    rsbvc   r7, r5, #110100480  ; 0x6900000
    4238:   6e754673    mrcvs   6, 3, r4, cr5, cr3, {3}
    423c:   090d0063    stmdbeq sp, {r0, r1, r5, r6}
    4240:   00436a06    subeq   r6, r3, r6, lsl #20
    4244:   54230200    strtpl  r0, [r3], #-512
    4248:   a7011300    strge   r1, [r1, -r0, lsl #6]
    424c:   5900003f    stmdbpl r0, {r0, r1, r2, r3, r4, r5}
    4250:   10000042    andne   r0, r0, r2, asr #32
    4254:   0000017f    andeq   r0, r0, pc, ror r1
    4258:   49040700    stmdbmi r4, {r8, r9, sl}
    425c:   0f000042    svceq   0x00000042
    4260:   00426b01    subeq   r6, r2, r1, lsl #22
    4264:   3fa71000    svccc   0x00a71000
    4268:   07000000    streq   r0, [r0, -r0]
    426c:   00425f04    subeq   r5, r2, r4, lsl #30
    4270:   7f011300    svcvc   0x00011300
    4274:   86000001    strhi   r0, [r0], -r1
    4278:   10000042    andne   r0, r0, r2, asr #32
    427c:   00003fa7    andeq   r3, r0, r7, lsr #31
    4280:   00018e10    andeq   r8, r1, r0, lsl lr
    4284:   04070000    streq   r0, [r7]
    4288:   00004271    andeq   r4, r0, r1, ror r2
    428c:   429d010f    addsmi  r0, sp, #-1073741821    ; 0xc0000003
    4290:   a7100000    ldrge   r0, [r0, -r0]
    4294:   1000003f    andne   r0, r0, pc, lsr r0
    4298:   0000018e    andeq   r0, r0, lr, lsl #3
    429c:   8c040700    stchi   7, cr0, [r4], {0}
    42a0:   13000042    movwne  r0, #66 ; 0x42
    42a4:   003fc301    eorseq  ip, pc, r1, lsl #6
    42a8:   0042b800    subeq   fp, r2, r0, lsl #16
    42ac:   018e1000    orreq   r1, lr, r0
    42b0:   8e100000    wxorhi  wr0, wr0, wr0
    42b4:   00000001    andeq   r0, r0, r1
    42b8:   42a30407    adcmi   r0, r3, #117440512  ; 0x7000000
    42bc:   010f0000    mrseq   r0, CPSR
    42c0:   000042ca    andeq   r4, r0, sl, asr #5
    42c4:   003fc310    eorseq  ip, pc, r0, lsl r3
    42c8:   04070000    streq   r0, [r7]
    42cc:   000042be    strheq  r4, [r0], -lr
    42d0:   017f0113    cmneq   pc, r3, lsl r1
    42d4:   42ea0000    rscmi   r0, sl, #0  ; 0x0
    42d8:   c3100000    tstgt   r0, #0  ; 0x0
    42dc:   1000003f    andne   r0, r0, pc, lsr r0
    42e0:   000000f1    strdeq  r0, [r0], -r1
    42e4:   00018e10    andeq   r8, r1, r0, lsl lr
    42e8:   04070000    streq   r0, [r7]
    42ec:   000042d0    ldrdeq  r4, [r0], -r0
    42f0:   018e0113    orreq   r0, lr, r3, lsl r1
    42f4:   43000000    movwmi  r0, #0  ; 0x0
    42f8:   8e100000    wxorhi  wr0, wr0, wr0
    42fc:   00000001    andeq   r0, r0, r1
    4300:   42f00407    rscsmi  r0, r0, #117440512  ; 0x7000000
    4304:   01130000    tsteq   r3, r0
    4308:   000000f1    strdeq  r0, [r0], -r1
    430c:   00004316    andeq   r4, r0, r6, lsl r3
    4310:   00008610    andeq   r8, r0, r0, lsl r6
    4314:   04070000    streq   r0, [r7]
    4318:   00004306    andeq   r4, r0, r6, lsl #6
    431c:   00f10113    rscseq  r0, r1, r3, lsl r1
    4320:   43310000    teqmi   r1, #0  ; 0x0
    4324:   b3100000    tstlt   r0, #0  ; 0x0
    4328:   10000001    andne   r0, r0, r1
    432c:   00000086    andeq   r0, r0, r6, lsl #1
    4330:   1c040700    stcne   7, cr0, [r4], {0}
    4334:   13000043    movwne  r0, #67 ; 0x43
    4338:   00017f01    andeq   r7, r1, r1, lsl #30
    433c:   00434800    subeq   r4, r3, r0, lsl #16
    4340:   01161000    tsteq   r6, r0
    4344:   001e0000    andseq  r0, lr, r0
    4348:   43370407    teqmi   r7, #117440512  ; 0x7000000
    434c:   01130000    tsteq   r3, r0
    4350:   0000017f    andeq   r0, r0, pc, ror r1
    4354:   0000435e    andeq   r4, r0, lr, asr r3
    4358:   00435e10    subeq   r5, r3, r0, lsl lr
    435c:   04070000    streq   r0, [r7]
    4360:   00004364    andeq   r4, r0, r4, ror #6
    4364:   03410407    movteq  r0, #5127   ; 0x1407
    4368:   04070000    streq   r0, [r7]
    436c:   0000434e    andeq   r4, r0, lr, asr #6
    4370:   50414611    subpl   r4, r1, r1, lsl r6
    4374:   59535f49    ldmdbpl r3, {r0, r3, r6, r8, r9, sl, fp, ip, lr}^
    4378:   65535f53    ldrbvs  r5, [r3, #-3923]
    437c:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    4380:   00547365    subseq  r7, r4, r5, ror #6
    4384:   df060b0d    svcle   0x00060b0d
    4388:   1f00003f    svcne   0x0000003f
    438c:   43474d01    movtmi  r4, #32001  ; 0x7d01
    4390:   6372445f    cmnvs   r2, #1593835520 ; 0x5f000000
    4394:   74696e49    strbtvc r6, [r9], #-3657
    4398:   01790100    cmneq   r9, r0, lsl #2
    439c:   00000160    andeq   r0, r0, r0, ror #2
    43a0:   00000000    andeq   r0, r0, r0
            43a0: R_ARM_ABS32   .text
    43a4:   0000032c    andeq   r0, r0, ip, lsr #6
            43a4: R_ARM_ABS32   .text
    43a8:   00000000    andeq   r0, r0, r0
            43a8: R_ARM_ABS32   .debug_loc
    43ac:   0000442a    andeq   r4, r0, sl, lsr #8
    43b0:   00017a20    andeq   r7, r1, r0, lsr #20
            43b1: R_ARM_ABS32   .debug_str
    43b4:   2a780100    bcs 1e047bc <MGC_DrcDumpPipe+0x1e00568>
    43b8:   03000044    movweq  r0, #68 ; 0x44
    43bc:   217efc91    ldrbcs  pc, [lr, #-193]!
    43c0:   646e4562    strbtvs r4, [lr], #-1378
    43c4:   607a0100    rsbsvs  r0, sl, r0, lsl #2
    43c8:   02000001    andeq   r0, r0, #1  ; 0x1
    43cc:   65216291    strvs   r6, [r1, #-657]!
    43d0:   0100646e    tsteq   r0, lr, ror #8
    43d4:   002bc37b    eoreq   ip, fp, fp, ror r3
    43d8:   80910300    addshi  r0, r1, r0, lsl #6
    43dc:   4662217f    undefined
    43e0:   536f6669    cmnpl   pc, #110100480  ; 0x6900000
    43e4:   00657a69    rsbeq   r7, r5, r9, ror #20
    43e8:   01607c01    cmneq   r0, r1, lsl #24
    43ec:   91020000    tstls   r2, r0
    43f0:   49702163    ldmdbmi r0!, {r0, r1, r5, r6, r8, sp}^
    43f4:   436c706d    cmnmi   ip, #109    ; 0x6d
    43f8:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    43fc:   656c6c6f    strbvs  r6, [ip, #-3183]!
    4400:   7d010072    stcvc   0, cr0, [r1, #-456]
    4404:   00003f67    andeq   r3, r0, r7, ror #30
    4408:   21649102    cmncs   r4, r2, lsl #2
    440c:   73614270    cmnvc   r1, #7  ; 0x7
    4410:   7e010065    cdpvc   0, 0, cr0, cr1, cr5, {3}
    4414:   000014e3    andeq   r1, r0, r3, ror #9
    4418:   21689102    cmncs   r8, r2, lsl #2
    441c:   006b4f62    rsbeq   r4, fp, r2, ror #30
    4420:   01607f01    cmneq   r0, r1, lsl #30
    4424:   91020000    tstls   r2, r0
    4428:   0407006f    streq   r0, [r7], #-111
    442c:   00003f96    muleq   r0, r6, pc
    4430:   474d011f    smlaldmi    r0, sp, pc, r1
    4434:   72445f43    subvc   r5, r4, #268    ; 0x10c
    4438:   72734963    rsbsvc  r4, r3, #1622016    ; 0x18c000
    443c:   01ca0100    biceq   r0, sl, r0, lsl #2
    4440:   000000a9    andeq   r0, r0, r9, lsr #1
    4444:   0000032c    andeq   r0, r0, ip, lsr #6
            4444: R_ARM_ABS32   .text
    4448:   000007a4    andeq   r0, r0, r4, lsr #15
            4448: R_ARM_ABS32   .text
    444c:   0000002a    andeq   r0, r0, sl, lsr #32
            444c: R_ARM_ABS32   .debug_loc
    4450:   0000451c    andeq   r4, r0, ip, lsl r5
    4454:   00011120    andeq   r1, r1, r0, lsr #2
            4455: R_ARM_ABS32   .debug_str
    4458:   67c80100    strbvs  r0, [r8, r0, lsl #2]
    445c:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4460:   62224c91    eorvs   r4, r2, #37120  ; 0x9100
    4464:   72746e49    rsbsvc  r6, r4, #1168   ; 0x490
    4468:   56627355    undefined
    446c:   65756c61    ldrbvs  r6, [r5, #-3169]!
    4470:   60c80100    sbcvs   r0, r8, r0, lsl #2
    4474:   02000001    andeq   r0, r0, #1  ; 0x1
    4478:   77224891    undefined
    447c:   72746e49    rsbsvc  r6, r4, #1168   ; 0x490
    4480:   61567854    cmpvs   r6, r4, asr r8
    4484:   0065756c    rsbeq   r7, r5, ip, ror #10
    4488:   016fc901    cmneq   pc, r1, lsl #18
    448c:   91020000    tstls   r2, r0
    4490:   49772244    ldmdbmi r7!, {r2, r6, r9, sp}^
    4494:   5272746e    rsbspl  r7, r2, #1845493760 ; 0x6e000000
    4498:   6c615678    stclvs  6, cr5, [r1], #-480
    449c:   01006575    tsteq   r0, r5, ror r5
    44a0:   00016fc9    andeq   r6, r1, r9, asr #31
    44a4:   40910200    addsmi  r0, r1, r0, lsl #4
    44a8:   74497121    strbvc  r7, [r9], #-289
    44ac:   01006d65    tsteq   r0, r5, ror #26
    44b0:   002861cb    eoreq   r6, r8, fp, asr #3
    44b4:   50910200    addspl  r0, r1, r0, lsl #4
    44b8:   52776421    rsbspl  r6, r7, #553648128  ; 0x21000000
    44bc:   61566765    cmpvs   r6, r5, ror #14
    44c0:   cc01006c    stcgt   0, cr0, [r1], {108}
    44c4:   0000018e    andeq   r0, r0, lr, lsl #3
    44c8:   21589102    cmpcs   r8, r2, lsl #2
    44cc:   65755162    ldrbvs  r5, [r5, #-354]!
    44d0:   01006575    tsteq   r0, r5, ror r5
    44d4:   000160cd    andeq   r6, r1, sp, asr #1
    44d8:   5f910200    svcpl   0x00910200
    44dc:   68536921    ldmdavs r3, {r0, r5, r8, fp, sp, lr}^
    44e0:   00746669    rsbseq  r6, r4, r9, ror #12
    44e4:   00cdce01    sbceq   ip, sp, r1, lsl #28
    44e8:   91020000    tstls   r2, r0
    44ec:   52692160    rsbpl   r2, r9, #24 ; 0x18
    44f0:   6c757365    ldclvs  3, cr7, [r5], #-404
    44f4:   cf010074    svcgt   0x00010074
    44f8:   000000a9    andeq   r0, r0, r9, lsr #1
    44fc:   23649102    cmncs   r4, #-2147483648    ; 0x80000000
    4500:   000001d4    ldrdeq  r0, [r0], -r4
            4500: R_ARM_ABS32   .debug_str
    4504:   39f9d001    ldmibcc r9!, {r0, ip, lr, pc}^
    4508:   91020000    tstls   r2, r0
    450c:   017a2368    cmneq   sl, r8, ror #6
            450e: R_ARM_ABS32   .debug_str
    4510:   d1010000    tstle   r1, r0
    4514:   0000442a    andeq   r4, r0, sl, lsr #8
    4518:   006c9102    rsbeq   r9, ip, r2, lsl #2
    451c:   474d0124    strbmi  r0, [sp, -r4, lsr #2]
    4520:   72445f43    subvc   r5, r4, #268    ; 0x10c
    4524:   6e694263    cdpvs   2, 6, cr4, cr9, cr3, {3}
    4528:   646e4564    strbtvs r4, [lr], #-1380
    452c:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    4530:   38010074    stmdacc r1, {r2, r4, r5, r6}
    4534:   2ca40101    stfcss  f0, [r4], #4
    4538:   07a40000    streq   r0, [r4, r0]!
            453a: R_ARM_ABS32   .text
    453c:   0b740000    bleq    1d00008 <MGC_DrcDumpPipe+0x1cfbdb4>
            453e: R_ARM_ABS32   .text
    4540:   00540000    subseq  r0, r4, r0
            4542: R_ARM_ABS32   .debug_loc
    4544:   46ae0000    strtmi  r0, [lr], r0
    4548:   7a250000    bvc 940008 <MGC_DrcDumpPipe+0x93bdb4>
            454b: R_ARM_ABS32   .debug_str
    454c:   01000001    tsteq   r0, r1
    4550:   442a0134    strtmi  r0, [sl], #-308
    4554:   91020000    tstls   r2, r0
    4558:   55702640    ldrbpl  r2, [r0, #-1600]!
    455c:   6e456273    mcrvs   2, 2, r6, cr5, cr3, {3}
    4560:   35010064    strcc   r0, [r1, #-100]
    4564:   003ac301    eorseq  ip, sl, r1, lsl #6
    4568:   bc910300    ldclt   3, cr0, [r1], {0}
    456c:   00c1257f    sbceq   r2, r1, pc, ror r5
            456e: R_ARM_ABS32   .debug_str
    4570:   36010000    strcc   r0, [r1], -r0
    4574:   0039ed01    eorseq  lr, r9, r1, lsl #26
    4578:   b8910300    ldmlt   r1, {r8, r9}
    457c:   4262267f    rsbmi   r2, r2, #133169152  ; 0x7f00000
    4580:   00646e69    rsbeq   r6, r4, r9, ror #28
    4584:   60013701    andvs   r3, r1, r1, lsl #14
    4588:   03000001    movweq  r0, #1  ; 0x1
    458c:   267fb491    undefined
    4590:   73496270    movtvc  r6, #37488  ; 0x9270
    4594:   01007854    tsteq   r0, r4, asr r8
    4598:   14e30137    strbtne r0, [r3], #311
    459c:   91020000    tstls   r2, r0
    45a0:   00ca2700    sbceq   r2, sl, r0, lsl #14
            45a2: R_ARM_ABS32   .debug_str
    45a4:   39010000    stmdbcc r1, {}
    45a8:   00018e01    andeq   r8, r1, r1, lsl #28
    45ac:   44910200    ldrmi   r0, [r1], #512
    45b0:   0001de27    andeq   sp, r1, r7, lsr #28
            45b1: R_ARM_ABS32   .debug_str
    45b4:   013a0100    teqeq   sl, r0, lsl #2
    45b8:   0000018e    andeq   r0, r0, lr, lsl #3
    45bc:   28489102    stmdacs r8, {r1, r8, ip, pc}^
    45c0:   49734962    ldmdbmi r3!, {r1, r5, r6, r8, fp, lr}^
    45c4:   3b01006e    blcc    44784 <MGC_DrcDumpPipe+0x40530>
    45c8:   00016001    andeq   r6, r1, r1
    45cc:   4e910200    cdpmi   2, 9, cr0, cr1, cr0, {0}
    45d0:   73496228    movtvc  r6, #37416  ; 0x9228
    45d4:   01007854    tsteq   r0, r4, asr r8
    45d8:   0160013c    cmneq   r0, ip, lsr r1
    45dc:   91020000    tstls   r2, r0
    45e0:   4562284f    strbmi  r2, [r2, #-2127]!
    45e4:   0100646e    tsteq   r0, lr, ror #8
    45e8:   0160013d    cmneq   r0, sp, lsr r1
    45ec:   91020000    tstls   r2, r0
    45f0:   00af2750    adceq   r2, pc, r0, asr r7
            45f2: R_ARM_ABS32   .debug_str
    45f4:   3e010000    cdpcc   0, 0, cr0, cr1, cr0, {0}
    45f8:   00016001    andeq   r6, r1, r1
    45fc:   51910200    orrspl  r0, r1, r0, lsl #4
    4600:   00008f27    andeq   r8, r0, r7, lsr #30
            4601: R_ARM_ABS32   .debug_str
    4604:   013f0100    teqeq   pc, r0, lsl #2
    4608:   0000016f    andeq   r0, r0, pc, ror #2
    460c:   28529102    ldmdacs r2, {r1, r8, ip, pc}^
    4610:   646e4570    strbtvs r4, [lr], #-1392
    4614:   01400100    cmpeq   r0, r0, lsl #2
    4618:   00002ca4    andeq   r2, r0, r4, lsr #25
    461c:   28549102    ldmdacs r4, {r1, r8, ip, pc}^
    4620:   69447764    stmdbvs r4, {r2, r5, r6, r8, r9, sl, ip, sp, lr}^
    4624:   01006666    tsteq   r0, r6, ror #12
    4628:   017f0141    cmneq   pc, r1, asr #2
    462c:   91020000    tstls   r2, r0
    4630:   77642858    undefined
    4634:   44716552    ldrbtmi r6, [r1], #-1362
    4638:   00666669    rsbeq   r6, r6, r9, ror #12
    463c:   7f014201    svcvc   0x00014201
    4640:   02000001    andeq   r0, r0, #1  ; 0x1
    4644:   1d275c91    stcne   12, cr5, [r7, #-580]!
            4647: R_ARM_ABS32   .debug_str
    4648:   01000001    tsteq   r0, r1
    464c:   016f0143    cmneq   pc, r3, asr #2
    4650:   91020000    tstls   r2, r0
    4654:   42702862    rsbsmi  r2, r0, #6422528    ; 0x620000
    4658:   45747365    ldrbmi  r7, [r4, #-869]!
    465c:   0100646e    tsteq   r0, lr, ror #8
    4660:   2ca40144    stfcss  f0, [r4], #272
    4664:   91020000    tstls   r2, r0
    4668:   42702864    rsbsmi  r2, r0, #6553600    ; 0x640000
    466c:   52747365    rsbspl  r7, r4, #-1811939327    ; 0x94000001
    4670:   6e457165    dvfvssz f7, f5, f5
    4674:   45010064    strmi   r0, [r1, #-100]
    4678:   002ca401    eoreq   sl, ip, r1, lsl #8
    467c:   68910200    ldmvs   r1, {r9}
    4680:   65427728    strbvs  r7, [r2, #-1832]
    4684:   69447473    stmdbvs r4, {r0, r1, r4, r5, r6, sl, ip, sp, lr}^
    4688:   01006666    tsteq   r0, r6, ror #12
    468c:   016f0146    cmneq   pc, r6, asr #2
    4690:   91020000    tstls   r2, r0
    4694:   4277286c    rsbsmi  r2, r7, #7077888    ; 0x6c0000
    4698:   52747365    rsbspl  r7, r4, #-1811939327    ; 0x94000001
    469c:   69447165    stmdbvs r4, {r0, r2, r5, r6, r8, ip, sp, lr}^
    46a0:   01006666    tsteq   r0, r6, ror #12
    46a4:   016f0147    cmneq   pc, r7, asr #2
    46a8:   91020000    tstls   r2, r0
    46ac:   0124006e    teqeq   r4, lr, rrx
    46b0:   5f43474d    svcpl   0x0043474d
    46b4:   53637244    cmnpl   r3, #1073741828 ; 0x40000004
    46b8:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
    46bc:   65446563    strbvs  r6, [r4, #-1379]
    46c0:   6c756166    ldfvse  f6, [r5], #-408
    46c4:   646e4574    strbtvs r4, [lr], #-1396
    46c8:   01aa0100    undefined instruction 0x01aa0100
    46cc:   00016001    andeq   r6, r1, r1
    46d0:   000b7400    andeq   r7, fp, r0, lsl #8
            46d1: R_ARM_ABS32   .text
    46d4:   0012c000    andseq  ip, r2, r0
            46d5: R_ARM_ABS32   .text
    46d8:   00007e00    andeq   r7, r0, r0, lsl #28
            46d9: R_ARM_ABS32   .debug_loc
    46dc:   0047b900    subeq   fp, r7, r0, lsl #18
    46e0:   017a2500    cmneq   sl, r0, lsl #10
            46e2: R_ARM_ABS32   .debug_str
    46e4:   a8010000    stmdage r1, {}
    46e8:   00442a01    subeq   r2, r4, r1, lsl #20
    46ec:   50910200    addspl  r0, r1, r0, lsl #4
    46f0:   74497026    strbvc  r7, [r9], #-38
    46f4:   01006d65    tsteq   r0, r5, ror #26
    46f8:   3c4d01a8    stfcce  f0, [sp], {168}
    46fc:   91020000    tstls   r2, r0
    4700:   7770264c    ldrbvc  r2, [r0, -ip, asr #12]!
    4704:   00727343    rsbseq  r7, r2, r3, asr #6
    4708:   b901a901    stmdblt r1, {r0, r8, fp, sp, pc}
    470c:   02000047    andeq   r0, r0, #71 ; 0x47
    4710:   67254891    undefined
            4713: R_ARM_ABS32   .debug_str
    4714:   01000000    tsteq   r0, r0
    4718:   016f01a9    smultbeq    pc, r9, r1
    471c:   91020000    tstls   r2, r0
    4720:   01652744    cmneq   r5, r4, asr #14
            4722: R_ARM_ABS32   .debug_str
    4724:   ab010000    blge    4472c <MGC_DrcDumpPipe+0x404d8>
    4728:   00016f01    andeq   r6, r1, r1, lsl #30
    472c:   58910200    ldmpl   r1, {r9}
    4730:   70537728    subsvc  r7, r3, r8, lsr #14
    4734:   00656361    rsbeq   r6, r5, r1, ror #6
    4738:   6f01ab01    svcvs   0x0001ab01
    473c:   02000001    andeq   r0, r0, #1  ; 0x1
    4740:   fa275a91    blx 9d6a4c <MGC_DrcDumpPipe+0x9d27f8>
            4743: R_ARM_ABS32   .debug_str
    4744:   01000000    tsteq   r0, r0
    4748:   14e301ac    strbtne r0, [r3], #428
    474c:   91020000    tstls   r2, r0
    4750:   00c1275c    sbceq   r2, r1, ip, asr r7
            4752: R_ARM_ABS32   .debug_str
    4754:   ad010000    stcge   0, cr0, [r1]
    4758:   0047bf01    subeq   fp, r7, r1, lsl #30
    475c:   60910200    addsvs  r0, r1, r0, lsl #4
    4760:   61566228    cmpvs   r6, r8, lsr #4
    4764:   ae01006c    cdpge   0, 0, cr0, cr1, cr12, {3}
    4768:   00016001    andeq   r6, r1, r1
    476c:   66910200    ldrvs   r0, [r1], r0, lsl #4
    4770:   754f6228    strbvc  r6, [pc, #-552] ; 4550 <.debug_info+0x4550>
    4774:   6c615674    stclvs  6, cr5, [r1], #-464
    4778:   01af0100    undefined instruction 0x01af0100
    477c:   00000160    andeq   r0, r0, r0, ror #2
    4780:   28579102    ldmdacs r7, {r1, r8, ip, pc}^
    4784:   73655262    cmnvc   r5, #536870918  ; 0x20000006
    4788:   00746c75    rsbseq  r6, r4, r5, ror ip
    478c:   6001b001    andvs   fp, r1, r1
    4790:   02000001    andeq   r0, r0, #1  ; 0x1
    4794:   73286791    teqvc   r8, #38010880   ; 0x2440000
    4798:   75746174    ldrbvc  r6, [r4, #-372]!
    479c:   b1010073    tstlt   r1, r3, ror r0
    47a0:   00016001    andeq   r6, r1, r1
    47a4:   56910200    ldrpl   r0, [r1], r0, lsl #4
    47a8:   6e457028    cdpvs   0, 4, cr7, cr5, cr8, {1}
    47ac:   b2010064    andlt   r0, r1, #100    ; 0x64
    47b0:   002ca401    eoreq   sl, ip, r1, lsl #8
    47b4:   68910200    ldmvs   r1, {r9}
    47b8:   6f040700    svcvs   0x00040700
    47bc:   07000001    streq   r0, [r0, -r1]
    47c0:   000d4d04    andeq   r4, sp, r4, lsl #26
    47c4:   4d012400    cfstrsmi    mvf2, [r1]
    47c8:   445f4347    ldrbmi  r4, [pc], #839  ; 47d0 <.debug_info+0x47d0>
    47cc:   65526372    ldrbvs  r6, [r2, #-882]
    47d0:   50746573    rsbspl  r6, r4, r3, ror r5
    47d4:   0074726f    rsbseq  r7, r4, pc, ror #4
    47d8:   01025a01    tsteq   r2, r1, lsl #20
    47dc:   0000018e    andeq   r0, r0, lr, lsl #3
    47e0:   000012c0    andeq   r1, r0, r0, asr #5
            47e0: R_ARM_ABS32   .text
    47e4:   000013cc    andeq   r1, r0, ip, asr #7
            47e4: R_ARM_ABS32   .text
    47e8:   000000a8    andeq   r0, r0, r8, lsr #1
            47e8: R_ARM_ABS32   .debug_loc
    47ec:   0000480f    andeq   r4, r0, pc, lsl #16
    47f0:   00017a25    andeq   r7, r1, r5, lsr #20
            47f1: R_ARM_ABS32   .debug_str
    47f4:   02590100    subseq  r0, r9, #0  ; 0x0
    47f8:   0000442a    andeq   r4, r0, sl, lsr #8
    47fc:   27689102    strbcs  r9, [r8, -r2, lsl #2]!
    4800:   00000111    andeq   r0, r0, r1, lsl r1
            4800: R_ARM_ABS32   .debug_str
    4804:   67025b01    strvs   r5, [r2, -r1, lsl #22]
    4808:   0200003f    andeq   r0, r0, #63 ; 0x3f
    480c:   29006c91    stmdbcs r0, {r0, r4, r7, sl, fp, sp, lr}
    4810:   5f43474d    svcpl   0x0043474d
    4814:   46637244    strbtmi r7, [r3], -r4, asr #4
    4818:   6873756c    ldmdavs r3!, {r2, r3, r5, r6, r8, sl, ip, sp, lr}^
    481c:   006c6c41    rsbeq   r6, ip, r1, asr #24
    4820:   01028401    tsteq   r2, r1, lsl #8
    4824:   000013cc    andeq   r1, r0, ip, asr #7
            4824: R_ARM_ABS32   .text
    4828:   000015f8    strdeq  r1, [r0], -r8
            4828: R_ARM_ABS32   .text
    482c:   000000d2    ldrdeq  r0, [r0], -r2
            482c: R_ARM_ABS32   .debug_loc
    4830:   000048b1    strheq  r4, [r0], -r1
    4834:   00017a25    andeq   r7, r1, r5, lsr #20
            4835: R_ARM_ABS32   .debug_str
    4838:   02830100    addeq   r0, r3, #0  ; 0x0
    483c:   0000442a    andeq   r4, r0, sl, lsr #8
    4840:   27549102    ldrbcs  r9, [r4, -r2, lsl #2]
    4844:   000000ca    andeq   r0, r0, sl, asr #1
            4844: R_ARM_ABS32   .debug_str
    4848:   8e028501    cfsh32hi    mvfx8, mvfx2, #1
    484c:   02000001    andeq   r0, r0, #1  ; 0x1
    4850:   de275891    mcrle   8, 1, r5, cr7, cr1, {4}
            4853: R_ARM_ABS32   .debug_str
    4854:   01000001    tsteq   r0, r1
    4858:   018e0286    orreq   r0, lr, r6, lsl #5
    485c:   91020000    tstls   r2, r0
    4860:   0067275c    rsbeq   r2, r7, ip, asr r7
            4862: R_ARM_ABS32   .debug_str
    4864:   87010000    strhi   r0, [r1, -r0]
    4868:   00016f02    andeq   r6, r1, r2, lsl #30
    486c:   60910200    addsvs  r0, r1, r0, lsl #4
    4870:   0000ba27    andeq   fp, r0, r7, lsr #20
            4871: R_ARM_ABS32   .debug_str
    4874:   02880100    addeq   r0, r8, #0  ; 0x0
    4878:   0000016f    andeq   r0, r0, pc, ror #2
    487c:   28629102    stmdacs r2!, {r1, r8, ip, pc}^
    4880:   70724970    rsbsvc  r4, r2, r0, ror r9
    4884:   02890100    addeq   r0, r9, #0  ; 0x0
    4888:   000000f1    strdeq  r0, [r0], -r1
    488c:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    4890:   646e4570    strbtvs r4, [lr], #-1392
    4894:   028a0100    addeq   r0, sl, #0  ; 0x0
    4898:   00002ca4    andeq   r2, r0, r4, lsr #25
    489c:   28689102    stmdacs r8!, {r1, r8, ip, pc}^
    48a0:   73614270    cmnvc   r1, #7  ; 0x7
    48a4:   8b010065    blhi    44a40 <MGC_DrcDumpPipe+0x407ec>
    48a8:   0014e302    andseq  lr, r4, r2, lsl #6
    48ac:   6c910200    lfmvs   f0, 4, [r1], {0}
    48b0:   4d012a00    fstsmi  s4, [r1]
    48b4:   445f4347    ldrbmi  r4, [pc], #839  ; 48bc <.debug_info+0x48bc>
    48b8:   744f6372    strbvc  r6, [pc], #882  ; 48c0 <.debug_info+0x48c0>
    48bc:   6d695467    cfstrdvs    mvd5, [r9, #-412]!
    48c0:   01007265    tsteq   r0, r5, ror #4
    48c4:   f80102b2    undefined instruction 0xf80102b2
            48c7: R_ARM_ABS32   .text
    48c8:   2c000015    stccs   0, cr0, [r0], {21}
            48cb: R_ARM_ABS32   .text
    48cc:   fc000017    stc2    0, cr0, [r0], {23}
            48cf: R_ARM_ABS32   .debug_loc
    48d0:   23000000    movwcs  r0, #0  ; 0x0
    48d4:   25000049    strcs   r0, [r0, #-73]
    48d8:   0000007e    andeq   r0, r0, lr, ror r0
            48d8: R_ARM_ABS32   .debug_str
    48dc:   f102b101    undefined instruction 0xf102b101
    48e0:   02000000    andeq   r0, r0, #0  ; 0x0
    48e4:   de256091    mcrle   0, 1, r6, cr5, cr1, {4}
            48e7: R_ARM_ABS32   .debug_str
    48e8:   01000000    tsteq   r0, r0
    48ec:   016f02b1    strheq  r0, [pc, #-33]  ; 48d3 <.debug_info+0x48d3>
    48f0:   91020000    tstls   r2, r0
    48f4:   0111275c    tsteq   r1, ip, asr r7
            48f6: R_ARM_ABS32   .debug_str
    48f8:   b3010000    movwlt  r0, #4096   ; 0x1000
    48fc:   003f6702    eorseq  r6, pc, r2, lsl #14
    4900:   64910200    ldrvs   r0, [r1], #512
    4904:   00017a27    andeq   r7, r1, r7, lsr #20
            4905: R_ARM_ABS32   .debug_str
    4908:   02b40100    adcseq  r0, r4, #0  ; 0x0
    490c:   0000442a    andeq   r4, r0, sl, lsr #8
    4910:   27689102    strbcs  r9, [r8, -r2, lsl #2]!
    4914:   000000e5    andeq   r0, r0, r5, ror #1
            4914: R_ARM_ABS32   .debug_str
    4918:   6d02b501    cfstr32vs   mvfx11, [r2, #-4]
    491c:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4920:   2b006c91    blcs    1fb6c <MGC_DrcDumpPipe+0x1b918>
    4924:   5f43474d    svcpl   0x0043474d
    4928:   74697845    strbtvc r7, [r9], #-2117
    492c:   48676e69    stmdami r7!, {r0, r3, r5, r6, r9, sl, fp, sp, lr}^
    4930:   0074736f    rsbseq  r7, r4, pc, ror #6
    4934:   0102d201    tsteq   r2, r1, lsl #4
    4938:   00000160    andeq   r0, r0, r0, ror #2
    493c:   0000172c    andeq   r1, r0, ip, lsr #14
            493c: R_ARM_ABS32   .text
    4940:   000017bc    strheq  r1, [r0], -ip
            4940: R_ARM_ABS32   .text
    4944:   00000126    andeq   r0, r0, r6, lsr #2
            4944: R_ARM_ABS32   .debug_loc
    4948:   0000496d    andeq   r4, r0, sp, ror #18
    494c:   4f6f7426    svcmi   0x006f7426
    4950:   01006774    tsteq   r0, r4, ror r7
    4954:   016002d1    ldrdeq  r0, [r0, #-33]!
    4958:   91020000    tstls   r2, r0
    495c:   017a256c    cmneq   sl, ip, ror #10
            495e: R_ARM_ABS32   .debug_str
    4960:   d1010000    tstle   r1, r0
    4964:   00442a02    subeq   r2, r4, r2, lsl #20
    4968:   68910200    ldmvs   r1, {r9}
    496c:   474d2900    strbmi  r2, [sp, -r0, lsl #18]
    4970:   78455f43    stmdavc r5, {r0, r1, r6, r8, r9, sl, fp, ip, lr}^
    4974:   63417469    movtvs  r7, #5225   ; 0x1469
    4978:   65766974    ldrbvs  r6, [r6, #-2420]!
    497c:   4d627355    stclmi  3, cr7, [r2, #-340]!
    4980:   0065646f    rsbeq   r6, r5, pc, ror #8
    4984:   0102ec01    tsteq   r2, r1, lsl #24
    4988:   000017bc    strheq  r1, [r0], -ip
            4988: R_ARM_ABS32   .text
    498c:   00001810    andeq   r1, r0, r0, lsl r8
            498c: R_ARM_ABS32   .text
    4990:   00000150    andeq   r0, r0, r0, asr r1
            4990: R_ARM_ABS32   .debug_loc
    4994:   000049b9    strheq  r4, [r0], -r9
    4998:   4f6f7426    svcmi   0x006f7426
    499c:   01006774    tsteq   r0, r4, ror r7
    49a0:   016002eb    smultteq    r0, fp, r2
    49a4:   91020000    tstls   r2, r0
    49a8:   017a256c    cmneq   sl, ip, ror #10
            49aa: R_ARM_ABS32   .debug_str
    49ac:   eb010000    bl  449b4 <MGC_DrcDumpPipe+0x40760>
    49b0:   00442a02    subeq   r2, r4, r2, lsl #20
    49b4:   68910200    ldmvs   r1, {r9}
    49b8:   474d2900    strbmi  r2, [sp, -r0, lsl #18]
    49bc:   65535f43    ldrbvs  r5, [r3, #-3907]
    49c0:   776f5074    undefined
    49c4:   61537265    cmpvs   r3, r5, ror #4
    49c8:   01006576    tsteq   r0, r6, ror r5
    49cc:   100102f8    strdne  r0, [r1], -r8
            49cf: R_ARM_ABS32   .text
    49d0:   34000018    strcc   r0, [r0], #-24
            49d3: R_ARM_ABS32   .text
    49d4:   7a000018    bvc 68 <.debug_info+0x68>
            49d7: R_ARM_ABS32   .debug_loc
    49d8:   00000001    andeq   r0, r0, r1
    49dc:   2500004a    strcs   r0, [r0, #-74]
    49e0:   0000017a    andeq   r0, r0, sl, ror r1
            49e0: R_ARM_ABS32   .debug_str
    49e4:   2a02f701    bcs c25f0 <MGC_DrcDumpPipe+0xbe39c>
    49e8:   02000044    andeq   r0, r0, #68 ; 0x44
    49ec:   62266c91    eorvs   r6, r6, #37120  ; 0x9100
    49f0:   65766153    ldrbvs  r6, [r6, #-339]!
    49f4:   02f70100    rscseq  r0, r7, #0  ; 0x0
    49f8:   00000160    andeq   r0, r0, r0, ror #2
    49fc:   00689102    rsbeq   r9, r8, r2, lsl #2
    4a00:   43474d29    movtmi  r4, #32041  ; 0x7d29
    4a04:   6174535f    cmnvs   r4, pc, asr r3
    4a08:   65507472    ldrbvs  r7, [r0, #-1138]
    4a0c:   68706972    ldmdavs r0!, {r1, r4, r5, r6, r8, fp, sp, lr}^
    4a10:   6c617265    sfmvs   f7, 2, [r1], #-404
    4a14:   65646f4d    strbvs  r6, [r4, #-3917]!
    4a18:   03060100    movweq  r0, #24832  ; 0x6100
    4a1c:   00183401    andseq  r3, r8, r1, lsl #8
            4a1d: R_ARM_ABS32   .text
    4a20:   00184c00    andseq  r4, r8, r0, lsl #24
            4a21: R_ARM_ABS32   .text
    4a24:   0001a400    andeq   sl, r1, r0, lsl #8
            4a25: R_ARM_ABS32   .debug_loc
    4a28:   004a3d00    subeq   r3, sl, r0, lsl #26
    4a2c:   017a2500    cmneq   sl, r0, lsl #10
            4a2e: R_ARM_ABS32   .debug_str
    4a30:   05010000    streq   r0, [r1]
    4a34:   00442a03    subeq   r2, r4, r3, lsl #20
    4a38:   6c910200    lfmvs   f0, 4, [r1], {0}
    4a3c:   474d2900    strbmi  r2, [sp, -r0, lsl #18]
    4a40:   69415f43    stmdbvs r1, {r0, r1, r6, r8, r9, sl, fp, ip, lr}^
    4a44:   64426c64    strbvs  r6, [r2], #-3172
    4a48:   69547369    ldmdbvs r4, {r0, r3, r5, r6, r8, r9, ip, sp, lr}^
    4a4c:   756f656d    strbvc  r6, [pc, #-1389]!   ; 44e7 <.debug_info+0x44e7>
    4a50:   17010074    smlsdxne    r1, r4, r0, r0
    4a54:   184c0103    stmdane ip, {r0, r1, r8}^
            4a56: R_ARM_ABS32   .text
    4a58:   19300000    ldmdbne r0!, {}
            4a5a: R_ARM_ABS32   .text
    4a5c:   01ce0000    biceq   r0, lr, r0
            4a5e: R_ARM_ABS32   .debug_loc
    4a60:   4ab20000    bmi fec84a68 <MGC_DrcDumpPipe+0xfec80814>
    4a64:   7e250000    cdpvc   0, 2, cr0, cr5, cr0, {0}
            4a67: R_ARM_ABS32   .debug_str
    4a68:   01000000    tsteq   r0, r0
    4a6c:   00f10316    rscseq  r0, r1, r6, lsl r3
    4a70:   91020000    tstls   r2, r0
    4a74:   00de2560    sbcseq  r2, lr, r0, ror #10
            4a76: R_ARM_ABS32   .debug_str
    4a78:   16010000    strne   r0, [r1], -r0
    4a7c:   00016f03    andeq   r6, r1, r3, lsl #30
    4a80:   5c910200    lfmpl   f0, 4, [r1], {0}
    4a84:   00011127    andeq   r1, r1, r7, lsr #2
            4a85: R_ARM_ABS32   .debug_str
    4a88:   03180100    tsteq   r8, #0  ; 0x0
    4a8c:   00003f67    andeq   r3, r0, r7, ror #30
    4a90:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    4a94:   0000017a    andeq   r0, r0, sl, ror r1
            4a94: R_ARM_ABS32   .debug_str
    4a98:   2a031901    bcs caea4 <MGC_DrcDumpPipe+0xc6c50>
    4a9c:   02000044    andeq   r0, r0, #68 ; 0x44
    4aa0:   e5276891    str r6, [r7, #-2193]!
            4aa3: R_ARM_ABS32   .debug_str
    4aa4:   01000000    tsteq   r0, r0
    4aa8:   3f6d031a    svccc   0x006d031a
    4aac:   91020000    tstls   r2, r0
    4ab0:   4d29006c    stcmi   0, cr0, [r9, #-432]!
    4ab4:   575f4347    ldrbpl  r4, [pc, -r7, asr #6]
    4ab8:   53746961    cmnpl   r4, #1589248    ; 0x184000
    4abc:   52737365    rsbspl  r7, r3, #-1811939327    ; 0x94000001
    4ac0:   69547165    ldmdbvs r4, {r0, r2, r5, r6, r8, ip, sp, lr}^
    4ac4:   756f656d    strbvc  r6, [pc, #-1389]!   ; 455f <.debug_info+0x455f>
    4ac8:   31010074    tstcc   r1, r4, ror r0
    4acc:   19300103    ldmdbne r0!, {r0, r1, r8}
            4ace: R_ARM_ABS32   .text
    4ad0:   1a140000    bne 500008 <MGC_DrcDumpPipe+0x4fbdb4>
            4ad2: R_ARM_ABS32   .text
    4ad4:   01f80000    mvnseq  r0, r0
            4ad6: R_ARM_ABS32   .debug_loc
    4ad8:   4b2a0000    blmi    a84ae0 <MGC_DrcDumpPipe+0xa8088c>
    4adc:   7e250000    cdpvc   0, 2, cr0, cr5, cr0, {0}
            4adf: R_ARM_ABS32   .debug_str
    4ae0:   01000000    tsteq   r0, r0
    4ae4:   00f10330    rscseq  r0, r1, r0, lsr r3
    4ae8:   91020000    tstls   r2, r0
    4aec:   00de2560    sbcseq  r2, lr, r0, ror #10
            4aee: R_ARM_ABS32   .debug_str
    4af0:   30010000    andcc   r0, r1, r0
    4af4:   00016f03    andeq   r6, r1, r3, lsl #30
    4af8:   5c910200    lfmpl   f0, 4, [r1], {0}
    4afc:   00011127    andeq   r1, r1, r7, lsr #2
            4afd: R_ARM_ABS32   .debug_str
    4b00:   03320100    teqeq   r2, #0  ; 0x0
    4b04:   00003f67    andeq   r3, r0, r7, ror #30
    4b08:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    4b0c:   0000017a    andeq   r0, r0, sl, ror r1
            4b0c: R_ARM_ABS32   .debug_str
    4b10:   2a033301    bcs d171c <MGC_DrcDumpPipe+0xcd4c8>
    4b14:   02000044    andeq   r0, r0, #68 ; 0x44
    4b18:   e5276891    str r6, [r7, #-2193]!
            4b1b: R_ARM_ABS32   .debug_str
    4b1c:   01000000    tsteq   r0, r0
    4b20:   3f6d0334    svccc   0x006d0334
    4b24:   91020000    tstls   r2, r0
    4b28:   4d29006c    stcmi   0, cr0, [r9, #-432]!
    4b2c:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    4b30:   5274736f    rsbspl  r7, r4, #-1140850687    ; 0xbc000001
    4b34:   74657365    strbtvc r7, [r5], #-869
    4b38:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
    4b3c:   4b010074    blmi    44d14 <MGC_DrcDumpPipe+0x40ac0>
    4b40:   1a140103    bne 500414 <MGC_DrcDumpPipe+0x4fc1c0>
            4b42: R_ARM_ABS32   .text
    4b44:   1ae80000    bne ffa00008 <MGC_DrcDumpPipe+0xff9fbdb4>
            4b46: R_ARM_ABS32   .text
    4b48:   02220000    eoreq   r0, r2, #0  ; 0x0
            4b4a: R_ARM_ABS32   .debug_loc
    4b4c:   4b8f0000    blmi    fe3c4b54 <MGC_DrcDumpPipe+0xfe3c0900>
    4b50:   7e250000    cdpvc   0, 2, cr0, cr5, cr0, {0}
            4b53: R_ARM_ABS32   .debug_str
    4b54:   01000000    tsteq   r0, r0
    4b58:   00f1034a    rscseq  r0, r1, sl, asr #6
    4b5c:   91020000    tstls   r2, r0
    4b60:   00de2564    sbcseq  r2, lr, r4, ror #10
            4b62: R_ARM_ABS32   .debug_str
    4b64:   4a010000    bmi 44b6c <MGC_DrcDumpPipe+0x40918>
    4b68:   00016f03    andeq   r6, r1, r3, lsl #30
    4b6c:   60910200    addsvs  r0, r1, r0, lsl #4
    4b70:   00011127    andeq   r1, r1, r7, lsr #2
            4b71: R_ARM_ABS32   .debug_str
    4b74:   034c0100    movteq  r0, #49408  ; 0xc100
    4b78:   00003f67    andeq   r3, r0, r7, ror #30
    4b7c:   27689102    strbcs  r9, [r8, -r2, lsl #2]!
    4b80:   0000017a    andeq   r0, r0, sl, ror r1
            4b80: R_ARM_ABS32   .debug_str
    4b84:   2a034d01    bcs d7f90 <MGC_DrcDumpPipe+0xd3d3c>
    4b88:   02000044    andeq   r0, r0, #68 ; 0x44
    4b8c:   29006c91    stmdbcs r0, {r0, r4, r7, sl, fp, sp, lr}
    4b90:   5f43474d    svcpl   0x0043474d
    4b94:   74736f48    ldrbtvc r6, [r3], #-3912
    4b98:   65736552    ldrbvs  r6, [r3, #-1362]!
    4b9c:   6d6f4374    stclvs  3, cr4, [pc, #-464]!
    4ba0:   74656c70    strbtvc r6, [r5], #-3184
    4ba4:   5f010065    svcpl   0x00010065
    4ba8:   1ae80103    bne ffa00414 <MGC_DrcDumpPipe+0xff9fc1c0>
            4baa: R_ARM_ABS32   .text
    4bac:   1c8c0000    stcne   0, cr0, [ip], {0}
            4bae: R_ARM_ABS32   .text
    4bb0:   024c0000    subeq   r0, ip, #0  ; 0x0
            4bb2: R_ARM_ABS32   .debug_loc
    4bb4:   4c180000    ldcmi   0, cr0, [r8], {0}
    4bb8:   7e250000    cdpvc   0, 2, cr0, cr5, cr0, {0}
            4bbb: R_ARM_ABS32   .debug_str
    4bbc:   01000000    tsteq   r0, r0
    4bc0:   00f1035e    rscseq  r0, r1, lr, asr r3
    4bc4:   91020000    tstls   r2, r0
    4bc8:   00de2560    sbcseq  r2, lr, r0, ror #10
            4bca: R_ARM_ABS32   .debug_str
    4bcc:   5e010000    cdppl   0, 0, cr0, cr1, cr0, {0}
    4bd0:   00016f03    andeq   r6, r1, r3, lsl #30
    4bd4:   5c910200    lfmpl   f0, 4, [r1], {0}
    4bd8:   0000d527    andeq   sp, r0, r7, lsr #10
            4bd9: R_ARM_ABS32   .debug_str
    4bdc:   03600100    cmneq   r0, #0  ; 0x0
    4be0:   00000160    andeq   r0, r0, r0, ror #2
    4be4:   28669102    stmdacs r6!, {r1, r8, ip, pc}^
    4be8:   65705362    ldrbvs  r5, [r0, #-866]!
    4bec:   01006465    tsteq   r0, r5, ror #8
    4bf0:   01600361    cmneq   r0, r1, ror #6
    4bf4:   91020000    tstls   r2, r0
    4bf8:   01112767    tsteq   r1, r7, ror #14
            4bfa: R_ARM_ABS32   .debug_str
    4bfc:   62010000    andvs   r0, r1, #0  ; 0x0
    4c00:   003f6703    eorseq  r6, pc, r3, lsl #14
    4c04:   68910200    ldmvs   r1, {r9}
    4c08:   00017a27    andeq   r7, r1, r7, lsr #20
            4c09: R_ARM_ABS32   .debug_str
    4c0c:   03630100    cmneq   r3, #0  ; 0x0
    4c10:   0000442a    andeq   r4, r0, sl, lsr #8
    4c14:   006c9102    rsbeq   r9, ip, r2, lsl #2
    4c18:   43474d29    movtmi  r4, #32041  ; 0x7d29
    4c1c:   7253425f    subsvc  r4, r3, #-268435451 ; 0xf0000005
    4c20:   69614670    stmdbvs r1!, {r4, r5, r6, r9, sl, lr}^
    4c24:   0064656c    rsbeq   r6, r4, ip, ror #10
    4c28:   01038a01    tsteq   r3, r1, lsl #20
    4c2c:   00001c8c    andeq   r1, r0, ip, lsl #25
            4c2c: R_ARM_ABS32   .text
    4c30:   00001e14    andeq   r1, r0, r4, lsl lr
            4c30: R_ARM_ABS32   .text
    4c34:   00000276    andeq   r0, r0, r6, ror r2
            4c34: R_ARM_ABS32   .debug_loc
    4c38:   00004c88    andeq   r4, r0, r8, lsl #25
    4c3c:   00007e25    andeq   r7, r0, r5, lsr #28
            4c3d: R_ARM_ABS32   .debug_str
    4c40:   03890100    orreq   r0, r9, #0  ; 0x0
    4c44:   000000f1    strdeq  r0, [r0], -r1
    4c48:   25609102    strbcs  r9, [r0, #-258]!
    4c4c:   000000de    ldrdeq  r0, [r0], -lr
            4c4c: R_ARM_ABS32   .debug_str
    4c50:   6f038901    svcvs   0x00038901
    4c54:   02000001    andeq   r0, r0, #1  ; 0x1
    4c58:   11275c91    strbne  r5, [r7, -r1]!
            4c5b: R_ARM_ABS32   .debug_str
    4c5c:   01000001    tsteq   r0, r1
    4c60:   3f67038b    svccc   0x0067038b
    4c64:   91020000    tstls   r2, r0
    4c68:   017a2764    cmneq   sl, r4, ror #14
            4c6a: R_ARM_ABS32   .debug_str
    4c6c:   8c010000    stchi   0, cr0, [r1], {0}
    4c70:   00442a03    subeq   r2, r4, r3, lsl #20
    4c74:   68910200    ldmvs   r1, {r9}
    4c78:   0000e527    andeq   lr, r0, r7, lsr #10
            4c79: R_ARM_ABS32   .debug_str
    4c7c:   038d0100    orreq   r0, sp, #0  ; 0x0
    4c80:   00003f6d    andeq   r3, r0, sp, ror #30
    4c84:   006c9102    rsbeq   r9, ip, r2, lsl #2
    4c88:   43474d29    movtmi  r4, #32041  ; 0x7d29
    4c8c:   6141425f    cmpvs   r1, pc, asr r2
    4c90:   42306573    eorsmi  r6, r0, #482344960  ; 0x1cc00000
    4c94:   54747372    ldrbtpl r7, [r4], #-882
    4c98:   6f656d69    svcvs   0x00656d69
    4c9c:   01007475    tsteq   r0, r5, ror r4
    4ca0:   140103b3    strne   r0, [r1], #-947
            4ca3: R_ARM_ABS32   .text
    4ca4:   0000001e    andeq   r0, r0, lr, lsl r0
            4ca7: R_ARM_ABS32   .text
    4ca8:   a000001f    andge   r0, r0, pc, lsl r0
            4cab: R_ARM_ABS32   .debug_loc
    4cac:   ff000002    undefined instruction 0xff000002
    4cb0:   2500004c    strcs   r0, [r0, #-76]
    4cb4:   0000007e    andeq   r0, r0, lr, ror r0
            4cb4: R_ARM_ABS32   .debug_str
    4cb8:   f103b201    undefined instruction 0xf103b201
    4cbc:   02000000    andeq   r0, r0, #0  ; 0x0
    4cc0:   de256091    mcrle   0, 1, r6, cr5, cr1, {4}
            4cc3: R_ARM_ABS32   .debug_str
    4cc4:   01000000    tsteq   r0, r0
    4cc8:   016f03b2    strheq  r0, [pc, #-50]  ; 4c9e <.debug_info+0x4c9e>
    4ccc:   91020000    tstls   r2, r0
    4cd0:   0111275c    tsteq   r1, ip, asr r7
            4cd2: R_ARM_ABS32   .debug_str
    4cd4:   b4010000    strlt   r0, [r1]
    4cd8:   003f6703    eorseq  r6, pc, r3, lsl #14
    4cdc:   64910200    ldrvs   r0, [r1], #512
    4ce0:   00017a27    andeq   r7, r1, r7, lsr #20
            4ce1: R_ARM_ABS32   .debug_str
    4ce4:   03b50100    undefined instruction 0x03b50100
    4ce8:   0000442a    andeq   r4, r0, sl, lsr #8
    4cec:   27689102    strbcs  r9, [r8, -r2, lsl #2]!
    4cf0:   000000e5    andeq   r0, r0, r5, ror #1
            4cf0: R_ARM_ABS32   .debug_str
    4cf4:   6d03b601    stcvs   6, cr11, [r3, #-4]
    4cf8:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4cfc:   29006c91    stmdbcs r0, {r0, r4, r7, sl, fp, sp, lr}
    4d00:   5f43474d    svcpl   0x0043474d
    4d04:   48746553    ldmdami r4!, {r0, r1, r4, r6, r8, sl, sp, lr}^
    4d08:   7553706e    ldrbvc  r7, [r3, #-110]
    4d0c:   726f7070    rsbvc   r7, pc, #112    ; 0x70
    4d10:   70724974    rsbsvc  r4, r2, r4, ror r9
    4d14:   706d6f43    rsbvc   r6, sp, r3, asr #30
    4d18:   6574656c    ldrbvs  r6, [r4, #-1388]!
    4d1c:   03cb0100    biceq   r0, fp, #0  ; 0x0
    4d20:   001f0001    andseq  r0, pc, r1
            4d21: R_ARM_ABS32   .text
    4d24:   001f2000    andseq  r2, pc, r0
            4d25: R_ARM_ABS32   .text
    4d28:   0002ca00    andeq   ip, r2, r0, lsl #20
            4d29: R_ARM_ABS32   .debug_loc
    4d2c:   004d5100    subeq   r5, sp, r0, lsl #2
    4d30:   000c2500    andeq   r2, ip, r0, lsl #10
            4d32: R_ARM_ABS32   .debug_str
    4d34:   ca010000    bgt 44d3c <MGC_DrcDumpPipe+0x40ae8>
    4d38:   0000f103    andeq   pc, r0, r3, lsl #2
    4d3c:   6c910200    lfmvs   f0, 4, [r1], {0}
    4d40:   72497026    subvc   r7, r9, #38 ; 0x26
    4d44:   ca010070    bgt 44f0c <MGC_DrcDumpPipe+0x40cb8>
    4d48:   004d5103    subeq   r5, sp, r3, lsl #2
    4d4c:   68910200    ldmvs   r1, {r9}
    4d50:   32040700    andcc   r0, r4, #0  ; 0x0
    4d54:   29000017    stmdbcs r0, {r0, r1, r2, r4}
    4d58:   5f43474d    svcpl   0x0043474d
    4d5c:   48746553    ldmdami r4!, {r0, r1, r4, r6, r8, sl, sp, lr}^
    4d60:   6e45706e    cdpvs   0, 4, cr7, cr5, cr14, {3}
    4d64:   656c6261    strbvs  r6, [ip, #-609]!
    4d68:   43707249    cmnmi   r0, #-1879048188    ; 0x90000004
    4d6c:   6c706d6f    ldclvs  13, cr6, [r0], #-444
    4d70:   00657465    rsbeq   r7, r5, r5, ror #8
    4d74:   0103d101    tsteq   r3, r1, lsl #2
    4d78:   00001f20    andeq   r1, r0, r0, lsr #30
            4d78: R_ARM_ABS32   .text
    4d7c:   00001fc4    andeq   r1, r0, r4, asr #31
            4d7c: R_ARM_ABS32   .text
    4d80:   000002f4    strdeq  r0, [r0], -r4
            4d80: R_ARM_ABS32   .debug_loc
    4d84:   00004dc6    andeq   r4, r0, r6, asr #27
    4d88:   00000c25    andeq   r0, r0, r5, lsr #24
            4d89: R_ARM_ABS32   .debug_str
    4d8c:   03d00100    bicseq  r0, r0, #0  ; 0x0
    4d90:   000000f1    strdeq  r0, [r0], -r1
    4d94:   26649102    strbtcs r9, [r4], -r2, lsl #2
    4d98:   70724970    rsbsvc  r4, r2, r0, ror r9
    4d9c:   03d00100    bicseq  r0, r0, #0  ; 0x0
    4da0:   00004d51    andeq   r4, r0, r1, asr sp
    4da4:   27609102    strbcs  r9, [r0, -r2, lsl #2]!
    4da8:   0000017a    andeq   r0, r0, sl, ror r1
            4da8: R_ARM_ABS32   .debug_str
    4dac:   2a03d201    bcs f95b8 <MGC_DrcDumpPipe+0xf5364>
    4db0:   02000044    andeq   r0, r0, #68 ; 0x44
    4db4:   d4276891    strtle  r6, [r7], #-2193
            4db7: R_ARM_ABS32   .debug_str
    4db8:   01000001    tsteq   r0, r1
    4dbc:   39f903d3    ldmibcc r9!, {r0, r1, r4, r6, r7, r8, r9}^
    4dc0:   91020000    tstls   r2, r0
    4dc4:   4d29006c    stcmi   0, cr0, [r9, #-432]!
    4dc8:   525f4347    subspl  r4, pc, #469762049  ; 0x1c000001
    4dcc:   766f6d65    strbtvc r6, [pc], -r5, ror #26
    4dd0:   73755365    cmnvc   r5, #-1811939327    ; 0x94000001
    4dd4:   646e6570    strbtvs r6, [lr], #-1392
    4dd8:   03e30100    mvneq   r0, #0  ; 0x0
    4ddc:   001fc401    andseq  ip, pc, r1, lsl #8
            4ddd: R_ARM_ABS32   .text
    4de0:   00201800    eoreq   r1, r0, r0, lsl #16
            4de1: R_ARM_ABS32   .text
    4de4:   00031e00    andeq   r1, r3, r0, lsl #28
            4de5: R_ARM_ABS32   .debug_loc
    4de8:   004e2a00    subeq   r2, lr, r0, lsl #20
    4dec:   007e2500    rsbseq  r2, lr, r0, lsl #10
            4dee: R_ARM_ABS32   .debug_str
    4df0:   e2010000    and r0, r1, #0  ; 0x0
    4df4:   0000f103    andeq   pc, r0, r3, lsl #2
    4df8:   64910200    ldrvs   r0, [r1], #512
    4dfc:   0000de25    andeq   sp, r0, r5, lsr #28
            4dfd: R_ARM_ABS32   .debug_str
    4e00:   03e20100    mvneq   r0, #0  ; 0x0
    4e04:   0000016f    andeq   r0, r0, pc, ror #2
    4e08:   27609102    strbcs  r9, [r0, -r2, lsl #2]!
    4e0c:   00000111    andeq   r0, r0, r1, lsl r1
            4e0c: R_ARM_ABS32   .debug_str
    4e10:   6703e401    strvs   lr, [r3, -r1, lsl #8]
    4e14:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4e18:   7a276891    bvc 9da24c <MGC_DrcDumpPipe+0x9d5ff8>
            4e1b: R_ARM_ABS32   .debug_str
    4e1c:   01000001    tsteq   r0, r1
    4e20:   442a03e5    strtmi  r0, [sl], #-997
    4e24:   91020000    tstls   r2, r0
    4e28:   012a006c    teqeq   sl, ip, rrx
    4e2c:   5f43474d    svcpl   0x0043474d
    4e30:   43637244    cmnmi   r3, #1073741828 ; 0x40000004
    4e34:   676e6168    strbvs  r6, [lr, -r8, ror #2]!
    4e38:   67744f65    ldrbvs  r4, [r4, -r5, ror #30]!
    4e3c:   74617453    strbtvc r7, [r1], #-1107
    4e40:   ef010065    svc 0x00010065
    4e44:   20180103    andscs  r0, r8, r3, lsl #2
            4e46: R_ARM_ABS32   .text
    4e48:   25bc0000    ldrcs   r0, [ip]!
            4e4a: R_ARM_ABS32   .text
    4e4c:   03480000    movteq  r0, #32768  ; 0x8000
            4e4e: R_ARM_ABS32   .debug_loc
    4e50:   4e950000    cdpmi   0, 9, cr0, cr5, cr0, {0}
    4e54:   74260000    strtvc  r0, [r6]
    4e58:   67744f6f    ldrbvs  r4, [r4, -pc, ror #30]!
    4e5c:   03ee0100    mvneq   r0, #0  ; 0x0
    4e60:   00000160    andeq   r0, r0, r0, ror #2
    4e64:   25649102    strbcs  r9, [r4, #-258]!
    4e68:   0000017a    andeq   r0, r0, sl, ror r1
            4e68: R_ARM_ABS32   .debug_str
    4e6c:   2a03ee01    bcs 100678 <MGC_DrcDumpPipe+0xfc424>
    4e70:   02000044    andeq   r0, r0, #68 ; 0x44
    4e74:   1b276091    blne    9d824c <MGC_DrcDumpPipe+0x9d3ff8>
            4e77: R_ARM_ABS32   .debug_str
    4e78:   01000000    tsteq   r0, r0
    4e7c:   4e9503f0    mrcmi   3, 4, r0, cr5, cr0, {7}
    4e80:   91020000    tstls   r2, r0
    4e84:   01d42768    bicseq  r2, r4, r8, ror #14
            4e86: R_ARM_ABS32   .debug_str
    4e88:   f1010000    setend  le
    4e8c:   0039f903    eorseq  pc, r9, r3, lsl #18
    4e90:   6c910200    lfmvs   f0, 4, [r1], {0}
    4e94:   9b040700    blls    106a9c <MGC_DrcDumpPipe+0x102848>
    4e98:   0800004e    stmdaeq r0, {r1, r2, r3, r6}
    4e9c:   0000104c    andeq   r1, r0, ip, asr #32
    4ea0:   474d012a    strbmi  r0, [sp, -sl, lsr #2]
    4ea4:   6f435f43    svcvs   0x00435f43
    4ea8:   656c706d    strbvs  r7, [ip, #-109]!
    4eac:   744f6574    strbvc  r6, [pc], #1396 ; 4eb4 <.debug_info+0x4eb4>
    4eb0:   61725467    cmnvs   r2, r7, ror #8
    4eb4:   7469736e    strbtvc r7, [r9], #-878
    4eb8:   006e6f69    rsbeq   r6, lr, r9, ror #30
    4ebc:   01046a01    tsteq   r4, r1, lsl #20
    4ec0:   000025bc    strheq  r2, [r0], -ip
            4ec0: R_ARM_ABS32   .text
    4ec4:   00002634    andeq   r2, r0, r4, lsr r6
            4ec4: R_ARM_ABS32   .text
    4ec8:   00000372    andeq   r0, r0, r2, ror r3
            4ec8: R_ARM_ABS32   .debug_loc
    4ecc:   00004eef    andeq   r4, r0, pc, ror #29
    4ed0:   00017a25    andeq   r7, r1, r5, lsr #20
            4ed1: R_ARM_ABS32   .debug_str
    4ed4:   04690100    strbteq r0, [r9], #-256
    4ed8:   0000442a    andeq   r4, r0, sl, lsr #8
    4edc:   27689102    strbcs  r9, [r8, -r2, lsl #2]!
    4ee0:   000000e5    andeq   r0, r0, r5, ror #1
            4ee0: R_ARM_ABS32   .debug_str
    4ee4:   6d046b01    vstrvs  d6, [r4, #-4]
    4ee8:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4eec:   29006c91    stmdbcs r0, {r0, r4, r7, sl, fp, sp, lr}
    4ef0:   5f43474d    svcpl   0x0043474d
    4ef4:   43637244    cmnmi   r3, #1073741828 ; 0x40000004
    4ef8:   6c706d6f    ldclvs  13, cr6, [r0], #-444
    4efc:   52657465    rsbpl   r7, r5, #1694498816 ; 0x65000000
    4f00:   6d757365    ldclvs  3, cr7, [r5, #-404]!
    4f04:   7c010065    stcvc   0, cr0, [r1], {101}
    4f08:   26340104    ldrtcs  r0, [r4], -r4, lsl #2
            4f0a: R_ARM_ABS32   .text
    4f0c:   27440000    strbcs  r0, [r4, -r0]
            4f0e: R_ARM_ABS32   .text
    4f10:   039c0000    orrseq  r0, ip, #0  ; 0x0
            4f12: R_ARM_ABS32   .debug_loc
    4f14:   4faa0000    svcmi   0x00aa0000
    4f18:   7e250000    cdpvc   0, 2, cr0, cr5, cr0, {0}
            4f1b: R_ARM_ABS32   .debug_str
    4f1c:   01000000    tsteq   r0, r0
    4f20:   00f1047b    rscseq  r0, r1, fp, ror r4
    4f24:   91020000    tstls   r2, r0
    4f28:   00de2554    sbcseq  r2, lr, r4, asr r5
            4f2a: R_ARM_ABS32   .debug_str
    4f2c:   7b010000    blvc    44f34 <MGC_DrcDumpPipe+0x40ce0>
    4f30:   00016f04    andeq   r6, r1, r4, lsl #30
    4f34:   50910200    addspl  r0, r1, r0, lsl #4
    4f38:   00006727    andeq   r6, r0, r7, lsr #14
            4f39: R_ARM_ABS32   .debug_str
    4f3c:   047d0100    ldrbteq r0, [sp], #-256
    4f40:   0000016f    andeq   r0, r0, pc, ror #2
    4f44:   27589102    ldrbcs  r9, [r8, -r2, lsl #2]
    4f48:   000000ba    strheq  r0, [r0], -sl
            4f48: R_ARM_ABS32   .debug_str
    4f4c:   6f047e01    svcvs   0x00047e01
    4f50:   02000001    andeq   r0, r0, #1  ; 0x1
    4f54:   80275a91    mlahi   r7, r1, sl, r5
            4f57: R_ARM_ABS32   .debug_str
    4f58:   01000001    tsteq   r0, r1
    4f5c:   1ca0047f    cfstrsne    mvf0, [r0], #508
    4f60:   91020000    tstls   r2, r0
    4f64:   4970285c    ldmdbmi r0!, {r2, r3, r4, r6, fp, sp}^
    4f68:   446c706d    strbtmi r7, [ip], #-109
    4f6c:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    4f70:   80010065    andhi   r0, r1, r5, rrx
    4f74:   004faa04    subeq   sl, pc, r4, lsl #20
    4f78:   60910200    addsvs  r0, r1, r0, lsl #4
    4f7c:   0001a927    andeq   sl, r1, r7, lsr #18
            4f7d: R_ARM_ABS32   .debug_str
    4f80:   04810100    streq   r0, [r1], #256
    4f84:   00001ec4    andeq   r1, r0, r4, asr #29
    4f88:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    4f8c:   00000111    andeq   r0, r0, r1, lsl r1
            4f8c: R_ARM_ABS32   .debug_str
    4f90:   67048201    strvs   r8, [r4, -r1, lsl #4]
    4f94:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4f98:   7a276891    bvc 9da24c <MGC_DrcDumpPipe+0x9d5ff8>
            4f9b: R_ARM_ABS32   .debug_str
    4f9c:   01000001    tsteq   r0, r1
    4fa0:   442a0483    strtmi  r0, [sl], #-1155
    4fa4:   91020000    tstls   r2, r0
    4fa8:   0407006c    streq   r0, [r7], #-108
    4fac:   0000369e    muleq   r0, lr, r6
    4fb0:   474d0124    strbmi  r0, [sp, -r4, lsr #2]
    4fb4:   72445f43    subvc   r5, r4, #268    ; 0x10c
    4fb8:   62735563    rsbsvs  r5, r3, #415236096  ; 0x18c00000
    4fbc:   00727349    rsbseq  r7, r2, r9, asr #6
    4fc0:   0104a401    tsteq   r4, r1, lsl #8
    4fc4:   00000160    andeq   r0, r0, r0, ror #2
    4fc8:   00002744    andeq   r2, r0, r4, asr #14
            4fc8: R_ARM_ABS32   .text
    4fcc:   00002c1c    andeq   r2, r0, ip, lsl ip
            4fcc: R_ARM_ABS32   .text
    4fd0:   000003c6    andeq   r0, r0, r6, asr #7
            4fd0: R_ARM_ABS32   .debug_loc
    4fd4:   0000508c    andeq   r5, r0, ip, lsl #1
    4fd8:   00017a25    andeq   r7, r1, r5, lsr #20
            4fd9: R_ARM_ABS32   .debug_str
    4fdc:   04a30100    strteq  r0, [r3], #256
    4fe0:   0000442a    andeq   r4, r0, sl, lsr #8
    4fe4:   26449102    strbcs  r9, [r4], -r2, lsl #2
    4fe8:   746e4962    strbtvc r4, [lr], #-2402
    4fec:   62735572    rsbsvs  r5, r3, #478150656  ; 0x1c800000
    4ff0:   006c6156    rsbeq   r6, ip, r6, asr r1
    4ff4:   6004a301    andvs   sl, r4, r1, lsl #6
    4ff8:   02000001    andeq   r0, r0, #1  ; 0x1
    4ffc:   71284091    strbvc  r4, [r8, -r1]!
    5000:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
    5004:   04a50100    strteq  r0, [r5], #256
    5008:   00002861    andeq   r2, r0, r1, ror #16
    500c:   27489102    strbcs  r9, [r8, -r2, lsl #2]
    5010:   000000ca    andeq   r0, r0, sl, asr #1
            5010: R_ARM_ABS32   .debug_str
    5014:   8e04a601    cfmadd32hi  mvax0, mvfx10, mvfx4, mvfx1
    5018:   02000001    andeq   r0, r0, #1  ; 0x1
    501c:   de275091    mcrle   0, 1, r5, cr7, cr1, {4}
            501f: R_ARM_ABS32   .debug_str
    5020:   01000001    tsteq   r0, r1
    5024:   018e04a7    orreq   r0, lr, r7, lsr #9
    5028:   91020000    tstls   r2, r0
    502c:   45702854    ldrbmi  r2, [r0, #-2132]!
    5030:   0100646e    tsteq   r0, lr, ror #8
    5034:   2ca404a8    cfstrscs    mvf0, [r4], #672
    5038:   91020000    tstls   r2, r0
    503c:   00502758    subseq  r2, r0, r8, asr r7
            503e: R_ARM_ABS32   .debug_str
    5040:   a9010000    stmdbge r1, {}
    5044:   00508c04    subseq  r8, r0, r4, lsl #24
    5048:   5c910200    lfmpl   f0, 4, [r1], {0}
    504c:   65526228    ldrbvs  r6, [r2, #-552]
    5050:   746c7573    strbtvc r7, [ip], #-1395
    5054:   04aa0100    strteq  r0, [sl], #256
    5058:   00000160    andeq   r0, r0, r0, ror #2
    505c:   27639102    strbcs  r9, [r3, -r2, lsl #2]!
    5060:   000001d4    ldrdeq  r0, [r0], -r4
            5060: R_ARM_ABS32   .debug_str
    5064:   f904ab01    undefined instruction 0xf904ab01
    5068:   02000039    andeq   r0, r0, #57 ; 0x39
    506c:   6c2c6491    cfstrsvs    mvf6, [ip], #-580
            506f: R_ARM_ABS32   .text
    5070:   e0000029    and r0, r0, r9, lsr #32
            5073: R_ARM_ABS32   .text
    5074:   2800002a    stmdacs r0, {r1, r3, r5}
    5078:   48736177    ldmdami r3!, {r0, r1, r2, r4, r5, r6, r8, sp, lr}^
    507c:   0074736f    rsbseq  r7, r4, pc, ror #6
    5080:   6004e201    andvs   lr, r4, r1, lsl #4
    5084:   02000001    andeq   r0, r0, #1  ; 0x1
    5088:   00006b91    muleq   r0, r1, fp
    508c:   19a60407    stmibne r6!, {r0, r1, r2, sl}
    5090:   012a0000    teqeq   sl, r0
    5094:   5f43474d    svcpl   0x0043474d
    5098:   42637244    rsbmi   r7, r3, #1073741828 ; 0x40000004
    509c:   01007273    tsteq   r0, r3, ror r2
    50a0:   1c01053b    cfstr32ne   mvfx0, [r1], {59}
            50a3: R_ARM_ABS32   .text
    50a4:   8800002c    stmdahi r0, {r2, r3, r5}
            50a7: R_ARM_ABS32   .text
    50a8:   f0000036    undefined instruction 0xf0000036
            50ab: R_ARM_ABS32   .debug_loc
    50ac:   c3000003    movwgt  r0, #3  ; 0x3
    50b0:   25000051    strcs   r0, [r0, #-81]
    50b4:   0000007e    andeq   r0, r0, lr, ror r0
            50b4: R_ARM_ABS32   .debug_str
    50b8:   f1053a01    undefined instruction 0xf1053a01
    50bc:   03000000    movweq  r0, #0  ; 0x0
    50c0:   287f9091    ldmdacs pc!, {r0, r4, r7, ip, pc}^
    50c4:   6d657469    cfstrdvs    mvd7, [r5, #-420]!
    50c8:   05400100    strbeq  r0, [r0, #-256]
    50cc:   00002861    andeq   r2, r0, r1, ror #16
    50d0:   7fbc9103    svcvc   0x00bc9103
    50d4:   6b4f6228    blvs    13dd97c <MGC_DrcDumpPipe+0x13d9728>
    50d8:   05410100    strbeq  r0, [r1, #-256]
    50dc:   00000160    andeq   r0, r0, r0, ror #2
    50e0:   27469102    strbcs  r9, [r6, -r2, lsl #2]
    50e4:   0000005a    andeq   r0, r0, sl, asr r0
            50e4: R_ARM_ABS32   .debug_str
    50e8:   60054201    andvs   r4, r5, r1, lsl #4
    50ec:   02000001    andeq   r0, r0, #1  ; 0x1
    50f0:   85274791    strhi   r4, [r7, #-1937]!
            50f3: R_ARM_ABS32   .debug_str
    50f4:   01000000    tsteq   r0, r0
    50f8:   2ca40543    cfstr32cs   mvfx0, [r4], #268
    50fc:   91020000    tstls   r2, r0
    5100:   49702848    ldmdbmi r0!, {r3, r6, fp, sp}^
    5104:   01007072    tsteq   r0, r2, ror r0
    5108:   51c30544    bicpl   r0, r3, r4, asr #10
    510c:   91020000    tstls   r2, r0
    5110:   4370284c    cmnmi   r0, #4980736    ; 0x4c0000
    5114:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    5118:   72496c6f    subvc   r6, r9, #28416  ; 0x6f00
    511c:   45010070    strmi   r0, [r1, #-112]
    5120:   004d5105    subeq   r5, sp, r5, lsl #2
    5124:   50910200    addspl  r0, r1, r0, lsl #4
    5128:   00005027    andeq   r5, r0, r7, lsr #32
            5129: R_ARM_ABS32   .debug_str
    512c:   05460100    strbeq  r0, [r6, #-256]
    5130:   0000508c    andeq   r5, r0, ip, lsl #1
    5134:   28549102    ldmdacs r4, {r1, r8, ip, pc}^
    5138:   61664970    smcvs   25744
    513c:   6f506563    svcvs   0x00506563
    5140:   01007472    tsteq   r0, r2, ror r4
    5144:   14d10547    ldrbne  r0, [r1], #1351
    5148:   91020000    tstls   r2, r0
    514c:   017a2758    cmneq   sl, r8, asr r7
            514e: R_ARM_ABS32   .debug_str
    5150:   48010000    stmdami r1, {}
    5154:   00442a05    subeq   r2, r4, r5, lsl #20
    5158:   5c910200    lfmpl   f0, 4, [r1], {0}
    515c:   00011127    andeq   r1, r1, r7, lsr #2
            515d: R_ARM_ABS32   .debug_str
    5160:   05490100    strbeq  r0, [r9, #-256]
    5164:   00003f67    andeq   r3, r0, r7, ror #30
    5168:   27609102    strbcs  r9, [r0, -r2, lsl #2]!
    516c:   000001d4    ldrdeq  r0, [r0], -r4
            516c: R_ARM_ABS32   .debug_str
    5170:   f9054a01    undefined instruction 0xf9054a01
    5174:   02000039    andeq   r0, r0, #57 ; 0x39
    5178:   e5276491    str r6, [r7, #-1169]!
            517b: R_ARM_ABS32   .debug_str
    517c:   01000000    tsteq   r0, r0
    5180:   3f6d054b    svccc   0x006d054b
    5184:   91020000    tstls   r2, r0
    5188:   6e632868    cdpvs   8, 6, cr2, cr3, cr8, {3}
    518c:   4d010074    stcmi   0, cr0, [r1, #-464]
    5190:   00016005    andeq   r6, r1, r5
    5194:   6e910200    cdpvs   2, 9, cr0, cr1, cr0, {0}
    5198:   00696928    rsbeq   r6, r9, r8, lsr #18
    519c:   60054d01    andvs   r4, r5, r1, lsl #26
    51a0:   02000001    andeq   r0, r0, #1  ; 0x1
    51a4:   64286f91    strtvs  r6, [r8], #-3985
    51a8:   725f616d    subsvc  r6, pc, #1073741851 ; 0x4000001b
    51ac:   65757165    ldrbvs  r7, [r5, #-357]!
    51b0:   615f7473    cmpvs   pc, r3, ror r4
    51b4:   01007272    tsteq   r0, r2, ror r2
    51b8:   51c9054e    bicpl   r0, r9, lr, asr #10
    51bc:   91030000    tstls   r3, r0
    51c0:   07007f94    undefined
    51c4:   00184704    andseq  r4, r8, r4, lsl #14
    51c8:   07c41c00    strbeq  r1, [r4, r0, lsl #24]
    51cc:   51d90000    bicspl  r0, r9, r0
    51d0:   dd1d0000    ldcle   0, cr0, [sp]
    51d4:   09000000    stmdbeq r0, {}
    51d8:   4d012400    cfstrsmi    mvf2, [r1]
    51dc:   445f4347    ldrbmi  r4, [pc], #839  ; 51e4 <.debug_info+0x51e4>
    51e0:   65536372    ldrbvs  r6, [r3, #-882]
    51e4:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    51e8:   736f4865    cmnvc   pc, #6619136    ; 0x650000
    51ec:   66654474    undefined
    51f0:   746c7561    strbtvc r7, [ip], #-1377
    51f4:   065f0100    ldrbeq  r0, [pc], -r0, lsl #2
    51f8:   00016001    andeq   r6, r1, r1
    51fc:   00368800    eorseq  r8, r6, r0, lsl #16
            51fd: R_ARM_ABS32   .text
    5200:   003b6c00    eorseq  r6, fp, r0, lsl #24
            5201: R_ARM_ABS32   .text
    5204:   00041a00    andeq   r1, r4, r0, lsl #20
            5205: R_ARM_ABS32   .debug_loc
    5208:   0052ad00    subseq  sl, r2, r0, lsl #26
    520c:   017a2500    cmneq   sl, r0, lsl #10
            520e: R_ARM_ABS32   .debug_str
    5210:   5d010000    stcpl   0, cr0, [r1]
    5214:   00442a06    subeq   r2, r4, r6, lsl #20
    5218:   4c910200    lfmmi   f0, 4, [r1], {0}
    521c:   00006725    andeq   r6, r0, r5, lsr #14
            521d: R_ARM_ABS32   .debug_str
    5220:   065d0100    ldrbeq  r0, [sp], -r0, lsl #2
    5224:   0000016f    andeq   r0, r0, pc, ror #2
    5228:   25489102    strbcs  r9, [r8, #-258]
    522c:   000001be    strheq  r0, [r0], -lr
            522c: R_ARM_ABS32   .debug_str
    5230:   60065e01    andvs   r5, r6, r1, lsl #28
    5234:   02000001    andeq   r0, r0, #1  ; 0x1
    5238:   70264491    mlavc   r6, r1, r4, r4
    523c:   72734362    rsbsvc  r4, r3, #-2013265919    ; 0x88000001
    5240:   006c6156    rsbeq   r6, ip, r6, asr r1
    5244:   e3065e01    movw    r5, #28161  ; 0x6e01
    5248:   02000014    andeq   r0, r0, #20 ; 0x14
    524c:   62284091    eorvs   r4, r8, #145    ; 0x91
    5250:   65726f4d    ldrbvs  r6, [r2, #-3917]!
    5254:   06600100    strbteq r0, [r0], -r0, lsl #2
    5258:   00000160    andeq   r0, r0, r0, ror #2
    525c:   27539102    ldrbcs  r9, [r3, -r2, lsl #2]
    5260:   000000fa    strdeq  r0, [r0], -sl
            5260: R_ARM_ABS32   .debug_str
    5264:   e3066101    movw    r6, #24833  ; 0x6101
    5268:   02000014    andeq   r0, r0, #20 ; 0x14
    526c:   65275491    strvs   r5, [r7, #-1169]!
            526f: R_ARM_ABS32   .debug_str
    5270:   01000001    tsteq   r0, r1
    5274:   016f0662    cmneq   pc, r2, ror #12
    5278:   91020000    tstls   r2, r0
    527c:   4570285a    ldrbmi  r2, [r0, #-2138]!
    5280:   0100646e    tsteq   r0, lr, ror #8
    5284:   2ca40663    stccs   6, cr0, [r4], #396
    5288:   91020000    tstls   r2, r0
    528c:   4970285c    ldmdbmi r0!, {r2, r3, r4, r6, fp, sp}^
    5290:   01007072    tsteq   r0, r2, ror r0
    5294:   4d510664    ldclmi  6, cr0, [r1, #-400]
    5298:   91020000    tstls   r2, r0
    529c:   00c12760    sbceq   r2, r1, r0, ror #14
            529e: R_ARM_ABS32   .debug_str
    52a0:   65010000    strvs   r0, [r1]
    52a4:   0047bf06    subeq   fp, r7, r6, lsl #30
    52a8:   64910200    ldrvs   r0, [r1], #512
    52ac:   4d012400    cfstrsmi    mvf2, [r1]
    52b0:   445f4347    ldrbmi  r4, [pc], #839  ; 52b8 <.debug_info+0x52b8>
    52b4:   63416372    movtvs  r6, #4978   ; 0x1372
    52b8:   74706563    ldrbtvc r6, [r0], #-1379
    52bc:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    52c0:   01006563    tsteq   r0, r3, ror #10
    52c4:   600106d2    ldrdvs  r0, [r1], -r2
    52c8:   6c000001    stcvs   0, cr0, [r0], {1}
            52cb: R_ARM_ABS32   .text
    52cc:   b800003b    stmdalt r0, {r0, r1, r3, r4, r5}
            52cf: R_ARM_ABS32   .text
    52d0:   4400003d    strmi   r0, [r0], #-61
            52d3: R_ARM_ABS32   .debug_loc
    52d4:   36000004    strcc   r0, [r0], -r4
    52d8:   25000053    strcs   r0, [r0, #-83]
    52dc:   0000017a    andeq   r0, r0, sl, ror r1
            52dc: R_ARM_ABS32   .debug_str
    52e0:   2a06d001    bcs 1b92ec <MGC_DrcDumpPipe+0x1b5098>
    52e4:   02000044    andeq   r0, r0, #68 ; 0x44
    52e8:   80256091    mlahi   r5, r1, r0, r6
            52eb: R_ARM_ABS32   .debug_str
    52ec:   01000001    tsteq   r0, r1
    52f0:   1ca006d0    stcne   6, cr0, [r0], #832
    52f4:   91020000    tstls   r2, r0
    52f8:   01a9255c    undefined instruction 0x01a9255c
            52fa: R_ARM_ABS32   .debug_str
    52fc:   d1010000    tstle   r1, r0
    5300:   001ec406    andseq  ip, lr, r6, lsl #8
    5304:   58910200    ldmpl   r1, {r9}
    5308:   00001b27    andeq   r1, r0, r7, lsr #22
            5309: R_ARM_ABS32   .debug_str
    530c:   06d30100    ldrbeq  r0, [r3], r0, lsl #2
    5310:   00004e95    muleq   r0, r5, lr
    5314:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    5318:   006b4f62    rsbeq   r4, fp, r2, ror #30
    531c:   6006d401    andvs   sp, r6, r1, lsl #8
    5320:   02000001    andeq   r0, r0, #1  ; 0x1
    5324:   e5276b91    str r6, [r7, #-2961]!
            5327: R_ARM_ABS32   .debug_str
    5328:   01000000    tsteq   r0, r0
    532c:   3f6d06d5    svccc   0x006d06d5
    5330:   91020000    tstls   r2, r0
    5334:   012a006c    teqeq   sl, ip, rrx
    5338:   5f43474d    svcpl   0x0043474d
    533c:   46637244    strbtmi r7, [r3], -r4, asr #4
    5340:   73696e69    cmnvc   r9, #1680   ; 0x690
    5344:   73655268    cmnvc   r5, #-2147483642    ; 0x80000006
    5348:   00656d75    rsbeq   r6, r5, r5, ror sp
    534c:   01070e01    tsteq   r7, r1, lsl #28
    5350:   00003db8    strheq  r3, [r0], -r8
            5350: R_ARM_ABS32   .text
    5354:   00003e44    andeq   r3, r0, r4, asr #28
            5354: R_ARM_ABS32   .text
    5358:   0000046e    andeq   r0, r0, lr, ror #8
            5358: R_ARM_ABS32   .debug_loc
    535c:   00005385    andeq   r5, r0, r5, lsl #7
    5360:   00017a25    andeq   r7, r1, r5, lsr #20
            5361: R_ARM_ABS32   .debug_str
    5364:   070d0100    streq   r0, [sp, -r0, lsl #2]
    5368:   0000442a    andeq   r4, r0, sl, lsr #8
    536c:   28689102    stmdacs r8!, {r1, r8, ip, pc}^
    5370:   69547764    ldmdbvs r4, {r2, r5, r6, r8, r9, sl, ip, sp, lr}^
    5374:   756f656d    strbvc  r6, [pc, #-1389]!   ; 4e0f <.debug_info+0x4e0f>
    5378:   0f010074    svceq   0x00010074
    537c:   00018e07    andeq   r8, r1, r7, lsl #28
    5380:   6c910200    lfmvs   f0, 4, [r1], {0}
    5384:   4d012400    cfstrsmi    mvf2, [r1]
    5388:   445f4347    ldrbmi  r4, [pc], #839  ; 5390 <.debug_info+0x5390>
    538c:   65536372    ldrbvs  r6, [r3, #-882]
    5390:   736f4874    cmnvc   pc, #7602176    ; 0x740000
    5394:   776f5074    undefined
    5398:   01007265    tsteq   r0, r5, ror #4
    539c:   8e010721    cdphi   7, 0, cr0, cr1, cr1, {1}
    53a0:   44000001    strmi   r0, [r0], #-1
            53a3: R_ARM_ABS32   .text
    53a4:   d800003e    stmdale r0, {r1, r2, r3, r4, r5}
            53a7: R_ARM_ABS32   .text
    53a8:   9800003e    stmdals r0, {r1, r2, r3, r4, r5}
            53ab: R_ARM_ABS32   .debug_loc
    53ac:   f6000004    undefined instruction 0xf6000004
    53b0:   25000053    strcs   r0, [r0, #-83]
    53b4:   00000111    andeq   r0, r0, r1, lsl r1
            53b4: R_ARM_ABS32   .debug_str
    53b8:   67071f01    strvs   r1, [r7, -r1, lsl #30]
    53bc:   0200003f    andeq   r0, r0, #63 ; 0x3f
    53c0:   77266491    undefined
    53c4:   65776f50    ldrbvs  r6, [r7, #-3920]!
    53c8:   20010072    andcs   r0, r1, r2, ror r0
    53cc:   00016f07    andeq   r6, r1, r7, lsl #30
    53d0:   60910200    addsvs  r0, r1, r0, lsl #4
    53d4:   73657228    cmnvc   r5, #-2147483646    ; 0x80000002
    53d8:   00746c75    rsbseq  r6, r4, r5, ror ip
    53dc:   8e072201    cdphi   2, 0, cr2, cr7, cr1, {0}
    53e0:   02000001    andeq   r0, r0, #1  ; 0x1
    53e4:   7a276891    bvc 9da24c <MGC_DrcDumpPipe+0x9d5ff8>
            53e7: R_ARM_ABS32   .debug_str
    53e8:   01000001    tsteq   r0, r1
    53ec:   442a0723    strtmi  r0, [sl], #-1827
    53f0:   91020000    tstls   r2, r0
    53f4:   4d29006c    stcmi   0, cr0, [r9, #-432]!
    53f8:   4f5f4347    svcmi   0x005f4347
    53fc:   74536774    ldrbvc  r6, [r3], #-1908
    5400:   47657461    strbmi  r7, [r5, -r1, ror #8]!
    5404:   64497465    strbvs  r7, [r9], #-1125
    5408:   07370100    ldreq   r0, [r7, -r0, lsl #2]!
    540c:   003ed801    eorseq  sp, lr, r1, lsl #16
            540d: R_ARM_ABS32   .text
    5410:   003f9800    eorseq  r9, pc, r0, lsl #16
            5411: R_ARM_ABS32   .text
    5414:   0004c200    andeq   ip, r4, r0, lsl #4
            5415: R_ARM_ABS32   .debug_loc
    5418:   00543c00    subseq  r3, r4, r0, lsl #24
    541c:   017a2500    cmneq   sl, r0, lsl #10
            541e: R_ARM_ABS32   .debug_str
    5420:   36010000    strcc   r0, [r1], -r0
    5424:   00442a07    subeq   r2, r4, r7, lsl #20
    5428:   6c910200    lfmvs   f0, 4, [r1], {0}
    542c:   00013d25    andeq   r3, r1, r5, lsr #26
            542d: R_ARM_ABS32   .debug_str
    5430:   07360100    ldreq   r0, [r6, -r0, lsl #2]!
    5434:   00000160    andeq   r0, r0, r0, ror #2
    5438:   00689102    rsbeq   r9, r8, r2, lsl #2
    543c:   554d0124    strbpl  r0, [sp, #-292]
    5440:   525f4253    subspl  r4, pc, #805306373  ; 0x30000005
    5444:   73696765    cmnvc   r9, #26476544   ; 0x1940000
    5448:   4f726574    svcmi   0x00726574
    544c:   6c436774    mcrrvs  7, 7, r6, r3, cr4
    5450:   746e6569    strbtvc r6, [lr], #-1385
    5454:   07620100    strbeq  r0, [r2, -r0, lsl #2]!
    5458:   00150301    andseq  r0, r5, r1, lsl #6
    545c:   003f9800    eorseq  r9, pc, r0, lsl #16
            545d: R_ARM_ABS32   .text
    5460:   0040fc00    subeq   pc, r0, r0, lsl #24
            5461: R_ARM_ABS32   .text
    5464:   0004ec00    andeq   lr, r4, r0, lsl #24
            5465: R_ARM_ABS32   .debug_loc
    5468:   0054f000    subseq  pc, r4, r0
    546c:   017a2500    cmneq   sl, r0, lsl #10
            546e: R_ARM_ABS32   .debug_str
    5470:   5e010000    cdppl   0, 0, cr0, cr1, cr0, {0}
    5474:   0014d107    andseq  sp, r4, r7, lsl #2
    5478:   5c910200    lfmpl   f0, 4, [r1], {0}
    547c:   00006e25    andeq   r6, r0, r5, lsr #28
            547d: R_ARM_ABS32   .debug_str
    5480:   075f0100    ldrbeq  r0, [pc, -r0, lsl #2]
    5484:   00003f79    andeq   r3, r0, r9, ror pc
    5488:   25589102    ldrbcs  r9, [r8, #-258]
    548c:   00000000    andeq   r0, r0, r0
            548c: R_ARM_ABS32   .debug_str
    5490:   73076001    movwvc  r6, #28673  ; 0x7001
    5494:   0200003f    andeq   r0, r0, #63 ; 0x3f
    5498:   e5255491    str r5, [r5, #-1169]!
            549b: R_ARM_ABS32   .debug_str
    549c:   01000000    tsteq   r0, r0
    54a0:   3f6d0761    svccc   0x006d0761
    54a4:   91020000    tstls   r2, r0
    54a8:   001b2750    andseq  r2, fp, r0, asr r7
            54aa: R_ARM_ABS32   .debug_str
    54ac:   63010000    movwvs  r0, #4096   ; 0x1000
    54b0:   0054f007    subseq  pc, r4, r7
    54b4:   60910200    addsvs  r0, r1, r0, lsl #4
    54b8:   00003027    andeq   r3, r0, r7, lsr #32
            54b9: R_ARM_ABS32   .debug_str
    54bc:   07640100    strbeq  r0, [r4, -r0, lsl #2]!
    54c0:   0000018e    andeq   r0, r0, lr, lsl #3
    54c4:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    54c8:   73655270    cmnvc   r5, #7  ; 0x7
    54cc:   00746c75    rsbseq  r6, r4, r5, ror ip
    54d0:   f1076501    undefined instruction 0xf1076501
    54d4:   02000000    andeq   r0, r0, #0  ; 0x0
    54d8:   70286891    mlavc   r8, r1, r8, r6
    54dc:   6c706d49    ldclvs  13, cr6, [r0], #-292
    54e0:   74726f50    ldrbtvc r6, [r2], #-3920
    54e4:   07660100    strbeq  r0, [r6, -r0, lsl #2]!
    54e8:   0000442a    andeq   r4, r0, sl, lsr #8
    54ec:   006c9102    rsbeq   r9, ip, r2, lsl #2
    54f0:   104c0407    subne   r0, ip, r7, lsl #8
    54f4:   012a0000    teqeq   sl, r0
    54f8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    54fc:   6c65525f    sfmvs   f5, 2, [r5], #-380
    5500:   75716e69    ldrbvc  r6, [r1, #-3689]!
    5504:   48687369    stmdami r8!, {r0, r3, r5, r6, r8, r9, ip, sp, lr}^
    5508:   0074736f    rsbseq  r7, r4, pc, ror #6
    550c:   01079b01    tsteq   r7, r1, lsl #22
    5510:   000040fc    strdeq  r4, [r0], -ip
            5510: R_ARM_ABS32   .text
    5514:   000041cc    andeq   r4, r0, ip, asr #3
            5514: R_ARM_ABS32   .text
    5518:   00000516    andeq   r0, r0, r6, lsl r5
            5518: R_ARM_ABS32   .debug_loc
    551c:   0000555e    andeq   r5, r0, lr, asr r5
    5520:   75426826    strbvc  r6, [r2, #-2086]
    5524:   9a010073    bls 456f8 <MGC_DrcDumpPipe+0x414a4>
    5528:   00150307    andseq  r0, r5, r7, lsl #6
    552c:   60910200    addsvs  r0, r1, r0, lsl #4
    5530:   00017a27    andeq   r7, r1, r7, lsr #20
            5531: R_ARM_ABS32   .debug_str
    5534:   079c0100    ldreq   r0, [ip, r0, lsl #2]
    5538:   0000442a    andeq   r4, r0, sl, lsr #8
    553c:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    5540:   00000111    andeq   r0, r0, r1, lsl r1
            5540: R_ARM_ABS32   .debug_str
    5544:   67079d01    strvs   r9, [r7, -r1, lsl #26]
    5548:   0200003f    andeq   r0, r0, #63 ; 0x3f
    554c:   d4276891    strtle  r6, [r7], #-2193
            554f: R_ARM_ABS32   .debug_str
    5550:   01000001    tsteq   r0, r1
    5554:   39f9079e    ldmibcc r9!, {r1, r2, r3, r4, r7, r8, r9, sl}^
    5558:   91020000    tstls   r2, r0
    555c:   0124006c    teqeq   r4, ip, rrx
    5560:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    5564:   7165525f    cmnvc   r5, pc, asr r2
    5568:   74736575    ldrbtvc r6, [r3], #-1397
    556c:   00737542    rsbseq  r7, r3, r2, asr #10
    5570:   0107b401    tsteq   r7, r1, lsl #8
    5574:   00000160    andeq   r0, r0, r0, ror #2
    5578:   000041cc    andeq   r4, r0, ip, asr #3
            5578: R_ARM_ABS32   .text
    557c:   00004254    andeq   r4, r0, r4, asr r2
            557c: R_ARM_ABS32   .text
    5580:   00000540    andeq   r0, r0, r0, asr #10
            5580: R_ARM_ABS32   .debug_loc
    5584:   000055a8    andeq   r5, r0, r8, lsr #11
    5588:   75426826    strbvc  r6, [r2, #-2086]
    558c:   b3010073    movwlt  r0, #4211   ; 0x1073
    5590:   00150307    andseq  r0, r5, r7, lsl #6
    5594:   68910200    ldmvs   r1, {r9}
    5598:   00017a27    andeq   r7, r1, r7, lsr #20
            5599: R_ARM_ABS32   .debug_str
    559c:   07b50100    ldreq   r0, [r5, r0, lsl #2]!
    55a0:   0000442a    andeq   r4, r0, sl, lsr #8
    55a4:   006c9102    rsbeq   r9, ip, r2, lsl #2
    55a8:   474d0124    strbmi  r0, [sp, -r4, lsr #2]
    55ac:   72445f43    subvc   r5, r4, #268    ; 0x10c
    55b0:   6d754463    cfldrdvs    mvd4, [r5, #-396]!
    55b4:   70695070    rsbvc   r5, r9, r0, ror r0
    55b8:   cb010065    blgt    45754 <MGC_DrcDumpPipe+0x41500>
    55bc:   00a90107    adceq   r0, r9, r7, lsl #2
    55c0:   42540000    subsmi  r0, r4, #0  ; 0x0
            55c2: R_ARM_ABS32   .text
    55c4:   427c0000    rsbsmi  r0, ip, #0  ; 0x0
            55c6: R_ARM_ABS32   .text
    55c8:   056a0000    strbeq  r0, [sl]!
            55ca: R_ARM_ABS32   .debug_loc
    55cc:   55f30000    ldrbpl  r0, [r3]!
    55d0:   11250000    teqne   r5, r0
            55d3: R_ARM_ABS32   .debug_str
    55d4:   01000001    tsteq   r0, r1
    55d8:   3f6707ca    svccc   0x006707ca
    55dc:   91020000    tstls   r2, r0
    55e0:   5070266c    rsbspl  r2, r0, ip, ror #12
    55e4:   00657069    rsbeq   r7, r5, r9, rrx
    55e8:   ac07ca01    stcge   10, cr12, [r7], {1}
    55ec:   02000039    andeq   r0, r0, #57 ; 0x39
    55f0:   1c006891    stcne   8, cr6, [r0], {145}
    55f4:   00000160    andeq   r0, r0, r0, ror #2
    55f8:   00005603    andeq   r5, r0, r3, lsl #12
    55fc:   0000dd1d    andeq   sp, r0, sp, lsl sp
    5600:   21000700    tstcs   r0, r0, lsl #14
    5604:   5f43474d    svcpl   0x0043474d
    5608:   74655361    strbtvc r5, [r5], #-865
    560c:   53706e48    cmnpl   r0, #1152   ; 0x480
    5610:   6f707075    svcvs   0x00707075
    5614:   61447472    cmpvs   r4, r2, ror r4
    5618:   01006174    tsteq   r0, r4, ror r1
    561c:   0056273a    subseq  r2, r6, sl, lsr r7
    5620:   00030500    andeq   r0, r3, r0, lsl #10
            5623: R_ARM_ABS32   .rodata
    5624:   08000000    stmdaeq r0, {}
    5628:   000055f3    strdeq  r5, [r0], -r3
    562c:   0001601c    andeq   r6, r1, ip, lsl r0
    5630:   00563c00    subseq  r3, r6, r0, lsl #24
    5634:   00dd1d00    sbcseq  r1, sp, r0, lsl #26
    5638:   00070000    andeq   r0, r7, r0
    563c:   43474d21    movtmi  r4, #32033  ; 0x7d21
    5640:   6553615f    ldrbvs  r6, [r3, #-351]
    5644:   706e4874    rsbvc   r4, lr, r4, ror r8
    5648:   62616e45    rsbvs   r6, r1, #1104   ; 0x450
    564c:   6144656c    cmpvs   r4, ip, ror #10
    5650:   01006174    tsteq   r0, r4, ror r1
    5654:   00565f44    subseq  r5, r6, r4, asr #30
    5658:   08030500    stmdaeq r3, {r8, sl}
            565b: R_ARM_ABS32   .rodata
    565c:   08000000    stmdaeq r0, {}
    5660:   0000562c    andeq   r5, r0, ip, lsr #12
    5664:   43474d21    movtmi  r4, #32033  ; 0x7d21
    5668:   7465535f    strbtvc r5, [r5], #-863
    566c:   53706e48    cmnpl   r0, #1152   ; 0x480
    5670:   6f707075    svcvs   0x00707075
    5674:   72497472    subvc   r7, r9, #1912602624 ; 0x72000000
    5678:   52010070    andpl   r0, r1, #112    ; 0x70
    567c:   00001732    andeq   r1, r0, r2, lsr r7
    5680:   00000305    andeq   r0, r0, r5, lsl #6
            5682: R_ARM_ABS32   .data
    5684:   4d210000    stcmi   0, cr0, [r1]
    5688:   535f4347    cmppl   pc, #469762049  ; 0x1c000001
    568c:   6e487465    cdpvs   4, 4, cr7, cr8, cr5, {3}
    5690:   616e4570    smcvs   58448
    5694:   49656c62    stmdbmi r5!, {r1, r5, r6, sl, fp, sp, lr}^
    5698:   01007072    tsteq   r0, r2, ror r0
    569c:   00173265    andseq  r3, r7, r5, ror #4
    56a0:   2c030500    cfstr32cs   mvfx0, [r3], {0}
            56a3: R_ARM_ABS32   .data
    56a4:   2d000000    stccs   0, cr0, [r0]
    56a8:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    56ac:   4253555f    subsmi  r5, r3, #398458880  ; 0x17c00000
    56b0:   65654b5f    strbvs  r4, [r5, #-2911]!
    56b4:   75625670    strbvc  r5, [r2, #-1648]!
    56b8:   616c4673    smcvs   50275
    56bc:   230e0067    movwcs  r0, #57447  ; 0xe067
    56c0:   0000018e    andeq   r0, r0, lr, lsl #3
    56c4:   0e1c0101    mufeqe  f0, f4, f1
    56c8:   d6000001    strle   r0, [r0], -r1
    56cc:   1d000056    stcne   0, cr0, [r0, #-344]
    56d0:   000000dd    ldrdeq  r0, [r0], -sp
    56d4:   4d2d00ff    stcmi   0, cr0, [sp, #-1020]!
    56d8:   445f4347    ldrbmi  r4, [pc], #839  ; 56e0 <.debug_info+0x56e0>
    56dc:   4d676169    stfmie  f6, [r7, #-420]!
    56e0:   0c006773    stceq   7, cr6, [r0], {115}
    56e4:   0056c642    subseq  ip, r6, r2, asr #12
    56e8:   2d010100    stfcss  f0, [r1]
    56ec:   5f43474d    svcpl   0x0043474d
    56f0:   61694462    cmnvs   r9, r2, ror #8
    56f4:   76654c67    strbtvc r4, [r5], -r7, ror #24
    56f8:   0c006c65    stceq   12, cr6, [r0], {101}
    56fc:   00016045    andeq   r6, r1, r5, asr #32
    5700:   2e010100    adfcss  f0, f1, f0
    5704:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    5708:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    570c:   7265535f    rsbvc   r5, r5, #2080374785 ; 0x7c000001
    5710:   65636976    strbvs  r6, [r3, #-2422]!
    5714:   130d0073    movwne  r0, #53363  ; 0xd073
    5718:   00437006    subeq   r7, r3, r6
    571c:   00010100    andeq   r0, r1, r0, lsl #2
Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:   0000054b    andeq   r0, r0, fp, asr #10
   4:   015f0002    cmpeq   pc, r2
   8:   01020000    tsteq   r2, r0
   c:   000d0efb    strdeq  r0, [sp], -fp
  10:   01010101    tsteq   r1, r1, lsl #2
  14:   01000000    tsteq   r0, r0
  18:   2e010000    cdpcs   0, 0, cr0, cr1, cr0, {0}
  1c:   64632f2e    strbtvs r2, [r3], #-3886
  20:   2f3a6300    svccs   0x003a6300
  24:   474f5250    smlsldmi    r5, pc, r0, r2
  28:   317e4152    cmncc   lr, r2, asr r1
  2c:   554e472f    strbpl  r4, [lr, #-1839]
  30:   2f4d5241    svccs   0x004d5241
  34:   2f62696c    svccs   0x0062696c
  38:   2f636367    svccs   0x00636367
  3c:   2d6d7261    sfmcs   f7, 2, [sp, #-388]!
  40:   2f666c65    svccs   0x00666c65
  44:   2e322e34    mrccs   14, 1, r2, cr2, cr4, {1}
  48:   6e692f32    mcrvs   15, 3, r2, cr9, cr2, {1}
  4c:   64756c63    ldrbtvs r6, [r5], #-3171
  50:   3a630065    bcc 18c01ec <MGC_DrcDumpPipe+0x18bbf98>
  54:   4f52502f    svcmi   0x0052502f
  58:   7e415247    cdpvc   2, 4, cr5, cr1, cr7, {2}
  5c:   4e472f31    mcrmi   15, 2, r2, cr7, cr1, {1}
  60:   4d524155    ldfmie  f4, [r2, #-340]
  64:   6d72612f    ldfvse  f6, [r2, #-188]!
  68:   666c652d    strbtvs r6, [ip], -sp, lsr #10
  6c:   636e692f    cmnvs   lr, #770048 ; 0xbc000
  70:   6564756c    strbvs  r7, [r4, #-1388]!
  74:   2f2e2e00    svccs   0x002e2e00
  78:   2e2f2e2e    cdpcs   14, 2, cr2, cr15, cr14, {1}
  7c:   2e2e2f2e    cdpcs   15, 2, cr2, cr14, cr14, {1}
  80:   2f2e2e2f    svccs   0x002e2e2f
  84:   74736e69    ldrbtvc r6, [r3], #-3689
  88:   2f6c6c61    svccs   0x006c6c61
  8c:   3638424d    ldrtcc  r4, [r8], -sp, asr #4
  90:   2f303648    svccs   0x00303648
  94:   6c636e69    stclvs  14, cr6, [r3], #-420
  98:   2f656475    svccs   0x00656475
  9c:   69706166    ldmdbvs r0!, {r1, r2, r5, r6, r8, sp, lr}^
  a0:   2f2e2e00    svccs   0x002e2e00
  a4:   692f2e2e    stmdbvs pc!, {r1, r2, r3, r5, r9, sl, fp, sp}
  a8:   756c636e    strbvc  r6, [ip, #-878]!
  ac:   00006564    andeq   r6, r0, r4, ror #10
  b0:   645f756d    ldrbvs  r7, [pc], #1389 ; b8 <.debug_line+0xb8>
  b4:   632e6372    teqvs   lr, #-939524095 ; 0xc8000001
  b8:   00000100    andeq   r0, r0, r0, lsl #2
  bc:   64647473    strbtvs r7, [r4], #-1139
  c0:   682e6665    stmdavs lr!, {r0, r2, r5, r6, r9, sl, sp, lr}
  c4:   00000200    andeq   r0, r0, r0, lsl #4
  c8:   69647473    stmdbvs r4!, {r0, r1, r4, r5, r6, sl, ip, sp, lr}^
  cc:   682e746e    stmdavs lr!, {r1, r2, r3, r5, r6, sl, ip, sp, lr}
  d0:   00000300    andeq   r0, r0, r0, lsl #6
  d4:   5f737973    svcpl   0x00737973
  d8:   76697264    strbtvc r7, [r9], -r4, ror #4
  dc:   682e7265    stmdavs lr!, {r0, r2, r5, r6, r9, ip, sp, lr}
  e0:   00000400    andeq   r0, r0, r0, lsl #8
  e4:   5f767264    svcpl   0x00767264
  e8:   2e616d64    cdpcs   13, 6, cr6, cr1, cr4, {3}
  ec:   00040068    andeq   r0, r4, r8, rrx
  f0:   5f756d00    svcpl   0x00756d00
  f4:   2e697364    cdpcs   3, 6, cr7, cr9, cr4, {3}
  f8:   00050068    andeq   r0, r5, r8, rrx
  fc:   5f756d00    svcpl   0x00756d00
 100:   2e766564    cdpcs   5, 7, cr6, cr6, cr4, {3}
 104:   00050068    andeq   r0, r5, r8, rrx
 108:   5f756d00    svcpl   0x00756d00
 10c:   2e696463    cdpcs   4, 6, cr6, cr9, cr3, {3}
 110:   00050068    andeq   r0, r5, r8, rrx
 114:   5f756d00    svcpl   0x00756d00
 118:   7473696c    ldrbtvc r6, [r3], #-2412
 11c:   0500682e    streq   r6, [r0, #-2094]
 120:   756d0000    strbvc  r0, [sp]!
 124:   6573685f    ldrbvs  r6, [r3, #-2143]!
 128:   00682e74    rsbeq   r2, r8, r4, ror lr
 12c:   6d000005    stcvs   0, cr0, [r0, #-20]
 130:   63645f75    cmnvs   r4, #468    ; 0x1d4
 134:   00682e69    rsbeq   r2, r8, r9, ror #28
 138:   6d000005    stcvs   0, cr0, [r0, #-20]
 13c:   6d695f75    stclvs  15, cr5, [r9, #-468]!
 140:   682e6c70    stmdavs lr!, {r4, r5, r6, sl, fp, sp, lr}
 144:   00000100    andeq   r0, r0, r0, lsl #2
 148:   5f737973    svcpl   0x00737973
 14c:   76726573    undefined
 150:   73656369    cmnvc   r5, #-1543503871    ; 0xa4000001
 154:   0400682e    streq   r6, [r0], #-2094
 158:   6c700000    ldclvs  0, cr0, [r0]
 15c:   635f7461    cmpvs   pc, #1627389952 ; 0x61000000
 160:   682e666e    stmdavs lr!, {r1, r2, r3, r5, r6, r9, sl, sp, lr}
 164:   00000500    andeq   r0, r0, r0, lsl #10
 168:   02050000    andeq   r0, r5, #0  ; 0x0
 16c:   00000000    andeq   r0, r0, r0
            16c: R_ARM_ABS32    .text
 170:   0100f803    tstpeq  r0, r3, lsl #16
 174:   4d6767a2    stclmi  7, cr6, [r7, #-648]!
 178:   9f686767    svcls   0x00686767
 17c:   03bb6b9f    undefined instruction 0x03bb6b9f
 180:   03900876    orrseq  r0, r0, #7733248    ; 0x760000
 184:   2a02ba0d    bcs ae9c0 <MGC_DrcDumpPipe+0xaa76c>
 188:   93088414    movwls  r8, #33812  ; 0x8414
 18c:   834b9f4d    movthi  r9, #48973  ; 0xbf4d
 190:   bd834b4b    vstrlt  d4, [r3, #300]
 194:   9f9f9f68    svcls   0x009f9f68
 198:   bb84bc4b    bllt    fe12f2cc <MGC_DrcDumpPipe+0xfe12b078>
 19c:   034cd76a    movteq  sp, #51050  ; 0xc76a
 1a0:   1703ba6e    strne   fp, [r3, -lr, ror #20]
 1a4:   84bf2008    ldrthi  r2, [pc], #8    ; 1ac <.debug_line+0x1ac>
 1a8:   4108892f    tstmi   r8, pc, lsr #18
 1ac:   6769674b    strbvs  r6, [r9, -fp, asr #14]!
 1b0:   5b082208    blpl    2089d8 <MGC_DrcDumpPipe+0x204784>
 1b4:   4202686b    andmi   r6, r2, #7012352    ; 0x6b0000
 1b8:   6402bc13    strvs   fp, [r2], #-3091
 1bc:   4dbc4b16    ldcmi   11, cr4, [ip, #88]!
 1c0:   6a68f3a0    bvs 1a3d048 <MGC_DrcDumpPipe+0x1a38df4>
 1c4:   30674bf7    strdcc  r4, [r7], #-183
 1c8:   683d08a0    ldmdavs sp!, {r5, r7, fp}
 1cc:   67f5676a    ldrbvs  r6, [r5, sl, ror #14]!
 1d0:   03667003    cmneq   r6, #3  ; 0x3
 1d4:   bc4b6614    mcrrlt  6, 1, r6, fp, cr4
 1d8:   30674b4c    rsbcc   r4, r7, ip, asr #22
 1dc:   683d08a0    ldmdavs sp!, {r5, r7, fp}
 1e0:   67f5676a    ldrbvs  r6, [r5, sl, ror #14]!
 1e4:   03667003    cmneq   r6, #3  ; 0x3
 1e8:   032f6613    teqeq   pc, #19922944   ; 0x1300000
 1ec:   0c03f20b    sfmeq   f7, 1, [r3], {11}
 1f0:   4b4b4bf2    blmi    12d31c0 <MGC_DrcDumpPipe+0x12cef6c>
 1f4:   8375084c    cmnhi   r5, #4980736    ; 0x4c0000
 1f8:   68bbc908    ldmvs   fp!, {r3, r8, fp, lr, pc}
 1fc:   142e02d7    strtne  r0, [lr], #-727
 200:   857508bc    ldrbhi  r0, [r5, #-2236]!
 204:   a1699f83    cmnge   r9, r3, lsl #31
 208:   4ba14d4b    blmi    fe85373c <MGC_DrcDumpPipe+0xfe84f4e8>
 20c:   2408836d    strcs   r8, [r8], #-877
 210:   2e58034b    cdpcs   3, 5, cr0, cr8, cr11, {2}
 214:   68d62f03    ldmvs   r6, {r0, r1, r8, r9, sl, fp, sp}^
 218:   64d8bc4e    ldrbvs  fp, [r8], #3150
 21c:   67676834    undefined
 220:   83839f68    orrhi   r9, r3, #416    ; 0x1a0
 224:   839f8683    orrshi  r8, pc, #137363456  ; 0x8300000
 228:   676a8383    strbvs  r8, [sl, -r3, lsl #7]!
 22c:   67f66d30    undefined
 230:   d84b4b4b    stmdale fp, {r0, r1, r3, r6, r8, r9, fp, lr}^
 234:   15240284    strne   r0, [r4, #-644]!
 238:   4b220867    blmi    8823dc <MGC_DrcDumpPipe+0x87e188>
 23c:   4b5b086a    blmi    16c23ec <MGC_DrcDumpPipe+0x16be198>
 240:   67a26784    strvs   r6, [r2, r4, lsl #15]!
 244:   68a0a567    stmiavs r0!, {r0, r1, r2, r5, r6, r8, sl, sp, pc}
 248:   a0142402    andsge  r2, r4, r2, lsl #8
 24c:   f4367ff5    undefined instruction 0xf4367ff5
 250:   67183e02    ldrvs   r3, [r8, -r2, lsl #28]
 254:   2208f369    andcs   pc, r8, #-1543503871    ; 0xa4000001
 258:   15280284    strne   r0, [r8, #-644]!
 25c:   4c83bb9f    fstmiaxmi   r3, {d11-d89}
 260:   2e02d985    cdpcs   9, 0, cr13, cr2, cr5, {4}
 264:   8368c018    cmnhi   r8, #24 ; 0x18
 268:   02c1be83    sbceq   fp, r1, #2096   ; 0x830
 26c:   5103182e    tstpl   r3, lr, lsr #16
 270:   2e36039e    mrccs   3, 1, r0, cr6, cr14, {4}
 274:   5908856a    stmdbpl r8, {r1, r3, r5, r6, r8, sl, pc}
 278:   68838369    stmvs   r3, {r0, r3, r5, r6, r8, r9, pc}
 27c:   183e02d8    ldmdane lr!, {r3, r4, r6, r7, r9}
 280:   08f36a67    ldmeq   r3!, {r0, r1, r2, r5, r6, r9, fp, sp, lr}^
 284:   83f48321    mvnshi  r8, #-2080374784    ; 0x84000000
 288:   67838783    strvs   r8, [r3, r3, lsl #15]
 28c:   08699fa2    stmdaeq r9!, {r1, r5, r7, r8, r9, sl, fp, ip, pc}^
 290:   85676724    strbhi  r6, [r7, #-1828]!
 294:   854cf585    strbhi  pc, [ip, #-1413]
 298:   68676784    stmdavs r7!, {r2, r7, r8, r9, sl, sp, lr}^
 29c:   6e2f4c67    cdpvs   12, 2, cr4, cr15, cr7, {3}
 2a0:   68bb85a5    ldmvs   fp!, {r0, r2, r5, r7, r8, sl, pc}
 2a4:   d7d768d7    undefined
 2a8:   d7d7d8d8    undefined
 2ac:   9bd768bb    blls    ff5da5a0 <MGC_DrcDumpPipe+0xff5d634c>
 2b0:   d768bbdb    undefined
 2b4:   d66c039b    undefined
 2b8:   84d61d03    ldrbhi  r1, [r6], #3331
 2bc:   674bd74e    strbvs  sp, [fp, -lr, asr #14]
 2c0:   93088368    movwls  r8, #33640  ; 0x8368
 2c4:   08ca0884    stmiaeq sl, {r2, r7, fp}^
 2c8:   8332685b    teqhi   r2, #5963776    ; 0x5b0000
 2cc:   69d78869    ldmibvs r7, {r0, r3, r5, r6, fp, pc}^
 2d0:   f4686968    vld2.16 {d22,d24}, [r8, :128], r8
 2d4:   0a036768    beq da07c <MGC_DrcDumpPipe+0xd5e28>
 2d8:   4d68d766    stclmi  7, cr13, [r8, #-408]!
 2dc:   dd4d8467    cfstrdle    mvd8, [sp, #-412]
 2e0:   9e0b0351    mcrls   3, 0, r0, cr11, cr1, {2}
 2e4:   674bd734    smlaldxvs   sp, fp, r4, r7
 2e8:   0830f468    ldmdaeq r0!, {r3, r5, r6, sl, ip, sp, lr, pc}
 2ec:   685c08cb    ldmdavs ip, {r0, r1, r3, r6, r7, fp}^
 2f0:   674bd78a    strbvs  sp, [fp, -sl, lsl #15]
 2f4:   0830f468    ldmdaeq r0!, {r3, r5, r6, sl, ip, sp, lr, pc}
 2f8:   685c08cb    ldmdavs ip, {r0, r1, r3, r6, r7, fp}^
 2fc:   684bd78a    stmdavs fp, {r1, r3, r7, r8, r9, sl, ip, lr, pc}^
 300:   67679208    strbvs  r9, [r7, -r8, lsl #4]!
 304:   a4940884    ldrge   r0, [r4], #2180
 308:   684b4bd8    stmdavs fp, {r3, r4, r6, r7, r8, r9, fp, lr}^
 30c:   84679208    strbthi r9, [r7], #-520
 310:   84688483    strbthi r8, [r8], #-1155
 314:   0868bb4e    stmdaeq r8!, {r1, r2, r3, r6, r8, r9, fp, ip, sp, pc}^
 318:   08b10876    ldmeq   r1!, {r1, r2, r4, r5, r6, fp}
 31c:   4bd7c293    blmi    ff5f0d70 <MGC_DrcDumpPipe+0xff5ecb1c>
 320:   84f86867    ldrbthi r6, [r8], #2151
 324:   7a08cc08    bvc 23334c <MGC_DrcDumpPipe+0x22f0f8>
 328:   08846767    stmeq   r4, {r0, r1, r2, r5, r6, r8, r9, sl, sp, lr}
 32c:   695c08cb    ldmdbvs ip, {r0, r1, r3, r6, r7, fp}^
 330:   674bd7a2    strbvs  sp, [fp, -r2, lsr #15]
 334:   cb088468    blgt    2214dc <MGC_DrcDumpPipe+0x21d288>
 338:   84675c08    strbthi r5, [r7], #-3080
 33c:   4ebc8669    cdpmi   6, 11, cr8, cr12, cr9, {3}
 340:   84844bbb    strhi   r4, [r4], #3003
 344:   79088367    stmdbvc r8, {r0, r1, r2, r5, r6, r8, r9, pc}
 348:   4bd76968    blmi    ff5da8f0 <MGC_DrcDumpPipe+0xff5d669c>
 34c:   50836768    addpl   r6, r3, r8, ror #14
 350:   85d985d8    ldrbhi  r8, [r9, #1496]
 354:   847508db    ldrbthi r0, [r5], #-2267
 358:   67165202    ldrvs   r5, [r6, -r2, lsl #4]
 35c:   67318367    ldrvs   r8, [r1, -r7, ror #6]!
 360:   5a088484    bpl 221578 <MGC_DrcDumpPipe+0x21d324>
 364:   84846731    strhi   r6, [r4], #1841
 368:   08365a08    ldmdaeq r6!, {r3, r9, fp, ip, lr}
 36c:   2e09035a    mcrcs   3, 0, r0, cr9, cr10, {2}
 370:   365b0884    ldrbcc  r0, [fp], -r4, lsl #17
 374:   5a088367    bpl 221118 <MGC_DrcDumpPipe+0x21cec4>
 378:   36028431    undefined
 37c:   6783f416    usad8vs r3, r6, r4
 380:   314d369a    strbcc  r3, [sp, #-106]
 384:   31134a02    tstcc   r3, r2, lsl #20
 388:   68157402    ldmdavs r5, {r1, sl, ip, sp, lr}
 38c:   9fcf084c    svcls   0x00cf084c
 390:   684da068    stmdavs sp, {r3, r5, r6, sp, pc}^
 394:   dc4e4008    mcrrle  0, 0, r4, lr, cr8
 398:   8567684b    strbhi  r6, [r7, #-2123]!
 39c:   68f368bb    ldmvs   r3!, {r0, r1, r3, r4, r5, r7, fp, sp, lr}^
 3a0:   84676867    strbthi r6, [r7], #-2151
 3a4:   03d67503    bicseq  r7, r6, #12582912   ; 0xc00000
 3a8:   0903d611    stmdbeq r3, {r0, r4, r9, sl, ip, lr, pc}
 3ac:   844bdc4a    strbhi  sp, [fp], #-3146
 3b0:   67678468    strbvs  r8, [r7, -r8, ror #8]!
 3b4:   6784f44b    strvs   pc, [r4, fp, asr #8]
 3b8:   6784f44b    strvs   pc, [r4, fp, asr #8]
 3bc:   67a0f44b    strvs   pc, [r0, fp, asr #8]!
 3c0:   6784f44b    strvs   pc, [r4, fp, asr #8]
 3c4:   85f44b67    ldrbhi  r4, [r4, #2919]!
 3c8:   67674b84    strbvs  r4, [r7, -r4, lsl #23]!
 3cc:   6a89f54b    bvs fe27d900 <MGC_DrcDumpPipe+0xfe2796ac>
 3d0:   4b678583    blmi    19e19e4 <MGC_DrcDumpPipe+0x19dd790>
 3d4:   85692308    strbhi  r2, [r9, #-776]!
 3d8:   4b67674b    blmi    19da10c <MGC_DrcDumpPipe+0x19d5eb8>
 3dc:   67832808    strvs   r2, [r3, r8, lsl #16]
 3e0:   4b838367    blmi    fe0e1184 <MGC_DrcDumpPipe+0xfe0dcf30>
 3e4:   3f084b67    svccc   0x00084b67
 3e8:   856b3067    strbhi  r3, [fp, #-103]!
 3ec:   d868bb85    stmdale r8!, {r0, r2, r7, r8, r9, fp, ip, sp, pc}^
 3f0:   08675b08    stmdaeq r7!, {r3, r8, r9, fp, ip, lr}^
 3f4:   08760322    ldmdaeq r6!, {r1, r5, r8, r9}^
 3f8:   d6110390    undefined
 3fc:   a52f4f84    strge   r4, [pc, #-3972]!   ; fffff480 <MGC_DrcDumpPipe+0xffffb22c>
 400:   4b9e0c03    blmi    fe783414 <MGC_DrcDumpPipe+0xfe77f1c0>
 404:   6c676767    stclvs  7, cr6, [r7], #-412
 408:   02303d08    eorseq  r3, r0, #512    ; 0x200
 40c:   91081952    tstls   r8, r2, asr r9
 410:   08770884    ldmdaeq r7!, {r2, r7, fp}^
 414:   68326740    ldmdavs r2!, {r6, r8, r9, sl, sp, lr}
 418:   95088367    strls   r8, [r8, #-871]
 41c:   85920831    ldrhi   r0, [r2, #2097]
 420:   32846767    addcc   r6, r4, #27000832   ; 0x19c0000
 424:   bb849108    bllt    fe12484c <MGC_DrcDumpPipe+0xfe1205f8>
 428:   86679108    strbthi r9, [r7], -r8, lsl #2
 42c:   34685a08    strbtcc r5, [r8], #-2568
 430:   08849208    stmeq   r4, {r3, r9, ip, pc}
 434:   6793083f    undefined
 438:   67316884    ldrvs   r6, [r1, -r4, lsl #17]!
 43c:   31683167    cmncc   r8, r7, ror #2
 440:   67849108    strvs   r9, [r4, r8, lsl #2]
 444:   683563f5    ldmdavs r5!, {r0, r2, r4, r5, r6, r7, r8, r9, sp, lr}
 448:   69920831    ldmibvs r2, {r0, r4, r5, fp}
 44c:   8393084c    orrshi  r0, r3, #4980736    ; 0x4c0000
 450:   84f43167    ldrbthi r3, [r4], #359
 454:   31683662    cmncc   r8, r2, ror #12
 458:   da6a6983    ble 1a9aa6c <MGC_DrcDumpPipe+0x1a96818>
 45c:   f3750867    vsub.i64    q8, <illegal reg q2.5>, <illegal reg q11.5>
 460:   dd0f2402    cfstrsle    mvf2, [pc, #-8]
 464:   84910831    ldrhi   r0, [r1], #2097
 468:   08bb8485    ldmeq   fp!, {r0, r2, r7, sl, pc}
 46c:   08866791    stmeq   r6, {r0, r4, r7, r8, r9, sl, sp, lr}
 470:   69838391    stmibvs r3, {r0, r4, r7, r8, r9, pc}
 474:   f2110332    vcge.s16    d0, d1, d18
 478:   f6920868    undefined instruction 0xf6920868
 47c:   31d8d84b    bicscc  sp, r8, fp, asr #16
 480:   31d9d84b    bicscc  sp, r9, fp, asr #16
 484:   68dcd84b    ldmvs   ip, {r0, r1, r3, r6, fp, ip, lr, pc}^
 488:   7708a3f4    undefined
 48c:   7dfe0389    ldclvc  3, cr0, [lr, #548]!
 490:   85033c08    strhi   r3, [r3, #-3080]
 494:   22026602    andcs   r6, r2, #2097152    ; 0x200000
 498:   4b21081a    blmi    842508 <MGC_DrcDumpPipe+0x83e2b4>
 49c:   67d74b4b    ldrbvs  r4, [r7, fp, asr #22]
 4a0:   69676869    stmdbvs r7!, {r0, r3, r5, r6, fp, sp, lr}^
 4a4:   2208bb85    andcs   fp, r8, #136192 ; 0x21400
 4a8:   67183e02    ldrvs   r3, [r8, -r2, lsl #28]
 4ac:   08bbf36a    ldmeq   fp!, {r1, r3, r5, r6, r8, r9, ip, sp, lr, pc}
 4b0:   67d76c5a    undefined
 4b4:   d78468a1    strle   r6, [r4, r1, lsr #17]
 4b8:   84152202    ldrhi   r2, [r5], #-514
 4bc:   173c0283    ldrne   r0, [ip, -r3, lsl #5]!
 4c0:   bb6a6bd7    bllt    1a9b424 <MGC_DrcDumpPipe+0x1a971d0>
 4c4:   3e023f08    cdpcc   15, 0, cr3, cr2, cr8, {0}
 4c8:   686a6718    stmdavs sl!, {r3, r4, r8, r9, sl, sp, lr}^
 4cc:   684f67f3    stmdavs pc, {r0, r1, r4, r5, r6, r7, r8, r9, sl, sp, lr}^
 4d0:   6768b008    strbvs  fp, [r8, -r8]!
 4d4:   d8660903    stmdale r6!, {r0, r1, r8, fp}^
 4d8:   4008694b    andmi   r6, r8, fp, asr #18
 4dc:   f44e5a08    vst1.8  {d21-d22}, [lr], r8
 4e0:   bc4e5a08    mcrrlt  10, 0, r5, lr, cr8
 4e4:   694d5a08    stmdbvs sp, {r3, r9, fp, ip, lr}^
 4e8:   163602f4    undefined
 4ec:   c06783f4    strdgt  r8, [r7], #-52
 4f0:   bd2f4d85    stclt   13, cr4, [pc, #-532]!
 4f4:   85834c9f    strhi   r4, [r3, #3231]
 4f8:   6dcb084c    stclvs  8, cr0, [fp, #304]
 4fc:   83684bd7    cmnhi   r8, #220160 ; 0x35c00
 500:   bc6a8484    cfstrdlt    mvd8, [sl], #-528
 504:   67d76c2f    ldrbvs  r6, [r7, pc, lsr #24]
 508:   82090367    andhi   r0, r9, #-1677721599    ; 0x9c000001
 50c:   68848484    stmvs   r4, {r2, r7, sl, pc}
 510:   69678886    stmdbvs r7!, {r1, r2, r7, fp, pc}^
 514:   f44a0903    vst2.8  {d16,d18}, [sl], r3
 518:   85694b4b    strbhi  r4, [r9, #-2891]!
 51c:   69a06968    stmibvs r0!, {r3, r5, r6, r8, fp, sp, lr}
 520:   5c086867    stcpl   8, cr6, [r8], {103}
 524:   6967f568    stmdbvs r7!, {r3, r5, r6, r8, sl, ip, sp, lr, pc}^
 528:   68676785    stmdavs r7!, {r0, r2, r7, r8, r9, sl, sp, lr}^
 52c:   6c2f4e69    stcvs   14, cr4, [pc], #-420
 530:   68674b9f    stmdavs r7!, {r0, r1, r2, r3, r4, r7, r8, r9, fp, lr}^
 534:   83878467    orrhi   r8, r7, #1728053248 ; 0x67000000
 538:   08688467    stmdaeq r8!, {r0, r1, r2, r5, r6, sl, pc}^
 53c:   4c9f695b    ldcmi   9, cr6, [pc], {91}
 540:   846867a1    strbthi r6, [r8], #-1953
 544:   6e676867    cdpvs   8, 6, cr6, cr7, cr7, {3}
 548:   06022fbc    undefined
 54c:   Address 0x0000054c is out of bounds.

Disassembly of section .rodata:

00000000 <MGC_aSetHnpSupportData>:
   0:   00040300    .word   0x00040300
   4:   00000000    .word   0x00000000

00000008 <MGC_aSetHnpEnableData>:
   8:   00030300    .word   0x00030300
   c:   00000000    .word   0x00000000
  10:   72746e49    .word   0x72746e49
  14:   3d425355    .word   0x3d425355
  18:   00000000    .word   0x00000000
  1c:   72746e49    .word   0x72746e49
  20:   003d7854    .word   0x003d7854
  24:   49202f20    .word   0x49202f20
  28:   5272746e    .word   0x5272746e
  2c:   00003d78    .word   0x00003d78
  30:   49564544    .word   0x49564544
  34:   4e204543    .word   0x4e204543
  38:   5220544f    .word   0x5220544f
  3c:   4f505345    .word   0x4f505345
  40:   4e49444e    .word   0x4e49444e
  44:   41282047    .word   0x41282047
  48:   7665642d    .word   0x7665642d
  4c:   20656369    .word   0x20656369
  50:   6e646964    .word   0x6e646964
  54:   63207427    .word   0x63207427
  58:   656e6e6f    .word   0x656e6e6f
  5c:   00297463    .word   0x00297463
  60:   49564544    .word   0x49564544
  64:   4e204543    .word   0x4e204543
  68:   5220544f    .word   0x5220544f
  6c:   4f505345    .word   0x4f505345
  70:   4e49444e    .word   0x4e49444e
  74:   64282047    .word   0x64282047
  78:   6e206469    .word   0x6e206469
  7c:   4820746f    .word   0x4820746f
  80:   0029504e    .word   0x0029504e
  84:   49564544    .word   0x49564544
  88:   4e204543    .word   0x4e204543
  8c:   5220544f    .word   0x5220544f
  90:   4f505345    .word   0x4f505345
  94:   4e49444e    .word   0x4e49444e
  98:   00000047    .word   0x00000047
  9c:   72617453    .word   0x72617453
  a0:   676e6974    .word   0x676e6974
  a4:   73657220    .word   0x73657220
  a8:   00007465    .word   0x00007465
  ac:   706f7453    .word   0x706f7453
  b0:   676e6970    .word   0x676e6970
  b4:   73657220    .word   0x73657220
  b8:   00007465    .word   0x00007465
  bc:   75736e49    .word   0x75736e49
  c0:   63696666    .word   0x63696666
  c4:   746e6569    .word   0x746e6569
  c8:   6d656d20    .word   0x6d656d20
  cc:   2079726f    .word   0x2079726f
  d0:   20726f66    .word   0x20726f66
  d4:   6d756e65    .word   0x6d756e65
  d8:   74617265    .word   0x74617265
  dc:   006e6f69    .word   0x006e6f69
  e0:   75736e49    .word   0x75736e49
  e4:   63696666    .word   0x63696666
  e8:   746e6569    .word   0x746e6569
  ec:   64646120    .word   0x64646120
  f0:   73736572    .word   0x73736572
  f4:   66207365    .word   0x66207365
  f8:   6520726f    .word   0x6520726f
  fc:   656d756e    .word   0x656d756e
 100:   69746172    .word   0x69746172
 104:   00006e6f    .word   0x00006e6f
 108:   52535f42    .word   0x52535f42
 10c:   61465f50    .word   0x61465f50
 110:   203a6c69    .word   0x203a6c69
 114:   6e616863    .word   0x6e616863
 118:   676e6967    .word   0x676e6967
 11c:   206f7420    .word   0x206f7420
 120:   45505f42    .word   0x45505f42
 124:   48504952    .word   0x48504952
 128:   4c415245    .word   0x4c415245
 12c:   00000000    .word   0x00000000
 130:   49564544    .word   0x49564544
 134:   4e204543    .word   0x4e204543
 138:   5220544f    .word   0x5220544f
 13c:   4f505345    .word   0x4f505345
 140:   4e49444e    .word   0x4e49444e
 144:   53282047    .word   0x53282047
 148:   00295052    .word   0x00295052
 14c:   6f727245    .word   0x6f727245
 150:   68632072    .word   0x68632072
 154:   69676e61    .word   0x69676e61
 158:   4f20676e    .word   0x4f20676e
 15c:   73204754    .word   0x73204754
 160:   65746174    .word   0x65746174
 164:   206f7420    .word   0x206f7420
 168:   00000000    .word   0x00000000
 16c:   2047544f    .word   0x2047544f
 170:   74617473    .word   0x74617473
 174:   68632065    .word   0x68632065
 178:   65676e61    .word   0x65676e61
 17c:   6f726620    .word   0x6f726620
 180:   0000206d    .word   0x0000206d
 184:   206f7420    .word   0x206f7420
 188:   00000000    .word   0x00000000
 18c:   75736552    .word   0x75736552
 190:   4920656d    .word   0x4920656d
 194:   7265746e    .word   0x7265746e
 198:   74707572    .word   0x74707572
 19c:   00000000    .word   0x00000000
 1a0:   73736553    .word   0x73736553
 1a4:   206e6f69    .word   0x206e6f69
 1a8:   75716552    .word   0x75716552
 1ac:   20747365    .word   0x20747365
 1b0:   65746e49    .word   0x65746e49
 1b4:   70757272    .word   0x70757272
 1b8:   00000074    .word   0x00000074
 1bc:   73756256    .word   0x73756256
 1c0:   72724520    .word   0x72724520
 1c4:   4920726f    .word   0x4920726f
 1c8:   7265746e    .word   0x7265746e
 1cc:   74707572    .word   0x74707572
 1d0:   00000000    .word   0x00000000
 1d4:   72746552    .word   0x72746552
 1d8:   676e6979    .word   0x676e6979
 1dc:   65756420    .word   0x65756420
 1e0:   206f7420    .word   0x206f7420
 1e4:   73756256    .word   0x73756256
 1e8:   72726520    .word   0x72726520
 1ec:   0000726f    .word   0x0000726f
 1f0:   70737553    .word   0x70737553
 1f4:   20646e65    .word   0x20646e65
 1f8:   65746e49    .word   0x65746e49
 1fc:   70757272    .word   0x70757272
 200:   00000074    .word   0x00000074
 204:   6e6e6f43    .word   0x6e6e6f43
 208:   20746365    .word   0x20746365
 20c:   65746e49    .word   0x65746e49
 210:   70757272    .word   0x70757272
 214:   00000074    .word   0x00000074
 218:   63736944    .word   0x63736944
 21c:   656e6e6f    .word   0x656e6e6f
 220:   49207463    .word   0x49207463
 224:   7265746e    .word   0x7265746e
 228:   74707572    .word   0x74707572
 22c:   00000000    .word   0x00000000
 230:   53554d5b    .word   0x53554d5b
 234:   43205d42    .word   0x43205d42
 238:   65636e61    .word   0x65636e61
 23c:   6f6e206c    .word   0x6f6e206c
 240:   69662074    .word   0x69662074
 244:   6873696e    .word   0x6873696e
 248:   44206465    .word   0x44206465
 24c:   7220414d    .word   0x7220414d
 250:   65757165    .word   0x65757165
 254:   0a2e7473    .word   0x0a2e7473
 258:   00000000    .word   0x00000000
 25c:   65736552    .word   0x65736552
 260:   6e492074    .word   0x6e492074
 264:   72726574    .word   0x72726574
 268:   00747075    .word   0x00747075
 26c:   72746552    .word   0x72746552
 270:   676e6979    .word   0x676e6979
 274:   65756420    .word   0x65756420
 278:   206f7420    .word   0x206f7420
 27c:   62626162    .word   0x62626162
 280:   0000656c    .word   0x0000656c
 284:   706f7453    .word   0x706f7453
 288:   676e6970    .word   0x676e6970
 28c:   65756420    .word   0x65756420
 290:   206f7420    .word   0x206f7420
 294:   62626162    .word   0x62626162
 298:   0000656c    .word   0x0000656c
Disassembly of section .debug_frame:

00000000 <.debug_frame>:
   0:   0000000c    .word   0x0000000c
   4:   ffffffff    .word   0xffffffff
   8:   7c010001    .word   0x7c010001
   c:   000d0c0e    .word   0x000d0c0e
  10:   0000001c    .word   0x0000001c
    ...
            14: R_ARM_ABS32 .debug_frame
            18: R_ARM_ABS32 .text
  1c:   0000032c    .word   0x0000032c
  20:   440c0d44    .word   0x440c0d44
  24:   038d028e    .word   0x038d028e
  28:   0c44048b    .word   0x0c44048b
  2c:   0000040b    .word   0x0000040b
  30:   0000001c    .word   0x0000001c
  34:   00000000    .word   0x00000000
            34: R_ARM_ABS32 .debug_frame
  38:   0000032c    .word   0x0000032c
            38: R_ARM_ABS32 .text
  3c:   00000478    .word   0x00000478
  40:   440c0d44    .word   0x440c0d44
  44:   038d028e    .word   0x038d028e
  48:   0c44048b    .word   0x0c44048b
  4c:   0000040b    .word   0x0000040b
  50:   0000001c    .word   0x0000001c
  54:   00000000    .word   0x00000000
            54: R_ARM_ABS32 .debug_frame
  58:   000007a4    .word   0x000007a4
            58: R_ARM_ABS32 .text
  5c:   000003d0    .word   0x000003d0
  60:   440c0d44    .word   0x440c0d44
  64:   038d028e    .word   0x038d028e
  68:   0c44048b    .word   0x0c44048b
  6c:   0000040b    .word   0x0000040b
  70:   0000001c    .word   0x0000001c
  74:   00000000    .word   0x00000000
            74: R_ARM_ABS32 .debug_frame
  78:   00000b74    .word   0x00000b74
            78: R_ARM_ABS32 .text
  7c:   0000074c    .word   0x0000074c
  80:   440c0d44    .word   0x440c0d44
  84:   038d028e    .word   0x038d028e
  88:   0584048b    .word   0x0584048b
  8c:   040b0c44    .word   0x040b0c44
  90:   0000001c    .word   0x0000001c
  94:   00000000    .word   0x00000000
            94: R_ARM_ABS32 .debug_frame
  98:   000012c0    .word   0x000012c0
            98: R_ARM_ABS32 .text
  9c:   0000010c    .word   0x0000010c
  a0:   440c0d44    .word   0x440c0d44
  a4:   038d028e    .word   0x038d028e
  a8:   0c44048b    .word   0x0c44048b
  ac:   0000040b    .word   0x0000040b
  b0:   0000001c    .word   0x0000001c
  b4:   00000000    .word   0x00000000
            b4: R_ARM_ABS32 .debug_frame
  b8:   000013cc    .word   0x000013cc
            b8: R_ARM_ABS32 .text
  bc:   0000022c    .word   0x0000022c
  c0:   440c0d44    .word   0x440c0d44
  c4:   038d028e    .word   0x038d028e
  c8:   0c44048b    .word   0x0c44048b
  cc:   0000040b    .word   0x0000040b
  d0:   0000001c    .word   0x0000001c
  d4:   00000000    .word   0x00000000
            d4: R_ARM_ABS32 .debug_frame
  d8:   000015f8    .word   0x000015f8
            d8: R_ARM_ABS32 .text
  dc:   00000134    .word   0x00000134
  e0:   440c0d44    .word   0x440c0d44
  e4:   038d028e    .word   0x038d028e
  e8:   0c44048b    .word   0x0c44048b
  ec:   0000040b    .word   0x0000040b
  f0:   0000001c    .word   0x0000001c
  f4:   00000000    .word   0x00000000
            f4: R_ARM_ABS32 .debug_frame
  f8:   0000172c    .word   0x0000172c
            f8: R_ARM_ABS32 .text
  fc:   00000090    .word   0x00000090
 100:   440c0d44    .word   0x440c0d44
 104:   038d028e    .word   0x038d028e
 108:   0c44048b    .word   0x0c44048b
 10c:   0000040b    .word   0x0000040b
 110:   0000001c    .word   0x0000001c
 114:   00000000    .word   0x00000000
            114: R_ARM_ABS32    .debug_frame
 118:   000017bc    .word   0x000017bc
            118: R_ARM_ABS32    .text
 11c:   00000054    .word   0x00000054
 120:   440c0d44    .word   0x440c0d44
 124:   038d028e    .word   0x038d028e
 128:   0c44048b    .word   0x0c44048b
 12c:   0000040b    .word   0x0000040b
 130:   0000001c    .word   0x0000001c
 134:   00000000    .word   0x00000000
            134: R_ARM_ABS32    .debug_frame
 138:   00001810    .word   0x00001810
            138: R_ARM_ABS32    .text
 13c:   00000024    .word   0x00000024
 140:   440c0d44    .word   0x440c0d44
 144:   038d028e    .word   0x038d028e
 148:   0c44048b    .word   0x0c44048b
 14c:   0000040b    .word   0x0000040b
 150:   0000001c    .word   0x0000001c
 154:   00000000    .word   0x00000000
            154: R_ARM_ABS32    .debug_frame
 158:   00001834    .word   0x00001834
            158: R_ARM_ABS32    .text
 15c:   00000018    .word   0x00000018
 160:   440c0d44    .word   0x440c0d44
 164:   038d028e    .word   0x038d028e
 168:   0c44048b    .word   0x0c44048b
 16c:   0000040b    .word   0x0000040b
 170:   0000001c    .word   0x0000001c
 174:   00000000    .word   0x00000000
            174: R_ARM_ABS32    .debug_frame
 178:   0000184c    .word   0x0000184c
            178: R_ARM_ABS32    .text
 17c:   000000e4    .word   0x000000e4
 180:   440c0d44    .word   0x440c0d44
 184:   038d028e    .word   0x038d028e
 188:   0c44048b    .word   0x0c44048b
 18c:   0000040b    .word   0x0000040b
 190:   0000001c    .word   0x0000001c
 194:   00000000    .word   0x00000000
            194: R_ARM_ABS32    .debug_frame
 198:   00001930    .word   0x00001930
            198: R_ARM_ABS32    .text
 19c:   000000e4    .word   0x000000e4
 1a0:   440c0d44    .word   0x440c0d44
 1a4:   038d028e    .word   0x038d028e
 1a8:   0c44048b    .word   0x0c44048b
 1ac:   0000040b    .word   0x0000040b
 1b0:   0000001c    .word   0x0000001c
 1b4:   00000000    .word   0x00000000
            1b4: R_ARM_ABS32    .debug_frame
 1b8:   00001a14    .word   0x00001a14
            1b8: R_ARM_ABS32    .text
 1bc:   000000d4    .word   0x000000d4
 1c0:   440c0d44    .word   0x440c0d44
 1c4:   038d028e    .word   0x038d028e
 1c8:   0c44048b    .word   0x0c44048b
 1cc:   0000040b    .word   0x0000040b
 1d0:   0000001c    .word   0x0000001c
 1d4:   00000000    .word   0x00000000
            1d4: R_ARM_ABS32    .debug_frame
 1d8:   00001ae8    .word   0x00001ae8
            1d8: R_ARM_ABS32    .text
 1dc:   000001a4    .word   0x000001a4
 1e0:   440c0d44    .word   0x440c0d44
 1e4:   038d028e    .word   0x038d028e
 1e8:   0c44048b    .word   0x0c44048b
 1ec:   0000040b    .word   0x0000040b
 1f0:   0000001c    .word   0x0000001c
 1f4:   00000000    .word   0x00000000
            1f4: R_ARM_ABS32    .debug_frame
 1f8:   00001c8c    .word   0x00001c8c
            1f8: R_ARM_ABS32    .text
 1fc:   00000188    .word   0x00000188
 200:   440c0d44    .word   0x440c0d44
 204:   038d028e    .word   0x038d028e
 208:   0c44048b    .word   0x0c44048b
 20c:   0000040b    .word   0x0000040b
 210:   0000001c    .word   0x0000001c
 214:   00000000    .word   0x00000000
            214: R_ARM_ABS32    .debug_frame
 218:   00001e14    .word   0x00001e14
            218: R_ARM_ABS32    .text
 21c:   000000ec    .word   0x000000ec
 220:   440c0d44    .word   0x440c0d44
 224:   038d028e    .word   0x038d028e
 228:   0c44048b    .word   0x0c44048b
 22c:   0000040b    .word   0x0000040b
 230:   0000001c    .word   0x0000001c
 234:   00000000    .word   0x00000000
            234: R_ARM_ABS32    .debug_frame
 238:   00001f00    .word   0x00001f00
            238: R_ARM_ABS32    .text
 23c:   00000020    .word   0x00000020
 240:   440c0d44    .word   0x440c0d44
 244:   038d028e    .word   0x038d028e
 248:   0c44048b    .word   0x0c44048b
 24c:   0000040b    .word   0x0000040b
 250:   0000001c    .word   0x0000001c
 254:   00000000    .word   0x00000000
            254: R_ARM_ABS32    .debug_frame
 258:   00001f20    .word   0x00001f20
            258: R_ARM_ABS32    .text
 25c:   000000a4    .word   0x000000a4
 260:   440c0d44    .word   0x440c0d44
 264:   038d028e    .word   0x038d028e
 268:   0c44048b    .word   0x0c44048b
 26c:   0000040b    .word   0x0000040b
 270:   0000001c    .word   0x0000001c
 274:   00000000    .word   0x00000000
            274: R_ARM_ABS32    .debug_frame
 278:   00001fc4    .word   0x00001fc4
            278: R_ARM_ABS32    .text
 27c:   00000054    .word   0x00000054
 280:   440c0d44    .word   0x440c0d44
 284:   038d028e    .word   0x038d028e
 288:   0c44048b    .word   0x0c44048b
 28c:   0000040b    .word   0x0000040b
 290:   0000001c    .word   0x0000001c
 294:   00000000    .word   0x00000000
            294: R_ARM_ABS32    .debug_frame
 298:   00002018    .word   0x00002018
            298: R_ARM_ABS32    .text
 29c:   000005a4    .word   0x000005a4
 2a0:   440c0d44    .word   0x440c0d44
 2a4:   038d028e    .word   0x038d028e
 2a8:   0c44048b    .word   0x0c44048b
 2ac:   0000040b    .word   0x0000040b
 2b0:   0000001c    .word   0x0000001c
 2b4:   00000000    .word   0x00000000
            2b4: R_ARM_ABS32    .debug_frame
 2b8:   000025bc    .word   0x000025bc
            2b8: R_ARM_ABS32    .text
 2bc:   00000078    .word   0x00000078
 2c0:   440c0d44    .word   0x440c0d44
 2c4:   038d028e    .word   0x038d028e
 2c8:   0c44048b    .word   0x0c44048b
 2cc:   0000040b    .word   0x0000040b
 2d0:   0000001c    .word   0x0000001c
 2d4:   00000000    .word   0x00000000
            2d4: R_ARM_ABS32    .debug_frame
 2d8:   00002634    .word   0x00002634
            2d8: R_ARM_ABS32    .text
 2dc:   00000110    .word   0x00000110
 2e0:   440c0d44    .word   0x440c0d44
 2e4:   038d028e    .word   0x038d028e
 2e8:   0c44048b    .word   0x0c44048b
 2ec:   0000040b    .word   0x0000040b
 2f0:   0000001c    .word   0x0000001c
 2f4:   00000000    .word   0x00000000
            2f4: R_ARM_ABS32    .debug_frame
 2f8:   00002744    .word   0x00002744
            2f8: R_ARM_ABS32    .text
 2fc:   000004d8    .word   0x000004d8
 300:   440c0d44    .word   0x440c0d44
 304:   038d028e    .word   0x038d028e
 308:   0584048b    .word   0x0584048b
 30c:   040b0c44    .word   0x040b0c44
 310:   0000001c    .word   0x0000001c
 314:   00000000    .word   0x00000000
            314: R_ARM_ABS32    .debug_frame
 318:   00002c1c    .word   0x00002c1c
            318: R_ARM_ABS32    .text
 31c:   00000a6c    .word   0x00000a6c
 320:   440c0d44    .word   0x440c0d44
 324:   038d028e    .word   0x038d028e
 328:   0c44048b    .word   0x0c44048b
 32c:   0000040b    .word   0x0000040b
 330:   00000020    .word   0x00000020
 334:   00000000    .word   0x00000000
            334: R_ARM_ABS32    .debug_frame
 338:   00003688    .word   0x00003688
            338: R_ARM_ABS32    .text
 33c:   000004e4    .word   0x000004e4
 340:   440c0d44    .word   0x440c0d44
 344:   038d028e    .word   0x038d028e
 348:   0585048b    .word   0x0585048b
 34c:   0c440684    .word   0x0c440684
 350:   0000040b    .word   0x0000040b
 354:   0000001c    .word   0x0000001c
 358:   00000000    .word   0x00000000
            358: R_ARM_ABS32    .debug_frame
 35c:   00003b6c    .word   0x00003b6c
            35c: R_ARM_ABS32    .text
 360:   0000024c    .word   0x0000024c
 364:   440c0d44    .word   0x440c0d44
 368:   038d028e    .word   0x038d028e
 36c:   0c44048b    .word   0x0c44048b
 370:   0000040b    .word   0x0000040b
 374:   0000001c    .word   0x0000001c
 378:   00000000    .word   0x00000000
            378: R_ARM_ABS32    .debug_frame
 37c:   00003db8    .word   0x00003db8
            37c: R_ARM_ABS32    .text
 380:   0000008c    .word   0x0000008c
 384:   440c0d44    .word   0x440c0d44
 388:   038d028e    .word   0x038d028e
 38c:   0c44048b    .word   0x0c44048b
 390:   0000040b    .word   0x0000040b
 394:   0000001c    .word   0x0000001c
 398:   00000000    .word   0x00000000
            398: R_ARM_ABS32    .debug_frame
 39c:   00003e44    .word   0x00003e44
            39c: R_ARM_ABS32    .text
 3a0:   00000094    .word   0x00000094
 3a4:   440c0d44    .word   0x440c0d44
 3a8:   038d028e    .word   0x038d028e
 3ac:   0c44048b    .word   0x0c44048b
 3b0:   0000040b    .word   0x0000040b
 3b4:   0000001c    .word   0x0000001c
 3b8:   00000000    .word   0x00000000
            3b8: R_ARM_ABS32    .debug_frame
 3bc:   00003ed8    .word   0x00003ed8
            3bc: R_ARM_ABS32    .text
 3c0:   000000c0    .word   0x000000c0
 3c4:   440c0d44    .word   0x440c0d44
 3c8:   038d028e    .word   0x038d028e
 3cc:   0c44048b    .word   0x0c44048b
 3d0:   0000040b    .word   0x0000040b
 3d4:   0000001c    .word   0x0000001c
 3d8:   00000000    .word   0x00000000
            3d8: R_ARM_ABS32    .debug_frame
 3dc:   00003f98    .word   0x00003f98
            3dc: R_ARM_ABS32    .text
 3e0:   00000164    .word   0x00000164
 3e4:   440c0d44    .word   0x440c0d44
 3e8:   038d028e    .word   0x038d028e
 3ec:   0c44048b    .word   0x0c44048b
 3f0:   0000040b    .word   0x0000040b
 3f4:   0000001c    .word   0x0000001c
 3f8:   00000000    .word   0x00000000
            3f8: R_ARM_ABS32    .debug_frame
 3fc:   000040fc    .word   0x000040fc
            3fc: R_ARM_ABS32    .text
 400:   000000d0    .word   0x000000d0
 404:   440c0d44    .word   0x440c0d44
 408:   038d028e    .word   0x038d028e
 40c:   0c44048b    .word   0x0c44048b
 410:   0000040b    .word   0x0000040b
 414:   0000001c    .word   0x0000001c
 418:   00000000    .word   0x00000000
            418: R_ARM_ABS32    .debug_frame
 41c:   000041cc    .word   0x000041cc
            41c: R_ARM_ABS32    .text
 420:   00000088    .word   0x00000088
 424:   440c0d44    .word   0x440c0d44
 428:   038d028e    .word   0x038d028e
 42c:   0c44048b    .word   0x0c44048b
 430:   0000040b    .word   0x0000040b
 434:   0000001c    .word   0x0000001c
 438:   00000000    .word   0x00000000
            438: R_ARM_ABS32    .debug_frame
 43c:   00004254    .word   0x00004254
            43c: R_ARM_ABS32    .text
 440:   00000028    .word   0x00000028
 444:   440c0d44    .word   0x440c0d44
 448:   038d028e    .word   0x038d028e
 44c:   0c44048b    .word   0x0c44048b
 450:   0000040b    .word   0x0000040b
Disassembly of section .debug_loc:

00000000 <.debug_loc>:
   0:   00000000    .word   0x00000000
   4:   00000004    .word   0x00000004
   8:   045d0001    .word   0x045d0001
   c:   0c000000    .word   0x0c000000
  10:   01000000    .word   0x01000000
  14:   000c5c00    .word   0x000c5c00
  18:   032c0000    .word   0x032c0000
  1c:   00020000    .word   0x00020000
  20:   0000047b    .word   0x0000047b
  24:   00000000    .word   0x00000000
  28:   032c0000    .word   0x032c0000
  2c:   03300000    .word   0x03300000
  30:   00010000    .word   0x00010000
  34:   0003305d    .word   0x0003305d
  38:   00033800    .word   0x00033800
  3c:   5c000100    .word   0x5c000100
  40:   00000338    .word   0x00000338
  44:   000007a4    .word   0x000007a4
  48:   047b0002    .word   0x047b0002
    ...
  54:   000007a4    .word   0x000007a4
  58:   000007a8    .word   0x000007a8
  5c:   a85d0001    .word   0xa85d0001
  60:   b0000007    .word   0xb0000007
  64:   01000007    .word   0x01000007
  68:   07b05c00    .word   0x07b05c00
  6c:   0b740000    .word   0x0b740000
  70:   00020000    .word   0x00020000
  74:   0000047b    .word   0x0000047b
  78:   00000000    .word   0x00000000
  7c:   0b740000    .word   0x0b740000
  80:   0b780000    .word   0x0b780000
  84:   00010000    .word   0x00010000
  88:   000b785d    .word   0x000b785d
  8c:   000b8000    .word   0x000b8000
  90:   5c000100    .word   0x5c000100
  94:   00000b80    .word   0x00000b80
  98:   000012c0    .word   0x000012c0
  9c:   047b0002    .word   0x047b0002
    ...
  a8:   000012c0    .word   0x000012c0
  ac:   000012c4    .word   0x000012c4
  b0:   c45d0001    .word   0xc45d0001
  b4:   cc000012    .word   0xcc000012
  b8:   01000012    .word   0x01000012
  bc:   12cc5c00    .word   0x12cc5c00
  c0:   13cc0000    .word   0x13cc0000
  c4:   00020000    .word   0x00020000
  c8:   0000047b    .word   0x0000047b
  cc:   00000000    .word   0x00000000
  d0:   13cc0000    .word   0x13cc0000
  d4:   13d00000    .word   0x13d00000
  d8:   00010000    .word   0x00010000
  dc:   0013d05d    .word   0x0013d05d
  e0:   0013d800    .word   0x0013d800
  e4:   5c000100    .word   0x5c000100
  e8:   000013d8    .word   0x000013d8
  ec:   000015f8    .word   0x000015f8
  f0:   047b0002    .word   0x047b0002
    ...
  fc:   000015f8    .word   0x000015f8
 100:   000015fc    .word   0x000015fc
 104:   fc5d0001    .word   0xfc5d0001
 108:   04000015    .word   0x04000015
 10c:   01000016    .word   0x01000016
 110:   16045c00    .word   0x16045c00
 114:   172c0000    .word   0x172c0000
 118:   00020000    .word   0x00020000
 11c:   0000047b    .word   0x0000047b
 120:   00000000    .word   0x00000000
 124:   172c0000    .word   0x172c0000
 128:   17300000    .word   0x17300000
 12c:   00010000    .word   0x00010000
 130:   0017305d    .word   0x0017305d
 134:   00173800    .word   0x00173800
 138:   5c000100    .word   0x5c000100
 13c:   00001738    .word   0x00001738
 140:   000017bc    .word   0x000017bc
 144:   047b0002    .word   0x047b0002
    ...
 150:   000017bc    .word   0x000017bc
 154:   000017c0    .word   0x000017c0
 158:   c05d0001    .word   0xc05d0001
 15c:   c8000017    .word   0xc8000017
 160:   01000017    .word   0x01000017
 164:   17c85c00    .word   0x17c85c00
 168:   18100000    .word   0x18100000
 16c:   00020000    .word   0x00020000
 170:   0000047b    .word   0x0000047b
 174:   00000000    .word   0x00000000
 178:   18100000    .word   0x18100000
 17c:   18140000    .word   0x18140000
 180:   00010000    .word   0x00010000
 184:   0018145d    .word   0x0018145d
 188:   00181c00    .word   0x00181c00
 18c:   5c000100    .word   0x5c000100
 190:   0000181c    .word   0x0000181c
 194:   00001834    .word   0x00001834
 198:   047b0002    .word   0x047b0002
    ...
 1a4:   00001834    .word   0x00001834
 1a8:   00001838    .word   0x00001838
 1ac:   385d0001    .word   0x385d0001
 1b0:   40000018    .word   0x40000018
 1b4:   01000018    .word   0x01000018
 1b8:   18405c00    .word   0x18405c00
 1bc:   184c0000    .word   0x184c0000
 1c0:   00020000    .word   0x00020000
 1c4:   0000047b    .word   0x0000047b
 1c8:   00000000    .word   0x00000000
 1cc:   184c0000    .word   0x184c0000
 1d0:   18500000    .word   0x18500000
 1d4:   00010000    .word   0x00010000
 1d8:   0018505d    .word   0x0018505d
 1dc:   00185800    .word   0x00185800
 1e0:   5c000100    .word   0x5c000100
 1e4:   00001858    .word   0x00001858
 1e8:   00001930    .word   0x00001930
 1ec:   047b0002    .word   0x047b0002
    ...
 1f8:   00001930    .word   0x00001930
 1fc:   00001934    .word   0x00001934
 200:   345d0001    .word   0x345d0001
 204:   3c000019    .word   0x3c000019
 208:   01000019    .word   0x01000019
 20c:   193c5c00    .word   0x193c5c00
 210:   1a140000    .word   0x1a140000
 214:   00020000    .word   0x00020000
 218:   0000047b    .word   0x0000047b
 21c:   00000000    .word   0x00000000
 220:   1a140000    .word   0x1a140000
 224:   1a180000    .word   0x1a180000
 228:   00010000    .word   0x00010000
 22c:   001a185d    .word   0x001a185d
 230:   001a2000    .word   0x001a2000
 234:   5c000100    .word   0x5c000100
 238:   00001a20    .word   0x00001a20
 23c:   00001ae8    .word   0x00001ae8
 240:   047b0002    .word   0x047b0002
    ...
 24c:   00001ae8    .word   0x00001ae8
 250:   00001aec    .word   0x00001aec
 254:   ec5d0001    .word   0xec5d0001
 258:   f400001a    .word   0xf400001a
 25c:   0100001a    .word   0x0100001a
 260:   1af45c00    .word   0x1af45c00
 264:   1c8c0000    .word   0x1c8c0000
 268:   00020000    .word   0x00020000
 26c:   0000047b    .word   0x0000047b
 270:   00000000    .word   0x00000000
 274:   1c8c0000    .word   0x1c8c0000
 278:   1c900000    .word   0x1c900000
 27c:   00010000    .word   0x00010000
 280:   001c905d    .word   0x001c905d
 284:   001c9800    .word   0x001c9800
 288:   5c000100    .word   0x5c000100
 28c:   00001c98    .word   0x00001c98
 290:   00001e14    .word   0x00001e14
 294:   047b0002    .word   0x047b0002
    ...
 2a0:   00001e14    .word   0x00001e14
 2a4:   00001e18    .word   0x00001e18
 2a8:   185d0001    .word   0x185d0001
 2ac:   2000001e    .word   0x2000001e
 2b0:   0100001e    .word   0x0100001e
 2b4:   1e205c00    .word   0x1e205c00
 2b8:   1f000000    .word   0x1f000000
 2bc:   00020000    .word   0x00020000
 2c0:   0000047b    .word   0x0000047b
 2c4:   00000000    .word   0x00000000
 2c8:   1f000000    .word   0x1f000000
 2cc:   1f040000    .word   0x1f040000
 2d0:   00010000    .word   0x00010000
 2d4:   001f045d    .word   0x001f045d
 2d8:   001f0c00    .word   0x001f0c00
 2dc:   5c000100    .word   0x5c000100
 2e0:   00001f0c    .word   0x00001f0c
 2e4:   00001f20    .word   0x00001f20
 2e8:   047b0002    .word   0x047b0002
    ...
 2f4:   00001f20    .word   0x00001f20
 2f8:   00001f24    .word   0x00001f24
 2fc:   245d0001    .word   0x245d0001
 300:   2c00001f    .word   0x2c00001f
 304:   0100001f    .word   0x0100001f
 308:   1f2c5c00    .word   0x1f2c5c00
 30c:   1fc40000    .word   0x1fc40000
 310:   00020000    .word   0x00020000
 314:   0000047b    .word   0x0000047b
 318:   00000000    .word   0x00000000
 31c:   1fc40000    .word   0x1fc40000
 320:   1fc80000    .word   0x1fc80000
 324:   00010000    .word   0x00010000
 328:   001fc85d    .word   0x001fc85d
 32c:   001fd000    .word   0x001fd000
 330:   5c000100    .word   0x5c000100
 334:   00001fd0    .word   0x00001fd0
 338:   00002018    .word   0x00002018
 33c:   047b0002    .word   0x047b0002
    ...
 348:   00002018    .word   0x00002018
 34c:   0000201c    .word   0x0000201c
 350:   1c5d0001    .word   0x1c5d0001
 354:   24000020    .word   0x24000020
 358:   01000020    .word   0x01000020
 35c:   20245c00    .word   0x20245c00
 360:   25bc0000    .word   0x25bc0000
 364:   00020000    .word   0x00020000
 368:   0000047b    .word   0x0000047b
 36c:   00000000    .word   0x00000000
 370:   25bc0000    .word   0x25bc0000
 374:   25c00000    .word   0x25c00000
 378:   00010000    .word   0x00010000
 37c:   0025c05d    .word   0x0025c05d
 380:   0025c800    .word   0x0025c800
 384:   5c000100    .word   0x5c000100
 388:   000025c8    .word   0x000025c8
 38c:   00002634    .word   0x00002634
 390:   047b0002    .word   0x047b0002
    ...
 39c:   00002634    .word   0x00002634
 3a0:   00002638    .word   0x00002638
 3a4:   385d0001    .word   0x385d0001
 3a8:   40000026    .word   0x40000026
 3ac:   01000026    .word   0x01000026
 3b0:   26405c00    .word   0x26405c00
 3b4:   27440000    .word   0x27440000
 3b8:   00020000    .word   0x00020000
 3bc:   0000047b    .word   0x0000047b
 3c0:   00000000    .word   0x00000000
 3c4:   27440000    .word   0x27440000
 3c8:   27480000    .word   0x27480000
 3cc:   00010000    .word   0x00010000
 3d0:   0027485d    .word   0x0027485d
 3d4:   00275000    .word   0x00275000
 3d8:   5c000100    .word   0x5c000100
 3dc:   00002750    .word   0x00002750
 3e0:   00002c1c    .word   0x00002c1c
 3e4:   047b0002    .word   0x047b0002
    ...
 3f0:   00002c1c    .word   0x00002c1c
 3f4:   00002c20    .word   0x00002c20
 3f8:   205d0001    .word   0x205d0001
 3fc:   2800002c    .word   0x2800002c
 400:   0100002c    .word   0x0100002c
 404:   2c285c00    .word   0x2c285c00
 408:   36880000    .word   0x36880000
 40c:   00020000    .word   0x00020000
 410:   0000047b    .word   0x0000047b
 414:   00000000    .word   0x00000000
 418:   36880000    .word   0x36880000
 41c:   368c0000    .word   0x368c0000
 420:   00010000    .word   0x00010000
 424:   00368c5d    .word   0x00368c5d
 428:   00369400    .word   0x00369400
 42c:   5c000100    .word   0x5c000100
 430:   00003694    .word   0x00003694
 434:   00003b6c    .word   0x00003b6c
 438:   047b0002    .word   0x047b0002
    ...
 444:   00003b6c    .word   0x00003b6c
 448:   00003b70    .word   0x00003b70
 44c:   705d0001    .word   0x705d0001
 450:   7800003b    .word   0x7800003b
 454:   0100003b    .word   0x0100003b
 458:   3b785c00    .word   0x3b785c00
 45c:   3db80000    .word   0x3db80000
 460:   00020000    .word   0x00020000
 464:   0000047b    .word   0x0000047b
 468:   00000000    .word   0x00000000
 46c:   3db80000    .word   0x3db80000
 470:   3dbc0000    .word   0x3dbc0000
 474:   00010000    .word   0x00010000
 478:   003dbc5d    .word   0x003dbc5d
 47c:   003dc400    .word   0x003dc400
 480:   5c000100    .word   0x5c000100
 484:   00003dc4    .word   0x00003dc4
 488:   00003e44    .word   0x00003e44
 48c:   047b0002    .word   0x047b0002
    ...
 498:   00003e44    .word   0x00003e44
 49c:   00003e48    .word   0x00003e48
 4a0:   485d0001    .word   0x485d0001
 4a4:   5000003e    .word   0x5000003e
 4a8:   0100003e    .word   0x0100003e
 4ac:   3e505c00    .word   0x3e505c00
 4b0:   3ed80000    .word   0x3ed80000
 4b4:   00020000    .word   0x00020000
 4b8:   0000047b    .word   0x0000047b
 4bc:   00000000    .word   0x00000000
 4c0:   3ed80000    .word   0x3ed80000
 4c4:   3edc0000    .word   0x3edc0000
 4c8:   00010000    .word   0x00010000
 4cc:   003edc5d    .word   0x003edc5d
 4d0:   003ee400    .word   0x003ee400
 4d4:   5c000100    .word   0x5c000100
 4d8:   00003ee4    .word   0x00003ee4
 4dc:   00003f98    .word   0x00003f98
 4e0:   047b0002    .word   0x047b0002
    ...
 4ec:   00003f98    .word   0x00003f98
 4f0:   00003f9c    .word   0x00003f9c
 4f4:   9c5d0001    .word   0x9c5d0001
 4f8:   a400003f    .word   0xa400003f
 4fc:   0100003f    .word   0x0100003f
 500:   3fa45c00    .word   0x3fa45c00
 504:   40fc0000    .word   0x40fc0000
 508:   00020000    .word   0x00020000
 50c:   0000047b    .word   0x0000047b
 510:   00000000    .word   0x00000000
 514:   40fc0000    .word   0x40fc0000
 518:   41000000    .word   0x41000000
 51c:   00010000    .word   0x00010000
 520:   0041005d    .word   0x0041005d
 524:   00410800    .word   0x00410800
 528:   5c000100    .word   0x5c000100
 52c:   00004108    .word   0x00004108
 530:   000041cc    .word   0x000041cc
 534:   047b0002    .word   0x047b0002
    ...
 540:   000041cc    .word   0x000041cc
 544:   000041d0    .word   0x000041d0
 548:   d05d0001    .word   0xd05d0001
 54c:   d8000041    .word   0xd8000041
 550:   01000041    .word   0x01000041
 554:   41d85c00    .word   0x41d85c00
 558:   42540000    .word   0x42540000
 55c:   00020000    .word   0x00020000
 560:   0000047b    .word   0x0000047b
 564:   00000000    .word   0x00000000
 568:   42540000    .word   0x42540000
 56c:   42580000    .word   0x42580000
 570:   00010000    .word   0x00010000
 574:   0042585d    .word   0x0042585d
 578:   00426000    .word   0x00426000
 57c:   5c000100    .word   0x5c000100
 580:   00004260    .word   0x00004260
 584:   0000427c    .word   0x0000427c
 588:   047b0002    .word   0x047b0002
    ...
Disassembly of section .debug_pubnames:

00000000 <.debug_pubnames>:
   0:   000001a5    andeq   r0, r0, r5, lsr #3
   4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_info
   8:   57200000    strpl   r0, [r0, -r0]!
   c:   438b0000    orrmi   r0, fp, #0  ; 0x0
  10:   474d0000    strbmi  r0, [sp, -r0]
  14:   72445f43    subvc   r5, r4, #268    ; 0x10c
  18:   696e4963    stmdbvs lr!, {r0, r1, r5, r6, r8, fp, lr}^
  1c:   44300074    ldrtmi  r0, [r0], #-116
  20:   474d0000    strbmi  r0, [sp, -r0]
  24:   72445f43    subvc   r5, r4, #268    ; 0x10c
  28:   72734963    rsbsvc  r4, r3, #1622016    ; 0x18c000
  2c:   00451c00    subeq   r1, r5, r0, lsl #24
  30:   43474d00    movtmi  r4, #32000  ; 0x7d00
  34:   6372445f    cmnvs   r2, #1593835520 ; 0x5f000000
  38:   646e6942    strbtvs r6, [lr], #-2370
  3c:   70646e45    rsbvc   r6, r4, r5, asr #28
  40:   746e696f    strbtvc r6, [lr], #-2415
  44:   0046ae00    subeq   sl, r6, r0, lsl #28
  48:   43474d00    movtmi  r4, #32000  ; 0x7d00
  4c:   6372445f    cmnvs   r2, #1593835520 ; 0x5f000000
  50:   76726553    undefined
  54:   44656369    strbtmi r6, [r5], #-873
  58:   75616665    strbvc  r6, [r1, #-1637]!
  5c:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
  60:   47c50064    strbmi  r0, [r5, r4, rrx]
  64:   474d0000    strbmi  r0, [sp, -r0]
  68:   72445f43    subvc   r5, r4, #268    ; 0x10c
  6c:   73655263    cmnvc   r5, #805306374  ; 0x30000006
  70:   6f507465    svcvs   0x00507465
  74:   b1007472    tstlt   r0, r2, ror r4
  78:   4d000048    stcmi   0, cr0, [r0, #-288]
  7c:   445f4347    ldrbmi  r4, [pc], #839  ; 84 <.debug_pubnames+0x84>
  80:   744f6372    strbvc  r6, [pc], #882  ; 88 <.debug_pubnames+0x88>
  84:   6d695467    cfstrdvs    mvd5, [r9, #-412]!
  88:   2a007265    bcs 1ca24 <MGC_DrcDumpPipe+0x187d0>
  8c:   4d00004e    stcmi   0, cr0, [r0, #-312]
  90:   445f4347    ldrbmi  r4, [pc], #839  ; 98 <.debug_pubnames+0x98>
  94:   68436372    stmdavs r3, {r1, r4, r5, r6, r8, r9, sp, lr}^
  98:   65676e61    strbvs  r6, [r7, #-3681]!
  9c:   5367744f    cmnpl   r7, #1325400064 ; 0x4f000000
  a0:   65746174    ldrbvs  r6, [r4, #-372]!
  a4:   004ea000    subeq   sl, lr, r0
  a8:   43474d00    movtmi  r4, #32000  ; 0x7d00
  ac:   6d6f435f    stclvs  3, cr4, [pc, #-380]!
  b0:   74656c70    strbtvc r6, [r5], #-3184
  b4:   67744f65    ldrbvs  r4, [r4, -r5, ror #30]!
  b8:   6e617254    mcrvs   2, 3, r7, cr1, cr4, {2}
  bc:   69746973    ldmdbvs r4!, {r0, r1, r4, r5, r6, r8, fp, sp, lr}^
  c0:   b0006e6f    andlt   r6, r0, pc, ror #28
  c4:   4d00004f    stcmi   0, cr0, [r0, #-316]
  c8:   445f4347    ldrbmi  r4, [pc], #839  ; d0 <.debug_pubnames+0xd0>
  cc:   73556372    cmpvc   r5, #-939524095 ; 0xc8000001
  d0:   72734962    rsbsvc  r4, r3, #1605632    ; 0x188000
  d4:   00509200    subseq  r9, r0, r0, lsl #4
  d8:   43474d00    movtmi  r4, #32000  ; 0x7d00
  dc:   6372445f    cmnvs   r2, #1593835520 ; 0x5f000000
  e0:   00727342    rsbseq  r7, r2, r2, asr #6
  e4:   000051d9    ldrdeq  r5, [r0], -r9
  e8:   5f43474d    svcpl   0x0043474d
  ec:   53637244    cmnpl   r3, #1073741828 ; 0x40000004
  f0:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
  f4:   6f486563    svcvs   0x00486563
  f8:   65447473    strbvs  r7, [r4, #-1139]
  fc:   6c756166    ldfvse  f6, [r5], #-408
 100:   52ad0074    adcpl   r0, sp, #116    ; 0x74
 104:   474d0000    strbmi  r0, [sp, -r0]
 108:   72445f43    subvc   r5, r4, #268    ; 0x10c
 10c:   63634163    cmnvs   r3, #-1073741800    ; 0xc0000018
 110:   44747065    ldrbtmi r7, [r4], #-101
 114:   63697665    cmnvs   r9, #105906176  ; 0x6500000
 118:   53360065    teqpl   r6, #101    ; 0x65
 11c:   474d0000    strbmi  r0, [sp, -r0]
 120:   72445f43    subvc   r5, r4, #268    ; 0x10c
 124:   6e694663    cdpvs   6, 6, cr4, cr9, cr3, {3}
 128:   52687369    rsbpl   r7, r8, #-1543503871    ; 0xa4000001
 12c:   6d757365    ldclvs  3, cr7, [r5, #-404]!
 130:   53850065    orrpl   r0, r5, #101    ; 0x65
 134:   474d0000    strbmi  r0, [sp, -r0]
 138:   72445f43    subvc   r5, r4, #268    ; 0x10c
 13c:   74655363    strbtvc r5, [r5], #-867
 140:   74736f48    ldrbtvc r6, [r3], #-3912
 144:   65776f50    ldrbvs  r6, [r7, #-3920]!
 148:   543c0072    ldrtpl  r0, [ip], #-114
 14c:   554d0000    strbpl  r0, [sp]
 150:   525f4253    subspl  r4, pc, #805306373  ; 0x30000005
 154:   73696765    cmnvc   r9, #26476544   ; 0x1940000
 158:   4f726574    svcmi   0x00726574
 15c:   6c436774    mcrrvs  7, 7, r6, r3, cr4
 160:   746e6569    strbtvc r6, [lr], #-1385
 164:   0054f600    subseq  pc, r4, r0, lsl #12
 168:   53554d00    cmppl   r5, #0  ; 0x0
 16c:   65525f42    ldrbvs  r5, [r2, #-3906]
 170:   716e696c    cmnvc   lr, ip, ror #18
 174:   68736975    ldmdavs r3!, {r0, r2, r4, r5, r6, r8, fp, sp, lr}^
 178:   74736f48    ldrbtvc r6, [r3], #-3912
 17c:   00555e00    subseq  r5, r5, r0, lsl #28
 180:   53554d00    cmppl   r5, #0  ; 0x0
 184:   65525f42    ldrbvs  r5, [r2, #-3906]
 188:   73657571    cmnvc   r5, #473956352  ; 0x1c400000
 18c:   73754274    cmnvc   r5, #1073741831 ; 0x40000007
 190:   0055a800    subseq  sl, r5, r0, lsl #16
 194:   43474d00    movtmi  r4, #32000  ; 0x7d00
 198:   6372445f    cmnvs   r2, #1593835520 ; 0x5f000000
 19c:   706d7544    rsbvc   r7, sp, r4, asr #10
 1a0:   65706950    ldrbvs  r6, [r0, #-2384]!
 1a4:   00000000    andeq   r0, r0, r0
    ...
Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
   0:   0000001c    andeq   r0, r0, ip, lsl r0
   4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_info
   8:   00040000    andeq   r0, r4, r0
    ...
            10: R_ARM_ABS32 .text
  14:   0000427c    andeq   r4, r0, ip, ror r2
    ...
Disassembly of section .debug_str:

00000000 <.debug_str>:
   0:   736f4870    cmnvc   pc, #7340032    ; 0x700000
   4:   696c4374    stmdbvs ip!, {r2, r4, r5, r6, r8, r9, lr}^
   8:   00746e65    rsbseq  r6, r4, r5, ror #28
   c:   6d6f4370    stclvs  3, cr4, [pc, #-448]!
  10:   74656c70    strbtvc r6, [r5], #-3184
  14:   72615065    rsbvc   r5, r1, #101    ; 0x65
  18:   70006d61    andvc   r6, r0, r1, ror #26
  1c:   4467744f    strbtmi r7, [r7], #-1103
  20:   00637365    rsbeq   r7, r3, r5, ror #6
  24:   73754262    cmnvc   r5, #536870918  ; 0x20000006
  28:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
  2c:   00737365    rsbseq  r7, r3, r5, ror #6
  30:   74537764    ldrbvc  r7, [r3], #-1892
  34:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
  38:   6e496200    cdpvs   2, 4, cr6, cr9, cr0, {0}
  3c:   76726574    undefined
  40:   62006c61    andvs   r6, r0, #24832  ; 0x6100
  44:   43727349    cmnmi   r2, #603979777  ; 0x24000001
  48:   626c6c61    rsbvs   r6, ip, #24832  ; 0x6100
  4c:   006b6361    rsbeq   r6, fp, r1, ror #6
  50:   6f734970    svcvs   0x00734970
  54:   72496863    subvc   r6, r9, #6488064    ; 0x630000
  58:   54620070    strbtpl r0, [r2], #-112
  5c:   66666172    undefined
  60:   79546369    ldmdbvc r4, {r0, r3, r5, r6, r8, r9, sp, lr}^
  64:   77006570    smlsdxvc    r0, r0, r5, r6
  68:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
  6c:   46700074    undefined
  70:   74636e75    strbtvc r6, [r3], #-3701
  74:   436e6f69    cmnmi   lr, #420    ; 0x1a4
  78:   6e65696c    cdpvs   9, 6, cr6, cr5, cr12, {3}
  7c:   50700074    rsbspl  r0, r0, r4, ror r0
  80:   6d617261    sfmvs   f7, 2, [r1, #-388]!
  84:   6f4c7000    svcvs   0x004c7000
  88:   456c6163    strbmi  r6, [ip, #-355]!
  8c:   7700646e    strvc   r6, [r0, -lr, ror #8]
  90:   6b636150    blvs    18d85d8 <MGC_DrcDumpPipe+0x18d4384>
  94:   69537465    ldmdbvs r3, {r0, r2, r5, r6, sl, ip, sp, lr}^
  98:   6100657a    tstvs   r0, sl, ror r5
  9c:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
  a0:   44676966    strbtmi r6, [r7], #-2406
  a4:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
  a8:   6f747069    svcvs   0x00747069
  ac:   62007372    andvs   r7, r0, #-939524095 ; 0xc8000001
  b0:   6c437349    mcrrvs  3, 4, r7, r3, cr9
  b4:   656d6961    strbvs  r6, [sp, #-2401]!
  b8:   49770064    ldmdbmi r7!, {r2, r5, r6}^
  bc:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
  c0:   65527000    ldrbvs  r7, [r2]
  c4:   73657571    cmnvc   r5, #473956352  ; 0x1c400000
  c8:   77640074    undefined
  cc:   43646e45    cmnmi   r4, #1104   ; 0x450
  d0:   746e756f    strbtvc r7, [lr], #-1391
  d4:   64416200    strbvs  r6, [r1], #-512
  d8:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
  dc:   54770073    ldrbtpl r0, [r7], #-115
  e0:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
  e4:   744f7000    strbvc  r7, [pc], #0    ; ec <.debug_str+0xec>
  e8:   696c4367    stmdbvs ip!, {r0, r1, r2, r5, r6, r8, r9, lr}^
  ec:   00746e65    rsbseq  r6, r4, r5, ror #28
  f0:   6c6c4162    stfvse  f4, [ip], #-392
  f4:   6d44776f    stclvs  7, cr7, [r4, #-444]
  f8:   46700061    ldrbtmi r0, [r0], -r1, rrx
  fc:   446f6669    strbtmi r6, [pc], #1641 ; 104 <.debug_str+0x104>
 100:   00747365    rsbseq  r7, r4, r5, ror #6
 104:   69725070    ldmdbvs r2!, {r4, r5, r6, ip, lr}^
 108:   65746176    ldrbvs  r6, [r4, #-374]!
 10c:   61746144    cmnvs   r4, r4, asr #2
 110:   6f437000    svcvs   0x00437000
 114:   6f72746e    svcvs   0x0072746e
 118:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
 11c:   614d7700    cmpvs   sp, r0, lsl #14
 120:   63615078    cmnvs   r1, #120    ; 0x78
 124:   5374656b    cmnpl   r4, #448790528  ; 0x1ac00000
 128:   00657a69    rsbeq   r7, r5, r9, ror #20
 12c:   6e654c62    cdpvs   12, 6, cr4, cr5, cr2, {3}
 130:   00687467    rsbeq   r7, r8, r7, ror #8
 134:   72655677    rsbvc   r5, r5, #124780544  ; 0x7700000
 138:   6e6f6973    mcrvs   9, 3, r6, cr15, cr3, {3}
 13c:   73496200    movtvc  r6, #37376  ; 0x9200
 140:   65736552    ldrbvs  r6, [r3, #-1362]!
 144:   44620074    strbtmi r0, [r2], #-116
 148:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
 14c:   6f747069    svcvs   0x00747069
 150:   70795472    rsbsvc  r5, r9, r2, ror r4
 154:   77640065    strbvc  r0, [r4, -r5, rrx]!
 158:   75746341    ldrbvc  r6, [r4, #-833]!
 15c:   654c6c61    strbvs  r6, [ip, #-3169]
 160:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
 164:   69467700    stmdbvs r6, {r8, r9, sl, ip, sp, lr}^
 168:   6f436f66    svcvs   0x00436f66
 16c:   00746e75    rsbseq  r6, r4, r5, ror lr
 170:   636f4c62    cmnvs   pc, #25088  ; 0x6200
 174:   6e456c61    cdpvs   12, 4, cr6, cr5, cr1, {3}
 178:   50700064    rsbspl  r0, r0, r4, rrx
 17c:   0074726f    rsbseq  r7, r4, pc, ror #4
 180:   76654470    undefined
 184:   00656369    rsbeq   r6, r5, r9, ror #6
 188:   65706950    ldrbvs  r6, [r0, #-2384]!
 18c:   7473694c    ldrbtvc r6, [r3], #-2380
 190:   65447700    strbvs  r7, [r4, #-1792]
 194:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
 198:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
 19c:   66667542    strbtvs r7, [r6], -r2, asr #10
 1a0:   654c7265    strbvs  r7, [ip, #-613]
 1a4:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
 1a8:   72447000    subvc   r7, r4, #0  ; 0x0
 1ac:   72657669    rsbvc   r7, r5, #110100480  ; 0x6900000
 1b0:   416d6200    cmnmi   sp, r0, lsl #4
 1b4:   69727474    ldmdbvs r2!, {r2, r4, r5, r6, sl, ip, sp, lr}^
 1b8:   65747562    ldrbvs  r7, [r4, #-1378]!
 1bc:   53620073    cmnpl   r2, #115    ; 0x73
 1c0:   75746174    ldrbvc  r6, [r4, #-372]!
 1c4:   66700073    undefined
 1c8:   43707249    cmnmi   r0, #-1879048188    ; 0x90000004
 1cc:   6c706d6f    ldclvs  13, cr6, [r0], #-444
 1d0:   00657465    rsbeq   r7, r5, r5, ror #8
 1d4:   72655370    rsbvc   r5, r5, #-1073741823    ; 0xc0000001
 1d8:   65636976    strbvs  r6, [r3, #-2422]!
 1dc:   77640073    undefined
 1e0:   49646e45    stmdbmi r4!, {r0, r2, r6, r9, sl, fp, sp, lr}^
 1e4:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
    ...
Disassembly of section .comment:

00000000 <.comment>:
   0:   43434700    movtmi  r4, #14080  ; 0x3700
   4:   4728203a    undefined
   8:   2029554e    eorcs   r5, r9, lr, asr #10
   c:   2e322e34    mrccs   14, 1, r2, cr2, cr4, {1}
  10:   Address 0x00000010 is out of bounds.

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:   00000f41    andeq   r0, r0, r1, asr #30
   4:   61656100    cmnvs   r5, r0, lsl #2
   8:   01006962    tsteq   r0, r2, ror #18
   c:   00000005    andeq   r0, r0, r5

#endif

