#ifndef STUDENT_H
#define STUDENT_H


class Student
{
    public:
        Student();
        void AddNewstudent(char*,char*,char*);
        void SortIndex();
        void SaveIndexes();
        void LoadData();
        int Hashing(char* );
        void DeleteStudent(char* );
        int BinarySearch_StudentID(char* );
        void PrintAllStudents();
        void ChooseRondumly(int );
        void insertDataBySize(int);
        void usermenu();
    private:
            const int MaxRecordSize=110;
            char oStudent[30]="StudentOriginalData.txt";
            char pxStudent[30]="StudentPrimaryIndex.txt";
            char hStudent[30]="StudentHashing.txt";
        struct primIndex
        {
            char sId[9];
        };
        struct hashData
        {
            char sId[9];
            int offset;
        };
        primIndex sPrimary [997];
        hashData hashArray[997];
        int next;
};

#endif // STUDENT_H
