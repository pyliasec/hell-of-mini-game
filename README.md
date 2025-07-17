# Hell of Mini Game

A collection of casino-style mini games written in C, featuring player authentication, betting systems, and comprehensive record keeping.

## 📋 필수 기능 구현 현황

### ✅ 필수 기능
- **구조체 사용**: Player, RouletteGame, HighScoreRecord, SlotResult 등 다양한 구조체 활용
- **동적 메모리 사용**: SlotResult 구조체의 동적 할당/해제 (malloc, free)
- **데이터 입력/출력**: 사용자 입력 처리, 게임 결과 출력, 메뉴 시스템
- **정렬 기능**: 하이스코어 보드에서 버블 정렬로 획득 금액 기준 정렬
- **검색 기능**: 사용자 인증 시스템에서 닉네임 검색, 게임별 기록 검색
- **메뉴 제공**: 각 게임별 상세 메뉴 시스템 및 네비게이션

### ✅ 부가 기능 (가산점)
- **파일 기반 데이터 관리**: CSV 파일을 통한 모든 데이터 읽기/쓰기 처리
- **보안**: SHA256 해시를 이용한 패스워드 암호화
- **페이지네이션**: 하이스코어 보드의 페이지 단위 데이터 표시

## Project Structure

```
hell_of_mini_game/
├── src/               # Source code files
├── include/           # Header files
├── data/              # CSV data files (player records, game history)
├── build/             # Compiled objects and executable
├── Makefile           # Build configuration
└── README.md          # This file
```

## Games Included

1. **Slot Machine** - Three-reel slot machine with weighted outcomes
2. **Horse Racing** - Multi-horse racing simulation with betting
3. **Roulette** - Color and number betting roulette game
4. **Rock Paper Scissors** - Classic game with betting system

## Features

- **Player Authentication**: Secure login/registration with password hashing
- **Coin System**: Virtual currency for betting across all games
- **Record Keeping**: All game results saved to CSV files
- **High Score Board**: View records across all games with pagination
- **Menu System**: Easy navigation between games and features

## Building and Running

### Prerequisites
- GCC compiler
- OpenSSL library (for password hashing)

### Installation
```bash
# Install dependencies (macOS with Homebrew)
make install-deps

# Build the project
make

# Run the game
make run
```

### Manual Build
```bash
# Compile all source files
gcc -Wall -Wextra -std=c99 -Iinclude src/*.c -o build/hellgame -lssl -lcrypto

# Run
cd build && ./hellgame
```

## Game Instructions

### Getting Started
1. Register a new account or login with existing credentials
2. Start with 1000 coins (default for new players)
3. Choose a game from the main menu
4. Place your bets and play!

### Slot Machine
- Bet coins on three spinning reels
- Match symbols for different payouts
- Special combinations offer bonus multipliers

### Horse Racing
- Choose from multiple horses
- Place bets on your favorite
- Watch the race simulation unfold

### Roulette
- Bet on colors (red/black) or specific numbers
- Color bets pay 2:1, number bets pay 36:1
- Simple and classic casino gameplay

### Rock Paper Scissors
- Play against the computer
- Bet coins on the outcome
- Best of luck strategy game

## Data Files

The game automatically creates and manages these CSV files in the `data/` directory:
- `users.csv` - Player authentication data
- `players.csv` - Player profiles and coin balances
- `slot_records.csv` - Slot machine game history
- `horse_records.csv` - Horse racing results
- `roulette_history.csv` - Roulette game logs
- `rock_records.csv` - Rock paper scissors history

## Development

### Code Organization
- **Authentication**: `auth.c/h` - User registration and login
- **Player Management**: `player.c/h` - Player data and persistence
- **Games**: Individual `.c/.h` files for each game
- **High Scores**: `highscore.c/h` - Record viewing and pagination
- **Main**: `main.c` - Menu system and game launcher

### Adding New Games
1. Create new `.c` and `.h` files in respective directories
2. Include the header in `main.c`
3. Add menu option in `showMenu()` function
4. Implement game logic with player integration
5. Add record saving functionality

## Makefile Targets

- `make` or `make all` - Build the project
- `make clean` - Remove object files and executable
- `make cleanall` - Remove all generated files including data
- `make run` - Build and run the game
- `make install-deps` - Install required dependencies (macOS)

## License

This project is for educational purposes. Feel free to modify and distribute.
