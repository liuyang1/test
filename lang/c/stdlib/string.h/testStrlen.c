/**
 * @author      : lyz (lyz@$HOSTNAME)
 * @file        : testStrlen
 * @created     : Thursday Feb 27, 2025 20:00:12 CST
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PNL_STRING_LENGTH 10

typedef struct {
    char pnl_str[MAX_PNL_STRING_LENGTH];
} PanelInfo;

// 错误的实现
void copyStringIncorrect(const char* someString, PanelInfo* stPanelInfo) {
    size_t u32StrLen = strlen(someString);
    if (u32StrLen >= MAX_PNL_STRING_LENGTH) {
        printf("String too long, truncating.\n");
        return;
    }
    strncpy(stPanelInfo->pnl_str, someString, MAX_PNL_STRING_LENGTH);
}

// 正确的实现
void copyStringCorrect(const char* someString, PanelInfo* stPanelInfo) {
    size_t u32StrLen = strlen(someString);
    if (u32StrLen >= MAX_PNL_STRING_LENGTH - 1) {
        printf("String too long, truncating.\n");
        return;
        strncpy(stPanelInfo->pnl_str, someString, MAX_PNL_STRING_LENGTH - 1);
        stPanelInfo->pnl_str[MAX_PNL_STRING_LENGTH - 1] = '\0';
        return;
    } else {
        strcpy(stPanelInfo->pnl_str, someString);
    }
}

// 检查字符串是否正确终止
bool isStringTerminated(const char* str, size_t maxLen) {
    for (size_t i = 0; i < maxLen; i++) {
        if (str[i] == '\0') {
            return true;
        }
    }
    return false;
}

// 测试函数
void runTest(const char* testString, void (*copyFunc)(const char*, PanelInfo*), const char* funcName) {
    PanelInfo info = {{0}}; // 初始化为全零
    copyFunc(testString, &info);
    
    printf("Test for %s with input \"%s\":\n", funcName, testString);
    printf("Result: \"%s\"\n", info.pnl_str);
    printf("Properly terminated: %s\n", isStringTerminated(info.pnl_str, MAX_PNL_STRING_LENGTH) ? "Yes" : "No");
    printf("Actual content in memory: ");
    for (int i = 0; i < MAX_PNL_STRING_LENGTH; i++) {
        printf("%c", info.pnl_str[i] ? info.pnl_str[i] : '_');
    }
    printf("\n\n");
}

int main() {
    const char* testCases[] = {
        "Short",           // 短于限制
        "ExactNine",       // 恰好 9 个字符（MAX_PNL_STRING_LENGTH - 1）
        "TenCharStr",      // 恰好 10 个字符（MAX_PNL_STRING_LENGTH）
        "ElevenCharStr"    // 超过限制
    };

    for (int i = 0; i < sizeof(testCases) / sizeof(testCases[0]); i++) {
        runTest(testCases[i], copyStringIncorrect, "Incorrect Function");
        runTest(testCases[i], copyStringCorrect, "Correct Function");
    }

    return 0;
}
