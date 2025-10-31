
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
wsJson* wsJsonInitChild(const char* key);
wsJson* wsJsonInitString(const char* key, const char* val);
wsJson* wsJsonInitNumber(const char* key, double val);
wsJson* wsJsonInitBool(const char* key, bool val);
wsJson* wsJsonInitArray(const char* key);
wsJson* wsJsonInitNull(const char* key);

// Adds a new child to the json object
void wsJsonAddField(wsJson* parent, wsJson* child);

// Adds an element to a json array
void wsJsonAddElement(wsJson* array, wsJson* element);
int32_t wsJsonToString(wsJson* obj, char* out, size_t size);
wsJson* wsStringToJson(const char** string);
wsJson* wsJsonGet(wsJson* obj, const char* key);
const char* wsJsonGetString(wsJson* obj, const char* key);
double wsJsonGetNumber(wsJson* obj, const char* key);

// Goes recursive trough the json tree and frees everything
void wsJsonFree(wsJson* obj);

#ifndef WSJSON_NO_MACROS
    #define wsJsonAddString(parent, key, val) (wsJsonAddField(parent, wsJsonInitString(key, val)))
    #define wsJsonAddNumber(parent, key, val) (wsJsonAddField(parent, wsJsonInitNumber(key, val)))
    #define wsJsonAddBool(parent, key, val) (wsJsonAddField(parent, wsJsonInitBool(key, val)))
#endif // WSJSON_MACROS

#endif
