#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <string>

enum Status{
    INIT = 0,
    RUNNING,
    FINISHED
};

class Task{
public:
    Task(std::string name, void* data = NULL):
        m_name(name),
        m_data(data)
    {

    }
    virtual ~Task(){};
    virtual void run() = 0;
protected:
    std::string     m_name;
    Status          m_status;
    void*           m_data;
};



#endif