#ifndef SIMPLEINDEX_H
#define SIMPLEINDEX_H
#include <vector>
#include <string>
using namespace std;
class SimpleIndex
{
    public:
        SimpleIndex();
            void AddNewBook();
            void SortIndex();
            void SaveIndexes();
            void LoadData();
            int BinarySearch_BookID(char *);
            int BinarySearch_AuthorID(char *);
            int Search_IDList(char *);
            void DeleteBook(char*);
            int PrintBook(char*);
            void PrintBook_AuthorID(char*);

            void AddNewAuthor();
            void SortIndex_Author();
            void SaveIndexes_Author();
            void LoadData_Author();
            int BinarySearch_PrimeAID(char *);
            int BinarySearch_AuthorName(char *);
            int Search_IDList_Author(char *);
            void DeleteAuthor(char*);
            int PrintAuthor(char*);
            void PrintAuthor_AuthorName(char*);
        void implementQuery(string);
        void printAll();
        void usermenu();
        void clearingfiles();
    private:
        const int MaxRecordSize=100;
        char oBook[30]="OriginalDataBook.txt";
        char Pxbook[30]="PrimaryIndexBook.txt";
        char Sxbook[30]="SeconderyIndexBook.txt";
        char oAuthor[30]="OriginalDataAuthor.txt";
        char PxAuthor[30]="PrimaryIndexAuthor.txt";
        char SxAuthor[30]="SeconderyIndexBook.txt";
        char iList[30]="InvertedList.txt";
        char iList_Author[30]="AuthorInvertedList.txt";


        struct Book
        {
            char Book_ID[13];
            char Author_ID[30];
            char Book_Title[50];
            float Book_Price;
        };
        struct Book_PIndex
        {
            char Id[13];
            int offset;
        };
        struct Book_SIndex
        {
            char Author_ID[30];
            char Id[13];
        };
        struct Label_id
        {
            char Book_ID[13];
            int Final;
        };
        Book_PIndex book_Index[100];
        Book_SIndex book_AuthorId[100];
        vector <Label_id> ID_list;
        int next,after;

        struct Author
        {
            char Author_ID[30];
            char Author_Name[50];
            char Author_Address[50];
            char Author_Mobile[11];
        };
        struct Author_PIndex
        {
            char Id[30];
            int offset;
        };
        struct Author_SIndex
        {
            char Author_Name[50];
            char Auther_ID[30];
        };
        struct Label_id_author
        {
            char Author_ID[30];
            int Final;
        };

        Author_PIndex Author_Index[100];
        Author_SIndex Author_AuthorName[100];
        vector <Label_id_author> ID_list_Author;
        int nextAuthor,afterAuthor;
};

#endif // SIMPLEINDEX_H
