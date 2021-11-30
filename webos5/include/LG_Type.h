/**
* Copyright (c) 2020 LG Electronics, Inc.
*
* Unless otherwise specified or set forth in the NOTICE file, all content,
* including all source code files and documentation files in this repository are:
* Confidential computer software. Valid license from LG required for
* possession, use or copying.
*/

/**
 *@file         LG_Type.h
 *@brief        This is a header file that defines common types
 *@author       daehyun.park@lge.com
 *@date         2020-03-19
 *@version      1.0.0 Initial vesion
 */


#ifndef LG_TYPE_H
#define LG_TYPE_H


/**
 *@brief	Enumeration for common return value
 */
enum Status
{
	LG_ERROR = -1,      ///< Error
	LG_OK = 0,          ///< OK
	LG_SUCCESS = LG_OK, ///< SUCCESS
	LG_INVALID_STATE,   ///< Invalid state (playback APIs)
	LG_TIMEOUT,         ///< Timeout
	LG_BUFFER_FULL,     ///< Buffer full (only Feed)
	LG_NO_DRM,          ///< No Drm Object

	// <TODO> value 에서 LG_ 를 제거.
};


/**
 *@brief	The type of ES stream
 */
enum LG_ELEMENTARY_STREAM
{
	ES_VIDEO = 0x01,    ///< video stream
	ES_AUDIO = 0x02,    ///< audio stream
	ES_SUBTITLE = 0x04  ///< subtitle stream
};
typedef LG_ELEMENTARY_STREAM estream_t;


/**
 *@brief    The mode of Encryption
 */
enum LG_ENCRYPTION_MODE {
	ENCRYPTION_MODE_NONE,         ///< Non Drm
    ENCRYPTION_MODE_AESCTR_CENC,  ///< AESCTR mode's cenc : PlayReady Default
	// <NOTE> Not supported in PlayReady 3.0 or lower.
	// ENCRYPTION_MODE_AESCTR_CENS,  ///< AESCTR mode's cens : Not supported PlayReady
    // ENCRYPTION_MODE_AESCBC_CBC1,  ///< AESCBC mode's cbc1 : Not supported PlayReady
    ENCRYPTION_MODE_AESCBC_CBCS = 4  ///< AESCBC mode's cbcs : Supported from PlayReady 4.0
};
typedef LG_ENCRYPTION_MODE encryption_t;


/**
 *@brief	A structure for subsample
 *@see		struct AccessUnit
 */
struct LG_Subsample
{
	uint32_t    clearBytes;      ///< Number of clear bytes in subsample
	uint32_t    encryptedBytes;  ///< Number of encrypted bytes in subsample
};
typedef LG_Subsample subsample_t;


#if 0
/**
 *@brief	A structure of The access unit for ES stream
 */
struct LG_AccessUnit
{
	// common data
	estream_t     type;            ///< type of es stream
	uint8_t*      data;            ///< A pointer of data
	uint32_t      size;            ///< size of date
	int64_t       pts;             ///< pts of media
	encryption_t  mode;            ///< encryption mode

	// encryption data
	uint8_t*      kid;             ///< key ID identifying the key with which this data is encrypted
	uint32_t      kidSize;         ///< key ID size
	uint8_t*      iv;              ///< intializaion vector
	uint32_t      ivSize;          ///< iv size
	subsample_t*  subsample;       ///< Array of clear/encypted pairs of the data.
	uint32_t      subsampleSize;   ///< Number of clear/encypted pairs.
};
#endif

#endif // LG_TYPE_H

