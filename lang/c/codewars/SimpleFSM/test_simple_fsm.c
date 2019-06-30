// #include <criterion/criterion.h>

#define cr_assert_not_null(p, s) \
    if (p == NULL) { printf("%s: fail\n", s); exit(-1); }

#define cr_assert_str_eq(s0, s1, ...) \
    if (0 != strcmp(s0, s1)) { printf(__VA_ARGS__); exit(-2); }

#define cr_expect_eq(a, b, ...) \
    if (a != b) { printf(__VA_ARGS__); exit(-3); }

#define cr_expect_str_eq cr_assert_str_eq

// #define Test(module, cas) int test_##module_##cas()

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Fsm Fsm;

Fsm *compile(const char *instructions);
int run_fsm(const Fsm *fsm, const char *start, const _Bool *input, const size_t len_input,
            const char **final_state, const char **path);
void free_fsm(Fsm *fsm);

#define S_UNINITIALIZED "<uninitialized>"
#define S_GUARD "<out-of-range guard>"

// Test(Simple_FSM_Compiler, should_pass_simple_test) {
int test_Simple_FSM_Compiler_should_pass_simple_test() {
    static const char instructions[] = "S1; S1, S2; 9\n"
                                       "S2; S1, S3; 10\n"
                                       "S3; S4, S3; 8\n"
                                       "S4; S4, S1; 0";
    static const _Bool input[]                = {0, 1, 1, 0, 1};
    const size_t       len_input              = sizeof(input) / sizeof(input[0]);
    const int          expected_final_output  = 9;
    static const char  expected_final_state[] = "S1";

    Fsm *const fsm = compile(instructions);
    cr_assert_not_null(fsm, "fsm is NULL");

    static const char start[] = "S1";
    const char *final_state = S_UNINITIALIZED;
    const char *path[len_input + 2];
    for (size_t i = 0; i <= len_input; i++) {
        path[i] = S_UNINITIALIZED;
    }
    path[len_input + 1] = S_GUARD;
    const int final_output = run_fsm(fsm, start, input, len_input, &final_state, &path);
    printf("after run fsm\n");

    cr_expect_eq(final_output, expected_final_output, "Final state output: expected %d, got %d",
                 expected_final_output, final_output);
    cr_expect_str_eq(final_state, expected_final_state, "Final state: expected %s, got %s",
                     expected_final_state, final_state);
    cr_assert_str_eq(path[len_input + 1], S_GUARD, "Written past the end of path");
    static const char *const expected_path[] = {"S1", "S1", "S2", "S3", "S4", "S1"};
    for (size_t i = 0; i <= len_input; i++) {
        cr_expect_str_eq(path[i], expected_path[i], "Path[%zu]: expected %s, got %s", i,
                         expected_path[i], path[i]);
    }

    free_fsm(fsm);
}

int main() {
    test_Simple_FSM_Compiler_should_pass_simple_test();
    return 0;
}
