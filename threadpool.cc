#include "threadpool.h"


ThreadPool::ThreadPool ( int threadNum )
{
    if( threadNum > MAX_THREAD_NUM ){
        m_max_thread_num = MAX_THREAD_NUM;
    }else if( threadNum < MIN_THREAD_NUM ){
        m_max_thread_num = MIN_THREAD_NUM;
    }else{
        m_max_thread_num = threadNum;
    }
}
ThreadPool::~ThreadPool()
{

}
void ThreadPool::threadFunc()
{

}
int ThreadPool::addTask(Task* task)
{

}
int ThreadPool::getTaskSize()
{

}
void ThreadPool::init()
{
    pthread_mutex_init( &m_mutex );
    pthread_cond_init( &m_cond );
    createThread();
}

int ThreadPool::createThread()
{

}