#include <string>
#include <iostream>
#include <cmath> 

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm> 

#include "game.h"

Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    start_color();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "It is crazily fast in the vertical direction.");
    mvwprintw(this->mWindows[0], 3, 1, "It would be much better if there are more lives and foods for the snake.");
    mvwprintw(this->mWindows[0], 4, 1, "Complicated windows are my next steps!");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");
    mvwprintw(this->mWindows[2], 7, 1, "Pause: SPACE");

    mvwprintw(this->mWindows[2], 9, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 12, 1, "Points");
    mvwprintw(this->mWindows[2], 15, 1, "Lives");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board 
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 18, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 18 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 18 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 13, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 10, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderLives() const
{
    std::string livesString = std::to_string(this->mLives);
    mvwprintw(this->mWindows[2], 16, 1, livesString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::initializeGame()
{
    // allocate memory for a new snake
		this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));

    /* TODO 
     * initialize the game pionts as zero
     * create a food at randome place
     * make the snake aware of the food
     * other initializations
     */
    this->mPoints = 0;
    createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    renderFood();
    renderSnake();
}

void Game::createRamdonFood()
{
    /* TODO 
    * create a food at random places
    * make sure that the food doesn't overlap with the snake.
    */
    int x,y;
    do {
        this->mPtrSnake->setRandomSeed();
        x = int(std::rand() % (this->mGameBoardWidth-2)) +1;
        y = int(std::rand() % (this->mGameBoardHeight-2))+1; 
    }
    while (this->mPtrSnake->isPartOfSnake(x,y) || this->mPtrSnake->isHead(x,y));
    SnakeBody tempFood(x,y);
    this->mFood = tempFood;
}

void Game::renderFood() const
{
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    wattron(mWindows[1], COLOR_PAIR(1));
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wattroff(mWindows[1], COLOR_PAIR(1));
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    wattron(mWindows[1], COLOR_PAIR(3));
    mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
    wattroff(mWindows[1], COLOR_PAIR(3));
    wattron(mWindows[1], COLOR_PAIR(2));
    for (int i = 1; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wattroff(mWindows[1], COLOR_PAIR(2));
    wrefresh(this->mWindows[1]);
}

std::string pauseWord()
{
    std::srand(std::time(nullptr));
    int Chika = rand();
    switch (Chika%4)
    {
    case 0: return "This snake goes crazily fast on the up-down side!";    
    case 1: return "What's your best score in this game? Ehh?";
    case 2: return "I love Watanabe You in Lovelive the most!";
    default: return "Many things should be improved......";
    }
}

bool Game::pauseGame() const
{
    
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    std::vector<std::string> menuItems = {"Continue", "End"};

    int index = 0;
    int offset = 6;
    mvwprintw(menu, 1, width/2 - 5, "// PAUSE \\\\");
    mvwprintw(menu, 3, 1, pauseWord().c_str());
    box(menu, 0, 0);
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void Game::controlSnake(bool& con) 
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
		    // TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
			// TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
			// TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
			// TODO change the direction of the snake.
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case 'P':
        case 'p':
        case ' ':
        {
            con = this->pauseGame();
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

std::vector<SnakeBody> Game::runGame()
{
    bool moveSuccess, pauseContinue = true;
    std::vector<SnakeBody> oldSnake = this->mPtrSnake->getSnake();
    while (true)
    {
		/* TODO 
		 * this is the main control loop of the game.
		 * it keeps running a while loop, and does the following things:
		 * 	1. process your keyboard input
		 * 	2. clear the window
		 * 	3. move the current snake forward
		 * 	4. check if the snake has eaten the food after movement
		 * 	5. check if the snake dies after the movement
		 * 	6. make corresponding steps for the ``if conditions'' in 3 and 4.
		 *  7. render the position of the food and snake in the new frame of window. 
		 *  8. update other game states and refresh the window
		 */

        this->controlSnake(pauseContinue);
        if (!pauseContinue) {
            mLives = 0;
            break;
        }
        werase(mWindows[1]);
        box(mWindows[1],0,0);
        moveSuccess = this->mPtrSnake->moveFoward();
        if (moveSuccess) {
            this->mPoints++;
            createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            oldSnake = this->mPtrSnake->getSnake();
        }
        if (this->mPtrSnake->checkCollision())
            break;
        
        this->renderFood();
		this->renderSnake();
        this->renderDifficulty();
        this->renderPoints();
        this->renderLives();
        
        this->adjustDelay();
		std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
    return oldSnake;
}

void Game::startGame()
{
    refresh();
    bool choice;
    while (true)
    {
        this->readLeaderBoard();
        this->renderBoards();
        this->initializeGame();
        while (true) {   
            std::vector<SnakeBody> oldSnake = this->runGame();
            --mLives;
            this->renderLives();
            if (mLives <= 0) break;
            this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength, oldSnake, this->mPtrSnake->getDireciton()));
            createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            renderFood();
            renderSnake();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        choice = this->renderRestartMenu();
        if (choice) mLives = 3;
        else break;
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}








