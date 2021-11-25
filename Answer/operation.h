#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "type.h"
#include "data.h"


/*** computational fuc ***/

/*********** GENERAL ***********/

int CompareABS(bigint* A, bigint* B);  // ok 
int Compare(bigint* A, bigint* B);   //ok

void bi_L_shift(bigint** y, bigint* x, int nbits);  //ok
void bi_R_shift(bigint** y, bigint* x, int nbits);  //ok

void bi_reduct(bigint** dst, bigint* src, int nbits);  //ok

void ABS(bigint** y, bigint* x);  //ok



/*********** ADDITION ************/
void ADD_AB(bigint** C, bigint* A, bigint* B); // C: result
int ADD_ABc(bigint** C, bigint* A, bigint* B, int c, int idx); // int c -> {0, 1}
void ADDC(bigint** C, bigint* A, bigint* B); // where WordLen(A) �� WordLen(B) and Sign(A) = Sign(B)
void ADD(bigint** C, bigint* A, bigint* B);

/********** SUBTRACTION ***********/
void SUB_AB(bigint** C, bigint* A, bigint* B);
int SUB_AbB(bigint** C, bigint* A, bigint* B, int b, int idx); // int b -> {0, 1}, idx -> A, B�� a  �ε���
void SUBC(bigint** C, bigint* A, bigint* B); // where WordLen(A) �� WordLen(B) and Sign(A) = Sign(B)
void SUB(bigint** C, bigint* A, bigint* B);

/********** MULTIPLICATION ***********/
void MUL_AB(bigint** C, bigint* A, bigint* B, int idxA, int idxB);  // A, B: 1 word 
void MUL(bigint** C, bigint* A, bigint* B);
void MULC(bigint** C, bigint* A, bigint* B);  //Schoolbook
void MULC_K(bigint** C, bigint* A, bigint* B); //Karatsuba �̸� �ٲ�?

/********** DIVISION ***********/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B);
void Long_DIV(bigint** Q, bigint* A, bigint* B); //void Long_Division

/********** SQUARING ***********/
void AA(bigint** C, bigint* A);
void SQUC(bigint** C, bigint* A);
void SQUC_K(bigint** C, bigint* A);
void SQU(bigint** C, bigint* A);

/********** MOD_EXP ***********/
void L2R(bigint** B, bigint* A, bigint* n, bigint* M); //Left to Right
void R2L(bigint** B, bigint* A, bigint* n, bigint* M); //Right to Left
void Montgomery();


#endif  //  !_OPERATION_H_
