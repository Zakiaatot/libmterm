#include <chrono>
#include "mterm_pool.h"
#include "mterm.h"
#include "rwlock.h"
#include "error_number.h"

#define MIN_POOL_SIZE 8
#define MAX_POOL_SIZE 64
#define GC_INTERVAL 1 // s
#define GC_TIMEOUT  3 //min


MtermPool::MtermPool()
    :rwlock_(new Rwlock), gcThread_(nullptr)
{
    for (int i = 0; i < MIN_POOL_SIZE; ++i)
    {
        InsertNewMterm();
    }
}

MtermPool::~MtermPool()
{
    for (auto it = mtermMap_.begin(); it != mtermMap_.end(); ++it)
    {
        delete it->second;
    }
    delete rwlock_;
    delete gcThread_;
}

int MtermPool::CreateMterm
(
    const char* cmd,
    const char* cwd,
    char* const argv[],
    char** envp,
    unsigned short rows,
    unsigned short cols
)
{
    rwlock_->LockWrite();
    int id = FindFreeMterm();
    if (id != POOL_NOFREE_ERROR)
    {
        int res = mtermMap_.find(id)->second->Create(cmd, cwd, argv, envp, rows, cols);
        if (res > 0)
        {
            checkedMap_[id] = CheckStatus::UNCHECKED;
        }
        else
            id = res;
    }
    else
    {
        auto msize = mtermMap_.size();
        auto mmaxsize = mtermMap_.max_size();

        if (msize < MAX_POOL_SIZE && msize < mmaxsize)
        {
            id = InsertNewMterm();
            int res = mtermMap_.find(id)->second->Create(cmd, cwd, argv, envp, rows, cols);
            if (res > 0)
            {
                checkedMap_[id] = CheckStatus::UNCHECKED;
            }
            else
                id = res;
        }
        else
        {
            id = POOL_FULL_ERROR;
        }
    }
    rwlock_->UnlockWrite();
    return id;
}

int MtermPool::CreateMterm
(
    unsigned short rows,
    unsigned short cols
)
{
    const char* cmd = "/bin/sh";
    const char* cwd = "/";
    char* const argv[] = { (char*)"sh",(char*)"-i", NULL };
    return CreateMterm(cmd, cwd, argv, NULL, rows, cols);
}

int MtermPool::DestroyMterm(unsigned int id)
{
    rwlock_->LockWrite();
    int res = 0;
    if (mtermMap_.find(id) == mtermMap_.end())
    {
        res = ID_INVALID_ERROR;
    }
    else
    {
        ResetMterm(id);
    }
    rwlock_->UnlockWrite();
    return res;
}

int MtermPool::ReadMterm(unsigned int id, char* buf, unsigned long size)
{
    if (!IsIdValid(id))
    {
        return ID_INVALID_ERROR;
    }
    return (mtermMap_.find(id)->second)->Read(buf, size);
}

int MtermPool::WriteMterm(unsigned int id, const char* buf, unsigned long size)
{
    if (!IsIdValid(id))
    {
        return ID_INVALID_ERROR;
    }
    return (mtermMap_.find(id)->second)->Write(buf, size);
}

int MtermPool::WaitMterm(unsigned int id)
{
    if (!IsIdValid(id))
    {
        return ID_INVALID_ERROR;
    }
    return (mtermMap_.find(id)->second)->Wait();
}


void MtermPool::SetReadNonblockMterm(unsigned int id)
{
    if (!IsIdValid(id))
    {
        return;
    }
    return (mtermMap_.find(id)->second)->SetReadNonblock();
}


bool MtermPool::CheckRunning(unsigned int id)
{
    if (!IsIdValid(id))
    {
        return false;
    }
    rwlock_->LockWrite();
    Mterm* mterm = mtermMap_.find(id)->second;
    mterm->UpdateRunning();
    bool res = mterm->IsRunning();
    if (!res)
    {
        checkedMap_[id] = CheckStatus::CHECKED;
        ResetMterm(id);
    }
    rwlock_->UnlockWrite();
    return res;
}

bool MtermPool::IsIdValid(unsigned int id)
{
    rwlock_->LockRead();
    bool res = mtermMap_.find(id) != mtermMap_.end();
    rwlock_->UnlockRead();
    return res;
}

int MtermPool::FindFreeMterm()
{
    for (auto it = mtermMap_.cbegin(); it != mtermMap_.cend(); ++it)
    {
        unsigned int id = it->first;
        if (checkedMap_.find(id)->second == CheckStatus::FREE)
        {
            return id;
        }
    }
    return POOL_NOFREE_ERROR;
}

void MtermPool::ResetMterm(unsigned int id)
{
    mtermMap_[id]->Destrory();
    checkedMap_[id] = CheckStatus::FREE;
}

unsigned int MtermPool::InsertNewMterm()
{
    mtermMap_.insert(std::make_pair(mtermMap_.size(), new Mterm));
    checkedMap_.insert(std::make_pair(mtermMap_.size(), CheckStatus::FREE));
    return mtermMap_.size();
}

void MtermPool::StartGCThread()
{
    if (gcThread_ == nullptr)
    {
        gcThread_ = new std::thread
        (
            [this]
            {
                while (true)
                {
                    rwlock_->LockWrite();
                    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                    std::chrono::milliseconds timestamp =
                        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                    for (auto& it : mtermMap_)
                    {
                        if (
                            it.second->lastReadTime_ != -1 &&
                            timestamp.count() - it.second->lastReadTime_ > GC_TIMEOUT
                            )
                        {
                            ResetMterm(it.first);
                        }
                    }
                    rwlock_->UnlockWrite();
                    std::this_thread::sleep_for(std::chrono::seconds(GC_INTERVAL));
                }
            }
        );
    }
}