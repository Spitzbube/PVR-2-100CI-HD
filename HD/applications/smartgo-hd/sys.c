

#include <unistd.h>

#if 0
#include <string.h>
#include <sys_init_customer.h>
#endif
#include <fapi/sys_init.h>
#include <fapi/board_ifc.h>
#if 0
#include <fapi/drv_boot.h>
#include <fapi/drv_flash.h>
#include <flashfs/flashfs.h>
#endif
#include "osdhandler/osdhandler.h"
#if 0
#include "rtos.h"
#endif
#include "rtos/rtos_kernel.h"
#include "app_main.h"
#if 0
#include "reg_bm.h"
#include "reg_gpio.h"
#include "reg_i2c.h"
#include "reg_irq_ctrl.h"
#include "reg_sflash.h"
#include "reg_uart.h"
#include "reg_upi.h"
#include "reg_vic.h"
#include "vfd.h"
#include "event_queue.h"
#include "input.h"
#include "fapi_adapter.h"
#include "fapi_audec.h"
#include "fapi_bm.h"
#include "drv_mmu.h"
#include "drv_mmu_heap.h"
//#include "fapi_flash.h"
#include "drv_gpreg.h"
#include "drv_hdmi.h"
#include "drv_i2c.h"
#include "drv_intr.h"
#include "drv_timer.h"
#include "drv_gpio.h"
#include "fapi_dma.h"
#include "fapi_ata.h"
#include "fapi_usb.h"
#include "fapi_sync.h"
#include "fapi_tsd.h"
#endif
#include "device.h"
#if 0
#include "drv_uart.h"
#include "dev_usb.h"
#endif
#include "sys.h"
#include "sys_osd.h"
#include "flash.h"
#if 0
//#include "app_main.h"
#include "stringop.h"






extern void func_21c0eee8(int a);

extern void func_21c0ef00(int a, int b, int c); 

extern void func_21c42e8c(int a, int b, int c, int d);
extern void func_21c42f20(int a, int b);

extern void func_21c6d860(void);
extern void func_21c6ead8(void);
extern void func_21c6f5b0(void);
extern void func_21c703c0(void);
extern void func_21c703e4(void);
extern void func_21c721a4(void);
extern void func_21c72844(void);
extern void func_21c72adc(void);
extern void func_21c72b04(void);
extern void func_21c72cfc(void);
extern void func_21c756a8(void);
extern void func_21c76544(void);
extern void func_21c76b00(void);

extern void CPU_Polling(void);
extern void func_21cb5250(void (*a)(void));
 
extern int Data_21efb16c;
extern int Data_21efb55c; 
extern int Data_21efae40;
extern int Data_21efc180;
extern int Data_21efc1a4;
extern int Data_21efc1c8;
extern int Data_21efa698;
extern int Data_21efa6bc;
extern int Data_21efb304;
extern int Data_21efb328;
extern int Data_21efc550;
extern int Data_21efbf28;
extern int Data_21efbf4c;
extern int Data_21efc834;
extern int Data_21efc858;
extern int Data_21efc87c;
extern int Data_21efe44c;
extern int Data_21efa424;
extern int Data_21efa3c0;


typedef void (*Func_21ed5db0)(void);

#if 0
typedef void (*FAPI_SYS_RegInitFunc)(void);
FAPI_SYS_RegInitFunc FAPI_SYS_RegInitArr[] = //21ed5d5c
{
   func_21c6d860, //FREG_AO_Init
   FREG_BM_Init,
   func_21c6ead8, //FREG_CLKPWR_Init
   func_21c6f5b0, //FREG_DMA_Init
   func_21c703c0, //FREG_ETH_Init
   func_21c703e4, //FREG_FPC_Init
   FREG_GPIO_Init,
   FREG_I2C_Init,
   func_21c721a4, //FREG_ICC_Init
   func_21c72844, //FREG_IR_Init
   FREG_IRQ_CTRL_Init,
   FREG_SFLASH_Init,
   func_21c72b04, //FREG_SSP_Init
   func_21c72cfc, //FREG_TSD_Init
   FREG_UART_Init,
   FREG_UPI_Init,
   func_21c756a8, //FREG_VE_Init
   func_21c76544, //FREG_VE_HD_Init
   FREG_VIC_Init,
   func_21c76b00, //FREG_VO_Init
   0, 
};
struct fapi_driver* FAPI_SYS_DriverArr[] = //21ed5db0
{
   &fapi_mmu_driver, //MMU
   &FAPI_GPREG_Driver,
   &fapi_intr_driver, //INTR 
   &fapi_gpio_driver, //GPIO
   &fapi_uart0_driver, //UART0
   &fapi_uart1_driver, //UART1
   &fapi_timer0_driver, //TIMER0
   &fapi_timer1_driver, //TIMER1
   &fapi_timer2_driver, //TIMER2
   &FAPI_UPI_Driver, //UPI
   &Data_21efa698, //CLKPWR
   &fapi_dma_driver, //DMA
   &fapi_usb_driver, //USB0
   &FAPI_I2C_Driver0, //I2C0
   &FAPI_I2C_Driver1, //I2C1
   &FAPI_I2C_Driver2, //I2C2
   &Data_21efb304, //ICC0
   &Data_21efb328, //ICC1
   &fapi_bm_driver0, //BM0
   &fapi_bm_driver1,
   &fapi_bm_driver2,
   &fapi_bm_driver3,
   &fapi_sync_driver, //SYNC
   &fapi_tsd_driver0, //TSD0
   &fapi_tsd_driver1,
   &fapi_tsd_driver2,
   &fapi_tsd_driver3,
   &Data_21efc550, //TTX
   &fapi_flash_driver, //FLASH
   &fapi_boot_driver, //BOOT
   &Data_21efbf28, //SSP0
   &Data_21efbf4c,
   &Data_21efc834, //VIDEC0
   &Data_21efc858, //VIDEC0?
   &Data_21efc87c, //VISCALE
   &Data_21efe44c, //VIENC
   &FAPI_HDMI_Driver,
   &fapi_ir_driver, //IR
   &Data_21efa424, //AUOUT
   &fapi_audec_driver, //AUDEC
   &fapi_ata_driver, //ATA
   0,
};
#endif

