#ifndef ROULETTE_H
#define ROULETTE_H

#define MAX_NAME_LEN 50

typedef enum {
    RED,
    BLACK,
    GREEN
} Color;

typedef struct {
    int chosenNumber;
    Color chosenColor;
    int betAmount;
} RouletteGame;

void startRouletteGame(const char* playerName, int* playerCoins);

#endif