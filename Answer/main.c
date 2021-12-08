 /** 
 * @mainpage ANSWER
 * @section intro Introduction
 * - Introduction :
 * @section Project Project Name
 * -Project : ANSWER
 * -Project Contents: Computational & Cryptographic Algorithm Using Big Integer Structure
 * @section CREATEINFO Team Information
 * - Team name: QUESTION
 * -Team member
 * @author Ryu Jieun (ofryuji@gmail.com )
 * @author Yoon Hyejin (hyejin91655@gmail.com)
 * @author Kim Taehee (Intern)(kth0305a@kookmin.ac.kr)
 * @version 1.0.0
 * @date 2021-12-08
 * @copyright Copyright Question 2021
 */

 /** 
 * @page subpage_1  Answer Manual
 *
 * @section section_1 Generate Test Value Manual
 * 
 * @subsection step1 Step 1
 * Access [Sagemathcell](https://sagecell.sagemath.org/)\n
 * @subsection step2 Step 2
 * Copy the code of Test.txt, paste it on the link above, and click Evlauate.
 * @subsection step3 Step 3
 * The output values are copied except for text such as A, B, Ann, etc., and save A.txt, B.txt, etc (as the file name is).
 * @warning When saving A.txt, B.txt, Ann.txt, Bnn.txt, the last line is a line.
 * @subsection step4 Step 4
 * Save each operation result, such as add, sub, etc. to a text file
 * @subsection step5 Step 5
 * Save each operation result, such as add, sub, etc. to a text file
 * 
 * @section section_2 Test Verify Manual
 * 
 * @subsection OS_W Windows
 * Open the command prompt (window key + R : cmd) and leave the file to compare in the same folder 
 * and move the cmd window to that folder (cd [the file location you want to change])
 * @note fc [Text file 1] [Text file 2] \n
 * 
 * @subsection OS_L Linux
 * Open the command prompt and leave the file to compare in the same folder 
 * and move the cmd window to that folder (cd [the file location you want to change])
 * @note diff [Text file 1] [Text file 2].
 */

 /**
  * @file main.c
  * @author Ryu Jieun (ofryuji@gmail.com )
  * @author Yoon Hyejin (hyejin91655@gmail.com)
  * @author Kim Taehee (kth0305a@kookmin.ac.kr)
  * @brief File that show Answer Library manual to test.
  * @version 1.0.0
  * @date 2021-12-08
  *
  * @copyright Copyright Question 2021
  *
  */

#include "operation.h"
#include "test.h"

void test_manual();

int main()
{
    srand((unsigned int)time(NULL));
    int test_op = 0;
    int scanf_eof = 0;
   
    while (1)
    {
        test_manual();
        scanf_eof = scanf("%d", &test_op);
        if (scanf_eof == -1 || test_op == 0)
        {
            break;
        }
        test__(test_op);
        //test_();
    }
    
    //test_();

    return 0;
}

void test_manual()
{
    printf("Welcome ANSWER Test\n");
    printf("***********************************************\n");
    printf("Operation type?\n *Exit : 0\n");
    printf("*Addition : 1\n");
    printf("*Subtraction : 2\n");
    printf("*Multiplication - Schoolbook : 3\n");
    printf("*Multiplication - Karatsuba : 4\n");
    printf("*Division - Multi Precision : 5\n");
    printf("*Division - Binary Long Division : 6\n");
    printf("*Squaring - Schoolbook : 7\n");
    printf("*Squaring - Karatsuba : 8\n");
    printf("*Modular Exponentiation - L2R : 9\n");
    printf("*Modular Exponentiation - R2L : 10\n");
    printf("*Modular Exponentiation - Montgomery : 11\n");
    printf("***********************************************\n");
    printf("Select Operation!! \n");

}