#endif


SYS_DATA_S* sysData = 0; //21f02a10
int Data_21f02a14; //21f02a14
int Data_21f02a1c[256]; //21f02a1c 

int Data_21f7ba30; //21f7ba30

struct appData sysConfig; //21f7be58

#if 0

void func_21b407ac(int* a)
{
   int r1, lr, r2;
   for (r1 = 0; r1 != 256; r1++)
   {
      r2 = r1 << 24;

      for (lr = 8; lr != 0; lr--)
      {
         r2 = (r2 & 0x80000000)? 
            0x4c11db7 ^ (r2 << 1):
            (r2 << 1);
      }

      a[r1] = r2;
   }
}

#endif

int func_21b865e4(void)
{
//   printf("func_21b865e4");
   return 0;
}


int func_21b8cee4(void)
{
//   printf("func_21b8cee4");
   return 0;
}


int func_21b8d494(void)
{
//   printf("func_21b8d494");
   return 0;
}


/* 21b8e50c - complete */
struct appData* SYS_GetConfig(void)
{
   return &sysConfig;
}


/* 21b8e53c - complete */
int SYS_GetPowerOn(void)
{
//   printf("SYS_GetPowerOn\n");

   return sysData->powerOn;
}

#if 0

/* 21b8e550 - complete */
int func_21b8e550(void)
{
   return sysData->Data_32;
}


/* 21b8e564 - complete */
int func_21b8e564(unsigned a)
{
   int res = 0;
   
   if (a < 2)
   {
      sysData->Data_32 = a;
   }
   else
   {
      res = -10000001;
   }
   
   return res;
}


#endif

/* 21b8e770 - complete */
uint64_t SYS_TimeDiff(uint64_t a, uint64_t b)
{
   if (b >= a)
   {
      return b - a;
   }
   else
   {
      return (b + ~a) + 1;
   }
}


/* 21b8e7bc - complete */
void* SYS_MemoryAllocate(int a)
{
   if (FAPI_SYS_Services.mallocCachedFunc == 0)
   {
      return 0;
   }
   
   return (FAPI_SYS_Services.mallocCachedFunc)(a);
}


/* 21b8e7e0 - complete */
void SYS_MemoryFree(void* a)
{
   FAPI_SYS_FREE(a);
}

/* 21b8e8ec - complete */
void SYS_PrintString(char* a)
{
   if (a != 0)
   {
      FAPI_SYS_PRINT_MSG(a);
      FAPI_SYS_PRINT_MSG("\n");
   }
}


/* 21b8eaf0 - todo */
int SYS_SendRequestMessage(Struct_35e444* a, void* msg, int* c)
{
   int sp4 = msg;
   int sp;

   rtos_request_semaphore(a->lock, -1);

   a->Data_24 = 1;

   if (a->Func_0 != 0)
   {
      (a->Func_0)();
   }

   rtos_mailbox_send(a->Data_16, &sp4, 0);

   if (0 == rtos_mailbox_send(a->request, &sp4, 0))
   {
      rtos_release_semaphore(a->lock, 0);

      return 0xff676596;
   }

   // Wait for the Response
   rtos_mailbox_receive(a->response, &sp, -1);

   if (c != 0)
   {
      *c = sp;
   }

   rtos_release_semaphore(a->lock, 0);

   return 0;
}


/* 21b8eabc - complete */
void* SYS_ReceiveRequestMessage(Struct_35e444* a)
{
   int sp;

   rtos_mailbox_receive(a->request, &sp, -1);

   if (sp != 0)
   {
      a->Data_24 = 0;
   }
   return (void*) sp;
}

#if 0

/* 21b8e9e8 - todo */
uint64_t func_21b8e9e8(void)
{
   return FAPI_TIMER_GetTimeStamp(1000);
}


/* 21b8e9f0 - todo */
int func_21b8e9f0(Struct_35e444* a, unsigned b)
{
   int r7;
   int r16;
   int r13;
   uint64_t sl_fp = FAPI_TIMER_GetTimeStamp(1000);
   int r2 = b;

   if (b == 0) return 1;

   do
   {
      //21b8ea1c
      r7 = r2;

      if (a->Data_24 == 1) return 0;

      int r5 = r2 + a->Data_20;

      if (r2 % a->Data_20)
      {
         //r2 += a->Data_20;
         //r2 -= (r2 % a->Data_20);
         r7 = r5 - (r5 % a->Data_20);
      }

//      r2 = r7;

      rtos_mailbox_receive(a->Data_16, &r16, r7); //r2);

      if (a->Data_24 != 0) return 0;

      if (r16 == 0)
      {
         //->35e7d4
         break;
      }

      r13 = SYS_TimeDiff(sl_fp, FAPI_TIMER_GetTimeStamp(1000));
      r2 = b - r13;
   }
   while (r2 > 0);
   //35e7d4
   return 1;
}


