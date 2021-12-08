/**
 * @file operation.c
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (kth0305a@kookmin.ac.kr)
 * @brief File that performs the Big Integer computational function.
 * @version 1.0.0
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "operation.h"

 /*********** GENERAL ***********/

 /**
  * @section GENERAL
  * @brief General computational function.
  *
  */

  /**
   * @fn int CompareABS(bigint* A, bigint* B)
   * 
   * @brief An operation that compares two bigint structures A and B with the same sign.
   *
   * @param A Bigint Structure 1 to compare
   * @param B Bigint Structure 2 to compare
   * @return int Result: A > B is 1, A = B is 0, A < B is -1.
   *
   */

int CompareABS(bigint* A, bigint* B)
{
	int n = A->wordlen;
	int m = B->wordlen;

	if (n > m)
		return BIGGER_FIRST_ARGUMENT;
	else if (n < m)
		return BIGGER_SECOND_ARGUMENT;
	for (int i = n - 1; i >= 0; i--)
	{
		if (A->a[i] == B->a[i])
			continue;
		else if (A->a[i] > B->a[i])
			return BIGGER_FIRST_ARGUMENT;
		else
			return BIGGER_SECOND_ARGUMENT;
	}
	return EQUAL_ARGUMENT;
}
/**
 *
 * @fn int Compare(bigint* A, bigint* B)
 * @brief An operation that compares two bigint structures A and B
 *
 * @param A Bigint Structure 1 to compare
 * @param B Bigint Structure 2 to compare
 * @return int Result: A > B is 1, A = B is 0, A < B is -1.
 *
 */
int Compare(bigint* A, bigint* B)
{
	if ((A->sign == NON_NEGATIVE) && (B->sign == NEGATIVE))
		return BIGGER_FIRST_ARGUMENT;
	if ((A->sign == NEGATIVE) && (B->sign == NON_NEGATIVE))
		return BIGGER_SECOND_ARGUMENT;

	int ret = CompareABS(A, B);
	if (A->sign == NON_NEGATIVE)
		return ret;
	else
		return ret * (-1);
}

word MASK_Set(int j)
{
	int hex_bytes = j / 4;
	word MASK = 0x0;
	int i = 0;
	for (i = 0; i < hex_bytes; i++)
	{
		MASK += ((word)(0xf) << (i * 4));
	}
	return MASK;
}

/**
 * @fn void bi_L_shift(bigint** y, bigint* x, int nbits)
 *
 * @brief A function of shigting structure x to the left by n bits and storing it in structure y.
 *
 * @param y A structure to be moved [Structure double pointer variable]
 * @param x A structure to store the moved value [Structure pointer variable]
 * @param nbits Number of spaces to move [integer type variable]
 */
void bi_L_shift(bigint** y, bigint* x, int nbits)   // bit shift
{
	int nword = nbits / Word_Bit_Len;  // word
	int rbits = nbits % Word_Bit_Len;

	if (nword != 0)
	{
		bi_resize(y, x->wordlen, x->wordlen + nword);
		int i;
		for (i = (*y)->wordlen - 1; i >= nword; --i)
			(*y)->a[i] = x->a[i - nword];
		for (; i >= 0; --i)
			(*y)->a[i] = 0;

	}

	if (rbits != 0)
	{
		bi_resize(y, x->wordlen, x->wordlen + nword + 1); 
		int i;
		(*y)->a[(*y)->wordlen - 1] = x->a[x->wordlen - 1] >> (Word_Bit_Len - rbits);

		for (i = ((*y)->wordlen - 2); i > nword; --i)
		{
			(*y)->a[i] = (x->a[i - nword] << rbits) | (x->a[i - nword - 1] >> (Word_Bit_Len - rbits));
		}
		(*y)->a[i] = x->a[0] << rbits;
		i--;
		for (; i >= 0; --i)
			(*y)->a[i] = 0;

	}
	if (nbits == 0)
		bi_assign(y, x); // 0 bit shift 입력이 들어온 경우
	bi_refine(*y);
	return;
}

/**
 * @brief A function of shigting structure x to the right by n bits and storing it in structure y.
 *
 * @param y A structure to be moved [Structure double pointer variable]
 * @param x A structure to store the moved value [Structure pointer variable]
 * @param nbits Number of spaces to move [integer type variable]
 */
void bi_R_shift(bigint** y, bigint* x, int nbits)
{
	int nword = nbits / Word_Bit_Len;  // word
	int remain = nbits % Word_Bit_Len;

	if (nword > x->wordlen)
	{
		/*
		bi_resize(y, x->wordlen, nword);
		for (int i = 0; i < nword; ++i)
		{
			(*y)->a[i] = 0;
		}*/
		for (int i = 0; i < x->wordlen; ++i)
		{
			(*y)->a[i] = 0;
		}
		return;
	}
	if (nword != 0)
	{
		int i;
		bi_resize(y, x->wordlen, x->wordlen - nword);
		for (i = 0; i < x->wordlen - nword; ++i)
			(*y)->a[i] = x->a[i + nword];


		//refine     
	}
	if (remain != 0)
	{
		int i;
		bi_resize(y, x->wordlen, x->wordlen - nword);
		
		for (i = 0; i < ((*y)->wordlen - 1); ++i)
		{
			(*y)->a[i] = (x->a[i] >> remain) | (x->a[i + 1] << (Word_Bit_Len - remain));
		}
		(*y)->a[i] = x->a[i] >> remain;

	}
	if (nbits == 0)
		bi_assign(y, x); // 0 bit shift 입력이 들어온 경우
	bi_refine(*y);
	return;
}

/**
 * @brief A function that reduces the size of the structure array.
 *
 * @param dst A structure to store the reduced array [Structure double pointer variable]
 * @param src A structure to reduce the size [Structure pointer variable]
 * @param nbits size to reduce [integer type variable]
 *
 * @details The result of taking the src value 2^nbits modular is stored in y. 
 * This is an operation of storing only the array values corresponding to nbits in y.
 */
