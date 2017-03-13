#include <stdio.h>
#include <string.h>
#include <windows.h>		//LocalAlloc, LPTR

#define EXPORTDLL extern "C" __declspec(dllexport)

EXPORTDLL char* getMessage();
EXPORTDLL void copyMessage(char * _input, char * _output);
extern "C" void printMessage(char* _input);

//dll 내부의 모든 코드는 extern "C" 안에 정의 되어야 한다.
extern "C"
{
	//C#에서 불러다 쓸 수 없다.
	//dll 내부에서만 호출이 가능하다.
	char * result = "C++ Dll로부터 응답";

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