#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <memory>
#include <vector>
#include <set>
#include <deque>
#include <string>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include "pthread.h"

#define MAX_THREAD_NUM 128
#define MIN_THREAD_NUM 16


class ThreadPool{
    using Task = std::function<void()>;
public:
    ThreadPool( int threadNum );
    virtual ~ThreadPool();
    static void* threadFunc(void*);

    void init();
    // 创建线程
    int createThread();

    // 添加任务
    template<typename F, typename... Args>
    auto addTask(F&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        using RetType = decltype( func(args...) );
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...)
        );
        std::future<RetType> future = task->get_future();
        
        pthread_mutex_lock( &m_mutex );
        m_task_list.emplace_back( [task](){ (*task)(); } );
        pthread_mutex_unlock( &m_mutex );


        pthread_cond_signal( &m_cond );

        return future;
    }
    // 获取任务队列大小
    int getTaskSize();
    // 停止线程池
    void stop();
    // 返回线程池是否结束
    bool isDestory();
    // 添加线程池中的线程数
    bool addThread();
    // 减少线程池中的线程数
    bool reduceThread();
private:
    // 任务队列
    std::deque<Task>           m_task_list;
    // 正在运行的线程
    std::set<pthread_t>        m_running_task_list;
    // 等待任务的线程
    std::set<pthread_t>        m_wait_task_list;
    volatile int               m_max_thread_num;
    volatile int               m_running_thread_num;
    pthread_cond_t             m_cond;
    pthread_mutex_t            m_mutex; 
    volatile bool              m_shutdown;
    pthread_t*                 m_pids; 
};

#endif
