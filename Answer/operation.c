/**
 * @file operation.c
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-11-03
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "operation.h"

 /*********** GENERAL ***********/

 /**
  * @section GENERAL
  * @brief 일반 연산 함수
  *
  */

  /**
   *
   * @fn int CompareABS(bigint* A, bigint* B)
   * @brief 부호가 같은 두 bigint 구조체 A와 B를 비교하는 연산
   *
   * @param A 비교할 bigint 구조체 1
   * @param B 비교할 bigint 구조체 2
   * @return int 결과가 1이면 A > B, 0이면 A = B, -1이면 A < B
   *
   * @remark
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
 * @brief 두 bigint 구조체를 비교하는 연산
 *
 * @param A 비교할 bigint 구조체 1
 * @param B 비교할 bigint 구조체 2
 * @return int 결과가 1이면 A > B, 0이면 A = B, -1이면 A < B
 *
 * @remark
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
/**
 * @brief
 *
 * @param y
 * @param x
 * @param nbits
 *
 * @remark
 *
 */
void bi_L_shift(bigint** y, bigint* x, int nbits)   // bit shift
{
    int nword = nbits / Word_Bit_Len;  // word
    int rbits = nbits % Word_Bit_Len;


    ///bi_assign(y, x);
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
        bi_resize(y, x->wordlen, x->wordlen + nword + 1);  //free   main    
        int i;
        (*y)->a[(*y)->wordlen - 1] = x->a[x->wordlen - 1] >> (Word_Bit_Len - rbits);
        for (i = ((*y)->wordlen - 2); i > 0; --i)
        {
            (*y)->a[i] = (x->a[i] << rbits) | (x->a[i - 1] >> (Word_Bit_Len - rbits));
        }
        (*y)->a[i] = x->a[0] << rbits;

	}
	if (nbits == 0)
		bi_assign(y, x); // 0 bit shift 입력이 들어온 경우
	bi_refine(*y);
	return;
}

/**
 * @brief
 *
 * @param y
 * @param x
 * @param nbits
 *
 * @remark
 *
 */
void bi_R_shift(bigint** y, bigint* x, int nbits)
{
    int nword = nbits / Word_Bit_Len;  // word
    int remain = nbits % Word_Bit_Len;
    //bi_resize(y, x->wordlen, x->wordlen - nword); 
    //bi_assign(y, x);
    if (nword > x->wordlen)
    {
        for (int i = 0; i < x->wordlen; ++i)
        {
            (*y)->a[i] = 0;
        }
        return;
    }
    if (nword != 0)
    {
        int i;
        //bi_resize(y, x->wordlen, x->wordlen - nword);
        for (i = 0; i < x->wordlen - nword; ++i)
            (*y)->a[i] = x->a[i + nword];


        //refine     
    }
    if (remain != 0)
    {
        int i;
        //bi_resize(y, x->wordlen, x->wordlen - nword + 1);
        //(*y)->a[(*y)->wordlen - 1] = x->a[x->wordlen - 1] >> nbits;
        for (i = 0; i < ((*y)->wordlen - 1); ++i)
        {

			//(*y)->a[i] = ((*y)->a[i] >> nbits) | ((*y)->a[i + 1] << (Word_Bit_Len - nbits));
			(*y)->a[i] = (x->a[i] >> nbits) | (x->a[i + 1] << (Word_Bit_Len - nbits));
		}
		//(*y)->a[i] >>= nbits;
		(*y)->a[i] = x->a[i] >> nbits;

	}
	if (nbits == 0)
		bi_assign(y, x); // 0 bit shift 입력이 들어온 경우
	bi_refine(*y);
	return;
}

/**
 * @brief
 *
 * @param x
 * @param nbits
 *
 * @remark
 *
 */
