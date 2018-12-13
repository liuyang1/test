#include "stdafx.h"
#include <iostream>
#include <windows.h>


// a simple timer class
struct Timer
{
    Timer() 
    {
        Reset();
    }

    double GetElapsedTimeSecs()
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter( &now );

        LARGE_INTEGER span;
        span.QuadPart = now.QuadPart - m_startTime.QuadPart;
        
        LARGE_INTEGER freq;
        QueryPerformanceFrequency( &freq ) ;
        return (double)span.QuadPart /(double)freq.QuadPart;
    }

    void Reset()
    {
        QueryPerformanceCounter(&m_startTime);
    }
    
private:

    LARGE_INTEGER m_startTime;
};


// Interface for all tasks to implement
class ITask
{
public:
    virtual ~ITask() {};
    virtual std::string GetName() = 0;
    virtual void Execute() = 0;
};



namespace Inheritance
{


class BaseLoggingTask : public ITask
{
public:
    virtual void Execute()
    {
        std::cout << "LOG: The task is starting - " << GetName().c_str() << std::endl;
        OnExecute();
        std::cout << "LOG: The task has completed - " << GetName().c_str() << std::endl;
    }

    virtual void OnExecute() = 0;
};

class BaseTimingTask : public ITask
{
    Timer timer_;

public:
    virtual void Execute()
    {
        timer_.Reset();
        OnExecute();
        double t = timer_.GetElapsedTimeSecs();
        std::cout << "Task Duration: " << t << " seconds" << std::endl;
    }

    virtual void OnExecute() = 0;
};

class MyTask : public BaseTimingTask
{
public:
    virtual void OnExecute()
    {
        std::cout << "...This is where the task is executed..." << std::endl;
    }

    virtual std::string GetName()
    {
        return "My task name";
    }
};

}


namespace Composition
{

class LoggingTask : public ITask
{
    ITask* task_;
public:
    LoggingTask( ITask* task ) : task_( task )
    {
    }

    ~LoggingTask()
    {
        delete task_;
    }

    virtual void Execute()
    {
        std::cout << "LOG: The task is starting - " << GetName().c_str() << std::endl;
        if( task_ ) task_->Execute();
        std::cout << "LOG: The task has completed - " << GetName().c_str() << std::endl;
    }

    virtual std::string GetName()
    {
        if( task_ )
        {
            return task_->GetName();
        }
        else
        {
            return "Unbound LoggingTask";
        }
    }
};

class TimingTask : public ITask
{
    ITask* task_;
    Timer timer_;
public:
    TimingTask( ITask* task ) : task_( task )
    {
    }

    ~TimingTask()
    {
        delete task_;
    }

    virtual void Execute()
    {
        timer_.Reset();
        if( task_ ) task_->Execute();
        double t = timer_.GetElapsedTimeSecs();
        std::cout << "Task Duration: " << t << " seconds" << std::endl;
    }

    virtual std::string GetName()
    {
        if( task_ )
        {
            return task_->GetName();
        }
        else
        {
            return "Unbound TimingTask";
        }
    }
};

class MyTask : public ITask
{
public:
    virtual void Execute()
    {
        std::cout << "...This is where the task is executed..." << std::endl;
    }

    virtual std::string GetName()
    {
        return "My task name";
    }
};

}



namespace Mixin
{

template< class T >
class LoggingTask : public T
{
public:
    void Execute()
    {
        std::cout << "LOG: The task is starting - " << GetName().c_str() << std::endl;
        T::Execute();
        std::cout << "LOG: The task has completed - " << GetName().c_str() << std::endl;
    }
};

template< class T >
class TimingTask : public T
{
    Timer timer_;
public:
    void Execute()
    {
        timer_.Reset();
        T::Execute();
        double t = timer_.GetElapsedTimeSecs();
        std::cout << "Task Duration: " << t << " seconds" << std::endl;
    }
};

class MyTask
{
public:
    void Execute()
    {
        std::cout << "...This is where the task is executed..." << std::endl;
    }

    std::string GetName()
    {
        return "My task name";
    }
};

template< class T >
class TaskAdapter : public ITask, public T
{
public:
    virtual void Execute()
    {
        T::Execute();
    }

    virtual std::string GetName()
    {
        return T::GetName();
    }
};

}

int main(int argc, _TCHAR* argv[])
{
    std::cout << "Inheritance approach..." << std::endl;
    {
        using namespace Inheritance;

        MyTask t;
        t.Execute();
    }

    
    std::cout << std::endl;
    std::cout << "Composition approach..." << std::endl;
    { 
        using namespace Composition;

        // Note: forced heap allocations, pointers, runtime checks, virtual functions, lack of compiler optimisations
        ITask* t = new LoggingTask( 
                      new TimingTask( 
                          new MyTask() ) );
        t->Execute();
        delete t;
    }

    
    std::cout << std::endl;
    std::cout << "Mixin approach..." << std::endl;
    {
        using namespace Mixin;

        // Note: no heap allocations, pointer, runtime checks, virtual functions and the optimizing compiler goes nuts
        typedef public TaskAdapter< 
                            LoggingTask< 
                                TimingTask< 
                                    MyTask > > > task;

        task t;
        ITask* it = &t;
        it->Execute();
    }
    

    std::cout << "Press enter to exit...";
    std::cin.get();
	return 0;
}

