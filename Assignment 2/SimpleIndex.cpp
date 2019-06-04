#include "SimpleIndex.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <strstream>
#include <iomanip>
using namespace std;
SimpleIndex::SimpleIndex()
{
    //ctor
}
void SimpleIndex::SortIndex()
{
    for(int i=0;i<next;i++)
    {
        for(int j=i;j<next;j++)
        {
            if(strcmp(book_Index[i].Id,book_Index[j].Id)==1)
            {
                swap(book_Index[i],book_Index[j]);
            }
        }
    }
    for(int i=0;i<after;i++)
    {
        for(int j=i;j<after;j++)
        {
            if(strcmp(book_AuthorId[i].Author_ID,book_AuthorId[j].Author_ID)==1)
            {
                swap(book_AuthorId[i],book_AuthorId[j]);
            }
        }
    }
}
void SimpleIndex::SaveIndexes()
{
    fstream file1(Pxbook,ios::out|ios::binary);
    fstream file2(Sxbook,ios::out|ios::binary);
    fstream file3(iList,ios::out|ios::binary);
    for(int i=0;i<next;i++)
    {
        file1.write((char*)& book_Index[i],sizeof(book_Index[i]));
    }
    for(int i=0;i<after;i++)
    {
        file2.write((char*)& book_AuthorId[i],sizeof(book_AuthorId[i]));
    }
    for(int unsigned i=0;i<ID_list.size();i++)
    {
        file3.write((char*)& ID_list[i],sizeof(ID_list[i]));
    }
    file1.close();
    file2.close();
    file3.close();
}
void SimpleIndex::LoadData()
{
    next=0,after=0;
    fstream file1(Pxbook,ios::in|ios::binary);
    fstream file2(Sxbook,ios::in|ios::binary);
    fstream file3(iList,ios::in|ios::binary);
    file1.seekp(0,ios::end);
    if(file1.tellp()==0)
    {
        return;
    }
    file1.seekg(0);
    file1.read((char*)& book_Index[next],sizeof(book_Index[next]));
    do{
        next++;
        file1.read((char*)& book_Index[next],sizeof(book_Index[next]));
    }while(!file1.eof());

    file2.read((char*)& book_AuthorId[after],sizeof(book_AuthorId[after]));
    do{
        after++;
        file2.read((char*)& book_AuthorId[after],sizeof(book_AuthorId[after]));
    }while(!file2.eof());

    Label_id Coming;
    ID_list.clear();
    file3.read((char*)& Coming,sizeof(Coming));
    do{
        ID_list.push_back(Coming);
        file3.read((char*)& Coming,sizeof(Coming));
    }while(!file3.eof());
    file1.close();
    file2.close();
    file3.close();
}
int SimpleIndex::BinarySearch_BookID(char *Pos_ID)
{
    SortIndex();
    SaveIndexes();
    LoadData();
    int low=0,high=next-1;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(strcmp(book_Index[mid].Id,Pos_ID)==0)
        {
            return mid;
        }
        else if(strcmp(book_Index[mid].Id,Pos_ID)==1)
        {
            high=mid-1;
        }
        else if(strcmp(book_Index[mid].Id,Pos_ID)==-1)
        {
            low=mid+1;
        }
    }
    return -1;
}
int SimpleIndex::BinarySearch_AuthorID(char *Pos_ID)
{
    SortIndex();
    SaveIndexes();
    LoadData();
    int low=0,high=after-1;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(strcmp(book_AuthorId[mid].Author_ID,Pos_ID)==0)
        {
            return mid;
        }
        else if(strcmp(book_AuthorId[mid].Author_ID,Pos_ID)==1)
        {
            high=mid-1;
        }
        else if(strcmp(book_AuthorId[mid].Author_ID,Pos_ID)==-1)
        {
            low=mid+1;
        }
    }
    return -1;
}
int SimpleIndex::Search_IDList(char *Pos_ID)
{
    for(int unsigned i=0;i<ID_list.size();i++)
    {
        if(strcmp(Pos_ID,ID_list[i].Book_ID)==0)
            return i;
    }
    return -1;
}
void SimpleIndex::AddNewBook()
{
    LoadData();
    char buffer[MaxRecordSize];
    short RecordLength;
    Book Book_data;
    bool bExist=false;
    while(true)//checking book ID  if exist
    {
        bExist=false;
        cout<<"Book ID: ";cin>>Book_data.Book_ID;
        for(int i=0;i<next;i++)
        {
            if(strcmp(Book_data.Book_ID,book_Index[i].Id)==0)
            {
                bExist=true;
                cout<<"this book ID is already exist!\n";
            }
        }
        if(bExist==true)
            continue;
        else if(bExist==false)
            break;
    }
    cout<<"Author ID Of The Book: ";cin>>Book_data.Author_ID;
    cout<<"Book Title: ";cin>>Book_data.Book_Title;
    cout<<"Book Price: ";cin>>Book_data.Book_Price;
    strcpy(buffer,Book_data.Book_ID);
    strcat(buffer,"|");
    strcat(buffer,Book_data.Author_ID);
    strcat(buffer,"|");
    strcat(buffer,Book_data.Book_Title);
    strcat(buffer,"|");
    strcat( buffer,(char*)& Book_data.Book_Price);
    strcat(buffer,"|");
    RecordLength=strlen(buffer);
    fstream file(oBook,ios::out|ios::app|ios::binary);
    file.seekp(0,ios::end);
    int location=file.tellp();
    file<<RecordLength<<" ";
    file.write(buffer,RecordLength);
    strcpy(book_Index[next].Id,Book_data.Book_ID);
    book_Index[next].offset=location;
    int position=BinarySearch_AuthorID(Book_data.Author_ID);
    if(position==-1)
    {
        strcpy(book_AuthorId[after].Author_ID,Book_data.Author_ID);
        strcpy(book_AuthorId[after].Id,Book_data.Book_ID);
        Label_id carry;
        strcpy(carry.Book_ID,Book_data.Book_ID);
        carry.Final=-1;
        ID_list.push_back(carry);
        after++;
    }
    else
    {
        vector <Label_id> container;
        Label_id carry;
        strcpy(carry.Book_ID,Book_data.Book_ID);
        carry.Final=ID_list.size();
        ID_list.push_back(carry);
        container.push_back(carry);
        char next_index[13];
        strcpy(next_index,book_AuthorId[position].Id);
        SaveIndexes();
        int stop=Search_IDList(next_index);
        while(true)
        {
            container.push_back(ID_list[stop]);
            if(ID_list[stop].Final==-1)
                break;
            strcpy(next_index,ID_list[ID_list[stop].Final].Book_ID);
            stop=Search_IDList(next_index);
        }
        for(int unsigned i=0;i<container.size();i++)
        {
            for(int unsigned j=i;j<container.size();j++)
            {
                if(strcmp(container[i].Book_ID,container[j].Book_ID)==-1)
                {
                    swap(container[i],container[j]);
                }
            }
        }
        strcpy(book_AuthorId[position].Id,container[0].Book_ID);
        for(int unsigned i=0;i<container.size()-1;i++)
        {
            container[i].Final=Search_IDList(container[i+1].Book_ID);
        }
        container[container.size()-1].Final=-1;
        int modify;
        for(int unsigned i=0;i<container.size();i++)
        {
            modify=Search_IDList(container[i].Book_ID);
            ID_list[modify].Final=container[i].Final;
        }
    }
    next++;
    SortIndex();
    SaveIndexes();
    file.close();
}
void SimpleIndex::DeleteBook(char* SearchID)
{
    int position=BinarySearch_BookID(SearchID);
    fstream file(oBook,ios::out|ios::in|ios::binary);
    if(position==-1)
    {
        cout<<"\n there is not a Book with this Book ID."<<endl;
        return;
    }
    int RecordOffset=book_Index[position].offset;
    char c='*';
    file.seekp(RecordOffset+2,ios::beg);
    file.put(c);
    for(int i=position;i<next-1;i++)
    {
        book_Index[i]=book_Index[i+1];
    }
    next--;
    SaveIndexes();
    file.close();
}
int SimpleIndex::PrintBook(char* SearchID)
{
    int position =BinarySearch_BookID(SearchID);
    if(position==-1)
    {
        return -1;
    }
    fstream file(oBook,ios::in|ios::binary);
    file.seekg(book_Index[position].offset,ios::beg);
    string sBuffer="";
    int iBuffer;
    getline(file,sBuffer,' ');
    istringstream change(sBuffer);
    change>>iBuffer;
    char *buffer=new char [iBuffer];
    file.read(buffer,iBuffer);
    istrstream Record(buffer);
    Book Data;
    Record.getline(Data.Book_ID,13,'|');
    Record.getline(Data.Author_ID,30,'|');
    Record.getline(Data.Book_Title,50,'|');
    Record.getline((char*)&Data.Book_Price,10,'|');
    cout<<Data.Book_ID<<"\t    |";
    cout<<Data.Author_ID<<"\t|";
    cout<<Data.Book_Title<<"\t\t";
    cout<<Data.Book_Price<<endl;
    return 1;
}
void SimpleIndex::PrintBook_AuthorID(char* SearchID)
{
    int position=BinarySearch_AuthorID(SearchID);
    if(position==-1)
    {
        cout<<"\n there is not a book with this Author ID."<<endl;
        return;
    }
    vector <Label_id> container;
    char next_index[13];
    strcpy(next_index,book_AuthorId[position].Id);
    int stop=Search_IDList(next_index);
    while(true)
    {
        container.push_back(ID_list[stop]);
        if(ID_list[stop].Final==-1)
            break;
        strcpy(next_index,ID_list[ID_list[stop].Final].Book_ID);
        stop=Search_IDList(next_index);
    }
    for(int unsigned i=0;i<container.size();i++)
    {
        int result=PrintBook(container[i].Book_ID);
        if(result==1)
            cout<<"\n\t";
    }
}



