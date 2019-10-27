#pragma once

#include "Types.h"
#include "ArchiveTypes.h"

// Signature values
constexpr u64 HeaderSignature               = 0xCE0F'B486'F6AA'7120; //Global header signature
constexpr u64 EndOfHeaderSignature          = 0x02C3'D9EF'EBC7'C39A; //Signature at end of archive header
constexpr u64 EntrySignature                = 0x094E'FD65'F62E'3A05; //Signature held in header of every archive entry
constexpr u64 DirectoryHeaderSignature      = 0xA5FF'0B08'DA00'C4B9; //For directory header
constexpr u64 DirectoryEntrySignature       = 0x938A'671D'B6F1'0C6B; //For every entry in directory
constexpr u64 DirectoryEntryHeaderSignature = 0xFF59'121C'B08E'3E87; //For the header of every entry in directory
constexpr u64 EndOfDirectorySignature       = 0x40D7'C909'3805'B578; //Signature for the mark of the end of the directory
constexpr u64 EndOfDirectoryRecordSignature = 0xC7E9'F8BA'492D'41F5; //
constexpr u64 EndOfArchiveRecordSignature   = 0xE021'84D2'ECDB'2A5F; //Marks end of entire archive

//constexpr std::array<unsigned char, 7> MagicPadding = { 0x8C, 0xE0, 0xD6, 0xAF, 0xD7, 0x21, 0xA9 };

constexpr unsigned char MagicPadding[7] = { 0x8C, 0xE0, 0xD6, 0xAF, 0xD7, 0x21, 0xA9 };

SPECIALIZE_CONSTRUCT_NEW_ENTITY(decltype(MagicPadding), Arr) { }

struct PreSignatureBytes
{
  char Bytes[8] = { 0 };
};

template<>
inline u64 LengthOf(PreSignatureBytes &Psb)
{
  return sizeof(Psb.Bytes);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(PreSignatureBytes, Bytes) { }

template<u64 Length>
struct Padding
{
  std::array<char, Length> arr = { 0 };

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(arr);
  }
};

struct SFArchiveHeader
{
  const unsigned char MagicNumber[7] = { 0x8C, 0xE0, 0xD6, 0xAF, 0xD7, 0x21, 0xA9 };
  u64                 Signature = HeaderSignature;
  VersionInformation  Version = { };
  BitFlags16          GeneralPurposeFlags;
  BitFlags16          ReservedFlags;
  EntryTimeInfo       TimeInfo;
  CRCInfo             Crc;
  SizeInfo            Size;
  u64                 OffsetToDirectory = 0; //offset to directory HEADER
  u64                 EndSignature = EndOfHeaderSignature;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(MagicNumber, Signature, Version, GeneralPurposeFlags, ReservedFlags,
      TimeInfo, Crc, Size, OffsetToDirectory, EndSignature);
  }
};

template<>
inline u64 LengthOf(SFArchiveHeader &Header)
{
  return LengthOf(Header.MagicNumber) + LengthOf(Header.Signature, Header.Version, Header.GeneralPurposeFlags,
    Header.ReservedFlags, Header.TimeInfo, Header.Crc, Header.Size, Header.OffsetToDirectory,
    Header.EndSignature);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveHeader, Header) { }

struct SFArchiveEntryHeader
{
  u64                   Signature = EntrySignature;
  VersionInformation    Version;
  CompressionPluginInfo CompresionInfo;
  EntryTimeInfo         TimeInfo;
  SizeInfo              Size;
  BitFlags8             Flags;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature, Version, CompresionInfo, TimeInfo, Size, Flags);
  }
};

template<>
inline u64 LengthOf(SFArchiveEntryHeader &Header)
{
  return LengthOf(Header.Signature, Header.Version, Header.CompresionInfo,
    Header.TimeInfo, Header.Size, Header.Flags);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveEntryHeader, Header) { }

struct SFArchiveDirectoryHeader
{
  u64                Signature  = DirectoryHeaderSignature;
  u64                NumEntries = 0;
  VersionInformation Version;
  BitFlags16         GeneralFlags;
  BitFlags16         ReservedFlags;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature, NumEntries, Version, GeneralFlags, ReservedFlags);
  }
};

template<>
inline u64 LengthOf(SFArchiveDirectoryHeader &Header)
{
  return LengthOf(Header.Signature, Header.NumEntries, Header.Version,
    Header.GeneralFlags, Header.ReservedFlags);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveDirectoryHeader, Header) { }

struct SFArchiveDirectoryEntryHeader
{
  u64 Signature = DirectoryEntryHeaderSignature;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature);
  }
};

template<>
inline u64 LengthOf(SFArchiveDirectoryEntryHeader &Header)
{
  return LengthOf(Header.Signature);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveDirectoryEntryHeader, Header) { }

struct SFArchiveDirectoryEntry
{
  u64                Signature = DirectoryEntrySignature;
  VersionInformation Version;
  ArchiveString      EntryName;
  u64                OffsetToEntryHeader = 0; // from start of file
  
  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature, Version, EntryName, OffsetToEntryHeader);
  }
};

template<>
inline u64 LengthOf(SFArchiveDirectoryEntry &Entry)
{
  return LengthOf(Entry.Signature, Entry.Version, Entry.EntryName, Entry.OffsetToEntryHeader);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveDirectoryEntry, Entry) { }

struct EndOfDirectoryRecord
{
  u64 Signature     = EndOfDirectoryRecordSignature;
  u64 DirectorySize = 0; // does not include directory header

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature, DirectorySize);
  }
};

template<>
inline u64 LengthOf(EndOfDirectoryRecord &Record)
{
  return LengthOf(Record.Signature, Record.DirectorySize);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(EndOfDirectoryRecord, Record) { }

struct EndOfArchiveRecord
{
  u64 Signature   = EndOfArchiveRecordSignature;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Signature);
  }
};

template<>
inline u64 LengthOf(EndOfArchiveRecord &Record)
{
  return LengthOf(Record.Signature);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(EndOfArchiveRecord, Record) { }

struct SFArchiveDirectory
{
  SFArchiveDirectoryHeader             Header;
  std::vector<SFArchiveDirectoryEntry> Entries;
  EndOfDirectoryRecord                 EndRecord;

  template<class Archive>
  void load(Archive &ar)
  {
    ar(Header);
    if (Header.NumEntries != 0)
    {
      Entries.resize(Header.NumEntries, SFArchiveDirectoryEntry{});
      for (auto &entry : Entries)
      {
        ar(entry);
      }
    }
    ar(EndRecord);
  }

  template<class Archive>
  void save(Archive &ar) const
  {
    ar(Header);
    if (Header.NumEntries != 0)
    {
      for (const auto &entry : Entries)
      {
        ar(entry);
      }
    }
    ar(EndRecord);
  }

};

template<>
inline u64 LengthOf(SFArchiveDirectory &Dir)
{
  return LengthOf(Dir.Header, Dir.Entries, Dir.EndRecord);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveDirectory, Directory) { }

struct SFArchiveDirectory_Empty
{
  SFArchiveDirectoryHeader Header;
  EndOfDirectoryRecord EndRecord;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(Header, EndRecord);
  }
};

template<>
inline u64 LengthOf(SFArchiveDirectory_Empty &Dir)
{
  return LengthOf(Dir.Header, Dir.EndRecord);
}

SPECIALIZE_CONSTRUCT_NEW_ENTITY(SFArchiveDirectory_Empty, Directory) { }
