#include <stdio.h>
#include <string.h>
#include <windows.h>		//LocalAlloc, LPTR

#define EXPORTDLL extern "C" __declspec(dllexport)

EXPORTDLL char* getMessage();
EXPORTDLL void copyMessage(char * _input, char * _output);
extern "C" void printMessage(char* _input);

//dll ������ ��� �ڵ�� extern "C" �ȿ� ���� �Ǿ�� �Ѵ�.
extern "C"
{
	//C#���� �ҷ��� �� �� ����.
	//dll ���ο����� ȣ���� �����ϴ�.
	char * result = "C++ Dll�κ��� ����";

	void printMessage(char * _input)
	{
		printf("'%s' is came from C# \n", _input);
	}
}

EXPORTDLL char* getMessage()
{
	char * returnchar = (char*)LocalAlloc(LPTR, strlen(result) + 1);

	strcat(returnchar, result);
	return returnchar;
}

EXPORTDLL void copyMessage(char * _input, char * _output)
{
	printMessage(_input);
	strcpy(_output, _input);
}