#include "revemu.h"
#include "string.h"
#include "../hlsdk.h"
#include "../utils.h"
#include "Windows.h"

char s_szDictionary[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

unsigned long iInputLen;
int uTreasure;

bool ScanLast3(char* pszInput, unsigned long uPrevHash) {
	unsigned long h1, h2, h3, hh;
	int i1, i2, i3;

	for (i1 = 0; i1 < ARRAYSIZE(s_szDictionary) - 1; i1++) {
		h1 = uPrevHash ^ ((uPrevHash >> 2) + (uPrevHash << 5) + s_szDictionary[i1]);
		hh = h1 ^ ((h1 >> 2) + (h1 << 5));
		hh = hh ^ ((hh >> 2) + (hh << 5));

		if ((hh ^ uTreasure) >> (8 + 5 + 3))
			continue;

		for (i2 = 0; i2 < ARRAYSIZE(s_szDictionary) - 1; i2++) {
			h2 = h1 ^ ((h1 >> 2) + (h1 << 5) + s_szDictionary[i2]);
			hh = h2 ^ ((h2 >> 2) + (h2 << 5));

			if ((hh ^ uTreasure) >> (8 + 3))
				continue;

			for (i3 = 0; i3 < ARRAYSIZE(s_szDictionary) - 1; i3++) {
				h3 = h2 ^ ((h2 >> 2) + (h2 << 5) + s_szDictionary[i3]);

				if (h3 == uTreasure) {
					pszInput[iInputLen - 3] = s_szDictionary[i1];
					pszInput[iInputLen - 2] = s_szDictionary[i2];
					pszInput[iInputLen - 1] = s_szDictionary[i3];

					return true;
				}
			}
		}
	}

	return false;
}

bool ScanNext(char* pszInput, unsigned long uIndex, unsigned long uPrevHash) {
	bool res;

	for (int i = 0; i < ARRAYSIZE(s_szDictionary) - 1; i++) {
		unsigned long h = uPrevHash ^ ((uPrevHash >> 2) + (uPrevHash << 5) + s_szDictionary[i]);

		if (uIndex + 1 < iInputLen - 3)
			res = ScanNext(pszInput, uIndex + 1, h);
		else
			res = ScanLast3(pszInput, h);

		if (res) {
			pszInput[uIndex] = s_szDictionary[i];

			return true;
		}
	}

	return false;
}

unsigned long calculate_revemu_hash(const char* auth_key) {
	unsigned long hash = 0x4E67C6A7;

	for (; *auth_key != '\0'; ++auth_key)
		hash ^= (hash >> 2) + (hash << 5) + *auth_key;

	return hash;
}

bool Spoof(char* pszDest, int uSID) {
	uTreasure = uSID;
	iInputLen = strlen(pszDest);

	unsigned long i = iInputLen - 7;
	i = (i < 0) ? 0 : i;

	pszDest[i] = '\0';

	unsigned long hash = calculate_revemu_hash(pszDest);

	return ScanNext(pszDest, i, hash);
}

int generate_revemu(void* pData, int steamid) {
	char auth_key[128];
	generate_random_string(alphanumeric_alphabet, auth_key, ARRAYSIZE(auth_key) - 1);

	if (steamid != 0 && !Spoof(auth_key, steamid))
		return 0;

	unsigned long* ticket = (unsigned long*)pData;

	unsigned long hash = calculate_revemu_hash(auth_key);

	ticket[0] = 'J';
	ticket[1] = hash;
	ticket[2] = 'rev';
	ticket[3] = 0;
	ticket[4] = hash << 1;
	ticket[5] = 0x01100001;
	strcpy((char*)&ticket[6], auth_key);

	return 152;
}