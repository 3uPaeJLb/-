#include <SFML/Graphics.hpp>
#include<list>

using namespace sf;
using namespace std;

struct snake_part
{
    int x;
    int y;
};

int window_hight = 800;
int window_width = 1000;
int loop_pause = 200;
bool dead = false;

int snake_part_size = 50;
list<snake_part> snake = { {8, 8},{9, 8},{10, 8} };
int snake_direction = 3;
Color snake_color = Color::Color(70, 115, 232);

Color BG_color = Color::Color(170, 215, 81);

Color food_color = Color::Color(231, 71, 29);
int food_x = 6;
int food_y = 10;

void snake_draw(int snake_part_size,Color snake_color, RenderWindow& window)
{    
    for (auto s : snake)
    {
        RectangleShape part;
        part.setPosition(s.x * snake_part_size, s.y * snake_part_size);
        part.setSize(Vector2f(snake_part_size, snake_part_size));
        part.setFillColor(snake_color);
        window.draw(part);
    }
};

void food_draw(int snake_part_size, Color snake_color, RenderWindow& window)
{
    for (auto s : snake)
    {
        RectangleShape food_shape;
        food_shape.setPosition(food_x * snake_part_size, food_y * snake_part_size);
        food_shape.setFillColor(food_color);
        food_shape.setSize(Vector2f(snake_part_size, snake_part_size));
        window.draw(food_shape);
    }
};
int main()
{
    RenderWindow window(VideoMode(window_width, window_hight), "Snake Game!");
   
    while (window.isOpen())
    { 
        //timer
        sleep(Time(milliseconds(loop_pause)));

        Event event;
        while (window.pollEvent(event)) // nessesery to close the window 
        {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case Keyboard::W :
                    if (snake_direction != 1)
                        snake_direction = 0;
                    break;
                
                case Keyboard::A :
                    if (snake_direction != 2)
                        snake_direction = 3;
                    break;

                case Keyboard::S :
                    if (snake_direction != 0)
                        snake_direction = 1;
                    break;

                case Keyboard::D:
                    if (snake_direction != 3)
                        snake_direction = 2;
                    break;
                }
            }
        }

        if (!dead)
        {

            window.clear(BG_color);

            // change snake position
            switch (snake_direction)
            {
            case 0: // UP
                snake.push_front({ snake.front().x , snake.front().y - 1 });

                if ((snake.front().y - 1) * snake_part_size == -100)
                    snake.push_front({ snake.front().x , 750 / 50 });

                break;

            case 1: // DOWN
                snake.push_front({ snake.front().x , snake.front().y + 1 });

                if ((snake.front().y + 1) * snake_part_size == 850)
                    snake.push_front({ snake.front().x , 0 });
                break;

            case 2: // RIGHT
                snake.push_front({ snake.front().x + 1, snake.front().y });
                if ((snake.front().x + 1) * snake_part_size == 1050)
                    snake.push_front({ 0, snake.front().y });
                break;

            case 3: // LEFT
                snake.push_front({ snake.front().x - 1, snake.front().y });
                if ((snake.front().x - 1) * snake_part_size == -100)
                    snake.push_front({ 950 / 50, snake.front().y });
                break;
            }



            // food detection
            bool newFoodPosotionFound = false;

            if (snake.front().x == food_x && snake.front().y == food_y)
            {
                while (!newFoodPosotionFound) // пока новое место для еды не найдено 
                {
                    food_x = rand() % (window_width / 50);
                    food_y = rand() % (window_hight / 50);

                    if (snake.front().x != food_x && snake.front().y != food_y)
                        newFoodPosotionFound = true;

                    for (auto s : snake)
                    {
                        if (s.x != food_x && s.y != food_y && newFoodPosotionFound)
                            newFoodPosotionFound = true;
                    }
                }

                snake.push_back({ snake.back().x, snake.back().y });

                if (loop_pause >= 100)
                    loop_pause -= 10;
            }

            for (list<snake_part>::iterator i = snake.begin(); i != snake.end(); i++)
            {
                if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
                    dead = true;
            }

            //destroy tail part
            snake.pop_back();

            //food draw
            food_draw(snake_part_size, snake_color, window);

            //snake draw
            snake_draw(snake_part_size, snake_color, window);
        }

        window.display();
    }

    return 0;
}