#ifndef __WIN32__
    #include <termios.h>
    #include <stdio.h>

    static struct termios old, new;

    void initTermios(int echo)
    {
        tcgetattr(0, &old);
        new = old;
        new.c_lflag &= ~ICANON;
        new.c_lflag &= echo ? ECHO : ~ECHO;
        tcsetattr(0, TCSANOW, &new);
    }

    void resetTermios(void)
    {
        tcsetattr(0, TCSANOW, &old);
    }

    char getch()
    {
        char ch;
        initTermios(0);
        ch = getchar();
        resetTermios();
        return ch;
    }

#endif
