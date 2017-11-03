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
		const wchar_t* DirSpec = path.append(TEXT("\\*")).c_str();                 //����Ҫ�������ļ��е�Ŀ¼  
		DWORD dwError;

		hFind = FindFirstFile(DirSpec, &FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  

		if (hFind == INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
		{
			FindClose(hFind);
			return;
		}
		else
		{
			while (FindNextFile(hFind, &FindFileData) != 0)                            //���ļ������ļ��д���ʱ  
			{
				if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)        //�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
				{
					continue;
				}
				//if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0)      //�ж�������ļ���  
				//{
				//	wchar_t DirAdd[MAX_PATH];
				//	StringCchCopy(DirAdd, MAX_PATH, Dir);
				//	StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));
				//	StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //ƴ�ӵõ����ļ��е�����·��  
				//	TraverseDirectory(DirAdd);                                  //ʵ�ֵݹ����  
				//}
				if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)    //��������ļ���  
				{
					std::wstring fileName( FindFileData.cFileName);
					auto  fullPath = wstringToString(std::wstring(ahead).append(fileName));
					auto name= wstringToString(fileName.substr(0, fileName.find_first_of('.')));
					files.emplace(name, fullPath);            //�������·��  
				}
			}
			FindClose(hFind);
		}
	}
}