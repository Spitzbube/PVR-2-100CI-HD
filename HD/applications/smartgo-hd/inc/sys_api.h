/*!
*****************************************************************************
**
** \file        ./applications/smartgo-hd/inc/sys_api.h
**
** \brief       SmartGo-HD system API.
**
**              This file contains all public functions of the SmartGo-HD
**              system library, usable by all SmartGo-HD applications.
**
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
#ifndef _SYS_API_H_
#define _SYS_API_H_

#if 0

#include <draw/draw.h>
#include <osdhandler/osdhandler.h>
#include <fontserver/fontserver.h>
#include <val/val.h>

#include <fapi/drv_auout.h>
#include <fapi/drv_tsd.h>
#include <fapi/drv_videc.h>
#include <fapi/drv_boot.h>
#include <fapi/drv_eth.h>

#include <fapi/board_ifc.h>

#include <fe/fe.h>
#endif

#include "smartgo_types.h"

#include "sys_languages.h"
#if 0
#include "config.h"

#ifdef APPL_BOOTLDR_ENABLED
#include <boot/booterlib.h>
#endif /* APPL_BOOTLDR_ENABLED */

#ifdef APPL_PVR_ENABLED
#include <pvr/pvr.h>
#endif /* APPL_PVR_ENABLED */

#ifdef APPL_JPEG_ENABLED
#include <jpeg/JPEG_api.h>
#endif /* APPL_JPEG_ENABLED */

#ifdef APPL_CI_ENABLED
#include <ci/ci.h>
#endif /* APPL_CI_ENABLED */

#ifdef APPL_CA_ENABLED
#include "fcnx_lib/fcnx_lib.h"
#endif /* APPL_CA_ENABLED */

#ifdef APPL_MHEG_ENABLED
#include <mheg/itk_redkey.h>
#include <mheg/itk.h>
#include <mheg/itk_appmmi.h>
#include <mheg/itk_audioclip.h>
#include <mheg/itk_avstream.h>
#include <mheg/itk_console.h>
#include <mheg/itk_crypto.h>
#include <mheg/itk_demux.h>
#include <mheg/itk_display.h>
#include <mheg/itk_dvr.h>
#include <mheg/itk_fontdriver.h>
#include <mheg/itk_http.h>
#include <mheg/itk_misc.h>
#include <mheg/itk_nvs.h>
#include <mheg/itk_os.h>
#include <mheg/itk_service.h>
#include <mheg/itk_ssu.h>
#include <mheg/itk_zlib.h>
#endif

#ifdef APPL_FATFS_ENABLED
#include <fatfs/api_f.h>
#endif /* APPL_FATFS_ENABLED */

#ifdef APPL_FRONTPANEL_ENABLED
#include <boards/boards_com_fp.h>
#endif

//#define APPL_DESCRREC_ENABLED

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#define OSD_MAX_NUM_LAYER                (OSDHANDLER_MAX_NUM_HW_LAYER)
#define OSDNUM                           1

#define SYS_VERSION_MAJOR                1
#define SYS_VERSION_MINOR                0

#define VIDEOSZ_X_720                    720   //!< Video size horizontal
#define VIDEOSZ_X_1280                  1280   //!< Video size horizontal
#define VIDEOSZ_X_1440                  1440   //!< Video size horizontal
#define VIDEOSZ_X_1920                  1920   //!< Video size horizontal
#define VIDEOSZ_Y_PAL                    576   //!< Video size vertical for PAL
#define VIDEOSZ_Y_NTSC                   480   //!< Video size vertical for NTSC
#define VIDEOSZ_Y_720                    720
#define VIDEOSZ_Y_1080                  1080

#if 0 //16 MB Flash
#define FLASHFS_START_ADDRESS            0x00900000
#else
#define FLASHFS_START_ADDRESS            0x00240000
#endif

//! common ID for all files handled by SmartGo-HD
#define FILEID_SMARTGO                   0x6A720000

/* file IDs */
#define FILEID_SATDAT                    (FILEID_SMARTGO +  1)
#define FILEID_TPLIST_SAT                (FILEID_SMARTGO +  2)
#define FILEID_TPLIST_TER                (FILEID_SMARTGO +  3)
#define FILEID_TPLIST_CAB                (FILEID_SMARTGO +  4)
#define FILEID_PVRLIST                   (FILEID_SMARTGO +  5)
#define FILEID_STARTUPPARAMS             (FILEID_SMARTGO +  6)
#define FILEID_SERVICEDATA               (FILEID_SMARTGO +  7)
#define FILEID_FREQCHGDATA               (FILEID_SMARTGO +  8)
#define FILEID_RECTIMER                  (FILEID_SMARTGO +  9)
#define FILEID_HDDDAT                    (FILEID_SMARTGO + 10)
#define FILEID_PCTRL                     (FILEID_SMARTGO + 11)
#define FILEID_CADATA                    (FILEID_SMARTGO + 12)
#define FILEID_PVRDAT                    (FILEID_SMARTGO + 13)

/* file names */
#define FILENAME_SATDAT                  "Satellite.dat"
#define FILENAME_TPLIST_SAT              "tplist_sat.dat"
#define FILENAME_TPLIST_TER              "tplist_ter.dat"
#define FILENAME_TPLIST_CAB              "tplist_cab.dat"
#define FILENAME_PVRLIST                 "pvrlist.dat"
#define FILENAME_STARTUPPARAMS           "startup.dat"
#define FILENAME_SERVICEDATA             "services.dat"
#define FILENAME_FREQCHGDATA             "fc.dat"
#define FILENAME_RECTIMER                "rectimers.dat"
#define FILENAME_HDDDAT                  "hdd.dat"
#define FILENAME_PCTRL                   "parental_ctrl.dat"
#define FILENAME_CADATA                  "ca.dat"
#define FILENAME_PVRDAT                  "pvrsetts.dat"

/* file revisions */
#define FILEREV_SATDAT                   2//4
#define FILEREV_TPLIST_SAT               2//1
#define FILEREV_TPLIST_TER               1
#define FILEREV_TPLIST_CAB               1
#define FILEREV_PVRLIST                  1
#define FILEREV_STARTUPPARAMS            4//12
#define FILEREV_SERVICEDATA              2//4
#define FILEREV_FREQCHGDATA              2
#define FILEREV_RECTIMER                 2//1
#define FILEREV_HDDDAT                   1
#define FILEREV_PCTRL                    2//1
#define FILEREV_CADATA                   2//1
#define FILEREV_PVRDAT                   1

#define RSRC_FE_MAX         2   //!< Max. number of supported frontends

/* Audio stream types: stored in 6-bit data field */
#define SYS_AUDIO_TYPE_NORMAL           0x01    //!< "undefined" and default.
#define SYS_AUDIO_TYPE_EFFECTS          0x02    //!< Clean effects.
#define SYS_AUDIO_TYPE_HEARING_IMPAIRED 0x04    //!< Hearing impaired.
#define SYS_AUDIO_TYPE_VISUAL_IMPAIRED  0x08    //!< Visual impaired commentary.
#define SYS_AUDIO_TYPE_UNKNOWN          0x10    //!< No audio type available.
#define SYS_AUDIO_TYPE_ALL              0x3F    //!< All audio types.

/* Subtitle stream types: stored in 4-bit data field */
#define SYS_SUBTITLE_TYPE_NORMAL           0x01    //!< "undefined" and default.
#define SYS_SUBTITLE_TYPE_HEARINGIMPAIRED  0x02    //!<
#define SYS_SUBTITLE_TYPE_ALL              0x0F    //!< All audio types.

/* Audio channel preference: stored in 1-bit data field */
#define SYS_AUDIOCHAN_LEFT              0x00    //!< Left audio channel.
#define SYS_AUDIOCHAN_RIGHT             0x01    //!< Right audio channel.
#endif

#define SYS_CHARSET_UNDEFINED   0x00    //!< Unknown character set.
#define SYS_CHARSET_TABLE0      0x01    //!< ISO/IEC-6937 + Euro Symb.(Fig. A.1)
#define SYS_CHARSET_ISO8859_1   0x02    //!< ISO/IEC-8859-1
#define SYS_CHARSET_ISO8859_2   0x03    //!< ISO/IEC-8859-2
#define SYS_CHARSET_ISO8859_3   0x04    //!< ISO/IEC-8859-3
#define SYS_CHARSET_ISO8859_4   0x05    //!< ISO/IEC-8859-4
#define SYS_CHARSET_ISO8859_5   0x06    //!< ISO/IEC-8859-5         (Fig. A.2)
#define SYS_CHARSET_ISO8859_6   0x07    //!< ISO/IEC-8859-6         (Fig. A.3)
#define SYS_CHARSET_ISO8859_7   0x08    //!< ISO/IEC-8859-7         (Fig. A.4)
#define SYS_CHARSET_ISO8859_8   0x09    //!< ISO/IEC-8859-8         (Fig. A.5)
#define SYS_CHARSET_ISO8859_9   0x0A    //!< ISO/IEC-8859-9         (Fig. A.6)
#define SYS_CHARSET_ISO8859_10  0x0B    //!< ISO/IEC-8859-10        (Fig. A.7)
#define SYS_CHARSET_ISO8859_11  0x0C    //!< ISO/IEC-8859-11        (Fig. A.8)
#define SYS_CHARSET_ISO8859_13  0x0D    //!< ISO/IEC-8859-13        (Fig. A.9)
#define SYS_CHARSET_ISO8859_14  0x0E    //!< ISO/IEC-8859-14        (Fig. A.10)
#define SYS_CHARSET_ISO8859_15  0x0F    //!< ISO/IEC-8859-15        (Fig. A.11)
#define SYS_CHARSET_ISO_10646   0x10    //!< ISO/IEC 10646-1 (16 Bit)
                                        //!< (Basic Multilingual Plane)
#define SYS_CHARSET_UTF8        0x11    //!< UTF-8 encoding of ISO/IEC 10646-1
                                        //!< (max. 3 bytes!)
                                        //!< (Basic Multilingual Plane)
#define SYS_CHARSET_NUMS        0x12    //!< Number of available character sets.

#define SYS_CHARSET_ASCII       SYS_CHARSET_ISO8859_1

#if 0

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

#define SYS_SUSPENDSRC_USER             0x01
#define SYS_SUSPENDSRC_LIVE             0x02
#define SYS_SUSPENDSRC_PLAY             0x04
#ifdef APPL_MHEG_ENABLED
#define SYS_SUSPENDSRC_MHEG             0x08
#endif

typedef uint8_t  SYS_SUSPENDSRC;         //!< Suspending source type.
#endif
typedef uint16_t SYS_CHAR;               //!< Character type.
typedef uint8_t  SYS_CHARSET;            //!< Character set type.
typedef int16_t  SLIST_SERVIDX;          //!< Service index type.
#if 0
typedef uint16_t SLIST_TYPE;             //!< Service list type.
typedef int16_t  CFGSAT_IDX;             //!< Satellite index type.
typedef int16_t  TPLIST_IDX;             //!< Carrier index type.
#endif
typedef int16_t  PVRLST_IDX;             //!< PVR recording index type.
typedef int32_t  RECTIMER_IDX;           //!< Record timer index type.
#if 0
typedef uint16_t RSRC_OP;                //!< Resource operation type.
typedef uint8_t  RSRC_TS_IN;             //!< TS input type.
typedef uint8_t  RSRC_CI_IN;             //!< CI input type.


/*----------------------------------------------------------------------------*/
/* Common structures and enums                                                */
/*----------------------------------------------------------------------------*/

typedef enum
{
    SYS_APP_PROFILE_NONE = 0,
    SYS_APP_PROFILE_UKDTT,
    SYS_APP_PROFILE_UKDTT_T2,
    SYS_APP_PROFILE_AUSTRALIA_FREEVIEW,
    SYS_APP_PROFILE_NUMS

} SYS_APP_PROFILE_E;

#endif

/*!
*******************************************************************************
**
** \brief Common time structure.
**
******************************************************************************/
typedef struct
{
    uint8_t             hour;   //!< hour:   0...23
    uint8_t             minute; //!< minute: 0...59
    uint8_t             second; //!< second: 0...59

} SYS_TIME_S;


/*!
*******************************************************************************
**
** \brief Common date structure.
**
******************************************************************************/
typedef enum
{
    SYS_TIME_WEEKDAY_UNKNOWN = 0,
    SYS_TIME_WEEKDAY_MONDAY,
    SYS_TIME_WEEKDAY_TUESDAY,
    SYS_TIME_WEEKDAY_WEDNESDAY,
    SYS_TIME_WEEKDAY_THURSDAY,
    SYS_TIME_WEEKDAY_FRIDAY,
    SYS_TIME_WEEKDAY_SATURDAY,
    SYS_TIME_WEEKDAY_SUNDAY

} SYS_TIME_WEEKDAY_E;

typedef struct
{
    uint16_t            year;       //!< year
    uint8_t             month;      //!< month: 1...12
    uint8_t             day;        //!< day: 1...31
    SYS_TIME_WEEKDAY_E  dayOfWeek;  //!< day of week: Monday (1) ... Sunday (7)

} SYS_DATE_S;

#if 0

typedef enum
{
    PICFORMAT_NONE = -1,    //!< None/invalid format.
    PICFORMAT_4_3  = 0,     //!< Picture/display format: 4:3
    PICFORMAT_16_9,         //!< Picture/display format: 16:9
    PICFORMAT_NUMS          //!< Number of valid formats, internally used.

} SYS_PICFORMAT_E;

typedef enum
{
    HDSYS_576P = 0,
    HDSYS_576I,
    HDSYS_720P,
    HDSYS_1080I,
    HDSYS_1080P,
    //HDSYS_AUTO, //TODO: it should be handled smartly
    HDSYS_NUMS

} SYS_HDTV_STANDARD;

typedef enum
{
    SDSYS_PAL = 0,
    SDSYS_NTSC,
    SDSYS_SECAM,
    SDSYS_PAL_M,
    SDSYS_PAL_N,
    SDSYS_PAL_N_ARG,
    //SDSYS_AUTO_PAL_NTSC, : it should be handled smartly
    SDSYS_NUMS

} SYS_SDTV_STANDARD;

typedef enum
{
    TVSYS_PAL = 0,
    TVSYS_NTSC,
    TVSYS_SECAM,
    TVSYS_PAL_M,
    TVSYS_PAL_N,
    TVSYS_PAL_N_ARG,
    TVSYS_576P,
    TVSYS_576I,
    TVSYS_720P,
    TVSYS_1080I,
    //TVSYS_AUTO, : it should be handled smartly
    TVSYS_NUMS

} SYS_TV_STANDARD;

enum    /* NOTE: The maximum number of audio formats is 8, as it is stored in a
                 three-bit-field. */
{
    SYS_AUDIO_MPEG = 0,
    SYS_AUDIO_AC3,
    SYS_AUDIO_AAC,
    SYS_AUDIO_HEAAC,
    SYS_AUDIO_DDPDCV,

    SYS_AUDIO_NUMS,
    SYS_AUDIO_ALL,              //!< Internally used only, not assigned to
                                //!< format variable!
};

typedef enum
{
    SYS_OWNER_SYSTEM = 0x01,
    SYS_OWNER_USER   = 0x02,
    SYS_OWNER_ALL    = 0x03

} SYS_OWNER_E;

typedef enum
{
    SYS_PRESSTATE_NONE = 0,         //!< No audio/video output.
    SYS_PRESSTATE_LIVETV,           //!< Live TV presentation.
    SYS_PRESSTATE_PLAYBACK,         //!< Playback from file.
    SYS_PRESSTATE_EXTINPUT,         //!< External (analog) input monitor.

} SYS_PRESSTATE_E;

/*!
*******************************************************************************
**
** \brief Service location in service list.
**
******************************************************************************/
typedef struct
{
    SLIST_TYPE          listId; //!< Target list.
    int16_t             index;  //!< Index in target list.

} SYS_SERVLOC_S;

typedef struct
{
    FE_SYSTEM_TYPE_E    type;   //!< delivery system.
    CFGSAT_IDX          satIdx; //!< Satellite index (used for satellite
                                //!< system only).
} SYS_NETWORK_S;

typedef enum
{
    SYS_FEUSAGE_NOTAVAILABLE = 0,   //!< Frontend is not available.
    SYS_FEUSAGE_FREE,               //!< Frontend is currently unused.
    SYS_FEUSAGE_INUSE,              //!< Frontend is currently used.

} SYS_FE_USAGE_E;

typedef enum
{
    SYS_FELOCK_UNUSED = 0,      //!< Frontend has not been tuned yet.
    SYS_FELOCK_NOTLOCKED,       //!< Frontend is not locked.
    SYS_FELOCK_RECOVERYTUNING,  //!< Frontend is being tuned after loss of
                                //!< signal.
    SYS_FELOCK_TUNING,          //!< Frontend is currently being tuned.
    SYS_FELOCK_LOCKED           //!< Frontend is locked.

} SYS_FE_LOCK_E;

typedef struct
{
    SYS_FE_USAGE_E      use;    //!< Usage status.
    SYS_FE_LOCK_E       lock;   //!< Lock status.
    FE_SYSTEM_TYPE_E    type;   //!< Type of delivery system.

} SYS_FE_STATUS_S;

typedef enum                    /* Service select event status definitions */
{
    SERVICE_STAT_START = 0,     //!< Service selection has started
    SERVICE_STAT_SUCCESS,       //!< Service selection successfully completed
    SERVICE_STAT_DECODED,       //!< Service is being decoded.
    SERVICE_STAT_STOPPED        //!< Running service has been stopped

} SYS_SERVICE_STAT_E;

typedef enum                    /* Service running status definitions */
{
    SYS_RUNSTAT_UNKNOWN = 0,    //!< Running status not known yet
    SYS_RUNSTAT_NOTRUNNING,     //!< Service is currently not running.
    SYS_RUNSTAT_RUNNING         //!< Service is currently running.

} SYS_RUNSTAT_E;

typedef enum                    /* Scan mode definitions */
{
    SCAN_MODE_SINGLE = 0,       //!< Single carrier is scanned.
    SCAN_MODE_LIST,             //!< Carrier list is scanned.
    SCAN_MODE_BLIND,            //!< Blind scan is performed.
    SCAN_MODE_CURRENTTS,        //!< Scan on current TS is performed.
    SCAN_MODE_FINISHED          //!< Scan operation finished (internally used)

} SYS_SCAN_MODE_E;

typedef enum                    /* Scan progress event status definitions */
{
    SCAN_STAT_IDLE     = -1,    //!<
    SCAN_STAT_SCAN_HOR = 0,     //!< Horizontal freq. scan (blind scan only)
    SCAN_STAT_SCAN_VER,         //!< Vertical freq. scan (blind scan only)
    SCAN_STAT_TPSCAN,           //!< Carrier scan is in progress
    SCAN_STAT_ABORTED,          //!< Scan has been aborted
    SCAN_STAT_COMPLETED         //!< Scan is completed

} SYS_SCAN_STAT_E;

/*!
*******************************************************************************
**
** \brief Audio stream information.
**
******************************************************************************/
typedef struct
{
    uint16_t        pid:13;       //!< Audio stream PID.
    uint8_t         format:3;     //!< Audio format, one of SYS_AUDIO_XXX.
    SYS_LANGCODE    langCode:24;  //!< ISO 639-2 language code: 3 ASCII-chars.
    uint8_t         type:6;       //!< Audio type, one of SYS_AUDIO_TYPE_XXX.
    uint8_t         channel:1;    //!< Channel preference if stream is dual
                                  //!< channel, one of SYS_AUDIOCHAN_XXX.
    uint8_t         componentTag; //!< component tag form
                                  //!< stream_identifier_descriptor

} SYS_STR_AUDIO_S;

typedef struct
{
    uint8_t             numStreams;                //!< Number of audio streams.
    SYS_STR_AUDIO_S     stream[SYS_AUDIO_STR_MAX]; //!< Audio stream information.

} SYS_STREAMINF_AUDIO_S;

/*!
*******************************************************************************
**
** \brief Video stream information.
**
******************************************************************************/
typedef struct
{
    uint16_t        pid:13;       //!< Audio stream PID.
    uint8_t         format;       //!< stream_type field of video component
    uint8_t         componentTag; //!< component tag form
                                  //!< stream_identifier_descriptor

} SYS_STR_VIDEO_S;

typedef struct
{
    uint8_t             numStreams;                //!< Number of audio streams.
    SYS_STR_VIDEO_S     stream[SYS_VIDEO_STR_MAX]; //!< Audio stream information.

} SYS_STREAMINF_VIDEO_S;

/*!
*******************************************************************************
**
** \brief Service stream information.
**
******************************************************************************/
typedef struct
{
    uint8_t                 valid:1;            //!< 1: PIDs are valid.
    uint16_t                pcr:13;             //!< PCR PID
    SYS_STREAMINF_VIDEO_S   video;              //!< Video stream information.
    SYS_STREAMINF_AUDIO_S   audio;              //!< Audio stream information.
    uint16_t                ttx:13;             //!< TTX PID
    uint8_t                 subttPresent:1;     //!< 1-Bit flag if subtitle is present.
    uint8_t                 mhegPresent:1;     //!< 1-Bit flag if MHEG-5 is present.

} SYS_STREAM_INFO_S;

/*!
*******************************************************************************
**
** \brief Power mode options.
**
******************************************************************************/
typedef enum
{
    SYS_PWRMODE_STARTUP = -1,   //!< System is powering up (internally used).
    SYS_PWRMODE_DOWN    = 0,    //!< System is in standby mode
    SYS_PWRMODE_UP, //1            //!< System is in active mode
    SYS_PWRMODE_NUMS

} SYS_PWRMODE_E;

/*!
*******************************************************************************
**
** \brief SPDIF output preference options.
**
******************************************************************************/
typedef enum
{
    SPDIFMODE_PCMONLY = 0,      //!< Always output PCM samples of currently
                                //!< active I2S audio output.
    SPDIFMODE_PREFERPASSTHROUGH,//!< Output compressed stream if available (e.g.
                                //!< multichannel AC-3). Preference among
                                //!< available multichannel streams is chosen
                                //!< internally.
    SPDIFMODE_NUMS

} SYS_SPDIF_MODE_E;

#endif

/*!
*******************************************************************************
**
** \brief External devices.
**
******************************************************************************/
typedef enum
{
    SYS_DEV_NULL = -1,      //!< Unknown/dummy device.
    SYS_DEV_HDD  = 0,       //!< Internal HDD
    SYS_DEV_USB,            //!< External USB mass storage device
    SYS_DEV_NUMS            //!< Number of known devices
                            //!< (not all of them might be enabled by
                            //!< SW configuration).
} SYS_DEVICE_E;

/*!
*******************************************************************************
**
** \brief Device power mode states.
**
******************************************************************************/
typedef enum
{
    SYS_DEV_PWRMODE_UNKNOWN = -1,       //!< Power mode is unknown.
    SYS_DEV_PWRMODE_STANDBY = 0,        //!< Device is in standby mode.
    SYS_DEV_PWRMODE_ACTIVE,             //!< Device is in active mode
                                        //!< (e.g. HDD is spinned up).
    SYS_DEV_PWRMODE_NUMS

} SYS_DEV_PWRMODE_E;

