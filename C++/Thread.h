#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <deque>
#include <vector>
#include <list>
#include <signal.h>

void* ThreadFunc(void* args);

class Thread
{
public:
    Thread(): _tid(-1), _retval(0) {}

    virtual ~Thread() {  }

    bool Start()
    {
        int status = pthread_create(&_tid, NULL, ThreadFunc, this);
        if(status == 0)
            return true;
        else
            return false;
    }

    bool Join()
    {
        int* retval = NULL;
        int status = pthread_join(_tid, (void**)&retval);
        if(retval)
        {
            _retval = *retval;
            delete retval;
        }
        if(status == 0)
            return true;
        else
            return false;
    }

    bool Terminate()
    {
        int status = pthread_cancel(_tid);
        if(status == 0)
            return true;
        else
            return false;
    }

    bool Kill(int sig)
    {
        int status = pthread_kill(_tid, sig);
        if(status == 0)
            return true;
        else
            return false;
    }

    virtual int Run() = 0;

protected:
    pthread_t _tid;
    int _retval;
};


class Job
{
public:
    Job() {}
    virtual ~Job() {}
    virtual int Start() = 0;
};

class WorkerThread: public Thread
{
public:
    WorkerThread() {}
    virtual ~WorkerThread() {}
    virtual int Run()
    {
        job->Start();
        pthread_mutex_lock(mutex_nthreads);
        *nthreads -= 1;
        pthread_mutex_unlock(mutex_nthreads);
        finished = true;
        pthread_cond_signal(cond_nthreads);
        return 0;
    }
public:
    Job* job;
    int* nthreads;
    pthread_mutex_t* mutex_nthreads;
    pthread_cond_t* cond_nthreads;
    bool finished;
};

class ThreadPool: public Thread
{
public:
    ThreadPool(int maxthreads) 
    {
        _maxthreads = maxthreads;
        _nthreads = 0;
        _shutdown = false;
        pthread_mutex_init(&_mutex_queue, NULL);
        pthread_cond_init(&_cond_queue, NULL);
        pthread_mutex_init(&_mutex_nthreads, NULL);
        pthread_cond_init(&_cond_nthreads, NULL);
        Start();
    }
    
    virtual ~ThreadPool()
    {
        Shutdown();
        Join();
        pthread_mutex_destroy(&_mutex_queue);
        pthread_cond_destroy(&_cond_queue);
        pthread_mutex_destroy(&_mutex_nthreads);
        pthread_cond_destroy(&_cond_nthreads);
    }
    
    void Shutdown()
    {
        _shutdown = true;
        pthread_cond_signal(&_cond_nthreads);
        pthread_cond_signal(&_cond_queue);
    }
    
    void Submit(Job* job)
    {
        pthread_mutex_lock(&_mutex_queue);
        _queue.push_back(job);
        pthread_mutex_unlock(&_mutex_queue);
        pthread_cond_signal(&_cond_queue);
    }
    
    void Wait()
    {
        pthread_mutex_lock(&_mutex_queue);
        while(!_queue.empty())
            pthread_cond_wait(&_cond_queue, &_mutex_queue);
        pthread_mutex_unlock(&_mutex_queue);
        
        pthread_mutex_lock(&_mutex_nthreads);
        while(_nthreads > 0)
            pthread_cond_wait(&_cond_nthreads, &_mutex_nthreads);
        pthread_mutex_unlock(&_mutex_nthreads);
    }
    
public:
    void CleanUp()
    {
        for(std::list<WorkerThread*>::iterator it = _workers.begin();
            it != _workers.end(); ++it)
        {
            if((*it)->finished)
            {
                (*it)->Join();
                delete (*it);
                *it = NULL;
            }
        }
        _workers.remove(NULL);
    }
    
    virtual int Run()
    {
        while(true)
        {
            pthread_mutex_lock(&_mutex_queue);
            while(_queue.empty())
            {
                if(_shutdown)
                {
                    // wait for unfinished workers
                    pthread_mutex_lock(&_mutex_nthreads);
                    while(_nthreads > 0)
                        pthread_cond_wait(&_cond_nthreads, &_mutex_nthreads);
                    pthread_mutex_unlock(&_mutex_nthreads);
                    CleanUp();
                    // shutdown
                    return 0;
                }
                // wait for new jobs
                else
                    pthread_cond_wait(&_cond_queue, &_mutex_queue);
        
            }
            pthread_mutex_unlock(&_mutex_queue);
            // check if maximum number of workers is reached
            pthread_mutex_lock(&_mutex_nthreads);
            while(_nthreads >= _maxthreads)
                pthread_cond_wait(&_cond_nthreads, &_mutex_nthreads);
            pthread_mutex_unlock(&_mutex_nthreads);
            // create a new worker
            pthread_mutex_lock(&_mutex_queue);
            Job* job = _queue.front();
            _queue.pop_front();
            pthread_mutex_unlock(&_mutex_queue);
            pthread_cond_signal(&_cond_queue);
            
            pthread_mutex_lock(&_mutex_nthreads);
            _nthreads += 1;
            pthread_mutex_unlock(&_mutex_nthreads);
            
            WorkerThread* worker = new WorkerThread();
            worker->job = job;
            worker->nthreads = &_nthreads;
            worker->mutex_nthreads = &_mutex_nthreads;
            worker->cond_nthreads = &_cond_nthreads;
            worker->finished = false;
            _workers.push_back(worker);
            worker->Start();
            // clean up finished workers
            CleanUp();
        }
        return 0;
    }
private:
    int _nthreads;
    int _maxthreads;
    bool _shutdown;
    std::list<WorkerThread*> _workers;
    std::deque<Job*> _queue;
    pthread_mutex_t _mutex_queue;
    pthread_cond_t  _cond_queue;
    pthread_mutex_t _mutex_nthreads;
    pthread_cond_t  _cond_nthreads;
};

#endif