void bi_reduct(bigint** dst, bigint* src, int nbits)
{
	int nword = nbits / Word_Bit_Len;  // word
	int k = nbits % Word_Bit_Len;
	if (nbits > src->wordlen * Word_Bit_Len)  // Exception
	{
		bi_assign(dst, src);
	}
	if (nword != 0)
	{

		for (int i = 0; i < nword; i++)
			(*dst)->a[i] = src->a[i];
		bi_resize(dst, (*dst)->wordlen, nword);
	}
	if (k != 0)
	{
		bi_copy(dst, src);
		bi_resize(dst, (*dst)->wordlen, nword + 1);

		for (int i = 0; i < nword + 1; i++)
		{
			if (i == nword)
				(*dst)->a[i] = src->a[i] & (MASK_Set(k));
			else
				(*dst)->a[i] = src->a[i];
		}
		return;
	}
}

/**
 * @brief A function to find the absolute value of a negative structure.
 *
 * @param y A structure to find the absolute value [Structure double pointer variable]
 * @param x A structure to store the absolute value [Structure pointer variable]
 *
 * @details If the sign of x is negative, convert the sign to a nonnegative integer.
 *
 */
void ABS(bigint** y, bigint* x)
{
	bi_assign(y, x);
	if (x->sign == NEGATIVE)
	{
		bi_flip_sign(y);
	}
	
}

/*********** ADDITION ************/

/**
 * @brief A function that adds single word A and B.
 *
 * @param C A structure to store the added value [Structure double pointer variable]
 * @param A Structure to be added [Structure pointer variable]
 * @param B Structure to be added [Structure pointer variable]
 *
 */
void ADD_AB(bigint** C, bigint* A, bigint* B) 
{
	if (*C != NULL)
	{
		bi_delete(C);
	}

	bi_new(C, 1);
	(*C)->a[0] = (A->a[0] + B->a[0]) & Word_Bit_Mask;  // C <- A + B mod W

	if ((*C)->a[0] < A->a[0])
	{
		bi_resize(C, 1, 2);
		(*C)->a[1] = 1;  //carry
	}
	(*C)->sign = A->sign;
}

/**
 * @brief A function that adds the idx-th word of A and B and carry.
 *
 * @param C A structure to store the added value [Structure double pointer variable]
 * @param A Structure to be added [Structure pointer variable]
 * @param B Structure to be added [Structure pointer variable]
 * @param c Carry [integer type variable]
 * @param idx The index of the word to add [integer type variable]
 *
 */
int ADD_ABc(bigint** C, bigint* A, bigint* B, int c, int idx) // int c -> {0, 1}
{

	int carry = 0;
	(*C)->a[idx] = (A->a[idx] + c) & Word_Bit_Mask; // C <- A + carry mod W

	if ((*C)->a[idx] < A->a[idx])
	{
		carry = 1; // carry <- 1
	}

	(*C)->a[idx] = ((*C)->a[idx] + B->a[idx]) & Word_Bit_Mask; // C <- C + B mod W

 	if ((*C)->a[idx] < B->a[idx])	// if C < carry then
	{
		carry += 1;  // carry <- carry + 1
	}
	return carry;
}

/**
 * @brief A function that adds A and B with the same sign.
 *
 * @param C A structure to store the added value [Structure double pointer variable]
 * @param A Structure to be added [Structure pointer variable]
 * @param B Structure to be added [Structure pointer variable]
 *
 */
void ADDC(bigint** C, bigint* A, bigint* B) // where WordLen(A)    WordLen(B) and Sign(A) = Sign(B)
{
	bigint* tmp_B = NULL;
	bi_assign(&tmp_B, B);
	bi_resize(&tmp_B, tmp_B->wordlen, A->wordlen);

	if ((*C) == NULL)
	{
		bi_new(C, A->wordlen + 1);
		bi_set_zero(C);
	}
	if ((*C)->wordlen < A->wordlen)
		bi_resize(C, (*C)->wordlen, A->wordlen + 1);

	int carry = 0; // carry <- 0
	for (int j = 0; j < A->wordlen; j++)  // for j = 0 to n-1 do
	{
		carry = ADD_ABc(C, A, tmp_B, carry, j);  // carry <- ADD_ABc(Aj, Bj, carry)
	}
	if (carry == 1)	// top word's carry = 1
	{
		bi_resize(C, A->wordlen, A->wordlen + 1);
		(*C)->a[(*C)->wordlen - 1] += 1;
	}
	if ((*C)->a[A->wordlen] == 0)  // if C[n] = 0
	{
		(*C)->sign = A->sign;
		bi_resize(C, A->wordlen + 1, A->wordlen);  // return C for j = 0 to n - 1
	}
	else  // if C[n] = 1
	{
		(*C)->sign = A->sign;  // return C for j = 0 to n
	}
	bi_delete(&tmp_B);
}

/**
 * @brief A function that adds any A and B.
 *
 * @param C A structure to store the added value [Structure double pointer variable]
 * @param A Structure to be added [Structure pointer variable]
 * @param B Structure to be added [Structure pointer variable]
 *
 */
void ADD(bigint** C, bigint* A, bigint* B)
{
	if (bi_is_zero(A))		// if A = 0 then
		bi_assign(C, B);	// return B

	else if (bi_is_zero(B))	// if B = 0 then
		bi_assign(C, A);	// return A

	else if (A->sign == NON_NEGATIVE && B->sign == NEGATIVE)  // if A > 0 and B < 0 then
	{
		bigint* abs = NULL;
		ABS(&abs, B);
		SUB(C, A, abs); // return SUB(A, |B|)
		bi_delete(&abs);
	}
	else if (A->sign == NEGATIVE && B->sign == NON_NEGATIVE) // if A < 0 and B > 0 then
	{
		bigint* abs = NULL;
		ABS(&abs, A);
		SUB(C, B, abs);  // return SUB(B, |A|)
		bi_delete(&abs);
	}
	else if (A->wordlen >= B->wordlen)  	// wordlen(A) >= wordlen(B) then
		ADDC(C, A, B);	// return ADDC(A, B)

	else
		ADDC(C, B, A);	// return ADDC(B, A)

	return;
}

/**
 * @brief A function that adds random A and C.
 *
 * @param C A structure to add and store the added value [Structure double pointer variable]
 * @param A Structure to be added [Structure pointer variable]
 */
void ADD_CA(bigint** C, bigint* A)
{
	bigint* temp = NULL;
	bi_assign(&temp, *C);
	ADD(C, temp, A);
	bi_delete(&temp);
}


/********** SUBTRACTION ***********/
/**
 * @brief A function that subtracts a single word B from a single word A.
 *
 * @param C A structure to store the subtracted value [Structure double pointer variable]
 * @param A Structure to be subtracted [structure pointer variable]
 * @param B Structure to subtract [structure pointer variable]
 *
 */
