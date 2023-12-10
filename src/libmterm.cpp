#include "libmterm.h"
#include "mterm_pool.h"

static MtermPool& POOL = Singleton<MtermPool>::GetInstance();

int CreateMterm
(
    const char* cmd,
    const char* cwd,
    char* const argv[],
    char** envp,
    unsigned short rows,
    unsigned short cols
)
{
    return POOL.CreateMterm(cmd, cwd, argv, envp, rows, cols);
}

int CreateMtermDefault()
{
    return POOL.CreateMterm();
}

int DestroyMterm(unsigned int id)
{
    return POOL.DestroyMterm(id);
}

int ReadMterm(unsigned int id, char* buf, unsigned long size)
{
    return POOL.ReadMterm(id, buf, size);
}

int WriteMterm(unsigned int id, const char* buf, unsigned long size)
{
    return POOL.WriteMterm(id, buf, size);
}

int WaitMterm(unsigned int id)
{
    return POOL.WaitMterm(id);
}

void SetReadNonblockMterm(unsigned int id)
{
    return POOL.SetReadNonblockMterm(id);
}

void SetWindowSizeMterm
(
    unsigned int id,
    unsigned short rows,
    unsigned short cols
)
{
    return POOL.SetWindowSizeMterm(id, rows, cols);
}

bool CheckRunningMterm(unsigned int id)
{
    return POOL.CheckRunning(id);
}