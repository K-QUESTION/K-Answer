#include "operation.h"
#include "test.h"

/**
 * @file test.c
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (kth0305a@kookmin.ac.kr)
 * @brief File that performs test function.
 * @version 1.0.0
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021
 *
 */

/**
 * @brief A function that stores the Bigint structure in a file in hexadecimal form as a result of the operation.
 *
 * @param fp File pointer to store bigint structure values
 * @param x A The result of the operation to be entered into the file
 */
void bi_hex_fprint(FILE* fp, bigint* x)
{

	if (x->sign == NEGATIVE)
	{
		fprintf(fp, "-0x");
	}
	else
		fprintf(fp, "0x");
	if (Word_Bit_Len == 8)
	{
		fprintf(fp, "%x", (unsigned char)x->a[x->wordlen - 1]);
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp, "%02x", (unsigned char)x->a[i]);
	}
	else if (Word_Bit_Len == 32)
	{
		fprintf(fp, "%x", (unsigned int)x->a[x->wordlen - 1]);
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp, "%08x", (unsigned int)x->a[i]);
	}
	else if (Word_Bit_Len == 64)
	{
		fprintf(fp, "%llx", (unsigned long long)x->a[x->wordlen - 1]);
		for (int i = x->wordlen - 2; i >= 0; i--)
			fprintf(fp, "%016llx", (unsigned long long)x->a[i]);
	}
	fprintf(fp, "\n");
}

/**
 * @brief A function that calls an operation according to the received operation number.
 *
 * @param op The operation number to perform the operation received from the user
 */
void test__(int op)
{
	switch (op)
	{
	case 1:
		test_add();
		break;
	case 2:
		test_sub();
		break;
	case 3:
		test_mul();
		break;
	case 4:
		test_mul_kara();
		break;
	case 5:
		test_div();
		break;
	case 6:
		test_div_binary();
		break;
	case 7:
		test_squ();
		break;
	case 8:
		test_squ_kara();
		break;
	case 9:
		test_mod_exp_l2r();
		break;
	case 10:
		test_mod_exp_r2l();
		break;
	case 11:
		test_mod_exp_mon();
		break;
	default:
		break;
	}
}

/**
 * @brief A function that performs the test of the add operation.
 */
void test_add(void)
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* ADD_Res = NULL;			// OUTPUT : ADD operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* B_file = NULL;			// INPUT  : File where B is saved.
	FILE* Add_result = NULL;		// OUTPUT : File to save Add results

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B

	switch (Word_Bit_Len)
	{
	case 8:
		Add_result = fopen("_Add.txt", "a+");
		break;
	case 32:
		Add_result = fopen("__Add_32.txt", "a+");
		break;
	case 64:
		Add_result = fopen("___Add_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");
	B_file = fopen("B.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Addition \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d\n", number);
			printf("\n * Word_bit length = %d", Word_Bit_Len);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}
		if (B_buf[0] == '-')
		{
			bi_set_by_string(&B, NEGATIVE, &B_buf[3]);
		}
		else
		{
			bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);
		}

		ADD(&ADD_Res, A, B);
		bi_hex_fprint(Add_result, ADD_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&ADD_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(Add_result);
}

/**
 * @brief A function that performs the test of the sub operation.
 */
void test_sub(void)
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* SUB_Res = NULL;		    // OUTPUT : SUB operation result
								    
	FILE* A_file = NULL;		    // INPUT  : File where A is saved.
	FILE* B_file = NULL;		    // INPUT  : File where B is saved.
	FILE* Sub_result = NULL;	    // OUTPUT : File to save Sub results

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B

	switch (Word_Bit_Len)
	{
	case 8:
		Sub_result = fopen("_Sub.txt", "a+");
		break;
	case 32:
		Sub_result = fopen("__Sub_32.txt", "a+");
		break;
	case 64:
		Sub_result = fopen("___Sub_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");
	B_file = fopen("B.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Subtraction \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");

			printf("\n * The number of Test vectors = %d\n", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}
		if (B_buf[0] == '-')
		{
			bi_set_by_string(&B, NEGATIVE, &B_buf[3]);
		}
		else
		{
			bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);
		}

		SUB(&SUB_Res, A, B);
		bi_hex_fprint(Sub_result, SUB_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&SUB_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(Sub_result);
}

/**
 * @brief A function that performs the test of the mul operation.
 */
