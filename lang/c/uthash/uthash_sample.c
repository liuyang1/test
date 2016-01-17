#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uthash.h"

struct my_struct {
    int id; // key
    char name[10];
    UT_hash_handle hh; // make this struct hashable
};

// global user hash table
struct my_struct *users = NULL;

void add_user(int user_id, char *name) {
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s); // id already in hash?
    if (s == NULL) {
        s = (struct my_struct *)malloc(sizeof(struct my_struct));
        s->id = user_id;
        HASH_ADD_INT(users, id, s); // id: NAME of key field
    }
    strcpy(s->name, name); // may overwrite old user with same id
}

struct my_struct *find_user(int user_id) {
    struct my_struct *s;
    HASH_FIND_INT(users, &user_id, s);
    return s;
}

void delete_user(struct my_struct *user) {
    HASH_DEL(users, user);
    free(user);
}

void delete_all() {
    struct my_struct *current_user, *tmp;

    HASH_ITER(hh, users, current_user, tmp) {
        HASH_DEL(users, current_user);
        free(current_user);
    }
}

void print_users() {
    struct my_struct *s;

    for (s = users; s != NULL; s = (struct my_struct *)(s->hh.next)) {
        printf("user id %d: name %s\n", s->id, s->name);
    }
}

int name_cmp(struct my_struct *a, struct my_struct *b) {
    return strcmp(a->name, b->name);
}
int id_cmp(struct my_struct *a, struct my_struct *b) {
    return (a->id - b->id);
}

void sort_by_name() {
    HASH_SORT(users, name_cmp);
}
void sort_by_id() {
    HASH_SORT(users, id_cmp);
}

void usage() {
    printf("1. add user\n");
    printf("2. add / rename user by id\n");
    printf("3. find user\n");
    printf("4. delete user\n");
    printf("5. delete all users\n");
    printf("6. sort items by name\n");
    printf("7. sort items by id\n");
    printf("8. print users\n");
    printf("9. count users\n");
    printf("0. quit\n");
}

#define SLEN        10
#define MYGETS(s)   fgets(s, SLEN, stdin)
int main() {
    char in[SLEN + 1];
    int id_inc = 1, running = 1, id;
    struct my_struct *s;
    unsigned int num_users = 0;

    usage();
    while (running) {
        MYGETS(in);
        switch (atoi(in)) {
            case 1:
                printf("name?\n");
                MYGETS(in);
                in[strcspn(in, "\r\n")] = 0; // strip newline
                add_user(id_inc++, in);
                break;
            case 2:
                printf("id?\n");
                MYGETS(in);
                id = atoi(in);
                printf("name?\n");
                add_user(id, MYGETS(in));
                break;
            case 3:
                printf("id?\n");
                id = atoi(MYGETS(in));
                s = find_user(id);
                printf("id %d: user [%s]\n", id, s ? s->name : "nil");
                break;
            case 4:
                printf("id?\n");
                s = find_user(atoi(MYGETS(in)));
                if (s) {
                    delete_user(s);
                } else {
                    printf("id nil\n");
                }
                break;
            case 5:
                delete_all();
                break;
            case 6:
                sort_by_name();
                break;
            case 7:
                sort_by_id();
                break;
            case 8:
                print_users();
                break;
            case 9:
                num_users = HASH_COUNT(users);
                printf("%u users\n", num_users);
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("unknown cmd Num\n");
        }
    }

    delete_all();
    return 0;
}
