#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, std::vector<SnakeBody> Snake, Direction dir);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Checking API for generating random food
    bool isPartOfMountain(int x, int y);
    bool isPartOfSnake(int x, int y);
    bool isHead(int x, int y);
    void senseFood(SnakeBody food);
    void senseMount(std::vector<SnakeBody> mountain);
    bool touchFood();
    // Check if the snake is dead
    bool hitWall();
    bool hitSelf();
    bool hitMount();
    bool checkCollision();

    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    Direction getDireciton();
    SnakeBody createNewHead();
    bool moveFoward();

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    std::vector<SnakeBody> mountains;
    std::vector<SnakeBody> mSnake;
};

#endif
