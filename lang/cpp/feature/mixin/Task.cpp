#include <iostream>

template < class T >
class LoggingTask : public T
{
    public:
        void Execute() {
            // std::cout << "LOG: The task is starting - " << GetName.c_str() << std::endl;
            std::cout << "LOG: The task is starting - " << std::endl;
            T::Execute();
            // std::cout << "LOG: The task has completed - " << GetName.c_str() << std::endl;
            std::cout << "LOG: The task has completed - " << std::endl;
        }
};

template < class T >
class TimingTask : public T
{
    // Timer timer_;
    public:
    void Execute() {
        // timer._Reset();
        T::Execute();
        // std :: cout << "Task Duration: " << t << " seconds" << std::endl;
        std :: cout << "Task Duration: " << " 1(fake) " << " seconds" << std::endl;
    }
};

class MyTask
{
    public:
        void Execute() {
            std::cout << "...This is where the task is executed..." << std::endl;
        }
        std::string GetName() {
            return "MyTaskName";
        }
};

int main() {
    MyTask t0;
    LoggingTask < MyTask > t1;
    TimingTask < MyTask > t2;
    LoggingTask < TimingTask < MyTask > > t3;
    TimingTask < LoggingTask  < MyTask > > t4;
    t0.Execute();
    t1.Execute();
    t2.Execute();
    t3.Execute();
    t4.Execute();

    return 0;
}
