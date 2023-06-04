#include <iostream>
#include <time.h>
#include <chrono>
#include <iomanip>//io�������������manip��manipulator��������������д
#include <thread>
void printfTime_C()
{
	time_t cur = time(NULL);
	printf("%lu\n", cur);

	printf("--------loclatime--------\n");
	struct tm* t = localtime(&cur);
	printf("%d �� %d �� %d �� %d ʱ %d �� %d ��\n", t->tm_year + 1900,
		t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	printf("--------asctime--------\n");
	printf("%s\n", asctime(t));

	printf("--------ctime--------\n");
	printf("%s\n", ctime(&cur));
}

void printfTime_CPlusPlus()
{
	std::cout << "ʱ���" << std::endl;
	std::chrono::seconds sec_3{ 3 };//3����
	std::chrono::milliseconds mills_5{ 500 };//500����
	auto duration = sec_3 - mills_5;//���߲�ֵ����λĬ��ת����С�� 2500ms
	std::cout << "3���� - 500���� = " << duration.count() << std::endl; //ֵΪ2500

	std::cout << "ʱ�䵥λת��" << std::endl;//chrono::duration_cast<>()
	std::chrono::minutes mintu_2{ 2 };//2����
	std::cout << "2���ӵȼ��� " << std::chrono::duration_cast<std::chrono::seconds>(mintu_2).count() << " ����" << std::endl;//2���ӻ���Ϊ120��

	std::cout << "��ȡ��ǰʱ��" << std::endl;//��ǰʱ�� time_point ��ʽ  
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();//stl���ȡ��ǰʱ��
	std::time_t newTime = std::chrono::system_clock::to_time_t(now);//ת��Ϊ��ʽ�ӿڣ���λ:��
	std::cout << "newTime = " << newTime << std::endl;
	std::time_t oldTime = time(nullptr);//c������ȡ��ǰʱ��
	std::cout << "oldTime = " << oldTime << std::endl;// oldTime == timeT

	std::cout << "\n��ʽ��ʱ��" << std::endl;
	auto formatTime = std::put_time(std::localtime(&newTime), "%Y-%m-%d %X");
	std::cout << "formatTime1 = " << formatTime << std::endl;
	struct tm* ptr;
	char str[24] = { 0 };
	strftime(str, sizeof(str), "%Y-%m-%d-%H%M%S", localtime(&oldTime));
	std::cout << "formatTime2 = " << str << std::endl;

	std::time_t last = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
	std::time_t next = std::chrono::system_clock::to_time_t(now + std::chrono::hours(24));
	std::cout << "One day ago, the time was " << std::put_time(std::localtime(&last), "%F %T") << std::endl;
	std::cout << "Next day, the time was " << std::put_time(std::localtime(&next), "%Y-%m-%d %X") << std::endl;

	std::cout << "\n�߳�����" << std::endl;//��Ҫ���<thread>
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //��ǰ�߳�����100����

	std::cout << "\n��ʱ" << std::endl;
	auto begin = std::chrono::steady_clock::now();
	double int_ms_1 = 0;//������
	int timeout = 5000;//Ŀ��ʱ��
	do {
		auto end = std::chrono::steady_clock::now();
		int_ms_1 = std::chrono::duration<double, std::milli>(end - begin).count();//����
		auto int_ms_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);//ת��
		auto int_us = std::chrono::duration<double, std::micro>(end - begin).count();
		std::cout << int_ms_1 << " :ms, "
			<< " or " << int_ms_2.count() << " ms, "
			<< int_us << ":us"
			<< std::endl;
	} while (timeout > int_ms_1);
}

/*
chrono����Ҫ�������������ͣ�ʱ����Duration��ʱ��Clocks��ʱ���Time point
*/

/*
duration��ʾһ��ʱ������������¼ʱ�䳤�ȣ����Ա�ʾ�����ӡ������ӻ��߼���Сʱ��ʱ������
duration��ԭ���ǣ�
template<class Rep, class Period = std::ratio<1>> class duration;
��һ��ģ�����Rep��һ����ֵ���ͣ���ʾʱ�Ӹ������ڶ���ģ�������һ��Ĭ��ģ�����std::ratio��
����ԭ���ǣ�template<std::intmax_t Num, std::intmax_t Denom = 1> class ratio;
ratio��ʾÿ��ʱ�����ڵ����������е�һ��ģ�����Num������ӣ�Denom�����ĸ����ĸĬ��Ϊ1��ratio�������һ�����ӳ��Է�ĸ�ķ���ֵ
��ratio<2>����һ��ʱ�����������룬ratio<60>������һ���ӣ�ratio<60*60>����һ��Сʱ��ratio<60*60*24>����һ�졣
��ratio<1, 1000>���������1/1000�뼴һ���룬ratio<1, 1000000>����һ΢�룬ratio<1, 1000000000>����һ���롣

chrono�����ռ䶨��õ�ʱ�䵥λ��
typedef duration <Rep, ratio<3600, 1>> hours;
typedef duration <Rep, ratio<60, 1>> minutes;
typedef duration <Rep, ratio<1, 1>> seconds;
typedef duration <Rep, ratio<1, 1000>> milliseconds;
typedef duration <Rep, ratio<1, 1000000>> microseconds;

*/

/*
��ȡʱ������ʱ�����ڸ����ķ���count()
chrono::duration_cast<seconds>() ʱ�䵥λת��,ת������
chrono::duration_cast<milliseconds>() ʱ�䵥λת��,ת���ɺ���
*/


/*
Clocks��ʾ��ǰ��ϵͳʱ�ӣ��ڲ���time_point, duration, Rep, Period����Ϣ��
����Ҫ������ȡ��ǰʱ�䣬�Լ�ʵ��time_t��time_point���໥ת����
Clocks��������ʱ�ӣ�
system_clock����ϵͳ��ȡ��ʱ�ӣ�
steady_clock�����ܱ��޸ĵ�ʱ�ӣ�(steady_clock���Ի�ȡ�ȶ��ɿ���ʱ��������һ�ε���now()��ֵ��ǰһ�εĲ�ֵ�ǲ���Ϊ�޸���ϵͳʱ����ı䣬����֤���ȶ���ʱ�����������÷���system�÷�һ����)
high_resolution_clock���߾���ʱ�ӣ�ʵ������system_clock����steady_clock�ı�����
*/


/*  chrono::system_clock::time_point��std::time_t���Ϳ��໥����
 *  chrono::system_clock::to_time_t()
 *  chrono::system_clock::from_time_t()
 */


 /*
 time_point��ʾһ��ʱ��㣬������ȡ1970.1.1�����������͵�ǰ��ʱ��, ������һЩʱ��ıȽϺ��������㣬��ctime����������ʾʱ�䡣
 time_point����Ҫclock����ʱ��time_point��һ������time_since_epoch()�������1970��1��1�յ�time_pointʱ�侭����duration��
 */