void test_mul()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* MUL_Res = NULL;			// OUTPUT : MUL operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* B_file = NULL;			// INPUT  : File where B is saved.
	FILE* Mul_result = NULL;		// OUTPUT : File to save Mul results

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double mul_clock = 0;       // Schoolbook mul

	switch (Word_Bit_Len)
	{
	case 8:
		Mul_result = fopen("_Mul.txt", "a+");
		break;
	case 32:
		Mul_result = fopen("__Mul_32.txt", "a+");
		break;
	case 64:
		Mul_result = fopen("___Mul_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");
	B_file = fopen("B.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Multiplication Schoolbook \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");

			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d", Word_Bit_Len);
			printf("\n * Flag = %d (for Karatsuba)\n", Flag);

			printf("\n [Running Time]\n");
			printf(" Mul Schoolbook  -> Total Performance Time: %.3lf sec\n", mul_clock);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}
		if (B_buf[0] == '-')
		{
			bi_set_by_string(&B, NEGATIVE, &B_buf[3]);
		}
		else
		{
			bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);
		}

		/*****MUL*****/
		start = clock();
		MUL(&MUL_Res, A, B);
		end = clock();
		mul_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Mul_result, MUL_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&MUL_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(Mul_result);
}

/**
 * @brief A function that performs the test of the mul karatsuba operation.
 */
void test_mul_kara()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* MUL_K_Res = NULL;		// OUTPUT : MUL Karatsuba operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* B_file = NULL;			// INPUT  : File where B is saved.
	FILE* Mul_K_result = NULL;		// OUTPUT : File to save Mul Karatsuba results

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double mul_k_clock = 0;		// Karatsuba Mul Time

	switch (Word_Bit_Len)
	{
	case 8:
		Mul_K_result = fopen("_Mul_K.txt", "a+");
		break;
	case 32:
		Mul_K_result = fopen("__Mul_K_32.txt", "a+");
		break;
	case 64:
		Mul_K_result = fopen("___Mul_K_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");
	B_file = fopen("B.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Multiplication Karatsuba \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");

			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d", Word_Bit_Len);
			printf("\n * Flag = %d (for Karatsuba)\n", Flag);

			printf("\n [Running Time]\n");
			printf(" Mul Karatsuba   -> Total Performance Time: %.3lf sec\n\n", mul_k_clock);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}
		if (B_buf[0] == '-')
		{
			bi_set_by_string(&B, NEGATIVE, &B_buf[3]);
		}
		else
		{
			bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);
		}

		/*****MUL_kara*****/
		start = clock();
		MULC_K(&MUL_K_Res, A, B);
		end = clock();
		mul_k_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Mul_K_result, MUL_K_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&MUL_K_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(Mul_K_result);
}

/**
 * @brief A function that performs the test of the div operation.
 */
void test_div()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* DIV_Q_Res = NULL;		// OUTPUT : DIV operation result - quotient 
	bigint* DIV_R_Res = NULL;		// OUTPUT : DIV operation result - remainder 

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* B_file = NULL;			// INPUT  : File where B is saved.
	FILE* Div_Q_result = NULL;		// OUTPUT : File to save Div results - quotient 
	FILE* Div_R_result = NULL;		// OUTPUT : File to save Div results - remainder

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double div_clock = 0;		// Multi-Precision

	switch (Word_Bit_Len)
	{
	case 8:
		Div_Q_result = fopen("_Div_Q.txt", "a+");
		Div_R_result = fopen("_Div_R.txt", "a+");
		break;
	case 32:
		Div_Q_result = fopen("__Div_Q_32.txt", "a+");
		Div_R_result = fopen("__Div_R_32.txt", "a+");
		break;
	case 64:
		Div_Q_result = fopen("___Div_Q_64.txt", "a+");
		Div_R_result = fopen("___Div_R_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("Ann.txt", "r");
	B_file = fopen("Bnn.txt", "r");
	int number = 0;
	printf("\n Start Test  -> Divisioin Multi-Precision \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);

			printf("\n [Running Time]\n");
			printf(" Div Naive   -> Total Performance Time: %.3lf sec\n", div_clock);
			break;
		}
		bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);

		/*****DIV*****/
		start = clock();
		DIV(&DIV_Q_Res, &DIV_R_Res, A, B);
		end = clock();
		div_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Div_Q_result, DIV_Q_Res);
		bi_hex_fprint(Div_R_result, DIV_R_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&DIV_Q_Res);
		bi_delete(&DIV_R_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(Div_Q_result);
	fclose(Div_R_result);
}

