/*
 * ============================================================================
 * 파일명: player.c
 * 설명: 플레이어 데이터 관리 모듈
 * 작성자: pyliasec
 * 작성일: 2025-07-17
 * 
 * 주요 기능:
 * - 플레이어 정보 파일 입출력
 * - 코인 관리 시스템
 * - 플레이어 데이터 영속성
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "player.h"

/*
 * ============================================================================
 * 부가 기능: 데이터 관리를 파일로 처리
 * 플레이어 데이터 파일 경로
 * ============================================================================
 */
#define SAVE_FILE "../data/players.csv"

/*
 * ============================================================================
 * 필수 기능: 데이터 입력/출력, 구조체 사용
 * 파일에서 플레이어 데이터를 로드하는 함수
 * 
 * 매개변수:
 *   p - 플레이어 구조체 포인터
 *   name - 플레이어 닉네임
 * 
 * 반환값:
 *   1 - 기존 플레이어 로드 성공
 *   0 - 신규 플레이어 (기본값 설정)
 * ============================================================================
 */
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