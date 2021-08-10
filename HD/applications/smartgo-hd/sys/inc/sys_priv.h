/*!
*****************************************************************************
**
** \file        ./applications/smartgo-hd/sys/inc/sys_priv.h
**
** \brief       Common private data of smartgo-HD engine.
**
** \attention   THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU MICROELECTRONICS
**              ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
**              OMMISSIONS.
**
** (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
** (C) Copyright 2010 by Fujitsu Semiconductor Europe GmbH
**
*****************************************************************************
*/
#ifndef _SYS_PRIV_H_
#define _SYS_PRIV_H_

#if 0

#include <stdio.h>

#include <rtos/rtos.h>

#endif

#include "sys_api.h"

#if 0

#include "psisi.h"

#ifdef APPL_MHEG_ENABLED
#include "mheg.h"
#endif /* APPL_MHEG_ENABLED */

#ifdef APPL_USALS_ENABLED
#include "usals.h"
#endif /* APPL_USALS_ENABLED */


#ifdef APPL_OAD_ENABLED
#include <fota/fota.h>
#endif /* APPL_OAD_ENABLED */
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

#define MAX_SECTOR_SIZE          0x20000    /* largest sector size in bytes */

#define SYS_STRBUF_SIZE                 256
#define SYS_DEVICE_DEVSTR_SIZE          32

/* TSD assignment for each operation */
#define RSRC_TSD_LIVE                   FAPI_TSD0
#define RSRC_TSD_PLAY                   FAPI_TSD0
#define RSRC_TSD_SCAN                   FAPI_TSD0
#define RSRC_TSD_OAD                    FAPI_TSD0
#define RSRC_TSD_MHEG                   FAPI_TSD0
#define RSRC_TSD_REC1                   FAPI_TSD1
#define RSRC_TSD_REC2                   FAPI_TSD2

#define LD16(X) (((X)[0] <<  8 ) |  (X)[1])
#define LD24(X) (((X)[0] << 16 ) | ((X)[1] <<  8 ) |  (X)[2])
#define LD32(X) (((X)[0] << 24UL ) | ((X)[1] << 16 ) | ((X)[2] << 8 ) | (X)[3])
#define LD_LEN(X) (LD16(X) & 0x0FFF)
#define LD_PID(X) (LD16(X) & 0x1FFF)

#include <fapi/drv_timer.h>
#define SYS_GET_TIMESTAMP_MS FAPI_TIMER_GetTimeStamp(FAPI_TIMER_RESOLUTION_1_MSEC)
/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

typedef struct
{
    fbool_t             inUse;
    SYS_OSDLAYERREQ_CB  reqCb;
    RTOS_SemaphoreT     layerMutex;

} SYS_LAYER_DAT_S;

#endif

typedef struct
{
    const char_t*       devName;
    void                (*wakeup)              (SYS_DEVICE_E device);
    void                (*shutdown)            (SYS_DEVICE_E device);
    int32_t             (*preAccess)           (SYS_DEVICE_E device);
    int32_t             (*getPowerMode)        (SYS_DEVICE_E device,
                                                SYS_DEV_PWRMODE_E* pMode);
    void                (*accessStatusChanged) (SYS_DEVICE_E device,
                                                fbool_t accessed);

} SYS_DEV_CFG_S;

typedef struct
{
    uint32_t            sectorCount; /*0*/   //!< Number of sectors on device.
    uint32_t            sectorSize; /*4*/    //!< Size of one sector in bytes.

    char_t              serialNumber[SYS_DEV_SERIALNUM_LEN]; //8
    char_t              firmwareRev [SYS_DEV_FIRMWARE_LEN];
    char_t              modelNumber [SYS_DEV_MODEL_LEN];

} SYS_DEVICE_PARAMS_S;

#if 0