void SUB_AB(bigint** C, bigint* A, bigint* B)
{
	if ((*C) != NULL)
	{
		bi_delete(C);
	}
	bi_new(C, 1);
	
	(*C)->a[0] = (A->a[0] - B->a[0]) & Word_Bit_Mask;  // C <- A - B mod W

	bi_resize(C, 1, 2);
	if (A->a[0] >= B->a[0])
	{
		(*C)->a[1] = 0;   // borrow <- 0
		(*C)->sign = NON_NEGATIVE;  
	}
	else
	{
		(*C)->a[1] = 1;   // borrow <- 1  i.e. -(borrow)*W + C
		(*C)->sign = NEGATIVE;  // sign
	}
}

/**
 * @brief A function that subtracts the idx-th word of A and B and carry.
 *
 * @param C A structure to store the subtracted value [Structure double pointer variable]
 * @param A Structure to be subtracted [structure pointer variable]
 * @param B Structure to subtract [structure pointer variable]
 * @param b Borrow [integer type value]
 * @param idx The index of the word to subtract. [integer type variable]
 */
int SUB_AbB(bigint** C, bigint* A, bigint* B, unsigned int b, int idx) // int b -> {0, 1}, idx -> A, B
{
	unsigned int borrow = 0;  // borrow = 1
	
	(*C)->a[idx] = ((A->a[idx] - b)) & Word_Bit_Mask;  // C <- A - b mod W

	if (A->a[idx] < b)
	{
		borrow = 1;  // borrow <- 1
	}

	if ((*C)->a[idx] < B->a[idx])  // if C < B then
	{
		borrow += 1;  // borrow <- borrow + 1
	}
	(*C)->a[idx] = (((*C)->a[idx] - B->a[idx])) & Word_Bit_Mask;    // C <- C - B mod W
	return borrow;
}

/**
 * @brief A function that subtracts B from A with the same sign.
 *
 * @param C A structure to store the subtracted value [Structure double pointer variable]
 * @param A Structure to be subtracted [structure pointer variable]
 * @param B Structure to subtract [structure pointer variable]
 */
void SUBC(bigint** C, bigint* A, bigint* B) // where WordLen(A)    WordLen(B) and Sign(A) = Sign(B)
{
	bigint* tmp_B = NULL;
	bi_assign(&tmp_B, B);
	bi_resize(&tmp_B, tmp_B->wordlen, A->wordlen);

	if ((*C) == NULL)
	{
		bi_new(C, A->wordlen);
		bi_set_zero(C);
	}
	if ((*C)->wordlen < A->wordlen)
		bi_resize(C, (*C)->wordlen, A->wordlen);
	unsigned int borrow = 0;
	for (int j = 0; j < A->wordlen; j++)
	{
		borrow = SUB_AbB(C, A, tmp_B, borrow, j);
	}
	bi_delete(&tmp_B);
	bi_refine(*C);
}

/**
 * @brief A function that subtracts any B from any A.
 *
 * @param C A structure to store the subtracted value [Structure double pointer variable]
 * @param A Structure to be subtracted [structure pointer variable]
 * @param B Structure to subtract [structure pointer variable]
 */
void SUB(bigint** C, bigint* A, bigint* B)
{
	bigint* tmp_A = NULL;
	bigint* tmp_B = NULL;
	bi_assign(&tmp_A, A);
	bi_refine(tmp_A);
	bi_assign(&tmp_B, B);
	bi_refine(tmp_B);

	if (bi_is_zero(tmp_A))  // if A = 0 then
	{
		bi_copy(C, tmp_B);
		bi_flip_sign(C); // return -B
	}
	else if (bi_is_zero(tmp_B))	// if B = 0 then
		bi_copy(C, tmp_A);	// return A

	else if (Compare(tmp_A, tmp_B) == EQUAL_ARGUMENT)	// if A = B then
		bi_set_zero(C);	// return 0

	else if (tmp_A->sign == NON_NEGATIVE && tmp_B->sign == NON_NEGATIVE)  // if 0 < B <= A then
	{
		if (CompareABS(tmp_A, tmp_B) == BIGGER_FIRST_ARGUMENT)
		{
			SUBC(C, tmp_A, tmp_B);
			(*C)->sign = NON_NEGATIVE;  // return SUBC(A, B)
		}
		else  // if 0 < A < B then
		{
			SUBC(C, tmp_B, tmp_A);
			(*C)->sign = NEGATIVE;  // return -SUBC(B, A)
		}
	}
	else if (tmp_A->sign == NEGATIVE && tmp_B->sign == NEGATIVE)  	// if 0 > A >= B then
	{
		if (CompareABS(tmp_A, tmp_B) == BIGGER_SECOND_ARGUMENT)  // return SUBC(|B|, |A|)
		{
			SUBC(C, tmp_B, tmp_A);
			(*C)->sign = NON_NEGATIVE;  // return SUBC(|B|, |A|)
		}
		else if (Compare(tmp_B, tmp_A))  // if 0 > B > A then
		{
			SUBC(C, tmp_A, tmp_B);
			(*C)->sign = NEGATIVE;  // return -SUBC(|A|, |B|)
		}
	}
	else if (tmp_A->sign == NON_NEGATIVE && tmp_B->sign == NEGATIVE)  //if A > 0 and B < 0 then
	{
		bigint* absB = NULL;
		ABS(&absB, tmp_B);
		if (tmp_A->wordlen >= tmp_B->wordlen)
			ADD(C, tmp_A, absB);
		else
			ADD(C, absB, tmp_A);
		(*C)->sign = NON_NEGATIVE;  // return ADD(A, |B|)
		bi_delete(&absB);
	}
	else
	{
		bigint* absA = NULL;
		ABS(&absA, tmp_A);
		if (tmp_A->wordlen >= tmp_B->wordlen)
			ADD(C, tmp_B, absA);
		else
			ADD(C, absA, tmp_B);
		(*C)->sign = NEGATIVE;  // return -ADD(|A|, B)
		bi_delete(&absA);

	}
	bi_delete(&tmp_A);
	bi_delete(&tmp_B);
	bi_refine(*C);
	return;
}

