/* ref: https://www.codewars.com/kata/brainfuck-translator/train/c
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #define LOG printf
#define LOG(...)

void reallocM(void *pp, int sz) {
    void *origin = *(void **)pp;
    void *p = origin;
    p = realloc(origin, sz);
    *(void **)pp = p;
    if (p == NULL) {
        // when realloc fail, return p == NULL
        // directly p = realloc(p, sz); will memory leak
        fprintf(stderr, "fail to extend buffer to %d", sz);
        free(origin);
        abort();
    }
}

bool any(char c, const char *p) {
    bool r = false;
    for (int k = 0; p[k] != '\0'; k++) {
        r = r || c == p[k];
    }
    return r;
}

void omit(char *in) {
    int i, j;
    for (i = j = 0; in[i] != '\0'; i++) {
        if (any(in[i],  "+-<>,.[]")) {
            in[j++] = in[i];
        }
    }
    in[j] = '\0';
}

bool bipair(char a, char b, char x, char y) {
    return (a == x && b == y) || (a == y && b == x);
}

// no change: return false; change, reutrn true;
bool reduce(char *in) {
    bool ret = false;
    if (strlen(in) <= 1) {
        return ret;
    }
    int i, j = 0;
    /* use stack to support reduce code in one pass
     * - When stack is not empty, and find matched char, reduce it
     * - When find reducable char, put it to stack
     * - When find same reducable char, increment counter
     * - When find unreducable char, clear stack
     * - When stack is full, extend it.
     */
    char *lastp = NULL;
    unsigned int *cntp = NULL;
    int stkidx = -1, stklen = 0;
    for (i = 0; in[i] != '\0'; i++) {
        if (stkidx >= 0 && (bipair(in[i], lastp[stkidx], '+', '-') ||
                            bipair(in[i], lastp[stkidx], '<', '>') ||
                            (lastp[stkidx] == '[' && in[i] == ']'))) {
            ret = true;
            j--;
            cntp[stkidx]--;
            if (cntp[stkidx] == 0) {
                lastp[stkidx] = 0;
                stkidx--;
            }
        } else {
            in[j++] = in[i];
            if (stkidx >= 0 && lastp[stkidx] == in[i]) {
                cntp[stkidx]++;
            } else if (any(in[i], "+-<>[")) {
                stkidx++;
                if (stkidx >= stklen) {
                    stklen = stklen == 0 ? 16 : 2 * stklen;
                    reallocM(&lastp, sizeof(*lastp) * stklen);
                    reallocM(&cntp, sizeof(*cntp) * stklen);
                }
                lastp[stkidx] = in[i];
                cntp[stkidx] = 1;
            } else {
                stkidx = -1;
            }
        }
    }
    if (lastp) {
        free(lastp);
    }
    if (cntp) {
        free(cntp);
    }
    in[j] = '\0';
    return ret;
}

char *opt(const char *in) {
    char *s = strdup(in);
    omit(s);
    LOG("after omit: %s\n", s);
    reduce(s);
    LOG("after reduce: %s\n", s);
    return s;
}

bool validBracket(const char *in) {
    int i, cnt;
    for (i = cnt = 0; in[i] != '\0'; i++) {
        cnt += in[i] == '[';
        cnt -= in[i] == ']';
        if (cnt < 0) {
            return false;
        }
    }
    if (cnt > 0) {
        return false;
    }
    return true;
}

// one time output shift * 2 + statment + '\n'. It's less than LINELEN
#define LINELEN         4096
#define PUT(...)  {                                                                 \
        for (int i = 0; i != shift; i ++) {                                         \
            offset += snprintf(out + offset, OUTLEN - offset, " ");                 \
        }                                                                           \
        offset += snprintf(out + offset, OUTLEN - offset, __VA_ARGS__);             \
        offset += snprintf(out + offset, OUTLEN - offset, "\n");                    \
        if (offset + LINELEN > OUTLEN) {                                            \
            LOG("offset=%lx %ld buffer=%lx %ld\n", offset, offset, OUTLEN, OUTLEN); \
            OUTLEN *= 2;                                                            \
            reallocM(&out, OUTLEN);                                                 \
        }                                                                           \
}

