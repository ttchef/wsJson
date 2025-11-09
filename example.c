
#define WS_JSON_LOG_LEVEL 1
#include <wsJson/ws_json.h>

int main() {

    // Init a new Json object
    wsJson* root = wsJsonInitObject(NULL);

    wsJsonAddNumber(root, "number", 32);
    wsJsonAddString(root, "name", "Fred");
    wsJsonAddBool(root, "bool", true);

    // Null Field with conversion
    wsJsonAddNull(root, "player");
    wsJson* player = wsJsonInitObject(NULL);
    wsJsonAddNumber(player, "lives", 100);
    wsJsonAddBool(player, "alive", true);
    wsJsonSetNullToObject(root, "player", player);

    // Get Value
    double lives = wsJsonGetNumber(root, "player.lives");
    printf("Lives: %.2lf\n", lives);

    // Set Value
    wsJsonSetString(root, "name", "Fredmaster");

    // Get new value
    char name[WS_JSON_MAX_VALUE_SIZE];
    wsJsonGetStringEx(root, "name", name, WS_JSON_MAX_VALUE_SIZE);
    printf("Name: %s\n", name);

    // Arrays
    wsJson* array = wsJsonInitArray("array");
    wsJson* num = wsJsonInitNumber("num", 5);
    wsJsonAddElement(array, num);
    wsJson* color = wsJsonInitObject("color");
    wsJsonAddNumber(color, "red", 0);
    wsJsonAddNumber(color, "green", 255);
    wsJsonAddNumber(color, "blue", 255);
    wsJsonAddString(color, "name", "cyan");
    wsJsonAddElement(array, color);
    wsJsonAddField(root, array);

    printf("Array Length: %d\n", wsJsonGetArrayLen(root, "array"));

    // Print Json
    char string[2000];
    wsJsonToStringPretty(root, string, 2000);
    printf("%s\n", string);

    wsJsonFree(root);

    return 0;
}

/*
Output:

Lives: 100.00
Name: Fredmaster
{"number": 32,"name": "Fredmaster","bool": true,"player": {"lives": 100,"alive": true}}

*/

