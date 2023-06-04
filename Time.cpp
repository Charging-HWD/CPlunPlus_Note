#include <iostream>
#include <time.h>
#include <chrono>
#include <iomanip>//io代表输入输出，manip是manipulator（操纵器）的缩写
#include <thread>
void printfTime_C()
{
	time_t cur = time(NULL);
	printf("%lu\n", cur);

	printf("--------loclatime--------\n");
	struct tm* t = localtime(&cur);
	printf("%d 年 %d 月 %d 日 %d 时 %d 分 %d 秒\n", t->tm_year + 1900,
		t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	printf("--------asctime--------\n");
	printf("%s\n", asctime(t));

	printf("--------ctime--------\n");
	printf("%s\n", ctime(&cur));
}

void printfTime_CPlusPlus()
{
	std::cout << "时间差" << std::endl;
	std::chrono::seconds sec_3{ 3 };//3秒钟
	std::chrono::milliseconds mills_5{ 500 };//500毫秒
	auto duration = sec_3 - mills_5;//两者差值，单位默认转到更小的 2500ms
	std::cout << "3秒钟 - 500毫秒 = " << duration.count() << std::endl; //值为2500

	std::cout << "时间单位转换" << std::endl;//chrono::duration_cast<>()
	std::chrono::minutes mintu_2{ 2 };//2分钟
	std::cout << "2分钟等价于 " << std::chrono::duration_cast<std::chrono::seconds>(mintu_2).count() << " 秒钟" << std::endl;//2分钟换算为120秒

	std::cout << "获取当前时间" << std::endl;//当前时间 time_point 格式  
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();//stl库获取当前时间
	std::time_t newTime = std::chrono::system_clock::to_time_t(now);//转换为旧式接口，单位:秒
	std::cout << "newTime = " << newTime << std::endl;
	std::time_t oldTime = time(nullptr);//c函数获取当前时间
	std::cout << "oldTime = " << oldTime << std::endl;// oldTime == timeT

	std::cout << "\n格式化时间" << std::endl;
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

	std::cout << "\n线程休眠" << std::endl;//需要添加<thread>
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //当前线程休眠100毫秒

	std::cout << "\n定时" << std::endl;
	auto begin = std::chrono::steady_clock::now();
	double int_ms_1 = 0;//计数器
	int timeout = 5000;//目标时间
	do {
		auto end = std::chrono::steady_clock::now();
		int_ms_1 = std::chrono::duration<double, std::milli>(end - begin).count();//计数
		auto int_ms_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);//转化
		auto int_us = std::chrono::duration<double, std::micro>(end - begin).count();
		std::cout << int_ms_1 << " :ms, "
			<< " or " << int_ms_2.count() << " ms, "
			<< int_us << ":us"
			<< std::endl;
	} while (timeout > int_ms_1);
}

/*
chrono库主要包含了三种类型：时间间隔Duration、时钟Clocks、时间点Time point
*/

/*
duration表示一段时间间隔，用来记录时间长度，可以表示几秒钟、几分钟或者几个小时的时间间隔，
duration的原型是：
template<class Rep, class Period = std::ratio<1>> class duration;
第一个模板参数Rep是一个数值类型，表示时钟个数；第二个模板参数是一个默认模板参数std::ratio，
它的原型是：template<std::intmax_t Num, std::intmax_t Denom = 1> class ratio;
ratio表示每个时钟周期的秒数，其中第一个模板参数Num代表分子，Denom代表分母，分母默认为1，ratio代表的是一个分子除以分母的分数值
如ratio<2>代表一个时钟周期是两秒，ratio<60>代表了一分钟，ratio<60*60>代表一个小时，ratio<60*60*24>代表一天。
而ratio<1, 1000>代表的则是1/1000秒即一毫秒，ratio<1, 1000000>代表一微秒，ratio<1, 1000000000>代表一纳秒。

chrono命名空间定义好的时间单位：
typedef duration <Rep, ratio<3600, 1>> hours;
typedef duration <Rep, ratio<60, 1>> minutes;
typedef duration <Rep, ratio<1, 1>> seconds;
typedef duration <Rep, ratio<1, 1000>> milliseconds;
typedef duration <Rep, ratio<1, 1000000>> microseconds;

*/

/*
获取时间间隔的时钟周期个数的方法count()
chrono::duration_cast<seconds>() 时间单位转换,转换成秒
chrono::duration_cast<milliseconds>() 时间单位转换,转换成毫秒
*/


/*
Clocks表示当前的系统时钟，内部有time_point, duration, Rep, Period等信息，
它主要用来获取当前时间，以及实现time_t和time_point的相互转换。
Clocks包含三种时钟：
system_clock：从系统获取的时钟；
steady_clock：不能被修改的时钟；(steady_clock可以获取稳定可靠的时间间隔，后一次调用now()的值和前一次的差值是不因为修改了系统时间而改变，它保证了稳定的时间间隔。它的用法和system用法一样。)
high_resolution_clock：高精度时钟，实际上是system_clock或者steady_clock的别名。
*/


/*  chrono::system_clock::time_point与std::time_t类型可相互函数
 *  chrono::system_clock::to_time_t()
 *  chrono::system_clock::from_time_t()
 */


 /*
 time_point表示一个时间点，用来获取1970.1.1以来的秒数和当前的时间, 可以做一些时间的比较和算术运算，和ctime库结合起来显示时间。
 time_point必须要clock来计时，time_point有一个函数time_since_epoch()用来获得1970年1月1日到time_point时间经过的duration。
 */