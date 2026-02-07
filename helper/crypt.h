#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

using namespace std;

long long mod_exp(long long base, long long exp, long long mod);

void xor_encrypt_decrypt(string &data, long long key);

#endif