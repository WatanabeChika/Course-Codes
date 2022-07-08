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
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;
    
		void renderBoards() const;
    
		void initializeGame();
    bool pauseGame() const;
    std::pair<std::vector<SnakeBody>, Direction> runGame();
    void returnGame() const;
    void renderCursor(WINDOW* win, int x, int y);
    void renderTitle(WINDOW* win, int x, int y);
    void renderPoints() const;
    void renderSpeed() const;
    void renderLives() const;
    void renderCoins() const;
    
		void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    void controlSnake(bool& con);
    
		void startGame();
    void mainMenu();
    int renderStartMenu();
    int renderDifficultySetting();
    bool renderRestartMenu() const;
    void adjustDelay();
    

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
    int mPoints = 0;
    int mSpeed = 0;
    int mDifficulty = 1;
    int mBaseDelay = 100;
    int mDelay;
    int mLives = 3;
    int total_coins;
    const std::string mRecordBoardFilePath = "recordpoints.dat";
    const std::string mRecordBoardFilePath2 = "recorddifficuly.dat";
    std::vector<int> mLeaderBoard;
    std::vector<int> mLeaderBoardDifficulty;
    const int mNumLeaders = 10;
};

#endif
