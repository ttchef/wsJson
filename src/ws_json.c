
#include "ws_json.h"
#include "ws_globals.h"

#include <ctype.h>
#include <string.h>

wsJson* wsJsonInitObject(const char* key) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json object: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_OBJECT;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    return obj;
}

wsJson* wsJsonInitString(const char* key, const char* val) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json object: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_STRING;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    if (val) strncpy(obj->stringValue, val, sizeof(obj->stringValue) - 1);
    return obj;
}

wsJson* wsJsonInitNumber(const char* key, double val) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json object: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_NUMBER;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    obj->numberValue = val;
    return obj;
}

wsJson* wsJsonInitBool(const char* key, bool val) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json object: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_BOOL;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    obj->boolValue = val;
    return obj;
}

wsJson* wsJsonInitArray(const char* key) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json array: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_ARRAY;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    return obj;
}

wsJson* wsJsonInitNull(const char* key) {
    wsJson* obj = malloc(sizeof(wsJson));
    if (!obj) {
        WS_LOG_ERROR("Failed to allocate memory for json null: %s\n", key);
        return NULL;
    }
    memset(obj, 0, sizeof(wsJson));
    obj->type = WS_JSON_NULL;
    if (key) strncpy(obj->key, key, sizeof(obj->key) - 1);
    return obj;
}

void wsJsonAddField(wsJson *parent, wsJson *child) {
    if (parent && parent->type == WS_JSON_OBJECT && parent->object.childCount < WS_JSON_OBJECT_MAX_FIELDS) {
        parent->object.children[parent->object.childCount++] = child;
    }
}

void wsJsonAddElement(wsJson *array, wsJson *element) {
    if (array && array->type == WS_JSON_ARRAY && array->array.elementCount < WS_JSON_OBJECT_MAX_FIELDS) {
        array->array.elements[array->array.elementCount++] = element;
    }
}

int32_t wsJsonToString(wsJson *obj, char *out, size_t size) {
    if (!obj) {
        WS_LOG_ERROR("Input json obj is NULL\n");
        return WS_ERROR;
    }
    if (!out) {
        WS_LOG_ERROR("Input buffer for output is NULL\n");
        return WS_ERROR;
    }
    
    size_t used = 0;

    switch (obj->type) {
        case WS_JSON_STRING:
            used += snprintf(out + used, size - used, "\"%s\"", obj->stringValue);
            break;
        case WS_JSON_NUMBER:
            used += snprintf(out + used, size - used, "%g", obj->numberValue);
            break;
        case WS_JSON_BOOL:
            used += snprintf(out + used, size - used, "%s", obj->boolValue ? "true" : "false");
            break;
        case WS_JSON_NULL:
            used += snprintf(out + used, size - used, "null");
            break;
        case WS_JSON_OBJECT:
            used += snprintf(out + used, size - used, "{");
            for (int32_t i = 0; i < obj->object.childCount; i++) {
                wsJson* child = obj->object.children[i];
                if (i > 0) {
                    used += snprintf(out + used, size - used, ",");
                }
                used += snprintf(out + used, size - used, "\"%s\": ", child->key);
                wsJsonToString(child, out + used, size - used);
                used = strlen(out);
            }
            used += snprintf(out + used, size - used, "}");
            break;
        case WS_JSON_ARRAY:
            used += snprintf(out + used, size - used, "[");
            for (int32_t i = 0; i < obj->array.elementCount; i++) {
                wsJson* element = obj->array.elements[i];
                if (i > 0) {
                    used += snprintf(out + used, size - used, ",");
                }
                wsJsonToString(element, out + used, size - used);
                used = strlen(out);
            }
            used += snprintf(out + used, size - used, "]");
            break;
        default:
            WS_LOG_ERROR("Failed to parse json into string\n");
            return WS_ERROR;
    }

    return used;
}

static char* skipWhitespaces(const char* string) {
    while (*string && isspace((unsigned char)*string)) string++;
    return (char*)string;
}

static char* parseString(const char** string) {
    (*string)++; // skip "
    const char* start = *string;

    // Find end 
    while (**string && **string != '"') (*string)++;
    size_t len = *string - start;

    char* out = malloc(len + 1);
    if (!out) {
        WS_LOG_ERROR("Failed to allocate for parsing json key\n");
        return NULL;
    }
    strncpy(out, start, len);
    out[len] = '\0';
    if (**string == '"') (*string)++; // skip closing " 
    return out;
}

