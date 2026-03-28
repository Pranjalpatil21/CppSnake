#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
using namespace std;

class Point
{
private:
    int x , y;
public:
    Point(int x , int y)
        :x(x) , y(y) {}
    
    int getX() const{return x;}
    int getY() const{return y;}
    
    bool equals(const Point &obj) const
    {
        if(obj.getX() == this-> x && obj.getY() == this-> y)
            return true;
        return false;
    }
};

enum Direction {UP , DOWN , LEFT , RIGHT};
bool isValidChange(Direction newDir , Direction current)
{
    if((current == UP && newDir == DOWN ) || (current == DOWN && newDir == UP))
        return false;
    else if((current == RIGHT && newDir == LEFT ) || (current == LEFT && newDir == RIGHT))
        return false;
    else
        return true;
}

class Snake
{
private:
    Direction dir;
public:
    vector<Point> body;
    Snake()
    {
        body.push_back(Point(0,0));
        dir = RIGHT;
    }
    void move(bool grow)
    {
        for(int i = 1; i < body.size() ; i++)
        {
            body[i] = body[i - 1];
        }
        if(dir == UP)
            body[0] = Point(this->getHead().getX() -1, this->getHead().getY());
        else if(dir == DOWN)
            body[0] = Point(this-> getHead().getX() + 1  , this->getHead().getY());          //change when test done!
        else if(dir == RIGHT)
            body[0] = Point(this->getHead().getX() , this->getHead().getY() + 1);
        else if(dir == LEFT)
            body[0] = Point(this->getHead().getX() , this->getHead().getY() - 1);
        if(!grow && body.size() > 1)
        {
            body.pop_back();
        }
    }

    void setDirection(Direction newDir)
    {
        if(isValidChange(newDir , dir))
        {
            dir = newDir;
        }
    }

    Point getHead() const
    {
        return body[0];
    }

    vector<Point> &getBody() 
    {
        return this->body;
    }

    bool checkSelfCollision() const
    {
        bool check = false;
        for(int i = 1; i < body.size() ; i++)                  //clear till here
        {
            if(getHead().equals(body.at(i)))
                check = true;
        }
        return check;
    }
};
class Food
{
private:
    Point position;
public:
    Food(int width , int height)
        :position(width , height) {}

    Point getPosition() const
    {
        return position;
    }
    void generate(int width , int height , vector<Point> & snakeBody)
    {
        bool distinct = false;
        while(!distinct)
        {
            distinct = true;
            width = rand() % 10;
            height = rand() % 10;
            for(int i = 0 ; i < snakeBody.size() ; i++)
            {
                if(Point(width , height).equals(snakeBody.at(i)))
                {
                    distinct = false;
                    break;
                }
            }
        }
        this-> position = Point(width , height);
    }
};

class Game
{
private:
    Snake snake;
    Food food;

    int width;
    int height;

    bool gameOver;
    int score;

public:
    Game()
        :width(10) , height(10) , gameOver(false) , score(0) , food(10 , 10)    {}
    
    void input()
    {
        char ch = ' ';

        if(_kbhit())
        {
            ch = _getch();
            if(ch == 'w' || ch == 'W')
                snake.setDirection(UP);
            else if(ch == 'a' || ch == 'A')
                snake.setDirection(LEFT);
            else if(ch == 's' || ch == 'S')
                snake.setDirection(DOWN);
            else if(ch == 'd' || ch == 'D')
                snake.setDirection(RIGHT);
        }
    }

    void update()
    {
        bool atefood = (food.getPosition().equals(snake.getHead()));

        snake.move(atefood);
        if(atefood)
        {
            score += 10;
            food.generate(10,10,snake.body);
        }

        if(checkWallCollision())
        {
            gameOver = true;
        }
        if(snake.checkSelfCollision())
        {
            gameOver = true;
        }
    }

    void render()
    {
        system("cls");  //clears screen
        for(int i = 0 ; i < 10 ; i++)
        {
            cout<<"===";
        }
        cout<<endl;
        for(int i = 0; i < 10; i++)
        {
            cout<<"|";
            for(int j = 0 ; j < 10 ; j++)
            {
                bool printed = false;
                if(Point(i , j).equals(snake.getHead()))
                {
                    cout<<" # ";
                    printed = true;
                }
                if(Point(i , j).equals(food.getPosition()))
                {
                    cout<<" * ";
                    printed = true;
                }
                for(int k = 1; k < snake.getBody().size() ; k++)
                {
                    if(Point(i , j).equals(snake.getBody().at(k)))  //clear
                    {
                        cout<<" 0 ";
                        printed = true;
                    }
                }
                if(!printed)
                    cout<<" . ";
            }
            cout<<"|\n";
        }
        cout<<endl;
        for(int i = 0 ; i < 10 ; i++)
        {
            cout<<"===";
        }
        cout<<endl;
        cout<<"Score : "<<score<<endl;
    }

    bool checkWallCollision() const
    {
        if(snake.getHead().getX() >= 10 || snake.getHead().getX() < 0)
            return true;
        if(snake.getHead().getY() >= 10 || snake.getHead().getY() < 0)
            return true;
        return false;
    }

    void run()
    {
        food.generate(10 , 10 , snake.body);
        render();
        while(true)
        {
            input();
            update();
            if(gameOver)
            {
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(400));
            render();
        }
        cout<<"Game Over :(\n";
    }
};
int main()
{
    srand(time(0));
    Game gojo;
    gojo.run();
    return 0;
}