void bi_reduct(bigint** dst, bigint* src, int nbits)
{
    int nword = nbits / Word_Bit_Len;  // word
    int k = nbits % Word_Bit_Len;
    if (nbits > src->wordlen * Word_Bit_Len)  // Exception
    {
        bi_copy(dst, src);
    }
    if (nword != 0)
    {

        /*word* X = NULL;
        X = (word*)malloc(sizeof(word) * nword);*/
        //      bi_new(&X, j);
        for (int i = 0; i < nword; i++)
            (*dst)->a[i] = src->a[i];
        //bi_copy(dst, src);
        bi_resize(dst, (*dst)->wordlen, nword);
    }
    if (k != 0)
    {
        bi_copy(dst, src);
        bi_resize(dst, (*dst)->wordlen, nword + 1);

        //      bigint* X = NULL;
        //      bi_new(&X, j + 1);
        for (int i = 0; i < nword + 1; i++)
        {
            if (i == nword)
                (*dst)->a[i] = src->a[i] & ((1 << k) - 1);
            else
                (*dst)->a[i] = src->a[i];
        }
        return;
    }
}

/**
 * @brief
 *
 * @param x
 * @return bigint*
 *
 * @remark
 *
 */
void ABS(bigint** y, bigint* x)
{
    bi_assign(y, x);
    bi_flip_sign(y);
}

/*********** ADDITION ************/


