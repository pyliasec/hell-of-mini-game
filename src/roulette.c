#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "roulette.h"

#define HISTORY_FILE "../data/roulette_history.csv"

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
                payout = game.betAmount * 36;
                printf("숫자 일치! 36배 당첨!\n");
            } else if (game.chosenColor == resultColor) {
                payout = game.betAmount * (resultColor == GREEN ? 10 : 2);
                printf("색상 일치! %dx 당첨!\n", (resultColor == GREEN ? 10 : 2));
            } else {
                printf("꽝! 배팅한 %d 코인 손실\n", game.betAmount);
            }

            *playerCoins += (payout - game.betAmount);
            printf("현재 잔액: %d\n", *playerCoins);

            saveGameResult(playerName, resultNum, resultColor, game.betAmount, payout);
        } else if (menu == 2) {
            printf("현재 잔액: %d\n", *playerCoins);
        } else if (menu == -1) {
            printf("룰렛 게임 종료\n");
        } else {
            printf("유효하지 않은 메뉴입니다.\n");
        }
    }
}