/**
* Copyright (c) 2020 LG Electronics, Inc.
*
* Unless otherwise specified or set forth in the NOTICE file, all content,
* including all source code files and documentation files in this repository are:
* Confidential computer software. Valid license from LG required for
* possession, use or copying.
*/

/**
 *@file         LG_EsPlayer.h
 *@brief        This is a header file that defines the LG_EsPlayer class.
 *@author       daehyun.park@lge.com
 *@date         2020-03-04
 *@version      1.0.0 Initial vesion
 */


#ifndef LG_ESPLAYER_H
#define LG_ESPLAYER_H

#include <memory>

#include "LG_Type.h"
#include "LG_Drm.h"


/**
 *@brief	The event type for ES Player callback event
 */
enum LG_ESPLAYER_EVENT
{
	LG_ESPLAYER_EVENT_UNKNWON = 0,          ///< undefined event
	LG_ESPLAYER_EVENT_CURRENT_TIME,         ///< current Time

	// event for async member function
	LG_ESPLAYER_EVENT_LOAD_DONE,            ///< Load() is done.
	LG_ESPLAYER_EVENT_UNLOAD_DONE,          ///< Unload() is done.
	LG_ESPLAYER_EVENT_PLAY_DONE,            ///< Play() is done.
	LG_ESPLAYER_EVENT_PAUSE_DONE,           ///< Pause() is done.
	LG_ESPLAYER_EVENT_SEEK_DONE,            ///< Seek() is done.
	LG_ESPLAYER_EVENT_END_OF_STREAM,        ///< PushEos() is done.

	// event for buffer
	LG_ESPLAYER_EVENT_BUFFER_FULL,          ///< The buffer is full.
	LG_ESPLAYER_EVENT_BUFFER_LOW,           ///< The buffer is low.
	LG_ESPLAYER_EVENT_FRAME_DROP,           ///< The frame is droped.

	// evert for error
	LG_ESPLAYER_EVENT_ERROR,                ///< The es player is broken.

	/**
	 *@brief	Resource released
	 *@details 	This event is occured when a old instance have released resources by new instance.\n
	  			If a old instance is receiving this event, then old instance should call unload to free occupied es player.
	 */
	LG_ESPLAYER_EVENT_RESOURCE_RELEASED,    ///< The es player is broken.

	/**
	 *@brief	Resource allocation error
	 *@details 	This event occurs when LG platform fails to allocate media resource\n
				when occupied resources have not been released by the other instance.
	 */
	LG_ESPLAYER_EVENT_ALLOCATION_FAILURE,   ///< The es player is broken.
};


/**
 *@brief	Enumeration for es player state
 *@details	State diagram of LG es player
 *
 *       	+---->-----[UNLOADED]
 *       	^             |
 *       	|           Load() : <NOTE> Feed () can be used after calling Load ()
 *       	|             |
 *       	|             V
 *       	+<-unload()--[LOADED/PAUSED]--Seek()->[current State]
 *       	|             |     ^                   |
 *       	^           Play()  |                   ^
 *       	|             |   Pause()               |
 *       	|             V     |                   |
 *       	+<-unload()--[PLAYING]--->---Seek()-->--+
 *
 *       	<NOTE> [SEEK] state automatically returns to  the previous state
 *        	       Feeding is possible after the load() is returned.
 */
enum LG_ESPLAYER_STATE
{
	LG_ESPLAYER_UNLOADED,    ///< Unloaded state (idle/stop)
	LG_ESPLAYER_LOADED,      ///< Loaded state
	LG_ESPLAYER_PLAYING,     ///< Playing state
	LG_ESPLAYER_PAUSED,      ///< Paused state
	LG_ESPLAYER_EOS          ///< End of Stream state (option)
};


/**
 *@brief	Enumeration for audio/video codec
 */
enum LG_MEDIA_CODEC_FORMAT
{
	CODEC_FORMAT_NONE = 0,                      ///< None

