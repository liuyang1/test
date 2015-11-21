#include <stdio.h>
#include <getopt.h>

#define diemsg(...) fprintf(stderr, __VA_ARGS__);

void showArg(int argc, char **argv)
{
    int i;
    for (i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
}

/*
POSIX conventions

# option name should be single alphanumeric char. Multidigit options should not
    be allowed.
# All options should begin with a '-' char.
# For options that don't require options arguments, it shuold be possible to
    group multiple options after a signle '-' char.
# When an option does require an option argument, the argument should be
    seprated from the option by a space.
    However, when option and operand could be in the same string.
# If an option takes an argument that may have multiple values, operands is
    separated by commnas or whitespaces.
    And this need splity by yourself.
# Options shuold come first on the command line, before operands.
# '--' indicate the end of all options.
# The order in which options are given should not matter.
    If support override, the last one wins.
*/

/* getopt_long convenitions
 * every short option should also have a long option
 * Addtional long options need not have a corresponding short option.
 * Long options can be abbreviated to the shortest string that remain unique.
 * options arguments are separaed from long options either by whitespace or by
 *      an '=' sign.
 * option and arguments may be interspered with operands on the command line.
 *      getopt_long will rearrange things so that all options are processed.
 * program can choose to allow long options to begin with a single dash.
 */

/* memory for argv
 * argv is pointer to pointer, better write with `char **` type.
 * argv[argc] == NULL
 */

/* getopt
 * int getopt(int argc, char *const argv[], const char *optstring);
 *
 * extern char *optarg;
 * extern int optind, opterr, optopt;
 */
int get_short_opt_arg(int argc, char **argv)
{
    // first char is +, it will stop at first nonoption arg
    // first char is -, it will return nonoption with oc == 1
    // after +/- or nothing, next char is colon, then we can distinguish
    //      between invalid option and missing option arguments.
    const char *short_options = ":ab:c::h";
    int oc;
    // opterr = 1; // default, getopt print error message for invalid options
    while (1) {
        oc = getopt(argc, argv, short_options);
        if (oc == -1) { // END of getopt
            break;
        }
        switch (oc) {
            case 'a':
            case 'h':
                printf("setting [%c] flag\n", oc);
                break;
            case 'b':
                printf("setting [%c] optarg=[%s]\n", oc, optarg);
                break;
            case 'c':
                // optional opt arg
                // -c   valid, no optarg
                // -c3  valid, "3" is optarg
                // -c 3 invalid, optarg == NULL
                if (optarg == NULL) {
                    printf("setting [%c] with NO optarg\n",
                           oc);
                } else {
                    printf("setting [%c] with optarg=[%s]\n",
                           oc, optarg);
                }
                break;
            case 1:
                printf("when optstring[0]=%c, optarg=[%s]\n",
                       short_options[0], optarg);
                break;
            case ':':
                diemsg("%s: option '-%c' %x requries an arguemnts\n",
                       argv[0], optopt, optopt);
                break;
            case '?':
            default:
                diemsg("%s: option '-%c' %c is invalid: ignore\n",
                       argv[0], oc, optopt);
                break;
        }
    }
    argc -= optind;
    argv += optind;
    showArg(argc, argv);
    return 0;
}

/* getopt_long
    struct option {
        const char *name; // name of option, without any dash
        int has_arg; // enum, no_argument, required_argument, optional_argument
        int *flag;
        int val;
        // if flag == NULL, then getopt_long return VAL.
        // if flag != NULL, *flag <= val, getopt_long return 0
    };
    array of options, should end with all 0 field. so it will find end of
        options.
 */
int get_long_opt_arg(int argc, char **argv)
{
    int isHelp = 0;
    struct option option_list[] = {
        {"all", no_argument, NULL, 'a'},
        {"bang", required_argument, NULL, 'b'},
        {"cee", optional_argument, NULL, 'c'},
        {"help", no_argument, &isHelp, 1},
        {0, 0, 0, 0}
    };
    char optstring[] = ":ab:c::h";
    int oc;
    while (1) {
        oc = getopt_long(argc, argv, optstring, option_list, NULL);
        if (oc == -1) {
            break;
        }
        switch (oc) {
            case 'b':
                printf("setting [%c] optarg=[%s]\n", oc, optarg);
                break;
            case 'a':
                printf("setting [%c] flag\n", oc);
                break;
            case 'h':
                printf("setting [%c] flag\n", oc);
                isHelp = 1;
                break;
            case 'c':
                if (optarg == NULL) {
                    printf("setting [%c] with NO optarg\n",
                           oc);
                } else {
                    printf("setting [%c] with optarg=[%s]\n",
                           oc, optarg);
                }
                break;
            case 1:
                printf("when optstring[0]=%c, optarg=[%s]\n",
                       optstring[0], optarg);
                break;
            case 0:
                printf("auto setting *flag <= val\n");
                break;
            case ':':
                diemsg("%s: option '-%c' %x requries an arguemnts\n",
                       argv[0], optopt, optopt);
                break;
            case '?':
            default:
                diemsg("%s: option '-%c' %c is invalid: ignore\n",
                       argv[0], oc, optopt);
                break;
        }
    }
    argc -= optind;
    argv += optind;
    showArg(argc, argv);
    return 0;
}

int main(int argc, char **argv)
{
    showArg(argc, argv);
    // get_short_opt_arg(argc, argv);
    get_long_opt_arg(argc, argv);
    return 0;
}
