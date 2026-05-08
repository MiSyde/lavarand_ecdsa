#ifndef LAVARAND_ECDSA_BINARY_PROCESSING_H
#define LAVARAND_ECDSA_BINARY_PROCESSING_H
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern uint32_t h0;
extern uint32_t h1;
extern uint32_t h2;
extern uint32_t h3;
extern uint32_t h4;
extern uint32_t h5;
extern uint32_t h6;
extern uint32_t h7;
extern const unsigned int constants[64];

unsigned char* pack(const unsigned char* binary, int size);

unsigned char* padding(unsigned char* packed_binary, int size, int* tot);

uint32_t* block(const unsigned char* packed_binary, int block_start);

void compress(const unsigned char* packed_binary, int total_blocks);

unsigned char* concatenate();

void print_bits(unsigned char v);

void print_sha(const unsigned char* sha);

#endif