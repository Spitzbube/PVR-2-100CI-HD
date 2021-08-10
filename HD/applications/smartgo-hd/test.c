

#if 0

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#endif
#include <fapi/sys_services.h>
#if 0
#include "famos.h"
#include "bget.h"
#include "drv_mmu_heap.h"
#include "fapi_adapter.h"

extern int func_21cbc338(int a,
                   void (*b)(void),
                   int c, int d);


int fontFreeTypeAvailable;


int Data_21e85e44; 

#endif

int Data_21ea13c8;

#if 0

int Data_21efb16c;
int Data_21efb55c; 
int Data_21efae40;
int Data_21efc180;
int Data_21efc1a4;
int Data_21efc1c8;
int Data_21efa698;
int Data_21efa6bc;
int Data_21efb304;
int Data_21efb328;
int Data_21efa524;
int Data_21efa548;
int Data_21efa56c;
int Data_21efa590;
int Data_21efc550;
int Data_21efbf28;
int Data_21efbf4c;
int Data_21efc834;
int Data_21efc858;
int Data_21efc87c;
int Data_21efe44c;
int Data_21efa424;
int Data_21efa3c0;

int Data_21f62220; //21f62220
int Data_21f62224; //21f62224
int Data_21f62228; //21f62228
struct
{
   int Data_0; //0
   int Data_4; //4
   int Data_8; //8
   int fill_12[16]; //12
} arData_21f6222c[8]; //21f6222c

union
{
   int data;
   volatile int vdata; 
   struct
   {
      int bits_0_7: 8;
      int bits_8_15: 8;
   } bits;
} Data_21f6a7ec;


extern void* Data_21f27c98; //21f27c98
extern int Data_21f27c9c; //21f27c9c
extern int Data_21f27ca0; //21f27ca0



void func_21b01e38(void)
{
   OSD_Fill8bpp();
}


void func_21b01e3c(void)
{
}



void func_21b0275c(void)
{
}


void func_21b027e0(void)
{
}

void func_21b027e8(void)
{
}


void func_21b028b0(void)
{
}


void func_21b02a64(void)
{
}

#endif

/* 21b02a8c - todo */
void MAIN_SetStartupDefaults(void)
{
}

#if 0

void func_21b02c2c(void)
{
}

#endif

void MAIN_GetStartupOperationMode(void)
{
}


/* 21ba53c0 - todo */
void func_21ba53c0(int a, int b)
{
   FAPI_SYS_PRINT_MSG("func_21ba53c0\n");
}


/* 21bb8290 - todo */
void func_21bb8290(unsigned a)
{
   FAPI_SYS_PRINT_MSG("21bb8290\n");
}


/* 21ca2d6c - todo */
int func_21ca2d6c(int a)
{
   FAPI_SYS_PRINT_MSG("21ca2d6c\n");
   
   return 0;
}


void func_21c0eee8(int a)
{
}


void func_21c0ef00(int a, int b, int c)
{
}

#if 0

void func_21c11678(void)
{
   
}


/* 21c6d860 - complete */
void func_21c6d860(void)
{
   Data_21f6a7ec.vdata = Data_21f6a7ec.data;
   Data_21f6a7ec.vdata = Data_21f6a7ec.data;
}


/* 21c6d87c - complete */
void func_21c6d87c(int a)
{
   *((int*) 0xcc000000) = a;
}


/* 21c6d888 - complete */
void func_21c6d888(int a)
{
   a = (a << 11) & (1 << 11);

   *((int*) 0xcc000000) = 
      (*((int*) 0xcc000000) & ~(1 << 11)) | a;
}


/* 21c6d8a8 - complete */
int func_21c6d8a8(void)
{
   return *((int*) 0xcc000000);
}


/* 21c6d8b4 - complete */
int func_21c6d8b4(void)
{
   return *((int*) 0xcc000004);
}


/* 21c6d8c0 - complete */
int func_21c6d8c0(int a)
{
   return (a & (1 << 13)) >> 13;
}


/* 21c6d8cc - complete */
int func_21c6d8cc(int a)
{
   return (a & (1 << 12)) >> 12;
}


/* 21c6d8d8 - complete */
int func_21c6d8d8(int a)
{
   return (a & (1 << 11)) >> 11;
}


/* 21c6d8e4 - complete */
int func_21c6d8e4(int a)
{
   return (a & (1 << 10)) >> 10;
}


/* 21c6d8f0 - complete */
int func_21c6d8f0(int a)
{
   return (a & (1 << 9)) >> 9;
}


/* 21c6d8fc - complete */
int func_21c6d8fc(int a)
{
   return (a & (1 << 8)) >> 8;
}


