/**
 * @file data.c
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-11-04
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "data.h"


 /*********** MEMORY ***********/

  /**
  * @brief
  *
  * @param x
  * @param wordlen
  *
  * @remark �ڼ��� ����
  */

void bi_new(bigint** x, int wordlen)
{
    if (*x != NULL)
    {
        bi_delete(x);
    }

    (*x) = (bigint*)malloc(sizeof(bigint));
    if (*x != NULL)
    {
        (*x)->sign = NON_NEGATIVE;
        (*x)->wordlen = wordlen;
        (*x)->a = (word*)calloc(wordlen, sizeof(word));
        //memset((*x)->a, 0, wordlen);
    }

}

/**
 * @brief ª�� ����
 *
 * @param x ���� ����
 *
 * @remark �ڼ��� ����
 */

void bi_delete(bigint** x)
{
    if (*x == NULL)
    {
        return;
    }

#ifdef ZERORIZE
    array_init((*x)->a, (*x)->wordlen);
#endif // ZERORIZE

    free((*x)->a);
    free(*x);
    *x = NULL;
}

/**
 * @brief
 *
 * @param x
 * @param prev_wordlen
 * @param new_wordlen
 *
 * @remark �ڼ��� ����
 */

void bi_resize(bigint** x, int prev_wordlen, int new_wordlen)
{
    if ((*x)->a == NULL)
    {
        (*x)->a = (word*)malloc(sizeof(word) * new_wordlen);
        return;
    }
    word* tmp;
    if (new_wordlen == 0)
    {
        tmp = (word*)calloc(1, sizeof(word));
        array_copy(tmp, (*x)->a, 1);
        (*x)->a = (word*)realloc((*x)->a, sizeof(word) * 1);
        if ((*x)->a != NULL)
        {
            (*x)->wordlen = 1;
            array_copy((*x)->a, tmp, (*x)->wordlen);
        }
    }
    if (prev_wordlen > new_wordlen)
    {
        tmp = (word*)calloc(new_wordlen, sizeof(word));
        array_copy(tmp, (*x)->a, new_wordlen);
        (*x)->a = (word*)realloc((*x)->a, sizeof(word) * new_wordlen);
        if ((*x)->a != NULL)
        {
            (*x)->wordlen = new_wordlen;
            array_copy((*x)->a, tmp, (*x)->wordlen);
        }
    }
    else
    {
        tmp = (word*)calloc(prev_wordlen, sizeof(word));
        array_copy(tmp, (*x)->a, prev_wordlen);
        (*x)->a = (word*)realloc((*x)->a, sizeof(word) * new_wordlen);
        if ((*x)->a != NULL)
        {
            (*x)->wordlen = new_wordlen;
            array_copy((*x)->a, tmp, prev_wordlen);
            for (int i = prev_wordlen; i < new_wordlen; i++)
            {
                (*x)->a[i] = 0;
            }
        }

    }

    free(tmp);
    tmp = NULL;
}

/*********** SET ***********/

/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_set_one(bigint** x)
{
    if (*x == NULL)
        bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x01;
    (*x)->wordlen = 1;
}
/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_set_zero(bigint** x)
{
    if ((*x) == NULL)
    {
        bi_new(x, 1);
    }
    (*x)->sign = NON_NEGATIVE;
    for (int i = 0; i < (*x)->wordlen; i++)
    {
        (*x)->a[i] = 0x0;
    }

}

/**
 * @brief
 *
 * @param x
 * @param sign
 * @param a
 * @param wordlen
 *
 * @remark
 */
void bi_set_by_array(bigint** x, int sign, word* a, int wordlen)
{
    if (((sign != NEGATIVE) && (sign != NON_NEGATIVE)) || (a == NULL) || a < 0 || wordlen < 0)
        return;

    //bi_new(x, wordlen);
    (*x)->sign = sign;
    (*x)->wordlen = wordlen;
    array_copy((*x)->a, a, wordlen);

}


/**
 * @brief
 *
 * @param x
 * @param sign
 * @param str
 *
 * @remark
 */