#if 0

/*!
*******************************************************************************
**
** \brief External device availability definitions.
**
******************************************************************************/
typedef enum
{
    SYS_DEVAVL_NOTAVAILABLE = 0,    //!< Device is currently not connected.
    SYS_DEVAVL_AVAILABLE,           //!< Device is available.
    SYS_DEVAVL_EXT_ACCESS           //!< Device is available but accessed
                                    //!< by a peripheral device. No mounting
                                    //!< is possible in this state.
} SYS_DEV_AVL_E;

/*!
*******************************************************************************
**
** \brief External device mounting status.
**
******************************************************************************/
typedef enum
{
    SYS_DEV_FS_NONE = 0,            //!< No file system has been mounted yet.
    SYS_DEV_FS_ERROR,               //!< FS mount has failed.
    SYS_DEV_FS_FORMATTING,          //!< Device is currently being formatted.
    SYS_DEV_FS_MOUNTING,            //!< FS is currently being mounted.
    SYS_DEV_FS_UNFORMATTED,         //!< FS is not formatted yet.
    SYS_DEV_FS_MOUNTED              //!< FS has been successfully mounted.

} SYS_DEV_FS_E;

typedef struct
{
    SYS_DEV_AVL_E       avlStatus;      //!< Availability status.
    SYS_DEV_FS_E        mountStatus;    //!< Mounting status.
    fbool_t             access;         //!< Access status (only valid if
                                        //!< mountStatus is SYS_DEV_FS_MOUNTED).

} SYS_DEV_STAT_S;

/*!
*******************************************************************************
**
** \brief Common size indication structure.
**
******************************************************************************/
typedef struct
{
    uint32_t            kbytes;
    uint32_t            bytes;

} SYS_SIZE_S;

#endif

/*!
*******************************************************************************
**
** \brief Common string structure.
**
******************************************************************************/
typedef struct
{
    SYS_CHARSET     cs;         //!< Character set/format.
    const char_t*   chr;        //!< Pointer to NULL-terminated character list.

} SYS_STRING_S;


typedef struct
{
    SYS_STRING_S    string;
    char_t*         buf;
    uint32_t        bufSize;
    fbool_t         memAllocated;

} SYS_STRBUF_S;

/* -------------------------------------------------------------------------- */


#ifdef __cplusplus
extern "C"
{
#endif


/*----------------------------------------------------------------------------*/
/* Event handling                                                             */
/*----------------------------------------------------------------------------*/

typedef uint32_t    SYS_EVTTYPE_E;

/*!
*******************************************************************************
**
** \brief Status event definitions.
**
******************************************************************************/
enum
{
    /* Core system messages */
    EVT_SERVICE_SELECTED = 0, //!< A new service has been selected.
    EVT_AUDIOSTREAM_CHANGED,  //!< A new audio stream has been selected.
    EVT_SPDIFSTREAM_CHANGED,  //!< A new SPDIF stream has been selected.
    EVT_IN_SYS_CHANGE,        //!< Input TV Standard has changed.
    EVT_OUT_SYS_CHANGE,       //!< Output TV Standard has changed.
    EVT_PIC_FORMAT_CHANGE,    //!< Input video frame format has changed.
    EVT_LOCK_STATUS,          //!< Frontend lock status has changed.
    EVT_PMT_RECEIVED,         //!< A PMT has been received.
    EVT_SDT_RECEIVED,         //!< A SDT has been received (live-TV only).
    EVT_NIT_RECEIVED,         //!< A NIT has been received (live-TV only).
    EVT_CAT_RECEIVED,         //!< A CAT has been received.
    EVT_TIME_UPDATE/*10*/,          //!< 10 New time and date is sent.
    EVT_TIMEZONE_CHANGE/*11*/,      //!< Timezone has changed (triggered by TOT).
    EVT_TIMESYNC_TIMEOUT,     //!< Requests the timer Menu to set Time manually
    EVT_TIMER_EXPIRED/*15*/,        //!< User timer has expired.
    EVT_SCAN_SERVICE_ADDED,   //!< A service has been found during scan.
    EVT_SCAN_CARRIER_ADDED,   //!< A carrier has been found and added to a list.
    EVT_SCAN_PROGRESS,        //!< Actual scanning progress is reported.
    EVT_EIT_UPDATE,           //!< New EIT current/next event is available.
    EVT_EIT_SCHED_UPDATE,     //!< New EIT schedule section received.
    EVT_SERVICE_RUNSTAT,      //!< Running status of curr. live service changed.
    EVT_SERVICE_NO_DECODING,  //!< 20 Timeout for start of decoding is expired
                              //!< (AV_SERVICE_DECODING_TIMEOUT).
    EVT_HDMI_CONNECTION,      //!< HDMI connection event occured.
    EVT_HDMI_HDCP_STATE,      //!< HDCP authenicate state of HDMI

    /* Configuration-dependant messages */
    EVT_KEY,                  //!< A remote control key has been pressed.

    EVT_PVR_PLAYBACK,         //!< PVR playback status has changed.
    EVT_PVR_RECORD,           //!< PVR record status has changed.
    EVT_PVR_PLAYEVENT,        //!< An event occurred during playback.
    EVT_PVR_RECEVENT,         //!< An event occurred during record.

    EVT_RECTIMER_LISTCHG,     //!< Record timer list has changed: a timer has
                              //!< been removed by system.
    EVT_RECTIMER_STATUS,      //!< Record timer has started or stopped.
    EVT_SLEEPTIMER_EXPIRED,   //!< Sleep timer has expired.

    EVT_FILL_32,
    EVT_FILL_33,
    EVT_FILL_34,
    
    EVT_CISYS_STATUSCHG, //35     //!< 30 Status of a CI slot has changed.
    EVT_CISYS_MMI, //36           //!< An MMI-related event occured.

    EVT_UPDATE_STATUS, //37       //!< Status of software update.

    EVT_FILEOP_STATUS,        //!< Actual file operation status/progress.

    EVT_DEVICE_STATUS,        //!< A device has been connected or removed.

    EVT_JPEG_FILEINFO,        //!< Information on JPEG file being decoded.

    EVT_JPEG_OSDINFO,         //!< information of osd layer for JPEG display
    EVT_JPEG_DECODEINFO,      //!< Information of picture decoding result.
    EVT_MP3_STATUS,           //!< Status of current MP3 playback.

    EVT_AFD_WSS_CHANGE_NOTIFY,   //!< (sys internal)

    EVT_MHEG_STATUS,          //!< 40 State change
    EVT_MHEG_OPEN,           //!< (sys internal)
    EVT_MHEG_CLOSE,           //!< (sys internal)
    EVT_MHEG_START,           //!< (sys internal)
    EVT_MHEG_STOP,            //!< (sys internal)
    EVT_MHEG_SHOW,            //!< (sys internal)
    EVT_MHEG_TUNE,            //!< (sys internal)
    EVT_MHEG_APPMMI_MSG,      //!< (sys internal)

    EVT_PCTRL_NEEDAUTH,       //!< Authorization requested by parental control
    EVT_PCTRL_NOMOREAUTH,     //!< Parental control authorization not needed
                              //!< (anymore).

    EVT_USB_CONNECT_STATUS = 59,   //!< Connection status of a usb device has
                              //!< changed.

    EVT_DEV_FORMAT_FINISH,    //!< Notify the application that the format
                              //!< process is finsihed

    EVT_MOTOR_MOVES,          //!< Notify the application that motor dish is moving.

    EVT_ETH_LINK,
    EVT_ETH_DHCP,
    EVT_ETH_DATA,

    EVT_TABLE_UPDATE,         //!<
    EVT_NUMS                  //!< end of system event area
};

typedef struct
{
    SYS_EVTTYPE_E       type; //0
    uint16_t            length; //4
    //8
} SYS_EVTHDR_S;

#if 0

typedef struct              /* Event: service selected/added */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_SERVLOC_S       servLoc;    //!< Service identification.
    int16_t             audioStr;   //!< audio stream index (only valid in case
                                    //!< of status 'successful'
    SYS_SERVICE_STAT_E  status;     //!< for 'service selected'-events only!
    int32_t             addStat;    //!< for 'service added'-events only
                                    //!< (see SYS_EVT_CARRIERADD_S::status)
    uint32_t            ctrlInfo;   //!< 0: "quiet" service selection mode,
                                    //!< only valid for 'service selected'-events
                                    //!< in status SERVICE_STAT_START.

} SYS_EVT_SERVICE_S;

typedef struct              /* Event: service's running status changed. */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_RUNSTAT_E       status;     //!< New running status.

} SYS_EVT_RUNSTAT_S;

typedef struct              /* Event: carrier added */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    int32_t             status;     //!< Operation status:
                                    //!< FAPI_OK - added to list,
                                    //!< SMARTGO_SYS_ERR_ALREADY_IN_LIST -
                                    //!< already in list,
                                    //!< SMARTGO_SYS_ERR_REPLACED_IN_LIST -
                                    //!< overwritten in list,
    SYS_NETWORK_S       network;    //!< target network
    int16_t             lstIdx;     //!< list index, -1 if not added to list

} SYS_EVT_CARRIERADD_S;

typedef struct              /* Event: EIT event update */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SLIST_SERVIDX       servIdx;    //!< Service index of received EIT event(s).

} SYS_EVT_EITEVT_S;

typedef struct              /* Event: Scan progress */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_SCAN_MODE_E     mode;       //!< Scan mode.
    SYS_SCAN_STAT_E     status;     //!< Scan status.
    uint16_t            tpDone;     //!< Number of carriers scanned.
    uint16_t            tpMax;      //!< Max. number of carriers.
    uint16_t            tpNext;     //!< Index of next carrier to be scanned
                                    //!< (only valid for status
                                    //!< SCAN_STAT_TPSCAN).
    uint16_t            progCurr;   //!< Current progress value (0...progMax).
    uint16_t            progMax;    //!< Maximum progress value.

} SYS_EVT_SCAN_S;

typedef struct              /* Event: time and date update */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_TIME_S          time;       //!< Local time (including local offset).
    SYS_DATE_S          date;       //!< Local date (including local offset).

} SYS_EVT_TIME_S;

typedef struct              /* Event: time and date update */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    int16_t             utcOffset;  //!< Signed UTC offset in minutes

} SYS_EVT_TIMEZONE_S;

typedef struct              /* Event: TV Standard output changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_HDTV_STANDARD   hdTvSys;    //!< new output HDTV standard
    SYS_SDTV_STANDARD   sdTvSys;    //!< new output SDTV standard

} SYS_EVT_TVSYS_S;

typedef struct              /* Event: picture format changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_PICFORMAT_E     format;     //!< new input picture format

} SYS_EVT_PICFORMAT_S;

typedef struct              /* Event: Frontend operation status changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint8_t             feNum;      //!< Frontend index (0...).
    SYS_FE_LOCK_E       status;     //!< New frontend lock status.

} SYS_EVT_LOCKSTAT_S;

typedef struct              /* Event: audio stream changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    int8_t              newIdx;     //!< Index of selected audio stream
                                    //!< (-1: disabled).
    SYS_PRESSTATE_E     state;      //!< Current presentation
                                    //!< (live or playback).

} SYS_EVT_AUDIO_S;

typedef struct              /* Event: SPDIF stream changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    int8_t              newIdx;     //!< Index of selected stream for SPDIF
                                    //!< passthrough (-1: none, current
                                    //!< I2S audio is output as PCM samples.
    SYS_PRESSTATE_E     state;      //!< Current presentation state (live or
                                    //!< playback).

} SYS_EVT_SPDIF_S;

typedef struct              /* Event: Device connected/disconnected */
{
    SYS_EVTHDR_S        hdr;            //!< Common event header.
    SYS_DEVICE_E        device;         //!< Device identification.
    SYS_DEV_STAT_S      status;         //!< Device status.

} SYS_EVT_DEVSTAT_S;

typedef struct              /* Event: PMT has been received */
{
    SYS_EVTHDR_S        hdr;            //!< Common event header.
    uint8_t             tsdIdx;         //!< TSD that filtered PMT.
    RSRC_OP             operation;      //!< Operation that triggered PMT.
    uint16_t            program_number; //!< PMT's program_number field.
    uint8_t             version_number; //!< PMT's version_number field.

} SYS_EVT_PMT_S;

typedef struct              /* Event: CAT has been received */
{
    SYS_EVTHDR_S        hdr;            //!< Common event header.
    uint8_t             tsdIdx;         //!< TSD that filtered CAT.
    uint8_t             version_number; //!< CAT's version_number field.

} SYS_EVT_CAT_S;

typedef struct              /* Event: SDT has been received for live-TV */
{
    SYS_EVTHDR_S        hdr;                //!< Common event header.
    uint8_t             version_number;     //!< PMT's version_number field.
    fbool_t             servicesChanged;    //!< FTRUE if update has caused
                                            //!< changes in service list
                                            //!< (e.g. name change).
} SYS_EVT_SDT_S;

typedef struct              /* Event: NIT has been received for live-TV */
{
    SYS_EVTHDR_S        hdr;                //!< Common event header.
    uint8_t             version_number;     //!< NIT's version_number field.
    fbool_t             servicesChanged;    //!< FTRUE if update has caused
                                            //!< changes in service list
                                            //!< (e.g. name change).
} SYS_EVT_NIT_S;

typedef struct              /* Event: HDMI connection/disconnection occured */
{
    SYS_EVTHDR_S        hdr;                //!< Common event header.
    fbool_t             connected;          //!< HDMI connection status.
    fbool_t             edidAvailable;      //!< EDID availability status.

} SYS_EVT_HDMICONNECT_S;


typedef struct              /* Event: change of HDCP authenticate occurred */
{
    SYS_EVTHDR_S        hdr;                //!< Common event header.
    VAL_HdmiHdcpStateT  state;              //!< HDCP authenticate state of HDMI

} SYS_EVT_HDCPSTATE_S;


#define TABLE_UPDATE_RESET_SERVICE   0x01
#define TABLE_UPDATE_STOP_SERVICE    0x02
#define TABLE_UPDATE_RESTART_SERVICE 0x04
#define TABLE_UPDATE_REMOVE_SERVICE  0x08

typedef struct              /* Event: change of tables occurred */
{
    SYS_EVTHDR_S            hdr;                //!< Common event header.
    uint8_t                 action;             //!< the action of type TABLE_UPDATE_XXX
    uint32_t                param;

} SYS_EVT_TABLEUPDATE_S;

int32_t EVT_Send (const void* pEvent, const char_t* debugString);

/*----------------------------------------------------------------------------*/
/* Key input management                                                       */
/*----------------------------------------------------------------------------*/

#define KEYIN_REP_INTERVAL_FIRST  200   // default first key rep. interval in ms
#define KEYIN_REP_INTERVAL_OTHERS 100   // default other key rep. intervals in ms

typedef enum
{
    KEYIN_KEY_PRESSED = 1,
    KEYIN_KEY_RELEASED,
    KEYIN_KEY_REPEATED

} KEYIN_KEY_TYPE_E;

typedef enum                /* Key event source definitions */
{
    KEY_SOURCE_RC = 0,      //!< Remote control button
    KEY_SOURCE_FRONT,       //!< STB front button
    KEY_SOURCE_CONSOLE      //!< Terminal key input (debug)

} KEYIN_SOURCE_E;

typedef struct              /* Key event structure */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint16_t            val;
    KEYIN_KEY_TYPE_E    type;
    KEYIN_SOURCE_E      source;

} KEYIN_EVT_KEY_S;

int32_t             KEYIN_SetRepeatInterval(uint16_t first,
                                            uint16_t others);
void                KEYIN_GetRepeatInterval(uint16_t* first,
                                            uint16_t* others);

/*----------------------------------------------------------------------------*/
/* Service list management                                                    */
/*----------------------------------------------------------------------------*/

#define SLIST_OFFSET_GLOB       0x0100  //!< Offset for global list identifiers.
#define SLIST_OFFSET_USR             0  //!< Offset for usr list identifiers.
#define SLIST_MASK_GLOB         0x00FF  //!< Mask for global list identifiers.
#define SLIST_NONE              0x02FF

#define SLIST_SERVIDX_NONE         -1   //!< Marks service index as invalid.
#define SLIST_SERVIDX_PLAY         -2   //!< Special service index for PVR
                                        //!< playback.

#define SLIST_CASYSTEMIDS_MAX        8

enum
{
    SLIST_TV = SLIST_OFFSET_GLOB,   //!< Global TV list.
    SLIST_RADIO,                    //!< Global Radio list.
    SLIST_INVISIBLE,                //!< Global List of invisible
                                    //!< services (LCN-specific).
    SLIST_GLOB_MAX,
    SLIST_GLOB_NUMLISTS = (SLIST_GLOB_MAX - SLIST_TV)
};

#define SLIST_NAME_LENGTH            33 //!< Max. service name length including
                                        //!< termination.
#define SLIST_PROVIDER_NAME_LENGTH   33 //!< Max. provider name length including
                                        //!< termination.

/* service item flags */
#define SLIST_FLAG_ALLFREE     0x01     //!< 1: all(!) streams are unencrypted.
#define SLIST_FLAG_SCHEDAVL    0x02     //!< 1: EIT schedule should be available.
#define SLIST_FLAG_CURRNEXTAVL 0x04     //!< 1: EIT present/following should be
                                        //!< available.
#define SLIST_FLAG_VISIBLESVC  0x08     //!< Visible service flag from LCN
                                        //!< descriptor (set by default if LCN is
                                        //!< unavailable).
#define SLIST_FLAG_HIDDEN_UK   0x10     //!< Visibility status from service
                                        //!< attribute descriptor (cleared by
                                        //!< default if information is unavailable).
#define SLIST_FLAG_NONSELECT   0x20     //!< Selectable status from service
                                        //!< attribute descriptor (cleared by
                                        //!< default if information is unavailable).
#ifdef APPL_EPG_ISDBT
#define SLIST_FLAG_LEITAVL      0x40    //!< 1: The L-EIT for the target service is transmitted.
#define SLIST_FLAG_HEITAVL      0x80    //!< 1: The H-EIT for the target service is transmitted.
#endif /* APPL_EPG_ISDBT */

#define SERVICE_TYPE_DIGITAL_TV                  0x01
#define SERVICE_TYPE_DIGITAL_RADIO               0x02
#define SERVICE_TYPE_DIGITAL_RADIO_ADVANCED      0x0A
#define SERVICE_TYPE_DATA_BROADCAST              0x0C
#define SERVICE_TYPE_MHP                         0x10
#define SERVICE_TYPE_DIGITAL_TV_HD               0x11
#define SERVICE_TYPE_DIGITAL_TV_ADVANCED         0x16
#define SERVICE_TYPE_DIGITAL_TV_HD_ADVANCED      0x19

typedef enum
{
    SLIST_SORT_BYINDEX = 0, //!< sort by index in service list database
    SLIST_SORT_BYNAME       //!< sort by service name
    // TODO : add option: sort by "local" language
    // TODO : add option: sort by TS affiliation

} SLIST_SORTMODE_E;

typedef struct
{
    uint8_t             numIds;
    uint16_t            ca_system_id[SLIST_CASYSTEMIDS_MAX];

} SLIST_CA_INFO_S;

/*!
*******************************************************************************
**
** \brief Service list item.
**
******************************************************************************/
#define SLIST_WITH_PROVIDER
typedef struct
{
    SYS_CHARSET         nameCharSet;    //!< Char. set of service name string.
    char_t              name[SLIST_NAME_LENGTH]; //!< service name
    #ifdef SLIST_WITH_PROVIDER
    SYS_CHARSET         providerCharSet;    //!< Char. set of provider name string.
    char_t              provider[SLIST_NAME_LENGTH]; //!< service provider name
    #endif

    uint16_t            serviceId;      //!< service_id.
    SYS_STREAM_INFO_S   strPid;         //!< Stored PIDs.

    SYS_NETWORK_S       network;        //!< Network reference.
    TPLIST_IDX          tpIdx;          //!< Carrier list reference (-1: none).
    uint8_t             flags;          //!< Misc. flags (see SLIST_FLAG_XXX).
    uint8_t             minAge;         //!< Min. viewing age as defined by
                                        //!< user or PCTRL_MINAGE_UNLIMITED or
                                        //!< PCTRL_MINAGE_LOCKED.
    uint16_t            lcn:10;         //!< Logical channel number: (0: undefined)

    uint8_t             renamed:1;      //!< service name is overwritten by user.

    #ifdef APPL_CONTENT_MANAGEMENT_ENABLED
    uint8_t             notScramble:1;  //!< do not scramble
    uint8_t             accessCtrl :2;  //!< control remote access over Internet
    uint8_t             notRevocate:1;  //!< do not apply revocation
    uint8_t             ftaPolicyPresent    :1;  //!< policy presence indicator
    #endif /* APPL_CONTENT_MANAGEMENT_ENABLED */

    uint8_t             serviceType;    //!< Service type: one of SERVICE_TYPE_XXX.

} SLIST_ITEM_S;


/*!
*******************************************************************************
**
** \brief Service list reference.
**
******************************************************************************/
typedef struct
{
    uint8_t             listNum;    //!< index of referenced global list
    int16_t             index;      //!< index in referenced list

} SLIST_REF_S;

typedef struct
{
    /*!
     ** LCN enable status.
     */
    fbool_t             sortByLcn;
    /*!
     ** If sortByLcn is enabled, services are assigned to either "broadcast
     ** range" (valid LCN) oder "variant range" (unassigned, duplicated or
     ** illegal LCN). variantRangeStart marks the start of the variant range
     ** and also declares the last valid LCN to be variantRangeStart - 1.
     **  Example: variantRangeStart = 800, variantRangeSize = 100:
     **
     **  Broadcast range:      1...799
     **  Variant rage:       800...899
     **  Invalid LCNs:    0; 900...
     */
    uint16_t            variantRangeStart;

    /*!
     ** If sortByLcn is enabled: length of variant range.
     */
    uint16_t            variantRangeLength;

    /*!
     ** Determines if visibility flag in LCN descriptor shall be used (i.e.
     ** a special list of invisible services is managed).
     */
    fbool_t             useLcnVisibility;

} SLIST_PARAMS_S;

int32_t         SLIST_SetParams         (const SLIST_PARAMS_S* params);
int32_t         SLIST_GetParams         (SLIST_PARAMS_S* params);
int32_t         SLIST_EntryGet          (const SYS_SERVLOC_S* pServLoc,
                                         SLIST_ITEM_S* pItem);
int32_t         SLIST_EntryAdd          (const SLIST_ITEM_S* pItem,
                                         SYS_SERVLOC_S* pNewServLoc);
int32_t         SLIST_EntryInsert       (const SYS_SERVLOC_S* pServLoc,
                                         SLIST_ITEM_S* pItem,
                                         SLIST_REF_S* ref);
int32_t         SLIST_EntryReplace      (const SYS_SERVLOC_S* pServLoc,
                                         SLIST_ITEM_S* pItem,
                                         SLIST_REF_S* ref);
int32_t         SLIST_EntryRemove       (const SYS_SERVLOC_S* pServLoc,
                                         fbool_t shiftFollowing);
