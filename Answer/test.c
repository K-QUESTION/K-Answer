#include "operation.h"

void test_(int test_op)
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* ADD_C = NULL;
	bigint* SUB_C = NULL;
	bigint* MUL_C = NULL;
	bigint* MUL_K = NULL;
	bigint* DIV_Q = NULL;
	bigint* DIV_R = NULL;
	bigint* SQU_A = NULL;
	bigint* SQU_B = NULL;

	bi_new(&A, 2);
	bi_new(&B, 1);
	//word w1[2] = { 0x82, 0xE2 };
	//word w2[2] = { 0x7B, 0xE8 };
	word w1[2] = { 0xE2, 0x82 };
	word w2[1] = { 0x88 };
	//	char* s = "0x1233";
		/*
		switch (test_op)
		{
		case 1:
			break;

		default:
			break;
		}*/
	bi_gen_rand(&A, NON_NEGATIVE, 2);
	//bi_set_by_array(&A, NON_NEGATIVE, w1, 2);
	//bi_set_one(&A);
	//bi_flip_sign(&A);
	bi_show(A);
	//getchar();
	Sleep(1000);

	//bi_set_one(&B);
	//bi_set_by_array(&B, NON_NEGATIVE, w2, 1);
	bi_gen_rand(&B, NON_NEGATIVE, 1);
	bi_show(B);
	//ADD(&ADD_C, A, B);
	//SUB(&SUB_C, A, B);
	//MUL(&MUL_C, A, B);

	//MULC_K(&MUL_K, A, B);

	//Long_DIV(&DIV_Q, &DIV_R, A, B);
	SQU(&SQU_A, A);
	SQUC_K(&SQU_B, B);

	//printf("ADD\n");
	//bi_show(ADD_C);
	//printf("SUB\n");
	//bi_show(SUB_C);
	//printf("MUL\n");
	//bi_show(MUL_C);
	//printf("Karatuba\n");
	//bi_show(MUL_K);
	//printf("DIV_Q\n");
	//bi_show(DIV_Q);
	//printf("DIV_R\n");
	//bi_show(DIV_R);

	printf("SQU\n");
	bi_show(SQU_A);
	printf("SQU_K\n");
	bi_show(SQU_B);


	//bi_show(B);

	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&ADD_C);
	bi_delete(&SUB_C);
	bi_delete(&MUL_C);
	bi_delete(&MUL_K);

}