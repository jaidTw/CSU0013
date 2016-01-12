#define TYPE_CHAR  1
#define TYPE_INT   2
#define TYPE_FLOAT 3
#define STACK_ERROR -100
#define TRUE 1
#define FALSE 0

typedef struct STACK * stack;
typedef struct NODE * node;
typedef struct NODE {
   int data;
   node next;
   }_NODE; 
typedef struct STACK {
   int data_type;
   node next; 
   }_STACK;


stack SNew (int dt) ;
int SFull (stack ptr);
int SEmpty (stack ptr);
int SPush (stack ptr, int op);
int SPop (stack ptr, int *op);
int SPrint (stack ptr);
