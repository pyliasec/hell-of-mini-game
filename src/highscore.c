#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "highscore.h"

#define MAX_RECORDS 1000
#define RECORDS_PER_PAGE 5

// 모든 게임 기록 파일들
const char* gameFiles[] = {
    "slot_records.csv",
    "horse_records.csv", 
    "rock_records.csv",
    "roulette_history.csv"
};

const char* gameNames[] = {
    "슬롯머신",
    "경마",
    "가위바위보",
    "룰렛"
};

const int numGames = 4;

// 함수 프로토타입
int loadSlotRecords(HighScoreRecord records[], int maxRecords);
int loadHorseRecords(HighScoreRecord records[], int maxRecords);
int loadRockRecords(HighScoreRecord records[], int maxRecords);
int loadRouletteRecords(HighScoreRecord records[], int maxRecords);
int loadAllRecords(HighScoreRecord records[], int maxRecords);
void sortRecordsByWinAmount(HighScoreRecord records[], int count);
void displayRecordsWithPagination(HighScoreRecord records[], int count, const char* title);

void showHighScoreMenu(void) {
    int choice = 0;
    while (choice != -1) {
        printf("\n[하이스코어 보드]\n");
        printf("1. 전체 하이스코어\n");
        printf("2. 슬롯머신 하이스코어\n");
        printf("3. 경마 하이스코어\n");
        printf("4. 가위바위보 하이스코어\n");
        printf("5. 룰렛 하이스코어\n");
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
    FILE* f = fopen("slot_records.csv", "r");
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
    FILE* f = fopen("horse_records.csv", "r");
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
    FILE* f = fopen("rock_records.csv", "r");
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
    FILE* f = fopen("roulette_history.csv", "r");
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

void sortRecordsByWinAmount(HighScoreRecord records[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
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
