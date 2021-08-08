#ifndef SYS_SERVICES_H_
#define SYS_SERVICES_H_

#include <fapi/sys_driver.h>

struct FAPI_SYS_Services
{
    void (*lockSchedulerFunc)(void); //0
    void (*unlockSchedulerFunc)(void); //4
    void (*sleepFunc)(uint32_t mSecs); //8
   unsigned (*disableIrq)(void); //12
   void (*enableIrq)(unsigned); //16
   void* (*createSemaphore)(int); //20
   void (*deleteSemaphore)(void*); //24
   int (*getSemaphoreFunc)(void*, int); //28
   void (*releaseSemaphore)(void*, int); //32
   uint32_t (*getPhysAddressFunc)(uint32_t virtAddress);
   uint32_t (*getVirtAddressFunc)(uint32_t physAddress);
   void* (*mallocFunc)(size_t size); //60??
   void* (*mallocUncachedFunc)(size_t size); //64
   void* (*mallocCachedFunc)(size_t size); //68
   void* (*mallocSegmentFunc)(FAPI_SYS_HandleT segment, size_t size); //72
   void (*freeFunc)(void* ptr); //76
   int (*printfFunc)(const char*, ...); //80
   int (*initDrivers)(FAPI_SYS_DriverT* a[]); //84
   //88
};


extern struct FAPI_SYS_Services FAPI_SYS_Services; //21b140a0

extern void FAPI_SYS_SetServices(struct FAPI_SYS_Services*);
extern void FAPI_SYS_InvalidateDataCache(int, int);
extern void FAPI_SYS_FlushDataCache(void*, int);
extern int FAPI_SYS_GetDeviceId(void);
extern int FAPI_SYS_IsIrqEnabled(void);
extern int FAPI_SYS_IsIsrActive(void);
extern void FAPI_SYS_SetMasterCpu(void);
extern int FAPI_SYS_IsMasterCpu(void);


#define FAPI_SYS_NO_SUSPEND         0x00000000UL
#define FAPI_SYS_SUSPEND            0xFFFFFFFFUL

#define FAPI_SYS_DEVICE_ID_UNKNOWN  0x00000000UL
#define FAPI_SYS_DEVICE_ID_MB86H60  0x48363000UL
#define FAPI_SYS_DEVICE_ID_MB86H60B 0x48363042UL


#define FAPI_SYS_DISABLE_IRQ                           \
        ( ( FAPI_SYS_Services.disableIrq != NULL ) \
        ? FAPI_SYS_Services.disableIrq()           \
        : 0 )

#define FAPI_SYS_ENABLE_IRQ(irqFlags)                      \
        {                                                  \
            if( FAPI_SYS_Services.enableIrq != NULL )  \
            {                                              \
                FAPI_SYS_Services.enableIrq(irqFlags); \
            }                                              \
        }


#define FAPI_SYS_LOCK_SCHEDULER                               \
        {                                                     \
            if( FAPI_SYS_Services.lockSchedulerFunc != NULL ) \
            {                                                 \
                FAPI_SYS_Services.lockSchedulerFunc();        \
            }                                                 \
        }

#define FAPI_SYS_UNLOCK_SCHEDULER                               \
        {                                                       \
            if( FAPI_SYS_Services.unlockSchedulerFunc != NULL ) \
            {                                                   \
                FAPI_SYS_Services.unlockSchedulerFunc();        \
            }                                                   \
        }


#define DI(cpu_sr) \
   cpu_sr = (FAPI_SYS_Services.disableIrq != 0)?\
      (FAPI_SYS_Services.disableIrq)(): 0;\
      
#define EI(cpu_sr) \
   if (FAPI_SYS_Services.enableIrq != 0)\
   {\
      (FAPI_SYS_Services.enableIrq)(cpu_sr);\
   }

#define LOCK(lock) \
   if (FAPI_SYS_Services.getSemaphoreFunc != 0)\
   {\
      (FAPI_SYS_Services.getSemaphoreFunc)(lock, -1);\
   }

#define UNLOCK(lock) \
   if (FAPI_SYS_Services.releaseSemaphore != 0)\
   {\
      (FAPI_SYS_Services.releaseSemaphore)(lock, 0);\
   }


#define FAPI_SYS_SLEEP(milliSeconds)                       \
        {                                                  \
            if( FAPI_SYS_Services.sleepFunc != NULL )      \
            {                                              \
                FAPI_SYS_Services.sleepFunc(milliSeconds); \
            }                                              \
        }


#define FAPI_SYS_CREATE_SEMAPHORE(val)                      \
        ( ( FAPI_SYS_Services.createSemaphore != NULL ) \
        ? FAPI_SYS_Services.createSemaphore(val)        \
        : 0)

#define FAPI_SYS_DESTROY_SEMAPHORE(sem )                         \
        {                                                        \
            if( FAPI_SYS_Services.deleteSemaphore != NULL ) \
            {                                                    \
                FAPI_SYS_Services.deleteSemaphore(sem);     \
            }                                                    \
        }

#define FAPI_SYS_GET_SEMAPHORE(sem,timeout)               \
        ( ( FAPI_SYS_Services.getSemaphoreFunc != NULL )  \
        ? FAPI_SYS_Services.getSemaphoreFunc(sem,timeout) \
        : -1 )                                            \

