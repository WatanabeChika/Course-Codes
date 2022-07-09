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
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
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
    this->mLeaderBoardDifficulty.assign(this->mNumLeaders, -1);
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

    mvwprintw(this->mWindows[2], 9, 1, "Speed:");
    mvwprintw(this->mWindows[2], 11, 1, "Points:");
    mvwprintw(this->mWindows[2], 13, 1, "Lives:");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board 
    if (this->mScreenHeight - this->mInformationHeight - 17 - 2 < 3)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 17, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    std::string difficulty;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 17 - 2); i ++)
    {
        switch (this->mLeaderBoardDifficulty[i])
        {
        case 0: { difficulty = "Easy"; break;}
        case 1: { difficulty = "Medium"; break;}
        case 2: { difficulty = "Hard"; break;}
        default: { difficulty = ""; break;}
        }
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 17 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 17 + (i + 1), 5, pointString.c_str());
        mvwprintw(this->mWindows[2], 17 + (i + 1), 9, difficulty.c_str());
    }
    wrefresh(this->mWindows[2]);
}

void Game::renderCoins() const
{
    std::string coinsString = std::to_string(this->total_coins);
    mvwprintw(this->mWindows[2], 15, 1, coinsString.c_str());

    wrefresh(this->mWindows[2]);

}

void Game::renderCursor(WINDOW* win, int x, int y)
{   
    wattron(win, COLOR_PAIR(4));
    mvwaddch(win, x, y, this->mSnakeSymbol);
    wattroff(win, COLOR_PAIR(4));
    wattron(win, COLOR_PAIR(3));
    mvwaddch(win, x, y-1, this->mSnakeSymbol);
    mvwaddch(win, x, y-2, this->mSnakeSymbol);
    wattroff(win, COLOR_PAIR(3));
}

void Game::renderTitle(WINDOW* win, int y, int x)
{
    mvwprintw(win,y+0,x,"*     *     * ");
    mvwprintw(win,y+1,x," *   * *   *  ");
    mvwprintw(win,y+2,x,"  * *   * *   ");
    mvwprintw(win,y+3,x,"  * *   * *   ");
    mvwprintw(win,y+4,x,"   *     *    ");
    wattron(win,COLOR_PAIR(7));
    mvwprintw(win,y+0,x+15,"   #####   ");
    mvwprintw(win,y+1,x+15,"  #     #  "); 
    mvwprintw(win,y+2,x+15," #       # ");
    mvwprintw(win,y+3,x+15,"  #     #  ");
    mvwprintw(win,y+4,x+15,"   #####   "); 
    wattroff(win,COLOR_PAIR(7)); 
    mvwprintw(win,y+0,x+27,"  ********   ");
    mvwprintw(win,y+1,x+27,"  *      *   "); 
    mvwprintw(win,y+2,x+27,"  ********   ");
    mvwprintw(win,y+3,x+27,"  *   **     ");
    mvwprintw(win,y+4,x+27,"  *     **   ");
    mvwprintw(win,y+0,x+41,"   *     *   ");
    mvwprintw(win,y+1,x+41,"  * *   * *  ");
    mvwprintw(win,y+2,x+41,"  * *   * *  ");
    mvwprintw(win,y+3,x+41," *   * *   * ");
    mvwprintw(win,y+4,x+41,"*     *     *");
    wattron(win,COLOR_PAIR(4));
    mvwprintw(win,y+0,x+60,"@");
    wattroff(win,COLOR_PAIR(4));
    wattron(win,COLOR_PAIR(3));
    mvwprintw(win,y+0,x+61,"@@@@@@@@@");
    mvwprintw(win,y+1,x+60,"      @@@ ");
    mvwprintw(win,y+2,x+60,"    @@@   ");
    mvwprintw(win,y+3,x+60,"  @@@     ");
    mvwprintw(win,y+4,x+60,"@@@@@@@@@@");
    wattroff(win,COLOR_PAIR(3));
    mvwprintw(win,y+0,x+72,"   *****   ");
    mvwprintw(win,y+1,x+72,"  *     *  "); 
    mvwprintw(win,y+2,x+72," *       * ");
    mvwprintw(win,y+3,x+72,"  *     *  ");
    mvwprintw(win,y+4,x+72,"   *****   "); 
    mvwprintw(win,y+0,x+85," **       * ");
    mvwprintw(win,y+1,x+85," * **     * "); 
    mvwprintw(win,y+2,x+85," *   **   * ");
    mvwprintw(win,y+3,x+85," *     ** * ");
    mvwprintw(win,y+4,x+85," *       ** "); 
    mvwprintw(win,y+0,x+99," ******** ");
    mvwprintw(win,y+1,x+99," **       "); 
    mvwprintw(win,y+2,x+99," *****    ");
    mvwprintw(win,y+3,x+99," **       ");
    mvwprintw(win,y+4,x+99," ******** ");
}

