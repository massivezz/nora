#include <stdio.h>
#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>
#include "util.h"

bool Util_Shutdown(int nFlag)
// Shutdown or logoff the system.
// Returns false if the function could not get the rights to shutdown.
{
	HANDLE				hToken;
	TOKEN_PRIVILEGES	tkp;

	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;	// Don't have the rights

	// Get the LUID for the shutdown privilege.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process.
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	// Cannot test the return value of AdjustTokenPrivileges.
	if (GetLastError() != ERROR_SUCCESS)
		return false;	// Don't have the rights


	// ExitWindows
	if (ExitWindowsEx(nFlag, 0))
		return true;
	else
		return false;
}

BOOL Util_ShutdownHandler(HWND hwnd, DWORD lParam)
{
	// if the window is me, don't terminate!
	Util_WinKill(hwnd);

	// Continue the enumeration.
	return TRUE;
}

void Util_WinKill(HWND hWnd)
{
	DWORD_PTR dwResult;
	// Use WM_CLOSE vs. SC_CLOSE in this case, since the target window is slightly more likely to
	// respond to that:
	if (!SendMessageTimeout(hWnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG, 500, &dwResult)) // Wait up to 500ms.
	{
		// Use more force - Mwuahaha
		DWORD pid = GetWindowThreadProcessId(hWnd, NULL);
		HANDLE hProcess = pid ? OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid) : NULL;
		if (hProcess)
		{
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
}