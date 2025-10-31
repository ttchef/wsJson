
#include <wsJson/ws_json.h>

int main() {

    // Init a new Json object
    wsJson* root = wsJsonInitChild(NULL);

    wsJsonAddNumber(root, "number", 32);
    wsJsonAddString(root, "name", "Fred");
    wsJsonAddBool(root, "bool", true);

    wsJsonAddField(root, wsJsonInitNull("idk"));

    // Print Json
    char string[2000];
    wsJsonToString(root, string, 2000);
    printf("%s\n", string);

    wsJsonFree(root);

    return 0;
}