typedef struct
{
    uint8_t*          pBuf;                //!< Pointer to buffer
                                           //!< (Alignment to at least 2 bytes
                                           //!< is mandatory!)
    uint32_t          bufSize;             //!< Size of buffer in bytes.
    uint32_t          flashDeviceType;     //!< Flash device type (parallel/serial)
    uint32_t          flashStartAddr;      //!< Flash start address
    int32_t           flashSize;           //!< Flash size in words.
    int32_t           flashSectors;        //!< Number of available flash sectors.
    uint16_t          flashUsrStartSector; //!< First sector of user area in flash
    SYS_PWRMODE_E     pwrMode; //28            //!< Current power mode.
    SYS_APP_PROFILE_E profile;             //!< Current profile.

    RTOS_SemaphoreT   mutexId;             //!< General purpose mutex.

    /* OSD layer management */
    SYS_LAYER_DAT_S layerDat[OSD_MAX_NUM_LAYER];

    char_t            strBuf[SYS_STRBUF_SIZE]; //!< General ASCII string buffer

} SYS_DATA_S;

typedef struct
{
    void  (*setAbortReq)(void);

} ITC_PARAMS_S;

//#define ITC_DEBUG
typedef struct
{
    ITC_PARAMS_S       params; //0

    RTOS_SemaphoreT    cmdMutex; //4
    RTOS_MailboxT      cmdBox; //8
    RTOS_MailboxT      ackBox; //12
    RTOS_MailboxT      wakeupBox; //16
    uint32_t           osTickPeriod; //20 //!< system tick in ms
    fbool_t            abortReq; //24

    #ifdef ITC_DEBUG
    RTOS_ThreadT       threadRx;
    #endif
    //28
} ITC_INST_S;

typedef struct
{
    RTOS_MailboxT      wakeupBox;
    RTOS_MailboxT      ackBox;
    RTOS_SemaphoreT    mutex;
    fbool_t            sleepMode;
    fbool_t            sleepReq;

} TSC_INST_S;

typedef struct
{
    uint16_t            sectorNum;
    uint32_t            startAddr;
    uint32_t            sizeInWords;

} FLASH_SECT_S;

typedef struct
{
    const FLASH_SECT_S* pSect;
    FAPI_SYS_HandleT    flHdl;
    int32_t             bytePos;    //!< Position in file.

} FLASH_FILE_S;

extern SYS_DATA_S* sysData;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

uint16_t    UTILS_DBVSIText2String (const char_t* pSrc, uint16_t srcLen,
                                    char_t *pDst, uint16_t dstLen,
                                    SYS_CHARSET* pCharSet, fbool_t noEmphasis,
                                    char_t crlfChar);
uint32_t    UTILS_Bcd2Dec          (uint32_t bcd);
void        UTILS_WrU16Masked      (uint8_t* buffer, uint16_t value,
                                    uint16_t mask);
uint16_t    UTILS_RdU16Masked      (const uint8_t* buffer, uint16_t mask);

void        SYS_BootOsdPrint       (const char_t* str);

int32_t     SYS_DeviceInit           (void);
int32_t     SYS_DeviceWakeup         (void);
int32_t     SYS_DeviceShutdown       (void);
fbool_t     SYS_DeviceStatusHandling (const SYS_EVT_DEVSTAT_S* pEvtData);
int32_t     SYS_DeviceRegister       (SYS_DEVICE_E device,
                                      SYS_DEV_CFG_S* cfg);
int32_t     SYS_DeviceSetAvailability(SYS_DEVICE_E device, fbool_t available,
                                      void* devFsHdl,
                                      const SYS_DEVICE_PARAMS_S* pDevParams);
int32_t     SYS_DeviceMount          (SYS_DEVICE_E device, int8_t partIdx);

int32_t     SYS_DeviceCheckPartition (SYS_DEVICE_E device, int8_t partIdx);
void        SYS_DevicePrepareThread  (void);

uint32_t    SYS_DeviceGetDevPath     (SYS_DEVICE_E device, char_t* buf,
                                      uint32_t bufSize);
uint32_t    SYS_DeviceGetPartName    (SYS_DEVICE_E device, int8_t partIdx,
                                      char_t* buf, uint32_t bufSize);
int32_t     SYS_DeviceReadDir(char_t *dirpath);
uint32_t    SYS_DeviceAddSeparator   (char_t* buf, uint32_t bufSize);

int32_t     SYS_ThreadCreate         (RTOS_ThreadFunctionT function, void* arg,
                                      int32_t priority,
                                      uint32_t ssize, const char_t* name,
                                      RTOS_ThreadT* id);

#ifdef APPL_BOOTLDR_ENABLED

int32_t     SYS_GetBootloaderInfo    (void);

#endif /* APPL_BOOTLDR_ENABLED */

