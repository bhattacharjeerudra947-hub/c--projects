#include <bits/stdc++.h>
using namespace std;
class subject{
    public:
    string subect;
    double credit,gradepoint;
    subject(string sname,double cr,double gp){
        this->subect=sname;
        this->credit=cr;
        this->gradepoint=gp;  

    }
};

class student{
    public:
    string studentname;
    vector<subject>subjects;
    student(string name){
        studentname=name;
    }
    void addsubject(string sub,double credit,double gradepoint){
        subject newsubject(sub,credit,gradepoint);
        subjects.push_back(newsubject);
    }
    float calculate(){
        double totalcredit=0;
        double weightgradepoint=0;
        for(const subject & sub : subjects){
            totalcredit+=sub.credit;
            weightgradepoint+=sub.credit * sub.gradepoint;
                 
        }
        return(totalcredit>0)?(weightgradepoint/totalcredit):0;
    }
    void displaycgpa(){
        
        cout <<"student name is : "<< studentname << "\n"<<"student cgpa = "<<calculate();


    }



};

int main(){
    string studentname;
    int studentsub;
    

    cout<<"student cgpa calculator\n";
    cout<<"enter student name :  \n  ";
    //cin.ignore();
    getline(cin,studentname);
   
    cout<<"enter number subject you have : \n";
    cin>>studentsub;
    student s(studentname);
    for(int i=0;i<studentsub;i++){
        string subject;
        double credit , gradepoint;
    
        cout<<"enter subject : \n";
        cin.ignore();
        getline(cin,subject);
        cout<<"enter credit : \n";
        cin>>credit;
        cout<<"enter grade point : \n";
        cin>>gradepoint;
        
        s.addsubject(subject,credit,gradepoint);

    }
    
    s.displaycgpa();
    return 0;
}