/**
 * @brief A function that subtracts random A and C.
 *
 * @param C A structure to subtract and store the subtracted value [Structure double pointer variable]
 * @param A Structure to be subtracted [Structure pointer variable]
 */
void SUB_CA(bigint** C, bigint* A)       //C-A를 C에 입력하도록 하는 함수.
{
	bigint* temp = NULL;
	bi_assign(&temp, *C);
	SUB(C, temp, A);
	bi_delete(&temp);
}


/********** MULTIPLICATION ***********/
/**
 * @brief A function that multiplies the idxA-th word of A and the idxB-th word of B.
 *
 * @param C A structure to store multiplied values [Structure double pointer variable]
 * @param A A structure to multiply [structure pointer variable]
 * @param B A structure to multiply [structure pointer variable]
 * @param idxA The index of array of A to multiply [integer type variable]
 * @param idxB The index of array of A to multiply [integer type variable]
 */
void MUL_AB(bigint** C, bigint* A, bigint* B, int idxA, int idxB)  // A, B: 1 word 
{
	word A1 = 0, A0 = 0;
	word B1 = 0, B0 = 0;
	word T1 = 0, T0 = 0, T = 0;
	word C1 = 0, C0 = 0;

	A1 = A->a[idxA] >> (Word_Bit_Len / 2);   // A1 <- A >> W/2
	A0 = A->a[idxA] & half_Word_Bit_Mask;	 // A0 <- A mod 2^(W/2)
	B1 = B->a[idxB] >> (Word_Bit_Len / 2);	 // B1 <- B >> W/2
	B0 = B->a[idxB] & half_Word_Bit_Mask;	 // B1 <- B mod 2^(W/2)

	T1 = A1 * B0;   // T1 <- A1*B0
	T0 = A0 * B1;	// T0 <- A0*B1
	T0 = (word)(T1 + T0) & Word_Bit_Mask;	// T0 <- T1 + T0 mod W
	T1 = T0 < T1;	// T1 <- T0 < T1
	C0 = A0 * B0;	// C1 <- A1*B1
	C1 = A1 * B1;	// C0 <- A0*B0
	T = C0;			// T <- C0

	if ((*C) == NULL)
	{
		bi_new(C, 1);
	}

	bi_set_zero(C);

	(*C)->a[idxA + idxB] = (C0 + (word)(T0 << (Word_Bit_Len / 2))) & Word_Bit_Mask; // C0 <- C0 + (T0 << W/2) mod W
	C0 = (*C)->a[idxA + idxB];
	(*C)->a[idxA + idxB + 1] = C1 + (word)(T1 << (Word_Bit_Len / 2)) + (word)(T0 >> (Word_Bit_Len / 2)) + (C0 < T);   // C1 <- C1 + (T1 << W/2) + (T0 >> W/2) + (C0 < T)
}

/**
 * @brief A function that multiplies positive integers A and B.
 *
 * @param C A structure to store multiplied values [Structure double pointer variable]
 * @param A A structure to multiply [structure pointer variable]
 * @param B A structure to multiply [structure pointer variable]
 */
void MULC(bigint** C, bigint* A, bigint* B)  //Schoolbook
{
	bigint* T = NULL;
	bi_new(&T, A->wordlen + B->wordlen);

	if ((*C) != NULL)
	{
		bi_delete(C);
	}
	bi_new(C, A->wordlen + B->wordlen);

	for (int i = 0; i < A->wordlen; i++)  // for i = 0 to n-1 do
	{
		for (int j = 0; j < B->wordlen; j++)  // for j = 0 to m-1 do
		{
			MUL_AB(&T, A, B, i, j);   	// T[i+j] <- Ai*Bj
			ADD_CA(C, T);				// C <- ADDC(C, T)
		} 
	}

	bi_delete(&T);
}

/**
 * @brief A function that multiplies positive integers A and B.
 *
 * @param C A structure to store multiplied values [Structure double pointer variable]
 * @param A A structure to multiply [structure pointer variable]
 * @param B A structure to multiply [structure pointer variable]
 */
void MUL(bigint** C, bigint* A, bigint* B)
{
	bigint* absA = NULL;
	bigint* absB = NULL;
	ABS(&absA, A);
	ABS(&absB, B);
	if (bi_is_zero(A) || bi_is_zero(B)) // if A = 0 or B = 0 then
	{
		bi_set_zero(C);  	// return 0
	}
	else if (bi_is_one(A) && A->sign == NON_NEGATIVE) // if A = 0 or B = 0 then
	{
		bi_new(C, B->wordlen);
		array_copy((*C)->a, B->a, B->wordlen);
		(*C)->sign = B->sign;
		(*C)->wordlen = B->wordlen;  // return B
	}
	else if (A->sign == NEGATIVE && bi_is_one(absA))  // if A = 1 then
	{
		bi_new(C, B->wordlen);
		array_copy((*C)->a, B->a, B->wordlen);
		(*C)->sign = A->sign ^ B->sign;
		(*C)->wordlen = B->wordlen;  // return -B
	}
	else if (bi_is_one(B) && B->sign == NON_NEGATIVE)  // if A = -1 then
	{
		bi_new(C, A->wordlen);
		array_copy((*C)->a, A->a, A->wordlen);
		(*C)->sign = A->sign;
		(*C)->wordlen = A->wordlen;  // return A
	}
	else if (B->sign == NEGATIVE && bi_is_one(absB))
	{
		bi_new(C, A->wordlen);
		array_copy((*C)->a, A->a, A->wordlen);
		(*C)->sign = A->sign ^ B->sign;
		(*C)->wordlen = A->wordlen;  // return -A
	}
	else
	{
		int sign = A->sign ^ B->sign;
		MULC(C, A, B);
		(*C)->sign = sign;
	}
	bi_delete(&absA);
	bi_delete(&absB);
	bi_refine(*C);
}

/**
 * @brief A function that performs quick multiplication by adjusting terms according to the rules for positive integers A and B.
 *
 * @param C A structure to store multiplied values [Structure double pointer variable]
 * @param A A structure to multiply [structure pointer variable]
 * @param B A structure to multiply [structure pointer variable]
 */
