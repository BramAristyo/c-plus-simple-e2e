#include "crypt.h"

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

void xor_encrypt_decrypt(string &data, long long key) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key;
    }
}