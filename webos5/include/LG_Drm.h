/**
* Copyright (c) 2020 LG Electronics, Inc.
*
* Unless otherwise specified or set forth in the NOTICE file, all content,
* including all source code files and documentation files in this repository are:
* Confidential computer software. Valid license from LG required for
* possession, use or copying.
*/

/**
 *@file			LG_Drm.h
 *@brief		This is a header file that defines the LG_Drm class.
 *@author		jungwon.cho@lge.com
 *@date			2020-03-10
 *@version		1.0.0 Initial vesion
 */


#ifndef __LG_DRM_H__
#define __LG_DRM_H__

#include <stdbool.h>
#include <stdint.h>

//======================================================================================

    /**
     *@brief Size of DRM id
     */
    #define LG_DRM_MAX_SESSION_ID_SIZE 16

    /**
     *@brief	Identifier of a session.
     *@details	This ID is passed to all functions to associate with the session.\n
                The value will be random number.
     */
    typedef struct
    {
        char id[LG_DRM_MAX_SESSION_ID_SIZE];

    } LG_DRM_SESSION_ID_T;

    /**
     *@brief	Buffer used for init data, DRM request message and DRM response message.
     */
    typedef struct
    {
        uint8_t* data;
        uint32_t size;

    } LG_DRM_BUFFER_T;

    /**
     *@brief	Callback messages type.
     *@details	sequence diagram of DRM API
     *
     *        App          DRM API Call
     *      ------------------------------------------------
     *  App Launching  --> Initialize()
     *          |
     *  ------->|      --> CreateSession()
     *  ^       |               |
     *  |       |      --> RegisterCallback()
     *  |       |               |
     *  |       |      --> GenerateRequest()
     *  |       |               |
     *  |       |      <-- callback(LG_DRM_LICENSE_REQUEST, message)
     *  |       |
     *  |       |        GetLincese form DRM Licnse Server(message)
     *  |       |
     *  |       |      --> ProcessResponse(response)
     *  |       |               |
     *  |       |      <-- callback(LG_DRM_LICENSE_KEYREADY, NULL)
     *  |       |               |
     *  |       |      --> Decrypt()
     *  |       |               |
     *  |       |      --> DeregisterCallback()
     *  |       V               |
     *  <-------|      --> DestroySession()
     *          |               |
     *  App Exiting    --> Finalize()
     */
    typedef enum
    {
        LG_DRM_LICENSE_REQUEST,     ///< result of GenerateRequest()
        LG_DRM_LICENSE_KEYREADY,    ///< result of ProcessResponse()

    } LG_DRM_MESSAGE_TYPE_T;


    /**
	 *@brief		message callback definition
	 *@details		to be sent to the DRM server
	 *@param		inMessageType [in] message type
	 *@param		inMessage     [in] message data
	 *@param		inUserData    [in] user data
	 *@see          LG_DRM_MESSAGE_TYPE_T
	 *@return		void
	 */
    typedef void (*lg_drm_message_cb)(LG_DRM_MESSAGE_TYPE_T inMessageType, const LG_DRM_BUFFER_T* inMessage, void* inUserData);

    class LG_Drm
    {

  public:

        /**
         *@brief		Initialization of DRM System.
         *@details		Should be done before use DRM system. Finalize() should be called in pairs.\n
                        CreateMediaKeys() is called internally.
         *@return		LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t Initialize() = 0;

        /**
         *@brief		Cleanup of DRM System.
         *@details      Should be done before closing DRM system.\n
                        DestroyMediaKeys() is called internally.
         *@return       LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t Finalize() = 0;

        /**
        *@brief         Create a DRM session and initialize for new session.
        *@details       A DRM session represents a handling of single license. Should key the outDrmSessionId for handling the license.\n
                        This will be passed to all functions to associate with this seesion.\n
                        DestroySession() shuold be called in pairs.
        *@param         outDrmSessionId [out] identifier of created session.
        *@see           LG_DRM_SESSION_ID_T
        *@return        LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t CreateSession(LG_DRM_SESSION_ID_T* outDrmSessionId) = 0;

        /**
        *@brief         Delete a DRM session and cleanup.
        *@details       Should be done before closing session.
        *@param         inDrmSessionId [in] session to be deleted.
        *@see           LG_DRM_SESSION_ID_T
        *@return        LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t DestroySession(LG_DRM_SESSION_ID_T* inDrmSessionId) = 0;

        /**
        *@brief         Releases all keys loaed into a session.
        *@param         inDrmSessionId [in] session to have keys released.
        *@see           LG_DRM_SESSION_ID_T
        *@return        LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t ReleaseKeys(LG_DRM_SESSION_ID_T* inDrmSessionId) = 0;

        /**
        *@brief        Generates a DRM request based on the provided inDrmInitData.
        *@details      The request is not returned from this method.\n
                       The buffer will be sent by a message callback with LG_DRM_LICENSE_REQUEST message type.\n
                       Application should request a DRM license server with the message.
        *@param        inDrmSessionId  [in] session associated with request.
        *@param        inDrmInitData   [in] TODO::: CENC, KIDs or event more complex
        *@return       LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t GenerateRequest(LG_DRM_SESSION_ID_T* inDrmSessionId,
                                        const LG_DRM_BUFFER_T* inDrmInitData) = 0;

        /**
        *@brief        Processes the response received from DRM license server.
        *@details      The processing should set up all keys from the response for decryption.
        *@param        inDrmSessionId  [in] session that created the related request.
        *@param        inDrmResponse   [in] response to be processed by the DRM implementation.
        *@return       LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t ProcessResponse(LG_DRM_SESSION_ID_T* inDrmSessionId,
                                        const LG_DRM_BUFFER_T* inDrmResponse) = 0;

        /**
        *@brief        Registers a callback.
        *@details      Should be called before call a generateRequest().
        *@param        inDrmSessionId  [in] session that created the related request.
        *@param        callback        [in] callback to be registered.
        *@param        userData        [in] user data to be passed to the callback.
        *@return       LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t RegisterCallback(LG_DRM_SESSION_ID_T* inDrmSessionId,
                                         lg_drm_message_cb callback,
                                         void* userData) = 0;

        /**
        *@brief        Deregister a callback.
        *@details      Should be called in pairs with RegisterCallback.
        *@param        inDrmSessionId  [in] session that created the related request.
        *@return       LG_SUCCESS on success or LG_ERROR of failure.
         */
        virtual int32_t DeregisterCallback(LG_DRM_SESSION_ID_T* inDrmSessionId) = 0;

        virtual ~LG_Drm() {}
    };

extern "C" LG_Drm* LG_CreateDrm();

#endif
