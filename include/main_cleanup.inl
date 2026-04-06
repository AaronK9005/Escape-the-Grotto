#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <ctype.h>

    #define setNonBlocking(bool) 

    int get_input() {
        if (_kbhit()) { return tolower(_getch()); }
        else return -1;
    }
#else
    #error Compilation failed: Currently this game only supports Windows (error in get_input).
#endif