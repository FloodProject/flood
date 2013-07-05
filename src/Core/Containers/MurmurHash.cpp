#include "Core/API.h"
#include "Core/Containers/MurmurHash.h"

namespace fld
{
	uint64 murmur_hash_64(const void * key, size_t len, uint64 seed)
	{
		const uint64 m = 0xc6a4a7935bd1e995ULL;
		const uint32 r = 47;

		uint64 h = seed ^ (len * m);

		const uint64 * data = (const uint64 *)key;
		const uint64 * end = data + (len/8);

		while(data != end)
		{
			#ifdef PLATFORM_BIG_ENDIAN
				uint64 k = *data++;
				char *p = (char *)&k;
				char c;
				c = p[0]; p[0] = p[7]; p[7] = c;
				c = p[1]; p[1] = p[6]; p[6] = c;
				c = p[2]; p[2] = p[5]; p[5] = c;
				c = p[3]; p[3] = p[4]; p[4] = c;
			#else
				uint64 k = *data++;
			#endif

			k *= m;
			k ^= k >> r;
			k *= m;
			
			h ^= k;
			h *= m;
		}

		const unsigned char * data2 = (const unsigned char*)data;

		switch(len & 7)
		{
		case 7: h ^= uint64(data2[6]) << 48;
		case 6: h ^= uint64(data2[5]) << 40;
		case 5: h ^= uint64(data2[4]) << 32;
		case 4: h ^= uint64(data2[3]) << 24;
		case 3: h ^= uint64(data2[2]) << 16;
		case 2: h ^= uint64(data2[1]) << 8;
		case 1: h ^= uint64(data2[0]);
				h *= m;
		};
		
		h ^= h >> r;
		h *= m;
		h ^= h >> r;

		return h;
	}
}
