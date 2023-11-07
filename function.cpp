/*
������������֮һ�Ϳɳ�Ϊ�ɵ��ö���
1.��һ������ָ��
2.��һ������ operator() ��Ա����������󣨷º�������lambada ���ʽ
3.��һ���ɱ�ת��Ϊ����ָ��������
4.��һ�����Ա��������ָ��
5.bind ���ʽ��������������

std::function �����ֿɵ��ö���ķ�װ�������԰� std::function ����һ�������������ڱ�ʾ�������������
std::function ��ʵ�����Դ洢�����ƺ͵����κοɵ��ö��󣬴洢�Ŀɵ��ö����Ϊ std::function ��Ŀ�ꡣ
�� std::function ����Ŀ�꣬�����Ϊ�գ����ÿյ� std::function ��Ŀ����׳��쳣 std::bad_function_call
*/

/*
���� std::function ������ʵĲ�����ͷ���ֵ�����ͱ���˿�������������һ����÷�ʽ�ĺ�����װ����
std::function �����������ص������������ C++�������Ҫʹ�ûص��Ǿ�һ��Ҫʹ�� std::function
*/

#include <iostream>
#include <functional>

//function �Ķ��� f �Ĳ����� int �������� void
std::function<void(int)> f;

struct Foo 
{
	Foo(int num) :num_(num) {}
	void printf_add(int i)const { std::cout << num_ + i << std::endl; }
	int num_;
};

void printf_num(int i)
{
	std::cout << i << std::endl;
}

struct PrintfNum
{
	void operator()(int i)const { std::cout << i << std::endl; }
};

int main()
{
	//�洢���ɺ���(������ int �������� void)
	std::function<void(int)> f_display_1 = printf_num;
	f_display_1(10);

	//�洢 lambada ����()
	std::function<void()>f_display_2 = []() { printf_num(20); };
	f_display_2();

	//�洢�� std::bind ���õĽ��(����������ΰ󶨳�һ������)
	std::function<void()> f_display_3 = std::bind(printf_num, 30);
	f_display_3();

	{
		//�洢����Ա�����ĵ���
		//f_add_display_1 ��һ�� function ������������һ�����Ա����ָ�� &Foo::printf_add
		std::function<void(const Foo&, int)>f_add_display = &Foo::printf_add;

		//����һ�� Foo ��Ķ��� foo
		const Foo foo(10);

		// ���ó�Ա����ʱ����Ҫ���������Ϊ��һ������
		f_add_display(foo, 10);

		//�洢�����ݳ�Ա�������ĵ���
		std::function<int(Foo const&)> f_num = &Foo::num_;
		std::cout << "num_:" << f_num(foo) << std::endl;
	}

	//�洢����Ա����������ĵ���
	using std::placeholders::_1;
	std::function<void(int)>f_add_display_1 = std::bind(&Foo::printf_add, foo, _1);
	f_add_display_1(10);

	//�洢����Ա�����Ͷ���ָ��ĵ���
	std::function<void(int)>f_add_display_2 = std::bind(&Foo::printf_add, &foo, _1);
	f_add_display_2(20);

	//�洢����������ĵ���
	std::function<void(int)>f_display_obj = PrintfNum();
	f_display_obj(30);
}