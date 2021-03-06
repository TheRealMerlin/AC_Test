// HackAC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
#include "proc.h"

void printInterface(bool bInfiniteAmmo, bool bInfiniteHealth) {
	// Trainer interface
	system("cls");
	std::cout << "--------------------\n" << "Merlin's Trainer\n" << "--------------------\n";
	std::cout << "[F1] Toggle Infinite Ammo  -  Status: " << std::boolalpha << bInfiniteAmmo << std::endl;
	std::cout << "[F2] Toggle Infinite Health  -  Status: " << std::boolalpha << bInfiniteHealth << std::endl;
}


int main() {
	int time = clock(); // Define time for the trainer refreshrate

	bool bInfiniteAmmo = false; // Define variable to store user's choice
	bool bInfiniteHealth = false;

	int defaultAmmo = 20; // Default values
	int defaultHealth = 100;

	int newValue = 9999; // Number to use for both health and ammo

	DWORD procId = GetProcId(L"ac_client.exe"); // Get ProcID of the target proccess
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe"); // Get module base address
	HANDLE hProcess = 0; // Get handle to proccess
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x50F4F4; // Resolve base address of the pointer chain

	std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0}; // Resolve the ammo pointer chain
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

	std::vector<unsigned int> healthOffsets = {0xf8}; // Resolve the health pointer chain
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);

	while(true) {
		if(clock() - time > 100) {
			time = clock();
			if(GetAsyncKeyState(VK_F1)) {
				bInfiniteAmmo = !bInfiniteAmmo; // Toggles infinite ammo when F1 is pressed
				if(bInfiniteAmmo == false) {
					WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &defaultAmmo, sizeof(defaultAmmo), nullptr); // Sets ammo value to default
				}
			}
			if(GetAsyncKeyState(VK_F2)) {
				bInfiniteHealth = !bInfiniteHealth; // Toggles infinite health when F2 is pressed
				if(bInfiniteHealth == false) {
					WriteProcessMemory(hProcess, (BYTE*)healthAddr, &defaultHealth, sizeof(defaultHealth), nullptr); // Sets health value to default
				}
			}
			if(bInfiniteAmmo) {
				WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newValue, sizeof(newValue), nullptr); // Sets new ammo values
			}
			if(bInfiniteHealth) {
				WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newValue, sizeof(newValue), nullptr); // Sets new health values
			}
			printInterface(bInfiniteAmmo, bInfiniteHealth);
		}
	}

	getchar(); // Ends the process
    return 0;
}

