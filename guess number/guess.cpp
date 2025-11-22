#include <bits/stdc++.h>
using namespace std;
class marks{
    public:
    float mar;
    string sub_name;
    marks(string sname,float mark){
        mar=mark;
        sub_name=sname;

    }
};
class students{
    public:
    string stu_name;
    vector<marks>newm;
    students(string studentname){
        stu_name=studentname;
    }
    void addsub(string sname,float mark){
        marks it(sname,mark);
        newm.push_back(it);
    }
    float cal(){
    double m=0,n=0;
    for(const marks &sub : newm){
        m+=sub.mar;
        n+=1;
    }
    return(m>0)?(m/(n*100)):0;

    }
    void showcgpa(){
        cout<<"student name is - "<<stu_name<<endl<<"score is - "<<cal();
    }
    };





int main(){
    string name,sub;
    int n;
    float sm;
    
    cout<<"#student percentage#\n";
    
    cout<<"enter your name";
    getline(cin,name);
    students s(name);

    cout<<"enter your no. of subject";
    cin>>n;
    for(int i=0;i<n;i++){
        cout<<"enter sub name\n";
        cin.ignore();
        cin>>sub;
        cout<<"enter your marks\n";
        cin>>sm;
        s.addsub(sub,sm);

        
    }
    s.showcgpa();
    cout<<name;
}