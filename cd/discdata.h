#ifndef __H_DISCDATA
#define __H_DISCDATA


#include "bigend_types.h"

/******************************************************************************
**
**  Copyright (C) 1995, an unpublished work by The 3DO Company. All rights reserved.
**  This material contains confidential information that is the property of The 3DO Company.
**  Any unauthorized duplication, disclosure or use is prohibited.
**  $Id: discdata.h,v 1.16 1994/10/25 15:57:33 vertex Exp $
**
******************************************************************************/


/*
  Define the position of the primary label on each Opera disc, the
  block offset between avatars, and the index of the last avatar
  (i.e. the avatar count minus one).  The latter figure *must* match
  the ROOT_HIGHEST_AVATAR figure from "filesystem.h", as the same
  File structure is use to read the label at boot time, and to provide
  access to the root directory.
*/

#define DISC_BLOCK_SIZE           2048
#define DISC_LABEL_OFFSET         225
#define DISC_LABEL_AVATAR_DELTA   32786
#define DISC_LABEL_HIGHEST_AVATAR 7
#define DISC_TOTAL_BLOCKS         330000

#define ROOT_HIGHEST_AVATAR       7
#define FILESYSTEM_MAX_NAME_LEN   32

#define VOLUME_STRUCTURE_OPERA_READONLY    1
#define VOLUME_STRUCTURE_LINKED_MEM        2

#define VOLUME_SYNC_BYTE          0x5A
#define VOLUME_SYNC_BYTE_LEN      5
#define VOLUME_COM_LEN      	  32
#define VOLUME_ID_LEN      	  32

#ifndef FILE_IS_DIRECTORY

        #define FILE_IS_DIRECTORY       0x00000001
        #define FILE_IS_READONLY        0x00000002
        #define FILE_IS_FOR_FILESYSTEM  0x00000004
        #define FILE_SUPPORTS_DIRSCAN   0x00000008
        #define FILE_INFO_NOT_CACHED    0x00000010
        #define FILE_SUPPORTS_ENTRY     0x00000020
        #define FILE_BLOCKS_CACHED      0x00000040
        #define FILE_NOT_READY          0x00000080
        #define FILE_HAS_LINK      	0x00000100
#endif
/*
// This disc won't necessarily cause a reboot when inserted.  This flag is
// advisory ONLY. Only by checking with cdromdipir can you be really sure.
// Place in dl_VolumeFlags.  Note: the first volume gets this flag also.
*/
#define	VOLUME_FLAGS_DATADISC	0x01

/*
  Data structures written on CD disc (Compact Disc disc?)
*/
struct FS3DO_DiscLabel {
  uint8    dl_RecordType;                   /* Should contain 1 */
  uint8    dl_VolumeSyncBytes[VOLUME_SYNC_BYTE_LEN]; /* Synchronization byte */
  uint8    dl_VolumeStructureVersion;       /* Should contain 1 */
  uint8    dl_VolumeFlags;                  /* Should contain 0 */
  uint8    dl_VolumeCommentary[VOLUME_COM_LEN];
					    /* Random comments about volume */
  uint8    dl_VolumeIdentifier[VOLUME_ID_LEN]; /* Should contain disc name */
  uint32b   dl_VolumeUniqueIdentifier;       /* Roll a billion-sided die */
  uint32b   dl_VolumeBlockSize;              /* Usually contains 2048 */
  uint32b   dl_VolumeBlockCount;             /* # of blocks on disc */
  uint32b   dl_RootUniqueIdentifier;         /* Roll a billion-sided die */
  uint32b   dl_RootDirectoryBlockCount;      /* # of blocks in root */
  uint32b   dl_RootDirectoryBlockSize;       /* usually same as vol blk size */
  uint32b   dl_RootDirectoryLastAvatarIndex; /* should contain 7 */
  uint32b   dl_RootDirectoryAvatarList[ROOT_HIGHEST_AVATAR+1];
};

struct FS3DO_DirectoryHeader {
  int32b      dh_NextBlock;
  int32b      dh_PrevBlock;
  uint32b     dh_Flags;
  uint32b     dh_FirstFreeByte;
  uint32b     dh_FirstEntryOffset;
};


struct FS3DO_DirectoryRecord {
  uint32b     dir_Flags;
  uint32b     dir_UniqueIdentifier;
  uint32b     dir_Type;
  uint32b     dir_BlockSize;
  uint32b     dir_ByteCount;
  uint32b     dir_BlockCount;
  uint32b     dir_Burst;
  uint32b     dir_Gap;
  int8        dir_FileName[FILESYSTEM_MAX_NAME_LEN];
  uint32b     dir_LastAvatarIndex;
  uint32b     dir_AvatarList[1];
};

struct FS3DO_CatapultPage {
  uint32         cp_MBZ;         /* must be zero */
  uint32         cp_Fingerprint; /* contains '*zap' */
  int32          cp_Entries;     /* contains # of valid entries */
  int32          cp_NextPage;    /* contains block # of next page, or -1 */
  struct {
    uint32       cpe_FileIdentifier;  /* unique ID of file */
    uint32       cpe_FileBlockOffset; /* offset of run in original file */
    uint32       cpe_RunLength;       /* # of blocks in this run */
    uint32       cpe_RunOffset;       /* offset of run in catapult file */
  } cpe[1];                    /* actually there are enough to fill page */
};


#define NVRAM_UNK_NUM   8
struct FS3DO_NVRAMRecord {
  uint32b     nv_UniqueIdentifier;
  int32b      nv_NextBlock;
  int32b      nv_PrevBlock;
  int32b      nv_Unk[NVRAM_UNK_NUM];
};

#define DIRECTORY_LAST_IN_DIR        0x80000000
#define DIRECTORY_LAST_IN_BLOCK      0x40000000

#define FILE_HIGHEST_AVATAR    255

/*****************************************************************************/


#endif /* __H_DISCDATA */
