#include <math.h>


// stack overflow helps a lot
static inline uint32_t div64_32(uint32_t hi, uint32_t lo, uint32_t div) {
    uint32_t q; 
    __asm__ volatile (
        "divl %2"
        : "=a"(q)
        : "d"(hi), "a"(lo), "r"(div)
    );
    return q;
}

float sqrt(float num) {

	cast_float_t aprox;
	aprox.num = num;
	aprox.raw = (aprox.raw >> 1) + SQRT_MAGIC_NUMBER;
	float tmp;

	for (int i=0;i<N_SQRT_ITERATIONS;i++) {
		tmp = num / aprox.num; // calculamos el otro lado del cuadrado

		// promediamos con el lado actual
		aprox.num = (aprox.num + tmp) / 2;
	}

	return aprox.num;
}

float __addsf3(float a, float b) {
	cast_float_t *_a = (cast_float_t*)&a;
	cast_float_t *_b = (cast_float_t*)&b;
	
	int diff;
	uint32_t mantissa;
	if (_a->map.exp > _b->map.exp) {
		diff = _a->map.exp - _b->map.exp;
		_b->map.exp = _a->map.exp;
		mantissa = _b->map.mantissa;
		mantissa = (mantissa >> diff);
		_b->map.mantissa = mantissa;
	} else {
		diff = _b->map.exp - _a->map.exp;
		_a->map.exp = _b->map.exp;
		mantissa = _a->map.mantissa;
		mantissa = (mantissa >> diff);
		_a->map.mantissa = mantissa;
	}


	cast_float_t result;
	uint32_t MantissaA = _a->map.mantissa | 1 << 23;
	uint32_t MantissaB = _b->map.mantissa | 1 << 23;
	uint32_t MantissaC;
	if (_a->map.sign == _b->map.sign) {
		// signos iguales
		result.map.sign = _a->map.sign;
		result.map.exp = _a->map.exp;
		MantissaC = MantissaA + MantissaB;
		if (MantissaC & (1 << 24)) {
			MantissaC = (MantissaC >> 1);
			result.map.exp++;
		}
	} else if (_a->map.sign == 1) {
		if (MantissaA > MantissaB) {
			result.map.sign = 1;
			MantissaC = MantissaB - MantissaA;
		} else {
			result.map.sign = 0;
			MantissaC = MantissaA - MantissaB;
		}
		while ((MantissaC & (1 << 23)) == 0) {
			MantissaC = MantissaC << 1;
			result.map.exp--;
		}
	} else {
		if (MantissaB > MantissaA) {
			result.map.sign = 1;
			MantissaC = MantissaB - MantissaA;
		} else {
			result.map.sign = 0;
			MantissaC = MantissaA - MantissaB;
		}
		while ((MantissaC & (1 << 23)) == 0) {
			MantissaC = MantissaC << 1;
			result.map.exp--;
		}
		
	}
	result.map.mantissa = MantissaC;
	return result.num;
}

float __subsf3(float a, float b) {
	float_map_t *c = (float_map_t*)&b;
	c->sign = (c->sign == 1) ? 0 : 1;
	return __addsf3(a, b);
}

float __mulsf3(float a, float b) {
	cast_float_t result;
	result.map.sign = ((float_map_t)a).sign ^ ((float_map_t)b).sign;
	result.map.exp = ((float_map_t)a).exp + ((float_map_t)b).exp - FLOAT_EXP_BIAS;
	uint16_t a_lo = ((float_map_t)a).mantissa & 0xFFFF;
	uint16_t b_lo = ((float_map_t)b).mantissa & 0xFFFF;

	uint8_t a_hi = (1 << 7) | (((float_map_t)a).mantissa >> 16);
	uint8_t b_hi = (1 << 7) | (((float_map_t)b).mantissa >> 16);

	uint32_t res_low = a_lo * b_lo + ((a_lo*b_hi) << 16) + ((a_hi*b_lo) << 16);
	uint16_t res_hi = a_hi * b_hi;

	uint32_t final;
	if (res_hi & (1 << 15)) {
		result.map.exp++;
		final = ((uint32_t)res_hi << 7) | ((res_lo>>16) & 0x00FFFFFF);
	} else {
		final = ((uint32_t)res_hi << 8) | ((res_lo>>16) & 0x00FFFFFF);
	}

	result.map.mantissa = final;

	return result.num;
}