void MULC_K(bigint** C, bigint* A, bigint* B) // Fast Multiplication  //return R-> C
{
	int l = 0;

	if (Flag >= min(A->wordlen, B->wordlen))  // flag >= min(wordlen(A), wordlen(B)) then
	{
		MUL(C, A, B);
		return;
	}
		
	l = (max(A->wordlen, B->wordlen) + 1) >> 1;	// l <- (max(wordlen(A), wordlen(B)) + 1) >> 1
	{
		bigint* A1 = NULL;
		bigint* A0 = NULL;
		bigint* B1 = NULL;
		bigint* B0 = NULL;
		bigint* T1 = NULL;
		bigint* T0 = NULL;
		bigint* S1 = NULL;
		bigint* S0 = NULL;
		bigint* S = NULL;
		bigint* R = NULL;
		bigint* tmp_S = NULL;

		bi_new(&A1, l); bi_new(&A0, l); bi_new(&B1, l); bi_new(&B0, l); bi_new(&T1, 2 * l); bi_new(&T0, 2 * l); bi_new(&S1, l); bi_new(&S0, l); bi_new(&S, 2 * l); bi_new(&R, 4 * l);

		bi_R_shift(&A1, A, l * Word_Bit_Len);  // A1 <- A >> lw
		bi_reduct(&A0, A, l * Word_Bit_Len);   // A0 <- A mod 2^(lw)
		bi_R_shift(&B1, B, l * Word_Bit_Len);  // B1 <- B >> lw
		bi_reduct(&B0, B, l * Word_Bit_Len);   // B0 <- B mod 2^(lw)
		MULC_K(&T1, A1, B1);				   // T1 <- MULC_K(A1, B1)
		MULC_K(&T0, A0, B0);				   // T0 <- MULC_K(A0, B0)

		array_init(R->a, 4 * l);
		for (int i = 0; i < 2 * l; i++)
		{
			if (i < T0->wordlen)
				R->a[i] = T0->a[i];
			else
				R->a[i] = 0;
		} // R <- T0
		for (int i = 0; i < 2 * l; i++)
		{
			if (i < T1->wordlen)
				R->a[i + 2 * l] = T1->a[i];
			else
				R->a[i + 2 * l] = 0;
		}  // R <- (T1 << 2lw) + R
		SUB(&S1, A0, A1);	// S1 <- SUB(A0, A1)
		SUB(&S0, B1, B0);	// S2 <- SUB(B1, B0)
		MULC_K(&S, S1, S0);	// S <- MULC_K(|S1|, |S0|)

		ADD_CA(&S, T1);	// S <- ADD(S, T1)
		ADD_CA(&S, T0);	// S <- ADD(S, T0)
		bi_assign(&tmp_S, S);
		bi_L_shift(&S, tmp_S, l * Word_Bit_Len);	// S <- S << lw
		ADD_CA(&R, S);	// R <- ADD(R, S)
		if ((*C) != NULL)
		{
			bi_resize(C, (*C)->wordlen, R->wordlen);
		}
		else
		{
			bi_new(C, R->wordlen);
		}
		array_init((*C)->a, (*C)->wordlen);
		bi_copy(C, R);
		(*C)->sign = A->sign ^ B->sign;  // return C = R

		bi_delete(&A1);
		bi_delete(&A0);
		bi_delete(&B1);
		bi_delete(&B0);
		bi_delete(&T1);
		bi_delete(&T0);
		bi_delete(&S1);
		bi_delete(&S0);
		bi_delete(&S);
		bi_delete(&tmp_S);
		bi_delete(&R);
	}
	bi_refine(*C);
}


/********** DIVISION ***********/

/**
 * @brief Binary long division function dividing A by a positive integer B
 *
 * @param Q A structure that stores the quotient of A divided by B [Structure double pointer variable]
 * @param R A structure that stores the remainder of A divided by B [Structure double pointer variable]
 * @param A The structure to be divided [structure pointer variable]
 * @param B A A structure to divide [structure pointer variable]
 *
 * @remark
 */
void Binary_Long_DIV(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	if (bi_get_sign(B) == NEGATIVE || bi_is_zero(B) == TRUE)      
		return;
	bigint* temp = NULL; 
	bigint* temp_R = NULL;
	bi_set_zero(R);    // R <- 0
	int i, n = bi_get_word_len(A) * Word_Bit_Len; 
	bi_new(Q, n);
	for (i = n - 1; i >= 0; i--)   // for i = n-1 downto 0 do
	{
		bi_assign(&temp_R, *R);
		bi_L_shift(R, temp_R, 1);
		(*R)->a[0] |= bi_get_j_bit(A, i);   // R <- 2R + Ai
		if (Compare(*R, B) != BIGGER_SECOND_ARGUMENT)    // if R >= B then
		{
			bi_new(&temp, (i / Word_Bit_Len) + 1);
			temp->a[bi_get_word_len(temp) - 1] ^= (word)1 << (i % Word_Bit_Len); 	// temp <- w^i
			ADD_CA(Q, temp);   // Q <- Q + temp
			SUB_CA(R, B);  // R <- R - B
		}
	}
	bi_refine(*Q);
	bi_delete(&temp);
	bi_delete(&temp_R);
	return;   
}

/**
 * @brief Binary long division function dividing two word A by a positive single word B
 *
 * @param Q A structure that stores the quotient of A divided by B [Structure double pointer variable]
 * @param A The structure to be divided [structure pointer variable]
 * @param B A A structure to divide [structure pointer variable]
 *
 * @remark
 */
void Long_DIV(bigint** Q, bigint* A, bigint* B) //void Long_Division, 2 word ver
{
	if (!(A->a[1] < B->a[0]))
	{
		printf("Long_DIV FAILURE : Enter a valid value.\n");
		return;
	}

	bigint* R = NULL;

	bi_new(Q, 1);	(*Q)->a[0] = 0;   	// Q = 0
	bi_new(&R, 1);	R->a[0] = A->a[1];	// R = A[1]

	bigint* tmp1 = NULL;	bi_new(&tmp1, 1);
	bigint* tmp2 = NULL;    bi_new(&tmp2, 1);

	bigint* W = NULL;		bi_new(&W, 1);
	W->a[0] = DIV_2_w_1;

	for (int i = Word_Bit_Len - 1; i >= 0; --i)  // for i = w-1 downto 0 do
	{
		if (Compare(R, W) == BIGGER_FIRST_ARGUMENT || Compare(R, W) == EQUAL_ARGUMENT)  	// if R >= 2^(w-1) then
		{

			(*Q)->a[0] += (word)1 << i;  // Q <- Q + 2^i
			bi_L_shift(&tmp2, R, 1);   // 2R
			SUB(&R, tmp2, B);   // 2R - B
			bi_resize(&tmp2, tmp2->wordlen, 1);
			tmp2->a[0] = (word)bi_get_j_bit(A, i);   // ai
			ADD_CA(&R, tmp2);   // R <- 2R + ai - B
		}
		else
		{
			bi_L_shift(&tmp1, R, 1);   // 2R
			tmp2->a[0] = (word)bi_get_j_bit(A, i);   // ai
			ADD_AB(&R, tmp1, tmp2);   // R <- 2R + ai
			if (Compare(R, B) == BIGGER_FIRST_ARGUMENT || Compare(R, B) == EQUAL_ARGUMENT)
			{
				(*Q)->a[0] += (word)1 << i;   // Q <- Q + 2^i
				SUB_CA(&R, B);   // R <- R - B
			}
		}
	}
	bi_delete(&R);
	bi_delete(&tmp1);
	bi_delete(&tmp2);
	bi_delete(&W);
	return;
}

