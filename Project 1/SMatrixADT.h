/*
 * CSU0013 Data Structures
 * Project 1
 * Sparse Matrix ADT
 */
 
#ifndef __SMTX_GUARD
#define __SMTX_GUARD

#define SMTX_ERROR -1
#define MAX_SMTX_SIZE 100

typedef struct {
	size_t row;
	size_t col;
	int val;
} _SMTX_Triplet;

typedef struct {
	_SMTX_Triplet lst[MAX_SMTX_SIZE + 1];
} SMatrixType;

int SMTX_read(SMatrixType *);
int SMTX_print(SMatrixType *);
int SMTX_add(SMatrixType *, SMatrixType *, SMatrixType *);
int SMTX_subtract(SMatrixType *, SMatrixType *, SMatrixType *);
int SMTX_transpose(SMatrixType *, SMatrixType *);
int SMTX_multiply(SMatrixType *, SMatrixType *, SMatrixType *);

#endif