void        SYS_I2cMutexWait          (void);
void        SYS_I2cMutexRelease       (void);
void        SYS_UpiDmaMutexWait       (void);
void        SYS_UpiDmaMutexRelease    (void);

int32_t     STR_Init                  (const SYS_CONFIGDAT_S* pSysCfg);

int32_t     ITC_Init                  (ITC_INST_S* inst, ITC_PARAMS_S* params);
void        ITC_Exit                  (ITC_INST_S* inst);
int32_t     ITC_CmdSend               (ITC_INST_S* inst, void * cmd,
                                           void ** ack);
void *      ITC_CmdWait               (ITC_INST_S* inst);
void *      ITC_CmdWaitTimeout        (ITC_INST_S * inst, uint32_t timeout);
fbool_t     ITC_SleepAndCheckAbortion (ITC_INST_S* inst, int32_t timeInMs);
int32_t     ITC_AckSend               (ITC_INST_S* inst, void *ack);

int32_t     TSC_Init                  (TSC_INST_S* inst);
int32_t     TSC_SendThreadToSleep     (TSC_INST_S* inst);
int32_t     TSC_WakeupThread          (TSC_INST_S* inst);
void        TSC_WaitForWakeup         (TSC_INST_S* inst);
fbool_t     TSC_IsSleepRequested      (TSC_INST_S* inst);

int32_t     FLASH_Init                (uint32_t stackSize, uint32_t prio);
int32_t     FLASH_CheckFileSize       (uint32_t fileId, uint32_t requiredSize);

int32_t     TPLIST_Init             (void);
void        TPLIST_Invalidate       (FE_SYSTEM_TYPE_E feType);
void        TPLIST_RestoreDefaults  (FE_SYSTEM_TYPE_E feType,
                                     fbool_t keepUserData);
int32_t     TPLIST_UpdateCarrier    (FE_SYSTEM_TYPE_E feType, PSISI_NIT_TP_INFO_S *pNitTPService, CFGSAT_IDX satIdx);

int32_t     RSRC_Init               (void);
void        RSRC_ConstraintsSetAll  (RSRC_CONSTR_S* pConstr, fbool_t allow);
void        RSRC_SetFeStatus        (int8_t feIdx, fbool_t open,
                                     FE_SYSTEM_TYPE_E type);
int32_t     RSRC_SetLiveService     (const RSRC_CFG_S* pCfg);
int32_t     RSRC_LiveServiceStopped (void);
int32_t     RSRC_SetScan            (const RSRC_CFG_S* pCfg);
int32_t     RSRC_ScanStopped        (int8_t feIdx);
int32_t     RSRC_SetTunemode        (const RSRC_CFG_S* pCfg);
int32_t     RSRC_TunemodeStopped    (int8_t feIdx);

#ifdef APPL_UPDATE_ENABLED

int32_t     RSRC_SetUpdate          (void);
int32_t     RSRC_UpdateStopped      (void);

#endif /* APPL_UPDATE_ENABLED */

#ifdef APPL_HDD_ENABLED

int32_t     HDD_Init                (void);
int32_t     HDD_SetMountDelay       (uint32_t delayInMs);
void        HDD_MediaStatusHandling (SYS_EVT_DEVSTAT_S* pEvent);
int32_t     HDD_Spindown            (void);
void        HDD_Tasks               (void);
void        HDD_RestoreDefaults     (void);

#endif /* APPL_HDD_ENABLED */

#ifdef APPL_PVR_ENABLED

int32_t     RSRC_SetRecord          (const RSRC_CFG_S* pCfg);
int32_t     RSRC_RecordStopped      (uint8_t recIdx);
int32_t     RSRC_SetPlayback        (const RSRC_CFG_S* pCfg);
int32_t     RSRC_PlaybackStopped    (void);

#ifdef APPL_CI_ENABLED

int32_t     RSRC_SetDescrRecord     (const RSRC_CFG_S* pCfg);
int32_t     RSRC_DescrRecordStopped (void);

#endif /* APPL_CI_ENABLED */

#endif /* APPL_PVR_ENABLED */

#ifdef APPL_PIP_ENABLED

int32_t     RSRC_SetPIP             (const RSRC_OP_PIP_S* pParams);
int32_t     RSRC_PIPStopped         (void);

#endif /* APPL_PIP_ENABLED */


