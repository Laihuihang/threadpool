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

    for( int i = 0; i < 100; ++i ){
        mytask* task = new mytask( "lai_huihang", i );
        pool.addTask( task );
    }
    sleep(3);
    std::cout << pool.getTaskSize() << std::endl;
    pool.stop();
    return 0;
}