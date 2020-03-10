/***************************************************************************//**
 * @file ci_script.c
 * @brief CLI implementations for the scripting features of the multiprotocol
 *   app. These features allow a user to upload a series of commands to the
 *   chip to be executed consecutively without waiting for further CLI input
 *   until the uploaded script is completed. This is useful in testing
 *   scenarios where they delay related to entering commands one-by-one
 *   is too great.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__ICCARM__)
// IAR doesn't have strings.h and puts those declarations in string.h
#include <strings.h>
#endif

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"

// The actual script used in scripted mode
char script[SCRIPT_LENGTH] = { '\0' };
// The index of the current character being read while in scripted mode
uint16_t scriptedMarker = 0;
// The length of the string in script. Memory for 'script' is not allocated
// by default, otherwise we could just use strlen(script)
uint16_t scriptLength = 0;

// Duration of time for which CI suspension is effective
uint32_t suspension = 0;
// Time at which CI suspension started
uint32_t suspensionStartTime = 0;

void printScript(int argc, char **argv)
{
  responsePrint(argv[0], "script:%s", script);
}

void enterScript(int argc, char **argv)
{
  // Set this all the way to the end of the script so we're definitely not in
  // scripted mode
  scriptedMarker = SCRIPT_LENGTH;

  // Allocate the new memory after making sure we clear it above.
  // Another alternative would just be to malloc this during startup
  // and only memset it every time we call this command, but it is a
  // lot of memory that shouldn't be allocated if the user doesn't want it.
  memset(script, '\0', SCRIPT_LENGTH);

  uint16_t index = 0;

  char input;
  bool endScriptFound = false;
  // Read from the input until we hit the max length, or until we hit 'endScript' which gets
  // us out of scriptEntry mode
  while (index < (SCRIPT_LENGTH - 1)) {
    input = getchar();
    if (input != '\0' && input != 0xFF) {
      script[index] = input;
      index++;

      if (input == '\n') {
        printf("> ");
      }

      if (index >= strlen("endScript") && strcasecmp("endScript", &(script[index - strlen("endScript")])) == 0) {
        endScriptFound = true;
        script[index - strlen("endScript")] = '\0';
        break;
      }
    }
  }

  if (!endScriptFound) {
    responsePrintError(argv[0], 0x12, "You never entered endScript, but ran out of room in the script buffer. Your script may be malformed.");
    return;
  }

  // Get the actual length of the script entered.
  scriptLength = strlen(script);

  // Print what was entered
  printScript(1, &argv[0]);
}

void startScript(int argc, char **argv)
{
  if (script == NULL) {
    responsePrintError(argv[0], 0x11, "No script available to run");
    return;
  }

  responsePrint(argv[0], "script:%s", "Starting");

  // Scripted mode is indicated by scriptedMarker being less than scriptLength
  scriptedMarker = 0;
}

void wait(int argc, char **argv)
{
  // Relative time by default
  RAIL_TimeMode_t timeMode = RAIL_TIME_DELAY;

  if (argc > 2 && !parseTimeModeFromString(argv[2], &timeMode)) {
    responsePrintError(argv[0], 0x3, "Invalid time mode");
    return;
  }

  suspensionStartTime = RAIL_GetTime();
  suspension = ciGetUnsigned(argv[1]);

  // In the code, everything is handled as relative time, so convert this
  // to a relative value
  if (timeMode == RAIL_TIME_ABSOLUTE) {
    suspension -= suspensionStartTime;
  }

  responsePrint(argv[0],
                "currentTime:%u,delay:%u,resumeTime:%u",
                suspensionStartTime,
                suspension,
                suspensionStartTime + suspension);
}
