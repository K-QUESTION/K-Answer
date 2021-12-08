#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "type.h"
#include "data.h"


/*** computational fuc ***/

/*********** GENERAL ***********/

int CompareABS(bigint* A, bigint* B);
int Compare(bigint* A, bigint* B);

void bi_L_shift(bigint** y, bigint* x, int nbits);
void bi_R_shift(bigint** y, bigint* x, int nbits);

void bi_reduct(bigint** dst, bigint* src, int nbits);

void ABS(bigint** y, bigint* x);



/*********** ADDITION ************/
void ADD_AB(bigint** C, bigint* A, bigint* B); 
int ADD_ABc(bigint** C, bigint* A, bigint* B, int c, int idx); 
void ADDC(bigint** C, bigint* A, bigint* B); 
void ADD(bigint** C, bigint* A, bigint* B);
void ADD_CA(bigint** C, bigint* A);

/********** SUBTRACTION ***********/
void SUB_AB(bigint** C, bigint* A, bigint* B);
int SUB_AbB(bigint** C, bigint* A, bigint* B, unsigned int b, int idx);
void SUBC(bigint** C, bigint* A, bigint* B);
void SUB(bigint** C, bigint* A, bigint* B);
void SUB_CA(bigint** C, bigint* A);

/********** MULTIPLICATION ***********/
void MUL_AB(bigint** C, bigint* A, bigint* B, int idxA, int idxB);
void MUL(bigint** C, bigint* A, bigint* B);
void MULC(bigint** C, bigint* A, bigint* B);
void MULC_K(bigint** C, bigint* A, bigint* B);

/********** DIVISION ***********/
void Binary_Long_DIV(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B);
void Long_DIV(bigint** Q, bigint* A, bigint* B);

/********** SQUARING ***********/
void AA(bigint** C, bigint* A);
void SQUC(bigint** C, bigint* A);
void SQUC_K(bigint** C, bigint* A);
void SQU(bigint** C, bigint* A);

/********** MOD_EXP ***********/
void L2R(bigint** B, bigint* A, bigint* n, bigint* M); //Left to Right
void R2L(bigint** B, bigint* A, bigint* n, bigint* M); //Right to Left
void Montgomery(bigint** B, bigint* A, bigint* n, bigint* M);


#endif  //  !_OPERATION_H_