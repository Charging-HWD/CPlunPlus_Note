#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
int count = 0;
std::string Message = "";

// Windows下通过 API 函数获取文件路径
void GetPath()
{
    char szModulePath[_MAX_PATH];
	   char szDriveName[_MAX_DRIVE];
	   char szDirName[_MAX_PATH];
	   char szFileName[_MAX_PATH];
	   char szFileExt[_MAX_PATH];
	   GetModuleFileName(NULL, szModulePath, _MAX_PATH);
	   _splitpath(szModulePath, szDriveName, szDirName, szFileName, szFileExt);
	   cout << "当前完整路径：" << szModulePath << endl;
	   cout << "盘符：" << szDriveName << endl;
	   cout << "中级文件夹名：" << szDirName << endl;
	   cout << "文件名：" << szFileName << endl;
	   cout << "文件扩展名：" << szFileExt << endl;
}

//读取文件内容，C风格
int ReadFile(char* FileName, char** Buf, char* ResponseData)
{
    FILE* fp = fopen(FileName, "rb");
    if (!fp)
    {
        sprintf(ResponseData,"{\"data\":{},\"code\":\"1\".\"msg\":\"Missing\"}\r\n");
        printf("%s",ResponseData);
        return -1;
    }
    fseek(fp, 0, SEEK_END);//将指针移动到文件尾
    int FileLen = ftell(fp);//获取文件长度
    fseek(fp, 0, SEEK_SET);//将指针移动到文件头
 
    char* jsonstr = new char[FileLen];
    if (!jsonstr)
    {
        sprintf(ResponseData, "{\"data\":{},\"code\":\"1\",\"msg\":\"Memory Error\"}\r\n");
        printf("%s", ResponseData);
        fclose(fp);
        return -2;
    }
    memset(jsonstr, 0, FileLen);
    int rLen = fread(jsonstr, 1, FileLen, fp);
    if (rLen != FileLen)//rLen为读取到的长度,FileLen为文件长度
    {
        sprintf(ResponseData, "{\"data\":{},\"code\":\"1\",\"msg\":\"Load Json Error\"}\r\n");
        printf("%s", ResponseData);
        fclose(fp);
        delete[] jsonstr;
        return -3;
    }
    *Buf = jsonstr;
    fclose(fp);
    return 0;
}
 
//读取文件内容，C++风格
void ReadFile(std::string FileName, std::string& out_Data, size_t& out_Size)
{
    std::ifstream in_array_file;
    in_array_file.open(FileName.c_str(), std::ios::in || std::ios::binary);
    if (in_array_file.is_open())
    {
        unsigned char* buffer_bmp = nullptr;
        unsigned int size_tmp = 0;
 
        /***********获取文件大小***********/
        in_array_file.seekg(0, std::ios::end);
        size_tmp = in_array_file.tellg();
        in_array_file.seekg(0, std::ios::beg);
 
        /***********读取文件***********/
        buffer_bmp = new unsigned char[size_tmp+1];
        if (buffer_bmp != nullptr)
        {
            memset(buffer_bmp, 0, size_tmp + 1);
            in_array_file.read((char*)buffer_bmp, size_tmp);
        }
        in_array_file.close();
 
        /***********拷贝文件内容***********/
        out_Data = (char*)buffer_bmp;
        out_Size = out_Data.length();
         
        if (buffer_bmp != nullptr)
        {
            delete[] buffer_bmp;
        }
    }
}
 