/**
 * @brief For A and B with one word length difference, a function that quickly calculates the quotient and the remainder by approximation.
 *
 * @param Q A structure that stores the approximate quotient of A divided by B [Structure double pointer variable]
 * @param R A structure that stores the remainder of A divided by B [Structure double pointer variable]
 * @param A The structure to be divided [structure pointer variable]
 * @param B A A structure to divide [structure pointer variable]
 *
 * @remark
 */
void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	// Input condition
	bigint* tmp1 = NULL;   bi_new(&tmp1, 1);
	bigint* tmp_R = NULL;
	bi_L_shift(&tmp1, B, Word_Bit_Len);  // tmp1 = BW
	
	if (Compare(A, B) == BIGGER_SECOND_ARGUMENT || Compare(A, tmp1) != BIGGER_SECOND_ARGUMENT || B->a[B->wordlen - 1] < DIV_2_w_1)   // !(B <= A < BW) && B_m-1 >= 2^w-1
	{
		printf("DIVCC FAILURE : Enter a valid value.\n");
		bi_delete(&tmp1);
		return;
	}

	// Process
	if (A->wordlen == B->wordlen)	// if wordlen(A) = wordlen(B) then
	{
		bi_new(Q, 1);
		(*Q)->a[0] = A->a[A->wordlen - 1] / B->a[B->wordlen - 1];   // Q <- floor_func(A_m-1 / B_m-1)
	}
	
	if (A->wordlen == B->wordlen + 1)	// if wordlen(A) = wordlen(B) + 1
	{
		if (A->a[A->wordlen - 1] == B->a[B->wordlen - 1])	// if Am = Bm-1 then
		{
			bi_new(Q, 1);
			(*Q)->a[0] = Word_Bit_Mask; 	// Q <- W - 1
		}
		else
		{
			bigint* A_ = NULL;	bi_new(&A_, 2);
			bigint* B_ = NULL;	bi_new(&B_, 1);
			A_->a[1] = A->a[B->wordlen]; A_->a[0] = A->a[B->wordlen - 1];
			B_->a[0] = B->a[B->wordlen - 1];

			Long_DIV(Q, A_, B_);   // Q <- floor_func( (A_m * W + A_m-1) / B_m-1 )

			bi_delete(&A_);
			bi_delete(&B_);
		}
	}

	MUL(&tmp_R, B, *Q);   // BQ
	SUB(R, A, tmp_R);   // R <- A - BQ
	
	while (bi_get_sign(*R) == NEGATIVE)	// while R < 0 do
	{
		bi_set_one(&tmp1);
		SUB_CA(Q, tmp1);   // Q <- Q - 1
		ADD_CA(R, B);   // R <- R + B
	}

	bi_delete(&tmp1);
	bi_delete(&tmp_R);
	return;
}

/**
 * @brief A function to find quotient and remainder when the word length of A is greater than or equal to one word length of B.
 *
 * @param Q A structure that stores the approximate quotient of A divided by B [Structure double pointer variable]
 * @param R A structure that stores the remainder of A divided by B [Structure double pointer variable]
 * @param A The structure to be divided [structure pointer variable]
 * @param B A A structure to divide [structure pointer variable]
 *
 * @remark
 */
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	// Input condition
	bigint* tmp1 = NULL;   bi_new(&tmp1, 1);
	bigint* tmp2 = NULL;
	bi_L_shift(&tmp1, B, Word_Bit_Len);   // tmp1 = BW
	
	if (A->sign == NEGATIVE || Compare(A, tmp1) != BIGGER_SECOND_ARGUMENT)   // !(0 <= A < BW)
	{
		printf("DIVC FAILURE : Enter a valid value.\n");
		bi_delete(&tmp1);
		return;
	}

	// Process
	if (Compare(A, B) == BIGGER_SECOND_ARGUMENT)   // if A < B then
	{
		bi_set_zero(Q);
		bi_assign(R, A);      // return (Q, R) = (0, A)
		bi_delete(&tmp1);
		return;
	}

	int Bm_1 = bi_get_bit_len(B) - (B->wordlen - 1) * Word_Bit_Len;
	int k = Word_Bit_Len - Bm_1;   // 2^(W-1) =< 2^k * B_m-1 < 2^W

	bi_set_one(&tmp1);
	bi_set_one(&tmp2);

	bi_L_shift(&tmp1, A, k);	// tmp1 <- A * 2^k
	bi_L_shift(&tmp2, B, k);	// tmp2 <- B * 2^k

	DIVCC(Q, R, tmp1, tmp2);	// (Q, R) <- DIVCC(tmp1, tmp2)

	bi_set_one(&tmp1);
	bi_R_shift(&tmp1, *R, k);
	bi_assign(R, tmp1);	// R <- 2^(-k) * R

	bi_delete(&tmp1);
	bi_delete(&tmp2);
	return;
}

