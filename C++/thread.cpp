#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include <unistd.h>
#include "Thread.h"

void* ThreadFunc(void* args)
{
    int* retval = new int;
    *retval = ((Thread*)args)->Run();
    return (void*)retval;
}

class MyThread: public Thread
{
public:
    MyThread(int id): _id(id) {}
    virtual ~MyThread() {}
    virtual int Run()
    {
        cout << "[" << _tid << "] thread " <<  _id << endl;
        sleep(3);
        return 0;
    }

private:
    int _id;
};

void TestThreads()
{
    int nthreads = 16;
    vector<MyThread*> threads(nthreads);
    for(int i = 0; i < nthreads; i ++)
    {
        threads[i] = new MyThread(i);
        threads[i]->Start();
    }
    for(int i = 0; i < nthreads/2; i ++)
    {
        threads[i]->Kill(SIGTERM);
        threads[i]->Join();
        delete threads[i];
    }

}

class MyJob: public Job
{
public:
    MyJob(int id): _id(id) {}
    virtual ~MyJob() {}
    virtual int Start()
    {
        cout << "JobId: " << _id << endl;
        sleep(1);
        return 0;
    }
private:
    int _id;
};

void TestThreadPool()
{
    ThreadPool pool(8);
    vector<MyJob*> jobs(33);
    for(size_t i = 0; i < jobs.size(); i ++)
    {
        jobs[i] = new MyJob(i);
        pool.Submit(jobs[i]);
    }
    pool.Wait();
    for(size_t i = 0; i < jobs.size(); i ++)
        delete jobs[i];
}

class IncArrayJob: public Job
{
public:
    IncArrayJob(vector<int>& a, int id, size_t offset = 0)
        : _a(a), _id(id), _offset(offset) {}
    virtual ~IncArrayJob() {}
    void CASUpdate()
    {
        ncas = 0;
        for(size_t i = _offset; i < _a.size(); i ++)
        {
            int* p = &(_a[i]);
            int oldval = *p;
            int curval = oldval + 1;
            while(!__atomic_compare_exchange(p, &oldval, &curval, false, 
                        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
            {
                curval = oldval + 1;
                ncas ++;
            }
        }
        for(size_t i = 0; i < _offset; i ++)
        {
            int* p = &(_a[i]);
            int oldval = *p;
            int curval = oldval + 1;
            while(!__atomic_compare_exchange(p, &oldval, &curval, false, 
                        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
            {
                curval = oldval + 1;
                ncas ++;
            }
        }
    }
    
    void AtomicUpdate()
    {
        for(size_t i = _offset; i < _a.size(); i ++)
            __atomic_add_fetch(&(_a[i]), 1, __ATOMIC_SEQ_CST);
        for(size_t i = 0; i < _offset; i ++)
            __atomic_add_fetch(&(_a[i]), 1, __ATOMIC_SEQ_CST);
    }
    
    virtual int Start()
    {
        cout << "Thread " << _id << " started" << endl;
        CASUpdate();
        //AtomicUpdate();

        return 0;
    }
    size_t ncas;
private:
    vector<int>& _a;
    int _id;
    size_t _offset;
};

void TestCompareAndSwap()
{
    const int nthreads = 12;
    const int njobs = 30;
    const size_t arraysize = (1 << 24);
    vector<int> a(arraysize, 0);
    ThreadPool pool(nthreads);
    vector<Job*> jobs(njobs);
    for(size_t i = 0; i < jobs.size(); i ++)
    {
        jobs[i] = (Job*)(new IncArrayJob(a, i + 1, arraysize/nthreads));
        pool.Submit(jobs[i]);
    }
    pool.Wait();
    // sleep(3);
    for(size_t i = 0; i < jobs.size(); i ++)
    {
        cout << "CAS operations in Thread " << i << ": " 
            << ((IncArrayJob*)jobs[i])->ncas << endl;
        delete jobs[i];
    }

    const int maxnerror = 10;
    int nerrors = 0;
    for(size_t i = 0; i < a.size(); i ++)
    {
        if(a[i] != njobs)
        {
            cout << "Array element(" << i << ") unequal to thread number: " 
                << njobs << " => " << a[i] << endl;
            nerrors ++;
            if(nerrors >= maxnerror)
                break;
        }
    }
}

int main()
{
    // TestThreads();
    //TestThreadPool();
    TestCompareAndSwap();

    return 0;
}
