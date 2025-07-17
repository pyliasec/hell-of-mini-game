#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "player.h"

#define TRACK 20
#define RECORD_FILE "../data/horse_records.csv"

// 함수 프로토타입
void startHorseRacingGame(const char* nickname, int* coins);
void print_track(double track[4]);
void saveHorseRecord(const char* nickname, int betHorse, int betCoins, int resultHorse, int reward);

void horseRacingMenu(const char* nickname, int* coins) {
    int choice = 0;
    while (choice != -1) {
        printf("\n🏇 [경마 게임 메뉴] 🏇\n");
        printf("1. 게임 시작\n");
        printf("2. 현재 코인 보기\n");
        printf("-1. 나가기\n");
        printf("선택 >> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startHorseRacingGame(nickname, coins);
                break;
            case 2:
                printf("%s님의 현재 코인: %d\n", nickname, *coins);
                break;
            case -1:
                printf("경마 게임을 종료합니다.\n");
                break;
            default:
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
    }
}

// 경주 트랙 출력
void print_track(double track[4]) {
    for (int i = 1; i <= 3; i++) {
        int pos = (int)track[i];
        if (pos >= TRACK) pos = TRACK - 1;
        printf("%d번 말: ", i);
        for (int j = 0; j < TRACK; j++) {
            printf(j == pos ? "🐎" : "-");
        }
        printf("\n");
    }
    printf("\n");
}

// 기록 저장
void saveHorseRecord(const char* nickname, int betHorse, int betCoins, int resultHorse, int reward) {
    FILE* f = fopen(RECORD_FILE, "a");
    if (f) {
        fprintf(f, "%s,%d,%d,%d,%d\n", nickname, betHorse, betCoins, resultHorse, reward);
        fclose(f);
    }
}

// 경마 게임 시작
void startHorseRacingGame(const char* nickname, int* coins) {
    int betCoins, horseChoice;
    double track[4] = { 0 };
    double moveValue[4] = { 0 };
    int winner = 0;

    printf("\n현재 코인: %d\n", *coins);
    printf("배팅할 코인을 입력하세요 (최소 5 ~ 최대 1000): ");
    scanf("%d", &betCoins);
    if (betCoins < 5 || betCoins > 1000 || betCoins > *coins) {
        printf("배팅 조건을 만족하지 않거나 보유 코인이 부족합니다.\n");
        return;
    }

    printf("🏇 배팅할 말 번호를 선택하세요 (1~3): ");
    scanf("%d", &horseChoice);
    if (horseChoice < 1 || horseChoice > 3) {
        printf("유효하지 않은 말 번호입니다.\n");
        return;
    }

    *coins -= betCoins;

    srand((unsigned)time(NULL));

    while (1) {
        system("clear");

        for (int i = 1; i <= 3; i++) {
            int chance = rand() % 100;
            if ((i == 1 && chance < 80) || (i == 2 && chance < 50) || (i == 3 && chance < 20)) {
                moveValue[i] = (rand() % 10 + 1) / 10.0 + ((i == 2) ? 0.8 : 1.4);
                track[i] += moveValue[i];
            }
        }

        print_track(track);

        if (track[1] >= TRACK || track[2] >= TRACK || track[3] >= TRACK) {
            if (track[1] >= TRACK) winner = 1;
            else if (track[2] >= TRACK) winner = 2;
            else winner = 3;
            break;
        }

        usleep(200000);
    }

    printf("🏁 %d번 말이 우승했습니다!\n", winner);
    int reward = 0;
    if (horseChoice == winner) {
        if (winner == 1) reward = 2 * betCoins;
        else if (winner == 2) reward = 4 * betCoins;
        else reward = 8 * betCoins;
        printf("축하합니다! %d 코인을 획득했습니다.\n", reward);
        *coins += reward;
    } else {
        printf("아쉽게도 배팅 실패입니다. 코인을 잃었습니다.\n");
    }

    saveHorseRecord(nickname, horseChoice, betCoins, winner, reward);
}