/* 21c6d908 - complete */
int func_21c6d908(int a)
{
   return (a & (1 << 7)) >> 7;
}


/* 21c6d914 - complete */
int func_21c6d914(int a)
{
   return (a & (1 << 6)) >> 6;
}


/* 21c6d920 - complete */
int func_21c6d920(int a)
{
   return (a & (1 << 5)) >> 5;
}


/* 21c6d92c - complete */
int func_21c6d92c(int a)
{
   return (a & (1 << 4)) >> 4;
}


/* 21c6d938 - complete */
int func_21c6d938(int a)
{
   return (a & (1 << 3)) >> 3;
}


/* 21c6d944 - complete */
int func_21c6d944(int a)
{
   return (a & (1 << 2)) >> 2;
}


/* 21c6d950 - complete */
int func_21c6d950(int a)
{
   return (a & (1 << 1)) >> 1;
}


/* 21c6d95c - complete */
int func_21c6d95c(int a)
{
   return (a & (1 << 0)) >> 0;
}


/* 21c6d964 - complete */
void func_21c6d964(int a, int b)
{
   ((int*) 0xcc000008)[a] = b;
}


/* 21c6d974 - todo */
void func_21c6d974(int a, int b)
{
   b = (b << 13) & (1 << 13);

   ((int*) 0xcc000008)[a] = 
      (((int*) 0xcc000008)[a] & ~(1 << 13)) | b;
}


/* 21c6d998 - todo */
void func_21c6d998(int a, int b)
{
   b = (b << 12) & (1 << 12);

   ((int*) 0xcc000008)[a] = 
      (((int*) 0xcc000008)[a] & ~(1 << 12)) | b;
}


/* 21c6d9bc - todo */
void func_21c6d9bc(int a, int b)
{
   b = (b << 10) & (3 << 10);

   ((int*) 0xcc000008)[a] = 
      (((int*) 0xcc000008)[a] & ~(3 << 10)) | b;
}


/* 21c6dde8 - complete */
int func_21c6dde8(int a)
{
   a = (a << 8) & 0xFF00;
   Data_21f6a7ec.data = 
      (Data_21f6a7ec.data & ~0xFF00) | a;
   return *((int*) 0xcc000068) = Data_21f6a7ec.data;
}


/* 21c6ead8 - complete */
void func_21c6ead8(void)
{
   /* empty */
}

void func_21c6f5b0(void)
{
}


void func_21c703c0(void)
{
}


void func_21c703e4(void)
{
}



void func_21c721a4(void)
{
}


void func_21c72844(void)
{
}


void func_21c72adc(void)
{
}


void func_21c72b04(void)
{
}


void func_21c72cfc(void)
{
}



void func_21c756a8(void)
{
}


void func_21c76544(void)
{
}



void func_21c76b00(void)
{
}

#endif

/* 21bc2464 - todo */
void func_21bc2464(void)
{
   FAPI_SYS_PRINT_MSG("21bc2464\n");
}


/* 21bc6c8c - todo */
void func_21bc6c8c(void)
{
   FAPI_SYS_PRINT_MSG("21bc6c8c\n");
}

#if 0

/* atexit */
int func_21cb5250(void (*a)(void))
{
   //return func_21cbc338(0, a, 0, 0);

   return __register_exitproc(0, a, 0, 0);
}

#endif


/* 21b7e0a8 - todo */
int func_21b7e0a8(void)
{
   FAPI_SYS_PRINT_MSG("func_21b7e0a8\n");

   return 0;
}


/* 21cb3d4c - todo */
int func_21cb3d4c(void* a)
{
   return 0;
}


/* 21bbbf58 - todo */
int func_21bbbf58(void)
{
   FAPI_SYS_PRINT_MSG("func_21bbbf58\n");

   return 0;
}


/* 21b807e8 - todo */
int func_21b807e8(void)
{
   FAPI_SYS_PRINT_MSG("func_21b807e8\n");

   return 0;
}


/* 21b946b0 - todo */
int func_21b946b0(void)
{
   FAPI_SYS_PRINT_MSG("func_21b946b0\n");

   return 0;
}


/* 21b98028 - todo */
void func_21b98028(int a)
{
   FAPI_SYS_PRINT_MSG("func_21b98028\n");

}


/* 21b7a260 - todo */
void func_21b7a260(void)
{
   FAPI_SYS_PRINT_MSG("func_21b7a260\n");
}

#if 0

/* 21b793b8 - todo */
int func_21b793b8(void* a)
{
   FAPI_SYS_PRINT_MSG("func_21b793b8\n");
   return 0;
}


