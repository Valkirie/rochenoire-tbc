#include "PatchCache.h"
#include "Log.h"
#include "Config/Config.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

static uint8 hexchr2bin(char hex)
{
    if (hex >= '0' && hex <= '9')
        return uint8(hex - '0');
    else
        return uint8(hex - 'A') + 10;
}

template<size_t len>
static void hexs2bin(const char* hex, uint8* out) //absolutely no validation, make sure hex string is correct before calling
{
    for (size_t i=0; i<len; i++) 
    {
        const uint8 b1 = hexchr2bin(*hex++);
        const uint8 b2 = hexchr2bin(*hex++);
        *out++ = (b1 << 4) | b2;
    }
}

PatchCache::Patch PatchCache::GetPatchInfo(uint16 build, std::string localeStr)
{
    Patch patch;
    patch.build = 0;
    patch.locale = 0;
    patch.filesize = 0;
    memset(patch.md5,0,sizeof(patch.md5));

    char fullPath[256];
    std::ifstream descrFile;
    sprintf(fullPath,"%s/%u%s.md5",_baseFolder.c_str(),uint32(build),localeStr.c_str());
    descrFile.open(fullPath);
    if (descrFile.is_open())
        DEBUG_LOG("Opened '%s' patch descr file for version %u and locale %s.",fullPath,uint32(build),localeStr.c_str());
    else
    {
        sLog.outError("Error opening '%s' patch descr file for version %u and locale %s.",fullPath,uint32(build),localeStr.c_str());
        return patch;
    }

    std::string descrLine;
    if (descrFile.is_open())
    {
        std::getline(descrFile,descrLine);
        descrFile.close();
    }

    std::string fileName;
    if (descrLine.length() > 0)
    {
        auto spaceIt = descrLine.find_first_of(' ');
        if (spaceIt == std::string::npos)
        {
            sLog.outError("Patch descr file '%s' invalid line format (no space).",fullPath);
            return patch;
        }
        else
        {
            fileName = descrLine.substr(spaceIt+1);
            descrLine.resize(spaceIt);
        }
    }

    const char* fnamePtr = fileName.c_str();
    while (*fnamePtr == '*' || *fnamePtr == ' ') //binary or ascii encoding specifier of filename, just ignore
        fnamePtr++;

    if (descrLine.length() != sizeof(patch.md5)*2)
    {
        sLog.outError("Patch descr file '%s' invalid line format (md5 hash invalid length).",fullPath);
        return patch;
    }
    else
    {
        for (size_t i=0; i<descrLine.length(); i++)
        {
            const char lchar = std::toupper(descrLine[i]);
            descrLine[i] = lchar;
            if (lchar >= '0' && lchar <= '9')
                continue;
            if (lchar >= 'A' && lchar <= 'F')
                continue;

            sLog.outError("Patch descr file '%s' invalid line format (md5 hash invalid char at position %u).",fullPath,uint32(i));
            return patch;
        }
    }

    if (*fnamePtr == 0)
    {
        sLog.outError("Patch descr file '%s' invalid line format (missing filename).",fullPath);
        return patch;
    }
    else
    {
        struct _stat fileStat;
        memset(&fileStat,0,sizeof(fileStat));
        sprintf(fullPath,"%s/%s",_baseFolder.c_str(),fnamePtr);
        if (_stat(fullPath,&fileStat) != 0 || (fileStat.st_mode & _S_IFDIR) != 0)
        {
            sLog.outError("Error querying patch file '%s' size.",fullPath);
            return patch;
        }
        else
        {
            patch.filename = fullPath;
            patch.filesize = uint64(fileStat.st_size);
        }
    }

    patch.build = build;
    patch.locale = *((int*)(localeStr.c_str()));
    hexs2bin<sizeof(patch.md5)>(descrLine.c_str(),patch.md5);

    return patch;
}

void PatchCache::Initialize()
{
    _baseFolder = sConfig.GetStringDefault("PatchesDir","./patches");
    DEBUG_LOG("Patches directory set to '%s'.",_baseFolder.c_str());
}