void bi_set_by_string(bigint** x, int sign, char* str)  //, int base)  //string : hex string, base = 16
{
    if (((sign != NEGATIVE) && (sign != NON_NEGATIVE)) || (str == NULL) || str < 0)//||(base != 2 && base != 10 && base != 16 ))
        return;
    if (*x != NULL)
        bi_delete(x);
    int str_len = 0;
    str_len = (int)strlen(str) % 2 ? (int)strlen(str) + 1 : (int)strlen(str);
    int array_size = 0;
    word tmp[3];


    //bi_new(x, array_size);
    switch (Word_Bit_Len)
    {
    case 8:
        array_size = str_len / 2;
        bi_new(x, array_size);
        strtoword((*x)->a, str, array_size);
        break;
    case 32:
        array_size = str_len / 4;
        bi_new(x, array_size);
        strtoword((*x)->a, str, array_size);
        break;
    case 64:
        array_size = str_len / 8;
        bi_new(x, array_size);
        strtoword((*x)->a, str, array_size);
        break;

    default:
        break;
    }
   
    (*x)->sign = sign;
    (*x)->wordlen = array_size;
}

/**
 * @brief
 *
 * @param dst
 * @param src
 *
 * @remark
 */
void bi_assign(bigint** dst, bigint* src)
{
    /**/
    if (*dst == NULL)
        bi_new(dst, src->wordlen);
    //bi_delete(dst);

    (*dst)->sign = src->sign;
    array_copy((*dst)->a, src->a, src->wordlen);
}

/**
 * @brief
 *
 * @param x
 * @param sign
 * @param wordlen
 *
 * @remark
 */
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

/**
 * @brief
 *
 * @param dst
 * @param wordlen
 *
 * @remark
 */
void array_rand(word* dst, int wordlen)
{
    srand((unsigned int)time(NULL));
    byte* p = (byte*)dst;
    int cnt = wordlen * sizeof(word);

    /*while (cnt > 0)
    {
        *p = rand() & 0xff;
        p++;
        cnt--;
    }*/
    if (Word_Bit_Len == 8)
    {
        while (cnt > 0)
        {
            *p = rand() & 0xff;
            p++;
            cnt--;
        }
    }
    if (Word_Bit_Len == 32)
    {
        while (cnt > 0)
        {
            *p = rand() & 0xffffffff;
            p++;
            cnt--;
        }
    }
    if (Word_Bit_Len == 64)
    {
        while (cnt > 0)
        {
            *p = rand() & 0xffffffffffffffff;
            p++;
            cnt--;
        }
    }

}

/**
 * @brief
 *
 * @param dst
 * @param src
 * @param word_len
 *
 * @remark
 */
void array_copy(word* dst, word* src, int word_len)  // dst -> destination, src -> source
{
    for (int i = 0; i < word_len; i++)
    {
        dst[i] = src[i];
    }
    //memcpy(dst, src, word_len);
}

/**
 * @brief
 *
 * @param a
 * @param word_len
 *
 * @remark
 */
void array_init(word* a, int word_len)
{
    memset(a, 0, word_len * sizeof(word));
}

/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_refine(bigint* x)
{
    if (x == NULL)
    {
        return;
    }

    int new_wordlen = x->wordlen;
    while (new_wordlen > 1)  // at least one word needed
    {
        if (x->a[new_wordlen - 1] != 0)
        {
            break;
        }
        new_wordlen--;
    }

    if (x->wordlen != new_wordlen)
    {
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    }
    if ((x->wordlen == 1) && (x->a[0] == 0x0))
    {
        x->sign = NON_NEGATIVE;
    }
}

/**
 * @brief
 *
 * @param dst
 * @param src
 *
 * @remark
 */
void bi_copy(bigint** dst, bigint* src)
{
    (*dst)->sign = src->sign;
    (*dst)->wordlen = src->wordlen;
    array_copy((*dst)->a, src->a, (*dst)->wordlen);
}

/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_flip_sign(bigint** x)
{
    (*x)->sign = (*x)->sign == NEGATIVE ? NON_NEGATIVE : NEGATIVE;
}

/*********** GET ***********/

/**
 * @brief
 *
 * @param x
 * @return int
 *
 * @remark
 */