int32_t         SLIST_EntryMove         (SLIST_TYPE listId,
                                         uint16_t indexOld,
                                         uint16_t indexNew);
fbool_t         SLIST_EntryIsInList     (SLIST_TYPE listId,
                                         SLIST_ITEM_S* pItem,
                                        SLIST_REF_S* ref);
int32_t         SLIST_EntryFindInList   (const SYS_SERVLOC_S* pServLoc,
                                         SLIST_TYPE reqListId,
                                         int16_t* pFoundIndex);

uint16_t        SLIST_ListSizeGet       (SLIST_TYPE listId);
SYS_STRING_S    SLIST_ListNameGet       (SLIST_TYPE listId);
int32_t         SLIST_ListNameSet       (SLIST_TYPE listId, SYS_STRING_S name);
fbool_t         SLIST_LoadLists         (void);
int32_t         SLIST_Save              (fbool_t force);
int32_t         SLIST_ClearList         (SLIST_TYPE listId);
int32_t         SLIST_ClearAllLists     (void);
fbool_t         SLIST_FlagIsSet         (const SLIST_ITEM_S* pItem,
                                         uint8_t flag);
int16_t         SLIST_Sort              (SLIST_TYPE listId,
                                         SLIST_SORTMODE_E mode,
                                         fbool_t ascending,
                                         int16_t* pList,
                                         uint16_t numEntries);
SLIST_SERVIDX   SLIST_GetServiceIndex   (const SYS_SERVLOC_S* pServLoc);
SYS_SERVLOC_S   SLIST_GetListReference  (SLIST_SERVIDX servIdx);
int32_t         SLIST_GetServByIndex    (SLIST_SERVIDX servIdx,
                                         SLIST_ITEM_S* pItem);
int32_t         SLIST_SetServNameByIndex(SLIST_SERVIDX index,
                                         SYS_STRING_S str);
int32_t         SLIST_SetServByIndex    (SLIST_SERVIDX servIdx,
                                         const SLIST_ITEM_S* pItem);
#ifdef APPL_EPG_ISDBT

fbool_t         SLIST_GetFlagsByServId(uint16_t serviceId, uint8_t *pFlags);

#endif /* APPL_EPG_ISDBT */

#ifdef APPL_LCN_ENABLED

int32_t         SLIST_GetLcn            (const SYS_SERVLOC_S* pServLoc,
                                         uint16_t* pLcn);
int32_t         SLIST_GetServLocByLcn   (uint16_t lcn,
                                         SLIST_TYPE listId,
                                         SYS_SERVLOC_S* pServLoc);

#endif /* APPL_LCN_ENABLED */

/*----------------------------------------------------------------------------*/
/* AV Management                                                              */
/*----------------------------------------------------------------------------*/

#define AV_VOLUME_STEP          8       //!< volume steps per key press

#define PMT_SECTSIZE_MAX        (uint32_t)1024    //!< Max. size of one PMT section.

typedef enum
{
    SPDIFSTATUS_DISABLED = -1,      //!< No SPDIF output selected.
    SPDIFSTATUS_CURRPCM = 0,        //!< SPDIF outputs PCM of currently decoded
                                    //!< stream.
    SPDIFSTATUS_PASSTHROUGH         //!< SPDIF output is a compressed stream.

} AV_SPDIF_STATUS_E;

typedef enum
{
    AV_STEREO_MODE_STEREO = 0,      //!< Stereo mode.
    AV_STEREO_MODE_MONO_LEFT,       //!< Mono from left channel on both channels.
    AV_STEREO_MODE_MONO_RIGHT,      //!< Mono from right channel on both channels.
    AV_STEREO_MODE_MONO_LEFT_ONLY,  //!< Mono from left channel on left speaker only.
    AV_STEREO_MODE_MONO_RIGHT_ONLY, //!< Mono from right channel on right speaker only.
    AV_STEREO_MODE_NUMS

} AV_STEREO_MODE_E;

/*
    defines for audio Left/Right balance setting
    balance is defined from equal for bot speakers to AV_AUDIO_BALANCE_LEFT_MAX which
    means tha the right speaker is muted or to AV_AUDIO_BALANCE_RIGHT_MAX which means
    that the left speaker is muted.
*/
#define         AV_AUDIO_BALANCE_LEFT_MAX  -100
#define         AV_AUDIO_BALANCE_EQUAL        0
#define         AV_AUDIO_BALANCE_RIGHT_MAX  100

typedef struct
{
    uint32_t            frequency;  //!< Transponder frequency in kHz
    uint16_t            symbol;     //!< Symbol Rate in kSymb/s
    FE_POL_E            pol;        //!< Polarisation
    FE_MOD_E            modulation; //!< modulation type

} TUNE_ITEM_SAT_S;

typedef struct
{
    uint32_t            frequency;  //!< Channel frequency in kHz
    uint16_t            symbol;     //!< Symbol Rate in kSymb/s
    FE_MOD_E            modulation; //!< QAM type

} TUNE_ITEM_CAB_S;

typedef struct
{
    uint32_t            frequency;  //!< Channel frequency in kHz
    FE_BANDWIDTH_E      bandwidth;  //!< Bandwidth
    FE_FEC_E            codeRate;   //!< Code rate

} TUNE_ITEM_TER_S;

typedef struct  /* Playback preference options (live-TV + PVR Playback) */
{
    SYS_LANGCODE        langAudio;  //!< Preferred audio language
                                    //!< (LANG_NOTSPECIFIED if no
                                    //!< preference requested).
    fbool_t             spdifPref;  //!< if FTRUE: spdifMode and langSpdif are
                                    //!< valid.
    SYS_SPDIF_MODE_E    spdifMode;  //!< SPDIF output preference.
                                    //!< - SPDIFMODE_PCMONLY or
                                    //!< - SPDIFMODE_PREFERPASSTHROUGH (choose
                                    //!< compressed stream according to
                                    //!< langSpdif).
    SYS_LANGCODE        langSpdif;  //!< Preferred SPDIF output language
                                    //!< (LANG_NOTSPECIFIED if no
                                    //!< preference requested).

} SYS_PLAYPREF_S;

/*!
*******************************************************************************
**
** \brief generic structure holding all necessary tuning parameters.
**
******************************************************************************/
typedef struct
{
    union
    {
        TUNE_ITEM_SAT_S     sat;
        TUNE_ITEM_CAB_S     cable;
        TUNE_ITEM_TER_S     terr;

    } params;

} TUNE_ITEM_S;

typedef enum
{
    AV_SCANCHOICE_ALL = 0,
    AV_SCANCHOICE_TVONLY,
    AV_SCANCHOICE_RADIOONLY,
    AV_SCANCHOICE_NUMS

} AV_SCANCHOICE_E;

typedef enum
{
    AV_SCANFILTER_ALL = 0,
    AV_SCANFILTER_FREEONLY,
    AV_SCANFILTER_CRYPTEDONLY,
    AV_SCANFILTER_NUMS

} AV_SCANFILTER_E;

/*!
*******************************************************************************
**
** \brief Scan options.
**
******************************************************************************/
typedef struct
{
    fbool_t scanNIT;              //!< FTRUE: search stream also for Network
                                    //!< Information Table (NIT) and add
                                    //!< all new-found carriers to list
    AV_SCANCHOICE_E     choice;     //!< Allows to scan only for Radio or TV services.
    AV_SCANFILTER_E     filter;     //!< Allows to filter scrambled/free services.

} AV_SCAN_OPTS_S;

typedef struct
{
    SYS_SERVICE_STAT_E  status;         //!< Service's selection status.
    int8_t              feIdx;          //!< Index of frontend used for live.
    SYS_RUNSTAT_E       runningStatus;  //!< Service's running status.

} AV_STATUS_LIVE_S;

typedef struct
{
    uint16_t            left;       //!< Left position of picture.
    uint16_t            top;        //!< Top position of picture.
    uint16_t            hsize;      //!< Horizontal size (width).
    uint16_t            vsize;      //!< Vertical size (height).

} AV_VO_PICTURE_WINDOW_S;

typedef enum
{
    AV_WSS_UNKNOWN              =  FAPI_VISCALE_UNKNOWN_WSS_STATE,
    AV_WSS_4_3_FULLFORMAT       =  FAPI_VISCALE_WSS_ENABLE_4_3_FULLFORMAT,
    AV_WSS_14_9_CENTRE          =  FAPI_VISCALE_WSS_ENABLE_14_9_CENTRE,
    AV_WSS_16_9_CENTRE          =  FAPI_VISCALE_WSS_ENABLE_16_9_BOX_CENTRE,
    AV_WSS_16_9_BOX_GT_CENTRE   =  FAPI_VISCALE_WSS_ENABLE_16_9_BOX_GT_CENTRE,
    AV_WSS_14_9_FULLFORMAT      =  FAPI_VISCALE_WSS_ENABLE_14_9_FULLFORMAT,
    AV_WSS_16_9_ANAMORPHIC      =  FAPI_VISCALE_WSS_ENABLE_16_9_ANAMORPHIC,
    AV_WSS_DISABLE              =  FAPI_VISCALE_WSS_DISABLE

} AV_WSS_E;

    #define SERVICE_CTRL_NONE           0x00
    #define SERVICE_CTRL_KEEP_DISPLAY   0x01
    #define SERVICE_CTRL_DISPLAY_BANNER 0x02
    #define SERVICE_CTRL_QUIET          0x04
    #define SERVICE_CTRL_USER_INITIATED 0x10
    #define SERVICE_CTRL_NORMAL         0x13 // SERVICE_CTRL_KEEP_DISPLAY|SERVICE_CTRL_DISPLAY_BANNER|SERVICE_CTRL_USER_INITIATED

typedef struct
{
    FAPI_VIDEC_VESParamsT   stream;
    FAPI_VIDEC_GetStateT    decoder;

} AV_VIDEO_STREAMINFO_S;

typedef struct
{
    FAPI_AUOUT_StreamInfoT  stream;

} AV_AUDIO_STREAMINFO_S;

/* ----- live-TV options --------------------------------------------------- */

int32_t             AV_GetLiveStatus        (AV_STATUS_LIVE_S* status);

int32_t             AV_StartInitialService  (void);
int32_t             AV_SelectService        (const SYS_SERVLOC_S* pServLoc,
                                             const SYS_PLAYPREF_S* pPref,
                                             uint32_t ctrlInfo,
                                             fbool_t forceSelection);
int32_t             AV_SwapService          (void);
int32_t             AV_Stop                 (void);
int32_t             AV_Freeze               (void);
int32_t             AV_SelectNextService    (void);
int32_t             AV_SelectPreviousService(void);
int32_t             AV_ResetService         (void);
int32_t             AV_RestartService       (void);

int32_t             AV_GetRequestedService  (SYS_SERVLOC_S* pServLoc);
int32_t             AV_GetQuietRequestedService(SYS_SERVLOC_S* pServLoc);
int32_t             AV_GetPrevService       (SYS_SERVLOC_S* pServLoc);
int32_t             AV_GetPrevServiceByList (SLIST_TYPE listId,
                                             int16_t* index);

int32_t             AV_GetAudioStream       (int8_t* index);
int32_t             AV_SetAudioStream       (int8_t index);
int32_t             AV_SetSPDIFStatus       (AV_SPDIF_STATUS_E type,
                                             int8_t index);
int32_t             AV_UpdateAudioSelect    (void);
int32_t             AV_GetSPDIFStatus       (AV_SPDIF_STATUS_E* type,
                                             int8_t* index);

int32_t             AV_VolumeGet            (uint8_t* lvlLeft,
                                             uint8_t* lvlRight,
                                             uint8_t* lvlMax,
                                             fbool_t* isMuted);
int32_t             AV_VolumeSet            (uint8_t lvlLeft, uint8_t lvlRight);
int32_t             AV_Mute                 (fbool_t onOffFlag);
int32_t             AV_VolumeGetStereoMode  (AV_STEREO_MODE_E* mode);
int32_t             AV_VolumeSetStereoMode  (AV_STEREO_MODE_E mode);

int32_t             AV_VolumeGetBalance     (int8_t* balance);
int32_t             AV_VolumeSetBalance     (int8_t balance);

int32_t             AV_OutputTvStandardCurrGet(
                                            SYS_HDTV_STANDARD* hdTvStandard,
                                            SYS_SDTV_STANDARD* sdTvStandard);
int32_t             AV_InputTvStandardCurrGet(SYS_TV_STANDARD* sdTvStandard);
int32_t             AV_PicFormatCurrGet     (SYS_PICFORMAT_E* picFormat);

int32_t             AV_IFrameShow           (const uint32_t* data,
                                             uint32_t sizeInBytes,
                                             uint32_t format);
int32_t             AV_IFrameHide           (void);

/* ----- scan options ------------------------------------------------------ */

int32_t             AV_ScanSingleCarrier(const SYS_NETWORK_S* network,
                                         uint16_t tpLstPos,
                                         const AV_SCAN_OPTS_S* opts,
                                         int8_t* feIdx);
int32_t             AV_ScanCarrierList  (const SYS_NETWORK_S* network,
                                         const AV_SCAN_OPTS_S* opts,
                                         int8_t* feIdx);
int32_t             AV_ScanBlind        (const SYS_NETWORK_S* network,
                                         const AV_SCAN_OPTS_S* opts,
                                         int8_t* feIdx);
int32_t             AV_ScanStop         (int8_t feIdx);

/* ----- Misc. options ----------------------------------------------------- */

int32_t             AV_AudioFormatIsDecodable(uint8_t format, fbool_t* pResult);
int32_t             AV_AudioFormatIsSpdifCompliant(uint8_t format,
                                                   fbool_t* pResult);

int32_t             AV_GetVideoStreamInfo(AV_VIDEO_STREAMINFO_S* info);
int32_t             AV_GetVideoFrameInfo (FAPI_VIDEC_VideoDecInfoStateT* info);
int32_t             AV_GetAudioStreamInfo(AV_AUDIO_STREAMINFO_S* info);

int32_t             AV_HDMIGetEDIDLipSyncDelay(int32_t* delay);
int32_t             AV_HDMISetLipSyncDelay(int32_t delay);
int32_t             AV_HDMIAutoSync(void);

int32_t             AV_PowerupScreenHide(void);
int32_t             AV_TuneModeStart(int8_t feIdx,
                                     const SYS_NETWORK_S* network,
                                     int16_t tpLstPos,
                                     const TUNE_ITEM_S* tunePars,
                                     int8_t* selFeIdx);
int32_t             AV_TuneModeStartByCarrier(int8_t feIdx,
                                              const SYS_NETWORK_S* network,
                                              TPLIST_IDX tpIdx,
                                              int8_t* selFeIdx);
int32_t             AV_TuneModeStop(int8_t feIdx);

int32_t             AV_GetSignalStrength(int8_t feIdx, uint16_t* levelCurr,
                                         uint16_t* lvlMax);
int32_t             AV_GetSignalQuality(int8_t feIdx, uint16_t* levelCurr,
                                        uint16_t* lvlMax);
int32_t             AV_GetFeStatus(int8_t feIdx, SYS_FE_STATUS_S* status);

int32_t             AV_SetVideoArea(fbool_t fullScreen,
                                    const AV_VO_PICTURE_WINDOW_S* pos);
int32_t             AV_GetVideoArea(AV_VO_PICTURE_WINDOW_S* pos,
                                    AV_VO_PICTURE_WINDOW_S* realPos);
int32_t             AV_ZoomVideo(fbool_t full,
                                 const AV_VO_PICTURE_WINDOW_S* pos);
int32_t             AV_FreezeVideo(fbool_t freezeOn);
int32_t             AV_Nop(void);
void                AV_EnableOutputFormatUpdate(fbool_t enable);
// TODO: this interface is to be replaced!
FAPI_SYS_HandleT    AV_GetViencHdHandle(void);
int32_t             AV_SetSubttStream(int8_t index, fbool_t isDvb);
int32_t             AV_UpdateSubttSelect(void);

#ifdef APPL_FESAT_ENABLED

int32_t             AV_SetAutoMotorMode(int8_t feIdx, fbool_t enable);

#endif /* APPL_FESAT_ENABLED */

/*---------------------------------------------------------------------------*/
/* Screen Capture API                                                        */
/*---------------------------------------------------------------------------*/

/* We want to keep this structure even when screen capture is not enabled
   to keep the setup data in flash consistent among all applications. */
typedef enum /* screen capture mode */
{
        //! Capture the video only.
        SCREENCAPTURE_VIDEO_ONLY = 0,
        //! Capture the video together with the OSD.
        SCREENCAPTURE_VIDEO_OSD,
        SCREENCAPTURE_NUMS

} SCREENCAPTURE_MODE_E;

#ifdef APPL_SCRNSHOT_ENABLE

int32_t SYS_CaptureScreen  (SYS_DEVICE_E device, const char_t* path,
                            SCREENCAPTURE_MODE_E captureMode,
                            SYS_SDTV_STANDARD sdSystem,
                            void (*afterCaptureCb)(void* arg),void* arg);

#endif /* APPL_SCRNSHOT_ENABLE */

/*---------------------------------------------------------------------------*/
/* Content Management API                                                    */
/*---------------------------------------------------------------------------*/
#ifdef APPL_CONTENT_MANAGEMENT_ENABLED
typedef struct
{
    uint8_t notScramble;
    uint8_t accessCtrl;
    uint8_t notRevocate;
    uint8_t present;

} CMGMT_FTA_POLICY_S;

int32_t CMGMT_GetFtaPolicy(CMGMT_FTA_POLICY_S* policy);
#endif /* APPL_CONTENT_MANAGEMENT_ENABLED */

/*---------------------------------------------------------------------------*/
/* PCM sample input management                                               */
/*---------------------------------------------------------------------------*/

#define PCM_BYTESPERSAMPLE   4

typedef struct
{
    FAPI_AUOUT_SampleFreqEnumT sampleFrequency;

} PCM_CONFIG_S;

typedef struct
{
    uint32_t            size;       //!< Buffer size in PCM sample pairs.
    uint32_t            spaceAvl;   //!< Free space in PCM sample pairs, starting at wrPos.
    uint32_t            baseAddr;   //!< Buffer base address (byte address).
    uint32_t            wrPos;      //!< Buffer write position relative to baseAddr.
    uint32_t            rdPos;      //!< Buffer read position relative to baseAddr.

} PCM_BUF_PARAMS_S;

int32_t             PCM_Open             (void);
int32_t             PCM_Close            (void);
int32_t             PCM_Pause            (fbool_t pauseOn);
int32_t             PCM_OutputStart      (const PCM_CONFIG_S* cfg);
int32_t             PCM_OutputStop       (void);
int32_t             PCM_GetBufferParams  (PCM_BUF_PARAMS_S* pParams);
int32_t             PCM_ResizeBuffer     (void);
int32_t             PCM_ResetBuffer      (void);
uint32_t            PCM_FeedData         (const uint32_t* srcBuf,
                                          uint32_t numSamples, fbool_t is16Bit,
                                          fbool_t isStereo);
int32_t             PCM_SetWritePosition (uint32_t pos);


/*---------------------------------------------------------------------------*/
/* EIT Management                                                            */
/*---------------------------------------------------------------------------*/

#define PSISI_RUNSTAT_UNDEFINED    0x00 //!< Running status is undefined.
#define PSISI_RUNSTAT_NOTRUNNING   0x01 //!< Event is not running.
#define PSISI_RUNSTAT_STARTSOON    0x02 //!< Event will start in a few seconds.
#define PSISI_RUNSTAT_PAUSING      0x03 //!< Event is pausing.
#define PSISI_RUNSTAT_RUNNING      0x04 //!< Event is running.

typedef enum
{
    EIT_TYPE_CURR = 0,
    EIT_TYPE_NEXT,
    EIT_TYPE_NUMS
} EIT_EVT_TYPE_E;

typedef enum
{
    EIT_FILTERMODE_OFF = 0,         //!< No filtering.
    EIT_FILTERMODE_THISSERVICE,     //!< Only EIT of curr. service is filtered.
    EIT_FILTERMODE_THISTS,          //!< All EIT for current TS is filtered.
    EIT_FILTERMODE_ALL              //!< All avl. EIT information is filtered.

} EIT_FILTERMODE_E;

/*!
*******************************************************************************
**
** \brief EIT event data.
**
******************************************************************************/
typedef struct
{
    uint16_t            id;                 //!< event_id.
    SYS_CHARSET         nameCharSet;
    char_t              name[AV_EIT_NAMELEN_MAX];
    SYS_CHARSET         descCharSet;
    char_t              desc[AV_EIT_DESCLEN_MAX];

    SYS_TIME_S          startTime;          //!< Event start time (UTC).
    SYS_DATE_S          startDate;          //!< Event start data (UTC).
    SYS_TIME_S          duration;           //!< Event duration.
    uint8_t             minAge;             //!< Min. viewing age.
                                            //!< (0: information not available).
    uint8_t             content;            //!< event Content.
                                            //!< (0: information not available).
#ifdef APPL_CONTENT_MANAGEMENT_ENABLED
    CMGMT_FTA_POLICY_S  ftaPolicy;          //!< event Content management policy
#endif /* APPL_CONTENT_MANAGEMENT_ENABLED */
    uint8_t             runningStatus;      //!< Running status (one of
                                            //!< PSISI_RUNSTAT_XXX).
} EIT_EVT_S;

/*!
*******************************************************************************
**
** \brief EIT extended information data.
**
******************************************************************************/
typedef struct
{
    char_t*             pBuf;       //!< Pointer to target buffer.
    uint32_t            maxLen;     //!< Size of target buffer.

    uint32_t            descStrLen; //!< String length in bytes.
    SYS_STRING_S        descStr;    //!< Extended event string.

} EIT_EXTEVT_S;

/*!
*******************************************************************************
**
** \brief EIT schedule database iterator (internally used).
**
******************************************************************************/
typedef struct
{
    SLIST_SERVIDX       servIdx;
    void*               pItem;

} EIT_POS_S;

typedef struct
{
    EIT_FILTERMODE_E    modeCurrNext;       //!< Filter mode for curr/next events.
    EIT_FILTERMODE_E    modeSchedule;       //!< Filter mode for schedule events.

} EIT_PARAMS_S;

int32_t             EIT_SetParams(const EIT_PARAMS_S* pParams);
int32_t             EIT_GetParams(EIT_PARAMS_S* pParams);
int32_t             EIT_ClearDatabase(void);

int32_t             EIT_GetEvent(const SYS_SERVLOC_S* pServLoc,
                                 EIT_EVT_TYPE_E type, EIT_EVT_S* pEvent);
int32_t             EIT_GetExtEvt(const SYS_SERVLOC_S* pServLoc,
                                  EIT_EVT_TYPE_E type, EIT_EXTEVT_S* pExt);

uint32_t            EIT_GetSchedListSize(const SYS_SERVLOC_S* pServLoc);

int32_t             EIT_GetSchedByDate(const SYS_SERVLOC_S* pServLoc,
                                       EIT_EVT_S* pEvt,
                                       EIT_EXTEVT_S* pExt,
                                       EIT_POS_S* pRef,
                                       SYS_TIME_S* startTime,
                                       SYS_DATE_S* startDate,
                                       fbool_t matchStartTime);

