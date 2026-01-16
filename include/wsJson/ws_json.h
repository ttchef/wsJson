
#ifndef WS_JSON_H
#define WS_JSON_H

#include "ws_log.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define WS_JSON_MAX_KEY_SIZE 64 
#define WS_JSON_MAX_VALUE_SIZE 256

#define WS_JSON_MALLOC(size) malloc(size)
#define WS_JSON_REALLOC(ptr, size) realloc(ptr, size)
#define WS_JSON_CALLOC(n, size) calloc(n, size)
#define WS_JSON_FREE(ptr) free(ptr)

typedef enum wsJsonType {
    WS_JSON_STRING,
    WS_JSON_NUMBER,
    WS_JSON_OBJECT,
    WS_JSON_BOOL,
    WS_JSON_ARRAY,
    WS_JSON_NULL
} wsJsonType;

typedef struct wsJson {
    char key[WS_JSON_MAX_KEY_SIZE];
    wsJsonType type;
    union {
        struct {
            char* stringValue;
            int32_t stringLength;
            int32_t stringCapacity;
        };
        double numberValue;
        bool boolValue;
        struct {
            struct wsJson** children;
            int32_t childCount;
            int32_t childCapacity;
        } object;
        struct {
            struct wsJson** elements;
            int32_t elementCount;
            int32_t elementCapacity;
        } array;
    };
} wsJson;

// Create functions
wsJson* wsJsonInitObject(const char* key);
wsJson* wsJsonInitString(const char* key, const char* val);
wsJson* wsJsonInitNumber(const char* key, double val);
wsJson* wsJsonInitBool(const char* key, bool val);
wsJson* wsJsonInitArray(const char* key);
wsJson* wsJsonInitNull(const char* key);

// Adds a new child to the json object
void wsJsonAddField(wsJson* parent, wsJson* child);

// Adds an element to a json array
void wsJsonAddElement(wsJson* array, wsJson* element);

// String conversions
int32_t wsJsonToString(wsJson* obj, char* out, size_t size);
int32_t wsJsonToStringPretty(wsJson* obj, char* out, size_t size);
wsJson* wsStringToJson(const char** string);

// Get Values 
wsJson* wsJsonGet(wsJson* obj, const char* key);
const char* wsJsonGetString(wsJson* obj, const char* key);
int32_t wsJsonGetStringEx(wsJson* obj, const char* key, char* out, size_t size);
double wsJsonGetNumber(wsJson* obj, const char* key);
bool wsJsonGetBool(wsJson* obj, const char* key);

// Array Getters
int32_t wsJsonGetArrayLen(wsJson* obj, const char* key);
wsJson* wsJsonGetArrayAt(wsJson* obj, const char* key, int32_t index);

// Setter Explicit Functions (if object is null it wont set)
int32_t wsJsonSetStringExplicit(wsJson* obj, const char* key, const char* val);
int32_t wsJsonSetNumberExplicit(wsJson* obj, const char* key, double val);
int32_t wsJsonSetBoolExplicit(wsJson* obj, const char* key, bool val);

// Setter functions (if object is null it will change it to the specified type)
int32_t wsJsonSetString(wsJson* obj, const char* key, const char* val);
int32_t wsJsonSetNumber(wsJson* obj, const char* key, double val);
int32_t wsJsonSetBool(wsJson* obj, const char* key, bool val);
int32_t wsJsonSetElement(wsJson* obj, const char* key, int32_t index, wsJson* element);

// Null conversions
int32_t wsJsonSetNullToObject(wsJson* obj, const char* key, wsJson* objects);
int32_t wsJsonSetNullToString(wsJson* obj, const char* key, const char* val);
int32_t wsJsonSetNullToNumber(wsJson* obj, const char* key, double val);
int32_t wsJsonSetNullToBool(wsJson* obj, const char* key, bool val);
int32_t wsJsonSetNullToArray(wsJson* obj, const char* key, wsJson* array);

// Goes recursive trough the json tree and frees everything
void wsJsonFree(wsJson* obj);

#ifndef WSJSON_NO_MACROS
    #define wsJsonAddString(parent, key, val) (wsJsonAddField(parent, wsJsonInitString(key, val)))
    #define wsJsonAddNumber(parent, key, val) (wsJsonAddField(parent, wsJsonInitNumber(key, val)))
    #define wsJsonAddBool(parent, key, val) (wsJsonAddField(parent, wsJsonInitBool(key, val)))
    #define wsJsonAddNull(parent, key) (wsJsonAddField(parent, wsJsonInitNull(key)))
#endif // WSJSON_MACROS

#endif

