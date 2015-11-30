#include <stdio.h>
#include <termios.h>

int main()
{
    int c;
    struct termios tios;

    tcgetattr(fileno(stdin), &tios);
    tios.c_lflag &= ~ICANON;
    tcsetattr(fileno(stdin), TCSANOW, &tios);

    c = getchar();
    while(c != EOF)
    {
        putchar(c);
        getchar();
    }

    return 0;
}