void ADD_AB(bigint** C, bigint* A, bigint* B) // C: result
{
    if (*C != NULL)
    {
        bi_delete(C);
    }

    bi_new(C, 1);
    (*C)->a[0] = (A->a[0] + B->a[0]) % (1 << Word_Bit_Len);

    if ((*C)->a[0] < A->a[0])
    {
        bi_resize(C, 1, 2);
        (*C)->a[1] = 1;  //carry
    }
    (*C)->sign = A->sign;
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 * @param c
 * @param idx
 *
 * @remark
 *
 */
int ADD_ABc(bigint** C, bigint* A, bigint* B, int c, int idx) // int c -> {0, 1}
{
	/*
		if (*C == NULL)
			bi_new(C, idx + 1);
		bi_resize(C, idx + 1, idx + 2);
	*/
	int carry = 0;
	(*C)->a[idx] = (A->a[idx] + c) % ((long long int)1 << Word_Bit_Len);

    if ((*C)->a[idx] < A->a[idx])
    {
        //(*C)->a[idx + 1] = 1;
        carry = 1;
    }

	(*C)->a[idx] = ((*C)->a[idx] + B->a[idx]) % ((long long int)1 << Word_Bit_Len);

    if ((*C)->a[idx] < B->a[idx])
    {
        //(*C)->a[idx + 1] += 1;
        carry += 1;
    }
    return carry;
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
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
	int carry = 0;
	for (int j = 0; j < A->wordlen; j++)
	{
		carry = ADD_ABc(C, A, tmp_B, carry, j);
		//carry = (*C)->a[j + 1];
	}
	if (carry == 1)
	{
		//bi_resize(C, (*C)->wordlen, (*C)->wordlen + 1);
		bi_resize(C, A->wordlen, A->wordlen + 1);
		(*C)->a[(*C)->wordlen - 1] += 1;
	}
	if ((*C)->a[A->wordlen] == 0)
	{
		(*C)->sign = A->sign;
		bi_resize(C, A->wordlen + 1, A->wordlen);
		return;
	}
	else
	{
		(*C)->sign = A->sign;
		return;
	}
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
 *
 */
void ADD(bigint** C, bigint* A, bigint* B)
{
    if (bi_is_zero(A))
        bi_copy(C, B);

    else if (bi_is_zero(B))
        bi_copy(C, A);

    else if (A->sign == NON_NEGATIVE && B->sign == NEGATIVE)
    {
        bigint* abs = NULL;
        ABS(&abs, B);
        SUB(C, A, abs);
        bi_delete(&abs);
    }
    else if (A->sign == NEGATIVE && B->sign == NON_NEGATIVE)
    {
        bigint* abs = NULL;
        ABS(&abs, A);
        SUB(C, B, abs);
        bi_delete(&abs);
    }
    else if (A->wordlen >= B->wordlen)
        ADDC(C, A, B);

    else
        ADDC(C, B, A);

    return;
}


/********** SUBTRACTION ***********/
/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
 */
void SUB_AB(bigint** C, bigint* A, bigint* B)
{
    if ((*C) != NULL)
    {
        bi_delete(C);
    }
    bi_new(C, 1);

	(*C)->a[0] = (A->a[0] - B->a[0]) % ((long long int)1 << Word_Bit_Len);
	//	bi_reduct(C, Word_Bit_Len);
	bi_resize(C, 1, 2);
	if (A->a[0] >= B->a[0])
	{
		(*C)->a[1] = 0;
		(*C)->sign = NON_NEGATIVE;  // sign
	}
	else
	{
		(*C)->a[1] = 1;
		(*C)->sign = NEGATIVE;  // sign
	}
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 * @param b
 * @param idx
 *
 * @remark
 */
int SUB_AbB(bigint** C, bigint* A, bigint* B, int b, int idx) // int b -> {0, 1}, idx -> A, B�� a  �ε���
{
	/*
	if ((*C) == NULL)
	{
		bi_new(C, idx + 1);
	}
	bi_resize(C, idx + 1, idx + 2);
	*/
	int borrow = 0;
	(*C)->a[idx] = ((A->a[idx] - b)) % ((long long int)1 << Word_Bit_Len);

	if (A->a[idx] < b)
	{
		borrow = 1;
	}

	if ((*C)->a[idx] < B->a[idx])
	{
		borrow += 1;
	}
	(*C)->a[idx] = (((*C)->a[idx] - B->a[idx])) % ((long long int)1 << Word_Bit_Len);
	return borrow;
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
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
	int borrow = 0;
	for (int j = 0; j < A->wordlen; j++)
	{
		borrow = SUB_AbB(C, A, tmp_B, borrow, j);
		//borrow = (*C)->a[j + 1];
	}

    bi_refine(*C);
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
 */
void SUB(bigint** C, bigint* A, bigint* B)
{
	if (bi_is_zero(A))
	{
		bi_copy(C, B);
		bi_flip_sign(C);
	}
	else if (bi_is_zero(B))
	{
		bi_copy(C, A);
	}
	else if (Compare(A, B) == EQUAL_ARGUMENT)
		bi_set_zero(C);
	else if (A->sign == NON_NEGATIVE && B->sign == NON_NEGATIVE)
	{
		if (CompareABS(A, B) == BIGGER_FIRST_ARGUMENT)
		{
			SUBC(C, A, B);
			(*C)->sign = NON_NEGATIVE;
		}
		else
		{
			SUBC(C, B, A);
			(*C)->sign = NEGATIVE;
		}
	}
	else if (A->sign == NEGATIVE && B->sign == NEGATIVE)
	{
		if (CompareABS(A, B) == -1)
		{
			bigint* absA = NULL;
			bigint* absB = NULL;
			ABS(&absA, A);
			ABS(&absB, B);
			SUBC(C, absB, absA);
			(*C)->sign = NON_NEGATIVE;
			bi_delete(&absA);
			bi_delete(&absB);
		}
		else if (Compare(B, A))
		{
			bigint* absA = NULL;
			bigint* absB = NULL;
			ABS(&absA, A);
			ABS(&absB, B);
			SUBC(C, absA, absB);
			(*C)->sign = NEGATIVE;
			bi_delete(&absA);
			bi_delete(&absB);
		}
	}
	else if (A->sign == NON_NEGATIVE && B->sign == NEGATIVE)
	{
		bigint* absB = NULL;
		ABS(&absB, B);
		if (A->wordlen >= B->wordlen)
			ADD(C, A, absB);
		else
			ADD(C, absB, A);
		(*C)->sign = NON_NEGATIVE;
		bi_delete(&absB);
	}
	else
	{
		bigint* absA = NULL;
		ABS(&absA, A);
		if (A->wordlen >= B->wordlen)
			ADD(C, B, absA);
		else
			ADD(C, absA, B);
		(*C)->sign = NEGATIVE;
		bi_delete(&absA);

    }
    bi_refine(*C);
    return;
}

/********** MULTIPLICATION ***********/
/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 * @param idxA
 * @param idxB
 *
 * @remark
 */
void MUL_AB(bigint** C, bigint* A, bigint* B, int idxA, int idxB)  // A, B: 1 word 
{
    word A1 = 0, A0 = 0;
    word B1 = 0, B0 = 0;
    word T1 = 0, T0 = 0, T = 0;
    word C1 = 0, C0 = 0;

	A1 = A->a[idxA] >> (Word_Bit_Len / 2);
	A0 = A->a[idxA] % ((long long int)1 << (Word_Bit_Len / 2));
	B1 = B->a[idxB] >> (Word_Bit_Len / 2);
	B0 = B->a[idxB] % ((long long int)1 << (Word_Bit_Len / 2));

	T1 = A1 * B0;
	T0 = A0 * B1;
	T0 = (T1 + T0) % ((long long int)1 << Word_Bit_Len);
	T1 = T0 < T1;
	C0 = A0 * B0;
	C1 = A1 * B1;
	T = C0;

    if ((*C) == NULL)
    {
        bi_new(C, 1);
    }

    bi_set_zero(C);

	(*C)->a[idxA + idxB] = (C0 + (T0 << (Word_Bit_Len / 2))) % ((long long int)1 << Word_Bit_Len);
	C0 = (*C)->a[idxA + idxB];
	(*C)->a[idxA + idxB + 1] = C1 + (T1 << (Word_Bit_Len / 2)) + (T0 >> (Word_Bit_Len / 2)) + (C0 < T);
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
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

    for (int i = 0; i < A->wordlen; i++)
    {
        for (int j = 0; j < B->wordlen; j++)
        {
            MUL_AB(&T, A, B, i, j);
            ADDC(C, *C, T);
        }
    }
    bi_delete(&T);
}

/**
 * @brief
 *
 * @param C
 * @param A
 * @param B
 *
 * @remark
 */
void MUL(bigint** C, bigint* A, bigint* B)
{
    bigint* absA = NULL;
    bigint* absB = NULL;
    ABS(&absA, A);
    ABS(&absB, B);

    if (bi_is_zero(A) || bi_is_zero(B))
    {
        bi_set_zero(C);
        return;
    }

    else if (bi_is_one(A) && A->sign == NON_NEGATIVE)
    {
        bi_new(C, B->wordlen);
        array_copy((*C)->a, B->a, B->wordlen);
        (*C)->sign = B->sign;
        (*C)->wordlen = B->wordlen;
    }
    else if (A->sign == NEGATIVE && bi_is_one(absA))
    {
        //bi_flip_sign(&B);
        bi_new(C, B->wordlen);
        array_copy((*C)->a, B->a, B->wordlen);
        (*C)->sign = absB->sign;
        (*C)->wordlen = B->wordlen;
    }
    else if (bi_is_one(B) && B->sign == NON_NEGATIVE)
    {
        bi_new(C, A->wordlen);
        array_copy((*C)->a, A->a, A->wordlen);
        (*C)->sign = A->sign;
        (*C)->wordlen = A->wordlen;
    }
    else if (B->sign == NEGATIVE && bi_is_one(absB))
    {
        bi_new(C, A->wordlen);
        array_copy((*C)->a, A->a, A->wordlen);
        (*C)->sign = absA->sign;
        (*C)->wordlen = A->wordlen;
    }

	else
	{
		int sign = A->sign ^ B->sign;
		MULC(C, A, B);
		//MULC_K(C, A, B);
		(*C)->sign = sign;
	}
	bi_refine(*C);
}



void MULC_K(bigint** C, bigint* A, bigint* B) // Fast Multiplication  //return R-> C
{
    int flag = 1;
    int l = 0;

    if (flag >= min(A->wordlen, B->wordlen))
    {
        MUL(C, A, B);
        return;
    }
    l = (max(A->wordlen, B->wordlen) + 1) >> 1;
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
        bi_new(&A1, l); bi_new(&A0, l); bi_new(&B1, l); bi_new(&B0, l); bi_new(&T1, 2 * l); bi_new(&T0, 2 * l); bi_new(&S1, l); bi_new(&S0, l); bi_new(&S, 2 * l); bi_new(&R, 4 * l);

		bi_R_shift(&A1, A, l * Word_Bit_Len);
		bi_reduct(&A0, A, l * Word_Bit_Len);
		bi_R_shift(&B1, B, l * Word_Bit_Len);
		bi_reduct(&B0, B, l * Word_Bit_Len);
		MULC_K(&T1, A1, B1);
		MULC_K(&T0, A0, B0);
		for (int i = 0; i < 2 * l; i++)
		{
			if (i < T0->wordlen)
				R->a[i] = T0->a[i];
			else
				R->a[i] = 0;
		}
		for (int i = 0; i < 2 * l; i++)
		{
			if (i < T1->wordlen)
				R->a[i + 2 * l] = T1->a[i];
			else
				R->a[i + 2 * l] = 0;
		}
		SUB(&S1, A0, A1);
		SUB(&S0, B1, B0);
		MULC_K(&S, S1, S0);
		ADD(&S, S, T1);
		ADD(&S, S, T0);
		bi_L_shift(&S, S, l * Word_Bit_Len);
		ADD(&R, R, S);
		bi_new(C, A->wordlen + B->wordlen);
		array_init((*C)->a, (*C)->wordlen);
		bi_copy(C, R);

        bi_delete(&A1);
        bi_delete(&A0);
        bi_delete(&B1);
        bi_delete(&B0);
        bi_delete(&T1);
        bi_delete(&T0);
        bi_delete(&S1);
        bi_delete(&S0);
        bi_delete(&S);
        bi_delete(&R);
    }
    bi_refine(*C);
}


/********** DIVISION ***********/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B)
{
    if (Compare(A, B) == BIGGER_SECOND_ARGUMENT)
    {
        bi_new(*Q, 1);
        (*Q)->a[0] = 0;         // Q = 0
        bi_new(*R, A->wordlen);
        bi_assign(R, A);      // R = A
        return;
    }

	bigint* r = NULL;	bi_new(&r, 1);	r->a[0] = 0;
	//bigint* W = NULL;	bi_new(&W, 2);	W->a[1] = 1;	W->a[0] = 0;
	bigint* Ai = NULL;	bi_new(&Ai, 1);
	bigint* rW = NULL;
	bigint* rW_Ai = NULL;
	bigint* Qi = NULL;

	bi_new(Q, A->wordlen);

	for (int i = A->wordlen - 1; i >= 0; i--)
	{
		Ai->a[0] = A->a[i];
		// MULC_K(&rW, r, W);
		bi_L_shift(&rW, r, Word_Bit_Len);
		ADDC(&rW_Ai, rW, Ai);
		DIVC(&Qi, R, rW_Ai, B);
		(*Q)->a[i] = Qi->a[0];
		bi_assign(&r, *R);

		bi_delete(&rW);
		bi_delete(&rW_Ai);
		bi_delete(&Qi);
		bi_delete(R);
	}
	bi_assign(R, r);
	bi_refine(*Q);

	bi_delete(&r);
	//bi_delete(&W);
	bi_delete(&Ai);
}

void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	// Input condition
	bigint* tmp1 = NULL;	bi_new(&tmp1, 1);	// tmp1 = BW
	bigint* tmp2 = NULL; //	bi_new(&tmp2, 2);   //아래 resize 수정
	bi_new(Q, 1);
	bi_new(R, 1);
	//tmp2->a[1] = 1;	tmp2->a[0] = 0;
	//MULC_K(&tmp1, B, tmp2);
	bi_L_shift(&tmp1, B, Word_Bit_Len);
	if (A->sign == NEGATIVE || Compare(A, tmp1) != BIGGER_SECOND_ARGUMENT)	// !(0 <= A < BW)
	{
		printf("DIVC FAILURE : Enter a valid value.\n");
		return;
	}

	// Process
	if (Compare(A, B) == BIGGER_SECOND_ARGUMENT)	// A < B
	{
		bi_new(Q, 1);
		(*Q)->a[0] = 0;			// Q = 0
		bi_new(R, A->wordlen);
		bi_assign(R, A);		// R = A
		return;
	}


	int Bm_1 = bi_get_bit_len(B) - (B->wordlen - 1) * Word_Bit_Len;
	int k = Word_Bit_Len - Bm_1;	// 2^(W-1) <= 2^k * B_m-1 < 2^W

	bi_resize(&tmp1, tmp1->wordlen, 1);	tmp1->a[0] = 1;	// tmp1 초기화
	// bi_resize(&tmp2, tmp2->wordlen, 1);	tmp2->a[0] = 1; // tmp2 초기화
	bi_new(&tmp2, 1);	tmp2->a[0] = 1;

    bi_L_shift(&tmp1, A, k);   // A <- A * 2^k
    bi_L_shift(&tmp2, B, k);   // B <- B * 2^k

	DIVCC(Q, R, tmp1, tmp2);

	//bi_delete(&tmp1);
	//bi_assign(&tmp1, *R);
	//bi_resize(R, (*R)->wordlen, 1);
	//(*R)->a[0] = tmp1->a[0] >> k;	// R <- R * 2^(-k)
	//bi_delete(R);
	//bi_assign(R, tmp1);
	bi_set_one(&tmp1);
	bi_resize(&tmp1, tmp1->wordlen, (*R)->wordlen);
	bi_R_shift(&tmp1, *R, k);
	bi_delete(R);
	bi_assign(R, tmp1);

    bi_delete(&tmp1);
    bi_delete(&tmp2);
}

void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	// Input condition
	bigint* tmp1 = NULL;	bi_new(&tmp1, 1);	// tmp1 = BW
	bigint* tmp2 = NULL;	bi_new(&tmp2, 2);   //아래 resize 수정

	//tmp2->a[1] = 1;	tmp2->a[0] = 0;
	//MULC_K(&tmp1, B, tmp2);
	bi_L_shift(&tmp1, B, Word_Bit_Len);
	if (Compare(A, B) == BIGGER_SECOND_ARGUMENT || Compare(A, tmp1) != BIGGER_SECOND_ARGUMENT || B->a[B->wordlen - 1] < 0x80)	// !(B <= A < BW) && B_m-1 >= 2^w-1
	{
		printf("DIVCC FAILURE : Enter a valid value.\n");
		return;
	}

	// Process
	bi_resize(&tmp1, tmp1->wordlen, 1);	// tmp1 <- 1
	tmp1->a[0] = 1;

	if (A->wordlen == B->wordlen)
	{
		bi_new(Q, 1);
		(*Q)->a[0] = A->a[A->wordlen - 1] / B->a[B->wordlen - 1];	// floor_func(A_m-1 / B_m-1)
	}
	if (A->wordlen == B->wordlen + 1)
	{
		if (A->a[A->wordlen - 1] == B->a[B->wordlen - 1])
		{
			bi_new(Q, 1);
			SUBC(Q, tmp2, tmp1);	// Q <- W - 1
		}
		else
		{
			bigint* A_ = NULL;
			bigint* B_ = NULL;
			bi_new(&A_, 2); A_->a[1] = A->a[B->wordlen]; A_->a[0] = A->a[B->wordlen - 1];
			bi_new(&B_, 1); B_->a[0] = B->a[B->wordlen - 1];
			Long_DIV(Q, A_, B_);	// Q hat	// Q <- floor_func( (A_m * W + A_m-1) / B_m-1 )

			bi_delete(&A_);
			bi_delete(&B_);
		}
	}
	bi_new(R, 1);
	MULC_K(R, B, *Q);	// BQ
	SUB(R, A, *R);	// R <- A - BQ

    while ((*R)->sign == NEGATIVE)
    {
        SUBC(Q, *Q, tmp1);   // Q <- Q - 1
        ADD(R, *R, B);   // R <- R + B
    }

    bi_delete(&tmp1);
    bi_delete(&tmp2);
}

