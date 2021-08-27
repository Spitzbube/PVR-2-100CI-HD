
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


#if 0
 The File Name Table:
  Entry Dir Time    Size    Name
  1 1   0   0   mu_hdr.c
  2 2   0   0   stddef.h
  3 3   0   0   stdint.h
  4 4   0   0   mu_dsi.h
  5 4   0   0   mu_dev.h
  6 4   0   0   mu_cdi.h
  7 4   0   0   mu_list.h
  8 4   0   0   mu_hset.h
  9 4   0   0   mu_dci.h
  10    1   0   0   mu_impl.h
  11    5   0   0   sys_driver.h
  12    5   0   0   sys_services.h
#endif

#if 0
  16076       MGC_HdrcInit
  16192       MGC_HdrcDynamicFifoLocation
  16365       MGC_HdrcIsr
  16588       MGC_HdrcStart
  16689       MGC_HdrcStop
  16775       MGC_HdrcDestroy
  16848       MGC_HdrcReadBusState
  16992       MGC_HdrcProgramBusState
  17175       MGC_HdrcBindEndpoint
  17431       MGC_HdrcStartRx
  17771       MGC_HdrcStartTx
  18154       MGC_HdrcFlushEndpoint
  18343       MGC_HdrcHaltEndpoint
  18514       MGC_HdrcDefaultEndResponse
  18683       MGC_HdrcServiceDefaultEnd
  18901       MGC_HdrcServiceTransmitAvail
  19108       MGC_HdrcServiceReceiveReady
  19566       MGC_HdrcSetPortTestMode
  19707       MGC_HdrcLoadFifo
  19928       MGC_HdrcUnloadFifo
  20130       MGC_HdrcUlpiVbusControl
  20289       MGC_HdrcReadUlpiReg
  20447       MGC_HdrcWriteUlpiReg
  20605       MGC_HdrcDmaChannelStatusChanged
  20904       MGC_HdrcDumpState
  20964       MGC_HdrcDumpEndpoint
#endif


extern uint8_t MGC_Read8(uint8_t* baseAddr, uint16_t offset);
extern uint16_t MGC_Read16(uint8_t* baseAddr, uint16_t offset);
extern void MGC_Write8(uint8_t* baseAddr, uint16_t offset, uint8_t);
extern void MGC_Write16(uint8_t* baseAddr, uint16_t offset, uint16_t);
extern uint8_t MGC_DrcInit(MGC_Port* pPort);
extern int MGC_DrcIsr(MGC_Controller*, uint8_t, uint16_t, uint16_t);


/* 21ccba9c - complete */
/* v3.8: func_4923b8 */
uint8_t MGC_HdrcInit(MGC_Port* pPort)
{
   uint8_t config; /*147*/ /*fp21*/
   MGC_Controller* pImplController = pPort->pController; /*fp20*/
   uint8_t* pBase = pImplController->pControllerAddressIst; /*fp16*/

   /* log core options */
   MGC_SelectEnd(pBase, 0);

   config = MGC_ReadCsr8(pBase, MGC_O_HDRC_CONFIGDATA, 0);

   if (config & MGC_M_CONFIGDATA_MPRXE)
   {
      pPort->bHasBulkCombine = 1;
      //"bulk combine"
   }
   if (config & MGC_M_CONFIGDATA_MPTXE)
   {
      pPort->bHasBulkSplit = 1;
      //"bulk split"
   }
   if (config & MGC_M_CONFIGDATA_HBRXE)
   {
      pPort->bHasHbIsoRx = 1;
      //"HB-ISO Rx"
   }
   if (config & MGC_M_CONFIGDATA_HBTXE)
   {
      pPort->bHasHbIsoTx = 1;
      //"HB-ISO Tx"
   }
   if (config & MGC_M_CONFIGDATA_DYNFIFO)
   {
      //ERR("Dynamic FIFOs detected in hardware; please rebuild\n");
      return 0;
   }

   return MGC_DrcInit(pPort);
}


/* 21ccba24 - complete */
int MGC_HdrcIsr(/*MUSB_Controller* a*/void* pParam/*fp44*/) /*312*/
{
   int result; /*fp40*/
   uint8_t bIntrUsbValue; /*fp33*/
   uint16_t wIntrTxValue; /*fp32*/
   uint16_t wIntrRxValue; /*fp30*/
   MUSB_Controller* pController = pParam; /*fp28*/
   MGC_Controller* pControllerImpl = pController->pPrivateData; /*fp24*/
   uint8_t* pBase = pControllerImpl->pControllerAddressIst; /*fp20*/
   uint8_t bIndex; /*fp13*/

   bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);

   bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
   wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
   wIntrRxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

   result = MGC_DrcIsr(pControllerImpl, bIntrUsbValue, wIntrTxValue, wIntrRxValue);

   MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);

   return result;
}


/* 21cc9e18 - complete */
uint32_t MGC_HdrcStart(MGC_Controller* pController/*fp28*/) /*357*/
{
    uint16_t val; /*fp22*/
    uint8_t* pBase = pController->pControllerAddressIsr; /*fp20*/
    MGC_Port* pPort = pController->pPort; /*fp16*/

    val = pPort->wEndMask;

   MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, val);

   val = pPort->wEndMask & ~1;

   MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, val);

   val = 0xF7; //1111 0111

   MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, val);

   pPort->bOtgState = 0;

   return 0;
}


/* 21cc9db4 - complete */
uint32_t MGC_HdrcStop(MGC_Controller* pController) /*387*/
{
   uint8_t* pBase = pController->pControllerAddressIst;

   MGC_Write8(pBase, MGC_O_HDRC_INTRUSBE, 0);
   MGC_Write16(pBase, MGC_O_HDRC_INTRTXE, 0);
   MGC_Write16(pBase, MGC_O_HDRC_INTRRXE, 0);
   MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);
   MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
   MGC_Read16(pBase, MGC_O_HDRC_INTRRX);

   return 0;
}



#if 0

mu_hdr-g.o:     file format elf32-littlearm
rw-rw-rw- 1007/513  54960 Nov 17 16:29 2010 mu_hdr-g.o
architecture: arm, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x00000000
private flags = 204: [interworking enabled] [APCS-32] [FPA float format] [software FP]

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         0000492c  00000000  00000000  00000034  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  00000000  00000000  00004960  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  00000000  00000000  00004960  2**0
                  ALLOC
  3 .debug_abbrev 00000264  00000000  00000000  00004960  2**0
                  CONTENTS, READONLY, DEBUGGING
  4 .debug_info   000052e6  00000000  00000000  00004bc4  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  5 .debug_line   0000057d  00000000  00000000  00009eaa  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
  6 .rodata       0000021c  00000000  00000000  0000a428  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .debug_frame  00000398  00000000  00000000  0000a644  2**2
                  CONTENTS, RELOC, READONLY, DEBUGGING
  8 .debug_loc    00000498  00000000  00000000  0000a9dc  2**0
                  CONTENTS, READONLY, DEBUGGING
  9 .debug_pubnames 00000299  00000000  00000000  0000ae74  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 10 .debug_aranges 00000020  00000000  00000000  0000b10d  2**0
                  CONTENTS, RELOC, READONLY, DEBUGGING
 11 .debug_str    000001e3  00000000  00000000  0000b12d  2**0
                  CONTENTS, READONLY, DEBUGGING
 12 .comment      00000012  00000000  00000000  0000b310  2**0
                  CONTENTS, READONLY
 13 .ARM.attributes 00000010  00000000  00000000  0000b322  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00000000 l    df *ABS*  00000000 mu_hdr.c
00000000 l    d  .text  00000000 .text
00000000 l    d  .data  00000000 .data
00000000 l    d  .bss   00000000 .bss
00000000 l    d  .debug_abbrev  00000000 .debug_abbrev
00000000 l    d  .debug_info    00000000 .debug_info
00000000 l    d  .debug_line    00000000 .debug_line
00000000 l    d  .rodata    00000000 .rodata
00000000 l     O .rodata    00000035 MGC_aTestPacket
00003b14 l     F .text  00000064 MGC_HdrcResetOff
00003b78 l     F .text  00000064 MGC_HdrcResumeOff
00000000 l    d  .debug_frame   00000000 .debug_frame
00000000 l    d  .debug_loc 00000000 .debug_loc
00000000 l    d  .debug_pubnames    00000000 .debug_pubnames
00000000 l    d  .debug_aranges 00000000 .debug_aranges
00000000 l    d  .debug_str 00000000 .debug_str
00000000 l    d  .comment   00000000 .comment
00000000 l    d  .ARM.attributes    00000000 .ARM.attributes
00000000 g     F .text  00000138 MGC_HdrcInit
00000000         *UND*  00000000 MGC_FAPI_WriteReg8
00000000         *UND*  00000000 MGC_FAPI_ReadReg8
00000000         *UND*  00000000 MGC_DrcInit
00000000         *UND*  00000000 MGC_bDiagLevel
00000138 g     F .text  00000038 MGC_HdrcDynamicFifoLocation
00000170 g     F .text  000000cc MGC_HdrcIsr
00000000         *UND*  00000000 MGC_FAPI_ReadReg16
00000000         *UND*  00000000 MGC_DrcIsr
0000023c g     F .text  00000254 MGC_HdrcStart
00000000         *UND*  00000000 MGC_FAPI_WriteReg16
00000000         *UND*  00000000 MGC_DiagMsg
00000490 g     F .text  0000009c MGC_HdrcStop
0000052c g     F .text  0000006c MGC_HdrcDestroy
00000000         *UND*  00000000 MGC_HostDestroy
00000000         *UND*  00000000 MGC_FunctionDestroy
00000000         *UND*  00000000 FAPI_SYS_Services
00000598 g     F .text  000001f0 MGC_HdrcReadBusState
00000788 g     F .text  000003d8 MGC_HdrcProgramBusState
00000000         *UND*  00000000 __udivsi3
00000b60 g     F .text  00000570 MGC_HdrcBindEndpoint
00000000         *UND*  00000000 MGC_DrcBindEndpoint
000010d0 g     F .text  000009dc MGC_HdrcStartRx
00000000         *UND*  00000000 MUSB_StringConcat
00000000         *UND*  00000000 MUSB_Stringize
00000000         *UND*  00000000 MUSB_PrintLine
00000000         *UND*  00000000 MGC_PipePacketReceived
00000000         *UND*  00000000 MGC_CompleteIrp
00000000         *UND*  00000000 MGC_DiagNumber
00001aac g     F .text  000004d0 MGC_HdrcStartTx
00001f7c g     F .text  00000398 MGC_HdrcFlushEndpoint
00002314 g     F .text  000002b8 MGC_HdrcHaltEndpoint
00000000         *UND*  00000000 MGC_StartNextIrp
000025cc g     F .text  00000250 MGC_HdrcDefaultEndResponse
00003f08 g     F .text  00000134 MGC_HdrcLoadFifo
0000281c g     F .text  00000754 MGC_HdrcServiceDefaultEnd
00000000         *UND*  00000000 MUSB_ArrayFetch
00000000         *UND*  00000000 MGC_StartNextControlTransfer
00000000         *UND*  00000000 MGC_DrcServiceDefaultEnd
00002f70 g     F .text  00000530 MGC_HdrcServiceTransmitAvail
00000000         *UND*  00000000 MGC_PipeTransmitReady
000034a0 g     F .text  00000674 MGC_HdrcServiceReceiveReady
00003bdc g     F .text  0000032c MGC_HdrcSetPortTestMode
00000000         *UND*  00000000 MGC_FAPI_WriteReg32
0000403c g     F .text  00000138 MGC_HdrcUnloadFifo
00000000         *UND*  00000000 MGC_FAPI_ReadReg32
00004174 g     F .text  000000e0 MGC_HdrcUlpiVbusControl
00004254 g     F .text  00000174 MGC_HdrcReadUlpiReg
00000000         *UND*  00000000 FAPI_TIMER_GetTimeStamp
000043c8 g     F .text  00000174 MGC_HdrcWriteUlpiReg
0000453c g     F .text  000003a8 MGC_HdrcDmaChannelStatusChanged
000048e4 g     F .text  00000020 MGC_HdrcDumpState
00004904 g     F .text  00000028 MGC_HdrcDumpEndpoint


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
   9      DW_TAG_subroutine_type    [has children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
    DW_AT_sibling      DW_FORM_ref4
   10      DW_TAG_formal_parameter    [no children]
    DW_AT_type         DW_FORM_ref4
   11      DW_TAG_subroutine_type    [has children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_sibling      DW_FORM_ref4
   12      DW_TAG_structure_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   13      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   14      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   15      DW_TAG_subroutine_type    [no children]
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_type         DW_FORM_ref4
   16      DW_TAG_const_type    [no children]
   17      DW_TAG_typedef    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
   18      DW_TAG_structure_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_sibling      DW_FORM_ref4
   19      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   20      DW_TAG_member    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_data_member_location DW_FORM_block1
   21      DW_TAG_enumeration_type    [has children]
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   22      DW_TAG_enumerator    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_const_value  DW_FORM_sdata
   23      DW_TAG_structure_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_sibling      DW_FORM_ref4
   24      DW_TAG_structure_type    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_byte_size    DW_FORM_data1
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
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
   34      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   35      DW_TAG_subprogram    [has children]
    DW_AT_external     DW_FORM_flag
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   36      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   37      DW_TAG_subprogram    [has children]
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
   38      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   39      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   40      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_strp
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   41      DW_TAG_formal_parameter    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_location     DW_FORM_block1
   42      DW_TAG_subprogram    [has children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   43      DW_TAG_subprogram    [has children]
    DW_AT_external     DW_FORM_flag
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_prototyped   DW_FORM_flag
    DW_AT_low_pc       DW_FORM_addr
    DW_AT_high_pc      DW_FORM_addr
    DW_AT_frame_base   DW_FORM_data4
    DW_AT_sibling      DW_FORM_ref4
   44      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data1
    DW_AT_type         DW_FORM_ref4
    DW_AT_external     DW_FORM_flag
    DW_AT_declaration  DW_FORM_flag
   45      DW_TAG_variable    [no children]
    DW_AT_name         DW_FORM_string
    DW_AT_decl_file    DW_FORM_data1
    DW_AT_decl_line    DW_FORM_data2
    DW_AT_type         DW_FORM_ref4
    DW_AT_external     DW_FORM_flag
    DW_AT_declaration  DW_FORM_flag

The section .debug_info contains:

  Compilation Unit @ offset 0x0:
   Length:        21218
   Version:       2
   Abbrev Offset: 0
   Pointer Size:  4
 <0><b>: Abbrev Number: 1 (DW_TAG_compile_unit)
  < c>     DW_AT_producer    : GNU C 4.2.2
  <18>     DW_AT_language    : 1    (ANSI C)
  <19>     DW_AT_name        : ../cd/mu_hdr.c
  <28>     DW_AT_comp_dir    : /cygdrive/d/PROJECTS/MB86H60/software/HDTV_branch/subsystems/bus/musb/src/systems
  <7a>     DW_AT_low_pc      : 0
  <7e>     DW_AT_high_pc     : 0x492c
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
 <1><121>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <122>     DW_AT_prototyped  : 1
  <123>     DW_AT_type        : <a9>
  <127>     DW_AT_sibling     : <131>
 <2><12b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <12c>     DW_AT_type        : <f1>
 <1><131>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <132>     DW_AT_byte_size   : 4
  <133>     DW_AT_type        : <121>
 <1><137>: Abbrev Number: 3 (DW_TAG_base_type)
  <138>     DW_AT_byte_size   : 2
  <139>     DW_AT_encoding    : 7   (unsigned)
  <13a>     DW_AT_name        : short unsigned int
 <1><14d>: Abbrev Number: 3 (DW_TAG_base_type)
  <14e>     DW_AT_byte_size   : 8
  <14f>     DW_AT_encoding    : 7   (unsigned)
  <150>     DW_AT_name        : long long unsigned int
 <1><167>: Abbrev Number: 3 (DW_TAG_base_type)
  <168>     DW_AT_byte_size   : 1
  <169>     DW_AT_encoding    : 6   (signed char)
  <16a>     DW_AT_name        : signed char
 <1><176>: Abbrev Number: 2 (DW_TAG_typedef)
  <177>     DW_AT_name        : uint8_t
  <17f>     DW_AT_decl_file   : 3
  <180>     DW_AT_decl_line   : 46
  <181>     DW_AT_type        : <e0>
 <1><185>: Abbrev Number: 2 (DW_TAG_typedef)
  <186>     DW_AT_name        : uint16_t
  <18f>     DW_AT_decl_file   : 3
  <190>     DW_AT_decl_line   : 58
  <191>     DW_AT_type        : <137>
 <1><195>: Abbrev Number: 2 (DW_TAG_typedef)
  <196>     DW_AT_name        : int32_t
  <19e>     DW_AT_decl_file   : 3
  <19f>     DW_AT_decl_line   : 83
  <1a0>     DW_AT_type        : <b0>
 <1><1a4>: Abbrev Number: 2 (DW_TAG_typedef)
  <1a5>     DW_AT_name        : uint32_t
  <1ae>     DW_AT_decl_file   : 3
  <1af>     DW_AT_decl_line   : 84
  <1b0>     DW_AT_type        : <94>
 <1><1b4>: Abbrev Number: 2 (DW_TAG_typedef)
  <1b5>     DW_AT_name        : uint64_t
  <1be>     DW_AT_decl_file   : 3
  <1bf>     DW_AT_decl_line   : 124
  <1c0>     DW_AT_type        : <14d>
 <1><1c4>: Abbrev Number: 2 (DW_TAG_typedef)
  <1c5>     DW_AT_name        : uint_fast16_t
  <1d3>     DW_AT_decl_file   : 3
  <1d4>     DW_AT_decl_line   : 170
  <1d5>     DW_AT_type        : <cd>
 <1><1d9>: Abbrev Number: 2 (DW_TAG_typedef)
  <1da>     DW_AT_name        : MUSB_pfControllerIsr
  <1ef>     DW_AT_decl_file   : 4
  <1f0>     DW_AT_decl_line   : 64
  <1f1>     DW_AT_type        : <131>
 <1><1f5>: Abbrev Number: 2 (DW_TAG_typedef)
  <1f6>     DW_AT_name        : MUSB_pfControllerBsr
  <20b>     DW_AT_decl_file   : 4
  <20c>     DW_AT_decl_line   : 75
  <20d>     DW_AT_type        : <211>
 <1><211>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <212>     DW_AT_byte_size   : 4
  <213>     DW_AT_type        : <217>
 <1><217>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <218>     DW_AT_prototyped  : 1
  <219>     DW_AT_sibling     : <223>
 <2><21d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <21e>     DW_AT_type        : <f1>
 <1><223>: Abbrev Number: 2 (DW_TAG_typedef)
  <224>     DW_AT_name        : MUSB_pfTimerExpired
  <238>     DW_AT_decl_file   : 4
  <239>     DW_AT_decl_line   : 88
  <23a>     DW_AT_type        : <23e>
 <1><23e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <23f>     DW_AT_byte_size   : 4
  <240>     DW_AT_type        : <244>
 <1><244>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <245>     DW_AT_prototyped  : 1
  <246>     DW_AT_sibling     : <255>
 <2><24a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <24b>     DW_AT_type        : <f1>
 <2><24f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <250>     DW_AT_type        : <185>
 <1><255>: Abbrev Number: 12 (DW_TAG_structure_type)
  <256>     DW_AT_byte_size   : 40
  <257>     DW_AT_decl_file   : 4
  <258>     DW_AT_decl_line   : 126
  <259>     DW_AT_sibling     : <32b>
 <2><25d>: Abbrev Number: 13 (DW_TAG_member)
  <25e>     DW_AT_name        : (indirect string, offset: 0x90): wVersion
  <262>     DW_AT_decl_file   : 4
  <263>     DW_AT_decl_line   : 127
  <264>     DW_AT_type        : <185>
  <268>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><26b>: Abbrev Number: 13 (DW_TAG_member)
  <26c>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <270>     DW_AT_decl_file   : 4
  <271>     DW_AT_decl_line   : 128
  <272>     DW_AT_type        : <f1>
  <276>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><279>: Abbrev Number: 14 (DW_TAG_member)
  <27a>     DW_AT_name        : wQueueLength
  <287>     DW_AT_decl_file   : 4
  <288>     DW_AT_decl_line   : 129
  <289>     DW_AT_type        : <185>
  <28d>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><290>: Abbrev Number: 14 (DW_TAG_member)
  <291>     DW_AT_name        : wQueueItemSize
  <2a0>     DW_AT_decl_file   : 4
  <2a1>     DW_AT_decl_line   : 130
  <2a2>     DW_AT_type        : <185>
  <2a6>     DW_AT_data_member_location: 2 byte block: 23 a  (DW_OP_plus_uconst: 10)
 <2><2a9>: Abbrev Number: 14 (DW_TAG_member)
  <2aa>     DW_AT_name        : wTimerCount
  <2b6>     DW_AT_decl_file   : 4
  <2b7>     DW_AT_decl_line   : 131
  <2b8>     DW_AT_type        : <185>
  <2bc>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><2bf>: Abbrev Number: 13 (DW_TAG_member)
  <2c0>     DW_AT_name        : (indirect string, offset: 0x15c): adwTimerResolutions
  <2c4>     DW_AT_decl_file   : 4
  <2c5>     DW_AT_decl_line   : 132
  <2c6>     DW_AT_type        : <32b>
  <2ca>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><2cd>: Abbrev Number: 14 (DW_TAG_member)
  <2ce>     DW_AT_name        : wLockCount
  <2d9>     DW_AT_decl_file   : 4
  <2da>     DW_AT_decl_line   : 133
  <2db>     DW_AT_type        : <185>
  <2df>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><2e2>: Abbrev Number: 14 (DW_TAG_member)
  <2e3>     DW_AT_name        : pfIsr
  <2e9>     DW_AT_decl_file   : 4
  <2ea>     DW_AT_decl_line   : 134
  <2eb>     DW_AT_type        : <1d9>
  <2ef>     DW_AT_data_member_location: 2 byte block: 23 18     (DW_OP_plus_uconst: 24)
 <2><2f2>: Abbrev Number: 14 (DW_TAG_member)
  <2f3>     DW_AT_name        : pIsrParam
  <2fd>     DW_AT_decl_file   : 4
  <2fe>     DW_AT_decl_line   : 135
  <2ff>     DW_AT_type        : <f1>
  <303>     DW_AT_data_member_location: 2 byte block: 23 1c     (DW_OP_plus_uconst: 28)
 <2><306>: Abbrev Number: 14 (DW_TAG_member)
  <307>     DW_AT_name        : pfBsr
  <30d>     DW_AT_decl_file   : 4
  <30e>     DW_AT_decl_line   : 136
  <30f>     DW_AT_type        : <1f5>
  <313>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <2><316>: Abbrev Number: 14 (DW_TAG_member)
  <317>     DW_AT_name        : pBsrParam
  <321>     DW_AT_decl_file   : 4
  <322>     DW_AT_decl_line   : 137
  <323>     DW_AT_type        : <f1>
  <327>     DW_AT_data_member_location: 2 byte block: 23 24     (DW_OP_plus_uconst: 36)
 <1><32b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <32c>     DW_AT_byte_size   : 4
  <32d>     DW_AT_type        : <331>
 <1><331>: Abbrev Number: 8 (DW_TAG_const_type)
  <332>     DW_AT_type        : <1a4>
 <1><336>: Abbrev Number: 2 (DW_TAG_typedef)
  <337>     DW_AT_name        : MUSB_Controller
  <347>     DW_AT_decl_file   : 4
  <348>     DW_AT_decl_line   : 138
  <349>     DW_AT_type        : <255>
 <1><34d>: Abbrev Number: 2 (DW_TAG_typedef)
  <34e>     DW_AT_name        : MUSB_pfMessageString
  <363>     DW_AT_decl_file   : 4
  <364>     DW_AT_decl_line   : 154
  <365>     DW_AT_type        : <369>
 <1><369>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <36a>     DW_AT_byte_size   : 4
  <36b>     DW_AT_type        : <36f>
 <1><36f>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <370>     DW_AT_prototyped  : 1
  <371>     DW_AT_type        : <176>
  <375>     DW_AT_sibling     : <389>
 <2><379>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <37a>     DW_AT_type        : <108>
 <2><37e>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <37f>     DW_AT_type        : <185>
 <2><383>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <384>     DW_AT_type        : <116>
 <1><389>: Abbrev Number: 2 (DW_TAG_typedef)
  <38a>     DW_AT_name        : MUSB_pfMessageNumber
  <39f>     DW_AT_decl_file   : 4
  <3a0>     DW_AT_decl_line   : 174
  <3a1>     DW_AT_type        : <3a5>
 <1><3a5>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3a6>     DW_AT_byte_size   : 4
  <3a7>     DW_AT_type        : <3ab>
 <1><3ab>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3ac>     DW_AT_prototyped  : 1
  <3ad>     DW_AT_type        : <176>
  <3b1>     DW_AT_sibling     : <3cf>
 <2><3b5>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3b6>     DW_AT_type        : <108>
 <2><3ba>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3bb>     DW_AT_type        : <185>
 <2><3bf>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3c0>     DW_AT_type        : <1a4>
 <2><3c4>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3c5>     DW_AT_type        : <176>
 <2><3c9>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3ca>     DW_AT_type        : <176>
 <1><3cf>: Abbrev Number: 2 (DW_TAG_typedef)
  <3d0>     DW_AT_name        : MUSB_pfGetTime
  <3df>     DW_AT_decl_file   : 4
  <3e0>     DW_AT_decl_line   : 184
  <3e1>     DW_AT_type        : <3e5>
 <1><3e5>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e6>     DW_AT_byte_size   : 4
  <3e7>     DW_AT_type        : <3eb>
 <1><3eb>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <3ec>     DW_AT_prototyped  : 1
  <3ed>     DW_AT_type        : <1a4>
 <1><3f1>: Abbrev Number: 12 (DW_TAG_structure_type)
  <3f2>     DW_AT_byte_size   : 16
  <3f3>     DW_AT_decl_file   : 4
  <3f4>     DW_AT_decl_line   : 208
  <3f5>     DW_AT_sibling     : <450>
 <2><3f9>: Abbrev Number: 13 (DW_TAG_member)
  <3fa>     DW_AT_name        : (indirect string, offset: 0x90): wVersion
  <3fe>     DW_AT_decl_file   : 4
  <3ff>     DW_AT_decl_line   : 209
  <400>     DW_AT_type        : <185>
  <404>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><407>: Abbrev Number: 14 (DW_TAG_member)
  <408>     DW_AT_name        : pfMessageString
  <418>     DW_AT_decl_file   : 4
  <419>     DW_AT_decl_line   : 210
  <41a>     DW_AT_type        : <34d>
  <41e>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><421>: Abbrev Number: 14 (DW_TAG_member)
  <422>     DW_AT_name        : pfMessageNumber
  <432>     DW_AT_decl_file   : 4
  <433>     DW_AT_decl_line   : 211
  <434>     DW_AT_type        : <389>
  <438>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><43b>: Abbrev Number: 14 (DW_TAG_member)
  <43c>     DW_AT_name        : pfGetTime
  <446>     DW_AT_decl_file   : 4
  <447>     DW_AT_decl_line   : 212
  <448>     DW_AT_type        : <3cf>
  <44c>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <1><450>: Abbrev Number: 2 (DW_TAG_typedef)
  <451>     DW_AT_name        : MUSB_SystemUtils
  <462>     DW_AT_decl_file   : 4
  <463>     DW_AT_decl_line   : 213
  <464>     DW_AT_type        : <3f1>
 <1><468>: Abbrev Number: 2 (DW_TAG_typedef)
  <469>     DW_AT_name        : MUSB_pfSystemToBusAddress
  <483>     DW_AT_decl_file   : 4
  <484>     DW_AT_decl_line   : 225
  <485>     DW_AT_type        : <489>
 <1><489>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <48a>     DW_AT_byte_size   : 4
  <48b>     DW_AT_type        : <48f>
 <1><48f>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <490>     DW_AT_prototyped  : 1
  <491>     DW_AT_type        : <f1>
  <495>     DW_AT_sibling     : <4a4>
 <2><499>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <49a>     DW_AT_type        : <f1>
 <2><49e>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <49f>     DW_AT_type        : <4a4>
 <1><4a4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4a5>     DW_AT_byte_size   : 4
  <4a6>     DW_AT_type        : <4aa>
 <1><4aa>: Abbrev Number: 16 (DW_TAG_const_type)
 <1><4ab>: Abbrev Number: 2 (DW_TAG_typedef)
  <4ac>     DW_AT_name        : MUSB_pfQueueBackgroundItem
  <4c7>     DW_AT_decl_file   : 4
  <4c8>     DW_AT_decl_line   : 238
  <4c9>     DW_AT_type        : <4cd>
 <1><4cd>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <4ce>     DW_AT_byte_size   : 4
  <4cf>     DW_AT_type        : <4d3>
 <1><4d3>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <4d4>     DW_AT_prototyped  : 1
  <4d5>     DW_AT_type        : <176>
  <4d9>     DW_AT_sibling     : <4e8>
 <2><4dd>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <4de>     DW_AT_type        : <f1>
 <2><4e2>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <4e3>     DW_AT_type        : <4a4>
 <1><4e8>: Abbrev Number: 2 (DW_TAG_typedef)
  <4e9>     DW_AT_name        : MUSB_pfDequeueBackgroundItem
  <506>     DW_AT_decl_file   : 4
  <507>     DW_AT_decl_line   : 251
  <508>     DW_AT_type        : <50c>
 <1><50c>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <50d>     DW_AT_byte_size   : 4
  <50e>     DW_AT_type        : <512>
 <1><512>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <513>     DW_AT_prototyped  : 1
  <514>     DW_AT_type        : <176>
  <518>     DW_AT_sibling     : <527>
 <2><51c>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <51d>     DW_AT_type        : <f1>
 <2><521>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <522>     DW_AT_type        : <f1>
 <1><527>: Abbrev Number: 17 (DW_TAG_typedef)
  <528>     DW_AT_name        : MUSB_pfFlushBackgroundQueue
  <544>     DW_AT_decl_file   : 4
  <545>     DW_AT_decl_line   : 259
  <547>     DW_AT_type        : <211>
 <1><54b>: Abbrev Number: 17 (DW_TAG_typedef)
  <54c>     DW_AT_name        : MUSB_pfArmTimer
  <55c>     DW_AT_decl_file   : 4
  <55d>     DW_AT_decl_line   : 274
  <55f>     DW_AT_type        : <563>
 <1><563>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <564>     DW_AT_byte_size   : 4
  <565>     DW_AT_type        : <569>
 <1><569>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <56a>     DW_AT_prototyped  : 1
  <56b>     DW_AT_type        : <176>
  <56f>     DW_AT_sibling     : <58d>
 <2><573>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <574>     DW_AT_type        : <f1>
 <2><578>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <579>     DW_AT_type        : <185>
 <2><57d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <57e>     DW_AT_type        : <1a4>
 <2><582>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <583>     DW_AT_type        : <176>
 <2><587>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <588>     DW_AT_type        : <223>
 <1><58d>: Abbrev Number: 17 (DW_TAG_typedef)
  <58e>     DW_AT_name        : MUSB_pfCancelTimer
  <5a1>     DW_AT_decl_file   : 4
  <5a2>     DW_AT_decl_line   : 285
  <5a4>     DW_AT_type        : <5a8>
 <1><5a8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <5a9>     DW_AT_byte_size   : 4
  <5aa>     DW_AT_type        : <5ae>
 <1><5ae>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <5af>     DW_AT_prototyped  : 1
  <5b0>     DW_AT_type        : <176>
  <5b4>     DW_AT_sibling     : <5c3>
 <2><5b8>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <5b9>     DW_AT_type        : <f1>
 <2><5bd>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <5be>     DW_AT_type        : <185>
 <1><5c3>: Abbrev Number: 17 (DW_TAG_typedef)
  <5c4>     DW_AT_name        : MUSB_pfLock
  <5d0>     DW_AT_decl_file   : 4
  <5d1>     DW_AT_decl_line   : 296
  <5d3>     DW_AT_type        : <5a8>
 <1><5d7>: Abbrev Number: 17 (DW_TAG_typedef)
  <5d8>     DW_AT_name        : MUSB_pfUnlock
  <5e6>     DW_AT_decl_file   : 4
  <5e7>     DW_AT_decl_line   : 307
  <5e9>     DW_AT_type        : <5a8>
 <1><5ed>: Abbrev Number: 17 (DW_TAG_typedef)
  <5ee>     DW_AT_name        : MUSB_pfPrintDiag
  <5ff>     DW_AT_decl_file   : 4
  <600>     DW_AT_decl_line   : 318
  <602>     DW_AT_type        : <606>
 <1><606>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <607>     DW_AT_byte_size   : 4
  <608>     DW_AT_type        : <60c>
 <1><60c>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <60d>     DW_AT_prototyped  : 1
  <60e>     DW_AT_type        : <176>
  <612>     DW_AT_sibling     : <621>
 <2><616>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <617>     DW_AT_type        : <f1>
 <2><61b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <61c>     DW_AT_type        : <116>
 <1><621>: Abbrev Number: 17 (DW_TAG_typedef)
  <622>     DW_AT_name        : MUSB_pfNewPowerLoad
  <636>     DW_AT_decl_file   : 4
  <637>     DW_AT_decl_line   : 346
  <639>     DW_AT_type        : <63d>
 <1><63d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <63e>     DW_AT_byte_size   : 4
  <63f>     DW_AT_type        : <643>
 <1><643>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <644>     DW_AT_prototyped  : 1
  <645>     DW_AT_type        : <176>
  <649>     DW_AT_sibling     : <667>
 <2><64d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <64e>     DW_AT_type        : <f1>
 <2><652>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <653>     DW_AT_type        : <185>
 <2><657>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <658>     DW_AT_type        : <176>
 <2><65c>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <65d>     DW_AT_type        : <667>
 <2><661>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <662>     DW_AT_type        : <176>
 <1><667>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <668>     DW_AT_byte_size   : 4
  <669>     DW_AT_type        : <66d>
 <1><66d>: Abbrev Number: 8 (DW_TAG_const_type)
  <66e>     DW_AT_type        : <176>
 <1><672>: Abbrev Number: 17 (DW_TAG_typedef)
  <673>     DW_AT_name        : MUSB_pfRemovePowerLoad
  <68a>     DW_AT_decl_file   : 4
  <68b>     DW_AT_decl_line   : 366
  <68d>     DW_AT_type        : <63d>
 <1><691>: Abbrev Number: 18 (DW_TAG_structure_type)
  <692>     DW_AT_byte_size   : 52
  <693>     DW_AT_decl_file   : 4
  <694>     DW_AT_decl_line   : 403
  <696>     DW_AT_sibling     : <7e2>
 <2><69a>: Abbrev Number: 19 (DW_TAG_member)
  <69b>     DW_AT_name        : (indirect string, offset: 0x90): wVersion
  <69f>     DW_AT_decl_file   : 4
  <6a0>     DW_AT_decl_line   : 404
  <6a2>     DW_AT_type        : <185>
  <6a6>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><6a9>: Abbrev Number: 19 (DW_TAG_member)
  <6aa>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <6ae>     DW_AT_decl_file   : 4
  <6af>     DW_AT_decl_line   : 405
  <6b1>     DW_AT_type        : <f1>
  <6b5>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><6b8>: Abbrev Number: 20 (DW_TAG_member)
  <6b9>     DW_AT_name        : pfSystemToBusAddress
  <6ce>     DW_AT_decl_file   : 4
  <6cf>     DW_AT_decl_line   : 406
  <6d1>     DW_AT_type        : <468>
  <6d5>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><6d8>: Abbrev Number: 20 (DW_TAG_member)
  <6d9>     DW_AT_name        : pfQueueBackgroundItem
  <6ef>     DW_AT_decl_file   : 4
  <6f0>     DW_AT_decl_line   : 407
  <6f2>     DW_AT_type        : <4ab>
  <6f6>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><6f9>: Abbrev Number: 20 (DW_TAG_member)
  <6fa>     DW_AT_name        : pfDequeueBackgroundItem
  <712>     DW_AT_decl_file   : 4
  <713>     DW_AT_decl_line   : 408
  <715>     DW_AT_type        : <4e8>
  <719>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><71c>: Abbrev Number: 20 (DW_TAG_member)
  <71d>     DW_AT_name        : pfFlushBackgroundQueue
  <734>     DW_AT_decl_file   : 4
  <735>     DW_AT_decl_line   : 409
  <737>     DW_AT_type        : <527>
  <73b>     DW_AT_data_member_location: 2 byte block: 23 14     (DW_OP_plus_uconst: 20)
 <2><73e>: Abbrev Number: 20 (DW_TAG_member)
  <73f>     DW_AT_name        : pfArmTimer
  <74a>     DW_AT_decl_file   : 4
  <74b>     DW_AT_decl_line   : 410
  <74d>     DW_AT_type        : <54b>
  <751>     DW_AT_data_member_location: 2 byte block: 23 18     (DW_OP_plus_uconst: 24)
 <2><754>: Abbrev Number: 20 (DW_TAG_member)
  <755>     DW_AT_name        : pfCancelTimer
  <763>     DW_AT_decl_file   : 4
  <764>     DW_AT_decl_line   : 411
  <766>     DW_AT_type        : <58d>
  <76a>     DW_AT_data_member_location: 2 byte block: 23 1c     (DW_OP_plus_uconst: 28)
 <2><76d>: Abbrev Number: 20 (DW_TAG_member)
  <76e>     DW_AT_name        : pfLock
  <775>     DW_AT_decl_file   : 4
  <776>     DW_AT_decl_line   : 412
  <778>     DW_AT_type        : <5c3>
  <77c>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <2><77f>: Abbrev Number: 20 (DW_TAG_member)
  <780>     DW_AT_name        : pfUnlock
  <789>     DW_AT_decl_file   : 4
  <78a>     DW_AT_decl_line   : 413
  <78c>     DW_AT_type        : <5d7>
  <790>     DW_AT_data_member_location: 2 byte block: 23 24     (DW_OP_plus_uconst: 36)
 <2><793>: Abbrev Number: 20 (DW_TAG_member)
  <794>     DW_AT_name        : pfPrintDiag
  <7a0>     DW_AT_decl_file   : 4
  <7a1>     DW_AT_decl_line   : 414
  <7a3>     DW_AT_type        : <5ed>
  <7a7>     DW_AT_data_member_location: 2 byte block: 23 28     (DW_OP_plus_uconst: 40)
 <2><7aa>: Abbrev Number: 20 (DW_TAG_member)
  <7ab>     DW_AT_name        : pfNewPowerLoad
  <7ba>     DW_AT_decl_file   : 4
  <7bb>     DW_AT_decl_line   : 415
  <7bd>     DW_AT_type        : <621>
  <7c1>     DW_AT_data_member_location: 2 byte block: 23 2c     (DW_OP_plus_uconst: 44)
 <2><7c4>: Abbrev Number: 20 (DW_TAG_member)
  <7c5>     DW_AT_name        : pfRemovePowerLoad
  <7d7>     DW_AT_decl_file   : 4
  <7d8>     DW_AT_decl_line   : 416
  <7da>     DW_AT_type        : <672>
  <7de>     DW_AT_data_member_location: 2 byte block: 23 30     (DW_OP_plus_uconst: 48)
 <1><7e2>: Abbrev Number: 17 (DW_TAG_typedef)
  <7e3>     DW_AT_name        : MUSB_SystemServices
  <7f7>     DW_AT_decl_file   : 4
  <7f8>     DW_AT_decl_line   : 417
  <7fa>     DW_AT_type        : <691>
 <1><7fe>: Abbrev Number: 12 (DW_TAG_structure_type)
  <7ff>     DW_AT_byte_size   : 20
  <800>     DW_AT_decl_file   : 5
  <801>     DW_AT_decl_line   : 194
  <802>     DW_AT_sibling     : <934>
 <2><806>: Abbrev Number: 13 (DW_TAG_member)
  <807>     DW_AT_name        : (indirect string, offset: 0x17): bLength
  <80b>     DW_AT_decl_file   : 5
  <80c>     DW_AT_decl_line   : 195
  <80d>     DW_AT_type        : <176>
  <811>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><814>: Abbrev Number: 13 (DW_TAG_member)
  <815>     DW_AT_name        : (indirect string, offset: 0xca): bDescriptorType
  <819>     DW_AT_decl_file   : 5
  <81a>     DW_AT_decl_line   : 196
  <81b>     DW_AT_type        : <176>
  <81f>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><822>: Abbrev Number: 14 (DW_TAG_member)
  <823>     DW_AT_name        : bcdUSB
  <82a>     DW_AT_decl_file   : 5
  <82b>     DW_AT_decl_line   : 197
  <82c>     DW_AT_type        : <185>
  <830>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><833>: Abbrev Number: 14 (DW_TAG_member)
  <834>     DW_AT_name        : bDeviceClass
  <841>     DW_AT_decl_file   : 5
  <842>     DW_AT_decl_line   : 198
  <843>     DW_AT_type        : <176>
  <847>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><84a>: Abbrev Number: 14 (DW_TAG_member)
  <84b>     DW_AT_name        : bDeviceSubClass
  <85b>     DW_AT_decl_file   : 5
  <85c>     DW_AT_decl_line   : 199
  <85d>     DW_AT_type        : <176>
  <861>     DW_AT_data_member_location: 2 byte block: 23 5  (DW_OP_plus_uconst: 5)
 <2><864>: Abbrev Number: 14 (DW_TAG_member)
  <865>     DW_AT_name        : bDeviceProtocol
  <875>     DW_AT_decl_file   : 5
  <876>     DW_AT_decl_line   : 200
  <877>     DW_AT_type        : <176>
  <87b>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <2><87e>: Abbrev Number: 14 (DW_TAG_member)
  <87f>     DW_AT_name        : bMaxPacketSize0
  <88f>     DW_AT_decl_file   : 5
  <890>     DW_AT_decl_line   : 201
  <891>     DW_AT_type        : <176>
  <895>     DW_AT_data_member_location: 2 byte block: 23 7  (DW_OP_plus_uconst: 7)
 <2><898>: Abbrev Number: 14 (DW_TAG_member)
  <899>     DW_AT_name        : idVendor
  <8a2>     DW_AT_decl_file   : 5
  <8a3>     DW_AT_decl_line   : 202
  <8a4>     DW_AT_type        : <185>
  <8a8>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><8ab>: Abbrev Number: 14 (DW_TAG_member)
  <8ac>     DW_AT_name        : idProduct
  <8b6>     DW_AT_decl_file   : 5
  <8b7>     DW_AT_decl_line   : 203
  <8b8>     DW_AT_type        : <185>
  <8bc>     DW_AT_data_member_location: 2 byte block: 23 a  (DW_OP_plus_uconst: 10)
 <2><8bf>: Abbrev Number: 14 (DW_TAG_member)
  <8c0>     DW_AT_name        : bcdDevice
  <8ca>     DW_AT_decl_file   : 5
  <8cb>     DW_AT_decl_line   : 204
  <8cc>     DW_AT_type        : <185>
  <8d0>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><8d3>: Abbrev Number: 14 (DW_TAG_member)
  <8d4>     DW_AT_name        : iManufacturer
  <8e2>     DW_AT_decl_file   : 5
  <8e3>     DW_AT_decl_line   : 205
  <8e4>     DW_AT_type        : <176>
  <8e8>     DW_AT_data_member_location: 2 byte block: 23 e  (DW_OP_plus_uconst: 14)
 <2><8eb>: Abbrev Number: 14 (DW_TAG_member)
  <8ec>     DW_AT_name        : iProduct
  <8f5>     DW_AT_decl_file   : 5
  <8f6>     DW_AT_decl_line   : 206
  <8f7>     DW_AT_type        : <176>
  <8fb>     DW_AT_data_member_location: 2 byte block: 23 f  (DW_OP_plus_uconst: 15)
 <2><8fe>: Abbrev Number: 14 (DW_TAG_member)
  <8ff>     DW_AT_name        : iSerialNumber
  <90d>     DW_AT_decl_file   : 5
  <90e>     DW_AT_decl_line   : 207
  <90f>     DW_AT_type        : <176>
  <913>     DW_AT_data_member_location: 2 byte block: 23 10     (DW_OP_plus_uconst: 16)
 <2><916>: Abbrev Number: 14 (DW_TAG_member)
  <917>     DW_AT_name        : bNumConfigurations
  <92a>     DW_AT_decl_file   : 5
  <92b>     DW_AT_decl_line   : 208
  <92c>     DW_AT_type        : <176>
  <930>     DW_AT_data_member_location: 2 byte block: 23 11     (DW_OP_plus_uconst: 17)
 <1><934>: Abbrev Number: 2 (DW_TAG_typedef)
  <935>     DW_AT_name        : MUSB_DeviceDescriptor
  <94b>     DW_AT_decl_file   : 5
  <94c>     DW_AT_decl_line   : 209
  <94d>     DW_AT_type        : <7fe>
 <1><951>: Abbrev Number: 12 (DW_TAG_structure_type)
  <952>     DW_AT_byte_size   : 8
  <953>     DW_AT_decl_file   : 5
  <954>     DW_AT_decl_line   : 215
  <955>     DW_AT_sibling     : <9bb>
 <2><959>: Abbrev Number: 13 (DW_TAG_member)
  <95a>     DW_AT_name        : (indirect string, offset: 0x17): bLength
  <95e>     DW_AT_decl_file   : 5
  <95f>     DW_AT_decl_line   : 216
  <960>     DW_AT_type        : <176>
  <964>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><967>: Abbrev Number: 13 (DW_TAG_member)
  <968>     DW_AT_name        : (indirect string, offset: 0xca): bDescriptorType
  <96c>     DW_AT_decl_file   : 5
  <96d>     DW_AT_decl_line   : 217
  <96e>     DW_AT_type        : <176>
  <972>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><975>: Abbrev Number: 14 (DW_TAG_member)
  <976>     DW_AT_name        : bEndpointAddress
  <987>     DW_AT_decl_file   : 5
  <988>     DW_AT_decl_line   : 218
  <989>     DW_AT_type        : <176>
  <98d>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><990>: Abbrev Number: 13 (DW_TAG_member)
  <991>     DW_AT_name        : (indirect string, offset: 0x3d): bmAttributes
  <995>     DW_AT_decl_file   : 5
  <996>     DW_AT_decl_line   : 219
  <997>     DW_AT_type        : <176>
  <99b>     DW_AT_data_member_location: 2 byte block: 23 3  (DW_OP_plus_uconst: 3)
 <2><99e>: Abbrev Number: 13 (DW_TAG_member)
  <99f>     DW_AT_name        : (indirect string, offset: 0x2e): wMaxPacketSize
  <9a3>     DW_AT_decl_file   : 5
  <9a4>     DW_AT_decl_line   : 220
  <9a5>     DW_AT_type        : <185>
  <9a9>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><9ac>: Abbrev Number: 13 (DW_TAG_member)
  <9ad>     DW_AT_name        : (indirect string, offset: 0x86): bInterval
  <9b1>     DW_AT_decl_file   : 5
  <9b2>     DW_AT_decl_line   : 221
  <9b3>     DW_AT_type        : <176>
  <9b7>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <1><9bb>: Abbrev Number: 2 (DW_TAG_typedef)
  <9bc>     DW_AT_name        : MUSB_EndpointDescriptor
  <9d4>     DW_AT_decl_file   : 5
  <9d5>     DW_AT_decl_line   : 222
  <9d6>     DW_AT_type        : <951>
 <1><9da>: Abbrev Number: 12 (DW_TAG_structure_type)
  <9db>     DW_AT_byte_size   : 12
  <9dc>     DW_AT_decl_file   : 5
  <9dd>     DW_AT_decl_line   : 244
  <9de>     DW_AT_sibling     : <a88>
 <2><9e2>: Abbrev Number: 13 (DW_TAG_member)
  <9e3>     DW_AT_name        : (indirect string, offset: 0x17): bLength
  <9e7>     DW_AT_decl_file   : 5
  <9e8>     DW_AT_decl_line   : 245
  <9e9>     DW_AT_type        : <176>
  <9ed>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><9f0>: Abbrev Number: 13 (DW_TAG_member)
  <9f1>     DW_AT_name        : (indirect string, offset: 0xca): bDescriptorType
  <9f5>     DW_AT_decl_file   : 5
  <9f6>     DW_AT_decl_line   : 246
  <9f7>     DW_AT_type        : <176>
  <9fb>     DW_AT_data_member_location: 2 byte block: 23 1  (DW_OP_plus_uconst: 1)
 <2><9fe>: Abbrev Number: 14 (DW_TAG_member)
  <9ff>     DW_AT_name        : wTotalLength
  <a0c>     DW_AT_decl_file   : 5
  <a0d>     DW_AT_decl_line   : 247
  <a0e>     DW_AT_type        : <185>
  <a12>     DW_AT_data_member_location: 2 byte block: 23 2  (DW_OP_plus_uconst: 2)
 <2><a15>: Abbrev Number: 14 (DW_TAG_member)
  <a16>     DW_AT_name        : bNumInterfaces
  <a25>     DW_AT_decl_file   : 5
  <a26>     DW_AT_decl_line   : 248
  <a27>     DW_AT_type        : <176>
  <a2b>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><a2e>: Abbrev Number: 14 (DW_TAG_member)
  <a2f>     DW_AT_name        : bConfigurationValue
  <a43>     DW_AT_decl_file   : 5
  <a44>     DW_AT_decl_line   : 249
  <a45>     DW_AT_type        : <176>
  <a49>     DW_AT_data_member_location: 2 byte block: 23 5  (DW_OP_plus_uconst: 5)
 <2><a4c>: Abbrev Number: 14 (DW_TAG_member)
  <a4d>     DW_AT_name        : iConfiguration
  <a5c>     DW_AT_decl_file   : 5
  <a5d>     DW_AT_decl_line   : 250
  <a5e>     DW_AT_type        : <176>
  <a62>     DW_AT_data_member_location: 2 byte block: 23 6  (DW_OP_plus_uconst: 6)
 <2><a65>: Abbrev Number: 13 (DW_TAG_member)
  <a66>     DW_AT_name        : (indirect string, offset: 0x3d): bmAttributes
  <a6a>     DW_AT_decl_file   : 5
  <a6b>     DW_AT_decl_line   : 251
  <a6c>     DW_AT_type        : <176>
  <a70>     DW_AT_data_member_location: 2 byte block: 23 7  (DW_OP_plus_uconst: 7)
 <2><a73>: Abbrev Number: 14 (DW_TAG_member)
  <a74>     DW_AT_name        : bMaxPower
  <a7e>     DW_AT_decl_file   : 5
  <a7f>     DW_AT_decl_line   : 252
  <a80>     DW_AT_type        : <176>
  <a84>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <1><a88>: Abbrev Number: 2 (DW_TAG_typedef)
  <a89>     DW_AT_name        : MUSB_ConfigurationDescriptor
  <aa6>     DW_AT_decl_file   : 5
  <aa7>     DW_AT_decl_line   : 253
  <aa8>     DW_AT_type        : <9da>
 <1><aac>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <aad>     DW_AT_byte_size   : 4
  <aae>     DW_AT_decl_file   : 6
  <aaf>     DW_AT_decl_line   : 36
  <ab0>     DW_AT_sibling     : <b0e>
 <2><ab4>: Abbrev Number: 22 (DW_TAG_enumerator)
  <ab5>     DW_AT_name        : MUSB_PORT_UNKNOWN
  <ac7>     DW_AT_const_value : 0
 <2><ac8>: Abbrev Number: 22 (DW_TAG_enumerator)
  <ac9>     DW_AT_name        : MUSB_PORT_TYPE_FUNCTION
  <ae1>     DW_AT_const_value : 1
 <2><ae2>: Abbrev Number: 22 (DW_TAG_enumerator)
  <ae3>     DW_AT_name        : MUSB_PORT_TYPE_HOST
  <af7>     DW_AT_const_value : 2
 <2><af8>: Abbrev Number: 22 (DW_TAG_enumerator)
  <af9>     DW_AT_name        : MUSB_PORT_TYPE_OTG
  <b0c>     DW_AT_const_value : 3
 <1><b0e>: Abbrev Number: 2 (DW_TAG_typedef)
  <b0f>     DW_AT_name        : MUSB_PortType
  <b1d>     DW_AT_decl_file   : 6
  <b1e>     DW_AT_decl_line   : 45
  <b1f>     DW_AT_type        : <aac>
 <1><b23>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <b24>     DW_AT_byte_size   : 4
  <b25>     DW_AT_decl_file   : 6
  <b26>     DW_AT_decl_line   : 52
  <b27>     DW_AT_sibling     : <b8a>
 <2><b2b>: Abbrev Number: 22 (DW_TAG_enumerator)
  <b2c>     DW_AT_name        : MUSB_PORT_SPEED_UNKNOWN
  <b44>     DW_AT_const_value : 0
 <2><b45>: Abbrev Number: 22 (DW_TAG_enumerator)
  <b46>     DW_AT_name        : MUSB_PORT_SPEED_LOW
  <b5a>     DW_AT_const_value : 1
 <2><b5b>: Abbrev Number: 22 (DW_TAG_enumerator)
  <b5c>     DW_AT_name        : MUSB_PORT_SPEED_FULL
  <b71>     DW_AT_const_value : 2
 <2><b72>: Abbrev Number: 22 (DW_TAG_enumerator)
  <b73>     DW_AT_name        : MUSB_PORT_SPEED_HIGH
  <b88>     DW_AT_const_value : 3
 <1><b8a>: Abbrev Number: 2 (DW_TAG_typedef)
  <b8b>     DW_AT_name        : MUSB_PortSpeed
  <b9a>     DW_AT_decl_file   : 6
  <b9b>     DW_AT_decl_line   : 61
  <b9c>     DW_AT_type        : <b23>
 <1><ba0>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <ba1>     DW_AT_byte_size   : 4
  <ba2>     DW_AT_decl_file   : 6
  <ba3>     DW_AT_decl_line   : 68
  <ba4>     DW_AT_sibling     : <c0a>
 <2><ba8>: Abbrev Number: 22 (DW_TAG_enumerator)
  <ba9>     DW_AT_name        : MUSB_POWER_OFF
  <bb8>     DW_AT_const_value : 0
 <2><bb9>: Abbrev Number: 22 (DW_TAG_enumerator)
  <bba>     DW_AT_name        : MUSB_POWERED
  <bc7>     DW_AT_const_value : 1
 <2><bc8>: Abbrev Number: 22 (DW_TAG_enumerator)
  <bc9>     DW_AT_name        : MUSB_DEFAULT
  <bd6>     DW_AT_const_value : 2
 <2><bd7>: Abbrev Number: 22 (DW_TAG_enumerator)
  <bd8>     DW_AT_name        : MUSB_ADDRESS
  <be5>     DW_AT_const_value : 3
 <2><be6>: Abbrev Number: 22 (DW_TAG_enumerator)
  <be7>     DW_AT_name        : MUSB_CONFIGURED
  <bf7>     DW_AT_const_value : 4
 <2><bf8>: Abbrev Number: 22 (DW_TAG_enumerator)
  <bf9>     DW_AT_name        : MUSB_SUSPENDED
  <c08>     DW_AT_const_value : 5
 <1><c0a>: Abbrev Number: 2 (DW_TAG_typedef)
  <c0b>     DW_AT_name        : MUSB_State
  <c16>     DW_AT_decl_file   : 6
  <c17>     DW_AT_decl_line   : 81
  <c18>     DW_AT_type        : <ba0>
 <1><c1c>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <c1d>     DW_AT_byte_size   : 4
  <c1e>     DW_AT_decl_file   : 6
  <c1f>     DW_AT_decl_line   : 88
  <c20>     DW_AT_sibling     : <ce7>
 <2><c24>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c25>     DW_AT_name        : MUSB_AB_IDLE
  <c32>     DW_AT_const_value : 0
 <2><c33>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c34>     DW_AT_name        : MUSB_WAIT_VRISE
  <c44>     DW_AT_const_value : 1
 <2><c45>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c46>     DW_AT_name        : MUSB_B_PERIPHERAL
  <c58>     DW_AT_const_value : 17
 <2><c59>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c5a>     DW_AT_name        : MUSB_B_WAIT_ACON
  <c6b>     DW_AT_const_value : 18
 <2><c6c>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c6d>     DW_AT_name        : MUSB_B_HOST
  <c79>     DW_AT_const_value : 19
 <2><c7a>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c7b>     DW_AT_name        : MUSB_B_SRP_INIT
  <c8b>     DW_AT_const_value : 20
 <2><c8c>: Abbrev Number: 22 (DW_TAG_enumerator)
  <c8d>     DW_AT_name        : MUSB_A_PERIPHERAL
  <c9f>     DW_AT_const_value : 33
 <2><ca0>: Abbrev Number: 22 (DW_TAG_enumerator)
  <ca1>     DW_AT_name        : MUSB_A_WAIT_BCON
  <cb2>     DW_AT_const_value : 34
 <2><cb3>: Abbrev Number: 22 (DW_TAG_enumerator)
  <cb4>     DW_AT_name        : MUSB_A_HOST
  <cc0>     DW_AT_const_value : 35
 <2><cc1>: Abbrev Number: 22 (DW_TAG_enumerator)
  <cc2>     DW_AT_name        : MUSB_A_SUSPEND
  <cd1>     DW_AT_const_value : 36
 <2><cd2>: Abbrev Number: 22 (DW_TAG_enumerator)
  <cd3>     DW_AT_name        : MUSB_S_TRANSITION
  <ce5>     DW_AT_const_value : 48
 <1><ce7>: Abbrev Number: 2 (DW_TAG_typedef)
  <ce8>     DW_AT_name        : MUSB_OtgState
  <cf6>     DW_AT_decl_file   : 6
  <cf7>     DW_AT_decl_line   : 111
  <cf8>     DW_AT_type        : <c1c>
 <1><cfc>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <cfd>     DW_AT_byte_size   : 4
  <cfe>     DW_AT_decl_file   : 6
  <cff>     DW_AT_decl_line   : 118
  <d00>     DW_AT_sibling     : <d7b>
 <2><d04>: Abbrev Number: 22 (DW_TAG_enumerator)
  <d05>     DW_AT_name        : MUSB_CONNECTION_SPEED_UNKNOWN
  <d23>     DW_AT_const_value : 0
 <2><d24>: Abbrev Number: 22 (DW_TAG_enumerator)
  <d25>     DW_AT_name        : MUSB_CONNECTION_SPEED_LOW
  <d3f>     DW_AT_const_value : 1
 <2><d40>: Abbrev Number: 22 (DW_TAG_enumerator)
  <d41>     DW_AT_name        : MUSB_CONNECTION_SPEED_FULL
  <d5c>     DW_AT_const_value : 2
 <2><d5d>: Abbrev Number: 22 (DW_TAG_enumerator)
  <d5e>     DW_AT_name        : MUSB_CONNECTION_SPEED_HIGH
  <d79>     DW_AT_const_value : 3
 <1><d7b>: Abbrev Number: 2 (DW_TAG_typedef)
  <d7c>     DW_AT_name        : MUSB_ConnectionSpeed
  <d91>     DW_AT_decl_file   : 6
  <d92>     DW_AT_decl_line   : 127
  <d93>     DW_AT_type        : <cfc>
 <1><d97>: Abbrev Number: 12 (DW_TAG_structure_type)
  <d98>     DW_AT_byte_size   : 16
  <d99>     DW_AT_decl_file   : 6
  <d9a>     DW_AT_decl_line   : 228
  <d9b>     DW_AT_sibling     : <de1>
 <2><d9f>: Abbrev Number: 13 (DW_TAG_member)
  <da0>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <da4>     DW_AT_decl_file   : 6
  <da5>     DW_AT_decl_line   : 229
  <da6>     DW_AT_type        : <f1>
  <daa>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><dad>: Abbrev Number: 14 (DW_TAG_member)
  <dae>     DW_AT_name        : bcdCdiRev
  <db8>     DW_AT_decl_file   : 6
  <db9>     DW_AT_decl_line   : 230
  <dba>     DW_AT_type        : <185>
  <dbe>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><dc1>: Abbrev Number: 14 (DW_TAG_member)
  <dc2>     DW_AT_name        : Type
  <dc7>     DW_AT_decl_file   : 6
  <dc8>     DW_AT_decl_line   : 231
  <dc9>     DW_AT_type        : <b0e>
  <dcd>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><dd0>: Abbrev Number: 14 (DW_TAG_member)
  <dd1>     DW_AT_name        : Speed
  <dd7>     DW_AT_decl_file   : 6
  <dd8>     DW_AT_decl_line   : 232
  <dd9>     DW_AT_type        : <b8a>
  <ddd>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <1><de1>: Abbrev Number: 2 (DW_TAG_typedef)
  <de2>     DW_AT_name        : MUSB_Port
  <dec>     DW_AT_decl_file   : 6
  <ded>     DW_AT_decl_line   : 233
  <dee>     DW_AT_type        : <d97>
 <1><df2>: Abbrev Number: 23 (DW_TAG_structure_type)
  <df3>     DW_AT_name        : _MUSB_Device
  <e00>     DW_AT_byte_size   : 64
  <e01>     DW_AT_decl_file   : 6
  <e02>     DW_AT_decl_line   : 235
  <e03>     DW_AT_sibling     : <f16>
 <2><e07>: Abbrev Number: 19 (DW_TAG_member)
  <e08>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <e0c>     DW_AT_decl_file   : 6
  <e0d>     DW_AT_decl_line   : 270
  <e0f>     DW_AT_type        : <f16>
  <e13>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><e16>: Abbrev Number: 19 (DW_TAG_member)
  <e17>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <e1b>     DW_AT_decl_file   : 6
  <e1c>     DW_AT_decl_line   : 271
  <e1e>     DW_AT_type        : <f1>
  <e22>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><e25>: Abbrev Number: 20 (DW_TAG_member)
  <e26>     DW_AT_name        : pDriverPrivateData
  <e39>     DW_AT_decl_file   : 6
  <e3a>     DW_AT_decl_line   : 272
  <e3c>     DW_AT_type        : <f1>
  <e40>     DW_AT_data_member_location: 2 byte block: 23 8  (DW_OP_plus_uconst: 8)
 <2><e43>: Abbrev Number: 20 (DW_TAG_member)
  <e44>     DW_AT_name        : DeviceDescriptor
  <e55>     DW_AT_decl_file   : 6
  <e56>     DW_AT_decl_line   : 273
  <e58>     DW_AT_type        : <934>
  <e5c>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <2><e5f>: Abbrev Number: 20 (DW_TAG_member)
  <e60>     DW_AT_name        : pCurrentConfiguration
  <e76>     DW_AT_decl_file   : 6
  <e77>     DW_AT_decl_line   : 274
  <e79>     DW_AT_type        : <f1c>
  <e7d>     DW_AT_data_member_location: 2 byte block: 23 20     (DW_OP_plus_uconst: 32)
 <2><e80>: Abbrev Number: 19 (DW_TAG_member)
  <e81>     DW_AT_name        : (indirect string, offset: 0x191): apConfigDescriptors
  <e85>     DW_AT_decl_file   : 6
  <e86>     DW_AT_decl_line   : 275
  <e88>     DW_AT_type        : <f22>
  <e8c>     DW_AT_data_member_location: 2 byte block: 23 24     (DW_OP_plus_uconst: 36)
 <2><e8f>: Abbrev Number: 20 (DW_TAG_member)
  <e90>     DW_AT_name        : pDescriptorBuffer
  <ea2>     DW_AT_decl_file   : 6
  <ea3>     DW_AT_decl_line   : 276
  <ea5>     DW_AT_type        : <f28>
  <ea9>     DW_AT_data_member_location: 2 byte block: 23 28     (DW_OP_plus_uconst: 40)
 <2><eac>: Abbrev Number: 19 (DW_TAG_member)
  <ead>     DW_AT_name        : (indirect string, offset: 0x1ae): wDescriptorBufferLength
  <eb1>     DW_AT_decl_file   : 6
  <eb2>     DW_AT_decl_line   : 277
  <eb4>     DW_AT_type        : <185>
  <eb8>     DW_AT_data_member_location: 2 byte block: 23 2c     (DW_OP_plus_uconst: 44)
 <2><ebb>: Abbrev Number: 20 (DW_TAG_member)
  <ebc>     DW_AT_name        : ConnectionSpeed
  <ecc>     DW_AT_decl_file   : 6
  <ecd>     DW_AT_decl_line   : 278
  <ecf>     DW_AT_type        : <d7b>
  <ed3>     DW_AT_data_member_location: 2 byte block: 23 30     (DW_OP_plus_uconst: 48)
 <2><ed6>: Abbrev Number: 19 (DW_TAG_member)
  <ed7>     DW_AT_name        : (indirect string, offset: 0x11b): bBusAddress
  <edb>     DW_AT_decl_file   : 6
  <edc>     DW_AT_decl_line   : 279
  <ede>     DW_AT_type        : <176>
  <ee2>     DW_AT_data_member_location: 2 byte block: 23 34     (DW_OP_plus_uconst: 52)
 <2><ee5>: Abbrev Number: 20 (DW_TAG_member)
  <ee6>     DW_AT_name        : pParentUsbDevice
  <ef7>     DW_AT_decl_file   : 6
  <ef8>     DW_AT_decl_line   : 280
  <efa>     DW_AT_type        : <f2e>
  <efe>     DW_AT_data_member_location: 2 byte block: 23 38     (DW_OP_plus_uconst: 56)
 <2><f01>: Abbrev Number: 20 (DW_TAG_member)
  <f02>     DW_AT_name        : bHubPort
  <f0b>     DW_AT_decl_file   : 6
  <f0c>     DW_AT_decl_line   : 281
  <f0e>     DW_AT_type        : <176>
  <f12>     DW_AT_data_member_location: 2 byte block: 23 3c     (DW_OP_plus_uconst: 60)
 <1><f16>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f17>     DW_AT_byte_size   : 4
  <f18>     DW_AT_type        : <de1>
 <1><f1c>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f1d>     DW_AT_byte_size   : 4
  <f1e>     DW_AT_type        : <a88>
 <1><f22>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f23>     DW_AT_byte_size   : 4
  <f24>     DW_AT_type        : <f1c>
 <1><f28>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f29>     DW_AT_byte_size   : 4
  <f2a>     DW_AT_type        : <176>
 <1><f2e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <f2f>     DW_AT_byte_size   : 4
  <f30>     DW_AT_type        : <df2>
 <1><f34>: Abbrev Number: 17 (DW_TAG_typedef)
  <f35>     DW_AT_name        : MUSB_Device
  <f41>     DW_AT_decl_file   : 6
  <f42>     DW_AT_decl_line   : 282
  <f44>     DW_AT_type        : <df2>
 <1><f48>: Abbrev Number: 17 (DW_TAG_typedef)
  <f49>     DW_AT_name        : MUSB_BusHandle
  <f58>     DW_AT_decl_file   : 6
  <f59>     DW_AT_decl_line   : 285
  <f5b>     DW_AT_type        : <f1>
 <1><f5f>: Abbrev Number: 18 (DW_TAG_structure_type)
  <f60>     DW_AT_byte_size   : 16
  <f61>     DW_AT_decl_file   : 6
  <f62>     DW_AT_decl_line   : 301
  <f64>     DW_AT_sibling     : <fa6>
 <2><f68>: Abbrev Number: 19 (DW_TAG_member)
  <f69>     DW_AT_name        : (indirect string, offset: 0x1db): pDevice
  <f6d>     DW_AT_decl_file   : 6
  <f6e>     DW_AT_decl_line   : 302
  <f70>     DW_AT_type        : <fa6>
  <f74>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><f77>: Abbrev Number: 20 (DW_TAG_member)
  <f78>     DW_AT_name        : UsbDescriptor
  <f86>     DW_AT_decl_file   : 6
  <f87>     DW_AT_decl_line   : 303
  <f89>     DW_AT_type        : <9bb>
  <f8d>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><f90>: Abbrev Number: 20 (DW_TAG_member)
  <f91>     DW_AT_name        : wNakLimit
  <f9b>     DW_AT_decl_file   : 6
  <f9c>     DW_AT_decl_line   : 304
  <f9e>     DW_AT_type        : <185>
  <fa2>     DW_AT_data_member_location: 2 byte block: 23 c  (DW_OP_plus_uconst: 12)
 <1><fa6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <fa7>     DW_AT_byte_size   : 4
  <fa8>     DW_AT_type        : <fac>
 <1><fac>: Abbrev Number: 8 (DW_TAG_const_type)
  <fad>     DW_AT_type        : <f34>
 <1><fb1>: Abbrev Number: 17 (DW_TAG_typedef)
  <fb2>     DW_AT_name        : MUSB_DeviceEndpoint
  <fc6>     DW_AT_decl_file   : 6
  <fc7>     DW_AT_decl_line   : 305
  <fc9>     DW_AT_type        : <f5f>
 <1><fcd>: Abbrev Number: 18 (DW_TAG_structure_type)
  <fce>     DW_AT_byte_size   : 12
  <fcf>     DW_AT_decl_file   : 6
  <fd0>     DW_AT_decl_line   : 321
  <fd2>     DW_AT_sibling     : <1011>
 <2><fd6>: Abbrev Number: 20 (DW_TAG_member)
  <fd7>     DW_AT_name        : dwBufferSize
  <fe4>     DW_AT_decl_file   : 6
  <fe5>     DW_AT_decl_line   : 322
  <fe7>     DW_AT_type        : <1a4>
  <feb>     DW_AT_data_member_location: 2 byte block: 23 0  (DW_OP_plus_uconst: 0)
 <2><fee>: Abbrev Number: 20 (DW_TAG_member)
  <fef>     DW_AT_name        : bmFlags
  <ff7>     DW_AT_decl_file   : 6
  <ff8>     DW_AT_decl_line   : 323
  <ffa>     DW_AT_type        : <1a4>
  <ffe>     DW_AT_data_member_location: 2 byte block: 23 4  (DW_OP_plus_uconst: 4)
 <2><1001>: Abbrev Number: 19 (DW_TAG_member)
  <1002>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <1006>     DW_AT_decl_file   : 6
  <1007>     DW_AT_decl_line   : 324
  <1009>     DW_AT_type        : <f1>
  <100d>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><1011>: Abbrev Number: 17 (DW_TAG_typedef)
  <1012>     DW_AT_name        : MUSB_EndpointResource
  <1028>     DW_AT_decl_file   : 6
  <1029>     DW_AT_decl_line   : 325
  <102b>     DW_AT_type        : <fcd>
 <1><102f>: Abbrev Number: 17 (DW_TAG_typedef)
  <1030>     DW_AT_name        : MUSB_Pipe
  <103a>     DW_AT_decl_file   : 6
  <103b>     DW_AT_decl_line   : 338
  <103d>     DW_AT_type        : <f1>
 <1><1041>: Abbrev Number: 17 (DW_TAG_typedef)
  <1042>     DW_AT_name        : MUSB_pfControlIrpComplete
  <105c>     DW_AT_decl_file   : 6
  <105d>     DW_AT_decl_line   : 348
  <105f>     DW_AT_type        : <1063>
 <1><1063>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1064>     DW_AT_byte_size   : 4
  <1065>     DW_AT_type        : <1069>
 <1><1069>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <106a>     DW_AT_prototyped  : 1
  <106b>     DW_AT_sibling     : <107a>
 <2><106f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1070>     DW_AT_type        : <f1>
 <2><1074>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1075>     DW_AT_type        : <107a>
 <1><107a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <107b>     DW_AT_byte_size   : 4
  <107c>     DW_AT_type        : <1080>
 <1><1080>: Abbrev Number: 24 (DW_TAG_structure_type)
  <1081>     DW_AT_name        : _MUSB_ControlIrp
  <1092>     DW_AT_byte_size   : 44
  <1093>     DW_AT_decl_file   : 6
  <1094>     DW_AT_decl_line   : 340
  <1096>     DW_AT_sibling     : <1177>
 <2><109a>: Abbrev Number: 19 (DW_TAG_member)
  <109b>     DW_AT_name        : (indirect string, offset: 0x1db): pDevice
  <109f>     DW_AT_decl_file   : 6
  <10a0>     DW_AT_decl_line   : 379
  <10a2>     DW_AT_type        : <fa6>
  <10a6>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><10a9>: Abbrev Number: 20 (DW_TAG_member)
  <10aa>     DW_AT_name        : pOutBuffer
  <10b5>     DW_AT_decl_file   : 6
  <10b6>     DW_AT_decl_line   : 380
  <10b8>     DW_AT_type        : <667>
  <10bc>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><10bf>: Abbrev Number: 20 (DW_TAG_member)
  <10c0>     DW_AT_name        : dwOutLength
  <10cc>     DW_AT_decl_file   : 6
  <10cd>     DW_AT_decl_line   : 381
  <10cf>     DW_AT_type        : <1a4>
  <10d3>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><10d6>: Abbrev Number: 20 (DW_TAG_member)
  <10d7>     DW_AT_name        : pInBuffer
  <10e1>     DW_AT_decl_file   : 6
  <10e2>     DW_AT_decl_line   : 382
  <10e4>     DW_AT_type        : <f28>
  <10e8>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><10eb>: Abbrev Number: 20 (DW_TAG_member)
  <10ec>     DW_AT_name        : dwInLength
  <10f7>     DW_AT_decl_file   : 6
  <10f8>     DW_AT_decl_line   : 383
  <10fa>     DW_AT_type        : <1a4>
  <10fe>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1101>: Abbrev Number: 19 (DW_TAG_member)
  <1102>     DW_AT_name        : (indirect string, offset: 0x50): dwStatus
  <1106>     DW_AT_decl_file   : 6
  <1107>     DW_AT_decl_line   : 384
  <1109>     DW_AT_type        : <1a4>
  <110d>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1110>: Abbrev Number: 20 (DW_TAG_member)
  <1111>     DW_AT_name        : dwActualOutLength
  <1123>     DW_AT_decl_file   : 6
  <1124>     DW_AT_decl_line   : 385
  <1126>     DW_AT_type        : <1a4>
  <112a>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><112d>: Abbrev Number: 20 (DW_TAG_member)
  <112e>     DW_AT_name        : dwActualInLength
  <113f>     DW_AT_decl_file   : 6
  <1140>     DW_AT_decl_line   : 386
  <1142>     DW_AT_type        : <1a4>
  <1146>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1149>: Abbrev Number: 19 (DW_TAG_member)
  <114a>     DW_AT_name        : (indirect string, offset: 0x66): pfIrpComplete
  <114e>     DW_AT_decl_file   : 6
  <114f>     DW_AT_decl_line   : 387
  <1151>     DW_AT_type        : <1041>
  <1155>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><1158>: Abbrev Number: 19 (DW_TAG_member)
  <1159>     DW_AT_name        : (indirect string, offset: 0x1f): pCompleteParam
  <115d>     DW_AT_decl_file   : 6
  <115e>     DW_AT_decl_line   : 388
  <1160>     DW_AT_type        : <f1>
  <1164>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><1167>: Abbrev Number: 19 (DW_TAG_member)
  <1168>     DW_AT_name        : (indirect string, offset: 0x17d): bAllowDma
  <116c>     DW_AT_decl_file   : 6
  <116d>     DW_AT_decl_line   : 389
  <116f>     DW_AT_type        : <176>
  <1173>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <1><1177>: Abbrev Number: 17 (DW_TAG_typedef)
  <1178>     DW_AT_name        : MUSB_ControlIrp
  <1188>     DW_AT_decl_file   : 6
  <1189>     DW_AT_decl_line   : 390
  <118b>     DW_AT_type        : <1080>
 <1><118f>: Abbrev Number: 17 (DW_TAG_typedef)
  <1190>     DW_AT_name        : MUSB_pfIrpComplete
  <11a3>     DW_AT_decl_file   : 6
  <11a4>     DW_AT_decl_line   : 400
  <11a6>     DW_AT_type        : <11aa>
 <1><11aa>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <11ab>     DW_AT_byte_size   : 4
  <11ac>     DW_AT_type        : <11b0>
 <1><11b0>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <11b1>     DW_AT_prototyped  : 1
  <11b2>     DW_AT_sibling     : <11c1>
 <2><11b6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <11b7>     DW_AT_type        : <f1>
 <2><11bb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <11bc>     DW_AT_type        : <11c1>
 <1><11c1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <11c2>     DW_AT_byte_size   : 4
  <11c3>     DW_AT_type        : <11c7>
 <1><11c7>: Abbrev Number: 24 (DW_TAG_structure_type)
  <11c8>     DW_AT_name        : _MUSB_Irp
  <11d2>     DW_AT_byte_size   : 32
  <11d3>     DW_AT_decl_file   : 6
  <11d4>     DW_AT_decl_line   : 392
  <11d6>     DW_AT_sibling     : <1288>
 <2><11da>: Abbrev Number: 20 (DW_TAG_member)
  <11db>     DW_AT_name        : hPipe
  <11e1>     DW_AT_decl_file   : 6
  <11e2>     DW_AT_decl_line   : 431
  <11e4>     DW_AT_type        : <102f>
  <11e8>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><11eb>: Abbrev Number: 19 (DW_TAG_member)
  <11ec>     DW_AT_name        : (indirect string, offset: 0xb8): pBuffer
  <11f0>     DW_AT_decl_file   : 6
  <11f1>     DW_AT_decl_line   : 432
  <11f3>     DW_AT_type        : <f28>
  <11f7>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><11fa>: Abbrev Number: 20 (DW_TAG_member)
  <11fb>     DW_AT_name        : dwLength
  <1204>     DW_AT_decl_file   : 6
  <1205>     DW_AT_decl_line   : 433
  <1207>     DW_AT_type        : <1a4>
  <120b>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><120e>: Abbrev Number: 19 (DW_TAG_member)
  <120f>     DW_AT_name        : (indirect string, offset: 0x50): dwStatus
  <1213>     DW_AT_decl_file   : 6
  <1214>     DW_AT_decl_line   : 434
  <1216>     DW_AT_type        : <1a4>
  <121a>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><121d>: Abbrev Number: 19 (DW_TAG_member)
  <121e>     DW_AT_name        : (indirect string, offset: 0x8): dwActualLength
  <1222>     DW_AT_decl_file   : 6
  <1223>     DW_AT_decl_line   : 435
  <1225>     DW_AT_type        : <1a4>
  <1229>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><122c>: Abbrev Number: 19 (DW_TAG_member)
  <122d>     DW_AT_name        : (indirect string, offset: 0x66): pfIrpComplete
  <1231>     DW_AT_decl_file   : 6
  <1232>     DW_AT_decl_line   : 436
  <1234>     DW_AT_type        : <118f>
  <1238>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><123b>: Abbrev Number: 19 (DW_TAG_member)
  <123c>     DW_AT_name        : (indirect string, offset: 0x1f): pCompleteParam
  <1240>     DW_AT_decl_file   : 6
  <1241>     DW_AT_decl_line   : 437
  <1243>     DW_AT_type        : <f1>
  <1247>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><124a>: Abbrev Number: 20 (DW_TAG_member)
  <124b>     DW_AT_name        : bAllowShortTransfer
  <125f>     DW_AT_decl_file   : 6
  <1260>     DW_AT_decl_line   : 438
  <1262>     DW_AT_type        : <176>
  <1266>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1269>: Abbrev Number: 19 (DW_TAG_member)
  <126a>     DW_AT_name        : (indirect string, offset: 0x170): bIsrCallback
  <126e>     DW_AT_decl_file   : 6
  <126f>     DW_AT_decl_line   : 439
  <1271>     DW_AT_type        : <176>
  <1275>     DW_AT_data_member_location: 2 byte block: 23 1d    (DW_OP_plus_uconst: 29)
 <2><1278>: Abbrev Number: 19 (DW_TAG_member)
  <1279>     DW_AT_name        : (indirect string, offset: 0x17d): bAllowDma
  <127d>     DW_AT_decl_file   : 6
  <127e>     DW_AT_decl_line   : 440
  <1280>     DW_AT_type        : <176>
  <1284>     DW_AT_data_member_location: 2 byte block: 23 1e    (DW_OP_plus_uconst: 30)
 <1><1288>: Abbrev Number: 17 (DW_TAG_typedef)
  <1289>     DW_AT_name        : MUSB_Irp
  <1292>     DW_AT_decl_file   : 6
  <1293>     DW_AT_decl_line   : 441
  <1295>     DW_AT_type        : <11c7>
 <1><1299>: Abbrev Number: 17 (DW_TAG_typedef)
  <129a>     DW_AT_name        : MUSB_pfIsochIrpComplete
  <12b2>     DW_AT_decl_file   : 6
  <12b3>     DW_AT_decl_line   : 451
  <12b5>     DW_AT_type        : <12b9>
 <1><12b9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <12ba>     DW_AT_byte_size   : 4
  <12bb>     DW_AT_type        : <12bf>
 <1><12bf>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <12c0>     DW_AT_prototyped  : 1
  <12c1>     DW_AT_sibling     : <12d0>
 <2><12c5>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <12c6>     DW_AT_type        : <f1>
 <2><12ca>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <12cb>     DW_AT_type        : <12d0>
 <1><12d0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <12d1>     DW_AT_byte_size   : 4
  <12d2>     DW_AT_type        : <12d6>
 <1><12d6>: Abbrev Number: 24 (DW_TAG_structure_type)
  <12d7>     DW_AT_name        : _MUSB_IsochIrp
  <12e6>     DW_AT_byte_size   : 36
  <12e7>     DW_AT_decl_file   : 6
  <12e8>     DW_AT_decl_line   : 443
  <12ea>     DW_AT_sibling     : <13dd>
 <2><12ee>: Abbrev Number: 20 (DW_TAG_member)
  <12ef>     DW_AT_name        : hPipe
  <12f5>     DW_AT_decl_file   : 6
  <12f6>     DW_AT_decl_line   : 488
  <12f8>     DW_AT_type        : <102f>
  <12fc>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><12ff>: Abbrev Number: 19 (DW_TAG_member)
  <1300>     DW_AT_name        : (indirect string, offset: 0xb8): pBuffer
  <1304>     DW_AT_decl_file   : 6
  <1305>     DW_AT_decl_line   : 489
  <1307>     DW_AT_type        : <f28>
  <130b>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><130e>: Abbrev Number: 20 (DW_TAG_member)
  <130f>     DW_AT_name        : adwLength
  <1319>     DW_AT_decl_file   : 6
  <131a>     DW_AT_decl_line   : 490
  <131c>     DW_AT_type        : <13dd>
  <1320>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1323>: Abbrev Number: 20 (DW_TAG_member)
  <1324>     DW_AT_name        : adwStatus
  <132e>     DW_AT_decl_file   : 6
  <132f>     DW_AT_decl_line   : 491
  <1331>     DW_AT_type        : <13dd>
  <1335>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1338>: Abbrev Number: 20 (DW_TAG_member)
  <1339>     DW_AT_name        : adwActualLength
  <1349>     DW_AT_decl_file   : 6
  <134a>     DW_AT_decl_line   : 492
  <134c>     DW_AT_type        : <13dd>
  <1350>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1353>: Abbrev Number: 19 (DW_TAG_member)
  <1354>     DW_AT_name        : (indirect string, offset: 0x66): pfIrpComplete
  <1358>     DW_AT_decl_file   : 6
  <1359>     DW_AT_decl_line   : 493
  <135b>     DW_AT_type        : <1299>
  <135f>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1362>: Abbrev Number: 19 (DW_TAG_member)
  <1363>     DW_AT_name        : (indirect string, offset: 0x1f): pCompleteParam
  <1367>     DW_AT_decl_file   : 6
  <1368>     DW_AT_decl_line   : 494
  <136a>     DW_AT_type        : <f1>
  <136e>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><1371>: Abbrev Number: 20 (DW_TAG_member)
  <1372>     DW_AT_name        : wFrameCount
  <137e>     DW_AT_decl_file   : 6
  <137f>     DW_AT_decl_line   : 495
  <1381>     DW_AT_type        : <185>
  <1385>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><1388>: Abbrev Number: 20 (DW_TAG_member)
  <1389>     DW_AT_name        : wCurrentFrame
  <1397>     DW_AT_decl_file   : 6
  <1398>     DW_AT_decl_line   : 496
  <139a>     DW_AT_type        : <185>
  <139e>     DW_AT_data_member_location: 2 byte block: 23 1e    (DW_OP_plus_uconst: 30)
 <2><13a1>: Abbrev Number: 20 (DW_TAG_member)
  <13a2>     DW_AT_name        : wCallbackInterval
  <13b4>     DW_AT_decl_file   : 6
  <13b5>     DW_AT_decl_line   : 497
  <13b7>     DW_AT_type        : <185>
  <13bb>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><13be>: Abbrev Number: 19 (DW_TAG_member)
  <13bf>     DW_AT_name        : (indirect string, offset: 0x170): bIsrCallback
  <13c3>     DW_AT_decl_file   : 6
  <13c4>     DW_AT_decl_line   : 498
  <13c6>     DW_AT_type        : <176>
  <13ca>     DW_AT_data_member_location: 2 byte block: 23 22    (DW_OP_plus_uconst: 34)
 <2><13cd>: Abbrev Number: 19 (DW_TAG_member)
  <13ce>     DW_AT_name        : (indirect string, offset: 0x17d): bAllowDma
  <13d2>     DW_AT_decl_file   : 6
  <13d3>     DW_AT_decl_line   : 499
  <13d5>     DW_AT_type        : <176>
  <13d9>     DW_AT_data_member_location: 2 byte block: 23 23    (DW_OP_plus_uconst: 35)
 <1><13dd>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <13de>     DW_AT_byte_size   : 4
  <13df>     DW_AT_type        : <1a4>
 <1><13e3>: Abbrev Number: 17 (DW_TAG_typedef)
  <13e4>     DW_AT_name        : MUSB_IsochIrp
  <13f2>     DW_AT_decl_file   : 6
  <13f3>     DW_AT_decl_line   : 500
  <13f5>     DW_AT_type        : <12d6>
 <1><13f9>: Abbrev Number: 17 (DW_TAG_typedef)
  <13fa>     DW_AT_name        : MUSB_pfNewUsbState
  <140d>     DW_AT_decl_file   : 6
  <140e>     DW_AT_decl_line   : 515
  <1410>     DW_AT_type        : <1414>
 <1><1414>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1415>     DW_AT_byte_size   : 4
  <1416>     DW_AT_type        : <141a>
 <1><141a>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <141b>     DW_AT_prototyped  : 1
  <141c>     DW_AT_sibling     : <1430>
 <2><1420>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1421>     DW_AT_type        : <f1>
 <2><1425>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1426>     DW_AT_type        : <f48>
 <2><142a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <142b>     DW_AT_type        : <c0a>
 <1><1430>: Abbrev Number: 17 (DW_TAG_typedef)
  <1431>     DW_AT_name        : MUSB_pfDeviceRequest
  <1446>     DW_AT_decl_file   : 6
  <1447>     DW_AT_decl_line   : 537
  <1449>     DW_AT_type        : <144d>
 <1><144d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <144e>     DW_AT_byte_size   : 4
  <144f>     DW_AT_type        : <1453>
 <1><1453>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1454>     DW_AT_prototyped  : 1
  <1455>     DW_AT_type        : <176>
  <1459>     DW_AT_sibling     : <1477>
 <2><145d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <145e>     DW_AT_type        : <f1>
 <2><1462>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1463>     DW_AT_type        : <f48>
 <2><1467>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1468>     DW_AT_type        : <1a4>
 <2><146c>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <146d>     DW_AT_type        : <667>
 <2><1471>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1472>     DW_AT_type        : <185>
 <1><1477>: Abbrev Number: 17 (DW_TAG_typedef)
  <1478>     DW_AT_name        : MUSB_pfDeviceConfigSelected
  <1494>     DW_AT_decl_file   : 6
  <1495>     DW_AT_decl_line   : 567
  <1497>     DW_AT_type        : <149b>
 <1><149b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <149c>     DW_AT_byte_size   : 4
  <149d>     DW_AT_type        : <14a1>
 <1><14a1>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <14a2>     DW_AT_prototyped  : 1
  <14a3>     DW_AT_type        : <176>
  <14a7>     DW_AT_sibling     : <14c0>
 <2><14ab>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14ac>     DW_AT_type        : <f1>
 <2><14b0>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14b1>     DW_AT_type        : <f48>
 <2><14b5>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14b6>     DW_AT_type        : <176>
 <2><14ba>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14bb>     DW_AT_type        : <14c0>
 <1><14c0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14c1>     DW_AT_byte_size   : 4
  <14c2>     DW_AT_type        : <102f>
 <1><14c6>: Abbrev Number: 17 (DW_TAG_typedef)
  <14c7>     DW_AT_name        : MUSB_pfInterfaceSet
  <14db>     DW_AT_decl_file   : 6
  <14dc>     DW_AT_decl_line   : 598
  <14de>     DW_AT_type        : <14e2>
 <1><14e2>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <14e3>     DW_AT_byte_size   : 4
  <14e4>     DW_AT_type        : <14e8>
 <1><14e8>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <14e9>     DW_AT_prototyped  : 1
  <14ea>     DW_AT_type        : <176>
  <14ee>     DW_AT_sibling     : <150c>
 <2><14f2>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14f3>     DW_AT_type        : <f1>
 <2><14f7>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14f8>     DW_AT_type        : <f48>
 <2><14fc>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <14fd>     DW_AT_type        : <176>
 <2><1501>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1502>     DW_AT_type        : <176>
 <2><1506>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1507>     DW_AT_type        : <14c0>
 <1><150c>: Abbrev Number: 18 (DW_TAG_structure_type)
  <150d>     DW_AT_byte_size   : 44
  <150e>     DW_AT_decl_file   : 6
  <150f>     DW_AT_decl_line   : 654
  <1511>     DW_AT_sibling     : <167d>
 <2><1515>: Abbrev Number: 19 (DW_TAG_member)
  <1516>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <151a>     DW_AT_decl_file   : 6
  <151b>     DW_AT_decl_line   : 655
  <151d>     DW_AT_type        : <f1>
  <1521>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1524>: Abbrev Number: 20 (DW_TAG_member)
  <1525>     DW_AT_name        : pStandardDescriptors
  <153a>     DW_AT_decl_file   : 6
  <153b>     DW_AT_decl_line   : 656
  <153d>     DW_AT_type        : <667>
  <1541>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1544>: Abbrev Number: 19 (DW_TAG_member)
  <1545>     DW_AT_name        : (indirect string, offset: 0x1ae): wDescriptorBufferLength
  <1549>     DW_AT_decl_file   : 6
  <154a>     DW_AT_decl_line   : 657
  <154c>     DW_AT_type        : <185>
  <1550>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1553>: Abbrev Number: 20 (DW_TAG_member)
  <1554>     DW_AT_name        : wStringDescriptorCount
  <156b>     DW_AT_decl_file   : 6
  <156c>     DW_AT_decl_line   : 658
  <156e>     DW_AT_type        : <185>
  <1572>     DW_AT_data_member_location: 2 byte block: 23 a     (DW_OP_plus_uconst: 10)
 <2><1575>: Abbrev Number: 20 (DW_TAG_member)
  <1576>     DW_AT_name        : pHighSpeedDescriptors
  <158c>     DW_AT_decl_file   : 6
  <158d>     DW_AT_decl_line   : 659
  <158f>     DW_AT_type        : <667>
  <1593>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1596>: Abbrev Number: 20 (DW_TAG_member)
  <1597>     DW_AT_name        : wHighSpeedDescriptorLength
  <15b2>     DW_AT_decl_file   : 6
  <15b3>     DW_AT_decl_line   : 660
  <15b5>     DW_AT_type        : <185>
  <15b9>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><15bc>: Abbrev Number: 20 (DW_TAG_member)
  <15bd>     DW_AT_name        : wControlBufferLength
  <15d2>     DW_AT_decl_file   : 6
  <15d3>     DW_AT_decl_line   : 661
  <15d5>     DW_AT_type        : <185>
  <15d9>     DW_AT_data_member_location: 2 byte block: 23 12    (DW_OP_plus_uconst: 18)
 <2><15dc>: Abbrev Number: 20 (DW_TAG_member)
  <15dd>     DW_AT_name        : pControlBuffer
  <15ec>     DW_AT_decl_file   : 6
  <15ed>     DW_AT_decl_line   : 662
  <15ef>     DW_AT_type        : <f28>
  <15f3>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><15f6>: Abbrev Number: 20 (DW_TAG_member)
  <15f7>     DW_AT_name        : pbSelfPowered
  <1605>     DW_AT_decl_file   : 6
  <1606>     DW_AT_decl_line   : 663
  <1608>     DW_AT_type        : <f28>
  <160c>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><160f>: Abbrev Number: 20 (DW_TAG_member)
  <1610>     DW_AT_name        : pfDeviceRequest
  <1620>     DW_AT_decl_file   : 6
  <1621>     DW_AT_decl_line   : 664
  <1623>     DW_AT_type        : <1430>
  <1627>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><162a>: Abbrev Number: 20 (DW_TAG_member)
  <162b>     DW_AT_name        : pfDeviceConfigSelected
  <1642>     DW_AT_decl_file   : 6
  <1643>     DW_AT_decl_line   : 665
  <1645>     DW_AT_type        : <1477>
  <1649>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><164c>: Abbrev Number: 20 (DW_TAG_member)
  <164d>     DW_AT_name        : pfInterfaceSet
  <165c>     DW_AT_decl_file   : 6
  <165d>     DW_AT_decl_line   : 666
  <165f>     DW_AT_type        : <14c6>
  <1663>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><1666>: Abbrev Number: 20 (DW_TAG_member)
  <1667>     DW_AT_name        : pfUsbState
  <1672>     DW_AT_decl_file   : 6
  <1673>     DW_AT_decl_line   : 667
  <1675>     DW_AT_type        : <13f9>
  <1679>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <1><167d>: Abbrev Number: 17 (DW_TAG_typedef)
  <167e>     DW_AT_name        : MUSB_FunctionClient
  <1692>     DW_AT_decl_file   : 6
  <1693>     DW_AT_decl_line   : 668
  <1695>     DW_AT_type        : <150c>
 <1><1699>: Abbrev Number: 17 (DW_TAG_typedef)
  <169a>     DW_AT_name        : MUSB_pfDeviceConnected
  <16b1>     DW_AT_decl_file   : 6
  <16b2>     DW_AT_decl_line   : 684
  <16b4>     DW_AT_type        : <16b8>
 <1><16b8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <16b9>     DW_AT_byte_size   : 4
  <16ba>     DW_AT_type        : <16be>
 <1><16be>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <16bf>     DW_AT_prototyped  : 1
  <16c0>     DW_AT_type        : <176>
  <16c4>     DW_AT_sibling     : <16dd>
 <2><16c8>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <16c9>     DW_AT_type        : <f1>
 <2><16cd>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <16ce>     DW_AT_type        : <f48>
 <2><16d2>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <16d3>     DW_AT_type        : <16dd>
 <2><16d7>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <16d8>     DW_AT_type        : <667>
 <1><16dd>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <16de>     DW_AT_byte_size   : 4
  <16df>     DW_AT_type        : <f34>
 <1><16e3>: Abbrev Number: 17 (DW_TAG_typedef)
  <16e4>     DW_AT_name        : MUSB_pfDeviceDisconnected
  <16fe>     DW_AT_decl_file   : 6
  <16ff>     DW_AT_decl_line   : 694
  <1701>     DW_AT_type        : <1705>
 <1><1705>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1706>     DW_AT_byte_size   : 4
  <1707>     DW_AT_type        : <170b>
 <1><170b>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <170c>     DW_AT_prototyped  : 1
  <170d>     DW_AT_sibling     : <1721>
 <2><1711>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1712>     DW_AT_type        : <f1>
 <2><1716>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1717>     DW_AT_type        : <f48>
 <2><171b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <171c>     DW_AT_type        : <16dd>
 <1><1721>: Abbrev Number: 17 (DW_TAG_typedef)
  <1722>     DW_AT_name        : MUSB_pfDriverTimerExpired
  <173c>     DW_AT_decl_file   : 6
  <173d>     DW_AT_decl_line   : 702
  <173f>     DW_AT_type        : <1743>
 <1><1743>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1744>     DW_AT_byte_size   : 4
  <1745>     DW_AT_type        : <1749>
 <1><1749>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <174a>     DW_AT_prototyped  : 1
  <174b>     DW_AT_sibling     : <175a>
 <2><174f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1750>     DW_AT_type        : <f1>
 <2><1754>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1755>     DW_AT_type        : <f48>
 <1><175a>: Abbrev Number: 17 (DW_TAG_typedef)
  <175b>     DW_AT_name        : MUSB_pfBusSuspended
  <176f>     DW_AT_decl_file   : 6
  <1770>     DW_AT_decl_line   : 710
  <1772>     DW_AT_type        : <1743>
 <1><1776>: Abbrev Number: 17 (DW_TAG_typedef)
  <1777>     DW_AT_name        : MUSB_pfBusResumed
  <1789>     DW_AT_decl_file   : 6
  <178a>     DW_AT_decl_line   : 718
  <178c>     DW_AT_type        : <1743>
 <1><1790>: Abbrev Number: 24 (DW_TAG_structure_type)
  <1791>     DW_AT_name        : _MUSB_DeviceDriver
  <17a4>     DW_AT_byte_size   : 28
  <17a5>     DW_AT_decl_file   : 6
  <17a6>     DW_AT_decl_line   : 740
  <17a8>     DW_AT_sibling     : <1851>
 <2><17ac>: Abbrev Number: 19 (DW_TAG_member)
  <17ad>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <17b1>     DW_AT_decl_file   : 6
  <17b2>     DW_AT_decl_line   : 741
  <17b4>     DW_AT_type        : <f1>
  <17b8>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><17bb>: Abbrev Number: 20 (DW_TAG_member)
  <17bc>     DW_AT_name        : bTimerCount
  <17c8>     DW_AT_decl_file   : 6
  <17c9>     DW_AT_decl_line   : 742
  <17cb>     DW_AT_type        : <176>
  <17cf>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><17d2>: Abbrev Number: 19 (DW_TAG_member)
  <17d3>     DW_AT_name        : (indirect string, offset: 0x15c): adwTimerResolutions
  <17d7>     DW_AT_decl_file   : 6
  <17d8>     DW_AT_decl_line   : 743
  <17da>     DW_AT_type        : <13dd>
  <17de>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><17e1>: Abbrev Number: 20 (DW_TAG_member)
  <17e2>     DW_AT_name        : pfDeviceConnected
  <17f4>     DW_AT_decl_file   : 6
  <17f5>     DW_AT_decl_line   : 744
  <17f7>     DW_AT_type        : <1699>
  <17fb>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><17fe>: Abbrev Number: 20 (DW_TAG_member)
  <17ff>     DW_AT_name        : pfDeviceDisconnected
  <1814>     DW_AT_decl_file   : 6
  <1815>     DW_AT_decl_line   : 745
  <1817>     DW_AT_type        : <16e3>
  <181b>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><181e>: Abbrev Number: 20 (DW_TAG_member)
  <181f>     DW_AT_name        : pfBusSuspended
  <182e>     DW_AT_decl_file   : 6
  <182f>     DW_AT_decl_line   : 746
  <1831>     DW_AT_type        : <175a>
  <1835>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><1838>: Abbrev Number: 20 (DW_TAG_member)
  <1839>     DW_AT_name        : pfBusResumed
  <1846>     DW_AT_decl_file   : 6
  <1847>     DW_AT_decl_line   : 747
  <1849>     DW_AT_type        : <1776>
  <184d>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <1><1851>: Abbrev Number: 17 (DW_TAG_typedef)
  <1852>     DW_AT_name        : MUSB_DeviceDriver
  <1864>     DW_AT_decl_file   : 6
  <1865>     DW_AT_decl_line   : 748
  <1867>     DW_AT_type        : <1790>
 <1><186b>: Abbrev Number: 18 (DW_TAG_structure_type)
  <186c>     DW_AT_byte_size   : 16
  <186d>     DW_AT_decl_file   : 6
  <186e>     DW_AT_decl_line   : 765
  <1870>     DW_AT_sibling     : <18f1>
 <2><1874>: Abbrev Number: 20 (DW_TAG_member)
  <1875>     DW_AT_name        : pPeripheralList
  <1885>     DW_AT_decl_file   : 6
  <1886>     DW_AT_decl_line   : 766
  <1888>     DW_AT_type        : <667>
  <188c>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><188f>: Abbrev Number: 20 (DW_TAG_member)
  <1890>     DW_AT_name        : wPeripheralListLength
  <18a6>     DW_AT_decl_file   : 6
  <18a7>     DW_AT_decl_line   : 767
  <18a9>     DW_AT_type        : <185>
  <18ad>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><18b0>: Abbrev Number: 20 (DW_TAG_member)
  <18b1>     DW_AT_name        : aDeviceDriverList
  <18c3>     DW_AT_decl_file   : 6
  <18c4>     DW_AT_decl_line   : 768
  <18c6>     DW_AT_type        : <18f1>
  <18ca>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><18cd>: Abbrev Number: 20 (DW_TAG_member)
  <18ce>     DW_AT_name        : bDeviceDriverListLength
  <18e6>     DW_AT_decl_file   : 6
  <18e7>     DW_AT_decl_line   : 769
  <18e9>     DW_AT_type        : <176>
  <18ed>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><18f1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <18f2>     DW_AT_byte_size   : 4
  <18f3>     DW_AT_type        : <1851>
 <1><18f7>: Abbrev Number: 17 (DW_TAG_typedef)
  <18f8>     DW_AT_name        : MUSB_HostClient
  <1908>     DW_AT_decl_file   : 6
  <1909>     DW_AT_decl_line   : 770
  <190b>     DW_AT_type        : <186b>
 <1><190f>: Abbrev Number: 17 (DW_TAG_typedef)
  <1910>     DW_AT_name        : MUSB_pfNewOtgState
  <1923>     DW_AT_decl_file   : 6
  <1924>     DW_AT_decl_line   : 780
  <1926>     DW_AT_type        : <192a>
 <1><192a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <192b>     DW_AT_byte_size   : 4
  <192c>     DW_AT_type        : <1930>
 <1><1930>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <1931>     DW_AT_prototyped  : 1
  <1932>     DW_AT_sibling     : <1946>
 <2><1936>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1937>     DW_AT_type        : <f1>
 <2><193b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <193c>     DW_AT_type        : <f48>
 <2><1940>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1941>     DW_AT_type        : <ce7>
 <1><1946>: Abbrev Number: 17 (DW_TAG_typedef)
  <1947>     DW_AT_name        : MUSB_pfOtgError
  <1957>     DW_AT_decl_file   : 6
  <1958>     DW_AT_decl_line   : 791
  <195a>     DW_AT_type        : <195e>
 <1><195e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <195f>     DW_AT_byte_size   : 4
  <1960>     DW_AT_type        : <1964>
 <1><1964>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <1965>     DW_AT_prototyped  : 1
  <1966>     DW_AT_sibling     : <197a>
 <2><196a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <196b>     DW_AT_type        : <f1>
 <2><196f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1970>     DW_AT_type        : <f48>
 <2><1974>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1975>     DW_AT_type        : <1a4>
 <1><197a>: Abbrev Number: 18 (DW_TAG_structure_type)
  <197b>     DW_AT_byte_size   : 16
  <197c>     DW_AT_decl_file   : 6
  <197d>     DW_AT_decl_line   : 809
  <197f>     DW_AT_sibling     : <19db>
 <2><1983>: Abbrev Number: 19 (DW_TAG_member)
  <1984>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <1988>     DW_AT_decl_file   : 6
  <1989>     DW_AT_decl_line   : 810
  <198b>     DW_AT_type        : <f1>
  <198f>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1992>: Abbrev Number: 20 (DW_TAG_member)
  <1993>     DW_AT_name        : pbDesireHostRole
  <19a4>     DW_AT_decl_file   : 6
  <19a5>     DW_AT_decl_line   : 811
  <19a7>     DW_AT_type        : <f28>
  <19ab>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><19ae>: Abbrev Number: 20 (DW_TAG_member)
  <19af>     DW_AT_name        : pfOtgState
  <19ba>     DW_AT_decl_file   : 6
  <19bb>     DW_AT_decl_line   : 812
  <19bd>     DW_AT_type        : <190f>
  <19c1>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><19c4>: Abbrev Number: 20 (DW_TAG_member)
  <19c5>     DW_AT_name        : pfOtgError
  <19d0>     DW_AT_decl_file   : 6
  <19d1>     DW_AT_decl_line   : 813
  <19d3>     DW_AT_type        : <1946>
  <19d7>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <1><19db>: Abbrev Number: 17 (DW_TAG_typedef)
  <19dc>     DW_AT_name        : MUSB_OtgClient
  <19eb>     DW_AT_decl_file   : 6
  <19ec>     DW_AT_decl_line   : 814
  <19ee>     DW_AT_type        : <197a>
 <1><19f2>: Abbrev Number: 17 (DW_TAG_typedef)
  <19f3>     DW_AT_name        : MUSB_pfHubEnumerationComplete
  <1a11>     DW_AT_decl_file   : 6
  <1a12>     DW_AT_decl_line   : 825
  <1a14>     DW_AT_type        : <1a18>
 <1><1a18>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1a19>     DW_AT_byte_size   : 4
  <1a1a>     DW_AT_type        : <1a1e>
 <1><1a1e>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <1a1f>     DW_AT_prototyped  : 1
  <1a20>     DW_AT_sibling     : <1a2f>
 <2><1a24>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1a25>     DW_AT_type        : <16dd>
 <2><1a29>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1a2a>     DW_AT_type        : <16dd>
 <1><1a2f>: Abbrev Number: 12 (DW_TAG_structure_type)
  <1a30>     DW_AT_byte_size   : 24
  <1a31>     DW_AT_decl_file   : 7
  <1a32>     DW_AT_decl_line   : 43
  <1a33>     DW_AT_sibling     : <1ac9>
 <2><1a37>: Abbrev Number: 14 (DW_TAG_member)
  <1a38>     DW_AT_name        : wItemSize
  <1a42>     DW_AT_decl_file   : 7
  <1a43>     DW_AT_decl_line   : 44
  <1a44>     DW_AT_type        : <1c4>
  <1a48>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1a4b>: Abbrev Number: 14 (DW_TAG_member)
  <1a4c>     DW_AT_name        : wStaticItemCount
  <1a5d>     DW_AT_decl_file   : 7
  <1a5e>     DW_AT_decl_line   : 45
  <1a5f>     DW_AT_type        : <1c4>
  <1a63>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1a66>: Abbrev Number: 14 (DW_TAG_member)
  <1a67>     DW_AT_name        : pStaticBuffer
  <1a75>     DW_AT_decl_file   : 7
  <1a76>     DW_AT_decl_line   : 46
  <1a77>     DW_AT_type        : <f1>
  <1a7b>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1a7e>: Abbrev Number: 14 (DW_TAG_member)
  <1a7f>     DW_AT_name        : wItemCount
  <1a8a>     DW_AT_decl_file   : 7
  <1a8b>     DW_AT_decl_line   : 47
  <1a8c>     DW_AT_type        : <1c4>
  <1a90>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1a93>: Abbrev Number: 14 (DW_TAG_member)
  <1a94>     DW_AT_name        : wDynamicItemCount
  <1aa6>     DW_AT_decl_file   : 7
  <1aa7>     DW_AT_decl_line   : 48
  <1aa8>     DW_AT_type        : <1c4>
  <1aac>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><1aaf>: Abbrev Number: 14 (DW_TAG_member)
  <1ab0>     DW_AT_name        : pDynamicBuffer
  <1abf>     DW_AT_decl_file   : 7
  <1ac0>     DW_AT_decl_line   : 49
  <1ac1>     DW_AT_type        : <f1>
  <1ac5>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <1><1ac9>: Abbrev Number: 2 (DW_TAG_typedef)
  <1aca>     DW_AT_name        : MUSB_Array
  <1ad5>     DW_AT_decl_file   : 7
  <1ad6>     DW_AT_decl_line   : 50
  <1ad7>     DW_AT_type        : <1a2f>
 <1><1adb>: Abbrev Number: 23 (DW_TAG_structure_type)
  <1adc>     DW_AT_name        : _MUSB_LinkedList
  <1aed>     DW_AT_byte_size   : 12
  <1aee>     DW_AT_decl_file   : 7
  <1aef>     DW_AT_decl_line   : 110
  <1af0>     DW_AT_sibling     : <1b21>
 <2><1af4>: Abbrev Number: 13 (DW_TAG_member)
  <1af5>     DW_AT_name        : (indirect string, offset: 0x74): pItem
  <1af9>     DW_AT_decl_file   : 7
  <1afa>     DW_AT_decl_line   : 111
  <1afb>     DW_AT_type        : <f1>
  <1aff>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1b02>: Abbrev Number: 14 (DW_TAG_member)
  <1b03>     DW_AT_name        : pNext
  <1b09>     DW_AT_decl_file   : 7
  <1b0a>     DW_AT_decl_line   : 112
  <1b0b>     DW_AT_type        : <1b21>
  <1b0f>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1b12>: Abbrev Number: 13 (DW_TAG_member)
  <1b13>     DW_AT_name        : (indirect string, offset: 0x0): dwCount
  <1b17>     DW_AT_decl_file   : 7
  <1b18>     DW_AT_decl_line   : 113
  <1b19>     DW_AT_type        : <94>
  <1b1d>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><1b21>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1b22>     DW_AT_byte_size   : 4
  <1b23>     DW_AT_type        : <1adb>
 <1><1b27>: Abbrev Number: 2 (DW_TAG_typedef)
  <1b28>     DW_AT_name        : MUSB_LinkedList
  <1b38>     DW_AT_decl_file   : 7
  <1b39>     DW_AT_decl_line   : 114
  <1b3a>     DW_AT_type        : <1adb>
 <1><1b3e>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <1b3f>     DW_AT_byte_size   : 4
  <1b40>     DW_AT_decl_file   : 8
  <1b41>     DW_AT_decl_line   : 30
  <1b42>     DW_AT_sibling     : <1c65>
 <2><1b46>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1b47>     DW_AT_name        : MUSB_HSET_PORT_NONE
  <1b5b>     DW_AT_const_value : 0
 <2><1b5c>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1b5d>     DW_AT_name        : MUSB_HSET_PORT_TEST_J
  <1b73>     DW_AT_const_value : 1
 <2><1b74>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1b75>     DW_AT_name        : MUSB_HSET_PORT_TEST_K
  <1b8b>     DW_AT_const_value : 2
 <2><1b8c>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1b8d>     DW_AT_name        : MUSB_HSET_PORT_TEST_SE0_NAK
  <1ba9>     DW_AT_const_value : 3
 <2><1baa>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1bab>     DW_AT_name        : MUSB_HSET_PORT_TEST_PACKET
  <1bc6>     DW_AT_const_value : 4
 <2><1bc7>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1bc8>     DW_AT_name        : MUSB_HSET_PORT_RESET
  <1bdd>     DW_AT_const_value : 5
 <2><1bde>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1bdf>     DW_AT_name        : MUSB_HSET_PORT_SUSPEND
  <1bf6>     DW_AT_const_value : 6
 <2><1bf7>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1bf8>     DW_AT_name        : MUSB_HSET_PORT_RESUME
  <1c0e>     DW_AT_const_value : 7
 <2><1c0f>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1c10>     DW_AT_name        : MUSB_HSET_PORT_SETUP_START
  <1c2b>     DW_AT_const_value : 8
 <2><1c2c>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1c2d>     DW_AT_name        : MUSB_HSET_PORT_SETUP_IN
  <1c45>     DW_AT_const_value : 9
 <2><1c46>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1c47>     DW_AT_name        : MUSB_HSET_PORT_SETUP_STATUS
  <1c63>     DW_AT_const_value : 10
 <1><1c65>: Abbrev Number: 2 (DW_TAG_typedef)
  <1c66>     DW_AT_name        : MUSB_HsetPortMode
  <1c78>     DW_AT_decl_file   : 8
  <1c79>     DW_AT_decl_line   : 53
  <1c7a>     DW_AT_type        : <1b3e>
 <1><1c7e>: Abbrev Number: 25 (DW_TAG_enumeration_type)
  <1c7f>     DW_AT_name        : _MUSB_DmaChannelStatus
  <1c96>     DW_AT_byte_size   : 4
  <1c97>     DW_AT_decl_file   : 9
  <1c98>     DW_AT_decl_line   : 57
  <1c99>     DW_AT_sibling     : <1d1f>
 <2><1c9d>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1c9e>     DW_AT_name        : MUSB_DMA_STATUS_UNKNOWN
  <1cb6>     DW_AT_const_value : 0
 <2><1cb7>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1cb8>     DW_AT_name        : MUSB_DMA_STATUS_FREE
  <1ccd>     DW_AT_const_value : 1
 <2><1cce>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1ccf>     DW_AT_name        : MUSB_DMA_STATUS_BUSY
  <1ce4>     DW_AT_const_value : 2
 <2><1ce5>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1ce6>     DW_AT_name        : MUSB_DMA_STATUS_BUS_ABORT
  <1d00>     DW_AT_const_value : 3
 <2><1d01>: Abbrev Number: 22 (DW_TAG_enumerator)
  <1d02>     DW_AT_name        : MUSB_DMA_STATUS_CORE_ABORT
  <1d1d>     DW_AT_const_value : 4
 <1><1d1f>: Abbrev Number: 2 (DW_TAG_typedef)
  <1d20>     DW_AT_name        : MUSB_DmaChannelStatus
  <1d36>     DW_AT_decl_file   : 9
  <1d37>     DW_AT_decl_line   : 68
  <1d38>     DW_AT_type        : <1c7e>
 <1><1d3c>: Abbrev Number: 12 (DW_TAG_structure_type)
  <1d3d>     DW_AT_byte_size   : 20
  <1d3e>     DW_AT_decl_file   : 9
  <1d3f>     DW_AT_decl_line   : 83
  <1d40>     DW_AT_sibling     : <1da0>
 <2><1d44>: Abbrev Number: 13 (DW_TAG_member)
  <1d45>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <1d49>     DW_AT_decl_file   : 9
  <1d4a>     DW_AT_decl_line   : 84
  <1d4b>     DW_AT_type        : <f1>
  <1d4f>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1d52>: Abbrev Number: 14 (DW_TAG_member)
  <1d53>     DW_AT_name        : dwMaxLength
  <1d5f>     DW_AT_decl_file   : 9
  <1d60>     DW_AT_decl_line   : 85
  <1d61>     DW_AT_type        : <1a4>
  <1d65>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1d68>: Abbrev Number: 13 (DW_TAG_member)
  <1d69>     DW_AT_name        : (indirect string, offset: 0x8): dwActualLength
  <1d6d>     DW_AT_decl_file   : 9
  <1d6e>     DW_AT_decl_line   : 86
  <1d6f>     DW_AT_type        : <1a4>
  <1d73>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1d76>: Abbrev Number: 14 (DW_TAG_member)
  <1d77>     DW_AT_name        : bStatus
  <1d7f>     DW_AT_decl_file   : 9
  <1d80>     DW_AT_decl_line   : 87
  <1d81>     DW_AT_type        : <1d1f>
  <1d85>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><1d88>: Abbrev Number: 14 (DW_TAG_member)
  <1d89>     DW_AT_name        : bDesiredMode
  <1d96>     DW_AT_decl_file   : 9
  <1d97>     DW_AT_decl_line   : 88
  <1d98>     DW_AT_type        : <176>
  <1d9c>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <1><1da0>: Abbrev Number: 2 (DW_TAG_typedef)
  <1da1>     DW_AT_name        : MUSB_DmaChannel
  <1db1>     DW_AT_decl_file   : 9
  <1db2>     DW_AT_decl_line   : 89
  <1db3>     DW_AT_type        : <1d3c>
 <1><1db7>: Abbrev Number: 2 (DW_TAG_typedef)
  <1db8>     DW_AT_name        : MUSB_pfDmaStartController
  <1dd2>     DW_AT_decl_file   : 9
  <1dd3>     DW_AT_decl_line   : 98
  <1dd4>     DW_AT_type        : <1dd8>
 <1><1dd8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1dd9>     DW_AT_byte_size   : 4
  <1dda>     DW_AT_type        : <1dde>
 <1><1dde>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1ddf>     DW_AT_prototyped  : 1
  <1de0>     DW_AT_type        : <176>
  <1de4>     DW_AT_sibling     : <1dee>
 <2><1de8>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1de9>     DW_AT_type        : <f1>
 <1><1dee>: Abbrev Number: 2 (DW_TAG_typedef)
  <1def>     DW_AT_name        : MUSB_pfDmaStopController
  <1e08>     DW_AT_decl_file   : 9
  <1e09>     DW_AT_decl_line   : 107
  <1e0a>     DW_AT_type        : <1dd8>
 <1><1e0e>: Abbrev Number: 2 (DW_TAG_typedef)
  <1e0f>     DW_AT_name        : MUSB_pfDmaAllocateChannel
  <1e29>     DW_AT_decl_file   : 9
  <1e2a>     DW_AT_decl_line   : 123
  <1e2b>     DW_AT_type        : <1e2f>
 <1><1e2f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1e30>     DW_AT_byte_size   : 4
  <1e31>     DW_AT_type        : <1e35>
 <1><1e35>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1e36>     DW_AT_prototyped  : 1
  <1e37>     DW_AT_type        : <1e59>
  <1e3b>     DW_AT_sibling     : <1e59>
 <2><1e3f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e40>     DW_AT_type        : <f1>
 <2><1e44>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e45>     DW_AT_type        : <176>
 <2><1e49>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e4a>     DW_AT_type        : <176>
 <2><1e4e>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e4f>     DW_AT_type        : <176>
 <2><1e53>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e54>     DW_AT_type        : <185>
 <1><1e59>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1e5a>     DW_AT_byte_size   : 4
  <1e5b>     DW_AT_type        : <1da0>
 <1><1e5f>: Abbrev Number: 2 (DW_TAG_typedef)
  <1e60>     DW_AT_name        : MUSB_pfDmaReleaseChannel
  <1e79>     DW_AT_decl_file   : 9
  <1e7a>     DW_AT_decl_line   : 132
  <1e7b>     DW_AT_type        : <1e7f>
 <1><1e7f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1e80>     DW_AT_byte_size   : 4
  <1e81>     DW_AT_type        : <1e85>
 <1><1e85>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <1e86>     DW_AT_prototyped  : 1
  <1e87>     DW_AT_sibling     : <1e91>
 <2><1e8b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1e8c>     DW_AT_type        : <1e59>
 <1><1e91>: Abbrev Number: 2 (DW_TAG_typedef)
  <1e92>     DW_AT_name        : MUSB_pfDmaAllocateBuffer
  <1eab>     DW_AT_decl_file   : 9
  <1eac>     DW_AT_decl_line   : 145
  <1ead>     DW_AT_type        : <1eb1>
 <1><1eb1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1eb2>     DW_AT_byte_size   : 4
  <1eb3>     DW_AT_type        : <1eb7>
 <1><1eb7>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1eb8>     DW_AT_prototyped  : 1
  <1eb9>     DW_AT_type        : <f28>
  <1ebd>     DW_AT_sibling     : <1ecc>
 <2><1ec1>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1ec2>     DW_AT_type        : <1e59>
 <2><1ec6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1ec7>     DW_AT_type        : <1a4>
 <1><1ecc>: Abbrev Number: 2 (DW_TAG_typedef)
  <1ecd>     DW_AT_name        : MUSB_pfDmaReleaseBuffer
  <1ee5>     DW_AT_decl_file   : 9
  <1ee6>     DW_AT_decl_line   : 158
  <1ee7>     DW_AT_type        : <1eeb>
 <1><1eeb>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1eec>     DW_AT_byte_size   : 4
  <1eed>     DW_AT_type        : <1ef1>
 <1><1ef1>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1ef2>     DW_AT_prototyped  : 1
  <1ef3>     DW_AT_type        : <176>
  <1ef7>     DW_AT_sibling     : <1f06>
 <2><1efb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1efc>     DW_AT_type        : <1e59>
 <2><1f00>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f01>     DW_AT_type        : <f28>
 <1><1f06>: Abbrev Number: 2 (DW_TAG_typedef)
  <1f07>     DW_AT_name        : MUSB_pfDmaProgramChannel
  <1f20>     DW_AT_decl_file   : 9
  <1f21>     DW_AT_decl_line   : 178
  <1f22>     DW_AT_type        : <1f26>
 <1><1f26>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1f27>     DW_AT_byte_size   : 4
  <1f28>     DW_AT_type        : <1f2c>
 <1><1f2c>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1f2d>     DW_AT_prototyped  : 1
  <1f2e>     DW_AT_type        : <176>
  <1f32>     DW_AT_sibling     : <1f50>
 <2><1f36>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f37>     DW_AT_type        : <1e59>
 <2><1f3b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f3c>     DW_AT_type        : <185>
 <2><1f40>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f41>     DW_AT_type        : <176>
 <2><1f45>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f46>     DW_AT_type        : <667>
 <2><1f4a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f4b>     DW_AT_type        : <1a4>
 <1><1f50>: Abbrev Number: 2 (DW_TAG_typedef)
  <1f51>     DW_AT_name        : MUSB_pfDmaGetChannelStatus
  <1f6c>     DW_AT_decl_file   : 9
  <1f6d>     DW_AT_decl_line   : 189
  <1f6e>     DW_AT_type        : <1f72>
 <1><1f72>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <1f73>     DW_AT_byte_size   : 4
  <1f74>     DW_AT_type        : <1f78>
 <1><1f78>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <1f79>     DW_AT_prototyped  : 1
  <1f7a>     DW_AT_type        : <1d1f>
  <1f7e>     DW_AT_sibling     : <1f88>
 <2><1f82>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <1f83>     DW_AT_type        : <1e59>
 <1><1f88>: Abbrev Number: 2 (DW_TAG_typedef)
  <1f89>     DW_AT_name        : MUSB_pfDmaControllerIsr
  <1fa1>     DW_AT_decl_file   : 9
  <1fa2>     DW_AT_decl_line   : 209
  <1fa3>     DW_AT_type        : <1dd8>
 <1><1fa7>: Abbrev Number: 12 (DW_TAG_structure_type)
  <1fa8>     DW_AT_byte_size   : 40
  <1fa9>     DW_AT_decl_file   : 9
  <1faa>     DW_AT_decl_line   : 230
  <1fab>     DW_AT_sibling     : <20ce>
 <2><1faf>: Abbrev Number: 13 (DW_TAG_member)
  <1fb0>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <1fb4>     DW_AT_decl_file   : 9
  <1fb5>     DW_AT_decl_line   : 231
  <1fb6>     DW_AT_type        : <f1>
  <1fba>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><1fbd>: Abbrev Number: 14 (DW_TAG_member)
  <1fbe>     DW_AT_name        : pfDmaStartController
  <1fd3>     DW_AT_decl_file   : 9
  <1fd4>     DW_AT_decl_line   : 232
  <1fd5>     DW_AT_type        : <1db7>
  <1fd9>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><1fdc>: Abbrev Number: 14 (DW_TAG_member)
  <1fdd>     DW_AT_name        : pfDmaStopController
  <1ff1>     DW_AT_decl_file   : 9
  <1ff2>     DW_AT_decl_line   : 233
  <1ff3>     DW_AT_type        : <1dee>
  <1ff7>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><1ffa>: Abbrev Number: 14 (DW_TAG_member)
  <1ffb>     DW_AT_name        : pfDmaAllocateChannel
  <2010>     DW_AT_decl_file   : 9
  <2011>     DW_AT_decl_line   : 234
  <2012>     DW_AT_type        : <1e0e>
  <2016>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2019>: Abbrev Number: 14 (DW_TAG_member)
  <201a>     DW_AT_name        : pfDmaReleaseChannel
  <202e>     DW_AT_decl_file   : 9
  <202f>     DW_AT_decl_line   : 235
  <2030>     DW_AT_type        : <1e5f>
  <2034>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><2037>: Abbrev Number: 14 (DW_TAG_member)
  <2038>     DW_AT_name        : pfDmaAllocateBuffer
  <204c>     DW_AT_decl_file   : 9
  <204d>     DW_AT_decl_line   : 236
  <204e>     DW_AT_type        : <1e91>
  <2052>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><2055>: Abbrev Number: 14 (DW_TAG_member)
  <2056>     DW_AT_name        : pfDmaReleaseBuffer
  <2069>     DW_AT_decl_file   : 9
  <206a>     DW_AT_decl_line   : 237
  <206b>     DW_AT_type        : <1ecc>
  <206f>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><2072>: Abbrev Number: 14 (DW_TAG_member)
  <2073>     DW_AT_name        : pfDmaProgramChannel
  <2087>     DW_AT_decl_file   : 9
  <2088>     DW_AT_decl_line   : 238
  <2089>     DW_AT_type        : <1f06>
  <208d>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><2090>: Abbrev Number: 14 (DW_TAG_member)
  <2091>     DW_AT_name        : pfDmaGetChannelStatus
  <20a7>     DW_AT_decl_file   : 9
  <20a8>     DW_AT_decl_line   : 239
  <20a9>     DW_AT_type        : <1f50>
  <20ad>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><20b0>: Abbrev Number: 14 (DW_TAG_member)
  <20b1>     DW_AT_name        : pfDmaControllerIsr
  <20c4>     DW_AT_decl_file   : 9
  <20c5>     DW_AT_decl_line   : 240
  <20c6>     DW_AT_type        : <1f88>
  <20ca>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <1><20ce>: Abbrev Number: 2 (DW_TAG_typedef)
  <20cf>     DW_AT_name        : MUSB_DmaController
  <20e2>     DW_AT_decl_file   : 9
  <20e3>     DW_AT_decl_line   : 241
  <20e4>     DW_AT_type        : <1fa7>
 <1><20e8>: Abbrev Number: 17 (DW_TAG_typedef)
  <20e9>     DW_AT_name        : MUSB_pfDmaChannelStatusChanged
  <2108>     DW_AT_decl_file   : 9
  <2109>     DW_AT_decl_line   : 257
  <210b>     DW_AT_type        : <210f>
 <1><210f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2110>     DW_AT_byte_size   : 4
  <2111>     DW_AT_type        : <2115>
 <1><2115>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <2116>     DW_AT_prototyped  : 1
  <2117>     DW_AT_type        : <176>
  <211b>     DW_AT_sibling     : <212f>
 <2><211f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2120>     DW_AT_type        : <f1>
 <2><2124>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2125>     DW_AT_type        : <176>
 <2><2129>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <212a>     DW_AT_type        : <176>
 <1><212f>: Abbrev Number: 17 (DW_TAG_typedef)
  <2130>     DW_AT_name        : MUSB_pfNewDmaController
  <2148>     DW_AT_decl_file   : 9
  <2149>     DW_AT_decl_line   : 290
  <214b>     DW_AT_type        : <214f>
 <1><214f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2150>     DW_AT_byte_size   : 4
  <2151>     DW_AT_type        : <2155>
 <1><2155>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <2156>     DW_AT_prototyped  : 1
  <2157>     DW_AT_type        : <217e>
  <215b>     DW_AT_sibling     : <217e>
 <2><215f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2160>     DW_AT_type        : <20e8>
 <2><2164>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2165>     DW_AT_type        : <f1>
 <2><2169>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <216a>     DW_AT_type        : <468>
 <2><216e>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <216f>     DW_AT_type        : <f1>
 <2><2173>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2174>     DW_AT_type        : <f28>
 <2><2178>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <2179>     DW_AT_type        : <f28>
 <1><217e>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <217f>     DW_AT_byte_size   : 4
  <2180>     DW_AT_type        : <20ce>
 <1><2184>: Abbrev Number: 17 (DW_TAG_typedef)
  <2185>     DW_AT_name        : MUSB_pfDestroyDmaController
  <21a1>     DW_AT_decl_file   : 9
  <21a2>     DW_AT_decl_line   : 296
  <21a4>     DW_AT_type        : <21a8>
 <1><21a8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <21a9>     DW_AT_byte_size   : 4
  <21aa>     DW_AT_type        : <21ae>
 <1><21ae>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <21af>     DW_AT_prototyped  : 1
  <21b0>     DW_AT_sibling     : <21ba>
 <2><21b4>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <21b5>     DW_AT_type        : <217e>
 <1><21ba>: Abbrev Number: 18 (DW_TAG_structure_type)
  <21bb>     DW_AT_byte_size   : 12
  <21bc>     DW_AT_decl_file   : 9
  <21bd>     DW_AT_decl_line   : 313
  <21bf>     DW_AT_sibling     : <2224>
 <2><21c3>: Abbrev Number: 20 (DW_TAG_member)
  <21c4>     DW_AT_name        : wCoreRegistersExtent
  <21d9>     DW_AT_decl_file   : 9
  <21da>     DW_AT_decl_line   : 314
  <21dc>     DW_AT_type        : <185>
  <21e0>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><21e3>: Abbrev Number: 20 (DW_TAG_member)
  <21e4>     DW_AT_name        : pfNewDmaController
  <21f7>     DW_AT_decl_file   : 9
  <21f8>     DW_AT_decl_line   : 315
  <21fa>     DW_AT_type        : <212f>
  <21fe>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2201>: Abbrev Number: 20 (DW_TAG_member)
  <2202>     DW_AT_name        : pfDestroyDmaController
  <2219>     DW_AT_decl_file   : 9
  <221a>     DW_AT_decl_line   : 316
  <221c>     DW_AT_type        : <2184>
  <2220>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <1><2224>: Abbrev Number: 17 (DW_TAG_typedef)
  <2225>     DW_AT_name        : MUSB_DmaControllerFactory
  <223f>     DW_AT_decl_file   : 9
  <2240>     DW_AT_decl_line   : 317
  <2242>     DW_AT_type        : <21ba>
 <1><2246>: Abbrev Number: 12 (DW_TAG_structure_type)
  <2247>     DW_AT_byte_size   : 8
  <2248>     DW_AT_decl_file   : 10
  <2249>     DW_AT_decl_line   : 148
  <224a>     DW_AT_sibling     : <2290>
 <2><224e>: Abbrev Number: 14 (DW_TAG_member)
  <224f>     DW_AT_name        : pData
  <2255>     DW_AT_decl_file   : 10
  <2256>     DW_AT_decl_line   : 152
  <2257>     DW_AT_type        : <f1>
  <225b>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><225e>: Abbrev Number: 14 (DW_TAG_member)
  <225f>     DW_AT_name        : bCause
  <2266>     DW_AT_decl_file   : 10
  <2267>     DW_AT_decl_line   : 153
  <2268>     DW_AT_type        : <176>
  <226c>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><226f>: Abbrev Number: 13 (DW_TAG_member)
  <2270>     DW_AT_name        : (indirect string, offset: 0x111): bLocalEnd
  <2274>     DW_AT_decl_file   : 10
  <2275>     DW_AT_decl_line   : 154
  <2276>     DW_AT_type        : <176>
  <227a>     DW_AT_data_member_location: 2 byte block: 23 5     (DW_OP_plus_uconst: 5)
 <2><227d>: Abbrev Number: 14 (DW_TAG_member)
  <227e>     DW_AT_name        : bStatus
  <2286>     DW_AT_decl_file   : 10
  <2287>     DW_AT_decl_line   : 155
  <2288>     DW_AT_type        : <176>
  <228c>     DW_AT_data_member_location: 2 byte block: 23 6     (DW_OP_plus_uconst: 6)
 <1><2290>: Abbrev Number: 2 (DW_TAG_typedef)
  <2291>     DW_AT_name        : MGC_BsrItem
  <229d>     DW_AT_decl_file   : 10
  <229e>     DW_AT_decl_line   : 156
  <229f>     DW_AT_type        : <2246>
 <1><22a3>: Abbrev Number: 18 (DW_TAG_structure_type)
  <22a4>     DW_AT_byte_size   : 96
  <22a5>     DW_AT_decl_file   : 10
  <22a6>     DW_AT_decl_line   : 269
  <22a8>     DW_AT_sibling     : <25f9>
 <2><22ac>: Abbrev Number: 19 (DW_TAG_member)
  <22ad>     DW_AT_name        : (indirect string, offset: 0x111): bLocalEnd
  <22b1>     DW_AT_decl_file   : 10
  <22b2>     DW_AT_decl_line   : 273
  <22b4>     DW_AT_type        : <176>
  <22b8>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><22bb>: Abbrev Number: 20 (DW_TAG_member)
  <22bc>     DW_AT_name        : bIsFifoShared
  <22ca>     DW_AT_decl_file   : 10
  <22cb>     DW_AT_decl_line   : 274
  <22cd>     DW_AT_type        : <176>
  <22d1>     DW_AT_data_member_location: 2 byte block: 23 1     (DW_OP_plus_uconst: 1)
 <2><22d4>: Abbrev Number: 20 (DW_TAG_member)
  <22d5>     DW_AT_name        : wMaxPacketSizeTx
  <22e6>     DW_AT_decl_file   : 10
  <22e7>     DW_AT_decl_line   : 275
  <22e9>     DW_AT_type        : <185>
  <22ed>     DW_AT_data_member_location: 2 byte block: 23 2     (DW_OP_plus_uconst: 2)
 <2><22f0>: Abbrev Number: 20 (DW_TAG_member)
  <22f1>     DW_AT_name        : wMaxPacketSizeRx
  <2302>     DW_AT_decl_file   : 10
  <2303>     DW_AT_decl_line   : 276
  <2305>     DW_AT_type        : <185>
  <2309>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><230c>: Abbrev Number: 19 (DW_TAG_member)
  <230d>     DW_AT_name        : (indirect string, offset: 0x11b): bBusAddress
  <2311>     DW_AT_decl_file   : 10
  <2312>     DW_AT_decl_line   : 279
  <2314>     DW_AT_type        : <176>
  <2318>     DW_AT_data_member_location: 2 byte block: 23 6     (DW_OP_plus_uconst: 6)
 <2><231b>: Abbrev Number: 20 (DW_TAG_member)
  <231c>     DW_AT_name        : bBusEnd
  <2324>     DW_AT_decl_file   : 10
  <2325>     DW_AT_decl_line   : 280
  <2327>     DW_AT_type        : <176>
  <232b>     DW_AT_data_member_location: 2 byte block: 23 7     (DW_OP_plus_uconst: 7)
 <2><232e>: Abbrev Number: 19 (DW_TAG_member)
  <232f>     DW_AT_name        : (indirect string, offset: 0x86): bInterval
  <2333>     DW_AT_decl_file   : 10
  <2334>     DW_AT_decl_line   : 281
  <2336>     DW_AT_type        : <176>
  <233a>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><233d>: Abbrev Number: 19 (DW_TAG_member)
  <233e>     DW_AT_name        : (indirect string, offset: 0x99): bTrafficType
  <2342>     DW_AT_decl_file   : 10
  <2343>     DW_AT_decl_line   : 282
  <2345>     DW_AT_type        : <176>
  <2349>     DW_AT_data_member_location: 2 byte block: 23 9     (DW_OP_plus_uconst: 9)
 <2><234c>: Abbrev Number: 20 (DW_TAG_member)
  <234d>     DW_AT_name        : bRxBusAddress
  <235b>     DW_AT_decl_file   : 10
  <235c>     DW_AT_decl_line   : 283
  <235e>     DW_AT_type        : <176>
  <2362>     DW_AT_data_member_location: 2 byte block: 23 a     (DW_OP_plus_uconst: 10)
 <2><2365>: Abbrev Number: 20 (DW_TAG_member)
  <2366>     DW_AT_name        : bRxBusEnd
  <2370>     DW_AT_decl_file   : 10
  <2371>     DW_AT_decl_line   : 284
  <2373>     DW_AT_type        : <176>
  <2377>     DW_AT_data_member_location: 2 byte block: 23 b     (DW_OP_plus_uconst: 11)
 <2><237a>: Abbrev Number: 20 (DW_TAG_member)
  <237b>     DW_AT_name        : bRxInterval
  <2387>     DW_AT_decl_file   : 10
  <2388>     DW_AT_decl_line   : 285
  <238a>     DW_AT_type        : <176>
  <238e>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2391>: Abbrev Number: 20 (DW_TAG_member)
  <2392>     DW_AT_name        : bRxTrafficType
  <23a1>     DW_AT_decl_file   : 10
  <23a2>     DW_AT_decl_line   : 286
  <23a4>     DW_AT_type        : <176>
  <23a8>     DW_AT_data_member_location: 2 byte block: 23 d     (DW_OP_plus_uconst: 13)
 <2><23ab>: Abbrev Number: 20 (DW_TAG_member)
  <23ac>     DW_AT_name        : bIsTx
  <23b2>     DW_AT_decl_file   : 10
  <23b3>     DW_AT_decl_line   : 287
  <23b5>     DW_AT_type        : <176>
  <23b9>     DW_AT_data_member_location: 2 byte block: 23 e     (DW_OP_plus_uconst: 14)
 <2><23bc>: Abbrev Number: 20 (DW_TAG_member)
  <23bd>     DW_AT_name        : bIsHalted
  <23c7>     DW_AT_decl_file   : 10
  <23c8>     DW_AT_decl_line   : 288
  <23ca>     DW_AT_type        : <176>
  <23ce>     DW_AT_data_member_location: 2 byte block: 23 f     (DW_OP_plus_uconst: 15)
 <2><23d1>: Abbrev Number: 20 (DW_TAG_member)
  <23d2>     DW_AT_name        : bIsClaimed
  <23dd>     DW_AT_decl_file   : 10
  <23de>     DW_AT_decl_line   : 289
  <23e0>     DW_AT_type        : <176>
  <23e4>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><23e7>: Abbrev Number: 20 (DW_TAG_member)
  <23e8>     DW_AT_name        : bRxClaimed
  <23f3>     DW_AT_decl_file   : 10
  <23f4>     DW_AT_decl_line   : 290
  <23f6>     DW_AT_type        : <176>
  <23fa>     DW_AT_data_member_location: 2 byte block: 23 11    (DW_OP_plus_uconst: 17)
 <2><23fd>: Abbrev Number: 20 (DW_TAG_member)
  <23fe>     DW_AT_name        : bStopTx
  <2406>     DW_AT_decl_file   : 10
  <2407>     DW_AT_decl_line   : 291
  <2409>     DW_AT_type        : <176>
  <240d>     DW_AT_data_member_location: 2 byte block: 23 12    (DW_OP_plus_uconst: 18)
 <2><2410>: Abbrev Number: 20 (DW_TAG_member)
  <2411>     DW_AT_name        : bIsRxHalted
  <241d>     DW_AT_decl_file   : 10
  <241e>     DW_AT_decl_line   : 292
  <2420>     DW_AT_type        : <176>
  <2424>     DW_AT_data_member_location: 2 byte block: 23 13    (DW_OP_plus_uconst: 19)
 <2><2427>: Abbrev Number: 20 (DW_TAG_member)
  <2428>     DW_AT_name        : wPacketSize
  <2434>     DW_AT_decl_file   : 10
  <2435>     DW_AT_decl_line   : 293
  <2437>     DW_AT_type        : <185>
  <243b>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><243e>: Abbrev Number: 20 (DW_TAG_member)
  <243f>     DW_AT_name        : wRxPacketSize
  <244d>     DW_AT_decl_file   : 10
  <244e>     DW_AT_decl_line   : 294
  <2450>     DW_AT_type        : <185>
  <2454>     DW_AT_data_member_location: 2 byte block: 23 16    (DW_OP_plus_uconst: 22)
 <2><2457>: Abbrev Number: 20 (DW_TAG_member)
  <2458>     DW_AT_name        : dwTxSize
  <2461>     DW_AT_decl_file   : 10
  <2462>     DW_AT_decl_line   : 295
  <2464>     DW_AT_type        : <1a4>
  <2468>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><246b>: Abbrev Number: 20 (DW_TAG_member)
  <246c>     DW_AT_name        : pTxIrp
  <2473>     DW_AT_decl_file   : 10
  <2474>     DW_AT_decl_line   : 297
  <2476>     DW_AT_type        : <f1>
  <247a>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><247d>: Abbrev Number: 20 (DW_TAG_member)
  <247e>     DW_AT_name        : TxIrpList
  <2488>     DW_AT_decl_file   : 10
  <2489>     DW_AT_decl_line   : 298
  <248b>     DW_AT_type        : <1b27>
  <248f>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><2492>: Abbrev Number: 20 (DW_TAG_member)
  <2493>     DW_AT_name        : dwTxOffset
  <249e>     DW_AT_decl_file   : 10
  <249f>     DW_AT_decl_line   : 299
  <24a1>     DW_AT_type        : <1a4>
  <24a5>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><24a8>: Abbrev Number: 20 (DW_TAG_member)
  <24a9>     DW_AT_name        : pRxIrp
  <24b0>     DW_AT_decl_file   : 10
  <24b1>     DW_AT_decl_line   : 300
  <24b3>     DW_AT_type        : <f1>
  <24b7>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><24ba>: Abbrev Number: 20 (DW_TAG_member)
  <24bb>     DW_AT_name        : RxIrpList
  <24c5>     DW_AT_decl_file   : 10
  <24c6>     DW_AT_decl_line   : 301
  <24c8>     DW_AT_type        : <1b27>
  <24cc>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><24cf>: Abbrev Number: 20 (DW_TAG_member)
  <24d0>     DW_AT_name        : dwRxOffset
  <24db>     DW_AT_decl_file   : 10
  <24dc>     DW_AT_decl_line   : 302
  <24de>     DW_AT_type        : <1a4>
  <24e2>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><24e5>: Abbrev Number: 19 (DW_TAG_member)
  <24e6>     DW_AT_name        : (indirect string, offset: 0x7a): pDmaChannel
  <24ea>     DW_AT_decl_file   : 10
  <24eb>     DW_AT_decl_line   : 308
  <24ed>     DW_AT_type        : <1e59>
  <24f1>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><24f4>: Abbrev Number: 20 (DW_TAG_member)
  <24f5>     DW_AT_name        : pRxDmaChannel
  <2503>     DW_AT_decl_file   : 10
  <2504>     DW_AT_decl_line   : 309
  <2506>     DW_AT_type        : <1e59>
  <250a>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><250d>: Abbrev Number: 20 (DW_TAG_member)
  <250e>     DW_AT_name        : wDmaBufferCount
  <251e>     DW_AT_decl_file   : 10
  <251f>     DW_AT_decl_line   : 310
  <2521>     DW_AT_type        : <185>
  <2525>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><2528>: Abbrev Number: 20 (DW_TAG_member)
  <2529>     DW_AT_name        : wRxDmaBufferCount
  <253b>     DW_AT_decl_file   : 10
  <253c>     DW_AT_decl_line   : 311
  <253e>     DW_AT_type        : <185>
  <2542>     DW_AT_data_member_location: 2 byte block: 23 4e    (DW_OP_plus_uconst: 78)
 <2><2545>: Abbrev Number: 20 (DW_TAG_member)
  <2546>     DW_AT_name        : bDmaTx
  <254d>     DW_AT_decl_file   : 10
  <254e>     DW_AT_decl_line   : 312
  <2550>     DW_AT_type        : <176>
  <2554>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><2557>: Abbrev Number: 20 (DW_TAG_member)
  <2558>     DW_AT_name        : bDmaResidual
  <2565>     DW_AT_decl_file   : 10
  <2566>     DW_AT_decl_line   : 313
  <2568>     DW_AT_type        : <176>
  <256c>     DW_AT_data_member_location: 2 byte block: 23 51    (DW_OP_plus_uconst: 81)
 <2><256f>: Abbrev Number: 19 (DW_TAG_member)
  <2570>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <2574>     DW_AT_decl_file   : 10
  <2575>     DW_AT_decl_line   : 316
  <2577>     DW_AT_type        : <f1>
  <257b>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <2><257e>: Abbrev Number: 20 (DW_TAG_member)
  <257f>     DW_AT_name        : dwWaitFrameCount
  <2590>     DW_AT_decl_file   : 10
  <2591>     DW_AT_decl_line   : 317
  <2593>     DW_AT_type        : <1a4>
  <2597>     DW_AT_data_member_location: 2 byte block: 23 58    (DW_OP_plus_uconst: 88)
 <2><259a>: Abbrev Number: 20 (DW_TAG_member)
  <259b>     DW_AT_name        : bTxFifoSize
  <25a7>     DW_AT_decl_file   : 10
  <25a8>     DW_AT_decl_line   : 320
  <25aa>     DW_AT_type        : <176>
  <25ae>     DW_AT_data_member_location: 2 byte block: 23 5c    (DW_OP_plus_uconst: 92)
 <2><25b1>: Abbrev Number: 20 (DW_TAG_member)
  <25b2>     DW_AT_name        : bRxFifoSize
  <25be>     DW_AT_decl_file   : 10
  <25bf>     DW_AT_decl_line   : 321
  <25c1>     DW_AT_type        : <176>
  <25c5>     DW_AT_data_member_location: 2 byte block: 23 5d    (DW_OP_plus_uconst: 93)
 <2><25c8>: Abbrev Number: 20 (DW_TAG_member)
  <25c9>     DW_AT_name        : bIsrResidual
  <25d6>     DW_AT_decl_file   : 10
  <25d7>     DW_AT_decl_line   : 323
  <25d9>     DW_AT_type        : <176>
  <25dd>     DW_AT_data_member_location: 2 byte block: 23 5e    (DW_OP_plus_uconst: 94)
 <2><25e0>: Abbrev Number: 20 (DW_TAG_member)
  <25e1>     DW_AT_name        : bBsrResidual
  <25ee>     DW_AT_decl_file   : 10
  <25ef>     DW_AT_decl_line   : 324
  <25f1>     DW_AT_type        : <176>
  <25f5>     DW_AT_data_member_location: 2 byte block: 23 5f    (DW_OP_plus_uconst: 95)
 <1><25f9>: Abbrev Number: 17 (DW_TAG_typedef)
  <25fa>     DW_AT_name        : MGC_EndpointResource
  <260f>     DW_AT_decl_file   : 10
  <2610>     DW_AT_decl_line   : 325
  <2612>     DW_AT_type        : <22a3>
 <1><2616>: Abbrev Number: 24 (DW_TAG_structure_type)
  <2617>     DW_AT_name        : _MGC_Pipe
  <2621>     DW_AT_byte_size   : 36
  <2622>     DW_AT_decl_file   : 10
  <2623>     DW_AT_decl_line   : 344
  <2625>     DW_AT_sibling     : <26e0>
 <2><2629>: Abbrev Number: 20 (DW_TAG_member)
  <262a>     DW_AT_name        : hSession
  <2633>     DW_AT_decl_file   : 10
  <2634>     DW_AT_decl_line   : 345
  <2636>     DW_AT_type        : <f1>
  <263a>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><263d>: Abbrev Number: 20 (DW_TAG_member)
  <263e>     DW_AT_name        : pLocalEnd
  <2648>     DW_AT_decl_file   : 10
  <2649>     DW_AT_decl_line   : 346
  <264b>     DW_AT_type        : <26e0>
  <264f>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2652>: Abbrev Number: 19 (DW_TAG_member)
  <2653>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <2657>     DW_AT_decl_file   : 10
  <2658>     DW_AT_decl_line   : 347
  <265a>     DW_AT_type        : <2fa4>
  <265e>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><2661>: Abbrev Number: 20 (DW_TAG_member)
  <2662>     DW_AT_name        : pSlot
  <2668>     DW_AT_decl_file   : 10
  <2669>     DW_AT_decl_line   : 348
  <266b>     DW_AT_type        : <2fd9>
  <266f>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2672>: Abbrev Number: 19 (DW_TAG_member)
  <2673>     DW_AT_name        : (indirect string, offset: 0x1db): pDevice
  <2677>     DW_AT_decl_file   : 10
  <2678>     DW_AT_decl_line   : 349
  <267a>     DW_AT_type        : <fa6>
  <267e>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><2681>: Abbrev Number: 19 (DW_TAG_member)
  <2682>     DW_AT_name        : (indirect string, offset: 0x2e): wMaxPacketSize
  <2686>     DW_AT_decl_file   : 10
  <2687>     DW_AT_decl_line   : 350
  <2689>     DW_AT_type        : <185>
  <268d>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><2690>: Abbrev Number: 20 (DW_TAG_member)
  <2691>     DW_AT_name        : wInterval
  <269b>     DW_AT_decl_file   : 10
  <269c>     DW_AT_decl_line   : 351
  <269e>     DW_AT_type        : <185>
  <26a2>     DW_AT_data_member_location: 2 byte block: 23 16    (DW_OP_plus_uconst: 22)
 <2><26a5>: Abbrev Number: 20 (DW_TAG_member)
  <26a6>     DW_AT_name        : dwMaxBusTime
  <26b3>     DW_AT_decl_file   : 10
  <26b4>     DW_AT_decl_line   : 352
  <26b6>     DW_AT_type        : <1a4>
  <26ba>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><26bd>: Abbrev Number: 20 (DW_TAG_member)
  <26be>     DW_AT_name        : bmFlags
  <26c6>     DW_AT_decl_file   : 10
  <26c7>     DW_AT_decl_line   : 353
  <26c9>     DW_AT_type        : <1a4>
  <26cd>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><26d0>: Abbrev Number: 19 (DW_TAG_member)
  <26d1>     DW_AT_name        : (indirect string, offset: 0x99): bTrafficType
  <26d5>     DW_AT_decl_file   : 10
  <26d6>     DW_AT_decl_line   : 354
  <26d8>     DW_AT_type        : <176>
  <26dc>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <1><26e0>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <26e1>     DW_AT_byte_size   : 4
  <26e2>     DW_AT_type        : <25f9>
 <1><26e6>: Abbrev Number: 26 (DW_TAG_structure_type)
  <26e7>     DW_AT_name        : _MGC_Port
  <26f1>     DW_AT_byte_size   : 608
  <26f3>     DW_AT_decl_file   : 10
  <26f4>     DW_AT_decl_line   : 241
  <26f5>     DW_AT_sibling     : <2fa4>
 <2><26f9>: Abbrev Number: 19 (DW_TAG_member)
  <26fa>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <26fe>     DW_AT_decl_file   : 10
  <26ff>     DW_AT_decl_line   : 866
  <2701>     DW_AT_type        : <390d>
  <2705>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><2708>: Abbrev Number: 20 (DW_TAG_member)
  <2709>     DW_AT_name        : pInterfacePort
  <2718>     DW_AT_decl_file   : 10
  <2719>     DW_AT_decl_line   : 867
  <271b>     DW_AT_type        : <f16>
  <271f>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><2722>: Abbrev Number: 19 (DW_TAG_member)
  <2723>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <2727>     DW_AT_decl_file   : 10
  <2728>     DW_AT_decl_line   : 868
  <272a>     DW_AT_type        : <f1>
  <272e>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><2731>: Abbrev Number: 20 (DW_TAG_member)
  <2732>     DW_AT_name        : LocalEnds
  <273c>     DW_AT_decl_file   : 10
  <273d>     DW_AT_decl_line   : 869
  <273f>     DW_AT_type        : <1ac9>
  <2743>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><2746>: Abbrev Number: 20 (DW_TAG_member)
  <2747>     DW_AT_name        : bWantHighSpeed
  <2756>     DW_AT_decl_file   : 10
  <2757>     DW_AT_decl_line   : 872
  <2759>     DW_AT_type        : <176>
  <275d>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><2760>: Abbrev Number: 20 (DW_TAG_member)
  <2761>     DW_AT_name        : bWantReset
  <276c>     DW_AT_decl_file   : 10
  <276d>     DW_AT_decl_line   : 873
  <276f>     DW_AT_type        : <176>
  <2773>     DW_AT_data_member_location: 2 byte block: 23 25    (DW_OP_plus_uconst: 37)
 <2><2776>: Abbrev Number: 20 (DW_TAG_member)
  <2777>     DW_AT_name        : bWantSuspend
  <2784>     DW_AT_decl_file   : 10
  <2785>     DW_AT_decl_line   : 874
  <2787>     DW_AT_type        : <176>
  <278b>     DW_AT_data_member_location: 2 byte block: 23 26    (DW_OP_plus_uconst: 38)
 <2><278e>: Abbrev Number: 20 (DW_TAG_member)
  <278f>     DW_AT_name        : bWantResume
  <279b>     DW_AT_decl_file   : 10
  <279c>     DW_AT_decl_line   : 875
  <279e>     DW_AT_type        : <176>
  <27a2>     DW_AT_data_member_location: 2 byte block: 23 27    (DW_OP_plus_uconst: 39)
 <2><27a5>: Abbrev Number: 19 (DW_TAG_member)
  <27a6>     DW_AT_name        : (indirect string, offset: 0xc0): bFuncAddr
  <27aa>     DW_AT_decl_file   : 10
  <27ab>     DW_AT_decl_line   : 876
  <27ad>     DW_AT_type        : <176>
  <27b1>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><27b4>: Abbrev Number: 20 (DW_TAG_member)
  <27b5>     DW_AT_name        : bWantSession
  <27c2>     DW_AT_decl_file   : 10
  <27c3>     DW_AT_decl_line   : 877
  <27c5>     DW_AT_type        : <176>
  <27c9>     DW_AT_data_member_location: 2 byte block: 23 29    (DW_OP_plus_uconst: 41)
 <2><27cc>: Abbrev Number: 20 (DW_TAG_member)
  <27cd>     DW_AT_name        : bWantHost
  <27d7>     DW_AT_decl_file   : 10
  <27d8>     DW_AT_decl_line   : 878
  <27da>     DW_AT_type        : <176>
  <27de>     DW_AT_data_member_location: 2 byte block: 23 2a    (DW_OP_plus_uconst: 42)
 <2><27e1>: Abbrev Number: 20 (DW_TAG_member)
  <27e2>     DW_AT_name        : bWantTestMode
  <27f0>     DW_AT_decl_file   : 10
  <27f1>     DW_AT_decl_line   : 879
  <27f3>     DW_AT_type        : <176>
  <27f7>     DW_AT_data_member_location: 2 byte block: 23 2b    (DW_OP_plus_uconst: 43)
 <2><27fa>: Abbrev Number: 20 (DW_TAG_member)
  <27fb>     DW_AT_name        : bTestMode
  <2805>     DW_AT_decl_file   : 10
  <2806>     DW_AT_decl_line   : 880
  <2808>     DW_AT_type        : <176>
  <280c>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><280f>: Abbrev Number: 20 (DW_TAG_member)
  <2810>     DW_AT_name        : bSessionActive
  <281f>     DW_AT_decl_file   : 10
  <2820>     DW_AT_decl_line   : 883
  <2822>     DW_AT_type        : <176>
  <2826>     DW_AT_data_member_location: 2 byte block: 23 2d    (DW_OP_plus_uconst: 45)
 <2><2829>: Abbrev Number: 20 (DW_TAG_member)
  <282a>     DW_AT_name        : bHasBulkSplit
  <2838>     DW_AT_decl_file   : 10
  <2839>     DW_AT_decl_line   : 886
  <283b>     DW_AT_type        : <176>
  <283f>     DW_AT_data_member_location: 2 byte block: 23 2e    (DW_OP_plus_uconst: 46)
 <2><2842>: Abbrev Number: 20 (DW_TAG_member)
  <2843>     DW_AT_name        : bHasBulkCombine
  <2853>     DW_AT_decl_file   : 10
  <2854>     DW_AT_decl_line   : 887
  <2856>     DW_AT_type        : <176>
  <285a>     DW_AT_data_member_location: 2 byte block: 23 2f    (DW_OP_plus_uconst: 47)
 <2><285d>: Abbrev Number: 20 (DW_TAG_member)
  <285e>     DW_AT_name        : bHasHbIsoTx
  <286a>     DW_AT_decl_file   : 10
  <286b>     DW_AT_decl_line   : 888
  <286d>     DW_AT_type        : <176>
  <2871>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><2874>: Abbrev Number: 20 (DW_TAG_member)
  <2875>     DW_AT_name        : bHasHbIsoRx
  <2881>     DW_AT_decl_file   : 10
  <2882>     DW_AT_decl_line   : 889
  <2884>     DW_AT_type        : <176>
  <2888>     DW_AT_data_member_location: 2 byte block: 23 31    (DW_OP_plus_uconst: 49)
 <2><288b>: Abbrev Number: 20 (DW_TAG_member)
  <288c>     DW_AT_name        : bIsMultipoint
  <289a>     DW_AT_decl_file   : 10
  <289b>     DW_AT_decl_line   : 890
  <289d>     DW_AT_type        : <176>
  <28a1>     DW_AT_data_member_location: 2 byte block: 23 32    (DW_OP_plus_uconst: 50)
 <2><28a4>: Abbrev Number: 20 (DW_TAG_member)
  <28a5>     DW_AT_name        : bEndCount
  <28af>     DW_AT_decl_file   : 10
  <28b0>     DW_AT_decl_line   : 891
  <28b2>     DW_AT_type        : <176>
  <28b6>     DW_AT_data_member_location: 2 byte block: 23 33    (DW_OP_plus_uconst: 51)
 <2><28b9>: Abbrev Number: 20 (DW_TAG_member)
  <28ba>     DW_AT_name        : dwFrame
  <28c2>     DW_AT_decl_file   : 10
  <28c3>     DW_AT_decl_line   : 894
  <28c5>     DW_AT_type        : <1a4>
  <28c9>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><28cc>: Abbrev Number: 20 (DW_TAG_member)
  <28cd>     DW_AT_name        : bIsHighSpeed
  <28da>     DW_AT_decl_file   : 10
  <28db>     DW_AT_decl_line   : 895
  <28dd>     DW_AT_type        : <176>
  <28e1>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <2><28e4>: Abbrev Number: 20 (DW_TAG_member)
  <28e5>     DW_AT_name        : bIsLowSpeed
  <28f1>     DW_AT_decl_file   : 10
  <28f2>     DW_AT_decl_line   : 896
  <28f4>     DW_AT_type        : <176>
  <28f8>     DW_AT_data_member_location: 2 byte block: 23 39    (DW_OP_plus_uconst: 57)
 <2><28fb>: Abbrev Number: 20 (DW_TAG_member)
  <28fc>     DW_AT_name        : bIsReset
  <2905>     DW_AT_decl_file   : 10
  <2906>     DW_AT_decl_line   : 897
  <2908>     DW_AT_type        : <176>
  <290c>     DW_AT_data_member_location: 2 byte block: 23 3a    (DW_OP_plus_uconst: 58)
 <2><290f>: Abbrev Number: 20 (DW_TAG_member)
  <2910>     DW_AT_name        : bIsSuspend
  <291b>     DW_AT_decl_file   : 10
  <291c>     DW_AT_decl_line   : 898
  <291e>     DW_AT_type        : <176>
  <2922>     DW_AT_data_member_location: 2 byte block: 23 3b    (DW_OP_plus_uconst: 59)
 <2><2925>: Abbrev Number: 20 (DW_TAG_member)
  <2926>     DW_AT_name        : bIsResume
  <2930>     DW_AT_decl_file   : 10
  <2931>     DW_AT_decl_line   : 899
  <2933>     DW_AT_type        : <176>
  <2937>     DW_AT_data_member_location: 2 byte block: 23 3c    (DW_OP_plus_uconst: 60)
 <2><293a>: Abbrev Number: 20 (DW_TAG_member)
  <293b>     DW_AT_name        : bIsSession
  <2946>     DW_AT_decl_file   : 10
  <2947>     DW_AT_decl_line   : 900
  <2949>     DW_AT_type        : <176>
  <294d>     DW_AT_data_member_location: 2 byte block: 23 3d    (DW_OP_plus_uconst: 61)
 <2><2950>: Abbrev Number: 20 (DW_TAG_member)
  <2951>     DW_AT_name        : bIsHost
  <2959>     DW_AT_decl_file   : 10
  <295a>     DW_AT_decl_line   : 901
  <295c>     DW_AT_type        : <176>
  <2960>     DW_AT_data_member_location: 2 byte block: 23 3e    (DW_OP_plus_uconst: 62)
 <2><2963>: Abbrev Number: 20 (DW_TAG_member)
  <2964>     DW_AT_name        : bConnectorId
  <2971>     DW_AT_decl_file   : 10
  <2972>     DW_AT_decl_line   : 902
  <2974>     DW_AT_type        : <176>
  <2978>     DW_AT_data_member_location: 2 byte block: 23 3f    (DW_OP_plus_uconst: 63)
 <2><297b>: Abbrev Number: 20 (DW_TAG_member)
  <297c>     DW_AT_name        : bBusVoltage
  <2988>     DW_AT_decl_file   : 10
  <2989>     DW_AT_decl_line   : 903
  <298b>     DW_AT_type        : <176>
  <298f>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><2992>: Abbrev Number: 20 (DW_TAG_member)
  <2993>     DW_AT_name        : bEnd0Status
  <299f>     DW_AT_decl_file   : 10
  <29a0>     DW_AT_decl_line   : 906
  <29a2>     DW_AT_type        : <176>
  <29a6>     DW_AT_data_member_location: 2 byte block: 23 41    (DW_OP_plus_uconst: 65)
 <2><29a9>: Abbrev Number: 20 (DW_TAG_member)
  <29aa>     DW_AT_name        : wEndMask
  <29b3>     DW_AT_decl_file   : 10
  <29b4>     DW_AT_decl_line   : 907
  <29b6>     DW_AT_type        : <185>
  <29ba>     DW_AT_data_member_location: 2 byte block: 23 42    (DW_OP_plus_uconst: 66)
 <2><29bd>: Abbrev Number: 20 (DW_TAG_member)
  <29be>     DW_AT_name        : bDmaCompleted
  <29cc>     DW_AT_decl_file   : 10
  <29cd>     DW_AT_decl_line   : 908
  <29cf>     DW_AT_type        : <176>
  <29d3>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><29d6>: Abbrev Number: 20 (DW_TAG_member)
  <29d7>     DW_AT_name        : bVbusErrorRetries
  <29e9>     DW_AT_decl_file   : 10
  <29ea>     DW_AT_decl_line   : 909
  <29ec>     DW_AT_type        : <176>
  <29f0>     DW_AT_data_member_location: 2 byte block: 23 45    (DW_OP_plus_uconst: 69)
 <2><29f3>: Abbrev Number: 20 (DW_TAG_member)
  <29f4>     DW_AT_name        : bHasDisablePing
  <2a04>     DW_AT_decl_file   : 10
  <2a05>     DW_AT_decl_line   : 910
  <2a07>     DW_AT_type        : <176>
  <2a0b>     DW_AT_data_member_location: 2 byte block: 23 46    (DW_OP_plus_uconst: 70)
 <2><2a0e>: Abbrev Number: 20 (DW_TAG_member)
  <2a0f>     DW_AT_name        : bBabbleRetries
  <2a1e>     DW_AT_decl_file   : 10
  <2a1f>     DW_AT_decl_line   : 911
  <2a21>     DW_AT_type        : <176>
  <2a25>     DW_AT_data_member_location: 2 byte block: 23 47    (DW_OP_plus_uconst: 71)
 <2><2a28>: Abbrev Number: 20 (DW_TAG_member)
  <2a29>     DW_AT_name        : bHasRqPktCount
  <2a38>     DW_AT_decl_file   : 10
  <2a39>     DW_AT_decl_line   : 912
  <2a3b>     DW_AT_type        : <176>
  <2a3f>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><2a42>: Abbrev Number: 20 (DW_TAG_member)
  <2a43>     DW_AT_name        : bPad1
  <2a49>     DW_AT_decl_file   : 10
  <2a4a>     DW_AT_decl_line   : 913
  <2a4c>     DW_AT_type        : <176>
  <2a50>     DW_AT_data_member_location: 2 byte block: 23 49    (DW_OP_plus_uconst: 73)
 <2><2a53>: Abbrev Number: 20 (DW_TAG_member)
  <2a54>     DW_AT_name        : bPad2
  <2a5a>     DW_AT_decl_file   : 10
  <2a5b>     DW_AT_decl_line   : 914
  <2a5d>     DW_AT_type        : <176>
  <2a61>     DW_AT_data_member_location: 2 byte block: 23 4a    (DW_OP_plus_uconst: 74)
 <2><2a64>: Abbrev Number: 20 (DW_TAG_member)
  <2a65>     DW_AT_name        : bPad3
  <2a6b>     DW_AT_decl_file   : 10
  <2a6c>     DW_AT_decl_line   : 915
  <2a6e>     DW_AT_type        : <176>
  <2a72>     DW_AT_data_member_location: 2 byte block: 23 4b    (DW_OP_plus_uconst: 75)
 <2><2a75>: Abbrev Number: 20 (DW_TAG_member)
  <2a76>     DW_AT_name        : bOtgState
  <2a80>     DW_AT_decl_file   : 10
  <2a81>     DW_AT_decl_line   : 918
  <2a83>     DW_AT_type        : <176>
  <2a87>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><2a8a>: Abbrev Number: 20 (DW_TAG_member)
  <2a8b>     DW_AT_name        : bRelinquish
  <2a97>     DW_AT_decl_file   : 10
  <2a98>     DW_AT_decl_line   : 919
  <2a9a>     DW_AT_type        : <176>
  <2a9e>     DW_AT_data_member_location: 2 byte block: 23 4d    (DW_OP_plus_uconst: 77)
 <2><2aa1>: Abbrev Number: 20 (DW_TAG_member)
  <2aa2>     DW_AT_name        : bIsHnpSupported
  <2ab2>     DW_AT_decl_file   : 10
  <2ab3>     DW_AT_decl_line   : 920
  <2ab5>     DW_AT_type        : <176>
  <2ab9>     DW_AT_data_member_location: 2 byte block: 23 4e    (DW_OP_plus_uconst: 78)
 <2><2abc>: Abbrev Number: 20 (DW_TAG_member)
  <2abd>     DW_AT_name        : bIsHnpAllowed
  <2acb>     DW_AT_decl_file   : 10
  <2acc>     DW_AT_decl_line   : 921
  <2ace>     DW_AT_type        : <176>
  <2ad2>     DW_AT_data_member_location: 2 byte block: 23 4f    (DW_OP_plus_uconst: 79)
 <2><2ad5>: Abbrev Number: 20 (DW_TAG_member)
  <2ad6>     DW_AT_name        : pOtgClient
  <2ae1>     DW_AT_decl_file   : 10
  <2ae2>     DW_AT_decl_line   : 922
  <2ae4>     DW_AT_type        : <3913>
  <2ae8>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><2aeb>: Abbrev Number: 20 (DW_TAG_member)
  <2aec>     DW_AT_name        : EnumerationData
  <2afc>     DW_AT_decl_file   : 10
  <2afd>     DW_AT_decl_line   : 926
  <2aff>     DW_AT_type        : <38f1>
  <2b03>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <2><2b06>: Abbrev Number: 20 (DW_TAG_member)
  <2b07>     DW_AT_name        : pHostClient
  <2b13>     DW_AT_decl_file   : 10
  <2b14>     DW_AT_decl_line   : 927
  <2b16>     DW_AT_type        : <3919>
  <2b1a>     DW_AT_data_member_location: 3 byte block: 23 ac 3  (DW_OP_plus_uconst: 428)
 <2><2b1e>: Abbrev Number: 20 (DW_TAG_member)
  <2b1f>     DW_AT_name        : pfDriverTimerExpired
  <2b34>     DW_AT_decl_file   : 10
  <2b35>     DW_AT_decl_line   : 928
  <2b37>     DW_AT_type        : <1721>
  <2b3b>     DW_AT_data_member_location: 3 byte block: 23 b0 3  (DW_OP_plus_uconst: 432)
 <2><2b3f>: Abbrev Number: 20 (DW_TAG_member)
  <2b40>     DW_AT_name        : pDriverTimerData
  <2b51>     DW_AT_decl_file   : 10
  <2b52>     DW_AT_decl_line   : 929
  <2b54>     DW_AT_type        : <f1>
  <2b58>     DW_AT_data_member_location: 3 byte block: 23 b4 3  (DW_OP_plus_uconst: 436)
 <2><2b5c>: Abbrev Number: 20 (DW_TAG_member)
  <2b5d>     DW_AT_name        : pRootDevice
  <2b69>     DW_AT_decl_file   : 10
  <2b6a>     DW_AT_decl_line   : 930
  <2b6c>     DW_AT_type        : <16dd>
  <2b70>     DW_AT_data_member_location: 3 byte block: 23 b8 3  (DW_OP_plus_uconst: 440)
 <2><2b74>: Abbrev Number: 20 (DW_TAG_member)
  <2b75>     DW_AT_name        : ConnectedDeviceList
  <2b89>     DW_AT_decl_file   : 10
  <2b8a>     DW_AT_decl_line   : 931
  <2b8c>     DW_AT_type        : <1b27>
  <2b90>     DW_AT_data_member_location: 3 byte block: 23 bc 3  (DW_OP_plus_uconst: 444)
 <2><2b94>: Abbrev Number: 20 (DW_TAG_member)
  <2b95>     DW_AT_name        : Schedule
  <2b9e>     DW_AT_decl_file   : 10
  <2b9f>     DW_AT_decl_line   : 932
  <2ba1>     DW_AT_type        : <3040>
  <2ba5>     DW_AT_data_member_location: 3 byte block: 23 c8 3  (DW_OP_plus_uconst: 456)
 <2><2ba9>: Abbrev Number: 20 (DW_TAG_member)
  <2baa>     DW_AT_name        : wHostMaxPower
  <2bb8>     DW_AT_decl_file   : 10
  <2bb9>     DW_AT_decl_line   : 933
  <2bbb>     DW_AT_type        : <185>
  <2bbf>     DW_AT_data_member_location: 3 byte block: 23 dc 3  (DW_OP_plus_uconst: 476)
 <2><2bc3>: Abbrev Number: 20 (DW_TAG_member)
  <2bc4>     DW_AT_name        : wHostUsedPower
  <2bd3>     DW_AT_decl_file   : 10
  <2bd4>     DW_AT_decl_line   : 934
  <2bd6>     DW_AT_type        : <185>
  <2bda>     DW_AT_data_member_location: 3 byte block: 23 de 3  (DW_OP_plus_uconst: 478)
 <2><2bde>: Abbrev Number: 20 (DW_TAG_member)
  <2bdf>     DW_AT_name        : pFunctionClient
  <2bef>     DW_AT_decl_file   : 10
  <2bf0>     DW_AT_decl_line   : 938
  <2bf2>     DW_AT_type        : <391f>
  <2bf6>     DW_AT_data_member_location: 3 byte block: 23 e0 3  (DW_OP_plus_uconst: 480)
 <2><2bfa>: Abbrev Number: 19 (DW_TAG_member)
  <2bfb>     DW_AT_name        : (indirect string, offset: 0x191): apConfigDescriptors
  <2bff>     DW_AT_decl_file   : 10
  <2c00>     DW_AT_decl_line   : 939
  <2c02>     DW_AT_type        : <3925>
  <2c06>     DW_AT_data_member_location: 3 byte block: 23 e4 3  (DW_OP_plus_uconst: 484)
 <2><2c0a>: Abbrev Number: 20 (DW_TAG_member)
  <2c0b>     DW_AT_name        : pCurrentConfig
  <2c1a>     DW_AT_decl_file   : 10
  <2c1b>     DW_AT_decl_line   : 940
  <2c1d>     DW_AT_type        : <392b>
  <2c21>     DW_AT_data_member_location: 3 byte block: 23 e8 3  (DW_OP_plus_uconst: 488)
 <2><2c25>: Abbrev Number: 20 (DW_TAG_member)
  <2c26>     DW_AT_name        : apPipe
  <2c2d>     DW_AT_decl_file   : 10
  <2c2e>     DW_AT_decl_line   : 941
  <2c30>     DW_AT_type        : <3936>
  <2c34>     DW_AT_data_member_location: 3 byte block: 23 ec 3  (DW_OP_plus_uconst: 492)
 <2><2c38>: Abbrev Number: 20 (DW_TAG_member)
  <2c39>     DW_AT_name        : bUsbState
  <2c43>     DW_AT_decl_file   : 10
  <2c44>     DW_AT_decl_line   : 942
  <2c46>     DW_AT_type        : <176>
  <2c4a>     DW_AT_data_member_location: 3 byte block: 23 f0 3  (DW_OP_plus_uconst: 496)
 <2><2c4e>: Abbrev Number: 20 (DW_TAG_member)
  <2c4f>     DW_AT_name        : bMaxFunctionEnds
  <2c60>     DW_AT_decl_file   : 10
  <2c61>     DW_AT_decl_line   : 943
  <2c63>     DW_AT_type        : <176>
  <2c67>     DW_AT_data_member_location: 3 byte block: 23 f1 3  (DW_OP_plus_uconst: 497)
 <2><2c6b>: Abbrev Number: 20 (DW_TAG_member)
  <2c6c>     DW_AT_name        : bCanWakeup
  <2c77>     DW_AT_decl_file   : 10
  <2c78>     DW_AT_decl_line   : 944
  <2c7a>     DW_AT_type        : <176>
  <2c7e>     DW_AT_data_member_location: 3 byte block: 23 f2 3  (DW_OP_plus_uconst: 498)
 <2><2c82>: Abbrev Number: 20 (DW_TAG_member)
  <2c83>     DW_AT_name        : bSetAddress
  <2c8f>     DW_AT_decl_file   : 10
  <2c90>     DW_AT_decl_line   : 945
  <2c92>     DW_AT_type        : <176>
  <2c96>     DW_AT_data_member_location: 3 byte block: 23 f3 3  (DW_OP_plus_uconst: 499)
 <2><2c9a>: Abbrev Number: 20 (DW_TAG_member)
  <2c9b>     DW_AT_name        : bConfigValue
  <2ca8>     DW_AT_decl_file   : 10
  <2ca9>     DW_AT_decl_line   : 946
  <2cab>     DW_AT_type        : <176>
  <2caf>     DW_AT_data_member_location: 3 byte block: 23 f4 3  (DW_OP_plus_uconst: 500)
 <2><2cb3>: Abbrev Number: 20 (DW_TAG_member)
  <2cb4>     DW_AT_name        : bParse
  <2cbb>     DW_AT_decl_file   : 10
  <2cbc>     DW_AT_decl_line   : 947
  <2cbe>     DW_AT_type        : <176>
  <2cc2>     DW_AT_data_member_location: 3 byte block: 23 f5 3  (DW_OP_plus_uconst: 501)
 <2><2cc6>: Abbrev Number: 20 (DW_TAG_member)
  <2cc7>     DW_AT_name        : wSetupDataIndex
  <2cd7>     DW_AT_decl_file   : 10
  <2cd8>     DW_AT_decl_line   : 948
  <2cda>     DW_AT_type        : <185>
  <2cde>     DW_AT_data_member_location: 3 byte block: 23 f6 3  (DW_OP_plus_uconst: 502)
 <2><2ce2>: Abbrev Number: 20 (DW_TAG_member)
  <2ce3>     DW_AT_name        : wSetupRxIndex
  <2cf1>     DW_AT_decl_file   : 10
  <2cf2>     DW_AT_decl_line   : 949
  <2cf4>     DW_AT_type        : <185>
  <2cf8>     DW_AT_data_member_location: 3 byte block: 23 f8 3  (DW_OP_plus_uconst: 504)
 <2><2cfc>: Abbrev Number: 20 (DW_TAG_member)
  <2cfd>     DW_AT_name        : wSetupDataSize
  <2d0c>     DW_AT_decl_file   : 10
  <2d0d>     DW_AT_decl_line   : 950
  <2d0f>     DW_AT_type        : <185>
  <2d13>     DW_AT_data_member_location: 3 byte block: 23 fa 3  (DW_OP_plus_uconst: 506)
 <2><2d17>: Abbrev Number: 20 (DW_TAG_member)
  <2d18>     DW_AT_name        : pSetupData
  <2d23>     DW_AT_decl_file   : 10
  <2d24>     DW_AT_decl_line   : 951
  <2d26>     DW_AT_type        : <f28>
  <2d2a>     DW_AT_data_member_location: 3 byte block: 23 fc 3  (DW_OP_plus_uconst: 508)
 <2><2d2e>: Abbrev Number: 20 (DW_TAG_member)
  <2d2f>     DW_AT_name        : dwSequenceNumber
  <2d40>     DW_AT_decl_file   : 10
  <2d41>     DW_AT_decl_line   : 952
  <2d43>     DW_AT_type        : <1a4>
  <2d47>     DW_AT_data_member_location: 3 byte block: 23 80 4  (DW_OP_plus_uconst: 512)
 <2><2d4b>: Abbrev Number: 20 (DW_TAG_member)
  <2d4c>     DW_AT_name        : abIfaceOffsets
  <2d5b>     DW_AT_decl_file   : 10
  <2d5c>     DW_AT_decl_line   : 953
  <2d5e>     DW_AT_type        : <38e1>
  <2d62>     DW_AT_data_member_location: 3 byte block: 23 84 4  (DW_OP_plus_uconst: 516)
 <2><2d66>: Abbrev Number: 19 (DW_TAG_member)
  <2d67>     DW_AT_name        : (indirect string, offset: 0xdf): pDmaController
  <2d6b>     DW_AT_decl_file   : 10
  <2d6c>     DW_AT_decl_line   : 969
  <2d6e>     DW_AT_type        : <217e>
  <2d72>     DW_AT_data_member_location: 3 byte block: 23 94 4  (DW_OP_plus_uconst: 532)
 <2><2d76>: Abbrev Number: 20 (DW_TAG_member)
  <2d77>     DW_AT_name        : pfReadBusState
  <2d86>     DW_AT_decl_file   : 10
  <2d87>     DW_AT_decl_line   : 972
  <2d89>     DW_AT_type        : <33c0>
  <2d8d>     DW_AT_data_member_location: 3 byte block: 23 98 4  (DW_OP_plus_uconst: 536)
 <2><2d91>: Abbrev Number: 20 (DW_TAG_member)
  <2d92>     DW_AT_name        : pfProgramBusState
  <2da4>     DW_AT_decl_file   : 10
  <2da5>     DW_AT_decl_line   : 973
  <2da7>     DW_AT_type        : <33f1>
  <2dab>     DW_AT_data_member_location: 3 byte block: 23 9c 4  (DW_OP_plus_uconst: 540)
 <2><2daf>: Abbrev Number: 20 (DW_TAG_member)
  <2db0>     DW_AT_name        : pfResetPort
  <2dbc>     DW_AT_decl_file   : 10
  <2dbd>     DW_AT_decl_line   : 974
  <2dbf>     DW_AT_type        : <340f>
  <2dc3>     DW_AT_data_member_location: 3 byte block: 23 a0 4  (DW_OP_plus_uconst: 544)
 <2><2dc7>: Abbrev Number: 20 (DW_TAG_member)
  <2dc8>     DW_AT_name        : pfBindEndpoint
  <2dd7>     DW_AT_decl_file   : 10
  <2dd8>     DW_AT_decl_line   : 975
  <2dda>     DW_AT_type        : <3427>
  <2dde>     DW_AT_data_member_location: 3 byte block: 23 a4 4  (DW_OP_plus_uconst: 548)
 <2><2de2>: Abbrev Number: 20 (DW_TAG_member)
  <2de3>     DW_AT_name        : pfProgramStartReceive
  <2df9>     DW_AT_decl_file   : 10
  <2dfa>     DW_AT_decl_line   : 976
  <2dfc>     DW_AT_type        : <3472>
  <2e00>     DW_AT_data_member_location: 3 byte block: 23 a8 4  (DW_OP_plus_uconst: 552)
 <2><2e04>: Abbrev Number: 20 (DW_TAG_member)
  <2e05>     DW_AT_name        : pfProgramStartTransmit
  <2e1c>     DW_AT_decl_file   : 10
  <2e1d>     DW_AT_decl_line   : 977
  <2e1f>     DW_AT_type        : <34c3>
  <2e23>     DW_AT_data_member_location: 3 byte block: 23 ac 4  (DW_OP_plus_uconst: 556)
 <2><2e27>: Abbrev Number: 20 (DW_TAG_member)
  <2e28>     DW_AT_name        : pfProgramFlushEndpoint
  <2e3f>     DW_AT_decl_file   : 10
  <2e40>     DW_AT_decl_line   : 978
  <2e42>     DW_AT_type        : <3510>
  <2e46>     DW_AT_data_member_location: 3 byte block: 23 b0 4  (DW_OP_plus_uconst: 560)
 <2><2e4a>: Abbrev Number: 20 (DW_TAG_member)
  <2e4b>     DW_AT_name        : pfProgramHaltEndpoint
  <2e61>     DW_AT_decl_file   : 10
  <2e62>     DW_AT_decl_line   : 979
  <2e64>     DW_AT_type        : <3558>
  <2e68>     DW_AT_data_member_location: 3 byte block: 23 b4 4  (DW_OP_plus_uconst: 564)
 <2><2e6c>: Abbrev Number: 20 (DW_TAG_member)
  <2e6d>     DW_AT_name        : pfDefaultEndResponse
  <2e82>     DW_AT_decl_file   : 10
  <2e83>     DW_AT_decl_line   : 980
  <2e85>     DW_AT_type        : <357a>
  <2e89>     DW_AT_data_member_location: 3 byte block: 23 b8 4  (DW_OP_plus_uconst: 568)
 <2><2e8d>: Abbrev Number: 20 (DW_TAG_member)
  <2e8e>     DW_AT_name        : pfServiceDefaultEnd
  <2ea2>     DW_AT_decl_file   : 10
  <2ea3>     DW_AT_decl_line   : 981
  <2ea5>     DW_AT_type        : <35b6>
  <2ea9>     DW_AT_data_member_location: 3 byte block: 23 bc 4  (DW_OP_plus_uconst: 572)
 <2><2ead>: Abbrev Number: 20 (DW_TAG_member)
  <2eae>     DW_AT_name        : pfServiceTransmitAvail
  <2ec5>     DW_AT_decl_file   : 10
  <2ec6>     DW_AT_decl_line   : 982
  <2ec8>     DW_AT_type        : <35f7>
  <2ecc>     DW_AT_data_member_location: 3 byte block: 23 c0 4  (DW_OP_plus_uconst: 576)
 <2><2ed0>: Abbrev Number: 20 (DW_TAG_member)
  <2ed1>     DW_AT_name        : pfServiceReceiveReady
  <2ee7>     DW_AT_decl_file   : 10
  <2ee8>     DW_AT_decl_line   : 983
  <2eea>     DW_AT_type        : <363a>
  <2eee>     DW_AT_data_member_location: 3 byte block: 23 c4 4  (DW_OP_plus_uconst: 580)
 <2><2ef2>: Abbrev Number: 20 (DW_TAG_member)
  <2ef3>     DW_AT_name        : pfLoadFifo
  <2efe>     DW_AT_decl_file   : 10
  <2eff>     DW_AT_decl_line   : 984
  <2f01>     DW_AT_type        : <365c>
  <2f05>     DW_AT_data_member_location: 3 byte block: 23 c8 4  (DW_OP_plus_uconst: 584)
 <2><2f09>: Abbrev Number: 20 (DW_TAG_member)
  <2f0a>     DW_AT_name        : pfUnloadFifo
  <2f17>     DW_AT_decl_file   : 10
  <2f18>     DW_AT_decl_line   : 985
  <2f1a>     DW_AT_type        : <3698>
  <2f1e>     DW_AT_data_member_location: 3 byte block: 23 cc 4  (DW_OP_plus_uconst: 588)
 <2><2f22>: Abbrev Number: 20 (DW_TAG_member)
  <2f23>     DW_AT_name        : pfAcceptDevice
  <2f32>     DW_AT_decl_file   : 10
  <2f33>     DW_AT_decl_line   : 986
  <2f35>     DW_AT_type        : <36d6>
  <2f39>     DW_AT_data_member_location: 3 byte block: 23 d0 4  (DW_OP_plus_uconst: 592)
 <2><2f3d>: Abbrev Number: 20 (DW_TAG_member)
  <2f3e>     DW_AT_name        : pfDmaChannelStatusChanged
  <2f58>     DW_AT_decl_file   : 10
  <2f59>     DW_AT_decl_line   : 987
  <2f5b>     DW_AT_type        : <20e8>
  <2f5f>     DW_AT_data_member_location: 3 byte block: 23 d4 4  (DW_OP_plus_uconst: 596)
 <2><2f63>: Abbrev Number: 20 (DW_TAG_member)
  <2f64>     DW_AT_name        : pfSetPortTestMode
  <2f76>     DW_AT_decl_file   : 10
  <2f77>     DW_AT_decl_line   : 988
  <2f79>     DW_AT_type        : <3711>
  <2f7d>     DW_AT_data_member_location: 3 byte block: 23 d8 4  (DW_OP_plus_uconst: 600)
 <2><2f81>: Abbrev Number: 20 (DW_TAG_member)
  <2f82>     DW_AT_name        : pfDynamicFifoLocation
  <2f98>     DW_AT_decl_file   : 10
  <2f99>     DW_AT_decl_line   : 989
  <2f9b>     DW_AT_type        : <3746>
  <2f9f>     DW_AT_data_member_location: 3 byte block: 23 dc 4  (DW_OP_plus_uconst: 604)
 <1><2fa4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2fa5>     DW_AT_byte_size   : 4
  <2fa6>     DW_AT_type        : <26e6>
 <1><2faa>: Abbrev Number: 23 (DW_TAG_structure_type)
  <2fab>     DW_AT_name        : _MGC_ScheduleSlot
  <2fbd>     DW_AT_byte_size   : 12
  <2fbe>     DW_AT_decl_file   : 10
  <2fbf>     DW_AT_decl_line   : 242
  <2fc0>     DW_AT_sibling     : <2fd9>
 <2><2fc4>: Abbrev Number: 20 (DW_TAG_member)
  <2fc5>     DW_AT_name        : PipeList
  <2fce>     DW_AT_decl_file   : 10
  <2fcf>     DW_AT_decl_line   : 378
  <2fd1>     DW_AT_type        : <1b27>
  <2fd5>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <1><2fd9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <2fda>     DW_AT_byte_size   : 4
  <2fdb>     DW_AT_type        : <2faa>
 <1><2fdf>: Abbrev Number: 17 (DW_TAG_typedef)
  <2fe0>     DW_AT_name        : MGC_Pipe
  <2fe9>     DW_AT_decl_file   : 10
  <2fea>     DW_AT_decl_line   : 355
  <2fec>     DW_AT_type        : <2616>
 <1><2ff0>: Abbrev Number: 18 (DW_TAG_structure_type)
  <2ff1>     DW_AT_byte_size   : 20
  <2ff2>     DW_AT_decl_file   : 10
  <2ff3>     DW_AT_decl_line   : 395
  <2ff5>     DW_AT_sibling     : <3040>
 <2><2ff9>: Abbrev Number: 20 (DW_TAG_member)
  <2ffa>     DW_AT_name        : dwTotalTime
  <3006>     DW_AT_decl_file   : 10
  <3007>     DW_AT_decl_line   : 396
  <3009>     DW_AT_type        : <1a4>
  <300d>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><3010>: Abbrev Number: 20 (DW_TAG_member)
  <3011>     DW_AT_name        : ScheduleSlots
  <301f>     DW_AT_decl_file   : 10
  <3020>     DW_AT_decl_line   : 397
  <3022>     DW_AT_type        : <1b27>
  <3026>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><3029>: Abbrev Number: 20 (DW_TAG_member)
  <302a>     DW_AT_name        : wSlotCount
  <3035>     DW_AT_decl_file   : 10
  <3036>     DW_AT_decl_line   : 398
  <3038>     DW_AT_type        : <185>
  <303c>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <1><3040>: Abbrev Number: 17 (DW_TAG_typedef)
  <3041>     DW_AT_name        : MGC_Schedule
  <304e>     DW_AT_decl_file   : 10
  <304f>     DW_AT_decl_line   : 399
  <3051>     DW_AT_type        : <2ff0>
 <1><3055>: Abbrev Number: 17 (DW_TAG_typedef)
  <3056>     DW_AT_name        : MGC_pfProgramStartController
  <3073>     DW_AT_decl_file   : 10
  <3074>     DW_AT_decl_line   : 435
  <3076>     DW_AT_type        : <307a>
 <1><307a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <307b>     DW_AT_byte_size   : 4
  <307c>     DW_AT_type        : <3080>
 <1><3080>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3081>     DW_AT_prototyped  : 1
  <3082>     DW_AT_type        : <1a4>
  <3086>     DW_AT_sibling     : <3090>
 <2><308a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <308b>     DW_AT_type        : <3090>
 <1><3090>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3091>     DW_AT_byte_size   : 4
  <3092>     DW_AT_type        : <3096>
 <1><3096>: Abbrev Number: 23 (DW_TAG_structure_type)
  <3097>     DW_AT_name        : _MGC_Controller
  <30a7>     DW_AT_byte_size   : 60
  <30a8>     DW_AT_decl_file   : 10
  <30a9>     DW_AT_decl_line   : 240
  <30aa>     DW_AT_sibling     : <325d>
 <2><30ae>: Abbrev Number: 20 (DW_TAG_member)
  <30af>     DW_AT_name        : pControllerAddressIsr
  <30c5>     DW_AT_decl_file   : 10
  <30c6>     DW_AT_decl_line   : 512
  <30c8>     DW_AT_type        : <f1>
  <30cc>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><30cf>: Abbrev Number: 20 (DW_TAG_member)
  <30d0>     DW_AT_name        : pControllerAddressIst
  <30e6>     DW_AT_decl_file   : 10
  <30e7>     DW_AT_decl_line   : 513
  <30e9>     DW_AT_type        : <f1>
  <30ed>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><30f0>: Abbrev Number: 20 (DW_TAG_member)
  <30f1>     DW_AT_name        : pUtils
  <30f8>     DW_AT_decl_file   : 10
  <30f9>     DW_AT_decl_line   : 514
  <30fb>     DW_AT_type        : <3397>
  <30ff>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><3102>: Abbrev Number: 20 (DW_TAG_member)
  <3103>     DW_AT_name        : pSystemServices
  <3113>     DW_AT_decl_file   : 10
  <3114>     DW_AT_decl_line   : 515
  <3116>     DW_AT_type        : <339d>
  <311a>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><311d>: Abbrev Number: 19 (DW_TAG_member)
  <311e>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <3122>     DW_AT_decl_file   : 10
  <3123>     DW_AT_decl_line   : 516
  <3125>     DW_AT_type        : <f1>
  <3129>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><312c>: Abbrev Number: 19 (DW_TAG_member)
  <312d>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <3131>     DW_AT_decl_file   : 10
  <3132>     DW_AT_decl_line   : 517
  <3134>     DW_AT_type        : <2fa4>
  <3138>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><313b>: Abbrev Number: 20 (DW_TAG_member)
  <313c>     DW_AT_name        : pDmaControllerFactory
  <3152>     DW_AT_decl_file   : 10
  <3153>     DW_AT_decl_line   : 519
  <3155>     DW_AT_type        : <33a3>
  <3159>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><315c>: Abbrev Number: 20 (DW_TAG_member)
  <315d>     DW_AT_name        : pfProgramStartController
  <3176>     DW_AT_decl_file   : 10
  <3177>     DW_AT_decl_line   : 522
  <3179>     DW_AT_type        : <3055>
  <317d>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><3180>: Abbrev Number: 20 (DW_TAG_member)
  <3181>     DW_AT_name        : pfProgramStopController
  <3199>     DW_AT_decl_file   : 10
  <319a>     DW_AT_decl_line   : 523
  <319c>     DW_AT_type        : <325d>
  <31a0>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><31a3>: Abbrev Number: 20 (DW_TAG_member)
  <31a4>     DW_AT_name        : pfDestroyController
  <31b8>     DW_AT_decl_file   : 10
  <31b9>     DW_AT_decl_line   : 524
  <31bb>     DW_AT_type        : <3281>
  <31bf>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><31c2>: Abbrev Number: 20 (DW_TAG_member)
  <31c3>     DW_AT_name        : pfSetControllerHostPower
  <31dc>     DW_AT_decl_file   : 10
  <31dd>     DW_AT_decl_line   : 525
  <31df>     DW_AT_type        : <32a1>
  <31e3>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><31e6>: Abbrev Number: 20 (DW_TAG_member)
  <31e7>     DW_AT_name        : pfDumpControllerState
  <31fd>     DW_AT_decl_file   : 10
  <31fe>     DW_AT_decl_line   : 528
  <3200>     DW_AT_type        : <32e1>
  <3204>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><3207>: Abbrev Number: 20 (DW_TAG_member)
  <3208>     DW_AT_name        : pfDumpPipeState
  <3218>     DW_AT_decl_file   : 10
  <3219>     DW_AT_decl_line   : 529
  <321b>     DW_AT_type        : <3319>
  <321f>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><3222>: Abbrev Number: 20 (DW_TAG_member)
  <3223>     DW_AT_name        : pfDumpLocalEndState
  <3237>     DW_AT_decl_file   : 10
  <3238>     DW_AT_decl_line   : 530
  <323a>     DW_AT_type        : <3356>
  <323e>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><3241>: Abbrev Number: 20 (DW_TAG_member)
  <3242>     DW_AT_name        : wControllerType
  <3252>     DW_AT_decl_file   : 10
  <3253>     DW_AT_decl_line   : 532
  <3255>     DW_AT_type        : <185>
  <3259>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <1><325d>: Abbrev Number: 17 (DW_TAG_typedef)
  <325e>     DW_AT_name        : MGC_pfProgramStopController
  <327a>     DW_AT_decl_file   : 10
  <327b>     DW_AT_decl_line   : 444
  <327d>     DW_AT_type        : <307a>
 <1><3281>: Abbrev Number: 17 (DW_TAG_typedef)
  <3282>     DW_AT_name        : MGC_pfDestroyController
  <329a>     DW_AT_decl_file   : 10
  <329b>     DW_AT_decl_line   : 452
  <329d>     DW_AT_type        : <307a>
 <1><32a1>: Abbrev Number: 17 (DW_TAG_typedef)
  <32a2>     DW_AT_name        : MGC_pfSetControllerHostPower
  <32bf>     DW_AT_decl_file   : 10
  <32c0>     DW_AT_decl_line   : 460
  <32c2>     DW_AT_type        : <32c6>
 <1><32c6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <32c7>     DW_AT_byte_size   : 4
  <32c8>     DW_AT_type        : <32cc>
 <1><32cc>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <32cd>     DW_AT_prototyped  : 1
  <32ce>     DW_AT_type        : <1a4>
  <32d2>     DW_AT_sibling     : <32e1>
 <2><32d6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <32d7>     DW_AT_type        : <3090>
 <2><32db>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <32dc>     DW_AT_type        : <185>
 <1><32e1>: Abbrev Number: 17 (DW_TAG_typedef)
  <32e2>     DW_AT_name        : MGC_pfDumpControllerState
  <32fc>     DW_AT_decl_file   : 10
  <32fd>     DW_AT_decl_line   : 468
  <32ff>     DW_AT_type        : <3303>
 <1><3303>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3304>     DW_AT_byte_size   : 4
  <3305>     DW_AT_type        : <3309>
 <1><3309>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <330a>     DW_AT_prototyped  : 1
  <330b>     DW_AT_type        : <a9>
  <330f>     DW_AT_sibling     : <3319>
 <2><3313>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3314>     DW_AT_type        : <3090>
 <1><3319>: Abbrev Number: 17 (DW_TAG_typedef)
  <331a>     DW_AT_name        : MGC_pfDumpPipeState
  <332e>     DW_AT_decl_file   : 10
  <332f>     DW_AT_decl_line   : 477
  <3331>     DW_AT_type        : <3335>
 <1><3335>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3336>     DW_AT_byte_size   : 4
  <3337>     DW_AT_type        : <333b>
 <1><333b>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <333c>     DW_AT_prototyped  : 1
  <333d>     DW_AT_type        : <a9>
  <3341>     DW_AT_sibling     : <3350>
 <2><3345>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3346>     DW_AT_type        : <3090>
 <2><334a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <334b>     DW_AT_type        : <3350>
 <1><3350>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3351>     DW_AT_byte_size   : 4
  <3352>     DW_AT_type        : <2fdf>
 <1><3356>: Abbrev Number: 17 (DW_TAG_typedef)
  <3357>     DW_AT_name        : MGC_pfDumpLocalEndState
  <336f>     DW_AT_decl_file   : 10
  <3370>     DW_AT_decl_line   : 487
  <3372>     DW_AT_type        : <3376>
 <1><3376>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3377>     DW_AT_byte_size   : 4
  <3378>     DW_AT_type        : <337c>
 <1><337c>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <337d>     DW_AT_prototyped  : 1
  <337e>     DW_AT_type        : <a9>
  <3382>     DW_AT_sibling     : <3391>
 <2><3386>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3387>     DW_AT_type        : <3090>
 <2><338b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <338c>     DW_AT_type        : <3391>
 <1><3391>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3392>     DW_AT_byte_size   : 4
  <3393>     DW_AT_type        : <1011>
 <1><3397>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3398>     DW_AT_byte_size   : 4
  <3399>     DW_AT_type        : <450>
 <1><339d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <339e>     DW_AT_byte_size   : 4
  <339f>     DW_AT_type        : <7e2>
 <1><33a3>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <33a4>     DW_AT_byte_size   : 4
  <33a5>     DW_AT_type        : <2224>
 <1><33a9>: Abbrev Number: 17 (DW_TAG_typedef)
  <33aa>     DW_AT_name        : MGC_Controller
  <33b9>     DW_AT_decl_file   : 10
  <33ba>     DW_AT_decl_line   : 533
  <33bc>     DW_AT_type        : <3096>
 <1><33c0>: Abbrev Number: 17 (DW_TAG_typedef)
  <33c1>     DW_AT_name        : MGC_pfReadBusState
  <33d4>     DW_AT_decl_file   : 10
  <33d5>     DW_AT_decl_line   : 547
  <33d7>     DW_AT_type        : <33db>
 <1><33db>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <33dc>     DW_AT_byte_size   : 4
  <33dd>     DW_AT_type        : <33e1>
 <1><33e1>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <33e2>     DW_AT_prototyped  : 1
  <33e3>     DW_AT_type        : <1a4>
  <33e7>     DW_AT_sibling     : <33f1>
 <2><33eb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <33ec>     DW_AT_type        : <2fa4>
 <1><33f1>: Abbrev Number: 17 (DW_TAG_typedef)
  <33f2>     DW_AT_name        : MGC_pfProgramBusState
  <3408>     DW_AT_decl_file   : 10
  <3409>     DW_AT_decl_line   : 556
  <340b>     DW_AT_type        : <33db>
 <1><340f>: Abbrev Number: 17 (DW_TAG_typedef)
  <3410>     DW_AT_name        : MGC_pfResetPort
  <3420>     DW_AT_decl_file   : 10
  <3421>     DW_AT_decl_line   : 565
  <3423>     DW_AT_type        : <33db>
 <1><3427>: Abbrev Number: 17 (DW_TAG_typedef)
  <3428>     DW_AT_name        : MGC_pfBindEndpoint
  <343b>     DW_AT_decl_file   : 10
  <343c>     DW_AT_decl_line   : 581
  <343e>     DW_AT_type        : <3442>
 <1><3442>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3443>     DW_AT_byte_size   : 4
  <3444>     DW_AT_type        : <3448>
 <1><3448>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3449>     DW_AT_prototyped  : 1
  <344a>     DW_AT_type        : <26e0>
  <344e>     DW_AT_sibling     : <3467>
 <2><3452>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3453>     DW_AT_type        : <2fa4>
 <2><3457>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3458>     DW_AT_type        : <3467>
 <2><345c>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <345d>     DW_AT_type        : <3391>
 <2><3461>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3462>     DW_AT_type        : <176>
 <1><3467>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3468>     DW_AT_byte_size   : 4
  <3469>     DW_AT_type        : <346d>
 <1><346d>: Abbrev Number: 8 (DW_TAG_const_type)
  <346e>     DW_AT_type        : <fb1>
 <1><3472>: Abbrev Number: 17 (DW_TAG_typedef)
  <3473>     DW_AT_name        : MGC_pfProgramStartReceive
  <348d>     DW_AT_decl_file   : 10
  <348e>     DW_AT_decl_line   : 600
  <3490>     DW_AT_type        : <3494>
 <1><3494>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3495>     DW_AT_byte_size   : 4
  <3496>     DW_AT_type        : <349a>
 <1><349a>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <349b>     DW_AT_prototyped  : 1
  <349c>     DW_AT_type        : <1a4>
  <34a0>     DW_AT_sibling     : <34c3>
 <2><34a4>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34a5>     DW_AT_type        : <2fa4>
 <2><34a9>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34aa>     DW_AT_type        : <26e0>
 <2><34ae>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34af>     DW_AT_type        : <f28>
 <2><34b3>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34b4>     DW_AT_type        : <1a4>
 <2><34b8>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34b9>     DW_AT_type        : <f1>
 <2><34bd>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34be>     DW_AT_type        : <176>
 <1><34c3>: Abbrev Number: 17 (DW_TAG_typedef)
  <34c4>     DW_AT_name        : MGC_pfProgramStartTransmit
  <34df>     DW_AT_decl_file   : 10
  <34e0>     DW_AT_decl_line   : 614
  <34e2>     DW_AT_type        : <34e6>
 <1><34e6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <34e7>     DW_AT_byte_size   : 4
  <34e8>     DW_AT_type        : <34ec>
 <1><34ec>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <34ed>     DW_AT_prototyped  : 1
  <34ee>     DW_AT_type        : <1a4>
  <34f2>     DW_AT_sibling     : <3510>
 <2><34f6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34f7>     DW_AT_type        : <2fa4>
 <2><34fb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <34fc>     DW_AT_type        : <26e0>
 <2><3500>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3501>     DW_AT_type        : <667>
 <2><3505>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3506>     DW_AT_type        : <1a4>
 <2><350a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <350b>     DW_AT_type        : <f1>
 <1><3510>: Abbrev Number: 17 (DW_TAG_typedef)
  <3511>     DW_AT_name        : MGC_pfProgramFlushEndpoint
  <352c>     DW_AT_decl_file   : 10
  <352d>     DW_AT_decl_line   : 629
  <352f>     DW_AT_type        : <3533>
 <1><3533>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3534>     DW_AT_byte_size   : 4
  <3535>     DW_AT_type        : <3539>
 <1><3539>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <353a>     DW_AT_prototyped  : 1
  <353b>     DW_AT_type        : <1a4>
  <353f>     DW_AT_sibling     : <3558>
 <2><3543>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3544>     DW_AT_type        : <2fa4>
 <2><3548>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3549>     DW_AT_type        : <26e0>
 <2><354d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <354e>     DW_AT_type        : <176>
 <2><3552>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3553>     DW_AT_type        : <176>
 <1><3558>: Abbrev Number: 17 (DW_TAG_typedef)
  <3559>     DW_AT_name        : MGC_pfProgramHaltEndpoint
  <3573>     DW_AT_decl_file   : 10
  <3574>     DW_AT_decl_line   : 646
  <3576>     DW_AT_type        : <3533>
 <1><357a>: Abbrev Number: 17 (DW_TAG_typedef)
  <357b>     DW_AT_name        : MGC_pfDefaultEndResponse
  <3594>     DW_AT_decl_file   : 10
  <3595>     DW_AT_decl_line   : 656
  <3597>     DW_AT_type        : <359b>
 <1><359b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <359c>     DW_AT_byte_size   : 4
  <359d>     DW_AT_type        : <35a1>
 <1><35a1>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <35a2>     DW_AT_prototyped  : 1
  <35a3>     DW_AT_type        : <1a4>
  <35a7>     DW_AT_sibling     : <35b6>
 <2><35ab>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <35ac>     DW_AT_type        : <2fa4>
 <2><35b0>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <35b1>     DW_AT_type        : <176>
 <1><35b6>: Abbrev Number: 17 (DW_TAG_typedef)
  <35b7>     DW_AT_name        : MGC_pfServiceDefaultEnd
  <35cf>     DW_AT_decl_file   : 10
  <35d0>     DW_AT_decl_line   : 665
  <35d2>     DW_AT_type        : <35d6>
 <1><35d6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <35d7>     DW_AT_byte_size   : 4
  <35d8>     DW_AT_type        : <35dc>
 <1><35dc>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <35dd>     DW_AT_prototyped  : 1
  <35de>     DW_AT_type        : <176>
  <35e2>     DW_AT_sibling     : <35f1>
 <2><35e6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <35e7>     DW_AT_type        : <2fa4>
 <2><35eb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <35ec>     DW_AT_type        : <35f1>
 <1><35f1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <35f2>     DW_AT_byte_size   : 4
  <35f3>     DW_AT_type        : <2290>
 <1><35f7>: Abbrev Number: 17 (DW_TAG_typedef)
  <35f8>     DW_AT_name        : MGC_pfServiceTransmitAvail
  <3613>     DW_AT_decl_file   : 10
  <3614>     DW_AT_decl_line   : 675
  <3616>     DW_AT_type        : <361a>
 <1><361a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <361b>     DW_AT_byte_size   : 4
  <361c>     DW_AT_type        : <3620>
 <1><3620>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3621>     DW_AT_prototyped  : 1
  <3622>     DW_AT_type        : <176>
  <3626>     DW_AT_sibling     : <363a>
 <2><362a>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <362b>     DW_AT_type        : <2fa4>
 <2><362f>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3630>     DW_AT_type        : <185>
 <2><3634>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3635>     DW_AT_type        : <35f1>
 <1><363a>: Abbrev Number: 17 (DW_TAG_typedef)
  <363b>     DW_AT_name        : MGC_pfServiceReceiveReady
  <3655>     DW_AT_decl_file   : 10
  <3656>     DW_AT_decl_line   : 685
  <3658>     DW_AT_type        : <361a>
 <1><365c>: Abbrev Number: 17 (DW_TAG_typedef)
  <365d>     DW_AT_name        : MGC_pfLoadFifo
  <366c>     DW_AT_decl_file   : 10
  <366d>     DW_AT_decl_line   : 695
  <366f>     DW_AT_type        : <3673>
 <1><3673>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3674>     DW_AT_byte_size   : 4
  <3675>     DW_AT_type        : <3679>
 <1><3679>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <367a>     DW_AT_prototyped  : 1
  <367b>     DW_AT_type        : <176>
  <367f>     DW_AT_sibling     : <3698>
 <2><3683>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3684>     DW_AT_type        : <2fa4>
 <2><3688>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3689>     DW_AT_type        : <176>
 <2><368d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <368e>     DW_AT_type        : <1a4>
 <2><3692>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3693>     DW_AT_type        : <667>
 <1><3698>: Abbrev Number: 17 (DW_TAG_typedef)
  <3699>     DW_AT_name        : MGC_pfUnloadFifo
  <36aa>     DW_AT_decl_file   : 10
  <36ab>     DW_AT_decl_line   : 705
  <36ad>     DW_AT_type        : <36b1>
 <1><36b1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <36b2>     DW_AT_byte_size   : 4
  <36b3>     DW_AT_type        : <36b7>
 <1><36b7>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <36b8>     DW_AT_prototyped  : 1
  <36b9>     DW_AT_type        : <176>
  <36bd>     DW_AT_sibling     : <36d6>
 <2><36c1>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <36c2>     DW_AT_type        : <2fa4>
 <2><36c6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <36c7>     DW_AT_type        : <176>
 <2><36cb>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <36cc>     DW_AT_type        : <1a4>
 <2><36d0>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <36d1>     DW_AT_type        : <f28>
 <1><36d6>: Abbrev Number: 17 (DW_TAG_typedef)
  <36d7>     DW_AT_name        : MGC_pfAcceptDevice
  <36ea>     DW_AT_decl_file   : 10
  <36eb>     DW_AT_decl_line   : 718
  <36ed>     DW_AT_type        : <36f1>
 <1><36f1>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <36f2>     DW_AT_byte_size   : 4
  <36f3>     DW_AT_type        : <36f7>
 <1><36f7>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <36f8>     DW_AT_prototyped  : 1
  <36f9>     DW_AT_type        : <176>
  <36fd>     DW_AT_sibling     : <3711>
 <2><3701>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3702>     DW_AT_type        : <2fa4>
 <2><3706>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3707>     DW_AT_type        : <16dd>
 <2><370b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <370c>     DW_AT_type        : <18f1>
 <1><3711>: Abbrev Number: 17 (DW_TAG_typedef)
  <3712>     DW_AT_name        : MGC_pfSetPortTestMode
  <3728>     DW_AT_decl_file   : 10
  <3729>     DW_AT_decl_line   : 725
  <372b>     DW_AT_type        : <372f>
 <1><372f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3730>     DW_AT_byte_size   : 4
  <3731>     DW_AT_type        : <3735>
 <1><3735>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <3736>     DW_AT_prototyped  : 1
  <3737>     DW_AT_sibling     : <3746>
 <2><373b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <373c>     DW_AT_type        : <2fa4>
 <2><3740>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3741>     DW_AT_type        : <1c65>
 <1><3746>: Abbrev Number: 17 (DW_TAG_typedef)
  <3747>     DW_AT_name        : MGC_pfDynamicFifoLocation
  <3761>     DW_AT_decl_file   : 10
  <3762>     DW_AT_decl_line   : 737
  <3764>     DW_AT_type        : <3768>
 <1><3768>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3769>     DW_AT_byte_size   : 4
  <376a>     DW_AT_type        : <376e>
 <1><376e>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <376f>     DW_AT_prototyped  : 1
  <3770>     DW_AT_sibling     : <3798>
 <2><3774>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3775>     DW_AT_type        : <2fa4>
 <2><3779>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <377a>     DW_AT_type        : <176>
 <2><377e>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <377f>     DW_AT_type        : <176>
 <2><3783>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3784>     DW_AT_type        : <176>
 <2><3788>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3789>     DW_AT_type        : <13dd>
 <2><378d>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <378e>     DW_AT_type        : <f28>
 <2><3792>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3793>     DW_AT_type        : <f28>
 <1><3798>: Abbrev Number: 27 (DW_TAG_structure_type)
  <3799>     DW_AT_byte_size   : 344
  <379b>     DW_AT_decl_file   : 10
  <379c>     DW_AT_decl_line   : 784
  <379e>     DW_AT_sibling     : <38c1>
 <2><37a2>: Abbrev Number: 20 (DW_TAG_member)
  <37a3>     DW_AT_name        : pfEnumerationComplete
  <37b9>     DW_AT_decl_file   : 10
  <37ba>     DW_AT_decl_line   : 785
  <37bc>     DW_AT_type        : <19f2>
  <37c0>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><37c3>: Abbrev Number: 19 (DW_TAG_member)
  <37c4>     DW_AT_name        : (indirect string, offset: 0x1db): pDevice
  <37c8>     DW_AT_decl_file   : 10
  <37c9>     DW_AT_decl_line   : 786
  <37cb>     DW_AT_type        : <16dd>
  <37cf>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><37d2>: Abbrev Number: 20 (DW_TAG_member)
  <37d3>     DW_AT_name        : aSetupTx
  <37dc>     DW_AT_decl_file   : 10
  <37dd>     DW_AT_decl_line   : 787
  <37df>     DW_AT_type        : <38c1>
  <37e3>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><37e6>: Abbrev Number: 20 (DW_TAG_member)
  <37e7>     DW_AT_name        : aSetupRx
  <37f0>     DW_AT_decl_file   : 10
  <37f1>     DW_AT_decl_line   : 788
  <37f3>     DW_AT_type        : <38d1>
  <37f7>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><37fa>: Abbrev Number: 20 (DW_TAG_member)
  <37fb>     DW_AT_name        : abmAddress
  <3806>     DW_AT_decl_file   : 10
  <3807>     DW_AT_decl_line   : 789
  <3809>     DW_AT_type        : <38e1>
  <380d>     DW_AT_data_member_location: 3 byte block: 23 90 2  (DW_OP_plus_uconst: 272)
 <2><3811>: Abbrev Number: 20 (DW_TAG_member)
  <3812>     DW_AT_name        : Irp
  <3816>     DW_AT_decl_file   : 10
  <3817>     DW_AT_decl_line   : 790
  <3819>     DW_AT_type        : <1177>
  <381d>     DW_AT_data_member_location: 3 byte block: 23 a0 2  (DW_OP_plus_uconst: 288)
 <2><3821>: Abbrev Number: 20 (DW_TAG_member)
  <3822>     DW_AT_name        : dwData
  <3829>     DW_AT_decl_file   : 10
  <382a>     DW_AT_decl_line   : 791
  <382c>     DW_AT_type        : <1a4>
  <3830>     DW_AT_data_member_location: 3 byte block: 23 cc 2  (DW_OP_plus_uconst: 332)
 <2><3834>: Abbrev Number: 20 (DW_TAG_member)
  <3835>     DW_AT_name        : wData
  <383b>     DW_AT_decl_file   : 10
  <383c>     DW_AT_decl_line   : 792
  <383e>     DW_AT_type        : <185>
  <3842>     DW_AT_data_member_location: 3 byte block: 23 d0 2  (DW_OP_plus_uconst: 336)
 <2><3846>: Abbrev Number: 20 (DW_TAG_member)
  <3847>     DW_AT_name        : bAddress
  <3850>     DW_AT_decl_file   : 10
  <3851>     DW_AT_decl_line   : 793
  <3853>     DW_AT_type        : <176>
  <3857>     DW_AT_data_member_location: 3 byte block: 23 d2 2  (DW_OP_plus_uconst: 338)
 <2><385b>: Abbrev Number: 20 (DW_TAG_member)
  <385c>     DW_AT_name        : bState
  <3863>     DW_AT_decl_file   : 10
  <3864>     DW_AT_decl_line   : 794
  <3866>     DW_AT_type        : <176>
  <386a>     DW_AT_data_member_location: 3 byte block: 23 d3 2  (DW_OP_plus_uconst: 339)
 <2><386e>: Abbrev Number: 19 (DW_TAG_member)
  <386f>     DW_AT_name        : (indirect string, offset: 0x141): bIndex
  <3873>     DW_AT_decl_file   : 10
  <3874>     DW_AT_decl_line   : 795
  <3876>     DW_AT_type        : <176>
  <387a>     DW_AT_data_member_location: 3 byte block: 23 d4 2  (DW_OP_plus_uconst: 340)
 <2><387e>: Abbrev Number: 20 (DW_TAG_member)
  <387f>     DW_AT_name        : bCount
  <3886>     DW_AT_decl_file   : 10
  <3887>     DW_AT_decl_line   : 796
  <3889>     DW_AT_type        : <176>
  <388d>     DW_AT_data_member_location: 3 byte block: 23 d5 2  (DW_OP_plus_uconst: 341)
 <2><3891>: Abbrev Number: 20 (DW_TAG_member)
  <3892>     DW_AT_name        : bRetries
  <389b>     DW_AT_decl_file   : 10
  <389c>     DW_AT_decl_line   : 797
  <389e>     DW_AT_type        : <176>
  <38a2>     DW_AT_data_member_location: 3 byte block: 23 d6 2  (DW_OP_plus_uconst: 342)
 <2><38a6>: Abbrev Number: 20 (DW_TAG_member)
  <38a7>     DW_AT_name        : bFatalRetries
  <38b5>     DW_AT_decl_file   : 10
  <38b6>     DW_AT_decl_line   : 798
  <38b8>     DW_AT_type        : <176>
  <38bc>     DW_AT_data_member_location: 3 byte block: 23 d7 2  (DW_OP_plus_uconst: 343)
 <1><38c1>: Abbrev Number: 28 (DW_TAG_array_type)
  <38c2>     DW_AT_type        : <176>
  <38c6>     DW_AT_sibling     : <38d1>
 <2><38ca>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <38cb>     DW_AT_type        : <dd>
  <38cf>     DW_AT_upper_bound : 7
 <1><38d1>: Abbrev Number: 28 (DW_TAG_array_type)
  <38d2>     DW_AT_type        : <176>
  <38d6>     DW_AT_sibling     : <38e1>
 <2><38da>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <38db>     DW_AT_type        : <dd>
  <38df>     DW_AT_upper_bound : 255
 <1><38e1>: Abbrev Number: 28 (DW_TAG_array_type)
  <38e2>     DW_AT_type        : <176>
  <38e6>     DW_AT_sibling     : <38f1>
 <2><38ea>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <38eb>     DW_AT_type        : <dd>
  <38ef>     DW_AT_upper_bound : 15
 <1><38f1>: Abbrev Number: 17 (DW_TAG_typedef)
  <38f2>     DW_AT_name        : MGC_EnumerationData
  <3906>     DW_AT_decl_file   : 10
  <3907>     DW_AT_decl_line   : 799
  <3909>     DW_AT_type        : <3798>
 <1><390d>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <390e>     DW_AT_byte_size   : 4
  <390f>     DW_AT_type        : <33a9>
 <1><3913>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3914>     DW_AT_byte_size   : 4
  <3915>     DW_AT_type        : <19db>
 <1><3919>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <391a>     DW_AT_byte_size   : 4
  <391b>     DW_AT_type        : <18f7>
 <1><391f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3920>     DW_AT_byte_size   : 4
  <3921>     DW_AT_type        : <167d>
 <1><3925>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3926>     DW_AT_byte_size   : 4
  <3927>     DW_AT_type        : <392b>
 <1><392b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <392c>     DW_AT_byte_size   : 4
  <392d>     DW_AT_type        : <3931>
 <1><3931>: Abbrev Number: 8 (DW_TAG_const_type)
  <3932>     DW_AT_type        : <a88>
 <1><3936>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3937>     DW_AT_byte_size   : 4
  <3938>     DW_AT_type        : <3350>
 <1><393c>: Abbrev Number: 17 (DW_TAG_typedef)
  <393d>     DW_AT_name        : MGC_Port
  <3946>     DW_AT_decl_file   : 10
  <3947>     DW_AT_decl_line   : 990
  <3949>     DW_AT_type        : <26e6>
 <1><394d>: Abbrev Number: 2 (DW_TAG_typedef)
  <394e>     DW_AT_name        : FAPI_SYS_HandleT
  <395f>     DW_AT_decl_file   : 11
  <3960>     DW_AT_decl_line   : 71
  <3961>     DW_AT_type        : <f1>
 <1><3965>: Abbrev Number: 21 (DW_TAG_enumeration_type)
  <3966>     DW_AT_byte_size   : 4
  <3967>     DW_AT_decl_file   : 11
  <3968>     DW_AT_decl_line   : 83
  <3969>     DW_AT_sibling     : <39bf>
 <2><396d>: Abbrev Number: 22 (DW_TAG_enumerator)
  <396e>     DW_AT_name        : FAPI_SYS_BSR_EXEC_UNDEFINED
  <398a>     DW_AT_const_value : 0
 <2><398b>: Abbrev Number: 22 (DW_TAG_enumerator)
  <398c>     DW_AT_name        : FAPI_SYS_BSR_EXEC_THREAD
  <39a5>     DW_AT_const_value : 1
 <2><39a6>: Abbrev Number: 22 (DW_TAG_enumerator)
  <39a7>     DW_AT_name        : FAPI_SYS_BSR_EXEC_IRQ
  <39bd>     DW_AT_const_value : 2
 <1><39bf>: Abbrev Number: 2 (DW_TAG_typedef)
  <39c0>     DW_AT_name        : FAPI_SYS_BsrExecutionEnumT
  <39db>     DW_AT_decl_file   : 11
  <39dc>     DW_AT_decl_line   : 125
  <39dd>     DW_AT_type        : <3965>
 <1><39e1>: Abbrev Number: 12 (DW_TAG_structure_type)
  <39e2>     DW_AT_byte_size   : 36
  <39e3>     DW_AT_decl_file   : 11
  <39e4>     DW_AT_decl_line   : 139
  <39e5>     DW_AT_sibling     : <3ab1>
 <2><39e9>: Abbrev Number: 14 (DW_TAG_member)
  <39ea>     DW_AT_name        : driverNamePtr
  <39f8>     DW_AT_decl_file   : 11
  <39f9>     DW_AT_decl_line   : 150
  <39fa>     DW_AT_type        : <116>
  <39fe>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><3a01>: Abbrev Number: 14 (DW_TAG_member)
  <3a02>     DW_AT_name        : irqNo
  <3a08>     DW_AT_decl_file   : 11
  <3a09>     DW_AT_decl_line   : 163
  <3a0a>     DW_AT_type        : <195>
  <3a0e>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><3a11>: Abbrev Number: 14 (DW_TAG_member)
  <3a12>     DW_AT_name        : bsrExecution
  <3a1f>     DW_AT_decl_file   : 11
  <3a20>     DW_AT_decl_line   : 177
  <3a21>     DW_AT_type        : <39bf>
  <3a25>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><3a28>: Abbrev Number: 14 (DW_TAG_member)
  <3a29>     DW_AT_name        : initDriverFunc
  <3a38>     DW_AT_decl_file   : 11
  <3a39>     DW_AT_decl_line   : 206
  <3a3a>     DW_AT_type        : <3ab7>
  <3a3e>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><3a41>: Abbrev Number: 14 (DW_TAG_member)
  <3a42>     DW_AT_name        : exitDriverFunc
  <3a51>     DW_AT_decl_file   : 11
  <3a52>     DW_AT_decl_line   : 222
  <3a53>     DW_AT_type        : <f5>
  <3a57>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><3a5a>: Abbrev Number: 20 (DW_TAG_member)
  <3a5b>     DW_AT_name        : isrFunc
  <3a63>     DW_AT_decl_file   : 11
  <3a64>     DW_AT_decl_line   : 257
  <3a66>     DW_AT_type        : <3e5>
  <3a6a>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><3a6d>: Abbrev Number: 20 (DW_TAG_member)
  <3a6e>     DW_AT_name        : bsrFunc
  <3a76>     DW_AT_decl_file   : 11
  <3a77>     DW_AT_decl_line   : 291
  <3a79>     DW_AT_type        : <3ac9>
  <3a7d>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><3a80>: Abbrev Number: 20 (DW_TAG_member)
  <3a81>     DW_AT_name        : bsrStackSize
  <3a8e>     DW_AT_decl_file   : 11
  <3a8f>     DW_AT_decl_line   : 302
  <3a91>     DW_AT_type        : <1a4>
  <3a95>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><3a98>: Abbrev Number: 20 (DW_TAG_member)
  <3a99>     DW_AT_name        : bsrFifoDepth
  <3aa6>     DW_AT_decl_file   : 11
  <3aa7>     DW_AT_decl_line   : 312
  <3aa9>     DW_AT_type        : <1a4>
  <3aad>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <1><3ab1>: Abbrev Number: 15 (DW_TAG_subroutine_type)
  <3ab2>     DW_AT_prototyped  : 1
  <3ab3>     DW_AT_type        : <195>
 <1><3ab7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3ab8>     DW_AT_byte_size   : 4
  <3ab9>     DW_AT_type        : <3ab1>
 <1><3abd>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <3abe>     DW_AT_prototyped  : 1
  <3abf>     DW_AT_sibling     : <3ac9>
 <2><3ac3>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3ac4>     DW_AT_type        : <1a4>
 <1><3ac9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3aca>     DW_AT_byte_size   : 4
  <3acb>     DW_AT_type        : <3abd>
 <1><3acf>: Abbrev Number: 17 (DW_TAG_typedef)
  <3ad0>     DW_AT_name        : FAPI_SYS_DriverT
  <3ae1>     DW_AT_decl_file   : 11
  <3ae2>     DW_AT_decl_line   : 313
  <3ae4>     DW_AT_type        : <39e1>
 <1><3ae8>: Abbrev Number: 17 (DW_TAG_typedef)
  <3ae9>     DW_AT_name        : FAPI_SYS_SemaphoreT
  <3afd>     DW_AT_decl_file   : 12
  <3afe>     DW_AT_decl_line   : 965
  <3b00>     DW_AT_type        : <1a4>
 <1><3b04>: Abbrev Number: 17 (DW_TAG_typedef)
  <3b05>     DW_AT_name        : FAPI_SYS_MailqueueT
  <3b19>     DW_AT_decl_file   : 12
  <3b1a>     DW_AT_decl_line   : 975
  <3b1c>     DW_AT_type        : <f1>
 <1><3b20>: Abbrev Number: 18 (DW_TAG_structure_type)
  <3b21>     DW_AT_byte_size   : 88
  <3b22>     DW_AT_decl_file   : 12
  <3b23>     DW_AT_decl_line   : 1029
  <3b25>     DW_AT_sibling     : <3d8a>
 <2><3b29>: Abbrev Number: 20 (DW_TAG_member)
  <3b2a>     DW_AT_name        : lockSchedulerFunc
  <3b3c>     DW_AT_decl_file   : 12
  <3b3d>     DW_AT_decl_line   : 1044
  <3b3f>     DW_AT_type        : <f5>
  <3b43>     DW_AT_data_member_location: 2 byte block: 23 0     (DW_OP_plus_uconst: 0)
 <2><3b46>: Abbrev Number: 20 (DW_TAG_member)
  <3b47>     DW_AT_name        : unlockSchedulerFunc
  <3b5b>     DW_AT_decl_file   : 12
  <3b5c>     DW_AT_decl_line   : 1060
  <3b5e>     DW_AT_type        : <f5>
  <3b62>     DW_AT_data_member_location: 2 byte block: 23 4     (DW_OP_plus_uconst: 4)
 <2><3b65>: Abbrev Number: 20 (DW_TAG_member)
  <3b66>     DW_AT_name        : sleepFunc
  <3b70>     DW_AT_decl_file   : 12
  <3b71>     DW_AT_decl_line   : 1073
  <3b73>     DW_AT_type        : <3ac9>
  <3b77>     DW_AT_data_member_location: 2 byte block: 23 8     (DW_OP_plus_uconst: 8)
 <2><3b7a>: Abbrev Number: 20 (DW_TAG_member)
  <3b7b>     DW_AT_name        : disableIrqFunc
  <3b8a>     DW_AT_decl_file   : 12
  <3b8b>     DW_AT_decl_line   : 1092
  <3b8d>     DW_AT_type        : <3e5>
  <3b91>     DW_AT_data_member_location: 2 byte block: 23 c     (DW_OP_plus_uconst: 12)
 <2><3b94>: Abbrev Number: 20 (DW_TAG_member)
  <3b95>     DW_AT_name        : enableIrqFunc
  <3ba3>     DW_AT_decl_file   : 12
  <3ba4>     DW_AT_decl_line   : 1109
  <3ba6>     DW_AT_type        : <3ac9>
  <3baa>     DW_AT_data_member_location: 2 byte block: 23 10    (DW_OP_plus_uconst: 16)
 <2><3bad>: Abbrev Number: 20 (DW_TAG_member)
  <3bae>     DW_AT_name        : createSemaphoreFunc
  <3bc2>     DW_AT_decl_file   : 12
  <3bc3>     DW_AT_decl_line   : 1133
  <3bc5>     DW_AT_type        : <3d9a>
  <3bc9>     DW_AT_data_member_location: 2 byte block: 23 14    (DW_OP_plus_uconst: 20)
 <2><3bcc>: Abbrev Number: 20 (DW_TAG_member)
  <3bcd>     DW_AT_name        : destroySemaphoreFunc
  <3be2>     DW_AT_decl_file   : 12
  <3be3>     DW_AT_decl_line   : 1154
  <3be5>     DW_AT_type        : <3dac>
  <3be9>     DW_AT_data_member_location: 2 byte block: 23 18    (DW_OP_plus_uconst: 24)
 <2><3bec>: Abbrev Number: 20 (DW_TAG_member)
  <3bed>     DW_AT_name        : getSemaphoreFunc
  <3bfe>     DW_AT_decl_file   : 12
  <3bff>     DW_AT_decl_line   : 1194
  <3c01>     DW_AT_type        : <3dc7>
  <3c05>     DW_AT_data_member_location: 2 byte block: 23 1c    (DW_OP_plus_uconst: 28)
 <2><3c08>: Abbrev Number: 20 (DW_TAG_member)
  <3c09>     DW_AT_name        : setSemaphoreFunc
  <3c1a>     DW_AT_decl_file   : 12
  <3c1b>     DW_AT_decl_line   : 1223
  <3c1d>     DW_AT_type        : <3dde>
  <3c21>     DW_AT_data_member_location: 2 byte block: 23 20    (DW_OP_plus_uconst: 32)
 <2><3c24>: Abbrev Number: 20 (DW_TAG_member)
  <3c25>     DW_AT_name        : createMailqueueFunc
  <3c39>     DW_AT_decl_file   : 12
  <3c3a>     DW_AT_decl_line   : 1249
  <3c3c>     DW_AT_type        : <3df9>
  <3c40>     DW_AT_data_member_location: 2 byte block: 23 24    (DW_OP_plus_uconst: 36)
 <2><3c43>: Abbrev Number: 20 (DW_TAG_member)
  <3c44>     DW_AT_name        : destroyMailqueueFunc
  <3c59>     DW_AT_decl_file   : 12
  <3c5a>     DW_AT_decl_line   : 1270
  <3c5c>     DW_AT_type        : <3e0b>
  <3c60>     DW_AT_data_member_location: 2 byte block: 23 28    (DW_OP_plus_uconst: 40)
 <2><3c63>: Abbrev Number: 20 (DW_TAG_member)
  <3c64>     DW_AT_name        : getMailqueueFunc
  <3c75>     DW_AT_decl_file   : 12
  <3c76>     DW_AT_decl_line   : 1316
  <3c78>     DW_AT_type        : <3e2b>
  <3c7c>     DW_AT_data_member_location: 2 byte block: 23 2c    (DW_OP_plus_uconst: 44)
 <2><3c7f>: Abbrev Number: 20 (DW_TAG_member)
  <3c80>     DW_AT_name        : setMailqueueFunc
  <3c91>     DW_AT_decl_file   : 12
  <3c92>     DW_AT_decl_line   : 1358
  <3c94>     DW_AT_type        : <3e2b>
  <3c98>     DW_AT_data_member_location: 2 byte block: 23 30    (DW_OP_plus_uconst: 48)
 <2><3c9b>: Abbrev Number: 20 (DW_TAG_member)
  <3c9c>     DW_AT_name        : getPhysAddressFunc
  <3caf>     DW_AT_decl_file   : 12
  <3cb0>     DW_AT_decl_line   : 1375
  <3cb2>     DW_AT_type        : <3e41>
  <3cb6>     DW_AT_data_member_location: 2 byte block: 23 34    (DW_OP_plus_uconst: 52)
 <2><3cb9>: Abbrev Number: 20 (DW_TAG_member)
  <3cba>     DW_AT_name        : getVirtAddressFunc
  <3ccd>     DW_AT_decl_file   : 12
  <3cce>     DW_AT_decl_line   : 1392
  <3cd0>     DW_AT_type        : <3e41>
  <3cd4>     DW_AT_data_member_location: 2 byte block: 23 38    (DW_OP_plus_uconst: 56)
 <2><3cd7>: Abbrev Number: 20 (DW_TAG_member)
  <3cd8>     DW_AT_name        : mallocFunc
  <3ce3>     DW_AT_decl_file   : 12
  <3ce4>     DW_AT_decl_line   : 1412
  <3ce6>     DW_AT_type        : <3e57>
  <3cea>     DW_AT_data_member_location: 2 byte block: 23 3c    (DW_OP_plus_uconst: 60)
 <2><3ced>: Abbrev Number: 20 (DW_TAG_member)
  <3cee>     DW_AT_name        : mallocUncachedFunc
  <3d01>     DW_AT_decl_file   : 12
  <3d02>     DW_AT_decl_line   : 1432
  <3d04>     DW_AT_type        : <3e57>
  <3d08>     DW_AT_data_member_location: 2 byte block: 23 40    (DW_OP_plus_uconst: 64)
 <2><3d0b>: Abbrev Number: 20 (DW_TAG_member)
  <3d0c>     DW_AT_name        : mallocCachedFunc
  <3d1d>     DW_AT_decl_file   : 12
  <3d1e>     DW_AT_decl_line   : 1452
  <3d20>     DW_AT_type        : <3e57>
  <3d24>     DW_AT_data_member_location: 2 byte block: 23 44    (DW_OP_plus_uconst: 68)
 <2><3d27>: Abbrev Number: 20 (DW_TAG_member)
  <3d28>     DW_AT_name        : mallocSegmentFunc
  <3d3a>     DW_AT_decl_file   : 12
  <3d3b>     DW_AT_decl_line   : 1473
  <3d3d>     DW_AT_type        : <3e72>
  <3d41>     DW_AT_data_member_location: 2 byte block: 23 48    (DW_OP_plus_uconst: 72)
 <2><3d44>: Abbrev Number: 20 (DW_TAG_member)
  <3d45>     DW_AT_name        : freeFunc
  <3d4e>     DW_AT_decl_file   : 12
  <3d4f>     DW_AT_decl_line   : 1491
  <3d51>     DW_AT_type        : <211>
  <3d55>     DW_AT_data_member_location: 2 byte block: 23 4c    (DW_OP_plus_uconst: 76)
 <2><3d58>: Abbrev Number: 20 (DW_TAG_member)
  <3d59>     DW_AT_name        : printfFunc
  <3d64>     DW_AT_decl_file   : 12
  <3d65>     DW_AT_decl_line   : 1512
  <3d67>     DW_AT_type        : <3e89>
  <3d6b>     DW_AT_data_member_location: 2 byte block: 23 50    (DW_OP_plus_uconst: 80)
 <2><3d6e>: Abbrev Number: 20 (DW_TAG_member)
  <3d6f>     DW_AT_name        : initDriversFunc
  <3d7f>     DW_AT_decl_file   : 12
  <3d80>     DW_AT_decl_line   : 1545
  <3d82>     DW_AT_type        : <3eab>
  <3d86>     DW_AT_data_member_location: 2 byte block: 23 54    (DW_OP_plus_uconst: 84)
 <1><3d8a>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3d8b>     DW_AT_prototyped  : 1
  <3d8c>     DW_AT_type        : <3ae8>
  <3d90>     DW_AT_sibling     : <3d9a>
 <2><3d94>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3d95>     DW_AT_type        : <195>
 <1><3d9a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3d9b>     DW_AT_byte_size   : 4
  <3d9c>     DW_AT_type        : <3d8a>
 <1><3da0>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <3da1>     DW_AT_prototyped  : 1
  <3da2>     DW_AT_sibling     : <3dac>
 <2><3da6>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3da7>     DW_AT_type        : <3ae8>
 <1><3dac>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3dad>     DW_AT_byte_size   : 4
  <3dae>     DW_AT_type        : <3da0>
 <1><3db2>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3db3>     DW_AT_prototyped  : 1
  <3db4>     DW_AT_type        : <195>
  <3db8>     DW_AT_sibling     : <3dc7>
 <2><3dbc>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3dbd>     DW_AT_type        : <3ae8>
 <2><3dc1>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3dc2>     DW_AT_type        : <1a4>
 <1><3dc7>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3dc8>     DW_AT_byte_size   : 4
  <3dc9>     DW_AT_type        : <3db2>
 <1><3dcd>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <3dce>     DW_AT_prototyped  : 1
  <3dcf>     DW_AT_sibling     : <3dde>
 <2><3dd3>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3dd4>     DW_AT_type        : <3ae8>
 <2><3dd8>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3dd9>     DW_AT_type        : <1a4>
 <1><3dde>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3ddf>     DW_AT_byte_size   : 4
  <3de0>     DW_AT_type        : <3dcd>
 <1><3de4>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3de5>     DW_AT_prototyped  : 1
  <3de6>     DW_AT_type        : <3b04>
  <3dea>     DW_AT_sibling     : <3df9>
 <2><3dee>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3def>     DW_AT_type        : <1a4>
 <2><3df3>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3df4>     DW_AT_type        : <1a4>
 <1><3df9>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3dfa>     DW_AT_byte_size   : 4
  <3dfb>     DW_AT_type        : <3de4>
 <1><3dff>: Abbrev Number: 11 (DW_TAG_subroutine_type)
  <3e00>     DW_AT_prototyped  : 1
  <3e01>     DW_AT_sibling     : <3e0b>
 <2><3e05>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e06>     DW_AT_type        : <3b04>
 <1><3e0b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e0c>     DW_AT_byte_size   : 4
  <3e0d>     DW_AT_type        : <3dff>
 <1><3e11>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e12>     DW_AT_prototyped  : 1
  <3e13>     DW_AT_type        : <195>
  <3e17>     DW_AT_sibling     : <3e2b>
 <2><3e1b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e1c>     DW_AT_type        : <3b04>
 <2><3e20>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e21>     DW_AT_type        : <f1>
 <2><3e25>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e26>     DW_AT_type        : <1a4>
 <1><3e2b>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e2c>     DW_AT_byte_size   : 4
  <3e2d>     DW_AT_type        : <3e11>
 <1><3e31>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e32>     DW_AT_prototyped  : 1
  <3e33>     DW_AT_type        : <1a4>
  <3e37>     DW_AT_sibling     : <3e41>
 <2><3e3b>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e3c>     DW_AT_type        : <1a4>
 <1><3e41>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e42>     DW_AT_byte_size   : 4
  <3e43>     DW_AT_type        : <3e31>
 <1><3e47>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e48>     DW_AT_prototyped  : 1
  <3e49>     DW_AT_type        : <f1>
  <3e4d>     DW_AT_sibling     : <3e57>
 <2><3e51>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e52>     DW_AT_type        : <86>
 <1><3e57>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e58>     DW_AT_byte_size   : 4
  <3e59>     DW_AT_type        : <3e47>
 <1><3e5d>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e5e>     DW_AT_prototyped  : 1
  <3e5f>     DW_AT_type        : <f1>
  <3e63>     DW_AT_sibling     : <3e72>
 <2><3e67>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e68>     DW_AT_type        : <394d>
 <2><3e6c>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e6d>     DW_AT_type        : <86>
 <1><3e72>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e73>     DW_AT_byte_size   : 4
  <3e74>     DW_AT_type        : <3e5d>
 <1><3e78>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e79>     DW_AT_prototyped  : 1
  <3e7a>     DW_AT_type        : <195>
  <3e7e>     DW_AT_sibling     : <3e89>
 <2><3e82>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e83>     DW_AT_type        : <116>
 <2><3e87>: Abbrev Number: 30 (DW_TAG_unspecified_parameters)
 <1><3e89>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3e8a>     DW_AT_byte_size   : 4
  <3e8b>     DW_AT_type        : <3e78>
 <1><3e8f>: Abbrev Number: 9 (DW_TAG_subroutine_type)
  <3e90>     DW_AT_prototyped  : 1
  <3e91>     DW_AT_type        : <195>
  <3e95>     DW_AT_sibling     : <3e9f>
 <2><3e99>: Abbrev Number: 10 (DW_TAG_formal_parameter)
  <3e9a>     DW_AT_type        : <3e9f>
 <1><3e9f>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3ea0>     DW_AT_byte_size   : 4
  <3ea1>     DW_AT_type        : <3ea5>
 <1><3ea5>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3ea6>     DW_AT_byte_size   : 4
  <3ea7>     DW_AT_type        : <3acf>
 <1><3eab>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3eac>     DW_AT_byte_size   : 4
  <3ead>     DW_AT_type        : <3e8f>
 <1><3eb1>: Abbrev Number: 17 (DW_TAG_typedef)
  <3eb2>     DW_AT_name        : FAPI_SYS_ServicesT
  <3ec5>     DW_AT_decl_file   : 12
  <3ec6>     DW_AT_decl_line   : 1547
  <3ec8>     DW_AT_type        : <3b20>
 <1><3ecc>: Abbrev Number: 31 (DW_TAG_subprogram)
  <3ecd>     DW_AT_external    : 1
  <3ece>     DW_AT_name        : MGC_HdrcInit
  <3edb>     DW_AT_decl_file   : 1
  <3edc>     DW_AT_decl_line   : 142
  <3edd>     DW_AT_prototyped  : 1
  <3ede>     DW_AT_type        : <176>
  <3ee2>     DW_AT_low_pc      : 0
  <3ee6>     DW_AT_high_pc     : 0x138
  <3eea>     DW_AT_frame_base  : 0  (location list)
  <3eee>     DW_AT_sibling     : <3f3a>
 <2><3ef2>: Abbrev Number: 32 (DW_TAG_formal_parameter)
  <3ef3>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <3ef7>     DW_AT_decl_file   : 1
  <3ef8>     DW_AT_decl_line   : 141
  <3ef9>     DW_AT_type        : <3f3a>
  <3efd>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><3f00>: Abbrev Number: 33 (DW_TAG_variable)
  <3f01>     DW_AT_name        : config
  <3f08>     DW_AT_decl_file   : 1
  <3f09>     DW_AT_decl_line   : 147
  <3f0a>     DW_AT_type        : <176>
  <3f0e>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><3f11>: Abbrev Number: 33 (DW_TAG_variable)
  <3f12>     DW_AT_name        : pImplController
  <3f22>     DW_AT_decl_file   : 1
  <3f23>     DW_AT_decl_line   : 148
  <3f24>     DW_AT_type        : <390d>
  <3f28>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><3f2b>: Abbrev Number: 34 (DW_TAG_variable)
  <3f2c>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <3f30>     DW_AT_decl_file   : 1
  <3f31>     DW_AT_decl_line   : 149
  <3f32>     DW_AT_type        : <f28>
  <3f36>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><3f3a>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <3f3b>     DW_AT_byte_size   : 4
  <3f3c>     DW_AT_type        : <393c>
 <1><3f40>: Abbrev Number: 35 (DW_TAG_subprogram)
  <3f41>     DW_AT_external    : 1
  <3f42>     DW_AT_name        : MGC_HdrcDynamicFifoLocation
  <3f5e>     DW_AT_decl_file   : 1
  <3f5f>     DW_AT_decl_line   : 246
  <3f60>     DW_AT_prototyped  : 1
  <3f61>     DW_AT_low_pc      : 0x138
  <3f65>     DW_AT_high_pc     : 0x170
  <3f69>     DW_AT_frame_base  : 0x2a   (location list)
  <3f6d>     DW_AT_sibling     : <3fed>
 <2><3f71>: Abbrev Number: 32 (DW_TAG_formal_parameter)
  <3f72>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <3f76>     DW_AT_decl_file   : 1
  <3f77>     DW_AT_decl_line   : 239
  <3f78>     DW_AT_type        : <3f3a>
  <3f7c>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><3f7f>: Abbrev Number: 32 (DW_TAG_formal_parameter)
  <3f80>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <3f84>     DW_AT_decl_file   : 1
  <3f85>     DW_AT_decl_line   : 240
  <3f86>     DW_AT_type        : <176>
  <3f8a>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><3f8d>: Abbrev Number: 36 (DW_TAG_formal_parameter)
  <3f8e>     DW_AT_name        : bIsTx
  <3f94>     DW_AT_decl_file   : 1
  <3f95>     DW_AT_decl_line   : 241
  <3f96>     DW_AT_type        : <176>
  <3f9a>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><3f9d>: Abbrev Number: 36 (DW_TAG_formal_parameter)
  <3f9e>     DW_AT_name        : bSet
  <3fa3>     DW_AT_decl_file   : 1
  <3fa4>     DW_AT_decl_line   : 242
  <3fa5>     DW_AT_type        : <176>
  <3fa9>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><3fac>: Abbrev Number: 36 (DW_TAG_formal_parameter)
  <3fad>     DW_AT_name        : pdwBase
  <3fb5>     DW_AT_decl_file   : 1
  <3fb6>     DW_AT_decl_line   : 243
  <3fb7>     DW_AT_type        : <13dd>
  <3fbb>     DW_AT_location    : 2 byte block: 91 0     (DW_OP_fbreg: 0)
 <2><3fbe>: Abbrev Number: 36 (DW_TAG_formal_parameter)
  <3fbf>     DW_AT_name        : pbSize83
  <3fc8>     DW_AT_decl_file   : 1
  <3fc9>     DW_AT_decl_line   : 244
  <3fca>     DW_AT_type        : <f28>
  <3fce>     DW_AT_location    : 2 byte block: 91 4     (DW_OP_fbreg: 4)
 <2><3fd1>: Abbrev Number: 36 (DW_TAG_formal_parameter)
  <3fd2>     DW_AT_name        : pbDoubleBuffered
  <3fe3>     DW_AT_decl_file   : 1
  <3fe4>     DW_AT_decl_line   : 245
  <3fe5>     DW_AT_type        : <f28>
  <3fe9>     DW_AT_location    : 2 byte block: 91 8     (DW_OP_fbreg: 8)
 <1><3fed>: Abbrev Number: 37 (DW_TAG_subprogram)
  <3fee>     DW_AT_external    : 1
  <3fef>     DW_AT_name        : MGC_HdrcIsr
  <3ffb>     DW_AT_decl_file   : 1
  <3ffc>     DW_AT_decl_line   : 313
  <3ffe>     DW_AT_prototyped  : 1
  <3fff>     DW_AT_type        : <a9>
  <4003>     DW_AT_low_pc      : 0x170
  <4007>     DW_AT_high_pc     : 0x23c
  <400b>     DW_AT_frame_base  : 0x54   (location list)
  <400f>     DW_AT_sibling     : <40c6>
 <2><4013>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4014>     DW_AT_name        : (indirect string, offset: 0x1cb): pParam
  <4018>     DW_AT_decl_file   : 1
  <4019>     DW_AT_decl_line   : 312
  <401b>     DW_AT_type        : <f1>
  <401f>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4022>: Abbrev Number: 39 (DW_TAG_variable)
  <4023>     DW_AT_name        : result
  <402a>     DW_AT_decl_file   : 1
  <402b>     DW_AT_decl_line   : 314
  <402d>     DW_AT_type        : <a9>
  <4031>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4034>: Abbrev Number: 39 (DW_TAG_variable)
  <4035>     DW_AT_name        : bIntrUsbValue
  <4043>     DW_AT_decl_file   : 1
  <4044>     DW_AT_decl_line   : 315
  <4046>     DW_AT_type        : <176>
  <404a>     DW_AT_location    : 2 byte block: 91 5b    (DW_OP_fbreg: -37)
 <2><404d>: Abbrev Number: 39 (DW_TAG_variable)
  <404e>     DW_AT_name        : wIntrTxValue
  <405b>     DW_AT_decl_file   : 1
  <405c>     DW_AT_decl_line   : 316
  <405e>     DW_AT_type        : <185>
  <4062>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4065>: Abbrev Number: 39 (DW_TAG_variable)
  <4066>     DW_AT_name        : wIntrRxValue
  <4073>     DW_AT_decl_file   : 1
  <4074>     DW_AT_decl_line   : 316
  <4076>     DW_AT_type        : <185>
  <407a>     DW_AT_location    : 2 byte block: 91 5e    (DW_OP_fbreg: -34)
 <2><407d>: Abbrev Number: 40 (DW_TAG_variable)
  <407e>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4082>     DW_AT_decl_file   : 1
  <4083>     DW_AT_decl_line   : 317
  <4085>     DW_AT_type        : <40c6>
  <4089>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><408c>: Abbrev Number: 39 (DW_TAG_variable)
  <408d>     DW_AT_name        : pControllerImpl
  <409d>     DW_AT_decl_file   : 1
  <409e>     DW_AT_decl_line   : 318
  <40a0>     DW_AT_type        : <390d>
  <40a4>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><40a7>: Abbrev Number: 40 (DW_TAG_variable)
  <40a8>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <40ac>     DW_AT_decl_file   : 1
  <40ad>     DW_AT_decl_line   : 324
  <40af>     DW_AT_type        : <f28>
  <40b3>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><40b6>: Abbrev Number: 40 (DW_TAG_variable)
  <40b7>     DW_AT_name        : (indirect string, offset: 0x141): bIndex
  <40bb>     DW_AT_decl_file   : 1
  <40bc>     DW_AT_decl_line   : 326
  <40be>     DW_AT_type        : <176>
  <40c2>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><40c6>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <40c7>     DW_AT_byte_size   : 4
  <40c8>     DW_AT_type        : <336>
 <1><40cc>: Abbrev Number: 37 (DW_TAG_subprogram)
  <40cd>     DW_AT_external    : 1
  <40ce>     DW_AT_name        : MGC_HdrcStart
  <40dc>     DW_AT_decl_file   : 1
  <40dd>     DW_AT_decl_line   : 358
  <40df>     DW_AT_prototyped  : 1
  <40e0>     DW_AT_type        : <1a4>
  <40e4>     DW_AT_low_pc      : 0x23c
  <40e8>     DW_AT_high_pc     : 0x490
  <40ec>     DW_AT_frame_base  : 0x7e   (location list)
  <40f0>     DW_AT_sibling     : <4131>
 <2><40f4>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <40f5>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <40f9>     DW_AT_decl_file   : 1
  <40fa>     DW_AT_decl_line   : 357
  <40fc>     DW_AT_type        : <390d>
  <4100>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4103>: Abbrev Number: 39 (DW_TAG_variable)
  <4104>     DW_AT_name        : val
  <4108>     DW_AT_decl_file   : 1
  <4109>     DW_AT_decl_line   : 359
  <410b>     DW_AT_type        : <185>
  <410f>     DW_AT_location    : 2 byte block: 91 66    (DW_OP_fbreg: -26)
 <2><4112>: Abbrev Number: 40 (DW_TAG_variable)
  <4113>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4117>     DW_AT_decl_file   : 1
  <4118>     DW_AT_decl_line   : 360
  <411a>     DW_AT_type        : <f28>
  <411e>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4121>: Abbrev Number: 40 (DW_TAG_variable)
  <4122>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4126>     DW_AT_decl_file   : 1
  <4127>     DW_AT_decl_line   : 361
  <4129>     DW_AT_type        : <3f3a>
  <412d>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4131>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4132>     DW_AT_external    : 1
  <4133>     DW_AT_name        : MGC_HdrcStop
  <4140>     DW_AT_decl_file   : 1
  <4141>     DW_AT_decl_line   : 388
  <4143>     DW_AT_prototyped  : 1
  <4144>     DW_AT_type        : <1a4>
  <4148>     DW_AT_low_pc      : 0x490
  <414c>     DW_AT_high_pc     : 0x52c
  <4150>     DW_AT_frame_base  : 0xa8   (location list)
  <4154>     DW_AT_sibling     : <4187>
 <2><4158>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4159>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <415d>     DW_AT_decl_file   : 1
  <415e>     DW_AT_decl_line   : 387
  <4160>     DW_AT_type        : <390d>
  <4164>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4167>: Abbrev Number: 39 (DW_TAG_variable)
  <4168>     DW_AT_name        : temp
  <416d>     DW_AT_decl_file   : 1
  <416e>     DW_AT_decl_line   : 389
  <4170>     DW_AT_type        : <185>
  <4174>     DW_AT_location    : 2 byte block: 91 6a    (DW_OP_fbreg: -22)
 <2><4177>: Abbrev Number: 40 (DW_TAG_variable)
  <4178>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <417c>     DW_AT_decl_file   : 1
  <417d>     DW_AT_decl_line   : 390
  <417f>     DW_AT_type        : <f28>
  <4183>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4187>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4188>     DW_AT_external    : 1
  <4189>     DW_AT_name        : MGC_HdrcDestroy
  <4199>     DW_AT_decl_file   : 1
  <419a>     DW_AT_decl_line   : 406
  <419c>     DW_AT_prototyped  : 1
  <419d>     DW_AT_type        : <1a4>
  <41a1>     DW_AT_low_pc      : 0x52c
  <41a5>     DW_AT_high_pc     : 0x598
  <41a9>     DW_AT_frame_base  : 0xd2   (location list)
  <41ad>     DW_AT_sibling     : <41d0>
 <2><41b1>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <41b2>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <41b6>     DW_AT_decl_file   : 1
  <41b7>     DW_AT_decl_line   : 405
  <41b9>     DW_AT_type        : <390d>
  <41bd>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><41c0>: Abbrev Number: 40 (DW_TAG_variable)
  <41c1>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <41c5>     DW_AT_decl_file   : 1
  <41c6>     DW_AT_decl_line   : 407
  <41c8>     DW_AT_type        : <3f3a>
  <41cc>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><41d0>: Abbrev Number: 37 (DW_TAG_subprogram)
  <41d1>     DW_AT_external    : 1
  <41d2>     DW_AT_name        : MGC_HdrcReadBusState
  <41e7>     DW_AT_decl_file   : 1
  <41e8>     DW_AT_decl_line   : 419
  <41ea>     DW_AT_prototyped  : 1
  <41eb>     DW_AT_type        : <1a4>
  <41ef>     DW_AT_low_pc      : 0x598
  <41f3>     DW_AT_high_pc     : 0x788
  <41f7>     DW_AT_frame_base  : 0xfc   (location list)
  <41fb>     DW_AT_sibling     : <4260>
 <2><41ff>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4200>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4204>     DW_AT_decl_file   : 1
  <4205>     DW_AT_decl_line   : 418
  <4207>     DW_AT_type        : <3f3a>
  <420b>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><420e>: Abbrev Number: 39 (DW_TAG_variable)
  <420f>     DW_AT_name        : devctl
  <4216>     DW_AT_decl_file   : 1
  <4217>     DW_AT_decl_line   : 420
  <4219>     DW_AT_type        : <176>
  <421d>     DW_AT_location    : 2 byte block: 91 65    (DW_OP_fbreg: -27)
 <2><4220>: Abbrev Number: 39 (DW_TAG_variable)
  <4221>     DW_AT_name        : power
  <4227>     DW_AT_decl_file   : 1
  <4228>     DW_AT_decl_line   : 421
  <422a>     DW_AT_type        : <176>
  <422e>     DW_AT_location    : 2 byte block: 91 66    (DW_OP_fbreg: -26)
 <2><4231>: Abbrev Number: 39 (DW_TAG_variable)
  <4232>     DW_AT_name        : vbus
  <4237>     DW_AT_decl_file   : 1
  <4238>     DW_AT_decl_line   : 422
  <423a>     DW_AT_type        : <176>
  <423e>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4241>: Abbrev Number: 40 (DW_TAG_variable)
  <4242>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4246>     DW_AT_decl_file   : 1
  <4247>     DW_AT_decl_line   : 423
  <4249>     DW_AT_type        : <390d>
  <424d>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4250>: Abbrev Number: 40 (DW_TAG_variable)
  <4251>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4255>     DW_AT_decl_file   : 1
  <4256>     DW_AT_decl_line   : 424
  <4258>     DW_AT_type        : <f28>
  <425c>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4260>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4261>     DW_AT_external    : 1
  <4262>     DW_AT_name        : MGC_HdrcProgramBusState
  <427a>     DW_AT_decl_file   : 1
  <427b>     DW_AT_decl_line   : 474
  <427d>     DW_AT_prototyped  : 1
  <427e>     DW_AT_type        : <1a4>
  <4282>     DW_AT_low_pc      : 0x788
  <4286>     DW_AT_high_pc     : 0xb60
  <428a>     DW_AT_frame_base  : 0x126  (location list)
  <428e>     DW_AT_sibling     : <4317>
 <2><4292>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4293>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4297>     DW_AT_decl_file   : 1
  <4298>     DW_AT_decl_line   : 473
  <429a>     DW_AT_type        : <3f3a>
  <429e>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><42a1>: Abbrev Number: 40 (DW_TAG_variable)
  <42a2>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <42a6>     DW_AT_decl_file   : 1
  <42a7>     DW_AT_decl_line   : 475
  <42a9>     DW_AT_type        : <390d>
  <42ad>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><42b0>: Abbrev Number: 40 (DW_TAG_variable)
  <42b1>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <42b5>     DW_AT_decl_file   : 1
  <42b6>     DW_AT_decl_line   : 476
  <42b8>     DW_AT_type        : <f28>
  <42bc>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><42bf>: Abbrev Number: 39 (DW_TAG_variable)
  <42c0>     DW_AT_name        : power
  <42c6>     DW_AT_decl_file   : 1
  <42c7>     DW_AT_decl_line   : 477
  <42c9>     DW_AT_type        : <176>
  <42cd>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <2><42d0>: Abbrev Number: 39 (DW_TAG_variable)
  <42d1>     DW_AT_name        : nPower
  <42d8>     DW_AT_decl_file   : 1
  <42d9>     DW_AT_decl_line   : 478
  <42db>     DW_AT_type        : <176>
  <42df>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><42e2>: Abbrev Number: 39 (DW_TAG_variable)
  <42e3>     DW_AT_name        : devctl
  <42ea>     DW_AT_decl_file   : 1
  <42eb>     DW_AT_decl_line   : 479
  <42ed>     DW_AT_type        : <176>
  <42f1>     DW_AT_location    : 2 byte block: 91 6d    (DW_OP_fbreg: -19)
 <2><42f4>: Abbrev Number: 39 (DW_TAG_variable)
  <42f5>     DW_AT_name        : nDevCtl
  <42fd>     DW_AT_decl_file   : 1
  <42fe>     DW_AT_decl_line   : 480
  <4300>     DW_AT_type        : <176>
  <4304>     DW_AT_location    : 2 byte block: 91 6e    (DW_OP_fbreg: -18)
 <2><4307>: Abbrev Number: 40 (DW_TAG_variable)
  <4308>     DW_AT_name        : (indirect string, offset: 0xc0): bFuncAddr
  <430c>     DW_AT_decl_file   : 1
  <430d>     DW_AT_decl_line   : 481
  <430f>     DW_AT_type        : <176>
  <4313>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><4317>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4318>     DW_AT_external    : 1
  <4319>     DW_AT_name        : MGC_HdrcBindEndpoint
  <432e>     DW_AT_decl_file   : 1
  <432f>     DW_AT_decl_line   : 580
  <4331>     DW_AT_prototyped  : 1
  <4332>     DW_AT_type        : <26e0>
  <4336>     DW_AT_low_pc      : 0xb60
  <433a>     DW_AT_high_pc     : 0x10d0
  <433e>     DW_AT_frame_base  : 0x150  (location list)
  <4342>     DW_AT_sibling     : <4417>
 <2><4346>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4347>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <434b>     DW_AT_decl_file   : 1
  <434c>     DW_AT_decl_line   : 576
  <434e>     DW_AT_type        : <3f3a>
  <4352>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4355>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4356>     DW_AT_name        : pUsbEnd
  <435e>     DW_AT_decl_file   : 1
  <435f>     DW_AT_decl_line   : 577
  <4361>     DW_AT_type        : <3467>
  <4365>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4368>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4369>     DW_AT_name        : pRequest
  <4372>     DW_AT_decl_file   : 1
  <4373>     DW_AT_decl_line   : 578
  <4375>     DW_AT_type        : <3391>
  <4379>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><437c>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <437d>     DW_AT_name        : bBind
  <4383>     DW_AT_decl_file   : 1
  <4384>     DW_AT_decl_line   : 579
  <4386>     DW_AT_type        : <176>
  <438a>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><438d>: Abbrev Number: 40 (DW_TAG_variable)
  <438e>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4392>     DW_AT_decl_file   : 1
  <4393>     DW_AT_decl_line   : 581
  <4395>     DW_AT_type        : <176>
  <4399>     DW_AT_location    : 2 byte block: 91 59    (DW_OP_fbreg: -39)
 <2><439c>: Abbrev Number: 39 (DW_TAG_variable)
  <439d>     DW_AT_name        : bIsTx
  <43a3>     DW_AT_decl_file   : 1
  <43a4>     DW_AT_decl_line   : 581
  <43a6>     DW_AT_type        : <176>
  <43aa>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><43ad>: Abbrev Number: 40 (DW_TAG_variable)
  <43ae>     DW_AT_name        : (indirect string, offset: 0x99): bTrafficType
  <43b2>     DW_AT_decl_file   : 1
  <43b3>     DW_AT_decl_line   : 581
  <43b5>     DW_AT_type        : <176>
  <43b9>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><43bc>: Abbrev Number: 40 (DW_TAG_variable)
  <43bd>     DW_AT_name        : (indirect string, offset: 0x86): bInterval
  <43c1>     DW_AT_decl_file   : 1
  <43c2>     DW_AT_decl_line   : 582
  <43c4>     DW_AT_type        : <176>
  <43c8>     DW_AT_location    : 2 byte block: 91 5b    (DW_OP_fbreg: -37)
 <2><43cb>: Abbrev Number: 40 (DW_TAG_variable)
  <43cc>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <43d0>     DW_AT_decl_file   : 1
  <43d1>     DW_AT_decl_line   : 583
  <43d3>     DW_AT_type        : <26e0>
  <43d7>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><43da>: Abbrev Number: 39 (DW_TAG_variable)
  <43db>     DW_AT_name        : reg
  <43df>     DW_AT_decl_file   : 1
  <43e0>     DW_AT_decl_line   : 584
  <43e2>     DW_AT_type        : <176>
  <43e6>     DW_AT_location    : 2 byte block: 91 61    (DW_OP_fbreg: -31)
 <2><43e9>: Abbrev Number: 39 (DW_TAG_variable)
  <43ea>     DW_AT_name        : csr
  <43ee>     DW_AT_decl_file   : 1
  <43ef>     DW_AT_decl_line   : 585
  <43f1>     DW_AT_type        : <185>
  <43f5>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><43f8>: Abbrev Number: 40 (DW_TAG_variable)
  <43f9>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <43fd>     DW_AT_decl_file   : 1
  <43fe>     DW_AT_decl_line   : 586
  <4400>     DW_AT_type        : <390d>
  <4404>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4407>: Abbrev Number: 40 (DW_TAG_variable)
  <4408>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <440c>     DW_AT_decl_file   : 1
  <440d>     DW_AT_decl_line   : 587
  <440f>     DW_AT_type        : <f28>
  <4413>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><4417>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4418>     DW_AT_external    : 1
  <4419>     DW_AT_name        : MGC_HdrcStartRx
  <4429>     DW_AT_decl_file   : 1
  <442a>     DW_AT_decl_line   : 762
  <442c>     DW_AT_prototyped  : 1
  <442d>     DW_AT_type        : <1a4>
  <4431>     DW_AT_low_pc      : 0x10d0
  <4435>     DW_AT_high_pc     : 0x1aac
  <4439>     DW_AT_frame_base  : 0x17a  (location list)
  <443d>     DW_AT_sibling     : <456b>
 <2><4441>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4442>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4446>     DW_AT_decl_file   : 1
  <4447>     DW_AT_decl_line   : 759
  <4449>     DW_AT_type        : <3f3a>
  <444d>     DW_AT_location    : 3 byte block: 91 bc 7f     (DW_OP_fbreg: -68)
 <2><4451>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4452>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <4456>     DW_AT_decl_file   : 1
  <4457>     DW_AT_decl_line   : 759
  <4459>     DW_AT_type        : <26e0>
  <445d>     DW_AT_location    : 3 byte block: 91 b8 7f     (DW_OP_fbreg: -72)
 <2><4461>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4462>     DW_AT_name        : (indirect string, offset: 0xb8): pBuffer
  <4466>     DW_AT_decl_file   : 1
  <4467>     DW_AT_decl_line   : 760
  <4469>     DW_AT_type        : <f28>
  <446d>     DW_AT_location    : 3 byte block: 91 b4 7f     (DW_OP_fbreg: -76)
 <2><4471>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4472>     DW_AT_name        : (indirect string, offset: 0x59): dwTotalBytes
  <4476>     DW_AT_decl_file   : 1
  <4477>     DW_AT_decl_line   : 760
  <4479>     DW_AT_type        : <1a4>
  <447d>     DW_AT_location    : 3 byte block: 91 b0 7f     (DW_OP_fbreg: -80)
 <2><4481>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4482>     DW_AT_name        : pIrp
  <4487>     DW_AT_decl_file   : 1
  <4488>     DW_AT_decl_line   : 761
  <448a>     DW_AT_type        : <f1>
  <448e>     DW_AT_location    : 2 byte block: 91 0     (DW_OP_fbreg: 0)
 <2><4491>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4492>     DW_AT_name        : (indirect string, offset: 0x17d): bAllowDma
  <4496>     DW_AT_decl_file   : 1
  <4497>     DW_AT_decl_line   : 761
  <4499>     DW_AT_type        : <176>
  <449d>     DW_AT_location    : 3 byte block: 91 ac 7f     (DW_OP_fbreg: -84)
 <2><44a1>: Abbrev Number: 40 (DW_TAG_variable)
  <44a2>     DW_AT_name        : (indirect string, offset: 0xdf): pDmaController
  <44a6>     DW_AT_decl_file   : 1
  <44a7>     DW_AT_decl_line   : 764
  <44a9>     DW_AT_type        : <217e>
  <44ad>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><44b0>: Abbrev Number: 40 (DW_TAG_variable)
  <44b1>     DW_AT_name        : (indirect string, offset: 0x7a): pDmaChannel
  <44b5>     DW_AT_decl_file   : 1
  <44b6>     DW_AT_decl_line   : 765
  <44b8>     DW_AT_type        : <1e59>
  <44bc>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><44bf>: Abbrev Number: 39 (DW_TAG_variable)
  <44c0>     DW_AT_name        : bDmaOk
  <44c7>     DW_AT_decl_file   : 1
  <44c8>     DW_AT_decl_line   : 766
  <44ca>     DW_AT_type        : <176>
  <44ce>     DW_AT_location    : 2 byte block: 91 53    (DW_OP_fbreg: -45)
 <2><44d1>: Abbrev Number: 39 (DW_TAG_variable)
  <44d2>     DW_AT_name        : wCsr
  <44d7>     DW_AT_decl_file   : 1
  <44d8>     DW_AT_decl_line   : 768
  <44da>     DW_AT_type        : <185>
  <44de>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><44e1>: Abbrev Number: 40 (DW_TAG_variable)
  <44e2>     DW_AT_name        : (indirect string, offset: 0x1a5): wRxCount
  <44e6>     DW_AT_decl_file   : 1
  <44e7>     DW_AT_decl_line   : 768
  <44e9>     DW_AT_type        : <185>
  <44ed>     DW_AT_location    : 2 byte block: 91 56    (DW_OP_fbreg: -42)
 <2><44f0>: Abbrev Number: 40 (DW_TAG_variable)
  <44f1>     DW_AT_name        : (indirect string, offset: 0x1d2): wIntrRxE
  <44f5>     DW_AT_decl_file   : 1
  <44f6>     DW_AT_decl_line   : 768
  <44f8>     DW_AT_type        : <185>
  <44fc>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><44ff>: Abbrev Number: 39 (DW_TAG_variable)
  <4500>     DW_AT_name        : qItem
  <4506>     DW_AT_decl_file   : 1
  <4507>     DW_AT_decl_line   : 769
  <4509>     DW_AT_type        : <2290>
  <450d>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><4510>: Abbrev Number: 40 (DW_TAG_variable)
  <4511>     DW_AT_name        : (indirect string, offset: 0x132): bResult
  <4515>     DW_AT_decl_file   : 1
  <4516>     DW_AT_decl_line   : 770
  <4518>     DW_AT_type        : <176>
  <451c>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><451f>: Abbrev Number: 39 (DW_TAG_variable)
  <4520>     DW_AT_name        : reg
  <4524>     DW_AT_decl_file   : 1
  <4525>     DW_AT_decl_line   : 771
  <4527>     DW_AT_type        : <176>
  <452b>     DW_AT_location    : 2 byte block: 91 5b    (DW_OP_fbreg: -37)
 <2><452e>: Abbrev Number: 40 (DW_TAG_variable)
  <452f>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4533>     DW_AT_decl_file   : 1
  <4534>     DW_AT_decl_line   : 772
  <4536>     DW_AT_type        : <390d>
  <453a>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><453d>: Abbrev Number: 40 (DW_TAG_variable)
  <453e>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4542>     DW_AT_decl_file   : 1
  <4543>     DW_AT_decl_line   : 773
  <4545>     DW_AT_type        : <f28>
  <4549>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><454c>: Abbrev Number: 40 (DW_TAG_variable)
  <454d>     DW_AT_name        : (indirect string, offset: 0x187): pServices
  <4551>     DW_AT_decl_file   : 1
  <4552>     DW_AT_decl_line   : 774
  <4554>     DW_AT_type        : <339d>
  <4558>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><455b>: Abbrev Number: 40 (DW_TAG_variable)
  <455c>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4560>     DW_AT_decl_file   : 1
  <4561>     DW_AT_decl_line   : 775
  <4563>     DW_AT_type        : <176>
  <4567>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <1><456b>: Abbrev Number: 37 (DW_TAG_subprogram)
  <456c>     DW_AT_external    : 1
  <456d>     DW_AT_name        : MGC_HdrcStartTx
  <457d>     DW_AT_decl_file   : 1
  <457e>     DW_AT_decl_line   : 1012
  <4580>     DW_AT_prototyped  : 1
  <4581>     DW_AT_type        : <1a4>
  <4585>     DW_AT_low_pc      : 0x1aac
  <4589>     DW_AT_high_pc     : 0x1f7c
  <458d>     DW_AT_frame_base  : 0x1a4  (location list)
  <4591>     DW_AT_sibling     : <46d8>
 <2><4595>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4596>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <459a>     DW_AT_decl_file   : 1
  <459b>     DW_AT_decl_line   : 1009
  <459d>     DW_AT_type        : <3f3a>
  <45a1>     DW_AT_location    : 3 byte block: 91 b4 7f     (DW_OP_fbreg: -76)
 <2><45a5>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <45a6>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <45aa>     DW_AT_decl_file   : 1
  <45ab>     DW_AT_decl_line   : 1009
  <45ad>     DW_AT_type        : <26e0>
  <45b1>     DW_AT_location    : 3 byte block: 91 b0 7f     (DW_OP_fbreg: -80)
 <2><45b5>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <45b6>     DW_AT_name        : (indirect string, offset: 0xb8): pBuffer
  <45ba>     DW_AT_decl_file   : 1
  <45bb>     DW_AT_decl_line   : 1010
  <45bd>     DW_AT_type        : <667>
  <45c1>     DW_AT_location    : 3 byte block: 91 ac 7f     (DW_OP_fbreg: -84)
 <2><45c5>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <45c6>     DW_AT_name        : (indirect string, offset: 0x59): dwTotalBytes
  <45ca>     DW_AT_decl_file   : 1
  <45cb>     DW_AT_decl_line   : 1010
  <45cd>     DW_AT_type        : <1a4>
  <45d1>     DW_AT_location    : 3 byte block: 91 a8 7f     (DW_OP_fbreg: -88)
 <2><45d5>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <45d6>     DW_AT_name        : pGenIrp
  <45de>     DW_AT_decl_file   : 1
  <45df>     DW_AT_decl_line   : 1011
  <45e1>     DW_AT_type        : <f1>
  <45e5>     DW_AT_location    : 2 byte block: 91 0     (DW_OP_fbreg: 0)
 <2><45e8>: Abbrev Number: 40 (DW_TAG_variable)
  <45e9>     DW_AT_name        : (indirect string, offset: 0xdf): pDmaController
  <45ed>     DW_AT_decl_file   : 1
  <45ee>     DW_AT_decl_line   : 1014
  <45f0>     DW_AT_type        : <217e>
  <45f4>     DW_AT_location    : 3 byte block: 91 b8 7f     (DW_OP_fbreg: -72)
 <2><45f8>: Abbrev Number: 40 (DW_TAG_variable)
  <45f9>     DW_AT_name        : (indirect string, offset: 0x7a): pDmaChannel
  <45fd>     DW_AT_decl_file   : 1
  <45fe>     DW_AT_decl_line   : 1015
  <4600>     DW_AT_type        : <1e59>
  <4604>     DW_AT_location    : 3 byte block: 91 bc 7f     (DW_OP_fbreg: -68)
 <2><4608>: Abbrev Number: 39 (DW_TAG_variable)
  <4609>     DW_AT_name        : bDmaOk
  <4610>     DW_AT_decl_file   : 1
  <4611>     DW_AT_decl_line   : 1016
  <4613>     DW_AT_type        : <176>
  <4617>     DW_AT_location    : 2 byte block: 91 43    (DW_OP_fbreg: -61)
 <2><461a>: Abbrev Number: 39 (DW_TAG_variable)
  <461b>     DW_AT_name        : dwLoadCount
  <4627>     DW_AT_decl_file   : 1
  <4628>     DW_AT_decl_line   : 1018
  <462a>     DW_AT_type        : <1a4>
  <462e>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4631>: Abbrev Number: 39 (DW_TAG_variable)
  <4632>     DW_AT_name        : wCsr
  <4637>     DW_AT_decl_file   : 1
  <4638>     DW_AT_decl_line   : 1019
  <463a>     DW_AT_type        : <185>
  <463e>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4641>: Abbrev Number: 39 (DW_TAG_variable)
  <4642>     DW_AT_name        : reg
  <4646>     DW_AT_decl_file   : 1
  <4647>     DW_AT_decl_line   : 1020
  <4649>     DW_AT_type        : <176>
  <464d>     DW_AT_location    : 2 byte block: 91 4b    (DW_OP_fbreg: -53)
 <2><4650>: Abbrev Number: 40 (DW_TAG_variable)
  <4651>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4655>     DW_AT_decl_file   : 1
  <4656>     DW_AT_decl_line   : 1021
  <4658>     DW_AT_type        : <390d>
  <465c>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><465f>: Abbrev Number: 40 (DW_TAG_variable)
  <4660>     DW_AT_name        : (indirect string, offset: 0x17d): bAllowDma
  <4664>     DW_AT_decl_file   : 1
  <4665>     DW_AT_decl_line   : 1023
  <4667>     DW_AT_type        : <176>
  <466b>     DW_AT_location    : 2 byte block: 91 53    (DW_OP_fbreg: -45)
 <2><466e>: Abbrev Number: 39 (DW_TAG_variable)
  <466f>     DW_AT_name        : pIrp
  <4674>     DW_AT_decl_file   : 1
  <4675>     DW_AT_decl_line   : 1024
  <4677>     DW_AT_type        : <46d8>
  <467b>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><467e>: Abbrev Number: 39 (DW_TAG_variable)
  <467f>     DW_AT_name        : pIsochIrp
  <4689>     DW_AT_decl_file   : 1
  <468a>     DW_AT_decl_line   : 1025
  <468c>     DW_AT_type        : <46de>
  <4690>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4693>: Abbrev Number: 39 (DW_TAG_variable)
  <4694>     DW_AT_name        : pControlIrp
  <46a0>     DW_AT_decl_file   : 1
  <46a1>     DW_AT_decl_line   : 1026
  <46a3>     DW_AT_type        : <46e4>
  <46a7>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><46aa>: Abbrev Number: 40 (DW_TAG_variable)
  <46ab>     DW_AT_name        : (indirect string, offset: 0x187): pServices
  <46af>     DW_AT_decl_file   : 1
  <46b0>     DW_AT_decl_line   : 1027
  <46b2>     DW_AT_type        : <339d>
  <46b6>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><46b9>: Abbrev Number: 40 (DW_TAG_variable)
  <46ba>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <46be>     DW_AT_decl_file   : 1
  <46bf>     DW_AT_decl_line   : 1029
  <46c1>     DW_AT_type        : <f28>
  <46c5>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><46c8>: Abbrev Number: 40 (DW_TAG_variable)
  <46c9>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <46cd>     DW_AT_decl_file   : 1
  <46ce>     DW_AT_decl_line   : 1030
  <46d0>     DW_AT_type        : <176>
  <46d4>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <1><46d8>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <46d9>     DW_AT_byte_size   : 4
  <46da>     DW_AT_type        : <1288>
 <1><46de>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <46df>     DW_AT_byte_size   : 4
  <46e0>     DW_AT_type        : <13e3>
 <1><46e4>: Abbrev Number: 7 (DW_TAG_pointer_type)
  <46e5>     DW_AT_byte_size   : 4
  <46e6>     DW_AT_type        : <1177>
 <1><46ea>: Abbrev Number: 37 (DW_TAG_subprogram)
  <46eb>     DW_AT_external    : 1
  <46ec>     DW_AT_name        : MGC_HdrcFlushEndpoint
  <4702>     DW_AT_decl_file   : 1
  <4703>     DW_AT_decl_line   : 1162
  <4705>     DW_AT_prototyped  : 1
  <4706>     DW_AT_type        : <1a4>
  <470a>     DW_AT_low_pc      : 0x1f7c
  <470e>     DW_AT_high_pc     : 0x2314
  <4712>     DW_AT_frame_base  : 0x1ce  (location list)
  <4716>     DW_AT_sibling     : <47a7>
 <2><471a>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <471b>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <471f>     DW_AT_decl_file   : 1
  <4720>     DW_AT_decl_line   : 1160
  <4722>     DW_AT_type        : <3f3a>
  <4726>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4729>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <472a>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <472e>     DW_AT_decl_file   : 1
  <472f>     DW_AT_decl_line   : 1160
  <4731>     DW_AT_type        : <26e0>
  <4735>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4738>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4739>     DW_AT_name        : bDir
  <473e>     DW_AT_decl_file   : 1
  <473f>     DW_AT_decl_line   : 1161
  <4741>     DW_AT_type        : <176>
  <4745>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4748>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4749>     DW_AT_name        : bReuse
  <4750>     DW_AT_decl_file   : 1
  <4751>     DW_AT_decl_line   : 1161
  <4753>     DW_AT_type        : <176>
  <4757>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><475a>: Abbrev Number: 39 (DW_TAG_variable)
  <475b>     DW_AT_name        : wCsr
  <4760>     DW_AT_decl_file   : 1
  <4761>     DW_AT_decl_line   : 1163
  <4763>     DW_AT_type        : <185>
  <4767>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><476a>: Abbrev Number: 40 (DW_TAG_variable)
  <476b>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <476f>     DW_AT_decl_file   : 1
  <4770>     DW_AT_decl_line   : 1164
  <4772>     DW_AT_type        : <390d>
  <4776>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4779>: Abbrev Number: 40 (DW_TAG_variable)
  <477a>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <477e>     DW_AT_decl_file   : 1
  <477f>     DW_AT_decl_line   : 1165
  <4781>     DW_AT_type        : <f28>
  <4785>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4788>: Abbrev Number: 39 (DW_TAG_variable)
  <4789>     DW_AT_name        : bTx
  <478d>     DW_AT_decl_file   : 1
  <478e>     DW_AT_decl_line   : 1166
  <4790>     DW_AT_type        : <176>
  <4794>     DW_AT_location    : 2 byte block: 91 6e    (DW_OP_fbreg: -18)
 <2><4797>: Abbrev Number: 40 (DW_TAG_variable)
  <4798>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <479c>     DW_AT_decl_file   : 1
  <479d>     DW_AT_decl_line   : 1167
  <479f>     DW_AT_type        : <176>
  <47a3>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><47a7>: Abbrev Number: 37 (DW_TAG_subprogram)
  <47a8>     DW_AT_external    : 1
  <47a9>     DW_AT_name        : MGC_HdrcHaltEndpoint
  <47be>     DW_AT_decl_file   : 1
  <47bf>     DW_AT_decl_line   : 1249
  <47c1>     DW_AT_prototyped  : 1
  <47c2>     DW_AT_type        : <1a4>
  <47c6>     DW_AT_low_pc      : 0x2314
  <47ca>     DW_AT_high_pc     : 0x25cc
  <47ce>     DW_AT_frame_base  : 0x1f8  (location list)
  <47d2>     DW_AT_sibling     : <4852>
 <2><47d6>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <47d7>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <47db>     DW_AT_decl_file   : 1
  <47dc>     DW_AT_decl_line   : 1247
  <47de>     DW_AT_type        : <3f3a>
  <47e2>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><47e5>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <47e6>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <47ea>     DW_AT_decl_file   : 1
  <47eb>     DW_AT_decl_line   : 1247
  <47ed>     DW_AT_type        : <26e0>
  <47f1>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><47f4>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <47f5>     DW_AT_name        : bDir
  <47fa>     DW_AT_decl_file   : 1
  <47fb>     DW_AT_decl_line   : 1248
  <47fd>     DW_AT_type        : <176>
  <4801>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4804>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4805>     DW_AT_name        : bHalt
  <480b>     DW_AT_decl_file   : 1
  <480c>     DW_AT_decl_line   : 1248
  <480e>     DW_AT_type        : <176>
  <4812>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4815>: Abbrev Number: 39 (DW_TAG_variable)
  <4816>     DW_AT_name        : val
  <481a>     DW_AT_decl_file   : 1
  <481b>     DW_AT_decl_line   : 1250
  <481d>     DW_AT_type        : <185>
  <4821>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4824>: Abbrev Number: 39 (DW_TAG_variable)
  <4825>     DW_AT_name        : bTx
  <4829>     DW_AT_decl_file   : 1
  <482a>     DW_AT_decl_line   : 1251
  <482c>     DW_AT_type        : <176>
  <4830>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4833>: Abbrev Number: 40 (DW_TAG_variable)
  <4834>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4838>     DW_AT_decl_file   : 1
  <4839>     DW_AT_decl_line   : 1252
  <483b>     DW_AT_type        : <390d>
  <483f>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4842>: Abbrev Number: 40 (DW_TAG_variable)
  <4843>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4847>     DW_AT_decl_file   : 1
  <4848>     DW_AT_decl_line   : 1253
  <484a>     DW_AT_type        : <f28>
  <484e>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4852>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4853>     DW_AT_external    : 1
  <4854>     DW_AT_name        : MGC_HdrcDefaultEndResponse
  <486f>     DW_AT_decl_file   : 1
  <4870>     DW_AT_decl_line   : 1369
  <4872>     DW_AT_prototyped  : 1
  <4873>     DW_AT_type        : <1a4>
  <4877>     DW_AT_low_pc      : 0x25cc
  <487b>     DW_AT_high_pc     : 0x281c
  <487f>     DW_AT_frame_base  : 0x222  (location list)
  <4883>     DW_AT_sibling     : <48fb>
 <2><4887>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4888>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <488c>     DW_AT_decl_file   : 1
  <488d>     DW_AT_decl_line   : 1368
  <488f>     DW_AT_type        : <3f3a>
  <4893>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4896>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4897>     DW_AT_name        : bStall
  <489e>     DW_AT_decl_file   : 1
  <489f>     DW_AT_decl_line   : 1368
  <48a1>     DW_AT_type        : <176>
  <48a5>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><48a8>: Abbrev Number: 39 (DW_TAG_variable)
  <48a9>     DW_AT_name        : wSize
  <48af>     DW_AT_decl_file   : 1
  <48b0>     DW_AT_decl_line   : 1370
  <48b2>     DW_AT_type        : <185>
  <48b6>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><48b9>: Abbrev Number: 39 (DW_TAG_variable)
  <48ba>     DW_AT_name        : wCsrVal
  <48c2>     DW_AT_decl_file   : 1
  <48c3>     DW_AT_decl_line   : 1370
  <48c5>     DW_AT_type        : <185>
  <48c9>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><48cc>: Abbrev Number: 39 (DW_TAG_variable)
  <48cd>     DW_AT_name        : bVal
  <48d2>     DW_AT_decl_file   : 1
  <48d3>     DW_AT_decl_line   : 1371
  <48d5>     DW_AT_type        : <176>
  <48d9>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><48dc>: Abbrev Number: 40 (DW_TAG_variable)
  <48dd>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <48e1>     DW_AT_decl_file   : 1
  <48e2>     DW_AT_decl_line   : 1372
  <48e4>     DW_AT_type        : <390d>
  <48e8>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><48eb>: Abbrev Number: 40 (DW_TAG_variable)
  <48ec>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <48f0>     DW_AT_decl_file   : 1
  <48f1>     DW_AT_decl_line   : 1373
  <48f3>     DW_AT_type        : <f28>
  <48f7>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><48fb>: Abbrev Number: 37 (DW_TAG_subprogram)
  <48fc>     DW_AT_external    : 1
  <48fd>     DW_AT_name        : MGC_HdrcServiceDefaultEnd
  <4917>     DW_AT_decl_file   : 1
  <4918>     DW_AT_decl_line   : 1423
  <491a>     DW_AT_prototyped  : 1
  <491b>     DW_AT_type        : <176>
  <491f>     DW_AT_low_pc      : 0x281c
  <4923>     DW_AT_high_pc     : 0x2f70
  <4927>     DW_AT_frame_base  : 0x24c  (location list)
  <492b>     DW_AT_sibling     : <49d5>
 <2><492f>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4930>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4934>     DW_AT_decl_file   : 1
  <4935>     DW_AT_decl_line   : 1422
  <4937>     DW_AT_type        : <3f3a>
  <493b>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><493e>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <493f>     DW_AT_name        : (indirect string, offset: 0x74): pItem
  <4943>     DW_AT_decl_file   : 1
  <4944>     DW_AT_decl_line   : 1422
  <4946>     DW_AT_type        : <35f1>
  <494a>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><494d>: Abbrev Number: 39 (DW_TAG_variable)
  <494e>     DW_AT_name        : wCsrVal
  <4956>     DW_AT_decl_file   : 1
  <4957>     DW_AT_decl_line   : 1424
  <4959>     DW_AT_type        : <185>
  <495d>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4960>: Abbrev Number: 39 (DW_TAG_variable)
  <4961>     DW_AT_name        : wCount
  <4968>     DW_AT_decl_file   : 1
  <4969>     DW_AT_decl_line   : 1424
  <496b>     DW_AT_type        : <185>
  <496f>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><4972>: Abbrev Number: 39 (DW_TAG_variable)
  <4973>     DW_AT_name        : bTestVal
  <497c>     DW_AT_decl_file   : 1
  <497d>     DW_AT_decl_line   : 1429
  <497f>     DW_AT_type        : <176>
  <4983>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4986>: Abbrev Number: 39 (DW_TAG_variable)
  <4987>     DW_AT_name        : bError
  <498e>     DW_AT_decl_file   : 1
  <498f>     DW_AT_decl_line   : 1430
  <4991>     DW_AT_type        : <176>
  <4995>     DW_AT_location    : 2 byte block: 91 65    (DW_OP_fbreg: -27)
 <2><4998>: Abbrev Number: 40 (DW_TAG_variable)
  <4999>     DW_AT_name        : (indirect string, offset: 0x132): bResult
  <499d>     DW_AT_decl_file   : 1
  <499e>     DW_AT_decl_line   : 1431
  <49a0>     DW_AT_type        : <176>
  <49a4>     DW_AT_location    : 2 byte block: 91 66    (DW_OP_fbreg: -26)
 <2><49a7>: Abbrev Number: 40 (DW_TAG_variable)
  <49a8>     DW_AT_name        : (indirect string, offset: 0x13a): status
  <49ac>     DW_AT_decl_file   : 1
  <49ad>     DW_AT_decl_line   : 1432
  <49af>     DW_AT_type        : <176>
  <49b3>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><49b6>: Abbrev Number: 40 (DW_TAG_variable)
  <49b7>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <49bb>     DW_AT_decl_file   : 1
  <49bc>     DW_AT_decl_line   : 1433
  <49be>     DW_AT_type        : <26e0>
  <49c2>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><49c5>: Abbrev Number: 40 (DW_TAG_variable)
  <49c6>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <49ca>     DW_AT_decl_file   : 1
  <49cb>     DW_AT_decl_line   : 1434
  <49cd>     DW_AT_type        : <f28>
  <49d1>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><49d5>: Abbrev Number: 37 (DW_TAG_subprogram)
  <49d6>     DW_AT_external    : 1
  <49d7>     DW_AT_name        : MGC_HdrcServiceTransmitAvail
  <49f4>     DW_AT_decl_file   : 1
  <49f5>     DW_AT_decl_line   : 1581
  <49f7>     DW_AT_prototyped  : 1
  <49f8>     DW_AT_type        : <176>
  <49fc>     DW_AT_low_pc      : 0x2f70
  <4a00>     DW_AT_high_pc     : 0x34a0
  <4a04>     DW_AT_frame_base  : 0x276  (location list)
  <4a08>     DW_AT_sibling     : <4aa4>
 <2><4a0c>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4a0d>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4a11>     DW_AT_decl_file   : 1
  <4a12>     DW_AT_decl_line   : 1580
  <4a14>     DW_AT_type        : <3f3a>
  <4a18>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4a1b>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4a1c>     DW_AT_name        : (indirect string, offset: 0x107): wEndIndex
  <4a20>     DW_AT_decl_file   : 1
  <4a21>     DW_AT_decl_line   : 1580
  <4a23>     DW_AT_type        : <185>
  <4a27>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4a2a>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4a2b>     DW_AT_name        : (indirect string, offset: 0x74): pItem
  <4a2f>     DW_AT_decl_file   : 1
  <4a30>     DW_AT_decl_line   : 1580
  <4a32>     DW_AT_type        : <35f1>
  <4a36>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4a39>: Abbrev Number: 39 (DW_TAG_variable)
  <4a3a>     DW_AT_name        : wVal
  <4a3f>     DW_AT_decl_file   : 1
  <4a40>     DW_AT_decl_line   : 1582
  <4a42>     DW_AT_type        : <185>
  <4a46>     DW_AT_location    : 2 byte block: 91 56    (DW_OP_fbreg: -42)
 <2><4a49>: Abbrev Number: 40 (DW_TAG_variable)
  <4a4a>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <4a4e>     DW_AT_decl_file   : 1
  <4a4f>     DW_AT_decl_line   : 1583
  <4a51>     DW_AT_type        : <26e0>
  <4a55>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4a58>: Abbrev Number: 40 (DW_TAG_variable)
  <4a59>     DW_AT_name        : (indirect string, offset: 0x132): bResult
  <4a5d>     DW_AT_decl_file   : 1
  <4a5e>     DW_AT_decl_line   : 1584
  <4a60>     DW_AT_type        : <176>
  <4a64>     DW_AT_location    : 2 byte block: 91 5f    (DW_OP_fbreg: -33)
 <2><4a67>: Abbrev Number: 40 (DW_TAG_variable)
  <4a68>     DW_AT_name        : (indirect string, offset: 0x13a): status
  <4a6c>     DW_AT_decl_file   : 1
  <4a6d>     DW_AT_decl_line   : 1585
  <4a6f>     DW_AT_type        : <1a4>
  <4a73>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4a76>: Abbrev Number: 40 (DW_TAG_variable)
  <4a77>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4a7b>     DW_AT_decl_file   : 1
  <4a7c>     DW_AT_decl_line   : 1586
  <4a7e>     DW_AT_type        : <176>
  <4a82>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4a85>: Abbrev Number: 40 (DW_TAG_variable)
  <4a86>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4a8a>     DW_AT_decl_file   : 1
  <4a8b>     DW_AT_decl_line   : 1587
  <4a8d>     DW_AT_type        : <390d>
  <4a91>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4a94>: Abbrev Number: 40 (DW_TAG_variable)
  <4a95>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4a99>     DW_AT_decl_file   : 1
  <4a9a>     DW_AT_decl_line   : 1588
  <4a9c>     DW_AT_type        : <f28>
  <4aa0>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4aa4>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4aa5>     DW_AT_external    : 1
  <4aa6>     DW_AT_name        : MGC_HdrcServiceReceiveReady
  <4ac2>     DW_AT_decl_file   : 1
  <4ac3>     DW_AT_decl_line   : 1751
  <4ac5>     DW_AT_prototyped  : 1
  <4ac6>     DW_AT_type        : <176>
  <4aca>     DW_AT_low_pc      : 0x34a0
  <4ace>     DW_AT_high_pc     : 0x3b14
  <4ad2>     DW_AT_frame_base  : 0x2a0  (location list)
  <4ad6>     DW_AT_sibling     : <4b81>
 <2><4ada>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4adb>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4adf>     DW_AT_decl_file   : 1
  <4ae0>     DW_AT_decl_line   : 1750
  <4ae2>     DW_AT_type        : <3f3a>
  <4ae6>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4ae9>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4aea>     DW_AT_name        : (indirect string, offset: 0x107): wEndIndex
  <4aee>     DW_AT_decl_file   : 1
  <4aef>     DW_AT_decl_line   : 1750
  <4af1>     DW_AT_type        : <185>
  <4af5>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4af8>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4af9>     DW_AT_name        : (indirect string, offset: 0x74): pItem
  <4afd>     DW_AT_decl_file   : 1
  <4afe>     DW_AT_decl_line   : 1750
  <4b00>     DW_AT_type        : <35f1>
  <4b04>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4b07>: Abbrev Number: 39 (DW_TAG_variable)
  <4b08>     DW_AT_name        : wVal
  <4b0d>     DW_AT_decl_file   : 1
  <4b0e>     DW_AT_decl_line   : 1752
  <4b10>     DW_AT_type        : <185>
  <4b14>     DW_AT_location    : 2 byte block: 91 5a    (DW_OP_fbreg: -38)
 <2><4b17>: Abbrev Number: 40 (DW_TAG_variable)
  <4b18>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <4b1c>     DW_AT_decl_file   : 1
  <4b1d>     DW_AT_decl_line   : 1753
  <4b1f>     DW_AT_type        : <26e0>
  <4b23>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4b26>: Abbrev Number: 40 (DW_TAG_variable)
  <4b27>     DW_AT_name        : (indirect string, offset: 0x1a5): wRxCount
  <4b2b>     DW_AT_decl_file   : 1
  <4b2c>     DW_AT_decl_line   : 1754
  <4b2e>     DW_AT_type        : <185>
  <4b32>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4b35>: Abbrev Number: 40 (DW_TAG_variable)
  <4b36>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4b3a>     DW_AT_decl_file   : 1
  <4b3b>     DW_AT_decl_line   : 1755
  <4b3d>     DW_AT_type        : <176>
  <4b41>     DW_AT_location    : 2 byte block: 91 62    (DW_OP_fbreg: -30)
 <2><4b44>: Abbrev Number: 40 (DW_TAG_variable)
  <4b45>     DW_AT_name        : (indirect string, offset: 0x132): bResult
  <4b49>     DW_AT_decl_file   : 1
  <4b4a>     DW_AT_decl_line   : 1756
  <4b4c>     DW_AT_type        : <176>
  <4b50>     DW_AT_location    : 2 byte block: 91 63    (DW_OP_fbreg: -29)
 <2><4b53>: Abbrev Number: 40 (DW_TAG_variable)
  <4b54>     DW_AT_name        : (indirect string, offset: 0x13a): status
  <4b58>     DW_AT_decl_file   : 1
  <4b59>     DW_AT_decl_line   : 1757
  <4b5b>     DW_AT_type        : <1a4>
  <4b5f>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4b62>: Abbrev Number: 40 (DW_TAG_variable)
  <4b63>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4b67>     DW_AT_decl_file   : 1
  <4b68>     DW_AT_decl_line   : 1758
  <4b6a>     DW_AT_type        : <390d>
  <4b6e>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4b71>: Abbrev Number: 40 (DW_TAG_variable)
  <4b72>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4b76>     DW_AT_decl_file   : 1
  <4b77>     DW_AT_decl_line   : 1759
  <4b79>     DW_AT_type        : <f28>
  <4b7d>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4b81>: Abbrev Number: 42 (DW_TAG_subprogram)
  <4b82>     DW_AT_name        : MGC_HdrcResetOff
  <4b93>     DW_AT_decl_file   : 1
  <4b94>     DW_AT_decl_line   : 1949
  <4b96>     DW_AT_prototyped  : 1
  <4b97>     DW_AT_low_pc      : 0x3b14
  <4b9b>     DW_AT_high_pc     : 0x3b78
  <4b9f>     DW_AT_frame_base  : 0x2ca  (location list)
  <4ba3>     DW_AT_sibling     : <4bf7>
 <2><4ba7>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4ba8>     DW_AT_name        : (indirect string, offset: 0x1cb): pParam
  <4bac>     DW_AT_decl_file   : 1
  <4bad>     DW_AT_decl_line   : 1948
  <4baf>     DW_AT_type        : <f1>
  <4bb3>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4bb6>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4bb7>     DW_AT_name        : wTimer
  <4bbe>     DW_AT_decl_file   : 1
  <4bbf>     DW_AT_decl_line   : 1948
  <4bc1>     DW_AT_type        : <185>
  <4bc5>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4bc8>: Abbrev Number: 39 (DW_TAG_variable)
  <4bc9>     DW_AT_name        : bReg
  <4bce>     DW_AT_decl_file   : 1
  <4bcf>     DW_AT_decl_line   : 1950
  <4bd1>     DW_AT_type        : <176>
  <4bd5>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4bd8>: Abbrev Number: 40 (DW_TAG_variable)
  <4bd9>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4bdd>     DW_AT_decl_file   : 1
  <4bde>     DW_AT_decl_line   : 1951
  <4be0>     DW_AT_type        : <390d>
  <4be4>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4be7>: Abbrev Number: 40 (DW_TAG_variable)
  <4be8>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4bec>     DW_AT_decl_file   : 1
  <4bed>     DW_AT_decl_line   : 1952
  <4bef>     DW_AT_type        : <f28>
  <4bf3>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4bf7>: Abbrev Number: 42 (DW_TAG_subprogram)
  <4bf8>     DW_AT_name        : MGC_HdrcResumeOff
  <4c0a>     DW_AT_decl_file   : 1
  <4c0b>     DW_AT_decl_line   : 1959
  <4c0d>     DW_AT_prototyped  : 1
  <4c0e>     DW_AT_low_pc      : 0x3b78
  <4c12>     DW_AT_high_pc     : 0x3bdc
  <4c16>     DW_AT_frame_base  : 0x2f4  (location list)
  <4c1a>     DW_AT_sibling     : <4c6e>
 <2><4c1e>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4c1f>     DW_AT_name        : (indirect string, offset: 0x1cb): pParam
  <4c23>     DW_AT_decl_file   : 1
  <4c24>     DW_AT_decl_line   : 1958
  <4c26>     DW_AT_type        : <f1>
  <4c2a>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4c2d>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4c2e>     DW_AT_name        : wTimer
  <4c35>     DW_AT_decl_file   : 1
  <4c36>     DW_AT_decl_line   : 1958
  <4c38>     DW_AT_type        : <185>
  <4c3c>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4c3f>: Abbrev Number: 39 (DW_TAG_variable)
  <4c40>     DW_AT_name        : bReg
  <4c45>     DW_AT_decl_file   : 1
  <4c46>     DW_AT_decl_line   : 1960
  <4c48>     DW_AT_type        : <176>
  <4c4c>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4c4f>: Abbrev Number: 40 (DW_TAG_variable)
  <4c50>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4c54>     DW_AT_decl_file   : 1
  <4c55>     DW_AT_decl_line   : 1961
  <4c57>     DW_AT_type        : <390d>
  <4c5b>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4c5e>: Abbrev Number: 40 (DW_TAG_variable)
  <4c5f>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4c63>     DW_AT_decl_file   : 1
  <4c64>     DW_AT_decl_line   : 1962
  <4c66>     DW_AT_type        : <f28>
  <4c6a>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4c6e>: Abbrev Number: 43 (DW_TAG_subprogram)
  <4c6f>     DW_AT_external    : 1
  <4c70>     DW_AT_name        : MGC_HdrcSetPortTestMode
  <4c88>     DW_AT_decl_file   : 1
  <4c89>     DW_AT_decl_line   : 1970
  <4c8b>     DW_AT_prototyped  : 1
  <4c8c>     DW_AT_low_pc      : 0x3bdc
  <4c90>     DW_AT_high_pc     : 0x3f08
  <4c94>     DW_AT_frame_base  : 0x31e  (location list)
  <4c98>     DW_AT_sibling     : <4cfb>
 <2><4c9c>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4c9d>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4ca1>     DW_AT_decl_file   : 1
  <4ca2>     DW_AT_decl_line   : 1969
  <4ca4>     DW_AT_type        : <3f3a>
  <4ca8>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4cab>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4cac>     DW_AT_name        : eMode
  <4cb2>     DW_AT_decl_file   : 1
  <4cb3>     DW_AT_decl_line   : 1969
  <4cb5>     DW_AT_type        : <1c65>
  <4cb9>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4cbc>: Abbrev Number: 39 (DW_TAG_variable)
  <4cbd>     DW_AT_name        : bReg
  <4cc2>     DW_AT_decl_file   : 1
  <4cc3>     DW_AT_decl_line   : 1972
  <4cc5>     DW_AT_type        : <176>
  <4cc9>     DW_AT_location    : 2 byte block: 91 63    (DW_OP_fbreg: -29)
 <2><4ccc>: Abbrev Number: 40 (DW_TAG_variable)
  <4ccd>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4cd1>     DW_AT_decl_file   : 1
  <4cd2>     DW_AT_decl_line   : 1974
  <4cd4>     DW_AT_type        : <390d>
  <4cd8>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4cdb>: Abbrev Number: 40 (DW_TAG_variable)
  <4cdc>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4ce0>     DW_AT_decl_file   : 1
  <4ce1>     DW_AT_decl_line   : 1975
  <4ce3>     DW_AT_type        : <f28>
  <4ce7>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4cea>: Abbrev Number: 39 (DW_TAG_variable)
  <4ceb>     DW_AT_name        : wReg
  <4cf0>     DW_AT_decl_file   : 1
  <4cf1>     DW_AT_decl_line   : 1976
  <4cf3>     DW_AT_type        : <185>
  <4cf7>     DW_AT_location    : 2 byte block: 91 6e    (DW_OP_fbreg: -18)
 <1><4cfb>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4cfc>     DW_AT_external    : 1
  <4cfd>     DW_AT_name        : MGC_HdrcLoadFifo
  <4d0e>     DW_AT_decl_file   : 1
  <4d0f>     DW_AT_decl_line   : 2044
  <4d11>     DW_AT_prototyped  : 1
  <4d12>     DW_AT_type        : <176>
  <4d16>     DW_AT_low_pc      : 0x3f08
  <4d1a>     DW_AT_high_pc     : 0x403c
  <4d1e>     DW_AT_frame_base  : 0x348  (location list)
  <4d22>     DW_AT_sibling     : <4dd8>
 <2><4d26>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4d27>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4d2b>     DW_AT_decl_file   : 1
  <4d2c>     DW_AT_decl_line   : 2043
  <4d2e>     DW_AT_type        : <3f3a>
  <4d32>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4d35>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4d36>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4d3a>     DW_AT_decl_file   : 1
  <4d3b>     DW_AT_decl_line   : 2043
  <4d3d>     DW_AT_type        : <176>
  <4d41>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4d44>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4d45>     DW_AT_name        : (indirect string, offset: 0x0): dwCount
  <4d49>     DW_AT_decl_file   : 1
  <4d4a>     DW_AT_decl_line   : 2043
  <4d4c>     DW_AT_type        : <1a4>
  <4d50>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4d53>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4d54>     DW_AT_name        : pSource
  <4d5c>     DW_AT_decl_file   : 1
  <4d5d>     DW_AT_decl_line   : 2043
  <4d5f>     DW_AT_type        : <667>
  <4d63>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4d66>: Abbrev Number: 39 (DW_TAG_variable)
  <4d67>     DW_AT_name        : dwIndex
  <4d6f>     DW_AT_decl_file   : 1
  <4d70>     DW_AT_decl_line   : 2045
  <4d72>     DW_AT_type        : <1a4>
  <4d76>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4d79>: Abbrev Number: 40 (DW_TAG_variable)
  <4d7a>     DW_AT_name        : (indirect string, offset: 0x152): dwIndex32
  <4d7e>     DW_AT_decl_file   : 1
  <4d7f>     DW_AT_decl_line   : 2045
  <4d81>     DW_AT_type        : <1a4>
  <4d85>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4d88>: Abbrev Number: 39 (DW_TAG_variable)
  <4d89>     DW_AT_name        : dwDatum
  <4d91>     DW_AT_decl_file   : 1
  <4d92>     DW_AT_decl_line   : 2046
  <4d94>     DW_AT_type        : <1a4>
  <4d98>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4d9b>: Abbrev Number: 40 (DW_TAG_variable)
  <4d9c>     DW_AT_name        : (indirect string, offset: 0x148): dwCount32
  <4da0>     DW_AT_decl_file   : 1
  <4da1>     DW_AT_decl_line   : 2047
  <4da3>     DW_AT_type        : <1a4>
  <4da7>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4daa>: Abbrev Number: 40 (DW_TAG_variable)
  <4dab>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4daf>     DW_AT_decl_file   : 1
  <4db0>     DW_AT_decl_line   : 2048
  <4db2>     DW_AT_type        : <390d>
  <4db6>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4db9>: Abbrev Number: 40 (DW_TAG_variable)
  <4dba>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4dbe>     DW_AT_decl_file   : 1
  <4dbf>     DW_AT_decl_line   : 2049
  <4dc1>     DW_AT_type        : <f28>
  <4dc5>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4dc8>: Abbrev Number: 40 (DW_TAG_variable)
  <4dc9>     DW_AT_name        : (indirect string, offset: 0xa6): bFifoOffset
  <4dcd>     DW_AT_decl_file   : 1
  <4dce>     DW_AT_decl_line   : 2050
  <4dd0>     DW_AT_type        : <176>
  <4dd4>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><4dd8>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4dd9>     DW_AT_external    : 1
  <4dda>     DW_AT_name        : MGC_HdrcUnloadFifo
  <4ded>     DW_AT_decl_file   : 1
  <4dee>     DW_AT_decl_line   : 2067
  <4df0>     DW_AT_prototyped  : 1
  <4df1>     DW_AT_type        : <176>
  <4df5>     DW_AT_low_pc      : 0x403c
  <4df9>     DW_AT_high_pc     : 0x4174
  <4dfd>     DW_AT_frame_base  : 0x372  (location list)
  <4e01>     DW_AT_sibling     : <4ea2>
 <2><4e05>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4e06>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4e0a>     DW_AT_decl_file   : 1
  <4e0b>     DW_AT_decl_line   : 2066
  <4e0d>     DW_AT_type        : <3f3a>
  <4e11>     DW_AT_location    : 2 byte block: 91 50    (DW_OP_fbreg: -48)
 <2><4e14>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4e15>     DW_AT_name        : (indirect string, offset: 0x1c6): bEnd
  <4e19>     DW_AT_decl_file   : 1
  <4e1a>     DW_AT_decl_line   : 2066
  <4e1c>     DW_AT_type        : <176>
  <4e20>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4e23>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4e24>     DW_AT_name        : (indirect string, offset: 0x0): dwCount
  <4e28>     DW_AT_decl_file   : 1
  <4e29>     DW_AT_decl_line   : 2066
  <4e2b>     DW_AT_type        : <1a4>
  <4e2f>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4e32>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4e33>     DW_AT_name        : pDest
  <4e39>     DW_AT_decl_file   : 1
  <4e3a>     DW_AT_decl_line   : 2066
  <4e3c>     DW_AT_type        : <f28>
  <4e40>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4e43>: Abbrev Number: 39 (DW_TAG_variable)
  <4e44>     DW_AT_name        : dwIndex
  <4e4c>     DW_AT_decl_file   : 1
  <4e4d>     DW_AT_decl_line   : 2068
  <4e4f>     DW_AT_type        : <1a4>
  <4e53>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4e56>: Abbrev Number: 40 (DW_TAG_variable)
  <4e57>     DW_AT_name        : (indirect string, offset: 0x152): dwIndex32
  <4e5b>     DW_AT_decl_file   : 1
  <4e5c>     DW_AT_decl_line   : 2068
  <4e5e>     DW_AT_type        : <1a4>
  <4e62>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4e65>: Abbrev Number: 40 (DW_TAG_variable)
  <4e66>     DW_AT_name        : (indirect string, offset: 0x148): dwCount32
  <4e6a>     DW_AT_decl_file   : 1
  <4e6b>     DW_AT_decl_line   : 2069
  <4e6d>     DW_AT_type        : <1a4>
  <4e71>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4e74>: Abbrev Number: 40 (DW_TAG_variable)
  <4e75>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4e79>     DW_AT_decl_file   : 1
  <4e7a>     DW_AT_decl_line   : 2070
  <4e7c>     DW_AT_type        : <390d>
  <4e80>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4e83>: Abbrev Number: 40 (DW_TAG_variable)
  <4e84>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4e88>     DW_AT_decl_file   : 1
  <4e89>     DW_AT_decl_line   : 2071
  <4e8b>     DW_AT_type        : <f28>
  <4e8f>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><4e92>: Abbrev Number: 40 (DW_TAG_variable)
  <4e93>     DW_AT_name        : (indirect string, offset: 0xa6): bFifoOffset
  <4e97>     DW_AT_decl_file   : 1
  <4e98>     DW_AT_decl_line   : 2072
  <4e9a>     DW_AT_type        : <176>
  <4e9e>     DW_AT_location    : 2 byte block: 91 6b    (DW_OP_fbreg: -21)
 <1><4ea2>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4ea3>     DW_AT_external    : 1
  <4ea4>     DW_AT_name        : MGC_HdrcUlpiVbusControl
  <4ebc>     DW_AT_decl_file   : 1
  <4ebd>     DW_AT_decl_line   : 2088
  <4ebf>     DW_AT_prototyped  : 1
  <4ec0>     DW_AT_type        : <176>
  <4ec4>     DW_AT_low_pc      : 0x4174
  <4ec8>     DW_AT_high_pc     : 0x4254
  <4ecc>     DW_AT_frame_base  : 0x39c  (location list)
  <4ed0>     DW_AT_sibling     : <4f41>
 <2><4ed4>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4ed5>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4ed9>     DW_AT_decl_file   : 1
  <4eda>     DW_AT_decl_line   : 2087
  <4edc>     DW_AT_type        : <3f3a>
  <4ee0>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><4ee3>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4ee4>     DW_AT_name        : bExtSource
  <4eef>     DW_AT_decl_file   : 1
  <4ef0>     DW_AT_decl_line   : 2087
  <4ef2>     DW_AT_type        : <176>
  <4ef6>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><4ef9>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4efa>     DW_AT_name        : bExtIndicator
  <4f08>     DW_AT_decl_file   : 1
  <4f09>     DW_AT_decl_line   : 2087
  <4f0b>     DW_AT_type        : <176>
  <4f0f>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4f12>: Abbrev Number: 39 (DW_TAG_variable)
  <4f13>     DW_AT_name        : bVal
  <4f18>     DW_AT_decl_file   : 1
  <4f19>     DW_AT_decl_line   : 2089
  <4f1b>     DW_AT_type        : <176>
  <4f1f>     DW_AT_location    : 2 byte block: 91 67    (DW_OP_fbreg: -25)
 <2><4f22>: Abbrev Number: 40 (DW_TAG_variable)
  <4f23>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4f27>     DW_AT_decl_file   : 1
  <4f28>     DW_AT_decl_line   : 2090
  <4f2a>     DW_AT_type        : <390d>
  <4f2e>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><4f31>: Abbrev Number: 40 (DW_TAG_variable)
  <4f32>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4f36>     DW_AT_decl_file   : 1
  <4f37>     DW_AT_decl_line   : 2091
  <4f39>     DW_AT_type        : <f28>
  <4f3d>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><4f41>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4f42>     DW_AT_external    : 1
  <4f43>     DW_AT_name        : MGC_HdrcReadUlpiReg
  <4f57>     DW_AT_decl_file   : 1
  <4f58>     DW_AT_decl_line   : 2106
  <4f5a>     DW_AT_prototyped  : 1
  <4f5b>     DW_AT_type        : <176>
  <4f5f>     DW_AT_low_pc      : 0x4254
  <4f63>     DW_AT_high_pc     : 0x43c8
  <4f67>     DW_AT_frame_base  : 0x3c6  (location list)
  <4f6b>     DW_AT_sibling     : <4fdf>
 <2><4f6f>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <4f70>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <4f74>     DW_AT_decl_file   : 1
  <4f75>     DW_AT_decl_line   : 2105
  <4f77>     DW_AT_type        : <3f3a>
  <4f7b>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><4f7e>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4f7f>     DW_AT_name        : bAddr
  <4f85>     DW_AT_decl_file   : 1
  <4f86>     DW_AT_decl_line   : 2105
  <4f88>     DW_AT_type        : <176>
  <4f8c>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><4f8f>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <4f90>     DW_AT_name        : pbData
  <4f97>     DW_AT_decl_file   : 1
  <4f98>     DW_AT_decl_line   : 2105
  <4f9a>     DW_AT_type        : <f28>
  <4f9e>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><4fa1>: Abbrev Number: 39 (DW_TAG_variable)
  <4fa2>     DW_AT_name        : bCtl
  <4fa7>     DW_AT_decl_file   : 1
  <4fa8>     DW_AT_decl_line   : 2107
  <4faa>     DW_AT_type        : <176>
  <4fae>     DW_AT_location    : 2 byte block: 91 53    (DW_OP_fbreg: -45)
 <2><4fb1>: Abbrev Number: 40 (DW_TAG_variable)
  <4fb2>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <4fb6>     DW_AT_decl_file   : 1
  <4fb7>     DW_AT_decl_line   : 2108
  <4fb9>     DW_AT_type        : <390d>
  <4fbd>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><4fc0>: Abbrev Number: 40 (DW_TAG_variable)
  <4fc1>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <4fc5>     DW_AT_decl_file   : 1
  <4fc6>     DW_AT_decl_line   : 2109
  <4fc8>     DW_AT_type        : <f28>
  <4fcc>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><4fcf>: Abbrev Number: 40 (DW_TAG_variable)
  <4fd0>     DW_AT_name        : (indirect string, offset: 0x127): start_time
  <4fd4>     DW_AT_decl_file   : 1
  <4fd5>     DW_AT_decl_line   : 2111
  <4fd7>     DW_AT_type        : <1b4>
  <4fdb>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <1><4fdf>: Abbrev Number: 37 (DW_TAG_subprogram)
  <4fe0>     DW_AT_external    : 1
  <4fe1>     DW_AT_name        : MGC_HdrcWriteUlpiReg
  <4ff6>     DW_AT_decl_file   : 1
  <4ff7>     DW_AT_decl_line   : 2143
  <4ff9>     DW_AT_prototyped  : 1
  <4ffa>     DW_AT_type        : <176>
  <4ffe>     DW_AT_low_pc      : 0x43c8
  <5002>     DW_AT_high_pc     : 0x453c
  <5006>     DW_AT_frame_base  : 0x3f0  (location list)
  <500a>     DW_AT_sibling     : <507d>
 <2><500e>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <500f>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <5013>     DW_AT_decl_file   : 1
  <5014>     DW_AT_decl_line   : 2142
  <5016>     DW_AT_type        : <3f3a>
  <501a>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><501d>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <501e>     DW_AT_name        : bAddr
  <5024>     DW_AT_decl_file   : 1
  <5025>     DW_AT_decl_line   : 2142
  <5027>     DW_AT_type        : <176>
  <502b>     DW_AT_location    : 2 byte block: 91 48    (DW_OP_fbreg: -56)
 <2><502e>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <502f>     DW_AT_name        : bData
  <5035>     DW_AT_decl_file   : 1
  <5036>     DW_AT_decl_line   : 2142
  <5038>     DW_AT_type        : <176>
  <503c>     DW_AT_location    : 2 byte block: 91 44    (DW_OP_fbreg: -60)
 <2><503f>: Abbrev Number: 39 (DW_TAG_variable)
  <5040>     DW_AT_name        : bCtl
  <5045>     DW_AT_decl_file   : 1
  <5046>     DW_AT_decl_line   : 2144
  <5048>     DW_AT_type        : <176>
  <504c>     DW_AT_location    : 2 byte block: 91 53    (DW_OP_fbreg: -45)
 <2><504f>: Abbrev Number: 40 (DW_TAG_variable)
  <5050>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <5054>     DW_AT_decl_file   : 1
  <5055>     DW_AT_decl_line   : 2145
  <5057>     DW_AT_type        : <390d>
  <505b>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><505e>: Abbrev Number: 40 (DW_TAG_variable)
  <505f>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <5063>     DW_AT_decl_file   : 1
  <5064>     DW_AT_decl_line   : 2146
  <5066>     DW_AT_type        : <f28>
  <506a>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><506d>: Abbrev Number: 40 (DW_TAG_variable)
  <506e>     DW_AT_name        : (indirect string, offset: 0x127): start_time
  <5072>     DW_AT_decl_file   : 1
  <5073>     DW_AT_decl_line   : 2148
  <5075>     DW_AT_type        : <1b4>
  <5079>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <1><507d>: Abbrev Number: 37 (DW_TAG_subprogram)
  <507e>     DW_AT_external    : 1
  <507f>     DW_AT_name        : MGC_HdrcDmaChannelStatusChanged
  <509f>     DW_AT_decl_file   : 1
  <50a0>     DW_AT_decl_line   : 2180
  <50a2>     DW_AT_prototyped  : 1
  <50a3>     DW_AT_type        : <176>
  <50a7>     DW_AT_low_pc      : 0x453c
  <50ab>     DW_AT_high_pc     : 0x48e4
  <50af>     DW_AT_frame_base  : 0x41a  (location list)
  <50b3>     DW_AT_sibling     : <51a8>
 <2><50b7>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <50b8>     DW_AT_name        : (indirect string, offset: 0xee): pPrivateData
  <50bc>     DW_AT_decl_file   : 1
  <50bd>     DW_AT_decl_line   : 2179
  <50bf>     DW_AT_type        : <f1>
  <50c3>     DW_AT_location    : 3 byte block: 91 bc 7f     (DW_OP_fbreg: -68)
 <2><50c7>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <50c8>     DW_AT_name        : (indirect string, offset: 0x111): bLocalEnd
  <50cc>     DW_AT_decl_file   : 1
  <50cd>     DW_AT_decl_line   : 2179
  <50cf>     DW_AT_type        : <176>
  <50d3>     DW_AT_location    : 3 byte block: 91 b8 7f     (DW_OP_fbreg: -72)
 <2><50d7>: Abbrev Number: 41 (DW_TAG_formal_parameter)
  <50d8>     DW_AT_name        : bTransmit
  <50e2>     DW_AT_decl_file   : 1
  <50e3>     DW_AT_decl_line   : 2179
  <50e5>     DW_AT_type        : <176>
  <50e9>     DW_AT_location    : 3 byte block: 91 b4 7f     (DW_OP_fbreg: -76)
 <2><50ed>: Abbrev Number: 39 (DW_TAG_variable)
  <50ee>     DW_AT_name        : qItem
  <50f4>     DW_AT_decl_file   : 1
  <50f5>     DW_AT_decl_line   : 2182
  <50f7>     DW_AT_type        : <2290>
  <50fb>     DW_AT_location    : 2 byte block: 91 40    (DW_OP_fbreg: -64)
 <2><50fe>: Abbrev Number: 39 (DW_TAG_variable)
  <50ff>     DW_AT_name        : bQueue
  <5106>     DW_AT_decl_file   : 1
  <5107>     DW_AT_decl_line   : 2183
  <5109>     DW_AT_type        : <176>
  <510d>     DW_AT_location    : 2 byte block: 91 4b    (DW_OP_fbreg: -53)
 <2><5110>: Abbrev Number: 40 (DW_TAG_variable)
  <5111>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <5115>     DW_AT_decl_file   : 1
  <5116>     DW_AT_decl_line   : 2184
  <5118>     DW_AT_type        : <26e0>
  <511c>     DW_AT_location    : 2 byte block: 91 4c    (DW_OP_fbreg: -52)
 <2><511f>: Abbrev Number: 39 (DW_TAG_variable)
  <5120>     DW_AT_name        : wVal
  <5125>     DW_AT_decl_file   : 1
  <5126>     DW_AT_decl_line   : 2185
  <5128>     DW_AT_type        : <185>
  <512c>     DW_AT_location    : 2 byte block: 91 52    (DW_OP_fbreg: -46)
 <2><512f>: Abbrev Number: 40 (DW_TAG_variable)
  <5130>     DW_AT_name        : (indirect string, offset: 0x1d2): wIntrRxE
  <5134>     DW_AT_decl_file   : 1
  <5135>     DW_AT_decl_line   : 2185
  <5137>     DW_AT_type        : <185>
  <513b>     DW_AT_location    : 2 byte block: 91 54    (DW_OP_fbreg: -44)
 <2><513e>: Abbrev Number: 40 (DW_TAG_variable)
  <513f>     DW_AT_name        : (indirect string, offset: 0x1a5): wRxCount
  <5143>     DW_AT_decl_file   : 1
  <5144>     DW_AT_decl_line   : 2186
  <5146>     DW_AT_type        : <185>
  <514a>     DW_AT_location    : 2 byte block: 91 56    (DW_OP_fbreg: -42)
 <2><514d>: Abbrev Number: 40 (DW_TAG_variable)
  <514e>     DW_AT_name        : (indirect string, offset: 0x13a): status
  <5152>     DW_AT_decl_file   : 1
  <5153>     DW_AT_decl_line   : 2187
  <5155>     DW_AT_type        : <1a4>
  <5159>     DW_AT_location    : 2 byte block: 91 58    (DW_OP_fbreg: -40)
 <2><515c>: Abbrev Number: 40 (DW_TAG_variable)
  <515d>     DW_AT_name        : (indirect string, offset: 0x4a): pPort
  <5161>     DW_AT_decl_file   : 1
  <5162>     DW_AT_decl_line   : 2188
  <5164>     DW_AT_type        : <3f3a>
  <5168>     DW_AT_location    : 2 byte block: 91 5c    (DW_OP_fbreg: -36)
 <2><516b>: Abbrev Number: 40 (DW_TAG_variable)
  <516c>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <5170>     DW_AT_decl_file   : 1
  <5171>     DW_AT_decl_line   : 2189
  <5173>     DW_AT_type        : <390d>
  <5177>     DW_AT_location    : 2 byte block: 91 60    (DW_OP_fbreg: -32)
 <2><517a>: Abbrev Number: 40 (DW_TAG_variable)
  <517b>     DW_AT_name        : (indirect string, offset: 0x187): pServices
  <517f>     DW_AT_decl_file   : 1
  <5180>     DW_AT_decl_line   : 2190
  <5182>     DW_AT_type        : <339d>
  <5186>     DW_AT_location    : 2 byte block: 91 64    (DW_OP_fbreg: -28)
 <2><5189>: Abbrev Number: 40 (DW_TAG_variable)
  <518a>     DW_AT_name        : (indirect string, offset: 0xb2): pBase
  <518e>     DW_AT_decl_file   : 1
  <518f>     DW_AT_decl_line   : 2191
  <5191>     DW_AT_type        : <f28>
  <5195>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <2><5198>: Abbrev Number: 40 (DW_TAG_variable)
  <5199>     DW_AT_name        : (indirect string, offset: 0x141): bIndex
  <519d>     DW_AT_decl_file   : 1
  <519e>     DW_AT_decl_line   : 2193
  <51a0>     DW_AT_type        : <176>
  <51a4>     DW_AT_location    : 2 byte block: 91 6f    (DW_OP_fbreg: -17)
 <1><51a8>: Abbrev Number: 37 (DW_TAG_subprogram)
  <51a9>     DW_AT_external    : 1
  <51aa>     DW_AT_name        : MGC_HdrcDumpState
  <51bc>     DW_AT_decl_file   : 1
  <51bd>     DW_AT_decl_line   : 2289
  <51bf>     DW_AT_prototyped  : 1
  <51c0>     DW_AT_type        : <a9>
  <51c4>     DW_AT_low_pc      : 0x48e4
  <51c8>     DW_AT_high_pc     : 0x4904
  <51cc>     DW_AT_frame_base  : 0x444  (location list)
  <51d0>     DW_AT_sibling     : <51e4>
 <2><51d4>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <51d5>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <51d9>     DW_AT_decl_file   : 1
  <51da>     DW_AT_decl_line   : 2288
  <51dc>     DW_AT_type        : <390d>
  <51e0>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <1><51e4>: Abbrev Number: 37 (DW_TAG_subprogram)
  <51e5>     DW_AT_external    : 1
  <51e6>     DW_AT_name        : MGC_HdrcDumpEndpoint
  <51fb>     DW_AT_decl_file   : 1
  <51fc>     DW_AT_decl_line   : 2298
  <51fe>     DW_AT_prototyped  : 1
  <51ff>     DW_AT_type        : <a9>
  <5203>     DW_AT_low_pc      : 0x4904
  <5207>     DW_AT_high_pc     : 0x492c
  <520b>     DW_AT_frame_base  : 0x46e  (location list)
  <520f>     DW_AT_sibling     : <5232>
 <2><5213>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <5214>     DW_AT_name        : (indirect string, offset: 0xfb): pController
  <5218>     DW_AT_decl_file   : 1
  <5219>     DW_AT_decl_line   : 2297
  <521b>     DW_AT_type        : <390d>
  <521f>     DW_AT_location    : 2 byte block: 91 6c    (DW_OP_fbreg: -20)
 <2><5222>: Abbrev Number: 38 (DW_TAG_formal_parameter)
  <5223>     DW_AT_name        : (indirect string, offset: 0xda): pEnd
  <5227>     DW_AT_decl_file   : 1
  <5228>     DW_AT_decl_line   : 2297
  <522a>     DW_AT_type        : <3391>
  <522e>     DW_AT_location    : 2 byte block: 91 68    (DW_OP_fbreg: -24)
 <1><5232>: Abbrev Number: 28 (DW_TAG_array_type)
  <5233>     DW_AT_type        : <176>
  <5237>     DW_AT_sibling     : <5242>
 <2><523b>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <523c>     DW_AT_type        : <dd>
  <5240>     DW_AT_upper_bound : 52
 <1><5242>: Abbrev Number: 33 (DW_TAG_variable)
  <5243>     DW_AT_name        : MGC_aTestPacket
  <5253>     DW_AT_decl_file   : 1
  <5254>     DW_AT_decl_line   : 47
  <5255>     DW_AT_type        : <525f>
  <5259>     DW_AT_location    : 5 byte block: 3 0 0 0 0    (DW_OP_addr: 0)
 <1><525f>: Abbrev Number: 8 (DW_TAG_const_type)
  <5260>     DW_AT_type        : <5232>
 <1><5264>: Abbrev Number: 28 (DW_TAG_array_type)
  <5265>     DW_AT_type        : <10e>
  <5269>     DW_AT_sibling     : <5274>
 <2><526d>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <526e>     DW_AT_type        : <dd>
  <5272>     DW_AT_upper_bound : 255
 <1><5274>: Abbrev Number: 44 (DW_TAG_variable)
  <5275>     DW_AT_name        : MGC_DiagMsg
  <5281>     DW_AT_decl_file   : 10
  <5282>     DW_AT_decl_line   : 66
  <5283>     DW_AT_type        : <5264>
  <5287>     DW_AT_external    : 1
  <5288>     DW_AT_declaration : 1
 <1><5289>: Abbrev Number: 28 (DW_TAG_array_type)
  <528a>     DW_AT_type        : <10e>
  <528e>     DW_AT_sibling     : <5299>
 <2><5292>: Abbrev Number: 29 (DW_TAG_subrange_type)
  <5293>     DW_AT_type        : <dd>
  <5297>     DW_AT_upper_bound : 32
 <1><5299>: Abbrev Number: 44 (DW_TAG_variable)
  <529a>     DW_AT_name        : MGC_DiagNumber
  <52a9>     DW_AT_decl_file   : 10
  <52aa>     DW_AT_decl_line   : 68
  <52ab>     DW_AT_type        : <5289>
  <52af>     DW_AT_external    : 1
  <52b0>     DW_AT_declaration : 1
 <1><52b1>: Abbrev Number: 44 (DW_TAG_variable)
  <52b2>     DW_AT_name        : MGC_bDiagLevel
  <52c1>     DW_AT_decl_file   : 10
  <52c2>     DW_AT_decl_line   : 69
  <52c3>     DW_AT_type        : <176>
  <52c7>     DW_AT_external    : 1
  <52c8>     DW_AT_declaration : 1
 <1><52c9>: Abbrev Number: 45 (DW_TAG_variable)
  <52ca>     DW_AT_name        : FAPI_SYS_Services
  <52dc>     DW_AT_decl_file   : 12
  <52dd>     DW_AT_decl_line   : 1555
  <52df>     DW_AT_type        : <3eb1>
  <52e3>     DW_AT_external    : 1
  <52e4>     DW_AT_declaration : 1


Dump of debug contents of section .debug_line:

  Length:                      1401
  DWARF Version:               2
  Prologue Length:             324
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
  ../../include
  ../../../../../install/MB86H60/include/fapi

 The File Name Table:
  Entry Dir Time    Size    Name
  1 1   0   0   mu_hdr.c
  2 2   0   0   stddef.h
  3 3   0   0   stdint.h
  4 4   0   0   mu_dsi.h
  5 4   0   0   mu_dev.h
  6 4   0   0   mu_cdi.h
  7 4   0   0   mu_list.h
  8 4   0   0   mu_hset.h
  9 4   0   0   mu_dci.h
  10    1   0   0   mu_impl.h
  11    5   0   0   sys_driver.h
  12    5   0   0   sys_services.h

 Line Number Statements:
  Extended opcode 2: set Address to 0x0
  Advance Line by 141 to 142
  Copy
  Special opcode 151: advance Address by 20 to 0x14 and Line by 6 to 148
  Special opcode 90: advance Address by 12 to 0x20 and Line by 1 to 149
  Advance Line by 15 to 164
  Special opcode 89: advance Address by 12 to 0x2c and Line by 0 to 164
  Special opcode 118: advance Address by 16 to 0x3c and Line by 1 to 165
  Special opcode 146: advance Address by 20 to 0x50 and Line by 1 to 166
  Special opcode 119: advance Address by 16 to 0x60 and Line by 2 to 168
  Special opcode 91: advance Address by 12 to 0x6c and Line by 2 to 170
  Special opcode 119: advance Address by 16 to 0x7c and Line by 2 to 172
  Special opcode 91: advance Address by 12 to 0x88 and Line by 2 to 174
  Special opcode 119: advance Address by 16 to 0x98 and Line by 2 to 176
  Special opcode 91: advance Address by 12 to 0xa4 and Line by 2 to 178
  Special opcode 119: advance Address by 16 to 0xb4 and Line by 2 to 180
  Advance Line by 50 to 230
  Special opcode 89: advance Address by 12 to 0xc0 and Line by 0 to 230
  Special opcode 119: advance Address by 16 to 0xd0 and Line by 2 to 232
  Advance PC by constant 34 to 0xf2
  Special opcode 132: advance Address by 18 to 0x104 and Line by 1 to 233
  Special opcode 91: advance Address by 12 to 0x110 and Line by 2 to 235
  Special opcode 147: advance Address by 20 to 0x124 and Line by 2 to 237
  Advance Line by 9 to 246
  Special opcode 145: advance Address by 20 to 0x138 and Line by 0 to 246
  Advance Line by 48 to 294
  Advance PC by constant 34 to 0x15a
  Special opcode 103: advance Address by 14 to 0x168 and Line by 0 to 294
  Advance Line by 19 to 313
  Special opcode 61: advance Address by 8 to 0x170 and Line by 0 to 313
  Special opcode 149: advance Address by 20 to 0x184 and Line by 4 to 317
  Special opcode 62: advance Address by 8 to 0x18c and Line by 1 to 318
  Special opcode 95: advance Address by 12 to 0x198 and Line by 6 to 324
  Special opcode 91: advance Address by 12 to 0x1a4 and Line by 2 to 326
  Special opcode 148: advance Address by 20 to 0x1b8 and Line by 3 to 329
  Special opcode 146: advance Address by 20 to 0x1cc and Line by 1 to 330
  Special opcode 146: advance Address by 20 to 0x1e0 and Line by 1 to 331
  Special opcode 148: advance Address by 20 to 0x1f4 and Line by 3 to 334
  Advance Line by 15 to 349
  Advance PC by constant 34 to 0x216
  Special opcode 19: advance Address by 2 to 0x218 and Line by 0 to 349
  Special opcode 147: advance Address by 20 to 0x22c and Line by 2 to 351
  Special opcode 34: advance Address by 4 to 0x230 and Line by 1 to 352
  Special opcode 95: advance Address by 12 to 0x23c and Line by 6 to 358
  Special opcode 147: advance Address by 20 to 0x250 and Line by 2 to 360
  Special opcode 90: advance Address by 12 to 0x25c and Line by 1 to 361
  Special opcode 92: advance Address by 12 to 0x268 and Line by 3 to 364
  Special opcode 90: advance Address by 12 to 0x274 and Line by 1 to 365
  Advance PC by 132 to 0x2f8
  Special opcode 6: advance Address by 0 to 0x2f8 and Line by 1 to 366
  Special opcode 146: advance Address by 20 to 0x30c and Line by 1 to 367
  Special opcode 146: advance Address by 20 to 0x320 and Line by 1 to 368
  Advance PC by 132 to 0x3a4
  Special opcode 6: advance Address by 0 to 0x3a4 and Line by 1 to 369
  Special opcode 150: advance Address by 20 to 0x3b8 and Line by 5 to 374
  Special opcode 63: advance Address by 8 to 0x3c0 and Line by 2 to 376
  Advance PC by 136 to 0x448
  Special opcode 6: advance Address by 0 to 0x448 and Line by 1 to 377
  Special opcode 176: advance Address by 24 to 0x460 and Line by 3 to 380
  Special opcode 90: advance Address by 12 to 0x46c and Line by 1 to 381
  Special opcode 34: advance Address by 4 to 0x470 and Line by 1 to 382
  Special opcode 235: advance Address by 32 to 0x490 and Line by 6 to 388
  Special opcode 147: advance Address by 20 to 0x4a4 and Line by 2 to 390
  Special opcode 92: advance Address by 12 to 0x4b0 and Line by 3 to 393
  Special opcode 118: advance Address by 16 to 0x4c0 and Line by 1 to 394
  Special opcode 118: advance Address by 16 to 0x4d0 and Line by 1 to 395
  Special opcode 120: advance Address by 16 to 0x4e0 and Line by 3 to 398
  Special opcode 146: advance Address by 20 to 0x4f4 and Line by 1 to 399
  Special opcode 146: advance Address by 20 to 0x508 and Line by 1 to 400
  Special opcode 147: advance Address by 20 to 0x51c and Line by 2 to 402
  Special opcode 34: advance Address by 4 to 0x520 and Line by 1 to 403
  Special opcode 92: advance Address by 12 to 0x52c and Line by 3 to 406
  Special opcode 146: advance Address by 20 to 0x540 and Line by 1 to 407
  Special opcode 91: advance Address by 12 to 0x54c and Line by 2 to 409
  Special opcode 62: advance Address by 8 to 0x554 and Line by 1 to 410
  Special opcode 119: advance Address by 16 to 0x564 and Line by 2 to 412
  Special opcode 175: advance Address by 24 to 0x57c and Line by 2 to 414
  Special opcode 90: advance Address by 12 to 0x588 and Line by 1 to 415
  Special opcode 121: advance Address by 16 to 0x598 and Line by 4 to 419
  Special opcode 149: advance Address by 20 to 0x5ac and Line by 4 to 423
  Special opcode 90: advance Address by 12 to 0x5b8 and Line by 1 to 424
  Special opcode 92: advance Address by 12 to 0x5c4 and Line by 3 to 427
  Special opcode 146: advance Address by 20 to 0x5d8 and Line by 1 to 428
  Special opcode 147: advance Address by 20 to 0x5ec and Line by 2 to 430
  Special opcode 205: advance Address by 28 to 0x608 and Line by 4 to 434
  Special opcode 91: advance Address by 12 to 0x614 and Line by 2 to 436
  Special opcode 118: advance Address by 16 to 0x624 and Line by 1 to 437
  Special opcode 202: advance Address by 28 to 0x640 and Line by 1 to 438
  Special opcode 204: advance Address by 28 to 0x65c and Line by 3 to 441
  Special opcode 146: advance Address by 20 to 0x670 and Line by 1 to 442
  Special opcode 202: advance Address by 28 to 0x68c and Line by 1 to 443
  Special opcode 202: advance Address by 28 to 0x6a8 and Line by 1 to 444
  Special opcode 202: advance Address by 28 to 0x6c4 and Line by 1 to 445
  Special opcode 202: advance Address by 28 to 0x6e0 and Line by 1 to 446
  Special opcode 118: advance Address by 16 to 0x6f0 and Line by 1 to 447
  Special opcode 232: advance Address by 32 to 0x710 and Line by 3 to 450
  Special opcode 90: advance Address by 12 to 0x71c and Line by 1 to 451
  Special opcode 35: advance Address by 4 to 0x720 and Line by 2 to 453
  Special opcode 90: advance Address by 12 to 0x72c and Line by 1 to 454
  Special opcode 35: advance Address by 4 to 0x730 and Line by 2 to 456
  Special opcode 90: advance Address by 12 to 0x73c and Line by 1 to 457
  Special opcode 35: advance Address by 4 to 0x740 and Line by 2 to 459
  Special opcode 90: advance Address by 12 to 0x74c and Line by 1 to 460
  Special opcode 35: advance Address by 4 to 0x750 and Line by 2 to 462
  Special opcode 94: advance Address by 12 to 0x75c and Line by 5 to 467
  Special opcode 203: advance Address by 28 to 0x778 and Line by 2 to 469
  Special opcode 34: advance Address by 4 to 0x77c and Line by 1 to 470
  Special opcode 93: advance Address by 12 to 0x788 and Line by 4 to 474
  Special opcode 146: advance Address by 20 to 0x79c and Line by 1 to 475
  Special opcode 90: advance Address by 12 to 0x7a8 and Line by 1 to 476
  Special opcode 90: advance Address by 12 to 0x7b4 and Line by 1 to 477
  Special opcode 146: advance Address by 20 to 0x7c8 and Line by 1 to 478
  Special opcode 62: advance Address by 8 to 0x7d0 and Line by 1 to 479
  Special opcode 146: advance Address by 20 to 0x7e4 and Line by 1 to 480
  Special opcode 62: advance Address by 8 to 0x7ec and Line by 1 to 481
  Special opcode 148: advance Address by 20 to 0x800 and Line by 3 to 484
  Special opcode 119: advance Address by 16 to 0x810 and Line by 2 to 486
  Special opcode 121: advance Address by 16 to 0x820 and Line by 4 to 490
  Special opcode 93: advance Address by 12 to 0x82c and Line by 4 to 494
  Special opcode 119: advance Address by 16 to 0x83c and Line by 2 to 496
  Special opcode 121: advance Address by 16 to 0x84c and Line by 4 to 500
  Special opcode 93: advance Address by 12 to 0x858 and Line by 4 to 504
  Special opcode 119: advance Address by 16 to 0x868 and Line by 2 to 506
  Special opcode 119: advance Address by 16 to 0x878 and Line by 2 to 508
  Special opcode 121: advance Address by 16 to 0x888 and Line by 4 to 512
  Special opcode 122: advance Address by 16 to 0x898 and Line by 5 to 517
  Special opcode 90: advance Address by 12 to 0x8a4 and Line by 1 to 518
  Special opcode 93: advance Address by 12 to 0x8b0 and Line by 4 to 522
  Special opcode 119: advance Address by 16 to 0x8c0 and Line by 2 to 524
  Special opcode 121: advance Address by 16 to 0x8d0 and Line by 4 to 528
  Special opcode 93: advance Address by 12 to 0x8dc and Line by 4 to 532
  Special opcode 119: advance Address by 16 to 0x8ec and Line by 2 to 534
  Special opcode 121: advance Address by 16 to 0x8fc and Line by 4 to 538
  Special opcode 93: advance Address by 12 to 0x908 and Line by 4 to 542
  Special opcode 119: advance Address by 16 to 0x918 and Line by 2 to 544
  Special opcode 121: advance Address by 16 to 0x928 and Line by 4 to 548
  Special opcode 93: advance Address by 12 to 0x934 and Line by 4 to 552
  Special opcode 147: advance Address by 20 to 0x948 and Line by 2 to 554
  Advance PC by 140 to 0x9d4
  Special opcode 6: advance Address by 0 to 0x9d4 and Line by 1 to 555
  Special opcode 175: advance Address by 24 to 0x9ec and Line by 2 to 557
  Special opcode 119: advance Address by 16 to 0x9fc and Line by 2 to 559
  Advance PC by 132 to 0xa80
  Special opcode 6: advance Address by 0 to 0xa80 and Line by 1 to 560
  Special opcode 147: advance Address by 20 to 0xa94 and Line by 2 to 562
  Special opcode 119: advance Address by 16 to 0xaa4 and Line by 2 to 564
  Advance PC by 132 to 0xb28
  Special opcode 6: advance Address by 0 to 0xb28 and Line by 1 to 565
  Special opcode 148: advance Address by 20 to 0xb3c and Line by 3 to 568
  Special opcode 34: advance Address by 4 to 0xb40 and Line by 1 to 569
  Advance Line by 11 to 580
  Special opcode 229: advance Address by 32 to 0xb60 and Line by 0 to 580
  Special opcode 233: advance Address by 32 to 0xb80 and Line by 4 to 584
  Special opcode 62: advance Address by 8 to 0xb88 and Line by 1 to 585
  Special opcode 62: advance Address by 8 to 0xb90 and Line by 1 to 586
  Special opcode 90: advance Address by 12 to 0xb9c and Line by 1 to 587
  Special opcode 94: advance Address by 12 to 0xba8 and Line by 5 to 592
  Advance PC by constant 34 to 0xbca
  Special opcode 49: advance Address by 6 to 0xbd0 and Line by 2 to 594
  Special opcode 176: advance Address by 24 to 0xbe8 and Line by 3 to 597
  Advance PC by constant 34 to 0xc0a
  Special opcode 132: advance Address by 18 to 0xc1c and Line by 1 to 598
  Special opcode 90: advance Address by 12 to 0xc28 and Line by 1 to 599
  Special opcode 148: advance Address by 20 to 0xc3c and Line by 3 to 602
  Advance PC by 96 to 0xc9c
  Special opcode 7: advance Address by 0 to 0xc9c and Line by 2 to 604
  Special opcode 91: advance Address by 12 to 0xca8 and Line by 2 to 606
  Advance PC by constant 34 to 0xcca
  Special opcode 22: advance Address by 2 to 0xccc and Line by 3 to 609
  Special opcode 92: advance Address by 12 to 0xcd8 and Line by 3 to 612
  Special opcode 120: advance Address by 16 to 0xce8 and Line by 3 to 615
  Special opcode 91: advance Address by 12 to 0xcf4 and Line by 2 to 617
  Special opcode 150: advance Address by 20 to 0xd08 and Line by 5 to 622
  Special opcode 146: advance Address by 20 to 0xd1c and Line by 1 to 623
  Special opcode 92: advance Address by 12 to 0xd28 and Line by 3 to 626
  Advance PC by constant 34 to 0xd4a
  Special opcode 160: advance Address by 22 to 0xd60 and Line by 1 to 627
  Special opcode 118: advance Address by 16 to 0xd70 and Line by 1 to 628
  Special opcode 147: advance Address by 20 to 0xd84 and Line by 2 to 630
  Special opcode 64: advance Address by 8 to 0xd8c and Line by 3 to 633
  Special opcode 177: advance Address by 24 to 0xda4 and Line by 4 to 637
  Special opcode 122: advance Address by 16 to 0xdb4 and Line by 5 to 642
  Special opcode 92: advance Address by 12 to 0xdc0 and Line by 3 to 645
  Special opcode 232: advance Address by 32 to 0xde0 and Line by 3 to 648
  Advance PC by 92 to 0xe3c
  Special opcode 2: advance Address by 0 to 0xe3c and Line by -3 to 645
  Advance Line by 10 to 655
  Special opcode 33: advance Address by 4 to 0xe40 and Line by 0 to 655
  Special opcode 175: advance Address by 24 to 0xe58 and Line by 2 to 657
  Special opcode 63: advance Address by 8 to 0xe60 and Line by 2 to 659
  Special opcode 231: advance Address by 32 to 0xe80 and Line by 2 to 661
  Special opcode 93: advance Address by 12 to 0xe8c and Line by 4 to 665
  Special opcode 119: advance Address by 16 to 0xe9c and Line by 2 to 667
  Special opcode 120: advance Address by 16 to 0xeac and Line by 3 to 670
  Special opcode 178: advance Address by 24 to 0xec4 and Line by 5 to 675
  Special opcode 119: advance Address by 16 to 0xed4 and Line by 2 to 677
  Special opcode 152: advance Address by 20 to 0xee8 and Line by 7 to 684
  Special opcode 120: advance Address by 16 to 0xef8 and Line by 3 to 687
  Special opcode 92: advance Address by 12 to 0xf04 and Line by 3 to 690
  Special opcode 118: advance Address by 16 to 0xf14 and Line by 1 to 691
  Special opcode 151: advance Address by 20 to 0xf28 and Line by 6 to 697
  Special opcode 146: advance Address by 20 to 0xf3c and Line by 1 to 698
  Special opcode 120: advance Address by 16 to 0xf4c and Line by 3 to 701
  Advance PC by constant 34 to 0xf6e
  Special opcode 160: advance Address by 22 to 0xf84 and Line by 1 to 702
  Special opcode 118: advance Address by 16 to 0xf94 and Line by 1 to 703
  Special opcode 147: advance Address by 20 to 0xfa8 and Line by 2 to 705
  Special opcode 64: advance Address by 8 to 0xfb0 and Line by 3 to 708
  Special opcode 177: advance Address by 24 to 0xfc8 and Line by 4 to 712
  Special opcode 118: advance Address by 16 to 0xfd8 and Line by 1 to 713
  Special opcode 206: advance Address by 28 to 0xff4 and Line by 5 to 718
  Special opcode 92: advance Address by 12 to 0x1000 and Line by 3 to 721
  Special opcode 174: advance Address by 24 to 0x1018 and Line by 1 to 722
  Special opcode 119: advance Address by 16 to 0x1028 and Line by 2 to 724
  Special opcode 94: advance Address by 12 to 0x1034 and Line by 5 to 729
  Special opcode 119: advance Address by 16 to 0x1044 and Line by 2 to 731
  Special opcode 66: advance Address by 8 to 0x104c and Line by 5 to 736
  Special opcode 119: advance Address by 16 to 0x105c and Line by 2 to 738
  Special opcode 119: advance Address by 16 to 0x106c and Line by 2 to 740
  Special opcode 119: advance Address by 16 to 0x107c and Line by 2 to 742
  Special opcode 92: advance Address by 12 to 0x1088 and Line by 3 to 745
  Special opcode 119: advance Address by 16 to 0x1098 and Line by 2 to 747
  Special opcode 119: advance Address by 16 to 0x10a8 and Line by 2 to 749
  Special opcode 149: advance Address by 20 to 0x10bc and Line by 4 to 753
  Special opcode 34: advance Address by 4 to 0x10c0 and Line by 1 to 754
  Special opcode 125: advance Address by 16 to 0x10d0 and Line by 8 to 762
  Advance Line by 9 to 771
  Advance PC by constant 34 to 0x10f2
  Special opcode 47: advance Address by 6 to 0x10f8 and Line by 0 to 771
  Special opcode 62: advance Address by 8 to 0x1100 and Line by 1 to 772
  Special opcode 90: advance Address by 12 to 0x110c and Line by 1 to 773
  Special opcode 90: advance Address by 12 to 0x1118 and Line by 1 to 774
  Special opcode 90: advance Address by 12 to 0x1124 and Line by 1 to 775
  Special opcode 91: advance Address by 12 to 0x1130 and Line by 2 to 777
  Special opcode 148: advance Address by 20 to 0x1144 and Line by 3 to 780
  Special opcode 146: advance Address by 20 to 0x1158 and Line by 1 to 781
  Special opcode 231: advance Address by 32 to 0x1178 and Line by 2 to 783
  Special opcode 90: advance Address by 12 to 0x1184 and Line by 1 to 784
  Special opcode 149: advance Address by 20 to 0x1198 and Line by 4 to 788
  Special opcode 148: advance Address by 20 to 0x11ac and Line by 3 to 791
  Special opcode 90: advance Address by 12 to 0x11b8 and Line by 1 to 792
  Special opcode 119: advance Address by 16 to 0x11c8 and Line by 2 to 794
  Special opcode 90: advance Address by 12 to 0x11d4 and Line by 1 to 795
  Special opcode 92: advance Address by 12 to 0x11e0 and Line by 3 to 798
  Special opcode 118: advance Address by 16 to 0x11f0 and Line by 1 to 799
  Advance PC by constant 34 to 0x1212
  Special opcode 48: advance Address by 6 to 0x1218 and Line by 1 to 800
  Advance Line by 16 to 816
  Special opcode 145: advance Address by 20 to 0x122c and Line by 0 to 816
  Special opcode 178: advance Address by 24 to 0x1244 and Line by 5 to 821
  Special opcode 120: advance Address by 16 to 0x1254 and Line by 3 to 824
  Advance PC by constant 34 to 0x1276
  Special opcode 132: advance Address by 18 to 0x1288 and Line by 1 to 825
  Advance PC by constant 34 to 0x12aa
  Special opcode 217: advance Address by 30 to 0x12c8 and Line by 2 to 827
  Special opcode 120: advance Address by 16 to 0x12d8 and Line by 3 to 830
  Special opcode 234: advance Address by 32 to 0x12f8 and Line by 5 to 835
  Special opcode 147: advance Address by 20 to 0x130c and Line by 2 to 837
  Special opcode 146: advance Address by 20 to 0x1320 and Line by 1 to 838
  Advance PC by 96 to 0x1380
  Special opcode 7: advance Address by 0 to 0x1380 and Line by 2 to 840
  Special opcode 90: advance Address by 12 to 0x138c and Line by 1 to 841
  Special opcode 90: advance Address by 12 to 0x1398 and Line by 1 to 842
  Special opcode 176: advance Address by 24 to 0x13b0 and Line by 3 to 845
  Special opcode 118: advance Address by 16 to 0x13c0 and Line by 1 to 846
  Special opcode 85: advance Address by 12 to 0x13cc and Line by -4 to 842
  Special opcode 39: advance Address by 4 to 0x13d0 and Line by 6 to 848
  Advance PC by constant 34 to 0x13f2
  Special opcode 134: advance Address by 18 to 0x1404 and Line by 3 to 851
  Special opcode 175: advance Address by 24 to 0x141c and Line by 2 to 853
  Advance PC by 84 to 0x1470
  Special opcode 9: advance Address by 0 to 0x1470 and Line by 4 to 857
  Special opcode 91: advance Address by 12 to 0x147c and Line by 2 to 859
  Special opcode 90: advance Address by 12 to 0x1488 and Line by 1 to 860
  Advance Line by 9 to 869
  Special opcode 89: advance Address by 12 to 0x1494 and Line by 0 to 869
  Special opcode 90: advance Address by 12 to 0x14a0 and Line by 1 to 870
  Special opcode 92: advance Address by 12 to 0x14ac and Line by 3 to 873
  Advance PC by 68 to 0x14f0
  Special opcode 8: advance Address by 0 to 0x14f0 and Line by 3 to 876
  Special opcode 91: advance Address by 12 to 0x14fc and Line by 2 to 878
  Special opcode 120: advance Address by 16 to 0x150c and Line by 3 to 881
  Special opcode 146: advance Address by 20 to 0x1520 and Line by 1 to 882
  Special opcode 147: advance Address by 20 to 0x1534 and Line by 2 to 884
  Advance PC by constant 34 to 0x1556
  Special opcode 49: advance Address by 6 to 0x155c and Line by 2 to 886
  Special opcode 232: advance Address by 32 to 0x157c and Line by 3 to 889
  Advance PC by constant 34 to 0x159e
  Special opcode 188: advance Address by 26 to 0x15b8 and Line by 1 to 890
  Special opcode 93: advance Address by 12 to 0x15c4 and Line by 4 to 894
  Special opcode 90: advance Address by 12 to 0x15d0 and Line by 1 to 895
  Special opcode 90: advance Address by 12 to 0x15dc and Line by 1 to 896
  Special opcode 118: advance Address by 16 to 0x15ec and Line by 1 to 897
  Special opcode 94: advance Address by 12 to 0x15f8 and Line by 5 to 902
  Advance PC by constant 34 to 0x161a
  Special opcode 107: advance Address by 14 to 0x1628 and Line by 4 to 906
  Special opcode 90: advance Address by 12 to 0x1634 and Line by 1 to 907
  Advance PC by constant 34 to 0x1656
  Special opcode 21: advance Address by 2 to 0x1658 and Line by 2 to 909
  Special opcode 148: advance Address by 20 to 0x166c and Line by 3 to 912
  Special opcode 91: advance Address by 12 to 0x1678 and Line by 2 to 914
  Advance PC by constant 34 to 0x169a
  Special opcode 77: advance Address by 10 to 0x16a4 and Line by 2 to 916
  Special opcode 231: advance Address by 32 to 0x16c4 and Line by 2 to 918
  Advance PC by constant 34 to 0x16e6
  Special opcode 189: advance Address by 26 to 0x1700 and Line by 2 to 920
  Special opcode 146: advance Address by 20 to 0x1714 and Line by 1 to 921
  Advance PC by constant 34 to 0x1736
  Special opcode 188: advance Address by 26 to 0x1750 and Line by 1 to 922
  Special opcode 93: advance Address by 12 to 0x175c and Line by 4 to 926
  Special opcode 146: advance Address by 20 to 0x1770 and Line by 1 to 927
  Special opcode 38: advance Address by 4 to 0x1774 and Line by 5 to 932
  Special opcode 92: advance Address by 12 to 0x1780 and Line by 3 to 935
  Special opcode 90: advance Address by 12 to 0x178c and Line by 1 to 936
  Special opcode 90: advance Address by 12 to 0x1798 and Line by 1 to 937
  Special opcode 176: advance Address by 24 to 0x17b0 and Line by 3 to 940
  Special opcode 118: advance Address by 16 to 0x17c0 and Line by 1 to 941
  Special opcode 85: advance Address by 12 to 0x17cc and Line by -4 to 937
  Special opcode 39: advance Address by 4 to 0x17d0 and Line by 6 to 943
  Advance PC by constant 34 to 0x17f2
  Special opcode 134: advance Address by 18 to 0x1804 and Line by 3 to 946
  Special opcode 175: advance Address by 24 to 0x181c and Line by 2 to 948
  Advance PC by 84 to 0x1870
  Special opcode 9: advance Address by 0 to 0x1870 and Line by 4 to 952
  Special opcode 91: advance Address by 12 to 0x187c and Line by 2 to 954
  Special opcode 90: advance Address by 12 to 0x1888 and Line by 1 to 955
  Special opcode 91: advance Address by 12 to 0x1894 and Line by 2 to 957
  Advance PC by 80 to 0x18e4
  Special opcode 8: advance Address by 0 to 0x18e4 and Line by 3 to 960
  Special opcode 92: advance Address by 12 to 0x18f0 and Line by 3 to 963
  Special opcode 119: advance Address by 16 to 0x1900 and Line by 2 to 965
  Special opcode 90: advance Address by 12 to 0x190c and Line by 1 to 966
  Special opcode 91: advance Address by 12 to 0x1918 and Line by 2 to 968
  Advance PC by constant 34 to 0x193a
  Special opcode 220: advance Address by 30 to 0x1958 and Line by 5 to 973
  Special opcode 118: advance Address by 16 to 0x1968 and Line by 1 to 974
  Special opcode 96: advance Address by 12 to 0x1974 and Line by 7 to 981
  Special opcode 119: advance Address by 16 to 0x1984 and Line by 2 to 983
  Special opcode 177: advance Address by 24 to 0x199c and Line by 4 to 987
  Special opcode 233: advance Address by 32 to 0x19bc and Line by 4 to 991
  Advance PC by constant 34 to 0x19de
  Special opcode 21: advance Address by 2 to 0x19e0 and Line by 2 to 993
  Advance PC by constant 34 to 0x1a02
  Special opcode 188: advance Address by 26 to 0x1a1c and Line by 1 to 994
  Special opcode 94: advance Address by 12 to 0x1a28 and Line by 5 to 999
  Special opcode 147: advance Address by 20 to 0x1a3c and Line by 2 to 1001
  Advance PC by constant 34 to 0x1a5e
  Special opcode 189: advance Address by 26 to 0x1a78 and Line by 2 to 1003
  Special opcode 90: advance Address by 12 to 0x1a84 and Line by 1 to 1004
  Advance PC by constant 34 to 0x1aa6
  Special opcode 55: advance Address by 6 to 0x1aac and Line by 8 to 1012
  Special opcode 236: advance Address by 32 to 0x1acc and Line by 7 to 1019
  Special opcode 62: advance Address by 8 to 0x1ad4 and Line by 1 to 1020
  Special opcode 62: advance Address by 8 to 0x1adc and Line by 1 to 1021
  Special opcode 91: advance Address by 12 to 0x1ae8 and Line by 2 to 1023
  Special opcode 62: advance Address by 8 to 0x1af0 and Line by 1 to 1024
  Special opcode 62: advance Address by 8 to 0x1af8 and Line by 1 to 1025
  Special opcode 62: advance Address by 8 to 0x1b00 and Line by 1 to 1026
  Special opcode 62: advance Address by 8 to 0x1b08 and Line by 1 to 1027
  Special opcode 91: advance Address by 12 to 0x1b14 and Line by 2 to 1029
  Special opcode 90: advance Address by 12 to 0x1b20 and Line by 1 to 1030
  Special opcode 91: advance Address by 12 to 0x1b2c and Line by 2 to 1032
  Special opcode 147: advance Address by 20 to 0x1b40 and Line by 2 to 1034
  Special opcode 204: advance Address by 28 to 0x1b5c and Line by 3 to 1037
  Special opcode 118: advance Address by 16 to 0x1b6c and Line by 1 to 1038
  Advance PC by constant 34 to 0x1b8e
  Special opcode 48: advance Address by 6 to 0x1b94 and Line by 1 to 1039
  Special opcode 149: advance Address by 20 to 0x1ba8 and Line by 4 to 1043
  Special opcode 231: advance Address by 32 to 0x1bc8 and Line by 2 to 1045
  Special opcode 199: advance Address by 28 to 0x1be4 and Line by -2 to 1043
  Special opcode 39: advance Address by 4 to 0x1be8 and Line by 6 to 1049
  Special opcode 204: advance Address by 28 to 0x1c04 and Line by 3 to 1052
  Special opcode 92: advance Address by 12 to 0x1c10 and Line by 3 to 1055
  Special opcode 206: advance Address by 28 to 0x1c2c and Line by 5 to 1060
  Special opcode 121: advance Address by 16 to 0x1c3c and Line by 4 to 1064
  Special opcode 62: advance Address by 8 to 0x1c44 and Line by 1 to 1065
  Advance PC by constant 34 to 0x1c66
  Special opcode 50: advance Address by 6 to 0x1c6c and Line by 3 to 1068
  Special opcode 62: advance Address by 8 to 0x1c74 and Line by 1 to 1069
  Special opcode 90: advance Address by 12 to 0x1c80 and Line by 1 to 1070
  Special opcode 35: advance Address by 4 to 0x1c84 and Line by 2 to 1072
  Special opcode 62: advance Address by 8 to 0x1c8c and Line by 1 to 1073
  Special opcode 90: advance Address by 12 to 0x1c98 and Line by 1 to 1074
  Special opcode 35: advance Address by 4 to 0x1c9c and Line by 2 to 1076
  Special opcode 62: advance Address by 8 to 0x1ca4 and Line by 1 to 1077
  Special opcode 91: advance Address by 12 to 0x1cb0 and Line by 2 to 1079
  Special opcode 90: advance Address by 12 to 0x1cbc and Line by 1 to 1080
  Special opcode 90: advance Address by 12 to 0x1cc8 and Line by 1 to 1081
  Special opcode 176: advance Address by 24 to 0x1ce0 and Line by 3 to 1084
  Special opcode 118: advance Address by 16 to 0x1cf0 and Line by 1 to 1085
  Special opcode 85: advance Address by 12 to 0x1cfc and Line by -4 to 1081
  Special opcode 39: advance Address by 4 to 0x1d00 and Line by 6 to 1087
  Advance PC by constant 34 to 0x1d22
  Special opcode 134: advance Address by 18 to 0x1d34 and Line by 3 to 1090
  Special opcode 175: advance Address by 24 to 0x1d4c and Line by 2 to 1092
  Advance PC by 84 to 0x1da0
  Special opcode 9: advance Address by 0 to 0x1da0 and Line by 4 to 1096
  Special opcode 91: advance Address by 12 to 0x1dac and Line by 2 to 1098
  Special opcode 90: advance Address by 12 to 0x1db8 and Line by 1 to 1099
  Special opcode 90: advance Address by 12 to 0x1dc4 and Line by 1 to 1100
  Advance PC by 80 to 0x1e14
  Special opcode 8: advance Address by 0 to 0x1e14 and Line by 3 to 1103
  Special opcode 91: advance Address by 12 to 0x1e20 and Line by 2 to 1105
  Special opcode 62: advance Address by 8 to 0x1e28 and Line by 1 to 1106
  Special opcode 91: advance Address by 12 to 0x1e34 and Line by 2 to 1108
  Advance PC by constant 34 to 0x1e56
  Special opcode 221: advance Address by 30 to 0x1e74 and Line by 6 to 1114
  Special opcode 118: advance Address by 16 to 0x1e84 and Line by 1 to 1115
  Special opcode 93: advance Address by 12 to 0x1e90 and Line by 4 to 1119
  Special opcode 93: advance Address by 12 to 0x1e9c and Line by 4 to 1123
  Advance PC by constant 34 to 0x1ebe
  Special opcode 20: advance Address by 2 to 0x1ec0 and Line by 1 to 1124
  Special opcode 91: advance Address by 12 to 0x1ecc and Line by 2 to 1126
  Special opcode 92: advance Address by 12 to 0x1ed8 and Line by 3 to 1129
  Special opcode 146: advance Address by 20 to 0x1eec and Line by 1 to 1130
  Special opcode 122: advance Address by 16 to 0x1efc and Line by 5 to 1135
  Special opcode 62: advance Address by 8 to 0x1f04 and Line by 1 to 1136
  Special opcode 119: advance Address by 16 to 0x1f14 and Line by 2 to 1138
  Special opcode 97: advance Address by 12 to 0x1f20 and Line by 8 to 1146
  Special opcode 90: advance Address by 12 to 0x1f2c and Line by 1 to 1147
  Special opcode 91: advance Address by 12 to 0x1f38 and Line by 2 to 1149
  Special opcode 233: advance Address by 32 to 0x1f58 and Line by 4 to 1153
  Special opcode 148: advance Address by 20 to 0x1f6c and Line by 3 to 1156
  Special opcode 34: advance Address by 4 to 0x1f70 and Line by 1 to 1157
  Special opcode 94: advance Address by 12 to 0x1f7c and Line by 5 to 1162
  Advance PC by constant 34 to 0x1f9e
  Special opcode 77: advance Address by 10 to 0x1fa8 and Line by 2 to 1164
  Special opcode 90: advance Address by 12 to 0x1fb4 and Line by 1 to 1165
  Special opcode 90: advance Address by 12 to 0x1fc0 and Line by 1 to 1166
  Special opcode 62: advance Address by 8 to 0x1fc8 and Line by 1 to 1167
  Special opcode 92: advance Address by 12 to 0x1fd4 and Line by 3 to 1170
  Special opcode 148: advance Address by 20 to 0x1fe8 and Line by 3 to 1173
  Special opcode 119: advance Address by 16 to 0x1ff8 and Line by 2 to 1175
  Special opcode 119: advance Address by 16 to 0x2008 and Line by 2 to 1177
  Special opcode 94: advance Address by 12 to 0x2014 and Line by 5 to 1182
  Special opcode 119: advance Address by 16 to 0x2024 and Line by 2 to 1184
  Special opcode 65: advance Address by 8 to 0x202c and Line by 4 to 1188
  Special opcode 91: advance Address by 12 to 0x2038 and Line by 2 to 1190
  Advance PC by 132 to 0x20bc
  Special opcode 7: advance Address by 0 to 0x20bc and Line by 2 to 1192
  Special opcode 231: advance Address by 32 to 0x20dc and Line by 2 to 1194
  Special opcode 202: advance Address by 28 to 0x20f8 and Line by 1 to 1195
  Special opcode 93: advance Address by 12 to 0x2104 and Line by 4 to 1199
  Special opcode 146: advance Address by 20 to 0x2118 and Line by 1 to 1200
  Special opcode 203: advance Address by 28 to 0x2134 and Line by 2 to 1202
  Special opcode 233: advance Address by 32 to 0x2154 and Line by 4 to 1206
  Advance PC by 132 to 0x21d8
  Special opcode 6: advance Address by 0 to 0x21d8 and Line by 1 to 1207
  Special opcode 119: advance Address by 16 to 0x21e8 and Line by 2 to 1209
  Special opcode 91: advance Address by 12 to 0x21f4 and Line by 2 to 1211
  Special opcode 146: advance Address by 20 to 0x2208 and Line by 1 to 1212
  Special opcode 90: advance Address by 12 to 0x2214 and Line by 1 to 1213
  Special opcode 177: advance Address by 24 to 0x222c and Line by 4 to 1217
  Special opcode 146: advance Address by 20 to 0x2240 and Line by 1 to 1218
  Special opcode 90: advance Address by 12 to 0x224c and Line by 1 to 1219
  Special opcode 149: advance Address by 20 to 0x2260 and Line by 4 to 1223
  Special opcode 231: advance Address by 32 to 0x2280 and Line by 2 to 1225
  Special opcode 202: advance Address by 28 to 0x229c and Line by 1 to 1226
  Special opcode 93: advance Address by 12 to 0x22a8 and Line by 4 to 1230
  Special opcode 146: advance Address by 20 to 0x22bc and Line by 1 to 1231
  Special opcode 203: advance Address by 28 to 0x22d8 and Line by 2 to 1233
  Advance Line by 10 to 1243
  Special opcode 201: advance Address by 28 to 0x22f4 and Line by 0 to 1243
  Special opcode 34: advance Address by 4 to 0x22f8 and Line by 1 to 1244
  Special opcode 206: advance Address by 28 to 0x2314 and Line by 5 to 1249
  Advance PC by constant 34 to 0x2336
  Special opcode 77: advance Address by 10 to 0x2340 and Line by 2 to 1251
  Special opcode 62: advance Address by 8 to 0x2348 and Line by 1 to 1252
  Special opcode 90: advance Address by 12 to 0x2354 and Line by 1 to 1253
  Special opcode 92: advance Address by 12 to 0x2360 and Line by 3 to 1256
  Special opcode 119: advance Address by 16 to 0x2370 and Line by 2 to 1258
  Special opcode 119: advance Address by 16 to 0x2380 and Line by 2 to 1260
  Special opcode 94: advance Address by 12 to 0x238c and Line by 5 to 1265
  Special opcode 119: advance Address by 16 to 0x239c and Line by 2 to 1267
  Special opcode 65: advance Address by 8 to 0x23a4 and Line by 4 to 1271
  Special opcode 175: advance Address by 24 to 0x23bc and Line by 2 to 1273
  Special opcode 92: advance Address by 12 to 0x23c8 and Line by 3 to 1276
  Special opcode 146: advance Address by 20 to 0x23dc and Line by 1 to 1277
  Special opcode 91: advance Address by 12 to 0x23e8 and Line by 2 to 1279
  Special opcode 120: advance Address by 16 to 0x23f8 and Line by 3 to 1282
  Special opcode 90: advance Address by 12 to 0x2404 and Line by 1 to 1283
  Special opcode 120: advance Address by 16 to 0x2414 and Line by 3 to 1286
  Special opcode 122: advance Address by 16 to 0x2424 and Line by 5 to 1291
  Special opcode 90: advance Address by 12 to 0x2430 and Line by 1 to 1292
  Special opcode 122: advance Address by 16 to 0x2440 and Line by 5 to 1297
  Special opcode 120: advance Address by 16 to 0x2450 and Line by 3 to 1300
  Special opcode 91: advance Address by 12 to 0x245c and Line by 2 to 1302
  Special opcode 149: advance Address by 20 to 0x2470 and Line by 4 to 1306
  Special opcode 91: advance Address by 12 to 0x247c and Line by 2 to 1308
  Special opcode 92: advance Address by 12 to 0x2488 and Line by 3 to 1311
  Special opcode 178: advance Address by 24 to 0x24a0 and Line by 5 to 1316
  Special opcode 146: advance Address by 20 to 0x24b4 and Line by 1 to 1317
  Special opcode 91: advance Address by 12 to 0x24c0 and Line by 2 to 1319
  Special opcode 120: advance Address by 16 to 0x24d0 and Line by 3 to 1322
  Special opcode 90: advance Address by 12 to 0x24dc and Line by 1 to 1323
  Special opcode 149: advance Address by 20 to 0x24f0 and Line by 4 to 1327
  Special opcode 122: advance Address by 16 to 0x2500 and Line by 5 to 1332
  Special opcode 120: advance Address by 16 to 0x2510 and Line by 3 to 1335
  Special opcode 90: advance Address by 12 to 0x251c and Line by 1 to 1336
  Special opcode 121: advance Address by 16 to 0x252c and Line by 4 to 1340
  Advance Line by 9 to 1349
  Advance PC by 80 to 0x257c
  Copy
  Special opcode 150: advance Address by 20 to 0x2590 and Line by 5 to 1354
  Special opcode 91: advance Address by 12 to 0x259c and Line by 2 to 1356
  Special opcode 92: advance Address by 12 to 0x25a8 and Line by 3 to 1359
  Special opcode 148: advance Address by 20 to 0x25bc and Line by 3 to 1362
  Special opcode 34: advance Address by 4 to 0x25c0 and Line by 1 to 1363
  Special opcode 95: advance Address by 12 to 0x25cc and Line by 6 to 1369
  Special opcode 203: advance Address by 28 to 0x25e8 and Line by 2 to 1371
  Special opcode 62: advance Address by 8 to 0x25f0 and Line by 1 to 1372
  Special opcode 90: advance Address by 12 to 0x25fc and Line by 1 to 1373
  Special opcode 91: advance Address by 12 to 0x2608 and Line by 2 to 1375
  Special opcode 118: advance Address by 16 to 0x2618 and Line by 1 to 1376
  Special opcode 147: advance Address by 20 to 0x262c and Line by 2 to 1378
  Special opcode 119: advance Address by 16 to 0x263c and Line by 2 to 1380
  Special opcode 120: advance Address by 16 to 0x264c and Line by 3 to 1383
  Special opcode 91: advance Address by 12 to 0x2658 and Line by 2 to 1385
  Special opcode 119: advance Address by 16 to 0x2668 and Line by 2 to 1387
  Special opcode 148: advance Address by 20 to 0x267c and Line by 3 to 1390
  Special opcode 146: advance Address by 20 to 0x2690 and Line by 1 to 1391
  Special opcode 63: advance Address by 8 to 0x2698 and Line by 2 to 1393
  Special opcode 174: advance Address by 24 to 0x26b0 and Line by 1 to 1394
  Special opcode 174: advance Address by 24 to 0x26c8 and Line by 1 to 1395
  Special opcode 118: advance Address by 16 to 0x26d8 and Line by 1 to 1396
  Special opcode 90: advance Address by 12 to 0x26e4 and Line by 1 to 1397
  Special opcode 175: advance Address by 24 to 0x26fc and Line by 2 to 1399
  Special opcode 90: advance Address by 12 to 0x2708 and Line by 1 to 1400
  Special opcode 90: advance Address by 12 to 0x2714 and Line by 1 to 1401
  Special opcode 118: advance Address by 16 to 0x2724 and Line by 1 to 1402
  Special opcode 151: advance Address by 20 to 0x2738 and Line by 6 to 1408
  Special opcode 90: advance Address by 12 to 0x2744 and Line by 1 to 1409
  Special opcode 93: advance Address by 12 to 0x2750 and Line by 4 to 1413
  Advance PC by 148 to 0x27e4
  Special opcode 6: advance Address by 0 to 0x27e4 and Line by 1 to 1414
  Special opcode 147: advance Address by 20 to 0x27f8 and Line by 2 to 1416
  Special opcode 34: advance Address by 4 to 0x27fc and Line by 1 to 1417
  Special opcode 235: advance Address by 32 to 0x281c and Line by 6 to 1423
  Special opcode 180: advance Address by 24 to 0x2834 and Line by 7 to 1430
  Special opcode 62: advance Address by 8 to 0x283c and Line by 1 to 1431
  Special opcode 62: advance Address by 8 to 0x2844 and Line by 1 to 1432
  Special opcode 62: advance Address by 8 to 0x284c and Line by 1 to 1433
  Special opcode 202: advance Address by 28 to 0x2868 and Line by 1 to 1434
  Special opcode 119: advance Address by 16 to 0x2878 and Line by 2 to 1436
  Special opcode 118: advance Address by 16 to 0x2888 and Line by 1 to 1437
  Special opcode 146: advance Address by 20 to 0x289c and Line by 1 to 1438
  Special opcode 146: advance Address by 20 to 0x28b0 and Line by 1 to 1439
  Advance PC by 152 to 0x2948
  Special opcode 6: advance Address by 0 to 0x2948 and Line by 1 to 1440
  Advance PC by 148 to 0x29dc
  Special opcode 7: advance Address by 0 to 0x29dc and Line by 2 to 1442
  Special opcode 119: advance Address by 16 to 0x29ec and Line by 2 to 1444
  Special opcode 119: advance Address by 16 to 0x29fc and Line by 2 to 1446
  Advance PC by constant 34 to 0x2a1e
  Special opcode 188: advance Address by 26 to 0x2a38 and Line by 1 to 1447
  Special opcode 62: advance Address by 8 to 0x2a40 and Line by 1 to 1448
  Special opcode 91: advance Address by 12 to 0x2a4c and Line by 2 to 1450
  Special opcode 119: advance Address by 16 to 0x2a5c and Line by 2 to 1452
  Advance PC by constant 34 to 0x2a7e
  Special opcode 188: advance Address by 26 to 0x2a98 and Line by 1 to 1453
  Special opcode 62: advance Address by 8 to 0x2aa0 and Line by 1 to 1454
  Special opcode 91: advance Address by 12 to 0x2aac and Line by 2 to 1456
  Special opcode 119: advance Address by 16 to 0x2abc and Line by 2 to 1458
  Advance PC by constant 34 to 0x2ade
  Special opcode 188: advance Address by 26 to 0x2af8 and Line by 1 to 1459
  Special opcode 62: advance Address by 8 to 0x2b00 and Line by 1 to 1460
  Special opcode 63: advance Address by 8 to 0x2b08 and Line by 2 to 1462
  Special opcode 92: advance Address by 12 to 0x2b14 and Line by 3 to 1465
  Special opcode 119: advance Address by 16 to 0x2b24 and Line by 2 to 1467
  Special opcode 230: advance Address by 32 to 0x2b44 and Line by 1 to 1468
  Special opcode 146: advance Address by 20 to 0x2b58 and Line by 1 to 1469
  Special opcode 230: advance Address by 32 to 0x2b78 and Line by 1 to 1470
  Special opcode 177: advance Address by 24 to 0x2b90 and Line by 4 to 1474
  Special opcode 118: advance Address by 16 to 0x2ba0 and Line by 1 to 1475
  Special opcode 146: advance Address by 20 to 0x2bb4 and Line by 1 to 1476
  Special opcode 146: advance Address by 20 to 0x2bc8 and Line by 1 to 1477
  Special opcode 230: advance Address by 32 to 0x2be8 and Line by 1 to 1478
  Special opcode 149: advance Address by 20 to 0x2bfc and Line by 4 to 1482
  Special opcode 92: advance Address by 12 to 0x2c08 and Line by 3 to 1485
  Special opcode 148: advance Address by 20 to 0x2c1c and Line by 3 to 1488
  Special opcode 119: advance Address by 16 to 0x2c2c and Line by 2 to 1490
  Special opcode 62: advance Address by 8 to 0x2c34 and Line by 1 to 1491
  Special opcode 230: advance Address by 32 to 0x2c54 and Line by 1 to 1492
  Special opcode 175: advance Address by 24 to 0x2c6c and Line by 2 to 1494
  Special opcode 119: advance Address by 16 to 0x2c7c and Line by 2 to 1496
  Special opcode 118: advance Address by 16 to 0x2c8c and Line by 1 to 1497
  Special opcode 150: advance Address by 20 to 0x2ca0 and Line by 5 to 1502
  Special opcode 204: advance Address by 28 to 0x2cbc and Line by 3 to 1505
  Special opcode 90: advance Address by 12 to 0x2cc8 and Line by 1 to 1506
  Special opcode 120: advance Address by 16 to 0x2cd8 and Line by 3 to 1509
  Special opcode 119: advance Address by 16 to 0x2ce8 and Line by 2 to 1511
  Special opcode 62: advance Address by 8 to 0x2cf0 and Line by 1 to 1512
  Special opcode 93: advance Address by 12 to 0x2cfc and Line by 4 to 1516
  Special opcode 119: advance Address by 16 to 0x2d0c and Line by 2 to 1518
  Special opcode 90: advance Address by 12 to 0x2d18 and Line by 1 to 1519
  Special opcode 62: advance Address by 8 to 0x2d20 and Line by 1 to 1520
  Advance PC by constant 34 to 0x2d42
  Special opcode 50: advance Address by 6 to 0x2d48 and Line by 3 to 1523
  Special opcode 62: advance Address by 8 to 0x2d50 and Line by 1 to 1524
  Special opcode 35: advance Address by 4 to 0x2d54 and Line by 2 to 1526
  Special opcode 62: advance Address by 8 to 0x2d5c and Line by 1 to 1527
  Special opcode 35: advance Address by 4 to 0x2d60 and Line by 2 to 1529
  Special opcode 62: advance Address by 8 to 0x2d68 and Line by 1 to 1530
  Special opcode 35: advance Address by 4 to 0x2d6c and Line by 2 to 1532
  Special opcode 62: advance Address by 8 to 0x2d74 and Line by 1 to 1533
  Special opcode 148: advance Address by 20 to 0x2d88 and Line by 3 to 1536
  Special opcode 120: advance Address by 16 to 0x2d98 and Line by 3 to 1539
  Special opcode 148: advance Address by 20 to 0x2dac and Line by 3 to 1542
  Special opcode 92: advance Address by 12 to 0x2db8 and Line by 3 to 1545
  Special opcode 91: advance Address by 12 to 0x2dc4 and Line by 2 to 1547
  Special opcode 119: advance Address by 16 to 0x2dd4 and Line by 2 to 1549
  Advance PC by constant 34 to 0x2df6
  Special opcode 77: advance Address by 10 to 0x2e00 and Line by 2 to 1551
  Special opcode 64: advance Address by 8 to 0x2e08 and Line by 3 to 1554
  Special opcode 95: advance Address by 12 to 0x2e14 and Line by 6 to 1560
  Advance PC by constant 34 to 0x2e36
  Special opcode 22: advance Address by 2 to 0x2e38 and Line by 3 to 1563
  Special opcode 91: advance Address by 12 to 0x2e44 and Line by 2 to 1565
  Special opcode 231: advance Address by 32 to 0x2e64 and Line by 2 to 1567
  Special opcode 119: advance Address by 16 to 0x2e74 and Line by 2 to 1569
  Advance PC by 152 to 0x2f0c
  Special opcode 6: advance Address by 0 to 0x2f0c and Line by 1 to 1570
  Special opcode 152: advance Address by 20 to 0x2f20 and Line by 7 to 1577
  Special opcode 90: advance Address by 12 to 0x2f2c and Line by 1 to 1578
  Advance PC by 68 to 0x2f70
  Special opcode 8: advance Address by 0 to 0x2f70 and Line by 3 to 1581
  Special opcode 232: advance Address by 32 to 0x2f90 and Line by 3 to 1584
  Special opcode 62: advance Address by 8 to 0x2f98 and Line by 1 to 1585
  Special opcode 62: advance Address by 8 to 0x2fa0 and Line by 1 to 1586
  Special opcode 62: advance Address by 8 to 0x2fa8 and Line by 1 to 1587
  Special opcode 90: advance Address by 12 to 0x2fb4 and Line by 1 to 1588
  Special opcode 91: advance Address by 12 to 0x2fc0 and Line by 2 to 1590
  Special opcode 90: advance Address by 12 to 0x2fcc and Line by 1 to 1591
  Special opcode 91: advance Address by 12 to 0x2fd8 and Line by 2 to 1593
  Special opcode 146: advance Address by 20 to 0x2fec and Line by 1 to 1594
  Special opcode 147: advance Address by 20 to 0x3000 and Line by 2 to 1596
  Special opcode 233: advance Address by 32 to 0x3020 and Line by 4 to 1600
  Special opcode 120: advance Address by 16 to 0x3030 and Line by 3 to 1603
  Special opcode 119: advance Address by 16 to 0x3040 and Line by 2 to 1605
  Special opcode 64: advance Address by 8 to 0x3048 and Line by 3 to 1608
  Special opcode 231: advance Address by 32 to 0x3068 and Line by 2 to 1610
  Special opcode 63: advance Address by 8 to 0x3070 and Line by 2 to 1612
  Special opcode 90: advance Address by 12 to 0x307c and Line by 1 to 1613
  Special opcode 90: advance Address by 12 to 0x3088 and Line by 1 to 1614
  Special opcode 146: advance Address by 20 to 0x309c and Line by 1 to 1615
  Special opcode 147: advance Address by 20 to 0x30b0 and Line by 2 to 1617
  Special opcode 92: advance Address by 12 to 0x30bc and Line by 3 to 1620
  Special opcode 119: advance Address by 16 to 0x30cc and Line by 2 to 1622
  Special opcode 90: advance Address by 12 to 0x30d8 and Line by 1 to 1623
  Special opcode 146: advance Address by 20 to 0x30ec and Line by 1 to 1624
  Advance PC by constant 34 to 0x310e
  Special opcode 105: advance Address by 14 to 0x311c and Line by 2 to 1626
  Special opcode 119: advance Address by 16 to 0x312c and Line by 2 to 1628
  Special opcode 91: advance Address by 12 to 0x3138 and Line by 2 to 1630
  Special opcode 93: advance Address by 12 to 0x3144 and Line by 4 to 1634
  Special opcode 92: advance Address by 12 to 0x3150 and Line by 3 to 1637
  Special opcode 119: advance Address by 16 to 0x3160 and Line by 2 to 1639
  Advance PC by constant 34 to 0x3182
  Special opcode 50: advance Address by 6 to 0x3188 and Line by 3 to 1642
  Special opcode 178: advance Address by 24 to 0x31a0 and Line by 5 to 1647
  Special opcode 202: advance Address by 28 to 0x31bc and Line by 1 to 1648
  Special opcode 177: advance Address by 24 to 0x31d4 and Line by 4 to 1652
  Special opcode 62: advance Address by 8 to 0x31dc and Line by 1 to 1653
  Special opcode 64: advance Address by 8 to 0x31e4 and Line by 3 to 1656
  Special opcode 93: advance Address by 12 to 0x31f0 and Line by 4 to 1660
  Special opcode 147: advance Address by 20 to 0x3204 and Line by 2 to 1662
  Special opcode 122: advance Address by 16 to 0x3214 and Line by 5 to 1667
  Special opcode 91: advance Address by 12 to 0x3220 and Line by 2 to 1669
  Special opcode 202: advance Address by 28 to 0x323c and Line by 1 to 1670
  Advance Line by 10 to 1680
  Special opcode 89: advance Address by 12 to 0x3248 and Line by 0 to 1680
  Special opcode 90: advance Address by 12 to 0x3254 and Line by 1 to 1681
  Special opcode 232: advance Address by 32 to 0x3274 and Line by 3 to 1684
  Special opcode 119: advance Address by 16 to 0x3284 and Line by 2 to 1686
  Advance PC by constant 34 to 0x32a6
  Special opcode 105: advance Address by 14 to 0x32b4 and Line by 2 to 1688
  Special opcode 119: advance Address by 16 to 0x32c4 and Line by 2 to 1690
  Special opcode 125: advance Address by 16 to 0x32d4 and Line by 8 to 1698
  Special opcode 119: advance Address by 16 to 0x32e4 and Line by 2 to 1700
  Special opcode 90: advance Address by 12 to 0x32f0 and Line by 1 to 1701
  Special opcode 146: advance Address by 20 to 0x3304 and Line by 1 to 1702
  Special opcode 35: advance Address by 4 to 0x3308 and Line by 2 to 1704
  Special opcode 119: advance Address by 16 to 0x3318 and Line by 2 to 1706
  Special opcode 90: advance Address by 12 to 0x3324 and Line by 1 to 1707
  Special opcode 148: advance Address by 20 to 0x3338 and Line by 3 to 1710
  Special opcode 92: advance Address by 12 to 0x3344 and Line by 3 to 1713
  Special opcode 119: advance Address by 16 to 0x3354 and Line by 2 to 1715
  Advance PC by constant 34 to 0x3376
  Special opcode 50: advance Address by 6 to 0x337c and Line by 3 to 1718
  Special opcode 178: advance Address by 24 to 0x3394 and Line by 5 to 1723
  Special opcode 202: advance Address by 28 to 0x33b0 and Line by 1 to 1724
  Special opcode 119: advance Address by 16 to 0x33c0 and Line by 2 to 1726
  Special opcode 146: advance Address by 20 to 0x33d4 and Line by 1 to 1727
  Special opcode 92: advance Address by 12 to 0x33e0 and Line by 3 to 1730
  Special opcode 119: advance Address by 16 to 0x33f0 and Line by 2 to 1732
  Special opcode 202: advance Address by 28 to 0x340c and Line by 1 to 1733
  Special opcode 92: advance Address by 12 to 0x3418 and Line by 3 to 1736
  Special opcode 146: advance Address by 20 to 0x342c and Line by 1 to 1737
  Special opcode 90: advance Address by 12 to 0x3438 and Line by 1 to 1738
  Special opcode 231: advance Address by 32 to 0x3458 and Line by 2 to 1740
  Advance PC by constant 34 to 0x347a
  Special opcode 110: advance Address by 14 to 0x3488 and Line by 7 to 1747
  Special opcode 90: advance Address by 12 to 0x3494 and Line by 1 to 1748
  Special opcode 92: advance Address by 12 to 0x34a0 and Line by 3 to 1751
  Special opcode 233: advance Address by 32 to 0x34c0 and Line by 4 to 1755
  Special opcode 62: advance Address by 8 to 0x34c8 and Line by 1 to 1756
  Special opcode 62: advance Address by 8 to 0x34d0 and Line by 1 to 1757
  Special opcode 62: advance Address by 8 to 0x34d8 and Line by 1 to 1758
  Special opcode 90: advance Address by 12 to 0x34e4 and Line by 1 to 1759
  Special opcode 91: advance Address by 12 to 0x34f0 and Line by 2 to 1761
  Special opcode 90: advance Address by 12 to 0x34fc and Line by 1 to 1762
  Special opcode 91: advance Address by 12 to 0x3508 and Line by 2 to 1764
  Special opcode 146: advance Address by 20 to 0x351c and Line by 1 to 1765
  Special opcode 147: advance Address by 20 to 0x3530 and Line by 2 to 1767
  Special opcode 233: advance Address by 32 to 0x3550 and Line by 4 to 1771
  Special opcode 120: advance Address by 16 to 0x3560 and Line by 3 to 1774
  Special opcode 119: advance Address by 16 to 0x3570 and Line by 2 to 1776
  Special opcode 63: advance Address by 8 to 0x3578 and Line by 2 to 1778
  Special opcode 90: advance Address by 12 to 0x3584 and Line by 1 to 1779
  Special opcode 148: advance Address by 20 to 0x3598 and Line by 3 to 1782
  Special opcode 119: advance Address by 16 to 0x35a8 and Line by 2 to 1784
  Special opcode 63: advance Address by 8 to 0x35b0 and Line by 2 to 1786
  Special opcode 90: advance Address by 12 to 0x35bc and Line by 1 to 1787
  Special opcode 148: advance Address by 20 to 0x35d0 and Line by 3 to 1790
  Special opcode 203: advance Address by 28 to 0x35ec and Line by 2 to 1792
  Special opcode 63: advance Address by 8 to 0x35f4 and Line by 2 to 1794
  Special opcode 90: advance Address by 12 to 0x3600 and Line by 1 to 1795
  Special opcode 147: advance Address by 20 to 0x3614 and Line by 2 to 1797
  Special opcode 92: advance Address by 12 to 0x3620 and Line by 3 to 1800
  Special opcode 91: advance Address by 12 to 0x362c and Line by 2 to 1802
  Special opcode 146: advance Address by 20 to 0x3640 and Line by 1 to 1803
  Advance PC by constant 34 to 0x3662
  Special opcode 104: advance Address by 14 to 0x3670 and Line by 1 to 1804
  Special opcode 93: advance Address by 12 to 0x367c and Line by 4 to 1808
  Special opcode 206: advance Address by 28 to 0x3698 and Line by 5 to 1813
  Special opcode 119: advance Address by 16 to 0x36a8 and Line by 2 to 1815
  Advance PC by constant 34 to 0x36ca
  Special opcode 50: advance Address by 6 to 0x36d0 and Line by 3 to 1818
  Special opcode 150: advance Address by 20 to 0x36e4 and Line by 5 to 1823
  Special opcode 146: advance Address by 20 to 0x36f8 and Line by 1 to 1824
  Advance PC by constant 34 to 0x371a
  Special opcode 133: advance Address by 18 to 0x372c and Line by 2 to 1826
  Special opcode 93: advance Address by 12 to 0x3738 and Line by 4 to 1830
  Special opcode 119: advance Address by 16 to 0x3748 and Line by 2 to 1832
  Special opcode 202: advance Address by 28 to 0x3764 and Line by 1 to 1833
  Special opcode 93: advance Address by 12 to 0x3770 and Line by 4 to 1837
  Special opcode 146: advance Address by 20 to 0x3784 and Line by 1 to 1838
  Special opcode 90: advance Address by 12 to 0x3790 and Line by 1 to 1839
  Special opcode 147: advance Address by 20 to 0x37a4 and Line by 2 to 1841
  Advance Line by -33 to 1808
  Advance PC by constant 34 to 0x37c6
  Special opcode 103: advance Address by 14 to 0x37d4 and Line by 0 to 1808
  Advance Line by 38 to 1846
  Special opcode 33: advance Address by 4 to 0x37d8 and Line by 0 to 1846
  Special opcode 62: advance Address by 8 to 0x37e0 and Line by 1 to 1847
  Advance Line by 9 to 1856
  Special opcode 89: advance Address by 12 to 0x37ec and Line by 0 to 1856
  Special opcode 119: advance Address by 16 to 0x37fc and Line by 2 to 1858
  Special opcode 90: advance Address by 12 to 0x3808 and Line by 1 to 1859
  Special opcode 146: advance Address by 20 to 0x381c and Line by 1 to 1860
  Special opcode 35: advance Address by 4 to 0x3820 and Line by 2 to 1862
  Special opcode 119: advance Address by 16 to 0x3830 and Line by 2 to 1864
  Special opcode 90: advance Address by 12 to 0x383c and Line by 1 to 1865
  Special opcode 146: advance Address by 20 to 0x3850 and Line by 1 to 1866
  Special opcode 38: advance Address by 4 to 0x3854 and Line by 5 to 1871
  Special opcode 204: advance Address by 28 to 0x3870 and Line by 3 to 1874
  Advance PC by constant 34 to 0x3892
  Special opcode 48: advance Address by 6 to 0x3898 and Line by 1 to 1875
  Special opcode 118: advance Address by 16 to 0x38a8 and Line by 1 to 1876
  Special opcode 36: advance Address by 4 to 0x38ac and Line by 3 to 1879
  Special opcode 204: advance Address by 28 to 0x38c8 and Line by 3 to 1882
  Special opcode 36: advance Address by 4 to 0x38cc and Line by 3 to 1885
  Special opcode 175: advance Address by 24 to 0x38e4 and Line by 2 to 1887
  Special opcode 177: advance Address by 24 to 0x38fc and Line by 4 to 1891
  Special opcode 119: advance Address by 16 to 0x390c and Line by 2 to 1893
  Advance PC by constant 34 to 0x392e
  Special opcode 50: advance Address by 6 to 0x3934 and Line by 3 to 1896
  Special opcode 146: advance Address by 20 to 0x3948 and Line by 1 to 1897
  Special opcode 92: advance Address by 12 to 0x3954 and Line by 3 to 1900
  Special opcode 119: advance Address by 16 to 0x3964 and Line by 2 to 1902
  Special opcode 93: advance Address by 12 to 0x3970 and Line by 4 to 1906
  Special opcode 146: advance Address by 20 to 0x3984 and Line by 1 to 1907
  Advance PC by constant 34 to 0x39a6
  Special opcode 134: advance Address by 18 to 0x39b8 and Line by 3 to 1910
  Special opcode 147: advance Address by 20 to 0x39cc and Line by 2 to 1912
  Advance PC by constant 34 to 0x39ee
  Special opcode 21: advance Address by 2 to 0x39f0 and Line by 2 to 1914
  Special opcode 90: advance Address by 12 to 0x39fc and Line by 1 to 1915
  Special opcode 147: advance Address by 20 to 0x3a10 and Line by 2 to 1917
  Special opcode 93: advance Address by 12 to 0x3a1c and Line by 4 to 1921
  Special opcode 119: advance Address by 16 to 0x3a2c and Line by 2 to 1923
  Special opcode 202: advance Address by 28 to 0x3a48 and Line by 1 to 1924
  Special opcode 92: advance Address by 12 to 0x3a54 and Line by 3 to 1927
  Advance PC by constant 34 to 0x3a76
  Special opcode 106: advance Address by 14 to 0x3a84 and Line by 3 to 1930
  Advance Line by -48 to 1882
  Special opcode 229: advance Address by 32 to 0x3aa4 and Line by 0 to 1882
  Advance PC by constant 34 to 0x3ac6
  Special opcode 184: advance Address by 26 to 0x3ae0 and Line by -3 to 1879
  Advance Line by 59 to 1938
  Special opcode 33: advance Address by 4 to 0x3ae4 and Line by 0 to 1938
  Special opcode 179: advance Address by 24 to 0x3afc and Line by 6 to 1944
  Special opcode 90: advance Address by 12 to 0x3b08 and Line by 1 to 1945
  Special opcode 93: advance Address by 12 to 0x3b14 and Line by 4 to 1949
  Special opcode 203: advance Address by 28 to 0x3b30 and Line by 2 to 1951
  Special opcode 62: advance Address by 8 to 0x3b38 and Line by 1 to 1952
  Special opcode 91: advance Address by 12 to 0x3b44 and Line by 2 to 1954
  Special opcode 146: advance Address by 20 to 0x3b58 and Line by 1 to 1955
  Special opcode 174: advance Address by 24 to 0x3b70 and Line by 1 to 1956
  Special opcode 64: advance Address by 8 to 0x3b78 and Line by 3 to 1959
  Special opcode 203: advance Address by 28 to 0x3b94 and Line by 2 to 1961
  Special opcode 62: advance Address by 8 to 0x3b9c and Line by 1 to 1962
  Special opcode 91: advance Address by 12 to 0x3ba8 and Line by 2 to 1964
  Special opcode 146: advance Address by 20 to 0x3bbc and Line by 1 to 1965
  Special opcode 174: advance Address by 24 to 0x3bd4 and Line by 1 to 1966
  Special opcode 65: advance Address by 8 to 0x3bdc and Line by 4 to 1970
  Special opcode 177: advance Address by 24 to 0x3bf4 and Line by 4 to 1974
  Special opcode 90: advance Address by 12 to 0x3c00 and Line by 1 to 1975
  Special opcode 90: advance Address by 12 to 0x3c0c and Line by 1 to 1976
  Special opcode 147: advance Address by 20 to 0x3c20 and Line by 2 to 1978
  Advance PC by constant 34 to 0x3c42
  Special opcode 190: advance Address by 26 to 0x3c5c and Line by 3 to 1981
  Special opcode 119: advance Address by 16 to 0x3c6c and Line by 2 to 1983
  Special opcode 202: advance Address by 28 to 0x3c88 and Line by 1 to 1984
  Special opcode 35: advance Address by 4 to 0x3c8c and Line by 2 to 1986
  Special opcode 118: advance Address by 16 to 0x3c9c and Line by 1 to 1987
  Special opcode 35: advance Address by 4 to 0x3ca0 and Line by 2 to 1989
  Special opcode 118: advance Address by 16 to 0x3cb0 and Line by 1 to 1990
  Special opcode 35: advance Address by 4 to 0x3cb4 and Line by 2 to 1992
  Special opcode 118: advance Address by 16 to 0x3cc4 and Line by 1 to 1993
  Special opcode 35: advance Address by 4 to 0x3cc8 and Line by 2 to 1995
  Special opcode 118: advance Address by 16 to 0x3cd8 and Line by 1 to 1996
  Special opcode 147: advance Address by 20 to 0x3cec and Line by 2 to 1998
  Special opcode 118: advance Address by 16 to 0x3cfc and Line by 1 to 1999
  Special opcode 118: advance Address by 16 to 0x3d0c and Line by 1 to 2000
  Special opcode 35: advance Address by 4 to 0x3d10 and Line by 2 to 2002
  Special opcode 146: advance Address by 20 to 0x3d24 and Line by 1 to 2003
  Special opcode 202: advance Address by 28 to 0x3d40 and Line by 1 to 2004
  Advance PC by constant 34 to 0x3d62
  Special opcode 134: advance Address by 18 to 0x3d74 and Line by 3 to 2007
  Special opcode 35: advance Address by 4 to 0x3d78 and Line by 2 to 2009
  Special opcode 146: advance Address by 20 to 0x3d8c and Line by 1 to 2010
  Special opcode 202: advance Address by 28 to 0x3da8 and Line by 1 to 2011
  Special opcode 35: advance Address by 4 to 0x3dac and Line by 2 to 2013
  Special opcode 174: advance Address by 24 to 0x3dc4 and Line by 1 to 2014
  Special opcode 202: advance Address by 28 to 0x3de0 and Line by 1 to 2015
  Advance PC by constant 34 to 0x3e02
  Special opcode 134: advance Address by 18 to 0x3e14 and Line by 3 to 2018
  Special opcode 36: advance Address by 4 to 0x3e18 and Line by 3 to 2021
  Special opcode 202: advance Address by 28 to 0x3e34 and Line by 1 to 2022
  Special opcode 118: advance Address by 16 to 0x3e44 and Line by 1 to 2023
  Special opcode 118: advance Address by 16 to 0x3e54 and Line by 1 to 2024
  Special opcode 118: advance Address by 16 to 0x3e64 and Line by 1 to 2025
  Special opcode 118: advance Address by 16 to 0x3e74 and Line by 1 to 2026
  Special opcode 36: advance Address by 4 to 0x3e78 and Line by 3 to 2029
  Special opcode 202: advance Address by 28 to 0x3e94 and Line by 1 to 2030
  Special opcode 118: advance Address by 16 to 0x3ea4 and Line by 1 to 2031
  Special opcode 118: advance Address by 16 to 0x3eb4 and Line by 1 to 2032
  Special opcode 36: advance Address by 4 to 0x3eb8 and Line by 3 to 2035
  Special opcode 202: advance Address by 28 to 0x3ed4 and Line by 1 to 2036
  Special opcode 118: advance Address by 16 to 0x3ee4 and Line by 1 to 2037
  Special opcode 121: advance Address by 16 to 0x3ef4 and Line by 4 to 2041
  Special opcode 148: advance Address by 20 to 0x3f08 and Line by 3 to 2044
  Advance PC by constant 34 to 0x3f2a
  Special opcode 22: advance Address by 2 to 0x3f2c and Line by 3 to 2047
  Advance PC by constant 34 to 0x3f4e
  Special opcode 104: advance Address by 14 to 0x3f5c and Line by 1 to 2048
  Special opcode 90: advance Address by 12 to 0x3f68 and Line by 1 to 2049
  Special opcode 90: advance Address by 12 to 0x3f74 and Line by 1 to 2050
  Special opcode 148: advance Address by 20 to 0x3f88 and Line by 3 to 2053
  Special opcode 147: advance Address by 20 to 0x3f9c and Line by 2 to 2055
  Special opcode 146: advance Address by 20 to 0x3fb0 and Line by 1 to 2056
  Special opcode 142: advance Address by 20 to 0x3fc4 and Line by -3 to 2053
  Advance PC by constant 34 to 0x3fe6
  Special opcode 52: advance Address by 6 to 0x3fec and Line by 5 to 2058
  Special opcode 35: advance Address by 4 to 0x3ff0 and Line by 2 to 2060
  Advance PC by constant 34 to 0x4012
  Special opcode 73: advance Address by 10 to 0x401c and Line by -2 to 2058
  Special opcode 122: advance Address by 16 to 0x402c and Line by 5 to 2063
  Special opcode 34: advance Address by 4 to 0x4030 and Line by 1 to 2064
  Special opcode 92: advance Address by 12 to 0x403c and Line by 3 to 2067
  Advance PC by constant 34 to 0x405e
  Special opcode 21: advance Address by 2 to 0x4060 and Line by 2 to 2069
  Advance PC by constant 34 to 0x4082
  Special opcode 104: advance Address by 14 to 0x4090 and Line by 1 to 2070
  Special opcode 90: advance Address by 12 to 0x409c and Line by 1 to 2071
  Special opcode 90: advance Address by 12 to 0x40a8 and Line by 1 to 2072
  Special opcode 148: advance Address by 20 to 0x40bc and Line by 3 to 2075
  Special opcode 147: advance Address by 20 to 0x40d0 and Line by 2 to 2077
  Advance PC by constant 34 to 0x40f2
  Special opcode 45: advance Address by 6 to 0x40f8 and Line by -2 to 2075
  Advance PC by constant 34 to 0x411a
  Special opcode 51: advance Address by 6 to 0x4120 and Line by 4 to 2079
  Special opcode 35: advance Address by 4 to 0x4124 and Line by 2 to 2081
  Advance PC by constant 34 to 0x4146
  Special opcode 101: advance Address by 14 to 0x4154 and Line by -2 to 2079
  Special opcode 122: advance Address by 16 to 0x4164 and Line by 5 to 2084
  Special opcode 34: advance Address by 4 to 0x4168 and Line by 1 to 2085
  Special opcode 92: advance Address by 12 to 0x4174 and Line by 3 to 2088
  Advance PC by constant 34 to 0x4196
  Special opcode 21: advance Address by 2 to 0x4198 and Line by 2 to 2090
  Special opcode 90: advance Address by 12 to 0x41a4 and Line by 1 to 2091
  Special opcode 92: advance Address by 12 to 0x41b0 and Line by 3 to 2094
  Special opcode 231: advance Address by 32 to 0x41d0 and Line by 2 to 2096
  Special opcode 92: advance Address by 12 to 0x41dc and Line by 3 to 2099
  Special opcode 146: advance Address by 20 to 0x41f0 and Line by 1 to 2100
  Advance PC by constant 34 to 0x4212
  Special opcode 160: advance Address by 22 to 0x4228 and Line by 1 to 2101
  Special opcode 146: advance Address by 20 to 0x423c and Line by 1 to 2102
  Special opcode 90: advance Address by 12 to 0x4248 and Line by 1 to 2103
  Special opcode 92: advance Address by 12 to 0x4254 and Line by 3 to 2106
  Special opcode 230: advance Address by 32 to 0x4274 and Line by 1 to 2107
  Special opcode 62: advance Address by 8 to 0x427c and Line by 1 to 2108
  Special opcode 90: advance Address by 12 to 0x4288 and Line by 1 to 2109
  Special opcode 95: advance Address by 12 to 0x4294 and Line by 6 to 2115
  Special opcode 231: advance Address by 32 to 0x42b4 and Line by 2 to 2117
  Special opcode 93: advance Address by 12 to 0x42c0 and Line by 4 to 2121
  Special opcode 146: advance Address by 20 to 0x42d4 and Line by 1 to 2122
  Special opcode 120: advance Address by 16 to 0x42e4 and Line by 3 to 2125
  Special opcode 175: advance Address by 24 to 0x42fc and Line by 2 to 2127
  Special opcode 36: advance Address by 4 to 0x4300 and Line by 3 to 2130
  Advance PC by 88 to 0x4358
  Special opcode 6: advance Address by 0 to 0x4358 and Line by 1 to 2131
  Special opcode 92: advance Address by 12 to 0x4364 and Line by 3 to 2134
  Advance Line by -7 to 2127
  Special opcode 145: advance Address by 20 to 0x4378 and Line by 0 to 2127
  Advance Line by 9 to 2136
  Special opcode 117: advance Address by 16 to 0x4388 and Line by 0 to 2136
  Special opcode 174: advance Address by 24 to 0x43a0 and Line by 1 to 2137
  Special opcode 119: advance Address by 16 to 0x43b0 and Line by 2 to 2139
  Special opcode 90: advance Address by 12 to 0x43bc and Line by 1 to 2140
  Special opcode 92: advance Address by 12 to 0x43c8 and Line by 3 to 2143
  Advance PC by constant 34 to 0x43ea
  Special opcode 20: advance Address by 2 to 0x43ec and Line by 1 to 2144
  Special opcode 62: advance Address by 8 to 0x43f4 and Line by 1 to 2145
  Special opcode 90: advance Address by 12 to 0x4400 and Line by 1 to 2146
  Special opcode 95: advance Address by 12 to 0x440c and Line by 6 to 2152
  Special opcode 231: advance Address by 32 to 0x442c and Line by 2 to 2154
  Special opcode 93: advance Address by 12 to 0x4438 and Line by 4 to 2158
  Special opcode 146: advance Address by 20 to 0x444c and Line by 1 to 2159
  Special opcode 146: advance Address by 20 to 0x4460 and Line by 1 to 2160
  Special opcode 119: advance Address by 16 to 0x4470 and Line by 2 to 2162
  Special opcode 175: advance Address by 24 to 0x4488 and Line by 2 to 2164
  Special opcode 36: advance Address by 4 to 0x448c and Line by 3 to 2167
  Advance PC by 88 to 0x44e4
  Special opcode 6: advance Address by 0 to 0x44e4 and Line by 1 to 2168
  Special opcode 92: advance Address by 12 to 0x44f0 and Line by 3 to 2171
  Advance Line by -7 to 2164
  Special opcode 145: advance Address by 20 to 0x4504 and Line by 0 to 2164
  Advance Line by 9 to 2173
  Special opcode 117: advance Address by 16 to 0x4514 and Line by 0 to 2173
  Special opcode 119: advance Address by 16 to 0x4524 and Line by 2 to 2175
  Special opcode 90: advance Address by 12 to 0x4530 and Line by 1 to 2176
  Special opcode 93: advance Address by 12 to 0x453c and Line by 4 to 2180
  Advance PC by constant 34 to 0x455e
  Special opcode 25: advance Address by 2 to 0x4560 and Line by 6 to 2186
  Special opcode 62: advance Address by 8 to 0x4568 and Line by 1 to 2187
  Special opcode 62: advance Address by 8 to 0x4570 and Line by 1 to 2188
  Special opcode 62: advance Address by 8 to 0x4578 and Line by 1 to 2189
  Special opcode 90: advance Address by 12 to 0x4584 and Line by 1 to 2190
  Special opcode 90: advance Address by 12 to 0x4590 and Line by 1 to 2191
  Special opcode 91: advance Address by 12 to 0x459c and Line by 2 to 2193
  Special opcode 147: advance Address by 20 to 0x45b0 and Line by 2 to 2195
  Special opcode 231: advance Address by 32 to 0x45d0 and Line by 2 to 2197
  Advance PC by constant 34 to 0x45f2
  Special opcode 161: advance Address by 22 to 0x4608 and Line by 2 to 2199
  Special opcode 93: advance Address by 12 to 0x4614 and Line by 4 to 2203
  Special opcode 91: advance Address by 12 to 0x4620 and Line by 2 to 2205
  Special opcode 202: advance Address by 28 to 0x463c and Line by 1 to 2206
  Special opcode 94: advance Address by 12 to 0x4648 and Line by 5 to 2211
  Special opcode 62: advance Address by 8 to 0x4650 and Line by 1 to 2212
  Advance PC by constant 34 to 0x4672
  Special opcode 24: advance Address by 2 to 0x4674 and Line by 5 to 2217
  Special opcode 146: advance Address by 20 to 0x4688 and Line by 1 to 2218
  Special opcode 148: advance Address by 20 to 0x469c and Line by 3 to 2221
  Special opcode 119: advance Address by 16 to 0x46ac and Line by 2 to 2223
  Advance PC by constant 34 to 0x46ce
  Special opcode 50: advance Address by 6 to 0x46d4 and Line by 3 to 2226
  Special opcode 146: advance Address by 20 to 0x46e8 and Line by 1 to 2227
  Special opcode 92: advance Address by 12 to 0x46f4 and Line by 3 to 2230
  Special opcode 119: advance Address by 16 to 0x4704 and Line by 2 to 2232
  Special opcode 92: advance Address by 12 to 0x4710 and Line by 3 to 2235
  Special opcode 147: advance Address by 20 to 0x4724 and Line by 2 to 2237
  Advance PC by constant 34 to 0x4746
  Special opcode 134: advance Address by 18 to 0x4758 and Line by 3 to 2240
  Advance PC by constant 34 to 0x477a
  Special opcode 21: advance Address by 2 to 0x477c and Line by 2 to 2242
  Special opcode 90: advance Address by 12 to 0x4788 and Line by 1 to 2243
  Special opcode 147: advance Address by 20 to 0x479c and Line by 2 to 2245
  Special opcode 92: advance Address by 12 to 0x47a8 and Line by 3 to 2248
  Special opcode 119: advance Address by 16 to 0x47b8 and Line by 2 to 2250
  Special opcode 202: advance Address by 28 to 0x47d4 and Line by 1 to 2251
  Special opcode 91: advance Address by 12 to 0x47e0 and Line by 2 to 2253
  Advance PC by constant 34 to 0x4802
  Special opcode 162: advance Address by 22 to 0x4818 and Line by 3 to 2256
  Special opcode 232: advance Address by 32 to 0x4838 and Line by 3 to 2259
  Special opcode 146: advance Address by 20 to 0x484c and Line by 1 to 2260
  Advance PC by constant 34 to 0x486e
  Special opcode 164: advance Address by 22 to 0x4884 and Line by 5 to 2265
  Special opcode 62: advance Address by 8 to 0x488c and Line by 1 to 2266
  Special opcode 232: advance Address by 32 to 0x48ac and Line by 3 to 2269
  Special opcode 92: advance Address by 12 to 0x48b8 and Line by 3 to 2272
  Special opcode 147: advance Address by 20 to 0x48cc and Line by 2 to 2274
  Special opcode 96: advance Address by 12 to 0x48d8 and Line by 7 to 2281
  Special opcode 97: advance Address by 12 to 0x48e4 and Line by 8 to 2289
  Special opcode 147: advance Address by 20 to 0x48f8 and Line by 2 to 2291
  Special opcode 34: advance Address by 4 to 0x48fc and Line by 1 to 2292
  Special opcode 67: advance Address by 8 to 0x4904 and Line by 6 to 2298
  Special opcode 175: advance Address by 24 to 0x491c and Line by 2 to 2300
  Special opcode 34: advance Address by 4 to 0x4920 and Line by 1 to 2301
  Advance PC by 12 to 0x492c
  Extended opcode 1: End of Sequence


The section .debug_frame contains:

00000000 0000000c ffffffff CIE
  Version:               1
  Augmentation:          ""
  Code alignment factor: 1
  Data alignment factor: -4
  Return address column: 14

  DW_CFA_def_cfa: r13 ofs 0

00000010 0000001c 00000000 FDE cie=00000000 pc=00000000..00000138
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

00000030 0000001c 00000000 FDE cie=00000000 pc=00000138..00000170
  DW_CFA_advance_loc: 4 to 0000013c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000140
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00000144
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000050 0000001c 00000000 FDE cie=00000000 pc=00000170..0000023c
  DW_CFA_advance_loc: 4 to 00000174
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000178
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 0000017c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000070 0000001c 00000000 FDE cie=00000000 pc=0000023c..00000490
  DW_CFA_advance_loc: 4 to 00000240
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000244
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00000248
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000090 0000001c 00000000 FDE cie=00000000 pc=00000490..0000052c
  DW_CFA_advance_loc: 4 to 00000494
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000498
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 0000049c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000b0 0000001c 00000000 FDE cie=00000000 pc=0000052c..00000598
  DW_CFA_advance_loc: 4 to 00000530
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000534
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00000538
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000d0 0000001c 00000000 FDE cie=00000000 pc=00000598..00000788
  DW_CFA_advance_loc: 4 to 0000059c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000005a0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000005a4
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000000f0 0000001c 00000000 FDE cie=00000000 pc=00000788..00000b60
  DW_CFA_advance_loc: 4 to 0000078c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000790
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00000794
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000110 0000001c 00000000 FDE cie=00000000 pc=00000b60..000010d0
  DW_CFA_advance_loc: 4 to 00000b64
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00000b68
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 00000b6c
  DW_CFA_def_cfa: r11 ofs 4

00000130 0000001c 00000000 FDE cie=00000000 pc=000010d0..00001aac
  DW_CFA_advance_loc: 4 to 000010d4
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000010d8
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 000010dc
  DW_CFA_def_cfa: r11 ofs 4

00000150 0000001c 00000000 FDE cie=00000000 pc=00001aac..00001f7c
  DW_CFA_advance_loc: 4 to 00001ab0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001ab4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 00001ab8
  DW_CFA_def_cfa: r11 ofs 4

00000170 0000001c 00000000 FDE cie=00000000 pc=00001f7c..00002314
  DW_CFA_advance_loc: 4 to 00001f80
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00001f84
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00001f88
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000190 0000001c 00000000 FDE cie=00000000 pc=00002314..000025cc
  DW_CFA_advance_loc: 4 to 00002318
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000231c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002320
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001b0 0000001c 00000000 FDE cie=00000000 pc=000025cc..0000281c
  DW_CFA_advance_loc: 4 to 000025d0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000025d4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000025d8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001d0 0000001c 00000000 FDE cie=00000000 pc=0000281c..00002f70
  DW_CFA_advance_loc: 4 to 00002820
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00002824
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002828
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000001f0 0000001c 00000000 FDE cie=00000000 pc=00002f70..000034a0
  DW_CFA_advance_loc: 4 to 00002f74
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00002f78
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00002f7c
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000210 0000001c 00000000 FDE cie=00000000 pc=000034a0..00003b14
  DW_CFA_advance_loc: 4 to 000034a4
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000034a8
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000034ac
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000230 0000001c 00000000 FDE cie=00000000 pc=00003b14..00003b78
  DW_CFA_advance_loc: 4 to 00003b18
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003b1c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003b20
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000250 0000001c 00000000 FDE cie=00000000 pc=00003b78..00003bdc
  DW_CFA_advance_loc: 4 to 00003b7c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003b80
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003b84
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000270 0000001c 00000000 FDE cie=00000000 pc=00003bdc..00003f08
  DW_CFA_advance_loc: 4 to 00003be0
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003be4
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003be8
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000290 0000001c 00000000 FDE cie=00000000 pc=00003f08..0000403c
  DW_CFA_advance_loc: 4 to 00003f0c
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00003f10
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00003f14
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000002b0 0000001c 00000000 FDE cie=00000000 pc=0000403c..00004174
  DW_CFA_advance_loc: 4 to 00004040
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00004044
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r4 at cfa-20
  DW_CFA_advance_loc: 4 to 00004048
  DW_CFA_def_cfa: r11 ofs 4

000002d0 0000001c 00000000 FDE cie=00000000 pc=00004174..00004254
  DW_CFA_advance_loc: 4 to 00004178
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000417c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00004180
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

000002f0 00000020 00000000 FDE cie=00000000 pc=00004254..000043c8
  DW_CFA_advance_loc: 4 to 00004258
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000425c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r6 at cfa-20
  DW_CFA_offset: r5 at cfa-24
  DW_CFA_offset: r4 at cfa-28
  DW_CFA_advance_loc: 4 to 00004260
  DW_CFA_def_cfa: r11 ofs 4

00000314 00000020 00000000 FDE cie=00000000 pc=000043c8..0000453c
  DW_CFA_advance_loc: 4 to 000043cc
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000043d0
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_offset: r6 at cfa-20
  DW_CFA_offset: r5 at cfa-24
  DW_CFA_offset: r4 at cfa-28
  DW_CFA_advance_loc: 4 to 000043d4
  DW_CFA_def_cfa: r11 ofs 4

00000338 0000001c 00000000 FDE cie=00000000 pc=0000453c..000048e4
  DW_CFA_advance_loc: 4 to 00004540
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 00004544
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00004548
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000358 0000001c 00000000 FDE cie=00000000 pc=000048e4..00004904
  DW_CFA_advance_loc: 4 to 000048e8
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 000048ec
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 000048f0
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

00000378 0000001c 00000000 FDE cie=00000000 pc=00004904..0000492c
  DW_CFA_advance_loc: 4 to 00004908
  DW_CFA_def_cfa_reg: r12
  DW_CFA_advance_loc: 4 to 0000490c
  DW_CFA_offset: r14 at cfa-8
  DW_CFA_offset: r13 at cfa-12
  DW_CFA_offset: r11 at cfa-16
  DW_CFA_advance_loc: 4 to 00004910
  DW_CFA_def_cfa: r11 ofs 4
  DW_CFA_nop
  DW_CFA_nop

Contents of the .debug_loc section:

    Offset   Begin    End      Expression
    00000000 00000000 00000004 (DW_OP_reg13)
    00000000 00000004 0000000c (DW_OP_reg12)
    00000000 0000000c 00000138 (DW_OP_breg11: 4)
    00000000 <End of list>
    0000002a 00000138 0000013c (DW_OP_reg13)
    0000002a 0000013c 00000144 (DW_OP_reg12)
    0000002a 00000144 00000170 (DW_OP_breg11: 4)
    0000002a <End of list>
    00000054 00000170 00000174 (DW_OP_reg13)
    00000054 00000174 0000017c (DW_OP_reg12)
    00000054 0000017c 0000023c (DW_OP_breg11: 4)
    00000054 <End of list>
    0000007e 0000023c 00000240 (DW_OP_reg13)
    0000007e 00000240 00000248 (DW_OP_reg12)
    0000007e 00000248 00000490 (DW_OP_breg11: 4)
    0000007e <End of list>
    000000a8 00000490 00000494 (DW_OP_reg13)
    000000a8 00000494 0000049c (DW_OP_reg12)
    000000a8 0000049c 0000052c (DW_OP_breg11: 4)
    000000a8 <End of list>
    000000d2 0000052c 00000530 (DW_OP_reg13)
    000000d2 00000530 00000538 (DW_OP_reg12)
    000000d2 00000538 00000598 (DW_OP_breg11: 4)
    000000d2 <End of list>
    000000fc 00000598 0000059c (DW_OP_reg13)
    000000fc 0000059c 000005a4 (DW_OP_reg12)
    000000fc 000005a4 00000788 (DW_OP_breg11: 4)
    000000fc <End of list>
    00000126 00000788 0000078c (DW_OP_reg13)
    00000126 0000078c 00000794 (DW_OP_reg12)
    00000126 00000794 00000b60 (DW_OP_breg11: 4)
    00000126 <End of list>
    00000150 00000b60 00000b64 (DW_OP_reg13)
    00000150 00000b64 00000b6c (DW_OP_reg12)
    00000150 00000b6c 000010d0 (DW_OP_breg11: 4)
    00000150 <End of list>
    0000017a 000010d0 000010d4 (DW_OP_reg13)
    0000017a 000010d4 000010dc (DW_OP_reg12)
    0000017a 000010dc 00001aac (DW_OP_breg11: 4)
    0000017a <End of list>
    000001a4 00001aac 00001ab0 (DW_OP_reg13)
    000001a4 00001ab0 00001ab8 (DW_OP_reg12)
    000001a4 00001ab8 00001f7c (DW_OP_breg11: 4)
    000001a4 <End of list>
    000001ce 00001f7c 00001f80 (DW_OP_reg13)
    000001ce 00001f80 00001f88 (DW_OP_reg12)
    000001ce 00001f88 00002314 (DW_OP_breg11: 4)
    000001ce <End of list>
    000001f8 00002314 00002318 (DW_OP_reg13)
    000001f8 00002318 00002320 (DW_OP_reg12)
    000001f8 00002320 000025cc (DW_OP_breg11: 4)
    000001f8 <End of list>
    00000222 000025cc 000025d0 (DW_OP_reg13)
    00000222 000025d0 000025d8 (DW_OP_reg12)
    00000222 000025d8 0000281c (DW_OP_breg11: 4)
    00000222 <End of list>
    0000024c 0000281c 00002820 (DW_OP_reg13)
    0000024c 00002820 00002828 (DW_OP_reg12)
    0000024c 00002828 00002f70 (DW_OP_breg11: 4)
    0000024c <End of list>
    00000276 00002f70 00002f74 (DW_OP_reg13)
    00000276 00002f74 00002f7c (DW_OP_reg12)
    00000276 00002f7c 000034a0 (DW_OP_breg11: 4)
    00000276 <End of list>
    000002a0 000034a0 000034a4 (DW_OP_reg13)
    000002a0 000034a4 000034ac (DW_OP_reg12)
    000002a0 000034ac 00003b14 (DW_OP_breg11: 4)
    000002a0 <End of list>
    000002ca 00003b14 00003b18 (DW_OP_reg13)
    000002ca 00003b18 00003b20 (DW_OP_reg12)
    000002ca 00003b20 00003b78 (DW_OP_breg11: 4)
    000002ca <End of list>
    000002f4 00003b78 00003b7c (DW_OP_reg13)
    000002f4 00003b7c 00003b84 (DW_OP_reg12)
    000002f4 00003b84 00003bdc (DW_OP_breg11: 4)
    000002f4 <End of list>
    0000031e 00003bdc 00003be0 (DW_OP_reg13)
    0000031e 00003be0 00003be8 (DW_OP_reg12)
    0000031e 00003be8 00003f08 (DW_OP_breg11: 4)
    0000031e <End of list>
    00000348 00003f08 00003f0c (DW_OP_reg13)
    00000348 00003f0c 00003f14 (DW_OP_reg12)
    00000348 00003f14 0000403c (DW_OP_breg11: 4)
    00000348 <End of list>
    00000372 0000403c 00004040 (DW_OP_reg13)
    00000372 00004040 00004048 (DW_OP_reg12)
    00000372 00004048 00004174 (DW_OP_breg11: 4)
    00000372 <End of list>
    0000039c 00004174 00004178 (DW_OP_reg13)
    0000039c 00004178 00004180 (DW_OP_reg12)
    0000039c 00004180 00004254 (DW_OP_breg11: 4)
    0000039c <End of list>
    000003c6 00004254 00004258 (DW_OP_reg13)
    000003c6 00004258 00004260 (DW_OP_reg12)
    000003c6 00004260 000043c8 (DW_OP_breg11: 4)
    000003c6 <End of list>
    000003f0 000043c8 000043cc (DW_OP_reg13)
    000003f0 000043cc 000043d4 (DW_OP_reg12)
    000003f0 000043d4 0000453c (DW_OP_breg11: 4)
    000003f0 <End of list>
    0000041a 0000453c 00004540 (DW_OP_reg13)
    0000041a 00004540 00004548 (DW_OP_reg12)
    0000041a 00004548 000048e4 (DW_OP_breg11: 4)
    0000041a <End of list>
    00000444 000048e4 000048e8 (DW_OP_reg13)
    00000444 000048e8 000048f0 (DW_OP_reg12)
    00000444 000048f0 00004904 (DW_OP_breg11: 4)
    00000444 <End of list>
    0000046e 00004904 00004908 (DW_OP_reg13)
    0000046e 00004908 00004910 (DW_OP_reg12)
    0000046e 00004910 0000492c (DW_OP_breg11: 4)
    0000046e <End of list>
Contents of the .debug_pubnames section:

  Length:                              661
  Version:                             2
  Offset into .debug_info section:     0
  Size of area in .debug_info section: 21222

    Offset  Name
    16076       MGC_HdrcInit
    16192       MGC_HdrcDynamicFifoLocation
    16365       MGC_HdrcIsr
    16588       MGC_HdrcStart
    16689       MGC_HdrcStop
    16775       MGC_HdrcDestroy
    16848       MGC_HdrcReadBusState
    16992       MGC_HdrcProgramBusState
    17175       MGC_HdrcBindEndpoint
    17431       MGC_HdrcStartRx
    17771       MGC_HdrcStartTx
    18154       MGC_HdrcFlushEndpoint
    18343       MGC_HdrcHaltEndpoint
    18514       MGC_HdrcDefaultEndResponse
    18683       MGC_HdrcServiceDefaultEnd
    18901       MGC_HdrcServiceTransmitAvail
    19108       MGC_HdrcServiceReceiveReady
    19566       MGC_HdrcSetPortTestMode
    19707       MGC_HdrcLoadFifo
    19928       MGC_HdrcUnloadFifo
    20130       MGC_HdrcUlpiVbusControl
    20289       MGC_HdrcReadUlpiReg
    20447       MGC_HdrcWriteUlpiReg
    20605       MGC_HdrcDmaChannelStatusChanged
    20904       MGC_HdrcDumpState
    20964       MGC_HdrcDumpEndpoint

The section .debug_aranges contains:

  Length:                   28
  Version:                  2
  Offset into .debug_info:  0
  Pointer Size:             4
  Segment Size:             0

    Address    Length
    0x00000000 0x492c
    0x00000000 0x0

Contents of the .debug_str section:

  0x00000000 6477436f 756e7400 64774163 7475616c dwCount.dwActual
  0x00000010 4c656e67 74680062 4c656e67 74680070 Length.bLength.p
  0x00000020 436f6d70 6c657465 50617261 6d00774d CompleteParam.wM
  0x00000030 61785061 636b6574 53697a65 00626d41 axPacketSize.bmA
  0x00000040 74747269 62757465 73007050 6f727400 ttributes.pPort.
  0x00000050 64775374 61747573 00647754 6f74616c dwStatus.dwTotal
  0x00000060 42797465 73007066 49727043 6f6d706c Bytes.pfIrpCompl
  0x00000070 65746500 70497465 6d007044 6d614368 ete.pItem.pDmaCh
  0x00000080 616e6e65 6c006249 6e746572 76616c00 annel.bInterval.
  0x00000090 77566572 73696f6e 00625472 61666669 wVersion.bTraffi
  0x000000a0 63547970 65006246 69666f4f 66667365 cType.bFifoOffse
  0x000000b0 74007042 61736500 70427566 66657200 t.pBase.pBuffer.
  0x000000c0 6246756e 63416464 72006244 65736372 bFuncAddr.bDescr
  0x000000d0 6970746f 72547970 65007045 6e640070 iptorType.pEnd.p
  0x000000e0 446d6143 6f6e7472 6f6c6c65 72007050 DmaController.pP
  0x000000f0 72697661 74654461 74610070 436f6e74 rivateData.pCont
  0x00000100 726f6c6c 65720077 456e6449 6e646578 roller.wEndIndex
  0x00000110 00624c6f 63616c45 6e640062 42757341 .bLocalEnd.bBusA
  0x00000120 64647265 73730073 74617274 5f74696d ddress.start_tim
  0x00000130 65006252 6573756c 74007374 61747573 e.bResult.status
  0x00000140 0062496e 64657800 6477436f 756e7433 .bIndex.dwCount3
  0x00000150 32006477 496e6465 78333200 61647754 2.dwIndex32.adwT
  0x00000160 696d6572 5265736f 6c757469 6f6e7300 imerResolutions.
  0x00000170 62497372 43616c6c 6261636b 0062416c bIsrCallback.bAl
  0x00000180 6c6f7744 6d610070 53657276 69636573 lowDma.pServices
  0x00000190 00617043 6f6e6669 67446573 63726970 .apConfigDescrip
  0x000001a0 746f7273 00775278 436f756e 74007744 tors.wRxCount.wD
  0x000001b0 65736372 6970746f 72427566 6665724c escriptorBufferL
  0x000001c0 656e6774 68006245 6e640070 50617261 ength.bEnd.pPara
  0x000001d0 6d007749 6e747252 78450070 44657669 m.wIntrRxE.pDevi
  0x000001e0 636500                              ce.

Disassembly of section .text:

00000000 <MGC_HdrcInit>:
       0:   e1a0c00d    mov ip, sp
       4:   e92dd800    push    {fp, ip, lr, pc}
       8:   e24cb004    sub fp, ip, #4  ; 0x4
       c:   e24dd014    sub sp, sp, #20 ; 0x14
      10:   e50b001c    str r0, [fp, #-28]
      14:   e51b301c    ldr r3, [fp, #-28]
      18:   e5933000    ldr r3, [r3]
      1c:   e50b3014    str r3, [fp, #-20]
      20:   e51b3014    ldr r3, [fp, #-20]
      24:   e5933004    ldr r3, [r3, #4]
      28:   e50b3010    str r3, [fp, #-16]
      2c:   e51b0010    ldr r0, [fp, #-16]
      30:   e3a0100e    mov r1, #14 ; 0xe
      34:   e3a02000    mov r2, #0  ; 0x0
      38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            38: R_ARM_PC24  MGC_FAPI_WriteReg8
      3c:   e51b0010    ldr r0, [fp, #-16]
      40:   e3a0101f    mov r1, #31 ; 0x1f
      44:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            44: R_ARM_PC24  MGC_FAPI_ReadReg8
      48:   e1a03000    mov r3, r0
      4c:   e54b3015    strb    r3, [fp, #-21]
      50:   e55b3015    ldrb    r3, [fp, #-21]
      54:   e6af3073    sxtb    r3, r3
      58:   e3530000    cmp r3, #0  ; 0x0
      5c:   aa000002    bge 6c <MGC_HdrcInit+0x6c>
      60:   e51b301c    ldr r3, [fp, #-28]
      64:   e3a02001    mov r2, #1  ; 0x1
      68:   e5c3202f    strb    r2, [r3, #47]
      6c:   e55b3015    ldrb    r3, [fp, #-21]
      70:   e2033040    and r3, r3, #64 ; 0x40
      74:   e3530000    cmp r3, #0  ; 0x0
      78:   0a000002    beq 88 <MGC_HdrcInit+0x88>
      7c:   e51b301c    ldr r3, [fp, #-28]
      80:   e3a02001    mov r2, #1  ; 0x1
      84:   e5c3202e    strb    r2, [r3, #46]
      88:   e55b3015    ldrb    r3, [fp, #-21]
      8c:   e2033010    and r3, r3, #16 ; 0x10
      90:   e3530000    cmp r3, #0  ; 0x0
      94:   0a000002    beq a4 <MGC_HdrcInit+0xa4>
      98:   e51b301c    ldr r3, [fp, #-28]
      9c:   e3a02001    mov r2, #1  ; 0x1
      a0:   e5c32031    strb    r2, [r3, #49]
      a4:   e55b3015    ldrb    r3, [fp, #-21]
      a8:   e2033008    and r3, r3, #8  ; 0x8
      ac:   e3530000    cmp r3, #0  ; 0x0
      b0:   0a000002    beq c0 <MGC_HdrcInit+0xc0>
      b4:   e51b301c    ldr r3, [fp, #-28]
      b8:   e3a02001    mov r2, #1  ; 0x1
      bc:   e5c32030    strb    r2, [r3, #48]
      c0:   e55b3015    ldrb    r3, [fp, #-21]
      c4:   e2033004    and r3, r3, #4  ; 0x4
      c8:   e3530000    cmp r3, #0  ; 0x0
      cc:   0a00000f    beq 110 <MGC_HdrcInit+0x110>
      d0:   e59f3058    ldr r3, [pc, #88]   ; 130 <MGC_HdrcInit+0x130>
      d4:   e5d33000    ldrb    r3, [r3]
      d8:   e3530000    cmp r3, #0  ; 0x0
      dc:   0a000008    beq 104 <MGC_HdrcInit+0x104>
      e0:   e51b3014    ldr r3, [fp, #-20]
      e4:   e593300c    ldr r3, [r3, #12]
      e8:   e5932028    ldr r2, [r3, #40]
      ec:   e51b3014    ldr r3, [fp, #-20]
      f0:   e593300c    ldr r3, [r3, #12]
      f4:   e5933004    ldr r3, [r3, #4]
      f8:   e1a00003    mov r0, r3
      fc:   e59f1030    ldr r1, [pc, #48]   ; 134 <MGC_HdrcInit+0x134>
     100:   e12fff32    blx r2
     104:   e3a03000    mov r3, #0  ; 0x0
     108:   e50b3020    str r3, [fp, #-32]
     10c:   ea000003    b   120 <MGC_HdrcInit+0x120>
     110:   e51b001c    ldr r0, [fp, #-28]
     114:   ebfffffe    bl  0 <MGC_DrcInit>
            114: R_ARM_PC24 MGC_DrcInit
     118:   e1a03000    mov r3, r0
     11c:   e50b3020    str r3, [fp, #-32]
     120:   e51b3020    ldr r3, [fp, #-32]
     124:   e1a00003    mov r0, r3
     128:   e24bd00c    sub sp, fp, #12 ; 0xc
     12c:   e89da800    ldm sp, {fp, sp, pc}
     130:   00000000    .word   0x00000000
            130: R_ARM_ABS32    MGC_bDiagLevel
     134:   00000038    .word   0x00000038
            134: R_ARM_ABS32    .rodata

00000138 <MGC_HdrcDynamicFifoLocation>:
     138:   e1a0c00d    mov ip, sp
     13c:   e92dd800    push    {fp, ip, lr, pc}
     140:   e24cb004    sub fp, ip, #4  ; 0x4
     144:   e24dd010    sub sp, sp, #16 ; 0x10
     148:   e50b0010    str r0, [fp, #-16]
     14c:   e1a00003    mov r0, r3
     150:   e1a03001    mov r3, r1
     154:   e54b3014    strb    r3, [fp, #-20]
     158:   e1a03002    mov r3, r2
     15c:   e54b3018    strb    r3, [fp, #-24]
     160:   e1a03000    mov r3, r0
     164:   e54b301c    strb    r3, [fp, #-28]
     168:   e24bd00c    sub sp, fp, #12 ; 0xc
     16c:   e89da800    ldm sp, {fp, sp, pc}

00000170 <MGC_HdrcIsr>:
     170:   e1a0c00d    mov ip, sp
     174:   e92dd800    push    {fp, ip, lr, pc}
     178:   e24cb004    sub fp, ip, #4  ; 0x4
     17c:   e24dd020    sub sp, sp, #32 ; 0x20
     180:   e50b002c    str r0, [fp, #-44]
     184:   e51b302c    ldr r3, [fp, #-44]
     188:   e50b301c    str r3, [fp, #-28]
     18c:   e51b301c    ldr r3, [fp, #-28]
     190:   e5933004    ldr r3, [r3, #4]
     194:   e50b3018    str r3, [fp, #-24]
     198:   e51b3018    ldr r3, [fp, #-24]
     19c:   e5933004    ldr r3, [r3, #4]
     1a0:   e50b3014    str r3, [fp, #-20]
     1a4:   e51b0014    ldr r0, [fp, #-20]
     1a8:   e3a0100e    mov r1, #14 ; 0xe
     1ac:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            1ac: R_ARM_PC24 MGC_FAPI_ReadReg8
     1b0:   e1a03000    mov r3, r0
     1b4:   e54b300d    strb    r3, [fp, #-13]
     1b8:   e51b0014    ldr r0, [fp, #-20]
     1bc:   e3a0100a    mov r1, #10 ; 0xa
     1c0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            1c0: R_ARM_PC24 MGC_FAPI_ReadReg8
     1c4:   e1a03000    mov r3, r0
     1c8:   e54b3021    strb    r3, [fp, #-33]
     1cc:   e51b0014    ldr r0, [fp, #-20]
     1d0:   e3a01002    mov r1, #2  ; 0x2
     1d4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1d4: R_ARM_PC24 MGC_FAPI_ReadReg16
     1d8:   e1a03000    mov r3, r0
     1dc:   e14b32b0    strh    r3, [fp, #-32]
     1e0:   e51b0014    ldr r0, [fp, #-20]
     1e4:   e3a01004    mov r1, #4  ; 0x4
     1e8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1e8: R_ARM_PC24 MGC_FAPI_ReadReg16
     1ec:   e1a03000    mov r3, r0
     1f0:   e14b31be    strh    r3, [fp, #-30]
     1f4:   e55b3021    ldrb    r3, [fp, #-33]
     1f8:   e15b22b0    ldrh    r2, [fp, #-32]
     1fc:   e15bc1be    ldrh    ip, [fp, #-30]
     200:   e51b0018    ldr r0, [fp, #-24]
     204:   e1a01003    mov r1, r3
     208:   e1a0300c    mov r3, ip
     20c:   ebfffffe    bl  0 <MGC_DrcIsr>
            20c: R_ARM_PC24 MGC_DrcIsr
     210:   e1a03000    mov r3, r0
     214:   e50b3028    str r3, [fp, #-40]
     218:   e55b300d    ldrb    r3, [fp, #-13]
     21c:   e51b0014    ldr r0, [fp, #-20]
     220:   e3a0100e    mov r1, #14 ; 0xe
     224:   e1a02003    mov r2, r3
     228:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            228: R_ARM_PC24 MGC_FAPI_WriteReg8
     22c:   e51b3028    ldr r3, [fp, #-40]
     230:   e1a00003    mov r0, r3
     234:   e24bd00c    sub sp, fp, #12 ; 0xc
     238:   e89da800    ldm sp, {fp, sp, pc}

0000023c <MGC_HdrcStart>:
     23c:   e1a0c00d    mov ip, sp
     240:   e92dd800    push    {fp, ip, lr, pc}
     244:   e24cb004    sub fp, ip, #4  ; 0x4
     248:   e24dd014    sub sp, sp, #20 ; 0x14
     24c:   e50b001c    str r0, [fp, #-28]
     250:   e51b301c    ldr r3, [fp, #-28]
     254:   e5933000    ldr r3, [r3]
     258:   e50b3014    str r3, [fp, #-20]
     25c:   e51b301c    ldr r3, [fp, #-28]
     260:   e5933014    ldr r3, [r3, #20]
     264:   e50b3010    str r3, [fp, #-16]
     268:   e51b3010    ldr r3, [fp, #-16]
     26c:   e1d334b2    ldrh    r3, [r3, #66]
     270:   e14b31b6    strh    r3, [fp, #-22]
     274:   e59f2200    ldr r2, [pc, #512]  ; 47c <MGC_HdrcStart+0x240>
     278:   e3a03000    mov r3, #0  ; 0x0
     27c:   e5c23000    strb    r3, [r2]
     280:   e51b301c    ldr r3, [fp, #-28]
     284:   e5933008    ldr r3, [r3, #8]
     288:   e5933004    ldr r3, [r3, #4]
     28c:   e59f01e8    ldr r0, [pc, #488]  ; 47c <MGC_HdrcStart+0x240>
     290:   e3a01c01    mov r1, #256    ; 0x100
     294:   e59f21e4    ldr r2, [pc, #484]  ; 480 <MGC_HdrcStart+0x244>
     298:   e12fff33    blx r3
     29c:   e51b301c    ldr r3, [fp, #-28]
     2a0:   e5933008    ldr r3, [r3, #8]
     2a4:   e593c008    ldr ip, [r3, #8]
     2a8:   e15b21b6    ldrh    r2, [fp, #-22]
     2ac:   e3a03004    mov r3, #4  ; 0x4
     2b0:   e58d3000    str r3, [sp]
     2b4:   e59f01c0    ldr r0, [pc, #448]  ; 47c <MGC_HdrcStart+0x240>
     2b8:   e3a01c01    mov r1, #256    ; 0x100
     2bc:   e3a03010    mov r3, #16 ; 0x10
     2c0:   e12fff3c    blx ip
     2c4:   e59f31b8    ldr r3, [pc, #440]  ; 484 <MGC_HdrcStart+0x248>
     2c8:   e5d33000    ldrb    r3, [r3]
     2cc:   e3530002    cmp r3, #2  ; 0x2
     2d0:   9a000008    bls 2f8 <MGC_HdrcStart+0xbc>
     2d4:   e51b301c    ldr r3, [fp, #-28]
     2d8:   e593300c    ldr r3, [r3, #12]
     2dc:   e5932028    ldr r2, [r3, #40]
     2e0:   e51b301c    ldr r3, [fp, #-28]
     2e4:   e593300c    ldr r3, [r3, #12]
     2e8:   e5933004    ldr r3, [r3, #4]
     2ec:   e1a00003    mov r0, r3
     2f0:   e59f1184    ldr r1, [pc, #388]  ; 47c <MGC_HdrcStart+0x240>
     2f4:   e12fff32    blx r2
     2f8:   e15b31b6    ldrh    r3, [fp, #-22]
     2fc:   e51b0014    ldr r0, [fp, #-20]
     300:   e3a01006    mov r1, #6  ; 0x6
     304:   e1a02003    mov r2, r3
     308:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            308: R_ARM_PC24 MGC_FAPI_WriteReg16
     30c:   e51b3010    ldr r3, [fp, #-16]
     310:   e1d324b2    ldrh    r2, [r3, #66]
     314:   e3e03001    mvn r3, #1  ; 0x1
     318:   e0023003    and r3, r2, r3
     31c:   e14b31b6    strh    r3, [fp, #-22]
     320:   e59f2154    ldr r2, [pc, #340]  ; 47c <MGC_HdrcStart+0x240>
     324:   e3a03000    mov r3, #0  ; 0x0
     328:   e5c23000    strb    r3, [r2]
     32c:   e51b301c    ldr r3, [fp, #-28]
     330:   e5933008    ldr r3, [r3, #8]
     334:   e5933004    ldr r3, [r3, #4]
     338:   e59f013c    ldr r0, [pc, #316]  ; 47c <MGC_HdrcStart+0x240>
     33c:   e3a01c01    mov r1, #256    ; 0x100
     340:   e59f2140    ldr r2, [pc, #320]  ; 488 <MGC_HdrcStart+0x24c>
     344:   e12fff33    blx r3
     348:   e51b301c    ldr r3, [fp, #-28]
     34c:   e5933008    ldr r3, [r3, #8]
     350:   e593c008    ldr ip, [r3, #8]
     354:   e15b21b6    ldrh    r2, [fp, #-22]
     358:   e3a03004    mov r3, #4  ; 0x4
     35c:   e58d3000    str r3, [sp]
     360:   e59f0114    ldr r0, [pc, #276]  ; 47c <MGC_HdrcStart+0x240>
     364:   e3a01c01    mov r1, #256    ; 0x100
     368:   e3a03010    mov r3, #16 ; 0x10
     36c:   e12fff3c    blx ip
     370:   e59f310c    ldr r3, [pc, #268]  ; 484 <MGC_HdrcStart+0x248>
     374:   e5d33000    ldrb    r3, [r3]
     378:   e3530002    cmp r3, #2  ; 0x2
     37c:   9a000008    bls 3a4 <MGC_HdrcStart+0x168>
     380:   e51b301c    ldr r3, [fp, #-28]
     384:   e593300c    ldr r3, [r3, #12]
     388:   e5932028    ldr r2, [r3, #40]
     38c:   e51b301c    ldr r3, [fp, #-28]
     390:   e593300c    ldr r3, [r3, #12]
     394:   e5933004    ldr r3, [r3, #4]
     398:   e1a00003    mov r0, r3
     39c:   e59f10d8    ldr r1, [pc, #216]  ; 47c <MGC_HdrcStart+0x240>
     3a0:   e12fff32    blx r2
     3a4:   e15b31b6    ldrh    r3, [fp, #-22]
     3a8:   e51b0014    ldr r0, [fp, #-20]
     3ac:   e3a01008    mov r1, #8  ; 0x8
     3b0:   e1a02003    mov r2, r3
     3b4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3b4: R_ARM_PC24 MGC_FAPI_WriteReg16
     3b8:   e3e03008    mvn r3, #8  ; 0x8
     3bc:   e14b31b6    strh    r3, [fp, #-22]
     3c0:   e59f20b4    ldr r2, [pc, #180]  ; 47c <MGC_HdrcStart+0x240>
     3c4:   e3a03000    mov r3, #0  ; 0x0
     3c8:   e5c23000    strb    r3, [r2]
     3cc:   e51b301c    ldr r3, [fp, #-28]
     3d0:   e5933008    ldr r3, [r3, #8]
     3d4:   e5933004    ldr r3, [r3, #4]
     3d8:   e59f009c    ldr r0, [pc, #156]  ; 47c <MGC_HdrcStart+0x240>
     3dc:   e3a01c01    mov r1, #256    ; 0x100
     3e0:   e59f20a4    ldr r2, [pc, #164]  ; 48c <MGC_HdrcStart+0x250>
     3e4:   e12fff33    blx r3
     3e8:   e51b301c    ldr r3, [fp, #-28]
     3ec:   e5933008    ldr r3, [r3, #8]
     3f0:   e593c008    ldr ip, [r3, #8]
     3f4:   e15b31b6    ldrh    r3, [fp, #-22]
     3f8:   e20320ff    and r2, r3, #255    ; 0xff
     3fc:   e3a03002    mov r3, #2  ; 0x2
     400:   e58d3000    str r3, [sp]
     404:   e59f0070    ldr r0, [pc, #112]  ; 47c <MGC_HdrcStart+0x240>
     408:   e3a01c01    mov r1, #256    ; 0x100
     40c:   e3a03010    mov r3, #16 ; 0x10
     410:   e12fff3c    blx ip
     414:   e59f3068    ldr r3, [pc, #104]  ; 484 <MGC_HdrcStart+0x248>
     418:   e5d33000    ldrb    r3, [r3]
     41c:   e3530002    cmp r3, #2  ; 0x2
     420:   9a000008    bls 448 <MGC_HdrcStart+0x20c>
     424:   e51b301c    ldr r3, [fp, #-28]
     428:   e593300c    ldr r3, [r3, #12]
     42c:   e5932028    ldr r2, [r3, #40]
     430:   e51b301c    ldr r3, [fp, #-28]
     434:   e593300c    ldr r3, [r3, #12]
     438:   e5933004    ldr r3, [r3, #4]
     43c:   e1a00003    mov r0, r3
     440:   e59f1034    ldr r1, [pc, #52]   ; 47c <MGC_HdrcStart+0x240>
     444:   e12fff32    blx r2
     448:   e15b31b6    ldrh    r3, [fp, #-22]
     44c:   e6ef3073    uxtb    r3, r3
     450:   e51b0014    ldr r0, [fp, #-20]
     454:   e3a0100b    mov r1, #11 ; 0xb
     458:   e1a02003    mov r2, r3
     45c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            45c: R_ARM_PC24 MGC_FAPI_WriteReg8
     460:   e51b2010    ldr r2, [fp, #-16]
     464:   e3a03000    mov r3, #0  ; 0x0
     468:   e5c2304c    strb    r3, [r2, #76]
     46c:   e3a03000    mov r3, #0  ; 0x0
     470:   e1a00003    mov r0, r3
     474:   e24bd00c    sub sp, fp, #12 ; 0xc
     478:   e89da800    ldm sp, {fp, sp, pc}
     47c:   00000000    .word   0x00000000
            47c: R_ARM_ABS32    MGC_DiagMsg
     480:   00000078    .word   0x00000078
            480: R_ARM_ABS32    .rodata
     484:   00000000    .word   0x00000000
            484: R_ARM_ABS32    MGC_bDiagLevel
     488:   0000008c    .word   0x0000008c
            488: R_ARM_ABS32    .rodata
     48c:   000000a0    .word   0x000000a0
            48c: R_ARM_ABS32    .rodata

00000490 <MGC_HdrcStop>:
     490:   e1a0c00d    mov ip, sp
     494:   e92dd800    push    {fp, ip, lr, pc}
     498:   e24cb004    sub fp, ip, #4  ; 0x4
     49c:   e24dd00c    sub sp, sp, #12 ; 0xc
     4a0:   e50b0018    str r0, [fp, #-24]
     4a4:   e51b3018    ldr r3, [fp, #-24]
     4a8:   e5933004    ldr r3, [r3, #4]
     4ac:   e50b3010    str r3, [fp, #-16]
     4b0:   e51b0010    ldr r0, [fp, #-16]
     4b4:   e3a0100b    mov r1, #11 ; 0xb
     4b8:   e3a02000    mov r2, #0  ; 0x0
     4bc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4bc: R_ARM_PC24 MGC_FAPI_WriteReg8
     4c0:   e51b0010    ldr r0, [fp, #-16]
     4c4:   e3a01006    mov r1, #6  ; 0x6
     4c8:   e3a02000    mov r2, #0  ; 0x0
     4cc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            4cc: R_ARM_PC24 MGC_FAPI_WriteReg16
     4d0:   e51b0010    ldr r0, [fp, #-16]
     4d4:   e3a01008    mov r1, #8  ; 0x8
     4d8:   e3a02000    mov r2, #0  ; 0x0
     4dc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            4dc: R_ARM_PC24 MGC_FAPI_WriteReg16
     4e0:   e51b0010    ldr r0, [fp, #-16]
     4e4:   e3a0100a    mov r1, #10 ; 0xa
     4e8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            4e8: R_ARM_PC24 MGC_FAPI_ReadReg8
     4ec:   e1a03000    mov r3, r0
     4f0:   e14b31b2    strh    r3, [fp, #-18]
     4f4:   e51b0010    ldr r0, [fp, #-16]
     4f8:   e3a01002    mov r1, #2  ; 0x2
     4fc:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            4fc: R_ARM_PC24 MGC_FAPI_ReadReg16
     500:   e1a03000    mov r3, r0
     504:   e14b31b2    strh    r3, [fp, #-18]
     508:   e51b0010    ldr r0, [fp, #-16]
     50c:   e3a01004    mov r1, #4  ; 0x4
     510:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            510: R_ARM_PC24 MGC_FAPI_ReadReg16
     514:   e1a03000    mov r3, r0
     518:   e14b31b2    strh    r3, [fp, #-18]
     51c:   e3a03000    mov r3, #0  ; 0x0
     520:   e1a00003    mov r0, r3
     524:   e24bd00c    sub sp, fp, #12 ; 0xc
     528:   e89da800    ldm sp, {fp, sp, pc}

0000052c <MGC_HdrcDestroy>:
     52c:   e1a0c00d    mov ip, sp
     530:   e92dd800    push    {fp, ip, lr, pc}
     534:   e24cb004    sub fp, ip, #4  ; 0x4
     538:   e24dd008    sub sp, sp, #8  ; 0x8
     53c:   e50b0014    str r0, [fp, #-20]
     540:   e51b3014    ldr r3, [fp, #-20]
     544:   e5933014    ldr r3, [r3, #20]
     548:   e50b3010    str r3, [fp, #-16]
     54c:   e51b0010    ldr r0, [fp, #-16]
     550:   ebfffffe    bl  0 <MGC_HostDestroy>
            550: R_ARM_PC24 MGC_HostDestroy
     554:   e51b3010    ldr r3, [fp, #-16]
     558:   e5933008    ldr r3, [r3, #8]
     55c:   e3530000    cmp r3, #0  ; 0x0
     560:   0a000005    beq 57c <MGC_HdrcDestroy+0x50>
     564:   e59f3028    ldr r3, [pc, #40]   ; 594 <MGC_HdrcDestroy+0x68>
     568:   e593204c    ldr r2, [r3, #76]
     56c:   e51b3010    ldr r3, [fp, #-16]
     570:   e5933008    ldr r3, [r3, #8]
     574:   e1a00003    mov r0, r3
     578:   e12fff32    blx r2
     57c:   e51b0010    ldr r0, [fp, #-16]
     580:   ebfffffe    bl  0 <MGC_FunctionDestroy>
            580: R_ARM_PC24 MGC_FunctionDestroy
     584:   e1a03000    mov r3, r0
     588:   e1a00003    mov r0, r3
     58c:   e24bd00c    sub sp, fp, #12 ; 0xc
     590:   e89da800    ldm sp, {fp, sp, pc}
     594:   00000000    .word   0x00000000
            594: R_ARM_ABS32    FAPI_SYS_Services

00000598 <MGC_HdrcReadBusState>:
     598:   e1a0c00d    mov ip, sp
     59c:   e92dd800    push    {fp, ip, lr, pc}
     5a0:   e24cb004    sub fp, ip, #4  ; 0x4
     5a4:   e24dd010    sub sp, sp, #16 ; 0x10
     5a8:   e50b001c    str r0, [fp, #-28]
     5ac:   e51b301c    ldr r3, [fp, #-28]
     5b0:   e5933000    ldr r3, [r3]
     5b4:   e50b3014    str r3, [fp, #-20]
     5b8:   e51b3014    ldr r3, [fp, #-20]
     5bc:   e5933004    ldr r3, [r3, #4]
     5c0:   e50b3010    str r3, [fp, #-16]
     5c4:   e51b0010    ldr r0, [fp, #-16]
     5c8:   e3a01060    mov r1, #96 ; 0x60
     5cc:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            5cc: R_ARM_PC24 MGC_FAPI_ReadReg8
     5d0:   e1a03000    mov r3, r0
     5d4:   e54b3017    strb    r3, [fp, #-23]
     5d8:   e55b3017    ldrb    r3, [fp, #-23]
     5dc:   e2033001    and r3, r3, #1  ; 0x1
     5e0:   e6ef3073    uxtb    r3, r3
     5e4:   e3530000    cmp r3, #0  ; 0x0
     5e8:   0a000006    beq 608 <MGC_HdrcReadBusState+0x70>
     5ec:   e55b3017    ldrb    r3, [fp, #-23]
     5f0:   e2033080    and r3, r3, #128    ; 0x80
     5f4:   e1a033c3    asr r3, r3, #7
     5f8:   e6ef3073    uxtb    r3, r3
     5fc:   e51b201c    ldr r2, [fp, #-28]
     600:   e5c2303f    strb    r3, [r2, #63]
     604:   ea000002    b   614 <MGC_HdrcReadBusState+0x7c>
     608:   e51b301c    ldr r3, [fp, #-28]
     60c:   e3a02002    mov r2, #2  ; 0x2
     610:   e5c3203f    strb    r2, [r3, #63]
     614:   e55b3017    ldrb    r3, [fp, #-23]
     618:   e2033001    and r3, r3, #1  ; 0x1
     61c:   e51b201c    ldr r2, [fp, #-28]
     620:   e5c2303d    strb    r3, [r2, #61]
     624:   e55b3017    ldrb    r3, [fp, #-23]
     628:   e2033004    and r3, r3, #4  ; 0x4
     62c:   e3530000    cmp r3, #0  ; 0x0
     630:   03a03000    moveq   r3, #0  ; 0x0
     634:   13a03001    movne   r3, #1  ; 0x1
     638:   e51b201c    ldr r2, [fp, #-28]
     63c:   e5c2303e    strb    r3, [r2, #62]
     640:   e55b3017    ldrb    r3, [fp, #-23]
     644:   e2033020    and r3, r3, #32 ; 0x20
     648:   e3530000    cmp r3, #0  ; 0x0
     64c:   03a03000    moveq   r3, #0  ; 0x0
     650:   13a03001    movne   r3, #1  ; 0x1
     654:   e51b201c    ldr r2, [fp, #-28]
     658:   e5c23039    strb    r3, [r2, #57]
     65c:   e51b0010    ldr r0, [fp, #-16]
     660:   e3a01001    mov r1, #1  ; 0x1
     664:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            664: R_ARM_PC24 MGC_FAPI_ReadReg8
     668:   e1a03000    mov r3, r0
     66c:   e54b3016    strb    r3, [fp, #-22]
     670:   e55b3016    ldrb    r3, [fp, #-22]
     674:   e2033010    and r3, r3, #16 ; 0x10
     678:   e3530000    cmp r3, #0  ; 0x0
     67c:   03a03000    moveq   r3, #0  ; 0x0
     680:   13a03001    movne   r3, #1  ; 0x1
     684:   e51b201c    ldr r2, [fp, #-28]
     688:   e5c23038    strb    r3, [r2, #56]
     68c:   e55b3016    ldrb    r3, [fp, #-22]
     690:   e2033002    and r3, r3, #2  ; 0x2
     694:   e3530000    cmp r3, #0  ; 0x0
     698:   03a03000    moveq   r3, #0  ; 0x0
     69c:   13a03001    movne   r3, #1  ; 0x1
     6a0:   e51b201c    ldr r2, [fp, #-28]
     6a4:   e5c2303b    strb    r3, [r2, #59]
     6a8:   e55b3016    ldrb    r3, [fp, #-22]
     6ac:   e2033008    and r3, r3, #8  ; 0x8
     6b0:   e3530000    cmp r3, #0  ; 0x0
     6b4:   03a03000    moveq   r3, #0  ; 0x0
     6b8:   13a03001    movne   r3, #1  ; 0x1
     6bc:   e51b201c    ldr r2, [fp, #-28]
     6c0:   e5c2303a    strb    r3, [r2, #58]
     6c4:   e55b3016    ldrb    r3, [fp, #-22]
     6c8:   e2033004    and r3, r3, #4  ; 0x4
     6cc:   e3530000    cmp r3, #0  ; 0x0
     6d0:   03a03000    moveq   r3, #0  ; 0x0
     6d4:   13a03001    movne   r3, #1  ; 0x1
     6d8:   e51b201c    ldr r2, [fp, #-28]
     6dc:   e5c2303c    strb    r3, [r2, #60]
     6e0:   e55b3017    ldrb    r3, [fp, #-23]
     6e4:   e2033018    and r3, r3, #24 ; 0x18
     6e8:   e1a031c3    asr r3, r3, #3
     6ec:   e54b3015    strb    r3, [fp, #-21]
     6f0:   e55b3015    ldrb    r3, [fp, #-21]
     6f4:   e3530003    cmp r3, #3  ; 0x3
     6f8:   979ff103    ldrls   pc, [pc, r3, lsl #2]
     6fc:   ea000013    b   750 <MGC_HdrcReadBusState+0x1b8>
     700:   00000710    .word   0x00000710
            700: R_ARM_ABS32    .text
     704:   00000720    .word   0x00000720
            704: R_ARM_ABS32    .text
     708:   00000730    .word   0x00000730
            708: R_ARM_ABS32    .text
     70c:   00000740    .word   0x00000740
            70c: R_ARM_ABS32    .text
     710:   e51b301c    ldr r3, [fp, #-28]
     714:   e3a02000    mov r2, #0  ; 0x0
     718:   e5c32040    strb    r2, [r3, #64]
     71c:   ea00000e    b   75c <MGC_HdrcReadBusState+0x1c4>
     720:   e51b301c    ldr r3, [fp, #-28]
     724:   e3a02001    mov r2, #1  ; 0x1
     728:   e5c32040    strb    r2, [r3, #64]
     72c:   ea00000a    b   75c <MGC_HdrcReadBusState+0x1c4>
     730:   e51b301c    ldr r3, [fp, #-28]
     734:   e3a02002    mov r2, #2  ; 0x2
     738:   e5c32040    strb    r2, [r3, #64]
     73c:   ea000006    b   75c <MGC_HdrcReadBusState+0x1c4>
     740:   e51b301c    ldr r3, [fp, #-28]
     744:   e3a02003    mov r2, #3  ; 0x3
     748:   e5c32040    strb    r2, [r3, #64]
     74c:   ea000002    b   75c <MGC_HdrcReadBusState+0x1c4>
     750:   e51b301c    ldr r3, [fp, #-28]
     754:   e3e02000    mvn r2, #0  ; 0x0
     758:   e5c32040    strb    r2, [r3, #64]
     75c:   e51b0010    ldr r0, [fp, #-16]
     760:   e3a0100c    mov r1, #12 ; 0xc
     764:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            764: R_ARM_PC24 MGC_FAPI_ReadReg16
     768:   e1a03000    mov r3, r0
     76c:   e1a02003    mov r2, r3
     770:   e51b301c    ldr r3, [fp, #-28]
     774:   e5832034    str r2, [r3, #52]
     778:   e3a03000    mov r3, #0  ; 0x0
     77c:   e1a00003    mov r0, r3
     780:   e24bd00c    sub sp, fp, #12 ; 0xc
     784:   e89da800    ldm sp, {fp, sp, pc}

00000788 <MGC_HdrcProgramBusState>:
     788:   e1a0c00d    mov ip, sp
     78c:   e92dd800    push    {fp, ip, lr, pc}
     790:   e24cb004    sub fp, ip, #4  ; 0x4
     794:   e24dd018    sub sp, sp, #24 ; 0x18
     798:   e50b0020    str r0, [fp, #-32]
     79c:   e51b3020    ldr r3, [fp, #-32]
     7a0:   e5933000    ldr r3, [r3]
     7a4:   e50b301c    str r3, [fp, #-28]
     7a8:   e51b301c    ldr r3, [fp, #-28]
     7ac:   e5933004    ldr r3, [r3, #4]
     7b0:   e50b3018    str r3, [fp, #-24]
     7b4:   e51b0018    ldr r0, [fp, #-24]
     7b8:   e3a01001    mov r1, #1  ; 0x1
     7bc:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            7bc: R_ARM_PC24 MGC_FAPI_ReadReg8
     7c0:   e1a03000    mov r3, r0
     7c4:   e54b3011    strb    r3, [fp, #-17]
     7c8:   e55b3011    ldrb    r3, [fp, #-17]
     7cc:   e54b3010    strb    r3, [fp, #-16]
     7d0:   e51b0018    ldr r0, [fp, #-24]
     7d4:   e3a01060    mov r1, #96 ; 0x60
     7d8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            7d8: R_ARM_PC24 MGC_FAPI_ReadReg8
     7dc:   e1a03000    mov r3, r0
     7e0:   e54b300f    strb    r3, [fp, #-15]
     7e4:   e55b300f    ldrb    r3, [fp, #-15]
     7e8:   e54b300e    strb    r3, [fp, #-14]
     7ec:   e51b0018    ldr r0, [fp, #-24]
     7f0:   e3a01000    mov r1, #0  ; 0x0
     7f4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            7f4: R_ARM_PC24 MGC_FAPI_ReadReg8
     7f8:   e1a03000    mov r3, r0
     7fc:   e54b300d    strb    r3, [fp, #-13]
     800:   e51b3020    ldr r3, [fp, #-32]
     804:   e5d33025    ldrb    r3, [r3, #37]
     808:   e3530000    cmp r3, #0  ; 0x0
     80c:   0a000003    beq 820 <MGC_HdrcProgramBusState+0x98>
     810:   e55b3010    ldrb    r3, [fp, #-16]
     814:   e3833008    orr r3, r3, #8  ; 0x8
     818:   e54b3010    strb    r3, [fp, #-16]
     81c:   ea000002    b   82c <MGC_HdrcProgramBusState+0xa4>
     820:   e55b3010    ldrb    r3, [fp, #-16]
     824:   e3c33008    bic r3, r3, #8  ; 0x8
     828:   e54b3010    strb    r3, [fp, #-16]
     82c:   e51b3020    ldr r3, [fp, #-32]
     830:   e5d33027    ldrb    r3, [r3, #39]
     834:   e3530000    cmp r3, #0  ; 0x0
     838:   0a000003    beq 84c <MGC_HdrcProgramBusState+0xc4>
     83c:   e55b3010    ldrb    r3, [fp, #-16]
     840:   e3833004    orr r3, r3, #4  ; 0x4
     844:   e54b3010    strb    r3, [fp, #-16]
     848:   ea000002    b   858 <MGC_HdrcProgramBusState+0xd0>
     84c:   e55b3010    ldrb    r3, [fp, #-16]
     850:   e3c33004    bic r3, r3, #4  ; 0x4
     854:   e54b3010    strb    r3, [fp, #-16]
     858:   e51b3020    ldr r3, [fp, #-32]
     85c:   e5d33026    ldrb    r3, [r3, #38]
     860:   e3530000    cmp r3, #0  ; 0x0
     864:   0a00000b    beq 898 <MGC_HdrcProgramBusState+0x110>
     868:   e51b3020    ldr r3, [fp, #-32]
     86c:   e5d3303e    ldrb    r3, [r3, #62]
     870:   e3530000    cmp r3, #0  ; 0x0
     874:   0a000003    beq 888 <MGC_HdrcProgramBusState+0x100>
     878:   e55b3010    ldrb    r3, [fp, #-16]
     87c:   e3833002    orr r3, r3, #2  ; 0x2
     880:   e54b3010    strb    r3, [fp, #-16]
     884:   ea000009    b   8b0 <MGC_HdrcProgramBusState+0x128>
     888:   e55b3010    ldrb    r3, [fp, #-16]
     88c:   e3833001    orr r3, r3, #1  ; 0x1
     890:   e54b3010    strb    r3, [fp, #-16]
     894:   ea000005    b   8b0 <MGC_HdrcProgramBusState+0x128>
     898:   e55b3010    ldrb    r3, [fp, #-16]
     89c:   e3c33002    bic r3, r3, #2  ; 0x2
     8a0:   e54b3010    strb    r3, [fp, #-16]
     8a4:   e55b3010    ldrb    r3, [fp, #-16]
     8a8:   e3c33001    bic r3, r3, #1  ; 0x1
     8ac:   e54b3010    strb    r3, [fp, #-16]
     8b0:   e51b3020    ldr r3, [fp, #-32]
     8b4:   e5d33024    ldrb    r3, [r3, #36]
     8b8:   e3530000    cmp r3, #0  ; 0x0
     8bc:   0a000003    beq 8d0 <MGC_HdrcProgramBusState+0x148>
     8c0:   e55b3010    ldrb    r3, [fp, #-16]
     8c4:   e3833020    orr r3, r3, #32 ; 0x20
     8c8:   e54b3010    strb    r3, [fp, #-16]
     8cc:   ea000002    b   8dc <MGC_HdrcProgramBusState+0x154>
     8d0:   e55b3010    ldrb    r3, [fp, #-16]
     8d4:   e3c33020    bic r3, r3, #32 ; 0x20
     8d8:   e54b3010    strb    r3, [fp, #-16]
     8dc:   e51b3020    ldr r3, [fp, #-32]
     8e0:   e5d33029    ldrb    r3, [r3, #41]
     8e4:   e3530000    cmp r3, #0  ; 0x0
     8e8:   0a000003    beq 8fc <MGC_HdrcProgramBusState+0x174>
     8ec:   e55b300e    ldrb    r3, [fp, #-14]
     8f0:   e3833001    orr r3, r3, #1  ; 0x1
     8f4:   e54b300e    strb    r3, [fp, #-14]
     8f8:   ea000002    b   908 <MGC_HdrcProgramBusState+0x180>
     8fc:   e55b300e    ldrb    r3, [fp, #-14]
     900:   e3c33001    bic r3, r3, #1  ; 0x1
     904:   e54b300e    strb    r3, [fp, #-14]
     908:   e51b3020    ldr r3, [fp, #-32]
     90c:   e5d3302a    ldrb    r3, [r3, #42]
     910:   e3530000    cmp r3, #0  ; 0x0
     914:   0a000003    beq 928 <MGC_HdrcProgramBusState+0x1a0>
     918:   e55b300e    ldrb    r3, [fp, #-14]
     91c:   e3833002    orr r3, r3, #2  ; 0x2
     920:   e54b300e    strb    r3, [fp, #-14]
     924:   ea000002    b   934 <MGC_HdrcProgramBusState+0x1ac>
     928:   e55b300e    ldrb    r3, [fp, #-14]
     92c:   e3c33002    bic r3, r3, #2  ; 0x2
     930:   e54b300e    strb    r3, [fp, #-14]
     934:   e51b3020    ldr r3, [fp, #-32]
     938:   e5d32028    ldrb    r2, [r3, #40]
     93c:   e55b300d    ldrb    r3, [fp, #-13]
     940:   e1530002    cmp r3, r2
     944:   0a000028    beq 9ec <MGC_HdrcProgramBusState+0x264>
     948:   e59f21fc    ldr r2, [pc, #508]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     94c:   e3a03000    mov r3, #0  ; 0x0
     950:   e5c23000    strb    r3, [r2]
     954:   e51b301c    ldr r3, [fp, #-28]
     958:   e5933008    ldr r3, [r3, #8]
     95c:   e5933004    ldr r3, [r3, #4]
     960:   e59f01e4    ldr r0, [pc, #484]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     964:   e3a01c01    mov r1, #256    ; 0x100
     968:   e59f21e0    ldr r2, [pc, #480]  ; b50 <MGC_HdrcProgramBusState+0x3c8>
     96c:   e12fff33    blx r3
     970:   e51b301c    ldr r3, [fp, #-28]
     974:   e5933008    ldr r3, [r3, #8]
     978:   e593c008    ldr ip, [r3, #8]
     97c:   e51b3020    ldr r3, [fp, #-32]
     980:   e5d33028    ldrb    r3, [r3, #40]
     984:   e1a02003    mov r2, r3
     988:   e3a03000    mov r3, #0  ; 0x0
     98c:   e58d3000    str r3, [sp]
     990:   e59f01b4    ldr r0, [pc, #436]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     994:   e3a01c01    mov r1, #256    ; 0x100
     998:   e3a0300a    mov r3, #10 ; 0xa
     99c:   e12fff3c    blx ip
     9a0:   e59f31ac    ldr r3, [pc, #428]  ; b54 <MGC_HdrcProgramBusState+0x3cc>
     9a4:   e5d33000    ldrb    r3, [r3]
     9a8:   e3530002    cmp r3, #2  ; 0x2
     9ac:   9a000008    bls 9d4 <MGC_HdrcProgramBusState+0x24c>
     9b0:   e51b301c    ldr r3, [fp, #-28]
     9b4:   e593300c    ldr r3, [r3, #12]
     9b8:   e5932028    ldr r2, [r3, #40]
     9bc:   e51b301c    ldr r3, [fp, #-28]
     9c0:   e593300c    ldr r3, [r3, #12]
     9c4:   e5933004    ldr r3, [r3, #4]
     9c8:   e1a00003    mov r0, r3
     9cc:   e59f1178    ldr r1, [pc, #376]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     9d0:   e12fff32    blx r2
     9d4:   e51b3020    ldr r3, [fp, #-32]
     9d8:   e5d33028    ldrb    r3, [r3, #40]
     9dc:   e51b0018    ldr r0, [fp, #-24]
     9e0:   e3a01000    mov r1, #0  ; 0x0
     9e4:   e1a02003    mov r2, r3
     9e8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            9e8: R_ARM_PC24 MGC_FAPI_WriteReg8
     9ec:   e55b2010    ldrb    r2, [fp, #-16]
     9f0:   e55b3011    ldrb    r3, [fp, #-17]
     9f4:   e1520003    cmp r2, r3
     9f8:   0a000025    beq a94 <MGC_HdrcProgramBusState+0x30c>
     9fc:   e59f2148    ldr r2, [pc, #328]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     a00:   e3a03000    mov r3, #0  ; 0x0
     a04:   e5c23000    strb    r3, [r2]
     a08:   e51b301c    ldr r3, [fp, #-28]
     a0c:   e5933008    ldr r3, [r3, #8]
     a10:   e5933004    ldr r3, [r3, #4]
     a14:   e59f0130    ldr r0, [pc, #304]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     a18:   e3a01c01    mov r1, #256    ; 0x100
     a1c:   e59f2134    ldr r2, [pc, #308]  ; b58 <MGC_HdrcProgramBusState+0x3d0>
     a20:   e12fff33    blx r3
     a24:   e51b301c    ldr r3, [fp, #-28]
     a28:   e5933008    ldr r3, [r3, #8]
     a2c:   e593c008    ldr ip, [r3, #8]
     a30:   e55b2010    ldrb    r2, [fp, #-16]
     a34:   e3a03002    mov r3, #2  ; 0x2
     a38:   e58d3000    str r3, [sp]
     a3c:   e59f0108    ldr r0, [pc, #264]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     a40:   e3a01c01    mov r1, #256    ; 0x100
     a44:   e3a03010    mov r3, #16 ; 0x10
     a48:   e12fff3c    blx ip
     a4c:   e59f3100    ldr r3, [pc, #256]  ; b54 <MGC_HdrcProgramBusState+0x3cc>
     a50:   e5d33000    ldrb    r3, [r3]
     a54:   e3530002    cmp r3, #2  ; 0x2
     a58:   9a000008    bls a80 <MGC_HdrcProgramBusState+0x2f8>
     a5c:   e51b301c    ldr r3, [fp, #-28]
     a60:   e593300c    ldr r3, [r3, #12]
     a64:   e5932028    ldr r2, [r3, #40]
     a68:   e51b301c    ldr r3, [fp, #-28]
     a6c:   e593300c    ldr r3, [r3, #12]
     a70:   e5933004    ldr r3, [r3, #4]
     a74:   e1a00003    mov r0, r3
     a78:   e59f10cc    ldr r1, [pc, #204]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     a7c:   e12fff32    blx r2
     a80:   e55b3010    ldrb    r3, [fp, #-16]
     a84:   e51b0018    ldr r0, [fp, #-24]
     a88:   e3a01001    mov r1, #1  ; 0x1
     a8c:   e1a02003    mov r2, r3
     a90:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            a90: R_ARM_PC24 MGC_FAPI_WriteReg8
     a94:   e55b200e    ldrb    r2, [fp, #-14]
     a98:   e55b300f    ldrb    r3, [fp, #-15]
     a9c:   e1520003    cmp r2, r3
     aa0:   0a000025    beq b3c <MGC_HdrcProgramBusState+0x3b4>
     aa4:   e59f20a0    ldr r2, [pc, #160]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     aa8:   e3a03000    mov r3, #0  ; 0x0
     aac:   e5c23000    strb    r3, [r2]
     ab0:   e51b301c    ldr r3, [fp, #-28]
     ab4:   e5933008    ldr r3, [r3, #8]
     ab8:   e5933004    ldr r3, [r3, #4]
     abc:   e59f0088    ldr r0, [pc, #136]  ; b4c <MGC_HdrcProgramBusState+0x3c4>
     ac0:   e3a01c01    mov r1, #256    ; 0x100
     ac4:   e59f2090    ldr r2, [pc, #144]  ; b5c <MGC_HdrcProgramBusState+0x3d4>
     ac8:   e12fff33    blx r3
     acc:   e51b301c    ldr r3, [fp, #-28]
     ad0:   e5933008    ldr r3, [r3, #8]
     ad4:   e593c008    ldr ip, [r3, #8]
     ad8:   e55b200e    ldrb    r2, [fp, #-14]
     adc:   e3a03002    mov r3, #2  ; 0x2
     ae0:   e58d3000    str r3, [sp]
     ae4:   e59f0060    ldr r0, [pc, #96]   ; b4c <MGC_HdrcProgramBusState+0x3c4>
     ae8:   e3a01c01    mov r1, #256    ; 0x100
     aec:   e3a03010    mov r3, #16 ; 0x10
     af0:   e12fff3c    blx ip
     af4:   e59f3058    ldr r3, [pc, #88]   ; b54 <MGC_HdrcProgramBusState+0x3cc>
     af8:   e5d33000    ldrb    r3, [r3]
     afc:   e3530002    cmp r3, #2  ; 0x2
     b00:   9a000008    bls b28 <MGC_HdrcProgramBusState+0x3a0>
     b04:   e51b301c    ldr r3, [fp, #-28]
     b08:   e593300c    ldr r3, [r3, #12]
     b0c:   e5932028    ldr r2, [r3, #40]
     b10:   e51b301c    ldr r3, [fp, #-28]
     b14:   e593300c    ldr r3, [r3, #12]
     b18:   e5933004    ldr r3, [r3, #4]
     b1c:   e1a00003    mov r0, r3
     b20:   e59f1024    ldr r1, [pc, #36]   ; b4c <MGC_HdrcProgramBusState+0x3c4>
     b24:   e12fff32    blx r2
     b28:   e55b300e    ldrb    r3, [fp, #-14]
     b2c:   e51b0018    ldr r0, [fp, #-24]
     b30:   e3a01060    mov r1, #96 ; 0x60
     b34:   e1a02003    mov r2, r3
     b38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            b38: R_ARM_PC24 MGC_FAPI_WriteReg8
     b3c:   e3a03000    mov r3, #0  ; 0x0
     b40:   e1a00003    mov r0, r3
     b44:   e24bd00c    sub sp, fp, #12 ; 0xc
     b48:   e89da800    ldm sp, {fp, sp, pc}
     b4c:   00000000    .word   0x00000000
            b4c: R_ARM_ABS32    MGC_DiagMsg
     b50:   000000b8    .word   0x000000b8
            b50: R_ARM_ABS32    .rodata
     b54:   00000000    .word   0x00000000
            b54: R_ARM_ABS32    MGC_bDiagLevel
     b58:   000000cc    .word   0x000000cc
            b58: R_ARM_ABS32    .rodata
     b5c:   000000e0    .word   0x000000e0
            b5c: R_ARM_ABS32    .rodata

00000b60 <MGC_HdrcBindEndpoint>:
     b60:   e1a0c00d    mov ip, sp
     b64:   e92dd810    push    {r4, fp, ip, lr, pc}
     b68:   e24cb004    sub fp, ip, #4  ; 0x4
     b6c:   e24dd038    sub sp, sp, #56 ; 0x38
     b70:   e50b0028    str r0, [fp, #-40]
     b74:   e50b102c    str r1, [fp, #-44]
     b78:   e50b2030    str r2, [fp, #-48]
     b7c:   e54b3034    strb    r3, [fp, #-52]
     b80:   e3a03000    mov r3, #0  ; 0x0
     b84:   e54b301b    strb    r3, [fp, #-27]
     b88:   e3a03000    mov r3, #0  ; 0x0
     b8c:   e14b31ba    strh    r3, [fp, #-26]
     b90:   e51b3028    ldr r3, [fp, #-40]
     b94:   e5933000    ldr r3, [r3]
     b98:   e50b3018    str r3, [fp, #-24]
     b9c:   e51b3018    ldr r3, [fp, #-24]
     ba0:   e5933004    ldr r3, [r3, #4]
     ba4:   e50b3014    str r3, [fp, #-20]
     ba8:   e55bc034    ldrb    ip, [fp, #-52]
     bac:   e24b3024    sub r3, fp, #36 ; 0x24
     bb0:   e58d3000    str r3, [sp]
     bb4:   e51b0028    ldr r0, [fp, #-40]
     bb8:   e51b102c    ldr r1, [fp, #-44]
     bbc:   e51b2030    ldr r2, [fp, #-48]
     bc0:   e1a0300c    mov r3, ip
     bc4:   ebfffffe    bl  0 <MGC_DrcBindEndpoint>
            bc4: R_ARM_PC24 MGC_DrcBindEndpoint
     bc8:   e1a03000    mov r3, r0
     bcc:   e50b3020    str r3, [fp, #-32]
     bd0:   e51b3020    ldr r3, [fp, #-32]
     bd4:   e3530000    cmp r3, #0  ; 0x0
     bd8:   0a000137    beq 10bc <MGC_HdrcBindEndpoint+0x55c>
     bdc:   e55b3034    ldrb    r3, [fp, #-52]
     be0:   e3530000    cmp r3, #0  ; 0x0
     be4:   0a000134    beq 10bc <MGC_HdrcBindEndpoint+0x55c>
     be8:   e55b3024    ldrb    r3, [fp, #-36]
     bec:   e3530000    cmp r3, #0  ; 0x0
     bf0:   0a000003    beq c04 <MGC_HdrcBindEndpoint+0xa4>
     bf4:   e51b3020    ldr r3, [fp, #-32]
     bf8:   e5d33009    ldrb    r3, [r3, #9]
     bfc:   e50b3044    str r3, [fp, #-68]
     c00:   ea000002    b   c10 <MGC_HdrcBindEndpoint+0xb0>
     c04:   e51b3020    ldr r3, [fp, #-32]
     c08:   e5d3300d    ldrb    r3, [r3, #13]
     c0c:   e50b3044    str r3, [fp, #-68]
     c10:   e51b1044    ldr r1, [fp, #-68]
     c14:   e1a03001    mov r3, r1
     c18:   e54b3022    strb    r3, [fp, #-34]
     c1c:   e51b3020    ldr r3, [fp, #-32]
     c20:   e5d33000    ldrb    r3, [r3]
     c24:   e54b3023    strb    r3, [fp, #-35]
     c28:   e55b3023    ldrb    r3, [fp, #-35]
     c2c:   e51b0014    ldr r0, [fp, #-20]
     c30:   e3a0100e    mov r1, #14 ; 0xe
     c34:   e1a02003    mov r2, r3
     c38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            c38: R_ARM_PC24 MGC_FAPI_WriteReg8
     c3c:   e55b3024    ldrb    r3, [fp, #-36]
     c40:   e3530000    cmp r3, #0  ; 0x0
     c44:   0a000007    beq c68 <MGC_HdrcBindEndpoint+0x108>
     c48:   e51b3020    ldr r3, [fp, #-32]
     c4c:   e5d33007    ldrb    r3, [r3, #7]
     c50:   e6ef3073    uxtb    r3, r3
     c54:   e50b3040    str r3, [fp, #-64]
     c58:   e51b2040    ldr r2, [fp, #-64]
     c5c:   e202200f    and r2, r2, #15 ; 0xf
     c60:   e50b2040    str r2, [fp, #-64]
     c64:   ea000006    b   c84 <MGC_HdrcBindEndpoint+0x124>
     c68:   e51b3020    ldr r3, [fp, #-32]
     c6c:   e5d3300b    ldrb    r3, [r3, #11]
     c70:   e6ef3073    uxtb    r3, r3
     c74:   e50b3040    str r3, [fp, #-64]
     c78:   e51b3040    ldr r3, [fp, #-64]
     c7c:   e203300f    and r3, r3, #15 ; 0xf
     c80:   e50b3040    str r3, [fp, #-64]
     c84:   e55b301b    ldrb    r3, [fp, #-27]
     c88:   e51b1040    ldr r1, [fp, #-64]
     c8c:   e1a02001    mov r2, r1
     c90:   e1823003    orr r3, r2, r3
     c94:   e6ef3073    uxtb    r3, r3
     c98:   e54b301b    strb    r3, [fp, #-27]
     c9c:   e55b3023    ldrb    r3, [fp, #-35]
     ca0:   e3530000    cmp r3, #0  ; 0x0
     ca4:   0a000008    beq ccc <MGC_HdrcBindEndpoint+0x16c>
     ca8:   e55b3022    ldrb    r3, [fp, #-34]
     cac:   e1a03203    lsl r3, r3, #4
     cb0:   e6ef3073    uxtb    r3, r3
     cb4:   e55b201b    ldrb    r2, [fp, #-27]
     cb8:   e1a01003    mov r1, r3
     cbc:   e1a03002    mov r3, r2
     cc0:   e1813003    orr r3, r1, r3
     cc4:   e6ef3073    uxtb    r3, r3
     cc8:   e54b301b    strb    r3, [fp, #-27]
     ccc:   e55b3024    ldrb    r3, [fp, #-36]
     cd0:   e3530000    cmp r3, #0  ; 0x0
     cd4:   0a000083    beq ee8 <MGC_HdrcBindEndpoint+0x388>
     cd8:   e51b3028    ldr r3, [fp, #-40]
     cdc:   e5d3303e    ldrb    r3, [r3, #62]
     ce0:   e3530000    cmp r3, #0  ; 0x0
     ce4:   0a000032    beq db4 <MGC_HdrcBindEndpoint+0x254>
     ce8:   e55b3023    ldrb    r3, [fp, #-35]
     cec:   e3530000    cmp r3, #0  ; 0x0
     cf0:   0a00002b    beq da4 <MGC_HdrcBindEndpoint+0x244>
     cf4:   e55b301b    ldrb    r3, [fp, #-27]
     cf8:   e51b0014    ldr r0, [fp, #-20]
     cfc:   e3a0101a    mov r1, #26 ; 0x1a
     d00:   e1a02003    mov r2, r3
     d04:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            d04: R_ARM_PC24 MGC_FAPI_WriteReg8
     d08:   e51b302c    ldr r3, [fp, #-44]
     d0c:   e5d3300a    ldrb    r3, [r3, #10]
     d10:   e3530010    cmp r3, #16 ; 0x10
     d14:   23a03010    movcs   r3, #16 ; 0x10
     d18:   e54b3021    strb    r3, [fp, #-33]
     d1c:   e55b3022    ldrb    r3, [fp, #-34]
     d20:   e3530002    cmp r3, #2  ; 0x2
     d24:   1a000018    bne d8c <MGC_HdrcBindEndpoint+0x22c>
     d28:   e51b302c    ldr r3, [fp, #-44]
     d2c:   e1d330bc    ldrh    r3, [r3, #12]
     d30:   e353000f    cmp r3, #15 ; 0xf
     d34:   8a000004    bhi d4c <MGC_HdrcBindEndpoint+0x1ec>
     d38:   e51b302c    ldr r3, [fp, #-44]
     d3c:   e1d330bc    ldrh    r3, [r3, #12]
     d40:   e6ef3073    uxtb    r3, r3
     d44:   e50b303c    str r3, [fp, #-60]
     d48:   ea000001    b   d54 <MGC_HdrcBindEndpoint+0x1f4>
     d4c:   e3a02010    mov r2, #16 ; 0x10
     d50:   e50b203c    str r2, [fp, #-60]
     d54:   e51b103c    ldr r1, [fp, #-60]
     d58:   e1a03001    mov r3, r1
     d5c:   e54b3021    strb    r3, [fp, #-33]
     d60:   e55b3021    ldrb    r3, [fp, #-33]
     d64:   e3530002    cmp r3, #2  ; 0x2
     d68:   33a03002    movcc   r3, #2  ; 0x2
     d6c:   e54b3021    strb    r3, [fp, #-33]
     d70:   e51b302c    ldr r3, [fp, #-44]
     d74:   e1d320bc    ldrh    r2, [r3, #12]
     d78:   e59f334c    ldr r3, [pc, #844]  ; 10cc <MGC_HdrcBindEndpoint+0x56c>
     d7c:   e1520003    cmp r2, r3
     d80:   1a000001    bne d8c <MGC_HdrcBindEndpoint+0x22c>
     d84:   e3a03000    mov r3, #0  ; 0x0
     d88:   e54b3021    strb    r3, [fp, #-33]
     d8c:   e55b3021    ldrb    r3, [fp, #-33]
     d90:   e51b0014    ldr r0, [fp, #-20]
     d94:   e3a0101b    mov r1, #27 ; 0x1b
     d98:   e1a02003    mov r2, r3
     d9c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            d9c: R_ARM_PC24 MGC_FAPI_WriteReg8
     da0:   ea000003    b   db4 <MGC_HdrcBindEndpoint+0x254>
     da4:   e51b0014    ldr r0, [fp, #-20]
     da8:   e3a0101b    mov r1, #27 ; 0x1b
     dac:   e3a02010    mov r2, #16 ; 0x10
     db0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            db0: R_ARM_PC24 MGC_FAPI_WriteReg8
     db4:   e55b3023    ldrb    r3, [fp, #-35]
     db8:   e3530000    cmp r3, #0  ; 0x0
     dbc:   0a000040    beq ec4 <MGC_HdrcBindEndpoint+0x364>
     dc0:   e51b3020    ldr r3, [fp, #-32]
     dc4:   e5d33009    ldrb    r3, [r3, #9]
     dc8:   e3530002    cmp r3, #2  ; 0x2
     dcc:   1a00001b    bne e40 <MGC_HdrcBindEndpoint+0x2e0>
     dd0:   e51b3028    ldr r3, [fp, #-40]
     dd4:   e5d3302e    ldrb    r3, [r3, #46]
     dd8:   e3530000    cmp r3, #0  ; 0x0
     ddc:   0a000017    beq e40 <MGC_HdrcBindEndpoint+0x2e0>
     de0:   e51b3020    ldr r3, [fp, #-32]
     de4:   e1d331b4    ldrh    r3, [r3, #20]
     de8:   e1a04003    mov r4, r3
     dec:   e51b3020    ldr r3, [fp, #-32]
     df0:   e1d320b2    ldrh    r2, [r3, #2]
     df4:   e51b3020    ldr r3, [fp, #-32]
     df8:   e1d331b4    ldrh    r3, [r3, #20]
     dfc:   e1a00002    mov r0, r2
     e00:   e1a01003    mov r1, r3
     e04:   ebfffffe    bl  0 <__udivsi3>
            e04: R_ARM_PC24 __udivsi3
     e08:   e1a03000    mov r3, r0
     e0c:   e6ff3073    uxth    r3, r3
     e10:   e2433001    sub r3, r3, #1  ; 0x1
     e14:   e1a03583    lsl r3, r3, #11
     e18:   e6ff3073    uxth    r3, r3
     e1c:   e1a02004    mov r2, r4
     e20:   e1823003    orr r3, r2, r3
     e24:   e6ff3073    uxth    r3, r3
     e28:   e6ff3073    uxth    r3, r3
     e2c:   e51b0014    ldr r0, [fp, #-20]
     e30:   e3a01010    mov r1, #16 ; 0x10
     e34:   e1a02003    mov r2, r3
     e38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            e38: R_ARM_PC24 MGC_FAPI_WriteReg16
     e3c:   ea000005    b   e58 <MGC_HdrcBindEndpoint+0x2f8>
     e40:   e51b3020    ldr r3, [fp, #-32]
     e44:   e1d331b4    ldrh    r3, [r3, #20]
     e48:   e51b0014    ldr r0, [fp, #-20]
     e4c:   e3a01010    mov r1, #16 ; 0x10
     e50:   e1a02003    mov r2, r3
     e54:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            e54: R_ARM_PC24 MGC_FAPI_WriteReg16
     e58:   e3a03a02    mov r3, #8192   ; 0x2000
     e5c:   e14b31ba    strh    r3, [fp, #-26]
     e60:   e51b3028    ldr r3, [fp, #-40]
     e64:   e5d3303e    ldrb    r3, [r3, #62]
     e68:   e3530000    cmp r3, #0  ; 0x0
     e6c:   1a000006    bne e8c <MGC_HdrcBindEndpoint+0x32c>
     e70:   e51b3020    ldr r3, [fp, #-32]
     e74:   e5d33009    ldrb    r3, [r3, #9]
     e78:   e3530001    cmp r3, #1  ; 0x1
     e7c:   1a000002    bne e8c <MGC_HdrcBindEndpoint+0x32c>
     e80:   e15b31ba    ldrh    r3, [fp, #-26]
     e84:   e3833901    orr r3, r3, #16384  ; 0x4000
     e88:   e14b31ba    strh    r3, [fp, #-26]
     e8c:   e51b0014    ldr r0, [fp, #-20]
     e90:   e3a01012    mov r1, #18 ; 0x12
     e94:   e3a02048    mov r2, #72 ; 0x48
     e98:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            e98: R_ARM_PC24 MGC_FAPI_WriteReg16
     e9c:   e51b0014    ldr r0, [fp, #-20]
     ea0:   e3a01012    mov r1, #18 ; 0x12
     ea4:   e3a02048    mov r2, #72 ; 0x48
     ea8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            ea8: R_ARM_PC24 MGC_FAPI_WriteReg16
     eac:   e15b31ba    ldrh    r3, [fp, #-26]
     eb0:   e51b0014    ldr r0, [fp, #-20]
     eb4:   e3a01012    mov r1, #18 ; 0x12
     eb8:   e1a02003    mov r2, r3
     ebc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            ebc: R_ARM_PC24 MGC_FAPI_WriteReg16
     ec0:   ea00007d    b   10bc <MGC_HdrcBindEndpoint+0x55c>
     ec4:   e51b0014    ldr r0, [fp, #-20]
     ec8:   e3a01012    mov r1, #18 ; 0x12
     ecc:   e3a02c01    mov r2, #256    ; 0x100
     ed0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            ed0: R_ARM_PC24 MGC_FAPI_WriteReg16
     ed4:   e51b0014    ldr r0, [fp, #-20]
     ed8:   e3a01012    mov r1, #18 ; 0x12
     edc:   e3a02c01    mov r2, #256    ; 0x100
     ee0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            ee0: R_ARM_PC24 MGC_FAPI_WriteReg16
     ee4:   ea000074    b   10bc <MGC_HdrcBindEndpoint+0x55c>
     ee8:   e51b3028    ldr r3, [fp, #-40]
     eec:   e5d3303e    ldrb    r3, [r3, #62]
     ef0:   e3530000    cmp r3, #0  ; 0x0
     ef4:   0a00003e    beq ff4 <MGC_HdrcBindEndpoint+0x494>
     ef8:   e55b3023    ldrb    r3, [fp, #-35]
     efc:   e3530000    cmp r3, #0  ; 0x0
     f00:   0a000030    beq fc8 <MGC_HdrcBindEndpoint+0x468>
     f04:   e51b0014    ldr r0, [fp, #-20]
     f08:   e3a01012    mov r1, #18 ; 0x12
     f0c:   e3a02000    mov r2, #0  ; 0x0
     f10:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            f10: R_ARM_PC24 MGC_FAPI_WriteReg16
     f14:   e55b301b    ldrb    r3, [fp, #-27]
     f18:   e51b0014    ldr r0, [fp, #-20]
     f1c:   e3a0101c    mov r1, #28 ; 0x1c
     f20:   e1a02003    mov r2, r3
     f24:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            f24: R_ARM_PC24 MGC_FAPI_WriteReg8
     f28:   e51b302c    ldr r3, [fp, #-44]
     f2c:   e5d3300a    ldrb    r3, [r3, #10]
     f30:   e3530010    cmp r3, #16 ; 0x10
     f34:   23a03010    movcs   r3, #16 ; 0x10
     f38:   e54b3021    strb    r3, [fp, #-33]
     f3c:   e51b3020    ldr r3, [fp, #-32]
     f40:   e5d3300d    ldrb    r3, [r3, #13]
     f44:   e3530002    cmp r3, #2  ; 0x2
     f48:   1a000018    bne fb0 <MGC_HdrcBindEndpoint+0x450>
     f4c:   e51b302c    ldr r3, [fp, #-44]
     f50:   e1d330bc    ldrh    r3, [r3, #12]
     f54:   e353000f    cmp r3, #15 ; 0xf
     f58:   8a000004    bhi f70 <MGC_HdrcBindEndpoint+0x410>
     f5c:   e51b302c    ldr r3, [fp, #-44]
     f60:   e1d330bc    ldrh    r3, [r3, #12]
     f64:   e6ef3073    uxtb    r3, r3
     f68:   e50b3038    str r3, [fp, #-56]
     f6c:   ea000001    b   f78 <MGC_HdrcBindEndpoint+0x418>
     f70:   e3a02010    mov r2, #16 ; 0x10
     f74:   e50b2038    str r2, [fp, #-56]
     f78:   e51b1038    ldr r1, [fp, #-56]
     f7c:   e1a03001    mov r3, r1
     f80:   e54b3021    strb    r3, [fp, #-33]
     f84:   e55b3021    ldrb    r3, [fp, #-33]
     f88:   e3530002    cmp r3, #2  ; 0x2
     f8c:   33a03002    movcc   r3, #2  ; 0x2
     f90:   e54b3021    strb    r3, [fp, #-33]
     f94:   e51b302c    ldr r3, [fp, #-44]
     f98:   e1d320bc    ldrh    r2, [r3, #12]
     f9c:   e59f3128    ldr r3, [pc, #296]  ; 10cc <MGC_HdrcBindEndpoint+0x56c>
     fa0:   e1520003    cmp r2, r3
     fa4:   1a000001    bne fb0 <MGC_HdrcBindEndpoint+0x450>
     fa8:   e3a03000    mov r3, #0  ; 0x0
     fac:   e54b3021    strb    r3, [fp, #-33]
     fb0:   e55b3021    ldrb    r3, [fp, #-33]
     fb4:   e51b0014    ldr r0, [fp, #-20]
     fb8:   e3a0101d    mov r1, #29 ; 0x1d
     fbc:   e1a02003    mov r2, r3
     fc0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            fc0: R_ARM_PC24 MGC_FAPI_WriteReg8
     fc4:   ea00000a    b   ff4 <MGC_HdrcBindEndpoint+0x494>
     fc8:   e51b0014    ldr r0, [fp, #-20]
     fcc:   e3a0101b    mov r1, #27 ; 0x1b
     fd0:   e3a02010    mov r2, #16 ; 0x10
     fd4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            fd4: R_ARM_PC24 MGC_FAPI_WriteReg8
     fd8:   e55b301b    ldrb    r3, [fp, #-27]
     fdc:   e3a020c0    mov r2, #192    ; 0xc0
     fe0:   e0033002    and r3, r3, r2
     fe4:   e51b0014    ldr r0, [fp, #-20]
     fe8:   e3a0101a    mov r1, #26 ; 0x1a
     fec:   e1a02003    mov r2, r3
     ff0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            ff0: R_ARM_PC24 MGC_FAPI_WriteReg8
     ff4:   e55b3023    ldrb    r3, [fp, #-35]
     ff8:   e3530000    cmp r3, #0  ; 0x0
     ffc:   0a00002e    beq 10bc <MGC_HdrcBindEndpoint+0x55c>
    1000:   e51b3020    ldr r3, [fp, #-32]
    1004:   e1d331b6    ldrh    r3, [r3, #22]
    1008:   e51b0014    ldr r0, [fp, #-20]
    100c:   e3a01014    mov r1, #20 ; 0x14
    1010:   e1a02003    mov r2, r3
    1014:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1014: R_ARM_PC24    MGC_FAPI_WriteReg16
    1018:   e51b3028    ldr r3, [fp, #-40]
    101c:   e5d3303e    ldrb    r3, [r3, #62]
    1020:   e3530000    cmp r3, #0  ; 0x0
    1024:   0a000002    beq 1034 <MGC_HdrcBindEndpoint+0x4d4>
    1028:   e3a03000    mov r3, #0  ; 0x0
    102c:   e14b31ba    strh    r3, [fp, #-26]
    1030:   ea000005    b   104c <MGC_HdrcBindEndpoint+0x4ec>
    1034:   e51b3020    ldr r3, [fp, #-32]
    1038:   e5d33009    ldrb    r3, [r3, #9]
    103c:   e3530001    cmp r3, #1  ; 0x1
    1040:   1a000001    bne 104c <MGC_HdrcBindEndpoint+0x4ec>
    1044:   e3a03901    mov r3, #16384  ; 0x4000
    1048:   e14b31ba    strh    r3, [fp, #-26]
    104c:   e51b0014    ldr r0, [fp, #-20]
    1050:   e3a01016    mov r1, #22 ; 0x16
    1054:   e3a02090    mov r2, #144    ; 0x90
    1058:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1058: R_ARM_PC24    MGC_FAPI_WriteReg16
    105c:   e51b0014    ldr r0, [fp, #-20]
    1060:   e3a01016    mov r1, #22 ; 0x16
    1064:   e3a02090    mov r2, #144    ; 0x90
    1068:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1068: R_ARM_PC24    MGC_FAPI_WriteReg16
    106c:   e51b3020    ldr r3, [fp, #-32]
    1070:   e5d3300d    ldrb    r3, [r3, #13]
    1074:   e3530003    cmp r3, #3  ; 0x3
    1078:   1a000002    bne 1088 <MGC_HdrcBindEndpoint+0x528>
    107c:   e15b31ba    ldrh    r3, [fp, #-26]
    1080:   e3833a01    orr r3, r3, #4096   ; 0x1000
    1084:   e14b31ba    strh    r3, [fp, #-26]
    1088:   e51b3028    ldr r3, [fp, #-40]
    108c:   e5d3303e    ldrb    r3, [r3, #62]
    1090:   e3530000    cmp r3, #0  ; 0x0
    1094:   1a000003    bne 10a8 <MGC_HdrcBindEndpoint+0x548>
    1098:   e15b31ba    ldrh    r3, [fp, #-26]
    109c:   e1e03883    mvn r3, r3, lsl #17
    10a0:   e1e038a3    mvn r3, r3, lsr #17
    10a4:   e14b31ba    strh    r3, [fp, #-26]
    10a8:   e15b31ba    ldrh    r3, [fp, #-26]
    10ac:   e51b0014    ldr r0, [fp, #-20]
    10b0:   e3a01016    mov r1, #22 ; 0x16
    10b4:   e1a02003    mov r2, r3
    10b8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            10b8: R_ARM_PC24    MGC_FAPI_WriteReg16
    10bc:   e51b3020    ldr r3, [fp, #-32]
    10c0:   e1a00003    mov r0, r3
    10c4:   e24bd010    sub sp, fp, #16 ; 0x10
    10c8:   e89da810    ldm sp, {r4, fp, sp, pc}
    10cc:   0000ffff    .word   0x0000ffff

000010d0 <MGC_HdrcStartRx>:
    10d0:   e1a0c00d    mov ip, sp
    10d4:   e92dd810    push    {r4, fp, ip, lr, pc}
    10d8:   e24cb004    sub fp, ip, #4  ; 0x4
    10dc:   e24dd050    sub sp, sp, #80 ; 0x50
    10e0:   e50b0040    str r0, [fp, #-64]
    10e4:   e50b1044    str r1, [fp, #-68]
    10e8:   e50b2048    str r2, [fp, #-72]
    10ec:   e50b304c    str r3, [fp, #-76]
    10f0:   e59b3008    ldr r3, [fp, #8]
    10f4:   e54b3050    strb    r3, [fp, #-80]
    10f8:   e3a03000    mov r3, #0  ; 0x0
    10fc:   e54b3021    strb    r3, [fp, #-33]
    1100:   e51b3040    ldr r3, [fp, #-64]
    1104:   e5933000    ldr r3, [r3]
    1108:   e50b3020    str r3, [fp, #-32]
    110c:   e51b3020    ldr r3, [fp, #-32]
    1110:   e5933004    ldr r3, [r3, #4]
    1114:   e50b301c    str r3, [fp, #-28]
    1118:   e51b3020    ldr r3, [fp, #-32]
    111c:   e593300c    ldr r3, [r3, #12]
    1120:   e50b3018    str r3, [fp, #-24]
    1124:   e51b3044    ldr r3, [fp, #-68]
    1128:   e5d33000    ldrb    r3, [r3]
    112c:   e54b3011    strb    r3, [fp, #-17]
    1130:   e55b3011    ldrb    r3, [fp, #-17]
    1134:   e51b001c    ldr r0, [fp, #-28]
    1138:   e3a0100e    mov r1, #14 ; 0xe
    113c:   e1a02003    mov r2, r3
    1140:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1140: R_ARM_PC24    MGC_FAPI_WriteReg8
    1144:   e51b001c    ldr r0, [fp, #-28]
    1148:   e3a01012    mov r1, #18 ; 0x12
    114c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            114c: R_ARM_PC24    MGC_FAPI_ReadReg16
    1150:   e1a03000    mov r3, r0
    1154:   e14b32b8    strh    r3, [fp, #-40]
    1158:   e15b32b8    ldrh    r3, [fp, #-40]
    115c:   e2033a02    and r3, r3, #8192   ; 0x2000
    1160:   e3530000    cmp r3, #0  ; 0x0
    1164:   0a00000b    beq 1198 <MGC_HdrcStartRx+0xc8>
    1168:   e51b3044    ldr r3, [fp, #-68]
    116c:   e5d33001    ldrb    r3, [r3, #1]
    1170:   e3530000    cmp r3, #0  ; 0x0
    1174:   0a000007    beq 1198 <MGC_HdrcStartRx+0xc8>
    1178:   e15b32b8    ldrh    r3, [fp, #-40]
    117c:   e3c33a02    bic r3, r3, #8192   ; 0x2000
    1180:   e14b32b8    strh    r3, [fp, #-40]
    1184:   e15b32b8    ldrh    r3, [fp, #-40]
    1188:   e51b001c    ldr r0, [fp, #-28]
    118c:   e3a01012    mov r1, #18 ; 0x12
    1190:   e1a02003    mov r2, r3
    1194:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1194: R_ARM_PC24    MGC_FAPI_WriteReg16
    1198:   e51b001c    ldr r0, [fp, #-28]
    119c:   e3a01016    mov r1, #22 ; 0x16
    11a0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            11a0: R_ARM_PC24    MGC_FAPI_ReadReg16
    11a4:   e1a03000    mov r3, r0
    11a8:   e14b32b8    strh    r3, [fp, #-40]
    11ac:   e15b32b8    ldrh    r3, [fp, #-40]
    11b0:   e3c33b0a    bic r3, r3, #10240  ; 0x2800
    11b4:   e14b32b8    strh    r3, [fp, #-40]
    11b8:   e51b3040    ldr r3, [fp, #-64]
    11bc:   e5d3303e    ldrb    r3, [r3, #62]
    11c0:   e3530000    cmp r3, #0  ; 0x0
    11c4:   0a000018    beq 122c <MGC_HdrcStartRx+0x15c>
    11c8:   e15b32b8    ldrh    r3, [fp, #-40]
    11cc:   e3833020    orr r3, r3, #32 ; 0x20
    11d0:   e14b32b8    strh    r3, [fp, #-40]
    11d4:   e15b32b8    ldrh    r3, [fp, #-40]
    11d8:   e3c33001    bic r3, r3, #1  ; 0x1
    11dc:   e14b32b8    strh    r3, [fp, #-40]
    11e0:   e51b3044    ldr r3, [fp, #-68]
    11e4:   e5d3300d    ldrb    r3, [r3, #13]
    11e8:   e1a03203    lsl r3, r3, #4
    11ec:   e54b3021    strb    r3, [fp, #-33]
    11f0:   e51b3044    ldr r3, [fp, #-68]
    11f4:   e5d3300b    ldrb    r3, [r3, #11]
    11f8:   e6ef3073    uxtb    r3, r3
    11fc:   e203300f    and r3, r3, #15 ; 0xf
    1200:   e55b2021    ldrb    r2, [fp, #-33]
    1204:   e1a01003    mov r1, r3
    1208:   e1a03002    mov r3, r2
    120c:   e1813003    orr r3, r1, r3
    1210:   e6ef3073    uxtb    r3, r3
    1214:   e54b3021    strb    r3, [fp, #-33]
    1218:   e55b3021    ldrb    r3, [fp, #-33]
    121c:   e51b001c    ldr r0, [fp, #-28]
    1220:   e3a0101c    mov r1, #28 ; 0x1c
    1224:   e1a02003    mov r2, r3
    1228:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1228: R_ARM_PC24    MGC_FAPI_WriteReg8
    122c:   e51b3044    ldr r3, [fp, #-68]
    1230:   e1d331b6    ldrh    r3, [r3, #22]
    1234:   e51b001c    ldr r0, [fp, #-28]
    1238:   e3a01014    mov r1, #20 ; 0x14
    123c:   e1a02003    mov r2, r3
    1240:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1240: R_ARM_PC24    MGC_FAPI_WriteReg16
    1244:   e51b3044    ldr r3, [fp, #-68]
    1248:   e5d3305e    ldrb    r3, [r3, #94]
    124c:   e51b2044    ldr r2, [fp, #-68]
    1250:   e5c2305f    strb    r3, [r2, #95]
    1254:   e51b001c    ldr r0, [fp, #-28]
    1258:   e3a01008    mov r1, #8  ; 0x8
    125c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            125c: R_ARM_PC24    MGC_FAPI_ReadReg16
    1260:   e1a03000    mov r3, r0
    1264:   e1a01003    mov r1, r3
    1268:   e55b2011    ldrb    r2, [fp, #-17]
    126c:   e3a03001    mov r3, #1  ; 0x1
    1270:   e1a03213    lsl r3, r3, r2
    1274:   e6ff3073    uxth    r3, r3
    1278:   e1a02001    mov r2, r1
    127c:   e1823003    orr r3, r2, r3
    1280:   e6ff3073    uxth    r3, r3
    1284:   e14b32b4    strh    r3, [fp, #-36]
    1288:   e55b2011    ldrb    r2, [fp, #-17]
    128c:   e3a03001    mov r3, #1  ; 0x1
    1290:   e1a03213    lsl r3, r3, r2
    1294:   e6ff3073    uxth    r3, r3
    1298:   e1e03003    mvn r3, r3
    129c:   e6ff3073    uxth    r3, r3
    12a0:   e15b22b4    ldrh    r2, [fp, #-36]
    12a4:   e1a01003    mov r1, r3
    12a8:   e1a03002    mov r3, r2
    12ac:   e0013003    and r3, r1, r3
    12b0:   e6ff3073    uxth    r3, r3
    12b4:   e6ff3073    uxth    r3, r3
    12b8:   e51b001c    ldr r0, [fp, #-28]
    12bc:   e3a01008    mov r1, #8  ; 0x8
    12c0:   e1a02003    mov r2, r3
    12c4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            12c4: R_ARM_PC24    MGC_FAPI_WriteReg16
    12c8:   e51b3040    ldr r3, [fp, #-64]
    12cc:   e5d3303e    ldrb    r3, [r3, #62]
    12d0:   e3530000    cmp r3, #0  ; 0x0
    12d4:   1a000126    bne 1774 <MGC_HdrcStartRx+0x6a4>
    12d8:   e51b001c    ldr r0, [fp, #-28]
    12dc:   e3a01016    mov r1, #22 ; 0x16
    12e0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            12e0: R_ARM_PC24    MGC_FAPI_ReadReg16
    12e4:   e1a03000    mov r3, r0
    12e8:   e2033001    and r3, r3, #1  ; 0x1
    12ec:   e6ef3073    uxtb    r3, r3
    12f0:   e3530000    cmp r3, #0  ; 0x0
    12f4:   0a00011e    beq 1774 <MGC_HdrcStartRx+0x6a4>
    12f8:   e15b32b8    ldrh    r3, [fp, #-40]
    12fc:   e2033001    and r3, r3, #1  ; 0x1
    1300:   e6ef3073    uxtb    r3, r3
    1304:   e3530000    cmp r3, #0  ; 0x0
    1308:   0a000113    beq 175c <MGC_HdrcStartRx+0x68c>
    130c:   e51b001c    ldr r0, [fp, #-28]
    1310:   e3a01018    mov r1, #24 ; 0x18
    1314:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1314: R_ARM_PC24    MGC_FAPI_ReadReg16
    1318:   e1a03000    mov r3, r0
    131c:   e14b32b6    strh    r3, [fp, #-38]
    1320:   e59f2768    ldr r2, [pc, #1896] ; 1a90 <MGC_HdrcStartRx+0x9c0>
    1324:   e3a03000    mov r3, #0  ; 0x0
    1328:   e5c23000    strb    r3, [r2]
    132c:   e59f075c    ldr r0, [pc, #1884] ; 1a90 <MGC_HdrcStartRx+0x9c0>
    1330:   e3a01c01    mov r1, #256    ; 0x100
    1334:   e59f2758    ldr r2, [pc, #1880] ; 1a94 <MGC_HdrcStartRx+0x9c4>
    1338:   ebfffffe    bl  0 <MUSB_StringConcat>
            1338: R_ARM_PC24    MUSB_StringConcat
    133c:   e15b22b6    ldrh    r2, [fp, #-38]
    1340:   e3a03000    mov r3, #0  ; 0x0
    1344:   e58d3000    str r3, [sp]
    1348:   e59f0748    ldr r0, [pc, #1864] ; 1a98 <MGC_HdrcStartRx+0x9c8>
    134c:   e3a01021    mov r1, #33 ; 0x21
    1350:   e3a03010    mov r3, #16 ; 0x10
    1354:   ebfffffe    bl  0 <MUSB_Stringize>
            1354: R_ARM_PC24    MUSB_Stringize
    1358:   e59f0730    ldr r0, [pc, #1840] ; 1a90 <MGC_HdrcStartRx+0x9c0>
    135c:   e3a01c01    mov r1, #256    ; 0x100
    1360:   e59f2730    ldr r2, [pc, #1840] ; 1a98 <MGC_HdrcStartRx+0x9c8>
    1364:   ebfffffe    bl  0 <MUSB_StringConcat>
            1364: R_ARM_PC24    MUSB_StringConcat
    1368:   e59f372c    ldr r3, [pc, #1836] ; 1a9c <MGC_HdrcStartRx+0x9cc>
    136c:   e5d33000    ldrb    r3, [r3]
    1370:   e3530002    cmp r3, #2  ; 0x2
    1374:   9a000001    bls 1380 <MGC_HdrcStartRx+0x2b0>
    1378:   e59f0710    ldr r0, [pc, #1808] ; 1a90 <MGC_HdrcStartRx+0x9c0>
    137c:   ebfffffe    bl  0 <MUSB_PrintLine>
            137c: R_ARM_PC24    MUSB_PrintLine
    1380:   e51b3040    ldr r3, [fp, #-64]
    1384:   e5933214    ldr r3, [r3, #532]
    1388:   e50b3034    str r3, [fp, #-52]
    138c:   e51b3044    ldr r3, [fp, #-68]
    1390:   e5933048    ldr r3, [r3, #72]
    1394:   e50b3030    str r3, [fp, #-48]
    1398:   e55b3050    ldrb    r3, [fp, #-80]
    139c:   e3530000    cmp r3, #0  ; 0x0
    13a0:   1a00000a    bne 13d0 <MGC_HdrcStartRx+0x300>
    13a4:   e51b3030    ldr r3, [fp, #-48]
    13a8:   e3530000    cmp r3, #0  ; 0x0
    13ac:   0a000007    beq 13d0 <MGC_HdrcStartRx+0x300>
    13b0:   e51b3034    ldr r3, [fp, #-52]
    13b4:   e5933010    ldr r3, [r3, #16]
    13b8:   e51b0030    ldr r0, [fp, #-48]
    13bc:   e12fff33    blx r3
    13c0:   e51b2044    ldr r2, [fp, #-68]
    13c4:   e3a03000    mov r3, #0  ; 0x0
    13c8:   e5823048    str r3, [r2, #72]
    13cc:   ea000089    b   15f8 <MGC_HdrcStartRx+0x528>
    13d0:   e55b3050    ldrb    r3, [fp, #-80]
    13d4:   e3530000    cmp r3, #0  ; 0x0
    13d8:   0a000086    beq 15f8 <MGC_HdrcStartRx+0x528>
    13dc:   e51b3018    ldr r3, [fp, #-24]
    13e0:   e5932008    ldr r2, [r3, #8]
    13e4:   e51b3018    ldr r3, [fp, #-24]
    13e8:   e5933004    ldr r3, [r3, #4]
    13ec:   e1a00003    mov r0, r3
    13f0:   e51b1048    ldr r1, [fp, #-72]
    13f4:   e12fff32    blx r2
    13f8:   e1a03000    mov r3, r0
    13fc:   e3530000    cmp r3, #0  ; 0x0
    1400:   0a00007c    beq 15f8 <MGC_HdrcStartRx+0x528>
    1404:   e51b3034    ldr r3, [fp, #-52]
    1408:   e3530000    cmp r3, #0  ; 0x0
    140c:   0a000017    beq 1470 <MGC_HdrcStartRx+0x3a0>
    1410:   e51b3030    ldr r3, [fp, #-48]
    1414:   e3530000    cmp r3, #0  ; 0x0
    1418:   1a000014    bne 1470 <MGC_HdrcStartRx+0x3a0>
    141c:   e51b3034    ldr r3, [fp, #-52]
    1420:   e593400c    ldr r4, [r3, #12]
    1424:   e51b3034    ldr r3, [fp, #-52]
    1428:   e5932000    ldr r2, [r3]
    142c:   e55b1011    ldrb    r1, [fp, #-17]
    1430:   e51b3044    ldr r3, [fp, #-68]
    1434:   e5d3300d    ldrb    r3, [r3, #13]
    1438:   e1a0c003    mov ip, r3
    143c:   e51b3044    ldr r3, [fp, #-68]
    1440:   e1d331b6    ldrh    r3, [r3, #22]
    1444:   e58d3000    str r3, [sp]
    1448:   e1a00002    mov r0, r2
    144c:   e3a02000    mov r2, #0  ; 0x0
    1450:   e1a0300c    mov r3, ip
    1454:   e12fff34    blx r4
    1458:   e1a02000    mov r2, r0
    145c:   e51b3044    ldr r3, [fp, #-68]
    1460:   e5832048    str r2, [r3, #72]
    1464:   e51b3044    ldr r3, [fp, #-68]
    1468:   e5933048    ldr r3, [r3, #72]
    146c:   e50b3030    str r3, [fp, #-48]
    1470:   e51b3030    ldr r3, [fp, #-48]
    1474:   e3530000    cmp r3, #0  ; 0x0
    1478:   0a00005e    beq 15f8 <MGC_HdrcStartRx+0x528>
    147c:   e51b2030    ldr r2, [fp, #-48]
    1480:   e3a03000    mov r3, #0  ; 0x0
    1484:   e5823008    str r3, [r2, #8]
    1488:   e51b2044    ldr r2, [fp, #-68]
    148c:   e3a03000    mov r3, #0  ; 0x0
    1490:   e5c23050    strb    r3, [r2, #80]
    1494:   e51b2044    ldr r2, [fp, #-68]
    1498:   e3a03001    mov r3, #1  ; 0x1
    149c:   e5c23051    strb    r3, [r2, #81]
    14a0:   e51b2044    ldr r2, [fp, #-68]
    14a4:   e59b3004    ldr r3, [fp, #4]
    14a8:   e5823030    str r3, [r2, #48]
    14ac:   e51b3034    ldr r3, [fp, #-52]
    14b0:   e593c01c    ldr ip, [r3, #28]
    14b4:   e51b3044    ldr r3, [fp, #-68]
    14b8:   e1d331b6    ldrh    r3, [r3, #22]
    14bc:   e1a01003    mov r1, r3
    14c0:   e51b3030    ldr r3, [fp, #-48]
    14c4:   e5932004    ldr r2, [r3, #4]
    14c8:   e51b304c    ldr r3, [fp, #-76]
    14cc:   e1520003    cmp r2, r3
    14d0:   31a03002    movcc   r3, r2
    14d4:   e58d3000    str r3, [sp]
    14d8:   e51b0030    ldr r0, [fp, #-48]
    14dc:   e3a02000    mov r2, #0  ; 0x0
    14e0:   e51b3048    ldr r3, [fp, #-72]
    14e4:   e12fff3c    blx ip
    14e8:   e1a03000    mov r3, r0
    14ec:   e54b3029    strb    r3, [fp, #-41]
    14f0:   e55b3029    ldrb    r3, [fp, #-41]
    14f4:   e3530000    cmp r3, #0  ; 0x0
    14f8:   0a000031    beq 15c4 <MGC_HdrcStartRx+0x4f4>
    14fc:   e51b3044    ldr r3, [fp, #-68]
    1500:   e5933030    ldr r3, [r3, #48]
    1504:   e3530000    cmp r3, #0  ; 0x0
    1508:   0a00001b    beq 157c <MGC_HdrcStartRx+0x4ac>
    150c:   e51b001c    ldr r0, [fp, #-28]
    1510:   e3a01016    mov r1, #22 ; 0x16
    1514:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1514: R_ARM_PC24    MGC_FAPI_ReadReg16
    1518:   e1a03000    mov r3, r0
    151c:   e14b32b8    strh    r3, [fp, #-40]
    1520:   e15b32b8    ldrh    r3, [fp, #-40]
    1524:   e38334ff    orr r3, r3, #-16777216  ; 0xff000000
    1528:   e38338ff    orr r3, r3, #16711680   ; 0xff0000
    152c:   e3833a0a    orr r3, r3, #40960  ; 0xa000
    1530:   e14b32b8    strh    r3, [fp, #-40]
    1534:   e15b32b8    ldrh    r3, [fp, #-40]
    1538:   e3833b02    orr r3, r3, #2048   ; 0x800
    153c:   e6ff3073    uxth    r3, r3
    1540:   e1a03423    lsr r3, r3, #8
    1544:   e6ff3073    uxth    r3, r3
    1548:   e6ef3073    uxtb    r3, r3
    154c:   e51b001c    ldr r0, [fp, #-28]
    1550:   e3a01017    mov r1, #23 ; 0x17
    1554:   e1a02003    mov r2, r3
    1558:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1558: R_ARM_PC24    MGC_FAPI_WriteReg8
    155c:   e15b32b8    ldrh    r3, [fp, #-40]
    1560:   e1a03423    lsr r3, r3, #8
    1564:   e6ff3073    uxth    r3, r3
    1568:   e6ef3073    uxtb    r3, r3
    156c:   e51b001c    ldr r0, [fp, #-28]
    1570:   e3a01017    mov r1, #23 ; 0x17
    1574:   e1a02003    mov r2, r3
    1578:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1578: R_ARM_PC24    MGC_FAPI_WriteReg8
    157c:   e59f3518    ldr r3, [pc, #1304] ; 1a9c <MGC_HdrcStartRx+0x9cc>
    1580:   e5d33000    ldrb    r3, [r3]
    1584:   e3530002    cmp r3, #2  ; 0x2
    1588:   9a00000a    bls 15b8 <MGC_HdrcStartRx+0x4e8>
    158c:   e51b3040    ldr r3, [fp, #-64]
    1590:   e5933000    ldr r3, [r3]
    1594:   e593300c    ldr r3, [r3, #12]
    1598:   e5932028    ldr r2, [r3, #40]
    159c:   e51b3040    ldr r3, [fp, #-64]
    15a0:   e5933000    ldr r3, [r3]
    15a4:   e593300c    ldr r3, [r3, #12]
    15a8:   e5933004    ldr r3, [r3, #4]
    15ac:   e1a00003    mov r0, r3
    15b0:   e59f14e8    ldr r1, [pc, #1256] ; 1aa0 <MGC_HdrcStartRx+0x9d0>
    15b4:   e12fff32    blx r2
    15b8:   e3a01000    mov r1, #0  ; 0x0
    15bc:   e50b1058    str r1, [fp, #-88]
    15c0:   ea00012e    b   1a80 <MGC_HdrcStartRx+0x9b0>
    15c4:   e51b2044    ldr r2, [fp, #-68]
    15c8:   e3a03000    mov r3, #0  ; 0x0
    15cc:   e5c23051    strb    r3, [r2, #81]
    15d0:   e51b2044    ldr r2, [fp, #-68]
    15d4:   e3a03000    mov r3, #0  ; 0x0
    15d8:   e5823030    str r3, [r2, #48]
    15dc:   e51b3034    ldr r3, [fp, #-52]
    15e0:   e5933010    ldr r3, [r3, #16]
    15e4:   e51b0030    ldr r0, [fp, #-48]
    15e8:   e12fff33    blx r3
    15ec:   e51b2044    ldr r2, [fp, #-68]
    15f0:   e3a03000    mov r3, #0  ; 0x0
    15f4:   e5823048    str r3, [r2, #72]
    15f8:   e15bc2b6    ldrh    ip, [fp, #-38]
    15fc:   e3a03000    mov r3, #0  ; 0x0
    1600:   e58d3000    str r3, [sp]
    1604:   e59b3004    ldr r3, [fp, #4]
    1608:   e58d3004    str r3, [sp, #4]
    160c:   e51b0040    ldr r0, [fp, #-64]
    1610:   e3a01000    mov r1, #0  ; 0x0
    1614:   e51b2044    ldr r2, [fp, #-68]
    1618:   e1a0300c    mov r3, ip
    161c:   ebfffffe    bl  0 <MGC_PipePacketReceived>
            161c: R_ARM_PC24    MGC_PipePacketReceived
    1620:   e1a03000    mov r3, r0
    1624:   e54b3022    strb    r3, [fp, #-34]
    1628:   e15b32b8    ldrh    r3, [fp, #-40]
    162c:   e3c33001    bic r3, r3, #1  ; 0x1
    1630:   e14b32b8    strh    r3, [fp, #-40]
    1634:   e51b3044    ldr r3, [fp, #-68]
    1638:   e1d321b6    ldrh    r2, [r3, #22]
    163c:   e15b32b6    ldrh    r3, [fp, #-38]
    1640:   e1530002    cmp r3, r2
    1644:   3a000003    bcc 1658 <MGC_HdrcStartRx+0x588>
    1648:   e15b32b8    ldrh    r3, [fp, #-40]
    164c:   e6bf3073    sxth    r3, r3
    1650:   e3530000    cmp r3, #0  ; 0x0
    1654:   ba000004    blt 166c <MGC_HdrcStartRx+0x59c>
    1658:   e15b32b8    ldrh    r3, [fp, #-40]
    165c:   e51b001c    ldr r0, [fp, #-28]
    1660:   e3a01016    mov r1, #22 ; 0x16
    1664:   e1a02003    mov r2, r3
    1668:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1668: R_ARM_PC24    MGC_FAPI_WriteReg16
    166c:   e55b3022    ldrb    r3, [fp, #-34]
    1670:   e3530000    cmp r3, #0  ; 0x0
    1674:   0a000038    beq 175c <MGC_HdrcStartRx+0x68c>
    1678:   e24b203c    sub r2, fp, #60 ; 0x3c
    167c:   e59b3004    ldr r3, [fp, #4]
    1680:   e58d3000    str r3, [sp]
    1684:   e1a00002    mov r0, r2
    1688:   e51b1044    ldr r1, [fp, #-68]
    168c:   e3a02000    mov r2, #0  ; 0x0
    1690:   e3a03000    mov r3, #0  ; 0x0
    1694:   ebfffffe    bl  0 <MGC_CompleteIrp>
            1694: R_ARM_PC24    MGC_CompleteIrp
    1698:   e1a03000    mov r3, r0
    169c:   e3530000    cmp r3, #0  ; 0x0
    16a0:   0a000007    beq 16c4 <MGC_HdrcStartRx+0x5f4>
    16a4:   e51b3018    ldr r3, [fp, #-24]
    16a8:   e593c00c    ldr ip, [r3, #12]
    16ac:   e51b3018    ldr r3, [fp, #-24]
    16b0:   e5933004    ldr r3, [r3, #4]
    16b4:   e24b203c    sub r2, fp, #60 ; 0x3c
    16b8:   e1a00003    mov r0, r3
    16bc:   e1a01002    mov r1, r2
    16c0:   e12fff3c    blx ip
    16c4:   e59f33d0    ldr r3, [pc, #976]  ; 1a9c <MGC_HdrcStartRx+0x9cc>
    16c8:   e5d33000    ldrb    r3, [r3]
    16cc:   e3530002    cmp r3, #2  ; 0x2
    16d0:   9a00000a    bls 1700 <MGC_HdrcStartRx+0x630>
    16d4:   e51b3040    ldr r3, [fp, #-64]
    16d8:   e5933000    ldr r3, [r3]
    16dc:   e593300c    ldr r3, [r3, #12]
    16e0:   e5932028    ldr r2, [r3, #40]
    16e4:   e51b3040    ldr r3, [fp, #-64]
    16e8:   e5933000    ldr r3, [r3]
    16ec:   e593300c    ldr r3, [r3, #12]
    16f0:   e5933004    ldr r3, [r3, #4]
    16f4:   e1a00003    mov r0, r3
    16f8:   e59f13a4    ldr r1, [pc, #932]  ; 1aa4 <MGC_HdrcStartRx+0x9d4>
    16fc:   e12fff32    blx r2
    1700:   e15b32b4    ldrh    r3, [fp, #-36]
    1704:   e51b001c    ldr r0, [fp, #-28]
    1708:   e3a01008    mov r1, #8  ; 0x8
    170c:   e1a02003    mov r2, r3
    1710:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1710: R_ARM_PC24    MGC_FAPI_WriteReg16
    1714:   e59f3380    ldr r3, [pc, #896]  ; 1a9c <MGC_HdrcStartRx+0x9cc>
    1718:   e5d33000    ldrb    r3, [r3]
    171c:   e3530002    cmp r3, #2  ; 0x2
    1720:   9a00000a    bls 1750 <MGC_HdrcStartRx+0x680>
    1724:   e51b3040    ldr r3, [fp, #-64]
    1728:   e5933000    ldr r3, [r3]
    172c:   e593300c    ldr r3, [r3, #12]
    1730:   e5932028    ldr r2, [r3, #40]
    1734:   e51b3040    ldr r3, [fp, #-64]
    1738:   e5933000    ldr r3, [r3]
    173c:   e593300c    ldr r3, [r3, #12]
    1740:   e5933004    ldr r3, [r3, #4]
    1744:   e1a00003    mov r0, r3
    1748:   e59f1350    ldr r1, [pc, #848]  ; 1aa0 <MGC_HdrcStartRx+0x9d0>
    174c:   e12fff32    blx r2
    1750:   e3a03000    mov r3, #0  ; 0x0
    1754:   e50b3058    str r3, [fp, #-88]
    1758:   ea0000c8    b   1a80 <MGC_HdrcStartRx+0x9b0>
    175c:   e51b001c    ldr r0, [fp, #-28]
    1760:   e3a01016    mov r1, #22 ; 0x16
    1764:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1764: R_ARM_PC24    MGC_FAPI_ReadReg16
    1768:   e1a03000    mov r3, r0
    176c:   e14b32b8    strh    r3, [fp, #-40]
    1770:   eafffee0    b   12f8 <MGC_HdrcStartRx+0x228>
    1774:   e51b2044    ldr r2, [fp, #-68]
    1778:   e59b3004    ldr r3, [fp, #4]
    177c:   e5823030    str r3, [r2, #48]
    1780:   e51b3040    ldr r3, [fp, #-64]
    1784:   e5933214    ldr r3, [r3, #532]
    1788:   e50b3034    str r3, [fp, #-52]
    178c:   e51b3044    ldr r3, [fp, #-68]
    1790:   e5933048    ldr r3, [r3, #72]
    1794:   e50b3030    str r3, [fp, #-48]
    1798:   e55b3050    ldrb    r3, [fp, #-80]
    179c:   e3530000    cmp r3, #0  ; 0x0
    17a0:   1a00000a    bne 17d0 <MGC_HdrcStartRx+0x700>
    17a4:   e51b3030    ldr r3, [fp, #-48]
    17a8:   e3530000    cmp r3, #0  ; 0x0
    17ac:   0a000007    beq 17d0 <MGC_HdrcStartRx+0x700>
    17b0:   e51b3034    ldr r3, [fp, #-52]
    17b4:   e5933010    ldr r3, [r3, #16]
    17b8:   e51b0030    ldr r0, [fp, #-48]
    17bc:   e12fff33    blx r3
    17c0:   e51b2044    ldr r2, [fp, #-68]
    17c4:   e3a03000    mov r3, #0  ; 0x0
    17c8:   e5823048    str r3, [r2, #72]
    17cc:   ea000068    b   1974 <MGC_HdrcStartRx+0x8a4>
    17d0:   e55b3050    ldrb    r3, [fp, #-80]
    17d4:   e3530000    cmp r3, #0  ; 0x0
    17d8:   0a000065    beq 1974 <MGC_HdrcStartRx+0x8a4>
    17dc:   e51b3018    ldr r3, [fp, #-24]
    17e0:   e5932008    ldr r2, [r3, #8]
    17e4:   e51b3018    ldr r3, [fp, #-24]
    17e8:   e5933004    ldr r3, [r3, #4]
    17ec:   e1a00003    mov r0, r3
    17f0:   e51b1048    ldr r1, [fp, #-72]
    17f4:   e12fff32    blx r2
    17f8:   e1a03000    mov r3, r0
    17fc:   e3530000    cmp r3, #0  ; 0x0
    1800:   0a00005b    beq 1974 <MGC_HdrcStartRx+0x8a4>
    1804:   e51b3034    ldr r3, [fp, #-52]
    1808:   e3530000    cmp r3, #0  ; 0x0
    180c:   0a000017    beq 1870 <MGC_HdrcStartRx+0x7a0>
    1810:   e51b3030    ldr r3, [fp, #-48]
    1814:   e3530000    cmp r3, #0  ; 0x0
    1818:   1a000014    bne 1870 <MGC_HdrcStartRx+0x7a0>
    181c:   e51b3034    ldr r3, [fp, #-52]
    1820:   e593400c    ldr r4, [r3, #12]
    1824:   e51b3034    ldr r3, [fp, #-52]
    1828:   e5932000    ldr r2, [r3]
    182c:   e55b1011    ldrb    r1, [fp, #-17]
    1830:   e51b3044    ldr r3, [fp, #-68]
    1834:   e5d3300d    ldrb    r3, [r3, #13]
    1838:   e1a0c003    mov ip, r3
    183c:   e51b3044    ldr r3, [fp, #-68]
    1840:   e1d331b6    ldrh    r3, [r3, #22]
    1844:   e58d3000    str r3, [sp]
    1848:   e1a00002    mov r0, r2
    184c:   e3a02000    mov r2, #0  ; 0x0
    1850:   e1a0300c    mov r3, ip
    1854:   e12fff34    blx r4
    1858:   e1a02000    mov r2, r0
    185c:   e51b3044    ldr r3, [fp, #-68]
    1860:   e5832048    str r2, [r3, #72]
    1864:   e51b3044    ldr r3, [fp, #-68]
    1868:   e5933048    ldr r3, [r3, #72]
    186c:   e50b3030    str r3, [fp, #-48]
    1870:   e51b3030    ldr r3, [fp, #-48]
    1874:   e3530000    cmp r3, #0  ; 0x0
    1878:   0a00003d    beq 1974 <MGC_HdrcStartRx+0x8a4>
    187c:   e51b2044    ldr r2, [fp, #-68]
    1880:   e3a03000    mov r3, #0  ; 0x0
    1884:   e5c23050    strb    r3, [r2, #80]
    1888:   e51b2030    ldr r2, [fp, #-48]
    188c:   e3a03000    mov r3, #0  ; 0x0
    1890:   e5823008    str r3, [r2, #8]
    1894:   e51b3034    ldr r3, [fp, #-52]
    1898:   e593401c    ldr r4, [r3, #28]
    189c:   e51b3044    ldr r3, [fp, #-68]
    18a0:   e1d331b6    ldrh    r3, [r3, #22]
    18a4:   e1a01003    mov r1, r3
    18a8:   e51b3030    ldr r3, [fp, #-48]
    18ac:   e5d33010    ldrb    r3, [r3, #16]
    18b0:   e1a0c003    mov ip, r3
    18b4:   e51b3030    ldr r3, [fp, #-48]
    18b8:   e5932004    ldr r2, [r3, #4]
    18bc:   e51b304c    ldr r3, [fp, #-76]
    18c0:   e1520003    cmp r2, r3
    18c4:   31a03002    movcc   r3, r2
    18c8:   e58d3000    str r3, [sp]
    18cc:   e51b0030    ldr r0, [fp, #-48]
    18d0:   e1a0200c    mov r2, ip
    18d4:   e51b3048    ldr r3, [fp, #-72]
    18d8:   e12fff34    blx r4
    18dc:   e1a03000    mov r3, r0
    18e0:   e54b3029    strb    r3, [fp, #-41]
    18e4:   e55b3029    ldrb    r3, [fp, #-41]
    18e8:   e3530000    cmp r3, #0  ; 0x0
    18ec:   0a000019    beq 1958 <MGC_HdrcStartRx+0x888>
    18f0:   e51b3040    ldr r3, [fp, #-64]
    18f4:   e5d3303e    ldrb    r3, [r3, #62]
    18f8:   e3530000    cmp r3, #0  ; 0x0
    18fc:   0a000005    beq 1918 <MGC_HdrcStartRx+0x848>
    1900:   e15b32b8    ldrh    r3, [fp, #-40]
    1904:   e3c33020    bic r3, r3, #32 ; 0x20
    1908:   e14b32b8    strh    r3, [fp, #-40]
    190c:   e15b32b8    ldrh    r3, [fp, #-40]
    1910:   e3833901    orr r3, r3, #16384  ; 0x4000
    1914:   e14b32b8    strh    r3, [fp, #-40]
    1918:   e51b3030    ldr r3, [fp, #-48]
    191c:   e5d33010    ldrb    r3, [r3, #16]
    1920:   e3530000    cmp r3, #0  ; 0x0
    1924:   0a000002    beq 1934 <MGC_HdrcStartRx+0x864>
    1928:   e3a03b2a    mov r3, #43008  ; 0xa800
    192c:   e50b3054    str r3, [fp, #-84]
    1930:   ea000001    b   193c <MGC_HdrcStartRx+0x86c>
    1934:   e3a03a0a    mov r3, #40960  ; 0xa000
    1938:   e50b3054    str r3, [fp, #-84]
    193c:   e15b32b8    ldrh    r3, [fp, #-40]
    1940:   e51b1054    ldr r1, [fp, #-84]
    1944:   e1a02001    mov r2, r1
    1948:   e1823003    orr r3, r2, r3
    194c:   e6ff3073    uxth    r3, r3
    1950:   e14b32b8    strh    r3, [fp, #-40]
    1954:   ea000006    b   1974 <MGC_HdrcStartRx+0x8a4>
    1958:   e51b3034    ldr r3, [fp, #-52]
    195c:   e5933010    ldr r3, [r3, #16]
    1960:   e51b0030    ldr r0, [fp, #-48]
    1964:   e12fff33    blx r3
    1968:   e51b2044    ldr r2, [fp, #-68]
    196c:   e3a03000    mov r3, #0  ; 0x0
    1970:   e5823048    str r3, [r2, #72]
    1974:   e51b3040    ldr r3, [fp, #-64]
    1978:   e5d3303e    ldrb    r3, [r3, #62]
    197c:   e3530000    cmp r3, #0  ; 0x0
    1980:   0a000005    beq 199c <MGC_HdrcStartRx+0x8cc>
    1984:   e15b32b8    ldrh    r3, [fp, #-40]
    1988:   e51b001c    ldr r0, [fp, #-28]
    198c:   e3a01016    mov r1, #22 ; 0x16
    1990:   e1a02003    mov r2, r3
    1994:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1994: R_ARM_PC24    MGC_FAPI_WriteReg16
    1998:   ea000007    b   19bc <MGC_HdrcStartRx+0x8ec>
    199c:   e15b32b8    ldrh    r3, [fp, #-40]
    19a0:   e1a03423    lsr r3, r3, #8
    19a4:   e6ff3073    uxth    r3, r3
    19a8:   e6ef3073    uxtb    r3, r3
    19ac:   e51b001c    ldr r0, [fp, #-28]
    19b0:   e3a01017    mov r1, #23 ; 0x17
    19b4:   e1a02003    mov r2, r3
    19b8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            19b8: R_ARM_PC24    MGC_FAPI_WriteReg8
    19bc:   e55b3050    ldrb    r3, [fp, #-80]
    19c0:   e3530000    cmp r3, #0  ; 0x0
    19c4:   0a000017    beq 1a28 <MGC_HdrcStartRx+0x958>
    19c8:   e51b3030    ldr r3, [fp, #-48]
    19cc:   e3530000    cmp r3, #0  ; 0x0
    19d0:   0a000014    beq 1a28 <MGC_HdrcStartRx+0x958>
    19d4:   e55b3029    ldrb    r3, [fp, #-41]
    19d8:   e3530000    cmp r3, #0  ; 0x0
    19dc:   0a000011    beq 1a28 <MGC_HdrcStartRx+0x958>
    19e0:   e59f30b4    ldr r3, [pc, #180]  ; 1a9c <MGC_HdrcStartRx+0x9cc>
    19e4:   e5d33000    ldrb    r3, [r3]
    19e8:   e3530002    cmp r3, #2  ; 0x2
    19ec:   9a00000a    bls 1a1c <MGC_HdrcStartRx+0x94c>
    19f0:   e51b3040    ldr r3, [fp, #-64]
    19f4:   e5933000    ldr r3, [r3]
    19f8:   e593300c    ldr r3, [r3, #12]
    19fc:   e5932028    ldr r2, [r3, #40]
    1a00:   e51b3040    ldr r3, [fp, #-64]
    1a04:   e5933000    ldr r3, [r3]
    1a08:   e593300c    ldr r3, [r3, #12]
    1a0c:   e5933004    ldr r3, [r3, #4]
    1a10:   e1a00003    mov r0, r3
    1a14:   e59f108c    ldr r1, [pc, #140]  ; 1aa8 <MGC_HdrcStartRx+0x9d8>
    1a18:   e12fff32    blx r2
    1a1c:   e3a03000    mov r3, #0  ; 0x0
    1a20:   e50b3058    str r3, [fp, #-88]
    1a24:   ea000015    b   1a80 <MGC_HdrcStartRx+0x9b0>
    1a28:   e15b32b4    ldrh    r3, [fp, #-36]
    1a2c:   e51b001c    ldr r0, [fp, #-28]
    1a30:   e3a01008    mov r1, #8  ; 0x8
    1a34:   e1a02003    mov r2, r3
    1a38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1a38: R_ARM_PC24    MGC_FAPI_WriteReg16
    1a3c:   e59f3058    ldr r3, [pc, #88]   ; 1a9c <MGC_HdrcStartRx+0x9cc>
    1a40:   e5d33000    ldrb    r3, [r3]
    1a44:   e3530002    cmp r3, #2  ; 0x2
    1a48:   9a00000a    bls 1a78 <MGC_HdrcStartRx+0x9a8>
    1a4c:   e51b3040    ldr r3, [fp, #-64]
    1a50:   e5933000    ldr r3, [r3]
    1a54:   e593300c    ldr r3, [r3, #12]
    1a58:   e5932028    ldr r2, [r3, #40]
    1a5c:   e51b3040    ldr r3, [fp, #-64]
    1a60:   e5933000    ldr r3, [r3]
    1a64:   e593300c    ldr r3, [r3, #12]
    1a68:   e5933004    ldr r3, [r3, #4]
    1a6c:   e1a00003    mov r0, r3
    1a70:   e59f1030    ldr r1, [pc, #48]   ; 1aa8 <MGC_HdrcStartRx+0x9d8>
    1a74:   e12fff32    blx r2
    1a78:   e3a01000    mov r1, #0  ; 0x0
    1a7c:   e50b1058    str r1, [fp, #-88]
    1a80:   e51b3058    ldr r3, [fp, #-88]
    1a84:   e1a00003    mov r0, r3
    1a88:   e24bd010    sub sp, fp, #16 ; 0x10
    1a8c:   e89da810    ldm sp, {r4, fp, sp, pc}
    1a90:   00000000    .word   0x00000000
            1a90: R_ARM_ABS32   MGC_DiagMsg
    1a94:   000000f4    .word   0x000000f4
            1a94: R_ARM_ABS32   .rodata
    ...
            1a98: R_ARM_ABS32   MGC_DiagNumber
            1a9c: R_ARM_ABS32   MGC_bDiagLevel
    1aa0:   00000114    .word   0x00000114
            1aa0: R_ARM_ABS32   .rodata
    1aa4:   0000012c    .word   0x0000012c
            1aa4: R_ARM_ABS32   .rodata
    1aa8:   00000144    .word   0x00000144
            1aa8: R_ARM_ABS32   .rodata

00001aac <MGC_HdrcStartTx>:
    1aac:   e1a0c00d    mov ip, sp
    1ab0:   e92dd810    push    {r4, fp, ip, lr, pc}
    1ab4:   e24cb004    sub fp, ip, #4  ; 0x4
    1ab8:   e24dd050    sub sp, sp, #80 ; 0x50
    1abc:   e50b0048    str r0, [fp, #-72]
    1ac0:   e50b104c    str r1, [fp, #-76]
    1ac4:   e50b2050    str r2, [fp, #-80]
    1ac8:   e50b3054    str r3, [fp, #-84]
    1acc:   e3a03000    mov r3, #0  ; 0x0
    1ad0:   e14b33b4    strh    r3, [fp, #-52]
    1ad4:   e3a03000    mov r3, #0  ; 0x0
    1ad8:   e54b3031    strb    r3, [fp, #-49]
    1adc:   e51b3048    ldr r3, [fp, #-72]
    1ae0:   e5933000    ldr r3, [r3]
    1ae4:   e50b3030    str r3, [fp, #-48]
    1ae8:   e3a03000    mov r3, #0  ; 0x0
    1aec:   e54b3029    strb    r3, [fp, #-41]
    1af0:   e3a03000    mov r3, #0  ; 0x0
    1af4:   e50b3028    str r3, [fp, #-40]
    1af8:   e3a03000    mov r3, #0  ; 0x0
    1afc:   e50b3024    str r3, [fp, #-36]
    1b00:   e3a03000    mov r3, #0  ; 0x0
    1b04:   e50b3020    str r3, [fp, #-32]
    1b08:   e51b3030    ldr r3, [fp, #-48]
    1b0c:   e593300c    ldr r3, [r3, #12]
    1b10:   e50b301c    str r3, [fp, #-28]
    1b14:   e51b3030    ldr r3, [fp, #-48]
    1b18:   e5933004    ldr r3, [r3, #4]
    1b1c:   e50b3018    str r3, [fp, #-24]
    1b20:   e51b304c    ldr r3, [fp, #-76]
    1b24:   e5d33000    ldrb    r3, [r3]
    1b28:   e54b3011    strb    r3, [fp, #-17]
    1b2c:   e55b3011    ldrb    r3, [fp, #-17]
    1b30:   e51b0018    ldr r0, [fp, #-24]
    1b34:   e3a0100e    mov r1, #14 ; 0xe
    1b38:   e1a02003    mov r2, r3
    1b3c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1b3c: R_ARM_PC24    MGC_FAPI_WriteReg8
    1b40:   e51b3048    ldr r3, [fp, #-72]
    1b44:   e5d3303e    ldrb    r3, [r3, #62]
    1b48:   e3530000    cmp r3, #0  ; 0x0
    1b4c:   0a000015    beq 1ba8 <MGC_HdrcStartTx+0xfc>
    1b50:   e55b3011    ldrb    r3, [fp, #-17]
    1b54:   e3530000    cmp r3, #0  ; 0x0
    1b58:   0a000012    beq 1ba8 <MGC_HdrcStartTx+0xfc>
    1b5c:   e51b304c    ldr r3, [fp, #-76]
    1b60:   e5d33009    ldrb    r3, [r3, #9]
    1b64:   e1a03203    lsl r3, r3, #4
    1b68:   e54b3031    strb    r3, [fp, #-49]
    1b6c:   e51b304c    ldr r3, [fp, #-76]
    1b70:   e5d33007    ldrb    r3, [r3, #7]
    1b74:   e6ef3073    uxtb    r3, r3
    1b78:   e203300f    and r3, r3, #15 ; 0xf
    1b7c:   e55b2031    ldrb    r2, [fp, #-49]
    1b80:   e1a01003    mov r1, r3
    1b84:   e1a03002    mov r3, r2
    1b88:   e1813003    orr r3, r1, r3
    1b8c:   e6ef3073    uxtb    r3, r3
    1b90:   e54b3031    strb    r3, [fp, #-49]
    1b94:   e55b3031    ldrb    r3, [fp, #-49]
    1b98:   e51b0018    ldr r0, [fp, #-24]
    1b9c:   e3a0101a    mov r1, #26 ; 0x1a
    1ba0:   e1a02003    mov r2, r3
    1ba4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1ba4: R_ARM_PC24    MGC_FAPI_WriteReg8
    1ba8:   e51b304c    ldr r3, [fp, #-76]
    1bac:   e5d33009    ldrb    r3, [r3, #9]
    1bb0:   e3530002    cmp r3, #2  ; 0x2
    1bb4:   1a00000b    bne 1be8 <MGC_HdrcStartTx+0x13c>
    1bb8:   e51b3048    ldr r3, [fp, #-72]
    1bbc:   e5d3302e    ldrb    r3, [r3, #46]
    1bc0:   e3530000    cmp r3, #0  ; 0x0
    1bc4:   0a000007    beq 1be8 <MGC_HdrcStartTx+0x13c>
    1bc8:   e51b304c    ldr r3, [fp, #-76]
    1bcc:   e1d330b2    ldrh    r3, [r3, #2]
    1bd0:   e1a02003    mov r2, r3
    1bd4:   e51b3054    ldr r3, [fp, #-84]
    1bd8:   e1520003    cmp r2, r3
    1bdc:   31a03002    movcc   r3, r2
    1be0:   e50b3038    str r3, [fp, #-56]
    1be4:   ea000006    b   1c04 <MGC_HdrcStartTx+0x158>
    1be8:   e51b304c    ldr r3, [fp, #-76]
    1bec:   e1d331b4    ldrh    r3, [r3, #20]
    1bf0:   e1a02003    mov r2, r3
    1bf4:   e51b3054    ldr r3, [fp, #-84]
    1bf8:   e1520003    cmp r2, r3
    1bfc:   31a03002    movcc   r3, r2
    1c00:   e50b3038    str r3, [fp, #-56]
    1c04:   e55b3011    ldrb    r3, [fp, #-17]
    1c08:   e3530000    cmp r3, #0  ; 0x0
    1c0c:   0a000006    beq 1c2c <MGC_HdrcStartTx+0x180>
    1c10:   e51b0018    ldr r0, [fp, #-24]
    1c14:   e3a01012    mov r1, #18 ; 0x12
    1c18:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            1c18: R_ARM_PC24    MGC_FAPI_ReadReg16
    1c1c:   e1a03000    mov r3, r0
    1c20:   e3c33004    bic r3, r3, #4  ; 0x4
    1c24:   e14b33b4    strh    r3, [fp, #-52]
    1c28:   ea000003    b   1c3c <MGC_HdrcStartTx+0x190>
    1c2c:   e51b0018    ldr r0, [fp, #-24]
    1c30:   e3a01012    mov r1, #18 ; 0x12
    1c34:   e3a02000    mov r2, #0  ; 0x0
    1c38:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1c38: R_ARM_PC24    MGC_FAPI_WriteReg16
    1c3c:   e3a03000    mov r3, #0  ; 0x0
    1c40:   e54b3039    strb    r3, [fp, #-57]
    1c44:   e51b304c    ldr r3, [fp, #-76]
    1c48:   e5d33009    ldrb    r3, [r3, #9]
    1c4c:   e50b305c    str r3, [fp, #-92]
    1c50:   e51b105c    ldr r1, [fp, #-92]
    1c54:   e3510000    cmp r1, #0  ; 0x0
    1c58:   0a000009    beq 1c84 <MGC_HdrcStartTx+0x1d8>
    1c5c:   e51b305c    ldr r3, [fp, #-92]
    1c60:   e3530001    cmp r3, #1  ; 0x1
    1c64:   0a000000    beq 1c6c <MGC_HdrcStartTx+0x1c0>
    1c68:   ea00000b    b   1c9c <MGC_HdrcStartTx+0x1f0>
    1c6c:   e59b3004    ldr r3, [fp, #4]
    1c70:   e50b3024    str r3, [fp, #-36]
    1c74:   e51b3024    ldr r3, [fp, #-36]
    1c78:   e5d33023    ldrb    r3, [r3, #35]
    1c7c:   e54b3029    strb    r3, [fp, #-41]
    1c80:   ea00000a    b   1cb0 <MGC_HdrcStartTx+0x204>
    1c84:   e59b3004    ldr r3, [fp, #4]
    1c88:   e50b3020    str r3, [fp, #-32]
    1c8c:   e51b3020    ldr r3, [fp, #-32]
    1c90:   e5d33028    ldrb    r3, [r3, #40]
    1c94:   e54b3029    strb    r3, [fp, #-41]
    1c98:   ea000004    b   1cb0 <MGC_HdrcStartTx+0x204>
    1c9c:   e59b3004    ldr r3, [fp, #4]
    1ca0:   e50b3028    str r3, [fp, #-40]
    1ca4:   e51b3028    ldr r3, [fp, #-40]
    1ca8:   e5d3301e    ldrb    r3, [r3, #30]
    1cac:   e54b3029    strb    r3, [fp, #-41]
    1cb0:   e51b3048    ldr r3, [fp, #-72]
    1cb4:   e5933214    ldr r3, [r3, #532]
    1cb8:   e50b3044    str r3, [fp, #-68]
    1cbc:   e51b304c    ldr r3, [fp, #-76]
    1cc0:   e5933044    ldr r3, [r3, #68]
    1cc4:   e50b3040    str r3, [fp, #-64]
    1cc8:   e55b3029    ldrb    r3, [fp, #-41]
    1ccc:   e3530000    cmp r3, #0  ; 0x0
    1cd0:   1a00000a    bne 1d00 <MGC_HdrcStartTx+0x254>
    1cd4:   e51b3040    ldr r3, [fp, #-64]
    1cd8:   e3530000    cmp r3, #0  ; 0x0
    1cdc:   0a000007    beq 1d00 <MGC_HdrcStartTx+0x254>
    1ce0:   e51b3044    ldr r3, [fp, #-68]
    1ce4:   e5933010    ldr r3, [r3, #16]
    1ce8:   e51b0040    ldr r0, [fp, #-64]
    1cec:   e12fff33    blx r3
    1cf0:   e51b204c    ldr r2, [fp, #-76]
    1cf4:   e3a03000    mov r3, #0  ; 0x0
    1cf8:   e5823044    str r3, [r2, #68]
    1cfc:   ea000063    b   1e90 <MGC_HdrcStartTx+0x3e4>
    1d00:   e55b3029    ldrb    r3, [fp, #-41]
    1d04:   e3530000    cmp r3, #0  ; 0x0
    1d08:   0a000060    beq 1e90 <MGC_HdrcStartTx+0x3e4>
    1d0c:   e51b301c    ldr r3, [fp, #-28]
    1d10:   e5932008    ldr r2, [r3, #8]
    1d14:   e51b301c    ldr r3, [fp, #-28]
    1d18:   e5933004    ldr r3, [r3, #4]
    1d1c:   e1a00003    mov r0, r3
    1d20:   e51b1050    ldr r1, [fp, #-80]
    1d24:   e12fff32    blx r2
    1d28:   e1a03000    mov r3, r0
    1d2c:   e3530000    cmp r3, #0  ; 0x0
    1d30:   0a000056    beq 1e90 <MGC_HdrcStartTx+0x3e4>
    1d34:   e51b3044    ldr r3, [fp, #-68]
    1d38:   e3530000    cmp r3, #0  ; 0x0
    1d3c:   0a000017    beq 1da0 <MGC_HdrcStartTx+0x2f4>
    1d40:   e51b3040    ldr r3, [fp, #-64]
    1d44:   e3530000    cmp r3, #0  ; 0x0
    1d48:   1a000014    bne 1da0 <MGC_HdrcStartTx+0x2f4>
    1d4c:   e51b3044    ldr r3, [fp, #-68]
    1d50:   e593400c    ldr r4, [r3, #12]
    1d54:   e51b3044    ldr r3, [fp, #-68]
    1d58:   e5932000    ldr r2, [r3]
    1d5c:   e55b1011    ldrb    r1, [fp, #-17]
    1d60:   e51b304c    ldr r3, [fp, #-76]
    1d64:   e5d33009    ldrb    r3, [r3, #9]
    1d68:   e1a0c003    mov ip, r3
    1d6c:   e51b304c    ldr r3, [fp, #-76]
    1d70:   e1d331b4    ldrh    r3, [r3, #20]
    1d74:   e58d3000    str r3, [sp]
    1d78:   e1a00002    mov r0, r2
    1d7c:   e3a02001    mov r2, #1  ; 0x1
    1d80:   e1a0300c    mov r3, ip
    1d84:   e12fff34    blx r4
    1d88:   e1a02000    mov r2, r0
    1d8c:   e51b304c    ldr r3, [fp, #-76]
    1d90:   e5832044    str r2, [r3, #68]
    1d94:   e51b304c    ldr r3, [fp, #-76]
    1d98:   e5933044    ldr r3, [r3, #68]
    1d9c:   e50b3040    str r3, [fp, #-64]
    1da0:   e51b3040    ldr r3, [fp, #-64]
    1da4:   e3530000    cmp r3, #0  ; 0x0
    1da8:   0a000038    beq 1e90 <MGC_HdrcStartTx+0x3e4>
    1dac:   e51b204c    ldr r2, [fp, #-76]
    1db0:   e3a03001    mov r3, #1  ; 0x1
    1db4:   e5c23050    strb    r3, [r2, #80]
    1db8:   e51b2040    ldr r2, [fp, #-64]
    1dbc:   e3a03000    mov r3, #0  ; 0x0
    1dc0:   e5823008    str r3, [r2, #8]
    1dc4:   e51b3044    ldr r3, [fp, #-68]
    1dc8:   e593401c    ldr r4, [r3, #28]
    1dcc:   e51b304c    ldr r3, [fp, #-76]
    1dd0:   e1d331b4    ldrh    r3, [r3, #20]
    1dd4:   e1a01003    mov r1, r3
    1dd8:   e51b3040    ldr r3, [fp, #-64]
    1ddc:   e5d33010    ldrb    r3, [r3, #16]
    1de0:   e1a0c003    mov ip, r3
    1de4:   e51b3040    ldr r3, [fp, #-64]
    1de8:   e5932004    ldr r2, [r3, #4]
    1dec:   e51b3054    ldr r3, [fp, #-84]
    1df0:   e1520003    cmp r2, r3
    1df4:   31a03002    movcc   r3, r2
    1df8:   e58d3000    str r3, [sp]
    1dfc:   e51b0040    ldr r0, [fp, #-64]
    1e00:   e1a0200c    mov r2, ip
    1e04:   e51b3050    ldr r3, [fp, #-80]
    1e08:   e12fff34    blx r4
    1e0c:   e1a03000    mov r3, r0
    1e10:   e54b3039    strb    r3, [fp, #-57]
    1e14:   e55b3039    ldrb    r3, [fp, #-57]
    1e18:   e3530000    cmp r3, #0  ; 0x0
    1e1c:   0a000014    beq 1e74 <MGC_HdrcStartTx+0x3c8>
    1e20:   e3a03000    mov r3, #0  ; 0x0
    1e24:   e50b3038    str r3, [fp, #-56]
    1e28:   e55b3011    ldrb    r3, [fp, #-17]
    1e2c:   e3530000    cmp r3, #0  ; 0x0
    1e30:   0a000016    beq 1e90 <MGC_HdrcStartTx+0x3e4>
    1e34:   e51b3040    ldr r3, [fp, #-64]
    1e38:   e5d33010    ldrb    r3, [r3, #16]
    1e3c:   e3530000    cmp r3, #0  ; 0x0
    1e40:   0a000002    beq 1e50 <MGC_HdrcStartTx+0x3a4>
    1e44:   e3a03b25    mov r3, #37888  ; 0x9400
    1e48:   e50b3058    str r3, [fp, #-88]
    1e4c:   ea000001    b   1e58 <MGC_HdrcStartTx+0x3ac>
    1e50:   e3a03a09    mov r3, #36864  ; 0x9000
    1e54:   e50b3058    str r3, [fp, #-88]
    1e58:   e15b33b4    ldrh    r3, [fp, #-52]
    1e5c:   e51b1058    ldr r1, [fp, #-88]
    1e60:   e1a02001    mov r2, r1
    1e64:   e1823003    orr r3, r2, r3
    1e68:   e6ff3073    uxth    r3, r3
    1e6c:   e14b33b4    strh    r3, [fp, #-52]
    1e70:   ea000006    b   1e90 <MGC_HdrcStartTx+0x3e4>
    1e74:   e51b3044    ldr r3, [fp, #-68]
    1e78:   e5933010    ldr r3, [r3, #16]
    1e7c:   e51b0040    ldr r0, [fp, #-64]
    1e80:   e12fff33    blx r3
    1e84:   e51b204c    ldr r2, [fp, #-76]
    1e88:   e3a03000    mov r3, #0  ; 0x0
    1e8c:   e5823044    str r3, [r2, #68]
    1e90:   e55b3039    ldrb    r3, [fp, #-57]
    1e94:   e3530000    cmp r3, #0  ; 0x0
    1e98:   1a000020    bne 1f20 <MGC_HdrcStartTx+0x474>
    1e9c:   e51b3048    ldr r3, [fp, #-72]
    1ea0:   e593c248    ldr ip, [r3, #584]
    1ea4:   e51b304c    ldr r3, [fp, #-76]
    1ea8:   e5d33000    ldrb    r3, [r3]
    1eac:   e51b0048    ldr r0, [fp, #-72]
    1eb0:   e1a01003    mov r1, r3
    1eb4:   e51b2038    ldr r2, [fp, #-56]
    1eb8:   e51b3050    ldr r3, [fp, #-80]
    1ebc:   e12fff3c    blx ip
    1ec0:   e51b204c    ldr r2, [fp, #-76]
    1ec4:   e51b3038    ldr r3, [fp, #-56]
    1ec8:   e5823018    str r3, [r2, #24]
    1ecc:   e55b3011    ldrb    r3, [fp, #-17]
    1ed0:   e3530000    cmp r3, #0  ; 0x0
    1ed4:   0a000008    beq 1efc <MGC_HdrcStartTx+0x450>
    1ed8:   e3e03a09    mvn r3, #36864  ; 0x9000
    1edc:   e1a02003    mov r2, r3
    1ee0:   e15b33b4    ldrh    r3, [fp, #-52]
    1ee4:   e0033002    and r3, r3, r2
    1ee8:   e14b33b4    strh    r3, [fp, #-52]
    1eec:   e15b33b4    ldrh    r3, [fp, #-52]
    1ef0:   e3833001    orr r3, r3, #1  ; 0x1
    1ef4:   e14b33b4    strh    r3, [fp, #-52]
    1ef8:   ea000008    b   1f20 <MGC_HdrcStartTx+0x474>
    1efc:   e3a0300a    mov r3, #10 ; 0xa
    1f00:   e14b33b4    strh    r3, [fp, #-52]
    1f04:   e51b3048    ldr r3, [fp, #-72]
    1f08:   e5d33046    ldrb    r3, [r3, #70]
    1f0c:   e3530000    cmp r3, #0  ; 0x0
    1f10:   0a000002    beq 1f20 <MGC_HdrcStartTx+0x474>
    1f14:   e15b33b4    ldrh    r3, [fp, #-52]
    1f18:   e3833b02    orr r3, r3, #2048   ; 0x800
    1f1c:   e14b33b4    strh    r3, [fp, #-52]
    1f20:   e51b304c    ldr r3, [fp, #-76]
    1f24:   e59b2004    ldr r2, [fp, #4]
    1f28:   e583201c    str r2, [r3, #28]
    1f2c:   e55b3011    ldrb    r3, [fp, #-17]
    1f30:   e3530000    cmp r3, #0  ; 0x0
    1f34:   0a000007    beq 1f58 <MGC_HdrcStartTx+0x4ac>
    1f38:   e15b33b4    ldrh    r3, [fp, #-52]
    1f3c:   e3833a02    orr r3, r3, #8192   ; 0x2000
    1f40:   e6ff3073    uxth    r3, r3
    1f44:   e51b0018    ldr r0, [fp, #-24]
    1f48:   e3a01012    mov r1, #18 ; 0x12
    1f4c:   e1a02003    mov r2, r3
    1f50:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1f50: R_ARM_PC24    MGC_FAPI_WriteReg16
    1f54:   ea000004    b   1f6c <MGC_HdrcStartTx+0x4c0>
    1f58:   e15b33b4    ldrh    r3, [fp, #-52]
    1f5c:   e51b0018    ldr r0, [fp, #-24]
    1f60:   e3a01012    mov r1, #18 ; 0x12
    1f64:   e1a02003    mov r2, r3
    1f68:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            1f68: R_ARM_PC24    MGC_FAPI_WriteReg16
    1f6c:   e3a03000    mov r3, #0  ; 0x0
    1f70:   e1a00003    mov r0, r3
    1f74:   e24bd010    sub sp, fp, #16 ; 0x10
    1f78:   e89da810    ldm sp, {r4, fp, sp, pc}

00001f7c <MGC_HdrcFlushEndpoint>:
    1f7c:   e1a0c00d    mov ip, sp
    1f80:   e92dd800    push    {fp, ip, lr, pc}
    1f84:   e24cb004    sub fp, ip, #4  ; 0x4
    1f88:   e24dd024    sub sp, sp, #36 ; 0x24
    1f8c:   e50b0020    str r0, [fp, #-32]
    1f90:   e50b1024    str r1, [fp, #-36]
    1f94:   e1a01003    mov r1, r3
    1f98:   e1a03002    mov r3, r2
    1f9c:   e54b3028    strb    r3, [fp, #-40]
    1fa0:   e1a03001    mov r3, r1
    1fa4:   e54b302c    strb    r3, [fp, #-44]
    1fa8:   e51b3020    ldr r3, [fp, #-32]
    1fac:   e5933000    ldr r3, [r3]
    1fb0:   e50b3018    str r3, [fp, #-24]
    1fb4:   e51b3018    ldr r3, [fp, #-24]
    1fb8:   e5933004    ldr r3, [r3, #4]
    1fbc:   e50b3014    str r3, [fp, #-20]
    1fc0:   e3a03000    mov r3, #0  ; 0x0
    1fc4:   e54b300e    strb    r3, [fp, #-14]
    1fc8:   e51b3024    ldr r3, [fp, #-36]
    1fcc:   e5d33000    ldrb    r3, [r3]
    1fd0:   e54b300d    strb    r3, [fp, #-13]
    1fd4:   e55b300d    ldrb    r3, [fp, #-13]
    1fd8:   e51b0014    ldr r0, [fp, #-20]
    1fdc:   e3a0100e    mov r1, #14 ; 0xe
    1fe0:   e1a02003    mov r2, r3
    1fe4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            1fe4: R_ARM_PC24    MGC_FAPI_WriteReg8
    1fe8:   e55b3028    ldrb    r3, [fp, #-40]
    1fec:   e6af3073    sxtb    r3, r3
    1ff0:   e3530000    cmp r3, #0  ; 0x0
    1ff4:   aa000006    bge 2014 <MGC_HdrcFlushEndpoint+0x98>
    1ff8:   e51b3020    ldr r3, [fp, #-32]
    1ffc:   e5d3303e    ldrb    r3, [r3, #62]
    2000:   e3530000    cmp r3, #0  ; 0x0
    2004:   1a000008    bne 202c <MGC_HdrcFlushEndpoint+0xb0>
    2008:   e3a03001    mov r3, #1  ; 0x1
    200c:   e54b300e    strb    r3, [fp, #-14]
    2010:   ea000005    b   202c <MGC_HdrcFlushEndpoint+0xb0>
    2014:   e51b3020    ldr r3, [fp, #-32]
    2018:   e5d3303e    ldrb    r3, [r3, #62]
    201c:   e3530000    cmp r3, #0  ; 0x0
    2020:   0a000001    beq 202c <MGC_HdrcFlushEndpoint+0xb0>
    2024:   e3a03001    mov r3, #1  ; 0x1
    2028:   e54b300e    strb    r3, [fp, #-14]
    202c:   e55b300e    ldrb    r3, [fp, #-14]
    2030:   e3530000    cmp r3, #0  ; 0x0
    2034:   0a000046    beq 2154 <MGC_HdrcFlushEndpoint+0x1d8>
    2038:   e59f22c4    ldr r2, [pc, #708]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    203c:   e3a03000    mov r3, #0  ; 0x0
    2040:   e5c23000    strb    r3, [r2]
    2044:   e51b3018    ldr r3, [fp, #-24]
    2048:   e5933008    ldr r3, [r3, #8]
    204c:   e5933004    ldr r3, [r3, #4]
    2050:   e59f02ac    ldr r0, [pc, #684]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    2054:   e3a01c01    mov r1, #256    ; 0x100
    2058:   e59f22a8    ldr r2, [pc, #680]  ; 2308 <MGC_HdrcFlushEndpoint+0x38c>
    205c:   e12fff33    blx r3
    2060:   e51b3018    ldr r3, [fp, #-24]
    2064:   e5933008    ldr r3, [r3, #8]
    2068:   e593c008    ldr ip, [r3, #8]
    206c:   e55b200d    ldrb    r2, [fp, #-13]
    2070:   e3a03002    mov r3, #2  ; 0x2
    2074:   e58d3000    str r3, [sp]
    2078:   e59f0284    ldr r0, [pc, #644]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    207c:   e3a01c01    mov r1, #256    ; 0x100
    2080:   e3a03010    mov r3, #16 ; 0x10
    2084:   e12fff3c    blx ip
    2088:   e59f327c    ldr r3, [pc, #636]  ; 230c <MGC_HdrcFlushEndpoint+0x390>
    208c:   e5d33000    ldrb    r3, [r3]
    2090:   e3530001    cmp r3, #1  ; 0x1
    2094:   9a000008    bls 20bc <MGC_HdrcFlushEndpoint+0x140>
    2098:   e51b3018    ldr r3, [fp, #-24]
    209c:   e593300c    ldr r3, [r3, #12]
    20a0:   e5932028    ldr r2, [r3, #40]
    20a4:   e51b3018    ldr r3, [fp, #-24]
    20a8:   e593300c    ldr r3, [r3, #12]
    20ac:   e5933004    ldr r3, [r3, #4]
    20b0:   e1a00003    mov r0, r3
    20b4:   e59f1248    ldr r1, [pc, #584]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    20b8:   e12fff32    blx r2
    20bc:   e51b3024    ldr r3, [fp, #-36]
    20c0:   e5933044    ldr r3, [r3, #68]
    20c4:   e3530000    cmp r3, #0  ; 0x0
    20c8:   0a00000d    beq 2104 <MGC_HdrcFlushEndpoint+0x188>
    20cc:   e51b3024    ldr r3, [fp, #-36]
    20d0:   e5d33050    ldrb    r3, [r3, #80]
    20d4:   e3530000    cmp r3, #0  ; 0x0
    20d8:   0a000009    beq 2104 <MGC_HdrcFlushEndpoint+0x188>
    20dc:   e51b3020    ldr r3, [fp, #-32]
    20e0:   e5933214    ldr r3, [r3, #532]
    20e4:   e5932010    ldr r2, [r3, #16]
    20e8:   e51b3024    ldr r3, [fp, #-36]
    20ec:   e5933044    ldr r3, [r3, #68]
    20f0:   e1a00003    mov r0, r3
    20f4:   e12fff32    blx r2
    20f8:   e51b2024    ldr r2, [fp, #-36]
    20fc:   e3a03000    mov r3, #0  ; 0x0
    2100:   e5823044    str r3, [r2, #68]
    2104:   e51b0014    ldr r0, [fp, #-20]
    2108:   e3a01012    mov r1, #18 ; 0x12
    210c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            210c: R_ARM_PC24    MGC_FAPI_ReadReg16
    2110:   e1a03000    mov r3, r0
    2114:   e14b31ba    strh    r3, [fp, #-26]
    2118:   e15b31ba    ldrh    r3, [fp, #-26]
    211c:   e3833048    orr r3, r3, #72 ; 0x48
    2120:   e6ff3073    uxth    r3, r3
    2124:   e51b0014    ldr r0, [fp, #-20]
    2128:   e3a01012    mov r1, #18 ; 0x12
    212c:   e1a02003    mov r2, r3
    2130:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2130: R_ARM_PC24    MGC_FAPI_WriteReg16
    2134:   e15b31ba    ldrh    r3, [fp, #-26]
    2138:   e3833008    orr r3, r3, #8  ; 0x8
    213c:   e6ff3073    uxth    r3, r3
    2140:   e51b0014    ldr r0, [fp, #-20]
    2144:   e3a01012    mov r1, #18 ; 0x12
    2148:   e1a02003    mov r2, r3
    214c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            214c: R_ARM_PC24    MGC_FAPI_WriteReg16
    2150:   ea000067    b   22f4 <MGC_HdrcFlushEndpoint+0x378>
    2154:   e59f21a8    ldr r2, [pc, #424]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    2158:   e3a03000    mov r3, #0  ; 0x0
    215c:   e5c23000    strb    r3, [r2]
    2160:   e51b3018    ldr r3, [fp, #-24]
    2164:   e5933008    ldr r3, [r3, #8]
    2168:   e5933004    ldr r3, [r3, #4]
    216c:   e59f0190    ldr r0, [pc, #400]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    2170:   e3a01c01    mov r1, #256    ; 0x100
    2174:   e59f2194    ldr r2, [pc, #404]  ; 2310 <MGC_HdrcFlushEndpoint+0x394>
    2178:   e12fff33    blx r3
    217c:   e51b3018    ldr r3, [fp, #-24]
    2180:   e5933008    ldr r3, [r3, #8]
    2184:   e593c008    ldr ip, [r3, #8]
    2188:   e55b200d    ldrb    r2, [fp, #-13]
    218c:   e3a03002    mov r3, #2  ; 0x2
    2190:   e58d3000    str r3, [sp]
    2194:   e59f0168    ldr r0, [pc, #360]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    2198:   e3a01c01    mov r1, #256    ; 0x100
    219c:   e3a03010    mov r3, #16 ; 0x10
    21a0:   e12fff3c    blx ip
    21a4:   e59f3160    ldr r3, [pc, #352]  ; 230c <MGC_HdrcFlushEndpoint+0x390>
    21a8:   e5d33000    ldrb    r3, [r3]
    21ac:   e3530001    cmp r3, #1  ; 0x1
    21b0:   9a000008    bls 21d8 <MGC_HdrcFlushEndpoint+0x25c>
    21b4:   e51b3018    ldr r3, [fp, #-24]
    21b8:   e593300c    ldr r3, [r3, #12]
    21bc:   e5932028    ldr r2, [r3, #40]
    21c0:   e51b3018    ldr r3, [fp, #-24]
    21c4:   e593300c    ldr r3, [r3, #12]
    21c8:   e5933004    ldr r3, [r3, #4]
    21cc:   e1a00003    mov r0, r3
    21d0:   e59f112c    ldr r1, [pc, #300]  ; 2304 <MGC_HdrcFlushEndpoint+0x388>
    21d4:   e12fff32    blx r2
    21d8:   e51b3020    ldr r3, [fp, #-32]
    21dc:   e5d3303e    ldrb    r3, [r3, #62]
    21e0:   e3530000    cmp r3, #0  ; 0x0
    21e4:   0a00001d    beq 2260 <MGC_HdrcFlushEndpoint+0x2e4>
    21e8:   e55b300d    ldrb    r3, [fp, #-13]
    21ec:   e3530000    cmp r3, #0  ; 0x0
    21f0:   0a00000d    beq 222c <MGC_HdrcFlushEndpoint+0x2b0>
    21f4:   e51b0014    ldr r0, [fp, #-20]
    21f8:   e3a01016    mov r1, #22 ; 0x16
    21fc:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            21fc: R_ARM_PC24    MGC_FAPI_ReadReg16
    2200:   e1a03000    mov r3, r0
    2204:   e14b31ba    strh    r3, [fp, #-26]
    2208:   e15b31ba    ldrh    r3, [fp, #-26]
    220c:   e3c33020    bic r3, r3, #32 ; 0x20
    2210:   e14b31ba    strh    r3, [fp, #-26]
    2214:   e15b31ba    ldrh    r3, [fp, #-26]
    2218:   e51b0014    ldr r0, [fp, #-20]
    221c:   e3a01016    mov r1, #22 ; 0x16
    2220:   e1a02003    mov r2, r3
    2224:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2224: R_ARM_PC24    MGC_FAPI_WriteReg16
    2228:   ea00000c    b   2260 <MGC_HdrcFlushEndpoint+0x2e4>
    222c:   e51b0014    ldr r0, [fp, #-20]
    2230:   e3a01012    mov r1, #18 ; 0x12
    2234:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            2234: R_ARM_PC24    MGC_FAPI_ReadReg16
    2238:   e1a03000    mov r3, r0
    223c:   e14b31ba    strh    r3, [fp, #-26]
    2240:   e15b31ba    ldrh    r3, [fp, #-26]
    2244:   e3c33020    bic r3, r3, #32 ; 0x20
    2248:   e14b31ba    strh    r3, [fp, #-26]
    224c:   e15b31ba    ldrh    r3, [fp, #-26]
    2250:   e51b0014    ldr r0, [fp, #-20]
    2254:   e3a01012    mov r1, #18 ; 0x12
    2258:   e1a02003    mov r2, r3
    225c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            225c: R_ARM_PC24    MGC_FAPI_WriteReg16
    2260:   e51b3024    ldr r3, [fp, #-36]
    2264:   e5933044    ldr r3, [r3, #68]
    2268:   e3530000    cmp r3, #0  ; 0x0
    226c:   0a00000d    beq 22a8 <MGC_HdrcFlushEndpoint+0x32c>
    2270:   e51b3024    ldr r3, [fp, #-36]
    2274:   e5d33050    ldrb    r3, [r3, #80]
    2278:   e3530000    cmp r3, #0  ; 0x0
    227c:   1a000009    bne 22a8 <MGC_HdrcFlushEndpoint+0x32c>
    2280:   e51b3020    ldr r3, [fp, #-32]
    2284:   e5933214    ldr r3, [r3, #532]
    2288:   e5932010    ldr r2, [r3, #16]
    228c:   e51b3024    ldr r3, [fp, #-36]
    2290:   e5933044    ldr r3, [r3, #68]
    2294:   e1a00003    mov r0, r3
    2298:   e12fff32    blx r2
    229c:   e51b2024    ldr r2, [fp, #-36]
    22a0:   e3a03000    mov r3, #0  ; 0x0
    22a4:   e5823044    str r3, [r2, #68]
    22a8:   e51b0014    ldr r0, [fp, #-20]
    22ac:   e3a01016    mov r1, #22 ; 0x16
    22b0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            22b0: R_ARM_PC24    MGC_FAPI_ReadReg16
    22b4:   e1a03000    mov r3, r0
    22b8:   e14b31ba    strh    r3, [fp, #-26]
    22bc:   e15b31ba    ldrh    r3, [fp, #-26]
    22c0:   e3833090    orr r3, r3, #144    ; 0x90
    22c4:   e6ff3073    uxth    r3, r3
    22c8:   e51b0014    ldr r0, [fp, #-20]
    22cc:   e3a01016    mov r1, #22 ; 0x16
    22d0:   e1a02003    mov r2, r3
    22d4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            22d4: R_ARM_PC24    MGC_FAPI_WriteReg16
    22d8:   e15b31ba    ldrh    r3, [fp, #-26]
    22dc:   e3833010    orr r3, r3, #16 ; 0x10
    22e0:   e6ff3073    uxth    r3, r3
    22e4:   e51b0014    ldr r0, [fp, #-20]
    22e8:   e3a01016    mov r1, #22 ; 0x16
    22ec:   e1a02003    mov r2, r3
    22f0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            22f0: R_ARM_PC24    MGC_FAPI_WriteReg16
    22f4:   e3a03000    mov r3, #0  ; 0x0
    22f8:   e1a00003    mov r0, r3
    22fc:   e24bd00c    sub sp, fp, #12 ; 0xc
    2300:   e89da800    ldm sp, {fp, sp, pc}
    2304:   00000000    .word   0x00000000
            2304: R_ARM_ABS32   MGC_DiagMsg
    2308:   00000150    .word   0x00000150
            2308: R_ARM_ABS32   .rodata
    230c:   00000000    .word   0x00000000
            230c: R_ARM_ABS32   MGC_bDiagLevel
    2310:   00000160    .word   0x00000160
            2310: R_ARM_ABS32   .rodata

00002314 <MGC_HdrcHaltEndpoint>:
    2314:   e1a0c00d    mov ip, sp
    2318:   e92dd800    push    {fp, ip, lr, pc}
    231c:   e24cb004    sub fp, ip, #4  ; 0x4
    2320:   e24dd020    sub sp, sp, #32 ; 0x20
    2324:   e50b001c    str r0, [fp, #-28]
    2328:   e50b1020    str r1, [fp, #-32]
    232c:   e1a01003    mov r1, r3
    2330:   e1a03002    mov r3, r2
    2334:   e54b3024    strb    r3, [fp, #-36]
    2338:   e1a03001    mov r3, r1
    233c:   e54b3028    strb    r3, [fp, #-40]
    2340:   e3a03000    mov r3, #0  ; 0x0
    2344:   e54b3015    strb    r3, [fp, #-21]
    2348:   e51b301c    ldr r3, [fp, #-28]
    234c:   e5933000    ldr r3, [r3]
    2350:   e50b3014    str r3, [fp, #-20]
    2354:   e51b3014    ldr r3, [fp, #-20]
    2358:   e5933004    ldr r3, [r3, #4]
    235c:   e50b3010    str r3, [fp, #-16]
    2360:   e55b3024    ldrb    r3, [fp, #-36]
    2364:   e6af3073    sxtb    r3, r3
    2368:   e3530000    cmp r3, #0  ; 0x0
    236c:   aa000006    bge 238c <MGC_HdrcHaltEndpoint+0x78>
    2370:   e51b301c    ldr r3, [fp, #-28]
    2374:   e5d3303e    ldrb    r3, [r3, #62]
    2378:   e3530000    cmp r3, #0  ; 0x0
    237c:   1a000008    bne 23a4 <MGC_HdrcHaltEndpoint+0x90>
    2380:   e3a03001    mov r3, #1  ; 0x1
    2384:   e54b3015    strb    r3, [fp, #-21]
    2388:   ea000005    b   23a4 <MGC_HdrcHaltEndpoint+0x90>
    238c:   e51b301c    ldr r3, [fp, #-28]
    2390:   e5d3303e    ldrb    r3, [r3, #62]
    2394:   e3530000    cmp r3, #0  ; 0x0
    2398:   0a000001    beq 23a4 <MGC_HdrcHaltEndpoint+0x90>
    239c:   e3a03001    mov r3, #1  ; 0x1
    23a0:   e54b3015    strb    r3, [fp, #-21]
    23a4:   e51b3020    ldr r3, [fp, #-32]
    23a8:   e5d33000    ldrb    r3, [r3]
    23ac:   e51b0010    ldr r0, [fp, #-16]
    23b0:   e3a0100e    mov r1, #14 ; 0xe
    23b4:   e1a02003    mov r2, r3
    23b8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            23b8: R_ARM_PC24    MGC_FAPI_WriteReg8
    23bc:   e55b3015    ldrb    r3, [fp, #-21]
    23c0:   e3530000    cmp r3, #0  ; 0x0
    23c4:   0a000035    beq 24a0 <MGC_HdrcHaltEndpoint+0x18c>
    23c8:   e51b0010    ldr r0, [fp, #-16]
    23cc:   e3a01012    mov r1, #18 ; 0x12
    23d0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            23d0: R_ARM_PC24    MGC_FAPI_ReadReg16
    23d4:   e1a03000    mov r3, r0
    23d8:   e14b31b8    strh    r3, [fp, #-24]
    23dc:   e55b3028    ldrb    r3, [fp, #-40]
    23e0:   e3530000    cmp r3, #0  ; 0x0
    23e4:   0a000015    beq 2440 <MGC_HdrcHaltEndpoint+0x12c>
    23e8:   e51b301c    ldr r3, [fp, #-28]
    23ec:   e5d3303e    ldrb    r3, [r3, #62]
    23f0:   e3530000    cmp r3, #0  ; 0x0
    23f4:   0a00000a    beq 2424 <MGC_HdrcHaltEndpoint+0x110>
    23f8:   e51b2020    ldr r2, [fp, #-32]
    23fc:   e3a03001    mov r3, #1  ; 0x1
    2400:   e5c23012    strb    r3, [r2, #18]
    2404:   e51b3020    ldr r3, [fp, #-32]
    2408:   e593301c    ldr r3, [r3, #28]
    240c:   e3530000    cmp r3, #0  ; 0x0
    2410:   1a00001c    bne 2488 <MGC_HdrcHaltEndpoint+0x174>
    2414:   e51b3020    ldr r3, [fp, #-32]
    2418:   e3a02001    mov r2, #1  ; 0x1
    241c:   e5c3200f    strb    r2, [r3, #15]
    2420:   ea000018    b   2488 <MGC_HdrcHaltEndpoint+0x174>
    2424:   e15b31b8    ldrh    r3, [fp, #-24]
    2428:   e3c33084    bic r3, r3, #132    ; 0x84
    242c:   e14b31b8    strh    r3, [fp, #-24]
    2430:   e15b31b8    ldrh    r3, [fp, #-24]
    2434:   e3833010    orr r3, r3, #16 ; 0x10
    2438:   e14b31b8    strh    r3, [fp, #-24]
    243c:   ea000011    b   2488 <MGC_HdrcHaltEndpoint+0x174>
    2440:   e51b301c    ldr r3, [fp, #-28]
    2444:   e5d3303e    ldrb    r3, [r3, #62]
    2448:   e3530000    cmp r3, #0  ; 0x0
    244c:   0a000007    beq 2470 <MGC_HdrcHaltEndpoint+0x15c>
    2450:   e51b3020    ldr r3, [fp, #-32]
    2454:   e3a02000    mov r2, #0  ; 0x0
    2458:   e5c3200f    strb    r2, [r3, #15]
    245c:   e51b001c    ldr r0, [fp, #-28]
    2460:   e51b1020    ldr r1, [fp, #-32]
    2464:   e3a02001    mov r2, #1  ; 0x1
    2468:   ebfffffe    bl  0 <MGC_StartNextIrp>
            2468: R_ARM_PC24    MGC_StartNextIrp
    246c:   ea000005    b   2488 <MGC_HdrcHaltEndpoint+0x174>
    2470:   e15b31b8    ldrh    r3, [fp, #-24]
    2474:   e3c33094    bic r3, r3, #148    ; 0x94
    2478:   e14b31b8    strh    r3, [fp, #-24]
    247c:   e15b31b8    ldrh    r3, [fp, #-24]
    2480:   e3833040    orr r3, r3, #64 ; 0x40
    2484:   e14b31b8    strh    r3, [fp, #-24]
    2488:   e15b31b8    ldrh    r3, [fp, #-24]
    248c:   e51b0010    ldr r0, [fp, #-16]
    2490:   e3a01012    mov r1, #18 ; 0x12
    2494:   e1a02003    mov r2, r3
    2498:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2498: R_ARM_PC24    MGC_FAPI_WriteReg16
    249c:   ea000046    b   25bc <MGC_HdrcHaltEndpoint+0x2a8>
    24a0:   e51b0010    ldr r0, [fp, #-16]
    24a4:   e3a01016    mov r1, #22 ; 0x16
    24a8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            24a8: R_ARM_PC24    MGC_FAPI_ReadReg16
    24ac:   e1a03000    mov r3, r0
    24b0:   e14b31b8    strh    r3, [fp, #-24]
    24b4:   e55b3028    ldrb    r3, [fp, #-40]
    24b8:   e3530000    cmp r3, #0  ; 0x0
    24bc:   0a00000f    beq 2500 <MGC_HdrcHaltEndpoint+0x1ec>
    24c0:   e51b301c    ldr r3, [fp, #-28]
    24c4:   e5d3303e    ldrb    r3, [r3, #62]
    24c8:   e3530000    cmp r3, #0  ; 0x0
    24cc:   0a000007    beq 24f0 <MGC_HdrcHaltEndpoint+0x1dc>
    24d0:   e51b2020    ldr r2, [fp, #-32]
    24d4:   e3a03001    mov r3, #1  ; 0x1
    24d8:   e5c23013    strb    r3, [r2, #19]
    24dc:   e15b31b8    ldrh    r3, [fp, #-24]
    24e0:   e3c33a06    bic r3, r3, #24576  ; 0x6000
    24e4:   e3c33020    bic r3, r3, #32 ; 0x20
    24e8:   e14b31b8    strh    r3, [fp, #-24]
    24ec:   ea00002d    b   25a8 <MGC_HdrcHaltEndpoint+0x294>
    24f0:   e15b31b8    ldrh    r3, [fp, #-24]
    24f4:   e3833020    orr r3, r3, #32 ; 0x20
    24f8:   e14b31b8    strh    r3, [fp, #-24]
    24fc:   ea000029    b   25a8 <MGC_HdrcHaltEndpoint+0x294>
    2500:   e51b301c    ldr r3, [fp, #-28]
    2504:   e5d3303e    ldrb    r3, [r3, #62]
    2508:   e3530000    cmp r3, #0  ; 0x0
    250c:   0a00001f    beq 2590 <MGC_HdrcHaltEndpoint+0x27c>
    2510:   e51b2020    ldr r2, [fp, #-32]
    2514:   e3a03000    mov r3, #0  ; 0x0
    2518:   e5c23013    strb    r3, [r2, #19]
    251c:   e51b3020    ldr r3, [fp, #-32]
    2520:   e5933030    ldr r3, [r3, #48]
    2524:   e3530000    cmp r3, #0  ; 0x0
    2528:   0a000013    beq 257c <MGC_HdrcHaltEndpoint+0x268>
    252c:   e51b3020    ldr r3, [fp, #-32]
    2530:   e5933044    ldr r3, [r3, #68]
    2534:   e3530000    cmp r3, #0  ; 0x0
    2538:   0a000006    beq 2558 <MGC_HdrcHaltEndpoint+0x244>
    253c:   e51b3020    ldr r3, [fp, #-32]
    2540:   e5d33050    ldrb    r3, [r3, #80]
    2544:   e3530000    cmp r3, #0  ; 0x0
    2548:   1a000002    bne 2558 <MGC_HdrcHaltEndpoint+0x244>
    254c:   e3a01a06    mov r1, #24576  ; 0x6000
    2550:   e50b102c    str r1, [fp, #-44]
    2554:   ea000001    b   2560 <MGC_HdrcHaltEndpoint+0x24c>
    2558:   e3a03020    mov r3, #32 ; 0x20
    255c:   e50b302c    str r3, [fp, #-44]
    2560:   e15b31b8    ldrh    r3, [fp, #-24]
    2564:   e51b102c    ldr r1, [fp, #-44]
    2568:   e1a02001    mov r2, r1
    256c:   e1823003    orr r3, r2, r3
    2570:   e6ff3073    uxth    r3, r3
    2574:   e14b31b8    strh    r3, [fp, #-24]
    2578:   ea00000a    b   25a8 <MGC_HdrcHaltEndpoint+0x294>
    257c:   e51b001c    ldr r0, [fp, #-28]
    2580:   e51b1020    ldr r1, [fp, #-32]
    2584:   e3a02000    mov r2, #0  ; 0x0
    2588:   ebfffffe    bl  0 <MGC_StartNextIrp>
            2588: R_ARM_PC24    MGC_StartNextIrp
    258c:   ea000005    b   25a8 <MGC_HdrcHaltEndpoint+0x294>
    2590:   e15b31b8    ldrh    r3, [fp, #-24]
    2594:   e3c33020    bic r3, r3, #32 ; 0x20
    2598:   e14b31b8    strh    r3, [fp, #-24]
    259c:   e15b31b8    ldrh    r3, [fp, #-24]
    25a0:   e3833080    orr r3, r3, #128    ; 0x80
    25a4:   e14b31b8    strh    r3, [fp, #-24]
    25a8:   e15b31b8    ldrh    r3, [fp, #-24]
    25ac:   e51b0010    ldr r0, [fp, #-16]
    25b0:   e3a01016    mov r1, #22 ; 0x16
    25b4:   e1a02003    mov r2, r3
    25b8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            25b8: R_ARM_PC24    MGC_FAPI_WriteReg16
    25bc:   e3a03000    mov r3, #0  ; 0x0
    25c0:   e1a00003    mov r0, r3
    25c4:   e24bd00c    sub sp, fp, #12 ; 0xc
    25c8:   e89da800    ldm sp, {fp, sp, pc}

000025cc <MGC_HdrcDefaultEndResponse>:
    25cc:   e1a0c00d    mov ip, sp
    25d0:   e92dd800    push    {fp, ip, lr, pc}
    25d4:   e24cb004    sub fp, ip, #4  ; 0x4
    25d8:   e24dd01c    sub sp, sp, #28 ; 0x1c
    25dc:   e50b0020    str r0, [fp, #-32]
    25e0:   e1a03001    mov r3, r1
    25e4:   e54b3024    strb    r3, [fp, #-36]
    25e8:   e3a03040    mov r3, #64 ; 0x40
    25ec:   e54b3015    strb    r3, [fp, #-21]
    25f0:   e51b3020    ldr r3, [fp, #-32]
    25f4:   e5933000    ldr r3, [r3]
    25f8:   e50b3014    str r3, [fp, #-20]
    25fc:   e51b3014    ldr r3, [fp, #-20]
    2600:   e5933004    ldr r3, [r3, #4]
    2604:   e50b3010    str r3, [fp, #-16]
    2608:   e51b0010    ldr r0, [fp, #-16]
    260c:   e3a0100e    mov r1, #14 ; 0xe
    2610:   e3a02000    mov r2, #0  ; 0x0
    2614:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            2614: R_ARM_PC24    MGC_FAPI_WriteReg8
    2618:   e51b0010    ldr r0, [fp, #-16]
    261c:   e3a01012    mov r1, #18 ; 0x12
    2620:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            2620: R_ARM_PC24    MGC_FAPI_ReadReg16
    2624:   e1a03000    mov r3, r0
    2628:   e14b31b8    strh    r3, [fp, #-24]
    262c:   e15b31b8    ldrh    r3, [fp, #-24]
    2630:   e2033010    and r3, r3, #16 ; 0x10
    2634:   e3530000    cmp r3, #0  ; 0x0
    2638:   0a000003    beq 264c <MGC_HdrcDefaultEndResponse+0x80>
    263c:   e55b3015    ldrb    r3, [fp, #-21]
    2640:   e1e03c83    mvn r3, r3, lsl #25
    2644:   e1e03ca3    mvn r3, r3, lsr #25
    2648:   e54b3015    strb    r3, [fp, #-21]
    264c:   e55b3024    ldrb    r3, [fp, #-36]
    2650:   e3530000    cmp r3, #0  ; 0x0
    2654:   0a000003    beq 2668 <MGC_HdrcDefaultEndResponse+0x9c>
    2658:   e55b3015    ldrb    r3, [fp, #-21]
    265c:   e3833020    orr r3, r3, #32 ; 0x20
    2660:   e54b3015    strb    r3, [fp, #-21]
    2664:   ea000039    b   2750 <MGC_HdrcDefaultEndResponse+0x184>
    2668:   e51b3020    ldr r3, [fp, #-32]
    266c:   e59f2194    ldr r2, [pc, #404]  ; 2808 <MGC_HdrcDefaultEndResponse+0x23c>
    2670:   e19330b2    ldrh    r3, [r3, r2]
    2674:   e3530000    cmp r3, #0  ; 0x0
    2678:   0a00002e    beq 2738 <MGC_HdrcDefaultEndResponse+0x16c>
    267c:   e55b3015    ldrb    r3, [fp, #-21]
    2680:   e51b0010    ldr r0, [fp, #-16]
    2684:   e3a01012    mov r1, #18 ; 0x12
    2688:   e1a02003    mov r2, r3
    268c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            268c: R_ARM_PC24    MGC_FAPI_WriteReg8
    2690:   e3a03000    mov r3, #0  ; 0x0
    2694:   e54b3015    strb    r3, [fp, #-21]
    2698:   e51b2020    ldr r2, [fp, #-32]
    269c:   e59f3164    ldr r3, [pc, #356]  ; 2808 <MGC_HdrcDefaultEndResponse+0x23c>
    26a0:   e19230b3    ldrh    r3, [r2, r3]
    26a4:   e3530040    cmp r3, #64 ; 0x40
    26a8:   23a03040    movcs   r3, #64 ; 0x40
    26ac:   e14b31ba    strh    r3, [fp, #-26]
    26b0:   e15b21ba    ldrh    r2, [fp, #-26]
    26b4:   e51b3020    ldr r3, [fp, #-32]
    26b8:   e59331fc    ldr r3, [r3, #508]
    26bc:   e51b0020    ldr r0, [fp, #-32]
    26c0:   e3a01000    mov r1, #0  ; 0x0
    26c4:   ebfffffe    bl  3f08 <MGC_HdrcLoadFifo>
            26c4: R_ARM_PC24    MGC_HdrcLoadFifo
    26c8:   e51b1020    ldr r1, [fp, #-32]
    26cc:   e59f2138    ldr r2, [pc, #312]  ; 280c <MGC_HdrcDefaultEndResponse+0x240>
    26d0:   e15b31ba    ldrh    r3, [fp, #-26]
    26d4:   e18130b2    strh    r3, [r1, r2]
    26d8:   e55b3015    ldrb    r3, [fp, #-21]
    26dc:   e3833002    orr r3, r3, #2  ; 0x2
    26e0:   e54b3015    strb    r3, [fp, #-21]
    26e4:   e51b2020    ldr r2, [fp, #-32]
    26e8:   e59f3118    ldr r3, [pc, #280]  ; 2808 <MGC_HdrcDefaultEndResponse+0x23c>
    26ec:   e19220b3    ldrh    r2, [r2, r3]
    26f0:   e15b31ba    ldrh    r3, [fp, #-26]
    26f4:   e1530002    cmp r3, r2
    26f8:   1a000014    bne 2750 <MGC_HdrcDefaultEndResponse+0x184>
    26fc:   e55b3015    ldrb    r3, [fp, #-21]
    2700:   e3833008    orr r3, r3, #8  ; 0x8
    2704:   e54b3015    strb    r3, [fp, #-21]
    2708:   e51b2020    ldr r2, [fp, #-32]
    270c:   e3a03001    mov r3, #1  ; 0x1
    2710:   e5c23041    strb    r3, [r2, #65]
    2714:   e51b1020    ldr r1, [fp, #-32]
    2718:   e59f20ec    ldr r2, [pc, #236]  ; 280c <MGC_HdrcDefaultEndResponse+0x240>
    271c:   e3a03000    mov r3, #0  ; 0x0
    2720:   e18130b2    strh    r3, [r1, r2]
    2724:   e51b1020    ldr r1, [fp, #-32]
    2728:   e59f20d8    ldr r2, [pc, #216]  ; 2808 <MGC_HdrcDefaultEndResponse+0x23c>
    272c:   e3a03000    mov r3, #0  ; 0x0
    2730:   e18130b2    strh    r3, [r1, r2]
    2734:   ea000005    b   2750 <MGC_HdrcDefaultEndResponse+0x184>
    2738:   e55b3015    ldrb    r3, [fp, #-21]
    273c:   e3833008    orr r3, r3, #8  ; 0x8
    2740:   e54b3015    strb    r3, [fp, #-21]
    2744:   e51b2020    ldr r2, [fp, #-32]
    2748:   e3a03001    mov r3, #1  ; 0x1
    274c:   e5c23041    strb    r3, [r2, #65]
    2750:   e59f20b8    ldr r2, [pc, #184]  ; 2810 <MGC_HdrcDefaultEndResponse+0x244>
    2754:   e3a03000    mov r3, #0  ; 0x0
    2758:   e5c23000    strb    r3, [r2]
    275c:   e51b3020    ldr r3, [fp, #-32]
    2760:   e5933000    ldr r3, [r3]
    2764:   e5933008    ldr r3, [r3, #8]
    2768:   e5933004    ldr r3, [r3, #4]
    276c:   e59f009c    ldr r0, [pc, #156]  ; 2810 <MGC_HdrcDefaultEndResponse+0x244>
    2770:   e3a01c01    mov r1, #256    ; 0x100
    2774:   e59f2098    ldr r2, [pc, #152]  ; 2814 <MGC_HdrcDefaultEndResponse+0x248>
    2778:   e12fff33    blx r3
    277c:   e51b3020    ldr r3, [fp, #-32]
    2780:   e5933000    ldr r3, [r3]
    2784:   e5933008    ldr r3, [r3, #8]
    2788:   e593c008    ldr ip, [r3, #8]
    278c:   e55b2015    ldrb    r2, [fp, #-21]
    2790:   e3a03002    mov r3, #2  ; 0x2
    2794:   e58d3000    str r3, [sp]
    2798:   e59f0070    ldr r0, [pc, #112]  ; 2810 <MGC_HdrcDefaultEndResponse+0x244>
    279c:   e3a01c01    mov r1, #256    ; 0x100
    27a0:   e3a03010    mov r3, #16 ; 0x10
    27a4:   e12fff3c    blx ip
    27a8:   e59f3068    ldr r3, [pc, #104]  ; 2818 <MGC_HdrcDefaultEndResponse+0x24c>
    27ac:   e5d33000    ldrb    r3, [r3]
    27b0:   e3530002    cmp r3, #2  ; 0x2
    27b4:   9a00000a    bls 27e4 <MGC_HdrcDefaultEndResponse+0x218>
    27b8:   e51b3020    ldr r3, [fp, #-32]
    27bc:   e5933000    ldr r3, [r3]
    27c0:   e593300c    ldr r3, [r3, #12]
    27c4:   e5932028    ldr r2, [r3, #40]
    27c8:   e51b3020    ldr r3, [fp, #-32]
    27cc:   e5933000    ldr r3, [r3]
    27d0:   e593300c    ldr r3, [r3, #12]
    27d4:   e5933004    ldr r3, [r3, #4]
    27d8:   e1a00003    mov r0, r3
    27dc:   e59f102c    ldr r1, [pc, #44]   ; 2810 <MGC_HdrcDefaultEndResponse+0x244>
    27e0:   e12fff32    blx r2
    27e4:   e55b3015    ldrb    r3, [fp, #-21]
    27e8:   e51b0010    ldr r0, [fp, #-16]
    27ec:   e3a01012    mov r1, #18 ; 0x12
    27f0:   e1a02003    mov r2, r3
    27f4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            27f4: R_ARM_PC24    MGC_FAPI_WriteReg8
    27f8:   e3a03000    mov r3, #0  ; 0x0
    27fc:   e1a00003    mov r0, r3
    2800:   e24bd00c    sub sp, fp, #12 ; 0xc
    2804:   e89da800    ldm sp, {fp, sp, pc}
    2808:   000001fa    .word   0x000001fa
    280c:   000001f6    .word   0x000001f6
    2810:   00000000    .word   0x00000000
            2810: R_ARM_ABS32   MGC_DiagMsg
    2814:   00000170    .word   0x00000170
            2814: R_ARM_ABS32   .rodata
    2818:   00000000    .word   0x00000000
            2818: R_ARM_ABS32   MGC_bDiagLevel

0000281c <MGC_HdrcServiceDefaultEnd>:
    281c:   e1a0c00d    mov ip, sp
    2820:   e92dd800    push    {fp, ip, lr, pc}
    2824:   e24cb004    sub fp, ip, #4  ; 0x4
    2828:   e24dd030    sub sp, sp, #48 ; 0x30
    282c:   e50b0020    str r0, [fp, #-32]
    2830:   e50b1024    str r1, [fp, #-36]
    2834:   e3a03000    mov r3, #0  ; 0x0
    2838:   e54b3017    strb    r3, [fp, #-23]
    283c:   e3a03000    mov r3, #0  ; 0x0
    2840:   e54b3016    strb    r3, [fp, #-22]
    2844:   e3a03000    mov r3, #0  ; 0x0
    2848:   e54b3015    strb    r3, [fp, #-21]
    284c:   e51b3020    ldr r3, [fp, #-32]
    2850:   e283300c    add r3, r3, #12 ; 0xc
    2854:   e1a00003    mov r0, r3
    2858:   e3a01000    mov r1, #0  ; 0x0
    285c:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            285c: R_ARM_PC24    MUSB_ArrayFetch
    2860:   e1a03000    mov r3, r0
    2864:   e50b3014    str r3, [fp, #-20]
    2868:   e51b3020    ldr r3, [fp, #-32]
    286c:   e5933000    ldr r3, [r3]
    2870:   e5933000    ldr r3, [r3]
    2874:   e50b3010    str r3, [fp, #-16]
    2878:   e51b0010    ldr r0, [fp, #-16]
    287c:   e3a0100e    mov r1, #14 ; 0xe
    2880:   e3a02000    mov r2, #0  ; 0x0
    2884:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            2884: R_ARM_PC24    MGC_FAPI_WriteReg8
    2888:   e51b0010    ldr r0, [fp, #-16]
    288c:   e3a01012    mov r1, #18 ; 0x12
    2890:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            2890: R_ARM_PC24    MGC_FAPI_ReadReg16
    2894:   e1a03000    mov r3, r0
    2898:   e14b31bc    strh    r3, [fp, #-28]
    289c:   e51b0010    ldr r0, [fp, #-16]
    28a0:   e3a01018    mov r1, #24 ; 0x18
    28a4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            28a4: R_ARM_PC24    MGC_FAPI_ReadReg8
    28a8:   e1a03000    mov r3, r0
    28ac:   e14b31ba    strh    r3, [fp, #-26]
    28b0:   e59f2680    ldr r2, [pc, #1664] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    28b4:   e3a03000    mov r3, #0  ; 0x0
    28b8:   e5c23000    strb    r3, [r2]
    28bc:   e51b3020    ldr r3, [fp, #-32]
    28c0:   e5933000    ldr r3, [r3]
    28c4:   e5933008    ldr r3, [r3, #8]
    28c8:   e5933004    ldr r3, [r3, #4]
    28cc:   e59f0664    ldr r0, [pc, #1636] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    28d0:   e3a01c01    mov r1, #256    ; 0x100
    28d4:   e59f2660    ldr r2, [pc, #1632] ; 2f3c <MGC_HdrcServiceDefaultEnd+0x720>
    28d8:   e12fff33    blx r3
    28dc:   e51b3020    ldr r3, [fp, #-32]
    28e0:   e5933000    ldr r3, [r3]
    28e4:   e5933008    ldr r3, [r3, #8]
    28e8:   e593c008    ldr ip, [r3, #8]
    28ec:   e15b31bc    ldrh    r3, [fp, #-28]
    28f0:   e1a02003    mov r2, r3
    28f4:   e3a03002    mov r3, #2  ; 0x2
    28f8:   e58d3000    str r3, [sp]
    28fc:   e59f0634    ldr r0, [pc, #1588] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2900:   e3a01c01    mov r1, #256    ; 0x100
    2904:   e3a03010    mov r3, #16 ; 0x10
    2908:   e12fff3c    blx ip
    290c:   e59f362c    ldr r3, [pc, #1580] ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    2910:   e5d33000    ldrb    r3, [r3]
    2914:   e3530002    cmp r3, #2  ; 0x2
    2918:   9a00000a    bls 2948 <MGC_HdrcServiceDefaultEnd+0x12c>
    291c:   e51b3020    ldr r3, [fp, #-32]
    2920:   e5933000    ldr r3, [r3]
    2924:   e593300c    ldr r3, [r3, #12]
    2928:   e5932028    ldr r2, [r3, #40]
    292c:   e51b3020    ldr r3, [fp, #-32]
    2930:   e5933000    ldr r3, [r3]
    2934:   e593300c    ldr r3, [r3, #12]
    2938:   e5933004    ldr r3, [r3, #4]
    293c:   e1a00003    mov r0, r3
    2940:   e59f15f0    ldr r1, [pc, #1520] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2944:   e12fff32    blx r2
    2948:   e59f25e8    ldr r2, [pc, #1512] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    294c:   e3a03000    mov r3, #0  ; 0x0
    2950:   e5c23000    strb    r3, [r2]
    2954:   e51b3020    ldr r3, [fp, #-32]
    2958:   e5933000    ldr r3, [r3]
    295c:   e5933008    ldr r3, [r3, #8]
    2960:   e5933004    ldr r3, [r3, #4]
    2964:   e59f05cc    ldr r0, [pc, #1484] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2968:   e3a01c01    mov r1, #256    ; 0x100
    296c:   e59f25d0    ldr r2, [pc, #1488] ; 2f44 <MGC_HdrcServiceDefaultEnd+0x728>
    2970:   e12fff33    blx r3
    2974:   e51b3020    ldr r3, [fp, #-32]
    2978:   e5933000    ldr r3, [r3]
    297c:   e5933008    ldr r3, [r3, #8]
    2980:   e593c008    ldr ip, [r3, #8]
    2984:   e15b21ba    ldrh    r2, [fp, #-26]
    2988:   e3a03002    mov r3, #2  ; 0x2
    298c:   e58d3000    str r3, [sp]
    2990:   e59f05a0    ldr r0, [pc, #1440] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2994:   e3a01c01    mov r1, #256    ; 0x100
    2998:   e3a03010    mov r3, #16 ; 0x10
    299c:   e12fff3c    blx ip
    29a0:   e59f3598    ldr r3, [pc, #1432] ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    29a4:   e5d33000    ldrb    r3, [r3]
    29a8:   e3530002    cmp r3, #2  ; 0x2
    29ac:   9a00000a    bls 29dc <MGC_HdrcServiceDefaultEnd+0x1c0>
    29b0:   e51b3020    ldr r3, [fp, #-32]
    29b4:   e5933000    ldr r3, [r3]
    29b8:   e593300c    ldr r3, [r3, #12]
    29bc:   e5932028    ldr r2, [r3, #40]
    29c0:   e51b3020    ldr r3, [fp, #-32]
    29c4:   e5933000    ldr r3, [r3]
    29c8:   e593300c    ldr r3, [r3, #12]
    29cc:   e5933004    ldr r3, [r3, #4]
    29d0:   e1a00003    mov r0, r3
    29d4:   e59f155c    ldr r1, [pc, #1372] ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    29d8:   e12fff32    blx r2
    29dc:   e51b3020    ldr r3, [fp, #-32]
    29e0:   e5d3303e    ldrb    r3, [r3, #62]
    29e4:   e3530000    cmp r3, #0  ; 0x0
    29e8:   0a00008b    beq 2c1c <MGC_HdrcServiceDefaultEnd+0x400>
    29ec:   e15b31bc    ldrh    r3, [fp, #-28]
    29f0:   e2033004    and r3, r3, #4  ; 0x4
    29f4:   e3530000    cmp r3, #0  ; 0x0
    29f8:   0a000013    beq 2a4c <MGC_HdrcServiceDefaultEnd+0x230>
    29fc:   e59f353c    ldr r3, [pc, #1340] ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    2a00:   e5d33000    ldrb    r3, [r3]
    2a04:   e3530001    cmp r3, #1  ; 0x1
    2a08:   9a00000a    bls 2a38 <MGC_HdrcServiceDefaultEnd+0x21c>
    2a0c:   e51b3020    ldr r3, [fp, #-32]
    2a10:   e5933000    ldr r3, [r3]
    2a14:   e593300c    ldr r3, [r3, #12]
    2a18:   e5932028    ldr r2, [r3, #40]
    2a1c:   e51b3020    ldr r3, [fp, #-32]
    2a20:   e5933000    ldr r3, [r3]
    2a24:   e593300c    ldr r3, [r3, #12]
    2a28:   e5933004    ldr r3, [r3, #4]
    2a2c:   e1a00003    mov r0, r3
    2a30:   e59f1510    ldr r1, [pc, #1296] ; 2f48 <MGC_HdrcServiceDefaultEnd+0x72c>
    2a34:   e12fff32    blx r2
    2a38:   e3e0307f    mvn r3, #127    ; 0x7f
    2a3c:   e54b3015    strb    r3, [fp, #-21]
    2a40:   e3a03001    mov r3, #1  ; 0x1
    2a44:   e54b3017    strb    r3, [fp, #-23]
    2a48:   ea00002e    b   2b08 <MGC_HdrcServiceDefaultEnd+0x2ec>
    2a4c:   e15b31bc    ldrh    r3, [fp, #-28]
    2a50:   e2033010    and r3, r3, #16 ; 0x10
    2a54:   e3530000    cmp r3, #0  ; 0x0
    2a58:   0a000013    beq 2aac <MGC_HdrcServiceDefaultEnd+0x290>
    2a5c:   e59f34dc    ldr r3, [pc, #1244] ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    2a60:   e5d33000    ldrb    r3, [r3]
    2a64:   e3530001    cmp r3, #1  ; 0x1
    2a68:   9a00000a    bls 2a98 <MGC_HdrcServiceDefaultEnd+0x27c>
    2a6c:   e51b3020    ldr r3, [fp, #-32]
    2a70:   e5933000    ldr r3, [r3]
    2a74:   e593300c    ldr r3, [r3, #12]
    2a78:   e5932028    ldr r2, [r3, #40]
    2a7c:   e51b3020    ldr r3, [fp, #-32]
    2a80:   e5933000    ldr r3, [r3]
    2a84:   e593300c    ldr r3, [r3, #12]
    2a88:   e5933004    ldr r3, [r3, #4]
    2a8c:   e1a00003    mov r0, r3
    2a90:   e59f14b4    ldr r1, [pc, #1204] ; 2f4c <MGC_HdrcServiceDefaultEnd+0x730>
    2a94:   e12fff32    blx r2
    2a98:   e3e0307d    mvn r3, #125    ; 0x7d
    2a9c:   e54b3015    strb    r3, [fp, #-21]
    2aa0:   e3a03001    mov r3, #1  ; 0x1
    2aa4:   e54b3017    strb    r3, [fp, #-23]
    2aa8:   ea000016    b   2b08 <MGC_HdrcServiceDefaultEnd+0x2ec>
    2aac:   e15b31bc    ldrh    r3, [fp, #-28]
    2ab0:   e2033080    and r3, r3, #128    ; 0x80
    2ab4:   e3530000    cmp r3, #0  ; 0x0
    2ab8:   0a000012    beq 2b08 <MGC_HdrcServiceDefaultEnd+0x2ec>
    2abc:   e59f347c    ldr r3, [pc, #1148] ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    2ac0:   e5d33000    ldrb    r3, [r3]
    2ac4:   e3530001    cmp r3, #1  ; 0x1
    2ac8:   9a00000a    bls 2af8 <MGC_HdrcServiceDefaultEnd+0x2dc>
    2acc:   e51b3020    ldr r3, [fp, #-32]
    2ad0:   e5933000    ldr r3, [r3]
    2ad4:   e593300c    ldr r3, [r3, #12]
    2ad8:   e5932028    ldr r2, [r3, #40]
    2adc:   e51b3020    ldr r3, [fp, #-32]
    2ae0:   e5933000    ldr r3, [r3]
    2ae4:   e593300c    ldr r3, [r3, #12]
    2ae8:   e5933004    ldr r3, [r3, #4]
    2aec:   e1a00003    mov r0, r3
    2af0:   e59f1458    ldr r1, [pc, #1112] ; 2f50 <MGC_HdrcServiceDefaultEnd+0x734>
    2af4:   e12fff32    blx r2
    2af8:   e3e0307d    mvn r3, #125    ; 0x7d
    2afc:   e54b3015    strb    r3, [fp, #-21]
    2b00:   e3a03001    mov r3, #1  ; 0x1
    2b04:   e54b3017    strb    r3, [fp, #-23]
    2b08:   e55b3015    ldrb    r3, [fp, #-21]
    2b0c:   e3530082    cmp r3, #130    ; 0x82
    2b10:   1a000039    bne 2bfc <MGC_HdrcServiceDefaultEnd+0x3e0>
    2b14:   e15b31bc    ldrh    r3, [fp, #-28]
    2b18:   e2033020    and r3, r3, #32 ; 0x20
    2b1c:   e3530000    cmp r3, #0  ; 0x0
    2b20:   0a00001a    beq 2b90 <MGC_HdrcServiceDefaultEnd+0x374>
    2b24:   e15b31bc    ldrh    r3, [fp, #-28]
    2b28:   e50b3038    str r3, [fp, #-56]
    2b2c:   e59f3420    ldr r3, [pc, #1056] ; 2f54 <MGC_HdrcServiceDefaultEnd+0x738>
    2b30:   e51b2038    ldr r2, [fp, #-56]
    2b34:   e0023003    and r3, r2, r3
    2b38:   e50b3038    str r3, [fp, #-56]
    2b3c:   e51b3038    ldr r3, [fp, #-56]
    2b40:   e14b31bc    strh    r3, [fp, #-28]
    2b44:   e15b31bc    ldrh    r3, [fp, #-28]
    2b48:   e51b0010    ldr r0, [fp, #-16]
    2b4c:   e3a01012    mov r1, #18 ; 0x12
    2b50:   e1a02003    mov r2, r3
    2b54:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2b54: R_ARM_PC24    MGC_FAPI_WriteReg16
    2b58:   e15b31bc    ldrh    r3, [fp, #-28]
    2b5c:   e50b3034    str r3, [fp, #-52]
    2b60:   e59f33f0    ldr r3, [pc, #1008] ; 2f58 <MGC_HdrcServiceDefaultEnd+0x73c>
    2b64:   e51b2034    ldr r2, [fp, #-52]
    2b68:   e0023003    and r3, r2, r3
    2b6c:   e50b3034    str r3, [fp, #-52]
    2b70:   e51b3034    ldr r3, [fp, #-52]
    2b74:   e14b31bc    strh    r3, [fp, #-28]
    2b78:   e15b31bc    ldrh    r3, [fp, #-28]
    2b7c:   e51b0010    ldr r0, [fp, #-16]
    2b80:   e3a01012    mov r1, #18 ; 0x12
    2b84:   e1a02003    mov r2, r3
    2b88:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2b88: R_ARM_PC24    MGC_FAPI_WriteReg16
    2b8c:   ea00001a    b   2bfc <MGC_HdrcServiceDefaultEnd+0x3e0>
    2b90:   e15b31bc    ldrh    r3, [fp, #-28]
    2b94:   e3833c01    orr r3, r3, #256    ; 0x100
    2b98:   e6ff3073    uxth    r3, r3
    2b9c:   e14b31bc    strh    r3, [fp, #-28]
    2ba0:   e15b31bc    ldrh    r3, [fp, #-28]
    2ba4:   e51b0010    ldr r0, [fp, #-16]
    2ba8:   e3a01012    mov r1, #18 ; 0x12
    2bac:   e1a02003    mov r2, r3
    2bb0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2bb0: R_ARM_PC24    MGC_FAPI_WriteReg16
    2bb4:   e15b31bc    ldrh    r3, [fp, #-28]
    2bb8:   e51b0010    ldr r0, [fp, #-16]
    2bbc:   e3a01012    mov r1, #18 ; 0x12
    2bc0:   e1a02003    mov r2, r3
    2bc4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2bc4: R_ARM_PC24    MGC_FAPI_WriteReg16
    2bc8:   e15b31bc    ldrh    r3, [fp, #-28]
    2bcc:   e50b3030    str r3, [fp, #-48]
    2bd0:   e59f3380    ldr r3, [pc, #896]  ; 2f58 <MGC_HdrcServiceDefaultEnd+0x73c>
    2bd4:   e51b2030    ldr r2, [fp, #-48]
    2bd8:   e0023003    and r3, r2, r3
    2bdc:   e50b3030    str r3, [fp, #-48]
    2be0:   e51b3030    ldr r3, [fp, #-48]
    2be4:   e14b31bc    strh    r3, [fp, #-28]
    2be8:   e15b31bc    ldrh    r3, [fp, #-28]
    2bec:   e51b0010    ldr r0, [fp, #-16]
    2bf0:   e3a01012    mov r1, #18 ; 0x12
    2bf4:   e1a02003    mov r2, r3
    2bf8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2bf8: R_ARM_PC24    MGC_FAPI_WriteReg16
    2bfc:   e55b3017    ldrb    r3, [fp, #-23]
    2c00:   e3530000    cmp r3, #0  ; 0x0
    2c04:   0a00006b    beq 2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2c08:   e51b0010    ldr r0, [fp, #-16]
    2c0c:   e3a01012    mov r1, #18 ; 0x12
    2c10:   e3a02000    mov r2, #0  ; 0x0
    2c14:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2c14: R_ARM_PC24    MGC_FAPI_WriteReg16
    2c18:   ea000066    b   2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2c1c:   e15b31bc    ldrh    r3, [fp, #-28]
    2c20:   e2033004    and r3, r3, #4  ; 0x4
    2c24:   e3530000    cmp r3, #0  ; 0x0
    2c28:   0a00000f    beq 2c6c <MGC_HdrcServiceDefaultEnd+0x450>
    2c2c:   e3a03001    mov r3, #1  ; 0x1
    2c30:   e54b3017    strb    r3, [fp, #-23]
    2c34:   e15b31bc    ldrh    r3, [fp, #-28]
    2c38:   e50b302c    str r3, [fp, #-44]
    2c3c:   e59f3318    ldr r3, [pc, #792]  ; 2f5c <MGC_HdrcServiceDefaultEnd+0x740>
    2c40:   e51b202c    ldr r2, [fp, #-44]
    2c44:   e0023003    and r3, r2, r3
    2c48:   e50b302c    str r3, [fp, #-44]
    2c4c:   e51b302c    ldr r3, [fp, #-44]
    2c50:   e14b31bc    strh    r3, [fp, #-28]
    2c54:   e15b31bc    ldrh    r3, [fp, #-28]
    2c58:   e51b0010    ldr r0, [fp, #-16]
    2c5c:   e3a01012    mov r1, #18 ; 0x12
    2c60:   e1a02003    mov r2, r3
    2c64:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2c64: R_ARM_PC24    MGC_FAPI_WriteReg16
    2c68:   ea000052    b   2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2c6c:   e15b31bc    ldrh    r3, [fp, #-28]
    2c70:   e2033010    and r3, r3, #16 ; 0x10
    2c74:   e3530000    cmp r3, #0  ; 0x0
    2c78:   0a000008    beq 2ca0 <MGC_HdrcServiceDefaultEnd+0x484>
    2c7c:   e51b1020    ldr r1, [fp, #-32]
    2c80:   e59f22d8    ldr r2, [pc, #728]  ; 2f60 <MGC_HdrcServiceDefaultEnd+0x744>
    2c84:   e3a03000    mov r3, #0  ; 0x0
    2c88:   e18130b2    strh    r3, [r1, r2]
    2c8c:   e51b1020    ldr r1, [fp, #-32]
    2c90:   e59f22cc    ldr r2, [pc, #716]  ; 2f64 <MGC_HdrcServiceDefaultEnd+0x748>
    2c94:   e3a03000    mov r3, #0  ; 0x0
    2c98:   e18130b2    strh    r3, [r1, r2]
    2c9c:   ea000045    b   2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2ca0:   e15b31ba    ldrh    r3, [fp, #-26]
    2ca4:   e3530000    cmp r3, #0  ; 0x0
    2ca8:   1a000042    bne 2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2cac:   e51b3020    ldr r3, [fp, #-32]
    2cb0:   e5d33041    ldrb    r3, [r3, #65]
    2cb4:   e3530000    cmp r3, #0  ; 0x0
    2cb8:   0a00003e    beq 2db8 <MGC_HdrcServiceDefaultEnd+0x59c>
    2cbc:   e51b2020    ldr r2, [fp, #-32]
    2cc0:   e3a03000    mov r3, #0  ; 0x0
    2cc4:   e5c23041    strb    r3, [r2, #65]
    2cc8:   e51b0010    ldr r0, [fp, #-16]
    2ccc:   e3a01012    mov r1, #18 ; 0x12
    2cd0:   e3a02000    mov r2, #0  ; 0x0
    2cd4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2cd4: R_ARM_PC24    MGC_FAPI_WriteReg16
    2cd8:   e51b3020    ldr r3, [fp, #-32]
    2cdc:   e5d331f3    ldrb    r3, [r3, #499]
    2ce0:   e3530000    cmp r3, #0  ; 0x0
    2ce4:   0a000004    beq 2cfc <MGC_HdrcServiceDefaultEnd+0x4e0>
    2ce8:   e51b0020    ldr r0, [fp, #-32]
    2cec:   ebfffffe    bl  788 <MGC_HdrcProgramBusState>
            2cec: R_ARM_PC24    MGC_HdrcProgramBusState
    2cf0:   e51b3020    ldr r3, [fp, #-32]
    2cf4:   e3a02000    mov r2, #0  ; 0x0
    2cf8:   e5c321f3    strb    r2, [r3, #499]
    2cfc:   e51b3020    ldr r3, [fp, #-32]
    2d00:   e5d3302b    ldrb    r3, [r3, #43]
    2d04:   e3530000    cmp r3, #0  ; 0x0
    2d08:   0a000027    beq 2dac <MGC_HdrcServiceDefaultEnd+0x590>
    2d0c:   e51b2020    ldr r2, [fp, #-32]
    2d10:   e3a03000    mov r3, #0  ; 0x0
    2d14:   e5c2302b    strb    r3, [r2, #43]
    2d18:   e3a03000    mov r3, #0  ; 0x0
    2d1c:   e54b3018    strb    r3, [fp, #-24]
    2d20:   e51b3020    ldr r3, [fp, #-32]
    2d24:   e5d3302c    ldrb    r3, [r3, #44]
    2d28:   e2433001    sub r3, r3, #1  ; 0x1
    2d2c:   e3530003    cmp r3, #3  ; 0x3
    2d30:   979ff103    ldrls   pc, [pc, r3, lsl #2]
    2d34:   ea000017    b   2d98 <MGC_HdrcServiceDefaultEnd+0x57c>
    2d38:   00002d48    .word   0x00002d48
            2d38: R_ARM_ABS32   .text
    2d3c:   00002d54    .word   0x00002d54
            2d3c: R_ARM_ABS32   .text
    2d40:   00002d60    .word   0x00002d60
            2d40: R_ARM_ABS32   .text
    2d44:   00002d6c    .word   0x00002d6c
            2d44: R_ARM_ABS32   .text
    2d48:   e3a03002    mov r3, #2  ; 0x2
    2d4c:   e54b3018    strb    r3, [fp, #-24]
    2d50:   ea000010    b   2d98 <MGC_HdrcServiceDefaultEnd+0x57c>
    2d54:   e3a03004    mov r3, #4  ; 0x4
    2d58:   e54b3018    strb    r3, [fp, #-24]
    2d5c:   ea00000d    b   2d98 <MGC_HdrcServiceDefaultEnd+0x57c>
    2d60:   e3a03001    mov r3, #1  ; 0x1
    2d64:   e54b3018    strb    r3, [fp, #-24]
    2d68:   ea00000a    b   2d98 <MGC_HdrcServiceDefaultEnd+0x57c>
    2d6c:   e3a03008    mov r3, #8  ; 0x8
    2d70:   e54b3018    strb    r3, [fp, #-24]
    2d74:   e51b0020    ldr r0, [fp, #-32]
    2d78:   e3a01000    mov r1, #0  ; 0x0
    2d7c:   e3a02035    mov r2, #53 ; 0x35
    2d80:   e59f31e0    ldr r3, [pc, #480]  ; 2f68 <MGC_HdrcServiceDefaultEnd+0x74c>
    2d84:   ebfffffe    bl  3f08 <MGC_HdrcLoadFifo>
            2d84: R_ARM_PC24    MGC_HdrcLoadFifo
    2d88:   e51b0010    ldr r0, [fp, #-16]
    2d8c:   e3a01012    mov r1, #18 ; 0x12
    2d90:   e3a02002    mov r2, #2  ; 0x2
    2d94:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2d94: R_ARM_PC24    MGC_FAPI_WriteReg16
    2d98:   e55b3018    ldrb    r3, [fp, #-24]
    2d9c:   e51b0010    ldr r0, [fp, #-16]
    2da0:   e3a0100f    mov r1, #15 ; 0xf
    2da4:   e1a02003    mov r2, r3
    2da8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            2da8: R_ARM_PC24    MGC_FAPI_WriteReg8
    2dac:   e3a02000    mov r2, #0  ; 0x0
    2db0:   e50b2028    str r2, [fp, #-40]
    2db4:   ea00005b    b   2f28 <MGC_HdrcServiceDefaultEnd+0x70c>
    2db8:   e55b3017    ldrb    r3, [fp, #-23]
    2dbc:   e3530000    cmp r3, #0  ; 0x0
    2dc0:   0a000013    beq 2e14 <MGC_HdrcServiceDefaultEnd+0x5f8>
    2dc4:   e51b3020    ldr r3, [fp, #-32]
    2dc8:   e5d3303e    ldrb    r3, [r3, #62]
    2dcc:   e3530000    cmp r3, #0  ; 0x0
    2dd0:   0a000052    beq 2f20 <MGC_HdrcServiceDefaultEnd+0x704>
    2dd4:   e55b2015    ldrb    r2, [fp, #-21]
    2dd8:   e51b3014    ldr r3, [fp, #-20]
    2ddc:   e593301c    ldr r3, [r3, #28]
    2de0:   e58d3000    str r3, [sp]
    2de4:   e51b0024    ldr r0, [fp, #-36]
    2de8:   e51b1014    ldr r1, [fp, #-20]
    2dec:   e3a03001    mov r3, #1  ; 0x1
    2df0:   ebfffffe    bl  0 <MGC_CompleteIrp>
            2df0: R_ARM_PC24    MGC_CompleteIrp
    2df4:   e1a03000    mov r3, r0
    2df8:   e3530000    cmp r3, #0  ; 0x0
    2dfc:   0a000001    beq 2e08 <MGC_HdrcServiceDefaultEnd+0x5ec>
    2e00:   e3a03001    mov r3, #1  ; 0x1
    2e04:   e54b3016    strb    r3, [fp, #-22]
    2e08:   e51b0020    ldr r0, [fp, #-32]
    2e0c:   ebfffffe    bl  0 <MGC_StartNextControlTransfer>
            2e0c: R_ARM_PC24    MGC_StartNextControlTransfer
    2e10:   ea000042    b   2f20 <MGC_HdrcServiceDefaultEnd+0x704>
    2e14:   e15bc1ba    ldrh    ip, [fp, #-26]
    2e18:   e24b301c    sub r3, fp, #28 ; 0x1c
    2e1c:   e51b0020    ldr r0, [fp, #-32]
    2e20:   e51b1024    ldr r1, [fp, #-36]
    2e24:   e1a02003    mov r2, r3
    2e28:   e1a0300c    mov r3, ip
    2e2c:   ebfffffe    bl  0 <MGC_DrcServiceDefaultEnd>
            2e2c: R_ARM_PC24    MGC_DrcServiceDefaultEnd
    2e30:   e1a03000    mov r3, r0
    2e34:   e54b3016    strb    r3, [fp, #-22]
    2e38:   e15b31bc    ldrh    r3, [fp, #-28]
    2e3c:   e3530000    cmp r3, #0  ; 0x0
    2e40:   0a000036    beq 2f20 <MGC_HdrcServiceDefaultEnd+0x704>
    2e44:   e51b3020    ldr r3, [fp, #-32]
    2e48:   e5d3303e    ldrb    r3, [r3, #62]
    2e4c:   e3530000    cmp r3, #0  ; 0x0
    2e50:   0a000007    beq 2e74 <MGC_HdrcServiceDefaultEnd+0x658>
    2e54:   e51b3020    ldr r3, [fp, #-32]
    2e58:   e5d33046    ldrb    r3, [r3, #70]
    2e5c:   e3530000    cmp r3, #0  ; 0x0
    2e60:   0a000003    beq 2e74 <MGC_HdrcServiceDefaultEnd+0x658>
    2e64:   e15b31bc    ldrh    r3, [fp, #-28]
    2e68:   e3833b02    orr r3, r3, #2048   ; 0x800
    2e6c:   e6ff3073    uxth    r3, r3
    2e70:   e14b31bc    strh    r3, [fp, #-28]
    2e74:   e59f20bc    ldr r2, [pc, #188]  ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2e78:   e3a03000    mov r3, #0  ; 0x0
    2e7c:   e5c23000    strb    r3, [r2]
    2e80:   e51b3020    ldr r3, [fp, #-32]
    2e84:   e5933000    ldr r3, [r3]
    2e88:   e5933008    ldr r3, [r3, #8]
    2e8c:   e5933004    ldr r3, [r3, #4]
    2e90:   e59f00a0    ldr r0, [pc, #160]  ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2e94:   e3a01c01    mov r1, #256    ; 0x100
    2e98:   e59f20cc    ldr r2, [pc, #204]  ; 2f6c <MGC_HdrcServiceDefaultEnd+0x750>
    2e9c:   e12fff33    blx r3
    2ea0:   e51b3020    ldr r3, [fp, #-32]
    2ea4:   e5933000    ldr r3, [r3]
    2ea8:   e5933008    ldr r3, [r3, #8]
    2eac:   e593c008    ldr ip, [r3, #8]
    2eb0:   e15b31bc    ldrh    r3, [fp, #-28]
    2eb4:   e1a02003    mov r2, r3
    2eb8:   e3a03002    mov r3, #2  ; 0x2
    2ebc:   e58d3000    str r3, [sp]
    2ec0:   e59f0070    ldr r0, [pc, #112]  ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2ec4:   e3a01c01    mov r1, #256    ; 0x100
    2ec8:   e3a03010    mov r3, #16 ; 0x10
    2ecc:   e12fff3c    blx ip
    2ed0:   e59f3068    ldr r3, [pc, #104]  ; 2f40 <MGC_HdrcServiceDefaultEnd+0x724>
    2ed4:   e5d33000    ldrb    r3, [r3]
    2ed8:   e3530002    cmp r3, #2  ; 0x2
    2edc:   9a00000a    bls 2f0c <MGC_HdrcServiceDefaultEnd+0x6f0>
    2ee0:   e51b3020    ldr r3, [fp, #-32]
    2ee4:   e5933000    ldr r3, [r3]
    2ee8:   e593300c    ldr r3, [r3, #12]
    2eec:   e5932028    ldr r2, [r3, #40]
    2ef0:   e51b3020    ldr r3, [fp, #-32]
    2ef4:   e5933000    ldr r3, [r3]
    2ef8:   e593300c    ldr r3, [r3, #12]
    2efc:   e5933004    ldr r3, [r3, #4]
    2f00:   e1a00003    mov r0, r3
    2f04:   e59f102c    ldr r1, [pc, #44]   ; 2f38 <MGC_HdrcServiceDefaultEnd+0x71c>
    2f08:   e12fff32    blx r2
    2f0c:   e15b31bc    ldrh    r3, [fp, #-28]
    2f10:   e51b0010    ldr r0, [fp, #-16]
    2f14:   e3a01012    mov r1, #18 ; 0x12
    2f18:   e1a02003    mov r2, r3
    2f1c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            2f1c: R_ARM_PC24    MGC_FAPI_WriteReg16
    2f20:   e55b3016    ldrb    r3, [fp, #-22]
    2f24:   e50b3028    str r3, [fp, #-40]
    2f28:   e51b3028    ldr r3, [fp, #-40]
    2f2c:   e1a00003    mov r0, r3
    2f30:   e24bd00c    sub sp, fp, #12 ; 0xc
    2f34:   e89da800    ldm sp, {fp, sp, pc}
    2f38:   00000000    .word   0x00000000
            2f38: R_ARM_ABS32   MGC_DiagMsg
    2f3c:   00000198    .word   0x00000198
            2f3c: R_ARM_ABS32   .rodata
    2f40:   00000000    .word   0x00000000
            2f40: R_ARM_ABS32   MGC_bDiagLevel
    2f44:   000001a0    .word   0x000001a0
            2f44: R_ARM_ABS32   .rodata
    2f48:   000001a8    .word   0x000001a8
            2f48: R_ARM_ABS32   .rodata
    2f4c:   000001b8    .word   0x000001b8
            2f4c: R_ARM_ABS32   .rodata
    2f50:   000001d4    .word   0x000001d4
            2f50: R_ARM_ABS32   .rodata
    2f54:   0000ffdf    .word   0x0000ffdf
    2f58:   0000ff7f    .word   0x0000ff7f
    2f5c:   0000fffb    .word   0x0000fffb
    2f60:   000001f6    .word   0x000001f6
    2f64:   000001fa    .word   0x000001fa
    2f68:   00000000    .word   0x00000000
            2f68: R_ARM_ABS32   .rodata
    2f6c:   000001f8    .word   0x000001f8
            2f6c: R_ARM_ABS32   .rodata

00002f70 <MGC_HdrcServiceTransmitAvail>:
    2f70:   e1a0c00d    mov ip, sp
    2f74:   e92dd800    push    {fp, ip, lr, pc}
    2f78:   e24cb004    sub fp, ip, #4  ; 0x4
    2f7c:   e24dd030    sub sp, sp, #48 ; 0x30
    2f80:   e50b002c    str r0, [fp, #-44]
    2f84:   e1a03001    mov r3, r1
    2f88:   e50b2034    str r2, [fp, #-52]
    2f8c:   e14b33b0    strh    r3, [fp, #-48]
    2f90:   e3a03000    mov r3, #0  ; 0x0
    2f94:   e54b301d    strb    r3, [fp, #-29]
    2f98:   e3a03000    mov r3, #0  ; 0x0
    2f9c:   e50b301c    str r3, [fp, #-28]
    2fa0:   e15b33b0    ldrh    r3, [fp, #-48]
    2fa4:   e54b3015    strb    r3, [fp, #-21]
    2fa8:   e51b302c    ldr r3, [fp, #-44]
    2fac:   e5933000    ldr r3, [r3]
    2fb0:   e50b3014    str r3, [fp, #-20]
    2fb4:   e51b3014    ldr r3, [fp, #-20]
    2fb8:   e5933004    ldr r3, [r3, #4]
    2fbc:   e50b3010    str r3, [fp, #-16]
    2fc0:   e51b2034    ldr r2, [fp, #-52]
    2fc4:   e3a03000    mov r3, #0  ; 0x0
    2fc8:   e5c23006    strb    r3, [r2, #6]
    2fcc:   e51b2034    ldr r2, [fp, #-52]
    2fd0:   e3a03000    mov r3, #0  ; 0x0
    2fd4:   e5c23004    strb    r3, [r2, #4]
    2fd8:   e55b3015    ldrb    r3, [fp, #-21]
    2fdc:   e51b0010    ldr r0, [fp, #-16]
    2fe0:   e3a0100e    mov r1, #14 ; 0xe
    2fe4:   e1a02003    mov r2, r3
    2fe8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            2fe8: R_ARM_PC24    MGC_FAPI_WriteReg8
    2fec:   e51b0010    ldr r0, [fp, #-16]
    2ff0:   e3a01012    mov r1, #18 ; 0x12
    2ff4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            2ff4: R_ARM_PC24    MGC_FAPI_ReadReg16
    2ff8:   e1a03000    mov r3, r0
    2ffc:   e14b32b6    strh    r3, [fp, #-38]
    3000:   e51b302c    ldr r3, [fp, #-44]
    3004:   e283300c    add r3, r3, #12 ; 0xc
    3008:   e15b23b0    ldrh    r2, [fp, #-48]
    300c:   e1a00003    mov r0, r3
    3010:   e1a01002    mov r1, r2
    3014:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            3014: R_ARM_PC24    MUSB_ArrayFetch
    3018:   e1a03000    mov r3, r0
    301c:   e50b3024    str r3, [fp, #-36]
    3020:   e51b302c    ldr r3, [fp, #-44]
    3024:   e5d3303e    ldrb    r3, [r3, #62]
    3028:   e3530000    cmp r3, #0  ; 0x0
    302c:   0a0000a8    beq 32d4 <MGC_HdrcServiceTransmitAvail+0x364>
    3030:   e15b32b6    ldrh    r3, [fp, #-38]
    3034:   e2033020    and r3, r3, #32 ; 0x20
    3038:   e3530000    cmp r3, #0  ; 0x0
    303c:   0a000001    beq 3048 <MGC_HdrcServiceTransmitAvail+0xd8>
    3040:   e3a03080    mov r3, #128    ; 0x80
    3044:   e50b301c    str r3, [fp, #-28]
    3048:   e15b32b6    ldrh    r3, [fp, #-38]
    304c:   e2033004    and r3, r3, #4  ; 0x4
    3050:   e3530000    cmp r3, #0  ; 0x0
    3054:   1a000003    bne 3068 <MGC_HdrcServiceTransmitAvail+0xf8>
    3058:   e15b32b6    ldrh    r3, [fp, #-38]
    305c:   e2033080    and r3, r3, #128    ; 0x80
    3060:   e3530000    cmp r3, #0  ; 0x0
    3064:   0a000011    beq 30b0 <MGC_HdrcServiceTransmitAvail+0x140>
    3068:   e3a03088    mov r3, #136    ; 0x88
    306c:   e50b301c    str r3, [fp, #-28]
    3070:   e15b32b6    ldrh    r3, [fp, #-38]
    3074:   e3c33002    bic r3, r3, #2  ; 0x2
    3078:   e14b32b6    strh    r3, [fp, #-38]
    307c:   e15b32b6    ldrh    r3, [fp, #-38]
    3080:   e3833008    orr r3, r3, #8  ; 0x8
    3084:   e14b32b6    strh    r3, [fp, #-38]
    3088:   e15b32b6    ldrh    r3, [fp, #-38]
    308c:   e51b0010    ldr r0, [fp, #-16]
    3090:   e3a01012    mov r1, #18 ; 0x12
    3094:   e1a02003    mov r2, r3
    3098:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3098: R_ARM_PC24    MGC_FAPI_WriteReg16
    309c:   e15b32b6    ldrh    r3, [fp, #-38]
    30a0:   e51b0010    ldr r0, [fp, #-16]
    30a4:   e3a01012    mov r1, #18 ; 0x12
    30a8:   e1a02003    mov r2, r3
    30ac:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            30ac: R_ARM_PC24    MGC_FAPI_WriteReg16
    30b0:   e51b301c    ldr r3, [fp, #-28]
    30b4:   e3530000    cmp r3, #0  ; 0x0
    30b8:   0a000021    beq 3144 <MGC_HdrcServiceTransmitAvail+0x1d4>
    30bc:   e15b32b6    ldrh    r3, [fp, #-38]
    30c0:   e3c33d42    bic r3, r3, #4224   ; 0x1080
    30c4:   e3c33024    bic r3, r3, #36 ; 0x24
    30c8:   e14b32b6    strh    r3, [fp, #-38]
    30cc:   e15b32b6    ldrh    r3, [fp, #-38]
    30d0:   e3833b02    orr r3, r3, #2048   ; 0x800
    30d4:   e14b32b6    strh    r3, [fp, #-38]
    30d8:   e15b32b6    ldrh    r3, [fp, #-38]
    30dc:   e51b0010    ldr r0, [fp, #-16]
    30e0:   e3a01012    mov r1, #18 ; 0x12
    30e4:   e1a02003    mov r2, r3
    30e8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            30e8: R_ARM_PC24    MGC_FAPI_WriteReg16
    30ec:   e51b301c    ldr r3, [fp, #-28]
    30f0:   e6ef3073    uxtb    r3, r3
    30f4:   e1a02003    mov r2, r3
    30f8:   e51b3024    ldr r3, [fp, #-36]
    30fc:   e593301c    ldr r3, [r3, #28]
    3100:   e58d3000    str r3, [sp]
    3104:   e51b0034    ldr r0, [fp, #-52]
    3108:   e51b1024    ldr r1, [fp, #-36]
    310c:   e3a03001    mov r3, #1  ; 0x1
    3110:   ebfffffe    bl  0 <MGC_CompleteIrp>
            3110: R_ARM_PC24    MGC_CompleteIrp
    3114:   e1a03000    mov r3, r0
    3118:   e54b301d    strb    r3, [fp, #-29]
    311c:   e51b3024    ldr r3, [fp, #-36]
    3120:   e5d33012    ldrb    r3, [r3, #18]
    3124:   e3530000    cmp r3, #0  ; 0x0
    3128:   0a000002    beq 3138 <MGC_HdrcServiceTransmitAvail+0x1c8>
    312c:   e51b3024    ldr r3, [fp, #-36]
    3130:   e3a02001    mov r2, #1  ; 0x1
    3134:   e5c3200f    strb    r2, [r3, #15]
    3138:   e55b301d    ldrb    r3, [fp, #-29]
    313c:   e50b3038    str r3, [fp, #-56]
    3140:   ea0000d2    b   3490 <MGC_HdrcServiceTransmitAvail+0x520>
    3144:   e51b3024    ldr r3, [fp, #-36]
    3148:   e3530000    cmp r3, #0  ; 0x0
    314c:   0a000020    beq 31d4 <MGC_HdrcServiceTransmitAvail+0x264>
    3150:   e51b3024    ldr r3, [fp, #-36]
    3154:   e5933044    ldr r3, [r3, #68]
    3158:   e3530000    cmp r3, #0  ; 0x0
    315c:   0a00000f    beq 31a0 <MGC_HdrcServiceTransmitAvail+0x230>
    3160:   e51b302c    ldr r3, [fp, #-44]
    3164:   e5933214    ldr r3, [r3, #532]
    3168:   e5932020    ldr r2, [r3, #32]
    316c:   e51b3024    ldr r3, [fp, #-36]
    3170:   e5933044    ldr r3, [r3, #68]
    3174:   e1a00003    mov r0, r3
    3178:   e12fff32    blx r2
    317c:   e1a03000    mov r3, r0
    3180:   e3530001    cmp r3, #1  ; 0x1
    3184:   1a00000c    bne 31bc <MGC_HdrcServiceTransmitAvail+0x24c>
    3188:   e51b3024    ldr r3, [fp, #-36]
    318c:   e5933044    ldr r3, [r3, #68]
    3190:   e5932008    ldr r2, [r3, #8]
    3194:   e51b3024    ldr r3, [fp, #-36]
    3198:   e583202c    str r2, [r3, #44]
    319c:   ea000006    b   31bc <MGC_HdrcServiceTransmitAvail+0x24c>
    31a0:   e51b3024    ldr r3, [fp, #-36]
    31a4:   e593202c    ldr r2, [r3, #44]
    31a8:   e51b3024    ldr r3, [fp, #-36]
    31ac:   e5933018    ldr r3, [r3, #24]
    31b0:   e0822003    add r2, r2, r3
    31b4:   e51b3024    ldr r3, [fp, #-36]
    31b8:   e583202c    str r2, [r3, #44]
    31bc:   e51b002c    ldr r0, [fp, #-44]
    31c0:   e51b1024    ldr r1, [fp, #-36]
    31c4:   ebfffffe    bl  0 <MGC_PipeTransmitReady>
            31c4: R_ARM_PC24    MGC_PipeTransmitReady
    31c8:   e1a03000    mov r3, r0
    31cc:   e54b301d    strb    r3, [fp, #-29]
    31d0:   ea000003    b   31e4 <MGC_HdrcServiceTransmitAvail+0x274>
    31d4:   e3a03001    mov r3, #1  ; 0x1
    31d8:   e54b301d    strb    r3, [fp, #-29]
    31dc:   e3a03087    mov r3, #135    ; 0x87
    31e0:   e50b301c    str r3, [fp, #-28]
    31e4:   e55b301d    ldrb    r3, [fp, #-29]
    31e8:   e3530000    cmp r3, #0  ; 0x0
    31ec:   0a0000a5    beq 3488 <MGC_HdrcServiceTransmitAvail+0x518>
    31f0:   e51b0010    ldr r0, [fp, #-16]
    31f4:   e3a01012    mov r1, #18 ; 0x12
    31f8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            31f8: R_ARM_PC24    MGC_FAPI_ReadReg16
    31fc:   e1a03000    mov r3, r0
    3200:   e14b32b6    strh    r3, [fp, #-38]
    3204:   e51b3024    ldr r3, [fp, #-36]
    3208:   e5933044    ldr r3, [r3, #68]
    320c:   e3530000    cmp r3, #0  ; 0x0
    3210:   0a00000c    beq 3248 <MGC_HdrcServiceTransmitAvail+0x2d8>
    3214:   e15b32b6    ldrh    r3, [fp, #-38]
    3218:   e3c33002    bic r3, r3, #2  ; 0x2
    321c:   e14b32b6    strh    r3, [fp, #-38]
    3220:   e51b302c    ldr r3, [fp, #-44]
    3224:   e5933214    ldr r3, [r3, #532]
    3228:   e5932010    ldr r2, [r3, #16]
    322c:   e51b3024    ldr r3, [fp, #-36]
    3230:   e5933044    ldr r3, [r3, #68]
    3234:   e1a00003    mov r0, r3
    3238:   e12fff32    blx r2
    323c:   e51b2024    ldr r2, [fp, #-36]
    3240:   e3a03000    mov r3, #0  ; 0x0
    3244:   e5823044    str r3, [r2, #68]
    3248:   e15b32b6    ldrh    r3, [fp, #-38]
    324c:   e3c33a01    bic r3, r3, #4096   ; 0x1000
    3250:   e14b32b6    strh    r3, [fp, #-38]
    3254:   e15b32b6    ldrh    r3, [fp, #-38]
    3258:   e1a03423    lsr r3, r3, #8
    325c:   e6ff3073    uxth    r3, r3
    3260:   e6ef3073    uxtb    r3, r3
    3264:   e51b0010    ldr r0, [fp, #-16]
    3268:   e3a01013    mov r1, #19 ; 0x13
    326c:   e1a02003    mov r2, r3
    3270:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3270: R_ARM_PC24    MGC_FAPI_WriteReg8
    3274:   e15b32b6    ldrh    r3, [fp, #-38]
    3278:   e2033002    and r3, r3, #2  ; 0x2
    327c:   e3530000    cmp r3, #0  ; 0x0
    3280:   1a00000b    bne 32b4 <MGC_HdrcServiceTransmitAvail+0x344>
    3284:   e51b301c    ldr r3, [fp, #-28]
    3288:   e6ef3073    uxtb    r3, r3
    328c:   e1a02003    mov r2, r3
    3290:   e51b3024    ldr r3, [fp, #-36]
    3294:   e593301c    ldr r3, [r3, #28]
    3298:   e58d3000    str r3, [sp]
    329c:   e51b0034    ldr r0, [fp, #-52]
    32a0:   e51b1024    ldr r1, [fp, #-36]
    32a4:   e3a03001    mov r3, #1  ; 0x1
    32a8:   ebfffffe    bl  0 <MGC_CompleteIrp>
            32a8: R_ARM_PC24    MGC_CompleteIrp
    32ac:   e1a03000    mov r3, r0
    32b0:   e54b301d    strb    r3, [fp, #-29]
    32b4:   e51b3024    ldr r3, [fp, #-36]
    32b8:   e5d33012    ldrb    r3, [r3, #18]
    32bc:   e3530000    cmp r3, #0  ; 0x0
    32c0:   0a000070    beq 3488 <MGC_HdrcServiceTransmitAvail+0x518>
    32c4:   e51b3024    ldr r3, [fp, #-36]
    32c8:   e3a02001    mov r2, #1  ; 0x1
    32cc:   e5c3200f    strb    r2, [r3, #15]
    32d0:   ea00006c    b   3488 <MGC_HdrcServiceTransmitAvail+0x518>
    32d4:   e15b32b6    ldrh    r3, [fp, #-38]
    32d8:   e2033020    and r3, r3, #32 ; 0x20
    32dc:   e3530000    cmp r3, #0  ; 0x0
    32e0:   0a000008    beq 3308 <MGC_HdrcServiceTransmitAvail+0x398>
    32e4:   e15b32b6    ldrh    r3, [fp, #-38]
    32e8:   e3c33020    bic r3, r3, #32 ; 0x20
    32ec:   e14b32b6    strh    r3, [fp, #-38]
    32f0:   e15b32b6    ldrh    r3, [fp, #-38]
    32f4:   e51b0010    ldr r0, [fp, #-16]
    32f8:   e3a01012    mov r1, #18 ; 0x12
    32fc:   e1a02003    mov r2, r3
    3300:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3300: R_ARM_PC24    MGC_FAPI_WriteReg16
    3304:   ea00005f    b   3488 <MGC_HdrcServiceTransmitAvail+0x518>
    3308:   e15b32b6    ldrh    r3, [fp, #-38]
    330c:   e2033004    and r3, r3, #4  ; 0x4
    3310:   e3530000    cmp r3, #0  ; 0x0
    3314:   0a000007    beq 3338 <MGC_HdrcServiceTransmitAvail+0x3c8>
    3318:   e15b32b6    ldrh    r3, [fp, #-38]
    331c:   e3c33004    bic r3, r3, #4  ; 0x4
    3320:   e14b32b6    strh    r3, [fp, #-38]
    3324:   e15b32b6    ldrh    r3, [fp, #-38]
    3328:   e51b0010    ldr r0, [fp, #-16]
    332c:   e3a01012    mov r1, #18 ; 0x12
    3330:   e1a02003    mov r2, r3
    3334:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3334: R_ARM_PC24    MGC_FAPI_WriteReg16
    3338:   e51b3024    ldr r3, [fp, #-36]
    333c:   e3530000    cmp r3, #0  ; 0x0
    3340:   0a000050    beq 3488 <MGC_HdrcServiceTransmitAvail+0x518>
    3344:   e51b3024    ldr r3, [fp, #-36]
    3348:   e5933044    ldr r3, [r3, #68]
    334c:   e3530000    cmp r3, #0  ; 0x0
    3350:   0a00000f    beq 3394 <MGC_HdrcServiceTransmitAvail+0x424>
    3354:   e51b302c    ldr r3, [fp, #-44]
    3358:   e5933214    ldr r3, [r3, #532]
    335c:   e5932020    ldr r2, [r3, #32]
    3360:   e51b3024    ldr r3, [fp, #-36]
    3364:   e5933044    ldr r3, [r3, #68]
    3368:   e1a00003    mov r0, r3
    336c:   e12fff32    blx r2
    3370:   e1a03000    mov r3, r0
    3374:   e3530001    cmp r3, #1  ; 0x1
    3378:   1a00000c    bne 33b0 <MGC_HdrcServiceTransmitAvail+0x440>
    337c:   e51b3024    ldr r3, [fp, #-36]
    3380:   e5933044    ldr r3, [r3, #68]
    3384:   e5932008    ldr r2, [r3, #8]
    3388:   e51b3024    ldr r3, [fp, #-36]
    338c:   e583202c    str r2, [r3, #44]
    3390:   ea000006    b   33b0 <MGC_HdrcServiceTransmitAvail+0x440>
    3394:   e51b3024    ldr r3, [fp, #-36]
    3398:   e593202c    ldr r2, [r3, #44]
    339c:   e51b3024    ldr r3, [fp, #-36]
    33a0:   e5933018    ldr r3, [r3, #24]
    33a4:   e0822003    add r2, r2, r3
    33a8:   e51b3024    ldr r3, [fp, #-36]
    33ac:   e583202c    str r2, [r3, #44]
    33b0:   e51b3024    ldr r3, [fp, #-36]
    33b4:   e593301c    ldr r3, [r3, #28]
    33b8:   e3530000    cmp r3, #0  ; 0x0
    33bc:   0a000031    beq 3488 <MGC_HdrcServiceTransmitAvail+0x518>
    33c0:   e51b002c    ldr r0, [fp, #-44]
    33c4:   e51b1024    ldr r1, [fp, #-36]
    33c8:   ebfffffe    bl  0 <MGC_PipeTransmitReady>
            33c8: R_ARM_PC24    MGC_PipeTransmitReady
    33cc:   e1a03000    mov r3, r0
    33d0:   e54b301d    strb    r3, [fp, #-29]
    33d4:   e55b301d    ldrb    r3, [fp, #-29]
    33d8:   e3530000    cmp r3, #0  ; 0x0
    33dc:   0a000029    beq 3488 <MGC_HdrcServiceTransmitAvail+0x518>
    33e0:   e51b3024    ldr r3, [fp, #-36]
    33e4:   e5933044    ldr r3, [r3, #68]
    33e8:   e3530000    cmp r3, #0  ; 0x0
    33ec:   0a000009    beq 3418 <MGC_HdrcServiceTransmitAvail+0x4a8>
    33f0:   e51b302c    ldr r3, [fp, #-44]
    33f4:   e5933214    ldr r3, [r3, #532]
    33f8:   e5932010    ldr r2, [r3, #16]
    33fc:   e51b3024    ldr r3, [fp, #-36]
    3400:   e5933044    ldr r3, [r3, #68]
    3404:   e1a00003    mov r0, r3
    3408:   e12fff32    blx r2
    340c:   e51b2024    ldr r2, [fp, #-36]
    3410:   e3a03000    mov r3, #0  ; 0x0
    3414:   e5823044    str r3, [r2, #68]
    3418:   e51b0010    ldr r0, [fp, #-16]
    341c:   e3a01012    mov r1, #18 ; 0x12
    3420:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            3420: R_ARM_PC24    MGC_FAPI_ReadReg16
    3424:   e1a03000    mov r3, r0
    3428:   e14b32b6    strh    r3, [fp, #-38]
    342c:   e15b32b6    ldrh    r3, [fp, #-38]
    3430:   e3c33a01    bic r3, r3, #4096   ; 0x1000
    3434:   e14b32b6    strh    r3, [fp, #-38]
    3438:   e15b32b6    ldrh    r3, [fp, #-38]
    343c:   e1a03423    lsr r3, r3, #8
    3440:   e6ff3073    uxth    r3, r3
    3444:   e6ef3073    uxtb    r3, r3
    3448:   e51b0010    ldr r0, [fp, #-16]
    344c:   e3a01013    mov r1, #19 ; 0x13
    3450:   e1a02003    mov r2, r3
    3454:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3454: R_ARM_PC24    MGC_FAPI_WriteReg8
    3458:   e51b301c    ldr r3, [fp, #-28]
    345c:   e6ef3073    uxtb    r3, r3
    3460:   e1a02003    mov r2, r3
    3464:   e51b3024    ldr r3, [fp, #-36]
    3468:   e593301c    ldr r3, [r3, #28]
    346c:   e58d3000    str r3, [sp]
    3470:   e51b0034    ldr r0, [fp, #-52]
    3474:   e51b1024    ldr r1, [fp, #-36]
    3478:   e3a03001    mov r3, #1  ; 0x1
    347c:   ebfffffe    bl  0 <MGC_CompleteIrp>
            347c: R_ARM_PC24    MGC_CompleteIrp
    3480:   e1a03000    mov r3, r0
    3484:   e54b301d    strb    r3, [fp, #-29]
    3488:   e55b301d    ldrb    r3, [fp, #-29]
    348c:   e50b3038    str r3, [fp, #-56]
    3490:   e51b3038    ldr r3, [fp, #-56]
    3494:   e1a00003    mov r0, r3
    3498:   e24bd00c    sub sp, fp, #12 ; 0xc
    349c:   e89da800    ldm sp, {fp, sp, pc}

000034a0 <MGC_HdrcServiceReceiveReady>:
    34a0:   e1a0c00d    mov ip, sp
    34a4:   e92dd800    push    {fp, ip, lr, pc}
    34a8:   e24cb004    sub fp, ip, #4  ; 0x4
    34ac:   e24dd034    sub sp, sp, #52 ; 0x34
    34b0:   e50b0028    str r0, [fp, #-40]
    34b4:   e1a03001    mov r3, r1
    34b8:   e50b2030    str r2, [fp, #-48]
    34bc:   e14b32bc    strh    r3, [fp, #-44]
    34c0:   e15b32bc    ldrh    r3, [fp, #-44]
    34c4:   e54b301a    strb    r3, [fp, #-26]
    34c8:   e3a03000    mov r3, #0  ; 0x0
    34cc:   e54b3019    strb    r3, [fp, #-25]
    34d0:   e3a03000    mov r3, #0  ; 0x0
    34d4:   e50b3018    str r3, [fp, #-24]
    34d8:   e51b3028    ldr r3, [fp, #-40]
    34dc:   e5933000    ldr r3, [r3]
    34e0:   e50b3014    str r3, [fp, #-20]
    34e4:   e51b3014    ldr r3, [fp, #-20]
    34e8:   e5933004    ldr r3, [r3, #4]
    34ec:   e50b3010    str r3, [fp, #-16]
    34f0:   e51b2030    ldr r2, [fp, #-48]
    34f4:   e3a03000    mov r3, #0  ; 0x0
    34f8:   e5c23006    strb    r3, [r2, #6]
    34fc:   e51b2030    ldr r2, [fp, #-48]
    3500:   e3a03000    mov r3, #0  ; 0x0
    3504:   e5c23004    strb    r3, [r2, #4]
    3508:   e55b301a    ldrb    r3, [fp, #-26]
    350c:   e51b0010    ldr r0, [fp, #-16]
    3510:   e3a0100e    mov r1, #14 ; 0xe
    3514:   e1a02003    mov r2, r3
    3518:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3518: R_ARM_PC24    MGC_FAPI_WriteReg8
    351c:   e51b0010    ldr r0, [fp, #-16]
    3520:   e3a01016    mov r1, #22 ; 0x16
    3524:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            3524: R_ARM_PC24    MGC_FAPI_ReadReg16
    3528:   e1a03000    mov r3, r0
    352c:   e14b32b2    strh    r3, [fp, #-34]
    3530:   e51b3028    ldr r3, [fp, #-40]
    3534:   e283300c    add r3, r3, #12 ; 0xc
    3538:   e15b22bc    ldrh    r2, [fp, #-44]
    353c:   e1a00003    mov r0, r3
    3540:   e1a01002    mov r1, r2
    3544:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            3544: R_ARM_PC24    MUSB_ArrayFetch
    3548:   e1a03000    mov r3, r0
    354c:   e50b3020    str r3, [fp, #-32]
    3550:   e51b3028    ldr r3, [fp, #-40]
    3554:   e5d3303e    ldrb    r3, [r3, #62]
    3558:   e3530000    cmp r3, #0  ; 0x0
    355c:   0a0000a2    beq 37ec <MGC_HdrcServiceReceiveReady+0x34c>
    3560:   e15b32b2    ldrh    r3, [fp, #-34]
    3564:   e2033040    and r3, r3, #64 ; 0x40
    3568:   e3530000    cmp r3, #0  ; 0x0
    356c:   0a000009    beq 3598 <MGC_HdrcServiceReceiveReady+0xf8>
    3570:   e3a03080    mov r3, #128    ; 0x80
    3574:   e50b3018    str r3, [fp, #-24]
    3578:   e15b32b2    ldrh    r3, [fp, #-34]
    357c:   e3c33020    bic r3, r3, #32 ; 0x20
    3580:   e14b32b2    strh    r3, [fp, #-34]
    3584:   e15b32b2    ldrh    r3, [fp, #-34]
    3588:   e51b0010    ldr r0, [fp, #-16]
    358c:   e3a01016    mov r1, #22 ; 0x16
    3590:   e1a02003    mov r2, r3
    3594:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3594: R_ARM_PC24    MGC_FAPI_WriteReg16
    3598:   e15b32b2    ldrh    r3, [fp, #-34]
    359c:   e203300c    and r3, r3, #12 ; 0xc
    35a0:   e3530000    cmp r3, #0  ; 0x0
    35a4:   0a000009    beq 35d0 <MGC_HdrcServiceReceiveReady+0x130>
    35a8:   e3a03089    mov r3, #137    ; 0x89
    35ac:   e50b3018    str r3, [fp, #-24]
    35b0:   e15b32b2    ldrh    r3, [fp, #-34]
    35b4:   e3c33020    bic r3, r3, #32 ; 0x20
    35b8:   e14b32b2    strh    r3, [fp, #-34]
    35bc:   e15b32b2    ldrh    r3, [fp, #-34]
    35c0:   e51b0010    ldr r0, [fp, #-16]
    35c4:   e3a01016    mov r1, #22 ; 0x16
    35c8:   e1a02003    mov r2, r3
    35cc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            35cc: R_ARM_PC24    MGC_FAPI_WriteReg16
    35d0:   e51b3018    ldr r3, [fp, #-24]
    35d4:   e3530000    cmp r3, #0  ; 0x0
    35d8:   1a00000d    bne 3614 <MGC_HdrcServiceReceiveReady+0x174>
    35dc:   e15b32b2    ldrh    r3, [fp, #-34]
    35e0:   e2033001    and r3, r3, #1  ; 0x1
    35e4:   e3530000    cmp r3, #0  ; 0x0
    35e8:   1a000009    bne 3614 <MGC_HdrcServiceReceiveReady+0x174>
    35ec:   e3a03087    mov r3, #135    ; 0x87
    35f0:   e50b3018    str r3, [fp, #-24]
    35f4:   e15b32b2    ldrh    r3, [fp, #-34]
    35f8:   e3c33020    bic r3, r3, #32 ; 0x20
    35fc:   e14b32b2    strh    r3, [fp, #-34]
    3600:   e15b32b2    ldrh    r3, [fp, #-34]
    3604:   e51b0010    ldr r0, [fp, #-16]
    3608:   e3a01016    mov r1, #22 ; 0x16
    360c:   e1a02003    mov r2, r3
    3610:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3610: R_ARM_PC24    MGC_FAPI_WriteReg16
    3614:   e51b3018    ldr r3, [fp, #-24]
    3618:   e3530000    cmp r3, #0  ; 0x0
    361c:   0a000016    beq 367c <MGC_HdrcServiceReceiveReady+0x1dc>
    3620:   e15b32b2    ldrh    r3, [fp, #-34]
    3624:   e3c3304d    bic r3, r3, #77 ; 0x4d
    3628:   e14b32b2    strh    r3, [fp, #-34]
    362c:   e15b32b2    ldrh    r3, [fp, #-34]
    3630:   e51b0010    ldr r0, [fp, #-16]
    3634:   e3a01016    mov r1, #22 ; 0x16
    3638:   e1a02003    mov r2, r3
    363c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            363c: R_ARM_PC24    MGC_FAPI_WriteReg16
    3640:   e51b3018    ldr r3, [fp, #-24]
    3644:   e6ef3073    uxtb    r3, r3
    3648:   e1a02003    mov r2, r3
    364c:   e51b3020    ldr r3, [fp, #-32]
    3650:   e5933030    ldr r3, [r3, #48]
    3654:   e58d3000    str r3, [sp]
    3658:   e51b0030    ldr r0, [fp, #-48]
    365c:   e51b1020    ldr r1, [fp, #-32]
    3660:   e3a03000    mov r3, #0  ; 0x0
    3664:   ebfffffe    bl  0 <MGC_CompleteIrp>
            3664: R_ARM_PC24    MGC_CompleteIrp
    3668:   e1a03000    mov r3, r0
    366c:   e54b3019    strb    r3, [fp, #-25]
    3670:   e3a03001    mov r3, #1  ; 0x1
    3674:   e54b3019    strb    r3, [fp, #-25]
    3678:   ea00011f    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    367c:   e51b3020    ldr r3, [fp, #-32]
    3680:   e3530000    cmp r3, #0  ; 0x0
    3684:   0a000053    beq 37d8 <MGC_HdrcServiceReceiveReady+0x338>
    3688:   e51b3020    ldr r3, [fp, #-32]
    368c:   e5933030    ldr r3, [r3, #48]
    3690:   e3530000    cmp r3, #0  ; 0x0
    3694:   0a00004f    beq 37d8 <MGC_HdrcServiceReceiveReady+0x338>
    3698:   e51b3020    ldr r3, [fp, #-32]
    369c:   e5933048    ldr r3, [r3, #72]
    36a0:   e3530000    cmp r3, #0  ; 0x0
    36a4:   0a00000e    beq 36e4 <MGC_HdrcServiceReceiveReady+0x244>
    36a8:   e51b3028    ldr r3, [fp, #-40]
    36ac:   e5933214    ldr r3, [r3, #532]
    36b0:   e5932020    ldr r2, [r3, #32]
    36b4:   e51b3020    ldr r3, [fp, #-32]
    36b8:   e5933048    ldr r3, [r3, #72]
    36bc:   e1a00003    mov r0, r3
    36c0:   e12fff32    blx r2
    36c4:   e1a03000    mov r3, r0
    36c8:   e3530001    cmp r3, #1  ; 0x1
    36cc:   1a000004    bne 36e4 <MGC_HdrcServiceReceiveReady+0x244>
    36d0:   e51b3020    ldr r3, [fp, #-32]
    36d4:   e5933048    ldr r3, [r3, #72]
    36d8:   e5932008    ldr r2, [r3, #8]
    36dc:   e51b3020    ldr r3, [fp, #-32]
    36e0:   e5832040    str r2, [r3, #64]
    36e4:   e51b0010    ldr r0, [fp, #-16]
    36e8:   e3a01018    mov r1, #24 ; 0x18
    36ec:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            36ec: R_ARM_PC24    MGC_FAPI_ReadReg16
    36f0:   e1a03000    mov r3, r0
    36f4:   e14b31bc    strh    r3, [fp, #-28]
    36f8:   e15bc1bc    ldrh    ip, [fp, #-28]
    36fc:   e51b3020    ldr r3, [fp, #-32]
    3700:   e5932030    ldr r2, [r3, #48]
    3704:   e3a03001    mov r3, #1  ; 0x1
    3708:   e58d3000    str r3, [sp]
    370c:   e58d2004    str r2, [sp, #4]
    3710:   e51b0028    ldr r0, [fp, #-40]
    3714:   e51b1018    ldr r1, [fp, #-24]
    3718:   e51b2020    ldr r2, [fp, #-32]
    371c:   e1a0300c    mov r3, ip
    3720:   ebfffffe    bl  0 <MGC_PipePacketReceived>
            3720: R_ARM_PC24    MGC_PipePacketReceived
    3724:   e1a03000    mov r3, r0
    3728:   e54b3019    strb    r3, [fp, #-25]
    372c:   e55b3019    ldrb    r3, [fp, #-25]
    3730:   e3530000    cmp r3, #0  ; 0x0
    3734:   0a0000f0    beq 3afc <MGC_HdrcServiceReceiveReady+0x65c>
    3738:   e51b3020    ldr r3, [fp, #-32]
    373c:   e5933048    ldr r3, [r3, #72]
    3740:   e3530000    cmp r3, #0  ; 0x0
    3744:   0a000009    beq 3770 <MGC_HdrcServiceReceiveReady+0x2d0>
    3748:   e51b3028    ldr r3, [fp, #-40]
    374c:   e5933214    ldr r3, [r3, #532]
    3750:   e5932010    ldr r2, [r3, #16]
    3754:   e51b3020    ldr r3, [fp, #-32]
    3758:   e5933048    ldr r3, [r3, #72]
    375c:   e1a00003    mov r0, r3
    3760:   e12fff32    blx r2
    3764:   e51b2020    ldr r2, [fp, #-32]
    3768:   e3a03000    mov r3, #0  ; 0x0
    376c:   e5823048    str r3, [r2, #72]
    3770:   e51b0010    ldr r0, [fp, #-16]
    3774:   e3a01016    mov r1, #22 ; 0x16
    3778:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            3778: R_ARM_PC24    MGC_FAPI_ReadReg16
    377c:   e1a03000    mov r3, r0
    3780:   e14b32b2    strh    r3, [fp, #-34]
    3784:   e15b32b2    ldrh    r3, [fp, #-34]
    3788:   e3c33001    bic r3, r3, #1  ; 0x1
    378c:   e14b32b2    strh    r3, [fp, #-34]
    3790:   e15b32b2    ldrh    r3, [fp, #-34]
    3794:   e51b0010    ldr r0, [fp, #-16]
    3798:   e3a01016    mov r1, #22 ; 0x16
    379c:   e1a02003    mov r2, r3
    37a0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            37a0: R_ARM_PC24    MGC_FAPI_WriteReg16
    37a4:   e51b3018    ldr r3, [fp, #-24]
    37a8:   e6ef3073    uxtb    r3, r3
    37ac:   e1a02003    mov r2, r3
    37b0:   e51b3020    ldr r3, [fp, #-32]
    37b4:   e5933030    ldr r3, [r3, #48]
    37b8:   e58d3000    str r3, [sp]
    37bc:   e51b0030    ldr r0, [fp, #-48]
    37c0:   e51b1020    ldr r1, [fp, #-32]
    37c4:   e3a03000    mov r3, #0  ; 0x0
    37c8:   ebfffffe    bl  0 <MGC_CompleteIrp>
            37c8: R_ARM_PC24    MGC_CompleteIrp
    37cc:   e1a03000    mov r3, r0
    37d0:   e54b3019    strb    r3, [fp, #-25]
    37d4:   ea0000c8    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    37d8:   e3a03001    mov r3, #1  ; 0x1
    37dc:   e54b3019    strb    r3, [fp, #-25]
    37e0:   e3a03087    mov r3, #135    ; 0x87
    37e4:   e50b3018    str r3, [fp, #-24]
    37e8:   ea0000c3    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    37ec:   e15b32b2    ldrh    r3, [fp, #-34]
    37f0:   e2033040    and r3, r3, #64 ; 0x40
    37f4:   e3530000    cmp r3, #0  ; 0x0
    37f8:   0a000008    beq 3820 <MGC_HdrcServiceReceiveReady+0x380>
    37fc:   e15b32b2    ldrh    r3, [fp, #-34]
    3800:   e3c33040    bic r3, r3, #64 ; 0x40
    3804:   e14b32b2    strh    r3, [fp, #-34]
    3808:   e15b32b2    ldrh    r3, [fp, #-34]
    380c:   e51b0010    ldr r0, [fp, #-16]
    3810:   e3a01016    mov r1, #22 ; 0x16
    3814:   e1a02003    mov r2, r3
    3818:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3818: R_ARM_PC24    MGC_FAPI_WriteReg16
    381c:   ea0000b6    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    3820:   e15b32b2    ldrh    r3, [fp, #-34]
    3824:   e2033004    and r3, r3, #4  ; 0x4
    3828:   e3530000    cmp r3, #0  ; 0x0
    382c:   0a000008    beq 3854 <MGC_HdrcServiceReceiveReady+0x3b4>
    3830:   e15b32b2    ldrh    r3, [fp, #-34]
    3834:   e3c33004    bic r3, r3, #4  ; 0x4
    3838:   e14b32b2    strh    r3, [fp, #-34]
    383c:   e15b32b2    ldrh    r3, [fp, #-34]
    3840:   e51b0010    ldr r0, [fp, #-16]
    3844:   e3a01016    mov r1, #22 ; 0x16
    3848:   e1a02003    mov r2, r3
    384c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            384c: R_ARM_PC24    MGC_FAPI_WriteReg16
    3850:   ea0000a9    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    3854:   e51b3020    ldr r3, [fp, #-32]
    3858:   e3530000    cmp r3, #0  ; 0x0
    385c:   0a000003    beq 3870 <MGC_HdrcServiceReceiveReady+0x3d0>
    3860:   e51b3020    ldr r3, [fp, #-32]
    3864:   e5d3300f    ldrb    r3, [r3, #15]
    3868:   e3530000    cmp r3, #0  ; 0x0
    386c:   0a00000e    beq 38ac <MGC_HdrcServiceReceiveReady+0x40c>
    3870:   e51b3020    ldr r3, [fp, #-32]
    3874:   e3530000    cmp r3, #0  ; 0x0
    3878:   0a000002    beq 3888 <MGC_HdrcServiceReceiveReady+0x3e8>
    387c:   e3a03000    mov r3, #0  ; 0x0
    3880:   e50b3038    str r3, [fp, #-56]
    3884:   ea000001    b   3890 <MGC_HdrcServiceReceiveReady+0x3f0>
    3888:   e3a03087    mov r3, #135    ; 0x87
    388c:   e50b3038    str r3, [fp, #-56]
    3890:   e51b3038    ldr r3, [fp, #-56]
    3894:   e50b3018    str r3, [fp, #-24]
    3898:   e51b0010    ldr r0, [fp, #-16]
    389c:   e3a01016    mov r1, #22 ; 0x16
    38a0:   e3a02020    mov r2, #32 ; 0x20
    38a4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            38a4: R_ARM_PC24    MGC_FAPI_WriteReg16
    38a8:   ea000093    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    38ac:   e51b3020    ldr r3, [fp, #-32]
    38b0:   e3530000    cmp r3, #0  ; 0x0
    38b4:   0a00008a    beq 3ae4 <MGC_HdrcServiceReceiveReady+0x644>
    38b8:   e51b3020    ldr r3, [fp, #-32]
    38bc:   e5933030    ldr r3, [r3, #48]
    38c0:   e3530000    cmp r3, #0  ; 0x0
    38c4:   0a000086    beq 3ae4 <MGC_HdrcServiceReceiveReady+0x644>
    38c8:   ea000075    b   3aa4 <MGC_HdrcServiceReceiveReady+0x604>
    38cc:   e51b3020    ldr r3, [fp, #-32]
    38d0:   e5d3205f    ldrb    r2, [r3, #95]
    38d4:   e51b3020    ldr r3, [fp, #-32]
    38d8:   e5d3305e    ldrb    r3, [r3, #94]
    38dc:   e1520003    cmp r2, r3
    38e0:   0a000005    beq 38fc <MGC_HdrcServiceReceiveReady+0x45c>
    38e4:   e51b3020    ldr r3, [fp, #-32]
    38e8:   e5d3305f    ldrb    r3, [r3, #95]
    38ec:   e2833001    add r3, r3, #1  ; 0x1
    38f0:   e6ef3073    uxtb    r3, r3
    38f4:   e51b2020    ldr r2, [fp, #-32]
    38f8:   e5c2305f    strb    r3, [r2, #95]
    38fc:   e51b3020    ldr r3, [fp, #-32]
    3900:   e5933048    ldr r3, [r3, #72]
    3904:   e3530000    cmp r3, #0  ; 0x0
    3908:   0a000011    beq 3954 <MGC_HdrcServiceReceiveReady+0x4b4>
    390c:   e51b3028    ldr r3, [fp, #-40]
    3910:   e5933214    ldr r3, [r3, #532]
    3914:   e5932020    ldr r2, [r3, #32]
    3918:   e51b3020    ldr r3, [fp, #-32]
    391c:   e5933048    ldr r3, [r3, #72]
    3920:   e1a00003    mov r0, r3
    3924:   e12fff32    blx r2
    3928:   e1a03000    mov r3, r0
    392c:   e3530001    cmp r3, #1  ; 0x1
    3930:   1a000007    bne 3954 <MGC_HdrcServiceReceiveReady+0x4b4>
    3934:   e51b3020    ldr r3, [fp, #-32]
    3938:   e5933048    ldr r3, [r3, #72]
    393c:   e5932008    ldr r2, [r3, #8]
    3940:   e51b3020    ldr r3, [fp, #-32]
    3944:   e5832040    str r2, [r3, #64]
    3948:   e51b2020    ldr r2, [fp, #-32]
    394c:   e3a03000    mov r3, #0  ; 0x0
    3950:   e5c23051    strb    r3, [r2, #81]
    3954:   e51b3020    ldr r3, [fp, #-32]
    3958:   e5d33051    ldrb    r3, [r3, #81]
    395c:   e3530000    cmp r3, #0  ; 0x0
    3960:   0a000002    beq 3970 <MGC_HdrcServiceReceiveReady+0x4d0>
    3964:   e3a03000    mov r3, #0  ; 0x0
    3968:   e50b3034    str r3, [fp, #-52]
    396c:   ea000064    b   3b04 <MGC_HdrcServiceReceiveReady+0x664>
    3970:   e51b0010    ldr r0, [fp, #-16]
    3974:   e3a01018    mov r1, #24 ; 0x18
    3978:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            3978: R_ARM_PC24    MGC_FAPI_ReadReg16
    397c:   e1a03000    mov r3, r0
    3980:   e14b31bc    strh    r3, [fp, #-28]
    3984:   e15bc1bc    ldrh    ip, [fp, #-28]
    3988:   e51b3020    ldr r3, [fp, #-32]
    398c:   e5932030    ldr r2, [r3, #48]
    3990:   e3a03000    mov r3, #0  ; 0x0
    3994:   e58d3000    str r3, [sp]
    3998:   e58d2004    str r2, [sp, #4]
    399c:   e51b0028    ldr r0, [fp, #-40]
    39a0:   e51b1018    ldr r1, [fp, #-24]
    39a4:   e51b2020    ldr r2, [fp, #-32]
    39a8:   e1a0300c    mov r3, ip
    39ac:   ebfffffe    bl  0 <MGC_PipePacketReceived>
            39ac: R_ARM_PC24    MGC_PipePacketReceived
    39b0:   e1a03000    mov r3, r0
    39b4:   e54b3019    strb    r3, [fp, #-25]
    39b8:   e51b0010    ldr r0, [fp, #-16]
    39bc:   e3a01016    mov r1, #22 ; 0x16
    39c0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            39c0: R_ARM_PC24    MGC_FAPI_ReadReg16
    39c4:   e1a03000    mov r3, r0
    39c8:   e14b32b2    strh    r3, [fp, #-34]
    39cc:   e51b3020    ldr r3, [fp, #-32]
    39d0:   e1d321b6    ldrh    r2, [r3, #22]
    39d4:   e15b31bc    ldrh    r3, [fp, #-28]
    39d8:   e1530002    cmp r3, r2
    39dc:   3a000003    bcc 39f0 <MGC_HdrcServiceReceiveReady+0x550>
    39e0:   e15b32b2    ldrh    r3, [fp, #-34]
    39e4:   e6bf3073    sxth    r3, r3
    39e8:   e3530000    cmp r3, #0  ; 0x0
    39ec:   ba000007    blt 3a10 <MGC_HdrcServiceReceiveReady+0x570>
    39f0:   e15b32b2    ldrh    r3, [fp, #-34]
    39f4:   e3c33001    bic r3, r3, #1  ; 0x1
    39f8:   e14b32b2    strh    r3, [fp, #-34]
    39fc:   e15b32b2    ldrh    r3, [fp, #-34]
    3a00:   e51b0010    ldr r0, [fp, #-16]
    3a04:   e3a01016    mov r1, #22 ; 0x16
    3a08:   e1a02003    mov r2, r3
    3a0c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3a0c: R_ARM_PC24    MGC_FAPI_WriteReg16
    3a10:   e55b3019    ldrb    r3, [fp, #-25]
    3a14:   e3530000    cmp r3, #0  ; 0x0
    3a18:   0a000021    beq 3aa4 <MGC_HdrcServiceReceiveReady+0x604>
    3a1c:   e51b3020    ldr r3, [fp, #-32]
    3a20:   e5933048    ldr r3, [r3, #72]
    3a24:   e3530000    cmp r3, #0  ; 0x0
    3a28:   0a000009    beq 3a54 <MGC_HdrcServiceReceiveReady+0x5b4>
    3a2c:   e51b3028    ldr r3, [fp, #-40]
    3a30:   e5933214    ldr r3, [r3, #532]
    3a34:   e5932010    ldr r2, [r3, #16]
    3a38:   e51b3020    ldr r3, [fp, #-32]
    3a3c:   e5933048    ldr r3, [r3, #72]
    3a40:   e1a00003    mov r0, r3
    3a44:   e12fff32    blx r2
    3a48:   e51b2020    ldr r2, [fp, #-32]
    3a4c:   e3a03000    mov r3, #0  ; 0x0
    3a50:   e5823048    str r3, [r2, #72]
    3a54:   e51b3018    ldr r3, [fp, #-24]
    3a58:   e6ef3073    uxtb    r3, r3
    3a5c:   e1a02003    mov r2, r3
    3a60:   e51b3020    ldr r3, [fp, #-32]
    3a64:   e5933030    ldr r3, [r3, #48]
    3a68:   e58d3000    str r3, [sp]
    3a6c:   e51b0030    ldr r0, [fp, #-48]
    3a70:   e51b1020    ldr r1, [fp, #-32]
    3a74:   e3a03000    mov r3, #0  ; 0x0
    3a78:   ebfffffe    bl  0 <MGC_CompleteIrp>
            3a78: R_ARM_PC24    MGC_CompleteIrp
    3a7c:   e1a03000    mov r3, r0
    3a80:   e54b3019    strb    r3, [fp, #-25]
    3a84:   e15b32b2    ldrh    r3, [fp, #-34]
    3a88:   e3c33b0a    bic r3, r3, #10240  ; 0x2800
    3a8c:   e1a03443    asr r3, r3, #8
    3a90:   e6ef3073    uxtb    r3, r3
    3a94:   e51b0010    ldr r0, [fp, #-16]
    3a98:   e3a01017    mov r1, #23 ; 0x17
    3a9c:   e1a02003    mov r2, r3
    3aa0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3aa0: R_ARM_PC24    MGC_FAPI_WriteReg8
    3aa4:   e51b3020    ldr r3, [fp, #-32]
    3aa8:   e5933030    ldr r3, [r3, #48]
    3aac:   e3530000    cmp r3, #0  ; 0x0
    3ab0:   0a000011    beq 3afc <MGC_HdrcServiceReceiveReady+0x65c>
    3ab4:   e51b3020    ldr r3, [fp, #-32]
    3ab8:   e5d3205f    ldrb    r2, [r3, #95]
    3abc:   e51b3020    ldr r3, [fp, #-32]
    3ac0:   e5d3305e    ldrb    r3, [r3, #94]
    3ac4:   e1520003    cmp r2, r3
    3ac8:   1affff7f    bne 38cc <MGC_HdrcServiceReceiveReady+0x42c>
    3acc:   e15b32b2    ldrh    r3, [fp, #-34]
    3ad0:   e2033001    and r3, r3, #1  ; 0x1
    3ad4:   e6ef3073    uxtb    r3, r3
    3ad8:   e3530000    cmp r3, #0  ; 0x0
    3adc:   1affff7a    bne 38cc <MGC_HdrcServiceReceiveReady+0x42c>
    3ae0:   ea000005    b   3afc <MGC_HdrcServiceReceiveReady+0x65c>
    3ae4:   e51b3020    ldr r3, [fp, #-32]
    3ae8:   e5d3305e    ldrb    r3, [r3, #94]
    3aec:   e2833001    add r3, r3, #1  ; 0x1
    3af0:   e6ef3073    uxtb    r3, r3
    3af4:   e51b2020    ldr r2, [fp, #-32]
    3af8:   e5c2305e    strb    r3, [r2, #94]
    3afc:   e55b3019    ldrb    r3, [fp, #-25]
    3b00:   e50b3034    str r3, [fp, #-52]
    3b04:   e51b3034    ldr r3, [fp, #-52]
    3b08:   e1a00003    mov r0, r3
    3b0c:   e24bd00c    sub sp, fp, #12 ; 0xc
    3b10:   e89da800    ldm sp, {fp, sp, pc}

00003b14 <MGC_HdrcResetOff>:
    3b14:   e1a0c00d    mov ip, sp
    3b18:   e92dd800    push    {fp, ip, lr, pc}
    3b1c:   e24cb004    sub fp, ip, #4  ; 0x4
    3b20:   e24dd014    sub sp, sp, #20 ; 0x14
    3b24:   e50b001c    str r0, [fp, #-28]
    3b28:   e1a03001    mov r3, r1
    3b2c:   e14b32b0    strh    r3, [fp, #-32]
    3b30:   e51b301c    ldr r3, [fp, #-28]
    3b34:   e50b3014    str r3, [fp, #-20]
    3b38:   e51b3014    ldr r3, [fp, #-20]
    3b3c:   e5933004    ldr r3, [r3, #4]
    3b40:   e50b3010    str r3, [fp, #-16]
    3b44:   e51b0010    ldr r0, [fp, #-16]
    3b48:   e3a01001    mov r1, #1  ; 0x1
    3b4c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            3b4c: R_ARM_PC24    MGC_FAPI_ReadReg8
    3b50:   e1a03000    mov r3, r0
    3b54:   e54b3015    strb    r3, [fp, #-21]
    3b58:   e55b3015    ldrb    r3, [fp, #-21]
    3b5c:   e20330f7    and r3, r3, #247    ; 0xf7
    3b60:   e51b0010    ldr r0, [fp, #-16]
    3b64:   e3a01001    mov r1, #1  ; 0x1
    3b68:   e1a02003    mov r2, r3
    3b6c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3b6c: R_ARM_PC24    MGC_FAPI_WriteReg8
    3b70:   e24bd00c    sub sp, fp, #12 ; 0xc
    3b74:   e89da800    ldm sp, {fp, sp, pc}

00003b78 <MGC_HdrcResumeOff>:
    3b78:   e1a0c00d    mov ip, sp
    3b7c:   e92dd800    push    {fp, ip, lr, pc}
    3b80:   e24cb004    sub fp, ip, #4  ; 0x4
    3b84:   e24dd014    sub sp, sp, #20 ; 0x14
    3b88:   e50b001c    str r0, [fp, #-28]
    3b8c:   e1a03001    mov r3, r1
    3b90:   e14b32b0    strh    r3, [fp, #-32]
    3b94:   e51b301c    ldr r3, [fp, #-28]
    3b98:   e50b3014    str r3, [fp, #-20]
    3b9c:   e51b3014    ldr r3, [fp, #-20]
    3ba0:   e5933004    ldr r3, [r3, #4]
    3ba4:   e50b3010    str r3, [fp, #-16]
    3ba8:   e51b0010    ldr r0, [fp, #-16]
    3bac:   e3a01001    mov r1, #1  ; 0x1
    3bb0:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            3bb0: R_ARM_PC24    MGC_FAPI_ReadReg8
    3bb4:   e1a03000    mov r3, r0
    3bb8:   e54b3015    strb    r3, [fp, #-21]
    3bbc:   e55b3015    ldrb    r3, [fp, #-21]
    3bc0:   e20330fb    and r3, r3, #251    ; 0xfb
    3bc4:   e51b0010    ldr r0, [fp, #-16]
    3bc8:   e3a01001    mov r1, #1  ; 0x1
    3bcc:   e1a02003    mov r2, r3
    3bd0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3bd0: R_ARM_PC24    MGC_FAPI_WriteReg8
    3bd4:   e24bd00c    sub sp, fp, #12 ; 0xc
    3bd8:   e89da800    ldm sp, {fp, sp, pc}

00003bdc <MGC_HdrcSetPortTestMode>:
    3bdc:   e1a0c00d    mov ip, sp
    3be0:   e92dd800    push    {fp, ip, lr, pc}
    3be4:   e24cb004    sub fp, ip, #4  ; 0x4
    3be8:   e24dd01c    sub sp, sp, #28 ; 0x1c
    3bec:   e50b0020    str r0, [fp, #-32]
    3bf0:   e50b1024    str r1, [fp, #-36]
    3bf4:   e51b3020    ldr r3, [fp, #-32]
    3bf8:   e5933000    ldr r3, [r3]
    3bfc:   e50b3018    str r3, [fp, #-24]
    3c00:   e51b3018    ldr r3, [fp, #-24]
    3c04:   e5933004    ldr r3, [r3, #4]
    3c08:   e50b3014    str r3, [fp, #-20]
    3c0c:   e51b0014    ldr r0, [fp, #-20]
    3c10:   e3a01002    mov r1, #2  ; 0x2
    3c14:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            3c14: R_ARM_PC24    MGC_FAPI_ReadReg16
    3c18:   e1a03000    mov r3, r0
    3c1c:   e14b30be    strh    r3, [fp, #-14]
    3c20:   e51b3024    ldr r3, [fp, #-36]
    3c24:   e353000a    cmp r3, #10 ; 0xa
    3c28:   979ff103    ldrls   pc, [pc, r3, lsl #2]
    3c2c:   ea0000b0    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3c30:   00003c5c    .word   0x00003c5c
            3c30: R_ARM_ABS32   .text
    3c34:   00003c8c    .word   0x00003c8c
            3c34: R_ARM_ABS32   .text
    3c38:   00003ca0    .word   0x00003ca0
            3c38: R_ARM_ABS32   .text
    3c3c:   00003cb4    .word   0x00003cb4
            3c3c: R_ARM_ABS32   .text
    3c40:   00003cc8    .word   0x00003cc8
            3c40: R_ARM_ABS32   .text
    3c44:   00003d10    .word   0x00003d10
            3c44: R_ARM_ABS32   .text
    3c48:   00003d78    .word   0x00003d78
            3c48: R_ARM_ABS32   .text
    3c4c:   00003dac    .word   0x00003dac
            3c4c: R_ARM_ABS32   .text
    3c50:   00003e18    .word   0x00003e18
            3c50: R_ARM_ABS32   .text
    3c54:   00003e78    .word   0x00003e78
            3c54: R_ARM_ABS32   .text
    3c58:   00003eb8    .word   0x00003eb8
            3c58: R_ARM_ABS32   .text
    3c5c:   e51b0014    ldr r0, [fp, #-20]
    3c60:   e3a0100f    mov r1, #15 ; 0xf
    3c64:   e3a02000    mov r2, #0  ; 0x0
    3c68:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3c68: R_ARM_PC24    MGC_FAPI_WriteReg8
    3c6c:   e15b30be    ldrh    r3, [fp, #-14]
    3c70:   e3833001    orr r3, r3, #1  ; 0x1
    3c74:   e6ff3073    uxth    r3, r3
    3c78:   e51b0014    ldr r0, [fp, #-20]
    3c7c:   e3a01002    mov r1, #2  ; 0x2
    3c80:   e1a02003    mov r2, r3
    3c84:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3c84: R_ARM_PC24    MGC_FAPI_WriteReg16
    3c88:   ea000099    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3c8c:   e51b0014    ldr r0, [fp, #-20]
    3c90:   e3a0100f    mov r1, #15 ; 0xf
    3c94:   e3a02002    mov r2, #2  ; 0x2
    3c98:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3c98: R_ARM_PC24    MGC_FAPI_WriteReg8
    3c9c:   ea000094    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3ca0:   e51b0014    ldr r0, [fp, #-20]
    3ca4:   e3a0100f    mov r1, #15 ; 0xf
    3ca8:   e3a02004    mov r2, #4  ; 0x4
    3cac:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3cac: R_ARM_PC24    MGC_FAPI_WriteReg8
    3cb0:   ea00008f    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3cb4:   e51b0014    ldr r0, [fp, #-20]
    3cb8:   e3a0100f    mov r1, #15 ; 0xf
    3cbc:   e3a02001    mov r2, #1  ; 0x1
    3cc0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3cc0: R_ARM_PC24    MGC_FAPI_WriteReg8
    3cc4:   ea00008a    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3cc8:   e51b0014    ldr r0, [fp, #-20]
    3ccc:   e3a0100e    mov r1, #14 ; 0xe
    3cd0:   e3a02000    mov r2, #0  ; 0x0
    3cd4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3cd4: R_ARM_PC24    MGC_FAPI_WriteReg8
    3cd8:   e51b0020    ldr r0, [fp, #-32]
    3cdc:   e3a01000    mov r1, #0  ; 0x0
    3ce0:   e3a02035    mov r2, #53 ; 0x35
    3ce4:   e59f3210    ldr r3, [pc, #528]  ; 3efc <MGC_HdrcSetPortTestMode+0x320>
    3ce8:   ebfffffe    bl  3f08 <MGC_HdrcLoadFifo>
            3ce8: R_ARM_PC24    MGC_HdrcLoadFifo
    3cec:   e51b0014    ldr r0, [fp, #-20]
    3cf0:   e3a01012    mov r1, #18 ; 0x12
    3cf4:   e3a02002    mov r2, #2  ; 0x2
    3cf8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3cf8: R_ARM_PC24    MGC_FAPI_WriteReg16
    3cfc:   e51b0014    ldr r0, [fp, #-20]
    3d00:   e3a0100f    mov r1, #15 ; 0xf
    3d04:   e3a02008    mov r2, #8  ; 0x8
    3d08:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3d08: R_ARM_PC24    MGC_FAPI_WriteReg8
    3d0c:   ea000078    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3d10:   e51b0014    ldr r0, [fp, #-20]
    3d14:   e3a01001    mov r1, #1  ; 0x1
    3d18:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            3d18: R_ARM_PC24    MGC_FAPI_ReadReg8
    3d1c:   e1a03000    mov r3, r0
    3d20:   e54b3019    strb    r3, [fp, #-25]
    3d24:   e55b3019    ldrb    r3, [fp, #-25]
    3d28:   e3833008    orr r3, r3, #8  ; 0x8
    3d2c:   e6ef3073    uxtb    r3, r3
    3d30:   e51b0014    ldr r0, [fp, #-20]
    3d34:   e3a01001    mov r1, #1  ; 0x1
    3d38:   e1a02003    mov r2, r3
    3d3c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3d3c: R_ARM_PC24    MGC_FAPI_WriteReg8
    3d40:   e51b3018    ldr r3, [fp, #-24]
    3d44:   e593300c    ldr r3, [r3, #12]
    3d48:   e593c018    ldr ip, [r3, #24]
    3d4c:   e51b3018    ldr r3, [fp, #-24]
    3d50:   e593300c    ldr r3, [r3, #12]
    3d54:   e5932004    ldr r2, [r3, #4]
    3d58:   e59f31a0    ldr r3, [pc, #416]  ; 3f00 <MGC_HdrcSetPortTestMode+0x324>
    3d5c:   e58d3000    str r3, [sp]
    3d60:   e1a00002    mov r0, r2
    3d64:   e3a01000    mov r1, #0  ; 0x0
    3d68:   e3a0203c    mov r2, #60 ; 0x3c
    3d6c:   e3a03000    mov r3, #0  ; 0x0
    3d70:   e12fff3c    blx ip
    3d74:   ea00005e    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3d78:   e51b0014    ldr r0, [fp, #-20]
    3d7c:   e3a01001    mov r1, #1  ; 0x1
    3d80:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            3d80: R_ARM_PC24    MGC_FAPI_ReadReg8
    3d84:   e1a03000    mov r3, r0
    3d88:   e54b3019    strb    r3, [fp, #-25]
    3d8c:   e55b3019    ldrb    r3, [fp, #-25]
    3d90:   e3833002    orr r3, r3, #2  ; 0x2
    3d94:   e6ef3073    uxtb    r3, r3
    3d98:   e51b0014    ldr r0, [fp, #-20]
    3d9c:   e3a01001    mov r1, #1  ; 0x1
    3da0:   e1a02003    mov r2, r3
    3da4:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3da4: R_ARM_PC24    MGC_FAPI_WriteReg8
    3da8:   ea000051    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3dac:   e51b0014    ldr r0, [fp, #-20]
    3db0:   e3a01001    mov r1, #1  ; 0x1
    3db4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            3db4: R_ARM_PC24    MGC_FAPI_ReadReg8
    3db8:   e1a03000    mov r3, r0
    3dbc:   e3c33002    bic r3, r3, #2  ; 0x2
    3dc0:   e54b3019    strb    r3, [fp, #-25]
    3dc4:   e55b3019    ldrb    r3, [fp, #-25]
    3dc8:   e3833004    orr r3, r3, #4  ; 0x4
    3dcc:   e6ef3073    uxtb    r3, r3
    3dd0:   e51b0014    ldr r0, [fp, #-20]
    3dd4:   e3a01001    mov r1, #1  ; 0x1
    3dd8:   e1a02003    mov r2, r3
    3ddc:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3ddc: R_ARM_PC24    MGC_FAPI_WriteReg8
    3de0:   e51b3018    ldr r3, [fp, #-24]
    3de4:   e593300c    ldr r3, [r3, #12]
    3de8:   e593c018    ldr ip, [r3, #24]
    3dec:   e51b3018    ldr r3, [fp, #-24]
    3df0:   e593300c    ldr r3, [r3, #12]
    3df4:   e5932004    ldr r2, [r3, #4]
    3df8:   e59f3104    ldr r3, [pc, #260]  ; 3f04 <MGC_HdrcSetPortTestMode+0x328>
    3dfc:   e58d3000    str r3, [sp]
    3e00:   e1a00002    mov r0, r2
    3e04:   e3a01000    mov r1, #0  ; 0x0
    3e08:   e3a0200a    mov r2, #10 ; 0xa
    3e0c:   e3a03000    mov r3, #0  ; 0x0
    3e10:   e12fff3c    blx ip
    3e14:   ea000036    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3e18:   e15b30be    ldrh    r3, [fp, #-14]
    3e1c:   e3a02001    mov r2, #1  ; 0x1
    3e20:   e0033002    and r3, r3, r2
    3e24:   e51b0014    ldr r0, [fp, #-20]
    3e28:   e3a01002    mov r1, #2  ; 0x2
    3e2c:   e1a02003    mov r2, r3
    3e30:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3e30: R_ARM_PC24    MGC_FAPI_WriteReg16
    3e34:   e51b0014    ldr r0, [fp, #-20]
    3e38:   e3a0100e    mov r1, #14 ; 0xe
    3e3c:   e3a02000    mov r2, #0  ; 0x0
    3e40:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3e40: R_ARM_PC24    MGC_FAPI_WriteReg8
    3e44:   e51b0014    ldr r0, [fp, #-20]
    3e48:   e3a01012    mov r1, #18 ; 0x12
    3e4c:   e3a02c01    mov r2, #256    ; 0x100
    3e50:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3e50: R_ARM_PC24    MGC_FAPI_WriteReg16
    3e54:   e51b0014    ldr r0, [fp, #-20]
    3e58:   e3a01012    mov r1, #18 ; 0x12
    3e5c:   e3a02c01    mov r2, #256    ; 0x100
    3e60:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3e60: R_ARM_PC24    MGC_FAPI_WriteReg16
    3e64:   e51b0014    ldr r0, [fp, #-20]
    3e68:   e3a01012    mov r1, #18 ; 0x12
    3e6c:   e3a0200a    mov r2, #10 ; 0xa
    3e70:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3e70: R_ARM_PC24    MGC_FAPI_WriteReg16
    3e74:   ea00001e    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3e78:   e15b30be    ldrh    r3, [fp, #-14]
    3e7c:   e3a02001    mov r2, #1  ; 0x1
    3e80:   e0033002    and r3, r3, r2
    3e84:   e51b0014    ldr r0, [fp, #-20]
    3e88:   e3a01002    mov r1, #2  ; 0x2
    3e8c:   e1a02003    mov r2, r3
    3e90:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3e90: R_ARM_PC24    MGC_FAPI_WriteReg16
    3e94:   e51b0014    ldr r0, [fp, #-20]
    3e98:   e3a0100e    mov r1, #14 ; 0xe
    3e9c:   e3a02000    mov r2, #0  ; 0x0
    3ea0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3ea0: R_ARM_PC24    MGC_FAPI_WriteReg8
    3ea4:   e51b0014    ldr r0, [fp, #-20]
    3ea8:   e3a01012    mov r1, #18 ; 0x12
    3eac:   e3a02020    mov r2, #32 ; 0x20
    3eb0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3eb0: R_ARM_PC24    MGC_FAPI_WriteReg16
    3eb4:   ea00000e    b   3ef4 <MGC_HdrcSetPortTestMode+0x318>
    3eb8:   e15b30be    ldrh    r3, [fp, #-14]
    3ebc:   e3a02001    mov r2, #1  ; 0x1
    3ec0:   e0033002    and r3, r3, r2
    3ec4:   e51b0014    ldr r0, [fp, #-20]
    3ec8:   e3a01002    mov r1, #2  ; 0x2
    3ecc:   e1a02003    mov r2, r3
    3ed0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3ed0: R_ARM_PC24    MGC_FAPI_WriteReg16
    3ed4:   e51b0014    ldr r0, [fp, #-20]
    3ed8:   e3a0100e    mov r1, #14 ; 0xe
    3edc:   e3a02000    mov r2, #0  ; 0x0
    3ee0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            3ee0: R_ARM_PC24    MGC_FAPI_WriteReg8
    3ee4:   e51b0014    ldr r0, [fp, #-20]
    3ee8:   e3a01012    mov r1, #18 ; 0x12
    3eec:   e3a02060    mov r2, #96 ; 0x60
    3ef0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            3ef0: R_ARM_PC24    MGC_FAPI_WriteReg16
    3ef4:   e24bd00c    sub sp, fp, #12 ; 0xc
    3ef8:   e89da800    ldm sp, {fp, sp, pc}
    ...
            3efc: R_ARM_ABS32   .rodata
            3f00: R_ARM_ABS32   MGC_HdrcResetOff
            3f04: R_ARM_ABS32   MGC_HdrcResumeOff

00003f08 <MGC_HdrcLoadFifo>:
    3f08:   e1a0c00d    mov ip, sp
    3f0c:   e92dd800    push    {fp, ip, lr, pc}
    3f10:   e24cb004    sub fp, ip, #4  ; 0x4
    3f14:   e24dd030    sub sp, sp, #48 ; 0x30
    3f18:   e50b002c    str r0, [fp, #-44]
    3f1c:   e50b2034    str r2, [fp, #-52]
    3f20:   e50b3038    str r3, [fp, #-56]
    3f24:   e1a03001    mov r3, r1
    3f28:   e54b3030    strb    r3, [fp, #-48]
    3f2c:   e51b3038    ldr r3, [fp, #-56]
    3f30:   e2033003    and r3, r3, #3  ; 0x3
    3f34:   e3530000    cmp r3, #0  ; 0x0
    3f38:   1a000003    bne 3f4c <MGC_HdrcLoadFifo+0x44>
    3f3c:   e51b3034    ldr r3, [fp, #-52]
    3f40:   e1a03123    lsr r3, r3, #2
    3f44:   e50b303c    str r3, [fp, #-60]
    3f48:   ea000001    b   3f54 <MGC_HdrcLoadFifo+0x4c>
    3f4c:   e3a03000    mov r3, #0  ; 0x0
    3f50:   e50b303c    str r3, [fp, #-60]
    3f54:   e51b303c    ldr r3, [fp, #-60]
    3f58:   e50b301c    str r3, [fp, #-28]
    3f5c:   e51b302c    ldr r3, [fp, #-44]
    3f60:   e5933000    ldr r3, [r3]
    3f64:   e50b3018    str r3, [fp, #-24]
    3f68:   e51b3018    ldr r3, [fp, #-24]
    3f6c:   e5933004    ldr r3, [r3, #4]
    3f70:   e50b3014    str r3, [fp, #-20]
    3f74:   e55b3030    ldrb    r3, [fp, #-48]
    3f78:   e1a03103    lsl r3, r3, #2
    3f7c:   e6ef3073    uxtb    r3, r3
    3f80:   e2833020    add r3, r3, #32 ; 0x20
    3f84:   e54b300d    strb    r3, [fp, #-13]
    3f88:   e3a03000    mov r3, #0  ; 0x0
    3f8c:   e50b3024    str r3, [fp, #-36]
    3f90:   e51b3024    ldr r3, [fp, #-36]
    3f94:   e50b3028    str r3, [fp, #-40]
    3f98:   ea00000f    b   3fdc <MGC_HdrcLoadFifo+0xd4>
    3f9c:   e51b2028    ldr r2, [fp, #-40]
    3fa0:   e51b3038    ldr r3, [fp, #-56]
    3fa4:   e0823003    add r3, r2, r3
    3fa8:   e5933000    ldr r3, [r3]
    3fac:   e50b3020    str r3, [fp, #-32]
    3fb0:   e55b300d    ldrb    r3, [fp, #-13]
    3fb4:   e51b0014    ldr r0, [fp, #-20]
    3fb8:   e1a01003    mov r1, r3
    3fbc:   e51b2020    ldr r2, [fp, #-32]
    3fc0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg32>
            3fc0: R_ARM_PC24    MGC_FAPI_WriteReg32
    3fc4:   e51b3024    ldr r3, [fp, #-36]
    3fc8:   e2833001    add r3, r3, #1  ; 0x1
    3fcc:   e50b3024    str r3, [fp, #-36]
    3fd0:   e51b3028    ldr r3, [fp, #-40]
    3fd4:   e2833004    add r3, r3, #4  ; 0x4
    3fd8:   e50b3028    str r3, [fp, #-40]
    3fdc:   e51b2024    ldr r2, [fp, #-36]
    3fe0:   e51b301c    ldr r3, [fp, #-28]
    3fe4:   e1520003    cmp r2, r3
    3fe8:   3affffeb    bcc 3f9c <MGC_HdrcLoadFifo+0x94>
    3fec:   ea00000a    b   401c <MGC_HdrcLoadFifo+0x114>
    3ff0:   e55b100d    ldrb    r1, [fp, #-13]
    3ff4:   e51b2028    ldr r2, [fp, #-40]
    3ff8:   e51b3038    ldr r3, [fp, #-56]
    3ffc:   e0823003    add r3, r2, r3
    4000:   e5d33000    ldrb    r3, [r3]
    4004:   e1a02003    mov r2, r3
    4008:   e51b3028    ldr r3, [fp, #-40]
    400c:   e2833001    add r3, r3, #1  ; 0x1
    4010:   e50b3028    str r3, [fp, #-40]
    4014:   e51b0014    ldr r0, [fp, #-20]
    4018:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4018: R_ARM_PC24    MGC_FAPI_WriteReg8
    401c:   e51b2028    ldr r2, [fp, #-40]
    4020:   e51b3034    ldr r3, [fp, #-52]
    4024:   e1520003    cmp r2, r3
    4028:   3afffff0    bcc 3ff0 <MGC_HdrcLoadFifo+0xe8>
    402c:   e3a03001    mov r3, #1  ; 0x1
    4030:   e1a00003    mov r0, r3
    4034:   e24bd00c    sub sp, fp, #12 ; 0xc
    4038:   e89da800    ldm sp, {fp, sp, pc}

0000403c <MGC_HdrcUnloadFifo>:
    403c:   e1a0c00d    mov ip, sp
    4040:   e92dd810    push    {r4, fp, ip, lr, pc}
    4044:   e24cb004    sub fp, ip, #4  ; 0x4
    4048:   e24dd02c    sub sp, sp, #44 ; 0x2c
    404c:   e50b002c    str r0, [fp, #-44]
    4050:   e50b2034    str r2, [fp, #-52]
    4054:   e50b3038    str r3, [fp, #-56]
    4058:   e1a03001    mov r3, r1
    405c:   e54b3030    strb    r3, [fp, #-48]
    4060:   e51b3038    ldr r3, [fp, #-56]
    4064:   e2033003    and r3, r3, #3  ; 0x3
    4068:   e3530000    cmp r3, #0  ; 0x0
    406c:   1a000003    bne 4080 <MGC_HdrcUnloadFifo+0x44>
    4070:   e51b3034    ldr r3, [fp, #-52]
    4074:   e1a03123    lsr r3, r3, #2
    4078:   e50b303c    str r3, [fp, #-60]
    407c:   ea000001    b   4088 <MGC_HdrcUnloadFifo+0x4c>
    4080:   e3a03000    mov r3, #0  ; 0x0
    4084:   e50b303c    str r3, [fp, #-60]
    4088:   e51b303c    ldr r3, [fp, #-60]
    408c:   e50b3020    str r3, [fp, #-32]
    4090:   e51b302c    ldr r3, [fp, #-44]
    4094:   e5933000    ldr r3, [r3]
    4098:   e50b301c    str r3, [fp, #-28]
    409c:   e51b301c    ldr r3, [fp, #-28]
    40a0:   e5933004    ldr r3, [r3, #4]
    40a4:   e50b3018    str r3, [fp, #-24]
    40a8:   e55b3030    ldrb    r3, [fp, #-48]
    40ac:   e1a03103    lsl r3, r3, #2
    40b0:   e6ef3073    uxtb    r3, r3
    40b4:   e2833020    add r3, r3, #32 ; 0x20
    40b8:   e54b3011    strb    r3, [fp, #-17]
    40bc:   e3a03000    mov r3, #0  ; 0x0
    40c0:   e50b3024    str r3, [fp, #-36]
    40c4:   e51b3024    ldr r3, [fp, #-36]
    40c8:   e50b3028    str r3, [fp, #-40]
    40cc:   ea00000f    b   4110 <MGC_HdrcUnloadFifo+0xd4>
    40d0:   e51b2028    ldr r2, [fp, #-40]
    40d4:   e51b3038    ldr r3, [fp, #-56]
    40d8:   e0823003    add r3, r2, r3
    40dc:   e1a04003    mov r4, r3
    40e0:   e55b3011    ldrb    r3, [fp, #-17]
    40e4:   e51b0018    ldr r0, [fp, #-24]
    40e8:   e1a01003    mov r1, r3
    40ec:   ebfffffe    bl  0 <MGC_FAPI_ReadReg32>
            40ec: R_ARM_PC24    MGC_FAPI_ReadReg32
    40f0:   e1a03000    mov r3, r0
    40f4:   e5843000    str r3, [r4]
    40f8:   e51b3024    ldr r3, [fp, #-36]
    40fc:   e2833001    add r3, r3, #1  ; 0x1
    4100:   e50b3024    str r3, [fp, #-36]
    4104:   e51b3028    ldr r3, [fp, #-40]
    4108:   e2833004    add r3, r3, #4  ; 0x4
    410c:   e50b3028    str r3, [fp, #-40]
    4110:   e51b2024    ldr r2, [fp, #-36]
    4114:   e51b3020    ldr r3, [fp, #-32]
    4118:   e1520003    cmp r2, r3
    411c:   3affffeb    bcc 40d0 <MGC_HdrcUnloadFifo+0x94>
    4120:   ea00000b    b   4154 <MGC_HdrcUnloadFifo+0x118>
    4124:   e51b2028    ldr r2, [fp, #-40]
    4128:   e51b3038    ldr r3, [fp, #-56]
    412c:   e0824003    add r4, r2, r3
    4130:   e55b3011    ldrb    r3, [fp, #-17]
    4134:   e51b0018    ldr r0, [fp, #-24]
    4138:   e1a01003    mov r1, r3
    413c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            413c: R_ARM_PC24    MGC_FAPI_ReadReg8
    4140:   e1a03000    mov r3, r0
    4144:   e5c43000    strb    r3, [r4]
    4148:   e51b3028    ldr r3, [fp, #-40]
    414c:   e2833001    add r3, r3, #1  ; 0x1
    4150:   e50b3028    str r3, [fp, #-40]
    4154:   e51b2028    ldr r2, [fp, #-40]
    4158:   e51b3034    ldr r3, [fp, #-52]
    415c:   e1520003    cmp r2, r3
    4160:   3affffef    bcc 4124 <MGC_HdrcUnloadFifo+0xe8>
    4164:   e3a03001    mov r3, #1  ; 0x1
    4168:   e1a00003    mov r0, r3
    416c:   e24bd010    sub sp, fp, #16 ; 0x10
    4170:   e89da810    ldm sp, {r4, fp, sp, pc}

00004174 <MGC_HdrcUlpiVbusControl>:
    4174:   e1a0c00d    mov ip, sp
    4178:   e92dd800    push    {fp, ip, lr, pc}
    417c:   e24cb004    sub fp, ip, #4  ; 0x4
    4180:   e24dd020    sub sp, sp, #32 ; 0x20
    4184:   e50b001c    str r0, [fp, #-28]
    4188:   e1a03001    mov r3, r1
    418c:   e54b3020    strb    r3, [fp, #-32]
    4190:   e1a03002    mov r3, r2
    4194:   e54b3024    strb    r3, [fp, #-36]
    4198:   e51b301c    ldr r3, [fp, #-28]
    419c:   e5933000    ldr r3, [r3]
    41a0:   e50b3014    str r3, [fp, #-20]
    41a4:   e51b3014    ldr r3, [fp, #-20]
    41a8:   e5933004    ldr r3, [r3, #4]
    41ac:   e50b3010    str r3, [fp, #-16]
    41b0:   e51b0010    ldr r0, [fp, #-16]
    41b4:   e3a01001    mov r1, #1  ; 0x1
    41b8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            41b8: R_ARM_PC24    MGC_FAPI_ReadReg8
    41bc:   e1a03000    mov r3, r0
    41c0:   e2033001    and r3, r3, #1  ; 0x1
    41c4:   e6ef3073    uxtb    r3, r3
    41c8:   e3530000    cmp r3, #0  ; 0x0
    41cc:   0a000002    beq 41dc <MGC_HdrcUlpiVbusControl+0x68>
    41d0:   e3a01000    mov r1, #0  ; 0x0
    41d4:   e50b102c    str r1, [fp, #-44]
    41d8:   ea000019    b   4244 <MGC_HdrcUlpiVbusControl+0xd0>
    41dc:   e55b3020    ldrb    r3, [fp, #-32]
    41e0:   e3530000    cmp r3, #0  ; 0x0
    41e4:   03a03000    moveq   r3, #0  ; 0x0
    41e8:   13a03001    movne   r3, #1  ; 0x1
    41ec:   e54b3015    strb    r3, [fp, #-21]
    41f0:   e55b3024    ldrb    r3, [fp, #-36]
    41f4:   e3530000    cmp r3, #0  ; 0x0
    41f8:   0a000002    beq 4208 <MGC_HdrcUlpiVbusControl+0x94>
    41fc:   e3a03002    mov r3, #2  ; 0x2
    4200:   e50b3028    str r3, [fp, #-40]
    4204:   ea000001    b   4210 <MGC_HdrcUlpiVbusControl+0x9c>
    4208:   e3a01000    mov r1, #0  ; 0x0
    420c:   e50b1028    str r1, [fp, #-40]
    4210:   e55b3015    ldrb    r3, [fp, #-21]
    4214:   e51b1028    ldr r1, [fp, #-40]
    4218:   e1a02001    mov r2, r1
    421c:   e1823003    orr r3, r2, r3
    4220:   e6ef3073    uxtb    r3, r3
    4224:   e54b3015    strb    r3, [fp, #-21]
    4228:   e55b3015    ldrb    r3, [fp, #-21]
    422c:   e51b0010    ldr r0, [fp, #-16]
    4230:   e3a01070    mov r1, #112    ; 0x70
    4234:   e1a02003    mov r2, r3
    4238:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4238: R_ARM_PC24    MGC_FAPI_WriteReg8
    423c:   e3a03001    mov r3, #1  ; 0x1
    4240:   e50b302c    str r3, [fp, #-44]
    4244:   e51b302c    ldr r3, [fp, #-44]
    4248:   e1a00003    mov r0, r3
    424c:   e24bd00c    sub sp, fp, #12 ; 0xc
    4250:   e89da800    ldm sp, {fp, sp, pc}

00004254 <MGC_HdrcReadUlpiReg>:
    4254:   e1a0c00d    mov ip, sp
    4258:   e92dd870    push    {r4, r5, r6, fp, ip, lr, pc}
    425c:   e24cb004    sub fp, ip, #4  ; 0x4
    4260:   e24dd02c    sub sp, sp, #44 ; 0x2c
    4264:   e50b0030    str r0, [fp, #-48]
    4268:   e1a03001    mov r3, r1
    426c:   e50b2038    str r2, [fp, #-56]
    4270:   e54b3034    strb    r3, [fp, #-52]
    4274:   e3a03000    mov r3, #0  ; 0x0
    4278:   e54b3029    strb    r3, [fp, #-41]
    427c:   e51b3030    ldr r3, [fp, #-48]
    4280:   e5933000    ldr r3, [r3]
    4284:   e50b3028    str r3, [fp, #-40]
    4288:   e51b3028    ldr r3, [fp, #-40]
    428c:   e5933004    ldr r3, [r3, #4]
    4290:   e50b3024    str r3, [fp, #-36]
    4294:   e51b0024    ldr r0, [fp, #-36]
    4298:   e3a01001    mov r1, #1  ; 0x1
    429c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            429c: R_ARM_PC24    MGC_FAPI_ReadReg8
    42a0:   e1a03000    mov r3, r0
    42a4:   e2033001    and r3, r3, #1  ; 0x1
    42a8:   e6ef3073    uxtb    r3, r3
    42ac:   e3530000    cmp r3, #0  ; 0x0
    42b0:   0a000002    beq 42c0 <MGC_HdrcReadUlpiReg+0x6c>
    42b4:   e3a03000    mov r3, #0  ; 0x0
    42b8:   e50b3044    str r3, [fp, #-68]
    42bc:   ea00003d    b   43b8 <MGC_HdrcReadUlpiReg+0x164>
    42c0:   e55b3034    ldrb    r3, [fp, #-52]
    42c4:   e51b0024    ldr r0, [fp, #-36]
    42c8:   e3a01075    mov r1, #117    ; 0x75
    42cc:   e1a02003    mov r2, r3
    42d0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            42d0: R_ARM_PC24    MGC_FAPI_WriteReg8
    42d4:   e51b0024    ldr r0, [fp, #-36]
    42d8:   e3a01076    mov r1, #118    ; 0x76
    42dc:   e3a02005    mov r2, #5  ; 0x5
    42e0:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            42e0: R_ARM_PC24    MGC_FAPI_WriteReg8
    42e4:   e3a00ffa    mov r0, #1000   ; 0x3e8
    42e8:   ebfffffe    bl  0 <FAPI_TIMER_GetTimeStamp>
            42e8: R_ARM_PC24    FAPI_TIMER_GetTimeStamp
    42ec:   e1a03000    mov r3, r0
    42f0:   e1a04001    mov r4, r1
    42f4:   e50b3020    str r3, [fp, #-32]
    42f8:   e50b401c    str r4, [fp, #-28]
    42fc:   ea00001d    b   4378 <MGC_HdrcReadUlpiReg+0x124>
    4300:   e3a00ffa    mov r0, #1000   ; 0x3e8
    4304:   ebfffffe    bl  0 <FAPI_TIMER_GetTimeStamp>
            4304: R_ARM_PC24    FAPI_TIMER_GetTimeStamp
    4308:   e1a02001    mov r2, r1
    430c:   e1a01000    mov r1, r0
    4310:   e24b3020    sub r3, fp, #32 ; 0x20
    4314:   e8930018    ldm r3, {r3, r4}
    4318:   e1a05001    mov r5, r1
    431c:   e1a06002    mov r6, r2
    4320:   e0555003    subs    r5, r5, r3
    4324:   e0c66004    sbc r6, r6, r4
    4328:   e50b5040    str r5, [fp, #-64]
    432c:   e50b603c    str r6, [fp, #-60]
    4330:   e51b603c    ldr r6, [fp, #-60]
    4334:   e3560000    cmp r6, #0  ; 0x0
    4338:   8a000006    bhi 4358 <MGC_HdrcReadUlpiReg+0x104>
    433c:   e51b303c    ldr r3, [fp, #-60]
    4340:   e3530000    cmp r3, #0  ; 0x0
    4344:   1a000006    bne 4364 <MGC_HdrcReadUlpiReg+0x110>
    4348:   e51b5040    ldr r5, [fp, #-64]
    434c:   e3550f7d    cmp r5, #500    ; 0x1f4
    4350:   8a000000    bhi 4358 <MGC_HdrcReadUlpiReg+0x104>
    4354:   ea000002    b   4364 <MGC_HdrcReadUlpiReg+0x110>
    4358:   e3a06000    mov r6, #0  ; 0x0
    435c:   e50b6044    str r6, [fp, #-68]
    4360:   ea000014    b   43b8 <MGC_HdrcReadUlpiReg+0x164>
    4364:   e51b0024    ldr r0, [fp, #-36]
    4368:   e3a01076    mov r1, #118    ; 0x76
    436c:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            436c: R_ARM_PC24    MGC_FAPI_ReadReg8
    4370:   e1a03000    mov r3, r0
    4374:   e54b3029    strb    r3, [fp, #-41]
    4378:   e55b3029    ldrb    r3, [fp, #-41]
    437c:   e2033002    and r3, r3, #2  ; 0x2
    4380:   e3530000    cmp r3, #0  ; 0x0
    4384:   0affffdd    beq 4300 <MGC_HdrcReadUlpiReg+0xac>
    4388:   e51b0024    ldr r0, [fp, #-36]
    438c:   e3a01074    mov r1, #116    ; 0x74
    4390:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            4390: R_ARM_PC24    MGC_FAPI_ReadReg8
    4394:   e1a03000    mov r3, r0
    4398:   e51b2038    ldr r2, [fp, #-56]
    439c:   e5c23000    strb    r3, [r2]
    43a0:   e51b0024    ldr r0, [fp, #-36]
    43a4:   e3a01076    mov r1, #118    ; 0x76
    43a8:   e3a02000    mov r2, #0  ; 0x0
    43ac:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            43ac: R_ARM_PC24    MGC_FAPI_WriteReg8
    43b0:   e3a03001    mov r3, #1  ; 0x1
    43b4:   e50b3044    str r3, [fp, #-68]
    43b8:   e51b3044    ldr r3, [fp, #-68]
    43bc:   e1a00003    mov r0, r3
    43c0:   e24bd018    sub sp, fp, #24 ; 0x18
    43c4:   e89da870    ldm sp, {r4, r5, r6, fp, sp, pc}

000043c8 <MGC_HdrcWriteUlpiReg>:
    43c8:   e1a0c00d    mov ip, sp
    43cc:   e92dd870    push    {r4, r5, r6, fp, ip, lr, pc}
    43d0:   e24cb004    sub fp, ip, #4  ; 0x4
    43d4:   e24dd02c    sub sp, sp, #44 ; 0x2c
    43d8:   e50b0030    str r0, [fp, #-48]
    43dc:   e1a03001    mov r3, r1
    43e0:   e54b3034    strb    r3, [fp, #-52]
    43e4:   e1a03002    mov r3, r2
    43e8:   e54b3038    strb    r3, [fp, #-56]
    43ec:   e3a03000    mov r3, #0  ; 0x0
    43f0:   e54b3029    strb    r3, [fp, #-41]
    43f4:   e51b3030    ldr r3, [fp, #-48]
    43f8:   e5933000    ldr r3, [r3]
    43fc:   e50b3028    str r3, [fp, #-40]
    4400:   e51b3028    ldr r3, [fp, #-40]
    4404:   e5933004    ldr r3, [r3, #4]
    4408:   e50b3024    str r3, [fp, #-36]
    440c:   e51b0024    ldr r0, [fp, #-36]
    4410:   e3a01001    mov r1, #1  ; 0x1
    4414:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            4414: R_ARM_PC24    MGC_FAPI_ReadReg8
    4418:   e1a03000    mov r3, r0
    441c:   e2033001    and r3, r3, #1  ; 0x1
    4420:   e6ef3073    uxtb    r3, r3
    4424:   e3530000    cmp r3, #0  ; 0x0
    4428:   0a000002    beq 4438 <MGC_HdrcWriteUlpiReg+0x70>
    442c:   e3a03000    mov r3, #0  ; 0x0
    4430:   e50b3044    str r3, [fp, #-68]
    4434:   ea00003c    b   452c <MGC_HdrcWriteUlpiReg+0x164>
    4438:   e55b3034    ldrb    r3, [fp, #-52]
    443c:   e51b0024    ldr r0, [fp, #-36]
    4440:   e3a01075    mov r1, #117    ; 0x75
    4444:   e1a02003    mov r2, r3
    4448:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4448: R_ARM_PC24    MGC_FAPI_WriteReg8
    444c:   e55b3038    ldrb    r3, [fp, #-56]
    4450:   e51b0024    ldr r0, [fp, #-36]
    4454:   e3a01074    mov r1, #116    ; 0x74
    4458:   e1a02003    mov r2, r3
    445c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            445c: R_ARM_PC24    MGC_FAPI_WriteReg8
    4460:   e51b0024    ldr r0, [fp, #-36]
    4464:   e3a01076    mov r1, #118    ; 0x76
    4468:   e3a02001    mov r2, #1  ; 0x1
    446c:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            446c: R_ARM_PC24    MGC_FAPI_WriteReg8
    4470:   e3a00ffa    mov r0, #1000   ; 0x3e8
    4474:   ebfffffe    bl  0 <FAPI_TIMER_GetTimeStamp>
            4474: R_ARM_PC24    FAPI_TIMER_GetTimeStamp
    4478:   e1a03000    mov r3, r0
    447c:   e1a04001    mov r4, r1
    4480:   e50b3020    str r3, [fp, #-32]
    4484:   e50b401c    str r4, [fp, #-28]
    4488:   ea00001d    b   4504 <MGC_HdrcWriteUlpiReg+0x13c>
    448c:   e3a00ffa    mov r0, #1000   ; 0x3e8
    4490:   ebfffffe    bl  0 <FAPI_TIMER_GetTimeStamp>
            4490: R_ARM_PC24    FAPI_TIMER_GetTimeStamp
    4494:   e1a02001    mov r2, r1
    4498:   e1a01000    mov r1, r0
    449c:   e24b3020    sub r3, fp, #32 ; 0x20
    44a0:   e8930018    ldm r3, {r3, r4}
    44a4:   e1a05001    mov r5, r1
    44a8:   e1a06002    mov r6, r2
    44ac:   e0555003    subs    r5, r5, r3
    44b0:   e0c66004    sbc r6, r6, r4
    44b4:   e50b5040    str r5, [fp, #-64]
    44b8:   e50b603c    str r6, [fp, #-60]
    44bc:   e51b603c    ldr r6, [fp, #-60]
    44c0:   e3560000    cmp r6, #0  ; 0x0
    44c4:   8a000006    bhi 44e4 <MGC_HdrcWriteUlpiReg+0x11c>
    44c8:   e51b303c    ldr r3, [fp, #-60]
    44cc:   e3530000    cmp r3, #0  ; 0x0
    44d0:   1a000006    bne 44f0 <MGC_HdrcWriteUlpiReg+0x128>
    44d4:   e51b5040    ldr r5, [fp, #-64]
    44d8:   e3550f7d    cmp r5, #500    ; 0x1f4
    44dc:   8a000000    bhi 44e4 <MGC_HdrcWriteUlpiReg+0x11c>
    44e0:   ea000002    b   44f0 <MGC_HdrcWriteUlpiReg+0x128>
    44e4:   e3a06000    mov r6, #0  ; 0x0
    44e8:   e50b6044    str r6, [fp, #-68]
    44ec:   ea00000e    b   452c <MGC_HdrcWriteUlpiReg+0x164>
    44f0:   e51b0024    ldr r0, [fp, #-36]
    44f4:   e3a01076    mov r1, #118    ; 0x76
    44f8:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            44f8: R_ARM_PC24    MGC_FAPI_ReadReg8
    44fc:   e1a03000    mov r3, r0
    4500:   e54b3029    strb    r3, [fp, #-41]
    4504:   e55b3029    ldrb    r3, [fp, #-41]
    4508:   e2033002    and r3, r3, #2  ; 0x2
    450c:   e3530000    cmp r3, #0  ; 0x0
    4510:   0affffdd    beq 448c <MGC_HdrcWriteUlpiReg+0xc4>
    4514:   e51b0024    ldr r0, [fp, #-36]
    4518:   e3a01076    mov r1, #118    ; 0x76
    451c:   e3a02000    mov r2, #0  ; 0x0
    4520:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4520: R_ARM_PC24    MGC_FAPI_WriteReg8
    4524:   e3a03001    mov r3, #1  ; 0x1
    4528:   e50b3044    str r3, [fp, #-68]
    452c:   e51b3044    ldr r3, [fp, #-68]
    4530:   e1a00003    mov r0, r3
    4534:   e24bd018    sub sp, fp, #24 ; 0x18
    4538:   e89da870    ldm sp, {r4, r5, r6, fp, sp, pc}

0000453c <MGC_HdrcDmaChannelStatusChanged>:
    453c:   e1a0c00d    mov ip, sp
    4540:   e92dd800    push    {fp, ip, lr, pc}
    4544:   e24cb004    sub fp, ip, #4  ; 0x4
    4548:   e24dd048    sub sp, sp, #72 ; 0x48
    454c:   e50b0040    str r0, [fp, #-64]
    4550:   e1a03001    mov r3, r1
    4554:   e54b3044    strb    r3, [fp, #-68]
    4558:   e1a03002    mov r3, r2
    455c:   e54b3048    strb    r3, [fp, #-72]
    4560:   e3a03000    mov r3, #0  ; 0x0
    4564:   e14b32b6    strh    r3, [fp, #-38]
    4568:   e3a03000    mov r3, #0  ; 0x0
    456c:   e50b3024    str r3, [fp, #-36]
    4570:   e51b3040    ldr r3, [fp, #-64]
    4574:   e50b3020    str r3, [fp, #-32]
    4578:   e51b3020    ldr r3, [fp, #-32]
    457c:   e5933000    ldr r3, [r3]
    4580:   e50b301c    str r3, [fp, #-28]
    4584:   e51b301c    ldr r3, [fp, #-28]
    4588:   e593300c    ldr r3, [r3, #12]
    458c:   e50b3018    str r3, [fp, #-24]
    4590:   e51b301c    ldr r3, [fp, #-28]
    4594:   e5933000    ldr r3, [r3]
    4598:   e50b3014    str r3, [fp, #-20]
    459c:   e51b0014    ldr r0, [fp, #-20]
    45a0:   e3a0100e    mov r1, #14 ; 0xe
    45a4:   ebfffffe    bl  0 <MGC_FAPI_ReadReg8>
            45a4: R_ARM_PC24    MGC_FAPI_ReadReg8
    45a8:   e1a03000    mov r3, r0
    45ac:   e54b300d    strb    r3, [fp, #-13]
    45b0:   e51b3020    ldr r3, [fp, #-32]
    45b4:   e283300c    add r3, r3, #12 ; 0xc
    45b8:   e55b2044    ldrb    r2, [fp, #-68]
    45bc:   e1a00003    mov r0, r3
    45c0:   e1a01002    mov r1, r2
    45c4:   ebfffffe    bl  0 <MUSB_ArrayFetch>
            45c4: R_ARM_PC24    MUSB_ArrayFetch
    45c8:   e1a03000    mov r3, r0
    45cc:   e50b3030    str r3, [fp, #-48]
    45d0:   e55b3048    ldrb    r3, [fp, #-72]
    45d4:   e3530000    cmp r3, #0  ; 0x0
    45d8:   0a000003    beq 45ec <MGC_HdrcDmaChannelStatusChanged+0xb0>
    45dc:   e51b3030    ldr r3, [fp, #-48]
    45e0:   e5933044    ldr r3, [r3, #68]
    45e4:   e3530000    cmp r3, #0  ; 0x0
    45e8:   0a000006    beq 4608 <MGC_HdrcDmaChannelStatusChanged+0xcc>
    45ec:   e55b3048    ldrb    r3, [fp, #-72]
    45f0:   e3530000    cmp r3, #0  ; 0x0
    45f4:   1a000006    bne 4614 <MGC_HdrcDmaChannelStatusChanged+0xd8>
    45f8:   e51b3030    ldr r3, [fp, #-48]
    45fc:   e5933048    ldr r3, [r3, #72]
    4600:   e3530000    cmp r3, #0  ; 0x0
    4604:   1a000002    bne 4614 <MGC_HdrcDmaChannelStatusChanged+0xd8>
    4608:   e3a03000    mov r3, #0  ; 0x0
    460c:   e50b304c    str r3, [fp, #-76]
    4610:   ea0000af    b   48d4 <MGC_HdrcDmaChannelStatusChanged+0x398>
    4614:   e55b3048    ldrb    r3, [fp, #-72]
    4618:   e3530000    cmp r3, #0  ; 0x0
    461c:   0a000014    beq 4674 <MGC_HdrcDmaChannelStatusChanged+0x138>
    4620:   e55b3044    ldrb    r3, [fp, #-68]
    4624:   e24b203c    sub r2, fp, #60 ; 0x3c
    4628:   e51b0020    ldr r0, [fp, #-32]
    462c:   e1a01003    mov r1, r3
    4630:   ebfffffe    bl  2f70 <MGC_HdrcServiceTransmitAvail>
            4630: R_ARM_PC24    MGC_HdrcServiceTransmitAvail
    4634:   e1a03000    mov r3, r0
    4638:   e54b3031    strb    r3, [fp, #-49]
    463c:   e55b3031    ldrb    r3, [fp, #-49]
    4640:   e3530000    cmp r3, #0  ; 0x0
    4644:   0a000098    beq 48ac <MGC_HdrcDmaChannelStatusChanged+0x370>
    4648:   e55b3044    ldrb    r3, [fp, #-68]
    464c:   e54b3037    strb    r3, [fp, #-55]
    4650:   e51b3018    ldr r3, [fp, #-24]
    4654:   e593c00c    ldr ip, [r3, #12]
    4658:   e51b3018    ldr r3, [fp, #-24]
    465c:   e5933004    ldr r3, [r3, #4]
    4660:   e24b203c    sub r2, fp, #60 ; 0x3c
    4664:   e1a00003    mov r0, r3
    4668:   e1a01002    mov r1, r2
    466c:   e12fff3c    blx ip
    4670:   ea00008d    b   48ac <MGC_HdrcDmaChannelStatusChanged+0x370>
    4674:   e55b3044    ldrb    r3, [fp, #-68]
    4678:   e51b0014    ldr r0, [fp, #-20]
    467c:   e3a0100e    mov r1, #14 ; 0xe
    4680:   e1a02003    mov r2, r3
    4684:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4684: R_ARM_PC24    MGC_FAPI_WriteReg8
    4688:   e51b0014    ldr r0, [fp, #-20]
    468c:   e3a01016    mov r1, #22 ; 0x16
    4690:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            4690: R_ARM_PC24    MGC_FAPI_ReadReg16
    4694:   e1a03000    mov r3, r0
    4698:   e14b32ba    strh    r3, [fp, #-42]
    469c:   e51b3030    ldr r3, [fp, #-48]
    46a0:   e5933048    ldr r3, [r3, #72]
    46a4:   e3530000    cmp r3, #0  ; 0x0
    46a8:   0a000011    beq 46f4 <MGC_HdrcDmaChannelStatusChanged+0x1b8>
    46ac:   e51b3020    ldr r3, [fp, #-32]
    46b0:   e5933214    ldr r3, [r3, #532]
    46b4:   e5932020    ldr r2, [r3, #32]
    46b8:   e51b3030    ldr r3, [fp, #-48]
    46bc:   e5933048    ldr r3, [r3, #72]
    46c0:   e1a00003    mov r0, r3
    46c4:   e12fff32    blx r2
    46c8:   e1a03000    mov r3, r0
    46cc:   e3530001    cmp r3, #1  ; 0x1
    46d0:   1a000007    bne 46f4 <MGC_HdrcDmaChannelStatusChanged+0x1b8>
    46d4:   e51b3030    ldr r3, [fp, #-48]
    46d8:   e5933048    ldr r3, [r3, #72]
    46dc:   e5932008    ldr r2, [r3, #8]
    46e0:   e51b3030    ldr r3, [fp, #-48]
    46e4:   e5832040    str r2, [r3, #64]
    46e8:   e51b2030    ldr r2, [fp, #-48]
    46ec:   e3a03000    mov r3, #0  ; 0x0
    46f0:   e5c23051    strb    r3, [r2, #81]
    46f4:   e51b3030    ldr r3, [fp, #-48]
    46f8:   e5d33051    ldrb    r3, [r3, #81]
    46fc:   e3530000    cmp r3, #0  ; 0x0
    4700:   0a000002    beq 4710 <MGC_HdrcDmaChannelStatusChanged+0x1d4>
    4704:   e3a03000    mov r3, #0  ; 0x0
    4708:   e50b304c    str r3, [fp, #-76]
    470c:   ea000070    b   48d4 <MGC_HdrcDmaChannelStatusChanged+0x398>
    4710:   e51b0014    ldr r0, [fp, #-20]
    4714:   e3a01018    mov r1, #24 ; 0x18
    4718:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            4718: R_ARM_PC24    MGC_FAPI_ReadReg16
    471c:   e1a03000    mov r3, r0
    4720:   e14b32b6    strh    r3, [fp, #-38]
    4724:   e15bc2b6    ldrh    ip, [fp, #-38]
    4728:   e51b3030    ldr r3, [fp, #-48]
    472c:   e5932030    ldr r2, [r3, #48]
    4730:   e3a03000    mov r3, #0  ; 0x0
    4734:   e58d3000    str r3, [sp]
    4738:   e58d2004    str r2, [sp, #4]
    473c:   e51b0020    ldr r0, [fp, #-32]
    4740:   e51b1024    ldr r1, [fp, #-36]
    4744:   e51b2030    ldr r2, [fp, #-48]
    4748:   e1a0300c    mov r3, ip
    474c:   ebfffffe    bl  0 <MGC_PipePacketReceived>
            474c: R_ARM_PC24    MGC_PipePacketReceived
    4750:   e1a03000    mov r3, r0
    4754:   e54b3031    strb    r3, [fp, #-49]
    4758:   e51b3030    ldr r3, [fp, #-48]
    475c:   e1d321b6    ldrh    r2, [r3, #22]
    4760:   e15b32b6    ldrh    r3, [fp, #-38]
    4764:   e1530002    cmp r3, r2
    4768:   3a000003    bcc 477c <MGC_HdrcDmaChannelStatusChanged+0x240>
    476c:   e15b32ba    ldrh    r3, [fp, #-42]
    4770:   e6bf3073    sxth    r3, r3
    4774:   e3530000    cmp r3, #0  ; 0x0
    4778:   ba000007    blt 479c <MGC_HdrcDmaChannelStatusChanged+0x260>
    477c:   e15b32ba    ldrh    r3, [fp, #-42]
    4780:   e3c33001    bic r3, r3, #1  ; 0x1
    4784:   e14b32ba    strh    r3, [fp, #-42]
    4788:   e15b32ba    ldrh    r3, [fp, #-42]
    478c:   e51b0014    ldr r0, [fp, #-20]
    4790:   e3a01016    mov r1, #22 ; 0x16
    4794:   e1a02003    mov r2, r3
    4798:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            4798: R_ARM_PC24    MGC_FAPI_WriteReg16
    479c:   e55b3031    ldrb    r3, [fp, #-49]
    47a0:   e3530000    cmp r3, #0  ; 0x0
    47a4:   0a000040    beq 48ac <MGC_HdrcDmaChannelStatusChanged+0x370>
    47a8:   e51b3030    ldr r3, [fp, #-48]
    47ac:   e5933048    ldr r3, [r3, #72]
    47b0:   e3530000    cmp r3, #0  ; 0x0
    47b4:   0a000009    beq 47e0 <MGC_HdrcDmaChannelStatusChanged+0x2a4>
    47b8:   e51b3020    ldr r3, [fp, #-32]
    47bc:   e5933214    ldr r3, [r3, #532]
    47c0:   e5932010    ldr r2, [r3, #16]
    47c4:   e51b3030    ldr r3, [fp, #-48]
    47c8:   e5933048    ldr r3, [r3, #72]
    47cc:   e1a00003    mov r0, r3
    47d0:   e12fff32    blx r2
    47d4:   e51b2030    ldr r2, [fp, #-48]
    47d8:   e3a03000    mov r3, #0  ; 0x0
    47dc:   e5823048    str r3, [r2, #72]
    47e0:   e51b3024    ldr r3, [fp, #-36]
    47e4:   e6ef3073    uxtb    r3, r3
    47e8:   e1a0c003    mov ip, r3
    47ec:   e51b3030    ldr r3, [fp, #-48]
    47f0:   e5933030    ldr r3, [r3, #48]
    47f4:   e24b203c    sub r2, fp, #60 ; 0x3c
    47f8:   e58d3000    str r3, [sp]
    47fc:   e1a00002    mov r0, r2
    4800:   e51b1030    ldr r1, [fp, #-48]
    4804:   e1a0200c    mov r2, ip
    4808:   e3a03000    mov r3, #0  ; 0x0
    480c:   ebfffffe    bl  0 <MGC_CompleteIrp>
            480c: R_ARM_PC24    MGC_CompleteIrp
    4810:   e1a03000    mov r3, r0
    4814:   e54b3031    strb    r3, [fp, #-49]
    4818:   e15b32ba    ldrh    r3, [fp, #-42]
    481c:   e3c33b0a    bic r3, r3, #10240  ; 0x2800
    4820:   e1a03443    asr r3, r3, #8
    4824:   e6ef3073    uxtb    r3, r3
    4828:   e51b0014    ldr r0, [fp, #-20]
    482c:   e3a01017    mov r1, #23 ; 0x17
    4830:   e1a02003    mov r2, r3
    4834:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            4834: R_ARM_PC24    MGC_FAPI_WriteReg8
    4838:   e51b0014    ldr r0, [fp, #-20]
    483c:   e3a01008    mov r1, #8  ; 0x8
    4840:   ebfffffe    bl  0 <MGC_FAPI_ReadReg16>
            4840: R_ARM_PC24    MGC_FAPI_ReadReg16
    4844:   e1a03000    mov r3, r0
    4848:   e14b32b8    strh    r3, [fp, #-40]
    484c:   e55b2044    ldrb    r2, [fp, #-68]
    4850:   e3a03001    mov r3, #1  ; 0x1
    4854:   e1a03213    lsl r3, r3, r2
    4858:   e6ff3073    uxth    r3, r3
    485c:   e15b22b8    ldrh    r2, [fp, #-40]
    4860:   e1a01003    mov r1, r3
    4864:   e1a03002    mov r3, r2
    4868:   e1813003    orr r3, r1, r3
    486c:   e6ff3073    uxth    r3, r3
    4870:   e6ff3073    uxth    r3, r3
    4874:   e51b0014    ldr r0, [fp, #-20]
    4878:   e3a01008    mov r1, #8  ; 0x8
    487c:   e1a02003    mov r2, r3
    4880:   ebfffffe    bl  0 <MGC_FAPI_WriteReg16>
            4880: R_ARM_PC24    MGC_FAPI_WriteReg16
    4884:   e55b3044    ldrb    r3, [fp, #-68]
    4888:   e54b3037    strb    r3, [fp, #-55]
    488c:   e51b3018    ldr r3, [fp, #-24]
    4890:   e593c00c    ldr ip, [r3, #12]
    4894:   e51b3018    ldr r3, [fp, #-24]
    4898:   e5933004    ldr r3, [r3, #4]
    489c:   e24b203c    sub r2, fp, #60 ; 0x3c
    48a0:   e1a00003    mov r0, r3
    48a4:   e1a01002    mov r1, r2
    48a8:   e12fff3c    blx ip
    48ac:   e51b3020    ldr r3, [fp, #-32]
    48b0:   e55b2031    ldrb    r2, [fp, #-49]
    48b4:   e5c32044    strb    r2, [r3, #68]
    48b8:   e55b300d    ldrb    r3, [fp, #-13]
    48bc:   e51b0014    ldr r0, [fp, #-20]
    48c0:   e3a0100e    mov r1, #14 ; 0xe
    48c4:   e1a02003    mov r2, r3
    48c8:   ebfffffe    bl  0 <MGC_FAPI_WriteReg8>
            48c8: R_ARM_PC24    MGC_FAPI_WriteReg8
    48cc:   e55b3031    ldrb    r3, [fp, #-49]
    48d0:   e50b304c    str r3, [fp, #-76]
    48d4:   e51b304c    ldr r3, [fp, #-76]
    48d8:   e1a00003    mov r0, r3
    48dc:   e24bd00c    sub sp, fp, #12 ; 0xc
    48e0:   e89da800    ldm sp, {fp, sp, pc}

000048e4 <MGC_HdrcDumpState>:
    48e4:   e1a0c00d    mov ip, sp
    48e8:   e92dd800    push    {fp, ip, lr, pc}
    48ec:   e24cb004    sub fp, ip, #4  ; 0x4
    48f0:   e24dd004    sub sp, sp, #4  ; 0x4
    48f4:   e50b0010    str r0, [fp, #-16]
    48f8:   e3a03000    mov r3, #0  ; 0x0
    48fc:   e1a00003    mov r0, r3
    4900:   e99da800    ldmib   sp, {fp, sp, pc}

00004904 <MGC_HdrcDumpEndpoint>:
    4904:   e1a0c00d    mov ip, sp
    4908:   e92dd800    push    {fp, ip, lr, pc}
    490c:   e24cb004    sub fp, ip, #4  ; 0x4
    4910:   e24dd008    sub sp, sp, #8  ; 0x8
    4914:   e50b0010    str r0, [fp, #-16]
    4918:   e50b1014    str r1, [fp, #-20]
    491c:   e3a03000    mov r3, #0  ; 0x0
    4920:   e1a00003    mov r0, r3
    4924:   e24bd00c    sub sp, fp, #12 ; 0xc
    4928:   e89da800    ldm sp, {fp, sp, pc}
Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:   25011101    strcs   r1, [r1, #-257]
   4:   030b1308    movweq  r1, #45832  ; 0xb308
   8:   11081b08    tstne   r8, r8, lsl #22
   c:   10011201    andne   r1, r1, r1, lsl #4
  10:   02000006    andeq   r0, r0, #6  ; 0x6
  14:   08030016    stmdaeq r3, {r1, r2, r4}
  18:   0b3b0b3a    bleq    ec2d08 <MGC_FAPI_WriteReg8+0xec2d08>
  1c:   00001349    andeq   r1, r0, r9, asr #6
  20:   0b002403    bleq    9034 <MGC_FAPI_WriteReg8+0x9034>
  24:   030b3e0b    movweq  r3, #48651  ; 0xbe0b
  28:   04000008    streq   r0, [r0], #-8
  2c:   0b0b0024    bleq    2c00c4 <MGC_FAPI_WriteReg8+0x2c00c4>
  30:   00000b3e    andeq   r0, r0, lr, lsr fp
  34:   0b000f05    bleq    3c50 <MGC_FAPI_WriteReg8+0x3c50>
  38:   0600000b    streq   r0, [r0], -fp
  3c:   0c270015    stceq   0, cr0, [r7], #-84
  40:   0f070000    svceq   0x00070000
  44:   490b0b00    stmdbmi fp, {r8, r9, fp}
  48:   08000013    stmdaeq r0, {r0, r1, r4}
  4c:   13490026    movtne  r0, #36902  ; 0x9026
  50:   15090000    strne   r0, [r9]
  54:   490c2701    stmdbmi ip, {r0, r8, r9, sl, sp}
  58:   00130113    andseq  r0, r3, r3, lsl r1
  5c:   00050a00    andeq   r0, r5, r0, lsl #20
  60:   00001349    andeq   r1, r0, r9, asr #6
  64:   2701150b    strcs   r1, [r1, -fp, lsl #10]
  68:   0013010c    andseq  r0, r3, ip, lsl #2
  6c:   01130c00    tsteq   r3, r0, lsl #24
  70:   0b3a0b0b    bleq    e82ca4 <MGC_FAPI_WriteReg8+0xe82ca4>
  74:   13010b3b    movwne  r0, #6971   ; 0x1b3b
  78:   0d0d0000    stceq   0, cr0, [sp]
  7c:   3a0e0300    bcc 380c84 <MGC_FAPI_WriteReg8+0x380c84>
  80:   490b3b0b    stmdbmi fp, {r0, r1, r3, r8, r9, fp, ip, sp}
  84:   000a3813    andeq   r3, sl, r3, lsl r8
  88:   000d0e00    andeq   r0, sp, r0, lsl #28
  8c:   0b3a0803    bleq    e820a0 <MGC_FAPI_WriteReg8+0xe820a0>
  90:   13490b3b    movtne  r0, #39739  ; 0x9b3b
  94:   00000a38    andeq   r0, r0, r8, lsr sl
  98:   2700150f    strcs   r1, [r0, -pc, lsl #10]
  9c:   0013490c    andseq  r4, r3, ip, lsl #18
  a0:   00261000    eoreq   r1, r6, r0
  a4:   16110000    ldrne   r0, [r1], -r0
  a8:   3a080300    bcc 200cb0 <MGC_FAPI_WriteReg8+0x200cb0>
  ac:   49053b0b    stmdbmi r5, {r0, r1, r3, r8, r9, fp, ip, sp}
  b0:   12000013    andne   r0, r0, #19 ; 0x13
  b4:   0b0b0113    bleq    2c0508 <MGC_FAPI_WriteReg8+0x2c0508>
  b8:   053b0b3a    ldreq   r0, [fp, #-2874]!
  bc:   00001301    andeq   r1, r0, r1, lsl #6
  c0:   03000d13    movweq  r0, #3347   ; 0xd13
  c4:   3b0b3a0e    blcc    2ce904 <MGC_FAPI_WriteReg8+0x2ce904>
  c8:   38134905    ldmdacc r3, {r0, r2, r8, fp, lr}
  cc:   1400000a    strne   r0, [r0], #-10
  d0:   0803000d    stmdaeq r3, {r0, r2, r3}
  d4:   053b0b3a    ldreq   r0, [fp, #-2874]!
  d8:   0a381349    beq e04e04 <MGC_FAPI_WriteReg8+0xe04e04>
  dc:   04150000    ldreq   r0, [r5]
  e0:   3a0b0b01    bcc 2c2cec <MGC_FAPI_WriteReg8+0x2c2cec>
  e4:   010b3b0b    tsteq   fp, fp, lsl #22
  e8:   16000013    undefined
  ec:   08030028    stmdaeq r3, {r3, r5}
  f0:   00000d1c    andeq   r0, r0, ip, lsl sp
  f4:   03011317    movweq  r1, #4887   ; 0x1317
  f8:   3a0b0b08    bcc 2c2d20 <MGC_FAPI_WriteReg8+0x2c2d20>
  fc:   010b3b0b    tsteq   fp, fp, lsl #22
 100:   18000013    stmdane r0, {r0, r1, r4}
 104:   08030113    stmdaeq r3, {r0, r1, r4, r8}
 108:   0b3a0b0b    bleq    e82d3c <MGC_FAPI_WriteReg8+0xe82d3c>
 10c:   1301053b    movwne  r0, #5435   ; 0x153b
 110:   04190000    ldreq   r0, [r9]
 114:   0b080301    bleq    200d20 <MGC_FAPI_WriteReg8+0x200d20>
 118:   3b0b3a0b    blcc    2ce94c <MGC_FAPI_WriteReg8+0x2ce94c>
 11c:   0013010b    andseq  r0, r3, fp, lsl #2
 120:   01131a00    tsteq   r3, r0, lsl #20
 124:   050b0803    streq   r0, [fp, #-2051]
 128:   0b3b0b3a    bleq    ec2e18 <MGC_FAPI_WriteReg8+0xec2e18>
 12c:   00001301    andeq   r1, r0, r1, lsl #6
 130:   0b01131b    bleq    44da4 <MGC_FAPI_WriteReg8+0x44da4>
 134:   3b0b3a05    blcc    2ce950 <MGC_FAPI_WriteReg8+0x2ce950>
 138:   00130105    andseq  r0, r3, r5, lsl #2
 13c:   01011c00    tsteq   r1, r0, lsl #24
 140:   13011349    movwne  r1, #4937   ; 0x1349
 144:   211d0000    tstcs   sp, r0
 148:   2f134900    svccs   0x00134900
 14c:   1e00000b    cdpne   0, 0, cr0, cr0, cr11, {0}
 150:   00000018    andeq   r0, r0, r8, lsl r0
 154:   3f012e1f    svccc   0x00012e1f
 158:   3a08030c    bcc 200d90 <MGC_FAPI_WriteReg8+0x200d90>
 15c:   270b3b0b    strcs   r3, [fp, -fp, lsl #22]
 160:   1113490c    tstne   r3, ip, lsl #18
 164:   40011201    andmi   r1, r1, r1, lsl #4
 168:   00130106    andseq  r0, r3, r6, lsl #2
 16c:   00052000    andeq   r2, r5, r0
 170:   0b3a0e03    bleq    e83984 <MGC_FAPI_WriteReg8+0xe83984>
 174:   13490b3b    movtne  r0, #39739  ; 0x9b3b
 178:   00000a02    andeq   r0, r0, r2, lsl #20
 17c:   03003421    movweq  r3, #1057   ; 0x421
 180:   3b0b3a08    blcc    2ce9a8 <MGC_FAPI_WriteReg8+0x2ce9a8>
 184:   0213490b    andseq  r4, r3, #180224 ; 0x2c000
 188:   2200000a    andcs   r0, r0, #10 ; 0xa
 18c:   0e030034    mcreq   0, 0, r0, cr3, cr4, {1}
 190:   0b3b0b3a    bleq    ec2e80 <MGC_FAPI_WriteReg8+0xec2e80>
 194:   0a021349    beq 84ec0 <MGC_FAPI_WriteReg8+0x84ec0>
 198:   2e230000    cdpcs   0, 2, cr0, cr3, cr0, {0}
 19c:   030c3f01    movweq  r3, #52993  ; 0xcf01
 1a0:   3b0b3a08    blcc    2ce9c8 <MGC_FAPI_WriteReg8+0x2ce9c8>
 1a4:   110c270b    tstne   ip, fp, lsl #14
 1a8:   40011201    andmi   r1, r1, r1, lsl #4
 1ac:   00130106    andseq  r0, r3, r6, lsl #2
 1b0:   00052400    andeq   r2, r5, r0, lsl #8
 1b4:   0b3a0803    bleq    e821c8 <MGC_FAPI_WriteReg8+0xe821c8>
 1b8:   13490b3b    movtne  r0, #39739  ; 0x9b3b
 1bc:   00000a02    andeq   r0, r0, r2, lsl #20
 1c0:   3f012e25    svccc   0x00012e25
 1c4:   3a08030c    bcc 200dfc <MGC_FAPI_WriteReg8+0x200dfc>
 1c8:   27053b0b    strcs   r3, [r5, -fp, lsl #22]
 1cc:   1113490c    tstne   r3, ip, lsl #18
 1d0:   40011201    andmi   r1, r1, r1, lsl #4
 1d4:   00130106    andseq  r0, r3, r6, lsl #2
 1d8:   00052600    andeq   r2, r5, r0, lsl #12
 1dc:   0b3a0e03    bleq    e839f0 <MGC_FAPI_WriteReg8+0xe839f0>
 1e0:   1349053b    movtne  r0, #38203  ; 0x953b
 1e4:   00000a02    andeq   r0, r0, r2, lsl #20
 1e8:   03003427    movweq  r3, #1063   ; 0x427
 1ec:   3b0b3a08    blcc    2cea14 <MGC_FAPI_WriteReg8+0x2cea14>
 1f0:   02134905    andseq  r4, r3, #81920  ; 0x14000
 1f4:   2800000a    stmdacs r0, {r1, r3}
 1f8:   0e030034    mcreq   0, 0, r0, cr3, cr4, {1}
 1fc:   053b0b3a    ldreq   r0, [fp, #-2874]!
 200:   0a021349    beq 84f2c <MGC_FAPI_WriteReg8+0x84f2c>
 204:   05290000    streq   r0, [r9]!
 208:   3a080300    bcc 200e10 <MGC_FAPI_WriteReg8+0x200e10>
 20c:   49053b0b    stmdbmi r5, {r0, r1, r3, r8, r9, fp, ip, sp}
 210:   000a0213    andeq   r0, sl, r3, lsl r2
 214:   012e2a00    teqeq   lr, r0, lsl #20
 218:   0b3a0803    bleq    e8222c <MGC_FAPI_WriteReg8+0xe8222c>
 21c:   0c27053b    cfstr32eq   mvfx0, [r7], #-236
 220:   01120111    tsteq   r2, r1, lsl r1
 224:   13010640    movwne  r0, #5696   ; 0x1640
 228:   2e2b0000    cdpcs   0, 2, cr0, cr11, cr0, {0}
 22c:   030c3f01    movweq  r3, #52993  ; 0xcf01
 230:   3b0b3a08    blcc    2cea58 <MGC_FAPI_WriteReg8+0x2cea58>
 234:   110c2705    tstne   ip, r5, lsl #14
 238:   40011201    andmi   r1, r1, r1, lsl #4
 23c:   00130106    andseq  r0, r3, r6, lsl #2
 240:   00342c00    eorseq  r2, r4, r0, lsl #24
 244:   0b3a0803    bleq    e82258 <MGC_FAPI_WriteReg8+0xe82258>
 248:   13490b3b    movtne  r0, #39739  ; 0x9b3b
 24c:   0c3c0c3f    ldceq   12, cr0, [ip], #-252
 250:   342d0000    strtcc  r0, [sp]
 254:   3a080300    bcc 200e5c <MGC_FAPI_WriteReg8+0x200e5c>
 258:   49053b0b    stmdbmi r5, {r0, r1, r3, r8, r9, fp, ip, sp}
 25c:   3c0c3f13    stccc   15, cr3, [ip], {19}
 260:   0000000c    andeq   r0, r0, ip
Disassembly of section .debug_info:

00000000 <.debug_info>:
       0:   000052e2    andeq   r5, r0, r2, ror #5
       4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_abbrev
       8:   01040000    tsteq   r4, r0
       c:   20554e47    subscs  r4, r5, r7, asr #28
      10:   2e342043    cdpcs   0, 3, cr2, cr4, cr3, {2}
      14:   00322e32    eorseq  r2, r2, r2, lsr lr
      18:   2f2e2e01    svccs   0x002e2e01
      1c:   6d2f6463    cfstrsvs    mvf6, [pc, #-396]!
      20:   64685f75    strbtvs r5, [r8], #-3957
      24:   00632e72    rsbeq   r2, r3, r2, ror lr
      28:   6779632f    ldrbvs  r6, [r9, -pc, lsr #6]!
      2c:   76697264    strbtvc r7, [r9], -r4, ror #4
      30:   2f642f65    svccs   0x00642f65
      34:   4a4f5250    bmi 13d497c <MGC_HdrcDumpEndpoint+0x13d0078>
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
      7c:   492c0000    stmdbmi ip!, {}
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
     120:   a9010900    stmdbge r1, {r8, fp}
     124:   31000000    tstcc   r0, r0
     128:   0a000001    beq 134 <.debug_info+0x134>
     12c:   000000f1    strdeq  r0, [r0], -r1
     130:   21040700    tstcs   r4, r0, lsl #14
     134:   03000001    movweq  r0, #1  ; 0x1
     138:   68730702    ldmdavs r3!, {r1, r8, r9, sl}^
     13c:   2074726f    rsbscs  r7, r4, pc, ror #4
     140:   69736e75    ldmdbvs r3!, {r0, r2, r4, r5, r6, r9, sl, fp, sp, lr}^
     144:   64656e67    strbtvs r6, [r5], #-3687
     148:   746e6920    strbtvc r6, [lr], #-2336
     14c:   07080300    streq   r0, [r8, -r0, lsl #6]
     150:   676e6f6c    strbvs  r6, [lr, -ip, ror #30]!
     154:   6e6f6c20    cdpvs   12, 6, cr6, cr15, cr0, {1}
     158:   6e752067    cdpvs   0, 7, cr2, cr5, cr7, {3}
     15c:   6e676973    mcrvs   9, 3, r6, cr7, cr3, {3}
     160:   69206465    stmdbvs r0!, {r0, r2, r5, r6, sl, sp, lr}
     164:   0300746e    movweq  r7, #1134   ; 0x46e
     168:   69730601    ldmdbvs r3!, {r0, r9, sl}^
     16c:   64656e67    strbtvs r6, [r5], #-3687
     170:   61686320    cmnvs   r8, r0, lsr #6
     174:   75020072    strvc   r0, [r2, #-114]
     178:   38746e69    ldmdacc r4!, {r0, r3, r5, r6, r9, sl, fp, sp, lr}^
     17c:   0300745f    movweq  r7, #1119   ; 0x45f
     180:   0000e02e    andeq   lr, r0, lr, lsr #32
     184:   69750200    ldmdbvs r5!, {r9}^
     188:   3631746e    ldrtcc  r7, [r1], -lr, ror #8
     18c:   0300745f    movweq  r7, #1119   ; 0x45f
     190:   0001373a    andeq   r3, r1, sl, lsr r7
     194:   6e690200    cdpvs   2, 6, cr0, cr9, cr0, {0}
     198:   5f323374    svcpl   0x00323374
     19c:   53030074    movwpl  r0, #12404  ; 0x3074
     1a0:   000000b0    strheq  r0, [r0], -r0
     1a4:   6e697502    cdpvs   5, 6, cr7, cr9, cr2, {0}
     1a8:   5f323374    svcpl   0x00323374
     1ac:   54030074    strpl   r0, [r3], #-116
     1b0:   00000094    muleq   r0, r4, r0
     1b4:   6e697502    cdpvs   5, 6, cr7, cr9, cr2, {0}
     1b8:   5f343674    svcpl   0x00343674
     1bc:   7c030074    stcvc   0, cr0, [r3], {116}
     1c0:   0000014d    andeq   r0, r0, sp, asr #2
     1c4:   6e697502    cdpvs   5, 6, cr7, cr9, cr2, {0}
     1c8:   61665f74    smcvs   26100
     1cc:   36317473    undefined
     1d0:   0300745f    movweq  r7, #1119   ; 0x45f
     1d4:   0000cdaa    andeq   ip, r0, sl, lsr #27
     1d8:   554d0200    strbpl  r0, [sp, #-512]
     1dc:   705f4253    subsvc  r4, pc, r3, asr r2
     1e0:   6e6f4366    cdpvs   3, 6, cr4, cr15, cr6, {3}
     1e4:   6c6f7274    sfmvs   f7, 2, [pc], #-464
     1e8:   4972656c    ldmdbmi r2!, {r2, r3, r5, r6, r8, sl, sp, lr}^
     1ec:   04007273    streq   r7, [r0], #-627
     1f0:   00013140    andeq   r3, r1, r0, asr #2
     1f4:   554d0200    strbpl  r0, [sp, #-512]
     1f8:   705f4253    subsvc  r4, pc, r3, asr r2
     1fc:   6e6f4366    cdpvs   3, 6, cr4, cr15, cr6, {3}
     200:   6c6f7274    sfmvs   f7, 2, [pc], #-464
     204:   4272656c    rsbsmi  r6, r2, #452984832  ; 0x1b000000
     208:   04007273    streq   r7, [r0], #-627
     20c:   0002114b    andeq   r1, r2, fp, asr #2
     210:   17040700    strne   r0, [r4, -r0, lsl #14]
     214:   0b000002    bleq    224 <.debug_info+0x224>
     218:   00022301    andeq   r2, r2, r1, lsl #6
     21c:   00f10a00    rscseq  r0, r1, r0, lsl #20
     220:   02000000    andeq   r0, r0, #0  ; 0x0
     224:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     228:   5466705f    strbtpl r7, [r6], #-95
     22c:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
     230:   69707845    ldmdbvs r0!, {r0, r2, r6, fp, ip, sp, lr}^
     234:   00646572    rsbeq   r6, r4, r2, ror r5
     238:   023e5804    eorseq  r5, lr, #262144 ; 0x40000
     23c:   04070000    streq   r0, [r7]
     240:   00000244    andeq   r0, r0, r4, asr #4
     244:   0255010b    subseq  r0, r5, #-1073741822    ; 0xc0000002
     248:   f10a0000    cpsie   ,#0
     24c:   0a000000    beq 254 <.debug_info+0x254>
     250:   00000185    andeq   r0, r0, r5, lsl #3
     254:   04280c00    strteq  r0, [r8], #-3072
     258:   00032b7e    andeq   r2, r3, lr, ror fp
     25c:   00900d00    addseq  r0, r0, r0, lsl #26
            25e: R_ARM_ABS32    .debug_str
     260:   7f040000    svcvc   0x00040000
     264:   00000185    andeq   r0, r0, r5, lsl #3
     268:   0d002302    stceq   3, cr2, [r0, #-8]
     26c:   000000ee    andeq   r0, r0, lr, ror #1
            26c: R_ARM_ABS32    .debug_str
     270:   00f18004    rscseq  r8, r1, r4
     274:   23020000    movwcs  r0, #8192   ; 0x2000
     278:   51770e04    cmnpl   r7, r4, lsl #28
     27c:   65756575    ldrbvs  r6, [r5, #-1397]!
     280:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
     284:   04006874    streq   r6, [r0], #-2164
     288:   00018581    andeq   r8, r1, r1, lsl #11
     28c:   08230200    stmdaeq r3!, {r9}
     290:   7551770e    ldrbvc  r7, [r1, #-1806]
     294:   49657565    stmdbmi r5!, {r0, r2, r5, r6, r8, sl, ip, sp, lr}^
     298:   536d6574    cmnpl   sp, #486539264  ; 0x1d000000
     29c:   00657a69    rsbeq   r7, r5, r9, ror #20
     2a0:   01858204    orreq   r8, r5, r4, lsl #4
     2a4:   23020000    movwcs  r0, #8192   ; 0x2000
     2a8:   54770e0a    ldrbtpl r0, [r7], #-3594
     2ac:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
     2b0:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
     2b4:   83040074    movwhi  r0, #16500  ; 0x4074
     2b8:   00000185    andeq   r0, r0, r5, lsl #3
     2bc:   0d0c2302    stceq   3, cr2, [ip, #-8]
     2c0:   0000015c    andeq   r0, r0, ip, asr r1
            2c0: R_ARM_ABS32    .debug_str
     2c4:   032b8404    teqeq   fp, #67108864   ; 0x4000000
     2c8:   23020000    movwcs  r0, #8192   ; 0x2000
     2cc:   4c770e10    ldclmi  14, cr0, [r7], #-64
     2d0:   436b636f    cmnmi   fp, #-1140850687    ; 0xbc000001
     2d4:   746e756f    strbtvc r7, [lr], #-1391
     2d8:   85850400    strhi   r0, [r5, #1024]
     2dc:   02000001    andeq   r0, r0, #1  ; 0x1
     2e0:   700e1423    andvc   r1, lr, r3, lsr #8
     2e4:   72734966    rsbsvc  r4, r3, #1671168    ; 0x198000
     2e8:   d9860400    stmible r6, {sl}
     2ec:   02000001    andeq   r0, r0, #1  ; 0x1
     2f0:   700e1823    andvc   r1, lr, r3, lsr #16
     2f4:   50727349    rsbspl  r7, r2, r9, asr #6
     2f8:   6d617261    sfmvs   f7, 2, [r1, #-388]!
     2fc:   f1870400    undefined instruction 0xf1870400
     300:   02000000    andeq   r0, r0, #0  ; 0x0
     304:   700e1c23    andvc   r1, lr, r3, lsr #24
     308:   72734266    rsbsvc  r4, r3, #1610612742 ; 0x60000006
     30c:   f5880400    undefined instruction 0xf5880400
     310:   02000001    andeq   r0, r0, #1  ; 0x1
     314:   700e2023    andvc   r2, lr, r3, lsr #32
     318:   50727342    rsbspl  r7, r2, r2, asr #6
     31c:   6d617261    sfmvs   f7, 2, [r1, #-388]!
     320:   f1890400    undefined instruction 0xf1890400
     324:   02000000    andeq   r0, r0, #0  ; 0x0
     328:   07002423    streq   r2, [r0, -r3, lsr #8]
     32c:   00033104    andeq   r3, r3, r4, lsl #2
     330:   01a40800    undefined instruction 0x01a40800
     334:   4d020000    stcmi   0, cr0, [r2]
     338:   5f425355    svcpl   0x00425355
     33c:   746e6f43    strbtvc r6, [lr], #-3907
     340:   6c6c6f72    stclvs  15, cr6, [ip], #-456
     344:   04007265    streq   r7, [r0], #-613
     348:   0002558a    andeq   r5, r2, sl, lsl #11
     34c:   554d0200    strbpl  r0, [sp, #-512]
     350:   705f4253    subsvc  r4, pc, r3, asr r2
     354:   73654d66    cmnvc   r5, #6528   ; 0x1980
     358:   65676173    strbvs  r6, [r7, #-371]!
     35c:   69727453    ldmdbvs r2!, {r0, r1, r4, r6, sl, ip, sp, lr}^
     360:   0400676e    streq   r6, [r0], #-1902
     364:   0003699a    muleq   r3, sl, r9
     368:   6f040700    svcvs   0x00040700
     36c:   09000003    stmdbeq r0, {r0, r1}
     370:   00017601    andeq   r7, r1, r1, lsl #12
     374:   00038900    andeq   r8, r3, r0, lsl #18
     378:   01080a00    tsteq   r8, r0, lsl #20
     37c:   850a0000    strhi   r0, [sl]
     380:   0a000001    beq 38c <.debug_info+0x38c>
     384:   00000116    andeq   r0, r0, r6, lsl r1
     388:   554d0200    strbpl  r0, [sp, #-512]
     38c:   705f4253    subsvc  r4, pc, r3, asr r2
     390:   73654d66    cmnvc   r5, #6528   ; 0x1980
     394:   65676173    strbvs  r6, [r7, #-371]!
     398:   626d754e    rsbvs   r7, sp, #327155712  ; 0x13800000
     39c:   04007265    streq   r7, [r0], #-613
     3a0:   0003a5ae    andeq   sl, r3, lr, lsr #11
     3a4:   ab040700    blge    101fac <MGC_HdrcDumpEndpoint+0xfd6a8>
     3a8:   09000003    stmdbeq r0, {r0, r1}
     3ac:   00017601    andeq   r7, r1, r1, lsl #12
     3b0:   0003cf00    andeq   ip, r3, r0, lsl #30
     3b4:   01080a00    tsteq   r8, r0, lsl #20
     3b8:   850a0000    strhi   r0, [sl]
     3bc:   0a000001    beq 3c8 <.debug_info+0x3c8>
     3c0:   000001a4    andeq   r0, r0, r4, lsr #3
     3c4:   0001760a    andeq   r7, r1, sl, lsl #12
     3c8:   01760a00    cmneq   r6, r0, lsl #20
     3cc:   02000000    andeq   r0, r0, #0  ; 0x0
     3d0:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     3d4:   4766705f    undefined
     3d8:   69547465    ldmdbvs r4, {r0, r2, r5, r6, sl, ip, sp, lr}^
     3dc:   0400656d    streq   r6, [r0], #-1389
     3e0:   0003e5b8    strheq  lr, [r3], -r8
     3e4:   eb040700    bl  101fec <MGC_HdrcDumpEndpoint+0xfd6e8>
     3e8:   0f000003    svceq   0x00000003
     3ec:   0001a401    andeq   sl, r1, r1, lsl #8
     3f0:   04100c00    ldreq   r0, [r0], #-3072
     3f4:   000450d0    ldrdeq  r5, [r4], -r0
     3f8:   00900d00    addseq  r0, r0, r0, lsl #26
            3fa: R_ARM_ABS32    .debug_str
     3fc:   d1040000    tstle   r4, r0
     400:   00000185    andeq   r0, r0, r5, lsl #3
     404:   0e002302    cdpeq   3, 0, cr2, cr0, cr2, {0}
     408:   654d6670    strbvs  r6, [sp, #-1648]
     40c:   67617373    undefined
     410:   72745365    rsbsvc  r5, r4, #-1811939327    ; 0x94000001
     414:   00676e69    rsbeq   r6, r7, r9, ror #28
     418:   034dd204    movteq  sp, #53764  ; 0xd204
     41c:   23020000    movwcs  r0, #8192   ; 0x2000
     420:   66700e04    ldrbtvs r0, [r0], -r4, lsl #28
     424:   7373654d    cmnvc   r3, #322961408  ; 0x13400000
     428:   4e656761    cdpmi   7, 6, cr6, cr5, cr1, {3}
     42c:   65626d75    strbvs  r6, [r2, #-3445]!
     430:   d3040072    movwle  r0, #16498  ; 0x4072
     434:   00000389    andeq   r0, r0, r9, lsl #7
     438:   0e082302    cdpeq   3, 0, cr2, cr8, cr2, {0}
     43c:   65476670    strbvs  r6, [r7, #-1648]
     440:   6d695474    cfstrdvs    mvd5, [r9, #-464]!
     444:   d4040065    strle   r0, [r4], #-101
     448:   000003cf    andeq   r0, r0, pc, asr #7
     44c:   000c2302    andeq   r2, ip, r2, lsl #6
     450:   53554d02    cmppl   r5, #128    ; 0x80
     454:   79535f42    ldmdbvc r3, {r1, r6, r8, r9, sl, fp, ip, lr}^
     458:   6d657473    cfstrdvs    mvd7, [r5, #-460]!
     45c:   6c697455    cfstrdvs    mvd7, [r9], #-340
     460:   d5040073    strle   r0, [r4, #-115]
     464:   000003f1    strdeq  r0, [r0], -r1
     468:   53554d02    cmppl   r5, #128    ; 0x80
     46c:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
     470:   74737953    ldrbtvc r7, [r3], #-2387
     474:   6f546d65    svcvs   0x00546d65
     478:   41737542    cmnmi   r3, r2, asr #10
     47c:   65726464    ldrbvs  r6, [r2, #-1124]!
     480:   04007373    streq   r7, [r0], #-883
     484:   000489e1    andeq   r8, r4, r1, ror #19
     488:   8f040700    svchi   0x00040700
     48c:   09000004    stmdbeq r0, {r2}
     490:   0000f101    andeq   pc, r0, r1, lsl #2
     494:   0004a400    andeq   sl, r4, r0, lsl #8
     498:   00f10a00    rscseq  r0, r1, r0, lsl #20
     49c:   a40a0000    strge   r0, [sl]
     4a0:   00000004    andeq   r0, r0, r4
     4a4:   04aa0407    strteq  r0, [sl], #1031
     4a8:   02100000    andseq  r0, r0, #0  ; 0x0
     4ac:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     4b0:   5166705f    qdsubpl r7, pc, r6
     4b4:   65756575    ldrbvs  r6, [r5, #-1397]!
     4b8:   6b636142    blvs    18d89c8 <MGC_HdrcDumpEndpoint+0x18d40c4>
     4bc:   756f7267    strbvc  r7, [pc, #-615]!    ; 25d <.debug_info+0x25d>
     4c0:   7449646e    strbvc  r6, [r9], #-1134
     4c4:   04006d65    streq   r6, [r0], #-3429
     4c8:   0004cdee    andeq   ip, r4, lr, ror #27
     4cc:   d3040700    movwle  r0, #18176  ; 0x4700
     4d0:   09000004    stmdbeq r0, {r2}
     4d4:   00017601    andeq   r7, r1, r1, lsl #12
     4d8:   0004e800    andeq   lr, r4, r0, lsl #16
     4dc:   00f10a00    rscseq  r0, r1, r0, lsl #20
     4e0:   a40a0000    strge   r0, [sl]
     4e4:   00000004    andeq   r0, r0, r4
     4e8:   53554d02    cmppl   r5, #128    ; 0x80
     4ec:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
     4f0:   75716544    ldrbvc  r6, [r1, #-1348]!
     4f4:   42657565    rsbmi   r7, r5, #423624704  ; 0x19400000
     4f8:   676b6361    strbvs  r6, [fp, -r1, ror #6]!
     4fc:   6e756f72    mrcvs   15, 3, r6, cr5, cr2, {3}
     500:   65744964    ldrbvs  r4, [r4, #-2404]!
     504:   fb04006d    blx 1006c2 <MGC_HdrcDumpEndpoint+0xfbdbe>
     508:   0000050c    andeq   r0, r0, ip, lsl #10
     50c:   05120407    ldreq   r0, [r2, #-1031]
     510:   01090000    tsteq   r9, r0
     514:   00000176    andeq   r0, r0, r6, ror r1
     518:   00000527    andeq   r0, r0, r7, lsr #10
     51c:   0000f10a    andeq   pc, r0, sl, lsl #2
     520:   00f10a00    rscseq  r0, r1, r0, lsl #20
     524:   11000000    tstne   r0, r0
     528:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     52c:   4666705f    undefined
     530:   6873756c    ldmdavs r3!, {r2, r3, r5, r6, r8, sl, ip, sp, lr}^
     534:   6b636142    blvs    18d8a44 <MGC_HdrcDumpEndpoint+0x18d4140>
     538:   756f7267    strbvc  r7, [pc, #-615]!    ; 2d9 <.debug_info+0x2d9>
     53c:   7551646e    ldrbvc  r6, [r1, #-1134]
     540:   00657565    rsbeq   r7, r5, r5, ror #10
     544:   11010304    tstne   r1, r4, lsl #6
     548:   11000002    tstne   r0, r2
     54c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     550:   4166705f    qdsubmi r7, pc, r6
     554:   69546d72    ldmdbvs r4, {r1, r4, r5, r6, r8, sl, fp, sp, lr}^
     558:   0072656d    rsbseq  r6, r2, sp, ror #10
     55c:   63011204    movwvs  r1, #4612   ; 0x1204
     560:   07000005    streq   r0, [r0, -r5]
     564:   00056904    andeq   r6, r5, r4, lsl #18
     568:   76010900    strvc   r0, [r1], -r0, lsl #18
     56c:   8d000001    stchi   0, cr0, [r0, #-4]
     570:   0a000005    beq 58c <.debug_info+0x58c>
     574:   000000f1    strdeq  r0, [r0], -r1
     578:   0001850a    andeq   r8, r1, sl, lsl #10
     57c:   01a40a00    undefined instruction 0x01a40a00
     580:   760a0000    strvc   r0, [sl], -r0
     584:   0a000001    beq 590 <.debug_info+0x590>
     588:   00000223    andeq   r0, r0, r3, lsr #4
     58c:   554d1100    strbpl  r1, [sp, #-256]
     590:   705f4253    subsvc  r4, pc, r3, asr r2
     594:   6e614366    cdpvs   3, 6, cr4, cr1, cr6, {3}
     598:   546c6563    strbtpl r6, [ip], #-1379
     59c:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
     5a0:   011d0400    tsteq   sp, r0, lsl #8
     5a4:   000005a8    andeq   r0, r0, r8, lsr #11
     5a8:   05ae0407    streq   r0, [lr, #1031]!
     5ac:   01090000    tsteq   r9, r0
     5b0:   00000176    andeq   r0, r0, r6, ror r1
     5b4:   000005c3    andeq   r0, r0, r3, asr #11
     5b8:   0000f10a    andeq   pc, r0, sl, lsl #2
     5bc:   01850a00    orreq   r0, r5, r0, lsl #20
     5c0:   11000000    tstne   r0, r0
     5c4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     5c8:   4c66705f    stclmi  0, cr7, [r6], #-380
     5cc:   006b636f    rsbeq   r6, fp, pc, ror #6
     5d0:   a8012804    stmdage r1, {r2, fp, sp}
     5d4:   11000005    tstne   r0, r5
     5d8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     5dc:   5566705f    strbpl  r7, [r6, #-95]!
     5e0:   636f6c6e    cmnvs   pc, #28160  ; 0x6e00
     5e4:   3304006b    movwcc  r0, #16491  ; 0x406b
     5e8:   0005a801    andeq   sl, r5, r1, lsl #16
     5ec:   554d1100    strbpl  r1, [sp, #-256]
     5f0:   705f4253    subsvc  r4, pc, r3, asr r2
     5f4:   69725066    ldmdbvs r2!, {r1, r2, r5, r6, ip, lr}^
     5f8:   6944746e    stmdbvs r4, {r1, r2, r3, r5, r6, sl, ip, sp, lr}^
     5fc:   04006761    streq   r6, [r0], #-1889
     600:   0606013e    undefined
     604:   04070000    streq   r0, [r7]
     608:   0000060c    andeq   r0, r0, ip, lsl #12
     60c:   01760109    cmneq   r6, r9, lsl #2
     610:   06210000    strteq  r0, [r1], -r0
     614:   f10a0000    cpsie   ,#0
     618:   0a000000    beq 620 <.debug_info+0x620>
     61c:   00000116    andeq   r0, r0, r6, lsl r1
     620:   554d1100    strbpl  r1, [sp, #-256]
     624:   705f4253    subsvc  r4, pc, r3, asr r2
     628:   77654e66    strbvc  r4, [r5, -r6, ror #28]!
     62c:   65776f50    ldrbvs  r6, [r7, #-3920]!
     630:   616f4c72    smcvs   62658
     634:   5a040064    bpl 1007cc <MGC_HdrcDumpEndpoint+0xfbec8>
     638:   00063d01    andeq   r3, r6, r1, lsl #26
     63c:   43040700    movwmi  r0, #18176  ; 0x4700
     640:   09000006    stmdbeq r0, {r1, r2}
     644:   00017601    andeq   r7, r1, r1, lsl #12
     648:   00066700    andeq   r6, r6, r0, lsl #14
     64c:   00f10a00    rscseq  r0, r1, r0, lsl #20
     650:   850a0000    strhi   r0, [sl]
     654:   0a000001    beq 660 <.debug_info+0x660>
     658:   00000176    andeq   r0, r0, r6, ror r1
     65c:   0006670a    andeq   r6, r6, sl, lsl #14
     660:   01760a00    cmneq   r6, r0, lsl #20
     664:   07000000    streq   r0, [r0, -r0]
     668:   00066d04    andeq   r6, r6, r4, lsl #26
     66c:   01760800    cmneq   r6, r0, lsl #16
     670:   4d110000    ldcmi   0, cr0, [r1]
     674:   5f425355    svcpl   0x00425355
     678:   65526670    ldrbvs  r6, [r2, #-1648]
     67c:   65766f6d    ldrbvs  r6, [r6, #-3949]!
     680:   65776f50    ldrbvs  r6, [r7, #-3920]!
     684:   616f4c72    smcvs   62658
     688:   6e040064    cdpvs   0, 0, cr0, cr4, cr4, {3}
     68c:   00063d01    andeq   r3, r6, r1, lsl #26
     690:   04341200    ldrteq  r1, [r4], #-512
     694:   07e20193    undefined
     698:   90130000    andsls  r0, r3, r0
            69b: R_ARM_ABS32    .debug_str
     69c:   04000000    streq   r0, [r0]
     6a0:   01850194    strbeq  r0, [r5, r4]
     6a4:   23020000    movwcs  r0, #8192   ; 0x2000
     6a8:   00ee1300    rsceq   r1, lr, r0, lsl #6
            6aa: R_ARM_ABS32    .debug_str
     6ac:   95040000    strls   r0, [r4]
     6b0:   0000f101    andeq   pc, r0, r1, lsl #2
     6b4:   04230200    strteq  r0, [r3], #-512
     6b8:   53667014    cmnpl   r6, #20 ; 0x14
     6bc:   65747379    ldrbvs  r7, [r4, #-889]!
     6c0:   426f546d    rsbmi   r5, pc, #1828716544 ; 0x6d000000
     6c4:   64417375    strbvs  r7, [r1], #-885
     6c8:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
     6cc:   96040073    undefined
     6d0:   00046801    andeq   r6, r4, r1, lsl #16
     6d4:   08230200    stmdaeq r3!, {r9}
     6d8:   51667014    cmnpl   r6, r4, lsl r0
     6dc:   65756575    ldrbvs  r6, [r5, #-1397]!
     6e0:   6b636142    blvs    18d8bf0 <MGC_HdrcDumpEndpoint+0x18d42ec>
     6e4:   756f7267    strbvc  r7, [pc, #-615]!    ; 485 <.debug_info+0x485>
     6e8:   7449646e    strbvc  r6, [r9], #-1134
     6ec:   04006d65    streq   r6, [r0], #-3429
     6f0:   04ab0197    strteq  r0, [fp], #407
     6f4:   23020000    movwcs  r0, #8192   ; 0x2000
     6f8:   6670140c    ldrbtvs r1, [r0], -ip, lsl #8
     6fc:   75716544    ldrbvc  r6, [r1, #-1348]!
     700:   42657565    rsbmi   r7, r5, #423624704  ; 0x19400000
     704:   676b6361    strbvs  r6, [fp, -r1, ror #6]!
     708:   6e756f72    mrcvs   15, 3, r6, cr5, cr2, {3}
     70c:   65744964    ldrbvs  r4, [r4, #-2404]!
     710:   9804006d    stmdals r4, {r0, r2, r3, r5, r6}
     714:   0004e801    andeq   lr, r4, r1, lsl #16
     718:   10230200    eorne   r0, r3, r0, lsl #4
     71c:   46667014    undefined
     720:   6873756c    ldmdavs r3!, {r2, r3, r5, r6, r8, sl, ip, sp, lr}^
     724:   6b636142    blvs    18d8c34 <MGC_HdrcDumpEndpoint+0x18d4330>
     728:   756f7267    strbvc  r7, [pc, #-615]!    ; 4c9 <.debug_info+0x4c9>
     72c:   7551646e    ldrbvc  r6, [r1, #-1134]
     730:   00657565    rsbeq   r7, r5, r5, ror #10
     734:   27019904    strcs   r9, [r1, -r4, lsl #18]
     738:   02000005    andeq   r0, r0, #5  ; 0x5
     73c:   70141423    andsvc  r1, r4, r3, lsr #8
     740:   6d724166    ldfvse  f4, [r2, #-408]!
     744:   656d6954    strbvs  r6, [sp, #-2388]!
     748:   9a040072    bls 100918 <MGC_HdrcDumpEndpoint+0xfc014>
     74c:   00054b01    andeq   r4, r5, r1, lsl #22
     750:   18230200    stmdane r3!, {r9}
     754:   43667014    cmnmi   r6, #20 ; 0x14
     758:   65636e61    strbvs  r6, [r3, #-3681]!
     75c:   6d69546c    cfstrdvs    mvd5, [r9, #-432]!
     760:   04007265    streq   r7, [r0], #-613
     764:   058d019b    streq   r0, [sp, #411]
     768:   23020000    movwcs  r0, #8192   ; 0x2000
     76c:   6670141c    undefined
     770:   6b636f4c    blvs    18dc4a8 <MGC_HdrcDumpEndpoint+0x18d7ba4>
     774:   019c0400    orrseq  r0, ip, r0, lsl #8
     778:   000005c3    andeq   r0, r0, r3, asr #11
     77c:   14202302    strtne  r2, [r0], #-770
     780:   6e556670    mrcvs   6, 2, r6, cr5, cr0, {3}
     784:   6b636f6c    blvs    18dc53c <MGC_HdrcDumpEndpoint+0x18d7c38>
     788:   019d0400    orrseq  r0, sp, r0, lsl #8
     78c:   000005d7    ldrdeq  r0, [r0], -r7
     790:   14242302    strtne  r2, [r4], #-770
     794:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
     798:   44746e69    ldrbtmi r6, [r4], #-3689
     79c:   00676169    rsbeq   r6, r7, r9, ror #2
     7a0:   ed019e04    stc 14, cr9, [r1, #-16]
     7a4:   02000005    andeq   r0, r0, #5  ; 0x5
     7a8:   70142823    andsvc  r2, r4, r3, lsr #16
     7ac:   77654e66    strbvc  r4, [r5, -r6, ror #28]!
     7b0:   65776f50    ldrbvs  r6, [r7, #-3920]!
     7b4:   616f4c72    smcvs   62658
     7b8:   9f040064    svcls   0x00040064
     7bc:   00062101    andeq   r2, r6, r1, lsl #2
     7c0:   2c230200    sfmcs   f0, 4, [r3]
     7c4:   52667014    rsbpl   r7, r6, #20 ; 0x14
     7c8:   766f6d65    strbtvc r6, [pc], -r5, ror #26
     7cc:   776f5065    strbvc  r5, [pc, -r5, rrx]!
     7d0:   6f4c7265    svcvs   0x004c7265
     7d4:   04006461    streq   r6, [r0], #-1121
     7d8:   067201a0    ldrbteq r0, [r2], -r0, lsr #3
     7dc:   23020000    movwcs  r0, #8192   ; 0x2000
     7e0:   4d110030    ldcmi   0, cr0, [r1, #-192]
     7e4:   5f425355    svcpl   0x00425355
     7e8:   74737953    ldrbtvc r7, [r3], #-2387
     7ec:   65536d65    ldrbvs  r6, [r3, #-3429]
     7f0:   63697672    cmnvs   r9, #119537664  ; 0x7200000
     7f4:   04007365    streq   r7, [r0], #-869
     7f8:   069101a1    ldreq   r0, [r1], r1, lsr #3
     7fc:   140c0000    strne   r0, [ip]
     800:   0934c205    ldmdbeq r4!, {r0, r2, r9, lr, pc}
     804:   170d0000    strne   r0, [sp, -r0]
            807: R_ARM_ABS32    .debug_str
     808:   05000000    streq   r0, [r0]
     80c:   000176c3    andeq   r7, r1, r3, asr #13
     810:   00230200    eoreq   r0, r3, r0, lsl #4
     814:   0000ca0d    andeq   ip, r0, sp, lsl #20
            815: R_ARM_ABS32    .debug_str
     818:   76c40500    strbvc  r0, [r4], r0, lsl #10
     81c:   02000001    andeq   r0, r0, #1  ; 0x1
     820:   620e0123    andvs   r0, lr, #-1073741816    ; 0xc0000008
     824:   53556463    cmppl   r5, #1660944384 ; 0x63000000
     828:   c5050042    strgt   r0, [r5, #-66]
     82c:   00000185    andeq   r0, r0, r5, lsl #3
     830:   0e022302    cdpeq   3, 0, cr2, cr2, cr2, {0}
     834:   76654462    strbtvc r4, [r5], -r2, ror #8
     838:   43656369    cmnmi   r5, #-1543503871    ; 0xa4000001
     83c:   7373616c    cmnvc   r3, #27 ; 0x1b
     840:   76c60500    strbvc  r0, [r6], r0, lsl #10
     844:   02000001    andeq   r0, r0, #1  ; 0x1
     848:   620e0423    andvs   r0, lr, #587202560  ; 0x23000000
     84c:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
     850:   75536563    ldrbvc  r6, [r3, #-1379]
     854:   616c4362    cmnvs   ip, r2, ror #6
     858:   05007373    streq   r7, [r0, #-883]
     85c:   000176c7    andeq   r7, r1, r7, asr #13
     860:   05230200    streq   r0, [r3, #-512]!
     864:   6544620e    strbvs  r6, [r4, #-526]
     868:   65636976    strbvs  r6, [r3, #-2422]!
     86c:   746f7250    strbtvc r7, [pc], #592  ; 874 <.debug_info+0x874>
     870:   6c6f636f    stclvs  3, cr6, [pc], #-444
     874:   76c80500    strbvc  r0, [r8], r0, lsl #10
     878:   02000001    andeq   r0, r0, #1  ; 0x1
     87c:   620e0623    andvs   r0, lr, #36700160   ; 0x2300000
     880:   5078614d    rsbspl  r6, r8, sp, asr #2
     884:   656b6361    strbvs  r6, [fp, #-865]!
     888:   7a695374    bvc 1a55660 <MGC_HdrcDumpEndpoint+0x1a50d5c>
     88c:   05003065    streq   r3, [r0, #-101]
     890:   000176c9    andeq   r7, r1, r9, asr #13
     894:   07230200    streq   r0, [r3, -r0, lsl #4]!
     898:   5664690e    strbtpl r6, [r4], -lr, lsl #18
     89c:   6f646e65    svcvs   0x00646e65
     8a0:   ca050072    bgt 140a70 <MGC_HdrcDumpEndpoint+0x13c16c>
     8a4:   00000185    andeq   r0, r0, r5, lsl #3
     8a8:   0e082302    cdpeq   3, 0, cr2, cr8, cr2, {0}
     8ac:   72506469    subsvc  r6, r0, #1761607680 ; 0x69000000
     8b0:   6375646f    cmnvs   r5, #1862270976 ; 0x6f000000
     8b4:   cb050074    blgt    140a8c <MGC_HdrcDumpEndpoint+0x13c188>
     8b8:   00000185    andeq   r0, r0, r5, lsl #3
     8bc:   0e0a2302    cdpeq   3, 0, cr2, cr10, cr2, {0}
     8c0:   44646362    strbtmi r6, [r4], #-866
     8c4:   63697665    cmnvs   r9, #105906176  ; 0x6500000
     8c8:   cc050065    stcgt   0, cr0, [r5], {101}
     8cc:   00000185    andeq   r0, r0, r5, lsl #3
     8d0:   0e0c2302    cdpeq   3, 0, cr2, cr12, cr2, {0}
     8d4:   6e614d69    cdpvs   13, 6, cr4, cr1, cr9, {3}
     8d8:   63616675    cmnvs   r1, #122683392  ; 0x7500000
     8dc:   65727574    ldrbvs  r7, [r2, #-1396]!
     8e0:   cd050072    stcgt   0, cr0, [r5, #-456]
     8e4:   00000176    andeq   r0, r0, r6, ror r1
     8e8:   0e0e2302    cdpeq   3, 0, cr2, cr14, cr2, {0}
     8ec:   6f725069    svcvs   0x00725069
     8f0:   74637564    strbtvc r7, [r3], #-1380
     8f4:   76ce0500    strbvc  r0, [lr], r0, lsl #10
     8f8:   02000001    andeq   r0, r0, #1  ; 0x1
     8fc:   690e0f23    stmdbvs lr, {r0, r1, r5, r8, r9, sl, fp}
     900:   69726553    ldmdbvs r2!, {r0, r1, r4, r6, r8, sl, sp, lr}^
     904:   754e6c61    strbvc  r6, [lr, #-3169]
     908:   7265626d    rsbvc   r6, r5, #-805306362 ; 0xd0000006
     90c:   76cf0500    strbvc  r0, [pc], r0, lsl #10
     910:   02000001    andeq   r0, r0, #1  ; 0x1
     914:   620e1023    andvs   r1, lr, #35 ; 0x23
     918:   436d754e    cmnmi   sp, #327155712  ; 0x13800000
     91c:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
     920:   61727567    cmnvs   r2, r7, ror #10
     924:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
     928:   d0050073    andle   r0, r5, r3, ror r0
     92c:   00000176    andeq   r0, r0, r6, ror r1
     930:   00112302    andseq  r2, r1, r2, lsl #6
     934:   53554d02    cmppl   r5, #128    ; 0x80
     938:   65445f42    strbvs  r5, [r4, #-3906]
     93c:   65636976    strbvs  r6, [r3, #-2422]!
     940:   63736544    cmnvs   r3, #285212672  ; 0x11000000
     944:   74706972    ldrbtvc r6, [r0], #-2418
     948:   0500726f    streq   r7, [r0, #-623]
     94c:   0007fed1    ldrdeq  pc, [r7], -r1
     950:   05080c00    streq   r0, [r8, #-3072]
     954:   0009bbd7    ldrdeq  fp, [r9], -r7
     958:   00170d00    andseq  r0, r7, r0, lsl #26
            95a: R_ARM_ABS32    .debug_str
     95c:   d8050000    stmdale r5, {}
     960:   00000176    andeq   r0, r0, r6, ror r1
     964:   0d002302    stceq   3, cr2, [r0, #-8]
     968:   000000ca    andeq   r0, r0, sl, asr #1
            968: R_ARM_ABS32    .debug_str
     96c:   0176d905    cmneq   r6, r5, lsl #18
     970:   23020000    movwcs  r0, #8192   ; 0x2000
     974:   45620e01    strbmi  r0, [r2, #-3585]!
     978:   6f70646e    svcvs   0x0070646e
     97c:   41746e69    cmnmi   r4, r9, ror #28
     980:   65726464    ldrbvs  r6, [r2, #-1124]!
     984:   05007373    streq   r7, [r0, #-883]
     988:   000176da    ldrdeq  r7, [r1], -sl
     98c:   02230200    eoreq   r0, r3, #0  ; 0x0
     990:   00003d0d    andeq   r3, r0, sp, lsl #26
            991: R_ARM_ABS32    .debug_str
     994:   76db0500    ldrbvc  r0, [fp], r0, lsl #10
     998:   02000001    andeq   r0, r0, #1  ; 0x1
     99c:   2e0d0323    cdpcs   3, 0, cr0, cr13, cr3, {1}
            99f: R_ARM_ABS32    .debug_str
     9a0:   05000000    streq   r0, [r0]
     9a4:   000185dc    ldrdeq  r8, [r1], -ip
     9a8:   04230200    strteq  r0, [r3], #-512
     9ac:   0000860d    andeq   r8, r0, sp, lsl #12
            9ad: R_ARM_ABS32    .debug_str
     9b0:   76dd0500    ldrbvc  r0, [sp], r0, lsl #10
     9b4:   02000001    andeq   r0, r0, #1  ; 0x1
     9b8:   02000623    andeq   r0, r0, #36700160   ; 0x2300000
     9bc:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     9c0:   646e455f    strbtvs r4, [lr], #-1375
     9c4:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
     9c8:   73654474    cmnvc   r5, #1946157056 ; 0x74000000
     9cc:   70697263    rsbvc   r7, r9, r3, ror #4
     9d0:   00726f74    rsbseq  r6, r2, r4, ror pc
     9d4:   0951de05    ldmdbeq r1, {r0, r2, r9, sl, fp, ip, lr, pc}^
     9d8:   0c0c0000    stceq   0, cr0, [ip], {0}
     9dc:   0a88f405    beq fe23d9f8 <MGC_HdrcDumpEndpoint+0xfe2390f4>
     9e0:   170d0000    strne   r0, [sp, -r0]
            9e3: R_ARM_ABS32    .debug_str
     9e4:   05000000    streq   r0, [r0]
     9e8:   000176f5    strdeq  r7, [r1], -r5
     9ec:   00230200    eoreq   r0, r3, r0, lsl #4
     9f0:   0000ca0d    andeq   ip, r0, sp, lsl #20
            9f1: R_ARM_ABS32    .debug_str
     9f4:   76f60500    ldrbtvc r0, [r6], r0, lsl #10
     9f8:   02000001    andeq   r0, r0, #1  ; 0x1
     9fc:   770e0123    strvc   r0, [lr, -r3, lsr #2]
     a00:   61746f54    cmnvs   r4, r4, asr pc
     a04:   6e654c6c    cdpvs   12, 6, cr4, cr5, cr12, {3}
     a08:   00687467    rsbeq   r7, r8, r7, ror #8
     a0c:   0185f705    orreq   pc, r5, r5, lsl #14
     a10:   23020000    movwcs  r0, #8192   ; 0x2000
     a14:   4e620e02    cdpmi   14, 6, cr0, cr2, cr2, {0}
     a18:   6e496d75    mcrvs   13, 2, r6, cr9, cr5, {3}
     a1c:   66726574    undefined
     a20:   73656361    cmnvc   r5, #-2080374783    ; 0x84000001
     a24:   76f80500    ldrbtvc r0, [r8], r0, lsl #10
     a28:   02000001    andeq   r0, r0, #1  ; 0x1
     a2c:   620e0423    andvs   r0, lr, #587202560  ; 0x23000000
     a30:   666e6f43    strbtvs r6, [lr], -r3, asr #30
     a34:   72756769    rsbsvc  r6, r5, #27525120   ; 0x1a40000
     a38:   6f697461    svcvs   0x00697461
     a3c:   6c61566e    stclvs  6, cr5, [r1], #-440
     a40:   05006575    streq   r6, [r0, #-1397]
     a44:   000176f9    strdeq  r7, [r1], -r9
     a48:   05230200    streq   r0, [r3, #-512]!
     a4c:   6f43690e    svcvs   0x0043690e
     a50:   6769666e    strbvs  r6, [r9, -lr, ror #12]!
     a54:   74617275    strbtvc r7, [r1], #-629
     a58:   006e6f69    rsbeq   r6, lr, r9, ror #30
     a5c:   0176fa05    cmnpeq  r6, r5, lsl #20
     a60:   23020000    movwcs  r0, #8192   ; 0x2000
     a64:   003d0d06    eorseq  r0, sp, r6, lsl #26
            a66: R_ARM_ABS32    .debug_str
     a68:   fb050000    blx 140a72 <MGC_HdrcDumpEndpoint+0x13c16e>
     a6c:   00000176    andeq   r0, r0, r6, ror r1
     a70:   0e072302    cdpeq   3, 0, cr2, cr7, cr2, {0}
     a74:   78614d62    stmdavc r1!, {r1, r5, r6, r8, sl, fp, lr}^
     a78:   65776f50    ldrbvs  r6, [r7, #-3920]!
     a7c:   fc050072    stc2    0, cr0, [r5], {114}
     a80:   00000176    andeq   r0, r0, r6, ror r1
     a84:   00082302    andeq   r2, r8, r2, lsl #6
     a88:   53554d02    cmppl   r5, #128    ; 0x80
     a8c:   6f435f42    svcvs   0x00435f42
     a90:   6769666e    strbvs  r6, [r9, -lr, ror #12]!
     a94:   74617275    strbtvc r7, [r1], #-629
     a98:   446e6f69    strbtmi r6, [lr], #-3945
     a9c:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
     aa0:   6f747069    svcvs   0x00747069
     aa4:   fd050072    stc2    0, cr0, [r5, #-456]
     aa8:   000009da    ldrdeq  r0, [r0], -sl
     aac:   24060415    strcs   r0, [r6], #-1045
     ab0:   00000b0e    andeq   r0, r0, lr, lsl #22
     ab4:   53554d16    cmppl   r5, #1408   ; 0x580
     ab8:   4f505f42    svcmi   0x00505f42
     abc:   555f5452    ldrbpl  r5, [pc, #-1106]    ; 672 <.debug_info+0x672>
     ac0:   4f4e4b4e    svcmi   0x004e4b4e
     ac4:   00004e57    andeq   r4, r0, r7, asr lr
     ac8:   53554d16    cmppl   r5, #1408   ; 0x580
     acc:   4f505f42    svcmi   0x00505f42
     ad0:   545f5452    ldrbpl  r5, [pc], #1106 ; ad8 <.debug_info+0xad8>
     ad4:   5f455059    svcpl   0x00455059
     ad8:   434e5546    movtmi  r5, #58694  ; 0xe546
     adc:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
     ae0:   4d160100    ldfmis  f0, [r6]
     ae4:   5f425355    svcpl   0x00425355
     ae8:   54524f50    ldrbpl  r4, [r2], #-3920
     aec:   5059545f    subspl  r5, r9, pc, asr r4
     af0:   4f485f45    svcmi   0x00485f45
     af4:   02005453    andeq   r5, r0, #1392508928 ; 0x53000000
     af8:   53554d16    cmppl   r5, #1408   ; 0x580
     afc:   4f505f42    svcmi   0x00505f42
     b00:   545f5452    ldrbpl  r5, [pc], #1106 ; b08 <.debug_info+0xb08>
     b04:   5f455059    svcpl   0x00455059
     b08:   0047544f    subeq   r5, r7, pc, asr #8
     b0c:   4d020003    stcmi   0, cr0, [r2, #-12]
     b10:   5f425355    svcpl   0x00425355
     b14:   74726f50    ldrbtvc r6, [r2], #-3920
     b18:   65707954    ldrbvs  r7, [r0, #-2388]!
     b1c:   ac2d0600    stcge   6, cr0, [sp]
     b20:   1500000a    strne   r0, [r0, #-10]
     b24:   8a340604    bhi d0233c <MGC_HdrcDumpEndpoint+0xcfda38>
     b28:   1600000b    strne   r0, [r0], -fp
     b2c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     b30:   524f505f    subpl   r5, pc, #95 ; 0x5f
     b34:   50535f54    subspl  r5, r3, r4, asr pc
     b38:   5f444545    svcpl   0x00444545
     b3c:   4e4b4e55    mcrmi   14, 2, r4, cr11, cr5, {2}
     b40:   004e574f    subeq   r5, lr, pc, asr #14
     b44:   554d1600    strbpl  r1, [sp, #-1536]
     b48:   505f4253    subspl  r4, pc, r3, asr r2
     b4c:   5f54524f    svcpl   0x0054524f
     b50:   45455053    strbmi  r5, [r5, #-83]
     b54:   4f4c5f44    svcmi   0x004c5f44
     b58:   16010057    undefined
     b5c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     b60:   524f505f    subpl   r5, pc, #95 ; 0x5f
     b64:   50535f54    subspl  r5, r3, r4, asr pc
     b68:   5f444545    svcpl   0x00444545
     b6c:   4c4c5546    cfstr64mi   mvdx5, [ip], {70}
     b70:   4d160200    lfmmi   f0, 4, [r6]
     b74:   5f425355    svcpl   0x00425355
     b78:   54524f50    ldrbpl  r4, [r2], #-3920
     b7c:   4550535f    ldrbmi  r5, [r0, #-863]
     b80:   485f4445    ldmdami pc, {r0, r2, r6, sl, lr}^
     b84:   00484749    subeq   r4, r8, r9, asr #14
     b88:   4d020003    stcmi   0, cr0, [r2, #-12]
     b8c:   5f425355    svcpl   0x00425355
     b90:   74726f50    ldrbtvc r6, [r2], #-3920
     b94:   65657053    strbvs  r7, [r5, #-83]!
     b98:   3d060064    stccc   0, cr0, [r6, #-400]
     b9c:   00000b23    andeq   r0, r0, r3, lsr #22
     ba0:   44060415    strmi   r0, [r6], #-1045
     ba4:   00000c0a    andeq   r0, r0, sl, lsl #24
     ba8:   53554d16    cmppl   r5, #1408   ; 0x580
     bac:   4f505f42    svcmi   0x00505f42
     bb0:   5f524557    svcpl   0x00524557
     bb4:   0046464f    subeq   r4, r6, pc, asr #12
     bb8:   554d1600    strbpl  r1, [sp, #-1536]
     bbc:   505f4253    subspl  r4, pc, r3, asr r2
     bc0:   5245574f    subpl   r5, r5, #20709376   ; 0x13c0000
     bc4:   01004445    tsteq   r0, r5, asr #8
     bc8:   53554d16    cmppl   r5, #1408   ; 0x580
     bcc:   45445f42    strbmi  r5, [r4, #-3906]
     bd0:   4c554146    ldfmie  f4, [r5], {70}
     bd4:   16020054    undefined
     bd8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     bdc:   4444415f    strbmi  r4, [r4], #-351
     be0:   53534552    cmppl   r3, #343932928  ; 0x14800000
     be4:   4d160300    ldcmi   3, cr0, [r6]
     be8:   5f425355    svcpl   0x00425355
     bec:   464e4f43    strbmi  r4, [lr], -r3, asr #30
     bf0:   52554749    subspl  r4, r5, #19136512   ; 0x1240000
     bf4:   04004445    streq   r4, [r0], #-1093
     bf8:   53554d16    cmppl   r5, #1408   ; 0x580
     bfc:   55535f42    ldrbpl  r5, [r3, #-3906]
     c00:   4e455053    mcrmi   0, 2, r5, cr5, cr3, {2}
     c04:   00444544    subeq   r4, r4, r4, asr #10
     c08:   4d020005    stcmi   0, cr0, [r2, #-20]
     c0c:   5f425355    svcpl   0x00425355
     c10:   74617453    strbtvc r7, [r1], #-1107
     c14:   51060065    tstpl   r6, r5, rrx
     c18:   00000ba0    andeq   r0, r0, r0, lsr #23
     c1c:   58060415    stmdapl r6, {r0, r2, r4, sl}
     c20:   00000ce7    andeq   r0, r0, r7, ror #25
     c24:   53554d16    cmppl   r5, #1408   ; 0x580
     c28:   42415f42    submi   r5, r1, #264    ; 0x108
     c2c:   4c44495f    mcrrmi  9, 5, r4, r4, cr15
     c30:   16000045    strne   r0, [r0], -r5, asr #32
     c34:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     c38:   4941575f    stmdbmi r1, {r0, r1, r2, r3, r4, r6, r8, r9, sl, ip, lr}^
     c3c:   52565f54    subspl  r5, r6, #336    ; 0x150
     c40:   00455349    subeq   r5, r5, r9, asr #6
     c44:   554d1601    strbpl  r1, [sp, #-1537]
     c48:   425f4253    subsmi  r4, pc, #805306373  ; 0x30000005
     c4c:   5245505f    subpl   r5, r5, #95 ; 0x5f
     c50:   45485049    strbmi  r5, [r8, #-73]
     c54:   004c4152    subeq   r4, ip, r2, asr r1
     c58:   554d1611    strbpl  r1, [sp, #-1553]
     c5c:   425f4253    subsmi  r4, pc, #805306373  ; 0x30000005
     c60:   4941575f    stmdbmi r1, {r0, r1, r2, r3, r4, r6, r8, r9, sl, ip, lr}^
     c64:   43415f54    movtmi  r5, #8020   ; 0x1f54
     c68:   12004e4f    andne   r4, r0, #1264   ; 0x4f0
     c6c:   53554d16    cmppl   r5, #1408   ; 0x580
     c70:   5f425f42    svcpl   0x00425f42
     c74:   54534f48    ldrbpl  r4, [r3], #-3912
     c78:   4d161300    ldcmi   3, cr1, [r6]
     c7c:   5f425355    svcpl   0x00425355
     c80:   52535f42    subspl  r5, r3, #264    ; 0x108
     c84:   4e495f50    mcrmi   15, 2, r5, cr9, cr0, {2}
     c88:   14005449    strne   r5, [r0], #-1097
     c8c:   53554d16    cmppl   r5, #1408   ; 0x580
     c90:   5f415f42    svcpl   0x00415f42
     c94:   49524550    ldmdbmi r2, {r4, r6, r8, sl, lr}^
     c98:   52454850    subpl   r4, r5, #5242880    ; 0x500000
     c9c:   21004c41    tstcs   r0, r1, asr #24
     ca0:   53554d16    cmppl   r5, #1408   ; 0x580
     ca4:   5f415f42    svcpl   0x00415f42
     ca8:   54494157    strbpl  r4, [r9], #-343
     cac:   4f43425f    svcmi   0x0043425f
     cb0:   1622004e    strtne  r0, [r2], -lr, asr #32
     cb4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     cb8:   485f415f    ldmdami pc, {r0, r1, r2, r3, r4, r6, r8, lr}^
     cbc:   0054534f    subseq  r5, r4, pc, asr #6
     cc0:   554d1623    strbpl  r1, [sp, #-1571]
     cc4:   415f4253    cmpmi   pc, r3, asr r2
     cc8:   5355535f    cmppl   r5, #2080374785 ; 0x7c000001
     ccc:   444e4550    strbmi  r4, [lr], #-1360
     cd0:   4d162400    cfldrsmi    mvf2, [r6]
     cd4:   5f425355    svcpl   0x00425355
     cd8:   52545f53    subspl  r5, r4, #332    ; 0x14c
     cdc:   49534e41    ldmdbmi r3, {r0, r6, r9, sl, fp, lr}^
     ce0:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
     ce4:   02003000    andeq   r3, r0, #0  ; 0x0
     ce8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     cec:   67744f5f    undefined
     cf0:   74617453    strbtvc r7, [r1], #-1107
     cf4:   6f060065    svcvs   0x00060065
     cf8:   00000c1c    andeq   r0, r0, ip, lsl ip
     cfc:   76060415    undefined
     d00:   00000d7b    andeq   r0, r0, fp, ror sp
     d04:   53554d16    cmppl   r5, #1408   ; 0x580
     d08:   4f435f42    svcmi   0x00435f42
     d0c:   43454e4e    movtmi  r4, #24142  ; 0x5e4e
     d10:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
     d14:   4550535f    ldrbmi  r5, [r0, #-863]
     d18:   555f4445    ldrbpl  r4, [pc, #-1093]    ; 8db <.debug_info+0x8db>
     d1c:   4f4e4b4e    svcmi   0x004e4b4e
     d20:   00004e57    andeq   r4, r0, r7, asr lr
     d24:   53554d16    cmppl   r5, #1408   ; 0x580
     d28:   4f435f42    svcmi   0x00435f42
     d2c:   43454e4e    movtmi  r4, #24142  ; 0x5e4e
     d30:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
     d34:   4550535f    ldrbmi  r5, [r0, #-863]
     d38:   4c5f4445    cfldrdmi    mvd4, [pc], {69}
     d3c:   0100574f    tsteq   r0, pc, asr #14
     d40:   53554d16    cmppl   r5, #1408   ; 0x580
     d44:   4f435f42    svcmi   0x00435f42
     d48:   43454e4e    movtmi  r4, #24142  ; 0x5e4e
     d4c:   4e4f4954    mcrmi   9, 2, r4, cr15, cr4, {2}
     d50:   4550535f    ldrbmi  r5, [r0, #-863]
     d54:   465f4445    ldrbmi  r4, [pc], -r5, asr #8
     d58:   004c4c55    subeq   r4, ip, r5, asr ip
     d5c:   554d1602    strbpl  r1, [sp, #-1538]
     d60:   435f4253    cmpmi   pc, #805306373  ; 0x30000005
     d64:   454e4e4f    strbmi  r4, [lr, #-3663]
     d68:   4f495443    svcmi   0x00495443
     d6c:   50535f4e    subspl  r5, r3, lr, asr #30
     d70:   5f444545    svcpl   0x00444545
     d74:   48474948    stmdami r7, {r3, r6, r8, fp, lr}^
     d78:   02000300    andeq   r0, r0, #0  ; 0x0
     d7c:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     d80:   6e6f435f    mcrvs   3, 3, r4, cr15, cr15, {2}
     d84:   7463656e    strbtvc r6, [r3], #-1390
     d88:   536e6f69    cmnpl   lr, #420    ; 0x1a4
     d8c:   64656570    strbtvs r6, [r5], #-1392
     d90:   fc7f0600    ldc2l   6, cr0, [pc]
     d94:   0c00000c    stceq   0, cr0, [r0], {12}
     d98:   e1e40610    mvn r0, r0, lsl r6
     d9c:   0d00000d    stceq   0, cr0, [r0, #-52]
     da0:   000000ee    andeq   r0, r0, lr, ror #1
            da0: R_ARM_ABS32    .debug_str
     da4:   00f1e506    rscseq  lr, r1, r6, lsl #10
     da8:   23020000    movwcs  r0, #8192   ; 0x2000
     dac:   63620e00    cmnvs   r2, #0  ; 0x0
     db0:   69644364    stmdbvs r4!, {r2, r5, r6, r8, r9, lr}^
     db4:   00766552    rsbseq  r6, r6, r2, asr r5
     db8:   0185e606    orreq   lr, r5, r6, lsl #12
     dbc:   23020000    movwcs  r0, #8192   ; 0x2000
     dc0:   79540e04    ldmdbvc r4, {r2, r9, sl, fp}^
     dc4:   06006570    undefined
     dc8:   000b0ee7    andeq   r0, fp, r7, ror #29
     dcc:   08230200    stmdaeq r3!, {r9}
     dd0:   6570530e    ldrbvs  r5, [r0, #-782]!
     dd4:   06006465    streq   r6, [r0], -r5, ror #8
     dd8:   000b8ae8    andeq   r8, fp, r8, ror #21
     ddc:   0c230200    sfmeq   f0, 4, [r3]
     de0:   554d0200    strbpl  r0, [sp, #-512]
     de4:   505f4253    subspl  r4, pc, r3, asr r2
     de8:   0074726f    rsbseq  r7, r4, pc, ror #4
     dec:   0d97e906    ldceq   9, cr14, [r7, #24]
     df0:   5f170000    svcpl   0x00170000
     df4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
     df8:   7665445f    undefined
     dfc:   00656369    rsbeq   r6, r5, r9, ror #6
     e00:   16eb0640    strbtne r0, [fp], r0, asr #12
     e04:   1300000f    movwne  r0, #15 ; 0xf
     e08:   0000004a    andeq   r0, r0, sl, asr #32
            e08: R_ARM_ABS32    .debug_str
     e0c:   16010e06    strne   r0, [r1], -r6, lsl #28
     e10:   0200000f    andeq   r0, r0, #15 ; 0xf
     e14:   ee130023    cdp 0, 1, cr0, cr3, cr3, {1}
            e17: R_ARM_ABS32    .debug_str
     e18:   06000000    streq   r0, [r0], -r0
     e1c:   00f1010f    rscseq  r0, r1, pc, lsl #2
     e20:   23020000    movwcs  r0, #8192   ; 0x2000
     e24:   44701404    ldrbtmi r1, [r0], #-1028
     e28:   65766972    ldrbvs  r6, [r6, #-2418]!
     e2c:   69725072    ldmdbvs r2!, {r1, r4, r5, r6, ip, lr}^
     e30:   65746176    ldrbvs  r6, [r4, #-374]!
     e34:   61746144    cmnvs   r4, r4, asr #2
     e38:   01100600    tsteq   r0, r0, lsl #12
     e3c:   000000f1    strdeq  r0, [r0], -r1
     e40:   14082302    strne   r2, [r8], #-770
     e44:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
     e48:   65446563    strbvs  r6, [r4, #-1379]
     e4c:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
     e50:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
     e54:   01110600    tsteq   r1, r0, lsl #12
     e58:   00000934    andeq   r0, r0, r4, lsr r9
     e5c:   140c2302    strne   r2, [ip], #-770
     e60:   72754370    rsbsvc  r4, r5, #-1073741823    ; 0xc0000001
     e64:   746e6572    strbtvc r6, [lr], #-1394
     e68:   666e6f43    strbtvs r6, [lr], -r3, asr #30
     e6c:   72756769    rsbsvc  r6, r5, #27525120   ; 0x1a40000
     e70:   6f697461    svcvs   0x00697461
     e74:   1206006e    andne   r0, r6, #110    ; 0x6e
     e78:   000f1c01    andeq   r1, pc, r1, lsl #24
     e7c:   20230200    eorcs   r0, r3, r0, lsl #4
     e80:   00019113    andeq   r9, r1, r3, lsl r1
            e81: R_ARM_ABS32    .debug_str
     e84:   01130600    tsteq   r3, r0, lsl #12
     e88:   00000f22    andeq   r0, r0, r2, lsr #30
     e8c:   14242302    strtne  r2, [r4], #-770
     e90:   73654470    cmnvc   r5, #1879048192 ; 0x70000000
     e94:   70697263    rsbvc   r7, r9, r3, ror #4
     e98:   42726f74    rsbsmi  r6, r2, #464    ; 0x1d0
     e9c:   65666675    strbvs  r6, [r6, #-1653]!
     ea0:   14060072    strne   r0, [r6], #-114
     ea4:   000f2801    andeq   r2, pc, r1, lsl #16
     ea8:   28230200    stmdacs r3!, {r9}
     eac:   0001ae13    andeq   sl, r1, r3, lsl lr
            ead: R_ARM_ABS32    .debug_str
     eb0:   01150600    tsteq   r5, r0, lsl #12
     eb4:   00000185    andeq   r0, r0, r5, lsl #3
     eb8:   142c2302    strtne  r2, [ip], #-770
     ebc:   6e6e6f43    cdpvs   15, 6, cr6, cr14, cr3, {2}
     ec0:   69746365    ldmdbvs r4!, {r0, r2, r5, r6, r8, r9, sp, lr}^
     ec4:   70536e6f    subsvc  r6, r3, pc, ror #28
     ec8:   00646565    rsbeq   r6, r4, r5, ror #10
     ecc:   7b011606    blvc    466ec <MGC_HdrcDumpEndpoint+0x41de8>
     ed0:   0200000d    andeq   r0, r0, #13 ; 0xd
     ed4:   1b133023    blne    4cc094 <MGC_HdrcDumpEndpoint+0x4c7790>
            ed7: R_ARM_ABS32    .debug_str
     ed8:   06000001    streq   r0, [r0], -r1
     edc:   01760117    cmneq   r6, r7, lsl r1
     ee0:   23020000    movwcs  r0, #8192   ; 0x2000
     ee4:   50701434    rsbspl  r1, r0, r4, lsr r4
     ee8:   6e657261    cdpvs   2, 6, cr7, cr5, cr1, {3}
     eec:   62735574    rsbsvs  r5, r3, #486539264  ; 0x1d000000
     ef0:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
     ef4:   06006563    streq   r6, [r0], -r3, ror #10
     ef8:   0f2e0118    svceq   0x002e0118
     efc:   23020000    movwcs  r0, #8192   ; 0x2000
     f00:   48621438    stmdami r2!, {r3, r4, r5, sl, ip}^
     f04:   6f506275    svcvs   0x00506275
     f08:   06007472    undefined
     f0c:   01760119    cmneq   r6, r9, lsl r1
     f10:   23020000    movwcs  r0, #8192   ; 0x2000
     f14:   0407003c    streq   r0, [r7], #-60
     f18:   00000de1    andeq   r0, r0, r1, ror #27
     f1c:   0a880407    beq fe201f40 <MGC_HdrcDumpEndpoint+0xfe1fd63c>
     f20:   04070000    streq   r0, [r7]
     f24:   00000f1c    andeq   r0, r0, ip, lsl pc
     f28:   01760407    cmneq   r6, r7, lsl #8
     f2c:   04070000    streq   r0, [r7]
     f30:   00000df2    strdeq  r0, [r0], -r2
     f34:   53554d11    cmppl   r5, #1088   ; 0x440
     f38:   65445f42    strbvs  r5, [r4, #-3906]
     f3c:   65636976    strbvs  r6, [r3, #-2422]!
     f40:   011a0600    tsteq   sl, r0, lsl #12
     f44:   00000df2    strdeq  r0, [r0], -r2
     f48:   53554d11    cmppl   r5, #1088   ; 0x440
     f4c:   75425f42    strbvc  r5, [r2, #-3906]
     f50:   6e614873    mcrvs   8, 3, r4, cr1, cr3, {3}
     f54:   00656c64    rsbeq   r6, r5, r4, ror #24
     f58:   f1011d06    undefined instruction 0xf1011d06
     f5c:   12000000    andne   r0, r0, #0  ; 0x0
     f60:   012d0610    teqeq   sp, r0, lsl r6
     f64:   00000fa6    andeq   r0, r0, r6, lsr #31
     f68:   0001db13    andeq   sp, r1, r3, lsl fp
            f69: R_ARM_ABS32    .debug_str
     f6c:   012e0600    teqeq   lr, r0, lsl #12
     f70:   00000fa6    andeq   r0, r0, r6, lsr #31
     f74:   14002302    strne   r2, [r0], #-770
     f78:   44627355    strbtmi r7, [r2], #-853
     f7c:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
     f80:   6f747069    svcvs   0x00747069
     f84:   2f060072    svccs   0x00060072
     f88:   0009bb01    andeq   fp, r9, r1, lsl #22
     f8c:   04230200    strteq  r0, [r3], #-512
     f90:   614e7714    cmpvs   lr, r4, lsl r7
     f94:   6d694c6b    stclvs  12, cr4, [r9, #-428]!
     f98:   06007469    streq   r7, [r0], -r9, ror #8
     f9c:   01850130    orreq   r0, r5, r0, lsr r1
     fa0:   23020000    movwcs  r0, #8192   ; 0x2000
     fa4:   0407000c    streq   r0, [r7], #-12
     fa8:   00000fac    andeq   r0, r0, ip, lsr #31
     fac:   000f3408    andeq   r3, pc, r8, lsl #8
     fb0:   554d1100    strbpl  r1, [sp, #-256]
     fb4:   445f4253    ldrbmi  r4, [pc], #595  ; fbc <.debug_info+0xfbc>
     fb8:   63697665    cmnvs   r9, #105906176  ; 0x6500000
     fbc:   646e4565    strbtvs r4, [lr], #-1381
     fc0:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
     fc4:   31060074    tstcc   r6, r4, ror r0
     fc8:   000f5f01    andeq   r5, pc, r1, lsl #30
     fcc:   060c1200    streq   r1, [ip], -r0, lsl #4
     fd0:   10110141    andsne  r0, r1, r1, asr #2
     fd4:   64140000    ldrvs   r0, [r4]
     fd8:   66754277    undefined
     fdc:   53726566    cmnpl   r2, #427819008  ; 0x19800000
     fe0:   00657a69    rsbeq   r7, r5, r9, ror #20
     fe4:   a4014206    strge   r4, [r1], #-518
     fe8:   02000001    andeq   r0, r0, #1  ; 0x1
     fec:   62140023    andsvs  r0, r4, #35 ; 0x23
     ff0:   616c466d    cmnvs   ip, sp, ror #12
     ff4:   06007367    streq   r7, [r0], -r7, ror #6
     ff8:   01a40143    undefined instruction 0x01a40143
     ffc:   23020000    movwcs  r0, #8192   ; 0x2000
    1000:   00ee1304    rsceq   r1, lr, r4, lsl #6
            1002: R_ARM_ABS32   .debug_str
    1004:   44060000    strmi   r0, [r6]
    1008:   0000f101    andeq   pc, r0, r1, lsl #2
    100c:   08230200    stmdaeq r3!, {r9}
    1010:   554d1100    strbpl  r1, [sp, #-256]
    1014:   455f4253    ldrbmi  r4, [pc, #-595] ; dc9 <.debug_info+0xdc9>
    1018:   6f70646e    svcvs   0x0070646e
    101c:   52746e69    rsbspl  r6, r4, #1680   ; 0x690
    1020:   756f7365    strbvc  r7, [pc, #-869]!    ; cc3 <.debug_info+0xcc3>
    1024:   00656372    rsbeq   r6, r5, r2, ror r3
    1028:   cd014506    cfstr32gt   mvfx4, [r1, #-24]
    102c:   1100000f    tstne   r0, pc
    1030:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1034:   7069505f    rsbvc   r5, r9, pc, asr r0
    1038:   52060065    andpl   r0, r6, #101    ; 0x65
    103c:   0000f101    andeq   pc, r0, r1, lsl #2
    1040:   554d1100    strbpl  r1, [sp, #-256]
    1044:   705f4253    subsvc  r4, pc, r3, asr r2
    1048:   6e6f4366    cdpvs   3, 6, cr4, cr15, cr6, {3}
    104c:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    1050:   43707249    cmnmi   r0, #-1879048188    ; 0x90000004
    1054:   6c706d6f    ldclvs  13, cr6, [r0], #-444
    1058:   00657465    rsbeq   r7, r5, r5, ror #8
    105c:   63015c06    movwvs  r5, #7174   ; 0x1c06
    1060:   07000010    smladeq r0, r0, r0, r0
    1064:   00106904    andseq  r6, r0, r4, lsl #18
    1068:   7a010b00    bvc 43c70 <MGC_HdrcDumpEndpoint+0x3f36c>
    106c:   0a000010    beq 10b4 <.debug_info+0x10b4>
    1070:   000000f1    strdeq  r0, [r0], -r1
    1074:   00107a0a    andseq  r7, r0, sl, lsl #20
    1078:   04070000    streq   r0, [r7]
    107c:   00001080    andeq   r1, r0, r0, lsl #1
    1080:   554d5f18    strbpl  r5, [sp, #-3864]
    1084:   435f4253    cmpmi   pc, #805306373  ; 0x30000005
    1088:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    108c:   72496c6f    subvc   r6, r9, #28416  ; 0x6f00
    1090:   062c0070    undefined
    1094:   11770154    cmnne   r7, r4, asr r1
    1098:   db130000    blle    4c0008 <MGC_HdrcDumpEndpoint+0x4bb704>
            109b: R_ARM_ABS32   .debug_str
    109c:   06000001    streq   r0, [r0], -r1
    10a0:   0fa6017b    svceq   0x00a6017b
    10a4:   23020000    movwcs  r0, #8192   ; 0x2000
    10a8:   4f701400    svcmi   0x00701400
    10ac:   75427475    strbvc  r7, [r2, #-1141]
    10b0:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    10b4:   017c0600    cmneq   ip, r0, lsl #12
    10b8:   00000667    andeq   r0, r0, r7, ror #12
    10bc:   14042302    strne   r2, [r4], #-770
    10c0:   754f7764    strbvc  r7, [pc, #-1892]    ; 964 <.debug_info+0x964>
    10c4:   6e654c74    mcrvs   12, 3, r4, cr5, cr4, {3}
    10c8:   00687467    rsbeq   r7, r8, r7, ror #8
    10cc:   a4017d06    strge   r7, [r1], #-3334
    10d0:   02000001    andeq   r0, r0, #1  ; 0x1
    10d4:   70140823    andsvc  r0, r4, r3, lsr #16
    10d8:   75426e49    strbvc  r6, [r2, #-3657]
    10dc:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    10e0:   017e0600    cmneq   lr, r0, lsl #12
    10e4:   00000f28    andeq   r0, r0, r8, lsr #30
    10e8:   140c2302    strne   r2, [ip], #-770
    10ec:   6e497764    cdpvs   7, 4, cr7, cr9, cr4, {3}
    10f0:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    10f4:   06006874    undefined
    10f8:   01a4017f    undefined instruction 0x01a4017f
    10fc:   23020000    movwcs  r0, #8192   ; 0x2000
    1100:   00501310    subseq  r1, r0, r0, lsl r3
            1102: R_ARM_ABS32   .debug_str
    1104:   80060000    andhi   r0, r6, r0
    1108:   0001a401    andeq   sl, r1, r1, lsl #8
    110c:   14230200    strtne  r0, [r3], #-512
    1110:   41776414    cmnmi   r7, r4, lsl r4
    1114:   61757463    cmnvs   r5, r3, ror #8
    1118:   74754f6c    ldrbtvc r4, [r5], #-3948
    111c:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    1120:   06006874    undefined
    1124:   01a40181    undefined instruction 0x01a40181
    1128:   23020000    movwcs  r0, #8192   ; 0x2000
    112c:   77641418    undefined
    1130:   75746341    ldrbvc  r6, [r4, #-833]!
    1134:   6e496c61    cdpvs   12, 4, cr6, cr9, cr1, {3}
    1138:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    113c:   06006874    undefined
    1140:   01a40182    undefined instruction 0x01a40182
    1144:   23020000    movwcs  r0, #8192   ; 0x2000
    1148:   0066131c    rsbeq   r1, r6, ip, lsl r3
            114a: R_ARM_ABS32   .debug_str
    114c:   83060000    movwhi  r0, #24576  ; 0x6000
    1150:   00104101    andseq  r4, r0, r1, lsl #2
    1154:   20230200    eorcs   r0, r3, r0, lsl #4
    1158:   00001f13    andeq   r1, r0, r3, lsl pc
            1159: R_ARM_ABS32   .debug_str
    115c:   01840600    orreq   r0, r4, r0, lsl #12
    1160:   000000f1    strdeq  r0, [r0], -r1
    1164:   13242302    teqne   r4, #134217728  ; 0x8000000
    1168:   0000017d    andeq   r0, r0, sp, ror r1
            1168: R_ARM_ABS32   .debug_str
    116c:   76018506    strvc   r8, [r1], -r6, lsl #10
    1170:   02000001    andeq   r0, r0, #1  ; 0x1
    1174:   11002823    tstne   r0, r3, lsr #16
    1178:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    117c:   6e6f435f    mcrvs   3, 3, r4, cr15, cr15, {2}
    1180:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    1184:   00707249    rsbseq  r7, r0, r9, asr #4
    1188:   80018606    andhi   r8, r1, r6, lsl #12
    118c:   11000010    tstne   r0, r0, lsl r0
    1190:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1194:   4966705f    stmdbmi r6!, {r0, r1, r2, r3, r4, r6, ip, sp, lr}^
    1198:   6f437072    svcvs   0x00437072
    119c:   656c706d    strbvs  r7, [ip, #-109]!
    11a0:   06006574    undefined
    11a4:   11aa0190    strbne  r0, [sl, r0]!
    11a8:   04070000    streq   r0, [r7]
    11ac:   000011b0    strheq  r1, [r0], -r0
    11b0:   11c1010b    bicne   r0, r1, fp, lsl #2
    11b4:   f10a0000    cpsie   ,#0
    11b8:   0a000000    beq 11c0 <.debug_info+0x11c0>
    11bc:   000011c1    andeq   r1, r0, r1, asr #3
    11c0:   c7040700    strgt   r0, [r4, -r0, lsl #14]
    11c4:   18000011    stmdane r0, {r0, r4}
    11c8:   53554d5f    cmppl   r5, #6080   ; 0x17c0
    11cc:   72495f42    subvc   r5, r9, #264    ; 0x108
    11d0:   06200070    undefined
    11d4:   12880188    addne   r0, r8, #34 ; 0x22
    11d8:   68140000    ldmdavs r4, {}
    11dc:   65706950    ldrbvs  r6, [r0, #-2384]!
    11e0:   01af0600    undefined instruction 0x01af0600
    11e4:   0000102f    andeq   r1, r0, pc, lsr #32
    11e8:   13002302    movwne  r2, #770    ; 0x302
    11ec:   000000b8    strheq  r0, [r0], -r8
            11ec: R_ARM_ABS32   .debug_str
    11f0:   2801b006    stmdacs r1, {r1, r2, ip, sp, pc}
    11f4:   0200000f    andeq   r0, r0, #15 ; 0xf
    11f8:   64140423    ldrvs   r0, [r4], #-1059
    11fc:   6e654c77    mcrvs   12, 3, r4, cr5, cr7, {3}
    1200:   00687467    rsbeq   r7, r8, r7, ror #8
    1204:   a401b106    strge   fp, [r1], #-262
    1208:   02000001    andeq   r0, r0, #1  ; 0x1
    120c:   50130823    andspl  r0, r3, r3, lsr #16
            120f: R_ARM_ABS32   .debug_str
    1210:   06000000    streq   r0, [r0], -r0
    1214:   01a401b2    strheq  r0, [r4, r2]!
    1218:   23020000    movwcs  r0, #8192   ; 0x2000
    121c:   0008130c    andeq   r1, r8, ip, lsl #6
            121e: R_ARM_ABS32   .debug_str
    1220:   b3060000    movwlt  r0, #24576  ; 0x6000
    1224:   0001a401    andeq   sl, r1, r1, lsl #8
    1228:   10230200    eorne   r0, r3, r0, lsl #4
    122c:   00006613    andeq   r6, r0, r3, lsl r6
            122d: R_ARM_ABS32   .debug_str
    1230:   01b40600    undefined instruction 0x01b40600
    1234:   0000118f    andeq   r1, r0, pc, lsl #3
    1238:   13142302    tstne   r4, #134217728  ; 0x8000000
    123c:   0000001f    andeq   r0, r0, pc, lsl r0
            123c: R_ARM_ABS32   .debug_str
    1240:   f101b506    undefined instruction 0xf101b506
    1244:   02000000    andeq   r0, r0, #0  ; 0x0
    1248:   62141823    andsvs  r1, r4, #2293760    ; 0x230000
    124c:   6f6c6c41    svcvs   0x006c6c41
    1250:   6f685377    svcvs   0x00685377
    1254:   72547472    subsvc  r7, r4, #1912602624 ; 0x72000000
    1258:   66736e61    ldrbtvs r6, [r3], -r1, ror #28
    125c:   06007265    streq   r7, [r0], -r5, ror #4
    1260:   017601b6    ldrheq  r0, [r6, #-22]!
    1264:   23020000    movwcs  r0, #8192   ; 0x2000
    1268:   0170131c    cmneq   r0, ip, lsl r3
            126a: R_ARM_ABS32   .debug_str
    126c:   b7060000    strlt   r0, [r6, -r0]
    1270:   00017601    andeq   r7, r1, r1, lsl #12
    1274:   1d230200    sfmne   f0, 4, [r3]
    1278:   00017d13    andeq   r7, r1, r3, lsl sp
            1279: R_ARM_ABS32   .debug_str
    127c:   01b80600    undefined instruction 0x01b80600
    1280:   00000176    andeq   r0, r0, r6, ror r1
    1284:   001e2302    andseq  r2, lr, r2, lsl #6
    1288:   53554d11    cmppl   r5, #1088   ; 0x440
    128c:   72495f42    subvc   r5, r9, #264    ; 0x108
    1290:   b9060070    stmdblt r6, {r4, r5, r6}
    1294:   0011c701    andseq  ip, r1, r1, lsl #14
    1298:   554d1100    strbpl  r1, [sp, #-256]
    129c:   705f4253    subsvc  r4, pc, r3, asr r2
    12a0:   6f734966    svcvs   0x00734966
    12a4:   72496863    subvc   r6, r9, #6488064    ; 0x630000
    12a8:   6d6f4370    stclvs  3, cr4, [pc, #-448]!
    12ac:   74656c70    strbtvc r6, [r5], #-3184
    12b0:   c3060065    movwgt  r0, #24677  ; 0x6065
    12b4:   0012b901    andseq  fp, r2, r1, lsl #18
    12b8:   bf040700    svclt   0x00040700
    12bc:   0b000012    bleq    130c <.debug_info+0x130c>
    12c0:   0012d001    andseq  sp, r2, r1
    12c4:   00f10a00    rscseq  r0, r1, r0, lsl #20
    12c8:   d00a0000    andle   r0, sl, r0
    12cc:   00000012    andeq   r0, r0, r2, lsl r0
    12d0:   12d60407    sbcsne  r0, r6, #117440512  ; 0x7000000
    12d4:   5f180000    svcpl   0x00180000
    12d8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    12dc:   6f73495f    svcvs   0x0073495f
    12e0:   72496863    subvc   r6, r9, #6488064    ; 0x630000
    12e4:   06240070    undefined
    12e8:   13dd01bb    bicsne  r0, sp, #-1073741778    ; 0xc000002e
    12ec:   68140000    ldmdavs r4, {}
    12f0:   65706950    ldrbvs  r6, [r0, #-2384]!
    12f4:   01e80600    mvneq   r0, r0, lsl #12
    12f8:   0000102f    andeq   r1, r0, pc, lsr #32
    12fc:   13002302    movwne  r2, #770    ; 0x302
    1300:   000000b8    strheq  r0, [r0], -r8
            1300: R_ARM_ABS32   .debug_str
    1304:   2801e906    stmdacs r1, {r1, r2, r8, fp, sp, lr, pc}
    1308:   0200000f    andeq   r0, r0, #15 ; 0xf
    130c:   61140423    tstvs   r4, r3, lsr #8
    1310:   654c7764    strbvs  r7, [ip, #-1892]
    1314:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1318:   01ea0600    mvneq   r0, r0, lsl #12
    131c:   000013dd    ldrdeq  r1, [r0], -sp
    1320:   14082302    strne   r2, [r8], #-770
    1324:   53776461    cmnpl   r7, #1627389952 ; 0x61000000
    1328:   75746174    ldrbvc  r6, [r4, #-372]!
    132c:   eb060073    bl  181500 <MGC_HdrcDumpEndpoint+0x17cbfc>
    1330:   0013dd01    andseq  sp, r3, r1, lsl #26
    1334:   0c230200    sfmeq   f0, 4, [r3]
    1338:   77646114    undefined
    133c:   75746341    ldrbvc  r6, [r4, #-833]!
    1340:   654c6c61    strbvs  r6, [ip, #-3169]
    1344:   6874676e    ldmdavs r4!, {r1, r2, r3, r5, r6, r8, r9, sl, sp, lr}^
    1348:   01ec0600    mvneq   r0, r0, lsl #12
    134c:   000013dd    ldrdeq  r1, [r0], -sp
    1350:   13102302    tstne   r0, #134217728  ; 0x8000000
    1354:   00000066    andeq   r0, r0, r6, rrx
            1354: R_ARM_ABS32   .debug_str
    1358:   9901ed06    stmdbls r1, {r1, r2, r8, sl, fp, sp, lr, pc}
    135c:   02000012    andeq   r0, r0, #18 ; 0x12
    1360:   1f131423    svcne   0x00131423
            1363: R_ARM_ABS32   .debug_str
    1364:   06000000    streq   r0, [r0], -r0
    1368:   00f101ee    rscseq  r0, r1, lr, ror #3
    136c:   23020000    movwcs  r0, #8192   ; 0x2000
    1370:   46771418    undefined
    1374:   656d6172    strbvs  r6, [sp, #-370]!
    1378:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    137c:   ef060074    svc 0x00060074
    1380:   00018501    andeq   r8, r1, r1, lsl #10
    1384:   1c230200    sfmne   f0, 4, [r3]
    1388:   75437714    strbvc  r7, [r3, #-1812]
    138c:   6e657272    mcrvs   2, 3, r7, cr5, cr2, {3}
    1390:   61724674    cmnvs   r2, r4, ror r6
    1394:   0600656d    streq   r6, [r0], -sp, ror #10
    1398:   018501f0    strdeq  r0, [r5, r0]
    139c:   23020000    movwcs  r0, #8192   ; 0x2000
    13a0:   4377141e    cmnmi   r7, #503316480  ; 0x1e000000
    13a4:   626c6c61    rsbvs   r6, ip, #24832  ; 0x6100
    13a8:   496b6361    stmdbmi fp!, {r0, r5, r6, r8, r9, sp, lr}^
    13ac:   7265746e    rsbvc   r7, r5, #1845493760 ; 0x6e000000
    13b0:   006c6176    rsbeq   r6, ip, r6, ror r1
    13b4:   8501f106    strhi   pc, [r1, #-262]
    13b8:   02000001    andeq   r0, r0, #1  ; 0x1
    13bc:   70132023    andsvc  r2, r3, r3, lsr #32
            13bf: R_ARM_ABS32   .debug_str
    13c0:   06000001    streq   r0, [r0], -r1
    13c4:   017601f2    ldrsheq r0, [r6, #-18]!
    13c8:   23020000    movwcs  r0, #8192   ; 0x2000
    13cc:   017d1322    cmneq   sp, r2, lsr #6
            13ce: R_ARM_ABS32   .debug_str
    13d0:   f3060000    vhadd.u8    d0, d6, d0
    13d4:   00017601    andeq   r7, r1, r1, lsl #12
    13d8:   23230200    teqcs   r3, #0  ; 0x0
    13dc:   a4040700    strge   r0, [r4], #-1792
    13e0:   11000001    tstne   r0, r1
    13e4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    13e8:   6f73495f    svcvs   0x0073495f
    13ec:   72496863    subvc   r6, r9, #6488064    ; 0x630000
    13f0:   f4060070    vst4.16 {d0-d3}, [r6, :256], r0
    13f4:   0012d601    andseq  sp, r2, r1, lsl #12
    13f8:   554d1100    strbpl  r1, [sp, #-256]
    13fc:   705f4253    subsvc  r4, pc, r3, asr r2
    1400:   77654e66    strbvc  r4, [r5, -r6, ror #28]!
    1404:   53627355    cmnpl   r2, #1409286145 ; 0x54000001
    1408:   65746174    ldrbvs  r6, [r4, #-372]!
    140c:   02030600    andeq   r0, r3, #0  ; 0x0
    1410:   00001414    andeq   r1, r0, r4, lsl r4
    1414:   141a0407    ldrne   r0, [sl], #-1031
    1418:   010b0000    tsteq   fp, r0
    141c:   00001430    andeq   r1, r0, r0, lsr r4
    1420:   0000f10a    andeq   pc, r0, sl, lsl #2
    1424:   0f480a00    svceq   0x00480a00
    1428:   0a0a0000    beq 281430 <MGC_HdrcDumpEndpoint+0x27cb2c>
    142c:   0000000c    andeq   r0, r0, ip
    1430:   53554d11    cmppl   r5, #1088   ; 0x440
    1434:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1438:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    143c:   65526563    ldrbvs  r6, [r2, #-1379]
    1440:   73657571    cmnvc   r5, #473956352  ; 0x1c400000
    1444:   19060074    stmdbne r6, {r2, r4, r5, r6}
    1448:   00144d02    andseq  r4, r4, r2, lsl #26
    144c:   53040700    movwpl  r0, #18176  ; 0x4700
    1450:   09000014    stmdbeq r0, {r2, r4}
    1454:   00017601    andeq   r7, r1, r1, lsl #12
    1458:   00147700    andseq  r7, r4, r0, lsl #14
    145c:   00f10a00    rscseq  r0, r1, r0, lsl #20
    1460:   480a0000    stmdami sl, {}
    1464:   0a00000f    beq 14a8 <.debug_info+0x14a8>
    1468:   000001a4    andeq   r0, r0, r4, lsr #3
    146c:   0006670a    andeq   r6, r6, sl, lsl #14
    1470:   01850a00    orreq   r0, r5, r0, lsl #20
    1474:   11000000    tstne   r0, r0
    1478:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    147c:   4466705f    strbtmi r7, [r6], #-95
    1480:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    1484:   6e6f4365    cdpvs   3, 6, cr4, cr15, cr5, {3}
    1488:   53676966    cmnpl   r7, #1671168    ; 0x198000
    148c:   63656c65    cmnvs   r5, #25856  ; 0x6500
    1490:   00646574    rsbeq   r6, r4, r4, ror r5
    1494:   9b023706    blls    8f0b4 <MGC_HdrcDumpEndpoint+0x8a7b0>
    1498:   07000014    smladeq r0, r4, r0, r0
    149c:   0014a104    andseq  sl, r4, r4, lsl #2
    14a0:   76010900    strvc   r0, [r1], -r0, lsl #18
    14a4:   c0000001    andgt   r0, r0, r1
    14a8:   0a000014    beq 1500 <.debug_info+0x1500>
    14ac:   000000f1    strdeq  r0, [r0], -r1
    14b0:   000f480a    andeq   r4, pc, sl, lsl #16
    14b4:   01760a00    cmneq   r6, r0, lsl #20
    14b8:   c00a0000    andgt   r0, sl, r0
    14bc:   00000014    andeq   r0, r0, r4, lsl r0
    14c0:   102f0407    eorne   r0, pc, r7, lsl #8
    14c4:   4d110000    ldcmi   0, cr0, [r1]
    14c8:   5f425355    svcpl   0x00425355
    14cc:   6e496670    mcrvs   6, 2, r6, cr9, cr0, {3}
    14d0:   66726574    undefined
    14d4:   53656361    cmnpl   r5, #-2080374783    ; 0x84000001
    14d8:   06007465    streq   r7, [r0], -r5, ror #8
    14dc:   14e20256    strbtne r0, [r2], #598
    14e0:   04070000    streq   r0, [r7]
    14e4:   000014e8    andeq   r1, r0, r8, ror #9
    14e8:   01760109    cmneq   r6, r9, lsl #2
    14ec:   150c0000    strne   r0, [ip]
    14f0:   f10a0000    cpsie   ,#0
    14f4:   0a000000    beq 14fc <.debug_info+0x14fc>
    14f8:   00000f48    andeq   r0, r0, r8, asr #30
    14fc:   0001760a    andeq   r7, r1, sl, lsl #12
    1500:   01760a00    cmneq   r6, r0, lsl #20
    1504:   c00a0000    andgt   r0, sl, r0
    1508:   00000014    andeq   r0, r0, r4, lsl r0
    150c:   8e062c12    mcrhi   12, 0, r2, cr6, cr2, {0}
    1510:   00167d02    andseq  r7, r6, r2, lsl #26
    1514:   00ee1300    rsceq   r1, lr, r0, lsl #6
            1516: R_ARM_ABS32   .debug_str
    1518:   8f060000    svchi   0x00060000
    151c:   0000f102    andeq   pc, r0, r2, lsl #2
    1520:   00230200    eoreq   r0, r3, r0, lsl #4
    1524:   74537014    ldrbvc  r7, [r3], #-20
    1528:   61646e61    cmnvs   r4, r1, ror #28
    152c:   65446472    strbvs  r6, [r4, #-1138]
    1530:   69726373    ldmdbvs r2!, {r0, r1, r4, r5, r6, r8, r9, sp, lr}^
    1534:   726f7470    rsbvc   r7, pc, #1879048192 ; 0x70000000
    1538:   90060073    andls   r0, r6, r3, ror r0
    153c:   00066702    andeq   r6, r6, r2, lsl #14
    1540:   04230200    strteq  r0, [r3], #-512
    1544:   0001ae13    andeq   sl, r1, r3, lsl lr
            1545: R_ARM_ABS32   .debug_str
    1548:   02910600    addseq  r0, r1, #0  ; 0x0
    154c:   00000185    andeq   r0, r0, r5, lsl #3
    1550:   14082302    strne   r2, [r8], #-770
    1554:   72745377    rsbsvc  r5, r4, #-603979775 ; 0xdc000001
    1558:   44676e69    strbtmi r6, [r7], #-3689
    155c:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
    1560:   6f747069    svcvs   0x00747069
    1564:   756f4372    strbvc  r4, [pc, #-882]!    ; 11fa <.debug_info+0x11fa>
    1568:   0600746e    streq   r7, [r0], -lr, ror #8
    156c:   01850292    strbeq  r0, [r5, r2]
    1570:   23020000    movwcs  r0, #8192   ; 0x2000
    1574:   4870140a    ldmdami r0!, {r1, r3, sl, ip}^
    1578:   53686769    cmnpl   r8, #27525120   ; 0x1a40000
    157c:   64656570    strbtvs r6, [r5], #-1392
    1580:   63736544    cmnvs   r3, #285212672  ; 0x11000000
    1584:   74706972    ldrbtvc r6, [r0], #-2418
    1588:   0073726f    rsbseq  r7, r3, pc, ror #4
    158c:   67029306    strvs   r9, [r2, -r6, lsl #6]
    1590:   02000006    andeq   r0, r0, #6  ; 0x6
    1594:   77140c23    ldrvc   r0, [r4, -r3, lsr #24]
    1598:   68676948    stmdavs r7!, {r3, r6, r8, fp, sp, lr}^
    159c:   65657053    strbvs  r7, [r5, #-83]!
    15a0:   73654464    cmnvc   r5, #1677721600 ; 0x64000000
    15a4:   70697263    rsbvc   r7, r9, r3, ror #4
    15a8:   4c726f74    ldclmi  15, cr6, [r2], #-464
    15ac:   74676e65    strbtvc r6, [r7], #-3685
    15b0:   94060068    strls   r0, [r6], #-104
    15b4:   00018502    andeq   r8, r1, r2, lsl #10
    15b8:   10230200    eorne   r0, r3, r0, lsl #4
    15bc:   6f437714    svcvs   0x00437714
    15c0:   6f72746e    svcvs   0x0072746e
    15c4:   6675426c    ldrbtvs r4, [r5], -ip, ror #4
    15c8:   4c726566    cfldr64mi   mvdx6, [r2], #-408
    15cc:   74676e65    strbtvc r6, [r7], #-3685
    15d0:   95060068    strls   r0, [r6, #-104]
    15d4:   00018502    andeq   r8, r1, r2, lsl #10
    15d8:   12230200    eorne   r0, r3, #0  ; 0x0
    15dc:   6f437014    svcvs   0x00437014
    15e0:   6f72746e    svcvs   0x0072746e
    15e4:   6675426c    ldrbtvs r4, [r5], -ip, ror #4
    15e8:   00726566    rsbseq  r6, r2, r6, ror #10
    15ec:   28029606    stmdacs r2, {r1, r2, r9, sl, ip, pc}
    15f0:   0200000f    andeq   r0, r0, #15 ; 0xf
    15f4:   70141423    andsvc  r1, r4, r3, lsr #8
    15f8:   6c655362    stclvs  3, cr5, [r5], #-392
    15fc:   776f5066    strbvc  r5, [pc, -r6, rrx]!
    1600:   64657265    strbtvs r7, [r5], #-613
    1604:   02970600    addseq  r0, r7, #0  ; 0x0
    1608:   00000f28    andeq   r0, r0, r8, lsr #30
    160c:   14182302    ldrne   r2, [r8], #-770
    1610:   65446670    strbvs  r6, [r4, #-1648]
    1614:   65636976    strbvs  r6, [r3, #-2422]!
    1618:   75716552    ldrbvc  r6, [r1, #-1362]!
    161c:   00747365    rsbseq  r7, r4, r5, ror #6
    1620:   30029806    andcc   r9, r2, r6, lsl #16
    1624:   02000014    andeq   r0, r0, #20 ; 0x14
    1628:   70141c23    andsvc  r1, r4, r3, lsr #24
    162c:   76654466    strbtvc r4, [r5], -r6, ror #8
    1630:   43656369    cmnmi   r5, #-1543503871    ; 0xa4000001
    1634:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
    1638:   6c655367    stclvs  3, cr5, [r5], #-412
    163c:   65746365    ldrbvs  r6, [r4, #-869]!
    1640:   99060064    stmdbls r6, {r2, r5, r6}
    1644:   00147702    andseq  r7, r4, r2, lsl #14
    1648:   20230200    eorcs   r0, r3, r0, lsl #4
    164c:   49667014    stmdbmi r6!, {r2, r4, ip, sp, lr}^
    1650:   7265746e    rsbvc   r7, r5, #1845493760 ; 0x6e000000
    1654:   65636166    strbvs  r6, [r3, #-358]!
    1658:   00746553    rsbseq  r6, r4, r3, asr r5
    165c:   c6029a06    strgt   r9, [r2], -r6, lsl #20
    1660:   02000014    andeq   r0, r0, #20 ; 0x14
    1664:   70142423    andsvc  r2, r4, r3, lsr #8
    1668:   62735566    rsbsvs  r5, r3, #427819008  ; 0x19800000
    166c:   74617453    strbtvc r7, [r1], #-1107
    1670:   9b060065    blls    18180c <MGC_HdrcDumpEndpoint+0x17cf08>
    1674:   0013f902    andseq  pc, r3, r2, lsl #18
    1678:   28230200    stmdacs r3!, {r9}
    167c:   554d1100    strbpl  r1, [sp, #-256]
    1680:   465f4253    undefined
    1684:   74636e75    strbtvc r6, [r3], #-3701
    1688:   436e6f69    cmnmi   lr, #420    ; 0x1a4
    168c:   6e65696c    cdpvs   9, 6, cr6, cr5, cr12, {3}
    1690:   9c060074    stcls   0, cr0, [r6], {116}
    1694:   00150c02    andseq  r0, r5, r2, lsl #24
    1698:   554d1100    strbpl  r1, [sp, #-256]
    169c:   705f4253    subsvc  r4, pc, r3, asr r2
    16a0:   76654466    strbtvc r4, [r5], -r6, ror #8
    16a4:   43656369    cmnmi   r5, #-1543503871    ; 0xa4000001
    16a8:   656e6e6f    strbvs  r6, [lr, #-3695]!
    16ac:   64657463    strbtvs r7, [r5], #-1123
    16b0:   02ac0600    adceq   r0, ip, #0  ; 0x0
    16b4:   000016b8    strheq  r1, [r0], -r8
    16b8:   16be0407    ldrtne  r0, [lr], r7, lsl #8
    16bc:   01090000    tsteq   r9, r0
    16c0:   00000176    andeq   r0, r0, r6, ror r1
    16c4:   000016dd    ldrdeq  r1, [r0], -sp
    16c8:   0000f10a    andeq   pc, r0, sl, lsl #2
    16cc:   0f480a00    svceq   0x00480a00
    16d0:   dd0a0000    stcle   0, cr0, [sl]
    16d4:   0a000016    beq 1734 <.debug_info+0x1734>
    16d8:   00000667    andeq   r0, r0, r7, ror #12
    16dc:   34040700    strcc   r0, [r4], #-1792
    16e0:   1100000f    tstne   r0, pc
    16e4:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    16e8:   4466705f    strbtmi r7, [r6], #-95
    16ec:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    16f0:   73694465    cmnvc   r9, #1694498816 ; 0x65000000
    16f4:   6e6e6f63    cdpvs   15, 6, cr6, cr14, cr3, {3}
    16f8:   65746365    ldrbvs  r6, [r4, #-869]!
    16fc:   b6060064    strlt   r0, [r6], -r4, rrx
    1700:   00170502    andseq  r0, r7, r2, lsl #10
    1704:   0b040700    bleq    10330c <MGC_HdrcDumpEndpoint+0xfea08>
    1708:   0b000017    bleq    176c <.debug_info+0x176c>
    170c:   00172101    andseq  r2, r7, r1, lsl #2
    1710:   00f10a00    rscseq  r0, r1, r0, lsl #20
    1714:   480a0000    stmdami sl, {}
    1718:   0a00000f    beq 175c <.debug_info+0x175c>
    171c:   000016dd    ldrdeq  r1, [r0], -sp
    1720:   554d1100    strbpl  r1, [sp, #-256]
    1724:   705f4253    subsvc  r4, pc, r3, asr r2
    1728:   69724466    ldmdbvs r2!, {r1, r2, r5, r6, sl, lr}^
    172c:   54726576    ldrbtpl r6, [r2], #-1398
    1730:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
    1734:   69707845    ldmdbvs r0!, {r0, r2, r6, fp, ip, sp, lr}^
    1738:   00646572    rsbeq   r6, r4, r2, ror r5
    173c:   4302be06    movwmi  fp, #11782  ; 0x2e06
    1740:   07000017    smladeq r0, r7, r0, r0
    1744:   00174904    andseq  r4, r7, r4, lsl #18
    1748:   5a010b00    bpl 44350 <MGC_HdrcDumpEndpoint+0x3fa4c>
    174c:   0a000017    beq 17b0 <.debug_info+0x17b0>
    1750:   000000f1    strdeq  r0, [r0], -r1
    1754:   000f480a    andeq   r4, pc, sl, lsl #16
    1758:   4d110000    ldcmi   0, cr0, [r1]
    175c:   5f425355    svcpl   0x00425355
    1760:   75426670    strbvc  r6, [r2, #-1648]
    1764:   73755373    cmnvc   r5, #-872415231 ; 0xcc000001
    1768:   646e6570    strbtvs r6, [lr], #-1392
    176c:   06006465    streq   r6, [r0], -r5, ror #8
    1770:   174302c6    strbne  r0, [r3, -r6, asr #5]
    1774:   4d110000    ldcmi   0, cr0, [r1]
    1778:   5f425355    svcpl   0x00425355
    177c:   75426670    strbvc  r6, [r2, #-1648]
    1780:   73655273    cmnvc   r5, #805306375  ; 0x30000007
    1784:   64656d75    strbtvs r6, [r5], #-3445
    1788:   02ce0600    sbceq   r0, lr, #0  ; 0x0
    178c:   00001743    andeq   r1, r0, r3, asr #14
    1790:   554d5f18    strbpl  r5, [sp, #-3864]
    1794:   445f4253    ldrbmi  r4, [pc], #595  ; 179c <.debug_info+0x179c>
    1798:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    179c:   69724465    ldmdbvs r2!, {r0, r2, r5, r6, sl, lr}^
    17a0:   00726576    rsbseq  r6, r2, r6, ror r5
    17a4:   02e4061c    rsceq   r0, r4, #29360128   ; 0x1c00000
    17a8:   00001851    andeq   r1, r0, r1, asr r8
    17ac:   0000ee13    andeq   lr, r0, r3, lsl lr
            17ad: R_ARM_ABS32   .debug_str
    17b0:   02e50600    rsceq   r0, r5, #0  ; 0x0
    17b4:   000000f1    strdeq  r0, [r0], -r1
    17b8:   14002302    strne   r2, [r0], #-770
    17bc:   6d695462    cfstrdvs    mvd5, [r9, #-392]!
    17c0:   6f437265    svcvs   0x00437265
    17c4:   00746e75    rsbseq  r6, r4, r5, ror lr
    17c8:   7602e606    strvc   lr, [r2], -r6, lsl #12
    17cc:   02000001    andeq   r0, r0, #1  ; 0x1
    17d0:   5c130423    cfldrspl    mvf0, [r3], {35}
            17d3: R_ARM_ABS32   .debug_str
    17d4:   06000001    streq   r0, [r0], -r1
    17d8:   13dd02e7    bicsne  r0, sp, #1879048206 ; 0x7000000e
    17dc:   23020000    movwcs  r0, #8192   ; 0x2000
    17e0:   66701408    ldrbtvs r1, [r0], -r8, lsl #8
    17e4:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
    17e8:   6f436563    svcvs   0x00436563
    17ec:   63656e6e    cmnvs   r5, #1760   ; 0x6e0
    17f0:   00646574    rsbeq   r6, r4, r4, ror r5
    17f4:   9902e806    stmdbls r2, {r1, r2, fp, sp, lr, pc}
    17f8:   02000016    andeq   r0, r0, #22 ; 0x16
    17fc:   70140c23    andsvc  r0, r4, r3, lsr #24
    1800:   76654466    strbtvc r4, [r5], -r6, ror #8
    1804:   44656369    strbtmi r6, [r5], #-873
    1808:   6f637369    svcvs   0x00637369
    180c:   63656e6e    cmnvs   r5, #1760   ; 0x6e0
    1810:   00646574    rsbeq   r6, r4, r4, ror r5
    1814:   e302e906    movw    lr, #10502  ; 0x2906
    1818:   02000016    andeq   r0, r0, #22 ; 0x16
    181c:   70141023    andsvc  r1, r4, r3, lsr #32
    1820:   73754266    cmnvc   r5, #1610612742 ; 0x60000006
    1824:   70737553    rsbsvc  r7, r3, r3, asr r5
    1828:   65646e65    strbvs  r6, [r4, #-3685]!
    182c:   ea060064    b   1819c4 <MGC_HdrcDumpEndpoint+0x17d0c0>
    1830:   00175a02    andseq  r5, r7, r2, lsl #20
    1834:   14230200    strtne  r0, [r3], #-512
    1838:   42667014    rsbmi   r7, r6, #20 ; 0x14
    183c:   65527375    ldrbvs  r7, [r2, #-885]
    1840:   656d7573    strbvs  r7, [sp, #-1395]!
    1844:   eb060064    bl  1819dc <MGC_HdrcDumpEndpoint+0x17d0d8>
    1848:   00177602    andseq  r7, r7, r2, lsl #12
    184c:   18230200    stmdane r3!, {r9}
    1850:   554d1100    strbpl  r1, [sp, #-256]
    1854:   445f4253    ldrbmi  r4, [pc], #595  ; 185c <.debug_info+0x185c>
    1858:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    185c:   69724465    ldmdbvs r2!, {r0, r2, r5, r6, sl, lr}^
    1860:   00726576    rsbseq  r6, r2, r6, ror r5
    1864:   9002ec06    andls   lr, r2, r6, lsl #24
    1868:   12000017    andne   r0, r0, #23 ; 0x17
    186c:   02fd0610    rscseq  r0, sp, #16777216   ; 0x1000000
    1870:   000018f1    strdeq  r1, [r0], -r1
    1874:   65507014    ldrbvs  r7, [r0, #-20]
    1878:   68706972    ldmdavs r0!, {r1, r4, r5, r6, r8, fp, sp, lr}^
    187c:   6c617265    sfmvs   f7, 2, [r1], #-404
    1880:   7473694c    ldrbtvc r6, [r3], #-2380
    1884:   02fe0600    rscseq  r0, lr, #0  ; 0x0
    1888:   00000667    andeq   r0, r0, r7, ror #12
    188c:   14002302    strne   r2, [r0], #-770
    1890:   72655077    rsbvc   r5, r5, #119    ; 0x77
    1894:   65687069    strbvs  r7, [r8, #-105]!
    1898:   4c6c6172    stfmie  f6, [ip], #-456
    189c:   4c747369    ldclmi  3, cr7, [r4], #-420
    18a0:   74676e65    strbtvc r6, [r7], #-3685
    18a4:   ff060068    undefined instruction 0xff060068
    18a8:   00018502    andeq   r8, r1, r2, lsl #10
    18ac:   04230200    strteq  r0, [r3], #-512
    18b0:   65446114    strbvs  r6, [r4, #-276]
    18b4:   65636976    strbvs  r6, [r3, #-2422]!
    18b8:   76697244    strbtvc r7, [r9], -r4, asr #4
    18bc:   694c7265    stmdbvs ip, {r0, r2, r5, r6, r9, ip, sp, lr}^
    18c0:   06007473    undefined
    18c4:   18f10300    ldmne   r1!, {r8, r9}^
    18c8:   23020000    movwcs  r0, #8192   ; 0x2000
    18cc:   44621408    strbtmi r1, [r2], #-1032
    18d0:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    18d4:   69724465    ldmdbvs r2!, {r0, r2, r5, r6, sl, lr}^
    18d8:   4c726576    cfldr64mi   mvdx6, [r2], #-472
    18dc:   4c747369    ldclmi  3, cr7, [r4], #-420
    18e0:   74676e65    strbtvc r6, [r7], #-3685
    18e4:   01060068    tsteq   r6, r8, rrx
    18e8:   00017603    andeq   r7, r1, r3, lsl #12
    18ec:   0c230200    sfmeq   f0, 4, [r3]
    18f0:   51040700    tstpl   r4, r0, lsl #14
    18f4:   11000018    tstne   r0, r8, lsl r0
    18f8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    18fc:   736f485f    cmnvc   pc, #6225920    ; 0x5f0000
    1900:   696c4374    stmdbvs ip!, {r2, r4, r5, r6, r8, r9, lr}^
    1904:   00746e65    rsbseq  r6, r4, r5, ror #28
    1908:   6b030206    blvs    c2128 <MGC_HdrcDumpEndpoint+0xbd824>
    190c:   11000018    tstne   r0, r8, lsl r0
    1910:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1914:   4e66705f    mcrmi   0, 3, r7, cr6, cr15, {2}
    1918:   744f7765    strbvc  r7, [pc], #1893 ; 1920 <.debug_info+0x1920>
    191c:   61745367    cmnvs   r4, r7, ror #6
    1920:   06006574    undefined
    1924:   192a030c    stmdbne sl!, {r2, r3, r8, r9}
    1928:   04070000    streq   r0, [r7]
    192c:   00001930    andeq   r1, r0, r0, lsr r9
    1930:   1946010b    stmdbne r6, {r0, r1, r3, r8}^
    1934:   f10a0000    cpsie   ,#0
    1938:   0a000000    beq 1940 <.debug_info+0x1940>
    193c:   00000f48    andeq   r0, r0, r8, asr #30
    1940:   000ce70a    andeq   lr, ip, sl, lsl #14
    1944:   4d110000    ldcmi   0, cr0, [r1]
    1948:   5f425355    svcpl   0x00425355
    194c:   744f6670    strbvc  r6, [pc], #1648 ; 1954 <.debug_info+0x1954>
    1950:   72724567    rsbsvc  r4, r2, #432013312  ; 0x19c00000
    1954:   0600726f    streq   r7, [r0], -pc, ror #4
    1958:   195e0317    ldmdbne lr, {r0, r1, r2, r4, r8, r9}^
    195c:   04070000    streq   r0, [r7]
    1960:   00001964    andeq   r1, r0, r4, ror #18
    1964:   197a010b    ldmdbne sl!, {r0, r1, r3, r8}^
    1968:   f10a0000    cpsie   ,#0
    196c:   0a000000    beq 1974 <.debug_info+0x1974>
    1970:   00000f48    andeq   r0, r0, r8, asr #30
    1974:   0001a40a    andeq   sl, r1, sl, lsl #8
    1978:   10120000    andsne  r0, r2, r0
    197c:   db032906    blle    cbd9c <MGC_HdrcDumpEndpoint+0xc7498>
    1980:   13000019    movwne  r0, #25 ; 0x19
    1984:   000000ee    andeq   r0, r0, lr, ror #1
            1984: R_ARM_ABS32   .debug_str
    1988:   f1032a06    undefined instruction 0xf1032a06
    198c:   02000000    andeq   r0, r0, #0  ; 0x0
    1990:   70140023    andsvc  r0, r4, r3, lsr #32
    1994:   73654462    cmnvc   r5, #1644167168 ; 0x62000000
    1998:   48657269    stmdami r5!, {r0, r3, r5, r6, r9, ip, sp, lr}^
    199c:   5274736f    rsbspl  r7, r4, #-1140850687    ; 0xbc000001
    19a0:   00656c6f    rsbeq   r6, r5, pc, ror #24
    19a4:   28032b06    stmdacs r3, {r1, r2, r8, r9, fp, sp}
    19a8:   0200000f    andeq   r0, r0, #15 ; 0xf
    19ac:   70140423    andsvc  r0, r4, r3, lsr #8
    19b0:   67744f66    ldrbvs  r4, [r4, -r6, ror #30]!
    19b4:   74617453    strbtvc r7, [r1], #-1107
    19b8:   2c060065    stccs   0, cr0, [r6], {101}
    19bc:   00190f03    andseq  r0, r9, r3, lsl #30
    19c0:   08230200    stmdaeq r3!, {r9}
    19c4:   4f667014    svcmi   0x00667014
    19c8:   72456774    subvc   r6, r5, #30408704   ; 0x1d00000
    19cc:   00726f72    rsbseq  r6, r2, r2, ror pc
    19d0:   46032d06    strmi   r2, [r3], -r6, lsl #26
    19d4:   02000019    andeq   r0, r0, #25 ; 0x19
    19d8:   11000c23    tstne   r0, r3, lsr #24
    19dc:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    19e0:   67744f5f    undefined
    19e4:   65696c43    strbvs  r6, [r9, #-3139]!
    19e8:   0600746e    streq   r7, [r0], -lr, ror #8
    19ec:   197a032e    ldmdbne sl!, {r1, r2, r3, r5, r8, r9}^
    19f0:   4d110000    ldcmi   0, cr0, [r1]
    19f4:   5f425355    svcpl   0x00425355
    19f8:   75486670    strbvc  r6, [r8, #-1648]
    19fc:   756e4562    strbvc  r4, [lr, #-1378]!
    1a00:   6172656d    cmnvs   r2, sp, ror #10
    1a04:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    1a08:   706d6f43    rsbvc   r6, sp, r3, asr #30
    1a0c:   6574656c    ldrbvs  r6, [r4, #-1388]!
    1a10:   03390600    teqeq   r9, #0  ; 0x0
    1a14:   00001a18    andeq   r1, r0, r8, lsl sl
    1a18:   1a1e0407    bne 782a3c <MGC_HdrcDumpEndpoint+0x77e138>
    1a1c:   010b0000    tsteq   fp, r0
    1a20:   00001a2f    andeq   r1, r0, pc, lsr #20
    1a24:   0016dd0a    andseq  sp, r6, sl, lsl #26
    1a28:   16dd0a00    ldrbne  r0, [sp], r0, lsl #20
    1a2c:   0c000000    stceq   0, cr0, [r0], {0}
    1a30:   c92b0718    stmdbgt fp!, {r3, r4, r8, r9, sl}
    1a34:   0e00001a    mcreq   0, 0, r0, cr0, cr10, {0}
    1a38:   65744977    ldrbvs  r4, [r4, #-2423]!
    1a3c:   7a69536d    bvc 1a567f8 <MGC_HdrcDumpEndpoint+0x1a51ef4>
    1a40:   2c070065    stccs   0, cr0, [r7], {101}
    1a44:   000001c4    andeq   r0, r0, r4, asr #3
    1a48:   0e002302    cdpeq   3, 0, cr2, cr0, cr2, {0}
    1a4c:   61745377    cmnvs   r4, r7, ror r3
    1a50:   49636974    stmdbmi r3!, {r2, r4, r5, r6, r8, fp, sp, lr}^
    1a54:   436d6574    cmnmi   sp, #486539264  ; 0x1d000000
    1a58:   746e756f    strbtvc r7, [lr], #-1391
    1a5c:   c42d0700    strtgt  r0, [sp], #-1792
    1a60:   02000001    andeq   r0, r0, #1  ; 0x1
    1a64:   700e0423    andvc   r0, lr, r3, lsr #8
    1a68:   74617453    strbtvc r7, [r1], #-1107
    1a6c:   75426369    strbvc  r6, [r2, #-873]
    1a70:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    1a74:   f12e0700    undefined instruction 0xf12e0700
    1a78:   02000000    andeq   r0, r0, #0  ; 0x0
    1a7c:   770e0823    strvc   r0, [lr, -r3, lsr #16]
    1a80:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
    1a84:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    1a88:   2f070074    svccs   0x00070074
    1a8c:   000001c4    andeq   r0, r0, r4, asr #3
    1a90:   0e0c2302    cdpeq   3, 0, cr2, cr12, cr2, {0}
    1a94:   6e794477    mrcvs   4, 3, r4, cr9, cr7, {3}
    1a98:   63696d61    cmnvs   r9, #6208   ; 0x1840
    1a9c:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
    1aa0:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    1aa4:   30070074    andcc   r0, r7, r4, ror r0
    1aa8:   000001c4    andeq   r0, r0, r4, asr #3
    1aac:   0e102302    cdpeq   3, 1, cr2, cr0, cr2, {0}
    1ab0:   6e794470    mrcvs   4, 3, r4, cr9, cr0, {3}
    1ab4:   63696d61    cmnvs   r9, #6208   ; 0x1840
    1ab8:   66667542    strbtvs r7, [r6], -r2, asr #10
    1abc:   07007265    streq   r7, [r0, -r5, ror #4]
    1ac0:   0000f131    andeq   pc, r0, r1, lsr r1
    1ac4:   14230200    strtne  r0, [r3], #-512
    1ac8:   554d0200    strbpl  r0, [sp, #-512]
    1acc:   415f4253    cmpmi   pc, r3, asr r2
    1ad0:   79617272    stmdbvc r1!, {r1, r4, r5, r6, r9, ip, sp, lr}^
    1ad4:   2f320700    svccs   0x00320700
    1ad8:   1700001a    smladne r0, sl, r0, r0
    1adc:   53554d5f    cmppl   r5, #6080   ; 0x17c0
    1ae0:   694c5f42    stmdbvs ip, {r1, r6, r8, r9, sl, fp, ip, lr}^
    1ae4:   64656b6e    strbtvs r6, [r5], #-2926
    1ae8:   7473694c    ldrbtvc r6, [r3], #-2380
    1aec:   6e070c00    cdpvs   12, 0, cr0, cr7, cr0, {0}
    1af0:   00001b21    andeq   r1, r0, r1, lsr #22
    1af4:   0000740d    andeq   r7, r0, sp, lsl #8
            1af5: R_ARM_ABS32   .debug_str
    1af8:   f16f0700    undefined instruction 0xf16f0700
    1afc:   02000000    andeq   r0, r0, #0  ; 0x0
    1b00:   700e0023    andvc   r0, lr, r3, lsr #32
    1b04:   7478654e    ldrbtvc r6, [r8], #-1358
    1b08:   21700700    cmncs   r0, r0, lsl #14
    1b0c:   0200001b    andeq   r0, r0, #27 ; 0x1b
    1b10:   000d0423    andeq   r0, sp, r3, lsr #8
            1b13: R_ARM_ABS32   .debug_str
    1b14:   07000000    streq   r0, [r0, -r0]
    1b18:   00009471    andeq   r9, r0, r1, ror r4
    1b1c:   08230200    stmdaeq r3!, {r9}
    1b20:   db040700    blle    103728 <MGC_HdrcDumpEndpoint+0xfee24>
    1b24:   0200001a    andeq   r0, r0, #26 ; 0x1a
    1b28:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1b2c:   6e694c5f    mcrvs   12, 3, r4, cr9, cr15, {2}
    1b30:   4c64656b    cfstr64mi   mvdx6, [r4], #-428
    1b34:   00747369    rsbseq  r7, r4, r9, ror #6
    1b38:   1adb7207    bne ff6de35c <MGC_HdrcDumpEndpoint+0xff6d9a58>
    1b3c:   04150000    ldreq   r0, [r5]
    1b40:   1c651e08    stclne  14, cr1, [r5], #-32
    1b44:   4d160000    ldcmi   0, cr0, [r6]
    1b48:   5f425355    svcpl   0x00425355
    1b4c:   54455348    strbpl  r5, [r5], #-840
    1b50:   524f505f    subpl   r5, pc, #95 ; 0x5f
    1b54:   4f4e5f54    svcmi   0x004e5f54
    1b58:   0000454e    andeq   r4, r0, lr, asr #10
    1b5c:   53554d16    cmppl   r5, #1408   ; 0x580
    1b60:   53485f42    movtpl  r5, #36674  ; 0x8f42
    1b64:   505f5445    subspl  r5, pc, r5, asr #8
    1b68:   5f54524f    svcpl   0x0054524f
    1b6c:   54534554    ldrbpl  r4, [r3], #-1364
    1b70:   01004a5f    tsteq   r0, pc, asr sl
    1b74:   53554d16    cmppl   r5, #1408   ; 0x580
    1b78:   53485f42    movtpl  r5, #36674  ; 0x8f42
    1b7c:   505f5445    subspl  r5, pc, r5, asr #8
    1b80:   5f54524f    svcpl   0x0054524f
    1b84:   54534554    ldrbpl  r4, [r3], #-1364
    1b88:   02004b5f    andeq   r4, r0, #97280  ; 0x17c00
    1b8c:   53554d16    cmppl   r5, #1408   ; 0x580
    1b90:   53485f42    movtpl  r5, #36674  ; 0x8f42
    1b94:   505f5445    subspl  r5, pc, r5, asr #8
    1b98:   5f54524f    svcpl   0x0054524f
    1b9c:   54534554    ldrbpl  r4, [r3], #-1364
    1ba0:   3045535f    subcc   r5, r5, pc, asr r3
    1ba4:   4b414e5f    blmi    1055528 <MGC_HdrcDumpEndpoint+0x1050c24>
    1ba8:   4d160300    ldcmi   3, cr0, [r6]
    1bac:   5f425355    svcpl   0x00425355
    1bb0:   54455348    strbpl  r5, [r5], #-840
    1bb4:   524f505f    subpl   r5, pc, #95 ; 0x5f
    1bb8:   45545f54    ldrbmi  r5, [r4, #-3924]
    1bbc:   505f5453    subspl  r5, pc, r3, asr r4
    1bc0:   454b4341    strbmi  r4, [fp, #-833]
    1bc4:   16040054    undefined
    1bc8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1bcc:   4553485f    ldrbmi  r4, [r3, #-2143]
    1bd0:   4f505f54    svcmi   0x00505f54
    1bd4:   525f5452    subspl  r5, pc, #1375731712 ; 0x52000000
    1bd8:   54455345    strbpl  r5, [r5], #-837
    1bdc:   4d160500    cfldr32mi   mvfx0, [r6]
    1be0:   5f425355    svcpl   0x00425355
    1be4:   54455348    strbpl  r5, [r5], #-840
    1be8:   524f505f    subpl   r5, pc, #95 ; 0x5f
    1bec:   55535f54    ldrbpl  r5, [r3, #-3924]
    1bf0:   4e455053    mcrmi   0, 2, r5, cr5, cr3, {2}
    1bf4:   16060044    strne   r0, [r6], -r4, asr #32
    1bf8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1bfc:   4553485f    ldrbmi  r4, [r3, #-2143]
    1c00:   4f505f54    svcmi   0x00505f54
    1c04:   525f5452    subspl  r5, pc, #1375731712 ; 0x52000000
    1c08:   4d555345    ldclmi  3, cr5, [r5, #-276]
    1c0c:   16070045    strne   r0, [r7], -r5, asr #32
    1c10:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1c14:   4553485f    ldrbmi  r4, [r3, #-2143]
    1c18:   4f505f54    svcmi   0x00505f54
    1c1c:   535f5452    cmppl   pc, #1375731712 ; 0x52000000
    1c20:   50555445    subspl  r5, r5, r5, asr #8
    1c24:   4154535f    cmpmi   r4, pc, asr r3
    1c28:   08005452    stmdaeq r0, {r1, r4, r6, sl, ip, lr}
    1c2c:   53554d16    cmppl   r5, #1408   ; 0x580
    1c30:   53485f42    movtpl  r5, #36674  ; 0x8f42
    1c34:   505f5445    subspl  r5, pc, r5, asr #8
    1c38:   5f54524f    svcpl   0x0054524f
    1c3c:   55544553    ldrbpl  r4, [r4, #-1363]
    1c40:   4e495f50    mcrmi   15, 2, r5, cr9, cr0, {2}
    1c44:   4d160900    ldcmi   9, cr0, [r6]
    1c48:   5f425355    svcpl   0x00425355
    1c4c:   54455348    strbpl  r5, [r5], #-840
    1c50:   524f505f    subpl   r5, pc, #95 ; 0x5f
    1c54:   45535f54    ldrbmi  r5, [r3, #-3924]
    1c58:   5f505554    svcpl   0x00505554
    1c5c:   54415453    strbpl  r5, [r1], #-1107
    1c60:   0a005355    beq 169bc <MGC_HdrcDumpEndpoint+0x120b8>
    1c64:   554d0200    strbpl  r0, [sp, #-512]
    1c68:   485f4253    ldmdami pc, {r0, r1, r4, r6, r9, lr}^
    1c6c:   50746573    rsbspl  r6, r4, r3, ror r5
    1c70:   4d74726f    lfmmi   f7, 2, [r4, #-444]!
    1c74:   0065646f    rsbeq   r6, r5, pc, ror #8
    1c78:   1b3e3508    blne    f8f0a0 <MGC_HdrcDumpEndpoint+0xf8a79c>
    1c7c:   5f190000    svcpl   0x00190000
    1c80:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1c84:   616d445f    cmnvs   sp, pc, asr r4
    1c88:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    1c8c:   536c656e    cmnpl   ip, #461373440  ; 0x1b800000
    1c90:   75746174    ldrbvc  r6, [r4, #-372]!
    1c94:   09040073    stmdbeq r4, {r0, r1, r4, r5, r6}
    1c98:   001d1f39    andseq  r1, sp, r9, lsr pc
    1c9c:   554d1600    strbpl  r1, [sp, #-1536]
    1ca0:   445f4253    ldrbmi  r4, [pc], #595  ; 1ca8 <.debug_info+0x1ca8>
    1ca4:   535f414d    cmppl   pc, #1073741843 ; 0x40000013
    1ca8:   55544154    ldrbpl  r4, [r4, #-340]
    1cac:   4e555f53    mrcmi   15, 2, r5, cr5, cr3, {2}
    1cb0:   574f4e4b    strbpl  r4, [pc, -fp, asr #28]
    1cb4:   1600004e    strne   r0, [r0], -lr, asr #32
    1cb8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1cbc:   414d445f    cmpmi   sp, pc, asr r4
    1cc0:   4154535f    cmpmi   r4, pc, asr r3
    1cc4:   5f535554    svcpl   0x00535554
    1cc8:   45455246    strbmi  r5, [r5, #-582]
    1ccc:   4d160100    ldfmis  f0, [r6]
    1cd0:   5f425355    svcpl   0x00425355
    1cd4:   5f414d44    svcpl   0x00414d44
    1cd8:   54415453    strbpl  r5, [r1], #-1107
    1cdc:   425f5355    subsmi  r5, pc, #1409286145 ; 0x54000001
    1ce0:   00595355    subseq  r5, r9, r5, asr r3
    1ce4:   554d1602    strbpl  r1, [sp, #-1538]
    1ce8:   445f4253    ldrbmi  r4, [pc], #595  ; 1cf0 <.debug_info+0x1cf0>
    1cec:   535f414d    cmppl   pc, #1073741843 ; 0x40000013
    1cf0:   55544154    ldrbpl  r4, [r4, #-340]
    1cf4:   55425f53    strbpl  r5, [r2, #-3923]
    1cf8:   42415f53    submi   r5, r1, #332    ; 0x14c
    1cfc:   0054524f    subseq  r5, r4, pc, asr #4
    1d00:   554d1603    strbpl  r1, [sp, #-1539]
    1d04:   445f4253    ldrbmi  r4, [pc], #595  ; 1d0c <.debug_info+0x1d0c>
    1d08:   535f414d    cmppl   pc, #1073741843 ; 0x40000013
    1d0c:   55544154    ldrbpl  r4, [r4, #-340]
    1d10:   4f435f53    svcmi   0x00435f53
    1d14:   415f4552    cmpmi   pc, r2, asr r5
    1d18:   54524f42    ldrbpl  r4, [r2], #-3906
    1d1c:   02000400    andeq   r0, r0, #0  ; 0x0
    1d20:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1d24:   616d445f    cmnvs   sp, pc, asr r4
    1d28:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    1d2c:   536c656e    cmnpl   ip, #461373440  ; 0x1b800000
    1d30:   75746174    ldrbvc  r6, [r4, #-372]!
    1d34:   44090073    strmi   r0, [r9], #-115
    1d38:   00001c7e    andeq   r1, r0, lr, ror ip
    1d3c:   5309140c    movwpl  r1, #37900  ; 0x940c
    1d40:   00001da0    andeq   r1, r0, r0, lsr #27
    1d44:   0000ee0d    andeq   lr, r0, sp, lsl #28
            1d45: R_ARM_ABS32   .debug_str
    1d48:   f1540900    undefined instruction 0xf1540900
    1d4c:   02000000    andeq   r0, r0, #0  ; 0x0
    1d50:   640e0023    strvs   r0, [lr], #-35
    1d54:   78614d77    stmdavc r1!, {r0, r1, r2, r4, r5, r6, r8, sl, fp, lr}^
    1d58:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
    1d5c:   09006874    stmdbeq r0, {r2, r4, r5, r6, fp, sp, lr}
    1d60:   0001a455    andeq   sl, r1, r5, asr r4
    1d64:   04230200    strteq  r0, [r3], #-512
    1d68:   0000080d    andeq   r0, r0, sp, lsl #16
            1d69: R_ARM_ABS32   .debug_str
    1d6c:   a4560900    ldrbge  r0, [r6], #-2304
    1d70:   02000001    andeq   r0, r0, #1  ; 0x1
    1d74:   620e0823    andvs   r0, lr, #2293760    ; 0x230000
    1d78:   74617453    strbtvc r7, [r1], #-1107
    1d7c:   09007375    stmdbeq r0, {r0, r2, r4, r5, r6, r8, r9, ip, sp, lr}
    1d80:   001d1f57    andseq  r1, sp, r7, asr pc
    1d84:   0c230200    sfmeq   f0, 4, [r3]
    1d88:   6544620e    strbvs  r6, [r4, #-526]
    1d8c:   65726973    ldrbvs  r6, [r2, #-2419]!
    1d90:   646f4d64    strbtvs r4, [pc], #3428 ; 1d98 <.debug_info+0x1d98>
    1d94:   58090065    stmdapl r9, {r0, r2, r5, r6}
    1d98:   00000176    andeq   r0, r0, r6, ror r1
    1d9c:   00102302    andseq  r2, r0, r2, lsl #6
    1da0:   53554d02    cmppl   r5, #128    ; 0x80
    1da4:   6d445f42    stclvs  15, cr5, [r4, #-264]
    1da8:   61684361    cmnvs   r8, r1, ror #6
    1dac:   6c656e6e    stclvs  14, cr6, [r5], #-440
    1db0:   3c590900    mrrccc  9, 0, r0, r9, cr0
    1db4:   0200001d    andeq   r0, r0, #29 ; 0x1d
    1db8:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1dbc:   4466705f    strbtmi r7, [r6], #-95
    1dc0:   7453616d    ldrbvc  r6, [r3], #-365
    1dc4:   43747261    cmnmi   r4, #268435462  ; 0x10000006
    1dc8:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    1dcc:   656c6c6f    strbvs  r6, [ip, #-3183]!
    1dd0:   62090072    andvs   r0, r9, #114    ; 0x72
    1dd4:   00001dd8    ldrdeq  r1, [r0], -r8
    1dd8:   1dde0407    cfldrdne    mvd0, [lr, #28]
    1ddc:   01090000    tsteq   r9, r0
    1de0:   00000176    andeq   r0, r0, r6, ror r1
    1de4:   00001dee    andeq   r1, r0, lr, ror #27
    1de8:   0000f10a    andeq   pc, r0, sl, lsl #2
    1dec:   4d020000    stcmi   0, cr0, [r2]
    1df0:   5f425355    svcpl   0x00425355
    1df4:   6d446670    stclvs  6, cr6, [r4, #-448]
    1df8:   6f745361    svcvs   0x00745361
    1dfc:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    1e00:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    1e04:   0072656c    rsbseq  r6, r2, ip, ror #10
    1e08:   1dd86b09    vldrne  d22, [r8, #36]
    1e0c:   4d020000    stcmi   0, cr0, [r2]
    1e10:   5f425355    svcpl   0x00425355
    1e14:   6d446670    stclvs  6, cr6, [r4, #-448]
    1e18:   6c6c4161    stfvse  f4, [ip], #-388
    1e1c:   7461636f    strbtvc r6, [r1], #-879
    1e20:   61684365    cmnvs   r8, r5, ror #6
    1e24:   6c656e6e    stclvs  14, cr6, [r5], #-440
    1e28:   2f7b0900    svccs   0x007b0900
    1e2c:   0700001e    smladeq r0, lr, r0, r0
    1e30:   001e3504    andseq  r3, lr, r4, lsl #10
    1e34:   59010900    stmdbpl r1, {r8, fp}
    1e38:   5900001e    stmdbpl r0, {r1, r2, r3, r4}
    1e3c:   0a00001e    beq 1ebc <.debug_info+0x1ebc>
    1e40:   000000f1    strdeq  r0, [r0], -r1
    1e44:   0001760a    andeq   r7, r1, sl, lsl #12
    1e48:   01760a00    cmneq   r6, r0, lsl #20
    1e4c:   760a0000    strvc   r0, [sl], -r0
    1e50:   0a000001    beq 1e5c <.debug_info+0x1e5c>
    1e54:   00000185    andeq   r0, r0, r5, lsl #3
    1e58:   a0040700    andge   r0, r4, r0, lsl #14
    1e5c:   0200001d    andeq   r0, r0, #29 ; 0x1d
    1e60:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    1e64:   4466705f    strbtmi r7, [r6], #-95
    1e68:   6552616d    ldrbvs  r6, [r2, #-365]
    1e6c:   7361656c    cmnvc   r1, #452984832  ; 0x1b000000
    1e70:   61684365    cmnvs   r8, r5, ror #6
    1e74:   6c656e6e    stclvs  14, cr6, [r5], #-440
    1e78:   7f840900    svcvc   0x00840900
    1e7c:   0700001e    smladeq r0, lr, r0, r0
    1e80:   001e8504    andseq  r8, lr, r4, lsl #10
    1e84:   91010b00    tstls   r1, r0, lsl #22
    1e88:   0a00001e    beq 1f08 <.debug_info+0x1f08>
    1e8c:   00001e59    andeq   r1, r0, r9, asr lr
    1e90:   554d0200    strbpl  r0, [sp, #-512]
    1e94:   705f4253    subsvc  r4, pc, r3, asr r2
    1e98:   616d4466    cmnvs   sp, r6, ror #8
    1e9c:   6f6c6c41    svcvs   0x006c6c41
    1ea0:   65746163    ldrbvs  r6, [r4, #-355]!
    1ea4:   66667542    strbtvs r7, [r6], -r2, asr #10
    1ea8:   09007265    stmdbeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    1eac:   001eb191    mulseq  lr, r1, r1
    1eb0:   b7040700    strlt   r0, [r4, -r0, lsl #14]
    1eb4:   0900001e    stmdbeq r0, {r1, r2, r3, r4}
    1eb8:   000f2801    andeq   r2, pc, r1, lsl #16
    1ebc:   001ecc00    andseq  ip, lr, r0, lsl #24
    1ec0:   1e590a00    fmscsne s1, s18, s0
    1ec4:   a40a0000    strge   r0, [sl]
    1ec8:   00000001    andeq   r0, r0, r1
    1ecc:   53554d02    cmppl   r5, #128    ; 0x80
    1ed0:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1ed4:   52616d44    rsbpl   r6, r1, #4352   ; 0x1100
    1ed8:   61656c65    cmnvs   r5, r5, ror #24
    1edc:   75426573    strbvc  r6, [r2, #-1395]
    1ee0:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    1ee4:   eb9e0900    bl  fe7842ec <MGC_HdrcDumpEndpoint+0xfe77f9e8>
    1ee8:   0700001e    smladeq r0, lr, r0, r0
    1eec:   001ef104    andseq  pc, lr, r4, lsl #2
    1ef0:   76010900    strvc   r0, [r1], -r0, lsl #18
    1ef4:   06000001    streq   r0, [r0], -r1
    1ef8:   0a00001f    beq 1f7c <.debug_info+0x1f7c>
    1efc:   00001e59    andeq   r1, r0, r9, asr lr
    1f00:   000f280a    andeq   r2, pc, sl, lsl #16
    1f04:   4d020000    stcmi   0, cr0, [r2]
    1f08:   5f425355    svcpl   0x00425355
    1f0c:   6d446670    stclvs  6, cr6, [r4, #-448]
    1f10:   6f725061    svcvs   0x00725061
    1f14:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    1f18:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    1f1c:   006c656e    rsbeq   r6, ip, lr, ror #10
    1f20:   1f26b209    svcne   0x0026b209
    1f24:   04070000    streq   r0, [r7]
    1f28:   00001f2c    andeq   r1, r0, ip, lsr #30
    1f2c:   01760109    cmneq   r6, r9, lsl #2
    1f30:   1f500000    svcne   0x00500000
    1f34:   590a0000    stmdbpl sl, {}
    1f38:   0a00001e    beq 1fb8 <.debug_info+0x1fb8>
    1f3c:   00000185    andeq   r0, r0, r5, lsl #3
    1f40:   0001760a    andeq   r7, r1, sl, lsl #12
    1f44:   06670a00    strbteq r0, [r7], -r0, lsl #20
    1f48:   a40a0000    strge   r0, [sl]
    1f4c:   00000001    andeq   r0, r0, r1
    1f50:   53554d02    cmppl   r5, #128    ; 0x80
    1f54:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1f58:   47616d44    strbmi  r6, [r1, -r4, asr #26]!
    1f5c:   68437465    stmdavs r3, {r0, r2, r5, r6, sl, ip, sp, lr}^
    1f60:   656e6e61    strbvs  r6, [lr, #-3681]!
    1f64:   6174536c    cmnvs   r4, ip, ror #6
    1f68:   00737574    rsbseq  r7, r3, r4, ror r5
    1f6c:   1f72bd09    svcne   0x0072bd09
    1f70:   04070000    streq   r0, [r7]
    1f74:   00001f78    andeq   r1, r0, r8, ror pc
    1f78:   1d1f0109    ldfnes  f0, [pc, #-36]
    1f7c:   1f880000    svcne   0x00880000
    1f80:   590a0000    stmdbpl sl, {}
    1f84:   0000001e    andeq   r0, r0, lr, lsl r0
    1f88:   53554d02    cmppl   r5, #128    ; 0x80
    1f8c:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    1f90:   43616d44    cmnmi   r1, #4352   ; 0x1100
    1f94:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    1f98:   656c6c6f    strbvs  r6, [ip, #-3183]!
    1f9c:   72734972    rsbsvc  r4, r3, #1867776    ; 0x1c8000
    1fa0:   d8d10900    ldmle   r1, {r8, fp}^
    1fa4:   0c00001d    stceq   0, cr0, [r0], {29}
    1fa8:   cee60928    cdpgt   9, 14, cr0, cr6, cr8, {1}
    1fac:   0d000020    stceq   0, cr0, [r0, #-128]
    1fb0:   000000ee    andeq   r0, r0, lr, ror #1
            1fb0: R_ARM_ABS32   .debug_str
    1fb4:   00f1e709    rscseq  lr, r1, r9, lsl #14
    1fb8:   23020000    movwcs  r0, #8192   ; 0x2000
    1fbc:   66700e00    ldrbtvs r0, [r0], -r0, lsl #28
    1fc0:   53616d44    cmnpl   r1, #4352   ; 0x1100
    1fc4:   74726174    ldrbtvc r6, [r2], #-372
    1fc8:   746e6f43    strbtvc r6, [lr], #-3907
    1fcc:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    1fd0:   09007265    stmdbeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    1fd4:   001db7e8    andseq  fp, sp, r8, ror #15
    1fd8:   04230200    strteq  r0, [r3], #-512
    1fdc:   4466700e    strbtmi r7, [r6], #-14
    1fe0:   7453616d    ldrbvc  r6, [r3], #-365
    1fe4:   6f43706f    svcvs   0x0043706f
    1fe8:   6f72746e    svcvs   0x0072746e
    1fec:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    1ff0:   eee90900    cdp 9, 14, cr0, cr9, cr0, {0}
    1ff4:   0200001d    andeq   r0, r0, #29 ; 0x1d
    1ff8:   700e0823    andvc   r0, lr, r3, lsr #16
    1ffc:   616d4466    cmnvs   sp, r6, ror #8
    2000:   6f6c6c41    svcvs   0x006c6c41
    2004:   65746163    ldrbvs  r6, [r4, #-355]!
    2008:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    200c:   006c656e    rsbeq   r6, ip, lr, ror #10
    2010:   1e0eea09    fmacsne s28, s28, s18
    2014:   23020000    movwcs  r0, #8192   ; 0x2000
    2018:   66700e0c    ldrbtvs r0, [r0], -ip, lsl #28
    201c:   52616d44    rsbpl   r6, r1, #4352   ; 0x1100
    2020:   61656c65    cmnvs   r5, r5, ror #24
    2024:   68436573    stmdavs r3, {r0, r1, r4, r5, r6, r8, sl, sp, lr}^
    2028:   656e6e61    strbvs  r6, [lr, #-3681]!
    202c:   eb09006c    bl  2421e4 <MGC_HdrcDumpEndpoint+0x23d8e0>
    2030:   00001e5f    andeq   r1, r0, pc, asr lr
    2034:   0e102302    cdpeq   3, 1, cr2, cr0, cr2, {0}
    2038:   6d446670    stclvs  6, cr6, [r4, #-448]
    203c:   6c6c4161    stfvse  f4, [ip], #-388
    2040:   7461636f    strbtvc r6, [r1], #-879
    2044:   66754265    ldrbtvs r4, [r5], -r5, ror #4
    2048:   00726566    rsbseq  r6, r2, r6, ror #10
    204c:   1e91ec09    cdpne   12, 9, cr14, cr1, cr9, {0}
    2050:   23020000    movwcs  r0, #8192   ; 0x2000
    2054:   66700e14    undefined
    2058:   52616d44    rsbpl   r6, r1, #4352   ; 0x1100
    205c:   61656c65    cmnvs   r5, r5, ror #24
    2060:   75426573    strbvc  r6, [r2, #-1395]
    2064:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    2068:   cced0900    stclgt  9, cr0, [sp]
    206c:   0200001e    andeq   r0, r0, #30 ; 0x1e
    2070:   700e1823    andvc   r1, lr, r3, lsr #16
    2074:   616d4466    cmnvs   sp, r6, ror #8
    2078:   676f7250    undefined
    207c:   436d6172    cmnmi   sp, #-2147483620    ; 0x8000001c
    2080:   6e6e6168    powvsez f6, f6, #0.0
    2084:   09006c65    stmdbeq r0, {r0, r2, r5, r6, sl, fp, sp, lr}
    2088:   001f06ee    andseq  r0, pc, lr, ror #13
    208c:   1c230200    sfmne   f0, 4, [r3]
    2090:   4466700e    strbtmi r7, [r6], #-14
    2094:   6547616d    strbvs  r6, [r7, #-365]
    2098:   61684374    smcvs   33844
    209c:   6c656e6e    stclvs  14, cr6, [r5], #-440
    20a0:   74617453    strbtvc r7, [r1], #-1107
    20a4:   09007375    stmdbeq r0, {r0, r2, r4, r5, r6, r8, r9, ip, sp, lr}
    20a8:   001f50ef    andseq  r5, pc, pc, ror #1
    20ac:   20230200    eorcs   r0, r3, r0, lsl #4
    20b0:   4466700e    strbtmi r7, [r6], #-14
    20b4:   6f43616d    svcvs   0x0043616d
    20b8:   6f72746e    svcvs   0x0072746e
    20bc:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    20c0:   00727349    rsbseq  r7, r2, r9, asr #6
    20c4:   1f88f009    svcne   0x0088f009
    20c8:   23020000    movwcs  r0, #8192   ; 0x2000
    20cc:   4d020024    stcmi   0, cr0, [r2, #-144]
    20d0:   5f425355    svcpl   0x00425355
    20d4:   43616d44    cmnmi   r1, #4352   ; 0x1100
    20d8:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    20dc:   656c6c6f    strbvs  r6, [ip, #-3183]!
    20e0:   f1090072    undefined instruction 0xf1090072
    20e4:   00001fa7    andeq   r1, r0, r7, lsr #31
    20e8:   53554d11    cmppl   r5, #1088   ; 0x440
    20ec:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    20f0:   43616d44    cmnmi   r1, #4352   ; 0x1100
    20f4:   6e6e6168    powvsez f6, f6, #0.0
    20f8:   74536c65    ldrbvc  r6, [r3], #-3173
    20fc:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
    2100:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    2104:   00646567    rsbeq   r6, r4, r7, ror #10
    2108:   0f010109    svceq   0x00010109
    210c:   07000021    streq   r0, [r0, -r1, lsr #32]
    2110:   00211504    eoreq   r1, r1, r4, lsl #10
    2114:   76010900    strvc   r0, [r1], -r0, lsl #18
    2118:   2f000001    svccs   0x00000001
    211c:   0a000021    beq 21a8 <.debug_info+0x21a8>
    2120:   000000f1    strdeq  r0, [r0], -r1
    2124:   0001760a    andeq   r7, r1, sl, lsl #12
    2128:   01760a00    cmneq   r6, r0, lsl #20
    212c:   11000000    tstne   r0, r0
    2130:   4253554d    subsmi  r5, r3, #322961408  ; 0x13400000
    2134:   4e66705f    mcrmi   0, 3, r7, cr6, cr15, {2}
    2138:   6d447765    stclvs  7, cr7, [r4, #-404]
    213c:   6e6f4361    cdpvs   3, 6, cr4, cr15, cr1, {3}
    2140:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    2144:   0072656c    rsbseq  r6, r2, ip, ror #10
    2148:   4f012209    svcmi   0x00012209
    214c:   07000021    streq   r0, [r0, -r1, lsr #32]
    2150:   00215504    eoreq   r5, r1, r4, lsl #10
    2154:   7e010900    cdpvc   9, 0, cr0, cr1, cr0, {0}
    2158:   7e000021    cdpvc   0, 0, cr0, cr0, cr1, {1}
    215c:   0a000021    beq 21e8 <.debug_info+0x21e8>
    2160:   000020e8    andeq   r2, r0, r8, ror #1
    2164:   0000f10a    andeq   pc, r0, sl, lsl #2
    2168:   04680a00    strbteq r0, [r8], #-2560
    216c:   f10a0000    cpsie   ,#0
    2170:   0a000000    beq 2178 <.debug_info+0x2178>
    2174:   00000f28    andeq   r0, r0, r8, lsr #30
    2178:   000f280a    andeq   r2, pc, sl, lsl #16
    217c:   04070000    streq   r0, [r7]
    2180:   000020ce    andeq   r2, r0, lr, asr #1
    2184:   53554d11    cmppl   r5, #1088   ; 0x440
    2188:   66705f42    ldrbtvs r5, [r0], -r2, asr #30
    218c:   74736544    ldrbtvc r6, [r3], #-1348
    2190:   44796f72    ldrbtmi r6, [r9], #-3954
    2194:   6f43616d    svcvs   0x0043616d
    2198:   6f72746e    svcvs   0x0072746e
    219c:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    21a0:   01280900    teqeq   r8, r0, lsl #18
    21a4:   000021a8    andeq   r2, r0, r8, lsr #3
    21a8:   21ae0407    undefined instruction 0x21ae0407
    21ac:   010b0000    tsteq   fp, r0
    21b0:   000021ba    strheq  r2, [r0], -sl
    21b4:   00217e0a    eoreq   r7, r1, sl, lsl #28
    21b8:   0c120000    ldceq   0, cr0, [r2], {0}
    21bc:   24013909    strcs   r3, [r1], #-2313
    21c0:   14000022    strne   r0, [r0], #-34
    21c4:   726f4377    rsbvc   r4, pc, #-603979775 ; 0xdc000001
    21c8:   67655265    strbvs  r5, [r5, -r5, ror #4]!
    21cc:   65747369    ldrbvs  r7, [r4, #-873]!
    21d0:   78457372    stmdavc r5, {r1, r4, r5, r6, r8, r9, ip, sp, lr}^
    21d4:   746e6574    strbtvc r6, [lr], #-1396
    21d8:   013a0900    teqeq   sl, r0, lsl #18
    21dc:   00000185    andeq   r0, r0, r5, lsl #3
    21e0:   14002302    strne   r2, [r0], #-770
    21e4:   654e6670    strbvs  r6, [lr, #-1648]
    21e8:   616d4477    smcvs   54343
    21ec:   746e6f43    strbtvc r6, [lr], #-3907
    21f0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    21f4:   09007265    stmdbeq r0, {r0, r2, r5, r6, r9, ip, sp, lr}
    21f8:   212f013b    teqcs   pc, fp, lsr r1
    21fc:   23020000    movwcs  r0, #8192   ; 0x2000
    2200:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2204:   74736544    ldrbtvc r6, [r3], #-1348
    2208:   44796f72    ldrbtmi r6, [r9], #-3954
    220c:   6f43616d    svcvs   0x0043616d
    2210:   6f72746e    svcvs   0x0072746e
    2214:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    2218:   013c0900    teqeq   ip, r0, lsl #18
    221c:   00002184    andeq   r2, r0, r4, lsl #3
    2220:   00082302    andeq   r2, r8, r2, lsl #6
    2224:   53554d11    cmppl   r5, #1088   ; 0x440
    2228:   6d445f42    stclvs  15, cr5, [r4, #-264]
    222c:   6e6f4361    cdpvs   3, 6, cr4, cr15, cr1, {3}
    2230:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    2234:   4672656c    ldrbtmi r6, [r2], -ip, ror #10
    2238:   6f746361    svcvs   0x00746361
    223c:   09007972    stmdbeq r0, {r1, r4, r5, r6, r8, fp, ip, sp, lr}
    2240:   21ba013d    undefined instruction 0x21ba013d
    2244:   080c0000    stmdaeq ip, {}
    2248:   2290940a    addscs  r9, r0, #167772160  ; 0xa000000
    224c:   700e0000    andvc   r0, lr, r0
    2250:   61746144    cmnvs   r4, r4, asr #2
    2254:   f1980a00    undefined instruction 0xf1980a00
    2258:   02000000    andeq   r0, r0, #0  ; 0x0
    225c:   620e0023    andvs   r0, lr, #35 ; 0x23
    2260:   73756143    cmnvc   r5, #-1073741808    ; 0xc0000010
    2264:   990a0065    stmdbls sl, {r0, r2, r5, r6}
    2268:   00000176    andeq   r0, r0, r6, ror r1
    226c:   0d042302    stceq   3, cr2, [r4, #-8]
    2270:   00000111    andeq   r0, r0, r1, lsl r1
            2270: R_ARM_ABS32   .debug_str
    2274:   01769a0a    cmneq   r6, sl, lsl #20
    2278:   23020000    movwcs  r0, #8192   ; 0x2000
    227c:   53620e05    cmnpl   r2, #80 ; 0x50
    2280:   75746174    ldrbvc  r6, [r4, #-372]!
    2284:   9b0a0073    blls    282458 <MGC_HdrcDumpEndpoint+0x27db54>
    2288:   00000176    andeq   r0, r0, r6, ror r1
    228c:   00062302    andeq   r2, r6, r2, lsl #6
    2290:   43474d02    movtmi  r4, #32002  ; 0x7d02
    2294:   7273425f    rsbsvc  r4, r3, #-268435451 ; 0xf0000005
    2298:   6d657449    cfstrdvs    mvd7, [r5, #-292]!
    229c:   469c0a00    ldrmi   r0, [ip], r0, lsl #20
    22a0:   12000022    andne   r0, r0, #34 ; 0x22
    22a4:   010d0a60    tsteq   sp, r0, ror #20
    22a8:   000025f9    strdeq  r2, [r0], -r9
    22ac:   00011113    andeq   r1, r1, r3, lsl r1
            22ad: R_ARM_ABS32   .debug_str
    22b0:   01110a00    tsteq   r1, r0, lsl #20
    22b4:   00000176    andeq   r0, r0, r6, ror r1
    22b8:   14002302    strne   r2, [r0], #-770
    22bc:   46734962    ldrbtmi r4, [r3], -r2, ror #18
    22c0:   536f6669    cmnpl   pc, #110100480  ; 0x6900000
    22c4:   65726168    ldrbvs  r6, [r2, #-360]!
    22c8:   120a0064    andne   r0, sl, #100    ; 0x64
    22cc:   00017601    andeq   r7, r1, r1, lsl #12
    22d0:   01230200    teqeq   r3, r0, lsl #4
    22d4:   614d7714    cmpvs   sp, r4, lsl r7
    22d8:   63615078    cmnvs   r1, #120    ; 0x78
    22dc:   5374656b    cmnpl   r4, #448790528  ; 0x1ac00000
    22e0:   54657a69    strbtpl r7, [r5], #-2665
    22e4:   130a0078    movwne  r0, #41080  ; 0xa078
    22e8:   00018501    andeq   r8, r1, r1, lsl #10
    22ec:   02230200    eoreq   r0, r3, #0  ; 0x0
    22f0:   614d7714    cmpvs   sp, r4, lsl r7
    22f4:   63615078    cmnvs   r1, #120    ; 0x78
    22f8:   5374656b    cmnpl   r4, #448790528  ; 0x1ac00000
    22fc:   52657a69    rsbpl   r7, r5, #430080 ; 0x69000
    2300:   140a0078    strne   r0, [sl], #-120
    2304:   00018501    andeq   r8, r1, r1, lsl #10
    2308:   04230200    strteq  r0, [r3], #-512
    230c:   00011b13    andeq   r1, r1, r3, lsl fp
            230d: R_ARM_ABS32   .debug_str
    2310:   01170a00    tsteq   r7, r0, lsl #20
    2314:   00000176    andeq   r0, r0, r6, ror r1
    2318:   14062302    strne   r2, [r6], #-770
    231c:   73754262    cmnvc   r5, #536870918  ; 0x20000006
    2320:   00646e45    rsbeq   r6, r4, r5, asr #28
    2324:   7601180a    strvc   r1, [r1], -sl, lsl #16
    2328:   02000001    andeq   r0, r0, #1  ; 0x1
    232c:   86130723    ldrhi   r0, [r3], -r3, lsr #14
            232f: R_ARM_ABS32   .debug_str
    2330:   0a000000    beq 2338 <.debug_info+0x2338>
    2334:   01760119    cmneq   r6, r9, lsl r1
    2338:   23020000    movwcs  r0, #8192   ; 0x2000
    233c:   00991308    addseq  r1, r9, r8, lsl #6
            233e: R_ARM_ABS32   .debug_str
    2340:   1a0a0000    bne 282348 <MGC_HdrcDumpEndpoint+0x27da44>
    2344:   00017601    andeq   r7, r1, r1, lsl #12
    2348:   09230200    stmdbeq r3!, {r9}
    234c:   78526214    ldmdavc r2, {r2, r4, r9, sp, lr}^
    2350:   41737542    cmnmi   r3, r2, asr #10
    2354:   65726464    ldrbvs  r6, [r2, #-1124]!
    2358:   0a007373    beq 1f12c <MGC_HdrcDumpEndpoint+0x1a828>
    235c:   0176011b    cmneq   r6, fp, lsl r1
    2360:   23020000    movwcs  r0, #8192   ; 0x2000
    2364:   5262140a    rsbpl   r1, r2, #167772160  ; 0xa000000
    2368:   73754278    cmnvc   r5, #-2147483641    ; 0x80000007
    236c:   00646e45    rsbeq   r6, r4, r5, asr #28
    2370:   76011c0a    strvc   r1, [r1], -sl, lsl #24
    2374:   02000001    andeq   r0, r0, #1  ; 0x1
    2378:   62140b23    andsvs  r0, r4, #35840  ; 0x8c00
    237c:   6e497852    mcrvs   8, 2, r7, cr9, cr2, {2}
    2380:   76726574    undefined
    2384:   0a006c61    beq 1d510 <MGC_HdrcDumpEndpoint+0x18c0c>
    2388:   0176011d    cmneq   r6, sp, lsl r1
    238c:   23020000    movwcs  r0, #8192   ; 0x2000
    2390:   5262140c    rsbpl   r1, r2, #201326592  ; 0xc000000
    2394:   61725478    cmnvs   r2, r8, ror r4
    2398:   63696666    cmnvs   r9, #106954752  ; 0x6600000
    239c:   65707954    ldrbvs  r7, [r0, #-2388]!
    23a0:   011e0a00    tsteq   lr, r0, lsl #20
    23a4:   00000176    andeq   r0, r0, r6, ror r1
    23a8:   140d2302    strne   r2, [sp], #-770
    23ac:   54734962    ldrbtpl r4, [r3], #-2402
    23b0:   1f0a0078    svcne   0x000a0078
    23b4:   00017601    andeq   r7, r1, r1, lsl #12
    23b8:   0e230200    cdpeq   2, 2, cr0, cr3, cr0, {0}
    23bc:   73496214    movtvc  r6, #37396  ; 0x9214
    23c0:   746c6148    strbtvc r6, [ip], #-328
    23c4:   0a006465    beq 1b560 <MGC_HdrcDumpEndpoint+0x16c5c>
    23c8:   01760120    cmneq   r6, r0, lsr #2
    23cc:   23020000    movwcs  r0, #8192   ; 0x2000
    23d0:   4962140f    stmdbmi r2!, {r0, r1, r2, r3, sl, ip}^
    23d4:   616c4373    smcvs   50227
    23d8:   64656d69    strbtvs r6, [r5], #-3433
    23dc:   01210a00    teqeq   r1, r0, lsl #20
    23e0:   00000176    andeq   r0, r0, r6, ror r1
    23e4:   14102302    ldrne   r2, [r0], #-770
    23e8:   43785262    cmnmi   r8, #536870918  ; 0x20000006
    23ec:   6d69616c    stfvse  f6, [r9, #-432]!
    23f0:   0a006465    beq 1b58c <MGC_HdrcDumpEndpoint+0x16c88>
    23f4:   01760122    cmneq   r6, r2, lsr #2
    23f8:   23020000    movwcs  r0, #8192   ; 0x2000
    23fc:   53621411    cmnpl   r2, #285212672  ; 0x11000000
    2400:   54706f74    ldrbtpl r6, [r0], #-3956
    2404:   230a0078    movwcs  r0, #41080  ; 0xa078
    2408:   00017601    andeq   r7, r1, r1, lsl #12
    240c:   12230200    eorne   r0, r3, #0  ; 0x0
    2410:   73496214    movtvc  r6, #37396  ; 0x9214
    2414:   61487852    cmpvs   r8, r2, asr r8
    2418:   6465746c    strbtvs r7, [r5], #-1132
    241c:   01240a00    teqeq   r4, r0, lsl #20
    2420:   00000176    andeq   r0, r0, r6, ror r1
    2424:   14132302    ldrne   r2, [r3], #-770
    2428:   63615077    cmnvs   r1, #119    ; 0x77
    242c:   5374656b    cmnpl   r4, #448790528  ; 0x1ac00000
    2430:   00657a69    rsbeq   r7, r5, r9, ror #20
    2434:   8501250a    strhi   r2, [r1, #-1290]
    2438:   02000001    andeq   r0, r0, #1  ; 0x1
    243c:   77141423    ldrvc   r1, [r4, -r3, lsr #8]
    2440:   61507852    cmpvs   r0, r2, asr r8
    2444:   74656b63    strbtvc r6, [r5], #-2915
    2448:   657a6953    ldrbvs  r6, [sl, #-2387]!
    244c:   01260a00    teqeq   r6, r0, lsl #20
    2450:   00000185    andeq   r0, r0, r5, lsl #3
    2454:   14162302    ldrne   r2, [r6], #-770
    2458:   78547764    ldmdavc r4, {r2, r5, r6, r8, r9, sl, ip, sp, lr}^
    245c:   657a6953    ldrbvs  r6, [sl, #-2387]!
    2460:   01270a00    teqeq   r7, r0, lsl #20
    2464:   000001a4    andeq   r0, r0, r4, lsr #3
    2468:   14182302    ldrne   r2, [r8], #-770
    246c:   49785470    ldmdbmi r8!, {r4, r5, r6, sl, ip, lr}^
    2470:   0a007072    beq 1e640 <MGC_HdrcDumpEndpoint+0x19d3c>
    2474:   00f10129    rscseq  r0, r1, r9, lsr #2
    2478:   23020000    movwcs  r0, #8192   ; 0x2000
    247c:   7854141c    ldmdavc r4, {r2, r3, r4, sl, ip}^
    2480:   4c707249    lfmmi   f7, 2, [r0], #-292
    2484:   00747369    rsbseq  r7, r4, r9, ror #6
    2488:   27012a0a    strcs   r2, [r1, -sl, lsl #20]
    248c:   0200001b    andeq   r0, r0, #27 ; 0x1b
    2490:   64142023    ldrvs   r2, [r4], #-35
    2494:   4f785477    svcmi   0x00785477
    2498:   65736666    ldrbvs  r6, [r3, #-1638]!
    249c:   2b0a0074    blcs    282674 <MGC_HdrcDumpEndpoint+0x27dd70>
    24a0:   0001a401    andeq   sl, r1, r1, lsl #8
    24a4:   2c230200    sfmcs   f0, 4, [r3]
    24a8:   78527014    ldmdavc r2, {r2, r4, ip, sp, lr}^
    24ac:   00707249    rsbseq  r7, r0, r9, asr #4
    24b0:   f1012c0a    undefined instruction 0xf1012c0a
    24b4:   02000000    andeq   r0, r0, #0  ; 0x0
    24b8:   52143023    andspl  r3, r4, #35 ; 0x23
    24bc:   70724978    rsbsvc  r4, r2, r8, ror r9
    24c0:   7473694c    ldrbtvc r6, [r3], #-2380
    24c4:   012d0a00    teqeq   sp, r0, lsl #20
    24c8:   00001b27    andeq   r1, r0, r7, lsr #22
    24cc:   14342302    ldrtne  r2, [r4], #-770
    24d0:   78527764    ldmdavc r2, {r2, r5, r6, r8, r9, sl, ip, sp, lr}^
    24d4:   7366664f    cmnvc   r6, #82837504   ; 0x4f00000
    24d8:   0a007465    beq 1f674 <MGC_HdrcDumpEndpoint+0x1ad70>
    24dc:   01a4012e    undefined instruction 0x01a4012e
    24e0:   23020000    movwcs  r0, #8192   ; 0x2000
    24e4:   007a1340    rsbseq  r1, sl, r0, asr #6
            24e6: R_ARM_ABS32   .debug_str
    24e8:   340a0000    strcc   r0, [sl]
    24ec:   001e5901    andseq  r5, lr, r1, lsl #18
    24f0:   44230200    strtmi  r0, [r3], #-512
    24f4:   78527014    ldmdavc r2, {r2, r4, ip, sp, lr}^
    24f8:   43616d44    cmnmi   r1, #4352   ; 0x1100
    24fc:   6e6e6168    powvsez f6, f6, #0.0
    2500:   0a006c65    beq 1d69c <MGC_HdrcDumpEndpoint+0x18d98>
    2504:   1e590135    mrcne   1, 2, r0, cr9, cr5, {1}
    2508:   23020000    movwcs  r0, #8192   ; 0x2000
    250c:   44771448    ldrbtmi r1, [r7], #-1096
    2510:   7542616d    strbvc  r6, [r2, #-365]
    2514:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    2518:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    251c:   360a0074    undefined
    2520:   00018501    andeq   r8, r1, r1, lsl #10
    2524:   4c230200    sfmmi   f0, 4, [r3]
    2528:   78527714    ldmdavc r2, {r2, r4, r8, r9, sl, ip, sp, lr}^
    252c:   42616d44    rsbmi   r6, r1, #4352   ; 0x1100
    2530:   65666675    strbvs  r6, [r6, #-1653]!
    2534:   756f4372    strbvc  r4, [pc, #-882]!    ; 21ca <.debug_info+0x21ca>
    2538:   0a00746e    beq 1f6f8 <MGC_HdrcDumpEndpoint+0x1adf4>
    253c:   01850137    orreq   r0, r5, r7, lsr r1
    2540:   23020000    movwcs  r0, #8192   ; 0x2000
    2544:   4462144e    strbtmi r1, [r2], #-1102
    2548:   7854616d    ldmdavc r4, {r0, r2, r3, r5, r6, r8, sp, lr}^
    254c:   01380a00    teqeq   r8, r0, lsl #20
    2550:   00000176    andeq   r0, r0, r6, ror r1
    2554:   14502302    ldrbne  r2, [r0], #-770
    2558:   616d4462    cmnvs   sp, r2, ror #8
    255c:   69736552    ldmdbvs r3!, {r1, r4, r6, r8, sl, sp, lr}^
    2560:   6c617564    cfstr64vs   mvdx7, [r1], #-400
    2564:   01390a00    teqeq   r9, r0, lsl #20
    2568:   00000176    andeq   r0, r0, r6, ror r1
    256c:   13512302    cmpne   r1, #134217728  ; 0x8000000
    2570:   000000ee    andeq   r0, r0, lr, ror #1
            2570: R_ARM_ABS32   .debug_str
    2574:   f1013c0a    undefined instruction 0xf1013c0a
    2578:   02000000    andeq   r0, r0, #0  ; 0x0
    257c:   64145423    ldrvs   r5, [r4], #-1059
    2580:   69615777    stmdbvs r1!, {r0, r1, r2, r4, r5, r6, r8, r9, sl, ip, lr}^
    2584:   61724674    cmnvs   r2, r4, ror r6
    2588:   6f43656d    svcvs   0x0043656d
    258c:   00746e75    rsbseq  r6, r4, r5, ror lr
    2590:   a4013d0a    strge   r3, [r1], #-3338
    2594:   02000001    andeq   r0, r0, #1  ; 0x1
    2598:   62145823    andsvs  r5, r4, #2293760    ; 0x230000
    259c:   69467854    stmdbvs r6, {r2, r4, r6, fp, ip, sp, lr}^
    25a0:   69536f66    ldmdbvs r3, {r1, r2, r5, r6, r8, r9, sl, fp, sp, lr}^
    25a4:   0a00657a    beq 1bb94 <MGC_HdrcDumpEndpoint+0x17290>
    25a8:   01760140    cmneq   r6, r0, asr #2
    25ac:   23020000    movwcs  r0, #8192   ; 0x2000
    25b0:   5262145c    rsbpl   r1, r2, #1543503872 ; 0x5c000000
    25b4:   66694678    undefined
    25b8:   7a69536f    bvc 1a5737c <MGC_HdrcDumpEndpoint+0x1a52a78>
    25bc:   410a0065    tstmi   sl, r5, rrx
    25c0:   00017601    andeq   r7, r1, r1, lsl #12
    25c4:   5d230200    sfmpl   f0, 4, [r3]
    25c8:   73496214    movtvc  r6, #37396  ; 0x9214
    25cc:   73655272    cmnvc   r5, #536870919  ; 0x20000007
    25d0:   61756469    cmnvs   r5, r9, ror #8
    25d4:   430a006c    movwmi  r0, #41068  ; 0xa06c
    25d8:   00017601    andeq   r7, r1, r1, lsl #12
    25dc:   5e230200    cdppl   2, 2, cr0, cr3, cr0, {0}
    25e0:   73426214    movtvc  r6, #8724   ; 0x2214
    25e4:   73655272    cmnvc   r5, #536870919  ; 0x20000007
    25e8:   61756469    cmnvs   r5, r9, ror #8
    25ec:   440a006c    strmi   r0, [sl], #-108
    25f0:   00017601    andeq   r7, r1, r1, lsl #12
    25f4:   5f230200    svcpl   0x00230200
    25f8:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    25fc:   6e455f43    cdpvs   15, 4, cr5, cr5, cr3, {2}
    2600:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    2604:   6552746e    ldrbvs  r7, [r2, #-1134]
    2608:   72756f73    rsbsvc  r6, r5, #460    ; 0x1cc
    260c:   0a006563    beq 1bba0 <MGC_HdrcDumpEndpoint+0x1729c>
    2610:   22a30145    adccs   r0, r3, #1073741841 ; 0x40000011
    2614:   5f180000    svcpl   0x00180000
    2618:   5f43474d    svcpl   0x0043474d
    261c:   65706950    ldrbvs  r6, [r0, #-2384]!
    2620:   580a2400    stmdapl sl, {sl, sp}
    2624:   0026e001    eoreq   lr, r6, r1
    2628:   53681400    cmnpl   r8, #0  ; 0x0
    262c:   69737365    ldmdbvs r3!, {r0, r2, r5, r6, r8, r9, ip, sp, lr}^
    2630:   0a006e6f    beq 1dff4 <MGC_HdrcDumpEndpoint+0x196f0>
    2634:   00f10159    rscseq  r0, r1, r9, asr r1
    2638:   23020000    movwcs  r0, #8192   ; 0x2000
    263c:   4c701400    cfldrdmi    mvd1, [r0]
    2640:   6c61636f    stclvs  3, cr6, [r1], #-444
    2644:   00646e45    rsbeq   r6, r4, r5, asr #28
    2648:   e0015a0a    and r5, r1, sl, lsl #20
    264c:   02000026    andeq   r0, r0, #38 ; 0x26
    2650:   4a130423    bmi 4c1094 <MGC_HdrcDumpEndpoint+0x4bc790>
            2653: R_ARM_ABS32   .debug_str
    2654:   0a000000    beq 265c <.debug_info+0x265c>
    2658:   2fa4015b    svccs   0x00a4015b
    265c:   23020000    movwcs  r0, #8192   ; 0x2000
    2660:   53701408    cmnpl   r0, #134217728  ; 0x8000000
    2664:   00746f6c    rsbseq  r6, r4, ip, ror #30
    2668:   d9015c0a    stmdble r1, {r1, r3, sl, fp, ip, lr}
    266c:   0200002f    andeq   r0, r0, #47 ; 0x2f
    2670:   db130c23    blle    4c3094 <MGC_HdrcDumpEndpoint+0x4be790>
            2673: R_ARM_ABS32   .debug_str
    2674:   0a000001    beq 2680 <.debug_info+0x2680>
    2678:   0fa6015d    svceq   0x00a6015d
    267c:   23020000    movwcs  r0, #8192   ; 0x2000
    2680:   002e1310    eoreq   r1, lr, r0, lsl r3
            2682: R_ARM_ABS32   .debug_str
    2684:   5e0a0000    cdppl   0, 0, cr0, cr10, cr0, {0}
    2688:   00018501    andeq   r8, r1, r1, lsl #10
    268c:   14230200    strtne  r0, [r3], #-512
    2690:   6e497714    mcrvs   7, 2, r7, cr9, cr4, {0}
    2694:   76726574    undefined
    2698:   0a006c61    beq 1d824 <MGC_HdrcDumpEndpoint+0x18f20>
    269c:   0185015f    orreq   r0, r5, pc, asr r1
    26a0:   23020000    movwcs  r0, #8192   ; 0x2000
    26a4:   77641416    undefined
    26a8:   4278614d    rsbsmi  r6, r8, #1073741843 ; 0x40000013
    26ac:   69547375    ldmdbvs r4, {r0, r2, r4, r5, r6, r8, r9, ip, sp, lr}^
    26b0:   0a00656d    beq 1bc6c <MGC_HdrcDumpEndpoint+0x17368>
    26b4:   01a40160    undefined instruction 0x01a40160
    26b8:   23020000    movwcs  r0, #8192   ; 0x2000
    26bc:   6d621418    cfstrdvs    mvd1, [r2, #-96]!
    26c0:   67616c46    strbvs  r6, [r1, -r6, asr #24]!
    26c4:   610a0073    tstvs   sl, r3, ror r0
    26c8:   0001a401    andeq   sl, r1, r1, lsl #8
    26cc:   1c230200    sfmne   f0, 4, [r3]
    26d0:   00009913    andeq   r9, r0, r3, lsl r9
            26d1: R_ARM_ABS32   .debug_str
    26d4:   01620a00    cmneq   r2, r0, lsl #20
    26d8:   00000176    andeq   r0, r0, r6, ror r1
    26dc:   00202302    eoreq   r2, r0, r2, lsl #6
    26e0:   25f90407    ldrbcs  r0, [r9, #1031]!
    26e4:   5f1a0000    svcpl   0x001a0000
    26e8:   5f43474d    svcpl   0x0043474d
    26ec:   74726f50    ldrbtvc r6, [r2], #-3920
    26f0:   0a026000    beq 9a6f8 <MGC_HdrcDumpEndpoint+0x95df4>
    26f4:   002fa4f1    strdeq  sl, [pc], -r1
    26f8:   00fb1300    rscseq  r1, fp, r0, lsl #6
            26fa: R_ARM_ABS32   .debug_str
    26fc:   620a0000    andvs   r0, sl, #0  ; 0x0
    2700:   00390d03    eorseq  r0, r9, r3, lsl #26
    2704:   00230200    eoreq   r0, r3, r0, lsl #4
    2708:   6e497014    mcrvs   0, 2, r7, cr9, cr4, {0}
    270c:   66726574    undefined
    2710:   50656361    rsbpl   r6, r5, r1, ror #6
    2714:   0074726f    rsbseq  r7, r4, pc, ror #4
    2718:   1603630a    strne   r6, [r3], -sl, lsl #6
    271c:   0200000f    andeq   r0, r0, #15 ; 0xf
    2720:   ee130423    cfmuld  mvd0, mvd3, mvd3
            2723: R_ARM_ABS32   .debug_str
    2724:   0a000000    beq 272c <.debug_info+0x272c>
    2728:   00f10364    rscseq  r0, r1, r4, ror #6
    272c:   23020000    movwcs  r0, #8192   ; 0x2000
    2730:   6f4c1408    svcvs   0x004c1408
    2734:   456c6163    strbmi  r6, [ip, #-355]!
    2738:   0073646e    rsbseq  r6, r3, lr, ror #8
    273c:   c903650a    stmdbgt r3, {r1, r3, r8, sl, sp, lr}
    2740:   0200001a    andeq   r0, r0, #26 ; 0x1a
    2744:   62140c23    andsvs  r0, r4, #8960   ; 0x2300
    2748:   746e6157    strbtvc r6, [lr], #-343
    274c:   68676948    stmdavs r7!, {r3, r6, r8, fp, sp, lr}^
    2750:   65657053    strbvs  r7, [r5, #-83]!
    2754:   680a0064    stmdavs sl, {r2, r5, r6}
    2758:   00017603    andeq   r7, r1, r3, lsl #12
    275c:   24230200    strtcs  r0, [r3], #-512
    2760:   61576214    cmpvs   r7, r4, lsl r2
    2764:   6552746e    ldrbvs  r7, [r2, #-1134]
    2768:   00746573    rsbseq  r6, r4, r3, ror r5
    276c:   7603690a    strvc   r6, [r3], -sl, lsl #18
    2770:   02000001    andeq   r0, r0, #1  ; 0x1
    2774:   62142523    andsvs  r2, r4, #146800640  ; 0x8c00000
    2778:   746e6157    strbtvc r6, [lr], #-343
    277c:   70737553    rsbsvc  r7, r3, r3, asr r5
    2780:   00646e65    rsbeq   r6, r4, r5, ror #28
    2784:   76036a0a    strvc   r6, [r3], -sl, lsl #20
    2788:   02000001    andeq   r0, r0, #1  ; 0x1
    278c:   62142623    andsvs  r2, r4, #36700160   ; 0x2300000
    2790:   746e6157    strbtvc r6, [lr], #-343
    2794:   75736552    ldrbvc  r6, [r3, #-1362]!
    2798:   0a00656d    beq 1bd54 <MGC_HdrcDumpEndpoint+0x17450>
    279c:   0176036b    cmneq   r6, fp, ror #6
    27a0:   23020000    movwcs  r0, #8192   ; 0x2000
    27a4:   00c01327    sbceq   r1, r0, r7, lsr #6
            27a6: R_ARM_ABS32   .debug_str
    27a8:   6c0a0000    stcvs   0, cr0, [sl], {0}
    27ac:   00017603    andeq   r7, r1, r3, lsl #12
    27b0:   28230200    stmdacs r3!, {r9}
    27b4:   61576214    cmpvs   r7, r4, lsl r2
    27b8:   6553746e    ldrbvs  r7, [r3, #-1134]
    27bc:   6f697373    svcvs   0x00697373
    27c0:   6d0a006e    stcvs   0, cr0, [sl, #-440]
    27c4:   00017603    andeq   r7, r1, r3, lsl #12
    27c8:   29230200    stmdbcs r3!, {r9}
    27cc:   61576214    cmpvs   r7, r4, lsl r2
    27d0:   6f48746e    svcvs   0x0048746e
    27d4:   0a007473    beq 1f9a8 <MGC_HdrcDumpEndpoint+0x1b0a4>
    27d8:   0176036e    cmneq   r6, lr, ror #6
    27dc:   23020000    movwcs  r0, #8192   ; 0x2000
    27e0:   5762142a    strbpl  r1, [r2, -sl, lsr #8]!
    27e4:   54746e61    ldrbtpl r6, [r4], #-3681
    27e8:   4d747365    ldclmi  3, cr7, [r4, #-404]!
    27ec:   0065646f    rsbeq   r6, r5, pc, ror #8
    27f0:   76036f0a    strvc   r6, [r3], -sl, lsl #30
    27f4:   02000001    andeq   r0, r0, #1  ; 0x1
    27f8:   62142b23    andsvs  r2, r4, #35840  ; 0x8c00
    27fc:   74736554    ldrbtvc r6, [r3], #-1364
    2800:   65646f4d    strbvs  r6, [r4, #-3917]!
    2804:   03700a00    cmneq   r0, #0  ; 0x0
    2808:   00000176    andeq   r0, r0, r6, ror r1
    280c:   142c2302    strtne  r2, [ip], #-770
    2810:   73655362    cmnvc   r5, #-2013265919    ; 0x88000001
    2814:   6e6f6973    mcrvs   9, 3, r6, cr15, cr3, {3}
    2818:   69746341    ldmdbvs r4!, {r0, r6, r8, r9, sp, lr}^
    281c:   0a006576    beq 1bdfc <MGC_HdrcDumpEndpoint+0x174f8>
    2820:   01760373    cmneq   r6, r3, ror r3
    2824:   23020000    movwcs  r0, #8192   ; 0x2000
    2828:   4862142d    stmdami r2!, {r0, r2, r3, r5, sl, ip}^
    282c:   75427361    strbvc  r7, [r2, #-865]
    2830:   70536b6c    subsvc  r6, r3, ip, ror #22
    2834:   0074696c    rsbseq  r6, r4, ip, ror #18
    2838:   7603760a    strvc   r7, [r3], -sl, lsl #12
    283c:   02000001    andeq   r0, r0, #1  ; 0x1
    2840:   62142e23    andsvs  r2, r4, #560    ; 0x230
    2844:   42736148    rsbsmi  r6, r3, #18 ; 0x12
    2848:   436b6c75    cmnmi   fp, #29952  ; 0x7500
    284c:   69626d6f    stmdbvs r2!, {r0, r1, r2, r3, r5, r6, r8, sl, fp, sp, lr}^
    2850:   0a00656e    beq 1be10 <MGC_HdrcDumpEndpoint+0x1750c>
    2854:   01760377    cmneq   r6, r7, ror r3
    2858:   23020000    movwcs  r0, #8192   ; 0x2000
    285c:   4862142f    stmdami r2!, {r0, r1, r2, r3, r5, sl, ip}^
    2860:   62487361    subvs   r7, r8, #-2080374783    ; 0x84000001
    2864:   546f7349    strbtpl r7, [pc], #841  ; 286c <.debug_info+0x286c>
    2868:   780a0078    stmdavc sl, {r3, r4, r5, r6}
    286c:   00017603    andeq   r7, r1, r3, lsl #12
    2870:   30230200    eorcc   r0, r3, r0, lsl #4
    2874:   61486214    cmpvs   r8, r4, lsl r2
    2878:   49624873    stmdbmi r2!, {r0, r1, r4, r5, r6, fp, lr}^
    287c:   78526f73    ldmdavc r2, {r0, r1, r4, r5, r6, r8, r9, sl, fp, sp, lr}^
    2880:   03790a00    cmneq   r9, #0  ; 0x0
    2884:   00000176    andeq   r0, r0, r6, ror r1
    2888:   14312302    ldrtne  r2, [r1], #-770
    288c:   4d734962    ldclmi  9, cr4, [r3, #-392]!
    2890:   69746c75    ldmdbvs r4!, {r0, r2, r4, r5, r6, sl, fp, sp, lr}^
    2894:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    2898:   7a0a0074    bvc 282a70 <MGC_HdrcDumpEndpoint+0x27e16c>
    289c:   00017603    andeq   r7, r1, r3, lsl #12
    28a0:   32230200    eorcc   r0, r3, #0  ; 0x0
    28a4:   6e456214    mcrvs   2, 2, r6, cr5, cr4, {0}
    28a8:   756f4364    strbvc  r4, [pc, #-868]!    ; 254c <.debug_info+0x254c>
    28ac:   0a00746e    beq 1fa6c <MGC_HdrcDumpEndpoint+0x1b168>
    28b0:   0176037b    cmneq   r6, fp, ror r3
    28b4:   23020000    movwcs  r0, #8192   ; 0x2000
    28b8:   77641433    undefined
    28bc:   6d617246    sfmvs   f7, 2, [r1, #-280]!
    28c0:   7e0a0065    cdpvc   0, 0, cr0, cr10, cr5, {3}
    28c4:   0001a403    andeq   sl, r1, r3, lsl #8
    28c8:   34230200    strtcc  r0, [r3], #-512
    28cc:   73496214    movtvc  r6, #37396  ; 0x9214
    28d0:   68676948    stmdavs r7!, {r3, r6, r8, fp, sp, lr}^
    28d4:   65657053    strbvs  r7, [r5, #-83]!
    28d8:   7f0a0064    svcvc   0x000a0064
    28dc:   00017603    andeq   r7, r1, r3, lsl #12
    28e0:   38230200    stmdacc r3!, {r9}
    28e4:   73496214    movtvc  r6, #37396  ; 0x9214
    28e8:   53776f4c    cmnpl   r7, #304    ; 0x130
    28ec:   64656570    strbtvs r6, [r5], #-1392
    28f0:   03800a00    orreq   r0, r0, #0  ; 0x0
    28f4:   00000176    andeq   r0, r0, r6, ror r1
    28f8:   14392302    ldrtne  r2, [r9], #-770
    28fc:   52734962    rsbspl  r4, r3, #1605632    ; 0x188000
    2900:   74657365    strbtvc r7, [r5], #-869
    2904:   03810a00    orreq   r0, r1, #0  ; 0x0
    2908:   00000176    andeq   r0, r0, r6, ror r1
    290c:   143a2302    ldrtne  r2, [sl], #-770
    2910:   53734962    cmnpl   r3, #1605632    ; 0x188000
    2914:   65707375    ldrbvs  r7, [r0, #-885]!
    2918:   0a00646e    beq 1bad8 <MGC_HdrcDumpEndpoint+0x171d4>
    291c:   01760382    cmneq   r6, r2, lsl #7
    2920:   23020000    movwcs  r0, #8192   ; 0x2000
    2924:   4962143b    stmdbmi r2!, {r0, r1, r3, r4, r5, sl, ip}^
    2928:   73655273    cmnvc   r5, #805306375  ; 0x30000007
    292c:   00656d75    rsbeq   r6, r5, r5, ror sp
    2930:   7603830a    strvc   r8, [r3], -sl, lsl #6
    2934:   02000001    andeq   r0, r0, #1  ; 0x1
    2938:   62143c23    andsvs  r3, r4, #8960   ; 0x2300
    293c:   65537349    ldrbvs  r7, [r3, #-841]
    2940:   6f697373    svcvs   0x00697373
    2944:   840a006e    strhi   r0, [sl], #-110
    2948:   00017603    andeq   r7, r1, r3, lsl #12
    294c:   3d230200    sfmcc   f0, 4, [r3]
    2950:   73496214    movtvc  r6, #37396  ; 0x9214
    2954:   74736f48    ldrbtvc r6, [r3], #-3912
    2958:   03850a00    orreq   r0, r5, #0  ; 0x0
    295c:   00000176    andeq   r0, r0, r6, ror r1
    2960:   143e2302    ldrtne  r2, [lr], #-770
    2964:   6e6f4362    cdpvs   3, 6, cr4, cr15, cr2, {3}
    2968:   7463656e    strbtvc r6, [r3], #-1390
    296c:   6449726f    strbvs  r7, [r9], #-623
    2970:   03860a00    orreq   r0, r6, #0  ; 0x0
    2974:   00000176    andeq   r0, r0, r6, ror r1
    2978:   143f2302    ldrtne  r2, [pc], #770  ; 2980 <.debug_info+0x2980>
    297c:   73754262    cmnvc   r5, #536870918  ; 0x20000006
    2980:   746c6f56    strbtvc r6, [ip], #-3926
    2984:   00656761    rsbeq   r6, r5, r1, ror #14
    2988:   7603870a    strvc   r8, [r3], -sl, lsl #14
    298c:   02000001    andeq   r0, r0, #1  ; 0x1
    2990:   62144023    andsvs  r4, r4, #35 ; 0x23
    2994:   30646e45    rsbcc   r6, r4, r5, asr #28
    2998:   74617453    strbtvc r7, [r1], #-1107
    299c:   0a007375    beq 1f778 <MGC_HdrcDumpEndpoint+0x1ae74>
    29a0:   0176038a    cmneq   r6, sl, lsl #7
    29a4:   23020000    movwcs  r0, #8192   ; 0x2000
    29a8:   45771441    ldrbmi  r1, [r7, #-1089]!
    29ac:   614d646e    cmpvs   sp, lr, ror #8
    29b0:   0a006b73    beq 1d784 <MGC_HdrcDumpEndpoint+0x18e80>
    29b4:   0185038b    orreq   r0, r5, fp, lsl #7
    29b8:   23020000    movwcs  r0, #8192   ; 0x2000
    29bc:   44621442    strbtmi r1, [r2], #-1090
    29c0:   6f43616d    svcvs   0x0043616d
    29c4:   656c706d    strbvs  r7, [ip, #-109]!
    29c8:   00646574    rsbeq   r6, r4, r4, ror r5
    29cc:   76038c0a    strvc   r8, [r3], -sl, lsl #24
    29d0:   02000001    andeq   r0, r0, #1  ; 0x1
    29d4:   62144423    andsvs  r4, r4, #587202560  ; 0x23000000
    29d8:   73756256    cmnvc   r5, #1610612741 ; 0x60000005
    29dc:   6f727245    svcvs   0x00727245
    29e0:   74655272    strbtvc r5, [r5], #-626
    29e4:   73656972    cmnvc   r5, #1867776    ; 0x1c8000
    29e8:   038d0a00    orreq   r0, sp, #0  ; 0x0
    29ec:   00000176    andeq   r0, r0, r6, ror r1
    29f0:   14452302    strbne  r2, [r5], #-770
    29f4:   73614862    cmnvc   r1, #6422528    ; 0x620000
    29f8:   61736944    cmnvs   r3, r4, asr #18
    29fc:   50656c62    rsbpl   r6, r5, r2, ror #24
    2a00:   00676e69    rsbeq   r6, r7, r9, ror #28
    2a04:   76038e0a    strvc   r8, [r3], -sl, lsl #28
    2a08:   02000001    andeq   r0, r0, #1  ; 0x1
    2a0c:   62144623    andsvs  r4, r4, #36700160   ; 0x2300000
    2a10:   62626142    rsbvs   r6, r2, #-2147483632    ; 0x80000010
    2a14:   6552656c    ldrbvs  r6, [r2, #-1388]
    2a18:   65697274    strbvs  r7, [r9, #-628]!
    2a1c:   8f0a0073    svchi   0x000a0073
    2a20:   00017603    andeq   r7, r1, r3, lsl #12
    2a24:   47230200    strmi   r0, [r3, -r0, lsl #4]!
    2a28:   61486214    cmpvs   r8, r4, lsl r2
    2a2c:   50715273    rsbspl  r5, r1, r3, ror r2
    2a30:   6f43746b    svcvs   0x0043746b
    2a34:   00746e75    rsbseq  r6, r4, r5, ror lr
    2a38:   7603900a    strvc   r9, [r3], -sl
    2a3c:   02000001    andeq   r0, r0, #1  ; 0x1
    2a40:   62144823    andsvs  r4, r4, #2293760    ; 0x230000
    2a44:   31646150    cmncc   r4, r0, asr r1
    2a48:   03910a00    orrseq  r0, r1, #0  ; 0x0
    2a4c:   00000176    andeq   r0, r0, r6, ror r1
    2a50:   14492302    strbne  r2, [r9], #-770
    2a54:   64615062    strbtvs r5, [r1], #-98
    2a58:   920a0032    andls   r0, sl, #50 ; 0x32
    2a5c:   00017603    andeq   r7, r1, r3, lsl #12
    2a60:   4a230200    bmi 8c3268 <MGC_HdrcDumpEndpoint+0x8be964>
    2a64:   61506214    cmpvs   r0, r4, lsl r2
    2a68:   0a003364    beq f800 <MGC_HdrcDumpEndpoint+0xaefc>
    2a6c:   01760393    ldrbeq  r0, [r6, #-51]!
    2a70:   23020000    movwcs  r0, #8192   ; 0x2000
    2a74:   4f62144b    svcmi   0x0062144b
    2a78:   74536774    ldrbvc  r6, [r3], #-1908
    2a7c:   00657461    rsbeq   r7, r5, r1, ror #8
    2a80:   7603960a    strvc   r9, [r3], -sl, lsl #12
    2a84:   02000001    andeq   r0, r0, #1  ; 0x1
    2a88:   62144c23    andsvs  r4, r4, #8960   ; 0x2300
    2a8c:   696c6552    stmdbvs ip!, {r1, r4, r6, r8, sl, sp, lr}^
    2a90:   6975716e    ldmdbvs r5!, {r1, r2, r3, r5, r6, r8, ip, sp, lr}^
    2a94:   0a006873    beq 1cc68 <MGC_HdrcDumpEndpoint+0x18364>
    2a98:   01760397    ldrbeq  r0, [r6, #-55]!
    2a9c:   23020000    movwcs  r0, #8192   ; 0x2000
    2aa0:   4962144d    stmdbmi r2!, {r0, r2, r3, r6, sl, ip}^
    2aa4:   706e4873    rsbvc   r4, lr, r3, ror r8
    2aa8:   70707553    rsbsvc  r7, r0, r3, asr r5
    2aac:   6574726f    ldrbvs  r7, [r4, #-623]!
    2ab0:   980a0064    stmdals sl, {r2, r5, r6}
    2ab4:   00017603    andeq   r7, r1, r3, lsl #12
    2ab8:   4e230200    cdpmi   2, 2, cr0, cr3, cr0, {0}
    2abc:   73496214    movtvc  r6, #37396  ; 0x9214
    2ac0:   41706e48    cmnmi   r0, r8, asr #28
    2ac4:   776f6c6c    strbvc  r6, [pc, -ip, ror #24]!
    2ac8:   0a006465    beq 1bc64 <MGC_HdrcDumpEndpoint+0x17360>
    2acc:   01760399    ldrbeq  r0, [r6, #-57]!
    2ad0:   23020000    movwcs  r0, #8192   ; 0x2000
    2ad4:   4f70144f    svcmi   0x0070144f
    2ad8:   6c436774    mcrrvs  7, 7, r6, r3, cr4
    2adc:   746e6569    strbtvc r6, [lr], #-1385
    2ae0:   039a0a00    orrseq  r0, sl, #0  ; 0x0
    2ae4:   00003913    andeq   r3, r0, r3, lsl r9
    2ae8:   14502302    ldrbne  r2, [r0], #-770
    2aec:   6d756e45    ldclvs  14, cr6, [r5, #-276]!
    2af0:   74617265    strbtvc r7, [r1], #-613
    2af4:   446e6f69    strbtmi r6, [lr], #-3945
    2af8:   00617461    rsbeq   r7, r1, r1, ror #8
    2afc:   f1039e0a    undefined instruction 0xf1039e0a
    2b00:   02000038    andeq   r0, r0, #56 ; 0x38
    2b04:   70145423    andsvc  r5, r4, r3, lsr #8
    2b08:   74736f48    ldrbtvc r6, [r3], #-3912
    2b0c:   65696c43    strbvs  r6, [r9, #-3139]!
    2b10:   0a00746e    beq 1fcd0 <MGC_HdrcDumpEndpoint+0x1b3cc>
    2b14:   3919039f    ldmdbcc r9, {r0, r1, r2, r3, r4, r7, r8, r9}
    2b18:   23030000    movwcs  r0, #12288  ; 0x3000
    2b1c:   701403ac    andsvc  r0, r4, ip, lsr #7
    2b20:   69724466    ldmdbvs r2!, {r1, r2, r5, r6, sl, lr}^
    2b24:   54726576    ldrbtpl r6, [r2], #-1398
    2b28:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
    2b2c:   69707845    ldmdbvs r0!, {r0, r2, r6, fp, ip, sp, lr}^
    2b30:   00646572    rsbeq   r6, r4, r2, ror r5
    2b34:   2103a00a    tstcs   r3, sl
    2b38:   03000017    movweq  r0, #23 ; 0x17
    2b3c:   1403b023    strne   fp, [r3], #-35
    2b40:   69724470    ldmdbvs r2!, {r4, r5, r6, sl, lr}^
    2b44:   54726576    ldrbtpl r6, [r2], #-1398
    2b48:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
    2b4c:   61746144    cmnvs   r4, r4, asr #2
    2b50:   03a10a00    undefined instruction 0x03a10a00
    2b54:   000000f1    strdeq  r0, [r0], -r1
    2b58:   03b42303    undefined instruction 0x03b42303
    2b5c:   6f527014    svcvs   0x00527014
    2b60:   6544746f    strbvs  r7, [r4, #-1135]
    2b64:   65636976    strbvs  r6, [r3, #-2422]!
    2b68:   03a20a00    undefined instruction 0x03a20a00
    2b6c:   000016dd    ldrdeq  r1, [r0], -sp
    2b70:   03b82303    undefined instruction 0x03b82303
    2b74:   6e6f4314    mcrvs   3, 3, r4, cr15, cr4, {0}
    2b78:   7463656e    strbtvc r6, [r3], #-1390
    2b7c:   65446465    strbvs  r6, [r4, #-1125]
    2b80:   65636976    strbvs  r6, [r3, #-2422]!
    2b84:   7473694c    ldrbtvc r6, [r3], #-2380
    2b88:   03a30a00    undefined instruction 0x03a30a00
    2b8c:   00001b27    andeq   r1, r0, r7, lsr #22
    2b90:   03bc2303    undefined instruction 0x03bc2303
    2b94:   68635314    stmdavs r3!, {r2, r4, r8, r9, ip, lr}^
    2b98:   6c756465    cfldrdvs    mvd6, [r5], #-404
    2b9c:   a40a0065    strge   r0, [sl], #-101
    2ba0:   00304003    eorseq  r4, r0, r3
    2ba4:   c8230300    stmdagt r3!, {r8, r9}
    2ba8:   48771403    ldmdami r7!, {r0, r1, sl, ip}^
    2bac:   4d74736f    ldclmi  3, cr7, [r4, #-444]!
    2bb0:   6f507861    svcvs   0x00507861
    2bb4:   00726577    rsbseq  r6, r2, r7, ror r5
    2bb8:   8503a50a    strhi   sl, [r3, #-1290]
    2bbc:   03000001    movweq  r0, #1  ; 0x1
    2bc0:   1403dc23    strne   sp, [r3], #-3107
    2bc4:   736f4877    cmnvc   pc, #7798784    ; 0x770000
    2bc8:   65735574    ldrbvs  r5, [r3, #-1396]!
    2bcc:   776f5064    strbvc  r5, [pc, -r4, rrx]!
    2bd0:   0a007265    beq 1f56c <MGC_HdrcDumpEndpoint+0x1ac68>
    2bd4:   018503a6    orreq   r0, r5, r6, lsr #7
    2bd8:   23030000    movwcs  r0, #12288  ; 0x3000
    2bdc:   701403de    ldrsbvc r0, [r4], -lr
    2be0:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    2be4:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    2be8:   65696c43    strbvs  r6, [r9, #-3139]!
    2bec:   0a00746e    beq 1fdac <MGC_HdrcDumpEndpoint+0x1b4a8>
    2bf0:   391f03aa    ldmdbcc pc, {r1, r3, r5, r7, r8, r9}
    2bf4:   23030000    movwcs  r0, #12288  ; 0x3000
    2bf8:   911303e0    tstls   r3, r0, ror #7
            2bfb: R_ARM_ABS32   .debug_str
    2bfc:   0a000001    beq 2c08 <.debug_info+0x2c08>
    2c00:   392503ab    stmdbcc r5!, {r0, r1, r3, r5, r7, r8, r9}
    2c04:   23030000    movwcs  r0, #12288  ; 0x3000
    2c08:   701403e4    andsvc  r0, r4, r4, ror #7
    2c0c:   72727543    rsbsvc  r7, r2, #281018368  ; 0x10c00000
    2c10:   43746e65    cmnmi   r4, #1616   ; 0x650
    2c14:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
    2c18:   ac0a0067    stcge   0, cr0, [sl], {103}
    2c1c:   00392b03    eorseq  r2, r9, r3, lsl #22
    2c20:   e8230300    stmda   r3!, {r8, r9}
    2c24:   70611403    rsbvc   r1, r1, r3, lsl #8
    2c28:   65706950    ldrbvs  r6, [r0, #-2384]!
    2c2c:   03ad0a00    undefined instruction 0x03ad0a00
    2c30:   00003936    andeq   r3, r0, r6, lsr r9
    2c34:   03ec2303    mvneq   r2, #201326592  ; 0xc000000
    2c38:   73556214    cmpvc   r5, #1073741825 ; 0x40000001
    2c3c:   61745362    cmnvs   r4, r2, ror #6
    2c40:   0a006574    beq 1c218 <MGC_HdrcDumpEndpoint+0x17914>
    2c44:   017603ae    cmneq   r6, lr, lsr #7
    2c48:   23030000    movwcs  r0, #12288  ; 0x3000
    2c4c:   621403f0    andsvs  r0, r4, #-1073741821    ; 0xc0000003
    2c50:   4678614d    ldrbtmi r6, [r8], -sp, asr #2
    2c54:   74636e75    strbtvc r6, [r3], #-3701
    2c58:   456e6f69    strbmi  r6, [lr, #-3945]!
    2c5c:   0073646e    rsbseq  r6, r3, lr, ror #8
    2c60:   7603af0a    strvc   sl, [r3], -sl, lsl #30
    2c64:   03000001    movweq  r0, #1  ; 0x1
    2c68:   1403f123    strne   pc, [r3], #-291
    2c6c:   6e614362    cdpvs   3, 6, cr4, cr1, cr2, {3}
    2c70:   656b6157    strbvs  r6, [fp, #-343]!
    2c74:   0a007075    beq 1ee50 <MGC_HdrcDumpEndpoint+0x1a54c>
    2c78:   017603b0    ldrheq  r0, [r6, #-48]!
    2c7c:   23030000    movwcs  r0, #12288  ; 0x3000
    2c80:   621403f2    andsvs  r0, r4, #-939524093 ; 0xc8000003
    2c84:   41746553    cmnmi   r4, r3, asr r5
    2c88:   65726464    ldrbvs  r6, [r2, #-1124]!
    2c8c:   0a007373    beq 1fa60 <MGC_HdrcDumpEndpoint+0x1b15c>
    2c90:   017603b1    ldrheq  r0, [r6, #-49]!
    2c94:   23030000    movwcs  r0, #12288  ; 0x3000
    2c98:   621403f3    andsvs  r0, r4, #-872415229 ; 0xcc000003
    2c9c:   666e6f43    strbtvs r6, [lr], -r3, asr #30
    2ca0:   61566769    cmpvs   r6, r9, ror #14
    2ca4:   0065756c    rsbeq   r7, r5, ip, ror #10
    2ca8:   7603b20a    strvc   fp, [r3], -sl, lsl #4
    2cac:   03000001    movweq  r0, #1  ; 0x1
    2cb0:   1403f423    strne   pc, [r3], #-1059
    2cb4:   72615062    rsbvc   r5, r1, #98 ; 0x62
    2cb8:   0a006573    beq 1c28c <MGC_HdrcDumpEndpoint+0x17988>
    2cbc:   017603b3    ldrheq  r0, [r6, #-51]!
    2cc0:   23030000    movwcs  r0, #12288  ; 0x3000
    2cc4:   771403f5    undefined
    2cc8:   75746553    ldrbvc  r6, [r4, #-1363]!
    2ccc:   74614470    strbtvc r4, [r1], #-1136
    2cd0:   646e4961    strbtvs r4, [lr], #-2401
    2cd4:   0a007865    beq 20e70 <MGC_HdrcDumpEndpoint+0x1c56c>
    2cd8:   018503b4    strheq  r0, [r5, r4]
    2cdc:   23030000    movwcs  r0, #12288  ; 0x3000
    2ce0:   771403f6    undefined
    2ce4:   75746553    ldrbvc  r6, [r4, #-1363]!
    2ce8:   49785270    ldmdbmi r8!, {r4, r5, r6, r9, ip, lr}^
    2cec:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
    2cf0:   03b50a00    undefined instruction 0x03b50a00
    2cf4:   00000185    andeq   r0, r0, r5, lsl #3
    2cf8:   03f82303    mvnseq  r2, #201326592  ; 0xc000000
    2cfc:   65537714    ldrbvs  r7, [r3, #-1812]
    2d00:   44707574    ldrbtmi r7, [r0], #-1396
    2d04:   53617461    cmnpl   r1, #1627389952 ; 0x61000000
    2d08:   00657a69    rsbeq   r7, r5, r9, ror #20
    2d0c:   8503b60a    strhi   fp, [r3, #-1546]
    2d10:   03000001    movweq  r0, #1  ; 0x1
    2d14:   1403fa23    strne   pc, [r3], #-2595
    2d18:   74655370    strbtvc r5, [r5], #-880
    2d1c:   61447075    cmpvs   r4, r5, ror r0
    2d20:   0a006174    beq 1b2f8 <MGC_HdrcDumpEndpoint+0x169f4>
    2d24:   0f2803b7    svceq   0x002803b7
    2d28:   23030000    movwcs  r0, #12288  ; 0x3000
    2d2c:   641403fc    ldrvs   r0, [r4], #-1020
    2d30:   71655377    smcvc   21815
    2d34:   636e6575    cmnvs   lr, #490733568  ; 0x1d400000
    2d38:   6d754e65    ldclvs  14, cr4, [r5, #-404]!
    2d3c:   00726562    rsbseq  r6, r2, r2, ror #10
    2d40:   a403b80a    strge   fp, [r3], #-2058
    2d44:   03000001    movweq  r0, #1  ; 0x1
    2d48:   14048023    strne   r8, [r4], #-35
    2d4c:   66496261    strbvs  r6, [r9], -r1, ror #4
    2d50:   4f656361    svcmi   0x00656361
    2d54:   65736666    ldrbvs  r6, [r3, #-1638]!
    2d58:   0a007374    beq 1fb30 <MGC_HdrcDumpEndpoint+0x1b22c>
    2d5c:   38e103b9    stmiacc r1!, {r0, r3, r4, r5, r7, r8, r9}^
    2d60:   23030000    movwcs  r0, #12288  ; 0x3000
    2d64:   df130484    svcle   0x00130484
            2d67: R_ARM_ABS32   .debug_str
    2d68:   0a000000    beq 2d70 <.debug_info+0x2d70>
    2d6c:   217e03c9    cmncs   lr, r9, asr #7
    2d70:   23030000    movwcs  r0, #12288  ; 0x3000
    2d74:   70140494    mulsvc  r4, r4, r4
    2d78:   61655266    cmnvs   r5, r6, ror #4
    2d7c:   73754264    cmnvc   r5, #1073741830 ; 0x40000006
    2d80:   74617453    strbtvc r7, [r1], #-1107
    2d84:   cc0a0065    stcgt   0, cr0, [sl], {101}
    2d88:   0033c003    eorseq  ip, r3, r3
    2d8c:   98230300    stmdals r3!, {r8, r9}
    2d90:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2d94:   676f7250    undefined
    2d98:   426d6172    rsbmi   r6, sp, #-2147483620    ; 0x8000001c
    2d9c:   74537375    ldrbvc  r7, [r3], #-885
    2da0:   00657461    rsbeq   r7, r5, r1, ror #8
    2da4:   f103cd0a    undefined instruction 0xf103cd0a
    2da8:   03000033    movweq  r0, #51 ; 0x33
    2dac:   14049c23    strne   r9, [r4], #-3107
    2db0:   65526670    ldrbvs  r6, [r2, #-1648]
    2db4:   50746573    rsbspl  r6, r4, r3, ror r5
    2db8:   0074726f    rsbseq  r7, r4, pc, ror #4
    2dbc:   0f03ce0a    svceq   0x0003ce0a
    2dc0:   03000034    movweq  r0, #52 ; 0x34
    2dc4:   1404a023    strne   sl, [r4], #-35
    2dc8:   69426670    stmdbvs r2, {r4, r5, r6, r9, sl, sp, lr}^
    2dcc:   6e45646e    cdpvs   4, 4, cr6, cr5, cr14, {3}
    2dd0:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    2dd4:   0a00746e    beq 1ff94 <MGC_HdrcDumpEndpoint+0x1b690>
    2dd8:   342703cf    strtcc  r0, [r7], #-975
    2ddc:   23030000    movwcs  r0, #12288  ; 0x3000
    2de0:   701404a4    andsvc  r0, r4, r4, lsr #9
    2de4:   6f725066    svcvs   0x00725066
    2de8:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    2dec:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
    2df0:   63655274    cmnvs   r5, #1073741831 ; 0x40000007
    2df4:   65766965    ldrbvs  r6, [r6, #-2405]!
    2df8:   03d00a00    bicseq  r0, r0, #0  ; 0x0
    2dfc:   00003472    andeq   r3, r0, r2, ror r4
    2e00:   04a82303    strteq  r2, [r8], #771
    2e04:   50667014    rsbpl   r7, r6, r4, lsl r0
    2e08:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    2e0c:   74536d61    ldrbvc  r6, [r3], #-3425
    2e10:   54747261    ldrbtpl r7, [r4], #-609
    2e14:   736e6172    cmnvc   lr, #-2147483620    ; 0x8000001c
    2e18:   0074696d    rsbseq  r6, r4, sp, ror #18
    2e1c:   c303d10a    movwgt  sp, #12554  ; 0x310a
    2e20:   03000034    movweq  r0, #52 ; 0x34
    2e24:   1404ac23    strne   sl, [r4], #-3107
    2e28:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
    2e2c:   6172676f    cmnvs   r2, pc, ror #14
    2e30:   756c466d    strbvc  r4, [ip, #-1645]!
    2e34:   6e456873    mcrvs   8, 2, r6, cr5, cr3, {3}
    2e38:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    2e3c:   0a00746e    beq 1fffc <MGC_HdrcDumpEndpoint+0x1b6f8>
    2e40:   351003d2    ldrcc   r0, [r0, #-978]
    2e44:   23030000    movwcs  r0, #12288  ; 0x3000
    2e48:   701404b0    ldrhvc  r0, [r4], -r0
    2e4c:   6f725066    svcvs   0x00725066
    2e50:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    2e54:   746c6148    strbtvc r6, [ip], #-328
    2e58:   70646e45    rsbvc   r6, r4, r5, asr #28
    2e5c:   746e696f    strbtvc r6, [lr], #-2415
    2e60:   03d30a00    bicseq  r0, r3, #0  ; 0x0
    2e64:   00003558    andeq   r3, r0, r8, asr r5
    2e68:   04b42303    ldrteq  r2, [r4], #771
    2e6c:   44667014    strbtmi r7, [r6], #-20
    2e70:   75616665    strbvc  r6, [r1, #-1637]!
    2e74:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
    2e78:   73655264    cmnvc   r5, #1073741830 ; 0x40000006
    2e7c:   736e6f70    cmnvc   lr, #448    ; 0x1c0
    2e80:   d40a0065    strle   r0, [sl], #-101
    2e84:   00357a03    eorseq  r7, r5, r3, lsl #20
    2e88:   b8230300    stmdalt r3!, {r8, r9}
    2e8c:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2e90:   76726553    undefined
    2e94:   44656369    strbtmi r6, [r5], #-873
    2e98:   75616665    strbvc  r6, [r1, #-1637]!
    2e9c:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
    2ea0:   d50a0064    strle   r0, [sl, #-100]
    2ea4:   0035b603    eorseq  fp, r5, r3, lsl #12
    2ea8:   bc230300    stclt   3, cr0, [r3]
    2eac:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2eb0:   76726553    undefined
    2eb4:   54656369    strbtpl r6, [r5], #-873
    2eb8:   736e6172    cmnvc   lr, #-2147483620    ; 0x8000001c
    2ebc:   4174696d    cmnmi   r4, sp, ror #18
    2ec0:   6c696176    stfvse  f6, [r9], #-472
    2ec4:   03d60a00    bicseq  r0, r6, #0  ; 0x0
    2ec8:   000035f7    strdeq  r3, [r0], -r7
    2ecc:   04c02303    strbeq  r2, [r0], #771
    2ed0:   53667014    cmnpl   r6, #20 ; 0x14
    2ed4:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
    2ed8:   65526563    ldrbvs  r6, [r2, #-1379]
    2edc:   76696563    strbtvc r6, [r9], -r3, ror #10
    2ee0:   61655265    cmnvs   r5, r5, ror #4
    2ee4:   0a007964    beq 2147c <MGC_HdrcDumpEndpoint+0x1cb78>
    2ee8:   363a03d7    undefined
    2eec:   23030000    movwcs  r0, #12288  ; 0x3000
    2ef0:   701404c4    andsvc  r0, r4, r4, asr #9
    2ef4:   616f4c66    cmnvs   pc, r6, ror #24
    2ef8:   66694664    strbtvs r4, [r9], -r4, ror #12
    2efc:   d80a006f    stmdale sl, {r0, r1, r2, r3, r5, r6}
    2f00:   00365c03    eorseq  r5, r6, r3, lsl #24
    2f04:   c8230300    stmdagt r3!, {r8, r9}
    2f08:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2f0c:   6f6c6e55    svcvs   0x006c6e55
    2f10:   69466461    stmdbvs r6, {r0, r5, r6, sl, sp, lr}^
    2f14:   0a006f66    beq 1ecb4 <MGC_HdrcDumpEndpoint+0x1a3b0>
    2f18:   369803d9    undefined
    2f1c:   23030000    movwcs  r0, #12288  ; 0x3000
    2f20:   701404cc    andsvc  r0, r4, ip, asr #9
    2f24:   63634166    cmnvs   r3, #-2147483623    ; 0x80000019
    2f28:   44747065    ldrbtmi r7, [r4], #-101
    2f2c:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    2f30:   da0a0065    ble 2830cc <MGC_HdrcDumpEndpoint+0x27e7c8>
    2f34:   0036d603    eorseq  sp, r6, r3, lsl #12
    2f38:   d0230300    eorle   r0, r3, r0, lsl #6
    2f3c:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2f40:   43616d44    cmnmi   r1, #4352   ; 0x1100
    2f44:   6e6e6168    powvsez f6, f6, #0.0
    2f48:   74536c65    ldrbvc  r6, [r3], #-3173
    2f4c:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
    2f50:   6e616843    cdpvs   8, 6, cr6, cr1, cr3, {2}
    2f54:   00646567    rsbeq   r6, r4, r7, ror #10
    2f58:   e803db0a    stmda   r3, {r1, r3, r8, r9, fp, ip, lr, pc}
    2f5c:   03000020    movweq  r0, #32 ; 0x20
    2f60:   1404d423    strne   sp, [r4], #-1059
    2f64:   65536670    ldrbvs  r6, [r3, #-1648]
    2f68:   726f5074    rsbvc   r5, pc, #116    ; 0x74
    2f6c:   73655474    cmnvc   r5, #1946157056 ; 0x74000000
    2f70:   646f4d74    strbtvs r4, [pc], #3444 ; 2f78 <.debug_info+0x2f78>
    2f74:   dc0a0065    stcle   0, cr0, [sl], {101}
    2f78:   00371103    eorseq  r1, r7, r3, lsl #2
    2f7c:   d8230300    stmdale r3!, {r8, r9}
    2f80:   66701404    ldrbtvs r1, [r0], -r4, lsl #8
    2f84:   616e7944    cmnvs   lr, r4, asr #18
    2f88:   4663696d    strbtmi r6, [r3], -sp, ror #18
    2f8c:   4c6f6669    stclmi  6, cr6, [pc], #-420
    2f90:   7461636f    strbtvc r6, [r1], #-879
    2f94:   006e6f69    rsbeq   r6, lr, r9, ror #30
    2f98:   4603dd0a    strmi   sp, [r3], -sl, lsl #26
    2f9c:   03000037    movweq  r0, #55 ; 0x37
    2fa0:   0004dc23    andeq   sp, r4, r3, lsr #24
    2fa4:   26e60407    strbtcs r0, [r6], r7, lsl #8
    2fa8:   5f170000    svcpl   0x00170000
    2fac:   5f43474d    svcpl   0x0043474d
    2fb0:   65686353    strbvs  r6, [r8, #-851]!
    2fb4:   656c7564    strbvs  r7, [ip, #-1380]!
    2fb8:   746f6c53    strbtvc r6, [pc], #3155 ; 2fc0 <.debug_info+0x2fc0>
    2fbc:   f20a0c00    undefined instruction 0xf20a0c00
    2fc0:   00002fd9    ldrdeq  r2, [r0], -r9
    2fc4:   70695014    rsbvc   r5, r9, r4, lsl r0
    2fc8:   73694c65    cmnvc   r9, #25856  ; 0x6500
    2fcc:   7a0a0074    bvc 2831a4 <MGC_HdrcDumpEndpoint+0x27e8a0>
    2fd0:   001b2701    andseq  r2, fp, r1, lsl #14
    2fd4:   00230200    eoreq   r0, r3, r0, lsl #4
    2fd8:   aa040700    bge 104be0 <MGC_HdrcDumpEndpoint+0x1002dc>
    2fdc:   1100002f    tstne   r0, pc, lsr #32
    2fe0:   5f43474d    svcpl   0x0043474d
    2fe4:   65706950    ldrbvs  r6, [r0, #-2384]!
    2fe8:   01630a00    cmneq   r3, r0, lsl #20
    2fec:   00002616    andeq   r2, r0, r6, lsl r6
    2ff0:   8b0a1412    blhi    288040 <MGC_HdrcDumpEndpoint+0x28373c>
    2ff4:   00304001    eorseq  r4, r0, r1
    2ff8:   77641400    strbvc  r1, [r4, -r0, lsl #8]!
    2ffc:   61746f54    cmnvs   r4, r4, asr pc
    3000:   6d69546c    cfstrdvs    mvd5, [r9, #-432]!
    3004:   8c0a0065    stchi   0, cr0, [sl], {101}
    3008:   0001a401    andeq   sl, r1, r1, lsl #8
    300c:   00230200    eoreq   r0, r3, r0, lsl #4
    3010:   68635314    stmdavs r3!, {r2, r4, r8, r9, ip, lr}^
    3014:   6c756465    cfldrdvs    mvd6, [r5], #-404
    3018:   6f6c5365    svcvs   0x006c5365
    301c:   0a007374    beq 1fdf4 <MGC_HdrcDumpEndpoint+0x1b4f0>
    3020:   1b27018d    blne    9c365c <MGC_HdrcDumpEndpoint+0x9bed58>
    3024:   23020000    movwcs  r0, #8192   ; 0x2000
    3028:   53771404    cmnpl   r7, #67108864   ; 0x4000000
    302c:   43746f6c    cmnmi   r4, #432    ; 0x1b0
    3030:   746e756f    strbtvc r7, [lr], #-1391
    3034:   018e0a00    orreq   r0, lr, r0, lsl #20
    3038:   00000185    andeq   r0, r0, r5, lsl #3
    303c:   00102302    andseq  r2, r0, r2, lsl #6
    3040:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3044:   6863535f    stmdavs r3!, {r0, r1, r2, r3, r4, r6, r8, r9, ip, lr}^
    3048:   6c756465    cfldrdvs    mvd6, [r5], #-404
    304c:   8f0a0065    svchi   0x000a0065
    3050:   002ff001    eoreq   pc, pc, r1
    3054:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3058:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    305c:   676f7250    undefined
    3060:   536d6172    cmnpl   sp, #-2147483620    ; 0x8000001c
    3064:   74726174    ldrbtvc r6, [r2], #-372
    3068:   746e6f43    strbtvc r6, [lr], #-3907
    306c:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    3070:   0a007265    beq 1fa0c <MGC_HdrcDumpEndpoint+0x1b108>
    3074:   307a01b3    ldrhtcc r0, [sl], #-19
    3078:   04070000    streq   r0, [r7]
    307c:   00003080    andeq   r3, r0, r0, lsl #1
    3080:   01a40109    undefined instruction 0x01a40109
    3084:   30900000    addscc  r0, r0, r0
    3088:   900a0000    andls   r0, sl, r0
    308c:   00000030    andeq   r0, r0, r0, lsr r0
    3090:   30960407    addscc  r0, r6, r7, lsl #8
    3094:   5f170000    svcpl   0x00170000
    3098:   5f43474d    svcpl   0x0043474d
    309c:   746e6f43    strbtvc r6, [lr], #-3907
    30a0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    30a4:   3c007265    sfmcc   f7, 4, [r0], {101}
    30a8:   325df00a    subscc  pc, sp, #10 ; 0xa
    30ac:   70140000    andsvc  r0, r4, r0
    30b0:   746e6f43    strbtvc r6, [lr], #-3907
    30b4:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    30b8:   64417265    strbvs  r7, [r1], #-613
    30bc:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
    30c0:   72734973    rsbsvc  r4, r3, #1884160    ; 0x1cc000
    30c4:   02000a00    andeq   r0, r0, #0  ; 0x0
    30c8:   000000f1    strdeq  r0, [r0], -r1
    30cc:   14002302    strne   r2, [r0], #-770
    30d0:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    30d4:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    30d8:   4172656c    cmnmi   r2, ip, ror #10
    30dc:   65726464    ldrbvs  r6, [r2, #-1124]!
    30e0:   73497373    movtvc  r7, #37747  ; 0x9373
    30e4:   010a0074    tsteq   sl, r4, ror r0
    30e8:   0000f102    andeq   pc, r0, r2, lsl #2
    30ec:   04230200    strteq  r0, [r3], #-512
    30f0:   74557014    ldrbvc  r7, [r5], #-20
    30f4:   00736c69    rsbseq  r6, r3, r9, ror #24
    30f8:   9702020a    strls   r0, [r2, -sl, lsl #4]
    30fc:   02000033    andeq   r0, r0, #51 ; 0x33
    3100:   70140823    andsvc  r0, r4, r3, lsr #16
    3104:   74737953    ldrbtvc r7, [r3], #-2387
    3108:   65536d65    ldrbvs  r6, [r3, #-3429]
    310c:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    3110:   0a007365    beq 1feac <MGC_HdrcDumpEndpoint+0x1b5a8>
    3114:   339d0203    orrscc  r0, sp, #805306368  ; 0x30000000
    3118:   23020000    movwcs  r0, #8192   ; 0x2000
    311c:   00ee130c    rsceq   r1, lr, ip, lsl #6
            311e: R_ARM_ABS32   .debug_str
    3120:   040a0000    streq   r0, [sl]
    3124:   0000f102    andeq   pc, r0, r2, lsl #2
    3128:   10230200    eorne   r0, r3, r0, lsl #4
    312c:   00004a13    andeq   r4, r0, r3, lsl sl
            312d: R_ARM_ABS32   .debug_str
    3130:   02050a00    andeq   r0, r5, #0  ; 0x0
    3134:   00002fa4    andeq   r2, r0, r4, lsr #31
    3138:   14142302    ldrne   r2, [r4], #-770
    313c:   616d4470    smcvs   54336
    3140:   746e6f43    strbtvc r6, [lr], #-3907
    3144:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    3148:   61467265    cmpvs   r6, r5, ror #4
    314c:   726f7463    rsbvc   r7, pc, #1660944384 ; 0x63000000
    3150:   070a0079    smlsdxeq    sl, r9, r0, r0
    3154:   0033a302    eorseq  sl, r3, r2, lsl #6
    3158:   18230200    stmdane r3!, {r9}
    315c:   50667014    rsbpl   r7, r6, r4, lsl r0
    3160:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    3164:   74536d61    ldrbvc  r6, [r3], #-3425
    3168:   43747261    cmnmi   r4, #268435462  ; 0x10000006
    316c:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    3170:   656c6c6f    strbvs  r6, [ip, #-3183]!
    3174:   0a0a0072    beq 283344 <MGC_HdrcDumpEndpoint+0x27ea40>
    3178:   00305502    eorseq  r5, r0, r2, lsl #10
    317c:   1c230200    sfmne   f0, 4, [r3]
    3180:   50667014    rsbpl   r7, r6, r4, lsl r0
    3184:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    3188:   74536d61    ldrbvc  r6, [r3], #-3425
    318c:   6f43706f    svcvs   0x0043706f
    3190:   6f72746e    svcvs   0x0072746e
    3194:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    3198:   020b0a00    andeq   r0, fp, #0  ; 0x0
    319c:   0000325d    andeq   r3, r0, sp, asr r2
    31a0:   14202302    strtne  r2, [r0], #-770
    31a4:   65446670    strbvs  r6, [r4, #-1648]
    31a8:   6f727473    svcvs   0x00727473
    31ac:   6e6f4379    mcrvs   3, 3, r4, cr15, cr9, {3}
    31b0:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    31b4:   0072656c    rsbseq  r6, r2, ip, ror #10
    31b8:   81020c0a    tsthi   r2, sl, lsl #24
    31bc:   02000032    andeq   r0, r0, #50 ; 0x32
    31c0:   70142423    andsvc  r2, r4, r3, lsr #8
    31c4:   74655366    strbtvc r5, [r5], #-870
    31c8:   746e6f43    strbtvc r6, [lr], #-3907
    31cc:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    31d0:   6f487265    svcvs   0x00487265
    31d4:   6f507473    svcvs   0x00507473
    31d8:   00726577    rsbseq  r6, r2, r7, ror r5
    31dc:   a1020d0a    tstge   r2, sl, lsl #26
    31e0:   02000032    andeq   r0, r0, #50 ; 0x32
    31e4:   70142823    andsvc  r2, r4, r3, lsr #16
    31e8:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    31ec:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    31f0:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    31f4:   5372656c    cmnpl   r2, #452984832  ; 0x1b000000
    31f8:   65746174    ldrbvs  r6, [r4, #-372]!
    31fc:   02100a00    andseq  r0, r0, #0  ; 0x0
    3200:   000032e1    andeq   r3, r0, r1, ror #5
    3204:   142c2302    strtne  r2, [ip], #-770
    3208:   75446670    strbvc  r6, [r4, #-1648]
    320c:   6950706d    ldmdbvs r0, {r0, r2, r3, r5, r6, ip, sp, lr}^
    3210:   74536570    ldrbvc  r6, [r3], #-1392
    3214:   00657461    rsbeq   r7, r5, r1, ror #8
    3218:   1902110a    stmdbne r2, {r1, r3, r8, ip}
    321c:   02000033    andeq   r0, r0, #51 ; 0x33
    3220:   70143023    andsvc  r3, r4, r3, lsr #32
    3224:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    3228:   636f4c70    cmnvs   pc, #28672  ; 0x7000
    322c:   6e456c61    cdpvs   12, 4, cr6, cr5, cr1, {3}
    3230:   61745364    cmnvs   r4, r4, ror #6
    3234:   0a006574    beq 1c80c <MGC_HdrcDumpEndpoint+0x17f08>
    3238:   33560212    cmpcc   r6, #536870913  ; 0x20000001
    323c:   23020000    movwcs  r0, #8192   ; 0x2000
    3240:   43771434    cmnmi   r7, #872415232  ; 0x34000000
    3244:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    3248:   656c6c6f    strbvs  r6, [ip, #-3183]!
    324c:   70795472    rsbsvc  r5, r9, r2, ror r4
    3250:   140a0065    strne   r0, [sl], #-101
    3254:   00018502    andeq   r8, r1, r2, lsl #10
    3258:   38230200    stmdacc r3!, {r9}
    325c:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3260:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3264:   676f7250    undefined
    3268:   536d6172    cmnpl   sp, #-2147483620    ; 0x8000001c
    326c:   43706f74    cmnmi   r0, #464    ; 0x1d0
    3270:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    3274:   656c6c6f    strbvs  r6, [ip, #-3183]!
    3278:   bc0a0072    stclt   0, cr0, [sl], {114}
    327c:   00307a01    eorseq  r7, r0, r1, lsl #20
    3280:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3284:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3288:   74736544    ldrbtvc r6, [r3], #-1348
    328c:   43796f72    cmnmi   r9, #456    ; 0x1c8
    3290:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    3294:   656c6c6f    strbvs  r6, [ip, #-3183]!
    3298:   c40a0072    strgt   r0, [sl], #-114
    329c:   00307a01    eorseq  r7, r0, r1, lsl #20
    32a0:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    32a4:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    32a8:   43746553    cmnmi   r4, #348127232  ; 0x14c00000
    32ac:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    32b0:   656c6c6f    strbvs  r6, [ip, #-3183]!
    32b4:   736f4872    cmnvc   pc, #7471104    ; 0x720000
    32b8:   776f5074    undefined
    32bc:   0a007265    beq 1fc58 <MGC_HdrcDumpEndpoint+0x1b354>
    32c0:   32c601cc    sbccc   r0, r6, #51 ; 0x33
    32c4:   04070000    streq   r0, [r7]
    32c8:   000032cc    andeq   r3, r0, ip, asr #5
    32cc:   01a40109    undefined instruction 0x01a40109
    32d0:   32e10000    rsccc   r0, r1, #0  ; 0x0
    32d4:   900a0000    andls   r0, sl, r0
    32d8:   0a000030    beq 33a0 <.debug_info+0x33a0>
    32dc:   00000185    andeq   r0, r0, r5, lsl #3
    32e0:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    32e4:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    32e8:   706d7544    rsbvc   r7, sp, r4, asr #10
    32ec:   746e6f43    strbtvc r6, [lr], #-3907
    32f0:   6c6c6f72    stclvs  15, cr6, [ip], #-456
    32f4:   74537265    ldrbvc  r7, [r3], #-613
    32f8:   00657461    rsbeq   r7, r5, r1, ror #8
    32fc:   0301d40a    movweq  sp, #5130   ; 0x140a
    3300:   07000033    smladxeq    r0, r3, r0, r0
    3304:   00330904    eorseq  r0, r3, r4, lsl #18
    3308:   a9010900    stmdbge r1, {r8, fp}
    330c:   19000000    stmdbne r0, {}
    3310:   0a000033    beq 33e4 <.debug_info+0x33e4>
    3314:   00003090    muleq   r0, r0, r0
    3318:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    331c:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3320:   706d7544    rsbvc   r7, sp, r4, asr #10
    3324:   65706950    ldrbvs  r6, [r0, #-2384]!
    3328:   74617453    strbtvc r7, [r1], #-1107
    332c:   dd0a0065    stcle   0, cr0, [sl, #-404]
    3330:   00333501    eorseq  r3, r3, r1, lsl #10
    3334:   3b040700    blcc    104f3c <MGC_HdrcDumpEndpoint+0x100638>
    3338:   09000033    stmdbeq r0, {r0, r1, r4, r5}
    333c:   0000a901    andeq   sl, r0, r1, lsl #18
    3340:   00335000    eorseq  r5, r3, r0
    3344:   30900a00    addscc  r0, r0, r0, lsl #20
    3348:   500a0000    andpl   r0, sl, r0
    334c:   00000033    andeq   r0, r0, r3, lsr r0
    3350:   2fdf0407    svccs   0x00df0407
    3354:   4d110000    ldcmi   0, cr0, [r1]
    3358:   705f4347    subsvc  r4, pc, r7, asr #6
    335c:   6d754466    cfldrdvs    mvd4, [r5, #-408]!
    3360:   636f4c70    cmnvs   pc, #28672  ; 0x7000
    3364:   6e456c61    cdpvs   12, 4, cr6, cr5, cr1, {3}
    3368:   61745364    cmnvs   r4, r4, ror #6
    336c:   0a006574    beq 1c944 <MGC_HdrcDumpEndpoint+0x18040>
    3370:   337601e7    cmncc   r6, #-1073741767    ; 0xc0000039
    3374:   04070000    streq   r0, [r7]
    3378:   0000337c    andeq   r3, r0, ip, ror r3
    337c:   00a90109    adceq   r0, r9, r9, lsl #2
    3380:   33910000    orrscc  r0, r1, #0  ; 0x0
    3384:   900a0000    andls   r0, sl, r0
    3388:   0a000030    beq 3450 <.debug_info+0x3450>
    338c:   00003391    muleq   r0, r1, r3
    3390:   11040700    tstne   r4, r0, lsl #14
    3394:   07000010    smladeq r0, r0, r0, r0
    3398:   00045004    andeq   r5, r4, r4
    339c:   e2040700    and r0, r4, #0  ; 0x0
    33a0:   07000007    streq   r0, [r0, -r7]
    33a4:   00222404    eoreq   r2, r2, r4, lsl #8
    33a8:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    33ac:   6f435f43    svcvs   0x00435f43
    33b0:   6f72746e    svcvs   0x0072746e
    33b4:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    33b8:   02150a00    andseq  r0, r5, #0  ; 0x0
    33bc:   00003096    muleq   r0, r6, r0
    33c0:   43474d11    movtmi  r4, #32017  ; 0x7d11
    33c4:   5266705f    rsbpl   r7, r6, #95 ; 0x5f
    33c8:   42646165    rsbmi   r6, r4, #1073741849 ; 0x40000019
    33cc:   74537375    ldrbvc  r7, [r3], #-885
    33d0:   00657461    rsbeq   r7, r5, r1, ror #8
    33d4:   db02230a    blle    8c004 <MGC_HdrcDumpEndpoint+0x87700>
    33d8:   07000033    smladxeq    r0, r3, r0, r0
    33dc:   0033e104    eorseq  lr, r3, r4, lsl #2
    33e0:   a4010900    strge   r0, [r1], #-2304
    33e4:   f1000001    cps #1
    33e8:   0a000033    beq 34bc <.debug_info+0x34bc>
    33ec:   00002fa4    andeq   r2, r0, r4, lsr #31
    33f0:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    33f4:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    33f8:   676f7250    undefined
    33fc:   426d6172    rsbmi   r6, sp, #-2147483620    ; 0x8000001c
    3400:   74537375    ldrbvc  r7, [r3], #-885
    3404:   00657461    rsbeq   r7, r5, r1, ror #8
    3408:   db022c0a    blle    8e438 <MGC_HdrcDumpEndpoint+0x89b34>
    340c:   11000033    tstne   r0, r3, lsr r0
    3410:   5f43474d    svcpl   0x0043474d
    3414:   65526670    ldrbvs  r6, [r2, #-1648]
    3418:   50746573    rsbspl  r6, r4, r3, ror r5
    341c:   0074726f    rsbseq  r7, r4, pc, ror #4
    3420:   db02350a    blle    90850 <MGC_HdrcDumpEndpoint+0x8bf4c>
    3424:   11000033    tstne   r0, r3, lsr r0
    3428:   5f43474d    svcpl   0x0043474d
    342c:   69426670    stmdbvs r2, {r4, r5, r6, r9, sl, sp, lr}^
    3430:   6e45646e    cdpvs   4, 4, cr6, cr5, cr14, {3}
    3434:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    3438:   0a00746e    beq 205f8 <MGC_HdrcDumpEndpoint+0x1bcf4>
    343c:   34420245    strbcc  r0, [r2], #-581
    3440:   04070000    streq   r0, [r7]
    3444:   00003448    andeq   r3, r0, r8, asr #8
    3448:   26e00109    strbtcs r0, [r0], r9, lsl #2
    344c:   34670000    strbtcc r0, [r7]
    3450:   a40a0000    strge   r0, [sl]
    3454:   0a00002f    beq 3518 <.debug_info+0x3518>
    3458:   00003467    andeq   r3, r0, r7, ror #8
    345c:   0033910a    eorseq  r9, r3, sl, lsl #2
    3460:   01760a00    cmneq   r6, r0, lsl #20
    3464:   07000000    streq   r0, [r0, -r0]
    3468:   00346d04    eorseq  r6, r4, r4, lsl #26
    346c:   0fb10800    svceq   0x00b10800
    3470:   4d110000    ldcmi   0, cr0, [r1]
    3474:   705f4347    subsvc  r4, pc, r7, asr #6
    3478:   6f725066    svcvs   0x00725066
    347c:   6d617267    sfmvs   f7, 2, [r1, #-412]!
    3480:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
    3484:   63655274    cmnvs   r5, #1073741831 ; 0x40000007
    3488:   65766965    ldrbvs  r6, [r6, #-2405]!
    348c:   02580a00    subseq  r0, r8, #0  ; 0x0
    3490:   00003494    muleq   r0, r4, r4
    3494:   349a0407    ldrcc   r0, [sl], #1031
    3498:   01090000    tsteq   r9, r0
    349c:   000001a4    andeq   r0, r0, r4, lsr #3
    34a0:   000034c3    andeq   r3, r0, r3, asr #9
    34a4:   002fa40a    eoreq   sl, pc, sl, lsl #8
    34a8:   26e00a00    strbtcs r0, [r0], r0, lsl #20
    34ac:   280a0000    stmdacs sl, {}
    34b0:   0a00000f    beq 34f4 <.debug_info+0x34f4>
    34b4:   000001a4    andeq   r0, r0, r4, lsr #3
    34b8:   0000f10a    andeq   pc, r0, sl, lsl #2
    34bc:   01760a00    cmneq   r6, r0, lsl #20
    34c0:   11000000    tstne   r0, r0
    34c4:   5f43474d    svcpl   0x0043474d
    34c8:   72506670    subsvc  r6, r0, #117440512  ; 0x7000000
    34cc:   6172676f    cmnvs   r2, pc, ror #14
    34d0:   6174536d    cmnvs   r4, sp, ror #6
    34d4:   72547472    subsvc  r7, r4, #1912602624 ; 0x72000000
    34d8:   6d736e61    ldclvs  14, cr6, [r3, #-388]!
    34dc:   0a007469    beq 20688 <MGC_HdrcDumpEndpoint+0x1bd84>
    34e0:   34e60266    strbtcc r0, [r6], #614
    34e4:   04070000    streq   r0, [r7]
    34e8:   000034ec    andeq   r3, r0, ip, ror #9
    34ec:   01a40109    undefined instruction 0x01a40109
    34f0:   35100000    ldrcc   r0, [r0]
    34f4:   a40a0000    strge   r0, [sl]
    34f8:   0a00002f    beq 35bc <.debug_info+0x35bc>
    34fc:   000026e0    andeq   r2, r0, r0, ror #13
    3500:   0006670a    andeq   r6, r6, sl, lsl #14
    3504:   01a40a00    undefined instruction 0x01a40a00
    3508:   f10a0000    cpsie   ,#0
    350c:   00000000    andeq   r0, r0, r0
    3510:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3514:   5066705f    rsbpl   r7, r6, pc, asr r0
    3518:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    351c:   6c466d61    mcrrvs  13, 6, r6, r6, cr1
    3520:   45687375    strbmi  r7, [r8, #-885]!
    3524:   6f70646e    svcvs   0x0070646e
    3528:   00746e69    rsbseq  r6, r4, r9, ror #28
    352c:   3302750a    movwcc  r7, #9482   ; 0x250a
    3530:   07000035    smladxeq    r0, r5, r0, r0
    3534:   00353904    eorseq  r3, r5, r4, lsl #18
    3538:   a4010900    strge   r0, [r1], #-2304
    353c:   58000001    stmdapl r0, {r0}
    3540:   0a000035    beq 361c <.debug_info+0x361c>
    3544:   00002fa4    andeq   r2, r0, r4, lsr #31
    3548:   0026e00a    eoreq   lr, r6, sl
    354c:   01760a00    cmneq   r6, r0, lsl #20
    3550:   760a0000    strvc   r0, [sl], -r0
    3554:   00000001    andeq   r0, r0, r1
    3558:   43474d11    movtmi  r4, #32017  ; 0x7d11
    355c:   5066705f    rsbpl   r7, r6, pc, asr r0
    3560:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
    3564:   61486d61    cmpvs   r8, r1, ror #26
    3568:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
    356c:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    3570:   0a00746e    beq 20730 <MGC_HdrcDumpEndpoint+0x1be2c>
    3574:   35330286    ldrcc   r0, [r3, #-646]!
    3578:   4d110000    ldcmi   0, cr0, [r1]
    357c:   705f4347    subsvc  r4, pc, r7, asr #6
    3580:   66654466    strbtvs r4, [r5], -r6, ror #8
    3584:   746c7561    strbtvc r7, [ip], #-1377
    3588:   52646e45    rsbpl   r6, r4, #1104   ; 0x450
    358c:   6f707365    svcvs   0x00707365
    3590:   0065736e    rsbeq   r7, r5, lr, ror #6
    3594:   9b02900a    blls    a75c4 <MGC_HdrcDumpEndpoint+0xa2cc0>
    3598:   07000035    smladxeq    r0, r5, r0, r0
    359c:   0035a104    eorseq  sl, r5, r4, lsl #2
    35a0:   a4010900    strge   r0, [r1], #-2304
    35a4:   b6000001    strlt   r0, [r0], -r1
    35a8:   0a000035    beq 3684 <.debug_info+0x3684>
    35ac:   00002fa4    andeq   r2, r0, r4, lsr #31
    35b0:   0001760a    andeq   r7, r1, sl, lsl #12
    35b4:   4d110000    ldcmi   0, cr0, [r1]
    35b8:   705f4347    subsvc  r4, pc, r7, asr #6
    35bc:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    35c0:   65636976    strbvs  r6, [r3, #-2422]!
    35c4:   61666544    cmnvs   r6, r4, asr #10
    35c8:   45746c75    ldrbmi  r6, [r4, #-3189]!
    35cc:   0a00646e    beq 1c78c <MGC_HdrcDumpEndpoint+0x17e88>
    35d0:   35d60299    ldrbcc  r0, [r6, #665]
    35d4:   04070000    streq   r0, [r7]
    35d8:   000035dc    ldrdeq  r3, [r0], -ip
    35dc:   01760109    cmneq   r6, r9, lsl #2
    35e0:   35f10000    ldrbcc  r0, [r1]!
    35e4:   a40a0000    strge   r0, [sl]
    35e8:   0a00002f    beq 36ac <.debug_info+0x36ac>
    35ec:   000035f1    strdeq  r3, [r0], -r1
    35f0:   90040700    andls   r0, r4, r0, lsl #14
    35f4:   11000022    tstne   r0, r2, lsr #32
    35f8:   5f43474d    svcpl   0x0043474d
    35fc:   65536670    ldrbvs  r6, [r3, #-1648]
    3600:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    3604:   61725465    cmnvs   r2, r5, ror #8
    3608:   696d736e    stmdbvs sp!, {r1, r2, r3, r5, r6, r8, r9, ip, sp, lr}^
    360c:   61764174    cmnvs   r6, r4, ror r1
    3610:   0a006c69    beq 1e7bc <MGC_HdrcDumpEndpoint+0x19eb8>
    3614:   361a02a3    ldrcc   r0, [sl], -r3, lsr #5
    3618:   04070000    streq   r0, [r7]
    361c:   00003620    andeq   r3, r0, r0, lsr #12
    3620:   01760109    cmneq   r6, r9, lsl #2
    3624:   363a0000    ldrtcc  r0, [sl], -r0
    3628:   a40a0000    strge   r0, [sl]
    362c:   0a00002f    beq 36f0 <.debug_info+0x36f0>
    3630:   00000185    andeq   r0, r0, r5, lsl #3
    3634:   0035f10a    eorseq  pc, r5, sl, lsl #2
    3638:   4d110000    ldcmi   0, cr0, [r1]
    363c:   705f4347    subsvc  r4, pc, r7, asr #6
    3640:   72655366    rsbvc   r5, r5, #-1744830463    ; 0x98000001
    3644:   65636976    strbvs  r6, [r3, #-2422]!
    3648:   65636552    strbvs  r6, [r3, #-1362]!
    364c:   52657669    rsbpl   r7, r5, #110100480  ; 0x6900000
    3650:   79646165    stmdbvc r4!, {r0, r2, r5, r6, r8, sp, lr}^
    3654:   02ad0a00    adceq   r0, sp, #0  ; 0x0
    3658:   0000361a    andeq   r3, r0, sl, lsl r6
    365c:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3660:   4c66705f    stclmi  0, cr7, [r6], #-380
    3664:   4664616f    strbtmi r6, [r4], -pc, ror #2
    3668:   006f6669    rsbeq   r6, pc, r9, ror #12
    366c:   7302b70a    movwvc  fp, #9994   ; 0x270a
    3670:   07000036    smladxeq    r0, r6, r0, r0
    3674:   00367904    eorseq  r7, r6, r4, lsl #18
    3678:   76010900    strvc   r0, [r1], -r0, lsl #18
    367c:   98000001    stmdals r0, {r0}
    3680:   0a000036    beq 3760 <.debug_info+0x3760>
    3684:   00002fa4    andeq   r2, r0, r4, lsr #31
    3688:   0001760a    andeq   r7, r1, sl, lsl #12
    368c:   01a40a00    undefined instruction 0x01a40a00
    3690:   670a0000    strvs   r0, [sl, -r0]
    3694:   00000006    andeq   r0, r0, r6
    3698:   43474d11    movtmi  r4, #32017  ; 0x7d11
    369c:   5566705f    strbpl  r7, [r6, #-95]!
    36a0:   616f6c6e    cmnvs   pc, lr, ror #24
    36a4:   66694664    strbtvs r4, [r9], -r4, ror #12
    36a8:   c10a006f    tstgt   sl, pc, rrx
    36ac:   0036b102    eorseq  fp, r6, r2, lsl #2
    36b0:   b7040700    strlt   r0, [r4, -r0, lsl #14]
    36b4:   09000036    stmdbeq r0, {r1, r2, r4, r5}
    36b8:   00017601    andeq   r7, r1, r1, lsl #12
    36bc:   0036d600    eorseq  sp, r6, r0, lsl #12
    36c0:   2fa40a00    svccs   0x00a40a00
    36c4:   760a0000    strvc   r0, [sl], -r0
    36c8:   0a000001    beq 36d4 <.debug_info+0x36d4>
    36cc:   000001a4    andeq   r0, r0, r4, lsr #3
    36d0:   000f280a    andeq   r2, pc, sl, lsl #16
    36d4:   4d110000    ldcmi   0, cr0, [r1]
    36d8:   705f4347    subsvc  r4, pc, r7, asr #6
    36dc:   63634166    cmnvs   r3, #-2147483623    ; 0x80000019
    36e0:   44747065    ldrbtmi r7, [r4], #-101
    36e4:   63697665    cmnvs   r9, #105906176  ; 0x6500000
    36e8:   ce0a0065    cdpgt   0, 0, cr0, cr10, cr5, {3}
    36ec:   0036f102    eorseq  pc, r6, r2, lsl #2
    36f0:   f7040700    undefined instruction 0xf7040700
    36f4:   09000036    stmdbeq r0, {r1, r2, r4, r5}
    36f8:   00017601    andeq   r7, r1, r1, lsl #12
    36fc:   00371100    eorseq  r1, r7, r0, lsl #2
    3700:   2fa40a00    svccs   0x00a40a00
    3704:   dd0a0000    stcle   0, cr0, [sl]
    3708:   0a000016    beq 3768 <.debug_info+0x3768>
    370c:   000018f1    strdeq  r1, [r0], -r1
    3710:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    3714:   66705f43    ldrbtvs r5, [r0], -r3, asr #30
    3718:   50746553    rsbspl  r6, r4, r3, asr r5
    371c:   5474726f    ldrbtpl r7, [r4], #-623
    3720:   4d747365    ldclmi  3, cr7, [r4, #-404]!
    3724:   0065646f    rsbeq   r6, r5, pc, ror #8
    3728:   2f02d50a    svccs   0x0002d50a
    372c:   07000037    smladxeq    r0, r7, r0, r0
    3730:   00373504    eorseq  r3, r7, r4, lsl #10
    3734:   46010b00    strmi   r0, [r1], -r0, lsl #22
    3738:   0a000037    beq 381c <.debug_info+0x381c>
    373c:   00002fa4    andeq   r2, r0, r4, lsr #31
    3740:   001c650a    andseq  r6, ip, sl, lsl #10
    3744:   4d110000    ldcmi   0, cr0, [r1]
    3748:   705f4347    subsvc  r4, pc, r7, asr #6
    374c:   6e794466    cdpvs   4, 7, cr4, cr9, cr6, {3}
    3750:   63696d61    cmnvs   r9, #6208   ; 0x1840
    3754:   6f666946    svcvs   0x00666946
    3758:   61636f4c    cmnvs   r3, ip, asr #30
    375c:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    3760:   02e10a00    rsceq   r0, r1, #0  ; 0x0
    3764:   00003768    andeq   r3, r0, r8, ror #14
    3768:   376e0407    strbcc  r0, [lr, -r7, lsl #8]!
    376c:   010b0000    tsteq   fp, r0
    3770:   00003798    muleq   r0, r8, r7
    3774:   002fa40a    eoreq   sl, pc, sl, lsl #8
    3778:   01760a00    cmneq   r6, r0, lsl #20
    377c:   760a0000    strvc   r0, [sl], -r0
    3780:   0a000001    beq 378c <.debug_info+0x378c>
    3784:   00000176    andeq   r0, r0, r6, ror r1
    3788:   0013dd0a    andseq  sp, r3, sl, lsl #26
    378c:   0f280a00    svceq   0x00280a00
    3790:   280a0000    stmdacs sl, {}
    3794:   0000000f    andeq   r0, r0, pc
    3798:   0a01581b    beq 5980c <MGC_HdrcDumpEndpoint+0x54f08>
    379c:   38c10310    stmiacc r1, {r4, r8, r9}^
    37a0:   70140000    andsvc  r0, r4, r0
    37a4:   756e4566    strbvc  r4, [lr, #-1382]!
    37a8:   6172656d    cmnvs   r2, sp, ror #10
    37ac:   6e6f6974    mcrvs   9, 3, r6, cr15, cr4, {3}
    37b0:   706d6f43    rsbvc   r6, sp, r3, asr #30
    37b4:   6574656c    ldrbvs  r6, [r4, #-1388]!
    37b8:   03110a00    tsteq   r1, #0  ; 0x0
    37bc:   000019f2    strdeq  r1, [r0], -r2
    37c0:   13002302    movwne  r2, #770    ; 0x302
    37c4:   000001db    ldrdeq  r0, [r0], -fp
            37c4: R_ARM_ABS32   .debug_str
    37c8:   dd03120a    sfmle   f1, 4, [r3, #-40]
    37cc:   02000016    andeq   r0, r0, #22 ; 0x16
    37d0:   61140423    tstvs   r4, r3, lsr #8
    37d4:   75746553    ldrbvc  r6, [r4, #-1363]!
    37d8:   00785470    rsbseq  r5, r8, r0, ror r4
    37dc:   c103130a    tstgt   r3, sl, lsl #6
    37e0:   02000038    andeq   r0, r0, #56 ; 0x38
    37e4:   61140823    tstvs   r4, r3, lsr #16
    37e8:   75746553    ldrbvc  r6, [r4, #-1363]!
    37ec:   00785270    rsbseq  r5, r8, r0, ror r2
    37f0:   d103140a    tstle   r3, sl, lsl #8
    37f4:   02000038    andeq   r0, r0, #56 ; 0x38
    37f8:   61141023    tstvs   r4, r3, lsr #32
    37fc:   64416d62    strbvs  r6, [r1], #-3426
    3800:   73657264    cmnvc   r5, #1073741830 ; 0x40000006
    3804:   150a0073    strne   r0, [sl, #-115]
    3808:   0038e103    eorseq  lr, r8, r3, lsl #2
    380c:   90230300    eorls   r0, r3, r0, lsl #6
    3810:   72491402    subvc   r1, r9, #33554432   ; 0x2000000
    3814:   160a0070    undefined
    3818:   00117703    andseq  r7, r1, r3, lsl #14
    381c:   a0230300    eorge   r0, r3, r0, lsl #6
    3820:   77641402    strbvc  r1, [r4, -r2, lsl #8]!
    3824:   61746144    cmnvs   r4, r4, asr #2
    3828:   03170a00    tsteq   r7, #0  ; 0x0
    382c:   000001a4    andeq   r0, r0, r4, lsr #3
    3830:   02cc2303    sbceq   r2, ip, #201326592  ; 0xc000000
    3834:   61447714    cmpvs   r4, r4, lsl r7
    3838:   0a006174    beq 1be10 <MGC_HdrcDumpEndpoint+0x1750c>
    383c:   01850318    orreq   r0, r5, r8, lsl r3
    3840:   23030000    movwcs  r0, #12288  ; 0x3000
    3844:   621402d0    andsvs  r0, r4, #13 ; 0xd
    3848:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
    384c:   00737365    rsbseq  r7, r3, r5, ror #6
    3850:   7603190a    strvc   r1, [r3], -sl, lsl #18
    3854:   03000001    movweq  r0, #1  ; 0x1
    3858:   1402d223    strne   sp, [r2], #-547
    385c:   61745362    cmnvs   r4, r2, ror #6
    3860:   0a006574    beq 1ce38 <MGC_HdrcDumpEndpoint+0x18534>
    3864:   0176031a    cmneq   r6, sl, lsl r3
    3868:   23030000    movwcs  r0, #12288  ; 0x3000
    386c:   411302d3    ldrsbmi r0, [r3, -r3]
            386f: R_ARM_ABS32   .debug_str
    3870:   0a000001    beq 387c <.debug_info+0x387c>
    3874:   0176031b    cmneq   r6, fp, lsl r3
    3878:   23030000    movwcs  r0, #12288  ; 0x3000
    387c:   621402d4    andsvs  r0, r4, #1073741837 ; 0x4000000d
    3880:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
    3884:   1c0a0074    stcne   0, cr0, [sl], {116}
    3888:   00017603    andeq   r7, r1, r3, lsl #12
    388c:   d5230300    strle   r0, [r3, #-768]!
    3890:   52621402    rsbpl   r1, r2, #33554432   ; 0x2000000
    3894:   69727465    ldmdbvs r2!, {r0, r2, r5, r6, sl, ip, sp, lr}^
    3898:   0a007365    beq 20634 <MGC_HdrcDumpEndpoint+0x1bd30>
    389c:   0176031d    cmneq   r6, sp, lsl r3
    38a0:   23030000    movwcs  r0, #12288  ; 0x3000
    38a4:   621402d6    andsvs  r0, r4, #1610612749 ; 0x6000000d
    38a8:   61746146    cmnvs   r4, r6, asr #2
    38ac:   7465526c    strbtvc r5, [r5], #-620
    38b0:   73656972    cmnvc   r5, #1867776    ; 0x1c8000
    38b4:   031e0a00    tsteq   lr, #0  ; 0x0
    38b8:   00000176    andeq   r0, r0, r6, ror r1
    38bc:   02d72303    sbcseq  r2, r7, #201326592  ; 0xc000000
    38c0:   01761c00    cmneq   r6, r0, lsl #24
    38c4:   38d10000    ldmcc   r1, {}^
    38c8:   dd1d0000    ldcle   0, cr0, [sp]
    38cc:   07000000    streq   r0, [r0, -r0]
    38d0:   01761c00    cmneq   r6, r0, lsl #24
    38d4:   38e10000    stmiacc r1!, {}^
    38d8:   dd1d0000    ldcle   0, cr0, [sp]
    38dc:   ff000000    undefined instruction 0xff000000
    38e0:   01761c00    cmneq   r6, r0, lsl #24
    38e4:   38f10000    ldmcc   r1!, {}^
    38e8:   dd1d0000    ldcle   0, cr0, [sp]
    38ec:   0f000000    svceq   0x00000000
    38f0:   474d1100    strbmi  r1, [sp, -r0, lsl #2]
    38f4:   6e455f43    cdpvs   15, 4, cr5, cr5, cr3, {2}
    38f8:   72656d75    rsbvc   r6, r5, #7488   ; 0x1d40
    38fc:   6f697461    svcvs   0x00697461
    3900:   7461446e    strbtvc r4, [r1], #-1134
    3904:   1f0a0061    svcne   0x000a0061
    3908:   00379803    eorseq  r9, r7, r3, lsl #16
    390c:   a9040700    stmdbge r4, {r8, r9, sl}
    3910:   07000033    smladxeq    r0, r3, r0, r0
    3914:   0019db04    andseq  sp, r9, r4, lsl #22
    3918:   f7040700    undefined instruction 0xf7040700
    391c:   07000018    smladeq r0, r8, r0, r0
    3920:   00167d04    andseq  r7, r6, r4, lsl #26
    3924:   2b040700    blcs    10552c <MGC_HdrcDumpEndpoint+0x100c28>
    3928:   07000039    smladxeq    r0, r9, r0, r0
    392c:   00393104    eorseq  r3, r9, r4, lsl #2
    3930:   0a880800    beq fe205938 <MGC_HdrcDumpEndpoint+0xfe201034>
    3934:   04070000    streq   r0, [r7]
    3938:   00003350    andeq   r3, r0, r0, asr r3
    393c:   43474d11    movtmi  r4, #32017  ; 0x7d11
    3940:   726f505f    rsbvc   r5, pc, #95 ; 0x5f
    3944:   de0a0074    mcrle   0, 0, r0, cr10, cr4, {3}
    3948:   0026e603    eoreq   lr, r6, r3, lsl #12
    394c:   41460200    cmpmi   r6, r0, lsl #4
    3950:   535f4950    cmppl   pc, #1310720    ; 0x140000
    3954:   485f5359    ldmdami pc, {r0, r3, r4, r6, r8, r9, ip, lr}^
    3958:   6c646e61    stclvs  14, cr6, [r4], #-388
    395c:   0b005465    bleq    18af8 <MGC_HdrcDumpEndpoint+0x141f4>
    3960:   0000f147    andeq   pc, r0, r7, asr #2
    3964:   0b041500    bleq    108d6c <MGC_HdrcDumpEndpoint+0x104468>
    3968:   0039bf53    eorseq  fp, r9, r3, asr pc
    396c:   41461600    cmpmi   r6, r0, lsl #12
    3970:   535f4950    cmppl   pc, #1310720    ; 0x140000
    3974:   425f5359    subsmi  r5, pc, #1677721601 ; 0x64000001
    3978:   455f5253    ldrbmi  r5, [pc, #-595] ; 372d <.debug_info+0x372d>
    397c:   5f434558    svcpl   0x00434558
    3980:   45444e55    strbmi  r4, [r4, #-3669]
    3984:   454e4946    strbmi  r4, [lr, #-2374]
    3988:   16000044    strne   r0, [r0], -r4, asr #32
    398c:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    3990:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    3994:   5253425f    subspl  r4, r3, #-268435451 ; 0xf0000005
    3998:   4558455f    ldrbmi  r4, [r8, #-1375]
    399c:   48545f43    ldmdami r4, {r0, r1, r6, r8, r9, sl, fp, ip, lr}^
    39a0:   44414552    strbmi  r4, [r1], #-1362
    39a4:   46160100    ldrmi   r0, [r6], -r0, lsl #2
    39a8:   5f495041    svcpl   0x00495041
    39ac:   5f535953    svcpl   0x00535953
    39b0:   5f525342    svcpl   0x00525342
    39b4:   43455845    movtmi  r5, #22597  ; 0x5845
    39b8:   5152495f    cmppl   r2, pc, asr r9
    39bc:   02000200    andeq   r0, r0, #0  ; 0x0
    39c0:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    39c4:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    39c8:   7273425f    rsbsvc  r4, r3, #-268435451 ; 0xf0000005
    39cc:   63657845    cmnvs   r5, #4521984    ; 0x450000
    39d0:   6f697475    svcvs   0x00697475
    39d4:   756e456e    strbvc  r4, [lr, #-1390]!
    39d8:   0b00546d    bleq    18b94 <MGC_HdrcDumpEndpoint+0x14290>
    39dc:   0039657d    eorseq  r6, r9, sp, ror r5
    39e0:   0b240c00    bleq    9069e8 <MGC_HdrcDumpEndpoint+0x9020e4>
    39e4:   003ab18b    eorseq  fp, sl, fp, lsl #3
    39e8:   72640e00    rsbvc   r0, r4, #0  ; 0x0
    39ec:   72657669    rsbvc   r7, r5, #110100480  ; 0x6900000
    39f0:   656d614e    strbvs  r6, [sp, #-334]!
    39f4:   00727450    rsbseq  r7, r2, r0, asr r4
    39f8:   0116960b    tsteq   r6, fp, lsl #12
    39fc:   23020000    movwcs  r0, #8192   ; 0x2000
    3a00:   72690e00    rsbvc   r0, r9, #0  ; 0x0
    3a04:   006f4e71    rsbeq   r4, pc, r1, ror lr
    3a08:   0195a30b    orrseq  sl, r5, fp, lsl #6
    3a0c:   23020000    movwcs  r0, #8192   ; 0x2000
    3a10:   73620e04    cmnvc   r2, #64 ; 0x40
    3a14:   65784572    ldrbvs  r4, [r8, #-1394]!
    3a18:   69747563    ldmdbvs r4!, {r0, r1, r5, r6, r8, sl, ip, sp, lr}^
    3a1c:   0b006e6f    bleq    1f3e0 <MGC_HdrcDumpEndpoint+0x1aadc>
    3a20:   0039bfb1    ldrhteq fp, [r9], -r1
    3a24:   08230200    stmdaeq r3!, {r9}
    3a28:   696e690e    stmdbvs lr!, {r1, r2, r3, r8, fp, sp, lr}^
    3a2c:   69724474    ldmdbvs r2!, {r2, r4, r5, r6, sl, lr}^
    3a30:   46726576    undefined
    3a34:   00636e75    rsbeq   r6, r3, r5, ror lr
    3a38:   3ab7ce0b    bcc fedf726c <MGC_HdrcDumpEndpoint+0xfedf2968>
    3a3c:   23020000    movwcs  r0, #8192   ; 0x2000
    3a40:   78650e0c    stmdavc r5!, {r2, r3, r9, sl, fp}^
    3a44:   72447469    subvc   r7, r4, #1761607680 ; 0x69000000
    3a48:   72657669    rsbvc   r7, r5, #110100480  ; 0x6900000
    3a4c:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3a50:   f5de0b00    undefined instruction 0xf5de0b00
    3a54:   02000000    andeq   r0, r0, #0  ; 0x0
    3a58:   69141023    ldmdbvs r4, {r0, r1, r5, ip}
    3a5c:   75467273    strbvc  r7, [r6, #-627]
    3a60:   0b00636e    bleq    1c820 <MGC_HdrcDumpEndpoint+0x17f1c>
    3a64:   03e50101    mvneq   r0, #1073741824 ; 0x40000000
    3a68:   23020000    movwcs  r0, #8192   ; 0x2000
    3a6c:   73621414    cmnvc   r2, #335544320  ; 0x14000000
    3a70:   6e754672    mrcvs   6, 3, r4, cr5, cr2, {3}
    3a74:   230b0063    movwcs  r0, #45155  ; 0xb063
    3a78:   003ac901    eorseq  ip, sl, r1, lsl #18
    3a7c:   18230200    stmdane r3!, {r9}
    3a80:   72736214    rsbsvc  r6, r3, #1073741825 ; 0x40000001
    3a84:   63617453    cmnvs   r1, #1392508928 ; 0x53000000
    3a88:   7a69536b    bvc 1a5883c <MGC_HdrcDumpEndpoint+0x1a53f38>
    3a8c:   2e0b0065    cdpcs   0, 0, cr0, cr11, cr5, {3}
    3a90:   0001a401    andeq   sl, r1, r1, lsl #8
    3a94:   1c230200    sfmne   f0, 4, [r3]
    3a98:   72736214    rsbsvc  r6, r3, #1073741825 ; 0x40000001
    3a9c:   6f666946    svcvs   0x00666946
    3aa0:   74706544    ldrbtvc r6, [r0], #-1348
    3aa4:   380b0068    stmdacc fp, {r3, r5, r6}
    3aa8:   0001a401    andeq   sl, r1, r1, lsl #8
    3aac:   20230200    eorcs   r0, r3, r0, lsl #4
    3ab0:   95010f00    strls   r0, [r1, #-3840]
    3ab4:   07000001    streq   r0, [r0, -r1]
    3ab8:   003ab104    eorseq  fp, sl, r4, lsl #2
    3abc:   c9010b00    stmdbgt r1, {r8, r9, fp}
    3ac0:   0a00003a    beq 3bb0 <.debug_info+0x3bb0>
    3ac4:   000001a4    andeq   r0, r0, r4, lsr #3
    3ac8:   bd040700    stclt   7, cr0, [r4]
    3acc:   1100003a    tstne   r0, sl, lsr r0
    3ad0:   49504146    ldmdbmi r0, {r1, r2, r6, r8, lr}^
    3ad4:   5359535f    cmppl   r9, #2080374785 ; 0x7c000001
    3ad8:   6972445f    ldmdbvs r2!, {r0, r1, r2, r3, r4, r6, sl, lr}^
    3adc:   54726576    ldrbtpl r6, [r2], #-1398
    3ae0:   01390b00    teqeq   r9, r0, lsl #22
    3ae4:   000039e1    andeq   r3, r0, r1, ror #19
    3ae8:   50414611    subpl   r4, r1, r1, lsl r6
    3aec:   59535f49    ldmdbpl r3, {r0, r3, r6, r8, r9, sl, fp, ip, lr}^
    3af0:   65535f53    ldrbvs  r5, [r3, #-3923]
    3af4:   6870616d    ldmdavs r0!, {r0, r2, r3, r5, r6, r8, sp, lr}^
    3af8:   5465726f    strbtpl r7, [r5], #-623
    3afc:   03c50c00    biceq   r0, r5, #0  ; 0x0
    3b00:   000001a4    andeq   r0, r0, r4, lsr #3
    3b04:   50414611    subpl   r4, r1, r1, lsl r6
    3b08:   59535f49    ldmdbpl r3, {r0, r3, r6, r8, r9, sl, fp, ip, lr}^
    3b0c:   614d5f53    cmpvs   sp, r3, asr pc
    3b10:   75716c69    ldrbvc  r6, [r1, #-3177]!
    3b14:   54657565    strbtpl r7, [r5], #-1381
    3b18:   03cf0c00    biceq   r0, pc, #0  ; 0x0
    3b1c:   000000f1    strdeq  r0, [r0], -r1
    3b20:   050c5812    streq   r5, [ip, #-2066]
    3b24:   003d8a04    eorseq  r8, sp, r4, lsl #20
    3b28:   6f6c1400    svcvs   0x006c1400
    3b2c:   63536b63    cmpvs   r3, #101376 ; 0x18c00
    3b30:   75646568    strbvc  r6, [r4, #-1384]!
    3b34:   4672656c    ldrbtmi r6, [r2], -ip, ror #10
    3b38:   00636e75    rsbeq   r6, r3, r5, ror lr
    3b3c:   f504140c    undefined instruction 0xf504140c
    3b40:   02000000    andeq   r0, r0, #0  ; 0x0
    3b44:   75140023    ldrvc   r0, [r4, #-35]
    3b48:   636f6c6e    cmnvs   pc, #28160  ; 0x6e00
    3b4c:   6863536b    stmdavs r3!, {r0, r1, r3, r5, r6, r8, r9, ip, lr}^
    3b50:   6c756465    cfldrdvs    mvd6, [r5], #-404
    3b54:   75467265    strbvc  r7, [r6, #-613]
    3b58:   0c00636e    stceq   3, cr6, [r0], {110}
    3b5c:   00f50424    rscseq  r0, r5, r4, lsr #8
    3b60:   23020000    movwcs  r0, #8192   ; 0x2000
    3b64:   6c731404    cfldrdvs    mvd1, [r3], #-16
    3b68:   46706565    ldrbtmi r6, [r0], -r5, ror #10
    3b6c:   00636e75    rsbeq   r6, r3, r5, ror lr
    3b70:   c904310c    stmdbgt r4, {r2, r3, r8, ip, sp}
    3b74:   0200003a    andeq   r0, r0, #58 ; 0x3a
    3b78:   64140823    ldrvs   r0, [r4], #-2083
    3b7c:   62617369    rsbvs   r7, r1, #-1543503871    ; 0xa4000001
    3b80:   7249656c    subvc   r6, r9, #452984832  ; 0x1b000000
    3b84:   6e754671    mrcvs   6, 3, r4, cr5, cr1, {3}
    3b88:   440c0063    strmi   r0, [ip], #-99
    3b8c:   0003e504    andeq   lr, r3, r4, lsl #10
    3b90:   0c230200    sfmeq   f0, 4, [r3]
    3b94:   616e6514    cmnvs   lr, r4, lsl r5
    3b98:   49656c62    stmdbmi r5!, {r1, r5, r6, sl, fp, sp, lr}^
    3b9c:   75467172    strbvc  r7, [r6, #-370]
    3ba0:   0c00636e    stceq   3, cr6, [r0], {110}
    3ba4:   3ac90455    bcc ff244d00 <MGC_HdrcDumpEndpoint+0xff2403fc>
    3ba8:   23020000    movwcs  r0, #8192   ; 0x2000
    3bac:   72631410    rsbvc   r1, r3, #268435456  ; 0x10000000
    3bb0:   65746165    ldrbvs  r6, [r4, #-357]!
    3bb4:   616d6553    cmnvs   sp, r3, asr r5
    3bb8:   726f6870    rsbvc   r6, pc, #7340032    ; 0x700000
    3bbc:   6e754665    cdpvs   6, 7, cr4, cr5, cr5, {3}
    3bc0:   6d0c0063    stcvs   0, cr0, [ip, #-396]
    3bc4:   003d9a04    eorseq  r9, sp, r4, lsl #20
    3bc8:   14230200    strtne  r0, [r3], #-512
    3bcc:   73656414    cmnvc   r5, #335544320  ; 0x14000000
    3bd0:   796f7274    stmdbvc pc!, {r2, r4, r5, r6, r9, ip, sp, lr}^
    3bd4:   616d6553    cmnvs   sp, r3, asr r5
    3bd8:   726f6870    rsbvc   r6, pc, #7340032    ; 0x700000
    3bdc:   6e754665    cdpvs   6, 7, cr4, cr5, cr5, {3}
    3be0:   820c0063    andhi   r0, ip, #99 ; 0x63
    3be4:   003dac04    eorseq  sl, sp, r4, lsl #24
    3be8:   18230200    stmdane r3!, {r9}
    3bec:   74656714    strbtvc r6, [r5], #-1812
    3bf0:   616d6553    cmnvs   sp, r3, asr r5
    3bf4:   726f6870    rsbvc   r6, pc, #7340032    ; 0x700000
    3bf8:   6e754665    cdpvs   6, 7, cr4, cr5, cr5, {3}
    3bfc:   aa0c0063    bge 303d90 <MGC_HdrcDumpEndpoint+0x2ff48c>
    3c00:   003dc704    eorseq  ip, sp, r4, lsl #14
    3c04:   1c230200    sfmne   f0, 4, [r3]
    3c08:   74657314    strbtvc r7, [r5], #-788
    3c0c:   616d6553    cmnvs   sp, r3, asr r5
    3c10:   726f6870    rsbvc   r6, pc, #7340032    ; 0x700000
    3c14:   6e754665    cdpvs   6, 7, cr4, cr5, cr5, {3}
    3c18:   c70c0063    strgt   r0, [ip, -r3, rrx]
    3c1c:   003dde04    eorseq  sp, sp, r4, lsl #28
    3c20:   20230200    eorcs   r0, r3, r0, lsl #4
    3c24:   65726314    ldrbvs  r6, [r2, #-788]!
    3c28:   4d657461    cfstrdmi    mvd7, [r5, #-388]!
    3c2c:   716c6961    cmnvc   ip, r1, ror #18
    3c30:   65756575    ldrbvs  r6, [r5, #-1397]!
    3c34:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3c38:   04e10c00    strbteq r0, [r1], #3072
    3c3c:   00003df9    strdeq  r3, [r0], -r9
    3c40:   14242302    strtne  r2, [r4], #-770
    3c44:   74736564    ldrbtvc r6, [r3], #-1380
    3c48:   4d796f72    ldclmi  15, cr6, [r9, #-456]!
    3c4c:   716c6961    cmnvc   ip, r1, ror #18
    3c50:   65756575    ldrbvs  r6, [r5, #-1397]!
    3c54:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3c58:   04f60c00    ldrbteq r0, [r6], #3072
    3c5c:   00003e0b    andeq   r3, r0, fp, lsl #28
    3c60:   14282302    strtne  r2, [r8], #-770
    3c64:   4d746567    cfldr64mi   mvdx6, [r4, #-412]!
    3c68:   716c6961    cmnvc   ip, r1, ror #18
    3c6c:   65756575    ldrbvs  r6, [r5, #-1397]!
    3c70:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3c74:   05240c00    streq   r0, [r4, #-3072]!
    3c78:   00003e2b    andeq   r3, r0, fp, lsr #28
    3c7c:   142c2302    strtne  r2, [ip], #-770
    3c80:   4d746573    cfldr64mi   mvdx6, [r4, #-460]!
    3c84:   716c6961    cmnvc   ip, r1, ror #18
    3c88:   65756575    ldrbvs  r6, [r5, #-1397]!
    3c8c:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3c90:   054e0c00    strbeq  r0, [lr, #-3072]
    3c94:   00003e2b    andeq   r3, r0, fp, lsr #28
    3c98:   14302302    ldrtne  r2, [r0], #-770
    3c9c:   50746567    rsbspl  r6, r4, r7, ror #10
    3ca0:   41737968    cmnmi   r3, r8, ror #18
    3ca4:   65726464    ldrbvs  r6, [r2, #-1124]!
    3ca8:   75467373    strbvc  r7, [r6, #-883]
    3cac:   0c00636e    stceq   3, cr6, [r0], {110}
    3cb0:   3e41055f    mcrcc   5, 2, r0, cr1, cr15, {2}
    3cb4:   23020000    movwcs  r0, #8192   ; 0x2000
    3cb8:   65671434    strbvs  r1, [r7, #-1076]!
    3cbc:   72695674    rsbvc   r5, r9, #121634816  ; 0x7400000
    3cc0:   64644174    strbtvs r4, [r4], #-372
    3cc4:   73736572    cmnvc   r3, #478150656  ; 0x1c800000
    3cc8:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3ccc:   05700c00    ldrbeq  r0, [r0, #-3072]!
    3cd0:   00003e41    andeq   r3, r0, r1, asr #28
    3cd4:   14382302    ldrtne  r2, [r8], #-770
    3cd8:   6c6c616d    stfvse  f6, [ip], #-436
    3cdc:   7546636f    strbvc  r6, [r6, #-879]
    3ce0:   0c00636e    stceq   3, cr6, [r0], {110}
    3ce4:   3e570584    cdpcc   5, 5, cr0, cr7, cr4, {4}
    3ce8:   23020000    movwcs  r0, #8192   ; 0x2000
    3cec:   616d143c    cmnvs   sp, ip, lsr r4
    3cf0:   636f6c6c    cmnvs   pc, #27648  ; 0x6c00
    3cf4:   61636e55    cmnvs   r3, r5, asr lr
    3cf8:   64656863    strbtvs r6, [r5], #-2147
    3cfc:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3d00:   05980c00    ldreq   r0, [r8, #3072]
    3d04:   00003e57    andeq   r3, r0, r7, asr lr
    3d08:   14402302    strbne  r2, [r0], #-770
    3d0c:   6c6c616d    stfvse  f6, [ip], #-436
    3d10:   6143636f    cmpvs   r3, pc, ror #6
    3d14:   64656863    strbtvs r6, [r5], #-2147
    3d18:   636e7546    cmnvs   lr, #293601280  ; 0x11800000
    3d1c:   05ac0c00    streq   r0, [ip, #3072]!
    3d20:   00003e57    andeq   r3, r0, r7, asr lr
    3d24:   14442302    strbne  r2, [r4], #-770
    3d28:   6c6c616d    stfvse  f6, [ip], #-436
    3d2c:   6553636f    ldrbvs  r6, [r3, #-879]
    3d30:   6e656d67    cdpvs   13, 6, cr6, cr5, cr7, {3}
    3d34:   6e754674    mrcvs   6, 3, r4, cr5, cr4, {3}
    3d38:   c10c0063    tstgt   ip, r3, rrx
    3d3c:   003e7205    eorseq  r7, lr, r5, lsl #4
    3d40:   48230200    stmdami r3!, {r9}
    3d44:   65726614    ldrbvs  r6, [r2, #-1556]!
    3d48:   6e754665    cdpvs   6, 7, cr4, cr5, cr5, {3}
    3d4c:   d30c0063    movwle  r0, #49251  ; 0xc063
    3d50:   00021105    andeq   r1, r2, r5, lsl #2
    3d54:   4c230200    sfmmi   f0, 4, [r3]
    3d58:   69727014    ldmdbvs r2!, {r2, r4, ip, sp, lr}^
    3d5c:   4666746e    strbtmi r7, [r6], -lr, ror #8
    3d60:   00636e75    rsbeq   r6, r3, r5, ror lr
    3d64:   8905e80c    stmdbhi r5, {r2, r3, fp, sp, lr, pc}
    3d68:   0200003e    andeq   r0, r0, #62 ; 0x3e
    3d6c:   69145023    ldmdbvs r4, {r0, r1, r5, ip, lr}
    3d70:   4474696e    ldrbtmi r6, [r4], #-2414
    3d74:   65766972    ldrbvs  r6, [r6, #-2418]!
    3d78:   75467372    strbvc  r7, [r6, #-882]
    3d7c:   0c00636e    stceq   3, cr6, [r0], {110}
    3d80:   3eab0609    cdpcc   6, 10, cr0, cr11, cr9, {0}
    3d84:   23020000    movwcs  r0, #8192   ; 0x2000
    3d88:   01090054    qaddeq  r0, r4, r9
    3d8c:   00003ae8    andeq   r3, r0, r8, ror #21
    3d90:   00003d9a    muleq   r0, sl, sp
    3d94:   0001950a    andeq   r9, r1, sl, lsl #10
    3d98:   04070000    streq   r0, [r7]
    3d9c:   00003d8a    andeq   r3, r0, sl, lsl #27
    3da0:   3dac010b    stfccs  f0, [ip, #44]!
    3da4:   e80a0000    stmda   sl, {}
    3da8:   0000003a    andeq   r0, r0, sl, lsr r0
    3dac:   3da00407    cfstrscc    mvf0, [r0, #28]!
    3db0:   01090000    tsteq   r9, r0
    3db4:   00000195    muleq   r0, r5, r1
    3db8:   00003dc7    andeq   r3, r0, r7, asr #27
    3dbc:   003ae80a    eorseq  lr, sl, sl, lsl #16
    3dc0:   01a40a00    undefined instruction 0x01a40a00
    3dc4:   07000000    streq   r0, [r0, -r0]
    3dc8:   003db204    eorseq  fp, sp, r4, lsl #4
    3dcc:   de010b00    fmacdle d0, d1, d0
    3dd0:   0a00003d    beq 3ecc <.debug_info+0x3ecc>
    3dd4:   00003ae8    andeq   r3, r0, r8, ror #21
    3dd8:   0001a40a    andeq   sl, r1, sl, lsl #8
    3ddc:   04070000    streq   r0, [r7]
    3de0:   00003dcd    andeq   r3, r0, sp, asr #27
    3de4:   3b040109    blcc    104210 <MGC_HdrcDumpEndpoint+0xff90c>
    3de8:   3df90000    ldclcc  0, cr0, [r9]
    3dec:   a40a0000    strge   r0, [sl]
    3df0:   0a000001    beq 3dfc <.debug_info+0x3dfc>
    3df4:   000001a4    andeq   r0, r0, r4, lsr #3
    3df8:   e4040700    str r0, [r4], #-1792
    3dfc:   0b00003d    bleq    3ef8 <.debug_info+0x3ef8>
    3e00:   003e0b01    eorseq  r0, lr, r1, lsl #22
    3e04:   3b040a00    blcc    10660c <MGC_HdrcDumpEndpoint+0x101d08>
    3e08:   07000000    streq   r0, [r0, -r0]
    3e0c:   003dff04    eorseq  pc, sp, r4, lsl #30
    3e10:   95010900    strls   r0, [r1, #-2304]
    3e14:   2b000001    blcs    3e20 <.debug_info+0x3e20>
    3e18:   0a00003e    beq 3f18 <.debug_info+0x3f18>
    3e1c:   00003b04    andeq   r3, r0, r4, lsl #22
    3e20:   0000f10a    andeq   pc, r0, sl, lsl #2
    3e24:   01a40a00    undefined instruction 0x01a40a00
    3e28:   07000000    streq   r0, [r0, -r0]
    3e2c:   003e1104    eorseq  r1, lr, r4, lsl #2
    3e30:   a4010900    strge   r0, [r1], #-2304
    3e34:   41000001    tstmi   r0, r1
    3e38:   0a00003e    beq 3f38 <.debug_info+0x3f38>
    3e3c:   000001a4    andeq   r0, r0, r4, lsr #3
    3e40:   31040700    tstcc   r4, r0, lsl #14
    3e44:   0900003e    stmdbeq r0, {r1, r2, r3, r4, r5}
    3e48:   0000f101    andeq   pc, r0, r1, lsl #2
    3e4c:   003e5700    eorseq  r5, lr, r0, lsl #14
    3e50:   00860a00    addeq   r0, r6, r0, lsl #20
    3e54:   07000000    streq   r0, [r0, -r0]
    3e58:   003e4704    eorseq  r4, lr, r4, lsl #14
    3e5c:   f1010900    undefined instruction 0xf1010900
    3e60:   72000000    andvc   r0, r0, #0  ; 0x0
    3e64:   0a00003e    beq 3f64 <.debug_info+0x3f64>
    3e68:   0000394d    andeq   r3, r0, sp, asr #18
    3e6c:   0000860a    andeq   r8, r0, sl, lsl #12
    3e70:   04070000    streq   r0, [r7]
    3e74:   00003e5d    andeq   r3, r0, sp, asr lr
    3e78:   01950109    orrseq  r0, r5, r9, lsl #2
    3e7c:   3e890000    cdpcc   0, 8, cr0, cr9, cr0, {0}
    3e80:   160a0000    strne   r0, [sl], -r0
    3e84:   1e000001    cdpne   0, 0, cr0, cr0, cr1, {0}
    3e88:   78040700    stmdavc r4, {r8, r9, sl}
    3e8c:   0900003e    stmdbeq r0, {r1, r2, r3, r4, r5}
    3e90:   00019501    andeq   r9, r1, r1, lsl #10
    3e94:   003e9f00    eorseq  r9, lr, r0, lsl #30
    3e98:   3e9f0a00    cdpcc   10, 9, cr0, cr15, cr0, {0}
    3e9c:   07000000    streq   r0, [r0, -r0]
    3ea0:   003ea504    eorseq  sl, lr, r4, lsl #10
    3ea4:   cf040700    svcgt   0x00040700
    3ea8:   0700003a    smladxeq    r0, sl, r0, r0
    3eac:   003e8f04    eorseq  r8, lr, r4, lsl #30
    3eb0:   41461100    cmpmi   r6, r0, lsl #2
    3eb4:   535f4950    cmppl   pc, #1310720    ; 0x140000
    3eb8:   535f5359    cmppl   pc, #1677721601 ; 0x64000001
    3ebc:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
    3ec0:   54736563    ldrbtpl r6, [r3], #-1379
    3ec4:   060b0c00    streq   r0, [fp], -r0, lsl #24
    3ec8:   00003b20    andeq   r3, r0, r0, lsr #22
    3ecc:   474d011f    smlaldmi    r0, sp, pc, r1
    3ed0:   64485f43    strbvs  r5, [r8], #-3907
    3ed4:   6e496372    mcrvs   3, 2, r6, cr9, cr2, {3}
    3ed8:   01007469    tsteq   r0, r9, ror #8
    3edc:   0176018e    cmneq   r6, lr, lsl #3
    3ee0:   00000000    andeq   r0, r0, r0
            3ee2: R_ARM_ABS32   .text
    3ee4:   01380000    teqeq   r8, r0
            3ee6: R_ARM_ABS32   .text
    3ee8:   00000000    andeq   r0, r0, r0
            3eea: R_ARM_ABS32   .debug_loc
    3eec:   3f3a0000    svccc   0x003a0000
    3ef0:   4a200000    bmi 800008 <MGC_HdrcDumpEndpoint+0x7fb704>
            3ef3: R_ARM_ABS32   .debug_str
    3ef4:   01000000    tsteq   r0, r0
    3ef8:   003f3a8d    eorseq  r3, pc, sp, lsl #21
    3efc:   60910200    addsvs  r0, r1, r0, lsl #4
    3f00:   6e6f6321    cdpvs   3, 6, cr6, cr15, cr1, {1}
    3f04:   00676966    rsbeq   r6, r7, r6, ror #18
    3f08:   01769301    cmneq   r6, r1, lsl #6
    3f0c:   91020000    tstls   r2, r0
    3f10:   49702167    ldmdbmi r0!, {r0, r1, r2, r5, r6, r8, sp}^
    3f14:   436c706d    cmnmi   ip, #109    ; 0x6d
    3f18:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
    3f1c:   656c6c6f    strbvs  r6, [ip, #-3183]!
    3f20:   94010072    strls   r0, [r1], #-114
    3f24:   0000390d    andeq   r3, r0, sp, lsl #18
    3f28:   22689102    rsbcs   r9, r8, #-2147483648    ; 0x80000000
    3f2c:   000000b2    strheq  r0, [r0], -r2
            3f2c: R_ARM_ABS32   .debug_str
    3f30:   0f289501    svceq   0x00289501
    3f34:   91020000    tstls   r2, r0
    3f38:   0407006c    streq   r0, [r7], #-108
    3f3c:   0000393c    andeq   r3, r0, ip, lsr r9
    3f40:   474d0123    strbmi  r0, [sp, -r3, lsr #2]
    3f44:   64485f43    strbvs  r5, [r8], #-3907
    3f48:   79446372    stmdbvc r4, {r1, r4, r5, r6, r8, r9, sp, lr}^
    3f4c:   696d616e    stmdbvs sp!, {r1, r2, r3, r5, r6, r8, sp, lr}^
    3f50:   66694663    strbtvs r4, [r9], -r3, ror #12
    3f54:   636f4c6f    cmnvs   pc, #28416  ; 0x6f00
    3f58:   6f697461    svcvs   0x00697461
    3f5c:   f601006e    undefined instruction 0xf601006e
    3f60:   00013801    andeq   r3, r1, r1, lsl #16
            3f61: R_ARM_ABS32   .text
    3f64:   00017000    andeq   r7, r1, r0
            3f65: R_ARM_ABS32   .text
    3f68:   00002a00    andeq   r2, r0, r0, lsl #20
            3f69: R_ARM_ABS32   .debug_loc
    3f6c:   003fed00    eorseq  lr, pc, r0, lsl #26
    3f70:   004a2000    subeq   r2, sl, r0
            3f72: R_ARM_ABS32   .debug_str
    3f74:   ef010000    svc 0x00010000
    3f78:   00003f3a    andeq   r3, r0, sl, lsr pc
    3f7c:   206c9102    rsbcs   r9, ip, r2, lsl #2
    3f80:   000001c6    andeq   r0, r0, r6, asr #3
            3f80: R_ARM_ABS32   .debug_str
    3f84:   0176f001    cmnpeq  r6, r1
    3f88:   91020000    tstls   r2, r0
    3f8c:   49622468    stmdbmi r2!, {r3, r5, r6, sl, sp}^
    3f90:   00785473    rsbseq  r5, r8, r3, ror r4
    3f94:   0176f101    cmnpeq  r6, r1, lsl #2
    3f98:   91020000    tstls   r2, r0
    3f9c:   53622464    cmnpl   r2, #1677721600 ; 0x64000000
    3fa0:   01007465    tsteq   r0, r5, ror #8
    3fa4:   000176f2    strdeq  r7, [r1], -r2
    3fa8:   60910200    addsvs  r0, r1, r0, lsl #4
    3fac:   77647024    strbvc  r7, [r4, -r4, lsr #32]!
    3fb0:   65736142    ldrbvs  r6, [r3, #-322]!
    3fb4:   ddf30100    ldflee  f0, [r3]
    3fb8:   02000013    andeq   r0, r0, #19 ; 0x13
    3fbc:   70240091    mlavc   r4, r1, r0, r0
    3fc0:   7a695362    bvc 1a58d50 <MGC_HdrcDumpEndpoint+0x1a5444c>
    3fc4:   00333865    eorseq  r3, r3, r5, ror #16
    3fc8:   0f28f401    svceq   0x0028f401
    3fcc:   91020000    tstls   r2, r0
    3fd0:   62702404    rsbsvs  r2, r0, #67108864   ; 0x4000000
    3fd4:   62756f44    rsbsvs  r6, r5, #272    ; 0x110
    3fd8:   7542656c    strbvc  r6, [r2, #-1388]
    3fdc:   72656666    rsbvc   r6, r5, #106954752  ; 0x6600000
    3fe0:   01006465    tsteq   r0, r5, ror #8
    3fe4:   000f28f5    strdeq  r2, [pc], -r5
    3fe8:   08910200    ldmeq   r1, {r9}
    3fec:   4d012500    cfstr32mi   mvfx2, [r1]
    3ff0:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    3ff4:   49637264    stmdbmi r3!, {r2, r5, r6, r9, ip, sp, lr}^
    3ff8:   01007273    tsteq   r0, r3, ror r2
    3ffc:   a9010139    stmdbge r1, {r0, r3, r4, r5, r8}
    4000:   70000000    andvc   r0, r0, r0
            4003: R_ARM_ABS32   .text
    4004:   3c000001    stccc   0, cr0, [r0], {1}
            4007: R_ARM_ABS32   .text
    4008:   54000002    strpl   r0, [r0], #-2
            400b: R_ARM_ABS32   .debug_loc
    400c:   c6000000    strgt   r0, [r0], -r0
    4010:   26000040    strcs   r0, [r0], -r0, asr #32
    4014:   000001cb    andeq   r0, r0, fp, asr #3
            4014: R_ARM_ABS32   .debug_str
    4018:   f1013801    undefined instruction 0xf1013801
    401c:   02000000    andeq   r0, r0, #0  ; 0x0
    4020:   72275091    eorvc   r5, r7, #145    ; 0x91
    4024:   6c757365    ldclvs  3, cr7, [r5], #-404
    4028:   3a010074    bcc 44200 <MGC_HdrcDumpEndpoint+0x3f8fc>
    402c:   0000a901    andeq   sl, r0, r1, lsl #18
    4030:   54910200    ldrpl   r0, [r1], #512
    4034:   6e496227    cdpvs   2, 4, cr6, cr9, cr7, {1}
    4038:   73557274    cmpvc   r5, #1073741831 ; 0x40000007
    403c:   6c615662    stclvs  6, cr5, [r1], #-392
    4040:   01006575    tsteq   r0, r5, ror r5
    4044:   0176013b    cmneq   r6, fp, lsr r1
    4048:   91020000    tstls   r2, r0
    404c:   4977275b    ldmdbmi r7!, {r0, r1, r3, r4, r6, r8, r9, sl, sp}^
    4050:   5472746e    ldrbtpl r7, [r2], #-1134
    4054:   6c615678    stclvs  6, cr5, [r1], #-480
    4058:   01006575    tsteq   r0, r5, ror r5
    405c:   0185013c    orreq   r0, r5, ip, lsr r1
    4060:   91020000    tstls   r2, r0
    4064:   4977275c    ldmdbmi r7!, {r2, r3, r4, r6, r8, r9, sl, sp}^
    4068:   5272746e    rsbspl  r7, r2, #1845493760 ; 0x6e000000
    406c:   6c615678    stclvs  6, cr5, [r1], #-480
    4070:   01006575    tsteq   r0, r5, ror r5
    4074:   0185013c    orreq   r0, r5, ip, lsr r1
    4078:   91020000    tstls   r2, r0
    407c:   00fb285e    rscseq  r2, fp, lr, asr r8
            407e: R_ARM_ABS32   .debug_str
    4080:   3d010000    stccc   0, cr0, [r1]
    4084:   0040c601    subeq   ip, r0, r1, lsl #12
    4088:   60910200    addsvs  r0, r1, r0, lsl #4
    408c:   6f437027    svcvs   0x00437027
    4090:   6f72746e    svcvs   0x0072746e
    4094:   72656c6c    rsbvc   r6, r5, #27648  ; 0x6c00
    4098:   6c706d49    ldclvs  13, cr6, [r0], #-292
    409c:   013e0100    teqeq   lr, r0, lsl #2
    40a0:   0000390d    andeq   r3, r0, sp, lsl #18
    40a4:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    40a8:   000000b2    strheq  r0, [r0], -r2
            40a8: R_ARM_ABS32   .debug_str
    40ac:   28014401    stmdacs r1, {r0, sl, lr}
    40b0:   0200000f    andeq   r0, r0, #15 ; 0xf
    40b4:   41286891    strbmi  r6, [r8, -r1]!
            40b7: R_ARM_ABS32   .debug_str
    40b8:   01000001    tsteq   r0, r1
    40bc:   01760146    cmneq   r6, r6, asr #2
    40c0:   91020000    tstls   r2, r0
    40c4:   0407006f    streq   r0, [r7], #-111
    40c8:   00000336    andeq   r0, r0, r6, lsr r3
    40cc:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    40d0:   64485f43    strbvs  r5, [r8], #-3907
    40d4:   74536372    ldrbvc  r6, [r3], #-882
    40d8:   00747261    rsbseq  r7, r4, r1, ror #4
    40dc:   01016601    tsteq   r1, r1, lsl #12
    40e0:   000001a4    andeq   r0, r0, r4, lsr #3
    40e4:   0000023c    andeq   r0, r0, ip, lsr r2
            40e4: R_ARM_ABS32   .text
    40e8:   00000490    muleq   r0, r0, r4
            40e8: R_ARM_ABS32   .text
    40ec:   0000007e    andeq   r0, r0, lr, ror r0
            40ec: R_ARM_ABS32   .debug_loc
    40f0:   00004131    andeq   r4, r0, r1, lsr r1
    40f4:   0000fb26    andeq   pc, r0, r6, lsr #22
            40f5: R_ARM_ABS32   .debug_str
    40f8:   01650100    cmneq   r5, r0, lsl #2
    40fc:   0000390d    andeq   r3, r0, sp, lsl #18
    4100:   27609102    strbcs  r9, [r0, -r2, lsl #2]!
    4104:   006c6176    rsbeq   r6, ip, r6, ror r1
    4108:   85016701    strhi   r6, [r1, #-1793]
    410c:   02000001    andeq   r0, r0, #1  ; 0x1
    4110:   b2286691    eorlt   r6, r8, #152043520  ; 0x9100000
            4113: R_ARM_ABS32   .debug_str
    4114:   01000000    tsteq   r0, r0
    4118:   0f280168    svceq   0x00280168
    411c:   91020000    tstls   r2, r0
    4120:   004a2868    subeq   r2, sl, r8, ror #16
            4122: R_ARM_ABS32   .debug_str
    4124:   69010000    stmdbvs r1, {}
    4128:   003f3a01    eorseq  r3, pc, r1, lsl #20
    412c:   6c910200    lfmvs   f0, 4, [r1], {0}
    4130:   4d012500    cfstr32mi   mvfx2, [r1]
    4134:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    4138:   53637264    cmnpl   r3, #1073741830 ; 0x40000006
    413c:   00706f74    rsbseq  r6, r0, r4, ror pc
    4140:   01018401    tsteq   r1, r1, lsl #8
    4144:   000001a4    andeq   r0, r0, r4, lsr #3
    4148:   00000490    muleq   r0, r0, r4
            4148: R_ARM_ABS32   .text
    414c:   0000052c    andeq   r0, r0, ip, lsr #10
            414c: R_ARM_ABS32   .text
    4150:   000000a8    andeq   r0, r0, r8, lsr #1
            4150: R_ARM_ABS32   .debug_loc
    4154:   00004187    andeq   r4, r0, r7, lsl #3
    4158:   0000fb26    andeq   pc, r0, r6, lsr #22
            4159: R_ARM_ABS32   .debug_str
    415c:   01830100    orreq   r0, r3, r0, lsl #2
    4160:   0000390d    andeq   r3, r0, sp, lsl #18
    4164:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    4168:   706d6574    rsbvc   r6, sp, r4, ror r5
    416c:   01850100    orreq   r0, r5, r0, lsl #2
    4170:   00000185    andeq   r0, r0, r5, lsl #3
    4174:   286a9102    stmdacs sl!, {r1, r8, ip, pc}^
    4178:   000000b2    strheq  r0, [r0], -r2
            4178: R_ARM_ABS32   .debug_str
    417c:   28018601    stmdacs r1, {r0, r9, sl, pc}
    4180:   0200000f    andeq   r0, r0, #15 ; 0xf
    4184:   25006c91    strcs   r6, [r0, #-3217]
    4188:   43474d01    movtmi  r4, #32001  ; 0x7d01
    418c:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4190:   73654463    cmnvc   r5, #1660944384 ; 0x63000000
    4194:   796f7274    stmdbvc pc!, {r2, r4, r5, r6, r9, ip, sp, lr}^
    4198:   01960100    orrseq  r0, r6, r0, lsl #2
    419c:   0001a401    andeq   sl, r1, r1, lsl #8
    41a0:   00052c00    andeq   r2, r5, r0, lsl #24
            41a1: R_ARM_ABS32   .text
    41a4:   00059800    andeq   r9, r5, r0, lsl #16
            41a5: R_ARM_ABS32   .text
    41a8:   0000d200    andeq   sp, r0, r0, lsl #4
            41a9: R_ARM_ABS32   .debug_loc
    41ac:   0041d000    subeq   sp, r1, r0
    41b0:   00fb2600    rscseq  r2, fp, r0, lsl #12
            41b2: R_ARM_ABS32   .debug_str
    41b4:   95010000    strls   r0, [r1]
    41b8:   00390d01    eorseq  r0, r9, r1, lsl #26
    41bc:   68910200    ldmvs   r1, {r9}
    41c0:   00004a28    andeq   r4, r0, r8, lsr #20
            41c1: R_ARM_ABS32   .debug_str
    41c4:   01970100    orrseq  r0, r7, r0, lsl #2
    41c8:   00003f3a    andeq   r3, r0, sl, lsr pc
    41cc:   006c9102    rsbeq   r9, ip, r2, lsl #2
    41d0:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    41d4:   64485f43    strbvs  r5, [r8], #-3907
    41d8:   65526372    ldrbvs  r6, [r2, #-882]
    41dc:   75426461    strbvc  r6, [r2, #-1121]
    41e0:   61745373    cmnvs   r4, r3, ror r3
    41e4:   01006574    tsteq   r0, r4, ror r5
    41e8:   a40101a3    strge   r0, [r1], #-419
    41ec:   98000001    stmdals r0, {r0}
            41ef: R_ARM_ABS32   .text
    41f0:   88000005    stmdahi r0, {r0, r2}
            41f3: R_ARM_ABS32   .text
    41f4:   fc000007    stc2    0, cr0, [r0], {7}
            41f7: R_ARM_ABS32   .debug_loc
    41f8:   60000000    andvs   r0, r0, r0
    41fc:   26000042    strcs   r0, [r0], -r2, asr #32
    4200:   0000004a    andeq   r0, r0, sl, asr #32
            4200: R_ARM_ABS32   .debug_str
    4204:   3a01a201    bcc 6ca10 <MGC_HdrcDumpEndpoint+0x6810c>
    4208:   0200003f    andeq   r0, r0, #63 ; 0x3f
    420c:   64276091    strtvs  r6, [r7], #-145
    4210:   74637665    strbtvc r7, [r3], #-1637
    4214:   a401006c    strge   r0, [r1], #-108
    4218:   00017601    andeq   r7, r1, r1, lsl #12
    421c:   65910200    ldrvs   r0, [r1, #512]
    4220:   776f7027    strbvc  r7, [pc, -r7, lsr #32]!
    4224:   01007265    tsteq   r0, r5, ror #4
    4228:   017601a5    cmneq   r6, r5, lsr #3
    422c:   91020000    tstls   r2, r0
    4230:   62762766    rsbsvs  r2, r6, #26738688   ; 0x1980000
    4234:   01007375    tsteq   r0, r5, ror r3
    4238:   017601a6    cmneq   r6, r6, lsr #3
    423c:   91020000    tstls   r2, r0
    4240:   00fb2867    rscseq  r2, fp, r7, ror #16
            4242: R_ARM_ABS32   .debug_str
    4244:   a7010000    strge   r0, [r1, -r0]
    4248:   00390d01    eorseq  r0, r9, r1, lsl #26
    424c:   68910200    ldmvs   r1, {r9}
    4250:   0000b228    andeq   fp, r0, r8, lsr #4
            4251: R_ARM_ABS32   .debug_str
    4254:   01a80100    undefined instruction 0x01a80100
    4258:   00000f28    andeq   r0, r0, r8, lsr #30
    425c:   006c9102    rsbeq   r9, ip, r2, lsl #2
    4260:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    4264:   64485f43    strbvs  r5, [r8], #-3907
    4268:   72506372    subsvc  r6, r0, #-939524095 ; 0xc8000001
    426c:   6172676f    cmnvs   r2, pc, ror #14
    4270:   7375426d    cmnvc   r5, #-805306362 ; 0xd0000006
    4274:   74617453    strbtvc r7, [r1], #-1107
    4278:   da010065    ble 44414 <MGC_HdrcDumpEndpoint+0x3fb10>
    427c:   01a40101    undefined instruction 0x01a40101
    4280:   07880000    streq   r0, [r8, r0]
            4282: R_ARM_ABS32   .text
    4284:   0b600000    bleq    1800008 <MGC_HdrcDumpEndpoint+0x17fb704>
            4286: R_ARM_ABS32   .text
    4288:   01260000    teqeq   r6, r0
            428a: R_ARM_ABS32   .debug_loc
    428c:   43170000    tstmi   r7, #0  ; 0x0
    4290:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            4293: R_ARM_ABS32   .debug_str
    4294:   01000000    tsteq   r0, r0
    4298:   3f3a01d9    svccc   0x003a01d9
    429c:   91020000    tstls   r2, r0
    42a0:   00fb285c    rscseq  r2, fp, ip, asr r8
            42a2: R_ARM_ABS32   .debug_str
    42a4:   db010000    blle    442ac <MGC_HdrcDumpEndpoint+0x3f9a8>
    42a8:   00390d01    eorseq  r0, r9, r1, lsl #26
    42ac:   60910200    addsvs  r0, r1, r0, lsl #4
    42b0:   0000b228    andeq   fp, r0, r8, lsr #4
            42b1: R_ARM_ABS32   .debug_str
    42b4:   01dc0100    bicseq  r0, ip, r0, lsl #2
    42b8:   00000f28    andeq   r0, r0, r8, lsr #30
    42bc:   27649102    strbcs  r9, [r4, -r2, lsl #2]!
    42c0:   65776f70    ldrbvs  r6, [r7, #-3952]!
    42c4:   dd010072    stcle   0, cr0, [r1, #-456]
    42c8:   00017601    andeq   r7, r1, r1, lsl #12
    42cc:   6b910200    blvs    fe444ad4 <MGC_HdrcDumpEndpoint+0xfe4401d0>
    42d0:   6f506e27    svcvs   0x00506e27
    42d4:   00726577    rsbseq  r6, r2, r7, ror r5
    42d8:   7601de01    strvc   sp, [r1], -r1, lsl #28
    42dc:   02000001    andeq   r0, r0, #1  ; 0x1
    42e0:   64276c91    strtvs  r6, [r7], #-3217
    42e4:   74637665    strbtvc r7, [r3], #-1637
    42e8:   df01006c    svcle   0x0001006c
    42ec:   00017601    andeq   r7, r1, r1, lsl #12
    42f0:   6d910200    lfmvs   f0, 4, [r1]
    42f4:   65446e27    strbvs  r6, [r4, #-3623]
    42f8:   6c744376    ldclvs  3, cr4, [r4], #-472
    42fc:   01e00100    mvneq   r0, r0, lsl #2
    4300:   00000176    andeq   r0, r0, r6, ror r1
    4304:   286e9102    stmdacs lr!, {r1, r8, ip, pc}^
    4308:   000000c0    andeq   r0, r0, r0, asr #1
            4308: R_ARM_ABS32   .debug_str
    430c:   7601e101    strvc   lr, [r1], -r1, lsl #2
    4310:   02000001    andeq   r0, r0, #1  ; 0x1
    4314:   25006f91    strcs   r6, [r0, #-3985]
    4318:   43474d01    movtmi  r4, #32001  ; 0x7d01
    431c:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4320:   6e694263    cdpvs   2, 6, cr4, cr9, cr3, {3}
    4324:   646e4564    strbtvs r4, [lr], #-1380
    4328:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    432c:   44010074    strmi   r0, [r1], #-116
    4330:   26e00102    strbtcs r0, [r0], r2, lsl #2
    4334:   0b600000    bleq    1800008 <MGC_HdrcDumpEndpoint+0x17fb704>
            4336: R_ARM_ABS32   .text
    4338:   10d00000    sbcsne  r0, r0, r0
            433a: R_ARM_ABS32   .text
    433c:   01500000    cmpeq   r0, r0
            433e: R_ARM_ABS32   .debug_loc
    4340:   44170000    ldrmi   r0, [r7]
    4344:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            4347: R_ARM_ABS32   .debug_str
    4348:   01000000    tsteq   r0, r0
    434c:   3f3a0240    svccc   0x003a0240
    4350:   91020000    tstls   r2, r0
    4354:   55702954    ldrbpl  r2, [r0, #-2388]!
    4358:   6e456273    mcrvs   2, 2, r6, cr5, cr3, {3}
    435c:   41010064    tstmi   r1, r4, rrx
    4360:   00346702    eorseq  r6, r4, r2, lsl #14
    4364:   50910200    addspl  r0, r1, r0, lsl #4
    4368:   65527029    ldrbvs  r7, [r2, #-41]
    436c:   73657571    cmnvc   r5, #473956352  ; 0x1c400000
    4370:   42010074    andmi   r0, r1, #116    ; 0x74
    4374:   00339102    eorseq  r9, r3, r2, lsl #2
    4378:   4c910200    lfmmi   f0, 4, [r1], {0}
    437c:   69426229    stmdbvs r2, {r0, r3, r5, r9, sp, lr}^
    4380:   0100646e    tsteq   r0, lr, ror #8
    4384:   01760243    cmneq   r6, r3, asr #4
    4388:   91020000    tstls   r2, r0
    438c:   01c62848    biceq   r2, r6, r8, asr #16
            438e: R_ARM_ABS32   .debug_str
    4390:   45010000    strmi   r0, [r1]
    4394:   00017602    andeq   r7, r1, r2, lsl #12
    4398:   59910200    ldmibpl r1, {r9}
    439c:   73496227    movtvc  r6, #37415  ; 0x9227
    43a0:   01007854    tsteq   r0, r4, asr r8
    43a4:   01760245    cmneq   r6, r5, asr #4
    43a8:   91020000    tstls   r2, r0
    43ac:   00992858    addseq  r2, r9, r8, asr r8
            43ae: R_ARM_ABS32   .debug_str
    43b0:   45010000    strmi   r0, [r1]
    43b4:   00017602    andeq   r7, r1, r2, lsl #12
    43b8:   5a910200    bpl fe444bc0 <MGC_HdrcDumpEndpoint+0xfe4402bc>
    43bc:   00008628    andeq   r8, r0, r8, lsr #12
            43bd: R_ARM_ABS32   .debug_str
    43c0:   02460100    subeq   r0, r6, #0  ; 0x0
    43c4:   00000176    andeq   r0, r0, r6, ror r1
    43c8:   285b9102    ldmdacs fp, {r1, r8, ip, pc}^
    43cc:   000000da    ldrdeq  r0, [r0], -sl
            43cc: R_ARM_ABS32   .debug_str
    43d0:   e0024701    and r4, r2, r1, lsl #14
    43d4:   02000026    andeq   r0, r0, #38 ; 0x26
    43d8:   72275c91    eorvc   r5, r7, #37120  ; 0x9100
    43dc:   01006765    tsteq   r0, r5, ror #14
    43e0:   01760248    cmneq   r6, r8, asr #4
    43e4:   91020000    tstls   r2, r0
    43e8:   73632761    cmnvc   r3, #25427968   ; 0x1840000
    43ec:   49010072    stmdbmi r1, {r1, r4, r5, r6}
    43f0:   00018502    andeq   r8, r1, r2, lsl #10
    43f4:   62910200    addsvs  r0, r1, #0  ; 0x0
    43f8:   0000fb28    andeq   pc, r0, r8, lsr #22
            43f9: R_ARM_ABS32   .debug_str
    43fc:   024a0100    subeq   r0, sl, #0  ; 0x0
    4400:   0000390d    andeq   r3, r0, sp, lsl #18
    4404:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    4408:   000000b2    strheq  r0, [r0], -r2
            4408: R_ARM_ABS32   .debug_str
    440c:   28024b01    stmdacs r2, {r0, r8, r9, fp, lr}
    4410:   0200000f    andeq   r0, r0, #15 ; 0xf
    4414:   25006891    strcs   r6, [r0, #-2193]
    4418:   43474d01    movtmi  r4, #32001  ; 0x7d01
    441c:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4420:   61745363    cmnvs   r4, r3, ror #6
    4424:   78527472    ldmdavc r2, {r1, r4, r5, r6, sl, ip, sp, lr}^
    4428:   02fa0100    rscseq  r0, sl, #0  ; 0x0
    442c:   0001a401    andeq   sl, r1, r1, lsl #8
    4430:   0010d000    andseq  sp, r0, r0
            4431: R_ARM_ABS32   .text
    4434:   001aac00    andseq  sl, sl, r0, lsl #24
            4435: R_ARM_ABS32   .text
    4438:   00017a00    andeq   r7, r1, r0, lsl #20
            4439: R_ARM_ABS32   .debug_loc
    443c:   00456b00    subeq   r6, r5, r0, lsl #22
    4440:   004a2600    subeq   r2, sl, r0, lsl #12
            4442: R_ARM_ABS32   .debug_str
    4444:   f7010000    undefined instruction 0xf7010000
    4448:   003f3a02    eorseq  r3, pc, r2, lsl #20
    444c:   bc910300    ldclt   3, cr0, [r1], {0}
    4450:   00da267f    sbcseq  r2, sl, pc, ror r6
            4452: R_ARM_ABS32   .debug_str
    4454:   f7010000    undefined instruction 0xf7010000
    4458:   0026e002    eoreq   lr, r6, r2
    445c:   b8910300    ldmlt   r1, {r8, r9}
    4460:   00b8267f    adcseq  r2, r8, pc, ror r6
            4462: R_ARM_ABS32   .debug_str
    4464:   f8010000    undefined instruction 0xf8010000
    4468:   000f2802    andeq   r2, pc, r2, lsl #16
    446c:   b4910300    ldrlt   r0, [r1], #768
    4470:   0059267f    subseq  r2, r9, pc, ror r6
            4472: R_ARM_ABS32   .debug_str
    4474:   f8010000    undefined instruction 0xf8010000
    4478:   0001a402    andeq   sl, r1, r2, lsl #8
    447c:   b0910300    addslt  r0, r1, r0, lsl #6
    4480:   4970297f    ldmdbmi r0!, {r0, r1, r2, r3, r4, r5, r6, r8, fp, sp}^
    4484:   01007072    tsteq   r0, r2, ror r0
    4488:   00f102f9    ldrshteq    r0, [r1], #41
    448c:   91020000    tstls   r2, r0
    4490:   017d2600    cmneq   sp, r0, lsl #12
            4492: R_ARM_ABS32   .debug_str
    4494:   f9010000    undefined instruction 0xf9010000
    4498:   00017602    andeq   r7, r1, r2, lsl #12
    449c:   ac910300    ldcge   3, cr0, [r1], {0}
    44a0:   00df287f    sbcseq  r2, pc, pc, ror r8
            44a2: R_ARM_ABS32   .debug_str
    44a4:   fc010000    stc2    0, cr0, [r1], {0}
    44a8:   00217e02    eoreq   r7, r1, r2, lsl #28
    44ac:   48910200    ldmmi   r1, {r9}
    44b0:   00007a28    andeq   r7, r0, r8, lsr #20
            44b1: R_ARM_ABS32   .debug_str
    44b4:   02fd0100    rscseq  r0, sp, #0  ; 0x0
    44b8:   00001e59    andeq   r1, r0, r9, asr lr
    44bc:   274c9102    strbcs  r9, [ip, -r2, lsl #2]
    44c0:   616d4462    cmnvs   sp, r2, ror #8
    44c4:   01006b4f    tsteq   r0, pc, asr #22
    44c8:   017602fe    ldrsheq r0, [r6, #-46]!
    44cc:   91020000    tstls   r2, r0
    44d0:   43772753    cmnmi   r7, #21757952   ; 0x14c0000
    44d4:   01007273    tsteq   r0, r3, ror r2
    44d8:   01850300    orreq   r0, r5, r0, lsl #6
    44dc:   91020000    tstls   r2, r0
    44e0:   01a52854    undefined instruction 0x01a52854
            44e2: R_ARM_ABS32   .debug_str
    44e4:   00010000    andeq   r0, r1, r0
    44e8:   00018503    andeq   r8, r1, r3, lsl #10
    44ec:   56910200    ldrpl   r0, [r1], r0, lsl #4
    44f0:   0001d228    andeq   sp, r1, r8, lsr #4
            44f1: R_ARM_ABS32   .debug_str
    44f4:   03000100    movweq  r0, #256    ; 0x100
    44f8:   00000185    andeq   r0, r0, r5, lsl #3
    44fc:   27589102    ldrbcs  r9, [r8, -r2, lsl #2]
    4500:   65744971    ldrbvs  r4, [r4, #-2417]!
    4504:   0101006d    tsteq   r1, sp, rrx
    4508:   00229003    eoreq   r9, r2, r3
    450c:   40910200    addsmi  r0, r1, r0, lsl #4
    4510:   00013228    andeq   r3, r1, r8, lsr #4
            4511: R_ARM_ABS32   .debug_str
    4514:   03020100    movweq  r0, #8448   ; 0x2100
    4518:   00000176    andeq   r0, r0, r6, ror r1
    451c:   275a9102    ldrbcs  r9, [sl, -r2, lsl #2]
    4520:   00676572    rsbeq   r6, r7, r2, ror r5
    4524:   76030301    strvc   r0, [r3], -r1, lsl #6
    4528:   02000001    andeq   r0, r0, #1  ; 0x1
    452c:   fb285b91    blx a16e4e <MGC_HdrcDumpEndpoint+0xa1254a>
            452f: R_ARM_ABS32   .debug_str
    4530:   01000000    tsteq   r0, r0
    4534:   390d0304    stmdbcc sp, {r2, r8, r9}
    4538:   91020000    tstls   r2, r0
    453c:   00b2285c    adcseq  r2, r2, ip, asr r8
            453e: R_ARM_ABS32   .debug_str
    4540:   05010000    streq   r0, [r1]
    4544:   000f2803    andeq   r2, pc, r3, lsl #16
    4548:   60910200    addsvs  r0, r1, r0, lsl #4
    454c:   00018728    andeq   r8, r1, r8, lsr #14
            454d: R_ARM_ABS32   .debug_str
    4550:   03060100    movweq  r0, #24832  ; 0x6100
    4554:   0000339d    muleq   r0, sp, r3
    4558:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    455c:   000001c6    andeq   r0, r0, r6, asr #3
            455c: R_ARM_ABS32   .debug_str
    4560:   76030701    strvc   r0, [r3], -r1, lsl #14
    4564:   02000001    andeq   r0, r0, #1  ; 0x1
    4568:   25006b91    strcs   r6, [r0, #-2961]
    456c:   43474d01    movtmi  r4, #32001  ; 0x7d01
    4570:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4574:   61745363    cmnvs   r4, r3, ror #6
    4578:   78547472    ldmdavc r4, {r1, r4, r5, r6, sl, ip, sp, lr}^
    457c:   03f40100    mvnseq  r0, #0  ; 0x0
    4580:   0001a401    andeq   sl, r1, r1, lsl #8
    4584:   001aac00    andseq  sl, sl, r0, lsl #24
            4585: R_ARM_ABS32   .text
    4588:   001f7c00    andseq  r7, pc, r0, lsl #24
            4589: R_ARM_ABS32   .text
    458c:   0001a400    andeq   sl, r1, r0, lsl #8
            458d: R_ARM_ABS32   .debug_loc
    4590:   0046d800    subeq   sp, r6, r0, lsl #16
    4594:   004a2600    subeq   r2, sl, r0, lsl #12
            4596: R_ARM_ABS32   .debug_str
    4598:   f1010000    setend  le
    459c:   003f3a03    eorseq  r3, pc, r3, lsl #20
    45a0:   b4910300    ldrlt   r0, [r1], #768
    45a4:   00da267f    sbcseq  r2, sl, pc, ror r6
            45a6: R_ARM_ABS32   .debug_str
    45a8:   f1010000    setend  le
    45ac:   0026e003    eoreq   lr, r6, r3
    45b0:   b0910300    addslt  r0, r1, r0, lsl #6
    45b4:   00b8267f    adcseq  r2, r8, pc, ror r6
            45b6: R_ARM_ABS32   .debug_str
    45b8:   f2010000    vhadd.s8    d0, d1, d0
    45bc:   00066703    andeq   r6, r6, r3, lsl #14
    45c0:   ac910300    ldcge   3, cr0, [r1], {0}
    45c4:   0059267f    subseq  r2, r9, pc, ror r6
            45c6: R_ARM_ABS32   .debug_str
    45c8:   f2010000    vhadd.s8    d0, d1, d0
    45cc:   0001a403    andeq   sl, r1, r3, lsl #8
    45d0:   a8910300    ldmge   r1, {r8, r9}
    45d4:   4770297f    undefined
    45d8:   72496e65    subvc   r6, r9, #1616   ; 0x650
    45dc:   f3010070    vqadd.u8    q0, <illegal reg q0.5>, q8
    45e0:   0000f103    andeq   pc, r0, r3, lsl #2
    45e4:   00910200    addseq  r0, r1, r0, lsl #4
    45e8:   0000df28    andeq   sp, r0, r8, lsr #30
            45e9: R_ARM_ABS32   .debug_str
    45ec:   03f60100    mvnseq  r0, #0  ; 0x0
    45f0:   0000217e    andeq   r2, r0, lr, ror r1
    45f4:   7fb89103    svcvc   0x00b89103
    45f8:   00007a28    andeq   r7, r0, r8, lsr #20
            45f9: R_ARM_ABS32   .debug_str
    45fc:   03f70100    mvnseq  r0, #0  ; 0x0
    4600:   00001e59    andeq   r1, r0, r9, asr lr
    4604:   7fbc9103    svcvc   0x00bc9103
    4608:   6d446227    sfmvs   f6, 2, [r4, #-156]
    460c:   006b4f61    rsbeq   r4, fp, r1, ror #30
    4610:   7603f801    strvc   pc, [r3], -r1, lsl #16
    4614:   02000001    andeq   r0, r0, #1  ; 0x1
    4618:   64274391    strtvs  r4, [r7], #-913
    461c:   616f4c77    smcvs   62663
    4620:   756f4364    strbvc  r4, [pc, #-868]!    ; 42c4 <.debug_info+0x42c4>
    4624:   0100746e    tsteq   r0, lr, ror #8
    4628:   01a403fa    strdeq  r0, [r4, sl]!
    462c:   91020000    tstls   r2, r0
    4630:   43772744    cmnmi   r7, #17825792   ; 0x1100000
    4634:   01007273    tsteq   r0, r3, ror r2
    4638:   018503fb    strdeq  r0, [r5, fp]
    463c:   91020000    tstls   r2, r0
    4640:   65722748    ldrbvs  r2, [r2, #-1864]!
    4644:   fc010067    stc2    0, cr0, [r1], {103}
    4648:   00017603    andeq   r7, r1, r3, lsl #12
    464c:   4b910200    blmi    fe444e54 <MGC_HdrcDumpEndpoint+0xfe440550>
    4650:   0000fb28    andeq   pc, r0, r8, lsr #22
            4651: R_ARM_ABS32   .debug_str
    4654:   03fd0100    mvnseq  r0, #0  ; 0x0
    4658:   0000390d    andeq   r3, r0, sp, lsl #18
    465c:   284c9102    stmdacs ip, {r1, r8, ip, pc}^
    4660:   0000017d    andeq   r0, r0, sp, ror r1
            4660: R_ARM_ABS32   .debug_str
    4664:   7603ff01    strvc   pc, [r3], -r1, lsl #30
    4668:   02000001    andeq   r0, r0, #1  ; 0x1
    466c:   70275391    mlavc   r7, r1, r3, r5
    4670:   00707249    rsbseq  r7, r0, r9, asr #4
    4674:   d8040001    stmdale r4, {r0}
    4678:   02000046    andeq   r0, r0, #70 ; 0x46
    467c:   70275491    mlavc   r7, r1, r4, r5
    4680:   636f7349    cmnvs   pc, #603979777  ; 0x24000001
    4684:   70724968    rsbsvc  r4, r2, r8, ror #18
    4688:   04010100    streq   r0, [r1], #-256
    468c:   000046de    ldrdeq  r4, [r0], -lr
    4690:   27589102    ldrbcs  r9, [r8, -r2, lsl #2]
    4694:   6e6f4370    mcrvs   3, 3, r4, cr15, cr0, {3}
    4698:   6c6f7274    sfmvs   f7, 2, [pc], #-464
    469c:   00707249    rsbseq  r7, r0, r9, asr #4
    46a0:   e4040201    str r0, [r4], #-513
    46a4:   02000046    andeq   r0, r0, #70 ; 0x46
    46a8:   87285c91    undefined
            46ab: R_ARM_ABS32   .debug_str
    46ac:   01000001    tsteq   r0, r1
    46b0:   339d0403    orrscc  r0, sp, #50331648   ; 0x3000000
    46b4:   91020000    tstls   r2, r0
    46b8:   00b22860    adcseq  r2, r2, r0, ror #16
            46ba: R_ARM_ABS32   .debug_str
    46bc:   05010000    streq   r0, [r1]
    46c0:   000f2804    andeq   r2, pc, r4, lsl #16
    46c4:   64910200    ldrvs   r0, [r1], #512
    46c8:   0001c628    andeq   ip, r1, r8, lsr #12
            46c9: R_ARM_ABS32   .debug_str
    46cc:   04060100    streq   r0, [r6], #-256
    46d0:   00000176    andeq   r0, r0, r6, ror r1
    46d4:   006b9102    rsbeq   r9, fp, r2, lsl #2
    46d8:   12880407    addne   r0, r8, #117440512  ; 0x7000000
    46dc:   04070000    streq   r0, [r7]
    46e0:   000013e3    andeq   r1, r0, r3, ror #7
    46e4:   11770407    cmnne   r7, r7, lsl #8
    46e8:   01250000    teqeq   r5, r0
    46ec:   5f43474d    svcpl   0x0043474d
    46f0:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
    46f4:   73756c46    cmnvc   r5, #17920  ; 0x4600
    46f8:   646e4568    strbtvs r4, [lr], #-1384
    46fc:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    4700:   8a010074    bhi 448d8 <MGC_HdrcDumpEndpoint+0x3ffd4>
    4704:   01a40104    undefined instruction 0x01a40104
    4708:   1f7c0000    svcne   0x007c0000
            470a: R_ARM_ABS32   .text
    470c:   23140000    tstcs   r4, #0  ; 0x0
            470e: R_ARM_ABS32   .text
    4710:   01ce0000    biceq   r0, lr, r0
            4712: R_ARM_ABS32   .debug_loc
    4714:   47a70000    strmi   r0, [r7, r0]!
    4718:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            471b: R_ARM_ABS32   .debug_str
    471c:   01000000    tsteq   r0, r0
    4720:   3f3a0488    svccc   0x003a0488
    4724:   91020000    tstls   r2, r0
    4728:   00da265c    sbcseq  r2, sl, ip, asr r6
            472a: R_ARM_ABS32   .debug_str
    472c:   88010000    stmdahi r1, {}
    4730:   0026e004    eoreq   lr, r6, r4
    4734:   58910200    ldmpl   r1, {r9}
    4738:   69446229    stmdbvs r4, {r0, r3, r5, r9, sp, lr}^
    473c:   89010072    stmdbhi r1, {r1, r4, r5, r6}
    4740:   00017604    andeq   r7, r1, r4, lsl #12
    4744:   54910200    ldrpl   r0, [r1], #512
    4748:   65526229    ldrbvs  r6, [r2, #-553]
    474c:   00657375    rsbeq   r7, r5, r5, ror r3
    4750:   76048901    strvc   r8, [r4], -r1, lsl #18
    4754:   02000001    andeq   r0, r0, #1  ; 0x1
    4758:   77275091    undefined
    475c:   00727343    rsbseq  r7, r2, r3, asr #6
    4760:   85048b01    strhi   r8, [r4, #-2817]
    4764:   02000001    andeq   r0, r0, #1  ; 0x1
    4768:   fb286291    blx a18a4e <MGC_HdrcDumpEndpoint+0xa1414a>
            476b: R_ARM_ABS32   .debug_str
    476c:   01000000    tsteq   r0, r0
    4770:   390d048c    stmdbcc sp, {r2, r3, r7, sl}
    4774:   91020000    tstls   r2, r0
    4778:   00b22864    adcseq  r2, r2, r4, ror #16
            477a: R_ARM_ABS32   .debug_str
    477c:   8d010000    stchi   0, cr0, [r1]
    4780:   000f2804    andeq   r2, pc, r4, lsl #16
    4784:   68910200    ldmvs   r1, {r9}
    4788:   78546227    ldmdavc r4, {r0, r1, r2, r5, r9, sp, lr}^
    478c:   048e0100    streq   r0, [lr], #256
    4790:   00000176    andeq   r0, r0, r6, ror r1
    4794:   286e9102    stmdacs lr!, {r1, r8, ip, pc}^
    4798:   000001c6    andeq   r0, r0, r6, asr #3
            4798: R_ARM_ABS32   .debug_str
    479c:   76048f01    strvc   r8, [r4], -r1, lsl #30
    47a0:   02000001    andeq   r0, r0, #1  ; 0x1
    47a4:   25006f91    strcs   r6, [r0, #-3985]
    47a8:   43474d01    movtmi  r4, #32001  ; 0x7d01
    47ac:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    47b0:   6c614863    stclvs  8, cr4, [r1], #-396
    47b4:   646e4574    strbtvs r4, [lr], #-1396
    47b8:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
    47bc:   e1010074    tst r1, r4, ror r0
    47c0:   01a40104    undefined instruction 0x01a40104
    47c4:   23140000    tstcs   r4, #0  ; 0x0
            47c6: R_ARM_ABS32   .text
    47c8:   25cc0000    strbcs  r0, [ip]
            47ca: R_ARM_ABS32   .text
    47cc:   01f80000    mvnseq  r0, r0
            47ce: R_ARM_ABS32   .debug_loc
    47d0:   48520000    ldmdami r2, {}^
    47d4:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            47d7: R_ARM_ABS32   .debug_str
    47d8:   01000000    tsteq   r0, r0
    47dc:   3f3a04df    svccc   0x003a04df
    47e0:   91020000    tstls   r2, r0
    47e4:   00da2660    sbcseq  r2, sl, r0, ror #12
            47e6: R_ARM_ABS32   .debug_str
    47e8:   df010000    svcle   0x00010000
    47ec:   0026e004    eoreq   lr, r6, r4
    47f0:   5c910200    lfmpl   f0, 4, [r1], {0}
    47f4:   69446229    stmdbvs r4, {r0, r3, r5, r9, sp, lr}^
    47f8:   e0010072    and r0, r1, r2, ror r0
    47fc:   00017604    andeq   r7, r1, r4, lsl #12
    4800:   58910200    ldmpl   r1, {r9}
    4804:   61486229    cmpvs   r8, r9, lsr #4
    4808:   0100746c    tsteq   r0, ip, ror #8
    480c:   017604e0    cmneq   r6, r0, ror #9
    4810:   91020000    tstls   r2, r0
    4814:   61762754    cmnvs   r6, r4, asr r7
    4818:   e201006c    and r0, r1, #108    ; 0x6c
    481c:   00018504    andeq   r8, r1, r4, lsl #10
    4820:   64910200    ldrvs   r0, [r1], #512
    4824:   78546227    ldmdavc r4, {r0, r1, r2, r5, r9, sp, lr}^
    4828:   04e30100    strbteq r0, [r3], #256
    482c:   00000176    andeq   r0, r0, r6, ror r1
    4830:   28679102    stmdacs r7!, {r1, r8, ip, pc}^
    4834:   000000fb    strdeq  r0, [r0], -fp
            4834: R_ARM_ABS32   .debug_str
    4838:   0d04e401    cfstrseq    mvf14, [r4, #-4]
    483c:   02000039    andeq   r0, r0, #57 ; 0x39
    4840:   b2286891    eorlt   r6, r8, #9502720    ; 0x910000
            4843: R_ARM_ABS32   .debug_str
    4844:   01000000    tsteq   r0, r0
    4848:   0f2804e5    svceq   0x002804e5
    484c:   91020000    tstls   r2, r0
    4850:   0125006c    teqeq   r5, ip, rrx
    4854:   5f43474d    svcpl   0x0043474d
    4858:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
    485c:   61666544    cmnvs   r6, r4, asr #10
    4860:   45746c75    ldrbmi  r6, [r4, #-3189]!
    4864:   6552646e    ldrbvs  r6, [r2, #-1134]
    4868:   6e6f7073    mcrvs   0, 3, r7, cr15, cr3, {3}
    486c:   01006573    tsteq   r0, r3, ror r5
    4870:   a4010559    strge   r0, [r1], #-1369
    4874:   cc000001    stcgt   0, cr0, [r0], {1}
            4877: R_ARM_ABS32   .text
    4878:   1c000025    stcne   0, cr0, [r0], {37}
            487b: R_ARM_ABS32   .text
    487c:   22000028    andcs   r0, r0, #40 ; 0x28
            487f: R_ARM_ABS32   .debug_loc
    4880:   fb000002    blx 4892 <.debug_info+0x4892>
    4884:   26000048    strcs   r0, [r0], -r8, asr #32
    4888:   0000004a    andeq   r0, r0, sl, asr #32
            4888: R_ARM_ABS32   .debug_str
    488c:   3a055801    bcc 15a898 <MGC_HdrcDumpEndpoint+0x155f94>
    4890:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4894:   62295c91    eorvs   r5, r9, #37120  ; 0x9100
    4898:   6c617453    cfstrdvs    mvd7, [r1], #-332
    489c:   5801006c    stmdapl r1, {r2, r3, r5, r6}
    48a0:   00017605    andeq   r7, r1, r5, lsl #12
    48a4:   58910200    ldmpl   r1, {r9}
    48a8:   69537727    ldmdbvs r3, {r0, r1, r2, r5, r8, r9, sl, ip, sp, lr}^
    48ac:   0100657a    tsteq   r0, sl, ror r5
    48b0:   0185055a    orreq   r0, r5, sl, asr r5
    48b4:   91020000    tstls   r2, r0
    48b8:   43772762    cmnmi   r7, #25690112   ; 0x1880000
    48bc:   61567273    cmpvs   r6, r3, ror r2
    48c0:   5a01006c    bpl 44a78 <MGC_HdrcDumpEndpoint+0x40174>
    48c4:   00018505    andeq   r8, r1, r5, lsl #10
    48c8:   64910200    ldrvs   r0, [r1], #512
    48cc:   61566227    cmpvs   r6, r7, lsr #4
    48d0:   5b01006c    blpl    44a88 <MGC_HdrcDumpEndpoint+0x40184>
    48d4:   00017605    andeq   r7, r1, r5, lsl #12
    48d8:   67910200    ldrvs   r0, [r1, r0, lsl #4]
    48dc:   0000fb28    andeq   pc, r0, r8, lsr #22
            48dd: R_ARM_ABS32   .debug_str
    48e0:   055c0100    ldrbeq  r0, [ip, #-256]
    48e4:   0000390d    andeq   r3, r0, sp, lsl #18
    48e8:   28689102    stmdacs r8!, {r1, r8, ip, pc}^
    48ec:   000000b2    strheq  r0, [r0], -r2
            48ec: R_ARM_ABS32   .debug_str
    48f0:   28055d01    stmdacs r5, {r0, r8, sl, fp, ip, lr}
    48f4:   0200000f    andeq   r0, r0, #15 ; 0xf
    48f8:   25006c91    strcs   r6, [r0, #-3217]
    48fc:   43474d01    movtmi  r4, #32001  ; 0x7d01
    4900:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4904:   72655363    rsbvc   r5, r5, #-1946157055    ; 0x8c000001
    4908:   65636976    strbvs  r6, [r3, #-2422]!
    490c:   61666544    cmnvs   r6, r4, asr #10
    4910:   45746c75    ldrbmi  r6, [r4, #-3189]!
    4914:   0100646e    tsteq   r0, lr, ror #8
    4918:   7601058f    strvc   r0, [r1], -pc, lsl #11
    491c:   1c000001    stcne   0, cr0, [r0], {1}
            491f: R_ARM_ABS32   .text
    4920:   70000028    andvc   r0, r0, r8, lsr #32
            4923: R_ARM_ABS32   .text
    4924:   4c00002f    stcmi   0, cr0, [r0], {47}
            4927: R_ARM_ABS32   .debug_loc
    4928:   d5000002    strle   r0, [r0, #-2]
    492c:   26000049    strcs   r0, [r0], -r9, asr #32
    4930:   0000004a    andeq   r0, r0, sl, asr #32
            4930: R_ARM_ABS32   .debug_str
    4934:   3a058e01    bcc 168140 <MGC_HdrcDumpEndpoint+0x16383c>
    4938:   0200003f    andeq   r0, r0, #63 ; 0x3f
    493c:   74265c91    strtvc  r5, [r6], #-3217
            493f: R_ARM_ABS32   .debug_str
    4940:   01000000    tsteq   r0, r0
    4944:   35f1058e    ldrbcc  r0, [r1, #1422]!
    4948:   91020000    tstls   r2, r0
    494c:   43772758    cmnmi   r7, #23068672   ; 0x1600000
    4950:   61567273    cmpvs   r6, r3, ror r2
    4954:   9001006c    andls   r0, r1, ip, rrx
    4958:   00018505    andeq   r8, r1, r5, lsl #10
    495c:   60910200    addsvs  r0, r1, r0, lsl #4
    4960:   6f437727    svcvs   0x00437727
    4964:   00746e75    rsbseq  r6, r4, r5, ror lr
    4968:   85059001    strhi   r9, [r5, #-1]
    496c:   02000001    andeq   r0, r0, #1  ; 0x1
    4970:   62276291    eorvs   r6, r7, #268435465  ; 0x10000009
    4974:   74736554    ldrbtvc r6, [r3], #-1364
    4978:   006c6156    rsbeq   r6, ip, r6, asr r1
    497c:   76059501    strvc   r9, [r5], -r1, lsl #10
    4980:   02000001    andeq   r0, r0, #1  ; 0x1
    4984:   62276491    eorvs   r6, r7, #-1862270976    ; 0x91000000
    4988:   6f727245    svcvs   0x00727245
    498c:   96010072    undefined
    4990:   00017605    andeq   r7, r1, r5, lsl #12
    4994:   65910200    ldrvs   r0, [r1, #512]
    4998:   00013228    andeq   r3, r1, r8, lsr #4
            4999: R_ARM_ABS32   .debug_str
    499c:   05970100    ldreq   r0, [r7, #256]
    49a0:   00000176    andeq   r0, r0, r6, ror r1
    49a4:   28669102    stmdacs r6!, {r1, r8, ip, pc}^
    49a8:   0000013a    andeq   r0, r0, sl, lsr r1
            49a8: R_ARM_ABS32   .debug_str
    49ac:   76059801    strvc   r9, [r5], -r1, lsl #16
    49b0:   02000001    andeq   r0, r0, #1  ; 0x1
    49b4:   da286791    ble a19e4c <MGC_HdrcDumpEndpoint+0xa15548>
            49b7: R_ARM_ABS32   .debug_str
    49b8:   01000000    tsteq   r0, r0
    49bc:   26e00599    usatcs  r0, #0, r9, lsl #11
    49c0:   91020000    tstls   r2, r0
    49c4:   00b22868    adcseq  r2, r2, r8, ror #16
            49c6: R_ARM_ABS32   .debug_str
    49c8:   9a010000    bls 449d0 <MGC_HdrcDumpEndpoint+0x400cc>
    49cc:   000f2805    andeq   r2, pc, r5, lsl #16
    49d0:   6c910200    lfmvs   f0, 4, [r1], {0}
    49d4:   4d012500    cfstr32mi   mvfx2, [r1]
    49d8:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    49dc:   53637264    cmnpl   r3, #1073741830 ; 0x40000006
    49e0:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
    49e4:   72546563    subsvc  r6, r4, #415236096  ; 0x18c00000
    49e8:   6d736e61    ldclvs  14, cr6, [r3, #-388]!
    49ec:   76417469    strbvc  r7, [r1], -r9, ror #8
    49f0:   006c6961    rsbeq   r6, ip, r1, ror #18
    49f4:   01062d01    tsteq   r6, r1, lsl #26
    49f8:   00000176    andeq   r0, r0, r6, ror r1
    49fc:   00002f70    andeq   r2, r0, r0, ror pc
            49fc: R_ARM_ABS32   .text
    4a00:   000034a0    andeq   r3, r0, r0, lsr #9
            4a00: R_ARM_ABS32   .text
    4a04:   00000276    andeq   r0, r0, r6, ror r2
            4a04: R_ARM_ABS32   .debug_loc
    4a08:   00004aa4    andeq   r4, r0, r4, lsr #21
    4a0c:   00004a26    andeq   r4, r0, r6, lsr #20
            4a0d: R_ARM_ABS32   .debug_str
    4a10:   062c0100    strteq  r0, [ip], -r0, lsl #2
    4a14:   00003f3a    andeq   r3, r0, sl, lsr pc
    4a18:   26509102    ldrbcs  r9, [r0], -r2, lsl #2
    4a1c:   00000107    andeq   r0, r0, r7, lsl #2
            4a1c: R_ARM_ABS32   .debug_str
    4a20:   85062c01    strhi   r2, [r6, #-3073]
    4a24:   02000001    andeq   r0, r0, #1  ; 0x1
    4a28:   74264c91    strtvc  r4, [r6], #-3217
            4a2b: R_ARM_ABS32   .debug_str
    4a2c:   01000000    tsteq   r0, r0
    4a30:   35f1062c    ldrbcc  r0, [r1, #1580]!
    4a34:   91020000    tstls   r2, r0
    4a38:   56772748    ldrbtpl r2, [r7], -r8, asr #14
    4a3c:   01006c61    tsteq   r0, r1, ror #24
    4a40:   0185062e    orreq   r0, r5, lr, lsr #12
    4a44:   91020000    tstls   r2, r0
    4a48:   00da2856    sbcseq  r2, sl, r6, asr r8
            4a4a: R_ARM_ABS32   .debug_str
    4a4c:   2f010000    svccs   0x00010000
    4a50:   0026e006    eoreq   lr, r6, r6
    4a54:   58910200    ldmpl   r1, {r9}
    4a58:   00013228    andeq   r3, r1, r8, lsr #4
            4a59: R_ARM_ABS32   .debug_str
    4a5c:   06300100    ldrteq  r0, [r0], -r0, lsl #2
    4a60:   00000176    andeq   r0, r0, r6, ror r1
    4a64:   285f9102    ldmdacs pc, {r1, r8, ip, pc}^
    4a68:   0000013a    andeq   r0, r0, sl, lsr r1
            4a68: R_ARM_ABS32   .debug_str
    4a6c:   a4063101    strge   r3, [r6], #-257
    4a70:   02000001    andeq   r0, r0, #1  ; 0x1
    4a74:   c6286091    undefined
            4a77: R_ARM_ABS32   .debug_str
    4a78:   01000001    tsteq   r0, r1
    4a7c:   01760632    cmneq   r6, r2, lsr r6
    4a80:   91020000    tstls   r2, r0
    4a84:   00fb2867    rscseq  r2, fp, r7, ror #16
            4a86: R_ARM_ABS32   .debug_str
    4a88:   33010000    movwcc  r0, #4096   ; 0x1000
    4a8c:   00390d06    eorseq  r0, r9, r6, lsl #26
    4a90:   68910200    ldmvs   r1, {r9}
    4a94:   0000b228    andeq   fp, r0, r8, lsr #4
            4a95: R_ARM_ABS32   .debug_str
    4a98:   06340100    ldrteq  r0, [r4], -r0, lsl #2
    4a9c:   00000f28    andeq   r0, r0, r8, lsr #30
    4aa0:   006c9102    rsbeq   r9, ip, r2, lsl #2
    4aa4:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    4aa8:   64485f43    strbvs  r5, [r8], #-3907
    4aac:   65536372    ldrbvs  r6, [r3, #-882]
    4ab0:   63697672    cmnvs   r9, #119537664  ; 0x7200000
    4ab4:   63655265    cmnvs   r5, #1342177286 ; 0x50000006
    4ab8:   65766965    ldrbvs  r6, [r6, #-2405]!
    4abc:   64616552    strbtvs r6, [r1], #-1362
    4ac0:   d7010079    smlsdxle    r1, r9, r0, r0
    4ac4:   01760106    cmneq   r6, r6, lsl #2
    4ac8:   34a00000    strtcc  r0, [r0]
            4aca: R_ARM_ABS32   .text
    4acc:   3b140000    blcc    500008 <MGC_HdrcDumpEndpoint+0x4fb704>
            4ace: R_ARM_ABS32   .text
    4ad0:   02a00000    adceq   r0, r0, #0  ; 0x0
            4ad2: R_ARM_ABS32   .debug_loc
    4ad4:   4b810000    blmi    fe044adc <MGC_HdrcDumpEndpoint+0xfe0401d8>
    4ad8:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            4adb: R_ARM_ABS32   .debug_str
    4adc:   01000000    tsteq   r0, r0
    4ae0:   3f3a06d6    svccc   0x003a06d6
    4ae4:   91020000    tstls   r2, r0
    4ae8:   01072654    tsteq   r7, r4, asr r6
            4aea: R_ARM_ABS32   .debug_str
    4aec:   d6010000    strle   r0, [r1], -r0
    4af0:   00018506    andeq   r8, r1, r6, lsl #10
    4af4:   50910200    addspl  r0, r1, r0, lsl #4
    4af8:   00007426    andeq   r7, r0, r6, lsr #8
            4af9: R_ARM_ABS32   .debug_str
    4afc:   06d60100    ldrbeq  r0, [r6], r0, lsl #2
    4b00:   000035f1    strdeq  r3, [r0], -r1
    4b04:   274c9102    strbcs  r9, [ip, -r2, lsl #2]
    4b08:   6c615677    stclvs  6, cr5, [r1], #-476
    4b0c:   06d80100    ldrbeq  r0, [r8], r0, lsl #2
    4b10:   00000185    andeq   r0, r0, r5, lsl #3
    4b14:   285a9102    ldmdacs sl, {r1, r8, ip, pc}^
    4b18:   000000da    ldrdeq  r0, [r0], -sl
            4b18: R_ARM_ABS32   .debug_str
    4b1c:   e006d901    and sp, r6, r1, lsl #18
    4b20:   02000026    andeq   r0, r0, #38 ; 0x26
    4b24:   a5285c91    strge   r5, [r8, #-3217]!
            4b27: R_ARM_ABS32   .debug_str
    4b28:   01000001    tsteq   r0, r1
    4b2c:   018506da    ldrdeq  r0, [r5, sl]
    4b30:   91020000    tstls   r2, r0
    4b34:   01c62860    biceq   r2, r6, r0, ror #16
            4b36: R_ARM_ABS32   .debug_str
    4b38:   db010000    blle    44b40 <MGC_HdrcDumpEndpoint+0x4023c>
    4b3c:   00017606    andeq   r7, r1, r6, lsl #12
    4b40:   62910200    addsvs  r0, r1, #0  ; 0x0
    4b44:   00013228    andeq   r3, r1, r8, lsr #4
            4b45: R_ARM_ABS32   .debug_str
    4b48:   06dc0100    ldrbeq  r0, [ip], r0, lsl #2
    4b4c:   00000176    andeq   r0, r0, r6, ror r1
    4b50:   28639102    stmdacs r3!, {r1, r8, ip, pc}^
    4b54:   0000013a    andeq   r0, r0, sl, lsr r1
            4b54: R_ARM_ABS32   .debug_str
    4b58:   a406dd01    strge   sp, [r6], #-3329
    4b5c:   02000001    andeq   r0, r0, #1  ; 0x1
    4b60:   fb286491    blx a1924e <MGC_HdrcDumpEndpoint+0xa1494a>
            4b63: R_ARM_ABS32   .debug_str
    4b64:   01000000    tsteq   r0, r0
    4b68:   390d06de    stmdbcc sp, {r1, r2, r3, r4, r6, r7, r9, sl}
    4b6c:   91020000    tstls   r2, r0
    4b70:   00b22868    adcseq  r2, r2, r8, ror #16
            4b72: R_ARM_ABS32   .debug_str
    4b74:   df010000    svcle   0x00010000
    4b78:   000f2806    andeq   r2, pc, r6, lsl #16
    4b7c:   6c910200    lfmvs   f0, 4, [r1], {0}
    4b80:   474d2a00    strbmi  r2, [sp, -r0, lsl #20]
    4b84:   64485f43    strbvs  r5, [r8], #-3907
    4b88:   65526372    ldrbvs  r6, [r2, #-882]
    4b8c:   4f746573    svcmi   0x00746573
    4b90:   01006666    tsteq   r0, r6, ror #12
    4b94:   1401079d    strne   r0, [r1], #-1949
            4b97: R_ARM_ABS32   .text
    4b98:   7800003b    stmdavc r0, {r0, r1, r3, r4, r5}
            4b9b: R_ARM_ABS32   .text
    4b9c:   ca00003b    bgt f4 <.debug_info+0xf4>
            4b9f: R_ARM_ABS32   .debug_loc
    4ba0:   f7000002    undefined instruction 0xf7000002
    4ba4:   2600004b    strcs   r0, [r0], -fp, asr #32
    4ba8:   000001cb    andeq   r0, r0, fp, asr #3
            4ba8: R_ARM_ABS32   .debug_str
    4bac:   f1079c01    undefined instruction 0xf1079c01
    4bb0:   02000000    andeq   r0, r0, #0  ; 0x0
    4bb4:   77296091    undefined
    4bb8:   656d6954    strbvs  r6, [sp, #-2388]!
    4bbc:   9c010072    stcls   0, cr0, [r1], {114}
    4bc0:   00018507    andeq   r8, r1, r7, lsl #10
    4bc4:   5c910200    lfmpl   f0, 4, [r1], {0}
    4bc8:   65526227    ldrbvs  r6, [r2, #-551]
    4bcc:   9e010067    cdpls   0, 0, cr0, cr1, cr7, {3}
    4bd0:   00017607    andeq   r7, r1, r7, lsl #12
    4bd4:   67910200    ldrvs   r0, [r1, r0, lsl #4]
    4bd8:   0000fb28    andeq   pc, r0, r8, lsr #22
            4bd9: R_ARM_ABS32   .debug_str
    4bdc:   079f0100    ldreq   r0, [pc, r0, lsl #2]
    4be0:   0000390d    andeq   r3, r0, sp, lsl #18
    4be4:   28689102    stmdacs r8!, {r1, r8, ip, pc}^
    4be8:   000000b2    strheq  r0, [r0], -r2
            4be8: R_ARM_ABS32   .debug_str
    4bec:   2807a001    stmdacs r7, {r0, sp, pc}
    4bf0:   0200000f    andeq   r0, r0, #15 ; 0xf
    4bf4:   2a006c91    bcs 1fe40 <MGC_HdrcDumpEndpoint+0x1b53c>
    4bf8:   5f43474d    svcpl   0x0043474d
    4bfc:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
    4c00:   75736552    ldrbvc  r6, [r3, #-1362]!
    4c04:   664f656d    strbvs  r6, [pc], -sp, ror #10
    4c08:   a7010066    strge   r0, [r1, -r6, rrx]
    4c0c:   3b780107    blcc    1e00424 <MGC_HdrcDumpEndpoint+0x1dfbb20>
            4c0e: R_ARM_ABS32   .text
    4c10:   3bdc0000    blcc    ff700008 <MGC_HdrcDumpEndpoint+0xff6fb704>
            4c12: R_ARM_ABS32   .text
    4c14:   02f40000    rscseq  r0, r4, #0  ; 0x0
            4c16: R_ARM_ABS32   .debug_loc
    4c18:   4c6e0000    stclmi  0, cr0, [lr]
    4c1c:   cb260000    blgt    980008 <MGC_HdrcDumpEndpoint+0x97b704>
            4c1f: R_ARM_ABS32   .debug_str
    4c20:   01000001    tsteq   r0, r1
    4c24:   00f107a6    rscseq  r0, r1, r6, lsr #15
    4c28:   91020000    tstls   r2, r0
    4c2c:   54772960    ldrbtpl r2, [r7], #-2400
    4c30:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
    4c34:   07a60100    streq   r0, [r6, r0, lsl #2]!
    4c38:   00000185    andeq   r0, r0, r5, lsl #3
    4c3c:   275c9102    ldrbcs  r9, [ip, -r2, lsl #2]
    4c40:   67655262    strbvs  r5, [r5, -r2, ror #4]!
    4c44:   07a80100    streq   r0, [r8, r0, lsl #2]!
    4c48:   00000176    andeq   r0, r0, r6, ror r1
    4c4c:   28679102    stmdacs r7!, {r1, r8, ip, pc}^
    4c50:   000000fb    strdeq  r0, [r0], -fp
            4c50: R_ARM_ABS32   .debug_str
    4c54:   0d07a901    stceq   9, cr10, [r7, #-4]
    4c58:   02000039    andeq   r0, r0, #57 ; 0x39
    4c5c:   b2286891    eorlt   r6, r8, #9502720    ; 0x910000
            4c5f: R_ARM_ABS32   .debug_str
    4c60:   01000000    tsteq   r0, r0
    4c64:   0f2807aa    svceq   0x002807aa
    4c68:   91020000    tstls   r2, r0
    4c6c:   012b006c    teqeq   fp, ip, rrx
    4c70:   5f43474d    svcpl   0x0043474d
    4c74:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
    4c78:   50746553    rsbspl  r6, r4, r3, asr r5
    4c7c:   5474726f    ldrbtpl r7, [r4], #-623
    4c80:   4d747365    ldclmi  3, cr7, [r4, #-404]!
    4c84:   0065646f    rsbeq   r6, r5, pc, ror #8
    4c88:   0107b201    tsteq   r7, r1, lsl #4
    4c8c:   00003bdc    ldrdeq  r3, [r0], -ip
            4c8c: R_ARM_ABS32   .text
    4c90:   00003f08    andeq   r3, r0, r8, lsl #30
            4c90: R_ARM_ABS32   .text
    4c94:   0000031e    andeq   r0, r0, lr, lsl r3
            4c94: R_ARM_ABS32   .debug_loc
    4c98:   00004cfb    strdeq  r4, [r0], -fp
    4c9c:   00004a26    andeq   r4, r0, r6, lsr #20
            4c9d: R_ARM_ABS32   .debug_str
    4ca0:   07b10100    ldreq   r0, [r1, r0, lsl #2]!
    4ca4:   00003f3a    andeq   r3, r0, sl, lsr pc
    4ca8:   295c9102    ldmdbcs ip, {r1, r8, ip, pc}^
    4cac:   646f4d65    strbtvs r4, [pc], #3429 ; 4cb4 <.debug_info+0x4cb4>
    4cb0:   b1010065    tstlt   r1, r5, rrx
    4cb4:   001c6507    andseq  r6, ip, r7, lsl #10
    4cb8:   58910200    ldmpl   r1, {r9}
    4cbc:   65526227    ldrbvs  r6, [r2, #-551]
    4cc0:   b4010067    strlt   r0, [r1], #-103
    4cc4:   00017607    andeq   r7, r1, r7, lsl #12
    4cc8:   63910200    orrsvs  r0, r1, #0  ; 0x0
    4ccc:   0000fb28    andeq   pc, r0, r8, lsr #22
            4ccd: R_ARM_ABS32   .debug_str
    4cd0:   07b60100    ldreq   r0, [r6, r0, lsl #2]!
    4cd4:   0000390d    andeq   r3, r0, sp, lsl #18
    4cd8:   28649102    stmdacs r4!, {r1, r8, ip, pc}^
    4cdc:   000000b2    strheq  r0, [r0], -r2
            4cdc: R_ARM_ABS32   .debug_str
    4ce0:   2807b701    stmdacs r7, {r0, r8, r9, sl, ip, sp, pc}
    4ce4:   0200000f    andeq   r0, r0, #15 ; 0xf
    4ce8:   77276891    undefined
    4cec:   00676552    rsbeq   r6, r7, r2, asr r5
    4cf0:   8507b801    strhi   fp, [r7, #-2049]
    4cf4:   02000001    andeq   r0, r0, #1  ; 0x1
    4cf8:   25006e91    strcs   r6, [r0, #-3729]
    4cfc:   43474d01    movtmi  r4, #32001  ; 0x7d01
    4d00:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4d04:   616f4c63    cmnvs   pc, r3, ror #24
    4d08:   66694664    strbtvs r4, [r9], -r4, ror #12
    4d0c:   fc01006f    stc2    0, cr0, [r1], {111}
    4d10:   01760107    cmneq   r6, r7, lsl #2
    4d14:   3f080000    svccc   0x00080000
            4d16: R_ARM_ABS32   .text
    4d18:   403c0000    eorsmi  r0, ip, r0
            4d1a: R_ARM_ABS32   .text
    4d1c:   03480000    movteq  r0, #32768  ; 0x8000
            4d1e: R_ARM_ABS32   .debug_loc
    4d20:   4dd80000    ldclmi  0, cr0, [r8]
    4d24:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            4d27: R_ARM_ABS32   .debug_str
    4d28:   01000000    tsteq   r0, r0
    4d2c:   3f3a07fb    svccc   0x003a07fb
    4d30:   91020000    tstls   r2, r0
    4d34:   01c62650    biceq   r2, r6, r0, asr r6
            4d36: R_ARM_ABS32   .debug_str
    4d38:   fb010000    blx 44d42 <MGC_HdrcDumpEndpoint+0x4043e>
    4d3c:   00017607    andeq   r7, r1, r7, lsl #12
    4d40:   4c910200    lfmmi   f0, 4, [r1], {0}
    4d44:   00000026    andeq   r0, r0, r6, lsr #32
            4d45: R_ARM_ABS32   .debug_str
    4d48:   07fb0100    ldrbeq  r0, [fp, r0, lsl #2]!
    4d4c:   000001a4    andeq   r0, r0, r4, lsr #3
    4d50:   29489102    stmdbcs r8, {r1, r8, ip, pc}^
    4d54:   756f5370    strbvc  r5, [pc, #-880]!    ; 49ec <.debug_info+0x49ec>
    4d58:   00656372    rsbeq   r6, r5, r2, ror r3
    4d5c:   6707fb01    strvs   pc, [r7, -r1, lsl #22]
    4d60:   02000006    andeq   r0, r0, #6  ; 0x6
    4d64:   64274491    strtvs  r4, [r7], #-1169
    4d68:   646e4977    strbtvs r4, [lr], #-2423
    4d6c:   01007865    tsteq   r0, r5, ror #16
    4d70:   01a407fd    strdeq  r0, [r4, sp]!
    4d74:   91020000    tstls   r2, r0
    4d78:   01522854    cmpeq   r2, r4, asr r8
            4d7a: R_ARM_ABS32   .debug_str
    4d7c:   fd010000    stc2    0, cr0, [r1]
    4d80:   0001a407    andeq   sl, r1, r7, lsl #8
    4d84:   58910200    ldmpl   r1, {r9}
    4d88:   44776427    ldrbtmi r6, [r7], #-1063
    4d8c:   6d757461    cfldrdvs    mvd7, [r5, #-388]!
    4d90:   07fe0100    ldrbeq  r0, [lr, r0, lsl #2]!
    4d94:   000001a4    andeq   r0, r0, r4, lsr #3
    4d98:   285c9102    ldmdacs ip, {r1, r8, ip, pc}^
    4d9c:   00000148    andeq   r0, r0, r8, asr #2
            4d9c: R_ARM_ABS32   .debug_str
    4da0:   a407ff01    strge   pc, [r7], #-3841
    4da4:   02000001    andeq   r0, r0, #1  ; 0x1
    4da8:   fb286091    blx a1824e <MGC_HdrcDumpEndpoint+0xa1394a>
            4dab: R_ARM_ABS32   .debug_str
    4dac:   01000000    tsteq   r0, r0
    4db0:   390d0800    stmdbcc sp, {fp}
    4db4:   91020000    tstls   r2, r0
    4db8:   00b22864    adcseq  r2, r2, r4, ror #16
            4dba: R_ARM_ABS32   .debug_str
    4dbc:   01010000    tsteq   r1, r0
    4dc0:   000f2808    andeq   r2, pc, r8, lsl #16
    4dc4:   68910200    ldmvs   r1, {r9}
    4dc8:   0000a628    andeq   sl, r0, r8, lsr #12
            4dc9: R_ARM_ABS32   .debug_str
    4dcc:   08020100    stmdaeq r2, {r8}
    4dd0:   00000176    andeq   r0, r0, r6, ror r1
    4dd4:   006f9102    rsbeq   r9, pc, r2, lsl #2
    4dd8:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    4ddc:   64485f43    strbvs  r5, [r8], #-3907
    4de0:   6e556372    mrcvs   3, 2, r6, cr5, cr2, {3}
    4de4:   64616f6c    strbtvs r6, [r1], #-3948
    4de8:   6f666946    svcvs   0x00666946
    4dec:   08130100    ldmdaeq r3, {r8}
    4df0:   00017601    andeq   r7, r1, r1, lsl #12
    4df4:   00403c00    subeq   r3, r0, r0, lsl #24
            4df5: R_ARM_ABS32   .text
    4df8:   00417400    subeq   r7, r1, r0, lsl #8
            4df9: R_ARM_ABS32   .text
    4dfc:   00037200    andeq   r7, r3, r0, lsl #4
            4dfd: R_ARM_ABS32   .debug_loc
    4e00:   004ea200    subeq   sl, lr, r0, lsl #4
    4e04:   004a2600    subeq   r2, sl, r0, lsl #12
            4e06: R_ARM_ABS32   .debug_str
    4e08:   12010000    andne   r0, r1, #0  ; 0x0
    4e0c:   003f3a08    eorseq  r3, pc, r8, lsl #20
    4e10:   50910200    addspl  r0, r1, r0, lsl #4
    4e14:   0001c626    andeq   ip, r1, r6, lsr #12
            4e15: R_ARM_ABS32   .debug_str
    4e18:   08120100    ldmdaeq r2, {r8}
    4e1c:   00000176    andeq   r0, r0, r6, ror r1
    4e20:   264c9102    strbcs  r9, [ip], -r2, lsl #2
    4e24:   00000000    andeq   r0, r0, r0
            4e24: R_ARM_ABS32   .debug_str
    4e28:   a4081201    strge   r1, [r8], #-513
    4e2c:   02000001    andeq   r0, r0, #1  ; 0x1
    4e30:   70294891    mlavc   r9, r1, r8, r4
    4e34:   74736544    ldrbtvc r6, [r3], #-1348
    4e38:   08120100    ldmdaeq r2, {r8}
    4e3c:   00000f28    andeq   r0, r0, r8, lsr #30
    4e40:   27449102    strbcs  r9, [r4, -r2, lsl #2]
    4e44:   6e497764    cdpvs   7, 4, cr7, cr9, cr4, {3}
    4e48:   00786564    rsbseq  r6, r8, r4, ror #10
    4e4c:   a4081401    strge   r1, [r8], #-1025
    4e50:   02000001    andeq   r0, r0, #1  ; 0x1
    4e54:   52285491    eorpl   r5, r8, #-1862270976    ; 0x91000000
            4e57: R_ARM_ABS32   .debug_str
    4e58:   01000001    tsteq   r0, r1
    4e5c:   01a40814    undefined instruction 0x01a40814
    4e60:   91020000    tstls   r2, r0
    4e64:   01482858    cmpeq   r8, r8, asr r8
            4e66: R_ARM_ABS32   .debug_str
    4e68:   15010000    strne   r0, [r1]
    4e6c:   0001a408    andeq   sl, r1, r8, lsl #8
    4e70:   5c910200    lfmpl   f0, 4, [r1], {0}
    4e74:   0000fb28    andeq   pc, r0, r8, lsr #22
            4e75: R_ARM_ABS32   .debug_str
    4e78:   08160100    ldmdaeq r6, {r8}
    4e7c:   0000390d    andeq   r3, r0, sp, lsl #18
    4e80:   28609102    stmdacs r0!, {r1, r8, ip, pc}^
    4e84:   000000b2    strheq  r0, [r0], -r2
            4e84: R_ARM_ABS32   .debug_str
    4e88:   28081701    stmdacs r8, {r0, r8, r9, sl, ip}
    4e8c:   0200000f    andeq   r0, r0, #15 ; 0xf
    4e90:   a6286491    undefined
            4e93: R_ARM_ABS32   .debug_str
    4e94:   01000000    tsteq   r0, r0
    4e98:   01760818    cmneq   r6, r8, lsl r8
    4e9c:   91020000    tstls   r2, r0
    4ea0:   0125006b    teqeq   r5, fp, rrx
    4ea4:   5f43474d    svcpl   0x0043474d
    4ea8:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
    4eac:   69706c55    ldmdbvs r0!, {r0, r2, r4, r6, sl, fp, sp, lr}^
    4eb0:   73756256    cmnvc   r5, #1610612741 ; 0x60000005
    4eb4:   746e6f43    strbtvc r6, [lr], #-3907
    4eb8:   006c6f72    rsbeq   r6, ip, r2, ror pc
    4ebc:   01082801    tsteq   r8, r1, lsl #16
    4ec0:   00000176    andeq   r0, r0, r6, ror r1
    4ec4:   00004174    andeq   r4, r0, r4, ror r1
            4ec4: R_ARM_ABS32   .text
    4ec8:   00004254    andeq   r4, r0, r4, asr r2
            4ec8: R_ARM_ABS32   .text
    4ecc:   0000039c    muleq   r0, ip, r3
            4ecc: R_ARM_ABS32   .debug_loc
    4ed0:   00004f41    andeq   r4, r0, r1, asr #30
    4ed4:   00004a26    andeq   r4, r0, r6, lsr #20
            4ed5: R_ARM_ABS32   .debug_str
    4ed8:   08270100    stmdaeq r7!, {r8}
    4edc:   00003f3a    andeq   r3, r0, sl, lsr pc
    4ee0:   29609102    stmdbcs r0!, {r1, r8, ip, pc}^
    4ee4:   74784562    ldrbtvc r4, [r8], #-1378
    4ee8:   72756f53    rsbsvc  r6, r5, #332    ; 0x14c
    4eec:   01006563    tsteq   r0, r3, ror #10
    4ef0:   01760827    cmneq   r6, r7, lsr #16
    4ef4:   91020000    tstls   r2, r0
    4ef8:   4562295c    strbmi  r2, [r2, #-2396]!
    4efc:   6e497478    mcrvs   4, 2, r7, cr9, cr8, {3}
    4f00:   61636964    cmnvs   r3, r4, ror #18
    4f04:   00726f74    rsbseq  r6, r2, r4, ror pc
    4f08:   76082701    strvc   r2, [r8], -r1, lsl #14
    4f0c:   02000001    andeq   r0, r0, #1  ; 0x1
    4f10:   62275891    eorvs   r5, r7, #9502720    ; 0x910000
    4f14:   006c6156    rsbeq   r6, ip, r6, asr r1
    4f18:   76082901    strvc   r2, [r8], -r1, lsl #18
    4f1c:   02000001    andeq   r0, r0, #1  ; 0x1
    4f20:   fb286791    blx a19e4e <MGC_HdrcDumpEndpoint+0xa1554a>
            4f23: R_ARM_ABS32   .debug_str
    4f24:   01000000    tsteq   r0, r0
    4f28:   390d082a    stmdbcc sp, {r1, r3, r5, fp}
    4f2c:   91020000    tstls   r2, r0
    4f30:   00b22868    adcseq  r2, r2, r8, ror #16
            4f32: R_ARM_ABS32   .debug_str
    4f34:   2b010000    blcs    44f3c <MGC_HdrcDumpEndpoint+0x40638>
    4f38:   000f2808    andeq   r2, pc, r8, lsl #16
    4f3c:   6c910200    lfmvs   f0, 4, [r1], {0}
    4f40:   4d012500    cfstr32mi   mvfx2, [r1]
    4f44:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    4f48:   52637264    rsbpl   r7, r3, #1073741830 ; 0x40000006
    4f4c:   55646165    strbpl  r6, [r4, #-357]!
    4f50:   5269706c    rsbpl   r7, r9, #108    ; 0x6c
    4f54:   01006765    tsteq   r0, r5, ror #14
    4f58:   7601083a    undefined
    4f5c:   54000001    strpl   r0, [r0], #-1
            4f5f: R_ARM_ABS32   .text
    4f60:   c8000042    stmdagt r0, {r1, r6}
            4f63: R_ARM_ABS32   .text
    4f64:   c6000043    strgt   r0, [r0], -r3, asr #32
            4f67: R_ARM_ABS32   .debug_loc
    4f68:   df000003    svcle   0x00000003
    4f6c:   2600004f    strcs   r0, [r0], -pc, asr #32
    4f70:   0000004a    andeq   r0, r0, sl, asr #32
            4f70: R_ARM_ABS32   .debug_str
    4f74:   3a083901    bcc 213380 <MGC_HdrcDumpEndpoint+0x20ea7c>
    4f78:   0200003f    andeq   r0, r0, #63 ; 0x3f
    4f7c:   62294c91    eorvs   r4, r9, #37120  ; 0x9100
    4f80:   72646441    rsbvc   r6, r4, #1090519040 ; 0x41000000
    4f84:   08390100    ldmdaeq r9!, {r8}
    4f88:   00000176    andeq   r0, r0, r6, ror r1
    4f8c:   29489102    stmdbcs r8, {r1, r8, ip, pc}^
    4f90:   61446270    cmpvs   r4, r0, ror r2
    4f94:   01006174    tsteq   r0, r4, ror r1
    4f98:   0f280839    svceq   0x00280839
    4f9c:   91020000    tstls   r2, r0
    4fa0:   43622744    cmnmi   r2, #17825792   ; 0x1100000
    4fa4:   01006c74    tsteq   r0, r4, ror ip
    4fa8:   0176083b    cmneq   r6, fp, lsr r8
    4fac:   91020000    tstls   r2, r0
    4fb0:   00fb2853    rscseq  r2, fp, r3, asr r8
            4fb2: R_ARM_ABS32   .debug_str
    4fb4:   3c010000    stccc   0, cr0, [r1], {0}
    4fb8:   00390d08    eorseq  r0, r9, r8, lsl #26
    4fbc:   54910200    ldrpl   r0, [r1], #512
    4fc0:   0000b228    andeq   fp, r0, r8, lsr #4
            4fc1: R_ARM_ABS32   .debug_str
    4fc4:   083d0100    ldmdaeq sp!, {r8}
    4fc8:   00000f28    andeq   r0, r0, r8, lsr #30
    4fcc:   28589102    ldmdacs r8, {r1, r8, ip, pc}^
    4fd0:   00000127    andeq   r0, r0, r7, lsr #2
            4fd0: R_ARM_ABS32   .debug_str
    4fd4:   b4083f01    strlt   r3, [r8], #-3841
    4fd8:   02000001    andeq   r0, r0, #1  ; 0x1
    4fdc:   25005c91    strcs   r5, [r0, #-3217]
    4fe0:   43474d01    movtmi  r4, #32001  ; 0x7d01
    4fe4:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
    4fe8:   69725763    ldmdbvs r2!, {r0, r1, r5, r6, r8, r9, sl, ip, lr}^
    4fec:   6c556574    cfldr64vs   mvdx6, [r5], {116}
    4ff0:   65526970    ldrbvs  r6, [r2, #-2416]
    4ff4:   5f010067    svcpl   0x00010067
    4ff8:   01760108    cmneq   r6, r8, lsl #2
    4ffc:   43c80000    bicmi   r0, r8, #0  ; 0x0
            4ffe: R_ARM_ABS32   .text
    5000:   453c0000    ldrmi   r0, [ip]!
            5002: R_ARM_ABS32   .text
    5004:   03f00000    mvnseq  r0, #0  ; 0x0
            5006: R_ARM_ABS32   .debug_loc
    5008:   507d0000    rsbspl  r0, sp, r0
    500c:   4a260000    bmi 980008 <MGC_HdrcDumpEndpoint+0x97b704>
            500f: R_ARM_ABS32   .debug_str
    5010:   01000000    tsteq   r0, r0
    5014:   3f3a085e    svccc   0x003a085e
    5018:   91020000    tstls   r2, r0
    501c:   4162294c    cmnmi   r2, ip, asr #18
    5020:   00726464    rsbseq  r6, r2, r4, ror #8
    5024:   76085e01    strvc   r5, [r8], -r1, lsl #28
    5028:   02000001    andeq   r0, r0, #1  ; 0x1
    502c:   62294891    eorvs   r4, r9, #9502720    ; 0x910000
    5030:   61746144    cmnvs   r4, r4, asr #2
    5034:   085e0100    ldmdaeq lr, {r8}^
    5038:   00000176    andeq   r0, r0, r6, ror r1
    503c:   27449102    strbcs  r9, [r4, -r2, lsl #2]
    5040:   6c744362    ldclvs  3, cr4, [r4], #-392
    5044:   08600100    stmdaeq r0!, {r8}^
    5048:   00000176    andeq   r0, r0, r6, ror r1
    504c:   28539102    ldmdacs r3, {r1, r8, ip, pc}^
    5050:   000000fb    strdeq  r0, [r0], -fp
            5050: R_ARM_ABS32   .debug_str
    5054:   0d086101    stfeqs  f6, [r8, #-4]
    5058:   02000039    andeq   r0, r0, #57 ; 0x39
    505c:   b2285491    eorlt   r5, r8, #-1862270976    ; 0x91000000
            505f: R_ARM_ABS32   .debug_str
    5060:   01000000    tsteq   r0, r0
    5064:   0f280862    svceq   0x00280862
    5068:   91020000    tstls   r2, r0
    506c:   01272858    teqeq   r7, r8, asr r8
            506e: R_ARM_ABS32   .debug_str
    5070:   64010000    strvs   r0, [r1]
    5074:   0001b408    andeq   fp, r1, r8, lsl #8
    5078:   5c910200    lfmpl   f0, 4, [r1], {0}
    507c:   4d012500    cfstr32mi   mvfx2, [r1]
    5080:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
    5084:   44637264    strbtmi r7, [r3], #-612
    5088:   6843616d    stmdavs r3, {r0, r2, r3, r5, r6, r8, sp, lr}^
    508c:   656e6e61    strbvs  r6, [lr, #-3681]!
    5090:   6174536c    cmnvs   r4, ip, ror #6
    5094:   43737574    cmnmi   r3, #486539264  ; 0x1d000000
    5098:   676e6168    strbvs  r6, [lr, -r8, ror #2]!
    509c:   01006465    tsteq   r0, r5, ror #8
    50a0:   76010884    strvc   r0, [r1], -r4, lsl #17
    50a4:   3c000001    stccc   0, cr0, [r0], {1}
            50a7: R_ARM_ABS32   .text
    50a8:   e4000045    str r0, [r0], #-69
            50ab: R_ARM_ABS32   .text
    50ac:   1a000048    bne 128 <.debug_info+0x128>
            50af: R_ARM_ABS32   .debug_loc
    50b0:   a8000004    stmdage r0, {r2}
    50b4:   26000051    undefined
    50b8:   000000ee    andeq   r0, r0, lr, ror #1
            50b8: R_ARM_ABS32   .debug_str
    50bc:   f1088301    undefined instruction 0xf1088301
    50c0:   03000000    movweq  r0, #0  ; 0x0
    50c4:   267fbc91    undefined
    50c8:   00000111    andeq   r0, r0, r1, lsl r1
            50c8: R_ARM_ABS32   .debug_str
    50cc:   76088301    strvc   r8, [r8], -r1, lsl #6
    50d0:   03000001    movweq  r0, #1  ; 0x1
    50d4:   297fb891    ldmdbcs pc!, {r0, r4, r7, fp, ip, sp, pc}^
    50d8:   61725462    cmnvs   r2, r2, ror #8
    50dc:   696d736e    stmdbvs sp!, {r1, r2, r3, r5, r6, r8, r9, ip, sp, lr}^
    50e0:   83010074    movwhi  r0, #4212   ; 0x1074
    50e4:   00017608    andeq   r7, r1, r8, lsl #12
    50e8:   b4910300    ldrlt   r0, [r1], #768
    50ec:   4971277f    ldmdbmi r1!, {r0, r1, r2, r3, r4, r5, r6, r8, r9, sl, sp}^
    50f0:   006d6574    rsbeq   r6, sp, r4, ror r5
    50f4:   90088601    andls   r8, r8, r1, lsl #12
    50f8:   02000022    andeq   r0, r0, #34 ; 0x22
    50fc:   62274091    eorvs   r4, r7, #145    ; 0x91
    5100:   75657551    strbvc  r7, [r5, #-1361]!
    5104:   87010065    strhi   r0, [r1, -r5, rrx]
    5108:   00017608    andeq   r7, r1, r8, lsl #12
    510c:   4b910200    blmi    fe445914 <MGC_HdrcDumpEndpoint+0xfe441010>
    5110:   0000da28    andeq   sp, r0, r8, lsr #20
            5111: R_ARM_ABS32   .debug_str
    5114:   08880100    stmeq   r8, {r8}
    5118:   000026e0    andeq   r2, r0, r0, ror #13
    511c:   274c9102    strbcs  r9, [ip, -r2, lsl #2]
    5120:   6c615677    stclvs  6, cr5, [r1], #-476
    5124:   08890100    stmeq   r9, {r8}
    5128:   00000185    andeq   r0, r0, r5, lsl #3
    512c:   28529102    ldmdacs r2, {r1, r8, ip, pc}^
    5130:   000001d2    ldrdeq  r0, [r0], -r2
            5130: R_ARM_ABS32   .debug_str
    5134:   85088901    strhi   r8, [r8, #-2305]
    5138:   02000001    andeq   r0, r0, #1  ; 0x1
    513c:   a5285491    strge   r5, [r8, #-1169]!
            513f: R_ARM_ABS32   .debug_str
    5140:   01000001    tsteq   r0, r1
    5144:   0185088a    orreq   r0, r5, sl, lsl #17
    5148:   91020000    tstls   r2, r0
    514c:   013a2856    teqeq   sl, r6, asr r8
            514e: R_ARM_ABS32   .debug_str
    5150:   8b010000    blhi    45158 <MGC_HdrcDumpEndpoint+0x40854>
    5154:   0001a408    andeq   sl, r1, r8, lsl #8
    5158:   58910200    ldmpl   r1, {r9}
    515c:   00004a28    andeq   r4, r0, r8, lsr #20
            515d: R_ARM_ABS32   .debug_str
    5160:   088c0100    stmeq   ip, {r8}
    5164:   00003f3a    andeq   r3, r0, sl, lsr pc
    5168:   285c9102    ldmdacs ip, {r1, r8, ip, pc}^
    516c:   000000fb    strdeq  r0, [r0], -fp
            516c: R_ARM_ABS32   .debug_str
    5170:   0d088d01    stceq   13, cr8, [r8, #-4]
    5174:   02000039    andeq   r0, r0, #57 ; 0x39
    5178:   87286091    undefined
            517b: R_ARM_ABS32   .debug_str
    517c:   01000001    tsteq   r0, r1
    5180:   339d088e    orrscc  r0, sp, #9306112    ; 0x8e0000
    5184:   91020000    tstls   r2, r0
    5188:   00b22864    adcseq  r2, r2, r4, ror #16
            518a: R_ARM_ABS32   .debug_str
    518c:   8f010000    svchi   0x00010000
    5190:   000f2808    andeq   r2, pc, r8, lsl #16
    5194:   68910200    ldmvs   r1, {r9}
    5198:   00014128    andeq   r4, r1, r8, lsr #2
            5199: R_ARM_ABS32   .debug_str
    519c:   08910100    ldmeq   r1, {r8}
    51a0:   00000176    andeq   r0, r0, r6, ror r1
    51a4:   006f9102    rsbeq   r9, pc, r2, lsl #2
    51a8:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    51ac:   64485f43    strbvs  r5, [r8], #-3907
    51b0:   75446372    strbvc  r6, [r4, #-882]
    51b4:   7453706d    ldrbvc  r7, [r3], #-109
    51b8:   00657461    rsbeq   r7, r5, r1, ror #8
    51bc:   0108f101    tstpeq  r8, r1, lsl #2
    51c0:   000000a9    andeq   r0, r0, r9, lsr #1
    51c4:   000048e4    andeq   r4, r0, r4, ror #17
            51c4: R_ARM_ABS32   .text
    51c8:   00004904    andeq   r4, r0, r4, lsl #18
            51c8: R_ARM_ABS32   .text
    51cc:   00000444    andeq   r0, r0, r4, asr #8
            51cc: R_ARM_ABS32   .debug_loc
    51d0:   000051e4    andeq   r5, r0, r4, ror #3
    51d4:   0000fb26    andeq   pc, r0, r6, lsr #22
            51d5: R_ARM_ABS32   .debug_str
    51d8:   08f00100    ldmeq   r0!, {r8}^
    51dc:   0000390d    andeq   r3, r0, sp, lsl #18
    51e0:   006c9102    rsbeq   r9, ip, r2, lsl #2
    51e4:   474d0125    strbmi  r0, [sp, -r5, lsr #2]
    51e8:   64485f43    strbvs  r5, [r8], #-3907
    51ec:   75446372    strbvc  r6, [r4, #-882]
    51f0:   6e45706d    cdpvs   0, 4, cr7, cr5, cr13, {3}
    51f4:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
    51f8:   0100746e    tsteq   r0, lr, ror #8
    51fc:   a90108fa    stmdbge r1, {r1, r3, r4, r5, r6, r7, fp}
    5200:   04000000    streq   r0, [r0]
            5203: R_ARM_ABS32   .text
    5204:   2c000049    stccs   0, cr0, [r0], {73}
            5207: R_ARM_ABS32   .text
    5208:   6e000049    cdpvs   0, 0, cr0, cr0, cr9, {2}
            520b: R_ARM_ABS32   .debug_loc
    520c:   32000004    andcc   r0, r0, #4  ; 0x4
    5210:   26000052    undefined
    5214:   000000fb    strdeq  r0, [r0], -fp
            5214: R_ARM_ABS32   .debug_str
    5218:   0d08f901    stceq   9, cr15, [r8, #-4]
    521c:   02000039    andeq   r0, r0, #57 ; 0x39
    5220:   da266c91    ble 99b24c <MGC_HdrcDumpEndpoint+0x996948>
            5223: R_ARM_ABS32   .debug_str
    5224:   01000000    tsteq   r0, r0
    5228:   339108f9    orrscc  r0, r1, #16318464   ; 0xf90000
    522c:   91020000    tstls   r2, r0
    5230:   761c0068    ldrvc   r0, [ip], -r8, rrx
    5234:   42000001    andmi   r0, r0, #1  ; 0x1
    5238:   1d000052    stcne   0, cr0, [r0, #-328]
    523c:   000000dd    ldrdeq  r0, [r0], -sp
    5240:   4d210034    stcmi   0, cr0, [r1, #-208]!
    5244:   615f4347    cmpvs   pc, r7, asr #6
    5248:   74736554    ldrbtvc r6, [r3], #-1364
    524c:   6b636150    blvs    18dd794 <MGC_HdrcDumpEndpoint+0x18d8e90>
    5250:   01007465    tsteq   r0, r5, ror #8
    5254:   00525f2f    subseq  r5, r2, pc, lsr #30
    5258:   00030500    andeq   r0, r3, r0, lsl #10
            525b: R_ARM_ABS32   .rodata
    525c:   08000000    stmdaeq r0, {}
    5260:   00005232    andeq   r5, r0, r2, lsr r2
    5264:   00010e1c    andeq   r0, r1, ip, lsl lr
    5268:   00527400    subseq  r7, r2, r0, lsl #8
    526c:   00dd1d00    sbcseq  r1, sp, r0, lsl #26
    5270:   00ff0000    rscseq  r0, pc, r0
    5274:   43474d2c    movtmi  r4, #32044  ; 0x7d2c
    5278:   6169445f    cmnvs   r9, pc, asr r4
    527c:   67734d67    ldrbvs  r4, [r3, -r7, ror #26]!
    5280:   64420a00    strbvs  r0, [r2], #-2560
    5284:   01000052    qaddeq  r0, r2, r0
    5288:   010e1c01    tsteq   lr, r1, lsl #24
    528c:   52990000    addspl  r0, r9, #0  ; 0x0
    5290:   dd1d0000    ldcle   0, cr0, [sp]
    5294:   20000000    andcs   r0, r0, r0
    5298:   474d2c00    strbmi  r2, [sp, -r0, lsl #24]
    529c:   69445f43    stmdbvs r4, {r0, r1, r6, r8, r9, sl, fp, ip, lr}^
    52a0:   754e6761    strbvc  r6, [lr, #-1889]
    52a4:   7265626d    rsbvc   r6, r5, #-805306362 ; 0xd0000006
    52a8:   89440a00    stmdbhi r4, {r9, fp}^
    52ac:   01000052    qaddeq  r0, r2, r0
    52b0:   474d2c01    strbmi  r2, [sp, -r1, lsl #24]
    52b4:   44625f43    strbtmi r5, [r2], #-3907
    52b8:   4c676169    stfmie  f6, [r7], #-420
    52bc:   6c657665    stclvs  6, cr7, [r5], #-404
    52c0:   76450a00    strbvc  r0, [r5], -r0, lsl #20
    52c4:   01000001    tsteq   r0, r1
    52c8:   41462d01    cmpmi   r6, r1, lsl #26
    52cc:   535f4950    cmppl   pc, #1310720    ; 0x140000
    52d0:   535f5359    cmppl   pc, #1677721601 ; 0x64000001
    52d4:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
    52d8:   00736563    rsbseq  r6, r3, r3, ror #10
    52dc:   b106130c    tstlt   r6, ip, lsl #6
    52e0:   0100003e    tsteq   r0, lr, lsr r0
    52e4:   Address 0x000052e4 is out of bounds.

Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:   00000579    andeq   r0, r0, r9, ror r5
   4:   01440002    cmpeq   r4, r2
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
  50:   3a630065    bcc 18c01ec <MGC_HdrcDumpEndpoint+0x18bb8e8>
  54:   4f52502f    svcmi   0x0052502f
  58:   7e415247    cdpvc   2, 4, cr5, cr1, cr7, {2}
  5c:   4e472f31    mcrmi   15, 2, r2, cr7, cr1, {1}
  60:   4d524155    ldfmie  f4, [r2, #-340]
  64:   6d72612f    ldfvse  f6, [r2, #-188]!
  68:   666c652d    strbtvs r6, [ip], -sp, lsr #10
  6c:   636e692f    cmnvs   lr, #770048 ; 0xbc000
  70:   6564756c    strbvs  r7, [r4, #-1388]!
  74:   2f2e2e00    svccs   0x002e2e00
  78:   692f2e2e    stmdbvs pc!, {r1, r2, r3, r5, r9, sl, fp, sp}
  7c:   756c636e    strbvc  r6, [ip, #-878]!
  80:   2e006564    cfsh32cs    mvfx6, mvfx0, #52
  84:   2e2e2f2e    cdpcs   15, 2, cr2, cr14, cr14, {1}
  88:   2f2e2e2f    svccs   0x002e2e2f
  8c:   2e2f2e2e    cdpcs   14, 2, cr2, cr15, cr14, {1}
  90:   6e692f2e    cdpvs   15, 6, cr2, cr9, cr14, {1}
  94:   6c617473    cfstrdvs    mvd7, [r1], #-460
  98:   424d2f6c    submi   r2, sp, #432    ; 0x1b0
  9c:   36483638    undefined
  a0:   6e692f30    mcrvs   15, 3, r2, cr9, cr0, {1}
  a4:   64756c63    ldrbtvs r6, [r5], #-3171
  a8:   61662f65    cmnvs   r6, r5, ror #30
  ac:   00006970    andeq   r6, r0, r0, ror r9
  b0:   685f756d    ldmdavs pc, {r0, r2, r3, r5, r6, r8, sl, ip, sp, lr}^
  b4:   632e7264    teqvs   lr, #1073741830 ; 0x40000006
  b8:   00000100    andeq   r0, r0, r0, lsl #2
  bc:   64647473    strbtvs r7, [r4], #-1139
  c0:   682e6665    stmdavs lr!, {r0, r2, r5, r6, r9, sl, sp, lr}
  c4:   00000200    andeq   r0, r0, r0, lsl #4
  c8:   69647473    stmdbvs r4!, {r0, r1, r4, r5, r6, sl, ip, sp, lr}^
  cc:   682e746e    stmdavs lr!, {r1, r2, r3, r5, r6, sl, ip, sp, lr}
  d0:   00000300    andeq   r0, r0, r0, lsl #6
  d4:   645f756d    ldrbvs  r7, [pc], #1389 ; dc <.debug_line+0xdc>
  d8:   682e6973    stmdavs lr!, {r0, r1, r4, r5, r6, r8, fp, sp, lr}
  dc:   00000400    andeq   r0, r0, r0, lsl #8
  e0:   645f756d    ldrbvs  r7, [pc], #1389 ; e8 <.debug_line+0xe8>
  e4:   682e7665    stmdavs lr!, {r0, r2, r5, r6, r9, sl, ip, sp, lr}
  e8:   00000400    andeq   r0, r0, r0, lsl #8
  ec:   635f756d    cmpvs   pc, #457179136  ; 0x1b400000
  f0:   682e6964    stmdavs lr!, {r2, r5, r6, r8, fp, sp, lr}
  f4:   00000400    andeq   r0, r0, r0, lsl #8
  f8:   6c5f756d    cfldr64vs   mvdx7, [pc], {109}
  fc:   2e747369    cdpcs   3, 7, cr7, cr4, cr9, {3}
 100:   00040068    andeq   r0, r4, r8, rrx
 104:   5f756d00    svcpl   0x00756d00
 108:   74657368    strbtvc r7, [r5], #-872
 10c:   0400682e    streq   r6, [r0], #-2094
 110:   756d0000    strbvc  r0, [sp]!
 114:   6963645f    stmdbvs r3!, {r0, r1, r2, r3, r4, r6, sl, sp, lr}^
 118:   0400682e    streq   r6, [r0], #-2094
 11c:   756d0000    strbvc  r0, [sp]!
 120:   706d695f    rsbvc   r6, sp, pc, asr r9
 124:   00682e6c    rsbeq   r2, r8, ip, ror #28
 128:   73000001    movwvc  r0, #1  ; 0x1
 12c:   645f7379    ldrbvs  r7, [pc], #889  ; 134 <.debug_line+0x134>
 130:   65766972    ldrbvs  r6, [r6, #-2418]!
 134:   00682e72    rsbeq   r2, r8, r2, ror lr
 138:   73000005    movwvc  r0, #5  ; 0x5
 13c:   735f7379    cmpvc   pc, #-469762047 ; 0xe4000001
 140:   69767265    ldmdbvs r6!, {r0, r2, r5, r6, r9, ip, sp, lr}^
 144:   2e736563    cdpcs   5, 7, cr6, cr3, cr3, {3}
 148:   00050068    andeq   r0, r5, r8, rrx
 14c:   05000000    streq   r0, [r0]
 150:   00000002    andeq   r0, r0, r2
            151: R_ARM_ABS32    .text
 154:   018d0300    orreq   r0, sp, r0, lsl #6
 158:   0367a401    cmneq   r7, #16777216   ; 0x1000000
 15c:   9f83660f    svcls   0x0083660f
 160:   68846884    stmvs   r4, {r2, r7, fp, sp, lr}
 164:   03846884    orreq   r6, r4, #8650752    ; 0x840000
 168:   08846632    stmeq   r4, {r1, r4, r5, r9, sl, sp, lr}
 16c:   03a06891    moveq   r6, #9502720    ; 0x910000
 170:   30039e09    andcc   r9, r3, r9, lsl #28
 174:   13037408    movwne  r7, #13320  ; 0x3408
 178:   6c4ba24a    sfmvs   f2, 3, [fp], {74}
 17c:   9f9fa168    svcls   0x009fa168
 180:   080f03a1    stmdaeq pc, {r0, r5, r7, r8, r9}
 184:   6c2fa020    stcvs   0, cr10, [pc], #-128
 188:   676967a0    strbvs  r6, [r9, -r0, lsr #15]!
 18c:   9f134202    svcls   0x00134202
 190:   1342029f    movtne  r0, #8863   ; 0x229f
 194:   44024ca3    strmi   r4, [r2], #-3235
 198:   2f67bd13    svccs   0x0067bd13
 19c:   8369a0f8    cmnhi   r9, #248    ; 0xf8
 1a0:   9f9f8583    svcls   0x009f8583
 1a4:   9f692fa0    svcls   0x00692fa0
 1a8:   bc844b68    vstmialt    r4, {d4-<overflow reg d55>}
 1ac:   67a28667    strvs   r8, [r2, r7, ror #12]!
 1b0:   daa09f69    ble fe827f5c <MGC_HdrcDumpEndpoint+0xfe823658>
 1b4:   d9d78368    ldmible r7, {r3, r5, r6, r8, r9, pc}^
 1b8:   d7d7d79f    bfcle   sp, #15, #9
 1bc:   67f583d7    ubfxvs  r8, r7, #7, #22
 1c0:   67306730    undefined
 1c4:   6b306730    blvs    c19e8c <MGC_HdrcDumpEndpoint+0xc15588>
 1c8:   9f6a2fd8    svcls   0x006a2fd8
 1cc:   4b9f6767    blmi    fe7d9f70 <MGC_HdrcDumpEndpoint+0xfe7d566c>
 1d0:   84a14b9f    strthi  r4, [r1], #2975
 1d4:   86846a86    strhi   r6, [r4], r6, lsl #21
 1d8:   8684846a    strhi   r8, [r4], sl, ror #8
 1dc:   846a6787    strbthi r6, [sl], #-1927
 1e0:   86846a86    strhi   r6, [r4], r6, lsl #21
 1e4:   6a86846a    bvs fe1a1394 <MGC_HdrcDumpEndpoint+0xfe19ca90>
 1e8:   134602a0    movtne  r0, #25248  ; 0x62a0
 1ec:   420284bc    andmi   r8, r2, #-1140850688    ; 0xbc000000
 1f0:   0284a013    addeq   sl, r4, #19 ; 0x13
 1f4:   2fa11342    svccs   0x00a11342
 1f8:   f6f20b03    undefined instruction 0xf6f20b03
 1fc:   6b674b4b    blvs    19d2f30 <MGC_HdrcDumpEndpoint+0x19ce62c>
 200:   08bd3e08    popeq   {r3, r9, sl, fp, ip, sp}
 204:   02a16791    adceq   r6, r1, #38010880   ; 0x2440000
 208:   08681430    stmdaeq r8!, {r4, r5, sl, ip}^
 20c:   68856923    stmvs   r5, {r0, r1, r5, r8, fp, sp, lr}
 210:   08699fa3    stmdaeq r9!, {r0, r1, r5, r7, r8, r9, sl, fp, ip, pc}^
 214:   4da083ad    stcmi   3, cr8, [r0, #692]!
 218:   f56987be    undefined instruction 0xf56987be
 21c:   030f2e02    movweq  r2, #65026  ; 0xfe02
 220:   4cbc2e0a    ldcmi   14, cr2, [ip], #40
 224:   85846af4    strhi   r6, [r4, #2804]
 228:   85a584bf    strhi   r8, [r5, #1215]!
 22c:   9fa48369    svcls   0x00a48369
 230:   83ad0885    undefined instruction 0x83ad0885
 234:   83be4da0    undefined instruction 0x83be4da0
 238:   84bb69db    ldrthi  r6, [fp], #2523
 23c:   844f846b    strbhi  r8, [pc], #1131 ; 244 <.debug_line+0x244>
 240:   84698484    strbthi r8, [r9], #-1156
 244:   8a2fa284    bhi be8c5c <MGC_HdrcDumpEndpoint+0xbe4358>
 248:   3c080903    stccc   9, cr0, [r8], {3}
 24c:   6767674b    strbvs  r6, [r7, -fp, asr #14]!
 250:   f49fa168    undefined instruction 0xf49fa168
 254:   67a1a267    strvs   sl, [r1, r7, ror #4]!
 258:   83696784    cmnhi   r9, #34603008   ; 0x2100000
 25c:   10033d08    andne   r3, r3, r8, lsl #26
 260:   0885bf9e    stmeq   r5, {r1, r2, r3, r4, r7, r8, r9, sl, fp, ip, sp, pc}
 264:   85e60891    strbhi  r0, [r6, #2193]!
 268:   029fa0f7    addseq  sl, pc, #247    ; 0xf7
 26c:   67671430    undefined
 270:   346283bd    strbtcc r8, [r2], #-957
 274:   02bc9308    adcseq  r9, ip, #536870912  ; 0x20000000
 278:   6768162a    strbvs  r1, [r8, -sl, lsr #12]!
 27c:   67660903    strbvs  r0, [r6, -r3, lsl #18]!
 280:   15220269    strne   r0, [r2, #-617]!
 284:   a09f8568    addsge  r8, pc, r8, ror #10
 288:   08f53e08    ldmeq   r5!, {r3, r9, sl, fp, ip, sp}^
 28c:   67676ac9    strbvs  r6, [r7, -r9, asr #21]!
 290:   78086b83    stmdavc r8, {r0, r1, r7, r8, r9, fp, sp, lr}
 294:   a1220867    teqge   r2, r7, ror #16
 298:   f45a0868    undefined instruction 0xf45a0868
 29c:   089fca08    ldmeq   pc, {r3, r9, fp, lr, pc}
 2a0:   339f6ac9    orrscc  r6, pc, #823296 ; 0xc9000
 2a4:   bd676769    stcllt  7, cr6, [r7, #-420]!
 2a8:   08346283    ldmdaeq r4!, {r0, r1, r7, r9, sp, lr}
 2ac:   2a02bc93    bcs af500 <MGC_HdrcDumpEndpoint+0xaabfc>
 2b0:   68676816    stmdavs r7!, {r1, r2, r4, fp, sp, lr}^
 2b4:   69152802    ldmdbvs r5, {r1, fp, sp}
 2b8:   08686784    stmdaeq r8!, {r2, r7, r8, r9, sl, sp, lr}^
 2bc:   846d83e9    strbthi r8, [sp], #-1001
 2c0:   2208f6be    andcs   pc, r8, #199229440  ; 0xbe00000
 2c4:   a06bc908    rsbge   ip, fp, r8, lsl #18
 2c8:   0867ca08    stmdaeq r7!, {r3, r9, fp, lr, pc}^
 2cc:   4b4bf944    blmi    12fe7e4 <MGC_HdrcDumpEndpoint+0x12f9ee0>
 2d0:   4b4b4b68    blmi    12d3078 <MGC_HdrcDumpEndpoint+0x12ce774>
 2d4:   6867684b    stmdavs r7!, {r0, r1, r3, r6, fp, sp, lr}^
 2d8:   0883d9a0    stmeq   r3, {r5, r7, r8, fp, ip, lr, pc}
 2dc:   d4f4a23d    ldrbtle sl, [r4], #573
 2e0:   db69d934    blle    1a767b8 <MGC_HdrcDumpEndpoint+0x1a71eb4>
 2e4:   3f084b86    svccc   0x00084b86
 2e8:   4b30674b    blmi    c1a01c <MGC_HdrcDumpEndpoint+0xc15718>
 2ec:   684b3067    stmdavs fp, {r0, r1, r2, r5, r6, ip, sp}^
 2f0:   83bd6767    undefined instruction 0x83bd6767
 2f4:   93083462    movwls  r3, #33890  ; 0x8462
 2f8:   162a02bc    undefined
 2fc:   02676768    rsbeq   r6, r7, #27262976   ; 0x1a00000
 300:   4b681528    blmi    1a057a8 <MGC_HdrcDumpEndpoint+0x1a00ea4>
 304:   83ea0868    mvnhi   r0, #6815744    ; 0x680000
 308:   21086a6a    tstcs   r8, sl, ror #20
 30c:   879f6968    ldrhi   r6, [pc, r8, ror #18]
 310:   676e844b    strbvs  r8, [lr, -fp, asr #8]!
 314:   2fa1f668    svccs   0x00a1f668
 318:   675a086b    ldrbvs  r0, [sl, -fp, ror #16]
 31c:   a1694b67    cmnge   r9, r7, ror #22
 320:   846b8484    strbthi r8, [fp], #-1156
 324:   4202684e    andmi   r6, r2, #5111808    ; 0x4e0000
 328:   6ad7f414    bvs ff5fd380 <MGC_HdrcDumpEndpoint+0xff5f8a7c>
 32c:   02f6d89f    rscseq  sp, r6, #10420224   ; 0x9f0000
 330:   68841342    stmvs   r4, {r1, r6, r8, r9, ip}
 334:   9fbe679f    svcls   0x00be679f
 338:   d7f4a267    ldrble  sl, [r4, r7, ror #4]!
 33c:   03d89f6a    bicseq  r9, r8, #424    ; 0x1a8
 340:   db2fd60a    blle    bf5b70 <MGC_HdrcDumpEndpoint+0xbf126c>
 344:   674b5a08    strbvs  r5, [fp, -r8, lsl #20]
 348:   6b848469    blvs    fe1214f4 <MGC_HdrcDumpEndpoint+0xfe11cbf0>
 34c:   69bc4e84    ldmibvs ip!, {r2, r7, r9, sl, fp, lr}
 350:   6785689f    undefined
 354:   87678785    strbhi  r8, [r7, -r5, lsl #15]!
 358:   68a26885    stmiavs r2!, {r0, r2, r7, fp, sp, lr}
 35c:   689fbf69    ldmvs   pc, {r0, r3, r5, r6, r8, r9, sl, fp, ip, sp, pc}
 360:   87a26785    strhi   r6, [r2, r5, lsl #15]!
 364:   03866785    orreq   r6, r6, #34865152   ; 0x2140000
 368:   01280209    teqeq   r8, r9, lsl #4
 36c:   a16968a3    cmnge   r9, r3, lsr #17
 370:   4bd86c2f    blmi    ff61b434 <MGC_HdrcDumpEndpoint+0xff616b30>
 374:   a0836867    addge   r6, r3, r7, ror #16
 378:   84688584    strbthi r8, [r8], #-1412
 37c:   bb4c9fa1    bllt    1328208 <MGC_HdrcDumpEndpoint+0x1323904>
 380:   bc6783bb    stcllt  3, cr8, [r7], #-748
 384:   a4836767    strge   r6, [r3], #1895
 388:   4a026a67    bmi 9ad2c <MGC_HdrcDumpEndpoint+0x96428>
 38c:   f82fa013    undefined instruction 0xf82fa013
 390:   4b4b4bc1    blmi    12d329c <MGC_HdrcDumpEndpoint+0x12ce998>
 394:   9f8384d7    svcls   0x008384d7
 398:   134c029f    movtne  r0, #49823  ; 0xc29f
 39c:   84144a02    ldrhi   r4, [r4], #-2562
 3a0:   4bc90884    blmi    ff2425b8 <MGC_HdrcDumpEndpoint+0xff23dcb4>
 3a4:   c9088468    stmdbgt r8, {r3, r5, r6, sl, pc}
 3a8:   0884684b    stmeq   r4, {r0, r1, r3, r6, fp, sp, lr}
 3ac:   694c4bc9    stmdbvs ip, {r0, r3, r6, r7, r8, r9, fp, lr}^
 3b0:   f39ff384    vsubw.u16   <illegal reg q7.5>, <illegal reg q15.5>, d4
 3b4:   9f9f83be    svcls   0x009f83be
 3b8:   a169a2f3    strdge  sl, [r9, #-35]!
 3bc:   bcf34b84    vldmialt    r3!, {d20-d21}
 3c0:   d9a38384    stmible r3!, {r2, r7, r8, r9, pc}
 3c4:   4b848567    blmi    fe121968 <MGC_HdrcDumpEndpoint+0xfe11d064>
 3c8:   4b67846a    blmi    19e1578 <MGC_HdrcDumpEndpoint+0x19dcc74>
 3cc:   304b3f08    subcc   r3, fp, r8, lsl #30
 3d0:   304b304b    subcc   r3, fp, fp, asr #32
 3d4:   a185a14b    orrge   sl, r5, fp, asr #2
 3d8:   08846869    stmeq   r4, {r0, r3, r5, r6, fp, sp, lr}
 3dc:   086c4d5a    stmdaeq ip!, {r1, r3, r4, r6, r8, sl, fp, lr}^
 3e0:   84f46823    ldrbthi r6, [r4], #2083
 3e4:   a5134c02    ldrge   r4, [r3, #-3074]
 3e8:   15220267    strne   r0, [r2, #-615]!
 3ec:   4b4b4bf5    blmi    12d33c8 <MGC_HdrcDumpEndpoint+0x12ceac4>
 3f0:   68676867    stmdavs r7!, {r0, r1, r2, r5, r6, fp, sp, lr}^
 3f4:   85f6a09f    ldrbhi  sl, [r6, #159]!
 3f8:   4cf44d84    ldclmi  13, cr4, [r4], #528
 3fc:   a09f6767    addsge  r6, pc, r7, ror #14
 400:   9f678469    svcls   0x00678469
 404:   68847608    stmvs   r4, {r3, r9, sl, ip, sp, lr}
 408:   0884696a    stmeq   r4, {r1, r3, r5, r6, r8, fp, sp, lr}
 40c:   bed7bf3f    mrclt   15, 6, fp, cr7, cr15, {1}
 410:   a06a4d4b    rsbge   r4, sl, fp, asr #26
 414:   03d76887    bicseq  r6, r7, #8847360    ; 0x870000
 418:   f567660a    undefined instruction 0xf567660a
 41c:   84760884    ldrbthi r0, [r6], #-2180
 420:   9f67848a    svcls   0x0067848a
 424:   a1678430    cmnge   r7, r0, lsr r4
 428:   3f088469    svccc   0x00088469
 42c:   9f84d7bf    svcls   0x0084d7bf
 430:   69d78469    ldmibvs r7, {r0, r3, r5, r6, sl, pc}^
 434:   08f4679f    ldmeq   r4!, {r0, r1, r2, r3, r4, r7, r8, r9, sl, sp, lr}^
 438:   f669677b    undefined instruction 0xf669677b
 43c:   674b4b4b    strbvs  r4, [fp, -fp, asr #22]
 440:   9f686768    svcls   0x00686768
 444:   8485f6a0    strhi   pc, [r5], #1696
 448:   84a1674c    strthi  r6, [r1], #1868
 44c:   d8a1674c    stmiale r1!, {r2, r3, r6, r8, r9, sl, sp, lr}
 450:   69a0674c    stmibvs r0!, {r2, r3, r6, r8, r9, sl, sp, lr}
 454:   75089f68    strvc   r9, [r8, #-3944]
 458:   0884db6a    stmeq   r4, {r1, r3, r5, r6, r8, r9, fp, ip, lr, pc}
 45c:   089fa33f    ldmeq   pc, {r0, r1, r2, r3, r4, r5, r8, r9, sp, pc}
 460:   d7846a92    undefined
 464:   a0679f6a    rsbge   r9, r7, sl, ror #30
 468:   74085f03    strvc   r5, [r8], #-3843
 46c:   4b2e2603    blmi    b89c80 <MGC_HdrcDumpEndpoint+0xb8537c>
 470:   84660903    strbthi r0, [r6], #-2307
 474:   84309f67    ldrthi  r9, [r0], #-3943
 478:   d9339f67    ldmdble r3!, {r0, r1, r2, r5, r6, r8, r9, sl, fp, ip, pc}
 47c:   31833d08    orrcc   r3, r3, r8, lsl #26
 480:   bebc31d9    mrclt   1, 5, r3, cr12, cr9, {6}
 484:   9f3f0884    svcls   0x003f0884
 488:   9f6a8469    svcls   0x006a8469
 48c:   08a09308    stmiaeq r0!, {r3, r8, r9, ip, pc}
 490:   6aa06722    bvs fe81a120 <MGC_HdrcDumpEndpoint+0xfe81581c>
 494:   0869d784    stmdaeq r9!, {r2, r7, r8, r9, sl, ip, lr, pc}^
 498:   f2500377    vcge.s16    q8, q0, <illegal reg q11.5>
 49c:   3b03c508    blcc    f18c4 <MGC_HdrcDumpEndpoint+0xecfc0>
 4a0:   6a67c02e    bvs 19f0560 <MGC_HdrcDumpEndpoint+0x19ebc5c>
 4a4:   9f684bd8    svcls   0x00684bd8
 4a8:   4bd84dbb    blmi    ff613b9c <MGC_HdrcDumpEndpoint+0xff60f298>
 4ac:   4ebb9f68    cdpmi   15, 11, cr9, cr11, cr8, {3}
 4b0:   a06767be    strhtge r6, [r7], #-126
 4b4:   d784cb08    strle   ip, [r4, r8, lsl #22]
 4b8:   83308330    teqhi   r0, #-1073741824    ; 0xc0000000
 4bc:   83308330    teqhi   r0, #-1073741824    ; 0xc0000000
 4c0:   308383a0    addcc   r8, r3, r0, lsr #7
 4c4:   9308d79f    movwls  sp, #34719  ; 0x879f
 4c8:   30d79f30    sbcscc  r9, r7, r0, lsr pc
 4cc:   9308d7bb    movwls  sp, #34747  ; 0x87bb
 4d0:   8383d731    orrhi   sp, r3, #12845056   ; 0xc40000
 4d4:   d7318383    ldrle   r8, [r1, -r3, lsl #7]!
 4d8:   d7318383    ldrle   r8, [r1, -r3, lsl #7]!
 4dc:   08a18683    stmiaeq r1!, {r0, r1, r7, r9, sl, pc}
 4e0:   67750823    ldrbvs  r0, [r5, -r3, lsr #16]!
 4e4:   9fa0a167    svcls   0x00a0a167
 4e8:   3041089b    umaalcc r0, r1, fp, r8
 4ec:   2f875608    svccs   0x00875608
 4f0:   08220869    stmdaeq r2!, {r0, r3, r5, r6, fp}
 4f4:   a1676775    smcge   30325
 4f8:   083a08a0    ldmdaeq sl!, {r5, r7, fp}
 4fc:   72083040    andvc   r3, r8, #64 ; 0x40
 500:   08692f87    stmdaeq r9!, {r0, r1, r2, r7, r8, r9, sl, fp, sp}^
 504:   f4696722    vld1.8  {d22}, [r9, :128], r2
 508:   ad089f69    stcge   15, cr9, [r8, #-420]
 50c:   f369679f    vaba.u32    d22, d25, d15
 510:   f46c674b    vld1.16 {d22}, [ip], fp
 514:   bc859f6a    stclt   15, cr9, [r5], {106}
 518:   132c0231    teqne   ip, #268435459  ; 0x10000003
 51c:   9e790369    cdpls   3, 7, cr0, cr9, cr9, {3}
 520:   bb820903    bllt    fe082934 <MGC_HdrcDumpEndpoint+0xfe07e030>
 524:   08696784    stmdaeq r9!, {r2, r7, r8, r9, sl, sp, lr}^
 528:   6c674b21    stclvs  11, cr4, [r7], #-132
 52c:   9f9f6af4    svcls   0x009f6af4
 530:   0231bc84    eorseq  fp, r1, #33792  ; 0x8400
 534:   0369132c    cmneq   r9, #-1342177280    ; 0xb0000000
 538:   09039e79    stmdbeq r3, {r0, r3, r4, r5, r6, r9, sl, fp, ip, pc}
 53c:   6a678482    bvs 19e174c <MGC_HdrcDumpEndpoint+0x19dce48>
 540:   4b4b2608    blmi    12c9d68 <MGC_HdrcDumpEndpoint+0x12c5464>
 544:   6867674b    stmdavs r7!, {r0, r1, r3, r6, r8, r9, sl, sp, lr}^
 548:   ae08f4a0    cfcvt32dge  mvd15, mvfx8
 54c:   6bd7686a    blvs    ff5da6fc <MGC_HdrcDumpEndpoint+0xff5d5df8>
 550:   9f25084b    svcls   0x0025084b
 554:   3f0884a1    svccc   0x000884a1
 558:   6984699f    stmibvs r4, {r0, r1, r2, r3, r4, r7, r8, fp, sp, lr}
 55c:   089308a0    ldmeq   r3, {r5, r7, fp}
 560:   69a06722    stmibvs r0!, {r1, r5, r8, r9, sl, sp, lr}
 564:   0868d784    stmdaeq r8!, {r2, r7, r8, r9, sl, ip, lr, pc}^
 568:   089ff5af    ldmeq   pc, {r0, r1, r2, r3, r5, r7, r8, sl, ip, sp, lr, pc}
 56c:   69f54bb1    ldmibvs r5!, {r0, r4, r5, r7, r8, r9, fp, lr}^
 570:   a06e6da0    rsbge   r6, lr, r0, lsr #27
 574:   2fbc502f    svccs   0x00bc502f
 578:   01000602    tsteq   r0, r2, lsl #12
 57c:   Address 0x0000057c is out of bounds.

Disassembly of section .rodata:

00000000 <MGC_aTestPacket>:
    ...
   8:   aaaaaa00    .word   0xaaaaaa00
   c:   aaaaaaaa    .word   0xaaaaaaaa
  10:   eeeeeeaa    .word   0xeeeeeeaa
  14:   eeeeeeee    .word   0xeeeeeeee
  18:   fffffeee    .word   0xfffffeee
  1c:   ffffffff    .word   0xffffffff
  20:   ffffffff    .word   0xffffffff
  24:   dfbf7fff    .word   0xdfbf7fff
  28:   fdfbf7ef    .word   0xfdfbf7ef
  2c:   dfbf7efc    .word   0xdfbf7efc
  30:   fdfbf7ef    .word   0xfdfbf7ef
  34:   0000007e    .word   0x0000007e
  38:   616e7944    .word   0x616e7944
  3c:   2063696d    .word   0x2063696d
  40:   4f464946    .word   0x4f464946
  44:   65642073    .word   0x65642073
  48:   74636574    .word   0x74636574
  4c:   62206465    .word   0x62206465
  50:   73207475    .word   0x73207475
  54:   6f707075    .word   0x6f707075
  58:   6e207472    .word   0x6e207472
  5c:   6220746f    .word   0x6220746f
  60:   746c6975    .word   0x746c6975
  64:   6c70203b    .word   0x6c70203b
  68:   65736165    .word   0x65736165
  6c:   62657220    .word   0x62657220
  70:   646c6975    .word   0x646c6975
  74:   00000000    .word   0x00000000
  78:   74746553    .word   0x74746553
  7c:   20676e69    .word   0x20676e69
  80:   72746e49    .word   0x72746e49
  84:   20457854    .word   0x20457854
  88:   00206f74    .word   0x00206f74
  8c:   74746553    .word   0x74746553
  90:   20676e69    .word   0x20676e69
  94:   72746e49    .word   0x72746e49
  98:   20457852    .word   0x20457852
  9c:   00206f74    .word   0x00206f74
  a0:   74746553    .word   0x74746553
  a4:   20676e69    .word   0x20676e69
  a8:   72746e49    .word   0x72746e49
  ac:   45627355    .word   0x45627355
  b0:   206f7420    .word   0x206f7420
  b4:   00000000    .word   0x00000000
  b8:   61647055    .word   0x61647055
  bc:   676e6974    .word   0x676e6974
  c0:   44414620    .word   0x44414620
  c4:   74205244    .word   0x74205244
  c8:   0000206f    .word   0x0000206f
  cc:   61647055    .word   0x61647055
  d0:   676e6974    .word   0x676e6974
  d4:   574f5020    .word   0x574f5020
  d8:   74205245    .word   0x74205245
  dc:   0000206f    .word   0x0000206f
  e0:   61647055    .word   0x61647055
  e4:   676e6974    .word   0x676e6974
  e8:   76654420    .word   0x76654420
  ec:   206c7443    .word   0x206c7443
  f0:   00206f74    .word   0x00206f74
  f4:   72617453    .word   0x72617453
  f8:   3a785274    .word   0x3a785274
  fc:   73657220    .word   0x73657220
 100:   61756469    .word   0x61756469
 104:   7962206c    .word   0x7962206c
 108:   63206574    .word   0x63206574
 10c:   746e756f    .word   0x746e756f
 110:   0000003d    .word   0x0000003d
 114:   20414d44    .word   0x20414d44
 118:   6f6c6e75    .word   0x6f6c6e75
 11c:   2d206461    .word   0x2d206461
 120:   61745320    .word   0x61745320
 124:   78527472    .word   0x78527472
 128:   0000005d    .word   0x0000005d
 12c:   20505249    .word   0x20505249
 130:   706d6f43    .word   0x706d6f43
 134:   6574656c    .word   0x6574656c
 138:   53202d20    .word   0x53202d20
 13c:   74726174    .word   0x74726174
 140:   005d7852    .word   0x005d7852
 144:   72617453    .word   0x72617453
 148:   5d785274    .word   0x5d785274
 14c:   00000000    .word   0x00000000
 150:   73756c46    .word   0x73756c46
 154:   676e6968    .word   0x676e6968
 158:   20785420    .word   0x20785420
 15c:   00206e6f    .word   0x00206e6f
 160:   73756c46    .word   0x73756c46
 164:   676e6968    .word   0x676e6968
 168:   20785220    .word   0x20785220
 16c:   00206e6f    .word   0x00206e6f
 170:   61666544    .word   0x61666544
 174:   45746c75    .word   0x45746c75
 178:   6552646e    .word   0x6552646e
 17c:   6e6f7073    .word   0x6e6f7073
 180:   203a6573    .word   0x203a6573
 184:   74746553    .word   0x74746553
 188:   20676e69    .word   0x20676e69
 18c:   30525343    .word   0x30525343
 190:   206f7420    .word   0x206f7420
 194:   00000000    .word   0x00000000
 198:   30525343    .word   0x30525343
 19c:   0000003d    .word   0x0000003d
 1a0:   6e756f43    .word   0x6e756f43
 1a4:   003d3074    .word   0x003d3074
 1a8:   20646e45    .word   0x20646e45
 1ac:   74732030    .word   0x74732030
 1b0:   0a6c6c61    .word   0x0a6c6c61
 1b4:   00000000    .word   0x00000000
 1b8:   20646e65    .word   0x20646e65
 1bc:   6e203a30    .word   0x6e203a30
 1c0:   6572206f    .word   0x6572206f
 1c4:   6e6f7073    .word   0x6e6f7073
 1c8:   28206573    .word   0x28206573
 1cc:   6f727265    .word   0x6f727265
 1d0:   000a2972    .word   0x000a2972
 1d4:   20646e65    .word   0x20646e65
 1d8:   6e203a30    .word   0x6e203a30
 1dc:   6572206f    .word   0x6572206f
 1e0:   6e6f7073    .word   0x6e6f7073
 1e4:   28206573    .word   0x28206573
 1e8:   204b414e    .word   0x204b414e
 1ec:   656d6974    .word   0x656d6974
 1f0:   2974756f    .word   0x2974756f
 1f4:   0000000a    .word   0x0000000a
 1f8:   76726553    .word   0x76726553
 1fc:   44656369    .word   0x44656369
 200:   75616665    .word   0x75616665
 204:   6e45746c    .word   0x6e45746c
 208:   53203a64    .word   0x53203a64
 20c:   69747465    .word   0x69747465
 210:   4320676e    .word   0x4320676e
 214:   20305253    .word   0x20305253
 218:   00206f74    .word   0x00206f74
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
  1c:   00000138    .word   0x00000138
  20:   440c0d44    .word   0x440c0d44
  24:   038d028e    .word   0x038d028e
  28:   0c44048b    .word   0x0c44048b
  2c:   0000040b    .word   0x0000040b
  30:   0000001c    .word   0x0000001c
  34:   00000000    .word   0x00000000
            34: R_ARM_ABS32 .debug_frame
  38:   00000138    .word   0x00000138
            38: R_ARM_ABS32 .text
  3c:   00000038    .word   0x00000038
  40:   440c0d44    .word   0x440c0d44
  44:   038d028e    .word   0x038d028e
  48:   0c44048b    .word   0x0c44048b
  4c:   0000040b    .word   0x0000040b
  50:   0000001c    .word   0x0000001c
  54:   00000000    .word   0x00000000
            54: R_ARM_ABS32 .debug_frame
  58:   00000170    .word   0x00000170
            58: R_ARM_ABS32 .text
  5c:   000000cc    .word   0x000000cc
  60:   440c0d44    .word   0x440c0d44
  64:   038d028e    .word   0x038d028e
  68:   0c44048b    .word   0x0c44048b
  6c:   0000040b    .word   0x0000040b
  70:   0000001c    .word   0x0000001c
  74:   00000000    .word   0x00000000
            74: R_ARM_ABS32 .debug_frame
  78:   0000023c    .word   0x0000023c
            78: R_ARM_ABS32 .text
  7c:   00000254    .word   0x00000254
  80:   440c0d44    .word   0x440c0d44
  84:   038d028e    .word   0x038d028e
  88:   0c44048b    .word   0x0c44048b
  8c:   0000040b    .word   0x0000040b
  90:   0000001c    .word   0x0000001c
  94:   00000000    .word   0x00000000
            94: R_ARM_ABS32 .debug_frame
  98:   00000490    .word   0x00000490
            98: R_ARM_ABS32 .text
  9c:   0000009c    .word   0x0000009c
  a0:   440c0d44    .word   0x440c0d44
  a4:   038d028e    .word   0x038d028e
  a8:   0c44048b    .word   0x0c44048b
  ac:   0000040b    .word   0x0000040b
  b0:   0000001c    .word   0x0000001c
  b4:   00000000    .word   0x00000000
            b4: R_ARM_ABS32 .debug_frame
  b8:   0000052c    .word   0x0000052c
            b8: R_ARM_ABS32 .text
  bc:   0000006c    .word   0x0000006c
  c0:   440c0d44    .word   0x440c0d44
  c4:   038d028e    .word   0x038d028e
  c8:   0c44048b    .word   0x0c44048b
  cc:   0000040b    .word   0x0000040b
  d0:   0000001c    .word   0x0000001c
  d4:   00000000    .word   0x00000000
            d4: R_ARM_ABS32 .debug_frame
  d8:   00000598    .word   0x00000598
            d8: R_ARM_ABS32 .text
  dc:   000001f0    .word   0x000001f0
  e0:   440c0d44    .word   0x440c0d44
  e4:   038d028e    .word   0x038d028e
  e8:   0c44048b    .word   0x0c44048b
  ec:   0000040b    .word   0x0000040b
  f0:   0000001c    .word   0x0000001c
  f4:   00000000    .word   0x00000000
            f4: R_ARM_ABS32 .debug_frame
  f8:   00000788    .word   0x00000788
            f8: R_ARM_ABS32 .text
  fc:   000003d8    .word   0x000003d8
 100:   440c0d44    .word   0x440c0d44
 104:   038d028e    .word   0x038d028e
 108:   0c44048b    .word   0x0c44048b
 10c:   0000040b    .word   0x0000040b
 110:   0000001c    .word   0x0000001c
 114:   00000000    .word   0x00000000
            114: R_ARM_ABS32    .debug_frame
 118:   00000b60    .word   0x00000b60
            118: R_ARM_ABS32    .text
 11c:   00000570    .word   0x00000570
 120:   440c0d44    .word   0x440c0d44
 124:   038d028e    .word   0x038d028e
 128:   0584048b    .word   0x0584048b
 12c:   040b0c44    .word   0x040b0c44
 130:   0000001c    .word   0x0000001c
 134:   00000000    .word   0x00000000
            134: R_ARM_ABS32    .debug_frame
 138:   000010d0    .word   0x000010d0
            138: R_ARM_ABS32    .text
 13c:   000009dc    .word   0x000009dc
 140:   440c0d44    .word   0x440c0d44
 144:   038d028e    .word   0x038d028e
 148:   0584048b    .word   0x0584048b
 14c:   040b0c44    .word   0x040b0c44
 150:   0000001c    .word   0x0000001c
 154:   00000000    .word   0x00000000
            154: R_ARM_ABS32    .debug_frame
 158:   00001aac    .word   0x00001aac
            158: R_ARM_ABS32    .text
 15c:   000004d0    .word   0x000004d0
 160:   440c0d44    .word   0x440c0d44
 164:   038d028e    .word   0x038d028e
 168:   0584048b    .word   0x0584048b
 16c:   040b0c44    .word   0x040b0c44
 170:   0000001c    .word   0x0000001c
 174:   00000000    .word   0x00000000
            174: R_ARM_ABS32    .debug_frame
 178:   00001f7c    .word   0x00001f7c
            178: R_ARM_ABS32    .text
 17c:   00000398    .word   0x00000398
 180:   440c0d44    .word   0x440c0d44
 184:   038d028e    .word   0x038d028e
 188:   0c44048b    .word   0x0c44048b
 18c:   0000040b    .word   0x0000040b
 190:   0000001c    .word   0x0000001c
 194:   00000000    .word   0x00000000
            194: R_ARM_ABS32    .debug_frame
 198:   00002314    .word   0x00002314
            198: R_ARM_ABS32    .text
 19c:   000002b8    .word   0x000002b8
 1a0:   440c0d44    .word   0x440c0d44
 1a4:   038d028e    .word   0x038d028e
 1a8:   0c44048b    .word   0x0c44048b
 1ac:   0000040b    .word   0x0000040b
 1b0:   0000001c    .word   0x0000001c
 1b4:   00000000    .word   0x00000000
            1b4: R_ARM_ABS32    .debug_frame
 1b8:   000025cc    .word   0x000025cc
            1b8: R_ARM_ABS32    .text
 1bc:   00000250    .word   0x00000250
 1c0:   440c0d44    .word   0x440c0d44
 1c4:   038d028e    .word   0x038d028e
 1c8:   0c44048b    .word   0x0c44048b
 1cc:   0000040b    .word   0x0000040b
 1d0:   0000001c    .word   0x0000001c
 1d4:   00000000    .word   0x00000000
            1d4: R_ARM_ABS32    .debug_frame
 1d8:   0000281c    .word   0x0000281c
            1d8: R_ARM_ABS32    .text
 1dc:   00000754    .word   0x00000754
 1e0:   440c0d44    .word   0x440c0d44
 1e4:   038d028e    .word   0x038d028e
 1e8:   0c44048b    .word   0x0c44048b
 1ec:   0000040b    .word   0x0000040b
 1f0:   0000001c    .word   0x0000001c
 1f4:   00000000    .word   0x00000000
            1f4: R_ARM_ABS32    .debug_frame
 1f8:   00002f70    .word   0x00002f70
            1f8: R_ARM_ABS32    .text
 1fc:   00000530    .word   0x00000530
 200:   440c0d44    .word   0x440c0d44
 204:   038d028e    .word   0x038d028e
 208:   0c44048b    .word   0x0c44048b
 20c:   0000040b    .word   0x0000040b
 210:   0000001c    .word   0x0000001c
 214:   00000000    .word   0x00000000
            214: R_ARM_ABS32    .debug_frame
 218:   000034a0    .word   0x000034a0
            218: R_ARM_ABS32    .text
 21c:   00000674    .word   0x00000674
 220:   440c0d44    .word   0x440c0d44
 224:   038d028e    .word   0x038d028e
 228:   0c44048b    .word   0x0c44048b
 22c:   0000040b    .word   0x0000040b
 230:   0000001c    .word   0x0000001c
 234:   00000000    .word   0x00000000
            234: R_ARM_ABS32    .debug_frame
 238:   00003b14    .word   0x00003b14
            238: R_ARM_ABS32    .text
 23c:   00000064    .word   0x00000064
 240:   440c0d44    .word   0x440c0d44
 244:   038d028e    .word   0x038d028e
 248:   0c44048b    .word   0x0c44048b
 24c:   0000040b    .word   0x0000040b
 250:   0000001c    .word   0x0000001c
 254:   00000000    .word   0x00000000
            254: R_ARM_ABS32    .debug_frame
 258:   00003b78    .word   0x00003b78
            258: R_ARM_ABS32    .text
 25c:   00000064    .word   0x00000064
 260:   440c0d44    .word   0x440c0d44
 264:   038d028e    .word   0x038d028e
 268:   0c44048b    .word   0x0c44048b
 26c:   0000040b    .word   0x0000040b
 270:   0000001c    .word   0x0000001c
 274:   00000000    .word   0x00000000
            274: R_ARM_ABS32    .debug_frame
 278:   00003bdc    .word   0x00003bdc
            278: R_ARM_ABS32    .text
 27c:   0000032c    .word   0x0000032c
 280:   440c0d44    .word   0x440c0d44
 284:   038d028e    .word   0x038d028e
 288:   0c44048b    .word   0x0c44048b
 28c:   0000040b    .word   0x0000040b
 290:   0000001c    .word   0x0000001c
 294:   00000000    .word   0x00000000
            294: R_ARM_ABS32    .debug_frame
 298:   00003f08    .word   0x00003f08
            298: R_ARM_ABS32    .text
 29c:   00000134    .word   0x00000134
 2a0:   440c0d44    .word   0x440c0d44
 2a4:   038d028e    .word   0x038d028e
 2a8:   0c44048b    .word   0x0c44048b
 2ac:   0000040b    .word   0x0000040b
 2b0:   0000001c    .word   0x0000001c
 2b4:   00000000    .word   0x00000000
            2b4: R_ARM_ABS32    .debug_frame
 2b8:   0000403c    .word   0x0000403c
            2b8: R_ARM_ABS32    .text
 2bc:   00000138    .word   0x00000138
 2c0:   440c0d44    .word   0x440c0d44
 2c4:   038d028e    .word   0x038d028e
 2c8:   0584048b    .word   0x0584048b
 2cc:   040b0c44    .word   0x040b0c44
 2d0:   0000001c    .word   0x0000001c
 2d4:   00000000    .word   0x00000000
            2d4: R_ARM_ABS32    .debug_frame
 2d8:   00004174    .word   0x00004174
            2d8: R_ARM_ABS32    .text
 2dc:   000000e0    .word   0x000000e0
 2e0:   440c0d44    .word   0x440c0d44
 2e4:   038d028e    .word   0x038d028e
 2e8:   0c44048b    .word   0x0c44048b
 2ec:   0000040b    .word   0x0000040b
 2f0:   00000020    .word   0x00000020
 2f4:   00000000    .word   0x00000000
            2f4: R_ARM_ABS32    .debug_frame
 2f8:   00004254    .word   0x00004254
            2f8: R_ARM_ABS32    .text
 2fc:   00000174    .word   0x00000174
 300:   440c0d44    .word   0x440c0d44
 304:   038d028e    .word   0x038d028e
 308:   0586048b    .word   0x0586048b
 30c:   07840685    .word   0x07840685
 310:   040b0c44    .word   0x040b0c44
 314:   00000020    .word   0x00000020
 318:   00000000    .word   0x00000000
            318: R_ARM_ABS32    .debug_frame
 31c:   000043c8    .word   0x000043c8
            31c: R_ARM_ABS32    .text
 320:   00000174    .word   0x00000174
 324:   440c0d44    .word   0x440c0d44
 328:   038d028e    .word   0x038d028e
 32c:   0586048b    .word   0x0586048b
 330:   07840685    .word   0x07840685
 334:   040b0c44    .word   0x040b0c44
 338:   0000001c    .word   0x0000001c
 33c:   00000000    .word   0x00000000
            33c: R_ARM_ABS32    .debug_frame
 340:   0000453c    .word   0x0000453c
            340: R_ARM_ABS32    .text
 344:   000003a8    .word   0x000003a8
 348:   440c0d44    .word   0x440c0d44
 34c:   038d028e    .word   0x038d028e
 350:   0c44048b    .word   0x0c44048b
 354:   0000040b    .word   0x0000040b
 358:   0000001c    .word   0x0000001c
 35c:   00000000    .word   0x00000000
            35c: R_ARM_ABS32    .debug_frame
 360:   000048e4    .word   0x000048e4
            360: R_ARM_ABS32    .text
 364:   00000020    .word   0x00000020
 368:   440c0d44    .word   0x440c0d44
 36c:   038d028e    .word   0x038d028e
 370:   0c44048b    .word   0x0c44048b
 374:   0000040b    .word   0x0000040b
 378:   0000001c    .word   0x0000001c
 37c:   00000000    .word   0x00000000
            37c: R_ARM_ABS32    .debug_frame
 380:   00004904    .word   0x00004904
            380: R_ARM_ABS32    .text
 384:   00000028    .word   0x00000028
 388:   440c0d44    .word   0x440c0d44
 38c:   038d028e    .word   0x038d028e
 390:   0c44048b    .word   0x0c44048b
 394:   0000040b    .word   0x0000040b
Disassembly of section .debug_loc:

00000000 <.debug_loc>:
   0:   00000000    .word   0x00000000
   4:   00000004    .word   0x00000004
   8:   045d0001    .word   0x045d0001
   c:   0c000000    .word   0x0c000000
  10:   01000000    .word   0x01000000
  14:   000c5c00    .word   0x000c5c00
  18:   01380000    .word   0x01380000
  1c:   00020000    .word   0x00020000
  20:   0000047b    .word   0x0000047b
  24:   00000000    .word   0x00000000
  28:   01380000    .word   0x01380000
  2c:   013c0000    .word   0x013c0000
  30:   00010000    .word   0x00010000
  34:   00013c5d    .word   0x00013c5d
  38:   00014400    .word   0x00014400
  3c:   5c000100    .word   0x5c000100
  40:   00000144    .word   0x00000144
  44:   00000170    .word   0x00000170
  48:   047b0002    .word   0x047b0002
    ...
  54:   00000170    .word   0x00000170
  58:   00000174    .word   0x00000174
  5c:   745d0001    .word   0x745d0001
  60:   7c000001    .word   0x7c000001
  64:   01000001    .word   0x01000001
  68:   017c5c00    .word   0x017c5c00
  6c:   023c0000    .word   0x023c0000
  70:   00020000    .word   0x00020000
  74:   0000047b    .word   0x0000047b
  78:   00000000    .word   0x00000000
  7c:   023c0000    .word   0x023c0000
  80:   02400000    .word   0x02400000
  84:   00010000    .word   0x00010000
  88:   0002405d    .word   0x0002405d
  8c:   00024800    .word   0x00024800
  90:   5c000100    .word   0x5c000100
  94:   00000248    .word   0x00000248
  98:   00000490    .word   0x00000490
  9c:   047b0002    .word   0x047b0002
    ...
  a8:   00000490    .word   0x00000490
  ac:   00000494    .word   0x00000494
  b0:   945d0001    .word   0x945d0001
  b4:   9c000004    .word   0x9c000004
  b8:   01000004    .word   0x01000004
  bc:   049c5c00    .word   0x049c5c00
  c0:   052c0000    .word   0x052c0000
  c4:   00020000    .word   0x00020000
  c8:   0000047b    .word   0x0000047b
  cc:   00000000    .word   0x00000000
  d0:   052c0000    .word   0x052c0000
  d4:   05300000    .word   0x05300000
  d8:   00010000    .word   0x00010000
  dc:   0005305d    .word   0x0005305d
  e0:   00053800    .word   0x00053800
  e4:   5c000100    .word   0x5c000100
  e8:   00000538    .word   0x00000538
  ec:   00000598    .word   0x00000598
  f0:   047b0002    .word   0x047b0002
    ...
  fc:   00000598    .word   0x00000598
 100:   0000059c    .word   0x0000059c
 104:   9c5d0001    .word   0x9c5d0001
 108:   a4000005    .word   0xa4000005
 10c:   01000005    .word   0x01000005
 110:   05a45c00    .word   0x05a45c00
 114:   07880000    .word   0x07880000
 118:   00020000    .word   0x00020000
 11c:   0000047b    .word   0x0000047b
 120:   00000000    .word   0x00000000
 124:   07880000    .word   0x07880000
 128:   078c0000    .word   0x078c0000
 12c:   00010000    .word   0x00010000
 130:   00078c5d    .word   0x00078c5d
 134:   00079400    .word   0x00079400
 138:   5c000100    .word   0x5c000100
 13c:   00000794    .word   0x00000794
 140:   00000b60    .word   0x00000b60
 144:   047b0002    .word   0x047b0002
    ...
 150:   00000b60    .word   0x00000b60
 154:   00000b64    .word   0x00000b64
 158:   645d0001    .word   0x645d0001
 15c:   6c00000b    .word   0x6c00000b
 160:   0100000b    .word   0x0100000b
 164:   0b6c5c00    .word   0x0b6c5c00
 168:   10d00000    .word   0x10d00000
 16c:   00020000    .word   0x00020000
 170:   0000047b    .word   0x0000047b
 174:   00000000    .word   0x00000000
 178:   10d00000    .word   0x10d00000
 17c:   10d40000    .word   0x10d40000
 180:   00010000    .word   0x00010000
 184:   0010d45d    .word   0x0010d45d
 188:   0010dc00    .word   0x0010dc00
 18c:   5c000100    .word   0x5c000100
 190:   000010dc    .word   0x000010dc
 194:   00001aac    .word   0x00001aac
 198:   047b0002    .word   0x047b0002
    ...
 1a4:   00001aac    .word   0x00001aac
 1a8:   00001ab0    .word   0x00001ab0
 1ac:   b05d0001    .word   0xb05d0001
 1b0:   b800001a    .word   0xb800001a
 1b4:   0100001a    .word   0x0100001a
 1b8:   1ab85c00    .word   0x1ab85c00
 1bc:   1f7c0000    .word   0x1f7c0000
 1c0:   00020000    .word   0x00020000
 1c4:   0000047b    .word   0x0000047b
 1c8:   00000000    .word   0x00000000
 1cc:   1f7c0000    .word   0x1f7c0000
 1d0:   1f800000    .word   0x1f800000
 1d4:   00010000    .word   0x00010000
 1d8:   001f805d    .word   0x001f805d
 1dc:   001f8800    .word   0x001f8800
 1e0:   5c000100    .word   0x5c000100
 1e4:   00001f88    .word   0x00001f88
 1e8:   00002314    .word   0x00002314
 1ec:   047b0002    .word   0x047b0002
    ...
 1f8:   00002314    .word   0x00002314
 1fc:   00002318    .word   0x00002318
 200:   185d0001    .word   0x185d0001
 204:   20000023    .word   0x20000023
 208:   01000023    .word   0x01000023
 20c:   23205c00    .word   0x23205c00
 210:   25cc0000    .word   0x25cc0000
 214:   00020000    .word   0x00020000
 218:   0000047b    .word   0x0000047b
 21c:   00000000    .word   0x00000000
 220:   25cc0000    .word   0x25cc0000
 224:   25d00000    .word   0x25d00000
 228:   00010000    .word   0x00010000
 22c:   0025d05d    .word   0x0025d05d
 230:   0025d800    .word   0x0025d800
 234:   5c000100    .word   0x5c000100
 238:   000025d8    .word   0x000025d8
 23c:   0000281c    .word   0x0000281c
 240:   047b0002    .word   0x047b0002
    ...
 24c:   0000281c    .word   0x0000281c
 250:   00002820    .word   0x00002820
 254:   205d0001    .word   0x205d0001
 258:   28000028    .word   0x28000028
 25c:   01000028    .word   0x01000028
 260:   28285c00    .word   0x28285c00
 264:   2f700000    .word   0x2f700000
 268:   00020000    .word   0x00020000
 26c:   0000047b    .word   0x0000047b
 270:   00000000    .word   0x00000000
 274:   2f700000    .word   0x2f700000
 278:   2f740000    .word   0x2f740000
 27c:   00010000    .word   0x00010000
 280:   002f745d    .word   0x002f745d
 284:   002f7c00    .word   0x002f7c00
 288:   5c000100    .word   0x5c000100
 28c:   00002f7c    .word   0x00002f7c
 290:   000034a0    .word   0x000034a0
 294:   047b0002    .word   0x047b0002
    ...
 2a0:   000034a0    .word   0x000034a0
 2a4:   000034a4    .word   0x000034a4
 2a8:   a45d0001    .word   0xa45d0001
 2ac:   ac000034    .word   0xac000034
 2b0:   01000034    .word   0x01000034
 2b4:   34ac5c00    .word   0x34ac5c00
 2b8:   3b140000    .word   0x3b140000
 2bc:   00020000    .word   0x00020000
 2c0:   0000047b    .word   0x0000047b
 2c4:   00000000    .word   0x00000000
 2c8:   3b140000    .word   0x3b140000
 2cc:   3b180000    .word   0x3b180000
 2d0:   00010000    .word   0x00010000
 2d4:   003b185d    .word   0x003b185d
 2d8:   003b2000    .word   0x003b2000
 2dc:   5c000100    .word   0x5c000100
 2e0:   00003b20    .word   0x00003b20
 2e4:   00003b78    .word   0x00003b78
 2e8:   047b0002    .word   0x047b0002
    ...
 2f4:   00003b78    .word   0x00003b78
 2f8:   00003b7c    .word   0x00003b7c
 2fc:   7c5d0001    .word   0x7c5d0001
 300:   8400003b    .word   0x8400003b
 304:   0100003b    .word   0x0100003b
 308:   3b845c00    .word   0x3b845c00
 30c:   3bdc0000    .word   0x3bdc0000
 310:   00020000    .word   0x00020000
 314:   0000047b    .word   0x0000047b
 318:   00000000    .word   0x00000000
 31c:   3bdc0000    .word   0x3bdc0000
 320:   3be00000    .word   0x3be00000
 324:   00010000    .word   0x00010000
 328:   003be05d    .word   0x003be05d
 32c:   003be800    .word   0x003be800
 330:   5c000100    .word   0x5c000100
 334:   00003be8    .word   0x00003be8
 338:   00003f08    .word   0x00003f08
 33c:   047b0002    .word   0x047b0002
    ...
 348:   00003f08    .word   0x00003f08
 34c:   00003f0c    .word   0x00003f0c
 350:   0c5d0001    .word   0x0c5d0001
 354:   1400003f    .word   0x1400003f
 358:   0100003f    .word   0x0100003f
 35c:   3f145c00    .word   0x3f145c00
 360:   403c0000    .word   0x403c0000
 364:   00020000    .word   0x00020000
 368:   0000047b    .word   0x0000047b
 36c:   00000000    .word   0x00000000
 370:   403c0000    .word   0x403c0000
 374:   40400000    .word   0x40400000
 378:   00010000    .word   0x00010000
 37c:   0040405d    .word   0x0040405d
 380:   00404800    .word   0x00404800
 384:   5c000100    .word   0x5c000100
 388:   00004048    .word   0x00004048
 38c:   00004174    .word   0x00004174
 390:   047b0002    .word   0x047b0002
    ...
 39c:   00004174    .word   0x00004174
 3a0:   00004178    .word   0x00004178
 3a4:   785d0001    .word   0x785d0001
 3a8:   80000041    .word   0x80000041
 3ac:   01000041    .word   0x01000041
 3b0:   41805c00    .word   0x41805c00
 3b4:   42540000    .word   0x42540000
 3b8:   00020000    .word   0x00020000
 3bc:   0000047b    .word   0x0000047b
 3c0:   00000000    .word   0x00000000
 3c4:   42540000    .word   0x42540000
 3c8:   42580000    .word   0x42580000
 3cc:   00010000    .word   0x00010000
 3d0:   0042585d    .word   0x0042585d
 3d4:   00426000    .word   0x00426000
 3d8:   5c000100    .word   0x5c000100
 3dc:   00004260    .word   0x00004260
 3e0:   000043c8    .word   0x000043c8
 3e4:   047b0002    .word   0x047b0002
    ...
 3f0:   000043c8    .word   0x000043c8
 3f4:   000043cc    .word   0x000043cc
 3f8:   cc5d0001    .word   0xcc5d0001
 3fc:   d4000043    .word   0xd4000043
 400:   01000043    .word   0x01000043
 404:   43d45c00    .word   0x43d45c00
 408:   453c0000    .word   0x453c0000
 40c:   00020000    .word   0x00020000
 410:   0000047b    .word   0x0000047b
 414:   00000000    .word   0x00000000
 418:   453c0000    .word   0x453c0000
 41c:   45400000    .word   0x45400000
 420:   00010000    .word   0x00010000
 424:   0045405d    .word   0x0045405d
 428:   00454800    .word   0x00454800
 42c:   5c000100    .word   0x5c000100
 430:   00004548    .word   0x00004548
 434:   000048e4    .word   0x000048e4
 438:   047b0002    .word   0x047b0002
    ...
 444:   000048e4    .word   0x000048e4
 448:   000048e8    .word   0x000048e8
 44c:   e85d0001    .word   0xe85d0001
 450:   f0000048    .word   0xf0000048
 454:   01000048    .word   0x01000048
 458:   48f05c00    .word   0x48f05c00
 45c:   49040000    .word   0x49040000
 460:   00020000    .word   0x00020000
 464:   0000047b    .word   0x0000047b
 468:   00000000    .word   0x00000000
 46c:   49040000    .word   0x49040000
 470:   49080000    .word   0x49080000
 474:   00010000    .word   0x00010000
 478:   0049085d    .word   0x0049085d
 47c:   00491000    .word   0x00491000
 480:   5c000100    .word   0x5c000100
 484:   00004910    .word   0x00004910
 488:   0000492c    .word   0x0000492c
 48c:   047b0002    .word   0x047b0002
    ...
Disassembly of section .debug_pubnames:

00000000 <.debug_pubnames>:
   0:   00000295    muleq   r0, r5, r2
   4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_info
   8:   52e60000    rscpl   r0, r6, #0  ; 0x0
   c:   3ecc0000    cdpcc   0, 12, cr0, cr12, cr0, {0}
  10:   474d0000    strbmi  r0, [sp, -r0]
  14:   64485f43    strbvs  r5, [r8], #-3907
  18:   6e496372    mcrvs   3, 2, r6, cr9, cr2, {3}
  1c:   40007469    andmi   r7, r0, r9, ror #8
  20:   4d00003f    stcmi   0, cr0, [r0, #-252]
  24:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
  28:   44637264    strbtmi r7, [r3], #-612
  2c:   6d616e79    stclvs  14, cr6, [r1, #-484]!
  30:   69466369    stmdbvs r6, {r0, r3, r5, r6, r8, r9, sp, lr}^
  34:   6f4c6f66    svcvs   0x004c6f66
  38:   69746163    ldmdbvs r4!, {r0, r1, r5, r6, r8, sp, lr}^
  3c:   ed006e6f    stc 14, cr6, [r0, #-444]
  40:   4d00003f    stcmi   0, cr0, [r0, #-252]
  44:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
  48:   49637264    stmdbmi r3!, {r2, r5, r6, r9, ip, sp, lr}^
  4c:   cc007273    sfmgt   f7, 4, [r0], {115}
  50:   4d000040    stcmi   0, cr0, [r0, #-256]
  54:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
  58:   53637264    cmnpl   r3, #1073741830 ; 0x40000006
  5c:   74726174    ldrbtvc r6, [r2], #-372
  60:   00413100    subeq   r3, r1, r0, lsl #2
  64:   43474d00    movtmi  r4, #32000  ; 0x7d00
  68:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
  6c:   6f745363    svcvs   0x00745363
  70:   41870070    orrmi   r0, r7, r0, ror r0
  74:   474d0000    strbmi  r0, [sp, -r0]
  78:   64485f43    strbvs  r5, [r8], #-3907
  7c:   65446372    strbvs  r6, [r4, #-882]
  80:   6f727473    svcvs   0x00727473
  84:   41d00079    bicsmi  r0, r0, r9, ror r0
  88:   474d0000    strbmi  r0, [sp, -r0]
  8c:   64485f43    strbvs  r5, [r8], #-3907
  90:   65526372    ldrbvs  r6, [r2, #-882]
  94:   75426461    strbvc  r6, [r2, #-1121]
  98:   61745373    cmnvs   r4, r3, ror r3
  9c:   60006574    andvs   r6, r0, r4, ror r5
  a0:   4d000042    stcmi   0, cr0, [r0, #-264]
  a4:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
  a8:   50637264    rsbpl   r7, r3, r4, ror #4
  ac:   72676f72    rsbvc   r6, r7, #456    ; 0x1c8
  b0:   75426d61    strbvc  r6, [r2, #-3425]
  b4:   61745373    cmnvs   r4, r3, ror r3
  b8:   17006574    smlsdxne    r0, r4, r5, r6
  bc:   4d000043    stcmi   0, cr0, [r0, #-268]
  c0:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
  c4:   42637264    rsbmi   r7, r3, #1073741830 ; 0x40000006
  c8:   45646e69    strbmi  r6, [r4, #-3689]!
  cc:   6f70646e    svcvs   0x0070646e
  d0:   00746e69    rsbseq  r6, r4, r9, ror #28
  d4:   00004417    andeq   r4, r0, r7, lsl r4
  d8:   5f43474d    svcpl   0x0043474d
  dc:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
  e0:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
  e4:   00785274    rsbseq  r5, r8, r4, ror r2
  e8:   0000456b    andeq   r4, r0, fp, ror #10
  ec:   5f43474d    svcpl   0x0043474d
  f0:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
  f4:   72617453    rsbvc   r7, r1, #1392508928 ; 0x53000000
  f8:   00785474    rsbseq  r5, r8, r4, ror r4
  fc:   000046ea    andeq   r4, r0, sl, ror #13
 100:   5f43474d    svcpl   0x0043474d
 104:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
 108:   73756c46    cmnvc   r5, #17920  ; 0x4600
 10c:   646e4568    strbtvs r4, [lr], #-1384
 110:   6e696f70    mcrvs   15, 3, r6, cr9, cr0, {3}
 114:   47a70074    undefined
 118:   474d0000    strbmi  r0, [sp, -r0]
 11c:   64485f43    strbvs  r5, [r8], #-3907
 120:   61486372    cmpvs   r8, r2, ror r3
 124:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
 128:   696f7064    stmdbvs pc!, {r2, r5, r6, ip, sp, lr}^
 12c:   5200746e    andpl   r7, r0, #1845493760 ; 0x6e000000
 130:   4d000048    stcmi   0, cr0, [r0, #-288]
 134:   485f4347    ldmdami pc, {r0, r1, r2, r6, r8, r9, lr}^
 138:   44637264    strbtmi r7, [r3], #-612
 13c:   75616665    strbvc  r6, [r1, #-1637]!
 140:   6e45746c    cdpvs   4, 4, cr7, cr5, cr12, {3}
 144:   73655264    cmnvc   r5, #1073741830 ; 0x40000006
 148:   736e6f70    cmnvc   lr, #448    ; 0x1c0
 14c:   48fb0065    ldmmi   fp!, {r0, r2, r5, r6}^
 150:   474d0000    strbmi  r0, [sp, -r0]
 154:   64485f43    strbvs  r5, [r8], #-3907
 158:   65536372    ldrbvs  r6, [r3, #-882]
 15c:   63697672    cmnvs   r9, #119537664  ; 0x7200000
 160:   66654465    strbtvs r4, [r5], -r5, ror #8
 164:   746c7561    strbtvc r7, [ip], #-1377
 168:   00646e45    rsbeq   r6, r4, r5, asr #28
 16c:   000049d5    ldrdeq  r4, [r0], -r5
 170:   5f43474d    svcpl   0x0043474d
 174:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
 178:   76726553    undefined
 17c:   54656369    strbtpl r6, [r5], #-873
 180:   736e6172    cmnvc   lr, #-2147483620    ; 0x8000001c
 184:   4174696d    cmnmi   r4, sp, ror #18
 188:   6c696176    stfvse  f6, [r9], #-472
 18c:   004aa400    subeq   sl, sl, r0, lsl #8
 190:   43474d00    movtmi  r4, #32000  ; 0x7d00
 194:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 198:   72655363    rsbvc   r5, r5, #-1946157055    ; 0x8c000001
 19c:   65636976    strbvs  r6, [r3, #-2422]!
 1a0:   65636552    strbvs  r6, [r3, #-1362]!
 1a4:   52657669    rsbpl   r7, r5, #110100480  ; 0x6900000
 1a8:   79646165    stmdbvc r4!, {r0, r2, r5, r6, r8, sp, lr}^
 1ac:   004c6e00    subeq   r6, ip, r0, lsl #28
 1b0:   43474d00    movtmi  r4, #32000  ; 0x7d00
 1b4:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 1b8:   74655363    strbtvc r5, [r5], #-867
 1bc:   74726f50    ldrbtvc r6, [r2], #-3920
 1c0:   74736554    ldrbtvc r6, [r3], #-1364
 1c4:   65646f4d    strbvs  r6, [r4, #-3917]!
 1c8:   004cfb00    subeq   pc, ip, r0, lsl #22
 1cc:   43474d00    movtmi  r4, #32000  ; 0x7d00
 1d0:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 1d4:   616f4c63    cmnvs   pc, r3, ror #24
 1d8:   66694664    strbtvs r4, [r9], -r4, ror #12
 1dc:   4dd8006f    ldclmi  0, cr0, [r8, #444]
 1e0:   474d0000    strbmi  r0, [sp, -r0]
 1e4:   64485f43    strbvs  r5, [r8], #-3907
 1e8:   6e556372    mrcvs   3, 2, r6, cr5, cr2, {3}
 1ec:   64616f6c    strbtvs r6, [r1], #-3948
 1f0:   6f666946    svcvs   0x00666946
 1f4:   004ea200    subeq   sl, lr, r0, lsl #4
 1f8:   43474d00    movtmi  r4, #32000  ; 0x7d00
 1fc:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 200:   706c5563    rsbvc   r5, ip, r3, ror #10
 204:   75625669    strbvc  r5, [r2, #-1641]!
 208:   6e6f4373    mcrvs   3, 3, r4, cr15, cr3, {3}
 20c:   6c6f7274    sfmvs   f7, 2, [pc], #-464
 210:   004f4100    subeq   r4, pc, r0, lsl #2
 214:   43474d00    movtmi  r4, #32000  ; 0x7d00
 218:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 21c:   61655263    cmnvs   r5, r3, ror #4
 220:   706c5564    rsbvc   r5, ip, r4, ror #10
 224:   67655269    strbvs  r5, [r5, -r9, ror #4]!
 228:   004fdf00    subeq   sp, pc, r0, lsl #30
 22c:   43474d00    movtmi  r4, #32000  ; 0x7d00
 230:   7264485f    rsbvc   r4, r4, #6225920    ; 0x5f0000
 234:   69725763    ldmdbvs r2!, {r0, r1, r5, r6, r8, r9, sl, ip, lr}^
 238:   6c556574    cfldr64vs   mvdx6, [r5], {116}
 23c:   65526970    ldrbvs  r6, [r2, #-2416]
 240:   507d0067    rsbspl  r0, sp, r7, rrx
 244:   474d0000    strbmi  r0, [sp, -r0]
 248:   64485f43    strbvs  r5, [r8], #-3907
 24c:   6d446372    stclvs  3, cr6, [r4, #-456]
 250:   61684361    cmnvs   r8, r1, ror #6
 254:   6c656e6e    stclvs  14, cr6, [r5], #-440
 258:   74617453    strbtvc r7, [r1], #-1107
 25c:   68437375    stmdavs r3, {r0, r2, r4, r5, r6, r8, r9, ip, sp, lr}^
 260:   65676e61    strbvs  r6, [r7, #-3681]!
 264:   51a80064    undefined instruction 0x51a80064
 268:   474d0000    strbmi  r0, [sp, -r0]
 26c:   64485f43    strbvs  r5, [r8], #-3907
 270:   75446372    strbvc  r6, [r4, #-882]
 274:   7453706d    ldrbvc  r7, [r3], #-109
 278:   00657461    rsbeq   r7, r5, r1, ror #8
 27c:   000051e4    andeq   r5, r0, r4, ror #3
 280:   5f43474d    svcpl   0x0043474d
 284:   63726448    cmnvs   r2, #1207959552 ; 0x48000000
 288:   706d7544    rsbvc   r7, sp, r4, asr #10
 28c:   70646e45    rsbvc   r6, r4, r5, asr #28
 290:   746e696f    strbtvc r6, [lr], #-2415
 294:   00000000    andeq   r0, r0, r0
    ...
Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
   0:   0000001c    andeq   r0, r0, ip, lsl r0
   4:   00000002    andeq   r0, r0, r2
            6: R_ARM_ABS32  .debug_info
   8:   00040000    andeq   r0, r4, r0
    ...
            10: R_ARM_ABS32 .text
  14:   0000492c    andeq   r4, r0, ip, lsr #18
    ...
Disassembly of section .debug_str:

00000000 <.debug_str>:
   0:   6f437764    svcvs   0x00437764
   4:   00746e75    rsbseq  r6, r4, r5, ror lr
   8:   63417764    movtvs  r7, #5988   ; 0x1764
   c:   6c617574    cfstr64vs   mvdx7, [r1], #-464
  10:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
  14:   62006874    andvs   r6, r0, #7602176    ; 0x740000
  18:   676e654c    strbvs  r6, [lr, -ip, asr #10]!
  1c:   70006874    andvc   r6, r0, r4, ror r8
  20:   706d6f43    rsbvc   r6, sp, r3, asr #30
  24:   6574656c    ldrbvs  r6, [r4, #-1388]!
  28:   61726150    cmnvs   r2, r0, asr r1
  2c:   4d77006d    ldclmi  0, cr0, [r7, #-436]!
  30:   61507861    cmpvs   r0, r1, ror #16
  34:   74656b63    strbtvc r6, [r5], #-2915
  38:   657a6953    ldrbvs  r6, [sl, #-2387]!
  3c:   416d6200    cmnmi   sp, r0, lsl #4
  40:   69727474    ldmdbvs r2!, {r2, r4, r5, r6, sl, ip, sp, lr}^
  44:   65747562    ldrbvs  r7, [r4, #-1378]!
  48:   50700073    rsbspl  r0, r0, r3, ror r0
  4c:   0074726f    rsbseq  r7, r4, pc, ror #4
  50:   74537764    ldrbvc  r7, [r3], #-1892
  54:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
  58:   54776400    ldrbtpl r6, [r7], #-1024
  5c:   6c61746f    cfstrdvs    mvd7, [r1], #-444
  60:   65747942    ldrbvs  r7, [r4, #-2370]!
  64:   66700073    undefined
  68:   43707249    cmnmi   r0, #-1879048188    ; 0x90000004
  6c:   6c706d6f    ldclvs  13, cr6, [r0], #-444
  70:   00657465    rsbeq   r7, r5, r5, ror #8
  74:   65744970    ldrbvs  r4, [r4, #-2416]!
  78:   4470006d    ldrbtmi r0, [r0], #-109
  7c:   6843616d    stmdavs r3, {r0, r2, r3, r5, r6, r8, sp, lr}^
  80:   656e6e61    strbvs  r6, [lr, #-3681]!
  84:   4962006c    stmdbmi r2!, {r2, r3, r5, r6}^
  88:   7265746e    rsbvc   r7, r5, #1845493760 ; 0x6e000000
  8c:   006c6176    rsbeq   r6, ip, r6, ror r1
  90:   72655677    rsbvc   r5, r5, #124780544  ; 0x7700000
  94:   6e6f6973    mcrvs   9, 3, r6, cr15, cr3, {3}
  98:   72546200    subsvc  r6, r4, #0  ; 0x0
  9c:   69666661    stmdbvs r6!, {r0, r5, r6, r9, sl, sp, lr}^
  a0:   70795463    rsbsvc  r5, r9, r3, ror #8
  a4:   46620065    strbtmi r0, [r2], -r5, rrx
  a8:   4f6f6669    svcmi   0x006f6669
  ac:   65736666    ldrbvs  r6, [r3, #-1638]!
  b0:   42700074    rsbsmi  r0, r0, #116    ; 0x74
  b4:   00657361    rsbeq   r7, r5, r1, ror #6
  b8:   66754270    undefined
  bc:   00726566    rsbseq  r6, r2, r6, ror #10
  c0:   6e754662    cdpvs   6, 7, cr4, cr5, cr2, {3}
  c4:   64644163    strbtvs r4, [r4], #-355
  c8:   44620072    strbtmi r0, [r2], #-114
  cc:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
  d0:   6f747069    svcvs   0x00747069
  d4:   70795472    rsbsvc  r5, r9, r2, ror r4
  d8:   45700065    ldrbmi  r0, [r0, #-101]!
  dc:   7000646e    andvc   r6, r0, lr, ror #8
  e0:   43616d44    cmnmi   r1, #4352   ; 0x1100
  e4:   72746e6f    rsbsvc  r6, r4, #1776   ; 0x6f0
  e8:   656c6c6f    strbvs  r6, [ip, #-3183]!
  ec:   50700072    rsbspl  r0, r0, r2, ror r0
  f0:   61766972    cmnvs   r6, r2, ror r9
  f4:   61446574    cmpvs   r4, r4, ror r5
  f8:   70006174    andvc   r6, r0, r4, ror r1
  fc:   746e6f43    strbtvc r6, [lr], #-3907
 100:   6c6c6f72    stclvs  15, cr6, [ip], #-456
 104:   77007265    strvc   r7, [r0, -r5, ror #4]
 108:   49646e45    stmdbmi r4!, {r0, r2, r6, r9, sl, fp, sp, lr}^
 10c:   7865646e    stmdavc r5!, {r1, r2, r3, r5, r6, sl, sp, lr}^
 110:   6f4c6200    svcvs   0x004c6200
 114:   456c6163    strbmi  r6, [ip, #-355]!
 118:   6200646e    andvs   r6, r0, #1845493760 ; 0x6e000000
 11c:   41737542    cmnmi   r3, r2, asr #10
 120:   65726464    ldrbvs  r6, [r2, #-1124]!
 124:   73007373    movwvc  r7, #883    ; 0x373
 128:   74726174    ldrbtvc r6, [r2], #-372
 12c:   6d69745f    cfstrdvs    mvd7, [r9, #-380]!
 130:   52620065    rsbpl   r0, r2, #101    ; 0x65
 134:   6c757365    ldclvs  3, cr7, [r5], #-404
 138:   74730074    ldrbtvc r0, [r3], #-116
 13c:   73757461    cmnvc   r5, #1627389952 ; 0x61000000
 140:   6e496200    cdpvs   2, 4, cr6, cr9, cr0, {0}
 144:   00786564    rsbseq  r6, r8, r4, ror #10
 148:   6f437764    svcvs   0x00437764
 14c:   33746e75    cmncc   r4, #1872   ; 0x750
 150:   77640032    undefined
 154:   65646e49    strbvs  r6, [r4, #-3657]!
 158:   00323378    eorseq  r3, r2, r8, ror r3
 15c:   54776461    ldrbtpl r6, [r7], #-1121
 160:   72656d69    rsbvc   r6, r5, #6720   ; 0x1a40
 164:   6f736552    svcvs   0x00736552
 168:   6974756c    ldmdbvs r4!, {r2, r3, r5, r6, r8, sl, ip, sp, lr}^
 16c:   00736e6f    rsbseq  r6, r3, pc, ror #28
 170:   72734962    rsbsvc  r4, r3, #1605632    ; 0x188000
 174:   6c6c6143    stfvse  f6, [ip], #-268
 178:   6b636162    blvs    18d8708 <MGC_HdrcDumpEndpoint+0x18d3e04>
 17c:   6c416200    sfmvs   f6, 2, [r1], {0}
 180:   44776f6c    ldrbtmi r6, [r7], #-3948
 184:   7000616d    andvc   r6, r0, sp, ror #2
 188:   76726553    undefined
 18c:   73656369    cmnvc   r5, #-1543503871    ; 0xa4000001
 190:   43706100    cmnmi   r0, #0  ; 0x0
 194:   69666e6f    stmdbvs r6!, {r0, r1, r2, r3, r5, r6, r9, sl, fp, sp, lr}^
 198:   73654467    cmnvc   r5, #1728053248 ; 0x67000000
 19c:   70697263    rsbvc   r7, r9, r3, ror #4
 1a0:   73726f74    cmnvc   r2, #464    ; 0x1d0
 1a4:   78527700    ldmdavc r2, {r8, r9, sl, ip, sp, lr}^
 1a8:   6e756f43    cdpvs   15, 7, cr6, cr5, cr3, {2}
 1ac:   44770074    ldrbtmi r0, [r7], #-116
 1b0:   72637365    rsbvc   r7, r3, #-1811939327    ; 0x94000001
 1b4:   6f747069    svcvs   0x00747069
 1b8:   66754272    undefined
 1bc:   4c726566    cfldr64mi   mvdx6, [r2], #-408
 1c0:   74676e65    strbtvc r6, [r7], #-3685
 1c4:   45620068    strbmi  r0, [r2, #-104]!
 1c8:   7000646e    andvc   r6, r0, lr, ror #8
 1cc:   61726150    cmnvs   r2, r0, asr r1
 1d0:   4977006d    ldmdbmi r7!, {r0, r2, r3, r5, r6}^
 1d4:   5272746e    rsbspl  r7, r2, #1845493760 ; 0x6e000000
 1d8:   70004578    andvc   r4, r0, r8, ror r5
 1dc:   69766544    ldmdbvs r6!, {r2, r6, r8, sl, sp, lr}^
 1e0:   Address 0x000001e0 is out of bounds.

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


