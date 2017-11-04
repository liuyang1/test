#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// #define LOG printf
#define LOG(...)

typedef unsigned char Elem;
Elem elemsqrt(Elem c) {
    int i;
    for (i = 0; i != 17; i++) {
        int i2 = i * i;
        if (i2 == c) {
            return i;
        }
        // round down to nearest integer
        if (i2 > c) {
            return i - 1;
        }
    }
    assert(0);
    return i;
}

#define CHECKREG(reg)                           \
    if ((reg) == -1) {                          \
        printf("Except: uninitiallized reg\n"); \
        goto except;                            \
    }
#define CHECKOBUF(oi, size)                                            \
    if (oi >= size) {                                                  \
        printf("Except: overflow, output buffer offset=%ld size=%d\n", \
               oi, size);                                              \
        goto except;                                                   \
    }

#define CELLSIZE    30000
#define OBUFSIZE    30000
char *poohbear(const char *code) {
    char *o0 = malloc(sizeof(char) * OBUFSIZE);
    size_t oi = 0;
    Elem *c0 = malloc(sizeof(Elem) * CELLSIZE), *c;
    memset(c0, 0, CELLSIZE);
    const char *p; // PC: pointer code
    int reg = -1; // only support one register
    int dir = 0;
    LOG("exec \"%s\"\n", code);
    for (p = code, c = c0; *p != '\0'; p++) {
        if (c < c0) {
            printf("Except: try access cell[%ld]\n", c - c0);
            return NULL;
        }
        LOG("%c: ", *p);
        switch (*p) {
            case 'W': // goto next E in code
                if (*c == 0) {
                    while (*p != 'E' && *p != '\0') {
                        p++;
                    }
                    if (*p == '\0') {
                        printf("Except: overflow, cannot find next E\n");
                        goto except;
                    }
                    // one more inc in for-loop to skip 'E'
                    LOG("skip forward to index=%ld char=%c", p - code, *p);
                } else {
                    LOG("looping");
                }
                break;
            case 'E': // goto back W in source code
                if (*c != 0) {
                    while (*p != 'W' && p >= code) {
                        p--;
                    }
                    if (p < code) {
                        printf("Except: underflow, cannot find prior W\n");
                        goto except;
                    }
                    LOG("skip backward to index=%ld char=%c", p - code, *p);
                    p--; // skip one more to pair with inc in for-loop
                } else {
                    LOG("stop loop");
                }
                break;
            // to support move cell pointer to both dirs
            case '>':
                if (dir == 0) {
                    LOG("init dir=1, ");
                    dir = 1;
                }
                c += dir;
                LOG("move to cell[%ld]", c - c0);
                break;
            case '<':
                if (dir == 0) {
                    LOG("init dir=-1, ");
                    dir = -1;
                }
                c -= dir;
                LOG("move to cell[%ld]", c - c0);
                break;
            case 'P':
                CHECKOBUF(oi, OBUFSIZE);
                oi += snprintf(o0 + oi, OBUFSIZE - oi, "%c", *c);
                LOG("output %c", *c);
                break;
            case 'N':
                CHECKOBUF(oi, OBUFSIZE);
                oi += snprintf(o0 + oi, OBUFSIZE - oi, "%d", *c);
                LOG("output %d", *c);
                break;
            case '+': *c += 1; LOG("inc 1 to %d", *c); break;
            case '-': *c -= 1; LOG("dec 1 to %d", *c); break;
            case 'L': *c += 2; LOG("inc 2 to %d", *c); break;
            case 'I': *c -= 2; LOG("dec 2 to %d", *c); break;
            case 'T': *c *= 2; LOG("double to %d", *c); break;
            case 'V': *c /= 2; LOG("halve  to %d", *c); break;
            case 'Q': *c = (*c) * (*c); LOG("square to %d", *c); break;
            case 'U': *c = elemsqrt(*c); LOG("sqrt to %d", *c); break;
            case 'c': reg = *c; LOG("reg=%d %c", reg, reg); break;
            case 'p': CHECKREG(reg); *c = reg; LOG("from reg %d", *c); break;
            case 'A': CHECKREG(reg); *c += reg; LOG("add reg=%d to %d", reg, *c); break;
            case 'B': CHECKREG(reg); *c -= reg; LOG("sub reg=%d to %d", reg, *c); break;
            case 'Y': CHECKREG(reg); *c *= reg; LOG("mul reg=%d to %d", reg, *c); break;
            case 'D': CHECKREG(reg); *c /= reg; LOG("div reg=%d to %d", reg, *c); break;
            default:
                LOG("warning: skip unknown instrucment\n");
                break;
        }
        LOG("\n");
    }
    goto final;
except:
    oi = 0;
final:
    free(c0);
    o0[oi] = '\0';
    LOG("sum: \"%s\"\n", o0);
    return o0;
}

bool unit(const char *input, char *expect) {
    char *output = poohbear(input);
    bool r = output != NULL && strcmp(expect, output) == 0;
    const char *sym = r ? "==" : "/=";
    if (!r) {
        printf("poohbear(\"%s\") = \"%s\" %s \"%s\"\n",
               input, output ? output : "(nil)", sym, expect);
        printf("ruler:    0123456789012345678901234567890\n");
        for (char *p = output; *p != '\0'; p++) {
            printf("%x ", *p);
        }
        printf("\n");
        assert(0);
    }
    free(output);
    return r;
}

int test() {
    unit("LILcABNBpYDYYYYLLL+P-+W-EQNW-ELLQUTTTT+P", "2'0A");
    unit("", "");
    unit("N", "0");
    unit("N+N", "01");
    unit("<+N>N", "10");
    unit("<<<<+c>A>A>A<A<A>A<N>N>N>N", "2310");
    unit("LTTLN", "10");
    unit("LTTLQN", "100");
    unit("LQQT+P", "!");
    unit("+LTQTP>LL+TQ+P>LLT-c<APP>IIc<AP>I->+LLT+TTP<+LLTLc>BP>+LQQLLLP<<<PL+PIIIPIIIIP>>>>LLQT+P",
         "Hello, World!");
    unit("LQTcQAP>pQBBTAI-PA-PPL+P<BVPAL+T+P>PL+PBLPBP<DLLLT+P", "Hello World!");
    unit("LQQT+P+P+P+P+P+P", "!\"#$%&");
    unit("+c BANANA", "12");
    unit("+W+EN", "0");
    unit("+LTQII>+WN<P>+E",
         "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 ");
    return 0;
}

int test_sqrt() {
    for (int c = 0; c != 256; c++) {
        char r = elemsqrt(c);
        if (r * r <= c && (r + 1) * (r + 1) > c) {
            continue;
        }
        printf("sqrt(%d) /= %d\n", c, r);
        assert(0);
    }
    return 0;
}

int main() {
    test_sqrt();
    test();
    return 0;
}