static wsJson* parseValue(const char** string);

static wsJson* parseArray(const char** string) {
    wsJson* array = wsJsonInitArray(NULL);
    if (!array) {
        WS_LOG_ERROR("Failed to allocate json array\n");
        return NULL;
    }

    *string = skipWhitespaces(*string);
    if (**string != '[') {
        WS_LOG_ERROR("Failed to parse array: missing '['\n");
        wsJsonFree(array);
        return NULL;
    }
    (*string)++;

    while (**string) {
        *string = skipWhitespaces(*string);
        if (**string == ']') {
            (*string)++;
            break;
        }

        wsJson* element = parseValue(string);
        if (!element) {
            WS_LOG_ERROR("Failed to parse array element\n");
            wsJsonFree(array);
            return NULL;
        }

        wsJsonAddElement(array, element);

        *string = skipWhitespaces(*string);
        if (**string == ',') (*string)++;
    }

    return array;
}

static wsJson* parseValue(const char** string) {
    (*string) = skipWhitespaces(*string);

    // Is String 
    if (**string == '"') {
        char* val = parseString(string);
        if (!val) {
            WS_LOG_ERROR("Failed to parse json value when parsing string\n");
            return NULL;
        }
        wsJson* node = calloc(1, sizeof(wsJson));
        if (!node) {
            WS_LOG_ERROR("Failed to allocate json node when parsing string\n");
            free(val);
            return NULL;
        }
        node->type = WS_JSON_STRING;

        size_t valLen = strlen(val);
        if (valLen + 1 > WS_JSON_MAX_VALUE_SIZE) {
            WS_LOG_ERROR("Value string is too long\n");
            free(val);
            wsJsonFree(node);
            return NULL;
        }

        strncpy(node->stringValue, val, valLen);
        node->stringValue[valLen] = '\0';
        free(val);
        return node;
    }
    
    // Is Field/Object 
    else if (**string == '{') {
        return wsStringToJson(string);
    }

    // Is Digit 
    else if (isdigit(**string) || **string == '-') {
        char* endPtr;
        double num = strtod(*string, &endPtr);
        wsJson* node = calloc(1, sizeof(wsJson));
        if (!node) {
            WS_LOG_ERROR("Failed to allocate json node when parsing string\n");
            return NULL;
        }
        node->type = WS_JSON_NUMBER;
        node->numberValue = num;
        *string = endPtr;
        return node;
    }

    // Is Bool (true)
    else if (strncmp(*string, "true", 4) == 0) {
        wsJson* node = calloc(1, sizeof(wsJson));
        if (!node) {
            WS_LOG_ERROR("Failed to allocate json node when parsing string\n");
            return NULL;
        }
        node->type = WS_JSON_BOOL;
        node->boolValue = true;
        *string += 4;
        return node;
    }

    // Is Bool (false)
    else if (strncmp(*string, "false", 5) == 0) {
        wsJson* node = calloc(1, sizeof(wsJson));
        if (!node) {
            WS_LOG_ERROR("Failed to allocate json node when parsing string\n");
            return NULL;
        }
        node->type = WS_JSON_BOOL;
        node->boolValue = false;
        *string += 5;
        return node;
    }

    // Is Null
    else if (strncmp(*string, "null", 4) == 0) {
        wsJson* node = calloc(1, sizeof(wsJson));
        if (!node) {
            WS_LOG_ERROR("Failed to allocate json node when parsing null\n");
            return NULL;
        }
        node->type = WS_JSON_NULL;
        *string += 4;
        return node;
    }

    // Is Array
    else if (**string == '[') {
        return parseArray(string);
    }

    return NULL;
}

