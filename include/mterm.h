#ifndef _MTERM_H_
#define _MTERM_H_

class MtermPool;

class Mterm
{
    friend class MtermPool;
private:
    int ptmFd_;
    int ptsProcessId_;
    long long lastReadTime_;
    bool isRunning_;
public:
    Mterm();
    ~Mterm();
    Mterm(const Mterm&) = delete;
    Mterm(const Mterm&&) = delete;
    Mterm& operator=(const Mterm&) = delete;
    Mterm& operator=(const Mterm&&) = delete;
    int Create
    (
        const char* cmd,
        const char* cwd,
        char* const argv[],
        char** envp,
        unsigned short rows,
        unsigned short cols
    );
    void Destrory();
    void UpdateRunning();
    bool IsRunning() const { return isRunning_; };
    int Read(char* buf, unsigned long size);
    int Write(const void* buf, unsigned long size) const;
    int Wait() const;
    void SetReadNonblock() const;
    void ResizeWindow(unsigned short rows, unsigned short cols);
};

#endif //_MTERM_H_
