#include "leet.h"
void connect(struct TreeLinkNode *root) {
    struct TreeLinkNode *bro, *sibling;
    for (bro = root; bro != NULL; bro = bro->left) {
        for (sibling = bro; sibling != NULL; sibling = sibling->next) {
            if (sibling->left && sibling->right) {
                sibling->left->next = sibling->right;
                sibling->right->next = sibling->next ? sibling->next->left : NULL;
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

int main() {
    case2();
    return 0;
}
