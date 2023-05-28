#ifndef CATALOG_H
#define CATALOG_H
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const string datafilename="data.txt";
const string outputfilename="output.txt";
const string commandfilename="commands.txt";

class ExceptionHandler{
    public:
        static void handle(string message);
};

class Entry{ //book, music, movie are all entries so they are inherited from entry class
    public:
        virtual string getAuthors()const;
        virtual string getTags()const;
        virtual string getArtists()const;
        virtual string getStarring()const;
        virtual string getDirector()const;
        virtual string getGenre()const;

        //common in all inherited classes
        string getYear()const;
        string getTitle()const;
        string& getYear();
        string& getTitle();
    private:
        string year,title; //these are common in all inherited classes
};
class Book : public Entry{
    public:
        Book();
        bool readData(const string& line);
        void print()const;
        string getAuthors()const override;
        string getTags()const override;
    private:
        string authors, tags;
};
class Music : public Entry{
    public:
        Music();
        bool readData(const string& line);
        void print()const;
        string getArtists()const override;
        string getGenre()const override;
    private:
        string artists, genre;
};
class Movie : public Entry{
    public:
        Movie();
        bool readData(const string& line);
        void print()const;
        string getDirector()const override;
        string getGenre()const override;
        string getStarring()const override;
    private:
        string director, genre, starring;
};

template<typename T>
class Catalog{
    public:
        Catalog();
        Catalog(const string& t);
        bool checkDuplicate(const string& name)const;
        void readData();
        void readCommand();
        void printToLog(const string& message)const;
        void searchByTitle(const string& title)const;
        void searchByArtists(const string& artist)const;
        void searchByYear(const string& year)const;
        void searchByGenre(const string& genre)const;
        void searchByDirector(const string& director)const;
        void searchByStarring(const string& starring)const;
        void searchByTags(const string& tags)const;
        void sortByTitle();
        void sortByArtists();
        void sortByYear();
        void sortByGenre();
        void sortByDirector();
        void sortByStarring();
        void sortByTags();
    private:
        vector<T> entries;
        string type,commandLine;
        int entryAmount=0;
};
#endif