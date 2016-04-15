#include "../kr.h"

enum {
    NoLeap = 0,
    Leap = 1,
};

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

bool isLeap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int day_of_year(int year, int month, int day) {
    char *tab = daytab[isLeap(year)];
    if (month > 12 || month <= 0 || day <= 0 || day > tab[month]) {
        return 0;
    }
    int i;
    for (i = 0; i != month; i++) {
        day += tab[i];
    }
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
    *pmonth = *pday = 0;
    char *tab = daytab[isLeap(year)];
    if (yearday <= 0) {
        return;
    }
    int i;
    for (i = 1; i <= 12 && yearday > tab[i]; i++) {
        yearday -= tab[i];
    }
    if (i > 12) {
        return;
    }
    *pmonth = i;
    *pday = yearday;
}

int test_month_day(int year, int yearday) {
    int rmon, rday;
    month_day(year, yearday, &rmon, &rday);
    printf("%d-%d -> %d/%d/%d\n", year, yearday, year, rmon, rday);
    return 0;
}

int group_test(int year, int month, int day) {
    int yearday = day_of_year(year, month, day);
    printf("%d/%d/%d -> %d\t", year, month, day, yearday);
    if (yearday == 0) {
        printf("\n");
        return 0;
    }
    test_month_day(year, yearday);
    return 0;
}


int main() {
    printf("test day out of range\n");
    group_test(2000, 2, 28);
    group_test(2000, 2, 29);
    group_test(2000, 2, 30);
    printf("test month out of range\n");
    group_test(2000, 0, 30);
    group_test(2000, 13, 30);
    printf("test month_day out of range\n");
    test_month_day(2016, 0);
    test_month_day(2016, 1);
    test_month_day(2016, 366);
    test_month_day(2016, 367);
    printf("group test\n");
    group_test(2016, 4, 15); // leap
    group_test(2000, 4, 15); // leap
    group_test(1900, 4, 15); // not leap
    group_test(2001, 4, 15); // not leap
    return 0;
}
