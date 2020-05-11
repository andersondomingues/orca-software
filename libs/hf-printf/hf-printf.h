/*
printf from hellfireos
https://github.com/sjohann81/hellfireos/tree/master/lib/libc
*/
#ifndef __HF_PRINTF__
#define __HF_PRINTF__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>

#define DEBUG_ADDR	0xf00000d0

#define isdigit(c)		('0' <= (c) && (c) <= '9')
#define NULL			((void *)0)

void putchar(int32_t value)
{	volatile uint32_t* DEBUG = (volatile uint32_t*) DEBUG_ADDR;
	*DEBUG = value;
}

void *memcpy(void *dst, const void *src, uint32_t n){
	int8_t *r1 = dst;
	const int8_t *r2 = src;

	while (n--)
		*r1++ = *r2++;

	return dst;
}

static uint32_t divide(long *n, int base)
{
	uint32_t res;

	res = ((uint32_t)*n) % base;
	*n = (long)(((uint32_t)*n) / base);
	return res;
}

static int toint(const char **s)
{
	int i = 0;
	while (isdigit((int)**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}

static void printchar(int8_t **str, int32_t c){
	if (str){
		**str = c;
		++(*str);
	}else{
		if (c) putchar(c);
	}
}

static int vsprintf(char **buf, const char *fmt, va_list args)
{
	char **p, *str;
	const char *digits = "0123456789abcdef";
	char pad, tmp[16];
	int width, base, sign, i;
	long num;

	for (p = buf; *fmt; fmt++) {
		if (*fmt != '%') {
			printchar(p, *fmt);
			continue;
		}
		// get flags 
		++fmt;
		pad = ' ';
		if (*fmt == '0') {
			pad = '0';
			fmt++;
		}
		// get width 
		width = -1;
		if (isdigit(*fmt)) {
			width = toint(&fmt);
		}
		// ignore long 
		if (*fmt == 'l')
			fmt++;
		base = 10;
		sign = 0;
		switch (*fmt) {
		case 'c':
			printchar(p, (char)va_arg(args, int));
			continue;
		case 's':
			str = va_arg(args, char *);
			if (str == NULL)
				str = "<NULL>";
			for (; *str && width != 0; str++, width--) {
				printchar(p, *str);
			}
			while (width-- > 0)
				printchar(p, pad);
			continue;
		case 'X':
		case 'x':
			base = 16;
			break;
		case 'd':
			sign = 1;
			break;
		case 'u':
			break;
		default:
			continue;
		}
		num = va_arg(args, long);
		if (sign && num < 0) {
			num = -num;
			printchar(p, '-');
			width--;
		}
		i = 0;
		if (num == 0)
			tmp[i++] = '0';
		else
			while (num != 0)
				tmp[i++] = digits[divide(&num, base)];
		width -= i;
		while (width-- > 0)
			printchar(p, pad);
		while (i-- > 0)
			printchar(p, tmp[i]);
	}
	printchar(p, '\0');
	
	return 0;
}

int32_t printf(const int8_t *fmt, ...){
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = vsprintf(0, fmt, args);
	va_end(args);
	return v;
}

int32_t sprintf(int8_t *out, const int8_t *fmt, ...){
	va_list args;
	int32_t v;

	va_start(args, fmt);
	v = vsprintf(&out, fmt, args);
	va_end(args);
	return v;
}

union float_long{
	float f;
	int32_t l;
	uint32_t u;
};

int8_t *itoa(int32_t i, int8_t *s, int32_t base){
	int8_t *ptr = s, *ptr1 = s, tmp_char;
	int32_t tmp_value;

	if (base < 2 || base > 36) {
		*s = '\0';
		return s;
	}
	do {
		tmp_value = i;
		i /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - i * base)];
	} while (i);
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return s;
}


int32_t ftoa(float f, int8_t *outbuf, int32_t precision){
	int32_t mantissa, int_part, frac_part, exp2, i;
	int8_t *p;
	union float_long fl;

	p = outbuf;

	if (f < 0.0){
		*p = '-';
		f = -f;
		p++;
	}

	fl.f = f;

	exp2 = (fl.l >> 23) - 127;
	mantissa = (fl.l & 0xffffff) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31){
		return -1;	/* too large */
	}else{
		if (exp2 < -23){
//			return -1;	/* too small */
		}else{
			if (exp2 >= 23){
				int_part = mantissa << (exp2 - 23);
			}else{
				if (exp2 >= 0){
					int_part = mantissa >> (23 - exp2);
					frac_part = (mantissa << (exp2 + 1)) & 0xffffff;
				}else{
					frac_part = (mantissa & 0xffffff) >> (-(exp2 + 1));
				}
			}
		}
	}

	if (int_part == 0){
		*p = '0';
		p++;
	}else{
		itoa(int_part, p, 10);
		while(*p) p++;
	}
	*p = '.';
	p++;

	for (i = 0; i < precision; i++){
		frac_part = (frac_part << 3) + (frac_part << 1);
		*p = (frac_part >> 24) + '0';
		p++;
		frac_part = frac_part & 0xffffff;
	}

	*p = 0;

	return 0;
}


#ifdef __cplusplus
}
#endif

#endif //__HF_PRINTF__