int32_t             EIT_GetSchedByIndex(const SYS_SERVLOC_S* pServLoc,
                                        EIT_EVT_S* pEvt,
                                        EIT_EXTEVT_S* pExt,
                                        EIT_POS_S* pRef, uint32_t index);

int32_t             EIT_GetSchedByReference(int16_t offset,
                                            EIT_EVT_S* pEvt,
                                            EIT_EXTEVT_S* pExt,
                                            EIT_POS_S* pRef);

int32_t             EIT_GetSchedNext(EIT_EVT_S* pEvt,
                                     EIT_EXTEVT_S* pExt, EIT_POS_S* pRef);

int32_t             EIT_GetSchedPrevious(EIT_EVT_S* pEvt,
                                         EIT_EXTEVT_S* pExt,
                                         EIT_POS_S* pRef);

void                EIT_CleanupSched(void);

int32_t             EIT_AddFocusCurrNext(const SYS_SERVLOC_S* pServLoc);
int32_t             EIT_AddFocusSched(const SYS_SERVLOC_S* pServLoc);
int32_t             EIT_RemoveFocusCurrNextAll(void);
int32_t             EIT_RemoveFocusSchedAll(void);


/*!
*******************************************************************************
**
** \brief general IP information structure.
**
******************************************************************************/
typedef struct
{
    fbool_t         useDhcp;
    FAPI_ETH_Ip4T   ipAddr;
    FAPI_ETH_Ip4T   netMask;
    FAPI_ETH_Ip4T   gateway;

} ETH_IP_S;


/*!
*******************************************************************************
**
** \brief general Domain Name Server address ( DNS ) information structure.
**
******************************************************************************/
typedef struct
{
    fbool_t         autoDns;
    FAPI_ETH_Ip4T   dns[2];

} ETH_DNS_S;

#ifdef APPL_ETHERNET_ENABLED

/**
 * Response types.
 */
typedef enum
{
    ETH_HTTP_HEADER,                /**< indicates the callback data is a response header */
    ETH_HTTP_CONTENT,               /**< indicates the callback data is response content */
    ETH_FTP_HEADER,                /**< indicates the callback data is a response header */
    ETH_FTP_CONTENT,               /**< indicates the callback data is response content */
    ETH_UDP_CONTENT,               /**< indicates the callback data is response content */
    ETH_TCP_CONTENT,               /**< indicates the callback data is response content */
    ETH_END,                   /**< indicates the response has completed */
    ETH_ERROR,                 /**< indicates the request has failed for internal reasons */
    ETH_ERROR_NETWORK,         /**< indicates the request failed because of network issues */
    ETH_ERROR_TLS              /**< indicates the https request failed because of TLS certificate issues */
} eth_response_t;

typedef struct
{
    FAPI_ETH_LinkEnumT      linkStatus;     //!< link is up or not
    FAPI_ETH_SpeedEnumT     linkSpeed;      //!< speed setting
    FAPI_ETH_DuplexEnumT    linkDuplex;     //!< duplex mode setting

    ETH_IP_S                ip;
    ETH_DNS_S               dns;
    FAPI_ETH_MacT           mac;

} ETH_INFO_S;


typedef struct              /* Event: service's running status changed. */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    FAPI_SYS_HandleT    request;
    eth_response_t      type;
    uint8_t             *data;
    size_t              dataLength;

} ETH_EVT_S;

#define ETH_TYPE_REQ 0
#define ETH_TYPE_UDP 1
#define ETH_TYPE_TCP 2

/**
 * Response callback function.
 * A function of this type is registered when a request is created with newRequest.
 * The function is invoked when a server response is received.  The function parameter type
 * identifies what data is being signalled.
 * If the type is HTTP_HEADER the data parameter points to the header data and dataLength
 * is the length of the header.
 * If the type is HTTP_CONTENT the data parameter points to the returned content data and
 * dataLength is the length of the content.  This can be file data for a GET request or server
 * data for a POST request.  If a response is chunked there can be several HTTP_CONTENT responses.
 * If the type is HTTP_END the response has been processed.  This type should always be sent
 * when that the request has been fully processed.  It can indicate the end of data transfer
 * for a GET request or is sent after the last header of a HEAD request.  The data pointer
 * will be NULL and dataLength zero.
 * If the type is HTTP_ERROR an internal error has occurred and the request has failed.
 * @param[in] context       The application context passed when the request was created.
 * @param[in] request       A pointer to the request object.
 * @param[in] type          The response type, HTTP_HEADER, HTTP_CONTENT, HTTP_END or HTTP_ERROR.
 * @param[in] data          The response data.
 * @param[in] dataLength    The response data length in bytes.
 */
typedef size_t (*eth_callback_t)(FAPI_SYS_HandleT request,
                        eth_response_t type, void *data, size_t dataLength);

typedef struct
{
    uint32_t        numReq;

} reqOpen_t;

typedef struct
{
    eth_callback_t  callback;

    uint32_t        port;
    FAPI_ETH_Ip4T   ipAddr;

    size_t          blockSize;

} udpOpen_t;

typedef struct
{
    eth_callback_t  callback;

    uint32_t        port;
    FAPI_ETH_Ip4T   ipAddr;

    size_t          blockSize;

} tcpOpen_t;

typedef struct
{
    uint8_t type;

    reqOpen_t   req;
    udpOpen_t   udp;
    tcpOpen_t   tcp;

}ethOpen_t;

FAPI_SYS_HandleT    ETH_Open                (ethOpen_t * openParams, int32_t* err);
int32_t             ETH_Close               (FAPI_SYS_HandleT handle);
int32_t ETH_GetInfo (ETH_INFO_S* info);
int32_t ETH_GetIp   (ETH_IP_S* ip);
int32_t ETH_SetIp   (ETH_IP_S* ip);
int32_t ETH_GetDns  (ETH_DNS_S* dns);
int32_t ETH_SetDns  (ETH_DNS_S* dns);


FAPI_ETH_LinkEnumT ETH_GetStatus(void);
FAPI_SYS_HandleT ETH_NewRequest(FAPI_SYS_HandleT handle,
                                const char_t *method,
                                const char_t *uri,
                                eth_callback_t callback,
                                void *context);
void ETH_ResetRequest(FAPI_SYS_HandleT handle,
                      const char_t *uri);
int32_t ETH_SendRequest(FAPI_SYS_HandleT handle,
                        void *content,
                        size_t contentLength);
int32_t ETH_AbortRequest(FAPI_SYS_HandleT handle);
void ETH_AddHeader(FAPI_SYS_HandleT handle,
                   const char_t *header);
void ETH_SetRequestTimeout(FAPI_SYS_HandleT handle, uint32_t timeout);
void ETH_ReleaseRequest(FAPI_SYS_HandleT handle);
void ETH_HandleDataEvt(ETH_EVT_S* data);

#endif
/*----------------------------------------------------------------------------*/
/* Settings API                                                               */
/*----------------------------------------------------------------------------*/

#define SETTINGS_UTC_OFFSET_MIN   -720  //!< Min. UTC offset in minutes.[-12h]
#define SETTINGS_UTC_OFFSET_MAX   840   //!< Max. UTC offset in minutes [+14h].

#define SETTINGS_DISPTIME_MIN    1
#define SETTINGS_DISPTIME_MAX    30

#define SETTINGS_USERDAT_MAX         4  //!< number of user data entries available
#define SETTINGS_AUDIOLANGPREF_MAX   2  //!< Size of audio language preference list
#define SETTINGS_UILANGPREF_MAX      2  //!< Size of user interface language preference list
#define SETTINGS_SUBTLANGPREF_MAX    2  //!< Size of subtitle language preference list

typedef enum                            /* Video output on SCART connector */
{
    VIDEOOUT_UNKNOWN = -1,              //!< Internally used!
    VIDEOOUT_RGB = 0,                   //!< Scart output: RGC
    //VIDEOOUT_CVBS,                    //!< Scart output: CVBS
    VIDEOOUT_SVIDEO,                    //!< Scart output: Y/C signal (S-Video)
    VIDEOOUT_NUMS

} SYS_VIDEO_OUT;

typedef enum                            /* Scaling method for 16:9 video */
{
    SCALEMETHOD_NONE = -1,              //!< for user defined active area
    SCALEMETHOD_PILLAR_LETTERBOX = 0,   //!< for Plillar/Letterboxing
    SCALEMETHOD_FULLSCREEN,             //!< for zooming to fullscreen
    SCALEMETHOD_CCO,                    //!< for center-cutout
    SCALEMETHOD_TV_SCALES,              //!< use TVs scaling feature
    SCALEMETHOD_NUMS

} SYS_SCALEMETHOD_E;

typedef enum                            /* Video start preference */
{
    VIDEOSTART_FAST = 0,
    VIDEOSTART_SYNC,
    VIDEOSTART_NUMS

} SYS_VIDSTART_MODE_E;

typedef enum                /* lip sync preference*/
{
    AV_LIPSYNC_MODE_AUTO = 0,
    AV_LIPSYNC_MODE_LEVEL,
    AV_LIPSYNC_MODE_NUMS
} AV_LIPSYNC_MODE_E;

fbool_t             SETTINGS_SavingIsNecessary  (void);
int32_t             SETTINGS_Save               (void);

int32_t             SETTINGS_AutoTimeSet        (fbool_t enable);
int32_t             SETTINGS_AutoTimeGet        (fbool_t* enabled);
int32_t             SETTINGS_AutoTimeOffsetSet  (fbool_t enable,
                                                 SYS_CTRYCODE country);
int32_t             SETTINGS_AutoTimeOffsetGet  (fbool_t* enabled,
                                                 SYS_CTRYCODE* country);
int32_t             SETTINGS_TimeOffsetGet      (int16_t* utcOffset,
                                                 fbool_t* dstIsActive);
int32_t             SETTINGS_TimeOffsetSet      (int16_t utcOffset,
                                                 fbool_t dstIsActive);
int32_t             SETTINGS_DisplayTimeGet     (uint16_t* timeInSeconds);
int32_t             SETTINGS_DisplayTimeSet     (uint16_t timeInSeconds);
int32_t             SETTINGS_DispFormatGet      (SYS_PICFORMAT_E* format,
                                                 SYS_SCALEMETHOD_E*
                                                 scaleMethod);
int32_t             SETTINGS_DispFormatSet      (SYS_PICFORMAT_E format,
                                                 SYS_SCALEMETHOD_E scaleMethod);
int32_t             SETTINGS_VideoOutputGet     (SYS_VIDEO_OUT* videoOut);
int32_t             SETTINGS_VideoOutputSet     (SYS_VIDEO_OUT videoOut);
int32_t             SETTINGS_OutputTvStandardGet(SYS_HDTV_STANDARD* hdSys,
                                                 SYS_SDTV_STANDARD* sdSys);
int32_t             SETTINGS_OutputTvStandardSet(SYS_HDTV_STANDARD hdSys,
                                                 SYS_SDTV_STANDARD sdSys);
int32_t             SETTINGS_UserDatGet         (uint8_t index, uint32_t* data);
int32_t             SETTINGS_UserDatSet         (uint8_t index, uint32_t data);
int32_t             SETTINGS_DefaultSPDIFModeSet(SYS_SPDIF_MODE_E spdifMode);
int32_t             SETTINGS_DefaultSPDIFModeGet(SYS_SPDIF_MODE_E* spdifMode);
int32_t             SETTINGS_SetServiceStartMode(SYS_VIDSTART_MODE_E reqMode);
int32_t             SETTINGS_GetServiceStartMode(SYS_VIDSTART_MODE_E*
                                                 startMode);
int32_t             SETTINGS_SetServiceSwitchMode(fbool_t freeze);
int32_t             SETTINGS_GetServiceSwitchMode(fbool_t* freeze);
int32_t             SETTINGS_SetPowerUpMode     (SYS_PWRMODE_E reqMode);
int32_t             SETTINGS_GetPowerUpMode     (SYS_PWRMODE_E* powerupMode);
#endif
int32_t             SETTINGS_OsdLangGet         (SYS_LANGCODE* lang);
int32_t             SETTINGS_OsdLangSet         (SYS_LANGCODE  lang);

#if 0
int32_t             SETTINGS_AudioLangPrefSet   (uint8_t prefIdx,
                                                 SYS_LANGCODE lang);
int32_t             SETTINGS_AudioLangPrefGet   (uint8_t prefIdx,
                                                 SYS_LANGCODE* lang);
int32_t             SETTINGS_SubtLangPrefSet    (uint8_t prefIdx,
                                                 SYS_LANGCODE lang);
int32_t             SETTINGS_SubtLangPrefGet    (uint8_t prefIdx,
                                                 SYS_LANGCODE* lang);
int32_t             SETTINGS_UILangPrefSet      (uint8_t prefIdx,
                                                 SYS_LANGCODE lang);
int32_t             SETTINGS_UILangPrefGet      (uint8_t prefIdx,
                                                 SYS_LANGCODE* lang);

int32_t             SETTINGS_AntennaPowerGet    (int8_t feIdx,
                                                 fbool_t* powerOn);
int32_t             SETTINGS_AntennaPowerSet    (int8_t feIdx, fbool_t powerOn);

int32_t             SETTINGS_StoreFreqChgData   (void);

int32_t             SETTINGS_PlayJingleGet      (fbool_t* jingleEnabled);
int32_t             SETTINGS_PlayJingleSet      (fbool_t jingleEnabled);

#ifdef APPL_SCRNSHOT_ENABLE

int32_t             SETTINGS_ScreenShotModeSet  (SCREENCAPTURE_MODE_E  mode);
int32_t             SETTINGS_ScreenShotModeGet  (SCREENCAPTURE_MODE_E* mode);
int32_t             SETTINGS_ScreenShotToSet    (SYS_DEVICE_E  device);
int32_t             SETTINGS_ScreenShotToGet    (SYS_DEVICE_E* device);

#endif /* APPL_SCRNSHOT_ENABLE */

int32_t             SETTINGS_LipSyncModeGet(AV_LIPSYNC_MODE_E* lipSyncMode);
int32_t             SETTINGS_LipSyncModeSet(AV_LIPSYNC_MODE_E lipSyncMode);
int32_t             SETTINGS_LipSyncDelayGet(int32_t* lipSyncLevel);
int32_t             SETTINGS_LipSyncDelaySet(int32_t lipSyncLevel);

#ifdef APPL_ETHERNET_ENABLED
int32_t             SETTINGS_EthIpGet(ETH_IP_S* ip);
int32_t             SETTINGS_EthIpSet(ETH_IP_S* ip);
int32_t             SETTINGS_EthDnsGet(ETH_DNS_S* ip);
int32_t             SETTINGS_EthDnsSet(ETH_DNS_S* ip);
#endif
/*---------------------------------------------------------------------------*/
/* Specific settings for satellite frontend                                  */
/*---------------------------------------------------------------------------*/

#ifdef APPL_FESAT_ENABLED

#define CFGSAT_LNB_MAX                   4
#define CFGSAT_SATNAME_SZ_MAX           32

#define CFGSAT_TONEBURST_ID_NUMS         2
#define CFGSAT_DISEQC10_ID_NUMS          4
#define CFGSAT_DISEQC11_DTH_ID_NUMS     64
#define CFGSAT_DISEQC11_SMATV_ID_NUMS    2
#define CFGSAT_DISEQC12_POS_NUMS        64

typedef enum
{
    CFGSAT_FEMODE_NONE = -1,    //!< invalid.
    CFGSAT_FEMODE_CLASSIC,      //!<  1 LNB, conrolled by 22 kHz, 14/18 V.
    CFGSAT_FEMODE_TONEBURST,    //!<  2 LNBs with additional signaling.
    CFGSAT_FEMODE_DISEQC10,     //!<  4 LNBs: DisEqC 1.0
    CFGSAT_FEMODE_DISEQC11_DTH, //!< 64 LNBs: DisEqC 1.1

    #ifdef APPL_SATPOSBY22KHZ_ENABLED
    CFGSAT_FEMODE_POSBY22KHZ,   //!<  2 singel Band LNBs, conrolled by 22 kHz
    #endif

    #ifdef APPL_DISEQC11_SMATV_ENABLED
    CFGSAT_FEMODE_DISEQC11_SMATV, //!< Diseqc 1.1 SMATV
    #endif /* APPL_DISEQC11_SMATV_ENABLED */

    #ifdef APPL_DISEQC12_ENABLED
    CFGSAT_FEMODE_DISEQC12,     //!< Motor device controlled by Diseqc 1.2
    #endif /* APPL_DISEQC12_ENABLED */

    #ifdef APPL_USALS_ENABLED
    CFGSAT_FEMODE_USALS,        //!< Motor device controlled by USALS.
    #endif /* APPL_USALS_ENABLED */

    CFGSAT_FEMODE_NUMS      //!< Number of modes available.
} CFGSAT_LNBMODE_E;

typedef enum                 /** Single or Multi-tuner installation options */
{
    CFGSAT_INSTALLTYPE_LOOPTHROUGH = 0,     //!< Loop-through (tuner 2 input is
    //!< tuner 1 output).
    CFGSAT_INSTALLTYPE_SAMESAT,     //!< Both tuners can be independently
    //!< controlled but point to the same
    //!< satellite.
    CFGSAT_INSTALLTYPE_INDEPENDENT, //!< Both tuners can be individually
    //!< controlled.
    CFGSAT_INSTALLTYPE_NUMS
} CFGSAT_INSTALLTYPE_E;

/*!
*******************************************************************************
**
** \brief LNB parameters.
**
******************************************************************************/
typedef struct
{
    fbool_t             active;         //!< FTRUE: satellite is accessible via
                                        //!< this frontend
    uint32_t            freqLo;         //!< low oscillator frequency in kHz
    uint32_t            freqHi;         //!< high oscillator frequency in kHz
                                        //!< NOTE: in case of a single oscillator,
                                        //!< freqHi must be equal to freqLo!
    uint8_t             toneburstId;    //!< Tonburst ID
    uint8_t             diseqc10Id;     //!< DisEqc 1.0 ID
    uint8_t             diseqc11Id;     //!< DisEqc 1.1 ID
    uint8_t             diseqc11Id_SMATV;//!< DisEqc 1.1 ID SMATV
    uint8_t             diseqc11Rep;    //!< DisEqc 1.1 repeats
    uint8_t             diseqc12Pos;    //!< DisEqc 1.2 position ( 0 = OFF )
    fbool_t             usalsAvl;       //!< indicates if satellite is accessable via
                                        //!< USALS (65 deg. limit!)

} CFGSAT_LNBCFG_S;

typedef struct
{
    /*!
     ** Frontend dependencies (needed for multi-frontend system only).
     */
    CFGSAT_INSTALLTYPE_E installType;

    /*!
     ** Operation mode of each frontend.
     */
    CFGSAT_LNBMODE_E    controlMode[RSRC_FE_MAX];

    uint32_t            ifFreq;  //!< IF Frequency for DiSEqc 1.1 SMATV [MHz]

} CFGSAT_FEPARS_S;

typedef struct
{
    uint16_t            degrees;    //!< orbital position [1/10 deg] (0-700)
    FE_WE_FLAG_E        westEastFlag;       //!< west east flag

} SYS_ORBPOS_S;

/*!
*******************************************************************************
**
** \brief Satellite parameters.
**
******************************************************************************/
typedef struct
{
    uint32_t            id;                         //!< General-purpose
                                                    //!< satellite ID field.
    SYS_CHARSET         nameCharSet;
    char_t              name[CFGSAT_SATNAME_SZ_MAX];
    fbool_t             active;                     //!< Activation status flag
                                                    //!< (appl.-specific).
    SYS_ORBPOS_S        orbitalPos;
    CFGSAT_LNBCFG_S     lnbCfg[RSRC_FE_MAX];        //!< LNB configuration for
                                                    //!< each tuner.

} CFGSAT_SAT_S;

int32_t             CFGSAT_AddSat          (const CFGSAT_SAT_S* dat,
                                            SYS_OWNER_E owner, int16_t* newPos);
int32_t             CFGSAT_RemoveSat       (int16_t lstPos);
int32_t             CFGSAT_ReplaceSat      (int16_t lstPos,
                                            const CFGSAT_SAT_S* dat,
                                            SYS_OWNER_E owner);
int32_t             CFGSAT_GetSatData      (int16_t lstPos, CFGSAT_SAT_S* dat);
int32_t             CFGSAT_GetNumSats      (int16_t* numSats);

int32_t             CFGSAT_GetIndex        (int16_t lstPos, CFGSAT_IDX* idx);
int32_t             CFGSAT_GetListPos      (CFGSAT_IDX idx, int16_t* lstPos);
int32_t             CFGSAT_GetDataByIndex  (CFGSAT_IDX idx,
                                            CFGSAT_SAT_S* pSatInf);
int32_t             CFGSAT_SetDataByIndex  (CFGSAT_IDX idx,
                                            const CFGSAT_SAT_S* pSatInf,
                                            SYS_OWNER_E owner);

int32_t             CFGSAT_GetFrontendSetup(CFGSAT_FEPARS_S* cfg);
int32_t             CFGSAT_SetFrontendSetup(const CFGSAT_FEPARS_S* cfg);

fbool_t             CFGSAT_SavingIsNecessary(void);
int32_t             CFGSAT_Load            (void);
int32_t             CFGSAT_Save            (fbool_t force);

/*---------------------------------------------------------------------------*/
/* DiseqC 1.2 motor control option                                           */
/*---------------------------------------------------------------------------*/

#ifdef APPL_DISEQC12_ENABLED

typedef struct              /* Event: record timer status change */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    fbool_t             moving;     //!< FTRUE motor moves; FFLALSE motor stopped

} SYS_EVT_MOTOR_S;


int32_t             AV_Dis12GotoSatPosition(int8_t feIdx, uint8_t satPos);
int32_t             AV_Dis12StoreSatPosition(int8_t feIdx, uint8_t satPos);
int32_t             AV_Dis12DriveMotor(int8_t feIdx,
                                       FE_WE_FLAG_E westEastFlag,
                                       uint8_t numSteps);
int32_t             AV_Dis12HaltMotor(int8_t feIdx);
int32_t             AV_Dis12ReCalcPos(int8_t feIdx);
int32_t             AV_Dis12SetLimitWest(int8_t feIdx);
int32_t             AV_Dis12SetLimitEast(int8_t feIdx);
int32_t             AV_Dis12ClearLimits(int8_t feIdx);
int32_t             AV_Dis12GotoZeroPosition(int8_t feIdx);

#endif /* APPL_DISEQC12_ENABLED */

/*---------------------------------------------------------------------------*/
/* USALS motor control options                                               */
/*---------------------------------------------------------------------------*/

typedef struct
{
    /*!
     ** Local longitude in 1/10 degrees: Positive values -> East of Greenwich.
     */
    int16_t             longitude;

    /*!
     ** Local latitude in 1/10 degrees: Positive values -> northern hemisphere.
     */
    int16_t             latitude;

} SYS_GEO_POS_S;

#ifdef APPL_USALS_ENABLED