int Game::renderStartMenu()
{
    WINDOW * menu;
    menu = newwin(this->mScreenHeight, this->mScreenWidth, 0, 0);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Start", "Difficulty", "Store", "Quit"};


    this->renderTitle(menu,this->mScreenHeight/7,this->mScreenWidth/2-54);
    this->renderCursor(menu, this->mScreenHeight/5*2+2, this->mScreenWidth/2-11);
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, this->mScreenHeight/5*2+2, this->mScreenWidth/2-9, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, this->mScreenHeight/5*2+4, this->mScreenWidth/2-9, menuItems[1].c_str());
    mvwprintw(menu, this->mScreenHeight/5*2+6, this->mScreenWidth/2-9, menuItems[2].c_str());
    mvwprintw(menu, this->mScreenHeight/5*2+8, this->mScreenWidth/2-9, menuItems[3].c_str());

    wrefresh(menu);

    int index = 0;
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
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-9, menuItems[index].c_str());
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-13, "   ");
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                this->renderCursor(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-11);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-9, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-9, menuItems[index].c_str());
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-13, "   ");
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                this->renderCursor(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-11);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index*2+this->mScreenHeight/5*2+2, this->mScreenWidth/2-9, menuItems[index].c_str());
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
    return index;
}

int Game::renderDifficultySetting()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Easy", "Medium (default)", "Hard", "Exit"};

    int index = 0;
    int offset = 3;
    mvwprintw(menu, 1, width/2-10, "Difficulty Setting");
    this->renderCursor(menu, 0 + offset, width/2-11);
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, width/2-9, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, width/2-9, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, width/2-9, menuItems[2].c_str());
    mvwprintw(menu, 3 + offset, width/2-9, menuItems[3].c_str());

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
                mvwprintw(menu, index + offset, width/2-9, menuItems[index].c_str());
                mvwprintw(menu, index + offset, width/2-13, "   ");
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                this->renderCursor(menu, index + offset, width/2-11);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, width/2-9, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, width/2-9, menuItems[index].c_str());
                mvwprintw(menu, index + offset, width/2-13, "   ");
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                this->renderCursor(menu, index + offset, width/2-11);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, width/2-9, menuItems[index].c_str());
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
    return index;
}

void Game::renderStoreMenu()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Body", "Head", "Exit"};

    int index = 0;
    int offset = 3;
    mvwprintw(menu, 1, width/2-6, "Body or Head?");
    this->renderCursor(menu, 0+offset, width/2-6);
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, width/2-4, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, width/2-4, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, width/2-4, menuItems[2].c_str());

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
                mvwprintw(menu, index + offset, width/2-4, menuItems[index].c_str());
                mvwprintw(menu, index + offset, width/2-8, "   ");
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                this->renderCursor(menu, index+offset, width/2-6);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, width/2-4, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, width/2-4, menuItems[index].c_str());
                mvwprintw(menu, index + offset, width/2-8, "   ");
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                this->renderCursor(menu, index+offset, width/2-6);
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, width/2-4, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if ((key == ' ' || key == 10) && index != 2)
        {
            this->renderStore(index);
            break;
        }
        else if ((key == ' ' || key == 10) && index == 2)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);
}