wsJson* wsStringToJson(const char** string) {
    if (!string) {
        WS_LOG_ERROR("Invalid input paramerter is NULL\n");
        return NULL;
    }

    wsJson* root = wsJsonInitObject(NULL);
    if (!root) {
        WS_LOG_ERROR("Failed to allocate json object\n");
        return NULL;
    }

    *string = skipWhitespaces(*string);
    if (**string != '{') {
        WS_LOG_ERROR("Failed to convert string to json\n");
        wsJsonFree(root);
        return NULL;
    }
    (*string)++;

    while (**string) {
        *string = skipWhitespaces(*string);
        if (**string == '}') {
            (*string)++;
            break;
        }

        // read key
        char* key = parseString(string);
        if (!key) {
            WS_LOG_ERROR("Failed to parse json key\n");
            wsJsonFree(root);
            break;
        }

        *string = skipWhitespaces(*string);
        if (**string != ':') {
            free(key);
            break;
        }
        (*string)++;

        // Read value
        *string = skipWhitespaces(*string);
        wsJson* val = parseValue(string);
        if (!val) {
            WS_LOG_ERROR("Failed to parse json value\n");
            free(key);
            wsJsonFree(root);
            break;
        }

        size_t keyLen = strlen(key);
        if (keyLen + 1 > WS_JSON_MAX_KEY_SIZE) {
            WS_LOG_ERROR("Json key Size is too long\n");
            free(key);
            wsJsonFree(val);
            wsJsonFree(root);
            break;
        }
        strncpy(val->key, key, keyLen);
        val->key[keyLen] = '\0';
        wsJsonAddField(root, val);

        *string = skipWhitespaces(*string);
        if (**string == ',') (*string)++;

        free(key);
    }

    return root;
}

wsJson* wsJsonGet(wsJson* obj, const char* key) {
    if (!obj || !key) {
        WS_LOG_ERROR("Invalid input is NULL\n");
        return NULL;
    } 
    if (obj->type != WS_JSON_OBJECT) {
        WS_LOG_ERROR("Obj is not from type WS_JSON_OBJECT\n");
        return NULL;
    }

    for (int32_t i = 0; i < obj->object.childCount; i++) {
        wsJson* child = obj->object.children[i];
        if (strcmp(child->key, key) == 0) {
            return child;
        }
    }

    return NULL;
}

char* wsJsonGetString(wsJson* obj, const char* key) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_STRING) {
        return child->stringValue;
    }
    return NULL;
}

double* wsJsonGetNumber(wsJson *obj, const char *key) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NUMBER) {
        return &child->numberValue;
    }
    return NULL;
}

bool* wsJsonGetBool(wsJson* obj, const char* key) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_BOOL) {
        return &child->boolValue;
    }
    return NULL;
}

int32_t wsJsonNullToObject(wsJson* obj, const char *key, wsJson *fields) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NULL) {
        child->type = WS_JSON_OBJECT;
        memcpy(&child->object, &fields->object, sizeof(child->object));
    }
    return WS_ERROR;
}

int32_t wsJsonNullToString(wsJson *obj, const char *key, const char *val) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NULL) {
        size_t length = strlen(val);
        if (length >= WS_JSON_MAX_VALUE_SIZE) return WS_ERROR;
        child->type = WS_JSON_STRING;
        strcpy(child->stringValue, val);
    }
    return WS_ERROR;
}

int32_t wsJsonNullToNumber(wsJson *obj, const char *key, double val) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NULL) {
        child->type = WS_JSON_NUMBER;
        child->numberValue = val;
    }
    return WS_ERROR;
}

int32_t wsJsonNullToBool(wsJson *obj, const char *key, bool val) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NULL) {
        child->type = WS_JSON_BOOL;
        child->boolValue = val;
    }
    return WS_ERROR;
}

int32_t wsJsonNullToArray(wsJson *obj, const char *key, wsJson *array) {
    wsJson* child = wsJsonGet(obj, key);
    if (child && child->type == WS_JSON_NULL) {
        child->type = WS_JSON_ARRAY;
        memcpy(&child->array, &array->array, sizeof(child->array));
    }
    return WS_ERROR;
}

void wsJsonFree(wsJson *obj) {
    if (!obj) {
        WS_LOG_ERROR("JSON obj is NULL on free!\n");
        return;
    }
    if (obj->type == WS_JSON_OBJECT) {
        for (int32_t i = 0; i < obj->object.childCount; i++) {
            wsJsonFree(obj->object.children[i]);
        }
    } else if (obj->type == WS_JSON_ARRAY) {
        for (int32_t i = 0; i < obj->array.elementCount; i++) {
            wsJsonFree(obj->array.elements[i]);
        }
    }
    free(obj);
}

