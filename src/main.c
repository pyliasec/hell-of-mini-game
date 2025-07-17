/*
 * ============================================================================
 * 파일명: main.c
 * 설명: 도박 중독 예방 교육용 게임 시뮬레이션
 * 작성자: pyliasec
 * 작성일: 2025-07-17
 * 
 * 프로그램 목적:
 * - 도박의 위험성과 중독성을 체험할 수 있는 교육적 시뮬레이션
 * - 확률과 손실에 대한 현실적 이해 제공
 * - 건전한 여가 생활의 중요성 인식 도모
 * 
 * 주의: 이 프로그램은 실제 도박을 권장하지 않으며,
 *          도박 중독 예방을 위한 교육 목적으로만 제작되었습니다.
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "auth.h"              // 사용자 인증 시스템
#include "player.h"            // 플레이어 데이터 관리
#include "rock.h"              // 가위바위보 게임
#include "horse_racing.h"      // 경마 게임
#include "slot_machine.h"      // 슬롯머신 게임
#include "roulette.h"          // 룰렛 게임
#include "highscore.h"         // 하이스코어 보드
#include "gambling_prevention.h" // 도박 예방 시스템

/*
 * ============================================================================
 * 필수 기능: 메뉴 제공 기능
 * 메인 게임 메뉴를 표시하는 함수
 * ============================================================================
 */
void showMenu() {
    printf("\n==== 게임 메뉴 ====\n");
    printf("1. 슬롯머신\n");
    printf("2. 텍스트 룰렛\n");
    printf("3. 경마\n");
    printf("4. 가위바위보\n");
    printf("5. 하이스코어 보기\n");
    printf("6. 현재 코인 보기\n");
    printf("-1. 종료\n");
    printf("===================\n");
    printf("선택 >> ");
}

/*
 * ============================================================================
 * 필수 기능: 구조체 사용, 데이터 입력/출력, 메뉴 제공 기능
 * 메인 함수 - 프로그램의 진입점
 * ============================================================================
 */
int main() {
    // 프로그램 시작 시 교육적 목적 설명
    showProjectPurpose();
    showGamblingPreventionMessage();
    
    Player player;  // 필수 기능: 구조체 사용
    char nickname[MAX_NAME_LEN] = "";
    int authChoice;

    /*
     * ========================================================================
     * 필수 기능: 데이터 입력/출력, 메뉴 제공 기능
     * 사용자 인증 시스템 (로그인/회원가입)
     * ========================================================================
     */
    while (strlen(nickname) == 0) {
        printf("\n==== 로그인 시스템 ====\n");
        printf("1. 로그인\n");
        printf("2. 회원가입\n");
        printf("-1. 종료\n");
        printf("선택 >> ");
        scanf("%d", &authChoice);

        if (authChoice == 1) {
            promptLoginFlow(nickname);
        } else if (authChoice == 2) {
            promptRegisterFlow(nickname);
        } else if (authChoice == -1) {
            printf("프로그램을 종료합니다.\n");
            return 0;
        } else {
            printf("잘못된 입력입니다.\n");
        }
    }

    // 유저 정보 불러오기
    // loadPlayer(&player, nickname);
    int isNew = !loadPlayer(&player, nickname);
    if (isNew) {
        player.coins = 100;
        savePlayer(&player);
    }

    printf("환영합니다, %s님! 현재 코인: %d\n", player.nickname, player.coins);
    printf(
            "               (( _______\n"
            "     _______     /\\O    O\\\n"
            "    /O     /\\   /  \\      \\\n"
            "   /   O  /O \\ / O  \\O____O\\ ))\n"
            "((/_____O/    \\\\    /O     /\n"
            "  \\O    O\\    / \\  /   O  /\n"
            "   \\O    O\\ O/   \\/_____O/\n"
            "    \\O____O\\/ ))          ))\n"
            "  ((\n"
            );
    
    // 메인 게임 루프
    int choice = 0;
    while (choice != -1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                slotMachineMenu(player.nickname, &player.coins);
                savePlayer(&player);
                break;
            case 2:
                startRouletteGame(player.nickname, &player.coins);
                savePlayer(&player);
                break;
            case 3:
                horseRacingMenu(player.nickname, &player.coins);
                savePlayer(&player);
                break;
            case 4:
                rockPaperScissorsMenu(player.nickname, &player.coins);
                savePlayer(&player);
                break;
            case 5:
                showHighScoreMenu();
                break;
            case 6:
                printf("\n현재 코인: %d\n", player.coins);
                break;
            case -1:
                savePlayer(&player);  // 종료 전 저장
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 입력입니다.\n");
                break;
        }
    }
    savePlayer(&player); // while 루프 종료 후에도 저장
    return 0;
}



/*

cd /Users/pyliasec/Documents/hell_of_mini_game && gcc main.c auth.c player.c horse_racing.c roulette.c slot_machine.c rock.c highscore.c \
  -I/opt/homebrew/opt/openssl/include \
  -L/opt/homebrew/opt/openssl/lib \
  -lssl -lcrypto -o hellgame


  cd /Users/pyliasec/Documents/hell_of_mini_game && echo "1
pyliasec
rlarbqls135
5
1
q
-1
-1" | ./hellgame

*/