//在指定位置修改文件内容
void ReviseFile(std::string FileName, std::string Data,int Place)
{
    //ofstream在打开文件时默认清空文件所有内容。如果使用ios::app来打开文件，虽然不会清空文件内容，但是每次写操作都追加到文件末尾，即使使用seekp也没用。
    //fstream fs("D:/test.txt", ios::binary | ios::out | ios::app);//使用ios::app来打开文件
    //fs.seekp(2, ios::beg);//跳转到开头的第二个字节位置进行写入。
    //fs.write("!!!", 3);//写入三个!!!，最后发现还是写在结尾，即使seekp也没用
    //fs.close();
 
    //将文件打开模式ios::app替换为ios::in，这样可以保证文件内容不会被清空，且文件指针偏移操作有效。
    //fstream fs("D:/test.txt", ios::binary | ios::out | ios::in);//使用ios::in来打开文件
    //fs.seekp(2, ios::beg);//跳转到开头的第二个字节位置进行写入
    //fs.write("!!!", 3);//写入三个!!!，正常写入
    //fs.close();
 
    //中间的位置必须使用feekp然后再写才能成功修改。并且seekp之后，如果继续读一些内容，再写也写不进去。必须seekp之后就写，才写的进去。
    std::fstream fs(FileName, std::ios::binary | std::ios::out | std::ios::in);
    fs.seekp(Place, std::ios::beg);//跳转到开头第Palce个字节
    fs.write(Data.c_str(), Data.length());
    fs.close();
}
 
//修改文件大小
void ReviseFile(std::string FileName, int Size)
{
    //需要c++17标准库
    //在100字节处进行截断，即只有100字节大小,多余100则多余部分舍弃，小于100则补0
    //filesystem::resize_file("F:\\test.txt", 100);
    std::filesystem::resize_file(FileName, Size);
}
 
//判断文件是否存在
bool IsFileExists(std::string FileName)
{   //C++17标准
    if (std::filesystem::exists(FileName))
    {
        if (std::filesystem::is_directory(FileName))
        {
            printf("%s is a directory\n", FileName.c_str());
        }
        else if (std::filesystem::is_regular_file(FileName)) {
            printf("%s is a file\n", FileName.c_str());
        }
        else
        {
            printf("%s exist\n", FileName.c_str());
        }
        return true;
    }
    else
    {
        printf("%s no exist\n", FileName.c_str());
        return false;
    }
}
void main()
{
    /***********从文件中读取数据***********/
    std::ifstream fin("D:/test.txt");  
    fin >> count;
    fin.close();
    std::cout << count << std::endl;
 
    count++;   
    /***********写入数据到文件中***********/
    std::ofstream fout("D:/test.txt");
    fout << count << "," << ++count <<"\n +";
    fout.close();
 
    /***********删除文件***********/
    remove("D:/test.txt");             
 
    std::ifstream _fin("D:/data.txt");
    int a ,b; char c;
    /***********读取数据***********/
    _fin >> a >> c >> b;
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;//c为，
    _fin.close();
 
    a++; b++;
    std::cout << "change a=" << a << std::endl;
    std::cout << "change b=" << b << std::endl;
 
    //std::ofstream _fout("d:/data.txt");//清空文件重新写入
    std::ofstream _fout("d:/data.txt",std::ios::out|std::ios::app);;//在文件末尾追加内容
    _fout << a << "," << b << "\n";
    _fout << "test" << "," << "\n";
    _fout << "test" << "," << "\n";
    _fout << "test" << "," << "\n";
    _fout.close();
 
    /***********判断文件是否存在***********/
    IsFileExists("D:/data.txt");
 
    /***********读取文件内容与大小***********/
    std::string data = "";
    size_t length = 0;
    ReadFile("D:/data.txt",data,length);
    std::cout << "file_data:" << data.c_str() << std::endl;
    std::cout << "file_length:" << length << std::endl;<br><br>　　　　CheckAndCreateFile("E:/TEST/test.txt");
}

#ifdef WIN32 //Windows
　　#include <direct.h>
　　#include <io.h>
#else // Linux
　　#include <sys/io.h>
　　#include <unistd.h>
　　#include <sys/stat.h>
　　#include <sys/types.h>
　　#include <dirent.h>
#endif

bool CheckAndCreateFile(std::string FileName)
{
　　FileName = FileName.substr(0, FileName.find_last_of('/'));
　　std::cout << "filename:" << FileName.c_str() << std::endl;
　　//判断该文件夹是否存在
　　if (access(FileName.c_str(), 0) == -1)
　　{
#ifdef WIN32
　　　　int flag = mkdir(FileName.c_str()); //Windows创建文件夹
#else
　　　　int flag = mkdir(FileName.c_str(), S_IRWXU); //Linux创建文件夹
#endif
　　}
}