	// Video
	CODEC_FORMAT_H264              = 0x1100,   ///< H.264/AVC codec
	CODEC_FORMAT_H265              = 0x1200,   ///< H.265/HEVC codec
	CODEC_FORMAT_H265_DOLBY_VISION = 0x1210,   ///< H.265/HEVC codec with Dolby Vision

	// Audio
	CODEC_FORMAT_AAC               = 0x2100,   ///< AAC codec
	CODEC_FORMAT_AAC_ADTS          = 0x2101,   ///< AAC codec with adts header
	CODEC_FORMAT_AC3               = 0x2200,   ///< AC3 codec (Dolby Digital)
	CODEC_FORMAT_EC3               = 0x2300,   ///< Dolby Digital Plus codec (EAC3,DD+)
	CODEC_FORMAT_EC3_DOLBY_ATMOS   = 0x2301,   ///< Dolby Digital Plus codec (EAC3,DD+) with Dolby ATMOS
};


/**
 *@brief    Enumeration for drm type supported by LMA
*/
enum LG_DRM
{
	LG_DRM_NONE,
	LG_DRM_PLAYREADY,
	LG_DRM_WIDEVINE
};


/**
 *@brief	Enumeration for media (audio/video) codec and format
 */
struct LG_MediaInfo
{
	struct video
	{
		LG_MEDIA_CODEC_FORMAT codec;       ///< video codec and format
		int                   reserved;    ///< Reserved
	} video;

	struct audio
	{
		LG_MEDIA_CODEC_FORMAT codec;       ///< audio codec and format
		int                   profile;     ///< profile of AAC
		int                   channels;    ///< number of channel
		int                   frequency;   ///< frequency (Unit: Hz)
		int                   reserved;    ///< Reserved
	} audio;

	LG_DRM                    drm;         ///< drm type
	int64_t                   startPts;    ///< where to start playing
	int                       reserved;    ///< Reserved
};


/**
 *@brief		callback for LG_EsPlayer
 *@param		type [in] type of callback event
 *@param		numValue [in] information of integer type
 *@param		strValue [in] information of string type
 *
 *@return		void
 */
using LG_EsPlayerCallback = void(*)(const int type, const int64_t numValue, const char* strValue, void* data);


/**
 *@brief		The LG_Pipeline class is an interface for using Media system of the LGTV platform.
 *@details		This function does not inlcude in the NDK (or SDL)
 */
class LG_EsPlayer
{
public:
	virtual ~LG_EsPlayer() {}

	/**
	 *@brief		Use this function to set the decrypt context object.
	 *@details		If DRM is used, it should be called before the first Feed () call.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int SetDrm (const LG_Drm* drm) = 0;

	/**
	 *@brief		Use this function to set the Media information.
	 *@details		The same function can be performed in Load(const LG_MediaInfo& mediaInfo).
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int SetMediaInfo (const LG_MediaInfo& mediaInfo) = 0;

	/**
	 *@brief		Use this function to load a pipeline.
	 *@details		When this function is complete, the callback function receives the LG_ESPLAYER_EVENT_LOADED event.
	 *@param		es [in] metadate information of media(ES stream).
	 *@param		callback [in] user callback fuction to get a pipeline event.
	 *@see			ESMetadata
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Load () = 0;
	virtual int Load (const LG_MediaInfo& mediaInfo) = 0;
	virtual int Load (const LG_MediaInfo& mediaInfo, LG_EsPlayerCallback callback) = 0;

	/**
	 *@brief		Use this function to unload a pipeline.
	 *@details		When this function is complete, the callback function receives the LG_ESPLAYER_EVENT_UNLOADED event.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Unload () = 0;

	/**
	 *@brief		Use this function to feed a pipeline.
	 *@details		When the buffer of the pipeline is empty, the callback function receives the LG_ESPLAYER_EVENT_BUFFER_LOW event.\n
	 *				When the buffer of the pipeline is full, the callback function receives the LG_ESPLAYER_EVENT_BUFFER_FULL event.\n
	 *				\<NOTE\> When the LG_ESPLAYER_EVENT_BUFFER_LOW event occurs, the pipeline does not take any action.\n
     *				You MUST pause the Pipeline before the LG_ESPLAYER_EVENT_BUFFER_LOW event occurs.
	 *@return		returns LG_SUCCESS on success, LG_ERROR on failure
	 */
	virtual int Feed (const uint8_t *data, uint32_t size, int64_t pts, estream_t type) const = 0;
	virtual int Feed (const uint8_t *data, uint32_t size, int64_t pts, estream_t type,
                      encryption_t mode,
                      const uint8_t *kid, uint32_t kidSize,
                      const uint8_t *iv, uint32_t ivSize,
                      const subsample_t *subsample, uint32_t subsampleSize) const = 0;


