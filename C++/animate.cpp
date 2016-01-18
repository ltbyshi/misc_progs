#include <vector>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

struct Toggle {
    bool enabled;
    Toggle(bool enabled = true) : enabled(enabled) {}
    bool operator()() {
        bool r = enabled;
        enabled = enabled? false : true;
        return r;
    }
};

struct Bar {
    int length;
    int columns;
    Toggle toggle;

    Bar() {}
    Bar(int length, int columns)
        : length(length), columns(columns) {}

    void Display() {
        int n = length % columns;
        if(!toggle.enabled)
            n = columns - length;
        for(int i = 0; i < n; i ++)
            printf("#");
        printf("\n");
        length = (length + 1) % columns;
        if(length == 0)
            toggle();
    }
};

int main()
{
    const int lines = 58;
    const int columns = 114;
    const int height = 50;
    const int frames = 100000000;
    const int fps = 200;
    struct timespec interval;
    interval.tv_sec = 0;
    interval.tv_nsec = 1000000000/fps;
    
    vector<Bar> bars(height);
    for(size_t i = 0; i < bars.size(); i ++)
    {
        bars[i].length = (columns/2)*sin(2*3.14159/50*i);
        bars[i].columns = columns;
    }

    for(int i = 0; i < frames; i ++)
    {
        for(size_t j = 0; j < bars.size(); j ++)
            bars[j].Display();
        nanosleep(&interval, NULL);
        for(int j = height; j < lines; j ++)
            printf("\n");
    }
    return 0;
}
            
