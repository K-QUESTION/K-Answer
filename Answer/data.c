/**
 * @file data.c
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (kth0305a@kookmin.ac.kr)
 * @brief  File that performs functions for managing memory and data, such as setting and outputting bigint structure values.
 * @version 1.0.0
 * @date 2021-12-08
 *
 * @copyright Copyright Question 2021
 *
 */
#include "data.h"
#include "operation.h"


/*********** MEMORY ***********/

 /**
 * @brief Allocates the Memory of a Bigint structure
 *
 * @param x Bigint structure to allocate memory [Double pointer Type]
 * @param wordlen Length of the Structure Array [Integer Type]
 *
 * @details After allocating [using 'malloc'] the memory of the bigint structure x input by the double pointer 
 * when memory is allocated, store the sign (nonnegative integer), word length, and allocate [using 'calloc'] word array memory to store the big integer values.
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
    }

}

/**
 * @brief  Release the memory of the structure.
 *
 * @param x Bigint structure to release memory [Double pointer Type]
 *
 * @details If the memory-allocated bigint structure x is empty, it is not released.
 * Otherwise, the arrangement is initialized, released, and the structure is released.
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
 * @brief Reallocating memory according to the length of the word of the structure.
 *
 * @param x Bigint structure to reallocate memory [Double pointer Type]
 * @param prev_wordlen Length of the word before the change. [Integer Type]
 * @param new_wordlen Length of the word after the change. [Integer Type]
 *
 * @details If the structure is empty before the change, space is allocated and returned by the length of the word after the change.
 * If the length of the word before the change in the structure is longer than the length of the word after the change, 
 * the memory is reallocated by the length after the change and the word length value of the structure is changed.
 * If the length of the word before and after the change is shorter than the length of the word after the change, 
  * the memory is reallocated by the length of the word after the change, the length of the word is saved to the value of the structure, and the remaining space is initialized to 0.
 */

void bi_resize(bigint** x, int prev_wordlen, int new_wordlen)
{

    //word* tmp_a = NULL;
    if ((*x) == NULL)
    {
        bi_new(x, new_wordlen);
        return;
    }
    if ((*x)->a == NULL)
    {
        (*x)->a = (word*)malloc(sizeof(word) * new_wordlen);
        return;
    }

    if (new_wordlen == 0)
    {
        
        (*x)->a = (word*)realloc((*x)->a, sizeof(word) * 1);
        if ((*x)->a == NULL)
        {
            return;
        }
        /*
        tmp_a = (word*)realloc((*x)->a, sizeof(word) * 1);
        if (tmp_a == NULL)
        {
            return;
        }
        (*x)->a = tmp_a;*/
        (*x)->wordlen = 1;
        (*x)->a[0] = 0x00;

    }
    else
    {
        
        (*x)->a = (word*)realloc((*x)->a, sizeof(word) * new_wordlen);
        if ((*x)->a == NULL)
        {
            return;
        }
       /*
        tmp_a = (word*)realloc((*x)->a, sizeof(word) * new_wordlen);
        if (tmp_a == NULL)
        {
            return;
        }
        (*x)->a = tmp_a; */
        (*x)->wordlen = new_wordlen;
        if (prev_wordlen < new_wordlen)
        {
            for (int i = prev_wordlen; i < new_wordlen; i++)
            {
                (*x)->a[i] = 0;
            }
        }

    }

}

/*********** SET ***********/

/**
 * @brief Generate a structure in which the stored value is 1.
 *
 * @param x Bigint structure to store value '1' [Double pointer Type]
 *
 * @details The sign is non-negative integer, value stored in an array is 1, and a structure having a length of 1 in the structure array is generated
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
 * @brief Generate a structure in which the stored value is 0.
 *
 * @param x Bigint structure to store value '0' [Double pointer Type]
 *
 * @details The sign is non-negative integer, value stored in an array is 0, and a structure having a length of 1 in the structure array is generated.
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
 * @brief Allocate an array of signs and hexadecimal numbers specified in x.
 *
 * @param x bigint structure [Double pointer Type]
 * @param sign sign of bigint structure [Integer Type]
 * @param a Hex array to be stored in the structure array [word pointer Type]
 * @param wordlen Length of the bigint structure word [Interger Type]
 *
 * @throws ValidException Sign != negative integer or Sign != nonnegative integer or word pointer value [a] is empty or less than 0 or  array length is less than 0 
 * @details Create a structure that allocates the signs and arrangements specified in x.
  * Sign  ->  sign value, word length of x -> wordlen, copies the value a to the array of structures.
 */
