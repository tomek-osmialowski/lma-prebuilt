/**
* Copyright (c) 2020 LG Electronics, Inc.
*
* Unless otherwise specified or set forth in the NOTICE file, all content,
* including all source code files and documentation files in this repository are:
* Confidential computer software. Valid license from LG required for
* possession, use or copying.
*/

#ifndef __LG_SVP_H__
#define __LG_SVP_H__

#include "LG_Type.h"


class ISecureVideoHandler
{
public:
    virtual ~ISecureVideoHandler() = default;

    /**
     *@brief        Use this function to decrypt PlayReady DRM contents in TEE
     *@details      call sequence               Decrypt() -> Feed()
     *@param        mode                        [in] Encryption schemes
     *@param        appContext                  [in] DRM_APP_CONTEXT
     *@param        kidSize                     [in] Kid size
     *@param        kid                         [in] The identifier of the desired key. 16 bytes array
     *@param        encryptedRegionMappingSize  [in] The number of encrypted-region mapping entries
     *@param        encryptedRegionMapping      [in] The map of clear and protected ranges of the sample
     *@param        encryptedRegionSkipSize     [in] Optional - playready 4.0 or higher. Region skip size.
     *@param        encryptedRegionSkip         [in] Optional - playready 4.0 or higher. Protection pattern. [0] : crypt, [1] : skip
     *@param        ivSize                      [in] IV size
     *@param        iv                          [in] IV value
     *@param        dataSize                    [in] Sample size
     *@param        data                        [in] Sample
     *@param        outClearSize                [out] Out size. this becomes an argument to Feed()
     *@param        outClearData                [out] Out data of addr. this becomes an argument to Feed()
     *@return       returns DRM_RESULT
     */
    virtual int32_t Decrypt(
        const encryption_t  mode,
        void               *appContext,
        uint32_t            kidSize,
        const uint8_t      *kid,
        uint32_t            encryptedRegionMappingSize,
        const uint32_t     *encryptedRegionMapping,
        uint32_t            encryptedRegionSkipSize,
        const uint32_t     *encryptedRegionSkip,
        uint32_t            ivSize,
        const uint8_t      *iv,
        uint32_t            dataSize,
        const uint8_t      *data,
        uint32_t           *outClearSize,
        uint8_t           **outClearData) = 0;

    virtual void ReleaseClearContent(
        const uint32_t      f_cbClearContentOpaque,
        const uint8_t      *f_pbClearContentOpaque ) = 0;

    virtual void Close() = 0;
};

extern "C" ISecureVideoHandler* Create();

#endif
