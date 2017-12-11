// filesearcher.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <io.h>
#include <iostream>
#include <fstream>
#include <codecvt>

void read_all_lines(const wchar_t *filename)
{
	std::wifstream wifs;
	std::wstring txtline;
	int c = 0;

	wifs.open(filename);
	if (!wifs.is_open())
	{
		std::wcerr << L"Unable to open file" << std::endl;
		return;
	}
	// We are going to read an UTF-8 file
	wifs.imbue(std::locale(wifs.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));
	while (std::getline(wifs, txtline))
		std::wcout << ++c << L'\t' << txtline << L'\n';
	std::wcout << std::endl;
}

void getFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int main()
{
	std::string currentPath = "D:\\mygit\\Javashop-B2C\\javashop\\b2c\\src\\main\\webapp\\admin";
	std::string searchStr("转换");
	std::vector<std::string> files;
	getFiles(currentPath, files);
	auto a = files.cbegin(); auto b = files.cend();
	while(a!=b)
	{
		std::string filePath= *a++;
		read_all_lines(TEXT(filePath));
		std::ifstream input(filePath);
		std::string content,line;
		int i = 1;
		while(std::getline(input,line))
		{
			if(line.find(searchStr) != std::string::npos)
			{
				std::cout << filePath<<":line:"<<i<< std::endl;
			}
			i++;
			content += line;
		}
		std::cout << content << std::endl;
//		content.assign()
	}
	getchar();
    return 0;
}

