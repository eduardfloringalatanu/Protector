#include "utils.h"
#include <random>

long generate_random_number(long a, long b) {
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_int_distribution<long> d(a, b);

	return d(mt);
}

void generate_random_string(const char* const alphabet, char* output, unsigned long length) {
	for (unsigned long i = 0; i < length; ++i)
		output[i] = alphabet[generate_random_number(0, strlen(alphabet) - 1)];
	output[length] = '\0';
}