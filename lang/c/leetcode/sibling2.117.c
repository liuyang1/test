#include "leet.h"

void connect(struct TreeLinkNode *root) {
    struct TreeLinkNode *bro, *sibling, *nbro, *prev;
    for (bro = root; bro != NULL; bro = nbro) {
        nbro = NULL;
        prev = NULL;
        for (sibling = bro; sibling != NULL; sibling = sibling->next) {
            if (sibling->left) {
                if (prev != NULL) {
                    prev->next = sibling->left;
                }
                if (sibling->right) {
                    sibling->left->next = sibling->right;
                    prev = sibling->right;
                } else {
                    prev = sibling->left;
                }
                if (nbro == NULL) {
                    nbro = sibling->left;
                }
            } else if (sibling->right) {
                if (prev != NULL) {
                    prev->next = sibling->right;
                }
                prev = sibling->right;
                if (nbro == NULL) {
                    nbro = sibling->right;
                }
            }
        }
    }
}


int case2() {
    struct TreeLinkNode *an = malloc(sizeof(struct TreeLinkNode) * 7);
    initTreeLink(an, 0, an + 1, an + 2);
    initTreeLink(an + 1, 1, an + 3, an + 4);
    initTreeLink(an + 2, 2, an + 5, an + 6);
    initTreeLink(an + 3, 3, NULL, NULL);
    initTreeLink(an + 4, 4, NULL, NULL);
    initTreeLink(an + 5, 5, NULL, NULL);
    initTreeLink(an + 6, 6, NULL, NULL);
    connect(an);
    showTreeLink(an);
    free(an);
    return 0;
}
int case3() {
    struct TreeLinkNode *an = malloc(sizeof(struct TreeLinkNode) * 7);
    initTreeLink(an, 0, an + 1, an + 2);
    initTreeLink(an + 1, 1, an + 3, NULL);
    initTreeLink(an + 2, 2, NULL, an + 6);
    initTreeLink(an + 3, 3, NULL, NULL);
    initTreeLink(an + 6, 6, NULL, NULL);
    connect(an);
    showTreeLink(an);
    free(an);
    return 0;
}
int case4() {
    struct TreeLinkNode *an = malloc(sizeof(struct TreeLinkNode) * 7);
    initTreeLink(an, 0, an + 1, an + 2);
    initTreeLink(an + 1, 1, NULL, NULL);
    initTreeLink(an + 2, 2, NULL, an + 6);
    initTreeLink(an + 6, 6, NULL, NULL);
    connect(an);
    showTreeLink(an);
    free(an);
    return 0;
}

int main() {
    case2();
    case3();
    case4();
    return 0;
}