void exp2i(bigint** ret, int i)
{
	bigint* tmp = NULL;
	bi_set_one(&tmp);
	bi_L_shift(ret, tmp, i);
	bi_delete(&tmp);
	return;
}

void Long_DIV(bigint** Q, bigint* A, bigint* B) //void Long_Division, 2 word ver
{
	bigint* R = NULL;
	if (!(A->a[1] < B->a[0]))
	{
		printf("Long_DIV FAILURE : Enter a valid value.\n");
		return;
	}

    bi_new(Q, 2);   // Q = 0
    (*Q)->a[0] = 0;   (*Q)->a[1] = 0;
    bi_new(&R, 1);   // R = A[1]

    R->a[0] = A->a[1];

    bigint* tmp1 = NULL;   bi_new(&tmp1, 1);
    bigint* tmp2 = NULL;    bi_new(&tmp2, 1);

    bigint* W = NULL;
    bi_new(&W, 1);
    W->a[0] = (1 << (Word_Bit_Len - 1));

	for (int i = Word_Bit_Len - 1; i >= 0; --i)
	{
		if (Compare(R, W) == BIGGER_FIRST_ARGUMENT || Compare(R, W) == EQUAL_ARGUMENT)
		{
			exp2i(&tmp1, i);   // 2^i
			ADD(Q, *Q, tmp1);   // Q <- Q + 2^i

			bi_L_shift(&tmp2, R, 1);   // 2R  // tmp 안 쓰면 짤림
			SUB(&R, tmp2, B);   // 2R - B
			bi_resize(&tmp2, tmp2->wordlen, 1);
			tmp2->a[0] = (word)bi_get_j_bit(A, i);   // a_j
			ADD(&R, R, tmp2);   // R <- 2R + 1 - B
		}
		else
		{
			bi_set_zero(&tmp1);
			bi_L_shift(&tmp1, R, 1);   // 2R
			tmp2->a[0] = (word)bi_get_j_bit(A, i);   // a_j
			ADD_ABc(&R, tmp1, tmp2, 0, 0);   // R <- 2R + 1
			if (Compare(R, B) == BIGGER_FIRST_ARGUMENT || Compare(R, B) == EQUAL_ARGUMENT)
			{
				exp2i(&tmp1, i);   // 2^i
				ADD(Q, *Q, tmp1);   // Q <- Q + 2^i
				bi_assign(&tmp2, R);
				SUB(&R, tmp2, B);   // R <- R - B
			}
		}
	}

	// 변수 해제
	bi_delete(&R);
	bi_delete(&tmp1);
	bi_delete(&tmp2);
	bi_delete(&W);
}

