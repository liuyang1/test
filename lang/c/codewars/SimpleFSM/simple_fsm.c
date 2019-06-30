/** https://www.codewars.com/kata/simple-finite-state-machine-compiler/c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>

typedef char *State;

typedef struct {
    State origin;
    State s0;
    State s1;
    int output;
} Entry;

typedef struct Fsm {
    Entry *elst;
    int ecnt;
} Fsm;

/** count times of char in string */
static unsigned int strcnt(const char *s, char c) {
    unsigned int sum;
    for (sum = 0; *s != '\0'; s++) {
        sum += *s == c;
    }
    return sum;
}

/** get token list by strtok
 * This function only for get list of new status table,
 * input only have two cases: 0, 1, so list only support length <= 2.
 */
static char **toklst(char *p, int *plen) {
    int i = 0, len = 2;
    char *ctx;
    char **lst = malloc(sizeof(char *) * len);
    for (;; p = NULL) {
        char *tok = strtok_r(p, " ,", &ctx);
        if (tok == NULL) {
            break;
        }
        if (i == len) {
            printf("ERROR: cannot put tok=%s to lst\n", tok);
            break;
        }
        lst[i++] = tok;
    }
    *plen = i;
    return lst;
}

static inline void showtoklst(char **l, int len) {
    int i;
    for (i = 0; i != len; i++) {
        printf("i=%d/'%s'\n", i, l[i]);
    }
}

#define ATOM_LST_LEN    256
char *atom_lst[ATOM_LST_LEN] = {0};

static void g_atom_reset() {
    int i;
    for (i = 0; i != ATOM_LST_LEN; i++) {
        if (atom_lst[i] != NULL) {
            free(atom_lst[i]);
            atom_lst[i] = NULL;
            continue;
        }
        break;
    }
}

/** add string to atom, if the string exist in atom list, directly reutrn it
 * With atom, we could quickly compare the string (Status) here.
 */
static char *atom(const char *s) {
    int i;
    for (i = 0; i != ATOM_LST_LEN; i++) {
        if (atom_lst[i] != NULL) {
            if (atom_lst[i] == s || strcmp(s, atom_lst[i]) == 0) {
                return atom_lst[i];
            }
        } else { // find atom list is empty, so this string haven't in atom
            atom_lst[i] = strdup(s);
            return atom_lst[i];
        }
    }
    printf("fail to put s=%s to atom list\n", s);
    return NULL;
}

/** quick compare on atom */
static bool atom_eq(char *a, char *b) {
    return a == b;
}

static void show_entry(Entry *e) {
    printf("origin='%s' s0='%s' s1='%s' output=%d\n",
           e->origin, e->s0, e->s1, e->output);
}

Fsm *compile(const char *instructions) {
    Fsm *const fsm = malloc(sizeof(Fsm));
    // printf("ins=\"%s\"\n", instructions);

    unsigned int ecnt = strcnt(instructions, '\n') + 1;
    fsm->elst = malloc(sizeof(Entry) * ecnt);
    fsm->ecnt = ecnt;
    // printf("ecnt=%d\n", ecnt);

    char *ins = strdup(instructions);
    char *ctx0, *ctx1;
    char *p0, *p1, *tok;
    int j;
    for (j = 0, p0 = ins;; j++, p0 = NULL) {
        tok = strtok_r(p0, "\n", &ctx0);
        if (tok == NULL) {
            break;
        }
        /** build entry */
        Entry e;
        int i;
        for (i = 0, p1 = tok;; i++, p1 = NULL) {
            tok = strtok_r(p1, ";", &ctx1);
            if (tok == NULL) {
                break;
            }
            int len;
            char **lst = toklst(tok, &len);
            switch (i) {
                case 0:
                    e.origin = atom(lst[0]);
                    break;
                case 1:
                    e.s0 = atom(lst[0]);
                    e.s1 = atom(lst[1]);
                    break;
                case 2:
                    e.output = atoi(lst[0]);
                    break;
                default:
                    printf("ERROR: unhandled in tok=%s\n", tok);
                    break;
            }
            free(lst);
        }
        show_entry(&e);
        fsm->elst[j] = e;
    }
    free(ins);

    return fsm;
}

/** search entry under status */
static Entry *fsm_entry(const Fsm *fsm, State s) {
    int j;
    for (j = 0; j != fsm->ecnt; j++) {
        Entry *e = &fsm->elst[j];
        if (atom_eq(e->origin, s)) {
            return e;
        }
    }
    return NULL;
}

// Return: final_state_value
// Return by reference: final_state, path (memory is preallocated)
// State name `const char *`s should be somewhere in your `Fsm`
int run_fsm(const Fsm *fsm, const char *start, const _Bool *input, const size_t len_input,
            const char **final_state, const char **path) {
    State s = atom(start);
    Entry *e = NULL;
    int i;
    for (i = 0; i != len_input; i++) {
        path[i] = s;
        Entry *e = fsm_entry(fsm, s);
        if (e == NULL) {
            printf("ERROR: cannot handled st=%s\n", s);
            return -1;
        }
        /** key concept of FSM: transform status based on internal status and input */
        bool in = input[i];
        if (in == false) {
            // printf("input %d: %s -> %s\n", in, s, e->s0);
            s = e->s0;
        } else {
            // printf("input %d: %s -> %s\n", in, s, e->s1);
            s = e->s1;
        }
    }
    path[i] = s;
    *final_state = s;
    e = fsm_entry(fsm, s);
    return e->output;
}

void free_fsm(Fsm *fsm) {
    g_atom_reset();
    if (fsm->elst != NULL) {
        free(fsm->elst);
    }
    free(fsm);
}
