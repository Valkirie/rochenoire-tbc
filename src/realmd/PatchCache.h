#ifndef _PATCHCACHE_H
#define _PATCHCACHE_H

#include "Common.h"
#include "Policies/Singleton.h"
#include <mutex>
#include <openssl/md5.h>

class PatchCache
{
public:
	struct Patch
	{
		uint16 build;
		int locale;
		std::string filename;
		uint64 filesize;
		uint8 md5[MD5_DIGEST_LENGTH];
	};

	void Initialize();
	Patch GetPatchInfo(uint16 _build, std::string _locale); //returns build number 0 if patch md5 not found
private:
	std::string _baseFolder;
};

#define sPatchCache MaNGOS::Singleton<PatchCache>::Instance()

#endif
