#include "StdInc.h"

class GameVersion108
{
private:
	DWORD m_baseOffset;

public:
	virtual const char* GetVersionNumber()
	{
		return "1.0.8";
	}

	virtual bool DetectVersion()
	{
		return *(DWORD*)(m_baseOffset + 0xC7EC77) == 0x000134EC;
	}

	virtual void ScriptThread() {}

	virtual bool DidGameRestart()
	{
		DWORD localPlayerIndex = *(DWORD*)(m_baseOffset + 0xF2E0B4);
		void** players = (void**)(m_baseOffset + 0x1100498);

		static DWORD lastPlayerIndex;
		static void* lastPlayer;

		if (localPlayerIndex != -1)
		{
			if (localPlayerIndex != lastPlayerIndex)
			{
				lastPlayerIndex = localPlayerIndex;
				lastPlayer = players[localPlayerIndex];

				return true;
			}

			if (lastPlayer != players[localPlayerIndex])
			{
				lastPlayer = players[localPlayerIndex];
				return true;
			}
		}

		return false;
	}

	virtual void* GetGlobalsPBuffer()
	{
		return (void*)(m_baseOffset + 0x1794FB8);
	}

	virtual void* GetLoadedScripts()
	{
		//return (void*)(m_baseOffset + 0x1794FCC);
		return (void*)(m_baseOffset + 0x1794FAC);
	}

	virtual void* GetThreadArray()
	{
		return (void*)(m_baseOffset + 0x190FDB8);
	}

	virtual void* GetOldFuncTable()
	{
		char* functionFromScriptHook = (char*)GetModuleHandle("scripthook.dll") + 0x55E0;

		printf("GetOldFuncTable1\n");
		injector::scoped_write<> patch;
		patch.write(functionFromScriptHook + 0x3C, 0x190FDD0, true);

		printf("GetOldFuncTable2\n");
		return injector::thiscall<void*(GameVersion108*)>::call(functionFromScriptHook, this);
	}

	virtual void* GetNewFuncTable()
	{
		return (void*)(m_baseOffset + 0x190FDD0);
	}

	virtual void* GtaThread_Run(void* a1, int a2)
	{
		return injector::thiscall<void*(void*, int)>::call(m_baseOffset + 0xB60730, a1, a2);
	}

	virtual void* GtaThread_Kill(void* a1)
	{
		return injector::thiscall<void*(void*)>::call(m_baseOffset + 0xB607B0, a1);
	}

	virtual uint32_t LowercaseHash(char* a1)
	{
		uint32_t hash = 0;
		size_t len = strlen(a1);

		for (size_t i = 0; i < len; i++)
		{
			hash += tolower(a1[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	virtual HWND GetGameWindowHandle()
	{
		return FindWindow("grcWindow", nullptr); // woah
	}

	virtual void* GetD3DDevice()
	{
		return (void*)(m_baseOffset + 0x10A9938 + 0x11AC);
	}

	virtual void* GetD3DParams()
	{
		return (void*)(m_baseOffset + 0x17455C0);
	}

};

extern "C" __declspec(dllexport) int fastmanwouldveneededteamviewerforthis()
{
	return 1337;
}

GameVersion108 gameVersion108;

void DoTheThing()
{
	char* scriptHook = (char*)GetModuleHandle("scripthook.dll");

	DWORD vtable108 = *(DWORD*)&gameVersion108;
	injector::WriteMemory<DWORD>(scriptHook + 0x513f, vtable108, true);
}

void CreateConsole()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

DWORD WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
#if _DEBUG
		CreateConsole();
#endif
		DoTheThing();
	}

	return 1;
}