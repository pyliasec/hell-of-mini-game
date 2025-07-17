#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include "auth.h"

#define MAX_PASS_LEN 50
#define USER_FILE "users.csv"

// SHA256 해시 함수
void hashPassword(const char* password, char* hashOutput) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password, strlen(password), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashOutput + (i * 2), "%02x", hash[i]);
    }
    hashOutput[64] = '\0';
}

// 닉네임 존재 여부 확인
bool checkUserExists(const char* nickname) {
    FILE* f = fopen(USER_FILE, "r");
    if (!f) return false;
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        char storedNick[MAX_NAME_LEN];
        sscanf(line, "%[^,]", storedNick);
        if (strcmp(nickname, storedNick) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

// 사용자 등록
bool registerUser(const char* nickname, const char* password) {
    if (checkUserExists(nickname)) return false;
    char hashed[65];
    hashPassword(password, hashed);
    FILE* f = fopen(USER_FILE, "a");
    if (!f) return false;
    fprintf(f, "%s,%s\n", nickname, hashed);
    fclose(f);
    return true;
}

// 로그인 시 닉네임+비밀번호 해시 확인
bool loginUser(const char* nickname, const char* password) {
    char hashIn[65];
    hashPassword(password, hashIn);
    FILE* f = fopen(USER_FILE, "r");
    if (!f) return false;
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        char storedNick[MAX_NAME_LEN], storedHash[65];
        sscanf(line, "%[^,],%s", storedNick, storedHash);
        if (strcmp(nickname, storedNick) == 0 && strcmp(hashIn, storedHash) == 0) {
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

// 로그인
void promptLoginFlow(char* outNickname) {
    char name[MAX_NAME_LEN];
    char pass[MAX_PASS_LEN];

    while (1) {
        printf("닉네임을 입력하세요: ");
        scanf("%s", name);
        if (!checkUserExists(name)) {
            printf("존재하지 않는 사용자입니다. 다시 시도하거나 회원가입으로 돌아가세요.\n");
            continue;
        }
        break;
    }

    printf("비밀번호를 입력하세요: ");
    scanf("%s", pass);

    if (loginUser(name, pass)) {
        printf("로그인 성공!\n");
        strcpy(outNickname, name);
    } else {
        printf("로그인 실패. 비밀번호가 틀렸습니다.\n");
        strcpy(outNickname, "");
    }
}

// 회원가입
void promptRegisterFlow(char* outNickname) {
    char name[MAX_NAME_LEN];
    char pass[MAX_PASS_LEN];

    printf("사용할 닉네임을 입력하세요: ");
    scanf("%s", name);
    if (checkUserExists(name)) {
        printf("이미 존재하는 닉네임입니다.\n");
        strcpy(outNickname, "");
        return;
    }

    printf("비밀번호를 설정하세요: ");
    scanf("%s", pass);

    if (registerUser(name, pass)) {
        printf("회원가입 완료. 로그인 상태로 전환됩니다.\n");
        strcpy(outNickname, name);
    } else {
        printf("회원가입 실패.\n");
        strcpy(outNickname, "");
    }
}