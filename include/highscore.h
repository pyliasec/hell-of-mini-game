#ifndef HIGHSCORE_H
#define HIGHSCORE_H

typedef struct {
    char nickname[50];
    char game[20];
    int betAmount;
    int winAmount;
} HighScoreRecord;

void showHighScoreMenu(void);
void showAllHighScores(void);
void showGameHighScores(const char* gameName);

#endif
