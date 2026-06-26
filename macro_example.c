
#define WS_JSON_IMPLEMENTATION
#include "src/wsJson.h"

int main() {
	wsJsonRoot(root) {
		wsJsonAddNumber(root, "hello", 67);

		wsJsonObject(player) {
			wsJsonAddNumber(player, "lives", 100);
			wsJsonAddString(player, "name", "ttchef");
		}
	}

	char string[256];
	wsJsonToStringPretty(root, string, sizeof(string));
	printf("%s\n", string);

	int lives = wsJsonGetNumber(root, "player.lives");
	printf("Lives: %d\n", lives);

	return 0;
} 