int32_t             USALS_SetGeoPosition    (const SYS_GEO_POS_S* pos);
int32_t             USALS_GetGeoPosition    (SYS_GEO_POS_S* pos);
int32_t             USALS_GotoSatPosition   (int8_t feIdx,
                                             const SYS_ORBPOS_S* orbPos);
int32_t             USALS_GotoRefPosition   (int8_t feIdx);

#endif /* APPL_USALS_ENABLED */

#endif /* APPL_FESAT_ENABLED */

/*---------------------------------------------------------------------------*/
/* Carrier list management                                                   */
/*---------------------------------------------------------------------------*/

#define TPLIST_CARRIERNAME_MAX  8       //!< includes termination

typedef struct
{
    int32_t             tsId;       //!< TS identification (16 bit, -1: unknown).
    int32_t             networkId;  //!< network ID (16 bit, -1: unknown)
    int32_t             onId;       //!< Original network ID (16 bit, -1: unknown).
    FE_TUNE_PARAMS_S    pars;       //!< Tune parameters.
    uint16_t            sigQuality; //!< Last known signal quality
                                    //!< (0: unknown, 1...: quality level).
    #ifdef APPL_EPG_ISDBT
    uint16_t            prServId;   //!< partial reception service_id
    #endif /* APPL_EPG_ISDBT */
    SYS_CHARSET         nameCharSet;
    char_t              name[TPLIST_CARRIERNAME_MAX];

    // TODO: add provider name

} TPLIST_ITEM_S;

int32_t             TPLIST_AddCarrier       (const SYS_NETWORK_S* network,
                                             const TPLIST_ITEM_S* pTp,
                                             SYS_OWNER_E owner,
                                             int16_t* newPos);
int32_t             TPLIST_RemoveCarrier    (const SYS_NETWORK_S* network,
                                             int16_t lstPos);
int32_t             TPLIST_ReplaceCarrier   (const SYS_NETWORK_S* network,
                                             int16_t lstPos,
                                             const TPLIST_ITEM_S* pTp,
                                             SYS_OWNER_E owner);
int32_t             TPLIST_GetCarrierData   (const SYS_NETWORK_S* network,
                                             int16_t lstPos,
                                             TPLIST_ITEM_S* pTp);
int32_t             TPLIST_GetNumCarriers   (const SYS_NETWORK_S* network,
                                             int16_t* numTp);
int32_t             TPLIST_GetIndex         (const SYS_NETWORK_S* network,
                                             int16_t lstPos, TPLIST_IDX* idx);
int32_t             TPLIST_GetDataByIndex   (const SYS_NETWORK_S* network,
                                             TPLIST_IDX idx,
                                             TPLIST_ITEM_S* pTp);
int32_t             TPLIST_SetDataByIndex   (const SYS_NETWORK_S* network,
                                             TPLIST_IDX idx,
                                             const TPLIST_ITEM_S* pTp,
                                             SYS_OWNER_E owner);

int32_t             TPLIST_Clear            (const SYS_NETWORK_S* network,
                                             SYS_OWNER_E owners);
int32_t             TPLIST_Load             (FE_SYSTEM_TYPE_E feType);
int32_t             TPLIST_Save             (FE_SYSTEM_TYPE_E feType,
                                             fbool_t force);
int32_t             TPLIST_ResortLists      (FE_SYSTEM_TYPE_E feType);
fbool_t             TPLIST_SavingIsNecessary(const SYS_NETWORK_S* network);
TPLIST_IDX          TPLIST_GetTPidx         (FE_SYSTEM_TYPE_E feType, uint16_t on_id, uint16_t ts_id, CFGSAT_IDX satIdx);

#ifdef APPL_EPG_ISDBT

fbool_t             TPLIST_GetTerPrServ(uint16_t *prServId);

#endif /* APPL_EPG_ISDBT */

/*---------------------------------------------------------------------------*/
/* System time handling                                                      */
/*---------------------------------------------------------------------------*/

#define TIME_SINGLETIMERS_MAX  4        //!< Number of single-shot timers
                                        //!< available.
#define TIME_PERIODTIMERS_MAX  2        //!< Number of periodic timers
                                        //!< available.
#define TIME_SECSPERDAY        86400    //!< Number of seconds per day.
#define TIME_SECSPERHOUR       3600     //!< Number of seconds per hour.

#define TIME_SYSTEMTIME_TICK   100      //!< System time tick length in ms.

typedef enum
{
    TIMER_SINGLE = 0,
    TIMER_PERIODIC

} SYS_TIMER_TYPE_E;

typedef struct              /* Event: User timer expired */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_TIMER_TYPE_E    type;
    uint8_t             num;        //!< Index of expired timer.

} SYS_EVT_TIMER_S;

int32_t             TIME_GetSystemTime(uint32_t* time);
uint32_t            TIME_GetSystemTimeInterval(uint32_t timeStart,
                                               uint32_t timeEnd);

int32_t             TIME_TimeAndDateSet(const SYS_TIME_S* time,
                                        const SYS_DATE_S* date);
int32_t             TIME_TimeAndDateGet(SYS_TIME_S* pTime,
                                        SYS_DATE_S* pDate,
                                        fbool_t* hasSynced);
int32_t             TIME_TimeAndDateGetUtc(SYS_TIME_S* pTime,
                                           SYS_DATE_S* pDate,
                                           fbool_t* hasSynced);
uint16_t            TIME_GetWeekdayByDate(SYS_DATE_S* pDate);
int32_t             TIME_AddUtcOffset(SYS_TIME_S* pTime,
                                      SYS_DATE_S* pDate);
void                TIME_AddSeconds(SYS_TIME_S* pTime, SYS_DATE_S* pDate,
                                    int32_t secsToAdd);
void                TIME_AddHours(SYS_TIME_S* pTime, SYS_DATE_S* pDate,
                                  int32_t hoursToAdd);
void                TIME_AddDays(SYS_DATE_S* pDate, int32_t daysToAdd);

fbool_t             TIME_IsLaterThan(const SYS_TIME_S* time1,
                                     const SYS_DATE_S* date1,
                                     const SYS_TIME_S* time2,
                                     const SYS_DATE_S* date2);

uint32_t            TIME_GetLengthInSeconds(const SYS_TIME_S* pTimeStart,
                                            const SYS_TIME_S* pTimeEnd);
uint32_t            TIME_ConvToSeconds(const SYS_TIME_S* pTime);
SYS_TIME_S          TIME_ConvToTime(uint32_t seconds);

int32_t             TIME_SingleTimerLaunch(uint8_t timerNum,
                                           uint16_t timeInDeciSecs);
int32_t             TIME_SingleTimerAbort(uint8_t timerNum);

int32_t             TIME_PeriodicTimerLaunch(uint8_t timerNum,
                                             uint16_t timeInDeciSecs);
int32_t             TIME_PeriodicTimerAbort(uint8_t timerNum);

fbool_t             TIME_IsAutoMode(void);
void                TIME_SetAutoMode(fbool_t mode);
fbool_t             TIME_IsSync(void);
uint16_t            TIME_YmdToMjd(uint16_t year, uint16_t month, uint16_t day);;

/*---------------------------------------------------------------------------*/
/* Record timer handling                                                     */
/*---------------------------------------------------------------------------*/

#endif

#define RECTIMER_INVALID      -1        // invalid record timer ID

#if 0

#ifdef APPL_RECTIMER_ENABLED

#define RECTIMER_DESC_LENGTH  32

typedef enum
{
    RECTIMER_STATUS_START = 0,      //!< Timer starts (in 'seconds' seconds).
    RECTIMER_STATUS_STOP,           //!< Timer is stopped.
    RECTIMER_STATUS_REMOVED         //!< A currently active (started) timer
                                    //!< has been removed.

} RECTIMER_STATUS_E;

typedef struct              /* Event: record timer status change */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    RECTIMER_IDX        idx;        //!< Timer ID.
    RECTIMER_STATUS_E   status;     //!< Timer status.
    uint32_t            seconds;    //!< If starting timer: seconds to start,
                                    //!< 0: now; stopping timer: not used
    fbool_t             removeIt;   //!< FTRUE: Application should remove timer,
                                    //!< only used with status
                                    //!< RECTIMER_STATUS_STOP.

} SYS_EVT_RECTIMER_STATUS_S;

typedef enum
{
    RECTIMER_OP_RECORD = 0,
    RECTIMER_OP_SWITCH_SERVICE,
    RECTIMER_OP_DESCRAMBLE_REC,
    RECTIMER_OP_NUMS

} RECTIMER_OP_E;

typedef enum
{
    RECTIMER_TYPE_SINGLE = 0,       //!< single timer, is removed after expiration
    RECTIMER_TYPE_WEEKDAYS, //!< period: weekdays (Monday - Friday)
    RECTIMER_TYPE_WEEKLY,   //!< period: once a week
    RECTIMER_TYPE_WEEKENDS, //!< period: week-end only (Saturday/Sunday)
    RECTIMER_TYPE_DAILY,    //!< period: daily
    RECTIMER_TYPE_NUMS      //!< number of timer types

} RECTIMER_TYPE_E;

/*!
*******************************************************************************
**
** \brief Record timer data.
**
******************************************************************************/
typedef struct
{
    RECTIMER_OP_E       operation;    //!< Operation requested by timer.
    RECTIMER_TYPE_E     type;         //!< Repetition type.
    fbool_t             enabled;      //!< FTRUE if timer is enabled.

    SYS_CHARSET         descCharSet;
    char_t              desc[RECTIMER_DESC_LENGTH];
    SYS_CHARSET         extDescCharSet;
    char_t              extDesc[RECTIMER_DESC_LENGTH];

    SYS_TIME_S          startTime;    //!< Timer start time (UTC)
    SYS_DATE_S          startDate;    //!< Timer start date (UTC)
    SYS_TIME_S          endTime;      //!< Timer end time   (UTC)
    SYS_DATE_S          endDate;      //!< Timer end date   (UTC)

    SLIST_SERVIDX       serviceIndex; //!< Service identification.
    uint16_t            eventId;      //!< event_id (0 if timer is not based on
                                      //!< EIT event).
    fbool_t             autoStandby;  //!< FTRUE: Enter standby at record end.
    SYS_DEVICE_E        targetDevice; //!< Target device for recording timer.

    PVRLST_IDX          pvrIdx;       //!< Source recording, for operation
                                      //!< RECTIMER_OP_DESCRAMBLE_REC only!

} RECTIMER_ITEM_S;

int32_t             RECTIMER_Add            (const RECTIMER_ITEM_S* pTimer,
                                             int16_t* newPos);
int32_t             RECTIMER_AddInstantRec  (const RECTIMER_ITEM_S* pTimer,
                                             int16_t* newPos,
                                             fbool_t keepInstRecTimer);
int32_t             RECTIMER_Remove         (int16_t lstPos);
int32_t             RECTIMER_Replace        (int16_t lstPos,
                                             const RECTIMER_ITEM_S* pTimer);
int32_t             RECTIMER_Get            (int16_t lstPos,
                                             RECTIMER_ITEM_S* pTimer);
int32_t             RECTIMER_GetIndexByEvtId(SLIST_SERVIDX servIdx,
                                             uint16_t eventId,
                                             RECTIMER_IDX* idx);
int32_t             RECTIMER_GetIndex       (int16_t lstPos, RECTIMER_IDX* idx);
int32_t             RECTIMER_GetNumTimers   (uint16_t* numTimers);

int32_t             RECTIMER_Reinsert       (RECTIMER_IDX idx);
int32_t             RECTIMER_GetByIndex     (RECTIMER_IDX idx,
                                             RECTIMER_ITEM_S* pTimer);
int32_t             RECTIMER_RemoveByIndex  (RECTIMER_IDX idx);
int32_t             RECTIMER_ReplaceByIndex (RECTIMER_IDX idx,
                                             const RECTIMER_ITEM_S* pTimer);
int32_t             RECTIMER_Load           (void);
int32_t             RECTIMER_Save           (fbool_t force);

int32_t             RECTIMER_SetSleepTimer  (const SYS_TIME_S* pTime,
                                             const SYS_DATE_S* pDate);
int32_t             RECTIMER_GetSleepTimer  (fbool_t* enabled,
                                             SYS_TIME_S* pTime,
                                             SYS_DATE_S* pDate);

#endif /* APPL_RECTIMER_ENABLED */

/*---------------------------------------------------------------------------*/
/* Update management                                                         */
/*---------------------------------------------------------------------------*/

#ifdef APPL_UPDATE_ENABLED

typedef enum                /* Software update status definitions */
{
    UPDATE_STAT_INACTIVE = 0,   //!< Status: Inactive
    UPDATE_STAT_SEARCH,         //!< Status: Searching for Data
    UPDATE_STAT_DATACOLLECT,    //!< Status: Acquirung image data
    UPDATE_STAT_PROGRAMMING,    //!< Status: Flash is being programmed
    UPDATE_STAT_FINISHED,       //!< Status: Update finished
    UPDATE_STAT_NOTFOUND,       //!< Status: No Update found
    UPDATE_STAT_ABORTED,        //!< Status: Update has been aborted
    UPDATE_STAT_ERROR           //!< Status: Error

} UPDATE_STATUS_E;

typedef enum                /* Software programming status definitions */
{
    UPDATE_PROGRAM_NONE = -1,   //!< Idle
    UPDATE_PROGRAM_ERASE = 0,   //!< Flash sectors are being erased
    UPDATE_PROGRAM_WRITE,       //!< Flash is being written
    UPDATE_PROGRAM_VERIFY       //!< Flash content is compared to source data

} UPDATE_PROGRAM_STAT_E;

typedef struct              /* Event: Status of software update */
{
    SYS_EVTHDR_S        hdr;            //!< Common event header.
    UPDATE_STATUS_E     status;         //!< Current status.
    UPDATE_PROGRAM_STAT_E programStat;  //!< Current programming status.
    uint32_t            progCurr;       //!< Current progress value
                                        //!< (0...progMax).
    uint32_t            progMax;        //!< Maximum progress value.

} SYS_EVT_UPDATE_S;

typedef struct
{
    UPDATE_STATUS_E     status;     //!< Current status
    uint32_t            imgBufSize; //!< Total size of image buffer in bytes.
    uint32_t            imgBufPos;  //!< Current image data write position.

} UPDATE_INFO_S;

int32_t             UPDATE_Open(void);
int32_t             UPDATE_Close(void);
int32_t             UPDATE_GetInfo(UPDATE_INFO_S* pInfo);
int32_t             UPDATE_AddData(const uint8_t* pData,
                                   uint32_t numBytes);
int32_t             UPDATE_SetData(const uint8_t* pData, uint32_t bytePos,
                                   uint32_t numBytes);
int32_t             UPDATE_ResetData(void);
int32_t             UPDATE_ProgramImage(uint32_t slotAddr);
int32_t             UPDATE_Abort(void);
uint32_t            UPDATE_CalcCrc(uint32_t bufPos, uint32_t sizeInBytes);
int32_t             UPDATE_InvalidateImage(uint32_t slotAddr);

#endif /* APPL_UPDATE_ENABLED */

/*---------------------------------------------------------------------------*/
/* Teletext API                                                              */
/*---------------------------------------------------------------------------*/

#define TTXSUBTT_STREAMS_MAX  16        //!< max. number of TTX Subtitle streams
//!< per service.

#ifdef APPL_TTXT_ENABLED

typedef struct
{
    uint8_t             layer;      //!< OSD Layer to be used for TTX presentation
    uint8_t             clutOffset; //!< CLUT index of first color used for TTX

} TTX_INIT_PARAMS_S;

/*!
*******************************************************************************
**
** \brief TTX display mode options.
**
******************************************************************************/
typedef enum
{
    TTX_NORM = 0,
    TTX_BGR_HALFTRANS,
    TTX_BGR_FULLTRANS,
    TTX_BGR_SPECIAL,
    TTX_MODENUMS

} TTX_MODE_E;

typedef struct
{
    uint16_t            pid;
    SYS_LANGCODE        langCode;
    uint8_t             type;
    uint32_t            pageNumber;

} TTXSUBTT_INF_S;

/* Software teletext decoder API */
int32_t             TTX_Show(uint16_t pageNumber);
int32_t             TTX_Hide(void);
TTX_MODE_E          TTX_ModeGet(void);
int32_t             TTX_ModeSet(TTX_MODE_E mode);
int32_t             TTX_KeyInput(uint16_t keyCode);
int32_t             TTX_KeySize(void);
int32_t             TTX_KeyReveal(void);
int32_t             TTX_PageSelect(uint16_t pageNumber);
int32_t             TTX_IndexSelect(void);
int32_t             TTX_ToggleLangGroup(void);

/* Teletext subtitles via software teletext decoder */
int32_t             TTXSUBTT_Select(int8_t index);
void                TTXSUBTT_Hide(void);
void                TTXSUBTT_Restart(void);
uint8_t             TTXSUBTT_NumStreamsGet(void);
int8_t              TTXSUBTT_CurrStreamGet(void);
int32_t             TTXSUBTT_StreamInfoGet(uint8_t index,
                                           TTXSUBTT_INF_S* pStrInf);

#endif /* APPL_TTXT_ENABLED */

/*---------------------------------------------------------------------------*/
/* MHEG-5 API                                                                */
/*---------------------------------------------------------------------------*/
#ifdef APPL_MHEG_ENABLED

typedef void  (*MHEG_SETKEYMASK_CB)  (const int32_t *keys, size_t keysCount);

typedef struct
{
    int32_t             eosPrio[5];
    int32_t             timerBsrPrio;
    uint32_t            timerBsrStackSz;
    char_t              timerBsrName[15];

} MHEG_OS_INIT_PARAMS_S;

typedef struct
{
    uint32_t                layer;      //!< OSD Layer to be used for MHEG presentation
    uint32_t*               splashData;
    uint32_t                splashSize;
    char_t                  hostString[10];
    MHEG_OS_INIT_PARAMS_S   os;
    redkey_profile_t        profile;
    MHEG_SETKEYMASK_CB      appKeyCallback;

} MHEG_INIT_PARAMS_S;

typedef enum
{
    MHEG_STATE_UNINITIALIZED = 0,
    MHEG_STATE_STOPPED,
    MHEG_STATE_RUNNING_APP,
    MHEG_STATE_TERMINATED,
    MHEG_STATE_NUMS

} MHEG_STATE_E;

typedef enum
{
    MHEG_PREF_LANG_ENG = 0,
    MHEG_PREF_LANG_WELCYM,
    MHEG_PREF_LANG_GAEGDH

} MHEG_PREF_LANG_E;

typedef struct
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    MHEG_STATE_E        state;
    itk_int32_t         index;
    itk_uint32_t        ctrlInfo;
    fbool_t             retuneReq;

} MHEG_EVT_STATE_S;

typedef struct
{
    SYS_EVTHDR_S            hdr;
    itk_appmmi_session_t*   sess;
    itk_appmmi_message_t    type;
    itk_appmmi_msg_t        msg;

} MHEG_EVT_APPMMI_MSG_S;

typedef struct
{
    SYS_EVTHDR_S        hdr;

} MHEG_EVT_START_S;

int32_t      MHEG_Open(void);
int32_t      MHEG_Close(void);
int32_t      MHEG_Stop(void);
int32_t      MHEG_Start(void);
void         MHEG_Enable (fbool_t enable, SYS_SUSPENDSRC src);
int32_t      MHEG_Show(void);
int32_t      MHEG_Hide(void);
redkey_profile_t MHEG_GetProfile(void);
MHEG_STATE_E MHEG_GetStatus(void);
fbool_t      MHEG_IsOpened(void);
fbool_t      MHEG_IsStarted(void);
fbool_t      MHEG_DisplayDispatchKey(itk_int32_t key);

#endif /* APPL_MHEG_ENABLED */

/*---------------------------------------------------------------------------*/
/* Subtitling API                                                            */
/*---------------------------------------------------------------------------*/

#define SUBTT_STREAMS_MAX  16   //!< max. number of subtitle streams per service

typedef enum
{
    SUBTT_DISPLAY_NONE = 0,  //!< Subtitles with no aspect ratio criticality.
    SUBTT_DISPLAY_4_3,       //!< Subtitles for display on 4:3 aspect ratio.
    SUBTT_DISPLAY_16_9,      //!< Subtitles for display on 16:9 aspect ratio.
    SUBTT_DISPLAY_221_1,     //!< Subtitles for display on 2.21:1 aspect ratio.
    SUBTT_DISPLAY_HD,        //!< Subtitles for display on a HD monitor
    SUBTT_ASPECT_NUMS

} SUBTT_DISPLAY_E;

#ifdef APPL_SUBTT_ENABLED

typedef struct
{
    uint8_t             layer;      //!< OSD Layer to be used for TTX presentation
    uint8_t             clutOffset; //!< CLUT index of first color used for TTX
    uint8_t             numColors;  //!< Max. number of colors available for SUBTT

    uint8_t             supportedSubtitleTypes; //!< Or' ed bitfield of
                                                //!< SYS_SUBTITLE_TYPE_XXX.
} SUBTT_INIT_PARAMS_S;

/*!
*******************************************************************************
**
** \brief Subtitle stream information.
**
******************************************************************************/
typedef struct
{
    SYS_LANGCODE        langCode;
    uint8_t             type;
    SUBTT_DISPLAY_E     displayType;

} SUBTT_STREAMINF_S;

int32_t             SUBTT_Select(int8_t index);
void                SUBTT_Suspend(void);
void                SUBTT_Restart(void);
void                SUBTT_ShowOsd(void);
void                SUBTT_HideOsd(void);
uint8_t             SUBTT_NumStreamsGet(void);
int8_t              SUBTT_CurrStreamGet(void);
int32_t             SUBTT_StreamInfoGet(uint8_t index,
                                        SUBTT_STREAMINF_S* pStrInf);
fbool_t             SUBTT_GetPresentationState( void );

#endif /* APPL_SUBTT_ENABLED */

/*---------------------------------------------------------------------------*/
/* PIP API                                                                   */
/*---------------------------------------------------------------------------*/

#ifdef APPL_PIP_ENABLED

typedef struct
{
    uint8_t             layer;      //!< OSD Layer to be used for PIP

} PIP_INIT_PARAMS_S;

typedef struct
{
    fbool_t             active;     //!< Activity status
    fbool_t             sourceIsRec;        //!< FTRUE if record is presented via PIP
    SYS_SERVLOC_S       service;    //!< Current service (valid if active = 1)
    int8_t              recIdx;     //!< Record identification
    AV_VO_PICTURE_WINDOW_S window;  //!< PIP window position and size

} PIP_STATUS_S;

int32_t             PIP_SelectService(const SYS_SERVLOC_S* pServLoc);
int32_t             PIP_Stop(void);
int32_t             PIP_Hide(void);
int32_t             PIP_SetPosition(const AV_VO_PICTURE_WINDOW_S* pWindow);
int32_t             PIP_GetStatus(PIP_STATUS_S* pStatus);

#ifdef APPL_PVR_ENABLED

int32_t             PIP_SelectRecord(uint8_t recIdx);

#endif /* APPL_PVR_ENABLED */

#endif /* APPL_PIP_ENABLED */

/*---------------------------------------------------------------------------*/
/* JPEG API                                                                  */
/*---------------------------------------------------------------------------*/

