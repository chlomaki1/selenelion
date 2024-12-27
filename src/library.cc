#include <fstream>
#include <windows.h>
#include <iostream>
#include <MinHook.h>
#include <thread>

#include "state/state.h"

bool running = true;
HINSTANCE hInstance;

DWORD WINAPI state_loop(void* hModule) {
    // TODO: more consistent way of figure out
    // how to delay this so that reading the internal state
    // doesn't crash the game
    std::this_thread::sleep_for(std::chrono::milliseconds(20000));
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    while (State::instance().isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (State::instance().getState() != State::instance().getInternalState()) {
            State::instance().setState(State::instance().getInternalState());

        	if (State::instance().getPreviousState() == CHARACTER_SELECT) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        	}

            State::instance().handleStateUpdate();
        }

        State::instance().update();
    }

    return 0;
}

inline void* (__thiscall* X2Main_ctor)(void* self);
void* __fastcall X2Main_ctor_H(void* self) {
    X2Main_ctor(self);

    State::instance().setRunning(true);
    CreateThread(0, 0x100, state_loop, hInstance, 0, 0);

    return self;
}

DWORD WINAPI dll_thread(void* hModule) {
    MH_Initialize();
    MH_CreateHook(
        reinterpret_cast<LPVOID>(0x1FA19B00),
        reinterpret_cast<LPVOID>(&X2Main_ctor_H),
        reinterpret_cast<LPVOID*>(&X2Main_ctor)
        );

    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        hInstance = hInst;
        CreateThread(0, 0x100, dll_thread, hInst, 0, 0);
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        State::instance().setRunning(false);
    	FreeConsole();
    }

    return true;
}