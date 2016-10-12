#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#define BUFLEN  128
#define TIMEFMTLEN      30
#define ASSERTSN(x, n) \
    if (x > (n - 1)) { \
        printf("WARN: snprintf buffer length not enough %d %d\n", n, x); \
        return 0; \
    }


int createdir(char *dirname)
{
    if (mkdir(dirname, S_IRWXG | S_IRWXU) < 0) {
        perror("create dump dir fail\n");
        return 0;
    }
    return 1;
}
int checkCreateDir(char *dirname)
{
    DIR *pDir = NULL;
    if ((pDir = opendir(dirname)) == NULL) {
        return createdir(dirname);
    }
    int ret = 0;
    closedir(pDir);
    int namelen = strlen(dirname) + TIMEFMTLEN;
    char *backupname = malloc(namelen);
    char timebuf[TIMEFMTLEN] = {0};
    if (backupname == NULL) {
        perror("malloc backupname fail\n");
        return 0;
    }
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);
    strftime(timebuf, TIMEFMTLEN, "%H%M%S.%Y%m%d", timenow);
    ret = snprintf(backupname, namelen, "%s.%s", dirname, timebuf);
    if (ret < namelen - 1) {
        perror("too long\n");
        goto EXIT;
    }
    ASSERTSN(ret, namelen);
    rename(dirname, backupname);
    printf("backup %s to %s\n", dirname, backupname);
    ret = createdir(dirname);
EXIT:
    free(backupname);
    backupname = NULL;
    return ret;
}
int startswith(char *str, char *head)
{
    size_t lenstr = strlen(str);
    size_t lenhd = strlen(head);
    return lenstr < lenhd ? 0 : strncmp(str, head, lenhd) == 0;
}
char* createDumpDir()
{
    DIR *pDir = NULL;
    char mediadir[] = "mnt/media";
    char datadir[] = "data";
    char *dirnamebuf = malloc(BUFLEN);
    struct dirent *pDirent = NULL;
    int ret = 0;
    int foundFlag = 0;
    memset(dirnamebuf, 0, BUFLEN);
    pDir = opendir(mediadir);
    if (pDir != NULL) {
        while ((pDirent = readdir(pDir)) != NULL) {
            if (startswith(pDirent->d_name, "usb.")) {
                ret = snprintf(dirnamebuf, BUFLEN, "%s/%s/AAA", mediadir, pDirent->d_name);
                ASSERTSN(ret, BUFLEN);
                foundFlag = 1;
            }
        }
        closedir(pDir);
    }
    if (foundFlag == 1) {
        ret = checkCreateDir(dirnamebuf);
    }
    if (ret == 0 || foundFlag == 0) {
        ret = snprintf(dirnamebuf, BUFLEN, "%s/AAA", datadir);
        ASSERTSN(ret, BUFLEN);
        ret = checkCreateDir(dirnamebuf);
    }
    if (ret) {
        return dirnamebuf;
    } else {
        free(dirnamebuf);
        return NULL;
    }
}

int main(void)
{
    char *dir = createDumpDir();
    if (dir != NULL) {
        printf("create dump dir %s\n", dir);
        free(dir);
    }
    return 0;
}
