#ifndef __H_DISCDATA
#define __H_DISCDATA

#include "bigend_types.h"

struct FS3DO_DiscLabel {
  uint8     pad[76];
  uint32b   sectorSize;              /* Usually contains 2048 */
  uint32b   sectorCount;             /* # of blocks on disc */
};

#endif /* __H_DISCDATA */