#ifdef APPL_JPEG_ENABLED

typedef enum
{
    PIC_TYPE_UNKNOWN = -1, //!< Picture type is unknown.
    PIC_TYPE_JPEG    = 0,  //!< Picture type is JPEG.
    PIC_TYPE_BMP,          //!< Picture type is bitmap (BMP).
    PIC_TYPE_PNG           //!< Picture type is PNG.

} PIC_TYPE_E;

typedef enum
{
    JPEG_DECODEINFO_OK = 0,
    JPEG_DECODEINFO_LOW_MEMORY,
    JPEG_DECODEINFO_UNKNOWN_TYPE,
    JPEG_DECODEINFO_ERROR

} JPEG_DECODEINFO_TYPE_E;

typedef struct              /* Event: Information on JPEG file */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint16_t            sizeX;      //!< Horizontal size in pixels.
    uint16_t            sizeY;      //!< Vertical size in pixels.
    uint16_t            bitsperpix; //!< Bits per pixel.
    PIC_TYPE_E          fileType;   //!< Picture coding type.

} JPEG_EVT_INFO_S;

typedef struct
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    fbool_t             osdIsReady;

} JPEG_EVT_OSDINFO_S;

typedef struct              /* Event: Information on picture decoding */
{
    SYS_EVTHDR_S                hdr;        //!< Common event header.
    JPEG_DECODEINFO_TYPE_E      infoType;   //!< Picture decoding information.

} JPEG_EVT_DECODEINFO_S;

typedef struct
{
    const char_t*       path;       //!< File path.
    int32_t             filepos;    //!< Start byte position in file.
    JPEG_orientation_t  orient;     //!< Picture orientation.
    uint16_t            zoom;       //!< Requested zoom factor in per cent.
    uint16_t            xpos;       //!< Hor. start in source picture.
    uint16_t            ypos;       //!< Vert. start in source picture.
    fbool_t             embedded;    //!< The flag indicates if the picture is
                                    //!< embedded inside another type of file,
                                    //!< e.g. PNG picutre in MP3 file
} JPEG_PIC_PARAMS_S;

typedef struct
{
    AV_VO_PICTURE_WINDOW_S pos;

} JPEG_SCREEN_PARAMS_S;

typedef struct
{
    uint8_t             layer;      //!< OSD Layer to be used for JPEG decoding.

} JPEG_INIT_PARAMS_S;

int32_t             JPEG_Open  (const JPEG_SCREEN_PARAMS_S* screen);
int32_t             JPEG_Show  (const JPEG_PIC_PARAMS_S* pic,
                                const JPEG_SCREEN_PARAMS_S* screen);
int32_t             JPEG_Clear (void);
int32_t             JPEG_Close (void);
fbool_t             JPEG_IsBusy(void);

#endif /* APPL_JPEG_ENABLED */

/*---------------------------------------------------------------------------*/
/* MP3 API                                                                   */
/*---------------------------------------------------------------------------*/

#ifdef APPL_MP3_ENABLED

#define MP3_TAG_STRLEN_MAX   32

typedef enum
{
    MP3_STATUS_END = 0,
    MP3_STATUS_PLAY,
    MP3_STATUS_PAUSE,
    MP3_STATUS_READY

} MP3_PLAY_STATUS_E;


typedef enum
{
    MP3_INFO_OK = 0,
    MP3_INFO_UNKNOWN_TYPE,
    MP3_INFO_ERROR

} MP3_INFO_TYPE_E;


typedef struct              /* Event: Information on JPEG file */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint32_t            timeTotal;  //!< Track length in seconds.
    uint32_t            timeCurr;   //!< Playback time in seconds.
    MP3_PLAY_STATUS_E   status;     //!< Playback status.
    MP3_INFO_TYPE_E     infoType;   //!< MP3 play information.

} MP3_EVT_STATUS_S;

typedef struct
{
    SYS_CHARSET     titleCs;
    char_t          title[MP3_TAG_STRLEN_MAX];  //!< ID3 title string.
    SYS_CHARSET     artistCs;
    char_t          artist[MP3_TAG_STRLEN_MAX]; //!< ID3 artist string.
    SYS_CHARSET     albumCs;
    char_t          album[MP3_TAG_STRLEN_MAX];  //!< ID3 album string.

    fbool_t         jpegAvl;                    //!< FTRUE: embedded JPEG pic.
                                                //!< is available.
    uint32_t        picPos;                     //!< Byte position of embedded
                                                //!< picture.
                                                //!< (0 if not available).
    uint32_t        length;                     //!< Play time in seconds
                                                //!< (0: not avl.).

} MP3_FILE_INFO_S;

typedef struct
{
    int32_t                     sampleFreq;     //!< Sample frequency
    uint32_t                    timeTotal;      //!< Total play time
    int32_t                     frameStartPos;  //!< Start position of 1st frame
    uint32_t                    bitHdr;         //!< First 4 bytes header info
                                                //!< in bit
    fbool_t                     ifMP3Valid;     //!< If the file is valid MP3
} MP3_FRAME_INFO_S;

int32_t             MP3_GetInfo(const char_t* filePath,
                                MP3_FILE_INFO_S* data);
int32_t             MP3_Start(const char_t* filePath);
int32_t             MP3_Stop(void);
int32_t             MP3_Pause(fbool_t pauseOn);

#endif /* APPL_MP3_ENABLED */

/*---------------------------------------------------------------------------*/
/* Parental Control API                                                      */
/*---------------------------------------------------------------------------*/

#define PCTRL_MINAGE_UNLIMITED  0    //!< User-defined viewing age: unlimited.
#define PCTRL_MINAGE_LOCKED     0xFF //!< User-defined viewing age: locked for
                                     //!< all ages.

#ifdef APPL_PARENTAL_ENABLED

#define PCTRL_PIN_LEN    4

typedef enum
{
    PCTRL_STAT_UNKNOWN = 0,         //!< Status is currently being evaluated.
    PCTRL_STAT_LOCKED,              //!< Waiting for authorization.
    PCTRL_STAT_UNLOCKED_SINGLE,     //!< Authorized by normal PIN.
    PCTRL_STAT_UNLOCKED_MASTER,     //!< Authorized by master PIN.
    PCTRL_STAT_NOTNEEDED            //!< No authorization needed for current
                                    //!< event or service.
} PCTRL_STAT_E;

typedef struct  /* Event: Authentication for current presentation needed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    SYS_PRESSTATE_E     state;      //!< Presentation state (live or playback).
    SYS_SERVLOC_S       servLoc;    //!< Service identification
                                    //!< (valid for live only).
    uint16_t            eventId;    //!< event ID (0: not event- but
                                    //!< service-specific)
    uint8_t             minAge;     //!< Requested minimum age.

} PCTRL_EVT_NEEDAUTH_S;

typedef enum
{
    PCTRL_PIN_SINGLE = 0,   //!< For single service/event authentication.
    PCTRL_PIN_MASTER,       //!< For master service/event authentication and
                            //!< any other purpose (e.h. setup menu protection).
    PCTRL_PIN_NUMS

} PCTRL_PIN_TYPE_E;

typedef struct
{
    SYS_CTRYCODE country;        //!< Preferred country for parental
                                 //!< rating evaluation.
    fbool_t      useStreamRating;//!< FTRUE: evaluate current event's parental
                                 //!< rating descriptor.
    uint8_t      viewerAge;      //!< Age of viewer in years.
    uint32_t     protectionArea; //!< May be used by application to store current
                                 //!< protection status of its menu areas.

} PCTRL_PARAMS_S;

typedef struct
{
    PCTRL_STAT_E        status;     //!< Current authorization status.
    fbool_t             outputLocked;       //!< Current lock status.

} PCTRL_STATUS_S;

#ifdef APPL_PVR_ENABLED

int32_t             PCTRL_SetRecordProtection   (PVRLST_IDX idx,
                                                 uint8_t minAge);
int32_t             PCTRL_GetRecordProtection   (PVRLST_IDX idx,
                                                 uint8_t* minAge);
#endif /* APPL_PVR_ENABLED */

int32_t             PCTRL_SetEnableStatus       (fbool_t enabled);
int32_t             PCTRL_GetEnableStatus       (fbool_t* enabled);
int32_t             PCTRL_SetServiceProtection  (const SYS_SERVLOC_S*
                                                 pServLoc, uint8_t minAge);
int32_t             PCTRL_GetServiceProtection  (const SYS_SERVLOC_S*
                                                 pServLoc, uint8_t* minAge);
int32_t             PCTRL_SetParams             (const PCTRL_PARAMS_S* params);
int32_t             PCTRL_GetParams             (PCTRL_PARAMS_S* params);
int32_t             PCTRL_Restart               (void);
int32_t             PCTRL_SetPin                (PCTRL_PIN_TYPE_E type,
                                                 const char_t* pin);
int32_t             PCTRL_CheckPin              (PCTRL_PIN_TYPE_E type,
                                                 const char_t* pin);
int32_t             PCTRL_AuthenticateService   (const SYS_SERVLOC_S* pServLoc,
                                                 const char_t* pin);
int32_t             PCTRL_GetStatus             (PCTRL_STATUS_S* pStatus);
int32_t             PCTRL_Load                  (void);
int32_t             PCTRL_Save                  (fbool_t force);

#endif /* APPL_PARENTAL_ENABLED */

/*----------------------------------------------------------------------------*/
/* Resource management                                                        */
/*----------------------------------------------------------------------------*/

#define RSRC_TSD_NUMS   4
#define RSRC_VD_NUMS    1
#define RSRC_REC_MAX    2

#define RSRC_OP_NONE             0x0000   //!< No operation.
#define RSRC_OP_LIVE             0x0001   //!< Operation: Live.
#define RSRC_OP_REC1             0x0002   //!< Operation: Record 1 (TSD-2).
#define RSRC_OP_REC2             0x0004   //!< Operation: Record 2 (TSD-3).
#define RSRC_OP_PLAY             0x0008   //!< Operation: PVR Playback.
#define RSRC_OP_PIP              0x0010   //!< Operation: PIP presentation.
#define RSRC_OP_SCAN             0x0020   //!< Operation: Service Scan.
#define RSRC_OP_TUNE1            0x0040   //!< Operation: Tune mode on FE-1.
#define RSRC_OP_TUNE2            0x0080   //!< Operation: Tune mode on FE-2.
#define RSRC_OP_UPDATE           0x0100   //!< Operation: Software update.
#define RSRC_OP_DESCRREC         0x0200   //!< Operation: Descramble recording.
#define RSRC_OP_RECANY           0x0006   //!< Mask: Rec1 || Rec2.
#define RSRC_OP_TUNEANY          0x00C0   //!< Mask: Tune1 || Tune2.
#define RSRC_OP_ANY              0x07FF   //!< Mask: All operations.
#define RSRC_OP_NUMS             12    //11   //!< Total number of operations.

#define RSRC_STAT_FE_OPENED      0x01     //!< Frontend is available
#define RSRC_STAT_FE_LIVE        0x02     //!< Frontend is used for live-TV decoding
#define RSRC_STAT_FE_SCAN        0x04     //!< Frontend is used for scan.
#define RSRC_STAT_FE_TUNEMODE    0x08     //!< Frontend is in tune mode.
#define RSRC_STAT_FE_REC1        0x10     //!< Frontend used for recording 1.
#define RSRC_STAT_FE_REC2        0x20     //!< Frontend used for recording 2.
#define RSRC_STAT_FE_REC_ANY     0x30     //!< Mask for any recording.
#define RSRC_STAT_FE_PIP         0x40     //!< Frontend is used for PIP.
#define RSRC_STAT_FE_ANY         0xFE     //!< Masks any frontend operation

#define RSRC_STAT_TSD_LIVE       0x01     //!< TSD is used for live-TV decoding.
#define RSRC_STAT_TSD_RECORD     0x02     //!< TSD is used for recording.
#define RSRC_STAT_TSD_PLAYBACK   0x04     //!< TSD is used for playback.
#define RSRC_STAT_TSD_PIP        0x08     //!< TSD is used for PIP presentation.
#define RSRC_STAT_TSD_SCAN       0x10     //!< TSD is used for scan.
#define RSRC_STAT_TSD_ANY        0xFF     //!< Masks any TSD usage.

#define RSRC_STAT_CI_UNUSED      0x00     //!< CI module currently not used.
#define RSRC_STAT_CI_LIVE        0x01     //!< CI module used for decoding live-TV.
#define RSRC_STAT_CI_REC1        0x02     //!< CI module used for recording 1.
#define RSRC_STAT_CI_REC2        0x04     //!< CI module used for recording 2.
#define RSRC_STAT_CI_REC_ANY     0x06     //!< Mask for any recording
#define RSRC_STAT_CI_PIP         0x08     //!< CI module used for PIP.
#define RSRC_STAT_CI_PLAY        0x10     //!< CI module used for playback (via
                                          //!< TS playback only).
#define RSRC_STAT_CI_ANY         0xFF     //!< Masks any CI operation.

typedef enum
{
    RSRC_TS_NONE = -1,
    RSRC_TS_A    = 0,
    RSRC_TS_B, //1
    RSRC_TS_NUMS //2

} RSRC_TS_E;

typedef enum
{
    RSRC_STAT_VD_UNUSED = 0,
    RSRC_STAT_VD_USED

} RSRC_STAT_VD_E;

typedef enum
{
    RSRC_STAT_VOUT_UNUSED = 0,
    RSRC_STAT_VOUT_LIVE,
    RSRC_STAT_VOUT_PLAYBACK

} RSRC_STAT_VOUT_E;

typedef enum    /* CA system definitions */
{
    RSRC_CAS_NONE = -1,
    RSRC_CAS_CONAX = 0,
    RSRC_CAS_NUMS

} RSRC_CAS_E;

typedef struct  /* resource configuration */
{
    RSRC_OP              op; //0       //!< Operation ID, one of RSRC_OP_XXX.

    /* configuration parameters */
    SLIST_SERVIDX       servIdx; //2   //!< Service index (for live/play).
    PVRLST_IDX          pvrIdx; //4    //!< Record reference (for rec/descrrec).

    int8_t              feIdx; //6     //!< Frontend to be used: 0...(n-1).
    RSRC_TS_E           ts; //8        //!< TS input used.
    uint8_t             tsdIdx; //12    //!< TSD used
    fbool_t             useCi; //16     //!< use external CAM for descrambling.
    RSRC_CI_IN          ciInput; //20   //!< CI input in case of 'useCi' = 1,
                                    //!< one of FAPI_BOARD_RSRC_CIIN_XXX.
    RSRC_CAS_E          cas; //24       //!< Embedded CA system to be used for
                                    //!< descrambling if any.

    /* informative parameters */
    fbool_t             ciMatch[RSRC_TS_NUMS]; //28
    //!< CAS matching status for each CAM, set for
    //!< both TS, independent of 'useCi'.
    
    int fill; //36
    //40
} RSRC_CFG_S;

typedef struct              /* resource configuration constraints */
{
    fbool_t             omitTs[RSRC_TS_NUMS];       //!< FTRUE: omit corresp. TS.
    fbool_t             omitTsd[RSRC_TSD_NUMS];     //!< FTRUE: omit corresp. TSD.

} RSRC_CONSTR_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_LIVE_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_PLAY_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_REC_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_DESCRREC_S;

typedef struct
{
    fbool_t             sourceIsRec;
    RSRC_CFG_S          cfg;
    int8_t              recIdx;

} RSRC_OP_PIP_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_SCAN_S;

typedef struct
{
    RSRC_CFG_S          cfg;

} RSRC_OP_TUNE_S;

typedef struct
{
    uint8_t             use;        //!< FE usage bitfield.
    FE_SYSTEM_TYPE_E    type;       //!< FE's delivery system type.
    CFGSAT_IDX          satIdx;     //!< Tune params: Satellite index.
    TPLIST_IDX          tpIdx;      //!< Tune params: Carrier index.

} RSRC_STAT_FE_S;

typedef struct
{
    uint8_t             use;        //!< TSD usage bitfield.
    RSRC_TS_E           tsIn;       //!< Selected TS-input

} RSRC_STAT_TSD_S;

typedef struct
{
    uint8_t             use;        //!< CI usage bitfield.
    SLIST_SERVIDX       servIdx;    //!< Service currently being
                                    //!< decoded.
    RSRC_CI_IN          input;      //!< CI input selection.

} RSRC_STAT_CI_S;

typedef struct
{
    RSRC_OP             opStat;             //!< Active operations (RSRC_OP_XXX)

    RSRC_STAT_FE_S      fe[RSRC_FE_MAX];    //!< Status of frontend(s).
    RSRC_STAT_TSD_S     tsd[RSRC_TSD_NUMS]; //!< Status of TSDs.
    RSRC_STAT_VD_E      vd[RSRC_VD_NUMS];   //!< Status of video decoder(s).
    RSRC_STAT_VOUT_E    videoout;           //!< Status of full-screen video
                                            //!< output.
    RSRC_STAT_CI_S      ci[RSRC_TS_NUMS];   //!< Status of CI modules.

} RSRC_STAT_HW_S;

typedef struct
{
    /* Operation resources */
    RSRC_OP             opStatus;           //!< Bitfield holding RSRC_OP_XXX
    RSRC_OP_LIVE_S      live; //4
    RSRC_OP_SCAN_S      scan;
    RSRC_OP_TUNE_S      tune[RSRC_FE_MAX];

    #ifdef APPL_PVR_ENABLED

    int fill[10]; 
    
    RSRC_OP_PLAY_S      play; //206
    RSRC_OP_REC_S       rec[RSRC_REC_MAX];  // rec[0]: TSD-2, (rec[1]: TSD-3)
    RSRC_OP_DESCRREC_S  descrRec;

    #endif /* APPL_PVR_ENABLED */

    #ifdef APPL_PIP_ENABLED

    RSRC_OP_PIP_S       pip;

    #endif /* APPL_PIP_ENABLED */

    /* Hardware resources */
    RSRC_STAT_HW_S      hw;

} RSRC_STATUS_S;

RSRC_STATUS_S*      RSRC_GetStatus(void);

int32_t             RSRC_ServiceAvlForLive(SLIST_SERVIDX servIdx,
                                           RSRC_OP op,
                                           const RSRC_CONSTR_S* pConstr,
                                           RSRC_CFG_S* pCfg);
int32_t             RSRC_NetworkAvlForScan(const SYS_NETWORK_S* network,
                                           RSRC_OP op, RSRC_CFG_S* pCfg);
int32_t             RSRC_NetworkAvlForTunemode(int8_t feIdx,
                                               const SYS_NETWORK_S*
                                               network, RSRC_OP op,
                                               RSRC_CFG_S* pCfg);

#ifdef APPL_PVR_ENABLED

int32_t             RSRC_ServiceAvlForRecord(SLIST_SERVIDX servIdx,
                                             RSRC_OP op,
                                             const RSRC_CONSTR_S* pConstr,
                                             RSRC_CFG_S* pCfg);
int32_t             RSRC_PlaybackAvailable(PVRLST_IDX idx, RSRC_OP op,
                                           const RSRC_CONSTR_S* pConstr,
                                           RSRC_CFG_S* pCfg);

#if defined(APPL_CI_ENABLED) && defined(APPL_DESCRREC_ENABLED)

int32_t             RSRC_DescrRecordAvailable(PVRLST_IDX idx, RSRC_OP op,
                                              const RSRC_CONSTR_S* pConstr,
                                              RSRC_CFG_S* pCfg);

#endif /* defined(APPL_CI_ENABLED) && defined(APPL_DESCRREC_ENABLED) */

#endif /* APPL_PVR_ENABLED */

#ifdef APPL_PIP_ENABLED

int32_t             RSRC_ServiceAvlForPIP(SLIST_SERVIDX servIdx, RSRC_OP op,
                                          RSRC_OP_PIP_S* pParams);

#ifdef APPL_PVR_ENABLED

int32_t             RSRC_RecordAvlForPIP(int8_t recIdx, RSRC_OP op,
                                         RSRC_OP_PIP_S* pParams);

#endif /* APPL_PVR_ENABLED */

#endif /* APPL_PIP_ENABLED */

#ifdef APPL_UPDATE_ENABLED

int32_t             RSRC_UpdateAvl(RSRC_OP op);

#endif /* APPL_UPDATE_ENABLED */

/*---------------------------------------------------------------------------*/
/* OAD API                                                                   */
/*---------------------------------------------------------------------------*/

#ifdef APPL_OAD_ENABLED
typedef enum
{
    OAD_NONE = -1,

    #ifdef APPL_OAD_FOTA_ENABLED
    OAD_FOTA,
    #endif /* #ifdef APPL_OAD_FOTA_ENABLED */

    #ifdef APPL_OAD_SSU_ENABLED
    OAD_SSU,
    #endif /* #ifdef APPL_OAD_SSU_ENABLED */

    OAD_NUMS

} OAD_TYPE_E;

typedef struct
{
    uint32_t            oui;        //<! Manufacturer ID
    uint32_t            versionId;  //<! Hardware Version

} OAD_INIT_PARAMS_S;

typedef struct
{

    /*
     * The type of over air download OAD_xxx
     */
    OAD_TYPE_E      type;

    /*
     * The system network to start simple OAD, for SSU this
     * parameter is ignored.
     */
    SYS_NETWORK_S*  network;

    /*
     * The carrier information to start simple OAD, for SSU this
     * parameter is ignored.
     */
    TPLIST_ITEM_S*  carrierInf;

} OAD_OPEN_S;

#ifdef APPL_OAD_SSU_ENABLED
typedef enum
{
    OAD_SSU_SLEEP =0,
    OAD_SSU_START,
    OAD_SSU_FILTER_START,
    OAD_SSU_FILTER_HANDLE,
    OAD_SSU_TUNE_START,
    OAD_SSU_TUNE_WAIT,
    OAD_SSU_DOWNLOAD_START,
    OAD_SSU_DOWNLOAD_WAIT,
    OAD_SSU_PROGRAM,
    OAD_SSU_FINISHED,
    OAD_SSU_ERROR
} OAD_SSU_STATUS_E;
#endif /* #ifdef APPL_OAD_SSU_ENABLED */

#ifdef APPL_OAD_FOTA_ENABLED
typedef enum
{
    OAD_FOTA_SLEEP = 0,
    OAD_FOTA_START,
    OAD_FOTA_FILTER,
    OAD_FOTA_RUNNING,
    OAD_FOTA_UPDATE,
    OAD_FOTA_FINISHED,
    OAD_FOTA_PROGRAM,
    OAD_FOTA_STOP,
    OAD_FOTA_ERROR

} OAD_FOTA_STATUS_E;
#endif /* #ifdef APPL_OAD_FOTA_ENABLED */

typedef struct              /* Event: audio stream changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.

    #ifdef APPL_OAD_FOTA_ENABLED
    OAD_FOTA_STATUS_E   state;
    #endif /* #ifdef APPL_OAD_FOTA_ENABLED */

} SYS_EVT_OAD_S;

FAPI_SYS_HandleT    OAD_Open(OAD_OPEN_S* pOpenParams);
int32_t             OAD_Close(FAPI_SYS_HandleT handle);
int32_t             OAD_Start(FAPI_SYS_HandleT handle, uint32_t slotAddr);
int32_t             OAD_Stop(FAPI_SYS_HandleT handle);

