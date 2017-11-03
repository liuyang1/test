#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char tbl[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 169, 196, 225};
// round down to nearest integer
char charsqrt(char c) {
    int i;
    for (i = 0; i != sizeof(tbl) / sizeof(tbl[0]); i++) {
        if ((i + 0.5) * (i + 0.5) <= c) {
            return i;
        }
    }
    return 16;
}

#define CHECKREG(reg, sp)                                                        \
    if ((reg) == -1) {                                                           \
        printf("Except: cannot copy from uninitiallized reg on sp=%ld\n", (sp)); \
        return NULL;                                                             \
    }
#define CHECKOSS(offset, size)                                                          \
    if (offset >= size) {                                                               \
        printf("Except: cannot write more data to output buffer offset=%ld size=%ld\n", \
               offset, size);                                                           \
        return NULL;                                                                    \
    }

char *poohbear(const char *sourcecode) {
    size_t cellsize = 30000, osize = 30000;
    char *ca = malloc(sizeof(char) * cellsize), *c;
    memset(ca, 0, cellsize);
    char *oss = malloc(sizeof(char) * osize);
    size_t offset = 0;
    const char *p;
    int reg = -1;
    int direction = 0;
    for (p = sourcecode, c = ca; *p != '\0'; p++) {
        if (c < ca) {
            printf("Except: try access cell[%ld]\n", c - ca);
            return NULL;
        }
        switch (*p) {
            case '+': *c += 1; break;
            case '-': *c -= 1; break;
            case '>':
                // to support move cell pointer to both directions
                if (direction == 0) {
                    direction = 1;
                }
                c += direction;
                break;
            case '<':
                if (direction == 0) {
                    direction = -1;
                }
                c -= direction;
                break;
            case 'c': reg = *c; break;
            case 'p':
                CHECKREG(reg, p - sourcecode);
                *c = reg;
                break;
            case 'W': // goto next E in sourcecode
                if (*c == 0) {
                    while (*p != 'E') {
                        p++;
                    }
                }
                break;
            case 'E': // goto back W in source code
                if (*c != 0) {
                    while (*p != 'W') {
                        p--;
                    }
                    p--; // skip one more to pair with inc in for-loop
                }
                break;
            case 'P':
                CHECKOSS(offset, osize);
                // offset += sprintf(oss + offset, "%c", *c);
                oss[offset++] = *c;
                break;
            case 'N':
                CHECKOSS(offset, osize);
                offset += sprintf(oss + offset, "%d", (unsigned char)*c);
                break;
            case 'Q': *c = (*c) * (*c); break;
            case 'U': *c = charsqrt(*c); break;
            case 'L': *c += 2; break;
            case 'I': *c -= 2; break;
            case 'T': *c *= 2; break;
            case 'V': *c /= 2; break;
            case 'A': CHECKREG(reg, p - sourcecode); *c += reg; break;
            case 'B': CHECKREG(reg, p - sourcecode); *c -= reg; break;
            case 'Y': CHECKREG(reg, p - sourcecode); *c *= reg; break;
            case 'D': CHECKREG(reg, p - sourcecode); *c /= reg; break;
            default:
                // printf("warning: skip unknown instrucment=%c\n", *c);
                break;
        }
    }
    oss[offset] = '\0';
    return oss;
}

bool unit(const char *input, char *expect) {
    char *output = poohbear(input);
    bool r = output != NULL && strcmp(expect, output) == 0;
    const char *sym = r ? "==" : "/=";
    printf("poohbear(\"%s\") = \"%s\" %s \"%s\"\n", input, output ? output : "(nil)", sym, expect);
    if (!r) {
        for (; *output != '\0'; output++) {
            printf("%x ", *output);
        }
    }
    return r;
}

int test() {
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
    unit("LILcABNBpYDYYYYLLL+P-+W-EQNW-ELLQUTTTT+P", "2'0\001");
    return 0;
}

int main() {
    test();
    return 0;
}
