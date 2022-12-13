#include<Windows.h>
#include<stdio.h>


void printconsole(const char *str) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);	//获取std句柄
	WriteConsoleA(hStdout, str, strlen(str), NULL, NULL);
}

void NTAPI TLS_Callback1(PVOID DllHandle, DWORD reason, PVOID Reserved) {
	char str[80];
	wsprintfA(str, "TLS_Callback1():DllHandle=%x\tReason=%x\tReserved=%x\n", DllHandle, reason, Reserved);
	printconsole(str);
}

void NTAPI TLS_Callback2(PVOID DllHandle, DWORD reason, PVOID Reserved) {
	char str[80];
	wsprintfA(str, "TLS_Callback2():DllHandle:%x\tReason:%x\tReserved:%x\n", DllHandle, reason, Reserved);
	printconsole(str);
}

void NTAPI debugtest(PVOID DllHandle, DWORD reason, PVOID Reserved) {
	if(reason== DLL_THREAD_ATTACH)
	if (IsDebuggerPresent()) {
		MessageBox(NULL, "debugger_test :(", "TLS_Test", MB_OK);
	}
	else {
		MessageBox(NULL, "no debugger_test :)", "TLS_Test", MB_OK);
	}
}

#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:__tls_callback")

EXTERN_C
#pragma data_seg(".CRT$XLB")
//PIMAGE_TLS_CALLBACK pTLS_Callbacks[] = { TLS_Callback1,TLS_Callback2, debugtest,0 };
PIMAGE_TLS_CALLBACK _tls_callback[] = { debugtest,TLS_Callback1,TLS_Callback2,0 };	//运行成功
//PIMAGE_TLS_CALLBACK p_tls_callback[] = { TLS_Callback1,TLS_Callback2,0 };	//运行成失败
#pragma data_seg()


DWORD WINAPI ThreadProc(LPVOID lParam) {	//创建子线程函数

	printconsole("ThreadProc() Start!\n");
	printconsole("ThreadProc() End!\n");
	return 0;
}

int main() {
	HANDLE thread = NULL;
	
	printconsole("main() Start!\n");
	
	thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	WaitForSingleObject(thread,60*1000);
	CloseHandle(thread);
	
	printconsole("main() End!\n");
	//system("pause");
	return 0;

}
