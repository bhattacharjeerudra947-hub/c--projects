#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
#include <ctime>




void showConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;   // true = visible, false = hide
    SetConsoleCursorInfo(out, &cursorInfo);
}


using namespace std;
HANDLE hBuffer[2];
int currentBuffer =0;
const int UI_HEIGHT = 3;


#define MAX_LENGTH 1000
//DIRECTION
const char d_up= 'w';
const char d_down='s';
const char d_right='d';
const char d_left='a';
int con_w,con_h;
void initscreen(){
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    con_h=csbi.srWindow.Bottom-csbi.srWindow.Top+1;
    con_w=csbi.srWindow.Right-csbi.srWindow.Left+1;
}

struct point{
    int xcoord;
    int ycoord;
    point(){
    }
    point(int x,int y)
    {
        xcoord=x;
        ycoord=y;
    }
};
class Snake{
    int length;
    char direction;
    public:
        point body[MAX_LENGTH];
        Snake(int x , int y)
        {
            length=1;
            body[0]=point(x,y);
            direction =d_right;
        }
   
    int getLength(){
        return length;
    }
    void changeDirection(char newDirection){
        if(newDirection == d_up && direction != d_down)
        {
            direction=newDirection;
        }
        else if(newDirection == d_down && direction!= d_up){
            direction=newDirection; 
        }
        else if(newDirection==d_right && direction !=d_left){
            direction=newDirection;
        }
        else if(newDirection==d_left && direction !=d_right){
            direction=newDirection;
        }
    }
    bool move(point food){
        point prevTail = body[length - 1];

        for(int i = length - 1; i > 0; i--){
        body[i] = body[i - 1];
        }

        switch(direction){
            case d_up:    body[0].ycoord--; break;
            case d_down:  body[0].ycoord++; break;
            case d_right: body[0].xcoord++; break;
            case d_left:  body[0].xcoord--; break;
        }
         // 👉 WALL COLLISION (HERE)
        if(body[0].xcoord < 0 || body[0].xcoord >= con_w ||body[0].ycoord < UI_HEIGHT || body[0].ycoord >= con_h)return false;


        // self collision
        for(int i = 1; i < length; i++){
            if(body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord) return false;
        }
        

        if(food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord){
            body[length] = prevTail;
            length++;
        }

    return true;
    }


};
class Board{
    const int BAR_WIDTH = 20;

    Snake *snake;
    const char SNAKE_BODY = 'X';
    //point smfood;
    //point spfood;
    point food;
    const char FOOD= 'o';
    int score;
    bool speedboost;
    bool cooldown;
    clock_t boostStartTime;
    clock_t cooldownStartTime;
    const int normal_speed =140;
    const int boost_speed=40;
    const int boost_time=5;
    const int cooldown_time=60;
    public:
    void drawUI(){
    // top border
        for(int x = 0; x < con_w; x++){
            gotoxy(x, 0); cout << "-";
        }

    // middle content
        gotoxy(1,1);
        cout << "Score: " << score;

        drawBoostBar();

    // bottom border
        for(int x = 0; x < con_w; x++){
            gotoxy(x, UI_HEIGHT - 1); cout << "-";
        }
    }


