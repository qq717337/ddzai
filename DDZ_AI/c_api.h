#pragma once

#ifdef __cplusplus
#define APP_EXTERN_C extern "C"
#else
#define APP_EXTERN_C
#endif


#ifdef _MSC_VER
#define APP_EXPORT __declspec(dllexport)
#define APP_C_EXPORT APP_EXTERN_C __declspec(dllexport)
#else
#define APP_EXPORT 
#define APP_C_EXPORT APP_EXTERN_C
#endif

static char* LastErrorMsg() { static  char err_msg[512] = "Everything is fine"; return err_msg; }

#pragma warning(disable : 4996)
inline void SetLastError(const char* msg) {
	std::strcpy(LastErrorMsg(), msg);
}

inline int APIHandleException(const std::exception& ex) {
	SetLastError(ex.what());
	return -1;
}
inline int APIHandleException(const std::string& ex) {
	SetLastError(ex.c_str());
	return -1;
}

#define API_BEGIN() try {

#define API_END() } \
catch(std::exception& ex) {APIHandleException(ex); return -1; } \
catch(...) {APIHandleException("unknown exception"); return -1; } \
return 0;

APP_C_EXPORT void DealSmoothCard(uint8_t * playCard0, uint8_t *playCard1, uint8_t *playCard2, uint8_t *deskCard);
APP_C_EXPORT uint8_t RobotPlay(uint8_t* ret, uint8_t len0, uint8_t len1, uint8_t len2, uint8_t * playCard0, uint8_t *playCard1, uint8_t *playCard2, uint8_t lastPlayCardLen, uint8_t* lastPlayCards, uint8_t lastPlayIdentity, uint8_t curPlayIdentity);