void bi_set_by_array(bigint** x, int sign, word* a, int wordlen)
{
    if (((sign != NEGATIVE) && (sign != NON_NEGATIVE)) || (a == NULL) || a < 0 || wordlen < 0)
        return;
    if((*x) == NULL)
        bi_new(x, wordlen);
    (*x)->sign = sign;
    (*x)->wordlen = wordlen;
    array_copy((*x)->a, a, wordlen);

}

/**
 * @brief A function that converts the data type of a string into words.
 *
 * @param c String to convert data types [charater type variable]
 *
 * @details If the string 'c' is a number, return the value subtracted by the ASCII value "0".
 * If the string is not a number, return the value subtracted by the ASCII value "a" and added by th ASCII value "10".
 */
word c2w(char c)
{
    if ('0' <= (c) && c <= '9')
    {
        return ((word)(c)-(word)('0'));
    }
    else
        return ((word)(c)-(word)('a') + (word)(10));
}

/**
 * @brief A function that converts a string into a word array.
 *
 * @param dst Array to store the converted value [Structure pointer variable]
 * @param str String to be converted [Structure pointer variable]
 * @param array_size The size of the array to be assigned according to the string length [integer type variable]
 *
 * @details Define hex_bits as Word_Bit_Len divided by 4, 4 is the size of the char type.
 * If the size of the string is not divided by hex_bits,
 * store the first remaining values of str in the top word of the array,
 * and store the divided parts in order from the remaining array.
 * If the size of the string is divided by hex_bits,
 * values of str are stored in order from the top word of the array.
 */
void str2word(word* dst, char* str, int array_size)
{
    int hex_bits = Word_Bit_Len / 4;
    int remain = (strlen(str) - 1) % hex_bits;
    if (remain != 0)
    {
        for (int i = 0; i < remain; i++)
        {
            dst[array_size - 1] += c2w(str[i]) << 4 * (remain - 1 - i);
        }
        for (int i = 1; i < array_size; i++)
        {
            for (int j = 0; j < hex_bits; j++)
            {
                dst[array_size - 1 - i] += c2w(str[hex_bits * i - (hex_bits - remain) + j]) << 4 * (hex_bits - j - 1);
            }
        }
    }
    else
    {
        for (int i = 0; i < array_size; i++)
        {
            for (int j = 0; j < hex_bits; j++)
            {
                dst[array_size - 1 - i] += c2w(str[hex_bits * i + j]) << 4 * (hex_bits - j - 1);
            }
        }
    }
}

/**
 * @brief A function that allocates an array of hexadecimal strings to x and a given sign.
 *
 * @param x bigint structure [structure double pointer variable]
 * @param sign Sign value of the structure [integer variable]
 * @param str Hexadecimal letter type variable.
 *
 * @details If the sign is not 0(non-negative integer) or 1(negative integer), or the address value of str is empty or negative, it is returned without structure setting.
 * First, divide strlen into bytes and obtain the array size. Then declare an array to store the values temporarily.
 * And the array values are stored using the str2word function.
 * The structure x is set to the specified sign and array_size.
 */
