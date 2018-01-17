// filesearcher.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <io.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>

std::wstring StringToWString(const std::string& str)
{
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}

//只拷贝低字节至string中
std::string WStringToString(const std::wstring& wstr)
{
	std::string str(wstr.length(), ' ');
	std::copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}

std::wstring readFile(const char* filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}


void getFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄  
	long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		}while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


int main()
{
	std::cout << "请输入搜索路径"<<std::endl;
	std::string	inPath;
	std::cin >> inPath;
	std::cout << "请输入搜索关键字" << std::endl;
	std::wstring key;
	std::wcin >>key ;

//	std::wstring findStr = L"你好";
//	std::string findPath = "D:\\mygit\\Javashop-B2C\\javashop\\b2c\\src\\main\\webapp\\admin";
	std::vector<std::string> pathVector;
	getFiles(inPath, pathVector);
	auto a = pathVector.cbegin();
	auto b = pathVector.cend();
	while (a != b)
	{
		std::string path = *a;
		std::wstring str = readFile(path.c_str());
		std::wcout.imbue(std::locale("chs"));
		std::wcout << str << std::endl;
		if(str.find(key)!=std::wstring::npos)
		{
			std::cout << "找到了"<< std::endl;
			std::cout << path;
		}
		a++;
	}
	std::cout << "请输入任意" << std::endl;

	std::wcin >> key;
	getchar();
	return 0;
}