#endif  /* APPL_OAD_ENABLED */

/*---------------------------------------------------------------------------*/
/* CI and CA common API                                                      */
/*---------------------------------------------------------------------------*/

#if defined(APPL_CI_ENABLED) || defined(APPL_CA_ENABLED)

#define CISYS_SLOTS_MAX        1 //RSRC_TS_NUMS
#define CISYS_MMISTR_MAX       MAX_CHAR_PER_LINE

#define CISYS_MODULENAME_MAXLEN     100
#define CISYS_VENDORNAME_MAXLEN     100

typedef struct
{
    RSRC_OP             opLst[RSRC_OP_NUMS];//!< Operations sorted by CI
                                            //!< usage priority (starting with
                                            //!< highest).
} CISYS_INIT_PARAMS_S;

typedef enum
{
    CISYS_MODULE_CI, //0
    CISYS_EMBEDDED_CA

} CISYS_MODULE_TYPE_E;

typedef enum
{
    CISYS_SLOT_UNAVAILABLE = 0,
    CISYS_SLOT_EMPTY, //1
    CISYS_SLOT_MODULE_INSERTED, //2
    CISYS_SLOT_MODULE_READY //3

} CISYS_SLOTSTATUS_E;

typedef enum
{
    CISYS_MMIEVT_SESSION_UNAVAILABLE = 0,   //!< A failure occured during session request
    CISYS_MMIEVT_NOT_AVAILABLE, //1            //!< Module is not able to provide MMI.
    CISYS_MMIEVT_SESSION_TERMINATED, //2        //!< Session has been terminated by module.
    CISYS_MMIEVT_OBJECT_AVL //3                //!< An MMI object is available.

} CISYS_MMIEVT_E;

typedef enum                /* High-level MMI object types */
{
    CISYS_MMIOBJ_NONE = 0,
    CISYS_MMIOBJ_ENQ, //1      //!< Enq object.
    CISYS_MMIOBJ_MENU, //2     //!< Menu object.
    CISYS_MMIOBJ_LIST //3      //!< List object.

} CISYS_MMI_OBJ_E;

typedef struct                      /* Event: CI slot status */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint8_t             slotIdx;    //!< CI slot index.
    CISYS_SLOTSTATUS_E  status;     //!< Current CI slot status.
    CISYS_MODULE_TYPE_E type;

} CISYS_EVT_STATUS_S;

typedef struct                      /* Event: MMI status */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    uint8_t             slotIdx;    //!< CI slot index.
    CISYS_MMIEVT_E      event;      //!< Type of event.
    CISYS_MMI_OBJ_E     objType;    //!< Type of object (used for event
                                    //!< CISYS_MMIEVT_OBJECT_AVL only).
    CISYS_MODULE_TYPE_E type;

} CISYS_EVT_MMI_S;

typedef struct
{
    uint8_t             numSlots;
    CISYS_SLOTSTATUS_E  slotStatus[CISYS_SLOTS_MAX];

} CISYS_STATUS_S;

typedef struct
{
    SYS_CHARSET         moduleNameCs; //0
    char_t              moduleName[CISYS_MODULENAME_MAXLEN]; //1
    SYS_CHARSET         vendorNameCs; //101
    char_t              vendorName[CISYS_VENDORNAME_MAXLEN]; //102
    //20
} CISYS_MODULEINFO_S;

typedef struct
{
    uint8_t             type;       //!< Type: Either LIST_TYPE or MENU_TYPE
    #ifndef APPL_CI_ENABLED
    T_LIST_MENUX        data;       //!< MMI list/menu data
    #else
    T_LIST_MENU         data;       //!< MMI list/menu data
    #endif

} CISYS_MMI_ITEM_S;

typedef struct
{
    int32_t             textLength;
    SYS_CHARSET         textCharSet;
    char_t              textBuf[CISYS_MMISTR_MAX];
    uint8_t             textMode;   //!< CLEARMODE or HIDEMODE.
    uint8_t             answerLen;  //!< Expected answer length.

} CISYS_ENQ_ITEM_S;

#endif

/*---------------------------------------------------------------------------*/
/* CI API                                                                    */
/*---------------------------------------------------------------------------*/

#ifdef APPL_CI_ENABLED

int32_t             CISYS_GetStatus         (CISYS_STATUS_S* pStatus);
int32_t             CISYS_GetModuleInfo     (uint8_t slotIdx,
                                             CISYS_MODULEINFO_S* pStatus);
int32_t             CISYS_MmiSessionRequest (uint8_t slotIdx);
int32_t             CISYS_MmiSessionClose   (uint8_t slotIdx);
int32_t             CISYS_MmiGetList        (uint8_t slotIdx,
                                             CISYS_MMI_ITEM_S** ppItem);
int32_t             CISYS_MmiGetMenu        (uint8_t slotIdx,
                                             CISYS_MMI_ITEM_S** ppItem);
int32_t             CISYS_MmiGetEnq         (uint8_t slotIdx,
                                             CISYS_ENQ_ITEM_S** ppItem);
int32_t             CISYS_MmiSendListCancel (uint8_t slotIdx);
int32_t             CISYS_MmiSendMenuAnswer (uint8_t slotIdx,
                                             uint8_t choiceIndex);
int32_t             CISYS_MmiSendMenuCancel (uint8_t slotIdx);
int32_t             CISYS_MmiSendEnqAnswer  (uint8_t slotIdx,
                                             const char_t* strBuf,
                                             uint8_t strLen);
int32_t             CISYS_MmiSendEnqCancel  (uint8_t slotIdx);
int32_t             CISYS_MmiGetAvailability(uint8_t slotIdx,
                                             CISYS_MMI_OBJ_E* pObjectType);

#endif /* APPL_CI_ENABLED */

/*----------------------------------------------------------------------------*/
/* CA API                                                                     */
/*----------------------------------------------------------------------------*/

#ifdef APPL_CA_ENABLED

typedef enum
{
    CA_TEXT_NAME,
    CA_TEXT_MONTH1,
    CA_TEXT_MONTH2,
    CA_TEXT_MONTH3,
    CA_TEXT_MONTH4,
    CA_TEXT_MONTH5,
    CA_TEXT_MONTH6,
    CA_TEXT_MONTH7,
    CA_TEXT_MONTH8,
    CA_TEXT_MONTH9,
    CA_TEXT_MONTH10,
    CA_TEXT_MONTH11,
    CA_TEXT_MONTH12,
    CA_TEXT_WAIT,

    CA_TEXT_MAIN_TITLE,
    CA_TEXT_MAIN_PRODORDER,
    CA_TEXT_MAIN_STATUS_SUB,
    CA_TEXT_MAIN_STATUS_EVT,
    CA_TEXT_MAIN_STATUS_TOK,
    CA_TEXT_MAIN_CHANGE_CA,
    CA_TEXT_MAIN_CHANGE_SIGN,
    CA_TEXT_MAIN_CHANGE_MAT,
    CA_TEXT_MAIN_SET_MODEM,
    CA_TEXT_MAIN_ABOUT,

    CA_TEXT_ENT_STATUS_SUB,
    CA_TEXT_ENT_STATUS_EVT,
    CA_TEXT_ENT_STATUS_TOK,
    CA_TEXT_ENT_NAME,
    CA_TEXT_ENT_START,
    CA_TEXT_ENT_END,
    CA_TEXT_ENT_DATE,
    CA_TEXT_ENT_TIME,
    CA_TEXT_ENT_LEFT,
    CA_TEXT_ENT_PURSE,
    CA_TEXT_ENT_BALANCE,
    CA_TEXT_ENT_STATUS_DEB,
    CA_TEXT_ENT_TOKENS,
    CA_TEXT_ENT_STATUS_CRED,
    CA_TEXT_ENT_LABEL,

    CA_TEXT_CHANGE_CA,
    CA_TEXT_PRESENT_CA,
    CA_TEXT_NEW_CA,
    CA_TEXT_CONFIRM_CA,
    CA_TEXT_CHANGE_SIGN,
    CA_TEXT_PRESENT_SIGN,
    CA_TEXT_NEW_SIGN,
    CA_TEXT_CONFIRM_SIGN,
    CA_TEXT_WRONG_CA,
    CA_TEXT_WRONG_SIGN,
    CA_TEXT_NO_CONFIRM,
    CA_TEXT_SIGN_PRE,
    CA_TEXT_SIGN_SUF,
    CA_TEXT_SIGN_LOCK,
    CA_TEXT_SIGN_SERVICE,
    CA_TEXT_CHANGED_CA,
    CA_TEXT_CHANGED_SIGN,

    CA_TEXT_CHANGE_MAT,
    CA_TEXT_MAT_CURR,
    CA_TEXT_MAT_AVL,
    CA_TEXT_MAT_G,
    CA_TEXT_MAT_PG,
    CA_TEXT_MAT_A,
    CA_TEXT_MAT_X,
    CA_TEXT_MAT_CA,

    CA_TEXT_INFO_SW,
    CA_TEXT_INFO_IF,
    CA_TEXT_INFO_SERIAL,
    CA_TEXT_INFO_SESS,
    CA_TEXT_INFO_LANG,
    CA_TEXT_INFO_CAID,

    CA_TEXT_NOCARD,
    CA_TEXT_WRONG_CONAX_CARD,
    CA_TEXT_WRONG_CARD,
    CA_TEXT_CARDERR,
    CA_TEXT_BAD_COMM,

    CA_TEXT_NOACCESS,
    CA_TEXT_ORDERING,
    CA_TEXT_ORDERPIN,
    CA_TEXT_START,
    CA_TEXT_MINLEFT,
    CA_TEXT_STARTPIN,
    CA_TEXT_BLACKOUT,
    CA_TEXT_MAT_LOCK,
    CA_TEXT_MAT_CARDLEVEL,
    CA_TEXT_MAT_PROGLEVEL,
    CA_TEXT_MATPIN,
    CA_TEXT_NETW_BLOCK,
    CA_TEXT_TOK_ACCESS,
    CA_TEXT_TOK_PER_MIN,
    CA_TEXT_TOKPIN,
    CA_TEXT_ENT_PURSE_BAL,
    CA_TEXT_TOK_PRICE,
    CA_TEXT_TOK_INSUF,

    CA_TEXT_NODECODE,
    CA_TEXT_EMPTY,
    CA_TEXT_NO_ENTRY,
    CA_TEXT_UNAVAILABLE

} CA_TEXT_E;

void                CONAX_CA_SetTextCallback(char_t*(*textCallback)
                                            (CA_TEXT_E strIdx));
int32_t             CONAX_CA_GetStatus(CISYS_STATUS_S* pStatus);
int32_t             CONAX_CA_GetModuleInfo(uint8_t slotIdx,
                                           CISYS_MODULEINFO_S* pStatus);
void                CONAX_CA_NoDecode(void);

int32_t             CONAX_CA_MmiSessionRequest(uint8_t slotIdx);
int32_t             CONAX_CA_MmiSessionClose(uint8_t slotIdx);
int32_t             CONAX_CA_MmiGetList(uint8_t slotIdx,
                                        CISYS_MMI_ITEM_S** ppItem);
int32_t             CONAX_CA_MmiGetMenu(uint8_t slotIdx,
                                        CISYS_MMI_ITEM_S** ppItem);
int32_t             CONAX_CA_MmiGetEnq(uint8_t slotIdx,
                                       CISYS_ENQ_ITEM_S** ppItem);
int32_t             CONAX_CA_MmiSendListCancel(uint8_t slotIdx);
int32_t             CONAX_CA_MmiSendMenuAnswer(uint8_t slotIdx,
                                               uint8_t choiceIndex);
int32_t             CONAX_CA_MmiSendMenuCancel(uint8_t slotIdx);
int32_t             CONAX_CA_MmiSendEnqAnswer(uint8_t slotIdx,
                                              const char_t* strBuf,
                                              uint8_t strLen);
int32_t             CONAX_CA_MmiSendEnqCancel(uint8_t slotIdx);
int32_t             CONAX_CA_MmiGetAvailability(uint8_t slotIdx,
                                                CISYS_MMI_OBJ_E*
                                                pObjectType);

#endif /* APPL_CA_ENABLED */

#endif

/*---------------------------------------------------------------------------*/
/* System startup                                                            */
/*---------------------------------------------------------------------------*/

#define SYS_DEV_SERIALNUM_LEN    21
#define SYS_DEV_FIRMWARE_LEN      9
#define SYS_DEV_MODEL_LEN        41

#if 0

/*!
*******************************************************************************
**
** \brief System startup parameter structure.
**
** The optional 'set default' handle functions are called whenever the
** corresponding information in Flash either:
**         - does not exist (e.g. after FS format)
**         - is corrupted
**         - is outdated
**
** The user application is given the opportunity to set custom default
** settings by using the handle functions below. The implementation of these
** function should use system API functions for setting default data.
**
** \attention The functions are called in order of declaration in the structure!
**
******************************************************************************/
typedef struct
{
    /* core system configuration */
    SYS_APP_PROFILE_E   appProfile;         //!< Application Profile

    uint32_t        lineBufSize;            //!< Size of internal line buffer
    uint32_t        asciiBufSize;           //!< Size of internal ASCII buffer
    uint32_t*       bootScreen;             //!< Optional MPEG I-frame shown at
                                            //!< startup.
    uint32_t        bootScreenSize;         //!< Size of boot screen in bytes.
    uint32_t*       radioScreen; //20       //!< Optional MPEG I-frame shown as
                                            //!< radio background.
    uint32_t        radioScreenSize; //24   //!< Size of radio screen in bytes.
    uint8_t*        welcomeJingle;          //!< Optional MPEG audio jingle
    uint32_t        welcomeJingleSize;      //!< Size of welcome jingle
    uint32_t        welcomeJingleDelay;     //!< Welcome jingle delay in ms.

    uint8_t         supportedAudioTypes;    //!< Or' ed bitfield of
                                            //!< SYS_AUDIO_TYPE_XXX.
    fbool_t         avPmtControlled;        //!< FTRUE: Start AV streams after
                                            //!< reception of first PMT. Stop
                                            //!< streams if no more PMT
                                            //!< available.
    SYS_CHARSET     altDefaultSiCharSet;    //!< Can be used to specify an
                                            //!< alternative default character
                                            //!< set for SI data as a workaround
                                            //!< for wrong encoding by the
                                            //!< broadcaster. If set to
                                            //!< SYS_CHARSET_UNDEFINED, standard
                                            //!< default SYS_CHARSET_TABLE0
                                            //!< will be used.

    /* Module configuration */
    #ifdef APPL_TTXT_ENABLED
    TTX_INIT_PARAMS_S   ttx;
    #endif                          /* APPL_TTXT_ENABLED */

    #ifdef APPL_MHEG_ENABLED
    MHEG_INIT_PARAMS_S  mheg;
    #endif                          /* APPL_MHEG_ENABLED */

    #ifdef APPL_OAD_ENABLED
    OAD_INIT_PARAMS_S   oad;
    #endif                          /* APPL_OAD_ENABLED */

    #ifdef APPL_SUBTT_ENABLED
    SUBTT_INIT_PARAMS_S subtt;
    #endif                          /* APPL_SUBTT_ENABLED */

    #ifdef APPL_PIP_ENABLED
    PIP_INIT_PARAMS_S   pip;
    #endif                          /* APPL_PIP_ENABLED */

    #ifdef APPL_JPEG_ENABLED
    JPEG_INIT_PARAMS_S  jpeg;
    #endif                          /* APPL_JPEG_ENABLED */

    #ifdef APPL_CI_ENABLED
    CISYS_INIT_PARAMS_S ci;
    #endif                          /* APPL_CI_ENABLED */

    //! optional handle for setting startup defaults
    void                (*setStartupDefaults) (void); //88

    //! optional handle for setting USALS defaults
    void                (*setUsalsDefaults) (void);

    /*! optional handle for setting sat configuration defaults
       (only for sat frontend configuration) */
    void                (*setSatDefaults) (void);

    //! optional handle for setting carrier list defaults
    void                (*setCarrierDefaults) (const SYS_NETWORK_S *
                                               network);

    //! optional handle for setting service list defaults
    void                (*setServListDefaults) (SLIST_TYPE listId);

    #ifdef APPL_HDD_ENABLED

    //! optional handle for setting HDD-related defaults
    void                (*setHddDefaults) (void); //112

    #endif /* APPL_HDD_ENABLED */

    #ifdef APPL_PVR_ENABLED

    //! optional handle for setting PVR-related defaults
    void                (*setPvrDefaults) (void); //116

    #endif /* APPL_PVR_ENABLED */

    #ifdef APPL_PARENTAL_ENABLED

    //! optional handle for setting parental control related defaults
    void                (*setParentalCtrlDefaults) (void); //120

    #endif /* APPL_PARENTAL_ENABLED */

    //! optional handle for application initialisation
    int32_t             (*appInit) (void); //124

    //! optional handle for receiving application events
    void                (*appEventReceived) (SYS_EVTTYPE_E type, //128
                                             const void *data);

    //! optional handle for receiving application events
    SYS_PWRMODE_E       (*appGetStartupOperationMode) (void); //132

    /* NOTE: callbacks will be called by the system software in the following
       order:

       setXXXDefaults
       ...
       setXXXDefaults

       appGetStartupOperationMode

       appInit

       while ( 1 )
       {
           appEventReceived
       }
     */
    //136
} SYS_CONFIGDAT_S;

typedef struct
{
    /* external hardware configuration */
    uint8_t         feNums;                 //!< (Max.) number of available
                                            //!< frontends
    RSRC_TS_IN      tsInput[RSRC_TS_NUMS];  //!< TS input
                                            //!< (one of _RSRC_TSIN_XXX).
    fbool_t         ciAvl[RSRC_TS_NUMS];    //!< FTRUE: TS input equipped
                                            //!< with CI slot.
    int Data_12; //12
    int Data_16; //16
    RSRC_CI_IN      ciInput[RSRC_TS_NUMS];  //!< CI input options (or'ed
                                            //!< bitfield of
                                            //!< FAPI_BOARD_RSRC_CIIN_XXX).
    fbool_t         usbHostOnly;            //!< If FTRUE, USB vbus will be
                                            //!< constantly powered and no
                                            //!< reconnect is necessary. No
                                            //!< other USB host shall be
                                            //!< connected in this case.
    int Data_28; //28
    //32
} SYS_HWCONFIG_S;

typedef struct
{
    uint32_t            major;
    uint32_t            minor;

} SYS_VERSION_S;

typedef             fbool_t(*SYS_OSDLAYERREQ_CB) (uint8_t layerIdx);

typedef struct
{
    OSDHANDLER_StartParamsT     params; //!< OSD Layer parameters.
    OSDHANDLER_ColourModeT      colourMode;
    OSDHANDLER_OsdLayoutEnumT   layoutMode;
    uint32_t                    fillColour;
    uint32_t                    layerIdx;   //!< Requested OSD Layer index.

} SYS_OSDLAYER_S;

typedef struct
{
    SYS_DEV_STAT_S      status;     //!< Device status.
    SYS_SIZE_S          capacity;   //!< Device capacity
    SYS_SIZE_S          freeSpace;  //!< Free space on mounted File system.
    SYS_CHARSET         serialNumberCs;
    char_t              serialNumber[SYS_DEV_SERIALNUM_LEN];
    SYS_CHARSET         firmwareRevCs;
    char_t              firmwareRev[SYS_DEV_FIRMWARE_LEN];
    SYS_CHARSET         modelNumberCs;
    char_t              modelNumber[SYS_DEV_MODEL_LEN];

} SYS_DEVICE_INFO_S;

int32_t             SYS_Start                (SYS_CONFIGDAT_S* pConfigDat);
const SYS_CONFIGDAT_S* SYS_GetConfig         (void);
const SYS_HWCONFIG_S*  SYS_GetHWConfig       (void);
int32_t             SYS_GetVersion           (SYS_VERSION_S* pVersion);
int32_t             SYS_RestoreDefaultState  (fbool_t keepUserData);
int32_t             SYS_SetOperationMode     (SYS_PWRMODE_E reqMode);
SYS_PWRMODE_E       SYS_GetOperationMode     (void);
int32_t             SYS_Reboot               (void);
int32_t             SYS_GetPresentationState (SYS_PRESSTATE_E* state);
int32_t             SYS_StopPresentation     (RSRC_OP op, fbool_t freeze);
int32_t             SYS_SetAppProfile        (SYS_APP_PROFILE_E profile);
SYS_APP_PROFILE_E   SYS_GetAppProfile        (void);
int32_t             SYS_GetOsdLayer          (const SYS_OSDLAYER_S* params,
                                              SYS_OSDLAYERREQ_CB reqCb);
int32_t             SYS_GiveOsdLayer         (uint32_t index);
int32_t             SYS_ModifyOsdLayer       (const SYS_OSDLAYER_S* params);
fbool_t             SYS_OsdLayerAvailable    (uint32_t index);
int32_t             SYS_GetOsdLayerMode      (uint32_t index,
                                              uint32_t* layerMode);
int32_t             SYS_MallocOsdMemory      (const uint32_t width,
                                              const uint32_t height,
                                              const uint32_t bpp,
                                              uint8_t** basePtr,
                                              uint8_t** osdPtr,
                                              fbool_t clearMem);
int32_t             SYS_SetPaletteColor      (uint32_t layer,
                                              uint16_t paletteIndex,
                                              uint32_t color);
int32_t             SYS_GetPaletteColor      (uint32_t layer,
                                              uint16_t paletteIndex,
                                              uint32_t* pColor);

int32_t             SYS_DevicePreAccess      (SYS_DEVICE_E device);
int32_t             SYS_DeviceFormat         (SYS_DEVICE_E device);
int32_t             SYS_DeviceMkdir          (SYS_DEVICE_E device,
                                              const char_t* dirName);
int32_t             SYS_DeviceMkdirByPath    (const char_t* dirPath);
int32_t             SYS_DeviceRmdir          (SYS_DEVICE_E device,
                                              const char_t* dirName);
int32_t             SYS_DeviceGetStatus      (SYS_DEVICE_E device,
                                              SYS_DEV_STAT_S* status);
int32_t             SYS_DeviceGetInfo        (SYS_DEVICE_E device,
                                              SYS_DEVICE_INFO_S* pInf);
int32_t             SYS_DeviceGetLabel       (SYS_DEVICE_E device,
                                              char_t* pStrBuf,
                                              uint32_t bufSize);
int32_t             SYS_DeviceGetPowerMode   (SYS_DEVICE_E device,
                                              SYS_DEV_PWRMODE_E* pMode);
int32_t             SYS_DeviceGetByPath      (const char_t* path,
                                              SYS_DEVICE_E* device,
                                              uint16_t* pathOffset);
int32_t             SYS_DeviceGetMountPath   (SYS_DEVICE_E device, char_t* buf,
                                              uint32_t bufSize);
int32_t             SYS_DeviceSetAccessStatus(SYS_DEVICE_E device,
                                              fbool_t access);
int32_t             SYS_DeviceSetExternalAccess(SYS_DEVICE_E device,
                                              fbool_t access);
