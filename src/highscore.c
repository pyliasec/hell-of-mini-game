/*
 * ============================================================================
 * 파일명: highscore.c
 * 설명: 하이스코어 보드 관리 모듈
 * 작성자: pyliasec
 * 작성일: 2025-07-17
 * 
 * 주요 기능:
 * - 모든 게임의 기록 통합 관리
 * - 정렬 및 검색 기능
 * - 페이지네이션을 통한 데이터 표시
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "highscore.h"

#define MAX_RECORDS 1000      // 최대 기록 수
#define RECORDS_PER_PAGE 5    // 페이지당 표시할 기록 수

/*
 * ============================================================================
 * 부가 기능: 데이터 관리를 파일로 처리
 * 모든 게임 기록 파일들의 경로
 * ============================================================================
 */
const char* gameFiles[] = {
    "../data/slot_records.csv",     // 슬롯머신 기록
    "../data/horse_records.csv",    // 경마 기록
    "../data/rock_records.csv",     // 가위바위보 기록
    "../data/roulette_history.csv"  // 룰렛 기록
};

const char* gameNames[] = {
    "슬롯머신",
    "경마", 
    "가위바위보",
    "룰렛"
};

const int numGames = 4;

/*
 * ============================================================================
 * 필수 기능: 데이터 입력/출력
 * 각 게임별 기록 로드 함수들의 프로토타입
 * ============================================================================
 */
int loadSlotRecords(HighScoreRecord records[], int maxRecords);
int loadHorseRecords(HighScoreRecord records[], int maxRecords);
int loadRockRecords(HighScoreRecord records[], int maxRecords);
int loadRouletteRecords(HighScoreRecord records[], int maxRecords);
int loadAllRecords(HighScoreRecord records[], int maxRecords);
void sortRecordsByWinAmount(HighScoreRecord records[], int count);
void displayRecordsWithPagination(HighScoreRecord records[], int count, const char* title);
void searchByNickname(void);  // 필수 기능: 검색 기능

void showHighScoreMenu(void) {
    int choice = 0;
    while (choice != -1) {
        printf("\n[하이스코어 보드]\n");
        printf("1. 전체 하이스코어\n");
        printf("2. 슬롯머신 하이스코어\n");
        printf("3. 경마 하이스코어\n");
        printf("4. 가위바위보 하이스코어\n");
        printf("5. 룰렛 하이스코어\n");
        printf("6. 닉네임별 검색\n");
        printf("-1. 나가기\n");
        printf("선택 >> ");
        
        if (scanf("%d", &choice) != 1) {
            // 입력 버퍼 클리어
            while (getchar() != '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                showAllHighScores();
                break;
            case 2:
                showGameHighScores("슬롯머신");
                break;
            case 3:
                showGameHighScores("경마");
                break;
            case 4:
                showGameHighScores("가위바위보");
                break;
            case 5:
                showGameHighScores("룰렛");
                break;
            case 6:
                searchByNickname();
                break;
            case -1:
                printf("하이스코어 보드를 종료합니다.\n");
                break;
            default:
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
        }
    }
}

void showAllHighScores(void) {
    HighScoreRecord records[MAX_RECORDS];
    int count = loadAllRecords(records, MAX_RECORDS);
    
    if (count == 0) {
        printf("기록이 없습니다.\n");
        return;
    }
    
    sortRecordsByWinAmount(records, count);
    displayRecordsWithPagination(records, count, "전체");
}

void showGameHighScores(const char* gameName) {
    HighScoreRecord records[MAX_RECORDS];
    int count = 0;
    
    if (strcmp(gameName, "슬롯머신") == 0) {
        count = loadSlotRecords(records, MAX_RECORDS);
    } else if (strcmp(gameName, "경마") == 0) {
        count = loadHorseRecords(records, MAX_RECORDS);
    } else if (strcmp(gameName, "가위바위보") == 0) {
        count = loadRockRecords(records, MAX_RECORDS);
    } else if (strcmp(gameName, "룰렛") == 0) {
        count = loadRouletteRecords(records, MAX_RECORDS);
    }
    
    if (count == 0) {
        printf("%s 기록이 없습니다.\n", gameName);
        return;
    }
    
    sortRecordsByWinAmount(records, count);
    displayRecordsWithPagination(records, count, gameName);
}

int loadSlotRecords(HighScoreRecord records[], int maxRecords) {
    FILE* f = fopen("../data/slot_records.csv", "r");
    if (!f) return 0;
    
    char line[256];
    int count = 0;
    char nickname[50];
    int bet, r0, r1, r2, win;
    
    while (fgets(line, sizeof(line), f) && count < maxRecords) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d", nickname, &bet, &r0, &r1, &r2, &win) == 6) {
            if (win > bet) { // 실제로 이익을 본 경우만
                strcpy(records[count].nickname, nickname);
                strcpy(records[count].game, "슬롯머신");
                records[count].betAmount = bet;
                records[count].winAmount = win - bet; // 순이익
                count++;
            }
        }
    }
    
    fclose(f);
    return count;
}

