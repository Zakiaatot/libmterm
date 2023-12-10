# libmterm

**A mini pseudo-terminal implementation library.**

&#x1F6E0; Warning : This is a work in progress...

## Usage

### cxx

See
[/example/cxx](/example/cxx) for details.

```c++
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <thread>
#include "libmterm.h"

int main(void)
{
    int id = CreateMtermDefault();
    std::thread readT([id] {
        SetReadNonblockMterm(id);
        char buf[4096];
        int res;
        while (true)
        {
            res = ReadMterm(id, buf, sizeof(buf));
            if (res > 0)
                write(1, buf, res);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        });

    std::thread writeT([id] {
        // stdin nonblock read
        fcntl(0, F_SETFL, O_NONBLOCK | fcntl(0, F_GETFL));
        // close echo and set canonical mode
        struct termios oldSettings, newSettings;
        tcgetattr(0, &oldSettings);
        newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &newSettings);

        char buf[4096];
        int res;
        while (true)
        {
            res = read(0, buf, sizeof(buf));
            if (res > 0)
                WriteMterm(id, buf, res);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        });
    readT.join();
    writeT.join();
    return 0;
}
```

### java

!todo ...
