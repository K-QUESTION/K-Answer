# K-Answer
## Answer library

### Created by Question on 2021/12/8.
--------------------------------
+ A library that can calculate bignum based on C. 
  + Bignum refers to a large number of words that cannot be expressed with the corresponding 1 word in units of 8/32/64 bits. 
  + That is, it enables operations on large integers that could not be solved by the existing function of C. 
  + Supports libraries on Windows/Linux
---------------------------------
+ Library developer
  + Ryu Ji-eun and Yoon Hye-jin 
    + Majoring in information security cryptography at Kookmin University. 
    + 4th Grade
+ Intern 
  + Kim Tae-hee
    + Majoring in information security cryptography at Kookmin University. 
    + 1st Grade
---------------------------------
+ When operating the library, the user can set the following items in the type.h file.

  + 1 word bit length (Word_Bit_Len) : 8/32/64 (default Word_Bit_Len=64)
  + When using the Karatsuba operation, the word length (Flag) of the flag (defalut flag = 4)
---------------------------------
+ Operations provided by this library.

  + Compare the size of two numbers
    + ComparABS( ), Compare( )
  + Left_shift, Right_shift
    + bi_L_shift( ), bi_R_shift( )
  + Modular 2^n.
    + bi_reduct( )
  + Addition
    + ADD( ), ADD_CA( ), ADDC( ), ADD_ABc( ), ADD_AB( )
  + Subtraction
      + SUB( ), SUB_CA( ), SUBC( ), SUB_AbB( ), SUB_AB( )
  + Multiplication
    + MUL( ), MULC( ), MULC_K( ), MUL_AB( )
  + Squaring
    + SQU( ), SQUC( ), SQUC_K( ), AA( )
  + Division
    + DIV( ), DIVC( ), DIVCC( ), Long_DIV( ), Binary_Long_DIV( )
  + Modular Exponentiation
    + L2R( ), R2L( ), Montgomery( )
---------------------------------
For more information and manuals, refer to the Answer v1.0.0.pdf file.
