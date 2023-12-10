#ifndef _MTERM_POOL_H_
#define _MTERM_POOL_H_

#include <map>
#include <thread>
#include "singleton.h"

class Mterm;
class Rwlock;

class MtermPool {
    friend class Singleton<MtermPool>;
public:
    enum class CheckStatus {
        FREE,
        UNCHECKED,
        CHECKED
    };
private:
    std::map<unsigned int, Mterm*> mtermMap_;
    std::map<unsigned int, CheckStatus> checkedMap_;
    Rwlock* rwlock_;
    std::thread* gcThread_;
private:
    MtermPool();
    ~MtermPool();
    MtermPool(const MtermPool&) = delete;
    MtermPool(const MtermPool&&) = delete;
    MtermPool& operator=(const MtermPool&) = delete;
    MtermPool& operator=(const MtermPool&&) = delete;
public:
    int CreateMterm
    (
        const char* cmd,
        const char* cwd,
        char* const argv[],
        char** envp,
        unsigned short rows,
        unsigned short cols
    );
    int CreateMterm
    (
        unsigned short rows = 25,
        unsigned short cols = 80
    );
    int DestroyMterm(unsigned int id);
    int ReadMterm(unsigned int id, char* buf, unsigned long size);
    int WriteMterm(unsigned int id, const char* buf, unsigned long size);
    int WaitMterm(unsigned int id);
    void SetReadNonblockMterm(unsigned int id);
    void SetWindowSizeMterm(unsigned int id, unsigned short rows, unsigned short cols);
    bool CheckRunning(unsigned int id);
private:
    bool IsIdValid(unsigned int id);
    int FindFreeMterm();
    void ResetMterm(unsigned int id);
    unsigned int InsertNewMterm();

    void StartGCThread();
};

#endif //_MTERM_POOL_H_
