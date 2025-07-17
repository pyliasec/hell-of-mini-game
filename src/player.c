#include <stdio.h>
#include <string.h>
#include "player.h"

#define SAVE_FILE "../data/players.csv"

int loadPlayer(Player* p, const char* name) {
    FILE* file = fopen("../data/players.csv", "r");
    if (!file) {
        strcpy(p->nickname, name);
        p->coins = 100;
        return 0;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char nick[MAX_NAME_LEN];
        int coins;
        if (sscanf(line, "%[^,],%d", nick, &coins) == 2) {
            if (strcmp(nick, name) == 0) {
                strcpy(p->nickname, nick);
                p->coins = coins;
                fclose(file);
                return 1;  // 기존 유저
            }
        }
    }

    fclose(file);
    strcpy(p->nickname, name);
    p->coins = 100;
    return 0;  // 신규 유저
}

void savePlayer(const Player* p) {
    FILE* input = fopen(SAVE_FILE, "r");
    FILE* temp = fopen("../data/temp.csv", "w");
    int found = 0;
    char line[100];

    if (input) {
        while (fgets(line, sizeof(line), input)) {
            char nick[MAX_NAME_LEN];
            sscanf(line, "%[^,]", nick);
            if (strcmp(nick, p->nickname) == 0) {
                fprintf(temp, "%s,%d\n", p->nickname, p->coins);
                found = 1;
            } else {
                fputs(line, temp);
            }
        }
        fclose(input);
    }

    if (!found) {
        fprintf(temp, "%s,%d\n", p->nickname, p->coins);
    }

    fclose(temp);
    remove(SAVE_FILE);
    rename("../data/temp.csv", SAVE_FILE);
}

void printPlayerInfo(const Player* p) {
    printf("닉네임: %s\n", p->nickname);
    printf("보유 코인: %d\n", p->coins);
}