#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
void showConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;   // true = visible, false = hide
    SetConsoleCursorInfo(out, &cursorInfo);
}
using namespace std;
const int UI_HEIGHT = 3;
void play_bgm(){
    PlaySound(
        TEXT("C:\\Users\\bhatt\OneDrive\Desktop\c++ project\c-projects\snake_game"),        // play this file
        NULL,               // it is not inside program
        SND_FILENAME |         // it's a file
        SND_ASYNC |            // play in background
        SND_LOOP               // keep repeating
    );
    
}
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
void play_eat(){
    PlaySound(
        TEXT("mixkit-winning-a-coin-video-game-2069.wav"),
        nullptr,
        SND_FILENAME | SND_ASYNC
    );
    Sleep(150);
    play_bgm();

}
void play_bonus(){
    PlaySound(TEXT("mixkit-game-bonus-reached-2065.wav"),nullptr,SND_FILENAME | SND_ASYNC);
    play_bgm();
}
void play_crash(){
    PlaySound(
        TEXT("mixkit-player-losing-or-failing-2042.wav"),
        nullptr,
        SND_FILENAME | SND_ASYNC

    );
    play_bgm();
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
        if(body[0].ycoord < UI_HEIGHT || body[0].ycoord >= con_h){
            play_crash();
            return false;
        } 
        // self collision
        for(int i = 1; i < length; i++){
            if(body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord){
                play_crash();
                return false;
            }
        }
        if(food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord){
            body[length] = prevTail;
            length++;
        }
    return true;
    }
};
class Board{
    bool bonusSoundPlayed=false;
    const int BAR_WIDTH = 20;
    Snake *snake;
    const char SNAKE_BODY = 'X';
    //point smfood;
    //point spfood;
    point food;
    const char FOOD= 'o';
    int temp_score;
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
        cout << "score: " << temp_score;
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
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
    }
    Board(){
       snake = new Snake(10, UI_HEIGHT + 2);
        temp_score = 0;
        speedboost = false;
        cooldown=false;
    }
    ~Board(){
        delete snake;
    }
    int getscore(){
        return temp_score;
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
            if(!bonusSoundPlayed){
                play_bonus();
                bonusSoundPlayed=true;
            }
            double boostElapsed = double(clock() - boostStartTime) / CLOCKS_PER_SEC;
            if(boostElapsed >= boost_time){
                speedboost = false;
                cooldown = true;
                cooldownStartTime = clock();   // start cooldown
                bonusSoundPlayed=false;
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
        cout<<"score" <<temp_score;
    }
    pair<int,string> highest_score(int temp_score){
        int highest=0 ;
        string name, oldName;
    // 1️⃣ Read old highest score and name  
        // 2️⃣ If new score is higher
        if ( temp_score > highest) {
            int x=50;
            int y= 4;
            gotoxy(x,y);
            cout<<"enter your name : ";
            cin >> name;
            ofstream fout("score.txt");
            fout << temp_score << " " << name;
            fout.close();
            highest= temp_score;
        }
        else{
            name=oldName;
        }
        ifstream fin("score.txt");
        if (fin >> highest >> oldName) {
        // read successful
        }
        fin.close();    
        return {highest,name};
    }
    void draw(){
        system("cls");
    // 🔵 DRAW TOP UI BOX
        drawUI();
    // 🟢 DRAW SNAKE (PLAY AREA ONLY)
        for(int i = 0; i < snake->getLength(); i++){
            gotoxy(snake->body[i].xcoord, snake->body[i].ycoord);
            cout << SNAKE_BODY;
        }
    // 🔴 DRAW FOOD
        gotoxy(food.xcoord, food.ycoord);
        cout << FOOD;
    }
    bool update(){
         bool isAlive = snake->move(food);
       if(isAlive == false)
       {
           return false;
       }
        if(food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord)
        {
            temp_score++;
            play_eat();
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
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        string highPlayer;
        pair<int,string> high=highest_score(temp_score);
        string name=high.second;
        int hs=high.first;
    // Clear screen
        system("cls");
    // Set bold red color
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED | FOREGROUND_INTENSITY
        );
        string h_score;
        string msg = "GAME OVER";
        string scoreMsg = "FINAL SCORE : " + to_string(temp_score);
        if(temp_score>hs){
            h_score="highes score is : " + to_string(hs) + "  it's you  ("+name + " )";

        }
        else  h_score="highes score is : " + to_string(hs)+" by "+"( "+name + " )";
        string highStr = h_score;
        int x1 = (con_w - msg.length()) / 2;
        int y1 = con_h / 2;
        int x2 = (con_w - scoreMsg.length()) / 2;
        int y2 = y1 + 2;
        int x3 =(con_w-highStr.length())/2;
        int y3 = y2+2;
        gotoxy(x1, y1);
        cout << msg;
        gotoxy(x2, y2);
        cout << scoreMsg<<"   "<<endl;
        gotoxy(x3,y3);
        cout<< h_score<<  " "<<endl;
    // Reset color back to normal
        SetConsoleTextAttribute(
            hConsole,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
        );
    }
};
int main(){
    play_bgm();
    showConsoleCursor(false);
    srand(time(0));
    initscreen();
    Board *board=new Board();
    board ->spawnFood();
    while(true){
        board->getInput();
        if(!board->update()){
            board->showGameOver();   // 👈 HERE
            break;
        }
        board->draw();
        Sleep(board->getCurrentSpeed());
    }
    delete board;   // ✅ free memory
    return 0;
}