void bi_set_by_string(bigint** x, int sign, char* str)  //, int base)  //string : hex string, base = 16
{
    if (((sign != NEGATIVE) && (sign != NON_NEGATIVE)) || (str == NULL) || str < 0)//||(base != 2 && base != 10 && base != 16 ))
        return;
    int a = (int)strlen(str) - 1;
    int r = 0;
    int str_len = 0;
    int array_size = 0;
    
    switch (Word_Bit_Len)
    {
    case 8:
        str_len = a % 2 == 1 ? a + 1 : a;
        array_size = str_len / 2;
        bi_new(x, array_size);
        str2word((*x)->a, str, array_size);
        break;
    case 32:
        r = (8 - a % 8) % 8;
        str_len = a + r;
        array_size = str_len / 8;
        bi_new(x, array_size);
        str2word((*x)->a, str, array_size);
        break;
    case 64:
        r = r = (16 - a % 16) % 16;
        str_len = a + r;
        array_size = str_len / 16;
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
 * @brief A function that copies and stores the value of src in dst.
 *
 * @param dst Bigint structure to be copied. [Structure double pointer variable]
 * @param src Bigint structure to copy [structure double pointer variable]
 *
 * @details If the dst structure is not empty, dst is initialized to store the value.
 * Then, the structure is generated in dst according to the size of src,
 * the sign of dst is set as the src sign, and the array value of src is copied to the array value of dst.
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
 * @brief A function that creates a structure with random array values.
 *
 * @param x A bigint structure with random array values [structure double pointer variable]
 * @param sign Sign of the structure [integer type variable]
 * @param wordlen The length of the structure arrangement. [integer type variable]
 *
 * @details Create a structure with an array size wordlen in x.
 * Set the sign of x to sign and assign a random value to the array value.
 * If the top word value is 0, remove it.
 */
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

/**
 * @brief A function that allocates a random value according to the word bit length.
 *
 * @param dst Address to store random array values [word pointer type variable]
 * @param wordlen The length of the structure array [integer type variable]
 *
 * @details We use the srand function so that the generated random numbers are different.
 * The value of the memory location indicated by the byte pointer p is stored in the memory location indicated by the dst.
 * And declare cnt (length of array * word size).
 * If cnt is greater than 0, calculate the random number function and 0xff as & and store it in pointer p.
 * After that, p is added by 1, and cnt is subtracted by 1.
 */
void array_rand(word* dst, int wordlen)
{
    byte* p = (byte*)dst;
    int cnt = wordlen * sizeof(word);

    while (cnt > 0)
    {
        *p = rand() & 0xff;
        p++;
        cnt--;
    }
}

/**
 * @brief A function that copies an array.
 *
 * @param dst The array to be copied. [Word pointer type variable]
 * @param src The array to copy. [Word pointer type variable]
 * @param word_len Word length [integer type variable]
 *
 * @details Copy the array src to the array dst up to the length of the word.
 */
void array_copy(word* dst, word* src, int word_len)  // dst -> destination, src -> source
{
    for (int i = 0; i < word_len; i++)
    {
        dst[i] = src[i];
    }
}


/**
 * @brief A function that initializes an array.
 *
 * @param a Address that stores the array to be initialized [word pointer type variable]
 * @param word_len Word length [integer type variable]
 *
 * @details Using memset function, the word pointer type variable a is initialized to 0 by (length of the word)*(size of the word).
 */
void array_init(word* a, int word_len)
{
    memset(a, 0, word_len * sizeof(word));
}

/**
 * @brief A function that summarizes the length of the array length.
 *
 * @param x A structure that needs to be arranged. [Structure pointer variable]
 *
 * @details If the structure x is empty, it will not be executed.
 * Declare the variable new wordlen to find the length to be reduced.
 * If the new wordlen is greater than 1, a repetition statement is executed until the value stored in the top word is not 0.
 * If the stored value is 0, shorten the length of the new wordlen by one.
 * Find a new wordlen, and adjust the size of x to new wordlen.
 */
void bi_refine(bigint* x)
{
    if (x == NULL)
    {
        return;
    }
    //word* tmp_a = NULL;
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
        
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
        if(x->a == NULL)
        {
            return;
        }
       /*
        tmp_a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
        if (tmp_a == NULL)
        {
            return;
        }
        x->a = tmp_a;*/
        x->wordlen = new_wordlen;
    }
    if ((x->wordlen == 1) && (x->a[0] == 0x0))
    {
        x->sign = NON_NEGATIVE;
    }
    
}

/**
 * @brief A function that copies a structure.
 *
 * @param dst Structure to be copied from the assigned value. [Structure double pointer variable]
 * @param src A structure that will copy the assigned value. [Structure double pointer variable]
 *
 * @details  The signal value of src is assigned as the signal value of dst,
 * the wordlen of src is assigned as the wordlen of dst,
 * and the array value of src is copied and stored as the array value of dst.
 */
void bi_copy(bigint** dst, bigint* src)
{
    (*dst)->sign = src->sign;
    (*dst)->wordlen = src->wordlen;
    array_copy((*dst)->a, src->a, (*dst)->wordlen);
}

/**
 * @brief A function that changes the sign.
 *
 * @param x Structure to change the sign. [Structure double pointer variable.]
 *
 * @details  If the sign of x is negative, change the sign to a nonnegative number.
 * If the sign of x is not negative, change it to negative.
 */
void bi_flip_sign(bigint** x)
{
    (*x)->sign = (*x)->sign == NEGATIVE ? NON_NEGATIVE : NEGATIVE;
}

/*********** GET ***********/
/**
 * @brief A function that checks whether the value stored in the array is 1.
 *
 * @param x A structure to check whether the value stored in the array is 1. [Structure pointer variable]
 * @return If the value of the int array is 1, true is returned. if it is not 1, false is returned.
 *
 * @details If the sign is negative or the value stored in [0] is not 1, 0 is returned.
 * If the value stored in an array other than [0] is not 0, return 0 because the total value stored in the array cannot be one.
 * When a value stored in an array other than [0] is 0, 1(true) is returned because the value stored in the array is one(Structure value).
 */
int bi_is_one(bigint* x)
{
    if (x->sign == NEGATIVE || x->a[0] != 0x1)   // if Sign(A) = 1 or A[0] != 1 then
        return FALSE;

    for (int i = x->wordlen - 1; i > 0; i--)     // for i = n - 1 downto 1 do
    {
        if (x->a[i] != 0x0) // if Ai != 0 then
            return FALSE;
    }

    return TRUE;
}

/**
 * @brief A function that checks whether the value stored in the array is 0.
 *
 * @param x A structure to check whether the value stored in the array is 0. [Structure pointer variable]
 * @return int If the value of the array is 0, it returns true. If it is not 0, return false.
 *
 * @details If the sign is negative or the value stored in [0] is not 0(Structure value), 0(false) is returned.
 * If the value stored in an array other than [0] is not 0, return 0(false) because the total value stored in the array cannot be zero(Structure value).
 * When a value stored in an array other than [0] is 0, 1 is returned because the value stored in the array is zero.
 */
int bi_is_zero(bigint* x)
{
    if (x->sign == NEGATIVE || x->a[0] != 0x0)  // if Sign(A) = 1 or A[0] != 0 then
        return FALSE;

    for (int i = x->wordlen - 1; i >= 0; i--)   // for i = n - 1 downto 1 do
    {
        if (x->a[i] != 0x0)  // if Ai != 0 then
            return FALSE;
    }

    return TRUE;
}

/**
 * @brief A function to check the word length.
 *
 * @param x Structure to check wordlen (word length) [Structure pointer variable]
 * @return int Returns the wordlen of structure x.
 *
 * @details Returns the wordlen of structure x.

 */
int bi_get_word_len(bigint* x)
{
    return x->wordlen;
}

 /**
  * @brief A function to check the bit length.
  *
  * @param x Structure to check bitlen. [Structure pointer variable]
  * @return int Return the bit length of the structure x is calculated.
  *
  * @details Find the most significant bit and calculate the bit length of Big Integer.
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
 * @brief A function to check the j-th bit value.
 *
 * @param x Structure to check the j-th bit value. [Structure pointer variable]
 * @param j Bit position to check j [integer type variable]
 * @return int Return the j-th beat.
 *
 * @details Declare the n-th word to be checked as the integer variable word_n and the j-th bit as the integer variable j_bit and then initialize them as 0.
 * Declare a temporary variable n_th_word to store one word in the array and then initialize it to 0.
 * A value obtained by dividing j by word_Bit_Len is stored in word_n. (Word_Bit_Len is the length of the word to which the j-th bit belongs)
 * Copy the word to which the j-th bit belongs to n_th_word.
 * In j_bit, shift the copied word to leave only one jth bit.
 * Then return the j-th bit.
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
 * @brief A function to check the sign.
 *
 * @param x Structure to check the sign. [Structure pointer variable]
 * @return int Returns the sign of x.
 *
 * @details
 */
int bi_get_sign(bigint* x)
{
    return x->sign;
}

/*********** SHOW ***********/
/**
 * @brief A function that outputs a structure value.
 *
 * @param x The structure to output the value. [Structure pointer variable]
 *
 * @details Print the value of x in hexadecimal.
 * Print the value of x as a decimal.
 * Print the value of x in binary.
 */
void bi_show(bigint* x)
{
    printf("HEX : ");
    bi_show_hex(x);
    printf("DEC : \n");
    bi_show_dec(x);
    printf("BIN : \n");
    bi_show_bin(x);
}

/**
 * @brief A function that outputs in hexadecimal numbers.
 *
 * @param x Structure to be printed in hexadecimal numbers. [Structure pointer variable]
 *
 * @details First, if the code is negative, the code '-'(minus) is output first.
 * Then, if the bit length of the word is 8, it is output in two digits from the top bit using a repetition statement.
 * If the bit length of the word is 32, it is output in units of 8 digits from the highest bit using a repetition statement.
 * If the bit length of the word is 64, it is output in units of 16 digits from the highest bit using a repetition statement.
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
        printf("%x", (unsigned char)x->a[x->wordlen - 1]);
        for (int i = x->wordlen - 2; i >= 0; i--)
            printf("%02x", (unsigned char)x->a[i]);
    }
    else if (Word_Bit_Len == 32)
    {
        printf("%x", (unsigned int)x->a[x->wordlen - 1]);
        for (int i = x->wordlen - 2; i >= 0; i--)
            printf("%08x", (unsigned int)x->a[i]);
    }
    else if (Word_Bit_Len == 64)
    {
        printf("%llx", (unsigned long long)x->a[x->wordlen - 1]);
        for (int i = x->wordlen - 2; i >= 0; i--)
            printf("%016llx", (unsigned long long)x->a[i]);
    }
    printf("\n");
}

/**
* @brief A function that outputs in decimal numbers.
*
* @param x Structure to be printed in decimal numbers. [Structure pointer variable]
*
* @details The remainder of the bigint structure x divided by the bigint structure 10 is stored in a decimal arrangement and
* continues until the quotient is less than 10. Output the decimal array dec.
*/
void bi_show_dec(bigint* x)
{
     int dec[3000];
     bigint* ten = NULL;
     bigint* q = NULL;
     bigint* r = NULL;
     bigint* dec_temp = NULL;

     bi_assign(&dec_temp, x);

     bi_new(&ten, 1);
     ten->a[0] = 10;
     int i = 0;
     while (1)                               //10씩 나눠가면서 나머지는 출력할수, 몫은 다시나누기
     {
         DIV(&q, &r, dec_temp, ten);
         dec[i] = (int)(r->a[0]);
         bi_assign(&dec_temp, q);
         i++;
         if (Compare(dec_temp, ten) == BIGGER_SECOND_ARGUMENT)   // 해당과정은 맨앞자리만 남을때까지
         {
             dec[i] = (int)(dec_temp->a[0]);
             break;
         }

     }
     bi_delete(&q);
     bi_delete(&r);
     bi_delete(&ten);
     bi_delete(&dec_temp);
     for (int j = i; j >= 0; j--)
         printf("%d", dec[j]);
  
     printf("\n");
 }


/**
* @brief A function that outputs in binary.
*
* @param x A structure to be output in binary. [Structure pointer variable]
*
* @details Declares a len that stores (length of the word)*(length of the word)-1 in x. This represents the highest bit of the structure array.
* Then, if the sign is negative, the sign '-'(minus) is output first.
* Then, using the iterative statement, print the values from the top bit to the bottom bit one by one if len is greater than or equal to zero.
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