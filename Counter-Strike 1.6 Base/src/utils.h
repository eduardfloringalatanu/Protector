#pragma once

const char alphanumeric_alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

long generate_random_number(long a, long b);
void generate_random_string(const char* const alphabet, char* output, unsigned long length);