char *translate(char *in) {
    size_t OUTLEN = 40960;
    char *out = malloc(sizeof(char) * OUTLEN);
    out[0] = '\0';
    size_t offset = 0;
    int i, last, cnt, shift;
    for (i = last = cnt = shift = 0;; i++) {
        if (last != 0 && in[i] != last) {
            switch (last) {
                case '+': PUT("*p += %d;", cnt); break;
                case '-': PUT("*p -= %d;", cnt); break;
                case '>': PUT("p += %d;", cnt); break;
                case '<': PUT("p -= %d;", cnt); break;
                default:
                    assert(0);
            }
            last = cnt = 0;
        }
        if (in[i] == '\0') {
            break;
        }
        switch (in[i]) {
            case '+':
            case '-':
            case '>':
            case '<':
                last = in[i];
                cnt++;
                break;
            case '.': PUT("putchar(*p);"); break;
            case ',': PUT("*p = getchar();"); break;
            case '[': PUT("if (*p) do {"); shift += 2; break;
            case ']': shift -= 2; PUT("} while (*p);"); break;
            default:
                assert(0);
        }
    }
    return out;
}

char *brainfuck_to_c(const char *source) {
    if (!validBracket(source)) {
        return strdup("Error!");
    }
    char *opted = opt(source);
    char *out = translate(opted);
    free(opted);
    return out;
}

/**** TEST CODE ****/

bool unit_opt(char *in, char *expect) {
    char *ret = opt(in);
    bool r = strcmp(ret, expect) == 0;
    if (!r) {
        printf("opt(%s) = %s %s %s\n",
               in, ret, r ? "==" : "!!!=", expect);
    }
    free(ret);
    return r;
}

int test_opt() {
    unit_opt("+.-.<.>[.].,", "+.-.<.>[.].,");
    unit_opt("+.-.aasdf<.>a{a}[.].,;", "+.-.<.>[.].,");
    unit_opt("+-", "");
    unit_opt(".", ".");
    unit_opt("+-asdf<>{}[].,;", ".,");
    unit_opt("++--+.", "+.");
    unit_opt("[][++]", "[++]");
    unit_opt("<>><", "");
    unit_opt("[+-]", "");
    return 0;
}

int unit_valid(const char *in, bool expect) {
    bool ret = validBracket(in);
    bool r = ret == expect;
    if (!r) {
        printf("valid(%s) = %s %s %s\n",
               in, ret ? "true" : "false",
               r ? "==" : "!!!=",
               expect ? "true" : "false");
    }
    return r;
}

int test_valid() {
    unit_valid("[][][]", true);
    unit_valid("[[[]]]", true);
    unit_valid("[[[]]", false);
    unit_valid("[[]]]", false);
    return 0;
}

bool unit_translate(const char *in, const char *expect) {
    char *ret = brainfuck_to_c(in);
    bool r = strcmp(ret, expect) == 0;
    if (!r) {
        printf("brainfunc_to_c(%s) = %s %s %s\n",
               in, ret, r ? "==" : "!!!=", expect);
    }
    free(ret);
    return r;
}