int bi_is_one(bigint* x)
{
    if (x->sign == NEGATIVE || x->a[0] != 0x1)
        return 0;

    for (int i = x->wordlen - 1; i > 0; i--)
    {
        if (x->a[i] != 0x0)
            return 0;
    }

    return 1;
}

/**
 * @brief
 *
 * @param x
 * @return int
 *
 * @remark
 */
int bi_is_zero(bigint* x)
{
    if (x->sign == NEGATIVE || x->a[0] != 0x0)
        return 0;

    for (int i = x->wordlen - 1; i >= 0; i--)
    {
        if (x->a[i] != 0x0)
            return 0;
    }

    return 1;
}

/**
 * @brief
 *
 * @param x
 * @return int
 *
 * @remark
 */
int bi_get_word_len(bigint* x)
{
    return x->wordlen;
}

/**
 * @brief
 *
 * @param x
 * @return int
 *
 * @remark
 */
int bi_get_bit_len(bigint* x)
{
    word tmp = 1;
    int i = 0;
    for (int i = Word_Bit_Len - 1; i >= 0; i--)
    {
        tmp = tmp << i;
        if (x->a[x->wordlen - 1] & tmp == 1)
            break;
    }
    return i + (x->wordlen - 1) * Word_Bit_Len;
}

/**
 * @brief
 *
 * @param x
 * @param j
 * @return int
 *
 * @remark
 */
int bi_get_j_bit(bigint* x, int j)
{
    int word_n = 0;  // n-th word (-,-,-,-,-,j-th,-,-)
    int j_bit = 0;
    word n_th_word = 0;
    word_n = (j / Word_Bit_Len); //x->wordlen - 1 - 

    n_th_word = x->a[word_n];
    j_bit = (n_th_word >> ((j % Word_Bit_Len))) & 0x1;

    return j_bit;
}

/**
 * @brief
 *
 * @param x
 * @return int
 *
 * @remark
 */
int bi_get_sign(bigint* x)
{
    return x->sign;
}


/*********** SHOW ***********/

/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_show(bigint* x)
{
    printf("HEX : \n");
    bi_show_hex(x);
    printf("DEC : \n");
    // bi_show_dec(x);
    printf("BIN : \n");
    bi_show_bin(x);
}

/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
void bi_show_hex(bigint* x)
{
    int len = x->wordlen * Word_Bit_Len - 1;
    if (x->sign == NEGATIVE)
    {
        printf("(-)");
    }
    if (Word_Bit_Len == 8)
    {
        for (int i = x->wordlen - 1; i >= 0; i--)
            printf("%02X", x->a[i]);
    }
    else if (Word_Bit_Len == 32)
    {
        for (int i = x->wordlen - 1; i >= 0; i--)
            printf("%08X", x->a[i]);
    }
    else if (Word_Bit_Len == 64)
    {
        for (int i = x->wordlen - 1; i >= 0; i--)
            printf("%016X", x->a[i]);
    }
    printf("\n");
}
/**
 * @brief
 *
 * @param x
 *
 * @remark
 */
 /*
 void bi_show_dec(bigint* x)
 {
     int dec_len = x->wordlen / 4;
     int len = x->wordlen * Word_Bit_Len - 1;
     unsigned int* dec = (unsigned int*)malloc(sizeof(unsigned int) * dec_len);
     for (int i = 0; i < dec_len; i++)
     {
         for (int j = 7; j >= 0; j--)
         {
             dec[i] += bi_get_j_bit(x, j + i * 32) * (1 << j);
         }
     }


     while (len >= 0)
     {
         dec += bi_get_j_bit(x, len) * (1 << len);
         len--;
     }
     if (x->sign == NON_NEGATIVE)
         printf("%u\n", dec);
     else
         printf("-%u\n", dec);
     free(dec);
 }
 */

 /**
  * @brief
  *
  * @param x
  *
  * @remark
  */
void bi_show_bin(bigint* x)  //max len
{
    int len = x->wordlen * Word_Bit_Len - 1;
    if (x->sign == NEGATIVE)
    {
        printf("(-)");
    }
    while (len >= 0)
    {
        printf("%d", bi_get_j_bit(x, len));
        len--;
    }
    printf("\n");
}
