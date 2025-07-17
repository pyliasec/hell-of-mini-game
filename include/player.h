#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LEN 50

typedef struct {
    char nickname[MAX_NAME_LEN];  // 로그인 시 입력한 닉네임
    int coins;                    // 현재 보유 코인
} Player;

int loadPlayer(Player* p, const char* name);
void savePlayer(const Player* p);
void printPlayerInfo(const Player* p);

#endif

// #ifndef PLAYER_H
// #define PLAYER_H

// #define MAX_NAME_LEN 50

// typedef struct {
//     char nickname[MAX_NAME_LEN];
//     int coins;
// } Player;

// int loadPlayer(Player* p, const char* name);
// void savePlayer(const Player* p);
// void printPlayerInfo(const Player* p);

// #endif