int test_large_input() {
    char *ret = brainfuck_to_c(
        ">-,+>><<.+<.<.><>+>>>>.<<,+,<,[,,],>...,><-.>,+,..,--+[><,+.]<+-.<,+-.,>,,[]+>+--+.+...---+[->>-,-+..+<-,,><>-<-<--><[][+[>,>[[,-..-]><<,>>+.,<->,.>--,,[>-,<,-+-]>--,>,,-<+<><,]++>.>]>>,<..+[<-,]<[+<.>,[]<,[+.+<[][<]-[,[-,]++-->[.+.<]][]<]>.-.,-[>[.[+[<][>[,,-.<+]]+-,-,->+-].+,,,[>[]+++--[-+[,+[+>-,-..+]][--<]]<[+.,>]]->[>]+-,-+.[.-,.>>-[>]][.<-.><[..].]+>.+,<..[[+--],+[<-<>,.>]+]<..>--.+-..-.-]<],-]].>.-.<[->[[<>,<.]+<[-+<[,-]-.[+-.]->+>[[.<-][[,++<>,[>.+[,,[>.][[>][],><.<-+-[--<[.-]>,<.++-.->[>-+<>]<[++>-[-,-[>.>[++]<].>,+,-->,[+>]+.],[.[<+[[-+,,,]-+].-<,.<+<+]++-<.+[->-<,<>>-+[>..+[,],>+<<<<]].]..<+]+,->>>><]]>.-,.>]]-<..,[-[.]+,--,]>,-...,><.[+-,>>,[-,<],-..+[.-,[[],.<-<[++>,>[[<-<+].[+>,+][+>.<[,<<,[>[]+,,>]><[<<]>+]<<]+-.[-]>]<]+]<-,[<-,+,..-,][-,..>-]+[.+]<.<]<].-,[<<>--+],[]<[[,][]++<-<++>]+,,.-]<+][.--[,+.+,....[-+<.,.>,,,<-]]>[+.[-+>>,>>>[<<+>>[[[]>,],>,>>[>.>-.<>[>-+]]<<[<[]]+>[><]<]<,,..-[<[+.][],.>><<[.-+]<.>.]<[--,]++<++-[<>>,<>,<.><+<+>[.,,,-]-..-<>[+>]+<---<-],<<<[,-.,.....+.<[-->-<>+>-.],<<.][>>.<,>>.<[>,+<-[.[.+<,+>.+.[<.[>>>+,[--+.]>.,+.[[+<].<<<+],--.]],>+]]>+[.+><]-,-].<[.[->.[<>[<,,[],>[].<[>>-,.<,.]]<-<,[.]-[]]+[-+[,<>.],.-+<,-->>.+>+,-<]][]<><][,.+.[[[[<-.>>.<+[-..+[.]>.,><,+,..><><].][-,[[,><-,.-<.[..],.[.>,,<><<]>[>[+][-[>]<><>>[<<-<<>.]<.[-+.+<><,+]-+,>-.[<-]--<-<.>->[]].-,-[.[<.][.-[-+,>[]+--]<],,[+.>]<>>,--<[<<->,[<,<<[,-++[[<<[>>].[-><+],],+],<<>[<-,><<<[>.><[+>].[+.[+,[..]++.<-,+[,>>[+<,,<,+]-[<-<[[.+,,-<,-+<-<[>...[]-<.>,>,<]]]>,+.+.-+.,<+.,....>,-,<,<<-+],>-<>][]-.+<.-+>[+<>[]+[.]<+]+<+[-.-<[<[,->+<,>[[>]-.]-[,].[].>,--,+>-.[,[>]+>+--][+<..[.[.+-.,.>,>][.-[].>[+->.>+,-<[<>[,<>,<+[[+..-].],,<-[<<+>[<.>.]-].<].<-,>,-,-<>-,..<].[>,[<+-[]]>[.-+,.>>.[[<<<,.]][,+[]-<<.+-]]>+][.].><>.+,><.+]<][-,,,<[-,+[+[<<[++,[<+]>.,[-[>]>[[][<>,[>,+,<-.>++--,<-+<->+->.-,..]->]+,<,]<>[.-][,>-].+[<.[>,,+.-]+]+[[<+--<],-.][<++...>>>>[+<,>].,->]]><[+-]-+]<[]-.,<<.<,]<-[+><+,>->..,]][-[[+-,<<<,>[,,>[<][<[<,,[],,][,]>.+-]<>-[.+><[+[.]+,<<<][<-[-<+-[+>><<.<<<.<<.]>..]>+]<,.<,]>[[+[.<<+-+++].[>-,+>,[>.[[->>+-...,>[.][],>---+][><[]+++[][[+>,<-+][,><-.[,[<[<,>.>,][<>,<]<..]-.<>.<,<-[-,<<,++>]]]<-[->.<<<>.,,[][],-.+><-[>.<[<<++>[.-,+,.,,]><][+>-]+]->>[,,-.+[.[>.>-.,,<]],+.[<],]-[<>[+,.,-+],+,,,]-++<[+[,-[,,+.<-[][]>,<,<+>]>+[].[.],].+-].>+>>[+]>,-+[.[>.>[]>[[]>]<,]<,<,+[....->-,.,[.,[..++>[[,.>,-.<]><[->+[>]+>.....<,<.<,<->-[[+[>>>+[.>>,,<<]>]-.]->][,.-<>-]+]>.-><>,.[->,+.><]>]->[.+].],>.][[>>]-]<>,[,<-[..,><-,[]+[].,,<]]+]][<<[]+..,][-,[,]>+,-..>[]-+<>[-[[],-]+<>,[-<>>>,.[<-[.>-<],,,<+[>[+[<>,],<>[++].<,.-,.--++.[>,[<[].>][>.-[+,><>-<<+><],]-<]<+.,]+]-><[+<>-[<]++>,++,>.+--[+[[<.><>[,+><>.-,<.<<<,]-->>+[[.<-[].-<>>>-][.<,,-[]>.+[[<.-+]+-+<-+>.-<<+-<>.+>>+].,]+--+..+<<+.[>],<<+[-<>[-<,<>,+],-]]<]>-,[+>.+<->,.>],+.[]][-+],,,],.[>[>.>+-+],,<.<+],,+]<->,>>+,-][>+..,]>.+<-]>.++>[[-<<.<.]<[,]-,<-,[+.<>,+-,>-<-[.[].+->..[,.[,<>].,-+,[+<+],][-+[--[[,.[]<]-<+[,]-.<,+]<+>>-><.]-<.+]<[><+..<-<,-<[-[+++>][,[<<>.[..>-[>+[+->><+[.>,>-+,<<<-.-[][,-,,-.><<.-.[<][<+,+<,--.,[+].]].]-.[>>],><[.>->]>,>-,>],.+]>-,+><[->,<[+-<[-,[+]-+<>,>[,[,>]],>+]<.,[]]].[,>-]>.]..[-,]][,]+[.,.++[,-<]]>.[.,,+]<.>[[<],.[[<>>><+,.,<.>.+><->.-]]-[,-+,+]+,-[]--<<[.][]<-+],,><+.<],-+[>.]<><>+[]+,-.]]],][>>>>]+.[-.[[<+>--,<,-><,].-,.>[.+],.>+.[><+,>.+-[-<,>.[>]><+,],>[<[][.---..>><-..,.-><.><+<,<[>,,.--[[-<-+,[,+[[]]+.]<-<<.[.->,->],..+.+][-,,..+<][],<>-[]+<.,<>.>.+>+]+,+<+->,>,<>[[]<-][+<.+]<<[,[<+>,.+.],+<>-<,..,<].<,,>]<[<,[-->.]+.[,,<[+,<>--].+-+>[,><[>.--,+,>>-,,>-<-.>-.<-.<+<-,,.[[][[[,>-<-]>>]+[<-,[>[<+][],.>[]->>[<[][[.[,.-..],],.]+<.<>+,>[.>,-],,-+[>>[>>[,[-+><>>][]].[<+]]>],]+.<>><,,+--<.<+->[[]+><+]].-+,[-+++.+>>-]]+>-<><<>-..,+.-[[<.[,-]><[.,[]-+,>,+.<-[<-[.<.>,-.,.[>.>[].]<+.<>-[,-[,...+-<>]+[[]<+].<[,<.,-.<.,+->,.++,+>+<->]-[,-+.,<-[-+>[]-[[.<,[,+<].,>..[+-[.-]+]<-><[-<<[.><,+><]].,.<]>-[<->-<.+]<-<>[]].[.[.<+.>-+++[+[+]]+++-],->+>>>,.][.,>][[->>+<.]<<,+,>[<>,.,[,+->[<+].,.-<-]]>.>[]+.<.>+<-<,,,<+.-<+[[>-,[]-[].[..].+->+-+-,>>>]>-.>><-<>,<>,.>,.<--,[.,+,>,<-+<<,[<][,.->><<.>]-<>.<+++.[.<<-]<-,><-[-+>,[[<-.,],+->-..,],]<>+.,,.],->-]>,,[+,.<.[,-,,[[.].>[<+,.]-..<[<.]+<[.]]--<-,]-,[-]].+--.+-,.,+>+[-]]-..><>[]<[>.[.>]-.><>+,[<,[-]<]>-,]-[---,-<][]+,+[<+-><]>-],][.]++.--+[+[.]]-+-]-<-<[<[][<[>[.]><>-.+>.],]>.>[+.,>]<++<+,-,<>+,<[>,[.><[<.]]>].-[[>]..+[.<[--<[,]]+<,.>>]+[,>.-->,>+][<<]<>]<,]><[[+.,,,[,]>>[>,->-,.<,-][]+,,+]><<-<->-,-->->..><<,[++[,,>-[]+<+-.<]]+]],-+-[+[+++>]-[]<+-,]][<].<,++<>-<<,>>,-[<->++].-,>+.,,>+[<><->.,<[+>><<]<]][<+>..[,]-+>>,]->]-++[][<[><.]][.+<+<.+>>>->].,-,++,<],>-,>[>][+--,+[[.+[+-<.,-],-,[,<>,,]<[-],>,+.>+-,.[>..,-,>.-[]<+][>,<-[+>+[,<->.]><,]<--,].-,]-->,+]-<].[<.<+[]<><][++>>-,]+[-.->,,<>+[[+.].+>[,>[.->>>[-[,<<+[<[>.,[..[,]-,<>-]]<[<<.<,[<,+><]]>,[+[-<<>.<.+[<<--.>,.<>.-]<><[<-[>,->]]>+.+,+[.[,.---+][-<.<[>.,,,+<[]]][,[.+<..>,[.,->>[.+.+,<<-].]+-<>+-++[],><]<>.][[>]>>]>+-,>..[->>,[>,-.<<[>,<+>+>-<-<-]<>>]>-]--.<<]]>>]-.]<[>+<.-,-<,.[]-->,,+]<[.>]],<>],+[.<+>[>+.+...[>.->>+<+]+>-]]>[,[]-+>,,,,.,>,]>].,<].<[,.<>-]-..><[<-><.+-.]]+[-.]+[>.+[<<.],....<<.-<-.->,+[]+-,]<+-<[]-,>-[,.+<+[],-><<[.,.,-..][,][..->]]+[<.<<+]+]<-,>]+.>[>.>>]++>><-+-[<.[+<>[[<.,<+[>[<,+][++<[<,+.>],]<-[[+]-<.+.[[+>+[,]>><>>,-.<[--<,]...]-[>>,,+.<>[<+[+,[<>.][][.[.[[[.[-[,>->.>.>].]<]<+<<]]-+.>,[<[,],><>+>-].]+,]..<],<>.].-.-[.>++-[>[><>.<><,]>[.]>,,..,.<>,<.>,+[[++,,,+-][-].,[--,-<>[,.[<]--[,,+[,[,>[>[>.>,+->.]].>-]-].].++<>,[<].><[>,>+,.>.[>>>],,.][.],+],+>>[]>[--+,<-.<,<.,[,],].,][<,].,-,]<[><<[+-].[<+<+>],[-+..].+,<[<+<[+]<][]><]+[].,[[-..-,,]-+<--][]<[++--,+..[.<+-]>+<[>,[.],.<.++,.+->][->>.]+.>->.[<[[>>+<[]-<+--+>>-]->,<+>+]<+.]][--,[+.+].,[<-<+[+[,.,>].->[[.--]+>><,+-][,.,-,,+-,],-<[,.]-[,+,.>[.<]>,>]]<<->>[>.,<+-]+.+<,[++[>+]].[+.+,-.-.[[]>].<+,,]<,[,>>[[.[+-><<+[>.[>,-,<[--+].]--]-+][++-+],[+.]>><<>..]>>]].--<<<+[-+].-+,]+-<<,<]]+],]-,<+<+,.-><+--,<>.,,+>]],<[+,-.<,]+>]]<+[]].+[+,,],<,[<+[-[,]>]]>+>.+-+]-[]<-].>,+>.[+[]]<[<],+><[]-.]..<.,<.<,-+><,<.]++.-<]>[,<+[[-.>-+-],-<[,[][<.-[->-<-,--,,-],+]-<+>-]><+]><]<..[<.][.>-<<+>+][+.<-><-[][-,.+,,[>]++].++]+]->[[+>+<.-+,-,>]>,-+[-<><[]>>>.+>+<+.[[-]<><-,[].[<]-+[.+.+<[<-.,,]+[-.]>+.,-,<]+>>.]<++<]].+.->-,<+.],<>[<],]++,[[[-,>[.<,[+>>+,[,]-,]>-+,[>[>++.<[]+>+.-<[>[]>,-],-+[.,[>]>[[<+,-]][,.-+[<+>-[-+.<-[>-,-,--..]>,[--.].-.<[]>.<[-+><,<[<,[<+-][>+-,>]+].,+,[>,.+<<<-][<[++-.+]<,--,..][-<-<-<-,<,.<+...<+->.,>,,++-[<+.<[+,+->],>,,>>[>-]<>-]<.,+,<.,,>--<>,]+,<--,>-+-<+.+[[-->-[+.-]+<.[,<+]<-[.<..[,].<,,[>>.->>,++].,,.-+<,+,>+<,[].--[,<[..]][[->,[<[,>[-[.>.,[[,[[>,+<+-<-+>[+..+.,],,>>[<--,]<[.,,.<[][<><<>.<-[.+,+,><--[>>[]<[>]-[,>-+<+..><<,<<[,,[]>+<[<.]+-<[,>[<>-<,,[-<]]<+-++[,-.],,-.,]..+.]],<[.,][[[->]-,,.]+<[<.>+>-]<[-,-.,,<-,...,+-<-[[.,-[.[>>+<,[<<,++<.]<[,+.[>><>>]][<[.,,.<--,[>>>-><--><-[.,,+-.-<]-+<-+,]<+>+,,-<[[<.-,>>][,.].+-[+-[].>[-.][,[>-,-<]<[[-.,>[-],<--[.]->...,.>-].,++[]+<.-[]>>>[-,-[<[[][]+<-++[.++,<>][[][].+><->>>>+.-<]<]<-<<,+]<--.+,<+[,]+>>[.[<]]<[[+[<.>+],[--<[[-][][[]>],,.>]>][,<,<.<-+][.+-,],>><-+-]],[[[[<<]<>,++<]+.+,<]<]<.+>.[>->.]<.[+]<+-.-<[,[+>>.+],<>],<,[+[],.<>[-,+[+]->-.]<><->+<.[,-[+-,-<->,<[.><.[,[]>]>-<]>-]]<,>.+.]--[-,[]-+-<,,][].<.]+]--,-+][,<-][>>.[>.+.>-]+],.,<>-+,.]][[[.-<.,,[[<.-,-..[>,]>[[<,.++],>-<,<[+]>]+[,>--<]]+[<<<-]..<<]<+..<-[>+><+<[+.>>>+--,.>>.]<<<+>>[+[].-.-.]+.<[.+]>-+<]<-[->[[-.<,>>[.><>.]<[<]><[.->+,,+<><>.[.[,[-.+<,>,[,<+>[-->+<-[.>+].,],,[+]][-><.>[[<<]][.+[+-]<[>.]]+<<-.-<>-,+-[[.--++[>+><<<<]<[++[+>[]][<+,,<<,-,>]]]-[+....]<-<.-[.],<.->>-+]->[+,<->>].+.--+-]].->,-]+][>.-.<,.[.<-.>+<.<,,].>+.]>++>]+<.[]]<>-.,>[,<>+,->>]<[,>]<[<]>[]+]<>.<,-,.-[]+,-++<-.<<-[-.->,+,,>[-..<>]><,]-[->-+.],>+-<<.+<]+--[+,-,,>+<]>+.,->].,+.[]-],<+,,[..-<..+<..>>>+><+>]]..>.<+<-+>-]>[++,+.++.<..],+.[,>,]++>,-,[[[]]>--].[,-[<[<.>+>,.,,+>[,,-.,.[.,<+>>-<<-]>.[-<+,[<+,]]<<,-]>[+,+[-+[[],.[,<--,>]-,>+.>,-],[<-[<[[,,[]+,-[-+<+,][[]>]>><+].>[-,+><>]-]><<><[,..>.+.+[,],>+..[--,+><[..+-[-+.+-<<,,<>.,[.,>].,.>.+[]<.<+[+--<+>.<,>,]]->,>-+.]][[,<[[-+.]<>.+.++].]>-.]<],>>[.><[><[-+,]>+.+><-[<.>[+<,-++-.->+,.],]>]<<[-[]+<[.<..<+>-]]>.[[-,-],<[+-<>>++<>-,[+].-.,>[<>.>],>,>[<>,<..+>.].-]>-[-<,.<++[],>-.+>.[<.>.[...<.,-<].].<>->]+,+[,-,.]<--+-]<].-->,-[>[,>[<<<,.]]+]+]<--<[<,[-.+->..-,[[,->.,<.+,.,><>[.>[[<-<--,<,.]-]-].<[..<>+<]],]<,].<+]<<,+-[-].[,>[,.][][]-+,<-[>],]][-[++[-[,.+.,.],<-++--][-+[<-<<,],[]],-],[+.-]>>-]...[[-,+[-.>]]]<-+>++[>.>>-.<-[,>[][.,<],]->><],+].<[.,.<]-<>.-<,]+[+]-.[[<+><>->+.<].,>[[[[<[<,,+<.]<+.[]+],].[<-,+<--,->+[[..,[++]],[<>-+>.+].-+.[.]>.+.>.,[,>-]+<[-<->++.][,[.-<[[<][++]+]]<.>->.>]],[[>,-.>]<[<]<,]-,.,.<].-.>]<++<<<,--.,+,]>.]-.,<+<,>+[[<>,><],[.<+,+++,[]+++]<<[,-+<[]]<-+..]->++,+[.>[<..,<<+<>]>..,-,>..][>-[..>->],].>,[[-]<-[-]+.+]][+].-[>[[-+.<-->]-<,.>.[.]..<][.+,<].-]+-,<[,[->+,.+-.-++][>]<[>,-.<-][[[<,,,[[,>+].[>>.].]>.-+,],>+<<.++-[][].--,[][+,.-<<[.-].+>,>].]<-]>,<<.,++[<,,.><],].>.+-+.><][>[..>>]]]+>+,][>],<.+>,-+-<.-.[.>---<<[+]]>>-<[]-<,,[>],.<<>.[]><,.>.]+>+,<,]+-.,<+]+>-.]]-[[>,>[-<]+++<<[-<,>>,].,.+.+.>].>]+>+<<][+.[+>-.-,]--]]-,[>.>]>-[.++[<.-.+[-[,..++[+>>-,,>-<><--.,>].,.>><+>,<>,...-[-[.+-]-]]+[<]>[><[.],]>-<,>.[,>.+-]+>],<[>]]+.-]<+-,,[<[[,-.-++-<<,>,]]>>[>-[>+----[]+-+,[+[+-<,.<<<+><<>,-,.[]>[],-+[++.+-<+<,]..[->,],,[,+->,][[+.+.]-[-,--]+><+--<.>][]]+,++,[+.],+,-,]]]+<]>.+->.]><].<]>,[].[><-++>-+>+>,+,>[][.>..]->],++>+>++<+]>.-.<,--]<],<.]--.<<[<,>-+,.,><-.[<,,--,<+[,++-[>,>[-,>-<<<+[]>-.+-].>,[]>,+],[<++[]+><+<-]].-,+-.+],>]-,---.+,-<<>]>+>,.+[-+>[--.[+<+-[+<[>[-,+,..>].[]]>]++.<>+>],+<-[-]+,],-><<-<<>.]+-][,.[<.,->]<<<>-+>-][<+[[,<.[-<<+<>]>[>>><[[]>++<+<<+,,,<.[>[+-[+->,-+><[<]+]>]],.,[,[]+[[>+<-<>-][[,>>]>.[,,,--.<],]+>+,],[[,--.-,-,,.+-].[+[,+[+->+>-->]<--+.]<-]>++].[+-<[>,<+-]>-++[..<>>.]-<[>,->,-+-+-[[.-],>+,>.]>[+<,][.<,,,.<,,-.>,<,>].>,[,.-[++,,.<]+<<<.]>[<-,>>]><+,+[<]]-[-[<,.<.-,,.],[+],,].[].+-]>->[-[,->+><>[>>--+].,[.<-,>]]-]<<-.]<.[,.[<>[[-<-.,],++[><[,.<><-,<>.,-.,+>[.>[>,>+]>>.+]]+[-.+,-]+-+<<]---]-,-<>,+>[-,><>]-.-<-><[>.-.<->].,-.+]<,]-.+,+[,[.>[[+->>,+>,>,[<<>].]][>,,>>.][<.]+],+,[->],[+>-<+<[<]][[><+,,++]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]");
    free(ret);
    return 0;
}

