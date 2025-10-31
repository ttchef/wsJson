
#include <wsJson/ws_json.h>

int main() {

    // Init a new Json object
    wsJson* root = wsJsonInitChild(NULL);

    wsJsonAddNumber(root, "number", 32);
    wsJsonAddString(root, "name", "Fred");
    wsJsonAddBool(root, "bool", true);

    // Null Field with conversion
    wsJsonAddNull(root, "player");
    wsJson* player = wsJsonInitChild(NULL);
    wsJsonAddNumber(player, "lives", 100);
    wsJsonAddBool(player, "alive", true);
    wsJsonNullToChild(root, "player", player);

    // Get Value
    wsJson* playerGet = wsJsonGet(root, "player");
    double* lives = wsJsonGetNumber(playerGet, "lives");
    printf("Lives: %.2lf\n", *lives);

    // Set Value
    const char* newName = "Fredmaster";
    char* dest = wsJsonGetString(root, "name");
    memset(dest, 0, WS_JSON_MAX_VALUE_SIZE);
    memcpy(dest, newName, strlen(newName));
    // Get new value
    char name[WS_JSON_MAX_VALUE_SIZE];
    memcpy(name, wsJsonGetString(root, "name"), WS_JSON_MAX_VALUE_SIZE);
    printf("Name: %s\n", name);

    // Print Json
    char string[2000];
    wsJsonToString(root, string, 2000);
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