bool whetherPurchased(const int& k, const int& c)
{
    std::ifstream infile;
    if (k == 0)
        infile.open("bodyPurchase.txt");
    else if (k == 1)
        infile.open("headPurchase.txt");
    if (!infile)
    {
        std::ofstream ifile("bodyPurchase.txt"), ofile("headPurchase.txt");
        if (ifile) ifile << 5 ;
        if (ofile) ofile << 5 ;
        ifile.close();
        ofile.close();
        return false;
    }

    std::string message;
    getline(infile, message);
    infile.close();
    for (int i = 0; i < message.length(); i++)
    {
        if (message[i] == (c +'0'))
            return true;
    }
    return false;
}

void updatePurchase(const int& k, const int& c)
{
    std::ifstream ifile;
    if (k == 0){
        ifile.open("bodyPurchase.txt");
        if (!ifile) {
            //ifile.open("bodyPurchase.txt", std::ios::out);
            return;
        }
    }
    else if (k == 1){
        ifile.open("headPurchase.txt");
        if (!ifile) {
            //ifile.open("headPurchase.txt", std::ios::out);
            return;
        }
    }

    std::string message;
    getline(ifile, message);
    message += std::to_string(c);
    ifile.close();

    std::ofstream ofile;
    if (k == 0){
        ofile.open("bodyPurchase.txt");
    }
    else if (k == 1){
        ofile.open("headPurchase.txt");
    }
    if (!ofile)
    {
        return;
    }
    ofile << message;
    ofile.close();
}

