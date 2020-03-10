#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_types.h"
#include "rail_chip_specific.h"

extern RAIL_Handle_t railHandle;
/***************************************************************************//**
 * @brief
 *   Select the RF Path to use for TX and RX.
 *
 * @details
 *   Provide access to set system function, where its source file changes for
 *   for each platform. Allows use with existing RPC config for all platforms.
 *
 * @param rfPath Sets the default antenna path.
 *
 ******************************************************************************/
#ifdef _SILICON_LABS_32B_SERIES_1
void RAILAPP_SetRfPath(RAIL_AntennaSel_t rfPath)
{
}
#elif defined(_SILICON_LABS_32B_SERIES_2)
void RAILAPP_SetRfPath(RAIL_AntennaSel_t rfPath)
{
  RAIL_AntennaConfig_t config = {
    0, /** Antenna 0 Pin Enable */
    0, /** Antenna 1 Pin Enable */
    rfPath,  /** default RF Path selection */
  };

  RAIL_ConfigAntenna(railHandle, &config);
}
#endif //_SILICON_LABS_32B_SERIES_2

#ifdef CLI_INTERFACE
void CI_SetRfPath(int argc, char **argv)
{
  uint32_t rfPath = ciGetUnsigned(argv[1]);
  RAILAPP_SetRfPath(rfPath);
  responsePrint(argv[0], "RfPath:%d", rfPath);
}
#endif
