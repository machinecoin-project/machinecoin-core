#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "arith_uint256.h"
#include "sph_blake.h"
#include "sph_bmw.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"
#include "sph_luffa.h"
#include "sph_cubehash.h"
#include "sph_shavite.h"
#include "sph_simd.h"
#include "sph_echo.h"
#include "sph_hamsi.h"
#include "sph_fugue.h"
#include "util.h"
#include <algorithm>  
#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_blake512_context     z_blake;
GLOBAL sph_bmw512_context       z_bmw;
GLOBAL sph_groestl512_context   z_groestl;
GLOBAL sph_jh512_context        z_jh;
GLOBAL sph_keccak512_context    z_keccak;
GLOBAL sph_skein512_context     z_skein;
GLOBAL sph_luffa512_context     z_luffa;
GLOBAL sph_cubehash512_context  z_cubehash;
GLOBAL sph_shavite512_context   z_shavite;
GLOBAL sph_simd512_context      z_simd;
GLOBAL sph_echo512_context      z_echo;
GLOBAL sph_hamsi512_context     z_hamsi;
GLOBAL sph_fugue512_context     z_fugue;

#define fillz() do { \
    sph_blake512_init(&z_blake); \
    sph_bmw512_init(&z_bmw); \
    sph_groestl512_init(&z_groestl); \
    sph_jh512_init(&z_jh); \
    sph_keccak512_init(&z_keccak); \
    sph_skein512_init(&z_skein); \
    sph_luffa512_init(&z_luffa); \
    sph_cubehash512_init(&z_cubehash); \
    sph_shavite512_init(&z_shavite); \
    sph_simd512_init(&z_simd); \
    sph_echo512_init(&z_echo); \
    sph_hamsi512_init(&z_hamsi); \
    sph_fugue512_init(&z_fugue); \
} while (0) 


#define ZBLAKE (memcpy(&ctx_blake, &z_blake, sizeof(z_blake)))
#define ZBMW (memcpy(&ctx_bmw, &z_bmw, sizeof(z_bmw)))
#define ZGROESTL (memcpy(&ctx_groestl, &z_groestl, sizeof(z_groestl)))
#define ZJH (memcpy(&ctx_jh, &z_jh, sizeof(z_jh)))
#define ZKECCAK (memcpy(&ctx_keccak, &z_keccak, sizeof(z_keccak)))
#define ZSKEIN (memcpy(&ctx_skein, &z_skein, sizeof(z_skein)))
#define ZHAMSI (memcpy(&ctx_hamsi, &z_hamsi, sizeof(z_hamsi)))
#define ZFUGUE (memcpy(&ctx_fugue, &z_fugue, sizeof(z_fugue)))

typedef   void(*HashFunction)(void *, const void *, size_t);
typedef   void(*HashInitFunction)(void *);
typedef   void(*HashCloseFunction)(void *, void *);

struct HashProvider {
	HashFunction HashFunc;
	HashInitFunction HashInitFunc;
	HashCloseFunction HashCloseFunc;
	void* HashContext;
};


enum Algo{
	BLAKE,
	BMW,
	GROESTL,
	SKEIN,
	JH,
	KECCAK,
	LUFFA,
	CUBEHASH,
	SHAVITE,
	SIMD,
	ECHO,
    HAMSI,
    FUGUE
};



inline unsigned arraysize(Algo *seq) { return 13; }

inline int getCurrentAlgoSeq(uint32_t current_time, uint32_t base_time){
	return (current_time - base_time) / (60 * 60 * 24);
}

inline Algo* genAlgoSequence(int number) {
	Algo tmpl[] = {
		BLAKE,
		BMW,
		GROESTL,
		SKEIN,
		JH,
		KECCAK,
		LUFFA,
		CUBEHASH,
		SHAVITE,
		SIMD,
		ECHO,
        HAMSI,
        FUGUE
	};

	Algo *list = new Algo[13];

	for (int i = 0; i < arraysize(tmpl); ++i){
		list[i] = tmpl[i];
	}


	bool remixEnabled = true;

	int cur_seq = 0;
	do
	{
		if (cur_seq >= number)
			break;
		cur_seq++;
	} while (remixEnabled && std::next_permutation(list, list + arraysize(list)));


	return list;
}


inline void logSequence(Algo* seq){
	LogPrintf("===BEGIN ALGO SEQ===\n");
	unsigned size = arraysize(seq);
	for (unsigned i = 0; i < size; ++i){
		Algo algo = seq[i];
		switch (algo){
		case BLAKE:
			LogPrintf("BLAKE\n");
			break;

		case BMW:
			LogPrintf("BMW\n");
			break;

		case GROESTL:
			LogPrintf("GROESTL\n");
			break;

		case SKEIN:
			LogPrintf("SKEIN\n");
			break;

		case JH:
			LogPrintf("JH\n");
			break;

		case KECCAK:
			LogPrintf("KECCAK\n");
			break;

		case LUFFA:
			LogPrintf("LUFFA\n");
			break;

		case CUBEHASH:
			LogPrintf("CUBEHASH\n");
			break;

		case SHAVITE:
			LogPrintf("SHAVITE\n");
			break;

		case SIMD:
			LogPrintf("SIMD\n");
			break;

		case ECHO:
			LogPrintf("ECHO\n");
			break;

		case HAMSI:
			LogPrintf("HAMSI\n");
			break;

		case FUGUE:
			LogPrintf("FUGUE\n");
			break;
		}
	}
	LogPrintf("===END ALGO SEQ===\n");
}

