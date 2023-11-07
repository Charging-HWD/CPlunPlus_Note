/*
Ĭ������£�std::map ʹ�� std::less ��������м��ıȽϣ�
����ζ�����ᰴ�ռ��������������

�����Ҫ����������ʽ����������򣬿����ṩ�Զ���ıȽϺ�������
�ȽϺ�������Ӧ�ý��������������ֱ���Ҫ�Ƚϵļ������ͣ�����ֵΪ bool ����
�����һ������С�ڵڶ����������򷵻� true,���򷵻� false;
*/

#include <iostream>
#include <map>
#include <string>

// �Զ���ȽϺ������󣬰��ռ��ĳ��Ƚ�������
// �������ļ�ֵ���ȵ���map�м�ֵ�ĳ��ȣ��򲻻���Ӹü�ֵ
struct CompareByLength
{
    bool operator()(const std::string& str1, const std::string& str2) const
    {
        return str1.length() < str2.length();
    }
};

// �Զ���ȽϺ������󣬰��ռ��Ĵ�С��������
struct CompareBySize {
    bool operator()(const std::string& a, const std::string& b) const {
        return a < b;
    }
};

int main()
{
    // ����һ�� std::map ���󣬲�ʹ���Զ���ȽϺ��������������
    // �Ƚϳ��� CompareByLength,myMap �򲻻���� orange �� grape
    // �Ƚϴ�С CompareBySize,myMap ������ȫ����ֵ
    std::map<std::string, int, CompareByLength> myMap;
    myMap.insert(std::make_pair("apple", 1));
    myMap.insert(std::make_pair("banana", 2));
    myMap.insert(std::make_pair("orange", 3));
    myMap.insert(std::make_pair("grape", 4));
    myMap.insert(std::make_pair("strawberry", 5));

    for (auto& iter : myMap)
    {
        std::cout << iter.first << ":" << iter.second << std::endl;
    }

    system("pause");
    return 0;
}