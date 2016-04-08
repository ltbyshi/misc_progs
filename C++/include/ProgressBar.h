#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
// progress bar
class ProgressBar
{
public:
    ProgressBar(double maxval, size_t size = 60)
    : _maxval(maxval), _curval(0), _size(size),
    _symbol('=')
    {
        gettimeofday(&_starttime, NULL);
        gettimeofday(&_curtime, NULL);
        pthread_mutex_init(&_mutex, NULL);
        _locked = false;
    }
    
    ~ProgressBar()
    {
        pthread_mutex_destroy(&_mutex);
    }
    
    void Start()
    {
        gettimeofday(&_starttime, NULL);
        _curval = 0;
    }
    
    void Display()
    {
        if(!_locked)
            pthread_mutex_lock(&_mutex);
        double progress = _curval/_maxval;
        gettimeofday(&_curtime, NULL);
        double cursec = double(_curtime.tv_usec)/1e6 + _curtime.tv_sec;
        double startsec = double(_starttime.tv_usec)/1e6 + _starttime.tv_sec;
        double elapsed = cursec - startsec;
        double remaining = (elapsed > 1e-3)? (elapsed/progress*(1.0 - progress)) : 0;
        
        std::string bar(_size, ' ');
        for(size_t i = 0; i < size_t(progress*_size); i ++)
            bar[i] = _symbol;
        fprintf(stderr, "\r[%s] %3.2lf%%, %.1lfs elapsed, %.1lfs remaining",
               bar.c_str(), progress*100.0, elapsed, remaining); 
        fflush(stderr);
        if(!_locked)
            pthread_mutex_unlock(&_mutex);
    }
    
    void Update(double val, bool display = true)
    {
        pthread_mutex_lock(&_mutex);
        _curval = val;
        _locked = true;
        Display();
        _locked = false;
        pthread_mutex_unlock(&_mutex);
    }
    
    void Increment(double val, bool display = true)
    {
        pthread_mutex_lock(&_mutex);
        _curval += val;
        _locked = true;
        Display();
        _locked = false;
        pthread_mutex_unlock(&_mutex);
    }
    
    void Finish() const
    {
        printf("\n");
        fflush(stdout);
    }
private:
    double _maxval;
    double _curval;
    size_t _size;
    char _symbol;
    struct timeval _starttime;
    struct timeval _curtime;
    pthread_mutex_t _mutex;
    bool _locked;
};

#endif
