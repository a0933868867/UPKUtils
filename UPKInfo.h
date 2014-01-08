///
/// This is the class to extract header information from UPK files:
/// summary, compressed chunks, name table, import table, export table and
/// depends table
///
#ifndef UPKINFO_H
#define UPKINFO_H

#include <vector>
#include <iostream>

#include "UFlags.h"

typedef int32_t UObjectReference;

struct FGuid
{
    /// persistent
    uint32_t GUID_A;
    uint32_t GUID_B;
    uint32_t GUID_C;
    uint32_t GUID_D;
};

struct FGenerationInfo
{
    int32_t ExportCount;
    int32_t NameCount;
    int32_t NetObjectCount;
};

struct UNameIndex
{
    uint32_t NameTableIdx;
    uint32_t Numeric;
};

struct FPackageFileSummary
{
    /// persistent
    uint32_t Signature;
    uint16_t Version;
    uint16_t LicenseeVersion;
    uint32_t HeaderSize;
    int32_t FolderNameLength;
    std::string FolderName;
    uint32_t PackageFlags;
    uint32_t NameCount;
    uint32_t NameOffset;
    uint32_t ExportCount;
    uint32_t ExportOffset;
    uint32_t ImportCount;
    uint32_t ImportOffset;
    uint32_t DependsOffset;
    uint32_t SerialOffset;
    uint32_t Unknown2;
    uint32_t Unknown3;
    uint32_t Unknown4;
    FGuid   GUID;
    uint32_t GenerationsCount;
    std::vector<FGenerationInfo> Generations;
    uint32_t EngineVersion;
    uint32_t CookerVersion;
    uint32_t CompressionFlags;
    uint32_t NumCompressedChunks;
    std::vector<char> CompressedChunksBuf;
    /// memory
    size_t HeaderSizeOffset;
    size_t NameCountOffset;
    size_t UPKFileSize;
};

struct FNameEntry
{
    /// persistent
    int32_t     NameLength;
    std::string Name;
    uint32_t    NameFlagsL;
    uint32_t    NameFlagsH;
    /// memory
    size_t      EntryOffset;
    size_t      EntrySize;
};

struct FObjectImport
{
    /// persistent
    UNameIndex       PackageIdx;
    UNameIndex       TypeIdx;
    UObjectReference OwnerRef;
    UNameIndex       NameIdx;
    /// memory
    size_t           EntryOffset;
    size_t           EntrySize;
    std::string      Name;
    std::string      FullName;
    std::string      Type;
};

struct FObjectExport
{
    /// persistent
    UObjectReference TypeRef;
    UObjectReference ParentClassRef;
    UObjectReference OwnerRef;
    UNameIndex       NameIdx;
    UObjectReference ArchetypeRef;
    uint32_t         ObjectFlagsH;
    uint32_t         ObjectFlagsL;
    uint32_t         SerialSize;
    uint32_t         SerialOffset;
    uint32_t         ExportFlags;
    uint32_t         NetObjectCount;
    FGuid            GUID;
    uint32_t         Unknown1;
    std::vector<uint32_t> NetObjects;     // 4 x NetObjectCount bytes of data
    /// memory
    size_t           EntryOffset;
    size_t           EntrySize;
    std::string      Name;
    std::string      FullName;
    std::string      Type;
};

class UPKInfo
{
    public:
        /// constructors
        UPKInfo();
        UPKInfo(std::istream& stream);
        /// destructor
        ~UPKInfo();
        /// read package header
        void Read(std::istream& stream);
        /// helpers
        std::string IndexToName(UNameIndex idx);
        std::string ObjRefToName(UObjectReference ObjRef);
        std::string ResolveFullName(UObjectReference ObjRef);
        UObjectReference GetOwnerRef(UObjectReference ObjRef);
        int FindName(std::string name);
        UObjectReference FindObject(std::string FullName, bool isExport = true);
        /// Getters
        FObjectExport GetExportEntry(uint32_t idx);
        /// format header to text string
        std::string FormatSummary();
        std::string FormatNames(bool verbose = false);
        std::string FormatImports(bool verbose = false);
        std::string FormatExports(bool verbose = false);
        std::string FormatName(uint32_t idx, bool verbose = false);
        std::string FormatImport(uint32_t idx, bool verbose = false);
        std::string FormatExport(uint32_t idx, bool verbose = false);
    protected:
        FPackageFileSummary Summary;
        std::vector<FNameEntry> NameTable;
        std::vector<FObjectImport> ImportTable;
        std::vector<FObjectExport> ExportTable;
        std::vector<char> DependsBuf;
};

/// helper functions
std::string FormatHEX(uint32_t val);
std::string FormatHEX(uint16_t val);
std::string FormatHEX(uint8_t val);
std::string FormatHEX(FGuid GUID);
std::string FormatHEX(UNameIndex NameIndex);
std::string FormatHEX(uint32_t L, uint32_t H);
std::string FormatHEX(std::vector<char> DataChunk);
std::string FormatHEX(std::string DataString);
/// format flags
std::string FormatPackageFlags(uint32_t flags);
std::string FormatCompressionFlags(uint32_t flags);
std::string FormatObjectFlagsL(uint32_t flags);
std::string FormatObjectFlagsH(uint32_t flags);
std::string FormatExportFlags(uint32_t flags);
std::string FormatFunctionFlags(uint32_t flags);
std::string FormatStructFlags(uint32_t flags);
std::string FormatClassFlags(uint32_t flags);
std::string FormatStateFlags(uint32_t flags);
std::string FormatPropertyFlagsL(uint32_t flags);
std::string FormatPropertyFlagsH(uint32_t flags);

#endif // UPKINFO_H