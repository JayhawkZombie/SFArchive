//#pragma once
//
//#include "Headers.h"
//#include "Helpers.h"
//#include "Signatures.h"
//
//namespace detail
//{
//  /*
//   *  Construct pod/arithmetic types
//   **/
//  template<> void Construct<u64>(u64 &value) { value = 0; }
//  template<> void Construct<u32>(u32 &value) { value = 0; }
//  template<> void Construct<i64>(i64 &value) { value = 0; }
//  template<> void Construct<i32>(i32 &value) { value = 0; }
//  template<> void Construct<float>(float &value) { value = 0.f; }
//  template<> void Construct<double>(double &value) { value = 0.0; }
//  template<> void Construct<sf_time_point>(sf_time_point &tp) { tp = zero_time; }
//
//  /*
//   *  Construct empty std::string
//   **/
//  template<>
//  void Construct<std::string>(std::string &str)
//  {
//    str.assign("");
//  }
//
//  /*
//   *  Construct an std::vector
//   **/
//  template<class T>
//  void Construct<std::vector<T>>(std::vector<T> &vec)
//  {
//    vec.clear();
//  }
//
//  /*
//   *  Construct new time info
//   **/
//  template<>
//  void Construct<EntryTimeInfo>(EntryTimeInfo &Ft)
//  {
//    Construct(Ft.Created);
//    Construct(Ft.LastModified);
//  }
//
//  /*
//   *  Construct archive string
//   **/
//  template<>
//  void Construct<ArchiveString>(ArchiveString &Str)
//  {
//    Construct(Str.Length);
//    Construct(Str.Data);
//  }
//
//  /*
//   *  Construct version info
//   **/
//  template<>
//  void Construct<VersionInformation>(VersionInformation &Version)
//  {
//    Construct(Version.VersionCreatedWith);
//    Construct(Version.MinimumVersionToExtract);
//  }
//
//  /*
//   *  Construct compression plugin info
//   **/
//  template<>
//  void Construct<CompressionPluginInfo>(CompressionPluginInfo &Info)
//  {
//    Construct(Info.Name);
//  }
//
//  /*
//   *  Construct crc info
//   **/
//  template<>
//  void Construct<CRCInfo>(CRCInfo &Info)
//  {
//    Construct(Info.Value);
//  }
//
//  template<>
//  void Construct<SizeInfo>(SizeInfo &Info)
//  {
//    Info.UncompressedSize = 0;
//    Info.CompressedSize = 0;
//  }
//
//  template<>
//  void Construct<ExtraFieldData>(ExtraFieldData &Data)
//  {
//    Construct(Data.Name);
//    Construct(Data.Data);
//  }
//
//  template<>
//  void Construct<ExtraFieldsInfo>(ExtraFieldsInfo &Info)
//  {
//    Construct(Info.Count);
//    Construct(Info.Fields);
//  }
//
//  /*
//   *  Construct new archive header entity (with timestamps showing NOW as the creation & modification time)
//   **/
//  template<>
//  void Construct<SFArchiveHeader>(SFArchiveHeader &Header)
//  {
//    Header.Signature    = HeaderSignature;
//    Header.EndSignature = EndOfHeaderSignature;
//    Construct(Header.TimeInfo);
//    Construct(Header.Crc);
//    Construct(Header.Version);
//    Construct(Header.OffsetToDirectory);
//  }
//
//  template<>
//  void Construct<SFArchiveEntryHeader>(SFArchiveEntryHeader &Header)
//  {
//    Header.Signature = EntrySignature;
//    Construct(Header.CompresionInfo);
//    Construct(Header.Size);
//    Construct(Header.Version);
//    Construct(Header.TimeInfo);
//  }
//
//  template<>
//  void Construct<SFArchiveDirectoryHeader>(SFArchiveDirectoryHeader &Header)
//  {
//    Header.Signature = DirectoryHeaderSignature;
//    Construct(Header.Version);
//    Construct(Header.NumEntries);
//    Construct(Header.Version);
//  }
//
//  template<>
//  void Construct<SFArchiveDirectoryEntry>(SFArchiveDirectoryEntry &Entry)
//  {
//    Entry.Signature = EntrySignature;
//    Construct(Entry.OffsetToEntryHeader);
//    Construct(Entry.Version);
//    Construct(Entry.EntryName);
//  }
//
//}
