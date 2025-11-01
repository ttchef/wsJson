
#ifndef WS_JSON_H
#define WS_JSON_H

#include "ws_globals.h"

#define WS_JSON_MAX_KEY_SIZE 64 
#define WS_JSON_MAX_VALUE_SIZE 256
#define WS_JSON_OBJECT_MAX_FIELDS 16

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    WS_JSON_STRING,
    WS_JSON_NUMBER,
    WS_JSON_OBJECT,
    WS_JSON_BOOL,
    WS_JSON_ARRAY,
    WS_JSON_NULL,
} wsJsonType;

typedef struct wsJson {
    char key[WS_JSON_MAX_KEY_SIZE];
    wsJsonType type;
    union {
        char stringValue[WS_JSON_MAX_VALUE_SIZE];
        double numberValue;
        bool boolValue;
        struct {
            struct wsJson* children[WS_JSON_OBJECT_MAX_FIELDS];
            int32_t childCount;
        } object;
        struct {
            struct wsJson* elements[WS_JSON_OBJECT_MAX_FIELDS];
            int32_t elementCount;
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
wsJson* wsStringToJson(const char** string);

// Get Values as Pointers so you can modify them
wsJson* wsJsonGet(wsJson* obj, const char* key);
const char* wsJsonGetString(wsJson* obj, const char* key);
double wsJsonGetNumber(wsJson* obj, const char* key);
bool wsJsonGetBool(wsJson* obj, const char* key);

// Setter functions
int32_t wsJsonSetString(wsJson* obj, const char* key, const char* val);
int32_t wsJsonSetNumber(wsJson* obj, const char* key, double val);
int32_t wsJsonSetBool(wsJson* obj, const char* key, bool val);

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

