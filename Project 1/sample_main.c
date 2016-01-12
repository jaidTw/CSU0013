#include <stdio.h>
#include "SMatrixADT.h"

#define DONE 'X'
#define ADDITION '+'
#define SUBTRACTION '-'
#define TRANSPOSE 'T'
#define Multiply '*'

int main (){
   SMatrixType	A, B, C, D;
   int  w1, w2;
   char op;

   /* read and print matrices */
   SMTX_read (&A); printf ("Matrix A:\n"); SMTX_print (&A);
   SMTX_read (&B); printf ("Matrix B:\n"); SMTX_print (&B);
   SMTX_read (&C); printf ("Matrix C:\n"); SMTX_print (&C);
   
   
   /* Perform matrix operations */ 
   while (getchar() != '\n');
   scanf ("%c", &op);
   while (op != DONE) {
      switch (op) {
         case ADDITION:
            printf ("Addition\n");
            if (SMTX_add (&A, &B, &C)!= SMTX_ERROR)
               SMTX_print (&C);
            else 
               printf ("Matrix ADDITION error!\n");
            break;
         case SUBTRACTION:
            if (SMTX_subtract (&A, &B, &C)!= SMTX_ERROR)
               SMTX_print (&C);
            else 
               printf ("Matrix SUBTRACTION error!\n");
            break;
         case TRANSPOSE:
            if (SMTX_transpose (&A, &C)!= SMTX_ERROR)
               SMTX_print (&C);
            else 
               printf ("Matrix TRNSPOSE error!\n");
            break;
         case Multiply:
            if (SMTX_multiply (&A, &B, &C)!= SMTX_ERROR)
               SMTX_print (&C);
            else 
               printf ("Matrix Multiply error!\n");
            break;
         default :
               printf ("Invalid OPERATION error!\n");
         }
printf ("Matrix A\n"); SMTX_print (&A);
printf ("Matrix B\n"); SMTX_print (&B);
printf ("Matrix C\n"); SMTX_print (&C);

      while (getchar() != '\n'); 
      scanf ("%c", &op);
      } /* while */

	 /** you should do more testing than the above **/
}

