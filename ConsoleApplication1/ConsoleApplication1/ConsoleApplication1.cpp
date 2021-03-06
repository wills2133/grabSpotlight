// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include <windows.h>
#include <fstream>
#include <direct.h>
#include <cstring>
using namespace std;

//void recordName(string path, string name) {
//	ofstream f1(path);//打开文件用于写，若文件不存在就创建它
//	if (!f1)return;//打开文件失败则结束运行
//	f1 << name << endl; //使用插入运算符写文件内容
//	f1.close(); //关闭文件
//}

void getFiles(string path, vector<string>& files, vector<int>& sizes)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, sizes);
			}
			else
			{
				files.push_back(fileinfo.name);
				sizes.push_back(fileinfo.size);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int main()
{
	char   file2Dir[MAX_PATH];
	_getcwd(file2Dir, MAX_PATH);
	string destDir(file2Dir);

	//record file name
	ifstream fin(destDir + "\\index.txt");//打开文件用于写，若文件不存在就创建它
	cout << destDir + "\\index.txt" << endl;
	if (!fin) {
		cout<<"faild to open "<< destDir + "\\index.txt" << endl;
		return 0;//打开文件失败则结束运行
	}
	// get dirs
	string preName, sourDir, destDir2;
	getline(fin, sourDir);
	cout << sourDir << endl;
	// get previous names and index
	vector <string> preNames;
	while (!fin.eof()) {
		getline(fin, preName);
		cout << preName << endl;
		preNames.push_back(preName);
	}
	fin.close();

	//char fileDir[] = "C:\\Users\\wills\\AppData\\Local\\Packages\\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\\LocalState\\Assets";
	//string sourceDir(fileDir);
	vector <string> files;
	vector <int> sizes;
	////获取该路径下的所有文件
	getFiles(sourDir.c_str(), files, sizes);
	int size = files.size();
	int idx = 0;
	preNames.pop_back();
	idx = stoi(preNames.back());
	preNames.pop_back();
	
	// write new index
	ofstream fout(destDir + "\\index.txt");
	fout << sourDir << endl;
	for (auto &it: preNames) {
		fout << it << endl;
	}
	for (int i = 0; i < size; i++)
	{	
		if (sizes[i]>200000) {
			vector<string>::iterator iter;
			iter = find(preNames.begin(), preNames.end(), files[i]);
			if (iter != preNames.end())
			{
				cout << files[i] << " exist" << endl;
			}
			else
			{
				cout << files[i] << " not exist" << endl;
				string sourcePath = sourDir + "\\" + files[i];
				string destPath = destDir + "\\" + to_string(idx++) + ".jpg";
				// convert to unicode
				WCHAR wcharSource[256];
				memset(wcharSource, 0, sizeof(wcharSource));
				MultiByteToWideChar(CP_ACP, 0, sourcePath.c_str(), strlen(sourcePath.c_str()) + 1, wcharSource, sizeof(wcharSource) / sizeof(wcharSource[0]));
				WCHAR wchardest[256];
				memset(wchardest, 0, sizeof(wchardest));
				MultiByteToWideChar(CP_ACP, 0, destPath.c_str(), strlen(sourcePath.c_str()) + 1, wchardest, sizeof(wchardest) / sizeof(wchardest[0]));

				fout << files[i] << endl; //使用插入运算符写文件内容
				//copy image file
				CopyFile(wcharSource, wchardest, TRUE);
				cout << sizes[i] << endl;
			}
		}
		
	}
	fout << to_string(idx) << endl;
	fout.close(); //关闭文件
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
