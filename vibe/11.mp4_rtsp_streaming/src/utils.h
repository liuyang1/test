#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/* --- byte reading from buffer (big-endian) --- */
static inline uint16_t rd16(const uint8_t *p) { return (p[0]<<8)|p[1]; }
static inline uint32_t rd24(const uint8_t *p) { return (p[0]<<16)|(p[1]<<8)|p[2]; }
static inline uint32_t rd32(const uint8_t *p) { return (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3]; }
static inline uint64_t rd64(const uint8_t *p) {
    return ((uint64_t)rd32(p)<<32) | rd32(p+4);
}

/* --- byte writing to buffer (big-endian) --- */
static inline void wr16(uint8_t *p, uint16_t v) { p[0]=v>>8; p[1]=v; }
static inline void wr32(uint8_t *p, uint32_t v) { p[0]=v>>24; p[1]=v>>16; p[2]=v>>8; p[3]=v; }

/* --- logging --- */
#define LOG(fmt, ...) fprintf(stderr, "[LOG] " fmt "\n", ##__VA_ARGS__)
#define ERR(fmt, ...) fprintf(stderr, "[ERR] " fmt "\n", ##__VA_ARGS__)

#endif