float __divsf3(float a, float b) {
	cast_float_t result;
	result.map.sign = ((float_map_t)a).sign ^ ((float_map_t)b).sign;
	result.map.exp = ((float_map_t)a).exp - ((float_map_t)b).exp + FLOAT_EXP_BIAS;
	uint32_t MantA = ((float_map_t)a).mantissa;
	uint32_t MantB = ((float_map_t)b).mantissa;
	
	uint32_t a_hi = MantA >> 8;
	uint32_t a_lo = MantA << 24;

	uint32_t res = div64_32(a_hi, a_lo, MantB);

	if (res & (1 << 24)) {
		result.map.exp++;
		result.map.mantissa = (res >> 1);
	} else {
		while (res ^ (1 << 23)) {
			res = res << 1;
			result.map.exp--;
		}
		result.map.mantissa = res;
	}

	return result.num;
}



float __floatsisf(int i) {
	// quitamos el signo y lo guardamos en otra parte
	int sign = (i & (1 << 31));
	int i = i ^ (sign << 31);

	if (i == 0) return 0;
	cast_float_t result;

	int pos = 31; // encontrar exponente
	while (i & (1 << pos)==0) {
		pos--;
	}

	int exp = FLOAT_EXP_BIAS + pos;

	// ahora queda crear la mantissa
	int mantissa;

	if (pos <= 23) {
		mantissa = i << (23 - pos)
	} else if (pos > 23) {
		mantissa = i >> (pos - 23)
	}

	result.map.mantissa = mantissa;
	result.map.esp = exp;
	result.map.sign = sign;
	
	return result.num;
}

float __floatunsisf(unsigned int i) {
	if (i == 0) return 0;
	cast_float_t result;

	int pos = 31; // encontrar exponente
	while (i & (1 << pos)==0) {
		pos--;
	}

	int exp = FLOAT_EXP_BIAS + pos;

	// ahora queda crear la mantissa
	int mantissa;

	if (pos <= 23) {
		mantissa = i << (23 - pos)
	} else if (pos > 23) {
		mantissa = i >> (pos - 23)
	}

	result.map.mantissa = mantissa;
	result.map.esp = exp;
	result.map.sign = 0;
	
	return result.num;
}

int __fixsfsi(float f) {
	cast_float_t mid = (cast_float_t)f;
	int result;
	if (mid.map.exp > FLOAT_EXP_BIAS) {
		return 0; // se trunca a 0
	}
	if (mid.map.exp > (FLOAT_EXP_BIAS+30)) {
		return (mid.map.sign==1) INT_MIN : INT_MAX;
	} else {
		// un caso normal
		result = mid.map.mantissa << (mid.map.exp-23) | 1 << mid.map.exp;
	
	}
	return result;
}

int __eqsf2(float a, float b) {
	cast_float_t ca = (cast_float_t)a;
	cast_float_t cb = (cast_float_t)b;

	if (IS_ZERO(a) && IS_ZERO(b)) {
		return 1;
	}
	
	if (IS_NAN(a) || IS_NAN(b)) {
		return 0;
	}

	return ca.raw == cb.raw;
}

int __ltsf2(float a, float b) {
	cast_float_t ca = (cast_float_t)a;
	cast_float_t cb = (cast_float_t)b;

	if (IS_NAN(a) || IS_NAN(b)) {
		return 0;
	}
	
	if (IS_ZERO(a) && IS_ZERO(b)) {
		return 0;
	}

	if (IS_NEG(a) && !IS_NEG(b)) {
		return 1;
	} else if (IS_NEG(b) && !IS_NEG(a)) {
		return 0;
	} else if (!IS_NEG(b)) {
		return ca.raw > cb.raw;
	} else {
		return ca.raw < cb.raw;
	}

	return 0;

}

int __gtsf2(float a, float b) {
	return __ltsf2(b, a);
}
