#include "stack.h"
#include <stdio.h>

int main () {
   stack tos_int, tos_char;
   int x;
   char c;

   /* initialize a stack to store chacters and one to store integers */
   tos_char = SNew (TYPE_CHAR);
   tos_int  = SNew (TYPE_INT);

   SPush (tos_int, 56789);
   SPrint (tos_int);
   SPush (tos_int, 12345);
   SPrint (tos_int);

   SPush (tos_char, 'A');
   SPrint (tos_char);
   SPush (tos_char, 'C');
   SPrint (tos_char);

   /****** now pop the entries, with error checking *****/

   printf ("====== Now pop integer stack. \n");

   if (!SEmpty (tos_int)) {
      SPop (tos_int, &x);
      printf ("SPop x = %d\n", x);
      }
   SPrint (tos_int);

   if (!SEmpty (tos_int)) {
      SPop (tos_int, &x);
      printf ("SPop x = %d\n", x);
      }
   SPrint (tos_int);

   printf ("====== Now pop character stack. \n");

   if (!SEmpty (tos_char)) {
      SPop (tos_char, &c);
      printf ("SPop c = %c\n", c);
      }
   SPrint (tos_char);

   if (!SEmpty (tos_char)) {
      SPop (tos_char, &c);
      printf ("SPop c = %c\n", c);
      }
   SPrint (tos_char);

   return 0;
   }



