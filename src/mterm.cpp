#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "mterm.h"
#include "error_number.h"

Mterm::Mterm()
    :ptmFd_(-1),
    ptsProcessId_(-1),
    lastReadTime_(-1),
    isRunning_(false)
{
}

Mterm::~Mterm()
{
    if (isRunning_)
    {
        Destrory();
    }
}

int Mterm::Create
(
    const char* cmd,
    const char* cwd,
    char* const argv[],
    char** envp,
    unsigned short rows,
    unsigned short cols
)
{
    int ptm = open("/dev/ptmx", O_RDWR);
    if (ptm < 0) return OPEN_PTMX_ERROR;

    // pts devname
    char devname[64];
    if (grantpt(ptm) || unlockpt(ptm) || ptsname_r(ptm, devname, sizeof(devname)))
        return GET_PTS_ERROR;

    // utf-8 mode
    struct termios tios;
    tcgetattr(ptm, &tios);
    tios.c_iflag |= IUTF8;
    tios.c_iflag &= ~(IXON | IXOFF);
    tcsetattr(ptm, TCSANOW, &tios);

    // init windows
    struct winsize ws = { .ws_row = rows,.ws_col = cols };
    ioctl(ptm, TIOCSWINSZ, &ws);

    // fork
    pid_t pid = fork();
    if (pid < 0)
    {
        return PROCESS_FORK_ERROR;
    }
    else if (pid > 0)
    {
        ptsProcessId_ = (int)pid;
        ptmFd_ = ptm;
        isRunning_ = true;
        return ptm;
    }
    else
    {
        // signal nonblock
        sigset_t signalsToNonblock;
        sigfillset(&signalsToNonblock);
        sigprocmask(SIG_UNBLOCK, &signalsToNonblock, NULL);

        close(ptm);
        setsid();

        int pts = open(devname, O_RDWR);
        if (pts < 0) exit(OPEN_PTS_ERROR);
        // redirect stdin stdout stderr
        dup2(pts, 0);
        dup2(pts, 1);
        dup2(pts, 2);
        // close all other fds
        DIR* dir = opendir("/proc/self/fd");
        if (dir != NULL)
        {
            int dirFd = dirfd(dir);
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL)
            {
                int fd = atoi(entry->d_name);
                if (fd > 2 && fd != dirFd) close(fd);
            }
            closedir(dir);
        }
        // env
        if (envp != NULL)
        {
            clearenv();
            for (;*envp;++envp) putenv(*envp);
        }
        // cwd
        if (cwd != NULL && chdir(cwd) != 0)
        {
            char* errorMsg;
            if (asprintf(&errorMsg, "chdir(\"%s\")", cwd) != -1)
                errorMsg = (char*)"chdir()";
            perror(errorMsg);
            fflush(stderr);
        }
        // exec
        execvp(cmd, argv);
        //exec failed
        char* errorMsg;
        if (asprintf(&errorMsg, "exec(\"%s\")", cmd) != -1)
            errorMsg = (char*)"exec()";
        perror(errorMsg);
        fflush(stderr);
        exit(PTS_EXEC_ERROR);
    }
}

void Mterm::Destrory()
{
    close(ptmFd_);
    ptmFd_ = -1;
    ptsProcessId_ = -1;
    lastReadTime_ = -1;
    isRunning_ = false;
}

void Mterm::UpdateRunning()
{
    int status;
    int res = waitpid(ptsProcessId_, &status, WNOHANG);
    if (res == 0)
        isRunning_ = true;
    else
        Destrory();
}

int Mterm::Read(char* buf, unsigned long  size)
{
    int res = read(ptmFd_, buf, size);
    if (res > 0)
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::chrono::milliseconds timestamp =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        lastReadTime_ = timestamp.count();
    }
    return res;
}

int Mterm::Write(const void* buf, unsigned long  size) const
{
    return write(ptmFd_, buf, size);
}

int Mterm::Wait() const
{
    int status;
    waitpid(ptmFd_, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
        return -WTERMSIG(status);
    }
    else {
        return WAIT_PID_ERROR;
    }
}

void Mterm::SetReadNonblock() const
{
    int flag = fcntl(ptmFd_, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(ptmFd_, F_SETFL, flag);
}

void Mterm::ResizeWindow(unsigned short rows, unsigned short cols)
{
    struct winsize ws = { .ws_row = rows,.ws_col = cols };
    ioctl(ptmFd_, TIOCSWINSZ, &ws);
}