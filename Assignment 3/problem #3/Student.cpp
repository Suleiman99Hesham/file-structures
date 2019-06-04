#include "Student.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cmath>
#include <sstream>
#include <strstream>

using namespace std;

Student::Student()
{
    for(int i=0;i<997;i++)
    {
        hashArray[i].offset=-1;
    }
    //ctor
}
void Student::SortIndex()
{
    for(int i=0;i<next;i++)
    {
        for(int j=i;j<next;j++)
        {
            if(strcmp(sPrimary[i].sId,sPrimary[j].sId)==1)
            {
                swap(sPrimary[i],sPrimary[j]);
            }
        }
    }
}
void Student::SaveIndexes()
{
    fstream file1(pxStudent,ios::out|ios::binary);
    fstream file2(hStudent,ios::out|ios::binary);
    for(int i=0;i<next;i++)
    {
        file1.write((char*)& sPrimary[i],sizeof(sPrimary[i]));
    }
    for(int i=0;i<997;i++)
    {
        file2.write((char*)& hashArray[i],sizeof(hashArray[i]));
    }
    file1.close();
    file2.close();
}
void Student::LoadData()
{
    next=0;
    int i=0;
    fstream file1(pxStudent,ios::in|ios::binary);
    fstream file2(hStudent,ios::in|ios::binary);
    file1.seekp(0,ios::end);
    if(file1.tellp()==0)
    {
        return;
    }
    file1.seekg(0);
    file1.read((char*)& sPrimary[next],sizeof(sPrimary[next]));
    do{
        next++;
        file1.read((char*)& sPrimary[next],sizeof(sPrimary[next]));
    }while(!file1.eof());
    file2.seekg(0);
    file2.read((char*)& hashArray[i],sizeof(hashArray[i]));
    do{
        i++;
        file2.read((char*)& hashArray[i],sizeof(hashArray[i]));
    }while(!file2.eof());
    file1.close();
    file2.close();
}
int Student::Hashing(char* hashID)
{
    int K=int(hashID[0])*int(hashID[1]);
    double A=0.6180339887;
    int result=997*fmod((K*A),1);
    return result;
};
void Student::AddNewstudent(char *rId,char *rName,char *rAddress)
{
    LoadData();
    char buffer[MaxRecordSize];
    short RecordLength;
    strcpy(buffer,rId);
    strcat(buffer,"|");
    strcat(buffer,rName);
    strcat(buffer,"|");
    strcat(buffer,rAddress);
    strcat(buffer,"|");
    RecordLength=strlen(buffer);
    fstream file(oStudent,ios::out|ios::app|ios::binary);
    file.seekp(0,ios::end);
    int offset=file.tellp();
    file<<RecordLength<<" ";
    file.write(buffer,RecordLength);
    strcpy(sPrimary[next].sId,rId);
    int hashValue=Hashing(rId);
    if(hashArray[hashValue].offset!=-1)
    {
        int i=0;
        while(true)
        {
            if(hashArray[i].offset!=-1)
            {
                i++;
                continue;
            }
            else
            {
                hashValue=i;
                break;
            }
        }
    }
    strcpy(hashArray[hashValue].sId,rId);
    hashArray[hashValue].offset=offset;
    next++;
    SortIndex();
    SaveIndexes();
    file.close();
}
int Student::BinarySearch_StudentID(char *Pos_ID)
{
    LoadData();
    SortIndex();
    int low=0,high=next-1;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(strcmp(sPrimary[mid].sId,Pos_ID)==0)
        {
            return mid;
        }
        else if(strcmp(sPrimary[mid].sId,Pos_ID)==1)
        {
            high=mid-1;
        }
        else if(strcmp(sPrimary[mid].sId,Pos_ID)==-1)
        {
            low=mid+1;
        }
    }
    return -1;
}
void Student::DeleteStudent(char* SearchID)
{
    LoadData();
    int position=Hashing(SearchID);
    fstream file(oStudent,ios::out|ios::in|ios::binary);
    if(hashArray[position].offset==-1)
    {
        cout<<"\n there is not a Student with this student ID."<<endl;
        return;
    }
    int RecordOffset=hashArray[position].offset;
    char c='*';
    file.seekp(RecordOffset+2,ios::beg);
    file.put(c);
    hashArray[position].offset=-1;
    int idPosition=BinarySearch_StudentID(SearchID);
    for(int i=idPosition;i<next-1;i++)
    {
        sPrimary[i]=sPrimary[i+1];
    }
    next--;
    SaveIndexes();
    file.close();
}
void Student::PrintAllStudents()
{
    LoadData();
    fstream file(oStudent,ios::in|ios::binary);
    for(int i=0;i<next;i++)
    {
        int position =Hashing(sPrimary[i].sId);
        if(hashArray[position].offset!= -1)
        {
            file.seekg(hashArray[position].offset,ios::beg);
            string sBuffer="";
            int iBuffer;
            getline(file,sBuffer,' ');
            istringstream change(sBuffer);
            change>>iBuffer;
            char *buffer=new char [iBuffer];
            file.read(buffer,iBuffer);
            istrstream Record(buffer);
            char sId[9],sName[49],sAddress[49];
            Record.getline(sId,9,'|');
            Record.getline(sName,49,'|');
            Record.getline(sAddress,49,'|');
            cout<<"\nID: "<<sId<<"\tName: "<<sName<<"\t Address: "<<sAddress;
        }
    }
    file.close();
}
void Student::ChooseRondumly(int nRecords)
{
    int index;
    for(int i=0;i<nRecords;i++)
    {
        index=rand()%(next);
        DeleteStudent(sPrimary[index].sId);
    }
}
/*void Student::insertDataBySize(int nRecords)
{
    fstream file1("student_ids.txt",ios::in|ios::binary);
    fstream file2("student_names.txt",ios::in|ios::binary);
    fstream file3("student_address.txt",ios::in|ios::binary);
    char id[9],name[49],address[49];
    for(int i=0;i<nRecords;i++)
    {
        file1.getline(id,9);
        file2.getline(name,49);
        file3.getline(address,49);
        AddNewstudent(id,name,address);
    }
}*/

void Student::usermenu()
{
    /*AddNewstudent("22958","Dark","Cairo");
    AddNewstudent("23966","Amr","Giza");
    AddNewstudent("24742","Tonie","Liver");
    AddNewstudent("25211","Modric","Mohand");
    AddNewstudent("26821","Kae","Sakya");
    AddNewstudent("27248","Lahm","Fisal");
    AddNewstudent("28730","Alba","Harm");
    AddNewstudent("29644","Mekki","Birlin");
    AddNewstudent("30891","Hossam","New Yord");
    AddNewstudent("31169","Ahmed","Lodond");
    AddNewstudent("55031","Suleiman","Mancity");
    AddNewstudent("33315","Kareem","Braca");
    AddNewstudent("34403","Mahmoud","Madrid");
    AddNewstudent("35508","Lala","Met Okba");
    AddNewstudent("36060","Ezzat","Sohag");
    AddNewstudent("37400","Suleiman","Cali");
    AddNewstudent("38215","Kareem","Iraq");
    AddNewstudent("39768","Mahmoud","Saft");
    AddNewstudent("40055","Lala","Talbya");
    AddNewstudent("41550","Ezzat","Orabi");*/
    AddNewstudent("26821","ahmed","ooooooooooo");
    AddNewstudent("26821","mohamed","Sakya");
    AddNewstudent("26821","ali","Sakya");
    PrintAllStudents();
}