int loadHorseRecords(HighScoreRecord records[], int maxRecords) {
    FILE* f = fopen("../data/horse_records.csv", "r");
    if (!f) return 0;
    
    char line[256];
    int count = 0;
    char nickname[50];
    int betHorse, betCoins, resultHorse, reward;
    
    while (fgets(line, sizeof(line), f) && count < maxRecords) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d", nickname, &betHorse, &betCoins, &resultHorse, &reward) == 5) {
            if (reward > betCoins) { // 실제로 이익을 본 경우만
                strcpy(records[count].nickname, nickname);
                strcpy(records[count].game, "경마");
                records[count].betAmount = betCoins;
                records[count].winAmount = reward - betCoins; // 순이익
                count++;
            }
        }
    }
    
    fclose(f);
    return count;
}

int loadRockRecords(HighScoreRecord records[], int maxRecords) {
    FILE* f = fopen("../data/rock_records.csv", "r");
    if (!f) return 0;
    
    char line[256];
    int count = 0;
    char nickname[50];
    int bet, user, computer, result, win;
    
    while (fgets(line, sizeof(line), f) && count < maxRecords) {
        if (sscanf(line, "%49[^,],%d,%d,%d,%d,%d", nickname, &bet, &user, &computer, &result, &win) == 6) {
            if (win > bet) { // 실제로 이익을 본 경우만
                strcpy(records[count].nickname, nickname);
                strcpy(records[count].game, "가위바위보");
                records[count].betAmount = bet;
                records[count].winAmount = win - bet; // 순이익
                count++;
            }
        }
    }
    
    fclose(f);
    return count;
}

int loadRouletteRecords(HighScoreRecord records[], int maxRecords) {
    FILE* f = fopen("../data/roulette_history.csv", "r");
    if (!f) return 0;
    
    char line[256];
    int count = 0;
    char nickname[50], color[10];
    int number, bet, payout;
    
    while (fgets(line, sizeof(line), f) && count < maxRecords) {
        if (sscanf(line, "%49[^,],%d,%9[^,],%d,%d", nickname, &number, color, &bet, &payout) == 5) {
            if (payout > bet) { // 실제로 이익을 본 경우만
                strcpy(records[count].nickname, nickname);
                strcpy(records[count].game, "룰렛");
                records[count].betAmount = bet;
                records[count].winAmount = payout - bet; // 순이익
                count++;
            }
        }
    }
    
    fclose(f);
    return count;
}

int loadAllRecords(HighScoreRecord records[], int maxRecords) {
    int totalCount = 0;
    
    totalCount += loadSlotRecords(records + totalCount, maxRecords - totalCount);
    totalCount += loadHorseRecords(records + totalCount, maxRecords - totalCount);
    totalCount += loadRockRecords(records + totalCount, maxRecords - totalCount);
    totalCount += loadRouletteRecords(records + totalCount, maxRecords - totalCount);
    
    return totalCount;
}

/*
 * ============================================================================
 * 필수 기능: 정렬 기능
 * 버블 정렬을 사용하여 기록을 획득 금액 기준 내림차순으로 정렬
 * 
 * 매개변수:
 *   records - 정렬할 기록 배열
 *   count - 기록의 개수
 * ============================================================================
 */
