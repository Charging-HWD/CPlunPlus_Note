/*
������ʵ�֣�
	1.���߳�ʵ�ֲ���
		��Ҫ�������ͨ�ż�����
		NO.1 һ�������ϣ��ܵ����ļ�����Ϣ���С������ڴ�
		NO.2 ��ͬ����ͨ��SOCKET����ͨ��ʵ��
	2.�������̣�����߳�ʵ�ֲ�������һ�����̶߳�����߳�ʵ�ֲ���
		һ�������е������̹߳�����ڴ�ռ�:ȫ�ֱ�����ָ������
*/

/*
�̵߳Ķ��ִ�����ʽ��
	1.ͨ��thread����������
	2.ͨ����Ͷ���
	3.ͨ��C++Lambada���ʽ��
	4.���η�ʽ��
	5.������ָ�룻
	6.��ĳ�Ա������
*/

/*
�߳�ֹͣ��
һ������µ��̺߳���ִ����ɺ��̡߳���Ȼ��ֹͣ������std::thread ����ʱ���ܻ�����߳��쳣��ֹ��
�������ǰ���߳̽���������ʹ�á�������������̶߳��ڵ�ȥ�������������Ҫ�˳�����ִֹͣ�У��˳��̺߳�����
ʹ�á������������Ҫע�⣬�ڶ�ˡ���CPU���������Ҫʹ�á�ԭ�ӡ�����
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

class Greet
{
	const char* owner = "Greet";
public:
	//STL�º���
	void operator()()
	{
		std::cout << "��º������߳�����" << std::endl;
	}
	void func(int& num)
	{
		std::cout << "�в����Ա����" << num << std::endl;
		std::cout << "ID:" << std::this_thread::get_id() << std::endl;
	}
	void SayHello(const char* name) {
		std::cout << "Hello " << name << " from " << this->owner << std::endl;
	}
};

//���κ������Ƿ������ò�����
void func1(int& a)
{
	std::cout << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < a; i++)
	{
		std::cout << "Hello Word��" << "�������ò���" << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}
void func2(const char* name)
{
	std::cout << std::this_thread::get_id() << std::endl;
	std::cout << "Hello " << name << std::endl;
}
void foo1() {
	std::cout << "Hello C++11" << std::endl;
}
void foo2() {
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Hello C++11" << std::endl;
	}
	
}

void print(std::unique_ptr<int> ptr)
{
	std::cout << "���߳�\t" << ptr.get() << std::endl;
}

/*
���߳���Ҫ�Թ�����Դ���б�����
	1.ͨ�� mutex.lock() �� mutex.unlock() ���������������Ч�ʵ��Ҳ���ȫ
	2.ͨ�� std::lock_guard<std::mutex> �����Զ��ϡ������������������Զ�������
	3.ͨ��ԭ�ӱ���
*/
//int glob_variable = 0;			
std::atomic<int> glob_variable = 0;	//����ԭ�ӱ���
std::mutex mtx1;
std::mutex mtx2;
void task()
{
	for (int i = 0; i < 99; i++)
	{
		//std::lock_guard<std::mutex> lock1(mtx1);
		//std::lock_guard<std::mutex> lock2(mtx2);
		glob_variable++;//ԭ�ӱ���������Ҫ���ܿ�
		glob_variable--;
	}
	std::cout << glob_variable << std::endl;
}

//�߳���ͣ
void pausable() 
{
	// sleep 500����
	std::this_thread::sleep_for(std::chrono:: milliseconds(500));
	// sleep ��ָ��ʱ���
	std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(500));
}



//1.Man();//MM ��������+() �ȼ��� operator()()
//2.Lambada���ʽ,���غ���ָ�� [](){};
//3.std::ref ���ڰ�װ ���ô���ֵ
//4.std::move 
int thread_test()
{
	/************���󴴽��߳�************/
	Greet greet;
	std::thread test1(greet);
	test1.join();
	std::thread test2((Greet()));//Man ��������+() �ȼ��� operator()()
	test2.join();
	/************���󴴽��߳�************/

	/************Lambada���ʽ�����߳�************/
	std::thread test3(
		[]()
		{
			std::cout << "Lambada���߳�����" << std::endl; 
		});
	test3.join();
	/************Lambada���ʽ�����߳�************/

	/************���������߳�************/
	int num = 10;
	std::thread test4(func1, std::ref(num));//func��ʹ�����ò�������Ҫref
	test4.join();
	std::thread test5(func2, "C++11");
	test5.join();
	std::thread test6(foo1);
	test6.join();
	/************���������߳�************/

	/************����ָ�봴���߳�************/
	//int* p = new int(1000);
	std::unique_ptr<int> ptr(new int(1000));
	std::thread test7(print,move(ptr));
	test7.join();
	/************����ָ�봴���߳�************/

	/************�������Ա���������߳�************/
	int count = 1007;
	std::thread test8(&Greet::func, greet, std::ref(count));
	test8.join();
	std::thread test9(&Greet::SayHello, &greet, "C++11");
	test9.join();
	/************�������Ա���������߳�************/

	/************���ʹ�����Դ************/
	std::thread task1(task);
	task1.detach();
	std::thread task2(task);
	task2.detach();
	/************���ʹ�����Դ************/

	/************�߳���ͣ************/
	std::thread test10(pausable);
	test10.join();
	/************�߳���ͣ************/

	/************�̴߳���************/
	std::thread test11(foo2);
	std::thread test12(std::move(test11));
	test12.join();
	/*
	ʹ��std::move�������̶߳���test11������Ȩת�Ƶ���test12��ʵ�����̶߳���Ĵ��ݡ�
	һ���̶߳����ƶ���ԭʼ���̶߳���Ͳ�����ʹ�ã�����ᵼ��δ������Ϊ��
	*/
	/************�̴߳���************/

	std::cout << "���߳�����," << "ID:" << std::this_thread::get_id() << std::endl;

	return 0;
}
