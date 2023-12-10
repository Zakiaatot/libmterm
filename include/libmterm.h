#ifndef _LIBMTERM_H_
#define _LIBMTERM_H_

#ifdef __cplusplus
extern "C"
{
#endif

    int CreateMterm
    (
        const char* cmd,
        const char* cwd,
        char* const argv[],
        char** envp,
        unsigned short rows,
        unsigned short cols
    );
    int CreateMtermDefault();
    int DestroyMterm(unsigned int id);
    int ReadMterm(unsigned int id, char* buf, unsigned long size);
    int WriteMterm(unsigned int id, const char* buf, unsigned long size);
    int WaitMterm(unsigned int id);
    void SetReadNonblockMterm(unsigned int id);
    void SetWindowSizeMterm(unsigned int id, unsigned short rows, unsigned short cols);
    bool CheckRunningMterm(unsigned int id);

#ifdef __cplusplus
}
#endif

#endif //_LIBMTERM_H_