#define FAPI_SYS_SET_SEMAPHORE(sem,timeout)                      \
        {                                                        \
            if( FAPI_SYS_Services.releaseSemaphore != NULL )     \
            {                                                    \
                FAPI_SYS_Services.releaseSemaphore(sem,timeout); \
            }                                                    \
        }

#define FAPI_SYS_GET_PHYSICAL_ADDRESS(virtualAddress)          \
        ( ( FAPI_SYS_Services.getPhysAddressFunc != NULL )     \
        ? FAPI_SYS_Services.getPhysAddressFunc(virtualAddress) \
        : virtualAddress )

#define FAPI_SYS_GET_VIRTUAL_ADDRESS(physicalAddress)           \
        ( ( FAPI_SYS_Services.getVirtAddressFunc != NULL )      \
        ? FAPI_SYS_Services.getVirtAddressFunc(physicalAddress) \
        : physicalAddress )

#define FAPI_SYS_MALLOC_CACHED(size)                     \
        ( ( FAPI_SYS_Services.mallocCachedFunc != NULL ) \
        ? FAPI_SYS_Services.mallocCachedFunc(size)       \
        : NULL )

#define FAPI_SYS_MALLOC_SEGMENT(segment,size)               \
        ( ( FAPI_SYS_Services.mallocSegmentFunc != NULL )   \
        ? FAPI_SYS_Services.mallocSegmentFunc(segment,size) \
        : NULL )

#define FAPI_SYS_MALLOC(size)                              \
        ( ( FAPI_SYS_Services.mallocFunc != NULL ) \
        ? FAPI_SYS_Services.mallocFunc(size)       \
        : NULL )

#define FAPI_SYS_MALLOC_UNCACHED(size)                     \
        ( ( FAPI_SYS_Services.mallocUncachedFunc != NULL ) \
        ? FAPI_SYS_Services.mallocUncachedFunc(size)       \
        : NULL )

#define FAPI_SYS_FREE(ptr)                           \
        {                                            \
            if( FAPI_SYS_Services.freeFunc != NULL ) \
            {                                        \
                FAPI_SYS_Services.freeFunc(ptr);     \
            }                                        \
        }


#define FAPI_SYS_INIT_DRIVERS(pDriverArr)                              \
        ((FAPI_SYS_Services.initDrivers != NULL)                   \
         ? FAPI_SYS_Services.initDrivers(pDriverArr)               \
         : 0)


//#define FAPI_SYS_PRINT_MSG printf
#define FAPI_SYS_PRINT_MSG(...)                                  \
        {                                                        \
            if( FAPI_SYS_Services.printfFunc )                   \
            {                                                    \
                /*lint -save -e522 */                            \
                (void)FAPI_SYS_Services.printfFunc(__VA_ARGS__); \
                /*lint -restore */                                \
            }                                                     \
        }


#if DEBUG > 0
    #if defined(__arm__) && !defined(__polyspace__)
        /*!
        ************************************************************************
        ** \brief Print a debug message.
        **
        ** This macro calls the registered FAPI_SYS_ServicesT::printfFunc
        ** (if there is one) to print a debug message in the operating
        ** environment.
        ** Debug messages will only be printed if the debug level parameter
        ** is lower or equal to the value of the \b DEBUG define.
        ** When DEBUG is 0 or undefined the macro stays empty.
        **
        ** \param level The required debug level for the message.
        **
        ** \param printArgs The aruments in 'printf' style.
        **
        ** \note MISRA 2004 Rule 14.2 (Lint rule 522) disabled because we
        **     dont need the return value of the internal printf function.
        ************************************************************************
        */

        #define FAPI_SYS_PRINT_DEBUG(level,...)                              \
                {                                                            \
                    /*lint -save -e506 -e522 -e774 */                        \
                    if( (DEBUG >= (level) ) && FAPI_SYS_Services.printfFunc) \
                    {                                                        \
                        (void)FAPI_SYS_Services.printfFunc(__VA_ARGS__);     \
                    }                                                        \
                    /*lint -restore */                                       \
                }
    #endif

    #if defined(_ARC) && !defined(__polyspace__)
        #define FAPI_SYS_PRINT_DEBUG(level,varargs...)                       \
                {                                                            \
                    /*lint -save -e506 -e522 -e774 */                        \
                    if( (DEBUG >= (level) ) && FAPI_SYS_Services.printfFunc) \
                    {                                                        \
                        (void)FAPI_SYS_Services.printfFunc(varargs);         \
                    }                                                        \
                    /*lint -restore */                                       \
                }
    #endif

    #if defined(__polyspace__)
        #define FAPI_SYS_PRINT_DEBUG psprintf
    #endif

#else
    #if defined(__arm__) && !defined(__polyspace__)
        #define FAPI_SYS_PRINT_DEBUG(level,...)
    #endif
    #if defined(_ARC) && !defined(__polyspace__)
        #define FAPI_SYS_PRINT_DEBUG(level,varargs...)
    #endif
    #if defined(__polyspace__)
        #define FAPI_SYS_PRINT_DEBUG psprintf
    #endif
#endif




typedef uint32_t FAPI_SYS_SemaphoreT;


#endif /*SYS_SERVICES_H_*/