int32_t     SLIST_Init              (void);
void        SLIST_Invalidate        (void);
int32_t     SLIST_RestoreDefaults   (void);
void        SLIST_SetFlag           (SLIST_ITEM_S* pItem, uint8_t flag,
                                     fbool_t val);
int32_t     SLIST_SdtUpdatedService (const SLIST_ITEM_S * pItem, SLIST_SERVIDX *srcIdx);
int32_t     SLIST_NitUpdatedService ( PSISI_NIT_TP_INFO_S * pnit_tpinfo,
                                      SYS_NETWORK_S *pnetwork,
                                      PSISI_SDT_S * pSdt);

#ifdef APPL_FESAT_ENABLED

int32_t     CFGSAT_Init             (void);
void        CFGSAT_RestoreDefaults  (fbool_t keepUserData);

#ifdef APPL_USALS_ENABLED

int32_t     CFGSAT_SetUsalsParams   (const USALS_PARAMS_S* params);
int32_t     CFGSAT_GetUsalsParams   (USALS_PARAMS_S* params);

#endif /* APPL_USALS_ENABLED */

#endif /* APPL_FESAT_ENABLED */

#ifdef APPL_FILEOP_ENABLED

int32_t     FILEOP_Init             (void);
void        FILEOP_HandleEvent      (const FILEOP_EVT_S* evt);

#endif /* APPL_FILEOP_ENABLED */

#ifdef APPL_USB_ENABLED

int32_t     USB_Init                (void);
void        USB_Tasks               (void);
void        USB_MediaStatusHandling (SYS_EVT_DEVSTAT_S* pEvent);
void        USB_ConnectStatusHandling(const SYS_EVT_USB_CONNECT_S* pEvent);

#endif /* APPL_USB_ENABLED */

void        CI_DEB_InstallCiDebug(void);

#ifdef APPL_CI_ENABLED
int32_t     CISYS_Init               (const CISYS_INIT_PARAMS_S* ciCfg,
                                      const SYS_HWCONFIG_S* hwCfg);
int32_t     CISYS_Wakeup             (void);
int32_t     CISYS_Shutdown           (void);
int32_t     CISYS_Reset              (RSRC_TS_E ts);
void        CISYS_HandleSlotStatusEvt(const CISYS_EVT_STATUS_S* data);
int32_t     CISYS_GetLiveCfgByPrio   (SLIST_SERVIDX servIdx, RSRC_TS_E tsNeeded,
                                      RSRC_CFG_S* pCfg);
int32_t     CISYS_FreeTsPathForCI    (RSRC_TS_E tsNeeded);

void        CISYS_SetInput           (const RSRC_CFG_S* cfg);
void        CISYS_ServiceSelected    (SLIST_SERVIDX servIdx, uint16_t*); //RSRC_TS_E ts);
void        CISYS_ServiceStop        (SLIST_SERVIDX servIdx, uint16_t*); //RSRC_TS_E ts);
void        CISYS_PmtReceived        (SLIST_SERVIDX servIdx, RSRC_TS_E ts,
                                      const PSISI_PMT_S* pPmt);
void        CISYS_Task               (RSRC_TS_E ts, const PSISI_PMT_S* pPmt);

fbool_t     CISYS_ModuleUseable      (RSRC_TS_E ts,
                                      const SLIST_CA_INFO_S* pCaInfo);
CISYS_SLOTSTATUS_E CISYS_GetSlotStatus(RSRC_TS_E ts);

fbool_t     CISYS_TSPlaybackAvailable(void);

#endif /* APPL_CI_ENABLED */

#ifdef APPL_CA_ENABLED

int32_t     CONAX_CA_Init(void);

#endif /* APPL_CA_ENABLED */

#if defined(APPL_CI_ENABLED) || defined(APPL_CA_ENABLED)

int32_t     SLIST_CaInfoSet          (SLIST_SERVIDX servIdx,
                                      const SLIST_CA_INFO_S* pInfo);
int32_t     SLIST_CaInfoGet          (SLIST_SERVIDX servIdx,
                                      SLIST_CA_INFO_S* pInfo);
int32_t     SLIST_CaInfoExtract      (const PSISI_PMT_S* pPmt,
                                      const SYS_STREAM_INFO_S* strPid,
                                      SLIST_CA_INFO_S* pInfo);
