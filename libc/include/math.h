#ifndef MUNIXSDK_LIBC_MATH_H
#define MUNIXSDK_LIBC_MATH_H

#include <stdint.h>
#include <stddef.h>

#define N_SQRT_ITERATIONS 5
#define SQRT_MAGIC_NUMBER 0x1fbb4f2f
#define FLOAT_EXP_BIAS 127

#define INT_MAX 2147483647
#define INT_MIN -2147483648


typedef struct {
	uint32_t mantissa : 23;
	uint32_t exp : 8
	uint32_t sign : 1;
} float_map_t;

typedef union {
	float num;
	uint32_t raw;
	float_map_t map;
} cast_float_t;

float sqrt(float num);

float __addsf3(float a, float b);
float __subsf3(float a, float b);
float __mulsf3(float a, float b);
float __divsf3(float a, float b);

float __floatsisf(int i);
float __floatunsisf(unsigned int i);
int __fixsfsi(float f);

int __eqsf2(float a, float b);
int __ltsf2(float a, float b);
int __gtsf2(float a, float b);


#endif
