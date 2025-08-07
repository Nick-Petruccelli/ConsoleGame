#pragma once

#include <stdint.h>

#define internal static
#define global static
#define local_percist static

#define uint8max 0xFF
#define uint16max 0xFFFF
#define uint32max 0xFFFFFFFF
#define uint64max 0xFFFFFFFFFFFFFFFF

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uintptr_t uintptr;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;
typedef int64 bool64;

typedef float real32;
typedef double real64;

typedef unsigned char byte;