fbool_t     SLIST_MatchStreamPid     (uint16_t pid,
                                      const SYS_STREAM_INFO_S* strPid);

#endif /* defined(APPL_CI_ENABLED) || defined(APPL_CA_ENABLED) */

#ifdef APPL_JPEG_ENABLED

int32_t     JPEG_Init               (const JPEG_INIT_PARAMS_S* params);

#endif /* APPL_JPEG_ENABLED */

#ifdef APPL_MP3_ENABLED

int32_t     MP3_Init                (void);
void        MP3_EventHandling       (const MP3_EVT_STATUS_S* pEvt);

#endif /* APPL_MP3_ENABLED */

#ifdef APPL_PARENTAL_ENABLED

int32_t     PCTRL_Init              (void);
int32_t     PCTRL_Wakeup            (void);
int32_t     PCTRL_Shutdown          (void);
void        PCTRL_LiveStartHandling (const SYS_SERVLOC_S* servLoc,
                                     const SLIST_ITEM_S* service);
void        PCTRL_Stop              (void);
void        PCTRL_EitEventHandling  (SLIST_SERVIDX servIdx);
void        PCTRL_Task              (void);

int32_t     PCTRL_RestoreDefaults   (void);

#ifdef APPL_PVR_ENABLED

void        PCTRL_PlayStartHandling (const PVR_LIST_ITEM_S* pvrItem,
                                     SLIST_SERVIDX servIdx);

#endif /* APPL_PVR_ENABLED */

#endif /* APPL_PARENTAL_ENABLED */

#ifdef APPL_MHEG_ENABLED

typedef struct
{
    SYS_EVTHDR_S        hdr;
    SYS_SERVLOC_S       service;
    uint32_t            ctrlInfo;

} MHEG_EVT_SERVICE_SELECT_S;

typedef struct
{
    SYS_EVTHDR_S                    hdr;

} MHEG_EVT_S;

int32_t        MHEG_Init            (const MHEG_INIT_PARAMS_S* params);
void           MHEG_Exit            (void);
void           MHEG_StateChange     (void* context, redkey_state_t mhegState);

#endif /* APPL_MHEG_ENABLED */

#if ( (defined APPL_MHEG_ENABLED) || (defined APPL_OAD_ENABLED) )

typedef struct
{
    SYS_EVTHDR_S        hdr;

} MHEG_EVT_PMT_REQUEST_S;

typedef struct
{
    SYS_EVTHDR_S        hdr;
    uint8_t*            buf;

} MHEG_EVT_PMT_NOTIFY_S;

typedef struct
{
    SYS_EVTHDR_S        hdr;

} MHEG_EVT_PAT_NOTIFY_S;

typedef struct
{
    SYS_EVTHDR_S      hdr;
    int32_t           index;
    uint32_t          info_ctrl;

} MHEG_EVT_TUNE_S;

#endif /* (defined APPL_MHEG_ENABLED) || (defined APPL_OAD_ENABLED) */

#ifdef APPL_LCN_ENABLED

int32_t         SLIST_LCN_GenerateLists (void);

#endif

#ifdef APPL_CONAX_ENABLED

int32_t     CONAX_CA_Init           (void);
fbool_t     CONAX_CA_CaSystemIdMatch(uint16_t ca_system_id);
void        CONAX_CA_PMTNotify      (uint8_t tsdIdx,
                                     const uint8_t* pSectionBuffer,
                                     uint16_t sectionSize);
void        CONAX_CA_CATSectionNotify (uint8_t tsdIdx,
                                     const uint8_t* pSectionBuffer,
                                     uint16_t sectionSize);

#endif /* APPL_CONAX_ENABLED */

#ifdef APPL_ETHERNET_ENABLED

int32_t             ETH_Init                (void);
int32_t             ETH_Exit                (void);

#endif /* APPL_ETHERNET_ENABLED */

#ifdef APPL_STRING_COMPRESSION_ENABLED
void STRING_HUFMANN_Decompress(uint8_t encoding_type_id,
                               uint8_t *pIn,
                               uint8_t *pout,
                               uint16_t *outlen);
#endif/*#ifdef APPL_STRING_COMPRESSION_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif

#endif /* _SYS_PRIV_H_ */

/******************************************************************************/
/* END OF FILE */