void sortRecordsByWinAmount(HighScoreRecord records[], int count) {
    // 버블 정렬 알고리즘 구현 (내림차순)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            // 획득 금액을 기준으로 비교하여 정렬
            if (records[j].winAmount < records[j + 1].winAmount) {
                HighScoreRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

void displayRecordsWithPagination(HighScoreRecord records[], int count, const char* title) {
    int currentPage = 0;
    int totalPages = (count + RECORDS_PER_PAGE - 1) / RECORDS_PER_PAGE;
    char input;
    
    if (count == 0) {
        printf("%s 하이스코어 기록이 없습니다.\n", title);
        return;
    }
    
    while (1) {
        printf("\n === %s 하이스코어 === \n", title);
        printf("페이지 %d/%d\n", currentPage + 1, totalPages);
        
        if (currentPage > 0) {
            printf("[이전 페이지 보기: < 입력]\n");
        }
        
        int start = currentPage * RECORDS_PER_PAGE;
        int end = start + RECORDS_PER_PAGE;
        if (end > count) end = count;
        
        for (int i = start; i < end; i++) {
            printf("%d. %s (%s) - 베팅: %d, 획득: +%d 코인\n", 
                   i - start + 1,
                   records[i].nickname,
                   records[i].game,
                   records[i].betAmount,
                   records[i].winAmount);
        }
        
        if (currentPage < totalPages - 1) {
            printf("[다음 페이지 보기: > 입력]\n");
        }
        printf("[메뉴로 돌아가기: q 입력]\n");
        printf("입력 >> ");
        
        // 입력 버퍼 클리어
        while (getchar() != '\n');
        
        if (scanf(" %c", &input) != 1) {
            input = 'q';
        }
        
        if (input == '<' && currentPage > 0) {
            currentPage--;
        } else if (input == '>' && currentPage < totalPages - 1) {
            currentPage++;
        } else if (input == 'q' || input == 'Q') {
            break;
        } else {
            printf("잘못된 입력입니다. (<, >, q 중 하나를 입력하세요)\n");
            printf("계속하려면 Enter를 누르세요...");
            while (getchar() != '\n');
        }
    }
}

/*
 * ============================================================================
 * 필수 기능: 검색 기능
 * 닉네임을 입력받아 해당 닉네임의 모든 게임 기록을 검색하여 표시
 * ============================================================================
 */
void searchByNickname(void) {
    char searchNickname[50];
    printf("\n검색할 닉네임을 입력하세요: ");
    scanf("%49s", searchNickname);
    
    // 모든 게임 기록을 로드
    HighScoreRecord allRecords[MAX_RECORDS];
    int totalCount = loadAllRecords(allRecords, MAX_RECORDS);
    
    // 검색된 기록을 저장할 배열
    HighScoreRecord searchResults[MAX_RECORDS];
    int foundCount = 0;
    
    // 닉네임이 정확히 일치하는 기록만 검색
    for (int i = 0; i < totalCount; i++) {
        if (strcmp(allRecords[i].nickname, searchNickname) == 0) {
            searchResults[foundCount] = allRecords[i];
            foundCount++;
        }
    }
    
    // 검색 결과 출력
    if (foundCount > 0) {
        // 검색된 기록을 획득 금액 기준으로 정렬
        sortRecordsByWinAmount(searchResults, foundCount);
        
        char title[100];
        sprintf(title, "%s님의 게임 기록", searchNickname);
        displayRecordsWithPagination(searchResults, foundCount, title);
        
        // 통계 정보 출력
        int totalBet = 0, totalWin = 0;
        for (int i = 0; i < foundCount; i++) {
            totalBet += searchResults[i].betAmount;
            totalWin += searchResults[i].winAmount;
        }
        
        printf("\n=== %s님의 게임 통계 ===\n", searchNickname);
        printf("총 게임 수: %d\n", foundCount);
        printf("총 베팅 금액: %d\n", totalBet);
        printf("총 획득 금액: %d\n", totalWin);
        printf("순손익: %s%d\n", (totalWin - totalBet >= 0) ? "+" : "", totalWin - totalBet);
        
    } else {
        printf("\n'%s' 닉네임의 게임 기록이 존재하지 않습니다.\n", searchNickname);
        printf("없는 닉네임이거나 아직 게임을 플레이하지 않았습니다.\n");
    }
}