inline HashProvider getHashProvider(Algo algo){
	HashProvider result;

	switch (algo){
	case BLAKE:
		result.HashFunc = &sph_blake512;
		result.HashInitFunc = &sph_blake512_init;
		result.HashCloseFunc = &sph_blake512_close;
		result.HashContext = new sph_blake512_context();
		break;

	case BMW:
		result.HashFunc = &sph_bmw512;
		result.HashInitFunc = &sph_bmw512_init;
		result.HashCloseFunc = &sph_bmw512_close;
		result.HashContext = new sph_bmw512_context();
		break;

	case GROESTL:
		result.HashFunc = &sph_groestl512;
		result.HashInitFunc = &sph_groestl512_init;
		result.HashCloseFunc = &sph_groestl512_close;
		result.HashContext = new sph_groestl512_context();
		break;

	case SKEIN:
		result.HashFunc = &sph_skein512;
		result.HashInitFunc = &sph_skein512_init;
		result.HashCloseFunc = &sph_skein512_close;
		result.HashContext = new sph_skein512_context();
		break;

	case JH:
		result.HashFunc = &sph_jh512;
		result.HashInitFunc = &sph_jh512_init;
		result.HashCloseFunc = &sph_jh512_close;
		result.HashContext = new sph_jh512_context();
		break;

	case KECCAK:
		result.HashFunc = &sph_keccak512;
		result.HashInitFunc = &sph_keccak512_init;
		result.HashCloseFunc = &sph_keccak512_close;
		result.HashContext = new sph_keccak512_context();
		break;

	case LUFFA:
		result.HashFunc = &sph_luffa512;
		result.HashInitFunc = &sph_luffa512_init;
		result.HashCloseFunc = &sph_luffa512_close;
		result.HashContext = new sph_luffa512_context();
		break;

	case CUBEHASH:
		result.HashFunc = &sph_cubehash512;
		result.HashInitFunc = &sph_cubehash512_init;
		result.HashCloseFunc = &sph_cubehash512_close;
		result.HashContext = new sph_cubehash512_context();
		break;

	case SHAVITE:
		result.HashFunc = &sph_shavite512;
		result.HashInitFunc = &sph_shavite512_init;
		result.HashCloseFunc = &sph_shavite512_close;
		result.HashContext = new sph_shavite512_context();
		break;

	case SIMD:
		result.HashFunc = &sph_simd512;
		result.HashInitFunc = &sph_simd512_init;
		result.HashCloseFunc = &sph_simd512_close;
		result.HashContext = new sph_simd512_context();
		break;

	case ECHO:
		result.HashFunc = &sph_echo512;
		result.HashInitFunc = &sph_echo512_init;
		result.HashCloseFunc = &sph_echo512_close;
		result.HashContext = new sph_echo512_context();
		break;

	case HAMSI:
		result.HashFunc = &sph_hamsi512;
		result.HashInitFunc = &sph_hamsi512_init;
		result.HashCloseFunc = &sph_hamsi512_close;
		result.HashContext = new sph_hamsi512_context();
		break;

	case FUGUE:
		result.HashFunc = &sph_fugue512;
		result.HashInitFunc = &sph_fugue512_init;
		result.HashCloseFunc = &sph_fugue512_close;
		result.HashContext = new sph_fugue512_context();
		break;
	}


	return result;
}

template<typename T1>
inline uint256 HashX13Evo(const T1 pbegin, const T1 pend, uint32_t time)
{
    static unsigned char pblank[1];

    arith_uint512 hash[13];

	Algo* list = genAlgoSequence(getCurrentAlgoSeq(time, 1389040865)); // Machinecoin: 1389040865
	//logSequence(list);

	unsigned size = arraysize(list);
	bool first = true;

	int hash_index = 0;
	for (unsigned i = 0; i < size; ++i){
		Algo algo = list[i];

		HashProvider provider = getHashProvider(algo);

		(*provider.HashInitFunc)(provider.HashContext);
		if (first){
			first = false;
			(*provider.HashFunc)(provider.HashContext, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
			(*provider.HashCloseFunc)(provider.HashContext, static_cast<void*>(&hash[0]));
		}
		else{
			(*provider.HashFunc)(provider.HashContext, static_cast<void*>(&hash[hash_index]), 64);
			(*provider.HashCloseFunc)(provider.HashContext, static_cast<void*>(&hash[++hash_index]));
		}

		delete provider.HashContext;
	}


	return ArithToUint256(hash[size - 1].trim256());
}

#endif // HASHBLOCK_H