void Game::renderStore(const int& k)
{
    WINDOW * store;
    store = newwin(this->mScreenHeight, this->mScreenWidth, 0, 0);
    box(store, 0, 0);

    for (int i = 2; i < this->mScreenWidth-1; i++)
        mvwprintw(store, mScreenHeight/3, i, "-");
    for (int i = 2; i <this->mScreenWidth-1; i++)
        mvwprintw(store, mScreenHeight/3*2, i, "-");

    std::vector<std::string> storeItems = {"Blue", "Cyan", "Green", "Magenta", "Red", "White", "Yellow", "Quit"};
    std::vector<std::pair<int, int>> itemsPosition;
    itemsPosition.push_back(std::make_pair(mScreenHeight/3-1, mScreenWidth/4-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight/3-1, mScreenWidth/4*3-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight/3*2-1, mScreenWidth/4-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight/3*2-1, mScreenWidth/4*3-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight-2, mScreenWidth/4-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight-2, mScreenWidth/2-4));
    itemsPosition.push_back(std::make_pair(mScreenHeight-2, mScreenWidth/4*3-4));
    itemsPosition.push_back(std::make_pair(1,mScreenWidth-10));

    for (int i = 0; i < storeItems.size(); i++)
    {
        if (i == 0)
        {
            this->renderCursor(store, itemsPosition[i].first, itemsPosition[i].second-2);
            wattron(store, A_STANDOUT);
            mvwprintw(store, itemsPosition[i].first, itemsPosition[i].second, storeItems[i].c_str());
            wattroff(store, A_STANDOUT);
        }
        else
            mvwprintw(store, itemsPosition[i].first, itemsPosition[i].second, storeItems[i].c_str());

        if (whetherPurchased(k, i))
            mvwprintw(store, itemsPosition[i].first, itemsPosition[i].second+1+storeItems[i].length(), "(Purchased)");
    }

    mvwprintw(store, 1, 2, "total coins: ");
    mvwprintw(store, 1, 15, std::to_string(total_coins).c_str());
    mvwprintw(store, 2, 2, "100$ per color");

    if (k == 0)
    {
        for (int i = 0; i < storeItems.size()-1; i++)
        {
            wattron(store, COLOR_PAIR(i+1));
            for (int j = 0; j < 5; j++)
            {
                mvwprintw(store, itemsPosition[i].first-2, itemsPosition[i].second+j, "@");
            }
            for (int j = 0; j < 5; j++)
            {
                mvwprintw(store, itemsPosition[i].first-2-j, itemsPosition[i].second+4, "@");
            }
            wattroff(store, COLOR_PAIR(i+1));
        }
    }
    else if (k == 1)
    {
        for (int i = 0; i < storeItems.size()-1; i++)
        {
            wattron(store, COLOR_PAIR(i+1));
            mvwprintw(store, itemsPosition[i].first-2, itemsPosition[i].second, "@");
            wattroff(store, COLOR_PAIR(i+1));
            for (int j = 1; j < 5; j++)
            {
                mvwprintw(store, itemsPosition[i].first-2, itemsPosition[i].second+j, "@");
            }
            for (int j = 0; j < 5; j++)
            {
                mvwprintw(store, itemsPosition[i].first-2-j, itemsPosition[i].second+4, "@");
            }

        }
    }
    wrefresh(store);

    int index = 0;
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
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second-4, "   ");
                if (index == 0)
                    index = 4;
                else if (index == 2)
                    index = 0;
                else if (index == 4)
                    index = 2;
                else if (index == 1)
                    index = 6;
                else if (index == 3)
                    index = 1;
                else if (index == 5 || index == 6)
                    index = 3;
                this->renderCursor(store, itemsPosition[index].first, itemsPosition[index].second-2);
                wattron(store, A_STANDOUT);
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                wattroff(store, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second-4, "   ");
                if (index == 0)
                    index = 2;
                else if (index == 2)
                    index = 4;
                else if (index == 4)
                    index = 0;
                else if (index == 1)
                    index = 3;
                else if (index == 3)
                    index = 6;
                else if (index == 5 || index == 6 || index == 7)
                    index = 1;
                this->renderCursor(store, itemsPosition[index].first, itemsPosition[index].second-2);
                wattron(store, A_STANDOUT);
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                wattroff(store, A_STANDOUT);
                break;
            }
            case 'A':
            case 'a':
            case KEY_LEFT:
            {
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second-4, "   ");
                if (index == 7) 
                    index = 1;
                else if (index == 0)
                    index = 6;
                else if (index == 2)
                    index = 7;
                else {
                    index --;
                    index = (index < 0) ? storeItems.size()-1 : index;
                }
                this->renderCursor(store, itemsPosition[index].first, itemsPosition[index].second-2);
                wattron(store, A_STANDOUT);
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                wattroff(store, A_STANDOUT);
                break;
            }
            case 'D':
            case 'd':
            case KEY_RIGHT:
            {
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second-4, "   ");
                if (index == 1) 
                    index = 7;
                else if (index == 7)
                    index = 2;
                else if (index == 6)
                    index = 0;
                else {
                    index ++;
                    index = (index < 0) ? storeItems.size()-1 : index;
                }
                this->renderCursor(store, itemsPosition[index].first, itemsPosition[index].second-2);
                wattron(store, A_STANDOUT);
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second, storeItems[index].c_str());
                wattroff(store, A_STANDOUT);
                break;
            }
        }
        wrefresh(store);

        if ((key == ' ' || key == 10) && index == storeItems.size()-1)
        {
            break;
        }
        else if ((key == ' ' || key == 10) && index != storeItems.size()-1 && whetherPurchased(k, index))
        {
            WINDOW * noteBoard = newwin(8, 40, mScreenHeight/3+2, mScreenWidth/2-17);
            box(noteBoard, 0, 0);
            if (k == 0)
            {
                snakeBodyColor = index+1;
                mvwprintw(noteBoard, 3, 3, "The Color of Snakebody has changed!");
                wrefresh(noteBoard);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                werase(noteBoard);
                wrefresh(noteBoard);
                wrefresh(store);
            }

            else if (k == 1)
            {
                snakeHeadColor = index+1;
                mvwprintw(noteBoard, 3, 3, "The Color of Snakehead has changed!");
                wrefresh(noteBoard);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                werase(noteBoard);
                wrefresh(noteBoard);
            }


        }
        else if ((key == ' ' || key == 10) && index != storeItems.size()-1 && !whetherPurchased(k, index))
        {
            if (total_coins >= 100)
            {
                total_coins -= 100;
                mvwprintw(store, itemsPosition[index].first, itemsPosition[index].second+1+storeItems[index].length(), "(Purchased)");
                mvwprintw(store, 1, 15, "      ");
                mvwprintw(store, 1, 15, std::to_string(total_coins).c_str());
                this->writeCoins();
                updatePurchase(k, index);
            }
            else
            {
                WINDOW * noteBoard = newwin(8, 40, mScreenHeight/3+2, mScreenWidth/2-17);
                box(noteBoard, 0, 0);
                mvwprintw(noteBoard, 3, 3, "Sorry, you don't have enough money.");
                wrefresh(noteBoard);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                werase(noteBoard);
                wrefresh(noteBoard);

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(store);
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
    std::vector<std::string> menuItems = {"Restart", "Back to Main Menu"};

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
        return true;
    else
        return false;
    
}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 11, 9, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderSpeed() const
{
    std::string speedString = std::to_string(this->mSpeed);
    mvwprintw(this->mWindows[2], 9, 8, speedString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderLives() const
{
    std::string livesString = std::to_string(this->mLives);
    mvwprintw(this->mWindows[2], 13, 8, livesString.c_str());
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
    wattron(mWindows[1], COLOR_PAIR(7));
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wattroff(mWindows[1], COLOR_PAIR(7));
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    
    wattron(mWindows[1], COLOR_PAIR(this->snakeHeadColor));
    mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
    wattroff(mWindows[1], COLOR_PAIR(this->snakeHeadColor));
    wattron(mWindows[1], COLOR_PAIR(this->snakeBodyColor));
    for (int i = 1; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wattroff(mWindows[1], COLOR_PAIR(this->snakeBodyColor));
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
        this->returnGame();
        return true;
    }
    else
    {
        return false;
    }
    
}

void Game::returnGame() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.25;
    int height = this->mGameBoardHeight * 0.25 + 3;
    int startX = this->mGameBoardWidth * 0.375;
    int startY = this->mGameBoardHeight * 0.375 + this->mInformationHeight - 2;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);

    std::string dot = "*";
    std::string space = " ";
    //std::string One = "1";
    mvwprintw(menu, 1, width/2 - 5, "// WARNING \\\\");
    for (int i = 0; i < 3; i++)
        mvwprintw(menu, 3, width/2 + i, dot.c_str());
    mvwprintw(menu, 4, width/2 + 2, dot.c_str());
    for (int i = 0; i < 3; i++)
        mvwprintw(menu, 5, width/2 + i, dot.c_str());
    mvwprintw(menu, 6, width/2, dot.c_str());
    for (int i = 0; i < 3; i++)
        mvwprintw(menu, 7, width/2 + i, dot.c_str());

    wrefresh(menu);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    for (int j = 0; j < 5; j++){
        for (int i = 0; i < 3; i++)
            mvwprintw(menu, 3 + j, width/2 + i, space.c_str());
    }

    for (int i = 0; i < 5; i++)
        mvwprintw(menu, 3 + i, width/2 + 2, dot.c_str());
    wrefresh(menu);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    delwin(menu);

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
    //this->renderCoins();
}


void Game::adjustDelay()
{
    this->mSpeed = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.95-0.1*this->mDifficulty, this->mSpeed) * (1-this->mDifficulty * 0.2);
    }
}

