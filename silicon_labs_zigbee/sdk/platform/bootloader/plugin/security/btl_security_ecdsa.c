/***************************************************************************//**
 * @file btl_security_ecdsa.c
 * @brief ECDSA signing functionality for Silicon Labs bootloader
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include "btl_security_ecdsa.h"
#include "em_device.h"
#include "btl_security_tokens.h"

#include <stddef.h>
#include <string.h> // For memset

#if defined(CRYPTO_PRESENT)

#include "ecc/ecc.h"
#include "em_cmu.h"

#if !defined(CRYPTO) && defined(CRYPTO0)
#define cmuClock_CRYPTO cmuClock_CRYPTO0
#define CRYPTO CRYPTO0
#endif

/** Verify the ECDSA signature of the SHA hash, using
 *  the public key in the relevant token, with the signature contained in
 *  the byte arrays pointed to.
 */
int32_t btl_verifyEcdsaP256r1(const uint8_t *sha256,
                              const uint8_t *signatureR,
                              const uint8_t *signatureS)
{
  if ((sha256 == NULL) || (signatureR == NULL) || (signatureS == NULL)) {
    return BOOTLOADER_ERROR_SECURITY_INVALID_PARAM;
  }

  // Re-enable the clock of the CRYPTO module, since mbedtls turned it off
  CMU_ClockEnable(cmuClock_CRYPTO, true);

  ECC_Point_t pubkey;
  memset(&pubkey, 0, sizeof(ECC_Point_t));
  ECC_ByteArrayToBigInt(pubkey.X, btl_getSignedBootloaderKeyXPtr());
  ECC_ByteArrayToBigInt(pubkey.Y, btl_getSignedBootloaderKeyYPtr());

  ECC_EcdsaSignature_t ecc_signature;
  ECC_ByteArrayToBigInt(ecc_signature.r, signatureR);
  ECC_ByteArrayToBigInt(ecc_signature.s, signatureS);

  return ECC_ECDSA_VerifySignatureP256(CRYPTO,
                                       sha256,
                                       sizeof(ECC_BigInt_t),
                                       &pubkey,
                                       &ecc_signature);
}

#elif defined(SEMAILBOX_PRESENT)

#include "em_se.h"

/** Verify the ECDSA signature of the SHA hash, using
 *  the public key in the relevant token, with the signature contained in
 *  the byte arrays pointed to.
 */
int32_t btl_verifyEcdsaP256r1(const uint8_t *sha256,
                              const uint8_t *signatureR,
                              const uint8_t *signatureS)
{
  if ((sha256 == NULL) || (signatureR == NULL) || (signatureS == NULL)) {
    return BOOTLOADER_ERROR_SECURITY_INVALID_PARAM;
  }

  // Keyspec for secp256r1, given public signing key
  uint32_t keyspec = 0x80000000 | 31 | (1 << 10) | (1 << 13);

  SE_Command_t command = SE_COMMAND_DEFAULT(SE_COMMAND_SIGNATURE_VERIFY);
  SE_addParameter(&command, keyspec); // 0 = key in host memory
  SE_addParameter(&command, 32); // length of hash

  SE_DataTransfer_t key_x = SE_DATATRANSFER_DEFAULT((uint8_t *)btl_getSignedBootloaderKeyXPtr(), 32);
  SE_DataTransfer_t key_y = SE_DATATRANSFER_DEFAULT((uint8_t *)btl_getSignedBootloaderKeyYPtr(), 32);

  SE_DataTransfer_t hash = SE_DATATRANSFER_DEFAULT((uint8_t *)sha256, 32);

  SE_DataTransfer_t signature_r = SE_DATATRANSFER_DEFAULT((uint8_t *)signatureR, 32);
  SE_DataTransfer_t signature_s = SE_DATATRANSFER_DEFAULT((uint8_t *)signatureS, 32);

  SE_addDataInput(&command, &key_x);
  SE_addDataInput(&command, &key_y);
  SE_addDataInput(&command, &hash);
  SE_addDataInput(&command, &signature_r);
  SE_addDataInput(&command, &signature_s);

  SE_executeCommand(&command);

  SE_Response_t rsp = SE_readCommandResponse();

  if (rsp == SE_RESPONSE_OK) {
    return BOOTLOADER_OK;
  }

  return BOOTLOADER_ERROR_SECURITY_REJECTED;
}
#endif