/* 21b7dfd8 - todo */
void func_21b7dfd8(void)
{
   FAPI_SYS_PRINT_MSG("func_21b7dfd8\n");
}


/* 21b7ef40 - todo */
void func_21b7ef40(void)
{
   FAPI_SYS_PRINT_MSG("func_21b7ef40\n");
}


/* 21b7a670 - todo */
void func_21b7a670(void)
{

}


/* 21b8e9b0 - todo */
void func_21b8e9b0(int a, int b)
{
   FAPI_SYS_PRINT_MSG("func_21b8e9b0\n");
}



/* 21b95498 - todo */
int func_21b95498(char a, int b)
{
   FAPI_SYS_PRINT_MSG("func_21b95498\n");

   return 0;
}


/* 21b7a648 - todo */
int func_21b7a648(void* a, char b)
{
   FAPI_SYS_PRINT_MSG("func_21b7a648\n");

   return 0;
}


/* 21bb0c88 - todo */
int func_21bb0c88(void* a, int b)
{
   FAPI_SYS_PRINT_MSG("func_21bb0c88\n");

   return 0;
}


#if 1
int FAPEXSAL_Mmap()
{
   return 0;
}


int fapex_COMMON_Open()
{
   return 0;
}

int fapex_COMMON_Ioctl()
{
   return 0;
}


int FAPEXSAL_Release()
{
   return 0;
}
#endif

#endif

void fm_seek()
{
   
}


void fm_tell()
{
   
}

#if 0

void ft_strncmp()
{
   
}


#endif



/* 21ba9c34 - todo */
int func_21ba9c34(void* a)
{
   printf("func_21ba9c34\n");
   
   return 0;
}


/* 21baa0d0 - todo */
int func_21baa0d0(void* a)
{
   printf("func_21baa0d0\n");
   
   return 0;
}


/* 21bab438 - todo */
int func_21bab438(void* a)
{
   printf("func_21bab438\n");
   
   return 0;
}


/* 21baba70 - todo */
int func_21baba70(void* a)
{
   printf("func_21baba70\n");
   
   return 0;
}


/* 21bafd24 - todo */
int func_21bafd24(void* a)
{
   printf("func_21bafd24\n");
   
   return 0;
}


/* 21cad4c0 - todo */
int func_21cad4c0(void)
{
   printf("func_21cad4c0\n");

   return 0;
}


/* 21b8d98c - todo */
void func_21b8d98c(int a, int b)
{
   printf("func_21b8d98c\n");
}


/* 21b9b08c - todo */
int func_21b9b08c(void* a)
{
   printf("func_21b9b08c\n");

   return 0;
}


/* 21b8cdb4 - todo */
void func_21b8cdb4(void* a)
{
   printf("func_21b8cdb4\n");
}


/* 21b02c3c - todo */
void func_21b02c3c(void)
{
   printf("21b02c3c\n");
}


/* 21b02df4 - todo */
void MAIN_UserDataSet(int a, int b)
{
    FAPI_SYS_PRINT_MSG("MAIN_UserDataSet(a=%d, b=%d)\n", a, b);
}




/* 21b02e64 - todo */
int MAIN_UserDataGet(int a, unsigned* b)
{
#if 0
   FAPI_SYS_PRINT_MSG("MAIN_UserDataGet(a=%d)\n", a);
#endif
   
   if (a == /*USERDAT_COLORSCHEME*/1)
   {
       *b = /*OSD_COLSCHEME_RED*/2;
   }

   return 0;
}


/* 21ca9b8c - todo */
int func_21ca9b8c(void* a, char* b, char* c)
{
   printf("func_21ca9b8c\n");
   
   return 0;
}


/* 21cadb54 - todo */
int func_21cadb54(int a)
{
   printf("func_21cadb54\n");
   
   return 0;
}

#if 0

/* 218aa904 - todo */
int func_218aa904(void* a, int* b)
{
   FAPI_SYS_PRINT_MSG("func_218aa904\n");
   
   return 0;
}


/* 218aa7b4 - todo */
void func_218aa7b4(int a)
{
   FAPI_SYS_PRINT_MSG("func_218aa7b4\n");
   
}


/* 218aaa38 - todo */
int func_218aaa38(int a, int b, void* c)
{
   FAPI_SYS_PRINT_MSG("func_218aaa38\n");
   
   return 0;
}


/* 218aaca8 - todo */
int func_218aaca8(int a, int b, void* c)
{
   FAPI_SYS_PRINT_MSG("func_218aaca8\n");
   
   return 0;
}

#endif

