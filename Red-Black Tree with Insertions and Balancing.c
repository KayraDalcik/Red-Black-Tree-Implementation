#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int deger;
    char renk;
    struct Node *sol, *sag, *ebeveyn;
} Node;

Node *NIL;

Node* yeniDugumOlustur(int deger) {
    Node *yeniDugum = (Node*)malloc(sizeof(Node));
    yeniDugum->deger = deger;
    yeniDugum->renk = 'R';
    yeniDugum->sol = NIL;
    yeniDugum->sag = NIL;
    yeniDugum->ebeveyn = NULL;
    return yeniDugum;
}

void solaDondur(Node **node, Node *x) {
    Node *y = x->sag;
    x->sag = y->sol;
    if (y->sol != NIL)
        y->sol->ebeveyn = x;
    y->ebeveyn = x->ebeveyn;
    if (x->ebeveyn == NULL)
        *node = y;
    else if (x == x->ebeveyn->sol)
        x->ebeveyn->sol = y;
    else
        x->ebeveyn->sag = y;
    y->sol = x;
    x->ebeveyn = y;
}

void sagaDondur(Node **node, Node *y) {
    Node *x = y->sol;
    y->sol = x->sag;
    if (x->sag != NIL)
        x->sag->ebeveyn = y;
    x->ebeveyn = y->ebeveyn;
    if (y->ebeveyn == NULL)
        *node = x;
    else if (y == y->ebeveyn->sag)
        y->ebeveyn->sag = x;
    else
        y->ebeveyn->sol = x;
    x->sag = y;
    y->ebeveyn = x;
}

void eklemeDuzenle(Node **node, Node *z) {
    while (z->ebeveyn->renk == 'R') {
        if (z->ebeveyn == z->ebeveyn->ebeveyn->sol) {
            Node *y = z->ebeveyn->ebeveyn->sag;
            if (y->renk == 'R') {
                z->ebeveyn->renk = 'B';
                y->renk = 'B';
                z->ebeveyn->ebeveyn->renk = 'R';
                z = z->ebeveyn->ebeveyn;
            } else {
                if (z == z->ebeveyn->sag) {
                    z = z->ebeveyn;
                    solaDondur(node, z);
                }
                z->ebeveyn->renk = 'B';
                z->ebeveyn->ebeveyn->renk = 'R';
                sagaDondur(node, z->ebeveyn->ebeveyn);
            }
        } else {
            Node *y = z->ebeveyn->ebeveyn->sol;
            if (y->renk == 'R') {
                z->ebeveyn->renk = 'B';
                y->renk = 'B';
                z->ebeveyn->ebeveyn->renk = 'R';
                z = z->ebeveyn->ebeveyn;
            } else {
                if (z == z->ebeveyn->sol) {
                    z = z->ebeveyn;
                    sagaDondur(node, z);
                }
                z->ebeveyn->renk = 'B';
                z->ebeveyn->ebeveyn->renk = 'R';
                solaDondur(node, z->ebeveyn->ebeveyn);
            }
        }
    }
    (*node)->renk = 'B';
}

void ekle(Node **node, int deger) {
    Node *z = yeniDugumOlustur(deger);
    Node *y = NIL;
    Node *x = *node;

    while (x != NIL) {
        y = x;
        if (z->deger < x->deger)
            x = x->sol;
        else
            x = x->sag;
    }
    z->ebeveyn = y;
    if (y == NIL)
        *node = z;
    else if (z->deger < y->deger)
        y->sol = z;
    else
        y->sag = z;

    eklemeDuzenle(node, z);
}

int yukseklik(Node *node) {
    if (node == NIL)
        return 0;
    int solYukseklik = yukseklik(node->sol);
    int sagYukseklik = yukseklik(node->sag);
    return (solYukseklik > sagYukseklik) ? solYukseklik + 1 : sagYukseklik + 1;
}

void seviyeyiYazdir(Node *node, int seviye) {
    if (node == NIL)
        return;
    if (seviye == 1) {
        if (node->ebeveyn == NIL) {
            printf("%d%c", node->deger, node->renk);
        } else {
            printf("%d%c(%d%s)", node->deger, node->renk, node->ebeveyn->deger, (node == node->ebeveyn->sol) ? "L" : "R");
        }
        printf(" ");
    } else if (seviye > 1) {
        seviyeyiYazdir(node->sol, seviye - 1);
        seviyeyiYazdir(node->sag, seviye - 1);
    }
}

void agaciYazdir(Node *node) {
    int h = yukseklik(node);
    for (int i = 1; i <= h; i++) {
        seviyeyiYazdir(node, i);
        printf("\n");
    }
}

void siraliYazdir(Node *node) {
    if (node != NIL) {
        siraliYazdir(node->sol);
        printf("%d\n", node->deger);
        siraliYazdir(node->sag);
    }
}

int main() {
    NIL = yeniDugumOlustur(0);
    NIL->renk = 'B';
    Node *node = NIL;

    int deger;
   
    while (1) {
        scanf("%d", &deger);
        if (deger == -1)
            break;
        ekle(&node, deger);
    }

    siraliYazdir(node);
    printf("\n");

    agaciYazdir(node);

    return 0;
}