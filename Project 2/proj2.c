/*
 * CSU0013 Data Structures
 * Project 2
 * Simple Arithmetic Expression Evaluator
 */
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define MAX_LEN 2048
#define OPNUM operator_convert

#define _OP_PLUS 0
#define _OP_MINUS 1
#define _OP_TIMES 2
#define _OP_DIVIDE 3
#define _OP_LEFT_PR 4
#define _OP_RIGHT_PR 5
#define IS_OPERATOR _OP_PLUS ... _OP_RIGHT_PR + 1
#define EV_ERR_RET(x) { fprintf(stderr, "AN ERROR OCCUR WHILE EVALUATION!\n"); return (x); }
#define CNV_ERR_RET(x) { fprintf(stderr, "AN ERROR OCCUR WHILE CONVERTING!\n"); return (x); }

// Check the parentheses matching, return -1 on error, else 0.
// The string has to be guaranteed end with '\0'.
int match_parentheses(char *);

// Eliminate the spaces inside the string, convert {}s, []s into ()s;
// The string has to be guaranteed end with '\0'.
int normalize_exp(char *);

// Convert an infix expression to postfix.
// The string has to be guaranteed end with '\0', return -1 on error, else 0.
int infix2postfix(char *);

// Evaluate an postfix arithmetic expression.
// The string has to be guaranteed end with '\0', return -1 on error, else 0.
int evaluate_postfix(char *);

// Convert an operator character to an integer used for indexing isp and icp.
// Return -1 if not an valid operator, else 0.
int operator_convert(char);

// Evaluate the expression of two operands and one operator.
int eval(int, int, int *, char);

// Return the top item of stack, beware inline in C is a GNU extension;
static inline int STop(stack ptr) { return ptr->next ? ptr->next->data : STACK_ERROR; }

//	isp and icp for  +  -  *  /  (  )
static char isp[] = {1, 1, 2, 2, 0, 3};
static char icp[] = {1, 1, 2, 2, 4, 3};
static char buf[MAX_LEN];

int main(void) {
	while(fgets(buf, MAX_LEN, stdin) != NULL) {
		buf[strcspn(buf, "\n")] = '\0';
		if(!strncmp(buf, "\0", 1)) continue;
		if(!strncmp(buf, "0\0", 2)) break;
		if(match_parentheses(buf)) { printf("Incorrectly matched parentheses\n"); continue;}
		normalize_exp(buf);
		if(infix2postfix(buf)) continue;
		printf("%d\n", evaluate_postfix(buf));
	}
}

int match_parentheses(char *p) {
	int op;
	stack pr_stk = SNew(TYPE_INT);
	for(; *p != '\0'; ++p) {
		if(*p == '(' || *p == '[' || *p == '{')
			SPush(pr_stk, *p);
		else if(*p == ')' || *p == ']' || *p == '}') {
			if(SEmpty(pr_stk)) return -1;
			SPop(pr_stk, &op);
			if(*p == ')' && op != '(') return -1;
			if(*p ==']' && op != '[') return -1;
			if(*p =='}' && op != '{') return -1;
		}
	}
	if(!SEmpty(pr_stk)) return -1;
	return 0;
}

int normalize_exp(char* str) {
	// inplace replacing, nsp points to the end of the new string.
	char *p, *nsp;
	for(p = nsp = str; *p != '\0'; ++p) {
		if(*p != ' ') {
			if(*p == '{' || *p == '[')		*nsp = '(';
			else if(*p == '}' || *p == ']')	*nsp = ')';
			else *nsp = *p;
			++nsp;
		}
	}
	*nsp = '\0';
	return 0;
}

int infix2postfix(char *p) {
	// inplace replacing, newp will point to the end of the new expression.
	char *newp = p;
	int opr;
	stack op_stk = SNew(TYPE_INT);
	for(; *p != '\0'; ++p) {
		switch(OPNUM(*p)) {
			case IS_OPERATOR: // beware it's a GNU extension.
				if(*p == ')') {
					while(SPop(op_stk, &opr), opr != '(')
						*(newp++) = opr;
					break;
				}
				else if(SEmpty(op_stk));
				else if(isp[OPNUM(STop(op_stk))] >= icp[OPNUM(*p)]) {
					SPop(op_stk, &opr);
					*(newp++) = opr;
				}
				SPush(op_stk, *p);
				break;
			default:
				*(newp++) = *p;
		}
	}
	while(!SEmpty(op_stk)) {
		SPop(op_stk, &opr);
		*(newp++) = opr;
	}
	*newp = '\0';
	return 0;
}

int evaluate_postfix(char *p) {
	stack opr_stk = SNew(TYPE_INT);
	int a, b, c;
	for(; *p != '\0'; ++p) {
		switch(OPNUM(*p)) {
			case IS_OPERATOR: // beware it's a GNU extension.
				if(SPop(opr_stk, &b) == STACK_ERROR) EV_ERR_RET(-1);
				if(SPop(opr_stk, &a) == STACK_ERROR) EV_ERR_RET(-1);
				if(eval(a, b, &c, OPNUM(*p)) == -1)	 EV_ERR_RET(-1);
				if(SPush(opr_stk, c) == STACK_ERROR) EV_ERR_RET(-1);
				break;
			case -1:
				if(SPush(opr_stk, *p - '0') == STACK_ERROR) EV_ERR_RET(-1);
				break;
			default: EV_ERR_RET(-1); // should not be here
		}
	}
	if(SPop(opr_stk, &a) == STACK_ERROR) EV_ERR_RET(-1);
	return a;
}

int operator_convert(char op) {
	switch(op) {
		case '+': return _OP_PLUS;
		case '-': return _OP_MINUS;
		case '*': return _OP_TIMES;
		case '/': return _OP_DIVIDE;
		case '(': return _OP_LEFT_PR;
		case ')': return _OP_RIGHT_PR;
		default: return -1;
	}
}

int eval(int a, int b, int *c, char opn) {
	switch(opn) {
		case _OP_PLUS:	*c = a + b; break;
		case _OP_MINUS:	*c = a - b; break;
		case _OP_TIMES:	*c = a * b; break;
		case _OP_DIVIDE:*c = a / b; break;
		default: return -1;
	}
}