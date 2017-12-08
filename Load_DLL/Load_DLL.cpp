#include <stdint.h>
#include<Windows.h>
typedef void(*Dllfun)(uint8_t* ret, uint8_t len0, uint8_t len1, uint8_t len2, uint8_t * playCard0, uint8_t *playCard1, uint8_t *playCard2, uint8_t lastPlayCardLen, uint8_t* lastPlayCards, uint8_t lastPlayIdentity, uint8_t curPlayIdentity);
using namespace std;
int main()
{
	Dllfun dllFunc;
	HINSTANCE hdll;
	//必须要使用相对路径？？？
	hdll = LoadLibraryEx(L"DDZ_AI.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hdll == NULL)
	{
		FreeLibrary(hdll);
	}
	dllFunc = (Dllfun)GetProcAddress(hdll, "RobotPlay");
	if (dllFunc == NULL)
	{
		FreeLibrary(hdll);
	}
	uint8_t ret[16];
	uint8_t c0[]{ 0x34, 0x44, 0x36, 0x17, 0x38, 0x49, 0x2a, 0x3c, 0x1d, 0x2d, 0x4d, 0x1e, 0x2e, 0x4e, 0x3f, 0x1 };
	uint8_t c1[]{ 0x14,0x26,0x46,0x18,0x28,0x48,0x3a,0x1b,0x2b,0x3b,0x3e,0x1f,0x2 };
	uint8_t c2[]{ 0x33,0x45,0x39,0x1a,0x4a,0x4b,0x1c,0x3d };
	uint8_t lastPlayCard[]{ 0x13,0x24,0x35,0x46 ,0x17 };
	dllFunc(ret, 16, 13, 8, c0, c1, c2, 5, lastPlayCard, 2, 0);

	FreeLibrary(hdll);

}
