#include "leet.h"

// serialize to Lisp style
/** Encodes a tree to a single string. */
#define NILSTR "nil"
char *serialize(struct TreeNode *root) {
    int len;
    char *r;
    if (root == NULL) {
        len = 4;
        r = malloc(sizeof(char) * len);
        strcpy(r, NILSTR);
        return r;
    }
    char *left = serialize(root->left);
    char *right = serialize(root->right);
    len = strlen(left) + strlen(right) + 16;
    r = malloc(sizeof(char) * len);
    snprintf(r, len, "(%d,%s,%s)",
             root->val, left, right);
    return r;
}

char *indexComma(char *s) {
    int parenCnt;
    for (; *s != '\0'; s++) {
        switch (*s) {
            case '(': parenCnt++; break;
            case ')': parenCnt--; break;
            case ',':
                if (parenCnt == 0) {
                    return s;
                }
                break;
        }
    }
    return s;
}

/** Decodes your encoded data to tree. */
struct TreeNode *deserialize(char *data) {
    // trust input, skip validation
    if (strncmp(data, NILSTR, strlen(NILSTR)) == 0) {
        return NULL;
    }
    data++; // skip (
    char *trail;
    int val = strtol(data, &trail, 10);
    char *left = trail + 1; // skip comma
    char *right = indexComma(left) + 1; // first next comma, skip it

    LOG("val=%d left=%s right=%s\n", val, left, right);

    struct TreeNode *node = malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = deserialize(left);
    node->right = deserialize(right);
    return node;
}

bool testCase(int *num, int numsSize) {
    struct TreeNode *root = buildTree(num, numsSize);
    showTree(root);

    // How to use serialize and deserialize
    char *data = serialize(root);
    struct TreeNode *node = deserialize(data);

    printf("serialize to [%s]\n", data);
    bool r = eqTree(root, node);
    printf("deserialize to\n");
    if (!r) {
        showTree(node);
    } else {
        printf("same\n");
    }
    printf("%s\n", expect(r));

    freeTree(root);
    freeTree(node);
    if (data) {
        free(data);
    }
    return r;
}

#define CASE(a) {testCase(a, COUNT_OF(a)); }
int main() {
    CASE(((int[]) {-1 * 0x7fffffff}));
    CASE(((int[]) {1, 2, 3, NIL, 4, 5}));
    CASE(((int[]) {1, NIL, 3, 4, 5}));
    return 0;
}
