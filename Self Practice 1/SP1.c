#include <stdio.h>

typedef struct node* nodePointer;
struct node {
    nodePointer llink;
    nodePointer rlink;
    int item;
};

void setnode(nodePointer N, nodePointer llink, nodePointer rlink, int val) {
    N->llink = llink;
    N->rlink = rlink;
    N->item = val;
}

void print_LL(nodePointer start) {
    nodePointer temp = start;
    if(temp == NULL)
        return;
    printf("%d ", temp->item);
    while(temp = temp->rlink, temp != start)
        printf("%d ", temp->item);
    printf("\n");
}

void invert_LL(nodePointer Head) {
    nodePointer T1, T2;
    do {
        T1 = Head->llink;
        T2 = Head->rlink;
        Head->llink = T2;
        Head->rlink = T1;
        Head = Head->rlink;
    } while(Head->rlink->llink != Head);
}

int main(void) {
    struct node N1, N2, N3, N4, N5;
    struct node T1, T2;
    struct node A3;
    setnode(&N1, &N5, &N2, -1);
    setnode(&N2, &N1, &N3, 1);
    setnode(&N3, &N2, &N4, 2);
    setnode(&N4, &N3, &N5, 3);
    setnode(&N5, &N4, &N1, 4);

    setnode(&T1, &T2, &T2, -1);
    setnode(&T2, &T1, &T1, 1);

    setnode(&A3, &A3, &A3, 1);

    print_LL(&N1);
    invert_LL(&N1);
    print_LL(&N1);
    invert_LL(&N1);
    print_LL(&N1);

    print_LL(&T1);
    invert_LL(&T1);
    print_LL(&T1);
    return 0;
}
