#ifndef _RWLOCK_H_
#define _RWLOCK_H_

#include <mutex>
#include <condition_variable>

class Rwlock
{
private:
    std::mutex mtx_;
    std::condition_variable readCond_;
    std::condition_variable writeCond_;
    volatile size_t readCount_ = 0;
    volatile size_t writeCount_ = 0;
    volatile bool isWrite_ = false;
public:
    Rwlock() = default;
    ~Rwlock() = default;
    Rwlock(const Rwlock&) = delete;
    Rwlock(const Rwlock&&) = delete;
    Rwlock& operator=(const Rwlock&) = delete;
    Rwlock& operator=(const Rwlock&&) = delete;
    void LockRead()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        readCond_.wait(lock, [this] { return writeCount_ == 0; });
        ++readCount_;
    }
    void UnlockRead()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        --readCount_;
        if (writeCount_ > 0) {
            writeCond_.notify_one();
        }
    }
    void LockWrite()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        ++writeCount_;
        writeCond_.wait(lock, [this] { return readCount_ == 0 && isWrite_ == false; });
        isWrite_ = true;
    }
    void UnlockWrite()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        --writeCount_;
        if (writeCount_ == 0)
        {
            readCond_.notify_all();
        }
        else
        {
            writeCond_.notify_one();
        }
        isWrite_ = false;
    }
};

#endif //_RWLOCK_H_
