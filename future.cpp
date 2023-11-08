/*
c++11 �����첽�����ṩ�� future ��ص��࣬
��Ҫ�� std::future��std::promise �� std::packaged_task
std::future �� std::thread �߼�Щ��std::future ��Ϊ�첽����Ĵ���ͨ����ͨ�� get()���Ժܷ���Ļ�ȡ�̺߳����ķ���ֵ
std::promise ������װһ��ֵ�������ݺ� future ��������
�� std::packaged_task ��������װһ�����ö��󣬽������� future �������������첽���á�
�� std::future �ǲ����Ը��Ƶģ������Ҫ���Ʒŵ������п���ʹ�� std::shared_future

std::future ���ڷ����첽�����Ľ����
std::promise �� std::packaged_task �� future ��һ�㣬�����ڲ�����һ�� future
promise��װ����һ��ֵ��packaged_task ��װ����һ������
����Ҫ��ȡ�߳��е�ĳ��ֵ������ʹ�� std::promise
����Ҫ��ȡ�̺߳�������ֵ������ʹ�� std::packaged_task
*/

#include <iostream>
#include <future>
#include <functional>
#include <thread>

void func_1(std::future<int>& fut) {
	int x = fut.get();
	std::cout << "value: " << x << std::endl;
}

void func_2(std::promise<int>& pro) {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	pro.set_value(100);
}

int Func(int in) {
	return in + 1;
}

int main() {
	std::promise<int> prom1;
	std::future<int> fut1 = prom1.get_future();
	std::thread t1(func_1, std::ref(fut1));
	prom1.set_value(144);
	t1.join();

	std::promise<int> prom2;
	std::future<int> fut2 = prom2.get_future();
	std::thread t2(func_2, std::ref(prom2));
	t2.detach();
	//��Ҫ prom2 ���� set_value ��Ż���� get
	std::cout << "result: " << fut2.get() << std::endl;
	

	std::packaged_task<int(int)> task(Func);
	std::future<int> Fut = task.get_future();
	std::thread(std::move(task), 5).detach();
	

	std::cout << "result " << Fut.get() << std::endl;
	return 0;
}