/********** SQUARING ***********/
void AA(bigint** C, bigint* A)
{
    bigint* A1 = NULL;
    bigint* A0 = NULL;
    bigint* C1 = NULL;
    bigint* C0 = NULL;
    bigint* T = NULL;

    bi_new(&A1, 1); bi_new(&A0, 1); bi_new(&C1, 1); bi_new(&C0, 1); bi_new(&T, 1);
    if (*C == NULL)
    {
        bi_new(C, 2);
    }
    bi_R_shift(&A1, A, (Word_Bit_Len >> 1));
    bi_reduct(&A0, A, (Word_Bit_Len >> 1));
    MUL(&C1, A1, A1);
    MUL(&C0, A0, A0);
    (*C)->a[1] = C1->a[0];
    (*C)->a[0] = C0->a[0];
    MUL(&T, A0, A1);
    bi_L_shift(&T, T, (Word_Bit_Len >> 1) + 1);
    ADD(C, *C, T);

    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&C1);
    bi_delete(&C0);
    bi_delete(&T);

}
void SQUC(bigint** C, bigint* A)
{
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
        MUL_AB(&T1, A, A, i, i);
        ADD(C, *C, T1);
        for (int j = i + 1; j < A->wordlen; j++)
        {
            MUL_AB(&T2, A, A, i, j);
            ADD(&C2, C2, T2);

        }
    }
    bi_L_shift(&C2, C2, 1);
    ADD(C, *C, C2);
    bi_delete(&C2);
    bi_delete(&T1);
    bi_delete(&T2);
}
void SQUC_K(bigint** C, bigint* A)
{
    int flag = 1;
    int l = 0;

    if (flag >= A->wordlen)
    {
        SQUC(C, A);
        return;
    }
    l = (A->wordlen + 1) >> 1;
    {
        bigint* A1 = NULL;
        bigint* A0 = NULL;
        bigint* T1 = NULL;
        bigint* T0 = NULL;
        bigint* S = NULL;
        bigint* R = NULL;
        bi_new(&A1, l); bi_new(&A0, l); bi_new(&T1, 2 * l); bi_new(&T0, 2 * l); bi_new(&S, 2 * l); bi_new(&R, 4 * l);

		bi_R_shift(&A1, A, l * Word_Bit_Len);
		bi_reduct(&A0, A, l * Word_Bit_Len);

		SQUC_K(&T1, A1);
		SQUC_K(&T0, A0);
		for (int i = 0; i < 2 * l; i++)
		{
			R->a[i] = T0->a[i];
		}
		for (int i = 0; i < 2 * l; i++)
		{
			R->a[i + 2 * l] = T1->a[i];
		}
		MULC_K(&S, A1, A0);
		bi_L_shift(&S, S, (l * Word_Bit_Len + 1));
		ADDC(&R, R, S);
		bi_new(C, 2 * A->wordlen);
		bi_copy(C, R);

        bi_delete(&A1);
        bi_delete(&A0);
        bi_delete(&T1);
        bi_delete(&T0);
        bi_delete(&S);
        bi_delete(&R);
    }
    bi_refine(*C);
}
void SQU(bigint** C, bigint* A)
{
    bigint* abs_A = NULL;
    ABS(&abs_A, A);
    if (bi_is_zero(A) || (bi_is_one(A)) || (A->sign = NEGATIVE && bi_is_one(abs_A)))
    {
        bi_assign(C, A);
        (*C)->sign = NON_NEGATIVE;
    }
    else
    {
        SQUC(C, A);
    }
}

/********** MOD_EXP ***********/
void L2R(bigint** B, bigint* A, bigint* n, bigint* M) //Left to Right
{
	bigint* T = NULL;
	bi_set_one(&T);     // T <- 1
	bigint* Q = NULL;
	bigint* R = NULL;

	int len = bi_get_bit_len(n);    // n의 비트 길이
	printf("%d", len);

	for (int i = len - 1; i >= 0; --i)
	{
		bigint* TT = NULL;
		SQUC(&TT, T); // T^2
		DIV(&Q, &R, TT, M); // R <- T^2 mod M
		bi_delete(&TT);

		if (bi_get_j_bit(n, i) == 1);
		{
			MUL(&T, R, A);  // T <- R * x : T^2 * x
			DIV(&Q, &R, T, M);  // T <- T^2 * x mod M
		}

		bi_delete(&T);
		bi_assign(&T, R);  // T <- (T^2 * x) mod M or T^2 mod M
	}

	bi_assign(B, T);
	bi_delete(&T);
	bi_delete(&Q);
	bi_delete(&R);
}
void R2L() //Right to Left
{

}
void Montgomery()
{

}
