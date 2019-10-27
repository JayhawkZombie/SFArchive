#pragma once

#include "Types.h"
#include "Content.h"

SPECIALIZE_CONSTRUCT_NEW_ENTITY(sf_time_point, tp) { tp = zero_time; }

struct ArchiveString
{
  u64         Length = 0;
  std::string Data{ "" };

  template<class Archive>
  void load(Archive &ar)
  {
    ar(Length);
    if (Length != 0)
    {
      ar(Data);
    }
  }

  template<class Archive>
  void save(Archive &ar) const
  {
    ar(Length);
    if (Length != 0)
    {
      ar(Data);
    }
  }
};

template<>
inline u64 LengthOf(ArchiveString &Str)
{
  return LengthOf(Str.Length, Str.Data);
}

struct VersionInformation
{
  u32 VersionCreatedWith = 0;
  u32 MinimumVersionToExtract = 0;

  template<class Archive>
  void serialize(Archive &ar)
  {
    ar(VersionCreatedWith, MinimumVersionToExtract);
  }
};

template<>
inline u64 LengthOf(VersionInformation &Version)
{
  return LengthOf(Version.VersionCreatedWith, Version.MinimumVersionToExtract);
}

using BitFlags4 = std::bitset<4>;
using BitFlags8 = std::bitset<8>;
using BitFlags16 = std::bitset<16>;
using BitFlags32 = std::bitset<32>;
using BitFlags64 = std::bitset<64>;

struct CompressionPluginInfo
{
  ArchiveString Name;

  template<class Archive>
  void serialize(Archive &archive)
  {
    archive(Name);
  }
};

template<>
inline u64 LengthOf(CompressionPluginInfo &Info)
{
  return LengthOf(Info.Name);
}

struct CRCInfo
{
  // temp, until crc implemented
  u64 Value = 0;

  template<class Archive>
  void serialize(Archive &archive)
  {
    archive(Value);
  }
};

template<>
inline u64 LengthOf(CRCInfo &Info)
{
  return LengthOf(Info.Value);
}

struct SizeInfo
{
  u64 CompressedSize = 0;
  u64 UncompressedSize = 0;

  template<class Archive>
  void serialize(Archive &archive)
  {
    archive(CompressedSize, UncompressedSize);
  }
};

template<>
inline u64 LengthOf(SizeInfo &Info)
{
  return LengthOf(Info.UncompressedSize, Info.CompressedSize);
}

struct EntryTimeInfo
{
  sf_time_point Created = zero_time;
  sf_time_point LastModified = zero_time;

  template<class Archive>
  void serialize(Archive &archive)
  {
    archive(Created, LastModified);
  }
};

template<>
inline u64 LengthOf(EntryTimeInfo &Info)
{
  return LengthOf(Info.Created, Info.LastModified);
}

struct ExtraFieldData
{
  ArchiveString Name;
  ArchiveString Data;

  template<class Archive>
  void serialize(Archive &archive)
  {
    archive(Name, Data);
  }
};

template<>
inline u64 LengthOf(ExtraFieldData &Data)
{
  return LengthOf(Data.Name, Data.Data);
}

struct ExtraFieldsInfo
{
  u64                         Count = 0;
  std::vector<ExtraFieldData> Fields;

  template<class Archive>
  void load(Archive &ar)
  {
    ar(Count);
    if (Count != 0)
    {
      Fields.resize(Count, ExtraFieldData{});
      for (auto &field : Fields)
      {
        ar(field);
      }
    }
  }

  template<class Archive>
  void save(Archive &ar) const
  {
    ar(Count);
    if (Count != 0)
    {
      for (const auto &field : Fields)
      {
        ar(field);
      }
    }
  }
};

template<>
inline u64 LengthOf(ExtraFieldsInfo &Info)
{
  return LengthOf(Info.Count, Info.Fields);
}

