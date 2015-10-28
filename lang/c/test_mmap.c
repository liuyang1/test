#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// - read
// Get file size by `stat` func, then `mmap` it with this size.
// - write
// First grow file size by `lseek` func, then `mmap` it with new size.
// Must have wirte permission, else will get `SIGSEGV` signal.
// After `mmap`, we can close fd. But we can continue write it.
// `munmap` won't write file back to disk.
// - offset must multple of page size.
// - anonymous mmap
// Use -1 as fd, or open `/dev/zero` file.
// Child process will inherit anonymous mmap file. so parent-child can
// communicate by this.
#define MEM_1K 1024
int main()
{
    int fd = open("/etc/lsb-release", O_RDONLY);
    char *ptr = (char *)mmap(NULL, MEM_1K, PROT_READ, MAP_PRIVATE,
                             fd, 0);
    printf("mmap ptr=%p\n", ptr);
    printf("\tshow mmaped file content\n");
    printf("%s", ptr);
    printf("\t----end\n");

    int ret = munmap(ptr, MEM_1K);
    printf("ret=%x\n", ret);
    return 0;
}