/**
 * @brief A function of dividing A by B for any positive integer.
 *
 * @param Q A structure that stores the quotient of A divided by B [Structure double pointer variable]
 * @param R A structure that stores the remainder of A divided by B [Structure double pointer variable]
 * @param A The structure to be divided [structure pointer variable]
 * @param B A A structure to divide [structure pointer variable]
 *
 * @remark
 */
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	// Input condition
	if (bi_is_zero(B) == TRUE || bi_get_sign(B) == NEGATIVE) // B <= 0 
		return ;

	// Process
	if (Compare(A, B) == BIGGER_SECOND_ARGUMENT) // if A < B then
	{
		bi_set_zero(Q);
		bi_assign(R, A);	// return (Q, R) = (0, A)
		return;
	}

	bigint* r = NULL;   bi_set_zero(&r);  // R <- 0
	bigint* Ai = NULL;	bi_new(&Ai, 1);
	bigint* rW = NULL;	
	bigint* rW_Ai = NULL;
	bigint* Qi = NULL;

	bi_new(Q, A->wordlen);

	for (int i = A->wordlen - 1; i >= 0; i--)  // for i = wordlen(A)-1 downto 0 do
	{
		Ai->a[0] = A->a[i];
		bi_L_shift(&rW, r, Word_Bit_Len);
		ADDC(&rW_Ai, rW, Ai);	// R <- RW + Ai
		DIVC(&Qi, &r, rW_Ai, B);	// (Qi, R) <- (R, B)
		(*Q)->a[i] = Qi->a[0];

		bi_delete(&rW);
		bi_delete(&rW_Ai);
		bi_delete(&Qi);
	}
	bi_assign(R, r);
	bi_refine(*Q);

	bi_delete(&r);
	bi_delete(&Ai); 
	return;
}


/********** SQUARING ***********/

/**
 * @brief A function that squares a single word A.
 *
 * @param C A structure to store square numbers [Structure double pointer variable]
 * @param A A structure to square [structure pointer variable]
 *
 * @remark
 */
void AA(bigint** C, bigint* A)
{
	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* C1 = NULL;
	bigint* C0 = NULL;
	bigint* tmp_T = NULL;
	bigint* T = NULL;
	
	bi_new(&A1, 1); bi_new(&A0, 1); bi_new(&C1, 1); bi_new(&C0, 1); bi_new(&T, 1);
	if (*C == NULL)
	{
		bi_new(C, 2);
	}
	bi_R_shift(&A1, A, (Word_Bit_Len >> 1));	// A1 <- |A| >> W/2
	bi_reduct(&A0, A, (Word_Bit_Len >> 1));		// A0 <- |A| mod 2^(W/2)
	MUL(&C1, A1, A1);	// C1 <- A1^2
	MUL(&C0, A0, A0);	// C0 <- A0^2
	(*C)->a[1] = C1->a[0];	// C <- (C1 << W)
	(*C)->a[0] = C0->a[0];	// C <- C + C0
	MUL(&T, A0, A1);	// T <- A0 * A1
	bi_assign(&tmp_T, T);
	bi_L_shift(&T, tmp_T, (Word_Bit_Len >> 1) + 1);	// T <- T << (W/2 + 1)
	ADD_CA(C, T);	// C <- C + T

	bi_delete(&A1);
	bi_delete(&A0);
	bi_delete(&C1);
	bi_delete(&C0);
	bi_delete(&tmp_T);
	bi_delete(&T);

}

/**
 * @brief A function that squares a A.
 *
 * @param C A structure to store square numbers [Structure double pointer variable]
 * @param A A structure to square [structure pointer variable]
 *
 * @remark
 */
void SQUC(bigint** C, bigint* A)
{
	bigint* tmp_C2 = NULL;
	bigint* C2 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;

	bi_new(&C2, 2 * A->wordlen); bi_new(&T1, 2 * A->wordlen); bi_new(&T2, 2 * A->wordlen);
	if (*C == NULL)
	{
		bi_new(C, 2 * A->wordlen);
	}

	for (int i = 0; i < A->wordlen; i++)
	{
		MUL_AB(&T1, A, A, i, i);	// T1 <- (Aj^2) << 2iw
		ADD_CA(C, T1);	// C <- T1 + C
		for (int j = i + 1; j < A->wordlen; j++)	// for j = i+1 to wordlen(A)-1 do
		{
			MUL_AB(&T2, A, A, i, j);	// T2 <- (Ai*Aj) << (i+j)W
			ADD_CA(&C2, T2);	// C2 <- C2 + T2
		}
	}
	bi_assign(&tmp_C2, C2);
	bi_L_shift(&C2, tmp_C2, 1);	// C2 <- C2 << 1
	ADD_CA(C, C2);	// return C + C2
	bi_refine(*C);
	bi_delete(&C2);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&tmp_C2);

}

/**
 * @brief A function that quickly squares by adjusting terms using specific techniques.
 *
 * @param C A structure to store square numbers [Structure double pointer variable]
 * @param A A structure to square [structure pointer variable]
 *
 * @remark
 */
void SQUC_K(bigint** C, bigint* A)
{
	int l = 0;

	if (Flag >= A->wordlen)		// if flag >= wordlen(A) then
	{
		SQUC(C, A);	// return SQUC(A)
		return;
	}
	l = (A->wordlen + 1) >> 1; 	// l <- (wordlen(A) + 1) >> 1
	{
		bigint* A1 = NULL;
		bigint* A0 = NULL;
		bigint* T1 = NULL;
		bigint* T0 = NULL;
		bigint* S = NULL;
		bigint* tmp_S = NULL;
		bigint* R = NULL;
		bi_new(&A1, l); bi_new(&A0, l); bi_new(&T1, 2 * l); bi_new(&T0, 2 * l); bi_new(&S, 2 * l); bi_new(&R, 4 * l);

		bi_R_shift(&A1, A, l * Word_Bit_Len);	// A1 <- |A| >> lw
		bi_reduct(&A0, A, l * Word_Bit_Len);	// A0 <- |A| mod 2^(lw)

		SQUC_K(&T1, A1);	// T1 <- SQUC_K(A1)
		SQUC_K(&T0, A0);	// T0 <- SQUC_K(A0)
		for (int i = 0; i < 2 * l; i++)
		{
			if (i < T0->wordlen)
				R->a[i] = T0->a[i];
			else
				R->a[i] = 0;
		}	// R <- T0
		for (int i = 0; i < T1->wordlen; i++)
		{
			if (i < T1->wordlen)
				R->a[i + 2 * l] = T1->a[i];
			else
				R->a[i + 2 * l] = 0;
		}	// R <- (T1 << 2lw) + R

		MULC_K(&S, A1, A0);		// S <- MULC_K(A1, A0)
		bi_assign(&tmp_S, S);
		bi_L_shift(&S, tmp_S, (l * Word_Bit_Len + 1));	// S <- S << (lw + 1)
		ADD_CA(&R, S);	// R <- R + S

		if ((*C) != NULL)
		{
			bi_resize(C, (*C)->wordlen, R->wordlen);
		}
		else
		{
			bi_new(C, R->wordlen);
		}
		array_init((*C)->a, (*C)->wordlen);
		bi_copy(C, R);
		(*C)->sign = NON_NEGATIVE; // return C = |R|
		bi_delete(&A1);
		bi_delete(&A0);
		bi_delete(&T1);
		bi_delete(&T0);
		bi_delete(&S);
		bi_delete(&tmp_S);
		bi_delete(&R);
	}
	bi_refine(*C);
}

