#include <windows.h>
#include <iostream>
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>

int main(int argc, char* argv[]) {
	Sleep(100);
	HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"semaphore");
	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"såm");
	HANDLE Finish = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)argv[0]),
		C = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"C"),
		D = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"D"),
		MESSAGE = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Messages");
	while (0 != WaitForSingleObject(MESSAGE, 100)) {
		while (true) {
			if (0 == WaitForSingleObject(C, 100)) {
				std::cout << "CHILD C" << std::endl;
				break;
			}
			if (0 == WaitForSingleObject(D, 100)) {
				std::cout << "CHILD D" << std::endl;
				break;
			}
		}
	}
	SetEvent(Finish);
	ReleaseSemaphore(semaphore, 1, NULL);
}