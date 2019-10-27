#pragma once

#include "Format.h"

enum ArchiveMode : u32
{
  Create        = 0b0000'0000'0000'0000'0000'0000'0000'0001,
  Append        = 0b0000'0000'0000'0000'0000'0000'0000'0010,
  Truncate      = 0b0000'0000'0000'0000'0000'0000'0000'0100,
  ReadOnly      = 0b0000'0000'0000'0000'0000'0000'0000'1000,
  CheckContents = 0b0000'0000'0000'0000'0000'0000'0001'0000,
  ErrorIfExists = 0b0000'0000'0000'0000'0000'0000'0010'0000
};

enum class ArchiveError
{
  AlreadyExists      = 0b0000'0000'0000'0000'0000'0000'0100'0000,
  Corrupted          = 0b0000'0000'0000'0000'0000'0000'1000'0000,
  InvalidPath        = 0b0000'0000'0000'0000'0000'0001'0000'0000,
  UnrecognizedFormat = 0b0000'0000'0000'0000'0000'0010'0000'0000,
  CannotOpen         = 0b0000'0000'0000'0000'0000'0100'0000'0000,
  NotCreated         = 0b0000'0000'0000'0000'0000'1000'0000'0000
};

class SFArchive;

class SFArchiveEntry
{
public:
  friend class SFArchive;

private:

  SFArchiveEntryHeader m_Header;
  

};

class SFArchive
{
public:

  SFArchive() = default;
  SFArchive(const std::string &Name)
  {
    m_File.open(Name, std::ios_base::binary | std::ios_base::out);

    if (!m_File)
      throw std::runtime_error("Failed to open file");
  }

  virtual ~SFArchive()
  {
    if (m_File.is_open())
    {
      m_File.flush();
      m_File.close();
    }
  }

  bool CreateNew(const std::string &Name)
  {
    try
    {
      //EmptyArchive archive;
      SFArchiveHeader Header;
      SFArchiveDirectory Directory;
      EndOfArchiveRecord EndOfArchive;

      
      detail::Construct construct;

      construct(Header, Directory, EndOfArchive);
      Header.Size.UncompressedSize = SizeOf<SFArchiveHeader>::value
                                   + SizeOf<SFArchiveDirectoryHeader>::value
                                   + SizeOf<EndOfDirectoryRecord>::value
                                   + SizeOf<EndOfArchiveRecord>::value;
      Header.Size.CompressedSize = Header.Size.UncompressedSize;
      Header.OffsetToDirectory = SizeOf<SFArchiveHeader>::value;

      //std::ofstream out(Name, std::ios_base::binary);

      cereal::PortableBinaryOutputArchive ar(m_File);
      ar(Header, Directory, EndOfArchive);

      return true;
    }
    catch (const std::exception &exc)
    {
      //... damn
      std::cerr << "Failed to create new archive: " << exc.what() << std::endl;
    }

    return false;
  }

  bool AddFile(const std::string &SrcFile, const std::string &DstFile)
  {
    try
    {
      if (!std::filesystem::exists(SrcFile) || !std::filesystem::is_regular_file(SrcFile))
        return false;

      SFArchiveHeader Header;
      SFArchiveDirectory Directory;
      EndOfArchiveRecord EndOfArchive;

      {

      }
    }
    catch (const std::exception&)
    {

    }

    return false;
  }

private:

private:

  std::fstream m_File;

};
