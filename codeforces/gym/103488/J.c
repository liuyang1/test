#include <stdio.h>
#include <assert.h>

const char *ratingName(int rating) {
    switch (rating) {
        case 0: return "Newbie";
        case 1: return "Pupil";
        case 2: return "Specialist";
        case 3: return "Expert";
        case 4: return "Candidate master";
        case 5: return "Master";
        case 6: return "International master";
        case 7: return "Grandmaster";
        case 8: return "International grandmaster";
        case 9: return "Legendary grandmaster";
        default: assert(0);
    }
}

int score2rating(int score) {
    if (score < 1200) {
        return 0;
    } else if (score < 1400) {
        return 1;
    } else if (score < 1600) {
        return 2;
    } else if (score < 1900) {
        return 3;
    } else if (score < 2100) {
        return 4;
    } else if (score < 2300) {
        return 5;
    } else if (score < 2400) {
        return 6;
    } else if (score < 2600) {
        return 7;
    } else if (score < 3000) {
        return 8;
    } else {
        return 9;
    }
}

int main() {
    int t;
    scanf("%d", &t);
    for (; t!= 0; t--) {
        int n, k, r, nr, nk;
        scanf("%d%d", &n, &k);
        r = score2rating(k);
        for (; n != 0; n--) {
            scanf("%d", &nk);
            k += nk;
            nr = score2rating(k);
            if (nr != r) {
                printf("%s -> %s\n", ratingName(r), ratingName(nr));
            }
            r = nr;
        }
        printf("%s\n", ratingName(r));
    }
    return 0;
}