void SimpleIndex::SortIndex_Author()
{
    for(int i=0;i<nextAuthor;i++)
    {
        for(int j=i;j<nextAuthor;j++)
        {
            if(strcmp(Author_Index[i].Id,Author_Index[j].Id)==1)
            {
                swap(Author_Index[i],Author_Index[j]);
            }
        }
    }
    for(int i=0;i<afterAuthor;i++)
    {
        for(int j=i;j<afterAuthor;j++)
        {
            if(strcmp(Author_AuthorName[i].Author_Name,Author_AuthorName[j].Author_Name)==1)
            {
                swap(Author_AuthorName[i],Author_AuthorName[j]);
            }
        }
    }
}
void SimpleIndex::SaveIndexes_Author()
{
    fstream file1(PxAuthor,ios::out|ios::binary);
    fstream file2(SxAuthor,ios::out|ios::binary);
    fstream file3(iList_Author,ios::out|ios::binary);
    for(int i=0;i<nextAuthor;i++)
    {
        file1.write((char*)& Author_Index[i],sizeof(Author_Index[i]));
    }
    for(int i=0;i<afterAuthor;i++)
    {
        file2.write((char*)& Author_AuthorName[i],sizeof(Author_AuthorName[i]));
    }
    for(int unsigned i=0;i<ID_list_Author.size();i++)
    {
        file3.write((char*)& ID_list_Author[i],sizeof(ID_list_Author[i]));
    }
    file1.close();
    file2.close();
    file3.close();
}
void SimpleIndex::LoadData_Author()
{
    nextAuthor=0,afterAuthor=0;
    fstream file1(PxAuthor,ios::in|ios::binary);
    fstream file2(SxAuthor,ios::in|ios::binary);
    fstream file3(iList_Author,ios::in|ios::binary);
    file1.seekp(0,ios::end);
    if(file1.tellp()==0)
    {
        return;
    }
    file1.seekg(0);
    file1.read((char*)& Author_Index[nextAuthor],sizeof(Author_Index[nextAuthor]));
    do{
        nextAuthor++;
        file1.read((char*)& Author_Index[nextAuthor],sizeof(Author_Index[nextAuthor]));
    }while(!file1.eof());

    file2.read((char*)& Author_AuthorName[afterAuthor],sizeof(Author_AuthorName[afterAuthor]));
    do{
        afterAuthor++;
        file2.read((char*)& Author_AuthorName[afterAuthor],sizeof(Author_AuthorName[afterAuthor]));
    }while(!file2.eof());

    Label_id_author Coming;
    ID_list_Author.clear();
    file3.read((char*)& Coming,sizeof(Coming));
    do{
        ID_list_Author.push_back(Coming);
        file3.read((char*)& Coming,sizeof(Coming));
    }while(!file3.eof());
    file1.close();
    file2.close();
    file3.close();
}
int SimpleIndex::BinarySearch_PrimeAID(char *Pos_ID)
{
    SortIndex_Author();
    SaveIndexes_Author();
    LoadData_Author();
    int low=0,high=nextAuthor-1;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(strcmp(Author_Index[mid].Id,Pos_ID)==0)
        {
            return mid;
        }
        else if(strcmp(Author_Index[mid].Id,Pos_ID)==1)
        {
            high=mid-1;
        }
        else if(strcmp(Author_Index[mid].Id,Pos_ID)==-1)
        {
            low=mid+1;
        }
    }
    return -1;
}
int SimpleIndex::BinarySearch_AuthorName(char *Pos_ID)
{
    SortIndex_Author();
    SaveIndexes_Author();
    LoadData_Author();
    int low=0,high=afterAuthor-1;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(strcmp(Author_AuthorName[mid].Author_Name,Pos_ID)==0)
        {
            return mid;
        }
        else if(strcmp(Author_AuthorName[mid].Author_Name,Pos_ID)==1)
        {
            high=mid-1;
        }
        else if(strcmp(Author_AuthorName[mid].Author_Name,Pos_ID)==-1)
        {
            low=mid+1;
        }
    }
    return -1;
}
int SimpleIndex::Search_IDList_Author(char *Pos_ID)
{
    for(int unsigned i=0;i<ID_list_Author.size();i++)
    {
        if(strcmp(Pos_ID,ID_list_Author[i].Author_ID)==0)
            return i;
    }
    return -1;
}
void SimpleIndex::AddNewAuthor()
{
    LoadData_Author();
    char buffer[MaxRecordSize];
    short RecordLength;
    Author Author_data;
    bool aExist=false;
    while(true)//checking Author ID  if exist
    {
        aExist=false;
        cout<<"Author ID: ";cin>>Author_data.Author_ID;
        for(int i=0;i<nextAuthor;i++)
        {
            if(strcmp(Author_data.Author_ID,Author_Index[i].Id)==0)
            {
                aExist=true;
                cout<<"this Author ID is already exist!\n";
            }
        }
        if(aExist==true)
            continue;
        else if(aExist==false)
            break;
    }
    cout<<"Author Name: ";cin>>Author_data.Author_Name;
    cout<<"Author Address: ";cin>>Author_data.Author_Address;
    cout<<"Author Mobile: ";cin>>Author_data.Author_Mobile;
    strcpy(buffer,Author_data.Author_ID);
    strcat(buffer,"|");
    strcat(buffer,Author_data.Author_Name);
    strcat(buffer,"|");
    strcat(buffer,Author_data.Author_Address);
    strcat(buffer,"|");
    strcat( buffer,Author_data.Author_Mobile);
    strcat(buffer,"|");
    RecordLength=strlen(buffer);
    fstream file(oAuthor,ios::out|ios::app|ios::binary);
    file.seekp(0,ios::end);
    int location=file.tellp();
    file<<RecordLength<<" ";
    file.write(buffer,RecordLength);
    strcpy(Author_Index[nextAuthor].Id,Author_data.Author_ID);
    Author_Index[nextAuthor].offset=location;
    int position=BinarySearch_AuthorName(Author_data.Author_Name);
    if(position==-1)
    {
        strcpy(Author_AuthorName[afterAuthor].Author_Name,Author_data.Author_Name);
        strcpy(Author_AuthorName[afterAuthor].Auther_ID,Author_data.Author_ID);
        Label_id_author carry;
        strcpy(carry.Author_ID,Author_data.Author_ID);
        carry.Final=-1;
        ID_list_Author.push_back(carry);
        afterAuthor++;
    }
    else
    {
        vector <Label_id_author> container;
        Label_id_author carry;
        strcpy(carry.Author_ID,Author_data.Author_ID);
        carry.Final=ID_list_Author.size();
        ID_list_Author.push_back(carry);
        container.push_back(carry);
        char next_index[30];
        strcpy(next_index,Author_AuthorName[position].Auther_ID);
        SaveIndexes_Author();
        int stop=Search_IDList_Author(next_index);
        while(true)
        {
            container.push_back(ID_list_Author[stop]);
            if(ID_list_Author[stop].Final==-1)
                break;
            strcpy(next_index,ID_list_Author[ID_list_Author[stop].Final].Author_ID);
            stop=Search_IDList_Author(next_index);
        }
        for(int unsigned i=0;i<container.size();i++)
        {
            for(int unsigned j=i;j<container.size();j++)
            {
                if(strcmp(container[i].Author_ID,container[j].Author_ID)==-1)
                {
                    swap(container[i],container[j]);
                }
            }
        }
        strcpy(Author_AuthorName[position].Auther_ID,container[0].Author_ID);
        for(int unsigned i=0;i<container.size()-1;i++)
        {
            container[i].Final=Search_IDList_Author(container[i+1].Author_ID);
        }
        container[container.size()-1].Final=-1;
        int modify;
        for(int unsigned i=0;i<container.size();i++)
        {
            modify=Search_IDList_Author(container[i].Author_ID);
            ID_list_Author[modify].Final=container[i].Final;
        }
    }
    nextAuthor++;
    SortIndex_Author();
    SaveIndexes_Author();
    file.close();
}
void SimpleIndex::DeleteAuthor(char* SearchID)
{
    int position=BinarySearch_AuthorID(SearchID);
    fstream file(oAuthor,ios::out|ios::in|ios::binary);
    if(position==-1)
    {
        cout<<"\n there is not an Author with this Author ID."<<endl;
        return;
    }
    int RecordOffset=Author_Index[position].offset;
    char c='*';
    file.seekp(RecordOffset+2,ios::beg);
    file.put(c);
    for(int i=position;i<nextAuthor-1;i++)
    {
        Author_Index[i]=Author_Index[i+1];
    }
    position=BinarySearch_AuthorID(SearchID);
    vector <Label_id> container;
    char next_index[13];
    strcpy(next_index,book_AuthorId[position].Id);
    int stop=Search_IDList(next_index);
    while(true)
    {
        container.push_back(ID_list[stop]);
        if(ID_list[stop].Final==-1)
            break;
        strcpy(next_index,ID_list[ID_list[stop].Final].Book_ID);
        stop=Search_IDList(next_index);
    }
    for(int unsigned i=0;i<container.size();i++)
    {
        DeleteBook(container[i].Book_ID);
    }
    for(int i=position;i<after;i++)
    {
        book_AuthorId[i]=book_AuthorId[i+1];
    }
    nextAuthor--;
    SaveIndexes();
    SaveIndexes_Author();
    file.close();
}
int SimpleIndex::PrintAuthor(char*SearchID)
{
    int position =BinarySearch_PrimeAID(SearchID);
    if(position==-1)
    {
        return -1;
    }
    fstream file(oAuthor,ios::in|ios::binary);
    file.seekg(Author_Index[position].offset,ios::beg);
    string sBuffer="";
    int iBuffer;
    getline(file,sBuffer,' ');
    istringstream change(sBuffer);
    change>>iBuffer;
    char *buffer=new char [iBuffer];
    file.read(buffer,iBuffer);
    istrstream Record(buffer);
    Author Data;
    Record.getline(Data.Author_ID,30,'|');
    Record.getline(Data.Author_Name,50,'|');
    Record.getline(Data.Author_Address,50,'|');
    Record.getline(Data.Author_Mobile,11,'|');
    cout<<Data.Author_ID<<"\t    |";
    cout<<Data.Author_Name<<"\t|";
    cout<<Data.Author_Address<<"\t|";
    cout<<Data.Author_Mobile<<"\t|";
    return 1;
}
void SimpleIndex::PrintAuthor_AuthorName(char* SearchID)
{
    int position=BinarySearch_AuthorName(SearchID);
    if(position==-1)
    {
        cout<<"\n there is not an Author with this Author name."<<endl;
        return;
    }
    vector <Label_id_author> container;
    char next_index[30];
    strcpy(next_index,Author_AuthorName[position].Auther_ID);
    int stop=Search_IDList_Author(next_index);
    while(true)
    {
        container.push_back(ID_list_Author[stop]);
        if(ID_list_Author[stop].Final==-1)
            break;
        strcpy(next_index,ID_list_Author[ID_list_Author[stop].Final].Author_ID);
        stop=Search_IDList_Author(next_index);
    }
    for(int unsigned i=0;i<container.size();i++)
    {
        int result=PrintAuthor(container[i].Author_ID);
        if(result==1)
            cout<<"\n\t";
    }
}



