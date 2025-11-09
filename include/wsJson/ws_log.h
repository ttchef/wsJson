
#ifndef WS_JSON_LOG_H
#define WS_JSON_LOG_H

#include "ws_config.h"
#include "ws_globals.h"

#include <threads.h>

typedef enum wsJsonResult {
    WS_JSON_UNDEFINIED = 1,
    WS_JSON_OK = 0,
    WS_JSON_ERROR = -1,
    WS_JSON_MALLOC = -2,
    WS_JSON_NULL_PARAMETERS = -3,
} wsJsonResult;

typedef enum wsJsonLogLevel {
    WS_JSON_LOG_API_DUMP = WS_JSON_LOG_LEVEL_API_DUMP,
    WS_JSON_LOG_INFO = WS_JSON_LOG_LEVEL_INFO,
    WS_JSON_LOG_DEBUG = WS_JSON_LOG_LEVEL_DEBUG,
    WS_JSON_LOG_WARNING = WS_JSON_LOG_LEVEL_WARNING,
    WS_JSON_LOG_ERROR = WS_JSON_LOG_LEVEL_ERROR,
} wsJsonLogLevel;

typedef struct wsJsonErrorInfo {
    wsJsonResult errorCode;
    const char* functionName;
    const char* fileName;
    uint32_t lineNumber;
    char message[256];
} wsJsonErrorInfo;

typedef void (*wsJsonErrorCallbackPFN)(const wsJsonErrorInfo* error);

extern _Thread_local wsJsonErrorInfo _wsJsonLastError;
extern _Thread_local wsJsonErrorCallbackPFN _wsJsonErrorCallback;

void wsJsonSetErrorCallback(wsJsonErrorCallbackPFN callback);
wsJsonErrorInfo* wsJsonGetLastError();
void wsJsonClearLastError();
const char* _wsJsonErrorLogLevelToString(wsJsonLogLevel level);

#define WS_JSON_SET_ERROR(code, msg, ...) \
    do { \
        _wsJsonLastError.errorCode = code; \
        _wsJsonLastError.functionName = __func__; \
        _wsJsonLastError.fileName = __FILE__; \
        _wsJsonLastError.lineNumber = __LINE__; \
        snprintf(_wsJsonLastError.message, sizeof(_wsJsonLastError.message), msg, ##__VA_ARGS__); \
        if (_wsJsonErrorCallback) _wsJsonErrorCallback(&_wsJsonLastError); \
    } while (0)

#define WS_JSON_LOG_IMPL(level, file, func, line, msg, ...) \
    do { \
        char formatedMsg[256]; \
        snprintf(formatedMsg, sizeof(formatedMsg), msg, ##__VA_ARGS__); \
        fprintf(stderr, "[%s] %s:%d (%s): %s\n", _wsJsonErrorLogLevelToString(level), file, line, func, formatedMsg); \
    } while (0)

#if WS_JSON_LOG_LEVEL <= WS_JSON_LOG_LEVEL_API_DUMP
    #define WS_JSON_LOG_API_DUMP(msg, ...) \
        WS_JSON_LOG_IMPL(WS_JSON_LOG_API_DUMP, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__)
#else
    #define WS_JSON_LOG_API_DUMP(...) ((void)0)
#endif

#if WS_JSON_LOG_LEVEL <= WS_JSON_LOG_LEVEL_INFO
    #define WS_JSON_LOG_INFO(msg, ...) \
        WS_JSON_LOG_IMPL(WS_JSON_LOG_INFO, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__)
#else
    #define WS_JSON_LOG_INFO(...) ((void)0)
#endif

#if WS_JSON_LOG_LEVEL <= WS_JSON_LOG_LEVEL_DEBUG
    #define WS_JSON_LOG_DEBUG(msg, ...) \
        WS_JSON_LOG_IMPL(WS_JSON_LOG_DEBUG, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__)
#else
    #define WS_JSON_LOG_DEBUG(...) ((void)0)
#endif

#if WS_JSON_LOG_LEVEL <= WS_JSON_LOG_LEVEL_WARNING
    #define WS_JSON_LOG_WARNING(msg, ...) \
            WS_JSON_LOG_IMPL(WS_JSON_LOG_WARNING, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__)
#else
    #define WS_JSON_LOG_WARNING(...) ((void)0)
#endif

#define WS_JSON_LOG_ERROR(msg, ...) \
    WS_JSON_LOG_IMPL(WS_JSON_LOG_ERROR, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__)

#endif // ws_log.h