int test_translate() {
    unit_translate("++++", "*p += 4;\n");
    unit_translate("----", "*p -= 4;\n");
    unit_translate(">>>>", "p += 4;\n");
    unit_translate("<<<<", "p -= 4;\n");
    unit_translate(".", "putchar(*p);\n");
    unit_translate(",", "*p = getchar();\n");
    unit_translate("[[[]]", "Error!");
    unit_translate("[.]", "if (*p) do {\n  putchar(*p);\n} while (*p);\n");
    unit_translate("[][]", "");
    return 0;
}

int test_memory() {
    int len = 4096 * 100;
    char *s = malloc(sizeof(char) * (len + 1));
    int i;
    for (i = 0; i != len; i++) {
        s[i] = '.';
    }
    s[i] = '\0';
    char *ret = brainfuck_to_c(s);
    printf("result: %ld\n", strlen(ret));
    free(ret);
    free(s);
    return 0;
}

int one_perf() {
    int len = 4096 * 10;
    char *s = malloc(sizeof(char) *  (len + 1));
    int i;
    for (i = 0; i != len / 2; i++) {
        s[i] = '+';
    }
    for (; i != len; i++) {
        s[i] = '-';
    }
    s[i] = '\0';
    unit_translate(s, "");
    char *ret = brainfuck_to_c(s);
    printf("result: \"%s\"\n", ret);
    free(ret);
    free(s);
    return 0;
}

int stack_perf() {
    int len = 40960;
    char *s = malloc(sizeof(char) * (len + 1));
    int i;
    for (i = 0; i != len / 4;) {
        s[i++] = '[';
        s[i++] = '+';
    }
    for (; i != len / 2;) {
        s[i++] = '-';
        s[i++] = ']';
    }
    s[i] = '\0';

    char *ret = brainfuck_to_c(s);
    printf("result: \"%s\"\n", ret);
    free(ret);

    free(s);
    return 0;
}

int test_perf() {
    one_perf();
    stack_perf();
    return 0;
}

int main() {
    test_opt();
    test_valid();
    test_translate();
    test_perf();
    test_memory();
    test_large_input();
    return 0;
}
