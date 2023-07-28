/*
mutex又称互斥量，用于提供对共享变量的互斥访问。C++11中mutex相关的类都在<mutex>头文件中。共四种互斥类:
std::mutex 					最基本也是最常用的互斥类;
std::recursive_mutex 		同一线程内可递归(重入)的互斥类;
std::timed_mutex 			除具备mutex功能外，还提供了带时限请求锁定的能力
std::recursive_timed_mutex 	同一线程内可递归(重入)的timed_mutex

与std::thread一样，mutex相关类不支持拷贝构造、不支持赋值。同时mutex类也不支持move语义(move构造、move赋值)。
*/


/*
mutex的标准操作:lock,try_lock,unlock;
调用lock时有三种情况:
    1.如果互斥量没有被锁住，则调用线程将该mutex锁住，直到调用线程调用unlock释放。(线程上锁后继续执行)。
    2.如果mutex已被其它线程lock，则调用线程将被阻塞，直到其它线程unlock该mutex。 (线程阻塞，其他线程解锁后，线程上锁并继续执行)
    3.如果当前mutex已经被调用者线程锁住，则std::mutex死锁，而recursive系列则成功返回。
调用unlock:
    解锁mutex，释放对mutex的所有权。对于recursive系列mutex，unlock次数需要与lock次数相同才可以完全解锁。
调用try_lock,尝试锁住mutex，调用该函数同样也有三种情况:
    1.如果互斥量没有被锁住，则调用线程将该mutex锁住(返回true)，直到调用线程调用unlock释放。
    2.如果mutex已被其它线程lock，则调用线程将失败，并返回false。
    3.如果当前mutex已经被调用者线程锁住，则std::mutex死锁，而recursive系列则成功返回true。
*/

/*
try_lock_for,try_lock_until
这两个函数仅用于timed系列的mutex(std::timed_mutex, std::recursive_timed_mutex)，函数最多会等待指定的时间，如果仍未获得锁，则返回false。
除超时设定外，这两个函数与try_lock行为一致。

//等待指定时长
template <class Rep, class Period>
    try_lock_for(const chrono::duration<Rep, Period>& rel_time);
//等待到指定时间
template <class Clock, class Duration>
    try_lock_until(const chrono::time_point<Clock, Duration>& abs_time);
*/

/*
lock_guard,在构造函数中上锁，析构函数中解锁。lock_guard是一个模板类，其原型为 template <class Mutex> class lock_guard
lock_guard 仅用于上锁、解锁，不对mutex承担供任何生周期的管理，不允许拷贝，因此在使用的时候，请确保lock_guard管理的mutex一直有效。
locak_guard的设计保证了即使程序在锁定期间发生了异常，也会安全的释放锁，不会发生死锁。
*/

/*
unique_lock拥有对Mutex的所有权，一但初始化了unique_lock，其就接管了该mutex, 在unique_lock结束生命周期前(析构前)，其它地方就不要再直接使用该mutex。
unique_lock 比 lock_guard 更加灵活
*/

/*
std::call_once:调用的函数只被执行一次,该函数需要与std::once_flag配合使用。
std::once_flag 被设计为对外封闭的，即外部没有任何渠道可以改变 once_flag 的值，仅可以通过 std::call_once 函数修改。
一般情况下我们在自己实现call_once效果时，往往使用一个全局变量，以及双重检查锁(DCL)来实现，即便这样该实现仍然会有很多问题
*/

/*
try_lock:当有多个mutex需要执行try_lock时,try_lock会按参数从左到右的顺序，对mutex顺次执行try_lock操作。
当其中某个mutex.try_lock失败(返回false或抛出异常)时，已成功锁定的mutex都将被解锁。
该函数成功时返回-1， 否则返回失败mutex的索引，索引从0开始计数。
template <class L1, class L2, class... L3>
  int try_lock(L1&, L2&, L3&...);
*/


/*
std::lock,较智能的上批量上锁方式，采用死锁算法来锁定给定的mutex列表，避免死锁。
该函数对mutex列表的上锁顺序是不确定的，如果成功，则所有mutex全部上锁，如果失败，则全部解锁。
template <class L1, class L2, class... L3>
  void lock(L1&, L2&, L3&...);
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

void inc(std::mutex& mutex, int loop, int& counter) {
    for (int i = 0; i < loop; i++) {
        mutex.lock();
        ++counter;
        mutex.unlock();
    }
}

void run500ms(std::timed_mutex& mutex) {
    auto _500ms = std::chrono::milliseconds(500);
    if (mutex.try_lock_for(_500ms)) {
        std::cout << "获得了锁" << std::endl;
    }
    else {
        std::cout << "未获得锁" << std::endl;
    }
}

void initialize() {
    std::cout << __FUNCTION__ << std::endl;
}
std::once_flag of;
void my_thread() {
    std::call_once(of, initialize);
}

int main() {
    /************************************/
    std::thread threads[5];
    std::mutex mutex;
    int counter = 0;

    for (std::thread& thr : threads) {
        thr = std::thread(inc, std::ref(mutex), 1000, std::ref(counter));
    }
    for (std::thread& thr : threads) {
        thr.join();
    }
    std::cout << counter << std::endl;// 输出：5000，如果inc中调用的是try_lock，则此处可能会<5000
    /************************************/

    /************************************/
    std::timed_mutex t_mutex;
    t_mutex.lock();
    std::thread thread1(run500ms, std::ref(t_mutex));//输出：未获得锁
    thread1.join();
    t_mutex.unlock();
    std::thread thread2(run500ms, std::ref(t_mutex));//输出：获得了锁
    thread2.join();
    /************************************/

    /************************************/
    std::thread threads2[10];
    for (std::thread& thr : threads2) {
        thr = std::thread(my_thread);
    }
    for (std::thread& thr : threads2) {
        thr.join();
    }
    //只执行了一次 initialize()函数
    /************************************/
    return 0;
}