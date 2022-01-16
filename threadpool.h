#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <set>
#include <deque>
#include <string>
#include <mutex>
#include <condition_variable>
#include "pthread.h"

#define MAX_THREAD_NUM 128
#define MIN_THREAD_NUM 16

enum Status{
    INIT = 0,
    RUNNING,
    FINISHED
};

class Task{
public:
    Task(std::string name, void* data = NULL):
        m_name(name),
        m_data(data),
        m_status(INIT)
    {
        
    }
    virtual ~Task(){};
    virtual void run() = 0;
    void reset()
    {
        m_data = NULL;
        m_status = INIT;
        m_name = "";
    }
    volatile Status getStatus()
    {
        return m_status;
    }
    void setStatus(const Status status)
    {
        m_status = status;
    }
protected:
    std::string              m_name;
    volatile Status          m_status;
    void*                    m_data;
};

class ThreadPool{
public:
    ThreadPool( int threadNum );
    virtual ~ThreadPool();
    static void* threadFunc(void*);

    void init();
    int createThread();
    int addTask(Task* task);
    int getTaskSize();
    void stop();
    bool isDestory();
    bool addThread();
    bool reduceThread();
private:
    std::deque<Task*>          m_task_list;
    std::set<pthread_t>        m_running_thread_list;
    std::set<pthread_t>        m_waiting_thread_list;
    volatile int               m_max_thread_num;
    volatile int               m_running_thread_num;
    pthread_cond_t             m_cond;
    pthread_mutex_t            m_mutex; 
    volatile bool              m_shutdown;
    volatile bool              m_reduce_flag;
    pthread_t*                 m_pids; 
};

#endif