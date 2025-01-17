#include "butil.h"

void* xmalloc(size_t size)
{
   void* ptr = malloc(size);
   if (ptr == NULL)
      pdie("malloc(%zu)", size);
   return ptr;
}
__attribute__((nonnull(1))) void* xrealloc(void* ptr, size_t size)
{
   void* ptrn = realloc(ptr, size);
   if (ptrn == NULL)
      pdie("realloc(%zu)", size);
   return ptrn;
}
void* xcalloc(size_t num, size_t size)
{
   void* ptrn = calloc(num, size);
   if (ptrn == NULL)
      pdie("calloc(%zu)", size);
   return ptrn;
}
FILE* xfopen(const char* file_path, const char* mode)
{
   FILE* fd = fopen(file_path, mode);
   if (fd == NULL)
      pdie("fopen(%s, %s)", file_path, mode);
   return fd;
}
char* xmfopen(const char* file_path)
{
    FILE* fd = xfopen(file_path, "r");

    fseek(fd, 0, SEEK_END);
    size_t size = ftell(fd);
    rewind(fd);

    char* buff = xmalloc(size + 1);
    fread(buff, 1, size, fd);

    buff[size] = '\0';
    fclose(fd);

    return buff;
}
