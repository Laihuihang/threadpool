#include <cstring>
#include <iostream>
#include "threadpool.h"

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
    while ( 1 )
    {
        pthread_mutex_lock( &pool->m_mutex );
        while( pool->m_task_list.size() == 0 && !pool->m_shutdown ){
            pthread_cond_wait( &pool->m_cond, &pool->m_mutex );
        }
        if( pool->m_shutdown ){
            pthread_mutex_unlock( &pool->m_mutex );
            break;
        }
        // 当queue中堆积的任务个数大于最大线程个数，需要扩容
        if( pool->m_task_list.size() > pool->m_max_thread_num ){
            
        }
        Task cur = pool->m_task_list.front();
        pool->m_task_list.pop_front();

        pthread_mutex_unlock( &pool->m_mutex );
        cur();
    }
    return (void*)0;
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

// bool ThreadPool::addThread()
// {
//     if ( m_max_thread_num == MAX_THREAD_NUM ){
//         return true;
//     }
//     int thread_num = 2 * m_max_thread_num;
//     if( thread_num > MAX_THREAD_NUM ){
//         thread_num = MAX_THREAD_NUM;
//     }
//     pthread_t* new_pids = (pthread_t*)malloc( sizeof(pthread_t) * thread_num );
//     memcpy( new_pids, m_pids, m_max_thread_num * sizeof(pthread_t) );
//     for( int i=m_max_thread_num; i<thread_num; ++i ){
//         if( pthread_create( new_pids+i, NULL, threadFunc, this ) != 0 ){
//             return false;
//         }
//     }
//     free( m_pids );
//     m_pids = new_pids;
//     m_max_thread_num = thread_num;
//     return true;
// }

// bool ThreadPool::reduceThread()
// {
//     if( m_max_thread_num <= MIN_THREAD_NUM ){
//         return true;
//     }
//     //std::cout << m_max_thread_num << std::endl;
//     int thread_num = m_max_thread_num / 2;
//     if( thread_num < MIN_THREAD_NUM ){
//         thread_num = MIN_THREAD_NUM;
//     }
//     pthread_t* new_pids = (pthread_t*)malloc( sizeof(pthread_t) * thread_num );
//     int index = 0;
//     // 将m_running_task_list拷贝到新的pids中
//     for( std::set<pthread_t>::iterator it = m_running_thread_list.begin(); 
//                                        it != m_running_thread_list.end(); 
//                                        it++ ){
//         new_pids[index++] = *it;
//     }
//     m_reduce_flag = true;
//     pthread_cond_broadcast( &m_cond );
//     // 这函数里面是有锁的，threadFunc里面唤醒后也要获取锁，因此线程销毁不了。
//     pthread_mutex_unlock( &m_mutex );
//     // 将多余的线程销毁掉
//     for( std::set<pthread_t>::iterator it = m_waiting_thread_list.begin(); 
//                                        it != m_waiting_thread_list.end(); 
//                                        it++ ){
//         pthread_join( *it, NULL );
//     }
//     pthread_mutex_lock( &m_mutex );
//     m_reduce_flag = false;
//     m_waiting_thread_list.clear();
//     // 将不足的线程补上
//     for( int i=m_running_thread_list.size(); i<thread_num; i++ ){
//         if( pthread_create( new_pids + i, NULL, threadFunc, (void*)this ) !=0 ){
//             return false;
//         }
//         // 更新m_waiting_task_list队列
//         m_waiting_thread_list.insert( *( m_pids + i ) );
//     }
//     free( m_pids );
//     m_pids = new_pids;
//     m_max_thread_num = thread_num;
//     return 0;
// }