std::pair<std::vector<SnakeBody>, Direction> Game::runGame()
{
    bool moveSuccess, pauseContinue = true;
    std::pair<std::vector<SnakeBody>, Direction> oldData(this->mPtrSnake->getSnake(), this->mPtrSnake->getDireciton());
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
            mLives = 1;
            break;
        }
        werase(mWindows[1]);
        box(mWindows[1],0,0);
        moveSuccess = this->mPtrSnake->moveFoward();
        if (moveSuccess) {
            this->mPoints++;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            oldData.first = this->mPtrSnake->getSnake();
            oldData.second = this->mPtrSnake->getDireciton();
        }
        if (this->mPtrSnake->checkCollision())
            break;
        
        this->renderFood();
		this->renderSnake();
        this->renderSpeed();
        this->renderPoints();
        this->renderLives();
        
        this->adjustDelay();
		std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
    return oldData;
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
            std::pair<std::vector<SnakeBody>, Direction> oldSnake = this->runGame();
            --mLives;
            this->renderLives();
            if (mLives <= 0) break;
            this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength, oldSnake.first, oldSnake.second));
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->renderFood();
            this->renderSnake();
            this->returnGame();
        }
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        this->total_coins += this->mPoints;
        choice = this->renderRestartMenu();
        if (choice) {
            this->mLives = 5-(this->mDifficulty)*2;
        }
        else break;
    }
}

