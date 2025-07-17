#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "player.h"

#define RECORD_FILE "../data/rock_records.csv"
#define MIN_BET 5
#define MAX_BET 500

// 함수 프로토타입
void rockPaperScissorsMenu(const char* nickname, int* coins);
void startRockPaperScissorsGame(const char* nickname, int* coins);
void printChoice(int choice);
int getBetAmount(int* coins);
void saveRockRecord(const char* nickname, int bet, int userChoice, int computerChoice, int result, int winAmount);
void showRockHistory(const char* nickname);

void rockPaperScissorsMenu(const char* nickname, int* coins) {
    int choice = 0;
    while (choice != -1) {
        printf("\n[가위바위보 게임 메뉴]\n");
        printf("1. 게임 시작\n");
        printf("2. 현재 코인 보기\n");
        printf("3. 게임 기록 보기\n");
        printf("-1. 나가기\n");
        printf("선택 >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startRockPaperScissorsGame(nickname, coins);
                break;
            case 2:
                printf("%s님의 현재 코인: %d\n", nickname, *coins);
                break;
            case 3:
                showRockHistory(nickname);
                break;
            case -1:
                printf("가위바위보 게임을 종료합니다.\n");
                break;
            default:
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
    }
}

void startRockPaperScissorsGame(const char* nickname, int* coins) {
    srand((unsigned)time(NULL));
    
    // 배율 입력받기
    float multiplier;
    printf("\n배율을 입력하세요 (예: 1.5, 2.0, 0.5): ");
    while (scanf("%f", &multiplier) != 1 || multiplier <= 0) {
        printf("올바른 배율을 입력하세요 (양수): ");
        while (getchar() != '\n'); // 입력 버퍼 클리어
    }
    
    int bet = getBetAmount(coins);
    if (bet == 0) return;
    
    int user, computer;
    printf("\n가위(1), 바위(2), 보(3) 중 하나를 선택하세요: ");
    scanf("%d", &user);

    if (user < 1 || user > 3) {
        printf("잘못된 입력입니다. 1~3 사이 숫자를 입력하세요.\n");
        return;
    }

    computer = rand() % 3 + 1;
    *coins -= bet;

    printf("\n사용자: ");
    printChoice(user);
    printf(" vs 컴퓨터: ");
    printChoice(computer);
    printf("\n");

    int result; // 0: 무승부, 1: 승리, 2: 패배
    int winAmount = 0;
    
    if (user == computer) {
        printf("무승부입니다! 베팅 금액을 돌려받습니다.\n");
        result = 0;
        winAmount = bet;
        *coins += bet;
    }
    else if ((user == 1 && computer == 3) ||
             (user == 2 && computer == 1) ||
             (user == 3 && computer == 2)) {
        // 승리 시 설정된 배율 적용
        winAmount = (int)(bet * multiplier * 2);
        printf("🏆 승리! 배율 %.1fx 적용 → +%d 코인\n", multiplier, winAmount);
        result = 1;
        *coins += winAmount;
    }
    else {
        printf("💸 패배! 베팅 금액을 잃었습니다.\n");
        result = 2;
        winAmount = 0;
    }
    
    printf("현재 코인: %d\n", *coins);
    saveRockRecord(nickname, bet, user, computer, result, winAmount);
}

void printChoice(int choice) {
    switch (choice) {
    case 1: printf("가위"); break;
    case 2: printf("바위"); break;
    case 3: printf("보"); break;
    default: printf("잘못된 선택"); break;
    }
}

int getBetAmount(int* coins) {
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

void saveRockRecord(const char* nickname, int bet, int userChoice, int computerChoice, int result, int winAmount) {
    FILE* f = fopen(RECORD_FILE, "a");
    if (f) {
        fprintf(f, "%s,%d,%d,%d,%d,%d\n", nickname, bet, userChoice, computerChoice, result, winAmount);
        fclose(f);
    }
}

void showRockHistory(const char* nickname) {
    FILE* f = fopen(RECORD_FILE, "r");
    if (!f) {
        printf("게임 기록이 없습니다.\n");
        return;
    }
    
    printf("\n=== %s님의 가위바위보 기록 ===\n", nickname);
    printf("베팅 | 나 vs 컴퓨터 | 결과 | 당첨금\n");
    printf("-----------------------------\n");
    
    char line[256];
    char savedName[50];
    int bet, user, computer, result, win;
    int totalBet = 0, totalWin = 0, games = 0;
    int wins = 0, losses = 0, draws = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d", savedName, &bet, &user, &computer, &result, &win) == 6) {
            if (strcmp(savedName, nickname) == 0) {
                printf("%4d | ", bet);
                printChoice(user);
                printf(" vs ");
                printChoice(computer);
                printf(" | ");
                
                switch(result) {
                    case 0: printf("무승부"); draws++; break;
                    case 1: printf("승리  "); wins++; break;
                    case 2: printf("패배  "); losses++; break;
                }
                printf(" | %4d\n", win);
                
                totalBet += bet;
                totalWin += win;
                games++;
            }
        }
    }
    
    if (games > 0) {
        printf("-----------------------------\n");
        printf("게임 수: %d (승:%d 패:%d 무:%d)\n", games, wins, losses, draws);
        printf("총 베팅: %d\n", totalBet);
        printf("총 당첨: %d\n", totalWin);
        printf("순손익: %s%d\n", (totalWin - totalBet >= 0) ? "+" : "", totalWin - totalBet);
        if (games > 0) {
            printf("승률: %.1f%%\n", (float)wins / games * 100);
        }
    } else {
        printf("기록이 없습니다.\n");
    }
    
    fclose(f);
}

