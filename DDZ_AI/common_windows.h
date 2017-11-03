#pragma once
#include <string>
#include <map>
#include <Windows.h>

namespace Common {
	static std::string wstringToString(const std::wstring& wstr)
	{
		LPCWSTR pwszSrc = wstr.c_str();
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen == 0)
			return std::string("");

		char* pszDst = new char[nLen];
		if (!pszDst)
			return std::string("");

		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		std::string str(pszDst);
		delete[] pszDst;
		pszDst = NULL;

		return str;
	}
	inline static void GetFilesWindows(std::wstring path, std::map<std::string, std::string>& files)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		auto ahead = std::wstring(path);
		ahead.push_back('\\');
		const wchar_t* DirSpec = path.append(TEXT("\\*")).c_str();                 //定义要遍历的文件夹的目录  
		DWORD dwError;

		hFind = FindFirstFile(DirSpec, &FindFileData);          //找到文件夹中的第一个文件  

		if (hFind == INVALID_HANDLE_VALUE)                               //如果hFind句柄创建失败，输出错误信息  
		{
			FindClose(hFind);
			return;
		}
		else
		{
			while (FindNextFile(hFind, &FindFileData) != 0)                            //当文件或者文件夹存在时  
			{
				if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)        //判断是文件夹&&表示为"."||表示为"."  
				{
					continue;
				}
				//if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0)      //判断如果是文件夹  
				//{
				//	wchar_t DirAdd[MAX_PATH];
				//	StringCchCopy(DirAdd, MAX_PATH, Dir);
				//	StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));
				//	StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //拼接得到此文件夹的完整路径  
				//	TraverseDirectory(DirAdd);                                  //实现递归调用  
				//}
				if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)    //如果不是文件夹  
				{
					std::wstring fileName( FindFileData.cFileName);
					auto  fullPath = wstringToString(std::wstring(ahead).append(fileName));
					auto name= wstringToString(fileName.substr(0, fileName.find_first_of('.')));
					files.emplace(name, fullPath);            //输出完整路径  
				}
			}
			FindClose(hFind);
		}
	}
}