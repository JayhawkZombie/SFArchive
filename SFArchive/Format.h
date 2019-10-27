#pragma once

#include "Detail/Headers.h"
#include "Detail/Helpers.h"
#include "Detail/Content.h"

/*  File format:
 *  |-------------------------------------|
 *  | Endianness flag (1 byte)            |
 *  |-------------------------------------|
 *  | Archive header (88 bytes)           |
 *  |-------------------------------------|
 *  | Archive entries (variable length)   |
 *  |-------------------------------------|
 *  | Directory header (32 bytes)         |
 *  |-------------------------------------|
 *  | Directory entries (variable length) |
 *  |-------------------------------------|
 *  | End of directory record (16 bytes)  |
 *  |-------------------------------------|
 *  | End of archive record (8 bytes)     |
 *  |-------------------------------------|
 *
 *
 *  An empty archive will look like (152 bytes)
 *  |-------------------------------------------------------------------------------------------------|
 *  | 0x0X                                 | (endianness, used to determine if byte swaps are needed) | <1>
 *  |-------------------------------------------------------------------------------------------------|
 *  | 0x8C 0xE0D6 0xAFD7 0x21A9            | (magic number, 7 bytes)                                  | <8>
 *  |-------------------------------------------------------------------------------------------------|
 *  | <archive header> (88 bytes)          |                                                          |
 *  | 0x2071 0xAAF6 0x86B4 0xOFCE (8)      | (header signature)                                       |
 *  | 0x0000 0x0000 0x0000 0x0000 (8)      | (version info)                                           |
 *  | 0x3000 0x0003 0x000         (6)      | (general and reserved flags)                             |
 *  | 0xXXXX 0xXXXX 0xXXXX 0xXXXX (8)      | (time archive created)                                   |
 *  | 0xXXXX 0xXXXX 0xXXXX 0xXXXX (8)      | (time archive last modified)                             |
 *  | 0xXXXX 0xXXXX 0xXXXX 0xXXXX (8)      | (crc value)                                              |
 *  | 0x9800 0x0000 0x0000 0x0000 (8)      | (uncompressed archive size)                              |
 *  | 0x9800 0x0000 0x0000 0x0000 (8)      | (compressed archive size)                                |
 *  | 0x5800 0x0000 0x0000 0x0000 (8)      | (offset from beg of file to directory, 88 bytes (0x58)   |
 *  | 0x9AC3 0xC7EB 0xEFD9 0xC302 (8)      | (end of header signature)                                | <96>
 *  |-------------------------------------------------------------------------------------------------|
 *  | <archive entries> (variable)         | (list of file entries in the archive)                    |
 *  |                                      | (no entries, 0 bytes)                                    |
 *  |-------------------------------------------------------------------------------------------------|
 *  | <directory header> (32 bytes)        |                                                          |
 *  | 0xB9C4 0x00DA 0x080B 0xFFA5 (8)      | (directory header)                                       |
 *  | 0x0000 0x0000 0x0000 0x0000 (8)      | (num entries in directory)                               |
 *  | 0x0000 0x0000 0x0000 0x0000 (8)      | (version info)                                           |
 *  | 0x0300 0x0003 0x0000        (6)      | (general and reserved flags)                             | <128>
 *  |-------------------------------------------------------------------------------------------------|
 *  | <directory entries> (variable)       | (directory entries)                                      |
 *  |                                      | (no entries, 0 bytes)                                    |
 *  |-------------------------------------------------------------------------------------------------|
 *  | <end of directory record> (16 bytes) |                                                          |
 *  | 0xF541 0x2D49 0xBAF8 0xE9C7 (8)      | (signature)                                              |
 *  | 0x0000 0x0000 0x0000 0x0000 (8)      | (directory size, in bytes)                               | <144>
 *  |-------------------------------------------------------------------------------------------------|
 *  | <end of archive record> (8 bytes)    |                                                          |
 *  | 0x5F2A 0xDBEC 0xD284 0x21E0 (8)      | (signature)                                              | <152>
 *  |-------------------------------------------------------------------------------------------------|
 *
 *
 *  example empty file
 *  |---------------------------------------------------------|
 *  | 0x018C 0xE0D6 0xAFD7 0x21A9 0x2071 0xAAF6 0x86B4 0x0FCE |
 *  | 0x0000 0x0000 0x0000 0x0000 0x0300 0x0003 0x0000 0x1812 |
 *  | 0x988C 0x0481 0x3700 0x1812 0x988C 0x0481 0x3700 0x0000 |
 *  | 0x0000 0x0000 0x0000 0x9000 0x0000 0x0000 0x0000 0x9000 |
 *  | 0x0000 0x0000 0x0000 0x5800 0x0000 0x0000 0x0000 0x9AC3 |
 *  | 0xC7EB 0xEFD9 0xc302 0xB9C4 0x00DA 0x080B 0xFFA5 0x0000 |
 *  | 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0300 |
 *  | 0x0003 0x0000 0xF541 0x2D49 0xBAF8 0xE9C7 0x0000 0x0000 |
 *  | 0x0000 0x0000 0x5F2A 0xDBEC 0xD284 0x21E0               |
 *  |---------------------------------------------------------|
 **/

struct SFArchiveEntryFlags
{
  u32 IsDirectory : 1;
  u32 IsReadOnly  : 1;
  u32 IsEmpty     : 1; // can be empty if a file was removed but the empty space not cleared out - if this is an empty entry, then all data should be zero'd out
  u32 Unused      : 29;
};

struct EmptyArchive
{
  SFArchiveHeader Header;
  SFArchiveDirectory Directory;
  EndOfArchiveRecord EndOfArchive;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Header, Directory, EndOfArchive);
  }
};

SPECIALIZE_CONSTRUCT_NEW_ENTITY(EmptyArchive, Ar) 
{ 
  const auto tp = detail::GetCurrentTimeAsEpoch();
  Ar.Header.TimeInfo.Created = tp;
  Ar.Header.TimeInfo.LastModified = tp;
  constexpr u64 size = SizeOf<SFArchiveHeader>::value + SizeOf<SFArchiveDirectoryHeader>::value + SizeOf<EndOfDirectoryRecord>::value + SizeOf<EndOfArchiveRecord>::value + SizeOf<PreSignatureBytes>::value;
  Ar.Header.Size.UncompressedSize = size;
  Ar.Header.Size.CompressedSize = size;
  Ar.Header.OffsetToDirectory = SizeOf<SFArchiveHeader>::value;
}
