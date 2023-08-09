/*
packaged_task������һ��������������������Ľ�����ݸ�std::future��������������ʱ�������쳣��
std::packaged_task֧��move������֧�ֿ���
std::packaged_task��װ�ĺ����ļ�������ͨ����֮��ϵ��std::future::get��ȡ(��Ȼ�������������߳����첽��ȡ)��
������std::future����ͨ��std::packaged_task::get_future��ȡ����get_future���ܵ���һ�Σ���ε��ûᴥ��std::future_error�쳣��
std::package_task���˿���ͨ���ɵ��ö������⣬��֧��ȱʡ����(�޲ι���)������ʱ����Ķ�����ֱ��ʹ�ã���ͨ����ֵ��ֵ���������˿ɵ��ö��������ſ�ʹ�á�
*/

/*
std::packaged_task::valid
�ú��������ж�std::packaged_task�����Ƿ�����Ч״̬��
��ͨ��ȱʡ�����ʼ��ʱ��������δ�����κοɵ��ö��������valid�᷵��false��
ֻ�е�std::packaged_task��������Ч�ĺ�����ɵ��ö���valid�ŷ���true��
*/

/*
std::packaged_task::operator()(ArgTypes...)
�ú��������std::packaged_task��������װ�ɵ��ö���R�����亯��ԭ����R���в�ͬ:void operator()(ArgTypes... );
operator()�ķ���ֵ��void�����޷���ֵ��
std::packaged_task�������Ҫ�����������첽���ã����R(ArgTypes...)�ļ�������ͨ��std::future::get����ȡ�ġ��ú�������ʵ�ؽ�R�ļ�����������std::future����ʹR�׳��쳣(��ʱstd::future::getҲ���׳�ͬ�����쳣)��
*/

/*
��std::packaged_task���߳��˳�ʱ�ٽ����������std::future
std::packaged_task::make_ready_at_thread_exit�������յĲ�����operator()(_ArgTypes...)һ������ΪҲһ����
ֻ��һ�����Ǿ��ǲ��Ὣ���������̷�����std::future����������ִ��ʱ���ڵ��߳̽�����std::future::get�Ż�ȡ�ý����
*/

/*
std::packaged_task::reset
��std::promise��һ���� std::promise������ִ��һ��set_value��set_exception��������std::packagged_task����ִ�ж��.
����ؾ���reset����ͨ�����¹���һ�� promise ���ﵽ��ε��õ�Ŀ�ġ�
����reset����Ҫ����get_future���Ա��ȡ�´�operator()ִ�еĽ����
���������¹�����promise�����reset����������Ӱ��֮ǰ���õ�make_ready_at_thread_exit�����
��֮ǰ�Ķ��Ƶ���Ϊ���߳��˳�ʱ�Իᷢ����
*/

#include <iostream> 
#include <thread>  
#include <future>   // std::packaged_task, std::future

int sum(int a, int b) {
    int ret_a = a * a;
    int ret_b = b * 2;
    return ret_a + ret_b;
}

int main() {
    /*****************************************/
    auto a = std::bind(sum, 3, 4);
    int ret = a();
    std::cout << "return value is " << ret << std::endl;

    auto b = std::bind(sum, 3, 4);
    std::packaged_task<int()> t(b);
    t();
    std::cout << "return value is " << t.get_future().get() << std::endl;
    /*****************************************/
   
    /**************��������а�װ******************/
    std::packaged_task<int(int, int)> task1(sum);
    task1(1, 2);
    std::cout << "return value is " << task1.get_future().get() << std::endl;
    /**************��������а�װ******************/

    /**************��������а�װ******************/
    std::packaged_task<int(int, int)> task2(sum);
    std::future<int> future = task2.get_future();
    std::thread t1(std::move(task2),1,2);// std::thread�ĵ�һ��������ֹ�Ǻ�������������һ���ɵ��ö��󣬼�֧��operator()(Args...)����
    t1.join();
    // �ȴ��첽������
    std::cout << "return value is " << future.get() << std::endl;
    /**************��������а�װ******************/
 
    std::packaged_task<void()> task3;                            // ȱʡ���졢Ĭ�Ϲ���
    std::cout << std::boolalpha << task3.valid() << std::endl;   // false

    std::packaged_task<void()> task4(std::move(task3));          // ��ֵ����
    std::cout << std::boolalpha << task4.valid() << std::endl;   // false

    task3 = std::packaged_task<void()>([]() {});                 // ��ֵ��ֵ, �ɵ��ö���
    std::cout << std::boolalpha << task3.valid() << std::endl;   // true
    return 0;
}
