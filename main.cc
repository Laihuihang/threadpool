#include "threadpool.h"
#include "unistd.h"
#include <string>
#include <iostream>

class mytask : public Task{
public:
    mytask(std::string name, int num):
        Task( name ),
        m_num( num )
    {

    }
    void run()
    {
            std::cout << m_num << std::endl;
            //sleep(1);
    }
private:
    int m_num;
};

class mytask1 : public Task{
public:
    mytask1(std::string name, int num):
        Task( name ),
        m_num( num )
    {

    }
    void run()
    {
        while(1){

        }
    }
private:
    int m_num;
};


int main()
{
    ThreadPool pool( 16 );
    pool.init();

    // for( int i = 0; i < 5; ++i ){
    //     mytask1* task = new mytask1( "lai_huihang", i );
    //     std::cout << "task num:" << pool.addTask( task ) << std::endl;
    // }
    for( int i = 0; i < 1000; ++i ){
        mytask* task = new mytask( "lai_huihang", i );
        pool.addTask( task );
        //usleep(10);
    }
    sleep(1);
    mytask* task = new mytask( "lai_huihang", 1001 );
    pool.addTask( task );
    //std::cout << pool.getTaskSize() << std::endl;
    pool.stop();
    return 0;
}