	/**
	 *@brief		Use this function to play media.
	 *@details		When this function is complete, the callback function receives the LG_ESPLAYER_EVENT_PLAYING event.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Play () = 0;

	/**
	 *@brief		Use this function to pause playback
	 *@details		When this function is complete, the callback function receives the LG_ESPLAYER_EVENT_PAUSED event.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Pause () = 0;

	/**
	 *@brief		Use this function to move the position to play
	 *@details		When this function is complete, the callback function receives the LG_ESPLAYER_EVENT_SEEK_DONE event.\n
					This function includes the flush function.
	 *@param		ms [in] where to play, The unit is milliseconds.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Seek (int ms) = 0;

	/**
	 *@brief		Use this function to inform the end of stream to pipeline
	 *@details		It indicates that there is no more streaming data.
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int PushEos () = 0;

	/// Deprecated : This function do not flush H/W level buffer
	/**
	 *@deprecated	avoid direct calls to this function.
	 *@brief		Use this function to clear the buffer from the pipeline
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Flush () const = 0;

	/**
	 *@brief		Use this function to get current pts
	 *@return		returns pts on success or -1 on failure
	 */
	virtual int64_t GetCurrentTime () const = 0;

	/**
	 *@brief		Use this function to set the window area to play video.
	 *@param		dispX [in] the x position of the window
	 *@param		dispY [in] the y position of the window
	 *@param		dispW [in] the width of the window
	 *@param		dispH [in] the height of the window
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int SetDisplayWindow (int dispX = 0, int dispY = 0, int dispW = 0, int dispH = 0) = 0;

	/**
	 *@brief		Use this function to set the area to be displayed after cropping a specific area of the video.
	 *@details		The Area of crop is set based on the original video resolution.
	 *              The Area of disp is set based on the created window resolution.
	 *              The cropped video is displayed to fill the display area.
	 *@param		cropX [in] Horizontal coordinate value of upper-left corner of the cropping area.
	 *@param		cropY [in] Vertical coordinate value of upper-left corner of the cropping area.
	 *@param		cropW [in] Width of the cropping area in pixels.
	 *@param		cropH [in] Height of the cropping area in pixels.
	 *@param		dispX [in] the x position of the window
	 *@param		dispY [in] the y position of the window
	 *@param		dispW [in] the width of the window
	 *@param		dispH [in] the height of the window
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int SetCropVideoDisplayWindow(int cropX, int cropY, int cropW, int cropH,
										  int dispX, int dispY, int dispW, int dispH) = 0;

	/**
	 *@brief		Use this function to enable mute of audio
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Mute () = 0;

	/**
	 *@brief		Use this function to disable mute of audio
	 *@return		returns LG_SUCCESS on success or LG_ERROR on failure
	 */
	virtual int Unmute () = 0;
};

extern "C" LG_EsPlayer* LG_CreateEsPlayer(LG_EsPlayerCallback callback);

#endif // LG_ESPLAYER_H

