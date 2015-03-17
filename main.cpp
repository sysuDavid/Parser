#include <iostream>
#include <cstring>
#include "Parser.h"
void printHelp();
int main(int argc, char* argv[])
{
	Parser parser;
	if (argc == 1)
	{
		while (true)
		{
			cout << ">>  ";
			parser.program();
		}
	}
	else if (argc == 2)
	{
		try {
			if (strcmp(argv[1],"-help") == 0)
			{
				printHelp();
			}
			else {
				parser.setPath_Tag(2, argv[1]);
				parser.program();
			}
		} catch (FileNotOpenException)
		{
			cout << "File can not open!" << endl;
			cout << "Is it a vaild file path?" << endl;
		}
	} 
	else
	{
		cout << "Unrecognized option: ";
		for (int i = 1; i < argc; i++)
		{
			cout << argv[i] << " ";
		}
		cout << endl;
		cout << "You can use \"-help\" option to see help menu: ssql -help" << endl;
	}
	return 0;
}
void printHelp()
{
	cout << "�÷�:  ssql" << endl;
	cout << "       ����ģʽ����ʹ��Ctrl + C �˳�����" << endl;
	cout << "  ��   ssql [file path]" << endl;
	cout << "       ��������¶����ļ�ĩβ���Զ��˳�" << endl;
	cout << "������ϸ������鿴readme" << endl;
}