uint32_t            SYS_DeviceGetFreekBytes  (SYS_DEVICE_E device);

void                SYS_SizeClear            (SYS_SIZE_S* size);
void                SYS_SizeAdd              (SYS_SIZE_S* size,
                                              uint32_t kbytes, uint32_t bytes);
fbool_t             SYS_SizeIsSmaller        (const SYS_SIZE_S* size1,
                                              const SYS_SIZE_S* size2);

#ifdef APPL_BOOTLDR_ENABLED

int32_t             SYS_GetBootInfo(FAPI_BOOT_InfoT* pBoot);
int32_t             SYS_GetBootSlotInfo(uint32_t slotIdx,
                                        BOOTER_FlashSlotHeaderT* pSlot);
int32_t             SYS_WriteBootImage(SYS_DEVICE_E device);

#endif /* APPL_BOOTLDR_ENABLED */

#endif

#define STR_CHAR_LINEBREAK      '\n'    //!< Used for both 8 and 16 bit
                                        //!< character sets.
#define STR_CHAR_BLANK          ' '     //!< Used for both 8 and 16 bit
                                        //!< character sets.

#if 0

uint32_t         STR_Copy               (SYS_STRING_S src, char_t* dstBuf,
                                         SYS_CHARSET* dstCs, SYS_CHARSET reqCs,
                                         uint32_t maxBytes);
uint32_t         STR_NCopy              (SYS_STRING_S src, char_t* dstBuf,
                                         SYS_CHARSET* dstCs, SYS_CHARSET reqCs,
                                         uint32_t maxBytes, uint32_t numBytes);
SYS_STRING_S     STR_Build              (SYS_CHARSET set, const char_t* str);
#endif
SYS_STRING_S     STR_GetASCII           (const char_t* str);
#if 0
void             STR_Clear              (SYS_STRING_S* string);
uint32_t         STR_Len                (SYS_STRING_S string);
int32_t          STR_Cmp                (SYS_STRING_S str1, SYS_STRING_S str2);
fbool_t          STR_IsEmpty            (SYS_STRING_S string);
void             STR_Terminate          (char_t* pBuf, uint32_t bufSize);
uint32_t         STR_Trim               (SYS_STRING_S string,
                                         uint32_t sizeInBytes);
void             STR_ReplaceLineBreaks  (SYS_STRING_S string,
                                         SYS_CHAR replaceChar,
                                         uint32_t bufSize);
uint32_t         STR_GetLineEnd         (SYS_STRING_S string);
#endif
uint32_t         STR_CharGet            (SYS_STRING_S string,
                                         uint32_t byteOffset, SYS_CHAR* chr);
#if 0
uint32_t         STR_CharSet            (SYS_STRING_S string,
                                         uint32_t byteOffset, SYS_CHAR chr);
uint32_t         STR_CharRemove         (SYS_STRING_S string,
                                         uint32_t byteOffset);

int32_t          STR_BufOpen            (SYS_STRBUF_S* pBuf, uint32_t bufSize,
                                         char_t* pMem);
int32_t          STR_BufClose           (SYS_STRBUF_S* pBuf);
int32_t          STR_BufClear           (SYS_STRBUF_S* pBuf);
uint32_t         STR_BufAppend          (SYS_STRBUF_S* pBuf, uint32_t bytePos,
                                         SYS_STRING_S string,
                                         SYS_CHARSET reqCs);
uint32_t         STR_BufNAppend         (SYS_STRBUF_S* pBuf, uint32_t bytePos,
                                         SYS_STRING_S string,
                                         SYS_CHARSET reqCs, uint32_t numBytes);
uint32_t         STR_BufAppendChar      (SYS_STRBUF_S* pBuf, uint32_t bytePos,
                                         SYS_CHAR chr, SYS_CHARSET reqCs);
uint32_t         STR_BufInsertChar      (SYS_STRBUF_S* pBuf, uint32_t bytePos,
                                         SYS_CHAR chr, SYS_CHARSET reqCs);
SYS_STRING_S     STR_IntASCIIStr        (void);
char_t*          STR_IntASCIIBuf        (void);
SYS_STRING_S     STR_IntLineStr         (void);

/*---------------------------------------------------------------------------*/
/* PVR API                                                                   */
/*---------------------------------------------------------------------------*/

#endif

#if 1//def APPL_PVR_ENABLED

#if 0

#define PVR_LIST_FLAG_SCRAMBLED   0x00000001    //!< Set if some contents are

#endif

typedef enum
{
    PLAY_TYPE_NONE = 0,     //!< Playback not active.
    PLAY_TYPE_NORMAL,       //!< Normal playback of recorded file.
    PLAY_TYPE_TIMESHIFT     //!< Time-shifted playback of currently
                            //!< recorded file.
} PVRSYS_PLAY_TYPE_E;

typedef enum
{
    PLAY_STATUS_INACTIVE = 0,   //!< Playback is not active.
    PLAY_STATUS_STOP,           //!< Playback has been stopped.
    PLAY_STATUS_NORMAL,         //!< Playback is running in normal speed.
    PLAY_STATUS_PAUSE,          //!< Playback is paused.
    PLAY_STATUS_FAST,           //!< Playback is fast forward/backward.
    PLAY_STATUS_SLOW            //!< Playback is slow forward/backward.

} PVRSYS_PLAY_STATUS_E;

#if 0

typedef enum
{
    PVR_JUMP_DATA_REL = 0,      //!< Relative jump value is in data chunks.
    PVR_JUMP_DATA_ABS,          //!< Absolute jump value is in data chunks.
    PVR_JUMP_TIME_REL,          //!< Relative jump value is in time (seconds)
    PVR_JUMP_TIME_ABS,          //!< Absolute jump value in time (seconds)
    PVR_JUMP_IFRAME_AND_LOCK

} PVRSYS_JUMP_MODE_E;

#endif

typedef enum
{
    REC_STATUS_INACTIVE = 0,    //!< Not recording.
    REC_STATUS_STOP,            //!< Recording has been automatically stopped
                                //!< (not by user action).
    REC_STATUS_ACTIVE           //!< Recording.

} PVRSYS_REC_STATUS_E;

typedef enum
{
    REC_TYPE_NONE = 0,      //!< Recording not active
    REC_TYPE_NORMAL,        //!< Instant/timer recording
    REC_TYPE_BACKGROUND,    //!< Background recording
    REC_TYPE_DESCRREC       //!< Descrambled recording of an existing
                            //!< recording.
} PVRSYS_REC_TYPE_E;


typedef struct
{
    PVRSYS_REC_STATUS_E status; //0
    PVRSYS_REC_TYPE_E   type; //4
    RECTIMER_IDX        timerIdx; /*8*/  //!< Record's timer ID (may be RECTIMER_INVALID)
    SLIST_SERVIDX       servIdx; /*12*/   //!< Service currently recorded.
    //16
} PVRSYS_STATREC_S;


typedef struct
{
    PVRSYS_PLAY_STATUS_E status; /*0*/     //!< Playback status/mode.
    PVRSYS_PLAY_TYPE_E   type; /*4*/      //!< Playback type: normal, timeshift, ...
    int16_t              speedIndex; /*8*/ //!< Playback speed index:
                                     //!< fast/slow modes only.
    uint16_t             speedFactor; /*10*/ //!< Speed factor.
    int8_t               recIdx; /*12*/    //!< Record reference (timeshift type only).
    PVRLST_IDX           lstIdx; /*14*/    //!< Record list reference.
    //16
} PVRSYS_STATPLAY_S;


typedef struct
{
    uint8_t             numActiveRecs; //0
    int8_t              bkgrRecIdx; /*1*/ //!< index of background recording
                                    //!< (-1: none)
    PVRSYS_STATREC_S    rec[/*RSRC_REC_MAX*/3]; //4
    int fill_52[8]; //52
    PVRSYS_STATPLAY_S   play; //84
    //100
} PVRSYS_STATUS_S;

#if 0

typedef enum
{
    PVR_PLAY_START = 0,
    PVR_PLAY_STOP,
    PVR_PLAY_MODECHANGE

} PVRSYS_PLAY_EVENT_E;

typedef enum
{
    PVR_REC_START = 0,
    PVR_REC_STOP

} PVRSYS_REC_EVENT_E;

typedef struct              /* Event: PVR playback status changed */
{
    SYS_EVTHDR_S        hdr;            //!< Common event header.
    PVRSYS_PLAY_EVENT_E action;
    PVRSYS_STATPLAY_S   mode;
    fbool_t             userInitiated;  //!< If FTRUE: Change of status is
                                        //!< (indirectly) caused by a user
                                        //!< request via API. If FFALSE:
                                        //!< status has changed because of an
                                        //!< event like device access error.

} SYS_EVT_PVRPLAY_S;

typedef struct              /* Event: PVR record status changed */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    PVRSYS_REC_EVENT_E  action;
    uint8_t             recIdx;

} SYS_EVT_PVRREC_S;

typedef struct              /* Event: PVR playback event occurred */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    PVR_PlayNotifyT     event;

} SYS_EVT_PLAYEVT_S;

typedef struct              /* Event: PVR record event occurred */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    PVR_RecordNotifyT   event;
    uint8_t             recIdx;

} SYS_EVT_RECEVT_S;

typedef struct
{
    SYS_DEVICE_E        device;     //!< Device where rec. resides on.
    SLIST_ITEM_S        service;    //!< Info on recorded service.
    SYS_CHARSET         nameCharSet;
    char_t              name[PVR_NAME_LENGTH];  //!< Recording title.
    SYS_TIME_S          startTime;  //!< Record start time (UTC).
    SYS_DATE_S          startDate;  //!< Record start date (UTC).
    uint32_t            size;       //!< File size in PVR data chunks.
    SYS_TIME_S          duration;   //!< Record duration.
    PVRSYS_REC_TYPE_E   type;       //!< Type of recording,
                                    //!< REC_TYPE_NORMAL or
                                    //!< REC_TYPE_BACKGROUND.
    int32_t             lastStop;   //!< Last stop position (-1: not
                                    //!< available).
    SLIST_CA_INFO_S     caInfo;     //!< CA system information.
    uint32_t            flags;      //!< Misc. flags
                                    //!< (see PVR_LIST_FLAG_XXX).
    uint32_t            reserved[3];//!< Reserved for future usage.
                                    //!< (set to 0)

} PVR_LIST_ITEM_S;

#define PVRSYS_PLAYMODE_FF_NUMS      8  //!< Number of fast forward modes
                                        //!< (index 0...).
#define PVRSYS_PLAYMODE_FR_NUMS      8  //!< Number of fast forward modes
                                        //!< (index 0...).
#define PVRSYS_PLAYMODE_SF_NUMS      3  //!< Number of slow forward modes
                                        //!< (index 0...).

#ifdef APPL_EDITMARKER_ENABLED

#define MAX_BLOCK_MARKERS       50

/* structure that describes block markers for each recording */
/* markers are given as units of data chunks */
typedef struct
{
    uint8_t             numBlockMarker;     //!< number of valid block markers
    FDVR_PLAY_MARKER_S  marker[MAX_BLOCK_MARKERS];

} PVR_BLOCK_MARKER_ITEM_S;

#endif /* APPL_EDITMARKER_ENABLED */

typedef enum /* Options for starting timeshift playback */
{
    PVRSYS_TIMESHIFT_BEGIN = 0,  //!< Start from beginning in pause.
    PVRSYS_TIMESHIFT_CURRRECPOS, //!< Start from current rec. position in pause.
    PVRSYS_TIMESHIFT_END         //!< Start from end in fast (backwards) speed.

} PVRSYS_TIMESHIFTPOS_E;


int32_t     PVRSYS_GetStatus   (PVRSYS_STATUS_S* status);
fbool_t     PVRSYS_IsRecording (void);

int32_t     PVRSYS_RecordStart  (SYS_DEVICE_E device, const char_t* dirName,
                                 SYS_STRING_S recordName,
                                 SLIST_SERVIDX servIdx,
                                 RECTIMER_IDX timerIdx, uint8_t* recIdx);
int32_t     PVRSYS_RecordBackgroundStart  (SYS_DEVICE_E device,
                                           const char_t* dirName,
                                           SYS_STRING_S recordName,
                                           SLIST_SERVIDX servIdx,
                                           uint32_t maxFileSize,
                                           uint8_t* recIdx);
int32_t     PVRSYS_RecordStop             (int8_t recIdx, fbool_t keepFile);
int32_t     PVRSYS_RecordGetInfo          (int8_t recIdx, PVR_LIST_ITEM_S* pInf,
                                           PVR_RecordInfoT* pParams);

int32_t     PVRSYS_PlayStartByPath  (const char_t* path, uint32_t filePos,
                                     const SYS_PLAYPREF_S* pPref,
                                     const RSRC_CFG_S* cfg);
int32_t     PVRSYS_PlayStart        (int16_t lstPos, uint32_t filePos,
                                     const SYS_PLAYPREF_S* pPref,
                                     const RSRC_CFG_S* cfg);
int32_t     PVRSYS_PlayStartByIndex (PVRLST_IDX idx, uint32_t filePos,
                                     const SYS_PLAYPREF_S* pPref,
                                     const RSRC_CFG_S* cfg);
int32_t     PVRSYS_PlayStop         (void);
int32_t     PVRSYS_PlayRecStart     (int8_t recIdx, PVRSYS_TIMESHIFTPOS_E pos,
                                     PVRSYS_PLAY_STATUS_E mode,
                                     int16_t speedIndex,
                                     const SYS_PLAYPREF_S* pPref,
                                     const RSRC_CFG_S* cfg);
int32_t     PVRSYS_PlaySetMode      (PVRSYS_PLAY_STATUS_E mode, int16_t
                                     speedIndex);
int32_t     PVRSYS_PlayJump         (int32_t jumpValue,
                                     PVRSYS_JUMP_MODE_E jumpMode);
int32_t     PVRSYS_PlaySeekToStart  (void);
int32_t     PVRSYS_PlayGetInfo      (PVR_PlayInfoT* pInf);
int32_t     PVRSYS_PlayGetStreamInfo(SYS_STREAM_INFO_S* pInf);

int32_t     PVRSYS_PlayGetAudioStream(int8_t* index);
int32_t     PVRSYS_PlaySetAudioStream(int8_t index);
int32_t     PVRSYS_PlayGetSPDIFStatus(AV_SPDIF_STATUS_E* type, int8_t* index);
int32_t     PVRSYS_PlaySetSPDIFStatus(AV_SPDIF_STATUS_E type, int8_t index);

#ifdef APPL_CI_ENABLED

int32_t     PVRSYS_DescrRecordStart(SYS_DEVICE_E device,
                                 SYS_STRING_S dirName,
                                 SYS_STRING_S recordName,
                                 PVRLST_IDX idx,
                                 RECTIMER_IDX timerIdx,
                                 uint8_t* recIdx);

#endif /* APPL_CI_ENABLED */

int32_t     PVRSYS_DeviceRemoved(SYS_DEVICE_E device);
int32_t     PVRSYS_DeviceSyncDirectory(SYS_DEVICE_E device,
                                            const char_t* path);
fbool_t     PVRSYS_DeviceIsSynced(SYS_DEVICE_E device);

int32_t     PVR_LIST_GetItemByPath(const char_t* path,
                                           PVR_LIST_ITEM_S* pItem);

int32_t     PVR_LIST_GetNumItems(int16_t* numItems);
int32_t     PVR_LIST_GetItem(int16_t lstPos,
                                     PVR_LIST_ITEM_S* pItem,
                                     int8_t* recIdx);
int32_t     PVR_LIST_GetItemByIndex(PVRLST_IDX idx,
                                            PVR_LIST_ITEM_S* pItem,
                                            int8_t* recIdx);
int32_t     PVR_LIST_GetListPosByRecIdx(int8_t recIdx,
                                                int16_t* lstPos);

int32_t     PVR_LIST_ReplaceItem(int16_t lstPos,
                                         const PVR_LIST_ITEM_S* pItem);
int32_t     PVR_LIST_RemoveItem(int16_t lstPos);
int32_t     PVR_LIST_GetIndex(int16_t lstPos, PVRLST_IDX* idx);

#ifdef APPL_EDITMARKER_ENABLED

/* function for handling of edit markers */
int32_t     PVR_PlayInitMarker  (FDVR_PLAY_MARKER_S* pMarker,
                                 uint32_t* pNumMarker);
int32_t     PVR_PlayEnableMarker(fbool_t enable);
int32_t     PVR_PlaySetMarker   (FDVR_PLAY_MARKER_TYPE_E type,
                                 uint32_t maxNumMarker);
int32_t     PVR_PlayRemoveMarker(uint32_t maxNumMarker,
                                 uint32_t* pPosition);

int32_t     PVR_PlayGetBlockData(PVR_BLOCK_MARKER_ITEM_S* pBlockData);
int32_t     PVR_PlaySetBlockData(PVR_BLOCK_MARKER_ITEM_S* pBlockData);
int32_t     PVR_PlayOpenAndValidateBlockMarker(PVRLST_IDX idx);

#endif /* APPL_EDITMARKER_ENABLED */

#endif

#endif /* APPL_PVR_ENABLED */

#if 0

/*---------------------------------------------------------------------------*/
/* HDD utility functions                                                     */
/*---------------------------------------------------------------------------*/

#ifdef APPL_HDD_ENABLED

#define HDD_TIMEOUT_READYWAIT   15000    //!< ready wait timeout in ms

int32_t             HDD_Spinup(fbool_t waitForSpinup, uint32_t timeoutInMs);
int32_t             HDD_SetSpindownTime(uint32_t timeInSeconds);
int32_t             HDD_GetSpindownTime(uint32_t* timeInSeconds);

#endif /* APPL_HDD_ENABLED */

#endif

/*---------------------------------------------------------------------------*/
/* USB utility functions                                                     */
/*---------------------------------------------------------------------------*/

#if 1//def APPL_USB_ENABLED

typedef enum
{
    USB_MSC_ACCESS_UNKNOWN = 0,     //!< Access rights not yet evaluated.
    USB_MSC_ACCESS_DENIED, /*1*/         //!< Access has been denied, HDD currently
                                    //!< in use.
    USB_MSC_ACCESS_GRANTED /*2*/         //!< Access has been granted, HDD will
                                    //!< be locked for internal usage.

} USB_MSC_ACCESS_STATUS;


typedef enum
{
    USB_DEVTYPE_NONE = -1,                //!< Unknown device type.
    USB_DEVTYPE_HOST = 0,                 //!< Host device (e.g. PC).
    USB_DEVTYPE_FUNCTION /*1*/                 //!< Function device (e.g. memory
                                          //!< stick).
} USB_DEVICE_TYPE_E;

typedef struct
{
    fbool_t                 connected;    //!< Connection status of USB host.
    USB_DEVICE_TYPE_E       devType;      //!< Type of connected or
                                          //!< disconnected device.
    USB_MSC_ACCESS_STATUS   accessStatus; //!< Access status of USB host device,
                                          //!< only valid if devtype =
                                          //!< USB_DEVTYPE_HOST.
} USB_CONNECT_STATUS_S;

typedef struct              /* Event: USB function mode: connection status */
{
    SYS_EVTHDR_S            hdr;          //!< Common event header.
    USB_CONNECT_STATUS_S    status;

} SYS_EVT_USB_CONNECT_S;

#if 0

int32_t             USB_GetConnectionStatus (USB_CONNECT_STATUS_S* pStatus);

#endif

#endif /* APPL_USB_ENABLED */

#if 0

#ifdef APPL_FILEOP_ENABLED

#define FILEOP_CLUSTERSIZE    32        //!< Expected file system cluster size in
//!< KBytes, used for internal calculations
//!< only

typedef enum
{
    FILEOP_TYPE_NONE = 0,   //!< No operation.
    FILEOP_TYPE_COPY,       //!< Copy operation (source file is kept).
    FILEOP_TYPE_MOVE,       //!< Move operation (source file is removed).
    FILEOP_TYPE_DELETE,     //!< Delete operation.
    FILEOP_TYPE_GETPROP     //!< Information on file/directory is collected.

} FILEOP_TYPE_E;

typedef enum
{
    FILEOP_STATUS_IDLE = 0, //!< Operation is inactive.
    FILEOP_STATUS_BUSY,     //!< Operation is active.
    FILEOP_STATUS_ABORTED,  //!< Operation has been aborted.
    FILEOP_STATUS_FINISHED, //!< Operation has successfully finished.
    FILEOP_STATUS_ERROR     //!< Operation has finished with error.

} FILEOP_STATUS_E;

typedef struct
{
    uint16_t            fileCurr;    //!< Index of current file being processed.
    uint16_t            fileMax;     //!< Number of files to be processed.
    SYS_SIZE_S          fileDataCurr;//!< Data in current file already
                                     //!< processed.
    SYS_SIZE_S          fileDataMax; //!< Size of current file.
    SYS_SIZE_S          allDataCurr; //!< Overall data already processed.
    SYS_SIZE_S          allDataMax;  //!< Overall data length
                                     //!< (sum of all file sizes).
    SYS_SIZE_S          sizeOnMedium;//!< Size on target medium based on given
                                     //!< cluster size (in bytes).
} FILEOP_PROGRESS_S;

typedef struct
{
    fbool_t             active;     //!< FTRUE: operation is currently active.
    FILEOP_TYPE_E       type;       //!< Type of operation.
    FILEOP_STATUS_E     status;     //!< Operation status.
    FILEOP_PROGRESS_S   progress;   //!< Operation progress.
    SYS_DEVICE_E        srcDevice;  //!< Source device of operation.
    SYS_DEVICE_E        dstDevice;  //!< Destination device of operation.
    int32_t             error;      //!< Error code, valid if status is
                                    //!< FILEOP_STATUS_ERROR.

} FILEOP_INFO_S;

typedef struct              /* Event: File operation progress */
{
    SYS_EVTHDR_S        hdr;        //!< Common event header.
    FILEOP_INFO_S       data;

} FILEOP_EVT_S;

int32_t             FILEOP_GetInfo      (FILEOP_INFO_S* pStatus);
int32_t             FILEOP_Copy         (char_t* src, char_t* dst);
int32_t             FILEOP_Move         (char_t* src, char_t* dst);
int32_t             FILEOP_Delete       (char_t* path);
int32_t             FILEOP_Abort        (void);
int32_t             FILEOP_Reset        (void);
int32_t             FILEOP_GetProperties(char_t* path);

#endif /* APPL_FILEOP_ENABLED */


#ifdef APPL_FRONTPANEL_ENABLED
int32_t  FP_LiveSet(const SYS_SERVLOC_S * servLoc,
                    const SLIST_ITEM_S * service);
int32_t  FP_LiveClear(void);
int32_t  FP_ControlSymbol(uint32_t type, fbool_t enable);
int32_t  FP_UpdateServiceNumber(uint32_t num);
int32_t  FP_SetPowerMode(BOARDS_FrontPanel_CtrlCodeT mode);
uint16_t FP_GetKey(void);
#endif

#endif

#ifdef __cplusplus
}
#endif


#endif /* _SYS_API_H_ */
/******************************************************************************/
/* END OF FILE */
