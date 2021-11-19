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
void bi_hex_fprint(FILE* fp, bigint* x)
{
	int len = x->wordlen * Word_Bit_Len - 1;
	if (x->sign == NEGATIVE)
	{
		fprintf(fp,"-0x");
	}
	else
		fprintf(fp, "0x");
	if (Word_Bit_Len == 8)
	{
		fprintf(fp, "%x", x->a[x->wordlen - 1]);
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp,"%02x", x->a[i]);
	}
	else if (Word_Bit_Len == 32)
	{
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp,"%08x", x->a[i]);
	}
	else if (Word_Bit_Len == 64)
	{
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp, "%016x", x->a[i]);
	}
	fprintf(fp, "\n");
}
void test__()
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

	FILE* A_file = NULL;
	FILE* B_file = NULL;
	FILE* Add_result = NULL;
	FILE* Sub_result = NULL;
	FILE* Mul_result = NULL;
	FILE* Mul_K_result = NULL;
	//FILE* Div_Q_result = NULL;
	//FILE* Div_R_result = NULL;
	FILE* Squ_A_result = NULL;
	FILE* Squ_B_result = NULL;
	//FILE* Div_result = NULL;

	char A_buf[128] = { 0x00, };
	char B_buf[128] = { 0x00, };
	int cnt = 1;
	char* A_eof = NULL;
	char* B_eof = NULL;

	A_file = fopen("A.txt", "r");
	B_file = fopen("B.txt", "r");
	Add_result = fopen("Add.txt", "a+");
	Sub_result = fopen("Sub.txt", "a+");
	Mul_result = fopen("Mul.txt", "a+");
	Mul_K_result = fopen("Mul_K.txt", "a+");
	//Div_Q_result = fopen("Div_Q.txt", "a+");
	//Div_R_result = fopen("Div_R.txt", "a+");
	Squ_A_result = fopen("Squ_A.txt", "a+");
	Squ_B_result = fopen("Squ_B.txt", "a+");
	/*
	//32 bit
	Add_result = fopen("Add_32.txt", "a+");
	Sub_result = fopen("Sub_32.txt", "a+");
	Mul_result = fopen("Mul_32.txt", "a+");
	Mul_K_result = fopen("Mul_K.txt", "a+");
	Div_Q_result = fopen("Div_Q.txt", "a+");
	Div_R_result = fopen("Div_R.txt", "a+");
	Squ_A_result = fopen("Squ_A_32.txt", "a+");
	Squ_B_result = fopen("Squ_B_32.txt", "a+");

	*/

	//fopen(Div_result, "a+");
	while (1)
	{
		//Div_Q_result = fopen("Div_Q.txt", "a+");
		//Div_R_result = fopen("Div_R.txt", "a+");
		A_eof = fgets(A_buf, 128, A_file);
		B_eof = fgets(B_buf, 128, B_file);

		if (A_eof == NULL || B_eof == NULL) // 마지막 줄
		{
			printf("End\n");
			break;
		}

		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);  // 만약 0x형식으로 들어가면 &A_buf[3]
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);  // 만약 0x형식으로 들어가면 &A_buf[3]
		}
		if (B_buf[0] == '-')
		{
			bi_set_by_string(&B, NEGATIVE, &B_buf[3]);  // 만약 0x형식으로 들어가면 &B_buf[3]
		}
		else
		{
			bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);  // 만약 0x형식으로 들어가면 &B_buf[3]
		}

		/*****ADD*****/
		ADD(&ADD_C, A, B);
		bi_hex_fprint(Add_result, ADD_C);
		/*****SUB*****/
		//SUB(&SUB_C, A, B);
		//bi_hex_fprint(Sub_result, SUB_C);
		///*****MUL*****/
		//MUL(&MUL_C, A, B);
		//bi_hex_fprint(Mul_result, MUL_C);
		///*****MUL_kara*****/
		//MUL(&MUL_K, A, B);
		//MUL_K->sign = A->sign ^ B->sign;
		//bi_hex_fprint(Mul_K_result, MUL_K);
		/*****DIV*****/
		//if (A->sign == NON_NEGATIVE && B->sign == NON_NEGATIVE)
		//{
		//	fprintf(Div_Q_result, "%d", cnt);
		//	fprintf(Div_R_result, "%d", cnt);
		//	DIV(&DIV_Q, &DIV_R, A, B);
		//	bi_hex_fprint(Div_Q_result, DIV_Q);
		//	bi_hex_fprint(Div_R_result, DIV_R);
		//}
		/*****SQU*****/
		//SQU(&SQU_A, A);
		//bi_hex_fprint(Squ_A_result, SQU_A);
		//SQU(&SQU_B, B);
		//bi_hex_fprint(Squ_B_result, SQU_B);


		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&ADD_C);
		//bi_delete(&SUB_C);
		//bi_delete(&MUL_C);
		//bi_delete(&MUL_K);
		//bi_delete(&DIV_Q);
		//bi_delete(&DIV_R);
		//bi_delete(&SQU_A);
		//bi_delete(&SQU_B);
		//fclose(Div_Q_result);
		//fclose(Div_R_result);
		memset(A_eof, 128, sizeof(char));
		memset(B_eof, 128, sizeof(char));
		cnt++;
	}
	fclose(A_file);
	fclose(B_file);
	fclose(Add_result);
	fclose(Sub_result);
	fclose(Mul_result);
	fclose(Mul_K_result);
	//fclose(Div_Q_result);
	//fclose(Div_R_result);
	fclose(Squ_A_result);
	fclose(Squ_B_result);
}


