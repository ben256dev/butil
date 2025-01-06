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
#ifdef _WIN32
BSHM bshm_open(const char* name, i64 bytes, const char* mode)
{
    HANDLE shm_fd;
    if (strcmp(mode, "r") == 0)
    {
        int i = 0;
        do shm_fd = OpenFileMapping(FILE_MAP_READ, FALSE, name), i++;
        while (i < BSHM_MAX_TRIES && shm_fd == NULL && GetLastError() == ERROR_FILE_NOT_FOUND);
    }
    else
        shm_fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, bytes, name);

    return (BSHM)(shm_fd);
}
BSHM xshm_open(const char* name, i64 bytes, const char* mode)
{
    HANDLE shm_fd = bshm_open(name, bytes, mode);
    if (shm_fd == NULL)
        wdie("OpenFileMapping");

    return (BSHM)(shm_fd);
}
void* xmmap(BSHM fd, size_t bytes, const char* mode)
{
    DWORD prot;
    if (strcmp(mode, "r") == 0)
        prot = FILE_MAP_READ;
    else if (strcmp(mode, "w") == 0)
        prot = FILE_MAP_WRITE;
    else
        prot = FILE_MAP_ALL_ACCESS;

    void* mem_ptr = MapViewOfFile(fd, prot, 0, 0, bytes);
    if (mem_ptr == NULL)
        wdie("MapViewOfFile");

    return mem_ptr;
}
BSEM xsem_open(const char* name, const char* mode)
{
    HANDLE sem;
    if (strcmp(mode, "r") == 0)
    {
        int i = 0;
        do sem = OpenSemaphore(READ_CONTROL, FALSE, name), i++;
        while (i < BSEM_MAX_TRIES && sem == NULL && GetLastError() == ERROR_FILE_NOT_FOUND);
    }
    else
        sem = CreateSemaphore(NULL, 0, 1, name);

    if (sem == NULL)
        wdie("OpenSemaphore");

    return (BSEM)(sem);
}
void bsem_post(BSEM sem_ptr)
{
    ReleaseSemaphore(sem_ptr, 1, NULL);
}
void xsem_wait(BSEM sem_ptr)
{
    if (WaitForSingleObject(sem_ptr, INFINITE) == WAIT_FAILED)
        wdie("WaitForSingleObject");
}
void bsleep(u32 seconds)
{
    Sleep(seconds * 1000);
}
void xmunmap(void* addr, size_t bytes)
{
    UnmapViewOfFile(addr);
}
void xclose(BSHM shm_fd, const char* name)
{
    CloseHandle(shm_fd);
}
void xsem_close(BSEM sem_ptr, const char* name)
{
    CloseHandle(sem_ptr);
}
#else
BSHM bshm_open(const char* name, i64 bytes, const char* mode)
{
    int shm_fd;
    if (strcmp(mode, "r") == 0)
    {
        int i = 0;
        do shm_fd = shm_open(name, O_RDONLY, 0666), i++;
        while (i < BSHM_MAX_TRIES && shm_fd == -1 && errno == ENOENT);
    }
    else
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    return (BSHM)(shm_fd);
}
BSHM xshm_open(const char* name, i64 bytes, const char* mode)
{
    BSHM shm_fd = bshm_open(name, bytes, mode);
    if (shm_fd == -1)
        pdie("shm_open");

    if (strcmp(mode, "r") != 0)
        if (ftruncate(shm_fd, bytes) == -1)
            pdie("ftruncate");

    return (BSHM)(shm_fd);
}
void* xmmap(BSHM fd, size_t bytes, const char* mode)
{
    int prot;
    if (strcmp(mode, "r") == 0)
        prot = PROT_READ;
    else if (strcmp(mode, "w") == 0)
        prot = PROT_WRITE;
    else
        prot = PROT_READ | PROT_WRITE;

    int flags = 0;
    if (fd)
        flags = MAP_SHARED;

    void* mem_ptr = mmap(0, bytes, prot, flags, fd, 0);
    if (mem_ptr == MAP_FAILED)
        pdie("mmap");
}
BSEM xsem_open(const char* name, const char* mode)
{
    sem_t* sem_ptr;
    if (strcmp(mode, "r") == 0)
    {
        int i = 0;
        do sem_ptr = sem_open(name, 0), i++;
        while (i < BSEM_MAX_TRIES && sem_ptr == SEM_FAILED && errno == ENOENT);
    }
    else
        sem_ptr = sem_open(name, O_CREAT, 0666, 0);

    if (sem_ptr == SEM_FAILED)
        pdie("sem_open");

    return (BSEM)(sem_ptr);
}
void bsem_post(BSEM sem_ptr)
{
    sem_post(sem_ptr);
}
void xsem_wait(BSEM sem_ptr)
{
    if (sem_wait(sem_ptr) == -1)
        pdie("sem_wait");
}
void bsleep(u32 seconds)
{
    sleep(seconds);
}
void xmunmap(void* addr, size_t bytes)
{
    if (munmap(addr, bytes) == -1)
        pdie("munmap");
}
void xclose(BSHM shm_fd, const char* name)
{
    if (shm_unlink(name) == -1)
        pdie("shm_unlink");
    if (close(shm_fd) == -1)
        pdie("close");
}
void xsem_close(BSEM sem_ptr, const char* name)
{
    sem_close(sem_ptr);
    sem_unlink(name);
}
#endif
