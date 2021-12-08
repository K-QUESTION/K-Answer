#ifndef _TYPE_H_
#define _TYPE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define Word_Bit_Len	64
#define Flag			4
#define ZERORIZE		1
#define Max_Buf_Size	512

#if Word_Bit_Len == 64
#define Word_Bit_Mask 0xFFFFFFFFFFFFFFFF
#define half_Word_Bit_Mask 0xFFFFFFFF
#define DIV_2_w_1 0x8000000000000000

typedef unsigned long long	word;
#elif Word_Bit_Len == 32
#define Word_Bit_Mask 0xFFFFFFFF
#define half_Word_Bit_Mask 0xFFFF
#define DIV_2_w_1 0x80000000
typedef unsigned int		word;
#else
#define Word_Bit_Mask 0xFF
#define half_Word_Bit_Mask 0xF
#define DIV_2_w_1 0x80
typedef unsigned char		word;
#endif

#define byte unsigned char
#define NON_NEGATIVE 0
#define NEGATIVE 1
#define TRUE	1
#define FALSE	0

#define BIGGER_FIRST_ARGUMENT		1
#define EQUAL_ARGUMENT				0
#define BIGGER_SECOND_ARGUMENT		-1

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif


typedef struct {
	int sign;
	int wordlen;
	word* a;
} bigint;


#endif // !_TYPE_H_