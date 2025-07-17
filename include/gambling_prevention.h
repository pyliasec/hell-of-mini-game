/*
 * ============================================================================
 * 파일명: gambling_prevention.h
 * 설명: 도박 중독 예방 및 교육 시스템
 * 작성자: pyliasec
 * 작성일: 2025-07-17
 * 
 * 주요 기능:
 * - 도박 중독 예방 메시지 표시
 * - 큰 손실 시 경고 시스템
 * - 교육적 정보 제공
 * ============================================================================
 */

#ifndef GAMBLING_PREVENTION_H
#define GAMBLING_PREVENTION_H

#include <stdbool.h>

// 도박 예방 관련 상수
#define MAJOR_LOSS_THRESHOLD 50000      // 큰 손실로 간주할 기준 (5만 코인)
#define BANKRUPTCY_THRESHOLD 1000       // 파산으로 간주할 기준 (1천 코인 미만)
#define CONSECUTIVE_LOSS_LIMIT 5        // 연속 손실 경고 기준

// 함수 프로토타입
void showProjectPurpose(void);
void showGamblingPreventionMessage(void);
void checkMajorLoss(int previousCoins, int currentCoins, const char* nickname);
void checkBankruptcy(int coins, const char* nickname);
void showGamblingRisks(void);
void showHealthyGamingTips(void);
bool askContinueAfterWarning(void);

#endif // GAMBLING_PREVENTION_H
