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
    if (*x != NULL)
        bi_delete(x);
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
    int a = (int)strlen(str);
    int str_len = (int)strlen(str) % 2 == 1 ? (int)strlen(str) - 1 : (int)strlen(str);
    int array_size = 0;


    /*
    for (int i = 1; i < array_size; i++)
    {
        memcpy(tmp, str + (i * 2), 2);
        tmp[2] = 0;
        (*x)->a[i] = (word)strtoul(tmp, NULL, 16);// strtoul -> str => usigned long
    }



    */
    switch (Word_Bit_Len)
    {
    case 8:
        array_size = str_len / 2;
        bi_new(x, array_size);
        str2word((*x)->a, str, array_size);
        break;
    case 32:
        array_size = str_len / 4;
        bi_new(x, array_size);
        str2word((*x)->a, str, array_size);
        break;
    case 64:
        array_size = str_len / 8;
        bi_new(x, array_size);
        str2word((*x)->a, str, array_size);
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
    if (*dst != NULL)
        bi_delete(dst);
    bi_new(dst, src->wordlen);

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
    switch (Word_Bit_Len)
    {
    case 8:
        while (cnt > 0)
        {
            *p = rand() & 0xff;
            p++;
            cnt--;
        }
        break;
    case 32:
        while (cnt > 0)
        {
            *p = rand() & 0xffffffff;
            p++;
            cnt--;
        }
        break;
    case 64:
        while (cnt > 0)
        {
            *p = rand() & 0xffffffffffffffff;
            p++;
            cnt--;
        }
        break;
    default:
        break;
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
    int i = Word_Bit_Len - 1;
    while (i > 0)
    {
        if (((x->a[x->wordlen - 1] >> i) & 1) == 1)
            break;
        i--;
    }
    return i + 1 + (x->wordlen - 1) * Word_Bit_Len;
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

void str2hex(bigint** hex, unsigned char* str)    // s길이가 길어지면 size 오류 발생. 이것도 W 단위로 끊어서 봐야하나...?
{
    if (*hex != NULL)
        return;

    int size = ((strlen(str) + 1) / 3); // str byte 수 : n = strlen(str)+1 / 3, 할당할 크기 : byte 단위
    word* tmp = malloc(size);   // str을 배열로 저장할 포인터 임시 변수
    int num;    // str을 띄어쓰기를 기준으로 (즉, 바이트씩) 저장할 임시 변수
    unsigned char* end; // 띄어쓰기(NULL) 끝 주소를 저장할 포인터

    /******************************************************************** 역순으로 저장됨
    num = strtol(str, &end, 16);
    tmp[size - 1] = num;

    for (int k = size - 2; k >0; k--)
    {
        num = strtol(end, &end, 16);
        tmp[k] = num;
    }

    num = strtol(end, NULL, 16);
    tmp[0] = num;


    for (int k = 0; k < size; k++)
    {
        printf("%02x", tmp[k]);
    }
    printf("\n");
*/

    num = strtol(str, &end, 16);  // 시작주소 s부터 띄어쓰기까지 16진수로 저장하고 end 주소를 반환
    tmp[0] = num;

    for (int k = 1; k < size - 1; k++)
    {
        num = strtol(end, &end, 16);
        tmp[k] = num;
    }

    num = strtol(end, NULL, 16);
    tmp[size - 1] = num;

    bi_new(hex, size);
    bi_set_by_array(hex, NON_NEGATIVE, tmp, size);

    free(tmp);
    tmp = NULL;
    end = NULL;
}
void str2word(word* dst, char* str, int array_size)
{
    switch (Word_Bit_Len)
    {
    case 8:
        if ((strlen(str)-1) % 2 == 1)
        {
            if (isdigit(str[0]))
                dst[array_size - 1] = (word)(str[0] - '0');
            else
                dst[array_size - 1] = (word)(str[0] - 'a' + 10);
            for (int i = 1; i < array_size; i++)
            {
                if (isdigit(str[2 * i - 1]))
                    dst[array_size - 1 - i] = (word)(str[2 * i - 1] - '0') << 4;
                else
                    dst[array_size - 1 - i] = (word)(str[2 * i - 1] - 'a' + 10) << 4;
                if (isdigit(str[2 * i]))
                    dst[array_size - 1 - i] += (word)(str[2 * i] - '0');
                else
                    dst[array_size - 1 - i] += (word)(str[2 * i] - 'a' +10);
                //dst[array_size - 1 - i] = (word)((str[2 * i - 1] - '0') << 4);
                //dst[array_size - 1 - i] += (word)(str[2 * i] - '0');
            }
        }
        else
        {

            for (int i = 0; i < array_size; i++)
            {
                if (isdigit(str[2 * i]))
                    dst[array_size - 1 - i] = (word)(str[2 * i] - '0') << 4;
                else
                    dst[array_size - 1 - i] = (word)(str[2 * i] - 'a' + 10) << 4;
                if (isdigit(str[2 * i + 1]))
                    dst[array_size - 1 - i] += (word)(str[2 * i + 1] - '0');
                else
                    dst[array_size - 1 - i] += (word)(str[2 * i + 1] - 'a' + 10);
                //dst[array_size - 1 - i] = (word)((str[2 * i] - '0') * 4);
                //dst[array_size - 1 - i] += (word)(str[2 * i + 1] - '0');
            }
        }
        break;
    case 32:

        if ((strlen(str) - 1) % 4 == 1)
        {
            dst[array_size - 1] = (word)(str[0] - '0');
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[4 * i - 1] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[4 * i] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[4 * i + 1] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[4 * i + 2] - '0'));
            }
        }
        else if ((strlen(str) - 1) % 4 == 2)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 4);
            dst[array_size - 1] += (word)((str[1] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[4 * i - 2] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[4 * i - 1] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[4 * i] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[4 * i + 1] - '0'));
            }
        }
        else if ((strlen(str) - 1) % 4 == 3)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 8);
            dst[array_size - 1] += (word)((str[1] - '0') << 4);
            dst[array_size - 1] += (word)((str[2] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[4 * i - 3] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[4 * i - 2] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[4 * i - 1] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[4 * i] - '0'));
            }
        }
        else
        {
            for (int i = 0; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[4 * i] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[4 * i + 1] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[4 * i + 2] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[4 * i + 3] - '0'));
            }
        }
        break;
    case 64:
        //bi_new(x, array_size);
        if (strlen(str) % 8 == 1)
        {
            dst[array_size - 1] = (word)(str[0] - '0');
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 1] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i + 3] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i + 4] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 5] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 6] - '0'));
            }
        }
        else if (strlen(str) % 8 == 2)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 4);
            dst[array_size - 1] += (word)((str[1] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 2] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i + 3] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 4] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 5] - '0'));
            }
        }
        else if (strlen(str) % 8 == 3)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 8);
            dst[array_size - 1] += (word)((str[1] - '0') << 4);
            dst[array_size - 1] += (word)((str[2] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 3] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 2] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 3] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 4] - '0'));
            }
        }
        else if (strlen(str) % 8 == 4)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 12);
            dst[array_size - 1] += (word)((str[1] - '0') << 8);
            dst[array_size - 1] += (word)((str[2] - '0') << 4);
            dst[array_size - 1] += (word)((str[3] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 4] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 3] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i - 2] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 3] - '0'));
            }
        }
        else if (strlen(str) % 8 == 5)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 16);
            dst[array_size - 1] += (word)((str[1] - '0') << 12);
            dst[array_size - 1] += (word)((str[2] - '0') << 8);
            dst[array_size - 1] += (word)((str[3] - '0') << 4);
            dst[array_size - 1] += (word)((str[4] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 5] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 4] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i - 3] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i - 2] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0'));
            }
        }
        else if (strlen(str) % 4 == 6)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 20);
            dst[array_size - 1] += (word)((str[1] - '0') << 16);
            dst[array_size - 1] += (word)((str[2] - '0') << 12);
            dst[array_size - 1] += (word)((str[3] - '0') << 8);
            dst[array_size - 1] += (word)((str[4] - '0') << 4);
            dst[array_size - 1] += (word)((str[5] - '0'));

            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 6] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 5] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i - 4] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i - 3] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i - 2] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0'));
            }
        }
        else if (strlen(str) % 4 == 7)
        {
            dst[array_size - 1] = (word)((str[0] - '0') << 24);
            dst[array_size - 1] += (word)((str[1] - '0') << 20);
            dst[array_size - 1] += (word)((str[2] - '0') << 16);
            dst[array_size - 1] += (word)((str[3] - '0') << 12);
            dst[array_size - 1] += (word)((str[4] - '0') << 8);
            dst[array_size - 1] += (word)((str[5] - '0') << 4);
            dst[array_size - 1] += (word)((str[6] - '0'));
            for (int i = 1; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i - 7] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i - 6] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i - 5] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i - 4] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i - 3] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i - 2] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i - 1] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i] - '0'));
            }
        }
        else
        {
            for (int i = 0; i < array_size; i++)
            {
                dst[array_size - 1 - i] = (word)((str[8 * i] - '0') << 28);
                dst[array_size - 1 - i] += (word)((str[8 * i + 1] - '0') << 24);
                dst[array_size - 1 - i] += (word)((str[8 * i + 2] - '0') << 20);
                dst[array_size - 1 - i] += (word)((str[8 * i + 3] - '0') << 16);
                dst[array_size - 1 - i] += (word)((str[8 * i + 4] - '0') << 12);
                dst[array_size - 1 - i] += (word)((str[8 * i + 5] - '0') << 8);
                dst[array_size - 1 - i] += (word)((str[8 * i + 6] - '0') << 4);
                dst[array_size - 1 - i] += (word)((str[8 * i + 7] - '0'));
            }
        }
        break;

    default:
        break;
    }
}
