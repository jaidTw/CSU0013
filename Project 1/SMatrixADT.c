/*
 * CSU0013 Data Structures
 * Project 1
 * Sparse Matrix ADT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SMatrixADT.h"

#define _SMTX_COMP_GREATER 1
#define _SMTX_COMP_EQUAL 0
#define _SMTX_COMP_LESS -1
#define _SMTX_COMP_A_OUT_OF_RANGE 2
#define _SMTX_COMP_B_OUT_OF_RANGE 3
#define _SMTX_COMP_BOTH_OUT_OF_RANGE 4

static inline void _smtx_set_triplet(_SMTX_Triplet *T, size_t row, size_t col, int val) {
	T->row = row;
	T->col = col;
	T->val = val;
}

static inline int trans_comp(const void* a, const void* b) {
	_SMTX_Triplet *t1 = (_SMTX_Triplet *)a, *t2 = (_SMTX_Triplet *)b;
	if(t1->row > t2->row) {
		return 1;
	}
	else if(t1->row < t2->row) {
		return -1;
	}
	else if(t1->col > t2->col) {
		return 1;
	}
	else if(t1->col < t2->col) {
		return -1;
	}
	else {
		return 0;
	}
}

static inline const int 
_smtx_compare_triplet(SMatrixType *A, SMatrixType *B, size_t a_idx, size_t b_idx) {
	/* Check range */
	if(a_idx > A->lst[0].val && b_idx > B->lst[0].val) {
		return _SMTX_COMP_BOTH_OUT_OF_RANGE;
	}
	else if(a_idx > A->lst[0].val) {
		return _SMTX_COMP_A_OUT_OF_RANGE;
	}
	else if(b_idx > B->lst[0].val) {
		return _SMTX_COMP_B_OUT_OF_RANGE;
	}
	/* Compare */
	if(A->lst[a_idx].row > B->lst[b_idx].row) {
		return _SMTX_COMP_GREATER;
	}
	else if(A->lst[a_idx].row < B->lst[b_idx].row) {
		return _SMTX_COMP_LESS;
	}
	else if(A->lst[a_idx].col > B->lst[b_idx].col) {
		return _SMTX_COMP_GREATER;
	}
	else if(A->lst[a_idx].col < B->lst[b_idx].col) {
		return _SMTX_COMP_LESS;
	}
	else {
		return _SMTX_COMP_EQUAL;
	}
}


int SMTX_read(SMatrixType *M) {
	size_t rows, cols;
	scanf("%d %d", &rows, &cols);
	size_t row, col, idx = 1;
	int val;
	for(row = 0; row < rows; ++row) {
		for(col = 0; col < cols; ++col) {
			if(idx > MAX_SMTX_SIZE) {
				return SMTX_ERROR;
			}
			scanf("%d", &val);
			if(val != 0) {
				_smtx_set_triplet(&(M->lst[idx++]), row, col, val);
			}
		}
	}
	_smtx_set_triplet(&(M->lst[0]), rows, cols, idx - 1);
}

int SMTX_print(SMatrixType *M) {
	printf("Rows = %d, Cols = %d, # of non-zero entries = %d\n", M->lst[0].row, M->lst[0].col, M->lst[0].val);
	size_t idx;
	for(idx = 1; idx <= M->lst[0].val; ++idx) {
		printf("< %d, %d, %d>\n", M->lst[idx].row, M->lst[idx].col, M->lst[idx].val);
	}
	return 0;
}

int SMTX_add(SMatrixType *A, SMatrixType *B, SMatrixType *C) {
	/* Check the sizes of matrices */
	if(A->lst[0].row != B->lst[0].row || A->lst[0].col != B->lst[0].col) {
		return SMTX_ERROR;
	}

	size_t a_idx = 1, b_idx = 1, c_idx = 1;
	for(;;) {
		switch(_smtx_compare_triplet(A, B, a_idx, b_idx)) {
			case _SMTX_COMP_A_OUT_OF_RANGE:
			case _SMTX_COMP_GREATER:
				C->lst[c_idx] = B->lst[b_idx++];
				break;
			case _SMTX_COMP_B_OUT_OF_RANGE:
			case _SMTX_COMP_LESS:
				C->lst[c_idx] = A->lst[a_idx++];
				break;
			case _SMTX_COMP_EQUAL:
				C->lst[c_idx] = A->lst[a_idx++];
				C->lst[c_idx].val += B->lst[b_idx++].val;
				/* if generates a zero, eliminates the term */
				c_idx -= (C->lst[c_idx].val == 0);
				break;
			case _SMTX_COMP_BOTH_OUT_OF_RANGE:
				_smtx_set_triplet(&(C->lst[0]), A->lst[0].row, A->lst[0].col, c_idx - 1);
				return 0;
			default:
				return SMTX_ERROR;
		}
		if(c_idx++ > MAX_SMTX_SIZE) {
			return SMTX_ERROR;
		}
	}
	return SMTX_ERROR;
}

