char* getHint(char* secret, char* guess) {
    int i, j, len = strlen(guess);
    int bull = 0, cow = 0;
    bool *baSecret = malloc(sizeof(bool) * len);
    bool *baGuess = malloc(sizeof(bool) * len);
    memset(baSecret, 0, sizeof(bool) * len);
    memset(baGuess, 0, sizeof(bool) * len);
    
    for (i = 0; i != len; i++) {
        if (secret[i] == guess[i]) {
            bull++;
            baSecret[i] = baGuess[i] = true;
        }
    }
    for (i = 0; i != len; i++) {
        if (baSecret[i]) {
            continue;
        }
        for (j = 0; j != len; j++) {
            if (baGuess[j]) {
                continue;
            }
            if (secret[i] == guess[j]) {
                baGuess[j] = true;
                cow++;
                break;
            }
        }
    }
#define LEN (10 * 2 + 2)
    char *r = malloc(sizeof(char) * LEN);
    snprintf(r, LEN, "%dA%dB", bull, cow);
    free(baSecret);
    free(baGuess);
    return r;
}