/* 21b7c588 - todo */
void func_21b7c588(int a, int b)
{
   FAPI_SYS_PRINT_MSG("func_21b7c588\n");

}

#if 0

/* 21ba60bc - todo */
void func_21ba60bc()
{
   FAPI_SYS_PRINT_MSG("func_21ba60bc\n");
}

#endif

void func_376790()
{
   
}

#if 0

void func_21923dec()
{
   FAPI_SYS_PRINT_MSG("func_21923dec\n");
}

#endif

/* 21b0d390 - todo */
void func_21b0d390()
{
   FAPI_SYS_PRINT_MSG("func_21b0d390\n");
}


void func_3724e4(void)
{
   FAPI_SYS_PRINT_MSG("func_3724e4\n");
}


void func_3725cc(void)
{
   FAPI_SYS_PRINT_MSG("func_3725cc\n");
}


void func_371c00(void)
{
   FAPI_SYS_PRINT_MSG("func_371c00\n");
}


/* 2181be04 - todo */
void func_2181be04()
{
   
}

void func_21ba2f64()
{
   
}


void time_get_days_per_month()
{
   
}


void time_compare()
{
   
}


void func_2182384c()
{
   
}


void func_21b01d40()
{

}


/* 21b0d188 - todo */
void func_21b0d188()
{

}


/* 21b98138 - todo */
int func_21b98138()
{

}


/* 21b99d68 - todo */
void func_21b99d68()
{

}


/* 21b0bfac - todo */
void func_21b0bfac()
{

}


/* 21b0bb2c - todo */
void func_21b0bb2c()
{

}


/* 21b0d130 - todo */
void func_21b0d130()
{

}


/* 21b23f74 - todo */
void func_21b23f74()
{

}


/* 21b0f568 - todo */
int func_21b0f568()
{
    FAPI_SYS_PRINT_MSG("func_21b0f568: TODO\n");

    return 1;
}


void func_21b12404()
{

}


void func_21bb9ee8()
{

}


void SLIST_ListNameSet()
{

}


/* 21bc22b0 - todo */
void func_21bc22b0()
{

}



/* 21bc2f14 - todo */
void func_21bc2f14()
{

}


/* 21b37144 - todo */
/*fbool_t*/ MENU_TimeEntry(/*FAPI_SYS_HandleT h, fbool_t test*/void)
{

}


/* 21b3f0c4 - todo */
/*fbool_t*/ MENU_AntProgEntry(/*FAPI_SYS_HandleT h, fbool_t test*/)
{

}


/* 21b7c96c - todo */
void func_21b7c96c()
{

}


/* 21b030ec - todo */
void MAIN_SaveSettingsIfChanged()
{

}


/* 21b8c0fc - todo */
void EIT_ClearDatabase()
{

}


/* 21b0d740 - todo */
void MAIN_VolumeBarHide()
{

}


/* 21bc4d10 - todo */
void SUBTT_ShowOsd()
{

}


/* 21ba386c - todo */
void TTXSUBTT_Restart()
{

}


/* 21bc4d44 - todo */
void SUBTT_HideOsd()
{

}


/* 21ba3878 - todo */
void TTXSUBTT_Hide()
{

}


/* 21bb9358 - todo */
int PVRDEC_Init()
{
    return 0;
}


/* 21be25e8 - todo */
int PVR_Init()
{
    return 0;
}


/* 21be250c - todo */
int PVR_SetFileExtension()
{
    return 0;
}


/* 21be24c8 - todo */
int PVR_SetFileSplitParams(int a, int b)
{
    return 0;
}


/* 21bb6884 - todo */
int PVRSYS_SetFastModes()
{
    return 0;
}


/* 21bbb458 - todo */
int pvr_list_init()
{
    return 0;
}


void PVR_GetPlaybackMode()
{

}


void PVRSYS_PlayGetSlowSpeedFactor()
{

}


void PVRSYS_PlayFastModeIdxToSpeed()
{

}


void PVRSYS_PlayGetFastSpeedFactor()
{

}

/* 21bc8e88 - todo */
void func_21bc8e88()
{

}


/* 21b0c8d0 - todo */
void func_21b0c8d0()
{

}


/* 21ba2148- todo */
void TIME_IsSync()
{

}


/* 21bbdf60 - todo */
void func_21bbdf60()
{

}


/* 21bbc5f4 - todo */
void func_21bbc5f4()
{

}


/* 21b0e834 - todo */
void func_21b0e834()
{

}


/* 21bc9394 - todo */
void func_21bc9394()
{

}


/* 21b0c620 - todo */
void func_21b0c620()
{

}




