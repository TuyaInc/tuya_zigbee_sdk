/*******************************************************************************
 * @file railapp_rmr.h
 * @brief This is the file that contains the Ram Modem Reconfiguration CI commands
 * @author David Cabrera
 * @copyright Copyright 2018 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __CI_RMR_H__
#define __CI_RMR_H__

#include <stdint.h>
#include <stdbool.h>

#include "rail_types.h"

// General structure of Commands
// <command> <args> <help text>
// u=uint8, v=uint16, w=uint32, s=int32, b=string, ?=Anything, *=0 or more of previous
#define RMR_CI_COMMANDS                                                                                                                                                      \
  RAILCMD_SEPARATOR("RAM Modem Reconfiguration Commands")                                                                                                                    \
  RAILCMD_ENTRY("writeRmrStructure", "uvuw*", CI_writeRmrStructure, "This command should only be called by Simplicity Studio and not directly from the CI.")                 \
  RAILCMD_ENTRY("updateConfigurationPointer", "uvu", CI_updateConfigurationPointer, "This command should only be called by Simplicity Studio and not directly from the CI.") \
  RAILCMD_ENTRY("reconfigureModem", "", CI_reconfigureModem, "This command should only be called by Simplicity Studio and not directly from the CI.")                        \
  RAILCMD_SEPARATOR("End of RAM Modem Reconfiguration Commands") // Please keep this line below last RAILCMD_ENTRY

// Prototypes of CLI functions
#define RAILCMD_ENTRY(command, args, callback, helpStr) \
  extern void callback(int argc, char **argv);
#define RAILCMD_SEPARATOR(string) /* no-op */
RMR_CI_COMMANDS
#undef  RAILCMD_ENTRY
#undef  RAILCMD_SEPARATOR

/**
 * @enum RAIL_RMR_StructureIndex_t
 * @brief Identifier for each structure used during reconfiguration
 **/
RAIL_ENUM(RAIL_RMR_StructureIndex_t)
{
  RMR_STRUCT_PHY_INFO,
  RMR_STRUCT_IRCAL_CONFIG,
  RMR_STRUCT_MODEM_CONFIG,
  RMR_STRUCT_FRAME_TYPE_CONFIG,
  RMR_STRUCT_FRAME_LENGTH_LIST,
  RMR_STRUCT_FRAME_CODING_TABLE,
  RMR_STRUCT_CHANNEL_CONFIG_ATTRIBUTES,
  RMR_STRUCT_CHANNEL_CONFIG_ENTRY,
  RMR_STRUCT_CONV_DECODE_BUFFER,
};
#define RMR_STRUCT_NULL 255

/**
 * @enum RAIL_RMR_CommandArguments_t
 * @brief RAIL enum for the reconfiguration command arguments
 **/
RAIL_ENUM(RAIL_RMR_CommandArguments_t) {
  RMR_CI_RESPONSE,
  RMR_CI_RMR_STRUCTURE,
  RMR_CI_OFFSET,
  RMR_CI_COUNT,
  RMR_CI_DATA_START,
};

// Ram Modem Reconfiguration buffer sizes.
#define RMR_PHY_INFO_LEN 10
#define RMR_IRCAL_LEN 26
#define RMR_MODEM_CONFIG_LEN 256
#define RMR_FRAMETYPE_LEN 8
#define RMR_FRAME_LENGTH_LIST_LEN 8
#define RMR_ENTRYATTR_LEN 1
#define RMR_FRAME_CODING_TABLE_LEN 640
#define RMR_CONV_DECODE_BUFFER_LEN 64
#define RMR_ARGUMENT_BUFFER_SIZE 10

#endif //__CI_RMR_H__
