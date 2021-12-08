#ifndef _DATA_H_
#define _DATA_H_
#include "type.h"

/*** data management fuc ***/

/*********** MEMORY ***********/
void bi_new(bigint** x, int wordlen);
void bi_delete(bigint** x);
void bi_resize(bigint** x, int prev_wordlen, int new_wordlen);

/*********** SET ***********/
void bi_set_one(bigint** x);  
void bi_set_zero(bigint** x); 
void bi_set_by_array(bigint** x, int sign, word* a, int worlen);
void str2word(word* dst, char* str, int array_size);
word c2w(char c);
void bi_set_by_string(bigint** x, int sign, char* str); 
void bi_assign(bigint** dst, bigint* src);  
void bi_gen_rand(bigint** x, int sign, int wordlen); 
void array_rand(word* dst, int wordlen); 
void array_copy(word* dst, word* src, int wordlen);  
void array_init(word* a, int word_len);    
void bi_refine(bigint* x);  
void bi_copy(bigint** dst, bigint* src);  
void bi_flip_sign(bigint** x); 

/*********** GET ***********/
int bi_is_one(bigint* x); 
int bi_is_zero(bigint* x); 
int bi_get_word_len(bigint* x); 
int bi_get_bit_len(bigint* x); 
int bi_get_j_bit(bigint* x, int j); 
int bi_get_sign(bigint* x); 

/*********** SHOW ***********/
void bi_show(bigint* x);
void bi_show_hex(bigint* x);
void bi_show_dec(bigint* x); 
void bi_show_bin(bigint* x);

#endif // !_DATA_H_
