#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"


SnakeBody::SnakeBody()
{
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
	// TODO overload the == operator for SnakeBody comparision.
    if (mX == snakeBody.getX() && mY == snakeBody.getY())
        return true;
    else
        return false;
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): 
mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, std::vector<SnakeBody> Snake, Direction dir):
mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength),
mSnake(Snake), mDirection(dir)
{
    this->setRandomSeed();
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
	// TODO check if a given point with axis x, y is on the body of the snake.
    for (int i = 1; i < this->mSnake.size(); ++i) {
        SnakeBody point = this->mSnake[i];
        if (point.getX() == x && point.getY() == y)
            return true;
        }
    return false;
}
bool Snake::isHead(int x, int y)
{
    SnakeBody head = this->mSnake[0];
    if (head.getX() == x && head.getY() == y)
        return true;
    else
        return false;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
	// TODO check if the snake has hit the wall
    SnakeBody head = this->mSnake[0];
    if (head.getX()<=0 || head.getX()>=this->mGameBoardWidth-1 || head.getY()<=0 || head.getY()>=this->mGameBoardHeight-1)
        return true;
    else
        return false;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
	// TODO check if the snake has hit itself.
    SnakeBody head = this->mSnake[0];
    return isPartOfSnake(head.getX(), head.getY());
}


bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

Direction Snake::getDireciton()
{
    return mDirection;
}

bool Snake::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
        case Direction::Up:
        {
			// what you need to do when the current direction of the snake is Up
			// and the user inputs a new direction? TODO
            if (newDirection == Direction::Up || newDirection == Direction::Down)
                return true;
            else {
                this->mDirection = newDirection;
                return true;
            }
        }
        case Direction::Down:
        {
			// what you need to do when the current direction of the snake is Down
			// and the user inputs a new direction? TODO
            if (newDirection == Direction::Up || newDirection == Direction::Down)
                return true;
            else {
                this->mDirection = newDirection;
                return true;
            }
        }
        case Direction::Left:
        {
			// what you need to do when the current direction of the snake is Left
			// and the user inputs a new direction? TODO
            if (newDirection == Direction::Left || newDirection == Direction::Right)
                return true;
            else {
                this->mDirection = newDirection;
                return true;
            }
        }
        case Direction::Right:
        {
			// what you need to do when the current direction of the snake is Right
			// and the user inputs a new direction? TODO
            if (newDirection == Direction::Left || newDirection == Direction::Right)
                return true;
            else {
                this->mDirection = newDirection;
                return true;
            }  
        }
    }

    return false;
}


SnakeBody Snake::createNewHead()
{
	/* TODO
	 * read the position of the current head
	 * read the current heading direction 
	 * add the new head according to the direction
	 * return the new snake
	 */
    SnakeBody oldHead = this->mSnake[0];
    int x,y;
    switch (this->mDirection)
    {
    case Direction::Up:
        x = oldHead.getX();
        y = oldHead.getY() - 1;
        break;
    
    case Direction::Down:
        x = oldHead.getX();
        y = oldHead.getY() + 1;
        break;

    case Direction::Left:
        x = oldHead.getX() - 1;
        y = oldHead.getY();
        break;

    case Direction::Right:
        x = oldHead.getX() + 1;
        y = oldHead.getY();
        break;
    }
    SnakeBody newHead(x,y);
    this->mSnake.insert(this->mSnake.begin(), newHead);
    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake::moveFoward()
{
    /* 
	 * TODO 
	 * move the snake forward. 
     * If eat food, return true, otherwise return false
     */

    if (touchFood()) {
        return true;
    }
    else {
        this->mSnake.pop_back();
        return false;
    }
}

bool Snake::checkCollision()
{
    if (this->hitWall() || this->hitSelf())
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Snake::getLength()
{
    return this->mSnake.size();
}

