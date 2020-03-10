#ifndef __RAILAPP_ANTENNA_H__
#define __RAILAPP_ANTENNA_H__

#include "rail_chip_specific.h"

void RAILAPP_SetRfPath(RAIL_AntennaSel_t rfPath);

#ifdef CLI_INTERFACE
#ifdef _SILICON_LABS_32B_SERIES_2
#define ANTENNA_CI_COMMANDS                                                  \
  RAILCMD_SEPARATOR("Antenna Commands")                                      \
  RAILCMD_ENTRY("setRfPath", "w", CI_SetRfPath, "[rfPath] 0=Path0, 1=Path1") \
  RAILCMD_SEPARATOR("End of Antenna Commands") // Please keep this line below last RAILCMD_ENTRY
#else
#define ANTENNA_CI_COMMANDS /* none */
#endif

// Prototypes of CLI functions
#define RAILCMD_ENTRY(command, args, callback, helpStr) \
  extern void callback(int argc, char **argv);
#define RAILCMD_SEPARATOR(string) /* no-op */
ANTENNA_CI_COMMANDS
#undef  RAILCMD_ENTRY
#undef  RAILCMD_SEPARATOR
#endif

#ifdef _SILICON_LABS_32B_SERIES_2
void RAILAPP_SetRfPath(RAIL_AntennaSel_t rfPath);
#endif //_SILICON_LABS_32B_SERIES_2

#endif
