#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <string>
#include <vector>
#include <memory>

#include "snake.h"


class Game
{
public:
    Game();
    ~Game();
    
		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;
    
		void createInstructionBoard();
    void renderInstructionBoard() const;
		
		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard(const int& k);
    bool updateLeaderBoard();
    bool writeLeaderBoard(const int& k);
    void renderLeaderBoard() const;
    
		void renderBoards(const int& k) const;
    
		void initializeGame(const bool& handicapmode);
    bool pauseGame() const;
    std::pair<std::vector<SnakeBody>, Direction> runGame(const int& k, const bool& handicapmode);
    void returnGame() const;
    void renderCursor(WINDOW* win, int x, int y);
    void renderTitle(WINDOW* win, int x, int y);
    void renderPoints() const;
    void renderSpeed() const;
    void renderLives() const;
    
		void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    void controlSnake(bool& con);

    void renderMountain() const;
    void createRamdonMount();
    
		void startGame(const int& k, const bool& handicapMode);
    void mainMenu();
    int renderStartMenu();
    int renderDifficultySetting();
    bool renderRestartMenu() const;
    void adjustDelay();

    void renderCoins() const;
    bool readCoins();
    bool writeCoins();
    void renderStore(const int& i);
    void renderStoreMenu();
    int renderModeSelection();
    

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';
    // Mountain information
    std::vector<SnakeBody> mountains;
    const char mountSymbol_1 = '/';
    const char mountSymbol_2 = '\\';
    int mPoints = 0;
    int mSpeed = 0;
    int mDifficulty = 1;
    int mBaseDelay = 100;
    int mDelay;
    int mLives = 3;
    int total_coins = 0;
    int snakeBodyColor = 6;
    int snakeHeadColor = 6;
    const std::string mRecordBoardFilePathForInfinite = "recordpoints_infinite.dat";
    const std::string mRecordBoardFilePathForCounting = "recordpoints_counting.dat";
    const std::string mRecordBoardFilePathForInfinite2 = "recorddifficuly_infinite.dat";
    const std::string mRecordBoardFilePathForCounting2 = "recorddifficuly_counting.dat";
    const std::string mCoinsFilePath = "coins.txt";
    std::vector<int> mLeaderBoard;
    std::vector<int> mLeaderBoardDifficulty;
    const int mNumLeaders = 10;
};

#endif
