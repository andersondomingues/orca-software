/*
testing the most common libm functions
*/
#include <stdint.h>
#include <stdio.h>
#include <math.h> 

// defined in libc_nano
//extern int __errno;

int32_t ftoa(float f, int8_t *outbuf, int32_t precision);
int8_t *itoa(int32_t i, int8_t *s, int32_t base);


int main() {
  int i, size;
  float af = 1.2f;
  float bf = 2.1f,cf,df;
  int ai = 1;
  int bi = 2;
  double ad = 1.2, bd = 2.1, cd;
  
  double num = 123412341234.123456789; 
  char str1[15],str2[15],str3[15];

  // testing math
  bf = floorf(af);
  bi = (int)bf;
  cf = af+bf;

  ftoa(af,str1, 4);   
  ftoa(bf,str2, 4);   
  ftoa(cf,str3, 4);   
  //printf("float: %f + %f = %f\n",a,b,a+b);
  printf("float str: %s + %s = %s\n",str1,str2,str3);
  printf("float hex: 0x%x + 0x%x = 0x%x\n",af,bf,cf);
  printf("float to int cast: %d\n",bi);

  ftoa(num,str1, 9); // nao funcionou
  printf("TESTING double: %s !!!!\n", str1);

  af=16.0f;
  bf=sqrtf(af);
  bi = (int)bf;
  ftoa(bf,str1, 9);
  printf("TESTING sqrt(16): %s  - (int) %d!!!!\n", str1, bi);

  af = powf(4,2);
  ftoa(af,str1, 4); 
  printf("TESTING pow: %d ^ %d = %s !!!!\n", 4,2, str1);



  printf("\n\nrepeating the test with double ...\n\n");

  // testing math
  bd = floor(ad);
  bi = (int)bd;
  cd = ad+bd;

  ftoa(ad,str1, 4);   
  ftoa(bd,str2, 4);   
  ftoa(cd,str3, 4);   
  //printf("float: %f + %f = %f\n",a,b,a+b);
  printf("float str: %s + %s = %s\n",str1,str2,str3);
  printf("float hex: 0x%x + 0x%x = 0x%x\n",ad,bd,cd);
  printf("float to int cast: %d\n",bi);

  ftoa(num,str1, 9); // nao funcionou
  printf("TESTING double: %s !!!!\n", str1);

  ad=16.0;
  bd=sqrt(ad);
  bi = (int)bd;
  ftoa(bd,str1, 9);
  printf("TESTING sqrt(16): %s  - (int) %d!!!!\n", str1, bi);

  ad = pow(4,2);
  ftoa(ad,str1, 4); 
  printf("TESTING pow: %d ^ %d = %s !!!!\n", 4,2, str1);



  return 0;
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


