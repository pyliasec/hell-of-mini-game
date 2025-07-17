#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "player.h"

#define NUM_SYMBOLS 6
#define MIN_BET      5
#define MAX_BET   1000
#define RECORD_FILE "slot_records.csv"

const char* symbols[NUM_SYMBOLS] = { "♠", "♥", "◆", "★", "♣", "♡" };
int weights[NUM_SYMBOLS] = { 30, 25, 20, 15, 7, 3 };
int payout3[NUM_SYMBOLS] = { 3, 4, 5, 10, 15, 20 };
int payout2[NUM_SYMBOLS] = { 1, 2, 2, 5, 7, 10 };

// 함수 프로토타입
void slotMachineMenu(const char* nickname, int* coins);
void startSlotMachineGame(const char* nickname, int* coins);
int getBet(int* coins);
int spinReel(void);
void saveSlotRecord(const char* nickname, int bet, int result0, int result1, int result2, int winAmount);
void showSlotHistory(const char* nickname);

void slotMachineMenu(const char* nickname, int* coins) {
    int choice = 0;
    while (choice != -1) {
        printf("\n[슬롯머신 게임 메뉴]\n");
        printf("1. 게임 시작\n");
        printf("2. 현재 코인 보기\n");
        printf("3. 게임 기록 보기\n");
        printf("-1. 나가기\n");
        printf("선택 >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startSlotMachineGame(nickname, coins);
                break;
            case 2:
                printf("%s님의 현재 코인: %d\n", nickname, *coins);
                break;
            case 3:
                showSlotHistory(nickname);
                break;
            case -1:
                printf("슬롯머신 게임을 종료합니다.\n");
                break;
            default:
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
    }
}

void startSlotMachineGame(const char* nickname, int* coins) {
    srand((unsigned)time(NULL));
    int bet = getBet(coins);
    if (bet > 0) {
        *coins -= bet;
        int result[3];
        for (int i = 0; i < 3; i++) result[i] = spinReel();
        
        printf("\n[스핀 결과] %s | %s | %s\n",
            symbols[result[0]], symbols[result[1]], symbols[result[2]]);
        
        int winAmount = 0;
        if (result[0] == result[1] && result[1] == result[2]) {
            int multiplier = payout3[result[0]];
            winAmount = bet * multiplier;
            printf("3개 일치 (%dx) → +%d 코인\n", multiplier, winAmount);
            *coins += winAmount;
        }
        else if (result[0] == result[1] || result[1] == result[2] || result[0] == result[2]) {
            int idx = (result[0] == result[1] ? result[0]
                : result[1] == result[2] ? result[1]
                : result[0]);
            int multiplier = payout2[idx];
            winAmount = bet * multiplier;
            printf("✨ 2개 일치 (%dx) → +%d 코인\n", multiplier, winAmount);
            *coins += winAmount;
        }
        else {
            printf("꽝! +0 코인\n");
        }
        
        printf("현재 코인: %d\n", *coins);
        saveSlotRecord(nickname, bet, result[0], result[1], result[2], winAmount);
    }
}

int getBet(int* coins) {
    int bet;
    printf("\n현재 코인: %d\n", *coins);
    do {
        printf("베팅 금액 (%d~%d) ▶ ", MIN_BET, MAX_BET);
        if (scanf("%d", &bet) != 1) {
            while (getchar() != '\n');
            bet = 0;
        }
        if (bet < MIN_BET || bet > MAX_BET)
            printf("  * %d에서 %d 사이로 입력하세요.\n", MIN_BET, MAX_BET);
        else if (bet > *coins)
            printf("  * 잔액(%d)보다 높습니다.\n", *coins);
    } while (bet < MIN_BET || bet > MAX_BET || bet > *coins);
    return bet;
}

int spinReel(void) {
    int total = 0;
    for (int i = 0; i < NUM_SYMBOLS; i++) total += weights[i];
    int r = rand() % total;
    int cum = 0;
    for (int i = 0; i < NUM_SYMBOLS; i++) {
        cum += weights[i];
        if (r < cum) return i;
    }
    return NUM_SYMBOLS - 1;
}

void saveSlotRecord(const char* nickname, int bet, int result0, int result1, int result2, int winAmount) {
    FILE* f = fopen(RECORD_FILE, "a");
    if (f) {
        fprintf(f, "%s,%d,%d,%d,%d,%d\n", nickname, bet, result0, result1, result2, winAmount);
        fclose(f);
    }
}

void showSlotHistory(const char* nickname) {
    FILE* f = fopen(RECORD_FILE, "r");
    if (!f) {
        printf("게임 기록이 없습니다.\n");
        return;
    }
    
    printf("\n=== %s님의 슬롯머신 기록 ===\n", nickname);
    printf("베팅 | 결과 | 당첨금\n");
    printf("------------------\n");
    
    char line[256];
    char savedName[50];
    int bet, r0, r1, r2, win;
    int totalBet = 0, totalWin = 0, games = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d", savedName, &bet, &r0, &r1, &r2, &win) == 6) {
            if (strcmp(savedName, nickname) == 0) {
                printf("%4d | %s %s %s | %4d\n", bet, symbols[r0], symbols[r1], symbols[r2], win);
                totalBet += bet;
                totalWin += win;
                games++;
            }
        }
    }
    
    if (games > 0) {
        printf("------------------\n");
        printf("게임 수: %d\n", games);
        printf("총 베팅: %d\n", totalBet);
        printf("총 당첨: %d\n", totalWin);
        printf("순손익: %s%d\n", (totalWin - totalBet >= 0) ? "+" : "", totalWin - totalBet);
    } else {
        printf("기록이 없습니다.\n");
    }
    
    fclose(f);
}


