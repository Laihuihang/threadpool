#include "unistd.h"
#include <string>
#include <iostream>
#include "threadpool.h"

class FUN{
public:
    static std::string func1(int i, std::string str){
        std::cout << "i:" << i << " str:" << str << std::endl;
        return str;
    }
};

void func2(int j){
    std::cout << "j:" << j << std::endl;
}

struct func3{
    int operator() (int x){
        std::cout << "x:" << x << std::endl;
        return x;
    }
};

int main()
{
    ThreadPool pool( 16 );
    pool.init();
    std::future<std::string> ret = pool.addTask(FUN::func1, 1, "lai");
    std::cout << "ret:" << ret.get() << std::endl;
    sleep(1);
    pool.addTask(func2, 2);
    sleep(1);
    std::future<int> ret1 = pool.addTask(func3(), 3);
    std::cout << "ret1:" << ret1.get() << std::endl;
    sleep(1);
    std::future<int> ret2 = pool.addTask([](int i, int j) -> int { std::cout << i << j << std::endl; return i+j;}, 4,5);
    std::cout << "ret2:" << ret2.get() << std::endl;
    pool.stop();
    return 0;
}