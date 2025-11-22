#include <bits/stdc++.h>
using namespace std;
int currentplayer;
char currentmarker;
char board[3][3]={{'1','2','3'},{'4','5','6'},{'7','8','9'}};
void drawbox(){
    cout<<" "<<board[0][0]<<" | "<<board[0][1]<<" | "<<board[0][2]<<endl;
    cout<<"---|---|--- \n";
    cout<<" "<<board[1][0]<<" | "<<board[1][1]<<" | "<<board[1][2]<<endl;
    cout<<"---|---|--- \n";
    cout<<" "<<board[2][0]<<" | "<<board[2][1]<<" | "<<board[2][2]<<endl;
    cout<<"---|---|--- \n";  


}
bool placemaker(int slot){
    int row=(slot-1)/3;
    int col=(slot-1)%3;
    if(board[row][col]!='x' && board[row][col]!='0'){
        board[row][col]=currentmarker;
        return true;
        
    }
    else{ return false;}
}
int winner(){
    //row
    for(int i=0;i<3;i++){

        if (board[i][0]==board[i][1] &&  board[i][1] ==board[i][2]){
            return currentplayer;
        }
         else if (board[0][i]==board[1][i]&&  board[1][i] ==board[2][i]){//column
            return currentplayer;
         }
    }
    if (board[0][0]==board[1][1]&& board[1][1] ==board[2][2]){//diagonal
        return currentplayer;
    }
    if (board[0][2]==board[1][1]&& board[1][1]==board[2][0]){
        return currentplayer;
    }
    return 0;
}
    void swapti(){
        if(currentmarker=='x'){
            currentmarker ='0';
        }
        else{
            currentmarker='x';
        }
         if(currentplayer==1){
            currentplayer=2;
        }
        else{
           currentplayer=1;
        }
    }
void game(){
    int playerwon;
    cout<<"player one your choice x or 0 : ";
    char markerp1;
    cin>>markerp1;
    currentplayer=1;
    currentmarker=markerp1; 
    drawbox();
    for (int i=0;i<9;i++){
        cout<<"its player"<<currentplayer<<" 's turn enter your slot : ";
        int slot;
        cin>> slot;
        if(slot<1 || slot>9){
            cout<<"it is not a valid slot , you may try again\n";
            i--;
            continue;


        }
        if(!placemaker(slot)){
            cout<< "slot occupied try again \n";
            i--;
            continue;

        }
        drawbox();
        playerwon=winner();
        if(playerwon == 1 ){
            cout<<"plwayer 1 won\n";
            break;

        }
        else if(playerwon == 2){
            cout<<"player 2 won\n";
            break;
        }
        swapti();
    }
    if(playerwon==0){
        cout<<"its a tie\n";
    }
}
int main(){
    game();
    return 0;
}
