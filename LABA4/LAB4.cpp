#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>

int main() {
	int child, parent, msgs;
	std::cout << "Input number (<= 4) of child proccesses: ";
	std::cin >> child;
	std::cout << "Input number of parent proccesses: ";
	std::cin >> parent;
	HANDLE
		A = CreateEvent(NULL, FALSE, FALSE, L"A"),
		B = CreateEvent(NULL, FALSE, FALSE, L"B"),
		C = CreateEvent(NULL, FALSE, FALSE, L"C"),
		D = CreateEvent(NULL, FALSE, FALSE, L"D"),
		MESSAGES = CreateEvent(NULL, FALSE, FALSE, L"Messages");
	if (child > 4)
		std::cout << "Wrong number";
	else {
		std::string commandLine = "0";
		HANDLE* events = new HANDLE[child + parent];
		int msgs;
		std::cout << "Input number of messages: ";
		std::cin >> msgs;
		HANDLE semaphore = CreateSemaphore(NULL, 5, 5, L"sem");
		events[0] = CreateEvent(NULL, FALSE, FALSE, L"0");
		std::wstring parent_cmd_str = L"Parent.exe " + std::to_wstring(msgs),
			child_cmd_str = L"Child.exe ";
		STARTUPINFO* st = new STARTUPINFO[child + parent];
		PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[child + parent];
		for (int i = 0; i < parent; i++) {
			commandLine = std::to_string(i);
			events[i] = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)const_cast<char*>(commandLine.c_str()));
			commandLine += " " + msgs;
			ZeroMemory(&st[i], sizeof(STARTUPINFO));
			st[i].cb = sizeof(STARTUPINFO);
			
			CreateProcess(L"Parent.exe", const_cast<wchar_t*>(parent_cmd_str.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &st[i], &pi[i]);
		}
		for (int i = parent; i < child + parent; i++) {
			commandLine = std::to_string(i);
			events[i] = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)const_cast<char*>(commandLine.c_str()));
			commandLine += " " + msgs;
			ZeroMemory(&st[i], sizeof(STARTUPINFO));
			st[i].cb = sizeof(STARTUPINFO);
			CreateProcess(L"Child.exe", const_cast<wchar_t*>(child_cmd_str.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &st[i], &pi[i]);
		}
		int count = msgs * parent;
		for (int i = 0; i < count; i++) {
			WaitForSingleObject(semaphore, INFINITE);
			while (true) {
				if (0 == WaitForSingleObject(A, 100)) {
					std::cout << "BOSS A -> C" << std::endl;
					SetEvent(C);
					break;
				}
				if (0 == WaitForSingleObject(B, 100)) {
					std::cout << "CHILD B -> D" << std::endl;
					SetEvent(D);
					break;
				}
			}
		}
		SetEvent(MESSAGES);
		WaitForMultipleObjects(parent + child, events, TRUE, INFINITE);
		delete[] events;
		delete[] st;
		delete[] pi;
		CloseHandle(A);
		CloseHandle(B);
		CloseHandle(C);
		CloseHandle(D);
		CloseHandle(MESSAGES);
	}
}