int SMTX_subtract(SMatrixType *A, SMatrixType *B, SMatrixType *C) {
	size_t idx;
	/* Making B negative */
	for(idx = 1; idx <= B->lst[0].val; B->lst[idx++].val *= -1);
	/* Substraction */
	int result = SMTX_add(A, B, C);
	/* Restore B*/
	for(idx = 1; idx <= B->lst[0].val; B->lst[idx++].val *= -1);
	return result;
}

int SMTX_transpose(SMatrixType *A, SMatrixType *A_T) {
	/* Initialize */
	_smtx_set_triplet(&(A_T->lst[0]), A->lst[0].col, A->lst[0].row, A->lst[0].val);

	int *counting;
	/* If the allocation failed, try to use the comparison model instead of the statistic model */
	if((counting = malloc(A->lst[0].col * sizeof(int))) == NULL) {
		size_t idx;
		for(idx = 1; idx <= A->lst[0].val; ++idx)
			_smtx_set_triplet(&(A_T->lst[idx]), A->lst[idx].col, A->lst[idx].row, A->lst[idx].val);
		qsort(A_T->lst + 1, A_T->lst[0].val, sizeof(_SMTX_Triplet), trans_comp);
		return 0;
	}
	size_t idx;
	for(counting[0] = 1, idx = 1; idx <= A->lst[0].val; ++idx) {
		++counting[A->lst[idx].col + 1];
	}
	for(idx = 1; idx < A->lst[0].col; ++idx) {
		counting[idx] += counting[idx - 1];
	}
	for(idx = 1; idx <= A->lst[0].val; ++idx) {
		size_t t_idx = counting[A->lst[idx].col]++;
		_smtx_set_triplet(&(A_T->lst[t_idx]), A->lst[idx].col, A->lst[idx].row, A->lst[idx].val);
	}
	free(counting);
	return 0;
}

int SMTX_multiply(SMatrixType *A, SMatrixType *B, SMatrixType *C) {
	/* Check the sizes of matrices. */
	if(A->lst[0].col != B->lst[0].row) {
		return SMTX_ERROR;
	}
	/* Transpose B */
	SMatrixType _B_T;
	SMatrixType *B_T = &_B_T;
	SMTX_transpose(B, B_T);
	/* Multiplication */
	size_t a_idx = 1, b_idx = 1, c_idx = 1;
	size_t a_row_begin = a_idx, b_row_begin = b_idx;
	int temp_sum = 0;
	while(a_idx <= A->lst[0].val) {
		if(c_idx > MAX_SMTX_SIZE) {
			return SMTX_ERROR;
		}
		if(A->lst[a_idx].col == B_T->lst[b_idx].col) {
			temp_sum += A->lst[a_idx].val * B_T->lst[b_idx].val;
			++b_idx;
		}
		else if(A->lst[a_idx].col < B_T->lst[b_idx].col) {
			++a_idx;
		}
		else if(A->lst[a_idx].col > B_T->lst[b_idx].col) {
			++b_idx;
		}
		/* A changed its row, set A back */
		if(A->lst[a_idx].row != A->lst[a_row_begin].row) {
			a_idx = a_row_begin;
			++b_idx;
		}
		/* B changed its row, set A back, write the value */
		if(B_T->lst[b_idx].row != B_T->lst[b_row_begin].row) {
			if(temp_sum != 0) {
				_smtx_set_triplet(&(C->lst[c_idx++]), A->lst[a_row_begin].row, B_T->lst[b_row_begin].row, temp_sum);
				temp_sum = 0;
			}
			b_row_begin = b_idx;
			a_idx = a_row_begin;
		}
		/* B out of range, make A to the next row, write the value. */
		if(b_idx > B_T->lst[0].val) {
			if(temp_sum != 0) {
				_smtx_set_triplet(&(C->lst[c_idx++]), A->lst[a_row_begin].row, B_T->lst[b_row_begin].row, temp_sum);
				temp_sum = 0;
			}
			while(++a_idx, A->lst[a_idx].row == A->lst[a_row_begin].row);
			a_row_begin = a_idx;
			b_idx = b_row_begin = 1;
		}
	}
	_smtx_set_triplet(&(C->lst[0]), A->lst[0].row, B->lst[0].col, c_idx - 1);
	return 0;
}
