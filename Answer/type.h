#ifndef _TYPE_H_
#define _TYPE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<windows.h>

#define Word_Bit_Len 32


#if Word_Bit_Len == 64
typedef unsigned long long	word;
#elif Word_Bit_Len == 32
typedef unsigned int		word;
#define TMP_type            uint64_t
#define MAX_VAL             ((TMP_type)(0xFFFFFFFF))
#else
typedef unsigned char		word;
#define TMP_type            uint32_t
#define MAX_VAL             ((TMP_type)(0xFF))
#endif

#define byte unsigned char
#define NON_NEGATIVE 0
#define NEGATIVE 1

#define BIGGER_FIRST_ARGUMENT		1
#define EQUAL_ARGUMENT				0
#define BIGGER_SECOND_ARGUMENT		-1

#define TRUE	1
#define FALSE	2
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

typedef struct {
	int sign;
	int wordlen;
	word* a;
} bigint;


#endif // !_TYPE_H_