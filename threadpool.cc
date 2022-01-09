#include "threadpool.h"

#include <iostream>

ThreadPool::ThreadPool ( int threadNum ) : 
    m_pids(NULL),
    m_shutdown(false)
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
    stop();
}

void* ThreadPool::threadFunc( void* data )
{
    ThreadPool* pool = (ThreadPool*)data;
    while ( !pool->m_shutdown )
    {
        pthread_mutex_lock( &pool->m_mutex );
        if( pool->m_task_list.size() == 0 )
        {
            pthread_cond_wait( &pool->m_cond, &pool->m_mutex );
        }
        Task* cur = pool->m_task_list.front();
        if( cur == NULL ){
            pthread_mutex_unlock( &pool->m_mutex );
            continue;
        }
        pool->m_task_list.pop_front();
        pthread_mutex_unlock( &pool->m_mutex );

        if( cur->getStatus() == INIT ){
            cur->setStatus( RUNNING );
            cur->run();
            cur->setStatus( FINISHED );
        }
    }
    return (void*)0;
}

int ThreadPool::addTask(Task* task)
{
    pthread_mutex_lock( &m_mutex );
    m_task_list.push_back( task );
    int size = m_task_list.size();
    pthread_mutex_unlock( &m_mutex );
    // 通知线程处理消费任务
    pthread_cond_signal( &m_cond );
    return size;
}

int ThreadPool::getTaskSize()
{
    pthread_mutex_lock( &m_mutex );
    int size = m_task_list.size();
    pthread_mutex_unlock( &m_mutex );
    return size;
}

void ThreadPool::init()
{
    pthread_mutex_init( &m_mutex, NULL );
    pthread_cond_init( &m_cond, NULL );
    if( createThread() != 0 ){
        std::cout << "create thread error!" << std::endl;
    }
    std::cout << "create success!" << std::endl;
}

int ThreadPool::createThread()
{
    m_pids = ( pthread_t* )malloc( sizeof( pthread_t ) * m_max_thread_num );
    for( int i = 0; i < m_max_thread_num; ++i ){
        if( pthread_create( m_pids+i, NULL, threadFunc, (void*)this ) != 0 ){
            return -1;
        }
    }
    return 0;
}

// 等待任务队列为空，且所有线程都运行结束。
void ThreadPool::stop()
{
    if( m_shutdown == true ){
        return;
    }
    // 消费task
    while( 1 ){
        if( m_task_list.size() == 0 ){
            break;
        }
    }
    m_shutdown = true;
    // 等待线程运行完
    pthread_cond_broadcast( &m_cond );
    for( int i=0; i<m_max_thread_num; ++i ){
        pthread_join( m_pids[i], NULL );
    }

    free( m_pids );
    m_pids = NULL;

    pthread_mutex_destroy( &m_mutex );
    pthread_cond_destroy( &m_cond );
}

bool ThreadPool::isDestory()
{
    return m_shutdown;
}