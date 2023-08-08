/*
�첽����(std::async):async��˼�ǽ�һ������A������һ�߳���ȥ���С�A�����Ǿ�̬������ȫ�ֺ������������Ա������
*/

/*
std::promise��һ��ģ����: 
template<typename R> class promise
�䷺�Ͳ���RΪstd::promise���󱣴��ֵ�����ͣ�R������void���͡�
std::promise�����ֵ�ɱ���֮������std::future��ȡ����ȡ�������Է����������̡߳�
std::promise����move����(��ֵ���죬��ֵ��ֵ)������������(�������졢��ֵ)��std::future��Ȼ��
std::promise��std::future������ͬʵ���˶��̼߳�ͨ�š�
*/

/*
ͨ����Ա���� set_value �������� std::promise �б����ֵ����ֵ���ջᱻ��֮������ std::future::get ��ȡ��
set_valueֻ�ܱ�����һ�Σ���ε��û��׳�std::future_error�쳣��
*/

/*
һ��std::promiseʵ��ֻ����һ��std::future��������״̬������ͬһ��std::promise�Ϸ�������get_future���׳�future_error�쳣��
��std::promise����ʱ��std::promise������빲��״̬�����������������״̬���Դ洢һ��R���͵�ֵ����һ����std::exception�����������쳣ֵ��
ͨ��std::promise::get_future���û�õ�std::future��std::promise������ͬ�Ĺ���״̬��
*/

/*
std::promise<void>�ǺϷ��ġ���ʱstd::promise.set_value�������κβ�����������֪ͨ������std::future.get()���������
*/

/*
std::promise֧�ֶ����߳��˳�ʱ����Ϊ��
    std::promise::set_value_at_thread_exit �߳��˳�ʱ��std::future�յ�ͨ���ú������õ�ֵ��
    std::promise::set_exception_at_thread_exit �߳��˳�ʱ��std::future���׳��ú���ָ�����쳣��
*/

/*

*/

#include <iostream> 
#include <thread>   
#include <string>  
#include <future>//std::promise, std::future
#include <chrono> 
using namespace std::chrono;

std::mutex _mtx;
std::condition_variable _cv;
void task1(int a, int b, int& ret)
{
	int ret_a = a * a;
	int ret_b = b * 2;
	std::this_thread::sleep_for(seconds(3));
	std::unique_lock<std::mutex> lock(_mtx);
	ret = ret_a + ret_b;
	_cv.notify_one();
}

//���߳̽��и�ֵ�����߳̽��л�ȡ
void task2(int a, int b, std::promise<int>& ret)
{
	int ret_a = a * a;
	int ret_b = b * 2;	
	ret.set_value(ret_a + ret_b);
}

//���߳��и�ֵ�����߳���ȡֵ����ֵ�����߳�����ȡֵ
void task3(int a,std::future<int>& b, std::promise<int>& ret)
{
	int ret_a = a * a;
	int ret_b = b.get() * 2;
	ret.set_value(ret_a + ret_b);
}

//���߳̽��и�ֵ�����߳̽��л�ȡ,������߳�
//shared_future�ɶ��get
void task4(int a, std::shared_future<int> b, std::promise<int> &ret)
{
	int ret_a = a * a;
	int ret_b = b.get() * 2;
	ret.set_value(ret_a + ret_b);
}

void read(std::future<std::string>* future) {
    std::cout << future->get() << std::endl;// future��һֱ������ֱ����ֵ����
}

int main() {
	/*************************ͨ�����������������и�ֵ���ȡ*************************/
	int ret = 0;
	std::thread t1(task1, 1, 2, std::ref(ret));
	//do something else
	//
	//get the return value
	std::unique_lock<std::mutex>lock(_mtx);
	_cv.wait(lock);
	std::cout << "return value is " << ret << std::endl;
	t1.join();
	/*************************ͨ�����������������и�ֵ���ȡ*************************/

	/*************************���߳̽��и�ֵ�����߳̽��л�ȡ*************************/
	std::promise<int> p1;
	std::future<int> f1 = p1.get_future();//�� future �� promise ��
	std::thread t2(task2, 1, 2, std::ref(p1));
	//do something else
	//
	//get the return value
	std::cout << "return value is " << f1.get() << std::endl;//get()ֻ�ܽ���һ��
	t2.join();
	/*************************���߳̽��и�ֵ�����߳̽��л�ȡ*************************/

	/*************************���߳��и�ֵ�����߳���ȡֵ����ֵ�����߳�����ȡֵ*************************/
	{
		std::promise<int> p_in;  std::future<int> f_in = p_in.get_future();
		std::promise<int> p_out; std::future<int> f_out = p_out.get_future();
		std::thread t3(task3, 1, std::ref(f_in), std::ref(p_out));
		p_in.set_value(2);
		std::cout << "return value is " << f_out.get() << std::endl;//get()ֻ�ܽ���һ��
		t3.join();
	}
	/*************************���߳��и�ֵ�����߳���ȡֵ����ֵ�����߳�����ȡֵ*************************/

	/*************************���߳̽��и�ֵ��������߳̽��л�ȡ*************************/
	{
		std::promise<int> p_in; std::future<int> f_in = p_in.get_future();
		std::promise<int> p_out1; std::future<int> f_ret1 = p_out1.get_future();
		std::promise<int> p_out2; std::future<int> f_ret2 = p_out2.get_future();
		std::promise<int> p_out3; std::future<int> f_ret3 = p_out3.get_future();
		std::shared_future<int> s_f = f_in.share();

		//������̻߳�ȡֵ
		std::thread t4(task4, 2, s_f, std::ref(p_out1));
		std::thread t5(task4, 3, s_f, std::ref(p_out2));
		std::thread t6(task4, 4, s_f, std::ref(p_out3));
		//do something else
		//
		p_in.set_value(2);
		std::cout << "return value is " << f_ret1.get() << std::endl;
		std::cout << "return value is " << f_ret2.get() << std::endl;
		std::cout << "return value is " << f_ret3.get() << std::endl;
		t4.join();
		t5.join();
		t6.join();
	}
	/*************************���߳̽��и�ֵ��������߳̽��л�ȡ*************************/

	/*************************���߳��и�ֵ�����߳���ȡֵ*************************/
	{
		std::promise<std::string> promise;                      // promise �൱�������ߣ����³�ŵ(����)
		std::future<std::string> future = promise.get_future(); // future �൱��������, ��ֵ���죬�������߹�����δ���õ�������ֵ(��ŵ)
		std::thread thread(read, &future);                      // ����һ�߳���ͨ�� future ����ȡ������ֵ
		std::this_thread::sleep_for(seconds(1));                // ��read��һ���
		promise.set_value("hello future");                      // �������������
		thread.join();                                          // ����̨��: hello future
	}
	/*************************���߳��и�ֵ�����߳���ȡֵ*************************/
	return 0;
}