#endif

/* 21b8e9c0 - complete */
int SYS_SendResponseMessage(Struct_35e444* a, void* b)
{
   if (0 == rtos_mailbox_send(a->response, b, 0))
   {
      return 0xff676596;
   }

   return 0;
}


/* 21b8ecf8 - todo */
int ITC_Init(Struct_35e444* a, int* b)
{
   memset(a, 0, sizeof(Struct_35e444));

   a->request = rtos_mailbox_create(4);
   if (a->request == 0)
   {
      return 0xff67658e;
   }

   a->response = rtos_mailbox_create(4);
   if (a->response == 0)
   {
      return 0xff67658e;
   }

   a->Data_16 = rtos_mailbox_create(4);
   if (a->Data_16 == 0)
   {
      return 0xff67658e;
   }

   a->lock = rtos_create_semaphore(1);
   if (a->lock == 0)
   {
      return 0xff67658e;
   }

   a->Data_20 = 1;
   a->Data_24 = 0;

   return 0;
}


/* 21b8eec0 - todo */
void SYS_PrintStartupHdr(void)
{
   FAPI_SYS_HandleT flHdl;
   const char_t* flName;
   FAPI_FLASH_OpenParamsT sp;
   struct 
   {
      int fill_0[2]; //0
      int Data_8; //8
      int fill_12[2]; //12
      //20
   } sp24;
   int32_t sp44;
   
   FAPI_SYS_PRINT_MSG("\n");
   FAPI_SYS_PRINT_MSG("############################################################\n");
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("#      Fujitsu MB86H60 - SmartGo-HD V%i.%-4i                #\n", 1, 0);
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("# (c) Fujitsu Microelectronics Europe 2002 - 2009          #\n");
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("# -------------------------------------------------------- #\n");
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("# Build              : %-15s %-19s #\n", 
      "Dec  4 2009", "11:02:54");
   FAPI_SYS_PRINT_MSG("# Flash              : ");
      
   sp.version = FAPI_FLASH_VERSION;
   sp.deviceType = FAPI_FLASH_DEVICE_SFLASH;
   sp.firstSector = 0;
   sp.sectorCount = 0;
   sp.fsysFlag = 0;
   sp.forceFlush = 0;
      
   if (((flHdl = FAPI_FLASH_Open(&sp, &sp44)) == 0) ||
         ((flName = FAPI_FLASH_GetDeviceName(flHdl)) == 0))
   {
      //->21b8f398
      FAPI_SYS_PRINT_MSG("%-35s # flHdl=%x %s\n",
               "<unknown>", flHdl, flName);
      //->21b8f024
   }
   else
   {
      //21b8f00c
      FAPI_SYS_PRINT_MSG("%-35s #\n", flName);
      //21b8f024
   }
   //21b8f024
   sp44 = FAPI_FLASH_Close(flHdl);
  
   FAPI_SYS_PRINT_MSG("# RTOS               : %-35s #\n", rtos_get_version());
   FAPI_SYS_PRINT_MSG("# Target Platform    : %-35s #\n", "gmi_h60_stb");
   FAPI_SYS_PRINT_MSG("# Satellite enabled  : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# Terr. enabled      : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "NO");
   FAPI_SYS_PRINT_MSG("# Cable enabled      : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "NO");
   FAPI_SYS_PRINT_MSG("# Direct TS enabled  : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "NO");
   FAPI_SYS_PRINT_MSG("# TTX enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# MHEG-5 enabled     : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "NO");
   FAPI_SYS_PRINT_MSG("# PVR enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# HDD enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# USB enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# PIP enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "NO");
   FAPI_SYS_PRINT_MSG("# DVB Subtt. enabled : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# CI enabled         : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# Parental c. enabled: ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# JPEG enabled       : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# MP3 enabled        : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# Debug mode         : ");
   FAPI_SYS_PRINT_MSG("%-35s #\n", "YES");
   FAPI_SYS_PRINT_MSG("# Booter information : ");
   //21b8f33c
   if (0 == SYS_GetBootInfo(&sp24))
   {
      FAPI_SYS_PRINT_MSG("boot slot %-2d               #\n", sp24.Data_8);
   }
   else
   {
      //21b8f3bc
      FAPI_SYS_PRINT_MSG("%-35s #\n", "Not available");
   }
   //21b8f364
   FAPI_SYS_PRINT_MSG("#                                                          #\n");
   FAPI_SYS_PRINT_MSG("############################################################\n");
   //21b8f390
}


/* 21b8f4bc - todo */
int SYS_ThreadCreate(void (*func)(int), 
                     int b, int prio, unsigned stacksize, 
                     const char* thread_name, void** thread)
{
   int res;

   void* r0 = rtos_thread_create(0, stacksize/4, prio, func, b, 0, thread_name);
   
   if (r0 == 0)
   {
      return -10001010;
   }
   
   if (thread != 0)
   {
      *thread = r0;
   }

   return 0;
}


/* 21b8fe1c - todo */
int SYS_GoUp(void)
{
//   printf("SYS_GoUp");

   av_send_start_message(); //->av_int.c
   av_send_null_message();
   
#if 0
   SYS_DeviceWakeup();
#endif

   //TODO

   return 0;
}


/* 21b90360 - todo */
int32_t SYS_Init(struct appData* a)
{
   FAPI_FLASH_OpenParamsT flashParams;
   int32_t res;
   FAPI_SYS_HandleT flashHandle;

   if (sysData != 0)
   {
      return -10000003;
   }
   
   sysData = SYS_MemoryAllocate(sizeof(SYS_DATA_S));
   
   if (sysData == 0)
   {
      return -10000002;
   }
   
   memset(sysData, 0, sizeof(SYS_DATA_S));
   
   sysData->powerOn = -1;
   sysData->arData_40[0].inUse = 0;
   sysData->arData_40[1].inUse = 0;
   sysData->arData_40[2].inUse = 0;
   sysData->Data_32 = 0;
   
   sysData->mutexId = rtos_create_semaphore(1);
   
   if (sysData->mutexId == 0)
   {
      return -10001010;
   }
   
   SYS_GetFlashDeviceType(&sysData->flashDeviceType);
   
   sysData->bufSize = 0x20000;
   sysData->pBuf = SYS_MemoryAllocate(0x20004); //131076);
   
   if (sysData->pBuf == 0)
   {
      return -10000002;
   }
   
   sysData->pBuf = (void*) (((unsigned)(sysData->pBuf) + 3) & ~3);
   sysData->flashDeviceType = 1;
   
   flashParams.version = FAPI_FLASH_VERSION;
   flashParams.deviceType = FAPI_FLASH_DEVICE_SFLASH;
   flashParams.firstSector = 0;
   flashParams.sectorCount = 0;
   flashParams.fsysFlag = 0;
   flashParams.forceFlush = 0;
   
   flashHandle = FAPI_FLASH_Open(&flashParams, &res);
   
   if (flashHandle == 0)
   {
      return -10000001;
   }
   
   res = FAPI_FLASH_GetSectorStart(flashHandle, 0);
   
   if (res < 0)
   {
      return res;
   }
   
   sysData->flashStartAddr = res;
   
   sysData->flashDeviceSize = FAPI_FLASH_GetDeviceSize(flashHandle);
   
   if (sysData->flashDeviceSize == 0)
   {
      return -10000001;
   }
   
   sysData->flashSectorCount = FAPI_FLASH_GetSectorCount(flashHandle);

   if (sysData->flashSectorCount == -10001)
   {
      return -10000001;
   }

   sysData->flashUsrStartSector = 0;

   while ((int32_t)sysData->flashUsrStartSector < sysData->flashSectorCount)
   {
      res = FAPI_FLASH_GetSectorStart(flashHandle, sysData->flashUsrStartSector);

      if (res < 0)
      {
         return res;
      }

      if ((uint32_t)res >= 0x240000)
      {
         break;
      }

      sysData->flashUsrStartSector++;
   }
   
   if (sysData->flashUsrStartSector >= sysData->flashSectorCount)
   {
      return -10001002;
   }

   res = FAPI_FLASH_Close(flashHandle);
   
   res = STR_Init(a);
   
   if (res != 0)
   {
      return res;
   }
   
   return device_init();
}


/* 21b90574 - nearly complete */
void StartupThread(int a)
{
   int32_t res;
   
   SYS_PrintStartupHdr();
   
#if 0
   res = VFD_Init();
   if (res != 0)
   {
      FAPI_SYS_PRINT_MSG("VFD_Init error!\n");
   }
   
   func_21ba500c();
   func_21ba4f80(0);
   func_21ba4eb4();   
   func_21ba5b48("booting", 0, 0);

   if ((char)(sysConfig.bData_0-1) < 2)
#endif
   {
#if 0
      //21b90664
      func_21ba5b48("flash...", 0, 0);
#endif
      
      res = SYS_Init(&sysConfig);
      if (res != 0)
      {
         goto end;
      }
      
      res = SYS_FlashInit(0x1000, 23);
      if (res != 0)
      {
         goto end;
      }
    
      res = func_21b97e54();
      if (res != 0)
      {
         goto end;
      }
      
      res = SETTINGS_StartupInit();
      if (res != 0)
      {
         goto end;
      }
      
      res = av_init();
      if (res != 0)
      {
         goto end;
      }

      if (sysConfig.appGetStartupOperationMode)
      {
         (sysConfig.appGetStartupOperationMode)();
      }
      
      AV_PowerupScreenShow(1);
      
      res = EVT_Init(); 
      if (res != 0)
      {
         goto end;
      }
      
      res = TIME_Init();
      if (res != 0)
      {
         goto end;
      }
      
      SYS_PrintString("Initialise system modules...");
      
      res = func_21b8d494();
      if (res != 0)
      {
         goto end;
      }
      
      res = SETTINGS_Init();
      if (res != 0)
      {
         goto end;
      }
      
      func_21c9de38(); //->api_f.c: f_enterFS()

      func_21b98644(&Data_21f7ba30); //->settings.c
      
#if 0
      if (Data_21f7ba30 == 0)
      {
         FAPI_SYS_PRINT_MSG("\n SYS_enableHDD = [%d] %d ",
               0, 0x399);
      }
      else
      {
         //21b908dc
         func_21ba5b48("hdd...", 0, 0);
         
         res = func_21bc2b90();
         if (res != 0)
         {
            goto end;
         }
         //->21b90748
      }
#endif
      //21b90748
      res = dev_usb_init();
      if (res != 0)
      {
         goto end;
      }
      
      res = PVRSYS_Init();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bb0bc4();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bb15bc();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21b865e4();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21ba3ce8();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bb2824(&sysConfig.bData_76);
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bc4e78(&sysConfig.bData_80);
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bc0814(&sysConfig.bData_84);
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bc1d4c();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21b9e284();
      
      FAPI_SYS_PRINT_MSG("\n %s %d retVal=%x ",
            "sys/src/main.c", 1012, res);
      
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bbf2d4();
      if (res != 0)
      {
         goto end;
      }

      res = input_init();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21b8cee4();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bc6ab8();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bc9b6c();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bcb390();
      if (res != 0)
      {
         goto end;
      }
      
      res = func_21bcdd0c(&sysConfig);
      
      FAPI_SYS_PRINT_MSG("\n %s %d retVal=%x ",
            "sys/src/main.c", 1102, res);
      
      if (res != 0)
      {
         goto end;
      }
      
      res = SYS_GoUp();
      
      FAPI_SYS_PRINT_MSG("\n %s %d retVal=%x ",
            "sys/src/main.c", 1124, res);
      
      if (res != 0)
      {
         goto end;
      }
      
      extern void ApplicationThread(int a);

      res = SYS_ThreadCreate(ApplicationThread,
            0, 48, 0x4000, "Appl", 0);
   }
#if 0
   else      
   {
      res = 0xff67697f;
   }
#endif
         
end:
   if (res != 0)
   {
      //21b905e0 
      switch (res)
      {            
         case 0xff67697e:
            //21b90658
            SYS_PrintString("System setup failed: OUT OF MEMORY!");
            break;
            
         default:
            //21b905ec
            SYS_PrintString("System setup failed!");
            FAPI_SYS_PRINT_MSG("System setup failed: %d\n", res);
            break;
      }

#if 0
      func_21ba5b48("fail", 0, 0);
#endif
      FAPI_MMU_ReportMemory(FAPI_MMU_HeapHandleSys0, 0);
      FAPI_MMU_ReportMemory(FAPI_MMU_HeapHandleSys1, 1);
      
      rtos_halt(-1);
   }
   //21b90648
   rtos_delete_thread(rtos_get_current_thread());
}


#if 0
/* 21b90aa8 - todo */
int FAPI_SYS_Init(void)
{
   int i;

   for (i = 0; FAPI_SYS_RegInitArr[i] != 0; i++)
   {
      (FAPI_SYS_RegInitArr[i])();
   }

   atexit(CPU_Polling);

   FAPI_UART_SetClockFrequency(81000000);
   FAPI_UART_SetBaudrate(0, 115200);
   FAPI_UART_SetDataBits(0, 8);
   FAPI_UART_SetStopBits(0, 1);
   FAPI_UART_SetParity(0, 0);
   FAPI_UART_SetFlowControl(0, 0);
   FAPI_UART_SetStdioBlockIndex(0);

   FAPI_TIMER_SetClockFrequency(81000000);

   FAPI_GPIO_SetPinFunction(2, 300);
   FAPI_GPIO_SetPinFunction(3, 299);
   FAPI_GPIO_SetPinFunction(4, 296);
   FAPI_GPIO_SetPinFunction(5, 311);
   FAPI_GPIO_SetPinFunction(6, 310);
   FAPI_GPIO_SetPinFunction(7, 298);
   FAPI_GPIO_SetPinFunction(8, 291);
   FAPI_GPIO_SetPinFunction(9, 309);
   FAPI_GPIO_SetPinFunction(10, 308);
   FAPI_GPIO_SetPinFunction(11, 295);
   FAPI_GPIO_SetPinFunction(12, 294);
   FAPI_GPIO_SetPinFunction(13, 297);
   FAPI_GPIO_SetPinFunction(14, 293);
   FAPI_GPIO_SetPinFunction(15, 307);
   FAPI_GPIO_SetPinFunction(16, 306);
   FAPI_GPIO_SetPinFunction(17, 305);
   FAPI_GPIO_SetPinFunction(18, 292);
   FAPI_GPIO_SetPinFunction(19, 304);
   FAPI_GPIO_SetPinFunction(20, 303);
   FAPI_GPIO_SetPinFunction(21, 302);
   FAPI_GPIO_SetPinFunction(22, 290);
   FAPI_GPIO_SetPinFunction(23, 301);
   FAPI_GPIO_SetPinFunction(24, 0x810000);
   FAPI_GPIO_SetPinFunction(25, 0x80);
   FAPI_GPIO_SetPinFunction(26, 0x80);
   FAPI_GPIO_SetPinFunction(27, 0x80);
   FAPI_GPIO_SetPinFunction(28, 0x80);
   FAPI_GPIO_SetPinFunction(29, 0x80);
   FAPI_GPIO_SetPinFunction(30, 0x80);
   FAPI_GPIO_SetPinFunction(31, 0xBD);
   FAPI_GPIO_SetPinFunction(32, 0x115); //UART0 RX
   FAPI_GPIO_SetPinFunction(33, 0x97); //UART0 TX
   FAPI_GPIO_SetPinFunction(34, 0xAC); //UART1 TX
   FAPI_GPIO_SetPinFunction(35, 0x140); //UART1 RX
   FAPI_GPIO_SetPinFunction(36, 0x90);
   FAPI_GPIO_SetPinFunction(37, 0x98010E);
   FAPI_GPIO_SetPinFunction(38, 0x99010F);
   FAPI_GPIO_SetPinFunction(39, 0xFF);
   FAPI_GPIO_SetPinFunction(41, 0xFF);
   FAPI_GPIO_SetPinFunction(42, 0x80);
   FAPI_GPIO_SetPinFunction(43, 0x102);
   FAPI_GPIO_SetPinFunction(44, 177);
   FAPI_GPIO_SetPinFunction(45, 178);
   FAPI_GPIO_SetPinFunction(46, 179);
   FAPI_GPIO_SetPinFunction(47, 180);
   FAPI_GPIO_SetPinFunction(64, 214);
   FAPI_GPIO_SetPinFunction(65, 215);
   FAPI_GPIO_SetPinFunction(66, 216);
   FAPI_GPIO_SetPinFunction(67, 217);
   FAPI_GPIO_SetPinFunction(73, 0x101);
   FAPI_GPIO_SetPinFunction(70, 173);
   FAPI_GPIO_SetPinFunction(71, 174);
   FAPI_GPIO_SetPinFunction(84, 176);
   FAPI_GPIO_SetPinFunction(82, 156);
   FAPI_GPIO_SetPinFunction(83, 155);
   FAPI_GPIO_SetPinFunction(69, 154);
   FAPI_GPIO_SetPinFunction(80, 0x10d);
   FAPI_GPIO_SetPinFunction(85, 0x113);
   FAPI_GPIO_SetPinFunction(86, 158);
   FAPI_GPIO_SetPinFunction(87, 0x112);
   FAPI_GPIO_SetPinFunction(88, 157);
   FAPI_GPIO_SetPinFunction(89, 147);
   FAPI_GPIO_SetPinFunction(90, 146);
   FAPI_GPIO_SetPinFunction(91, 145);
   FAPI_GPIO_SetPinFunction(92, 0x111);
   FAPI_GPIO_SetPinFunction(93, 149);
   FAPI_GPIO_SetPinFunction(94, 272);
   FAPI_GPIO_SetPinFunction(95, 148);

   FAPI_INTR_SetReceiver(5, 1); //TIMER0
   FAPI_INTR_SetReceiver(6, 1); //TIMER1
   FAPI_INTR_SetReceiver(7, 0); //TIMER2
   FAPI_INTR_SetReceiver(8, 1); //UART0
   FAPI_INTR_SetReceiver(9, 1); //UART1
   FAPI_INTR_SetReceiver(10, 1);
   FAPI_INTR_SetReceiver(11, 1);
   FAPI_INTR_SetReceiver(12, 0);
   FAPI_INTR_SetReceiver(13, 0);
   FAPI_INTR_SetReceiver(14, 1);
   FAPI_INTR_SetReceiver(15, 1); //I2C0
   FAPI_INTR_SetReceiver(16, 1); //I2C1
   FAPI_INTR_SetReceiver(17, 1); //I2C2
   FAPI_INTR_SetReceiver(18, 1);
   FAPI_INTR_SetReceiver(19, 1); //GPIO
   FAPI_INTR_SetReceiver(20, 1);
   FAPI_INTR_SetReceiver(21, 1);
   FAPI_INTR_SetReceiver(22, 1);
   FAPI_INTR_SetReceiver(23, 1);
   FAPI_INTR_SetReceiver(24, 1); //BM0 / BM1
   FAPI_INTR_SetReceiver(25, 1); //BM2 / BM3
   FAPI_INTR_SetReceiver(26, 1);
   FAPI_INTR_SetReceiver(27, 1);
   FAPI_INTR_SetReceiver(28, 1);
   FAPI_INTR_SetReceiver(29, 1);
   FAPI_INTR_SetReceiver(30, 0); //HDMI

   FAPI_INTR_SetPriority(5, 13);
   FAPI_INTR_SetPriority(6, 13);
   FAPI_INTR_SetPriority(7, 13);
   FAPI_INTR_SetPriority(8, 13);
   FAPI_INTR_SetPriority(9, 13);
   FAPI_INTR_SetPriority(10, 13);
   FAPI_INTR_SetPriority(11, 13);
   FAPI_INTR_SetPriority(12, 13);
   FAPI_INTR_SetPriority(13, 13);
   FAPI_INTR_SetPriority(14, 7);
   FAPI_INTR_SetPriority(15, 13);
   FAPI_INTR_SetPriority(16, 13);
   FAPI_INTR_SetPriority(17, 13);
   FAPI_INTR_SetPriority(18, 13);
   FAPI_INTR_SetPriority(19, 13);
   FAPI_INTR_SetPriority(20, 13);
   FAPI_INTR_SetPriority(21, 13);
   FAPI_INTR_SetPriority(22, 13);
   FAPI_INTR_SetPriority(23, 13);
   FAPI_INTR_SetPriority(24, 13);
   FAPI_INTR_SetPriority(25, 13);
   FAPI_INTR_SetPriority(26, 13);
   FAPI_INTR_SetPriority(27, 13);
   FAPI_INTR_SetPriority(28, 13);
   FAPI_INTR_SetPriority(29, 13);
   FAPI_INTR_SetPriority(30, 13);

   // Configure the buffer manager
   fapi_bm_set_buffer_base_address(0, 0x20000000);
   fapi_bm_set_buffer_base_address(2, 0x20000000);
   func_21c126a0(0, 0, 0x18c0000, 0x10000);
   func_21c126a0(1, 0, 0x18d0000, 0x10000);
   func_21c126a0(2, 0, 0x18e0000, 0x10000);
   func_21c126a0(3, 0, 0x18f0000, 0x10000);
   func_21c126a0(4, 0, 0x1900000, 0x10000);
   func_21c126a0(5, 0, 0x1910000, 0x10000);
   func_21c126a0(7, 0, 0x1920000, 0x10000);

   FAPI_FLASH_SetEnable(FAPI_PARALLEL_FLASH, 1);
   FAPI_FLASH_SetEnable(FAPI_SERIAL_FLASH, 1);
   FAPI_FLASH_SetUpiIndex(FAPI_PARALLEL_FLASH, 0);

   func_21c0ef00(0, 0, 2);

   func_21c42e8c(0x20000000, 0x4000000, 0x40000000, 0x4000000);
   func_21c42f20(0, 0x20000000);

   FAPI_MMU_SetSegmentSizeMBytes(0, 12); //sys0 segment
   FAPI_MMU_SetSegmentSizeMBytes(1, 14); //sys1 segment

   func_21c0eee8(0);

   if (FAPI_SYS_Services.initDrivers != 0)
   {
      return (FAPI_SYS_Services.initDrivers)(FAPI_SYS_DriverArr);
   }

   return 0;
}
#endif


#define BOARDS_INSTALL_PLATFORM_SUPPORT      BOARD_GMI_MB86H60_STB_CI_RegisterBoardIfc
#define BOARDS_INSTALL_FRONTEND_SUPPORT      BOARD_GMI_MB86H60_STB_CI_RegisterFrontends

extern int BOARD_GMI_MB86H60_STB_CI_RegisterBoardIfc(int, int);


/* 21b91298 - todo */
int SYS_Start(struct appData* a)
{
   int i, j, crc;
   int res = 0;

   Data_21f02a14 = 0;

   BOARDS_INSTALL_PLATFORM_SUPPORT(0, 0);
   BOARDS_INSTALL_FRONTEND_SUPPORT(0, 0);

   sysConfig = *a;

   RTOS_InitServices();

   res = FAPI_SYS_Init();

#if 1
   printf("FAPI_SYS_Init() res=%d\n", res);
#endif
   
   if (res == 0)
   {      
      if (0 != RTOS_InitKernel())
      {
         FAPI_BOARD_INIT_DETECT_FUNCTION(0, 0);

         SYS_GetBootloaderInfo();

         // Initialize the CRC table
         for (i = 0; i < 256; i++)
         {
            crc = i << 24;

            for (j = 8; j != 0; j--)
            {
               if (crc & 0x80000000)
               {
                  crc = 0x4c11db7 ^ (crc << 1);
               }
               else
               {
                  crc <<= 1;
               }
            }

            Data_21f02a1c[i] = crc;
         }

         res = SYS_ThreadCreate(StartupThread, 
            0, 58, 0x2000, "Startup", 0);
      }
      else
      {
         //21b9137c
         _exit(1);
      }  
   }
   else
   {
      //21b9137c
      _exit(1);
   }
   
   if (res != 0) return res;
   
   rtos_start();

   return res;
}


/* 21b97e54 - todo */
int func_21b97e54()
{
//   printf("func_21b97e54");
   return 0;
}


int func_21b9e284(void)
{
//   printf("func_21b9e284");
   return 0;
}


int func_21ba3ce8(void)
{
//   printf("func_21ba3ce8");
   return 0;
}


int func_21bb0bc4(void)
{
//   printf("func_21bb0bc4");
   return 0;
}


int func_21bb15bc(void)
{
//   printf("func_21bb15bc");
   return 0;
}


int func_21bb2824(int a)
{
//   printf("func_21bb2824");
   return 0;
}


int func_21bbf2d4(void)
{
//   printf("func_21bbf2d4");
   return 0;
}


int func_21bc0814(int a)
{
//   printf("func_21bc0814");
   return 0;
}


int func_21bc1d4c(void)
{
//   printf("func_21bc1d4c");
   return 0;
}

#if 0

int func_21bc2b90(void)
{
//   printf("func_21bc2b90");
   return 0;
}

#endif

int func_21bc4e78(int a)
{
//   printf("func_21bc4e78");
   return 0;
}


int func_21bc6ab8(void)
{
//   printf("func_21bc6ab8");
   return 0;
}


int bootInfValid; //21f0b1d8
FAPI_BOOT_InfoT bootInf; //21f7e338
uint32_t bootMode; //21f7e34c


/* 21bcb810 - todo */
int SYS_GetBootloaderInfo(void)
{
   int32_t res;
   FAPI_SYS_HandleT r6;
   FAPI_BOOT_OpenParamsT sp;
   
   sp.version = FAPI_BOOT_VERSION;
   
   r6 = FAPI_BOOT_Open(&sp, &res);
   
   if (res != 0)
   {
      return res;            
   }
   
   res = FAPI_BOOT_GetBootMode(r6, &bootMode);
   
   if (res != 0)
   {
      //21bcb86c
      FAPI_BOOT_Close(r6);
      return res;
   }
   
   if (bootMode == 3)
   {
      FAPI_BOOT_GetInfo(r6, &bootInf);
      
      FAPI_SYS_PRINT_MSG("FAPI_BOOT_GetInfo says mode %d device %d\n",
            bootInf.bootMode,
            bootInf.bootDevice);
      
      if (bootInf.slotAddress1 == 0)
      {         
         if (bootInf.slotAddress0 == 0)
         {
            //21bcb8f8
            bootInf.slotAddress1 = 0xa0000;
            bootInf.slotIndex = 0;
            bootInf.slotAddress0 = 0x40000;
            bootInf.slotAddress1 = 0xa0000;
            //->21bcb8e8
         }
         else 
         {
            //21bcb8c0            
            int r3 = bootInf.slotAddress0 - 0x40000;
            int r2 = bootInf.slotAddress0 - 0xa0000;
            
            if (r3 < 0) r3 = -r3;            
            if (r2 < 0) r2 = -r2;
            
            if (r3 > r2)
            {
               bootInf.slotAddress1 = 0x40000;
            }
            else
            {
               bootInf.slotAddress1 = 0xa0000;
            }
         }
      }
      //->21bcb8e8
      bootInfValid = 1;
   }   
   //21bcb86c
   FAPI_BOOT_Close(r6);
   
   return res;
}


int func_21bc9b6c(void)
{
//   printf("func_21bc9b6c");
   return 0;
}


/* 21bcb4a4 - complete */
int SYS_GetBootInfo(FAPI_BOOT_InfoT* a)
{
   int res = 0;
   
   if (bootInfValid == 0)
   {
      res = -10001006;
   }
   else
   {
      *a = bootInf;
   }
   
   return res;
}


int func_21bcb390(void)
{
//   printf("func_21bcb390");
   return 0;
}


int func_21bcdd0c(void* a)
{
//   printf("func_21bcdd0c");
   return 0;
}

#if 0

int func_21b35b58(void* a)
{
   return 0;
}

#endif

/* 21b8e58c - todo */
/* v3.8: 36072c - complete */
int SYS_StrNCpy(char_t* a, const char_t* b, uint32_t max_count)
{
   int count = 0;
   
   if (max_count == 0) return 0;
   if ((b == 0) || (a == 0)) return 0;
   
   while ((b[0] != 0) && (max_count-- > 1))
   {
      *a++ = *b++;
      count++;
   }
   
   *a = 0;
   
   return count;
}


/* 21b8e92c - complete */
/* v3.8: 35e0e0 - complete */
void func_21b8e92c(Struct_35e0a8* a, unsigned b, unsigned c)
{
   a->a += (b + c / 1024);
   a->b += c % 1024;
   
   while (a->b > 1024)
   {
      a->a++;
      a->b -= 1024;
   } 
}


int SYS_SwitchOn(void)
{
   printf("SYS_SwitchOn\n");
   
   return 0;
}


int SYS_SwitchOff(void)
{
   printf("SYS_SwitchOff\n");
   
   return 0;
}


/* 21b8fea0 - complete */
int SYS_OnOff(int a)
{     
   int res;
   
   if (a == sysData->powerOn)
   {
      return 0;
   } 
   
   switch (a)
   {
      case 0:
         res = SYS_SwitchOff();
         break;
         
      case 1:
         res = SYS_SwitchOn();
         break;
         
      default:
         res = -10000001;
         break;
   } 
   
   return res;
}


/* 21b8e854 - complete */
int sys_check_osd_layer_index(unsigned layer)
{   
   if (layer > 2)
   {
      return -10000001;
   }
   
   return 0;
}


/* 21b8e86c - complete */
int sys_get_osd_layer_index(unsigned layer)
{   
   return (layer > 2)? -1: layer;
}


/* 21b8f67c - complete */
int SYS_GetOsdLayer(struct sys_osd_layer_params* r5, int (*reqCb)(char))
{
   uint32_t errorCode;
   int res;
   int index;
   int layer = r5->layer;
   
   if (0 != sys_check_osd_layer_index(layer))
   {
      FAPI_SYS_PRINT_MSG("[SYS] Invalid OSD Layer 0x%08x\n",
            layer);      
      return -10000001;
   }

   index = sys_get_osd_layer_index(layer);
   
   if (sysData->arData_40[index].inUse)
   {
      if ((sysData->arData_40[index].reqCb != 0) &&
            (0 != (sysData->arData_40[index].reqCb)(layer)))
      {
         res = SYS_GiveOsdLayer(r5->layer);
         
         if (res != 0)
         {
            FAPI_SYS_PRINT_MSG("[SYS] SYS_GiveOsdLayer() failed: %ld\n", res);
            return res;
         }

         layer = r5->layer;
      }
      else
      {
         FAPI_SYS_PRINT_MSG("[SYS] reqCb 0x%x\n", 
               sysData->arData_40[index].reqCb);
         return -10001001;
      }
   }

   res = OSDHANDLER_StartLayer(layer, &r5->params, &errorCode);
   
   if (res != 0)
   {
      FAPI_SYS_PRINT_MSG("[SYS] OSDHANDLER_StartLayer() failed: %ld\n", res);
      return res;
   }

   sysData->arData_40[index].inUse = 1;
   sysData->arData_40[index].reqCb = reqCb;
   
   return 0;
}


/* 21b8f588 - complete */
int SYS_GiveOsdLayer(int layer)
{
//   printf("SYS_GiveOsdLayer\n");
   
   uint32_t errorCode;
   int index;
   
   if (0 != sys_check_osd_layer_index(layer))
   {
      return -10000001;
   }
   
   index = sys_get_osd_layer_index(layer);
   
   if (sysData->arData_40[index].inUse)
   {
      OSDHANDLER_StopLayer(layer, &errorCode);
      
      sysData->arData_40[index].inUse = 0;
   }
   
   return 0;
}


/* 21b8e600 - complete */
uint32_t UTILS_strlen(const char_t *str)
{
    uint32_t                      cnt = 0;

    while(*str++ >= ' ')
        cnt++;

    return cnt;
}