void Game::mainMenu()
{
    refresh();
    this->readCoins();
    while (true)
    {
        int i = this->renderStartMenu();
        this->mLives = 5-(this->mDifficulty)*2;
        if (i == 0) {
            this->startGame();
            this->writeCoins();
        }
        else if (i == 1) {
            int diffi_choice = this->renderDifficultySetting();
            if (diffi_choice < 3)
                this->mDifficulty = diffi_choice;
        }
        else if (i == 2)
            this->renderStoreMenu();
        else if (i == 3)
            break;
        else
            continue;
    }
}


// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    std::fstream fhand2(this->mRecordBoardFilePath2, fhand2.binary | fhand2.in);
    if (!fhand.is_open() || !fhand2.is_open())
    {
        return false;
    }
    int temp;
    int temp_diff;
    int i = 0;
    while ((!fhand.eof()) && (!fhand2.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        fhand2.read(reinterpret_cast<char*>(&temp_diff), sizeof(temp_diff));
        this->mLeaderBoard[i] = temp;
        this->mLeaderBoardDifficulty[i] = temp_diff;
        i ++;
    }
    fhand.close();
    fhand2.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    int newdiff = this->mDifficulty;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] > this->mPoints || (this->mLeaderBoard[i] == this->mPoints && this->mLeaderBoardDifficulty[i] >= this->mDifficulty))
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        int olddiff = this->mLeaderBoardDifficulty[i];
        this->mLeaderBoard[i] = newScore;
        this->mLeaderBoardDifficulty[i] = newdiff;
        newScore = oldScore;
        newdiff = olddiff;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    std::fstream fhand2(this->mRecordBoardFilePath2, fhand2.binary | fhand2.trunc | fhand2.out);
    if (!fhand.is_open() || !fhand2.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));
        fhand2.write(reinterpret_cast<char*>(&this->mLeaderBoardDifficulty[i]), sizeof(this->mLeaderBoardDifficulty[i]));
    }
    fhand.close();
    fhand2.close();
    return true;
}

bool Game::readCoins()
{
    std::ifstream fhand;
    fhand.open(mCoinsFilePath.c_str());
    if (!fhand.is_open())
    {
        return false;
    }
    std::string coins;
    getline(fhand, coins);
    this->total_coins = stoi(coins);
    fhand.close();
    return true;
}

bool Game::writeCoins()
{
    // trunc: clear the data file
    std::ofstream fhand;
    fhand.open(mCoinsFilePath.c_str());
    if (!fhand.is_open())
    {
        fhand.open(mCoinsFilePath.c_str(), std::ios::out);
        return false;
    }
    fhand << std::to_string(total_coins) << std::endl;
    fhand.close();
    return true;
}






