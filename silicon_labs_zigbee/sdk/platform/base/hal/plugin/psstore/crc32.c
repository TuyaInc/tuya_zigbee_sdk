#include <stdint.h>
#include "em_device.h"

uint32_t bg_crc_calc(uint32_t inCrc32, int size, const uint8_t*data)

{
#if defined (_SILICON_LABS_32B_SERIES_2)
  GPCRC->EN = GPCRC_EN_EN;
  GPCRC->CTRL = GPCRC_CTRL_BYTEMODE;
#else // defined (_SILICON_LABS_32B_SERIES_2)
  GPCRC->CTRL = GPCRC_CTRL_BYTEMODE | GPCRC_CTRL_EN;
#endif //  defined (_SILICON_LABS_32B_SERIES_2)
  GPCRC->INIT = inCrc32;
  GPCRC->CMD = GPCRC_CMD_INIT;

  while (size--) {
    GPCRC->INPUTDATABYTE = *data++;
  }
  return GPCRC->DATA ^ 0xffffffff;
}
