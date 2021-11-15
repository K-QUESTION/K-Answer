#ifndef _DATA_H_
#define _DATA_H_
#include "type.h"

/*** data management fuc ***/

/*********** MEMORY ***********/
void bi_new(bigint** x, int wordlen);
void bi_delete(bigint** x);
void bi_resize(bigint** x, int prev_wordlen, int new_wordlen);

/*********** SET ***********/
void bi_set_one(bigint** x);  //ok
void bi_set_zero(bigint** x);  //ok
void bi_set_by_array(bigint** x, int sign, word* a, int worlen);  //array 저장 순서 의문
void bi_set_by_string(bigint** x, int sign, char* str);  // 저장 순서 수정하기
void bi_assign(bigint** dst, bigint* src);  // ok
void bi_gen_rand(bigint** x, int sign, int wordlen);  //ok
void array_rand(word* dst, int wordlen);  //ok
void array_copy(word* dst, word* src, int wordlen);  //ok
void array_init(word* a, int word_len);    //ok
void bi_refine(bigint* x);  //ok
void bi_copy(bigint** dst, bigint* src);  //ok
void bi_flip_sign(bigint** x);  //ok

/*********** GET ***********/
int bi_is_one(bigint* x);  //ok
int bi_is_zero(bigint* x);  //ok
int bi_get_word_len(bigint* x);  //ok
int bi_get_bit_len(bigint* x);  //수정
int bi_get_j_bit(bigint* x, int j);  //ok
int bi_get_sign(bigint* x);  //ok

/*********** SHOW ***********/
void bi_show(bigint* x);
void bi_show_hex(bigint* x);
void bi_show_dec(bigint* x);  //unsinged int 범위를 넘어가면 계산이 제대로 안됨...
void bi_show_bin(bigint* x);

#endif // !_DATA_H_