void SimpleIndex::implementQuery(string sQuery){
    LoadData();
    LoadData_Author();
    bool bFlags;
    string *sQueryWords=new string [10];
    string sSelect, sFrom, sCondition,sLast,sGood="",sValue;
    int i=0;
    istringstream iSN(sQuery);
    do{
        iSN>> sQueryWords[i];
        i++;
    } while (iSN);
    for (int j=0;j<10;j++){
        if(sQueryWords[j]=="select"){
            sSelect=sQueryWords[j+1];
            sGood+='1';
        }
        else if(sQueryWords[j]=="from"){
            sFrom=sQueryWords[j+1];
            sGood+='1';
        }
        else if(sQueryWords[j]=="where"){
            sCondition=sQueryWords[j];
            sLast=sQueryWords[j+1];
            sGood+='1';
        }
        else if(sQueryWords[j]=="and"){
            sCondition=sQueryWords[j];
            sLast=sQueryWords[j+1];
            sGood+='1';
        }
        else if(sQueryWords[j]=="="){
            sValue=sQueryWords[j+1];
        }
    }
    if (sGood!="111"){
        cout<<"You've Entered an Invalid Query \n";
        //return;
    }
    if (sSelect=="all"){
        if(sFrom=="Books"){
            if (sCondition=="and"){
                if(sLast=="Authors"){
                    cout<<"\n\t Author_ID | Author_Name | Author_Address | Author_Mob|";
                    cout<<"  Book_ID | Book_title | Book_Price \n";
                    printAll();
                }
            }
            else if(sCondition=="where"){
                if(sLast=="Author_ID"){
                    cout<<"\n\t Book_ID | Author_ID | Book_title | Book_Price \n";
                    char cAuthorID[50];
                    strcpy(cAuthorID,sValue.c_str());
                    PrintBook_AuthorID(cAuthorID);
                }
            }
        }
        else if (sFrom=="Authors"){
            if(sCondition=="where"){
                if(sLast=="Author_ID"){
                    cout<<"\n\t Author_ID | Author_Name | Author_Address | Author_Mobile \n\t";
                    char cAuthorID[50];
                    strcpy(cAuthorID,sValue.c_str());
                    PrintAuthor(cAuthorID);
                }
            }
        }
    }
    else if ( sSelect=="Book_Title"){
        if(sFrom=="Books"){
            if(sCondition=="where"){
                if(sLast=="Book_ID"){
                    cout<<"@\n";
                    for(int i=0;i<next;i++){
                        bFlags=0;
                        if(strcmp(book_AuthorId[i].Id,sValue.c_str())==0){
                            bFlags=1;
                            break;
                        }
                    }
                    if(!bFlags)
                        cout<<"\t\t You've Entered an Invalid ID\n";
                }
            }
        }
    }

}
void SimpleIndex::printAll(){
    vector <string> vIDs;
    LoadData();
    LoadData_Author();
    char cBookID[13]; //primary key
    char cAuthorID[30]; //sec. key
    char cBookTitle[50];
    char *cBuffer,cBookPrice[20];
    fstream file(oBook,ios::in);
    for(int i=0;i<next;i++){
        int iPosition=book_Index[i].offset;
        file.seekg(iPosition,ios::beg);
        string sLen;
        int iLen;
        getline(file,sLen,' ');
        istringstream change(sLen); /* Bt7wl el String L Integer */
        change>>iLen;
        cBuffer=new char[iLen];
        file.read(cBuffer,iLen); /* Read Record Data From File In cBuffer*/
        istrstream split(cBuffer);
        split.getline(cBookID,13,'|');
        split.getline(cAuthorID,30,'|');
        split.getline(cBookTitle,50,'|');
        split.getline(cBookPrice,20,'|');
        vIDs.push_back(cAuthorID);
        cout<<"\t";
        PrintAuthor(cAuthorID);
        cout<<"|";
        cout<<setw(7)<<cBookID<<" | "<<setw(6)<<cBookTitle<<"     | "<<setw(7)<<cBookPrice<<endl;
    }
    file.close();
    bool bLock;
    for(int i=0;i<after;i++){
        bLock=1;
        for(int unsigned j=0;j<vIDs.size();j++){
            if(strcmp(Author_Index[i].Id,vIDs[j].c_str())==0){
                bLock=0;
                break;
            }
        }
        if (bLock){
            PrintAuthor(Author_Index[i].Id);
            cout<<endl;
        }
    }
};
void SimpleIndex::usermenu()
{
    clearingfiles();
    cout<<"\t\t\tWelcome\nPlease select one of the options to do: ";
    string choice,sQuery;
    while (true)
    {
        sQuery="";
        cout<<"\n\t1)Add New Book\n\t2)Add New Author\n\t3)Delete Book (ID)\n\t4)Delete Author (ID)\n\t5)Print Book (ID)\n\t";
        cout<<"6)Print Book(Author_ID)\n\t7)Print Author(ID)\n\t8)Print Author(Name)\n\t9)Write a Query\n\t10)Exit\nyour choice: ";
        cin>>choice;
        if(choice=="1")
            AddNewBook();
        else if(choice=="2")
            AddNewAuthor();
        else if(choice=="3")
        {
            cout<<"please enter th Book ID you want to delete: ";
            char word[100];
            cin>>word;
            DeleteBook(word);
        }
        else if(choice=="4")
        {
            cout<<"please enter th Author ID you want to delete: ";
            char word[100];
            cin>>word;
            DeleteAuthor(word);
        }
        else if(choice=="5")
        {
            cout<<"please enter th Book ID you want to print: ";
            char word[100];
            cin>>word;
            cout<<"\n\t Book_ID | Author_ID | Book_title | Book_Price \n\t";
            int result=PrintBook(word);
            if(result==-1)
                cout<<"\n there is not a Book with this Book ID."<<endl;
        }
        else if(choice=="6")
        {
            cout<<"please enter th Author ID you want to print his Books: ";
            char word[100];
            cin>>word;
            cout<<"\n\t Book_ID | Author_ID | Book_title | Book_Price \n\t";
            PrintBook_AuthorID(word);
        }
        else if(choice=="7")
        {
            cout<<"please enter th Author ID you want to print his Information: ";
            char word[100];
            cin>>word;
            cout<<"\n\t Author_ID | Author_Name | Author_Address | Author_Mob|\n\t";
            int result=PrintAuthor(word);
            if(result==-1)
                cout<<"\n there is not an Author with this Author ID."<<endl;
        }
        else if(choice=="8")
        {
            cout<<"please enter th Author Name you want to print his Information: ";
            char word[100];
            cin>>word;
            cout<<"\n\t Author_ID | Author_Name | Author_Address | Author_Mob|\n\t";
            PrintAuthor_AuthorName(word);
        }
        else if(choice=="9")
        {
            cout<<"Please Enter a Query To Implement: ";
            cin.ignore();
            getline(cin,sQuery);
            implementQuery(sQuery);
        }
        else if(choice=="10")
        {
            break;
        }
        else
            cout<<"\nInvalid Choice!!\n";
    }
}
void SimpleIndex::clearingfiles()
{
    fstream file1(oBook,ios::trunc|ios::out);
    fstream file2(Pxbook,ios::trunc|ios::out);
    fstream file3(Sxbook,ios::trunc|ios::out);
    fstream file4(oAuthor,ios::trunc|ios::out);
    fstream file5(PxAuthor,ios::trunc|ios::out);
    fstream file6(SxAuthor,ios::trunc|ios::out);
    fstream file7(iList,ios::trunc|ios::out);
    fstream file8(iList_Author,ios::trunc|ios::out);
    file1.close();
    file2.close();
    file3.close();
    file4.close();
    file5.close();
    file6.close();
    file7.close();
    file8.close();
}
