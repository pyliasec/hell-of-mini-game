#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "roulette.h"
#include "gambling_prevention.h"

#define HISTORY_FILE "data/roulette_history.csv"
#define MIN_MULTIPLIER 0.1    // 최소 배율
#define MAX_MULTIPLIER 10.0   // 최대 배율

const char* colorToStr(Color c) {
    return c == RED ? "빨강" : c == BLACK ? "검정" : "초록";
}

Color getColorFromNumber(int number) {
    if (number == 0) return GREEN;
    return number % 2 == 0 ? BLACK : RED;
}

void saveGameResult(const char* nickname, int number, Color color, int bet, int payout) {
    FILE* f = fopen(HISTORY_FILE, "a");
    if (f) {
        fprintf(f, "%s,%d,%s,%d,%d\n", nickname, number, colorToStr(color), bet, payout);
        fclose(f);
    }
}

void rouletteMenu() {
    printf("\n-- 텍스트 룰렛 게임 메뉴 --\n");
    printf("1. 게임 시작\n");
    printf("2. 잔액 보기\n");
    printf("-1. 나가기\n");
    printf("선택 >> ");
}

void startRouletteGame(const char* playerName, int* playerCoins) {
    int menu = 0;
    while (menu != -1) {
        rouletteMenu();
        scanf("%d", &menu);
        if (menu == 1) {
            if (*playerCoins < 5) {
                printf("코인이 부족합니다! (최소 5 필요)\n");
                continue;
            }

            // 게임 시작 전 코인 저장 (손실 체크용)
            int previousCoins = *playerCoins;

            // 배율 입력받기
            float multiplier;
            printf("\n배율을 입력하세요 (%.1f ~ %.1f): ", MIN_MULTIPLIER, MAX_MULTIPLIER);
            while (scanf("%f", &multiplier) != 1 || multiplier < MIN_MULTIPLIER || multiplier > MAX_MULTIPLIER) {
                printf("올바른 배율을 입력하세요 (%.1f ~ %.1f): ", MIN_MULTIPLIER, MAX_MULTIPLIER);
                while (getchar() != '\n'); // 입력 버퍼 클리어
            }

            int bet;
            printf("배팅할 금액 (5~1000): ");
            scanf("%d", &bet);
            if (bet < 5 || bet > 1000 || bet > *playerCoins) {
                printf("유효하지 않은 배팅 금액입니다.\n");
                continue;
            }

            int chosenNum;
            printf("숫자 선택 (0~36): ");
            scanf("%d", &chosenNum);
            if (chosenNum < 0 || chosenNum > 36) {
                printf("숫자 범위 오류.\n");
                continue;
            }

            int colorChoice;
            printf("색상 선택 (0=빨강, 1=검정, 2=초록): ");
            scanf("%d", &colorChoice);
            if (colorChoice < 0 || colorChoice > 2) {
                printf("색상 선택 오류.\n");
                continue;
            }

            RouletteGame game = { chosenNum, (Color)colorChoice, bet };

            srand((unsigned int)time(NULL));
            int resultNum = rand() % 37;
            Color resultColor = getColorFromNumber(resultNum);

            printf("결과: %d번 (%s)\n", resultNum, colorToStr(resultColor));

            int payout = 0;
            if (game.chosenNumber == resultNum) {
                // 숫자 일치 시 설정된 배율 적용
                payout = (int)(game.betAmount * multiplier * 36);
                printf("숫자 일치! 배율 %.1fx 적용 -> +%d 코인\n", multiplier, payout);
            } else if (game.chosenColor == resultColor) {
                // 색상 일치 시 설정된 배율 적용
                int baseMultiplier = (resultColor == GREEN ? 10 : 2);
                payout = (int)(game.betAmount * multiplier * baseMultiplier);
                printf("색상 일치! 배율 %.1fx 적용 -> +%d 코인\n", multiplier, payout);
            } else {
                printf("꽝! 배팅한 %d 코인 손실\n", game.betAmount);
            }

            *playerCoins += (payout - game.betAmount);
            printf("현재 잔액: %d\n", *playerCoins);

            saveGameResult(playerName, resultNum, resultColor, game.betAmount, payout);
            
            // 게임이 완전히 끝난 후 손실 체크
            if (payout == 0) {
                checkMajorLoss(previousCoins, *playerCoins, playerName);
            }
            
            // 게임이 완전히 끝난 후 파산 상태 체크 (10개 이하)
            if (*playerCoins <= 10) {
                checkBankruptcy(*playerCoins, playerName);
            }
        } else if (menu == 2) {
            printf("현재 잔액: %d\n", *playerCoins);
        } else if (menu == -1) {
            printf("룰렛 게임 종료\n");
        } else {
            printf("유효하지 않은 메뉴입니다.\n");
        }
    }
}