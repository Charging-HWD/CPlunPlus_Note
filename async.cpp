/*
async �Ǳ� future��packaged_task��promise ���߼��Ķ���
���ǻ���������첽������ͨ�� async ����ֱ�Ӵ����첽������
���صĽ���ᱣ���� future �У�����Ҫ�� packaged_task �� promise ��ô�鷳

async �����﷨���£�
async(std::launch::async | std::launch::deferred, func, args...);
��һ�������Ǵ������ԣ�
std::launch::async ��ʾ����ִ������һ�߳�
std::launch::deferred ��ʾ�ӳ�ִ�����񣬵��� get ���� wait ʱ�Ż�ִ�У����ᴴ���̣߳�����ִ���ڵ�ǰ�̡߳�
�������ȷָ���������ԣ��������������� async ��Ĭ�ϲ��ԣ�����δ���壬����һ����������ĳ�����ƣ��ڲ���һ������
��(�̳߳�)�������ʵ����������������ֲ��ԡ�

���� std::async ��õ� std::future δ���ƶ���󶨵����ã������������ʽ��β
std::future ����������������ֱ���첽������ɣ�ʵ�����൱��ͬ��������
std::async(std::launch::async, []{ f(); }); // ��ʱ�������������ȴ� f()
std::async(std::launch::async, []{ g(); });
*/

#include <functional>
#include <future>
#include <iostream>
#include <thread>

int func(int in) 
{ 
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout <<"func return " << std::endl; 
	return in + 1;
}

int main() {
	auto res = std::async(func, 5);
	std::cout << "do thing" << std::endl;
	std::cout << res.get() << std::endl; // ����ֱ����������
	return 0;
}