/**
 * @brief A function that performs the test of the binary div operation.
 */
void test_div_binary()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* B = NULL;				// INPUT  : B
	bigint* B_DIV_Q_Res = NULL;		// OUTPUT : DIV Binary operation result - quotient 
	bigint* B_DIV_R_Res = NULL;		// OUTPUT : DIV Binary operation result - remainder 

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* B_file = NULL;			// INPUT  : File where B is saved.
	FILE* B_Div_Q_result = NULL;	// OUTPUT : File to save Div Binary results - quotient 
	FILE* B_Div_R_result = NULL;	// OUTPUT : File to save Div Binary results - remainder

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char B_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store B value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* B_eof = NULL;				// The value indicating the end of file B
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double b_div_clock = 0;		// Binary Long Division

	switch (Word_Bit_Len)
	{
	case 8:
		B_Div_Q_result = fopen("_B_Div_Q.txt", "a+");
		B_Div_R_result = fopen("_B_Div_R.txt", "a+");
		break;
	case 32:
		B_Div_Q_result = fopen("__B_Div_Q_32.txt", "a+");
		B_Div_R_result = fopen("__B_Div_R_32.txt", "a+");
		break;
	case 64:
		B_Div_Q_result = fopen("___B_Div_Q_64.txt", "a+");
		B_Div_R_result = fopen("___B_Div_R_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("Ann.txt", "r");
	B_file = fopen("Bnn.txt", "r");
	int number = 0;
	printf("\n Start Test  -> Divisioin Binary Long \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		B_eof = fgets(B_buf, Max_Buf_Size, B_file);

		if (A_eof == NULL || B_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);

			printf("\n [Running Time]\n");
			printf(" Div Binary Long -> Total Performance Time: %.3lf sec\n\n", b_div_clock);
			break;
		}
		bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		bi_set_by_string(&B, NON_NEGATIVE, &B_buf[2]);

		/*****Binary Long Div*****/
		start = clock();
		Binary_Long_DIV(&B_DIV_Q_Res, &B_DIV_R_Res, A, B);
		end = clock();
		b_div_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(B_Div_Q_result, B_DIV_Q_Res);
		bi_hex_fprint(B_Div_R_result, B_DIV_R_Res);

		bi_delete(&A);
		bi_delete(&B);
		bi_delete(&B_DIV_Q_Res);
		bi_delete(&B_DIV_R_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(B_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(B_file);
	fclose(B_Div_Q_result);
	fclose(B_Div_R_result);
}

/**
 * @brief A function that performs the test of the squ operation.
 */
void test_squ()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* SQU_Res = NULL;			// OUTPUT : SQU operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* Squ_result = NULL;		// OUTPUT : File to save Squ result

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double squ_clock = 0;		// Schoolbook Squ

	switch (Word_Bit_Len)
	{
	case 8:
		Squ_result = fopen("_Squ.txt", "a+");
		break;
	case 32:
		Squ_result = fopen("__Squ_32.txt", "a+");
		break;
	case 64:
		Squ_result = fopen("___Squ_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Squaring Schoolbook \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);

		if (A_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");

			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d", Word_Bit_Len);
			printf("\n * Flag = %d (for Karastuba)\n", Flag);

			printf("\n [Running Time]\n");
			printf(" Squ Schoolbook  -> Total Performance Time: %.3lf sec\n", squ_clock);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}


		/*****SQU*****/
		start = clock();
		SQU(&SQU_Res, A);
		end = clock();
		squ_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Squ_result, SQU_Res);

		bi_delete(&A);
		bi_delete(&SQU_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(Squ_result);
}

/**
 * @brief A function that performs the test of the squ karatsuba operation.
 */
void test_squ_kara()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* SQU_K_Res = NULL;		// OUTPUT : SQU Karatsuba operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* Squ_K_result = NULL;		// OUTPUT : File to save Squ Karatsuba result

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double squ_k_clock = 0;		// Karastuba Squ

	switch (Word_Bit_Len)
	{
	case 8:
		Squ_K_result = fopen("_Squ_K.txt", "a+");
		break;
	case 32:
		Squ_K_result = fopen("__Squ_K_32.txt", "a+");
		break;
	case 64:
		Squ_K_result = fopen("___Squ_K_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("A.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Squaring Karatsuba \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);

		if (A_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");

			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d", Word_Bit_Len);
			printf("\n * Flag = %d (for Karastuba)\n", Flag);

			printf("\n [Running Time]\n");
			printf(" Squ Karatsuba   -> Total Performance Time: %.3lf sec\n\n", squ_k_clock);
			break;
		}
		if (A_buf[0] == '-')
		{
			bi_set_by_string(&A, NEGATIVE, &A_buf[3]);
		}
		else
		{
			bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		}

		/*****SQU_kara*****/
		start = clock();
		SQUC_K(&SQU_K_Res, A);
		end = clock();
		squ_k_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Squ_K_result, SQU_K_Res);

		bi_delete(&A);
		bi_delete(&SQU_K_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));

		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(Squ_K_result);
}

/**
 * @brief A function that performs the test of the modular exponention L2R operation.
 */
void test_mod_exp_l2r()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* M = NULL;				// INPUT  : Exp Mod operation - Mod value
	bigint* n = NULL;				// INPUT  : Exp Mod operation - exponent
	bigint* L2R_Res = NULL;			// OUTPUT : Mod Exp L2R operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* n_file = NULL;			// INPUT  : File where n is saved.
	FILE* M_file = NULL;			// INPUT  : File where M is saved.
	FILE* L2R_result = NULL;		// OUTPUT : File to save Mod Exp L2R operation result

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char M_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store M value in file
	char n_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store n value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* M_eof = NULL;				// The value indicating the end of file M
	char* n_eof = NULL;				// The value indicating the end of file n
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double l2r_clock = 0;		// L2R

	switch (Word_Bit_Len)
	{
	case 8:
		L2R_result = fopen("_exp_L2R.txt", "a+");
		break;
	case 32:
		L2R_result = fopen("__exp_L2R_32.txt", "a+");
		break;
	case 64:
		L2R_result = fopen("___exp_L2R_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("Ann.txt", "r");
	n_file = fopen("n.txt", "r");
	M_file = fopen("M.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Modular Exponentiation L2R \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		n_eof = fgets(n_buf, Max_Buf_Size, n_file);
		M_eof = fgets(M_buf, Max_Buf_Size, M_file);
		if (A_eof == NULL || n_eof == NULL || M_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);

			printf("\n [Running Time]\n");
			printf(" L2R      -> Total Performance Time: %.3lf sec\n", l2r_clock);
			break;
		}
		bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		bi_set_by_string(&n, NON_NEGATIVE, &n_buf[2]);
		bi_set_by_string(&M, NON_NEGATIVE, &M_buf[2]);

		/*****Mod Exp*****/
		/*****L2R*****/
		start = clock();
		L2R(&L2R_Res, A, n, M);
		end = clock();
		l2r_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(L2R_result, L2R_Res);

		bi_delete(&A);
		bi_delete(&n);
		bi_delete(&M);

		bi_delete(&L2R_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(n_buf, Max_Buf_Size, sizeof(char));
		memset(M_buf, Max_Buf_Size, sizeof(char));
		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(M_file);
	fclose(n_file);
	fclose(L2R_result);
}

/**
 * @brief A function that performs the test of the modular exponention R2L operation.
 */
void test_mod_exp_r2l()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* n = NULL;				// INPUT  : Exp Mod operation - Mod value
	bigint* M = NULL;				// INPUT  : Exp Mod operation - exponent
	bigint* R2L_Res = NULL;			// OUTPUT : Mod Exp R2L operation result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* n_file = NULL;			// INPUT  : File where n is saved.
	FILE* M_file = NULL;			// INPUT  : File where M is saved.
	FILE* R2L_result = NULL;		// OUTPUT : File to save Mod Exp R2L operation result

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char M_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store M value in file
	char n_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store n value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* M_eof = NULL;				// The value indicating the end of file M
	char* n_eof = NULL;				// The value indicating the end of file n
	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double r2l_clock = 0;		// R2L

	switch (Word_Bit_Len)
	{
	case 8:
		R2L_result = fopen("_exp_R2L.txt", "a+");
		break;
	case 32:
		R2L_result = fopen("__exp_R2L_32.txt", "a+");
		break;
	case 64:
		R2L_result = fopen("___exp_R2L_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("Ann.txt", "r");
	n_file = fopen("n.txt", "r");
	M_file = fopen("M.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Modular Exponentiation R2L \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		n_eof = fgets(n_buf, Max_Buf_Size, n_file);
		M_eof = fgets(M_buf, Max_Buf_Size, M_file);
		if (A_eof == NULL || n_eof == NULL || M_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);

			printf("\n [Running Time]\n");
			printf(" R2L      -> Total Performance Time: %.3lf sec\n", r2l_clock);
			break;
		}
		bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		bi_set_by_string(&n, NON_NEGATIVE, &n_buf[2]);
		bi_set_by_string(&M, NON_NEGATIVE, &M_buf[2]);

		/*****Mod Exp*****/
		/*****R2L*****/
		start = clock();
		R2L(&R2L_Res, A, n, M);
		end = clock();
		r2l_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(R2L_result, R2L_Res);

		bi_delete(&A);
		bi_delete(&n);
		bi_delete(&M);
		bi_delete(&R2L_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(n_buf, Max_Buf_Size, sizeof(char));
		memset(M_buf, Max_Buf_Size, sizeof(char));
		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(M_file);
	fclose(n_file);
	fclose(R2L_result);

}

/**
 * @brief A function that performs the test of the modular exponention Montgomery operation.
 */
void test_mod_exp_mon()
{
	bigint* A = NULL;				// INPUT  : A
	bigint* n = NULL;				// INPUT  : Exp Mod operation - Mod value
	bigint* M = NULL;				// INPUT  : Exp Mod operation - exponent
	bigint* Mon_Res = NULL;			// OUTPUT : Mod Exp L2R Montgomery result

	FILE* A_file = NULL;			// INPUT  : File where A is saved.
	FILE* n_file = NULL;			// INPUT  : File where n is saved.
	FILE* M_file = NULL;			// INPUT  : File where M is saved.
	FILE* Mon_result = NULL;		// OUTPUT : File to save Mod Exp Montgomery operation result 

	/*
	* Buffer that brings up file values.
	*/
	char A_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store A value in file
	char M_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store M value in file
	char n_buf[Max_Buf_Size] = { 0x00, };	// Buffer to store n value in file
	/*
	* A value to check if it's the last file
	*/
	char* A_eof = NULL;				// The value indicating the end of file A
	char* M_eof = NULL;				// The value indicating the end of file M
	char* n_eof = NULL;				// The value indicating the end of file n

	/*
	* Speed measurement variable.
	*/
	double start = 0;			// Measurement starts
	double end = 0;				// Measurement done
	double mont_clock = 0;		// Montgomery

	switch (Word_Bit_Len)
	{
	case 8:
		Mon_result = fopen("_exp_Mon.txt", "a+");
		break;
	case 32:
		Mon_result = fopen("__exp_Mon_32.txt", "a+");
		break;
	case 64:
		Mon_result = fopen("___exp_Mon_64.txt", "a+");
		break;
	default:
		break;
	}

	A_file = fopen("Ann.txt", "r");
	n_file = fopen("n.txt", "r");
	M_file = fopen("M.txt", "r");

	int number = 0;
	printf("\n Start Test  -> Modular Exponentiation Montgomery \n");
	while (1)
	{
		A_eof = fgets(A_buf, Max_Buf_Size, A_file);
		n_eof = fgets(n_buf, Max_Buf_Size, n_file);
		M_eof = fgets(M_buf, Max_Buf_Size, M_file);
		if (A_eof == NULL || n_eof == NULL || M_eof == NULL) // The last line
		{
			//printf("]");
			printf("\n Test vector calculation completed.\n");
			printf("\n * The number of Test vectors = %d", number);
			printf("\n * Word_bit length = %d\n", Word_Bit_Len);

			printf("\n [Running Time]\n");
			printf(" Montgomery      -> Total Performance Time: %.3lf sec\n\n", mont_clock);
			break;
		}
		bi_set_by_string(&A, NON_NEGATIVE, &A_buf[2]);
		bi_set_by_string(&n, NON_NEGATIVE, &n_buf[2]);
		bi_set_by_string(&M, NON_NEGATIVE, &M_buf[2]);

		/*****Mod Exp*****/
		/*****L2R*****/
		start = clock();
		L2R(&Mon_Res, A, n, M);
		end = clock();
		mont_clock += (double)(end - start) / (double)1000;
		bi_hex_fprint(Mon_result, Mon_Res);

		bi_delete(&A);
		bi_delete(&n);
		bi_delete(&M);
		bi_delete(&Mon_Res);

		memset(A_buf, Max_Buf_Size, sizeof(char));
		memset(n_buf, Max_Buf_Size, sizeof(char));
		memset(M_buf, Max_Buf_Size, sizeof(char));
		//printf("-");
		number++;
	}

	fclose(A_file);
	fclose(M_file);
	fclose(n_file);
	fclose(Mon_result);
}
