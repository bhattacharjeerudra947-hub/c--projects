#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
void game_display(){
    cout<<"*************************************welcome*************************************\n";
    cout<<"####################################rock paper scissor####################################\n";
    cout<<"###########################################game###########################################/n";
    cout<<"\t\t\t\t\t\t\t\t\t\t\t\t\t\t created by rudra \n";
    cout<<"press any key to continue \n";
    getch();
    system("cls");
}
void game(){
    cout<<"enter your name \n";
    string name;
    getline(cin,name);
    string arr[3]={"rock ","paper","scissor"};
    choice:
    cout<<"1.rock(🪨)  2.paper(📃) 3.scissor(✂️) press(1/2/3)\n";
    int ch;
    cin>>ch;
    if(ch>=1 && ch<=3   ){
        if(ch==rand()%3){
            cout<<"congratulation "<<name<<" you choose " << arr[ch-1] << " and you won \n";
            valid:
            cout<<"want to play again (1.yes/2.no) \n";
            int n;
            cin >> n;
            if(n==1) goto choice;
            else if(n==2) {
                cout << "thanks for playing \n";
                exit;
            }
            else{

                cout<<"valid input please " ;
                goto  valid; 
            }
        }
        else{
            cout<<"you choose "<<arr[ch-1]<<endl;
            cout<<"sorry  "<< name<<" you loose better luck next time \n";
            cout<<"want to play again (1.yes/2.no) \n";
            int n;
            cin >> n;
            if(n==1) goto choice;
            else if(n==2) {
                cout << "thanks for playing /n";
                exit;
            }
            else{

                cout<<"valid input please " ;
                goto  valid; 
            }

        }
    }
    else{
        cout<<"invalid input try again";
        goto choice;
    }

}
int main(){
    game_display();
    game();
}