    void drawBoostBar(){
        gotoxy(con_w - BAR_WIDTH - 12, 1);
   // below score line

        cout << "BOOST: [";

        double percent = 1.0;   // 0 → empty, 1 → full

    // If boost is active → bar decreases
        if(speedboost){
            double elapsed =
            double(clock() - boostStartTime) / CLOCKS_PER_SEC;
            percent = 1.0 - (elapsed / boost_time);
            if(percent < 0) percent = 0;
        }
    // If cooldown is active → bar increases
        else if(cooldown){
            double elapsed =
            double(clock() - cooldownStartTime) / CLOCKS_PER_SEC;
            percent = elapsed / cooldown_time;
            if(percent > 1) percent = 1;
        }

        int filled = int(percent * BAR_WIDTH);

        for(int i = 0; i < BAR_WIDTH; i++){
            if(i < filled) cout << char(219);   // █ block
            else cout << " ";
        }

        cout << "]";
    }
    void gotoxy(int x,int y){
        COORD coord;
        coord.X=x;
        coord.Y=y;
        SetConsoleCursorPosition(hBuffer[currentBuffer],coord);

    }
    

    
    Board(){
       snake = new Snake(10, UI_HEIGHT + 2);

        score = 0;
        speedboost = false;
        cooldown=false;

    }
    ~Board(){
        delete snake;
    }
    int getscore(){
        return score;
    }
    void spawnFood(){
        while(true){
            int x = rand() % (con_w - 2);
            int y = rand() % (con_h - UI_HEIGHT - 1) + UI_HEIGHT;

            bool onSnake = false;

            for(int i = 0; i < snake->getLength(); i++){
                if(snake->body[i].xcoord == x && snake->body[i].ycoord == y){
                    onSnake = true;
                    break;
                }
            }

            if(!onSnake){
                food = point(x,y);
                break;
            }
        }
    }
    int getCurrentSpeed(){

    // BOOST ACTIVE
        if(speedboost){
            double boostElapsed = double(clock() - boostStartTime) / CLOCKS_PER_SEC;

            if(boostElapsed >= boost_time){
                speedboost = false;
                cooldown = true;
                cooldownStartTime = clock();   // start cooldown
                return normal_speed;
            }
            return boost_speed;
        }

    // COOLDOWN ACTIVE
        if(cooldown){
            double cooldownElapsed = double(clock() - cooldownStartTime) / CLOCKS_PER_SEC;

            if(cooldownElapsed >= cooldown_time){
                cooldown = false;  // boost available again
            }
        }

        return normal_speed;
    }
    void displaycurr_score(){
        gotoxy(con_w/2,0);
        cout<<"score" <<score;
    }
    void draw(){
        int back = 1 - currentBuffer;   // 🔁 back buffer index

    // clear BACK buffer
        DWORD written;
        COORD topLeft = {0, 0};
        FillConsoleOutputCharacter(
            hBuffer[back],
            ' ',
            con_w * con_h,
            topLeft,
            &written
        );

    // draw UI on BACK buffer
        COORD coord;

    // top border
        for(int x = 0; x < con_w; x++){
            coord = { (SHORT)x, 0 };
            SetConsoleCursorPosition(hBuffer[back], coord);
            cout << "-";
        }

        coord = {1,1};
        SetConsoleCursorPosition(hBuffer[back], coord);
        cout << "Score: " << score;

    // boost bar
        coord = {(SHORT)(con_w - BAR_WIDTH - 12), 1};
        SetConsoleCursorPosition(hBuffer[back], coord);
        cout << "BOOST: [";

        double percent = 1.0;
        if(speedboost){
            percent = 1.0 - double(clock() - boostStartTime) / CLOCKS_PER_SEC / boost_time;
        } 
        else if(cooldown){
            percent = double(clock() - cooldownStartTime) / CLOCKS_PER_SEC / cooldown_time;
        }
        percent = max(0.0, min(1.0, percent));

        int filled = int(percent * BAR_WIDTH);
        for(int i = 0; i < BAR_WIDTH; i++){
            cout << (i < filled ? char(219) : ' ');
            cout << "]";
        }

    // bottom UI border
        for(int x = 0; x < con_w; x++){
            coord = { (SHORT)x, UI_HEIGHT - 1 };
            SetConsoleCursorPosition(hBuffer[back], coord);
            cout << "-";
        }

    // draw snake
        for(int i = 0; i < snake->getLength(); i++){
            coord = {
                (SHORT)snake->body[i].xcoord,
                (SHORT)snake->body[i].ycoord
            };
            SetConsoleCursorPosition(hBuffer[back], coord);
            cout << SNAKE_BODY;
        }

    // draw food
        coord = {(SHORT)food.xcoord, (SHORT)food.ycoord};
        SetConsoleCursorPosition(hBuffer[back], coord);
        cout << FOOD;

    // 🔥 SHOW BACK BUFFER
        SetConsoleActiveScreenBuffer(hBuffer[back]);
        currentBuffer = back;   // swap
    }


    bool update(){
         bool isAlive = snake->move(food);
       if(isAlive == false)
       {
           return false;
       }

        if(food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord)
        {
            score++;
            spawnFood();
        }
       return true;
    }
    void getInput(){
        if(kbhit())
        {
            int key = getch();
            if(key == 'w' || key == 'W')
            {
                snake->changeDirection(d_up);
            }
            else if(key == 'a' || key == 'A')
            {
                snake->changeDirection(d_left);
            }
            else if(key == 's' || key == 'S')
            { 
                snake->changeDirection(d_down);
            }
            else if(key == 'd' || key == 'D')
            {
                snake->changeDirection(d_right);
            }
           else if(key == ' ' ){
                if(!speedboost && !cooldown){
                    speedboost=true;
                    boostStartTime=clock();

                }
            }
      
        }
    }
    void showGameOver(){
        HANDLE hConsole = hBuffer[currentBuffer];

   
    // Set bold red color
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED | FOREGROUND_INTENSITY
        );

        string msg = "GAME OVER";
        string scoreMsg = "FINAL SCORE : " + to_string(score);

        int x1 = (con_w - msg.length()) / 2;
        int y1 = con_h / 2;

        int x2 = (con_w - scoreMsg.length()) / 2;
        int y2 = y1 + 2;

        gotoxy(x1, y1);
        cout << msg;

        gotoxy(x2, y2);
        cout << scoreMsg;

    // Reset color back to normal
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
        );
    }

};
int main(){
    showConsoleCursor(false);
    srand(time(0));
    initscreen();
    hBuffer[0] = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    hBuffer[1] = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    SetConsoleActiveScreenBuffer(hBuffer[0]);

   
    Board *board = new Board();
    board->spawnFood();

    while(true){
        board->getInput();
        if(!board->update()){
            board->showGameOver();
            break;
        }
        board->draw();
        Sleep(board->getCurrentSpeed());
    }

    delete board;
    return 0;
}

