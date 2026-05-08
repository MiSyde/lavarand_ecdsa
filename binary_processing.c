#include "binary_processing.h"

uint32_t h0 = 0x6a09e667;
uint32_t h1 = 0xbb67ae85;
uint32_t h2 = 0x3c6ef372;
uint32_t h3 = 0xa54ff53a;
uint32_t h4 = 0x510e527f;
uint32_t h5 = 0x9b05688c;
uint32_t h6 = 0x1f83d9ab;
uint32_t h7 = 0x5be0cd19;
const unsigned int constants[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b ,0xc24b8b70 ,0xc76c51a3 ,0xd192e819 ,0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3 ,0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee ,0x78a5636f, 0x84c87814, 0x8cc70208 ,0x90befffa, 0xa4506ceb ,0xbef9a3f7, 0xc67178f2};
unsigned char* pack(const unsigned char* binary, const int size) {
    const int packed_size = (size + 7) / 8;
    unsigned char* packed = calloc(packed_size, 1);
    for (int i = 0; i < size; ++i) {
        if (binary[i] == 255) {
            packed[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
    return packed;
}

unsigned char* padding(unsigned char* packed_binary, const int size, int* total_blocks) {
    const int packed_size = (size + 7) / 8;
    const int remainder = packed_size % 64;
    const int needed_padding = (remainder < 56) ? (56 - remainder) : (120 - remainder);
    const int total_size = packed_size + needed_padding + 8;
    *total_blocks = total_size / 64;
    unsigned char* temp = realloc(packed_binary, packed_size + needed_padding + 8);
    if (temp == NULL) {
        perror("Failed to reallocate memory at binary_processing => padding.\nexiting...");
        free(temp);
        temp = NULL;
        free(packed_binary);
        packed_binary = NULL;
        exit(1);
    }
    packed_binary = temp;
    packed_binary[packed_size] = 0x80;
    memset(packed_binary + packed_size + 1, 0, needed_padding - 1);
    uint64_t bit_length = (uint64_t)packed_size * 8;
    for (int i = 7; i >= 0; --i) {
        packed_binary[packed_size + needed_padding + i] = bit_length & 0xFF;
        bit_length >>= 8;
    }
    return packed_binary;
}

uint32_t* create_word_block(const unsigned char* packed_binary, const int block_start) {
    uint32_t* words = malloc(sizeof(uint32_t)*64);
    for (int i = 0; i < 16; ++i) {
        const int b = block_start + i * 4;
        words[i] = (uint32_t)packed_binary[b] << 24
                                        | (uint32_t)packed_binary[b+1] << 16
                                        | (uint32_t)packed_binary[b+2] << 8
                                        | (uint32_t)packed_binary[b+3];
    }
    memset(words + 16, 0, 48*sizeof(uint32_t));
    for (int i = 16; i < 64; ++i) {
        const uint32_t s0 = ROTR(words[i-15], 7) ^ ROTR(words[i-15], 18) ^ (words[i-15] >> 3);
        const uint32_t s1 = ROTR(words[i-2], 17) ^ ROTR(words[i-2], 19)  ^ (words[i-2]  >> 10);
        words[i] = words[i-16] + s0 + words[i-7] + s1;
    }
    return words;
}

void compress(const unsigned char* packed_binary, const int total_blocks) {
    for (int block = 0; block < total_blocks; ++block) {
        uint32_t* words = create_word_block(packed_binary, block * 64);

        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;

        for (int i = 0; i < 64; ++i) {
            const uint32_t S0 = ROTR(a,2) ^ ROTR(a,13) ^ ROTR(a,22);
            const uint32_t S1 = ROTR(e,6) ^ ROTR(e,11) ^ ROTR(e,25);
            const uint32_t Ch = (e & f) ^ (~e & g);
            const uint32_t Maj = (a & b) ^ (a & c) ^ (b & c);
            const uint32_t temp1 = h + S1 + Ch + constants[i] + words[i];
            const uint32_t temp2 = S0 + Maj;

            h = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }
        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e; h5 += f; h6 += g; h7 += h;

        free(words);
    }
}

unsigned char* concatenate() {
    unsigned char* hash = malloc(sizeof(unsigned char)*32);
    const uint32_t h[8] = {h0, h1, h2, h3, h4, h5, h6, h7};

    for (int i = 0; i < 8; ++i) {
        hash[i*4] = (h[i] >> 24) & 0xFF;
        hash[i*4+1] = (h[i] >> 16) & 0xFF;
        hash[i*4+2] = (h[i] >> 8) & 0xFF;
        hash[i*4+3] = h[i] & 0xFF;
    }

    return hash;
}

void print_sha(const unsigned char* sha) {
    for (int i = 0; i < 32; ++i) {
        printf("%02x", sha[i]);
    }
}

void print_bits(const unsigned char v) {
    for(int i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}