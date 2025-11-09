
#include "../include/wsJson/ws_log.h"

void _wsJsonDefaultErrorCallback(const wsJsonErrorInfo* error);

_Thread_local wsJsonErrorInfo _wsJsonLastError = {0};
_Thread_local wsJsonErrorCallbackPFN _wsJsonErrorCallback = _wsJsonDefaultErrorCallback;
_Thread_local int32_t _wsJsonLogLevel = WS_JSON_LOG_LEVEL_WARNING;

const char* wsJsonErrorCodeToString(wsJsonResult errorCode) {
    switch(errorCode) {
        case WS_JSON_UNDEFINIED:            return "Undefinied";
        case WS_JSON_OK:                    return "Success";
        case WS_JSON_ERROR:                 return "General Error";
        case WS_JSON_MALLOC:                return "Malloc Error";
        case WS_JSON_NULL_PARAMETERS:       return "Null Function Parameters";
        default:                            return "Unkown Error";
    };
}

const char* _wsJsonErrorLogLevelToString(wsJsonLogLevel level) {
    switch (level) {
        case WS_JSON_LOG_LEVEL_API_DUMP:      return "WS_JSON_API_DUMP";
        case WS_JSON_LOG_LEVEL_INFO:          return "WS_JSON_INFO";
        case WS_JSON_LOG_LEVEL_DEBUG:         return "WS_JSON_DEBUG";
        case WS_JSON_LOG_LEVEL_WARNING:       return "WS_JSON_WARNING";
        case WS_JSON_LOG_LEVEL_ERROR:         return "WS_JSON_ERROR";
        default:                        return "WS_JSON_UNKNOWN_LOG_LEVEL";
    };
}

void _wsJsonDefaultErrorCallback(const wsJsonErrorInfo *error) {
    fprintf(stderr, "Error in %s:%d (%s): %s - %s\n",
        error->fileName,
        error->lineNumber,
        error->functionName,
        wsJsonErrorCodeToString(error->errorCode),
        error->message
    );
}

wsJsonErrorInfo* wsJsonGetLastError() {
    return &_wsJsonLastError;
}

void wsJsonClearLastError() {
    memset(&_wsJsonLastError, 0, sizeof(wsJsonErrorInfo));
    _wsJsonLastError.errorCode = WS_JSON_UNDEFINIED;
}

void wsJsonSetLogLevel(int32_t level) {
    _wsJsonLogLevel = level;
}

