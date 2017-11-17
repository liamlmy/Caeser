//
//  main.cpp
//  Caeser
//
//  Created by 李明阳 on 11/6/17.
//  Copyright © 2017 李明阳. All rights reserved.
//

#include <iostream>
#include <mysql.h>
#include <stdlib.h>

using namespace std;
MYSQL *conn;

void login();
void Studentmenu(string);
void Transcript(string);
void Course(string);
void Enroll(string);
void withdraw(string);
void withdraw_right(string, string, int, string);
void PersonalDetails(string);

string int2str(int n)
{
    char t[256];
    string s;
    sprintf(t, "%d", n);
    s = t;
    return s;
}

string student_semester_decide(int x)
{
    if (x >= 9 && x <= 12)
    {
        return "Q1";
    }
    else if (x >= 1 && x <= 3)
    {
        return "Q2";
    }
    else if (x >= 4 && x <= 6)
    {
        return "Q3";
    }
    else if (x >= 7 && x <= 8)
    {
        return "Q4";
    }
    else if (x > 12)
    {
        return student_semester_decide(x - 12);
    }
    else
    {
        return "0";
    }
}

void enroll_right2015 (string chosecourse, string id)
{
    string semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    year = now -> tm_year + 1900;
    semester = student_semester_decide(now -> tm_mon);
    
    MYSQL_RES *res_set;
    
    string enroll2 = "CALL addclass (" + id+",'" + chosecourse + "'," + int2str(year) + ",'" + semester + "');";
    mysql_query(conn, enroll2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "============  Successfully enrolled!  ============" << endl;
    cout << "Enter 1 to go back to enroll menu" << endl;
    cout << "Enter 2 to go back to student menu" << endl;
    cout << "Your option is: ";
    
    string enroll_option;
    cin >> enroll_option;
    if ( enroll_option == "1" )
    {
        Enroll(id);
    }
    else if ( enroll_option == "2" )
    {
        Studentmenu(id);
    }
    else {
        cout << "*** You need to enter a valid command! ***" << endl;
        cout << "*** Go back to student menu! ***" << endl << endl;
        Studentmenu(id);
        
    }
}

void enroll_right2016 (string chosecourse ,string id)
{
    string semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime ( &t );
    year = now -> tm_year + 1900 + 1;
    semester = student_semester_decide ( now -> tm_mon + 3);
    
    MYSQL_RES *res_set;
    
    string enroll2 = "CALL addclass (" + id + ",'" + chosecourse + "'," + int2str(year) + ",'" + semester + "');";
    mysql_query(conn, enroll2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "============  Successfully enrolled!  ============" << endl;
    cout << "Enter 1 to go back to enroll menu" << endl;
    cout << "Enter 2 to go back to student menu" << endl;
    cout << "Your option is: ";
    
    string enroll_option;
    cin >> enroll_option;
    if (enroll_option == "1" )
    {
        Enroll(id);
    }
    else if (enroll_option == "2")
    {
        Studentmenu(id);
    }
    else {
        cout << "*** You need to enter a valid command! ***" << endl;
        cout << "*** Go back to student menu! ***" << endl << endl;
        Studentmenu(id);
    }
}

// Main Function
int main(int argc, const char * argv[])
{
    conn = mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
    
    cout << "====================================================" << endl;
    cout << "Welcome to the Northwestern Caesar’s Academic System" << endl;
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "============ %Y/%m/%d %X %A ============",localtime(&t) );
    puts(tmp);
    
    while (true) {
        login();
    }
}

void login()
{
    string id;
    string password;
    
    cout << "---------------------------------" << endl;
    cout << "please input your id and password" << endl;
    cout << "---------------------------------" << endl;
    
    cout << ">> enter your id: " << endl;
    cin >> id;                                      // input the id
    cout << ">> enter your password: " << endl;
    cin >> password;                                // input the password
    
    MYSQL_RES *res_set;                 // A result of a qeury
    MYSQL_ROW row;                      // A row of table
    string query = "SELECT Password FROM Student WHERE Id = " + id + ";";
    mysql_query(conn, query.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int) mysql_num_rows(res_set);
    row = mysql_fetch_row(res_set);
    
    if (numrows == 1)
    {
        if (row[0] == password)
        {
            Studentmenu(id);
        }
        else
        {
            cout << "---------------------" << endl;
            cout << "The password is wrong!" << endl;
            cout << "---------------------" << endl;
            login();
        }
    }
    else
    {
        cout << "----------------" << endl;
        cout << "The id is wrong!" << endl;
        cout << "----------------" << endl;
        login();
    }
    mysql_free_result(res_set);
}

// Menu Page
void Studentmenu(string id)
{
    string option;
    string logout = "logout";
    string semester;
    int year;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    year = now -> tm_year + 1900;
    semester = student_semester_decide(now -> tm_mon);      // call new function
    
    mysql_close(conn);
    conn = mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    string course = "SELECT * FROM transcript WHERE StudId = " + id + " AND year = '" + int2str(year) + "' AND semester= '" + semester + "' AND grade is null;";
    mysql_query(conn, course.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    cout << "============ Current Courses ============" << endl;
    cout << "  student ID: " << id << endl;
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if (row != NULL)
        {
            cout << "  Course: " << row[1] << "         ";
            cout << "  Year: " << row[3] << endl;
        }
    }
    
    cout << "=========================================" << endl;
    cout << "Please select one of the following option:" << endl;
    cout << "Enter 1 for Transcript" << endl;
    cout << "Enter 2 for Enroll" << endl;
    cout << "Enter 3 for Withdraw" << endl;
    cout << "Enter 4 for Personal Details" << endl;
    cout << "Enter 5 for Logout" << endl;
    cout << "Your option is :" ;
    cin >> option;
    if ( option == "1" )
    {
        Transcript(id);
    }
    else if(option=="2")
    {
        Enroll(id);
    }
    else if ( option == "3")
    {
        withdraw(id);
    }
    else if ( option == "4")
    {
        PersonalDetails(id);
    }
    else if ( option == "5")
    {
        login();
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl << endl;
        Studentmenu(id);
    }
}

void Transcript(string id)
{
    string Transcript_option;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    string transcripts = "SELECT * FROM Transcript WHERE StudId = " + id + ";";
    mysql_query(conn, transcripts.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int) mysql_num_rows(res_set);
    
    cout << "============ Transcript =============" << endl;
    for (int i = 0; i < numrows; i++)
    {
        row = mysql_fetch_row(res_set);
        if (row[4] != NULL)
        {
            cout << "  Course: " << row[1] << "    ";
            cout << "  Grade: " << row[4] << endl;
        }
        else
        {
            cout << "  Course: " << row[1] << "    ";
            cout << "  Grade: NULL" << endl;
        }
    }
    cout << "=====================================" << endl;
    mysql_free_result(res_set);
    
    cout << "Please select one of the following option:" << endl;
    cout << "Enter 1 for Course information" << endl;
    cout << "Enter 2 for Main menu" << endl;
    
    cin >> Transcript_option;
    if ( Transcript_option == "1" )
    {
        Course(id);
    }
    else if ( Transcript_option == "2")
    {
        Studentmenu(id);
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl << endl;
        Transcript(id);
    }
}

void Course(string id)
{
    string course_number;
    cout << "Please enter a course number" << endl;
    cin >> course_number;
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string courseinformation = "SELECT t.UoSCode,UoSName,t.Year,t.Semester,Enrollment,MaxEnrollment,Name,Grade,StudId FROM faculty f, transcript t, unitofstudy un,uosoffering uo WHERE StudId = " + id + " AND t.UoSCode= '" + course_number + "'AND t.UoSCode=un.UoSCode AND t.UoSCode=uo.UoSCode AND uo.InstructorId=f.Id AND t.Year=uo.Year AND t.Semester=uo.Semester ;";
    mysql_query(conn, courseinformation.c_str());
    res_set = mysql_store_result(conn);
    int numrows = (int)mysql_num_rows(res_set);
    
    if (numrows == 1)
    {
        row = mysql_fetch_row(res_set);
        if (row[7] != NULL)
        {
            cout << "============ Course Information =============" << endl;
            cout << "Course number: " << row[0] << endl;
            cout << "Title: " << row[1] << endl;
            cout << "Year: " << row[2] << endl;
            cout << "Quarter: " << row[3] << endl;
            cout << "Enroll number: " << row[4] << endl;
            cout << "Max Enroll: " << row[5] << endl;
            cout << "Lecturer: " << row[6] << endl;
            cout << "Grade: " << row[7] << endl;
            cout << "=============================================" << endl;
            cout << "Enter 1 for another course detail" << endl;
            cout << "Enter 2 to Transcript" << endl;
            cout << "Your option is: ";
            string see;
            cin >> see;
            if( see == "1" )
            {
                Course(id);
            }
            else if ( see == "2" )
            {
                Transcript(id);
            }
            else
            {
                cout << "*** You need to enter a valid option! ***" << endl << endl;
                Course(id);
            }
        }
        else
        {
            cout << "============ Course Information =============" << endl;
            cout << "Course number: " << row[0] << endl;
            cout << "Title: " << row[1] << endl;
            cout << "Year: " << row[2] << endl;
            cout << "Quarter: " << row[3] << endl;
            cout << "Enroll number: " << row[4] << endl;
            cout << "Max Enroll: " << row[5] << endl;
            cout << "Lecturer: " << row[6] << endl;
            cout << "Grade: NULL" << endl;
            cout << "=============================================" << endl;
            cout << "Enter 1 for another course detail" << endl;
            cout << "Enter 2 to Transcript" << endl;
            cout << "Your option is: ";
            string see;
            cin >> see;
            if(see == "1")
            {
                Course(id);
            }
            else if (see == "2")
            {
                Transcript(id);
            }
            else
            {
                cout << "*** You need to enter a valid option! ***" << endl << endl;
                Course(id);
            }
        }
    }
    else
    {
        cout << "Please enter a valid course number" << endl;
        Course(id);
    }
    mysql_free_result(res_set);
}

void PersonalDetails(string id)
{
    string change;

    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string pd_id = "SELECT * FROM student where Id=" + id + ";";
    mysql_query(conn, pd_id.c_str());
    res_set = mysql_store_result(conn);
    row = mysql_fetch_row(res_set);
    
    cout << "============ Personal Details =============" << endl;
    cout << "Id: " << row[0] << endl;
    cout << "Name: " << row[1] << endl;
    cout << "Password: " << row[2] << endl;
    cout << "Address: " << row[3] << endl;
    cout << "===========================================" << endl;
    
    cout << "Enter 1 to change the password" << endl;
    cout << "Enter 2 to change the address" << endl;
    cout << "Enter 3 for Main menu" << endl;
    cout << "Your option is: " << endl;
    cin >> change;
    if (change == "1")
    {
        cout << "Enter a new password: " << endl;
        string newpassword;
        cin >> newpassword;
        string revise1 = "wt = '" + newpassword + "' WHERE Student.Id = " + id + ";";
        mysql_query(conn, "START TRANSACTION;");
        mysql_query(conn, revise1.c_str());
        mysql_query(conn,"COMMIT;");
        res_set = mysql_store_result(conn);
        cout << "*** The password is changed successfully! ***" << endl;
        PersonalDetails(id);
    }
    else if (change == "2")
    {
        cout << "Enter a new address" << endl;
        string newaddress;
        cin >> newaddress;
        string revise2 = "UPDATE student SET student.Address = '" + newaddress + "' WHERE student.Id = " + id + ";";
        mysql_query(conn,"START TRANSACTION;");
        mysql_query(conn, revise2.c_str());
        mysql_query(conn,"COMMIT;");
        res_set = mysql_store_result(conn);
        cout << "*** The address is changed successfully! ***" << endl;
        PersonalDetails(id);
    }
    else if (change == "3")
    {
        Studentmenu(id);
    }
    else
    {
        cout << "*** You need to enter a valid option! ***" << endl;
        PersonalDetails(id);
    }
}

void Enroll(string id)
{
    string semester;
    string semester1;
    int year;
    int year1;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    year = now -> tm_year + 1900;                                   // current year
    semester = semester = student_semester_decide(now -> tm_mon);   // current semester
    year1 = now -> tm_year + 1900 + 1;                              // next year
    semester1 = student_semester_decide(now -> tm_mon + 3);         // next semester
    
    mysql_close(conn);
    conn = mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
    
    cout << "You can only enroll the class of the current year and quarter or the following quarter." << endl;
    cout << "Please enter the year and quarter you want to enroll class" << endl;
    cout << "Year: ";
    string whichyear;
    cin >> whichyear;
    cout << "Quarter: ";
    string whichquarter;
    cin >> whichquarter;
    
    if ((whichyear == int2str(year)) && (whichquarter == semester))
    {
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string enroll1="CALL enrollclass (" + id + "," + int2str(year) + ",'" + semester + "');" ;
        mysql_query(conn, enroll1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        cout << "============ Enrollment =============" << endl;
        cout << "Course can enroll: " << endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if (row[0] != NULL)
            {
                cout << "|" << row[0] << "|" << endl;
            }
        }
        
        mysql_free_result( res_set );
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        MYSQL_RES *res_set4;
        MYSQL_ROW row2;
        
        string cannotenroll = "CALL cannotenroll (" + id + "," + int2str(year) + ",'" + semester + "');" ;
        mysql_query(conn, cannotenroll.c_str());
        res_set4 = mysql_store_result(conn);
        int numrows4 = (int)mysql_num_rows(res_set4);
        cout << "Course cannot enroll: " << endl;
        for (int i = 0; i < numrows4; i++)
        {
            row2 = mysql_fetch_row(res_set4);
            if(row2[0] != NULL)
            {
                cout << "|" << row2[0] << "|" << endl;
            }
        }
        mysql_free_result(res_set4);
        
        mysql_close(conn);
        conn = mysql_init(NULL);
        mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        cout << "Enter 1 to enroll a class" << endl;
        cout << "Enter 2 to see the preclass of the course you cannot enroll" << endl;
        cout << "Your option is: ";
        
        string enroll_or_not;
        cin >> enroll_or_not;
        if (enroll_or_not == "1")
        {
            cout << "Please enter the course you want to enroll" << endl;
            string chosecourse1;
            cin >> chosecourse1;
            
            MYSQL_RES *res_set1;
            string enrollchoice = "CALL whetherrightclass('" + chosecourse1 + "'," + id + "," + int2str(year) + "," + semester + "');";
            mysql_query(conn, enrollchoice.c_str());
            res_set1 = mysql_store_result(conn);
            int numrows1 = (int) mysql_num_rows(res_set1);
            
            if (numrows1 == 1)
            {
                mysql_free_result( res_set1 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn, "localhost", "root","root", "project3-nudb", 3306, NULL, 0);
                
                enroll_right2015(chosecourse1,id);
            }
            else
            {
                cout << "*** You need to enter a valid command! ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                Enroll(id);
            }
        }
        else if (enroll_or_not == "2")
        {
            cout << "Please enter the course you cannot enroll to see the pre-class" << endl;
            string chosecourse2;
            cin >> chosecourse2;
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn, "localhost","root", "root", "project3-nudb", 3306, NULL, 0);
            
            MYSQL_RES *res_set2;
            string needpreclass = "CALL needpreclass('" + chosecourse2 + "'," + id + "," + int2str(year) + ",'" + semester + "');";
            mysql_query(conn, needpreclass.c_str());
            res_set2 = mysql_store_result(conn);
            int numrows2 = (int)mysql_num_rows(res_set2);
            
            if (numrows2 == 1)
            {
                cout << "Pre-class of this course" << endl;
                
                mysql_free_result(res_set2);
                
                mysql_close(conn);
                conn = mysql_init (NULL);
                mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
                
                MYSQL_RES *res_set3;
                string preclass = "CALL preclass('" + chosecourse2 + "');";
                mysql_query(conn, preclass.c_str());
                res_set3 = mysql_store_result(conn);
                int numrows3 = (int) mysql_num_rows(res_set3);
                
                for (int i = 0; i < numrows3; i++)
                {
                    row = mysql_fetch_row(res_set3);
                    if(row[0] != NULL)
                    {
                        cout << row[0] << endl;
                    }
                }
                mysql_free_result(res_set3);
                mysql_close(conn);
                conn = mysql_init (NULL);
                mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
                
                cout << "Enter 1 to go back to enroll menu" << endl;
                cout << "Enter 2 to go back to student menu" << endl;
                cout << "Your option is: " << endl;
                
                string enroll_option;
                cin >> enroll_option;
                if (enroll_option == "1")
                {
                    Enroll(id);
                }
                else if (enroll_option == "2")
                {
                    Studentmenu(id);
                }
                else
                {
                    cout << "*** You need to enter a valid command! ***" << endl;
                    cout << "*** Go back to the enroll menu! ***" << endl << endl;
                    Studentmenu(id);
                }
            }
            else
            {
                cout << "*** Invalid course number! ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                Enroll(id);
            }
        }
        else
        {
            cout << "*** You need to enter a valid command! ***" << endl;
            cout << "*** Go back to the enroll menu! ***" << endl << endl;
            Enroll(id);
        }
    }
    else if ((whichyear == int2str(year1)) && (whichquarter == semester1))
    {
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string enroll1 = "CALL enrollclass (" + id + "," + int2str(year1) + ",'" + semester1 + "');";
        mysql_query(conn, enroll1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        cout << "============ Enrollment =============" << endl;
        cout << "Course can enroll: " << endl;
        for (int i = 0; i < numrows; i++)
        {
            row = mysql_fetch_row(res_set);
            if (row[0] != NULL)
            {
                cout << "|" << row[0] << "|" << endl;
            }
        }
        mysql_free_result( res_set );
        
        mysql_close(conn);
        conn = mysql_init (NULL);
        mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
            
        MYSQL_RES *res_set4;
        MYSQL_ROW row2;
        
        string cannotenroll = "CALL cannotenroll (" + id + "," + int2str(year1) + ",'" + semester1 + "');";
        mysql_query(conn, cannotenroll.c_str());
        res_set4 = mysql_store_result(conn);
        int numrows4 = (int)mysql_num_rows(res_set4);
        cout << " Course cannot enroll : " << endl;
        for (int i = 0; i < numrows4; i++)
        {
            row2 = mysql_fetch_row(res_set4);
            if(row2[0] != NULL)
            {
                cout << "|" << row2[0] << "|" << endl;
            }
        }
        mysql_free_result( res_set4 );
        
        mysql_close(conn);
        conn = mysql_init (NULL);
        mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        cout << "Enter 1 to enroll a class" << endl;
        cout << "Enter 2 to see the preclass of the course you cannot enroll" << endl;
        cout << "Your option is: ";
        
        string enroll_or_not;
        cin >> enroll_or_not;
        if (enroll_or_not == "1")
        {
            cout << "Please enter the course you want to enroll" << endl;
            string chosecourse1;
            cin >> chosecourse1;
            
            MYSQL_RES *res_set1;
            string enrollchoice = "CALL whetherrightclass('" + chosecourse1 + "'," + id+","+ int2str(year1) +",'" + semester1 + "');";
            mysql_query(conn, enrollchoice.c_str());
            res_set1 = mysql_store_result(conn);
            int numrows1 = (int)mysql_num_rows(res_set1);
    
            if (numrows1 == 1)
            {
                mysql_free_result( res_set1 );
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
                
                enroll_right2016(chosecourse1, id);
            }
            else
            {
                cout << "*** You need to enter a valid command! ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                Enroll(id);
            }
        }
        else if (enroll_or_not == "2")
        {
            cout << "Please enter the course you cannot enroll to see the pre-class" << endl;
            string chosecourse2;
            cin >> chosecourse2;
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
            
            MYSQL_RES *res_set2;
            string needpreclass = "CALL needpreclass('" + chosecourse2 + "'," + id + ","+ int2str(year1) + ",'" + semester1 + "');";
            mysql_query(conn, needpreclass.c_str());
            res_set2 = mysql_store_result(conn);
            int numrows2 = (int)mysql_num_rows(res_set2);
            
            if (numrows2 == 1)
            {
                cout << "Pre-class of this course" << endl;
                mysql_free_result(res_set2);
                
                mysql_close(conn);
                conn = mysql_init ( NULL );
                mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
    
                MYSQL_RES *res_set3;
                string preclass = "CALL preclass('" + chosecourse2 + "');" ;
                mysql_query(conn, preclass.c_str());
                res_set3 = mysql_store_result(conn);
                int numrows3 = (int) mysql_num_rows(res_set3);
                
                for (int i = 0; i < numrows3; i++)
                {
                    row = mysql_fetch_row(res_set3);
                    if(row[0] != NULL)
                    {
                        cout << row[0] << endl;
                    }
                }
                mysql_free_result(res_set3);
                
                cout << "Enter 1 to go back to enroll menu" << endl;
                cout << "Enter 2 to go back to student menu" << endl;
                cout << "Your option is: ";
            
                string enroll_option;
                cin >> enroll_option;
                if (enroll_option == "1")
                {
                    Enroll(id);
                }
                else if (enroll_option == "2")
                {
                    Studentmenu(id);
                }
                else {
                    cout << "*** You need to enter a valid command! ***" << endl;
                    cout << "*** Go back to the student menu! ***" << endl << endl;
                    Studentmenu(id);
                    
                }
            }
            else
            {
                cout << "*** Invalid course number! ***" << endl;
                cout << "*** Go back to the enroll menu! ***" << endl << endl;
                Enroll(id);
            }
            mysql_close(conn);
            conn = mysql_init(NULL);
            mysql_real_connect(conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
            
            cout << "Enter 1 to go back to enroll menu" << endl;
            cout << "Enter 2 to go back to student menu" << endl;
            cout << "Your option is: ";
            
            string enroll_option;
            cin >> enroll_option;
            if (enroll_option == "1")
            {
                Enroll(id);
            }
            else if (enroll_option == "2")
            {
                Studentmenu(id);
            }
            else {
                cout << "*** You need to enter a valid command! ***" << endl;
                cout << "*** Go back to student menu! ***" << endl << endl;
                Studentmenu(id);
            }
        }
        else
        {
            cout << "*** You need to enter a valid command! ***" << endl;
            cout << "*** Go back to enroll menu! ***" << endl << endl;
            Enroll(id);
        }
    }
    else
    {
        cout << "*** Please enter valid year and quarter! ***" <<endl << endl;
        Enroll(id);
    }
}

void withdraw(string id)
{
    string semester;
    string semester1;
    int year;
    int year1;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    year = now -> tm_year + 1900;
    semester = student_semester_decide( now -> tm_mon);
    year1 = now -> tm_year + 1900 + 1;
    semester1 = student_semester_decide( now -> tm_mon + 3);
    
    mysql_close(conn);
    conn = mysql_init (NULL);
    mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
    
    cout << "You can only withdraw the class of the current year and quarter or the following quarter." << endl;
    cout << "Please enter the year and quarter you want to withdraw class" << endl;
    cout << "Year: " ;
    string whichyear;
    cin >> whichyear;
    cout << "Quarter:" ;
    string whichquarter;
    cin >> whichquarter;
    
    if ((whichyear == int2str(year)) && (whichquarter == semester) )
    {
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn,"localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        
        string withdraw1 = "CALL withdrawclass (" + id + "," + int2str(year) + ",'" + semester + "');";
        mysql_query(conn, withdraw1.c_str());
        res_set = mysql_store_result(conn);
        int numrows = (int)mysql_num_rows(res_set);
        
        if (numrows != 0)
        {
            cout << "============ Withdraw =============" << endl;
            cout << "Course can withdraw: " << endl;
            
            for (int i = 0; i < numrows; i++)
            {
                row = mysql_fetch_row(res_set);
                if (row != NULL)
                {
                    cout << "|" << row[0] << "|" << endl;
                }
            }
        }
        else
        {
            cout << "*** No course can withdraw! ***" << endl;
            cout << "*** Go back to student menu! ***" << endl << endl;
            Studentmenu(id);
        }
        
        mysql_free_result(res_set);
        
        mysql_close(conn);
        conn = mysql_init ( NULL );
        mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        cout << "Please enter the course you want to withdraw" << endl;
        string chosecourse;
        cin >> chosecourse;
        
        MYSQL_RES *res_set1;
        string enrollchoice = "CALL whether_right_withdraw('" + chosecourse + "'," + id + "," + int2str(year) + ",'" + semester + "');";
        mysql_query(conn, enrollchoice.c_str());
        res_set1 = mysql_store_result(conn);
        int numrows1 = (int)mysql_num_rows(res_set1);
        
        if (numrows1 == 1)
        {
            mysql_free_result( res_set1 );
            
            mysql_close(conn);
            conn = mysql_init (NULL);
            mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
            
            withdraw_right(chosecourse, id, year, semester);
        }
        else
        {
            mysql_free_result(res_set1);
            cout << "*** Invalid course number! ***" << endl;
            cout << "*** Go back to the withdraw menu! ***" << endl << endl;
            withdraw(id);
        }
    }
    else if ((whichyear == int2str(year1)) && (whichquarter == semester1))
    {
        MYSQL_RES *res_set2;
        MYSQL_ROW row;
        
        string withdraw1 = "CALL withdrawclass (" + id + "," + int2str(year1) + ",'" + semester1 + "');" ;
        mysql_query(conn, withdraw1.c_str());
        res_set2 = mysql_store_result(conn);
        int numrows2 = (int) mysql_num_rows(res_set2);
        
        if (numrows2 != 0)
        {
            cout << "============ Withdraw =============" << endl;
            cout << "Course can withdraw: " << endl;
            
            for (int i = 0; i < numrows2; i++)
            {
                row = mysql_fetch_row(res_set2);
                if (row!= NULL)
                {
                    cout << "|" << row[0] << "|" << endl;
                }
            }
        }
        else
        {
            cout << "*** No course can withdraw! ***" << endl;
            cout << "*** Go back to student menu! ***" << endl << endl;
            Studentmenu(id);
        }
        
        mysql_free_result(res_set2);
        
        mysql_close(conn);
        conn = mysql_init (NULL);
        mysql_real_connect (conn, "localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
        
        cout << "Please enter the course you want to withdraw" << endl;
        string chosecourse;
        cin >> chosecourse;
        
        MYSQL_RES *res_set3;
        string enrollchoice = "CALL whether_right_withdraw('" + chosecourse+"'," + id + "," + int2str(year1) + ",'" + semester1 + "');";
        mysql_query(conn, enrollchoice.c_str());
        res_set3 = mysql_store_result(conn);
        int numrows3 = (int)mysql_num_rows(res_set3);
        
        if ( numrows3 == 1)
        {
            mysql_free_result( res_set3 );
            
            mysql_close(conn);
            conn = mysql_init ( NULL );
            mysql_real_connect (conn,"localhost", "root", "root", "project3-nudb", 3306, NULL, 0);
            
            withdraw_right(chosecourse, id, year1, semester1);
        }
        else
        {
            mysql_free_result(res_set3);
            cout << "*** Invalid course number! ***" << endl;
            cout << "*** Go back to the withdraw menu! ***" << endl << endl;
            withdraw(id);
        }
    }
    else{
        cout << "*** Please enter valid year and quarter! ***" <<endl << endl;
        withdraw(id);
    }
}

void withdraw_right ( string chosecourse ,string student_id,int year,string student_semester)
{
    MYSQL_RES *res_set;
    
    string withdraw2="CALL decreaseclass ("+student_id+",'"+chosecourse+"',"+ int2str(year) +",'"+student_semester+"');";
    mysql_query(conn, withdraw2.c_str());
    res_set = mysql_store_result(conn);
    mysql_free_result( res_set );
    
    cout << "*** Successfully withdraw! ***" << endl << endl;
    
    
    MYSQL_RES *res_set10;
    string WARNING = "SELECT * FROM Warning ;";
    mysql_query(conn, WARNING.c_str());
    res_set10 = mysql_store_result(conn);
    int numrows10 = (int)mysql_num_rows(res_set10);
    
    if(numrows10!=0)
    {cout << "*** Warning: Enrollment number goes below 50% of the MaxEnrollment! ***" << endl<< endl;
        mysql_free_result(res_set10);}
    
    cout << "Enter 1 to withdraw another course " << endl << "Enter 2 to go back to memu" << endl;
    cout << "Your option is : ";
    string withdraw_option;
    cin >> withdraw_option;
    if ( withdraw_option == "1" )
    {
        withdraw(student_id);
    }
    else if ( withdraw_option == "2" )
    {
        Studentmenu(student_id);
    }
    else
    {
        cout << "*** Invalid command! ***" << endl;
        Studentmenu(student_id);
    }
}
