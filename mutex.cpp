/*
mutex�ֳƻ������������ṩ�Թ�������Ļ�����ʡ�C++11��mutex��ص��඼��<mutex>ͷ�ļ��С������ֻ�����:
std::mutex 					�����Ҳ����õĻ�����;
std::recursive_mutex 		ͬһ�߳��ڿɵݹ�(����)�Ļ�����;
std::timed_mutex 			���߱�mutex�����⣬���ṩ�˴�ʱ����������������
std::recursive_timed_mutex 	ͬһ�߳��ڿɵݹ�(����)��timed_mutex

��std::threadһ����mutex����಻֧�ֿ������졢��֧�ָ�ֵ��ͬʱmutex��Ҳ��֧��move����(move���졢move��ֵ)��
*/


/*
mutex�ı�׼����:lock,try_lock,unlock;
����lockʱ���������:
    1.���������û�б���ס��������߳̽���mutex��ס��ֱ�������̵߳���unlock�ͷš�(�߳����������ִ��)��
    2.���mutex�ѱ������߳�lock��������߳̽���������ֱ�������߳�unlock��mutex�� (�߳������������߳̽������߳�����������ִ��)
    3.�����ǰmutex�Ѿ����������߳���ס����std::mutex��������recursiveϵ����ɹ����ء�
����unlock:
    ����mutex���ͷŶ�mutex������Ȩ������recursiveϵ��mutex��unlock������Ҫ��lock������ͬ�ſ�����ȫ������
����try_lock,������סmutex�����øú���ͬ��Ҳ���������:
    1.���������û�б���ס��������߳̽���mutex��ס(����true)��ֱ�������̵߳���unlock�ͷš�
    2.���mutex�ѱ������߳�lock��������߳̽�ʧ�ܣ�������false��
    3.�����ǰmutex�Ѿ����������߳���ס����std::mutex��������recursiveϵ����ɹ�����true��
*/

/*
try_lock_for,try_lock_until
����������������timedϵ�е�mutex(std::timed_mutex, std::recursive_timed_mutex)����������ȴ�ָ����ʱ�䣬�����δ��������򷵻�false��
����ʱ�趨�⣬������������try_lock��Ϊһ�¡�

//�ȴ�ָ��ʱ��
template <class Rep, class Period>
    try_lock_for(const chrono::duration<Rep, Period>& rel_time);
//�ȴ���ָ��ʱ��
template <class Clock, class Duration>
    try_lock_until(const chrono::time_point<Clock, Duration>& abs_time);
*/

/*
lock_guard,�ڹ��캯�������������������н�����lock_guard��һ��ģ���࣬��ԭ��Ϊ template <class Mutex> class lock_guard
lock_guard ����������������������mutex�е����κ������ڵĹ������������������ʹ�õ�ʱ����ȷ��lock_guard�����mutexһֱ��Ч��
locak_guard����Ʊ�֤�˼�ʹ�����������ڼ䷢�����쳣��Ҳ�ᰲȫ���ͷ��������ᷢ��������
*/

/*
unique_lockӵ�ж�Mutex������Ȩ��һ����ʼ����unique_lock����ͽӹ��˸�mutex, ��unique_lock������������ǰ(����ǰ)�������ط��Ͳ�Ҫ��ֱ��ʹ�ø�mutex��
unique_lock �� lock_guard �������
*/

/*
std::call_once:���õĺ���ֻ��ִ��һ��,�ú�����Ҫ��std::once_flag���ʹ�á�
std::once_flag �����Ϊ�����յģ����ⲿû���κ��������Ըı� once_flag ��ֵ��������ͨ�� std::call_once �����޸ġ�
һ��������������Լ�ʵ��call_onceЧ��ʱ������ʹ��һ��ȫ�ֱ������Լ�˫�ؼ����(DCL)��ʵ�֣�����������ʵ����Ȼ���кܶ�����
*/

/*
try_lock:���ж��mutex��Ҫִ��try_lockʱ,try_lock�ᰴ���������ҵ�˳�򣬶�mutex˳��ִ��try_lock������
������ĳ��mutex.try_lockʧ��(����false���׳��쳣)ʱ���ѳɹ�������mutex������������
�ú����ɹ�ʱ����-1�� ���򷵻�ʧ��mutex��������������0��ʼ������
template <class L1, class L2, class... L3>
  int try_lock(L1&, L2&, L3&...);
*/


/*
std::lock,�����ܵ�������������ʽ�����������㷨������������mutex�б�����������
�ú�����mutex�б������˳���ǲ�ȷ���ģ�����ɹ���������mutexȫ�����������ʧ�ܣ���ȫ��������
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
        std::cout << "�������" << std::endl;
    }
    else {
        std::cout << "δ�����" << std::endl;
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
    std::cout << counter << std::endl;// �����5000�����inc�е��õ���try_lock����˴����ܻ�<5000
    /************************************/

    /************************************/
    std::timed_mutex t_mutex;
    t_mutex.lock();
    std::thread thread1(run500ms, std::ref(t_mutex));//�����δ�����
    thread1.join();
    t_mutex.unlock();
    std::thread thread2(run500ms, std::ref(t_mutex));//������������
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
    //ִֻ����һ�� initialize()����
    /************************************/
    return 0;
}