/**
 * @brief A function that squares an arbitrary number.
 *
 * @param C A structure to store square numbers [Structure double pointer variable]
 * @param A A structure to square [structure pointer variable]
 *
 * @remark
 */
void SQU(bigint** C, bigint* A)
{
	bigint* abs_A = NULL;
	ABS(&abs_A, A);
	if (bi_is_zero(A) || (bi_is_one(A)) || (A->sign = NEGATIVE && bi_is_one(abs_A)))	// if A = 0 or A = 1 or A = -1
	{
		bi_assign(C, A);
		(*C)->sign = NON_NEGATIVE;	//return |A|
	}
	else
	{
		SQUC(C, A);
		(*C)->sign = NON_NEGATIVE;	// return SQUC(A)
	}
	bi_delete(&abs_A);
}

/********** MOD_EXP ***********/
/**
 * @brief Functions that affect squares in order to the coefficients from the highest term to the 0th term.
 *
 * @param B A structure to store the value of the modular exponential operation [Structure double pointer variable]
 * @param A base  [structure pointer variable]
 * @param n exponent [structure pointer variable]
 * @param M Modular value. [structure pointer variable]
 *
 * @remark
 */
void L2R(bigint** B, bigint* A, bigint* n, bigint* M) //Left to Right
{
	bigint* T = NULL;	bi_set_one(&T);     // T <- 1
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* TT = NULL;
	int len = bi_get_bit_len(n); 
	for (int i = len - 1; i >= 0; --i)  // for i = len-1 downto 0 do
	{
		SQU(&TT, T); // T^2
		DIV(&Q, &R, TT, M);	// T <- T^2 mod M

		if (bi_get_j_bit(n, i) == 1)
		{
			MUL(&T, R, A);  // T <- T * x^ni
		}
		else
		{
			bi_assign(&T, R);
		}
		DIV(&Q, &R, T, M);	// T <- T mod M
		bi_delete(&T);
		bi_assign(&T, R);
		bi_delete(&TT);
	}

	bi_assign(B, T);  // return T
	bi_delete(&T);
	bi_delete(&Q);
	bi_delete(&R);
	
}

/**
 * @brief Functions that affect squares in order to the coefficients from 0th term to the the highest term.
 *
 * @param B A structure to store the value of the modular exponential operation [Structure double pointer variable]
 * @param A base  [structure pointer variable]
 * @param n exponent [structure pointer variable]
 * @param M Modular value. [structure pointer variable]
 *
 * @remark
 */
void R2L(bigint** B, bigint* A, bigint* n, bigint* M) //Left to Right
{
	bigint* T0 = NULL;	bi_set_one(&T0);     // T0 <- 1
	bigint* T1 = NULL;	bi_assign(&T1, A);	// T1 <- A
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* TT = NULL;
	int len = bi_get_bit_len(n);

	for (int i = 0; i < len; i++)	// for i = len-1 downto 0 do
	{
		
		if (bi_get_j_bit(n, i) == 1)
		{
			MUL(&TT, T0, T1);  // TT <- T0 * T1
			DIV(&Q, &R, TT, M);	// R <- TT mod M
			bi_delete(&T0);
			bi_assign(&T0, R);  // T0 <- T0 * T1 mod M
		}
		
		bi_delete(&TT);
		SQU(&TT, T1);
		DIV(&Q, &R, TT, M); // R <- T1^2 mod M
		bi_delete(&T1);
		bi_assign(&T1, R);

		bi_delete(&TT);
	}

	bi_assign(B, T0); 	// return T0
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&Q);
	bi_delete(&R);
}

/**
 * @brief Modular exponential function that calls square and multiplication operations the same number of times.
 *
 * @param B A structure to store the value of the modular exponential operation [Structure double pointer variable]
 * @param A base  [structure pointer variable]
 * @param n exponent [structure pointer variable]
 * @param M Modular value. [structure pointer variable]
 *
 * @remark
 */
void Montgomery(bigint** B, bigint* A, bigint* n, bigint* M)
{
	bigint* T0 = NULL;	bi_set_one(&T0);	// T0 <- 1
	bigint* T1 = NULL;	bi_assign(&T1, A);	// T1 <- A
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* TT = NULL;
	int len = bi_get_bit_len(n);

	for (int i = len - 1; i >= 0; i--)	// for i = len-1 downto 0 do
	{
		if (bi_get_j_bit(n, i) == 1)	// if ni = 1 then
		{
			MUL(&TT, T0, T1);	// T0 <- T0 * T1
			DIV(&Q, &R, TT, M);	// R <- T0 mod M

			bi_delete(&T0);
			bi_assign(&T0, R);	// T0 <- T0 * T1 mod M

			bi_delete(&TT);
			SQU(&TT, T1);	// T1^2
			DIV(&Q, &R, TT, M);	// T1 mod M
			bi_delete(&T1);
			bi_assign(&T1, R);	// T1 <- T1^2 mod M
			bi_delete(&TT);
		}
		else
		{
			MUL(&TT, T0, T1);	// T1 <- T0 * T1
			DIV(&Q, &R, TT, M);	// R <- T1 mod M

			bi_delete(&T1);
			bi_assign(&T1, R);	// T1 <- T0 * T1 mod M

			bi_delete(&TT);
			SQU(&TT, T0); // T0^2
			DIV(&Q, &R, TT, M);	// T0^2 mod M
			bi_delete(&T0);
			bi_assign(&T0, R);	// T0 <- T0^2 mod M
			bi_delete(&TT);
		}
	}
	bi_assign(B, T0);	// return T0
	bi_delete(&T1);
	bi_delete(&T0);
	bi_delete(&Q);
	bi_delete(&R);
}