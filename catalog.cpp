#include "catalog.h"

template<typename T> //default constructor
Catalog<T>::Catalog(){}

template<typename T> //user defined constructor that sets type to t
Catalog<T>::Catalog(const string& t) : type(t){}

template<typename T> //check if there is an entry with same title
bool Catalog<T>::checkDuplicate(const string& name)const{
    for(const auto& entry:entries) if(entry.getTitle()==name) return true;
    return false;
}

template<typename T>  //read data from "data.txt"
void Catalog<T>::readData(){
    ifstream file(datafilename);
    if(!file) {
        throw runtime_error("Failed to open file: "+datafilename);
        exit(1);
    }
    string line; //read first line which is catalog type
    if(getline(file,line)) printToLog("Catalog Read: "+line);
    else{
        throw invalid_argument("Empty file");
        exit(2);
    }
    while(getline(file,line)){ //read the file line by line
        T obj; //might be music,movie or book class object
        try{
            if(obj.readData(line)){ //seperate the line for title, year etc.
                //its in if statement because if returns false, exception for missing field has already been thrown, 
                //so no need to throw duplicate entry

                //if checkduplicate returns true throw invalid_argument exception with duplicate entry message
                if(checkDuplicate(obj.getTitle())) throw invalid_argument("duplicate entry");

                //this part executes if checkduplicate returns false 
                entries.push_back(obj);  //push into entries vector
                entryAmount++; //increment the entryamount
            }
        }
        catch(const invalid_argument& exception){
            ExceptionHandler::handle(exception.what()); //call static handle function from exceptionhandler class
            obj.print(); //print the entry details, title, year etc.
        }
    }
    printToLog(to_string(entryAmount)+" unique entries"); 
    file.close();
}

template<typename T>
void Catalog<T>::readCommand(){ //read commands from "commands.txt"
    ifstream file(commandfilename);
    if(!file) {
        throw runtime_error("Failed to open file: "+commandfilename);
        exit(1);
    }
    while(getline(file, commandLine))
    {
        try{
            string command, str, field; //command: first word
            //str: first string between "";  field: second string between ""
            int counter=0;
            bool commandtaken=false; //set this to true if the first word is taken
            for(const char& ch:commandLine){
                if(!commandtaken && ch==' ') commandtaken=true; //when the first space is encountered this means first word is taken
                else if(!commandtaken) command +=ch;  //until the first space, keep copying chars into command string
                //when encounter " counter++, if counter is odd number, this means quotes are open so keep copying chars into str string
                if(ch=='"') counter++; 
                else if(counter%2==1 && counter<2) str+=ch;
                //if counter is 3 this means quotes are open for second string, so keep cpying chars into field string
                else if(counter%2==1 && counter<4) field+=ch;
            }
            if(command=="search"){ //if command is search
                if (field == "title"){
                    printToLog(commandLine); //print command line into log file
                    searchByTitle(str); //search by title function, str holds the string to be included in field
                }
                else if(field == "artists"){
                    //books or movies has no member named artists, so if field is artists and type is book or movie throw exception
                    if(type == "book" || type == "movie") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    searchByArtists(str);
                }
                else if(field == "year"){
                    printToLog(commandLine);
                    searchByYear(str);
                }
                else if(field == "genre"){
                    if (type == "book")  throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    searchByGenre(str);
                }
                else if(field == "director"){
                    if(type == "book" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    searchByDirector(str);
                }
                else if(field == "starring"){
                    if(type == "book" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    searchByStarring(str);
                }
                else if(field == "tags"){
                    if (type == "movie" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    searchByTags(str);
                }
                else throw invalid_argument("command is wrong"); //if field is invalid
            }
            else if(command=="sort"){
                //here str represents field
                if (str == "title"){
                    printToLog(commandLine);
                    sortByTitle();
                }
                else if(str == "artists"){
                    if(type == "book" || type == "movie") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    sortByArtists();
                }
                else if(str == "year"){
                    printToLog(commandLine);
                    sortByYear();
                }
                else if(str == "genre"){
                    if (type == "book")  throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    sortByGenre();
                }
                else if(str == "director"){
                    if(type == "book" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    sortByDirector();
                }
                else if(str == "starring"){
                    if(type == "book" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    sortByStarring();
                }
                else if(str == "tags"){
                    if (type == "movie" || type == "music") throw invalid_argument("command is wrong");
                    printToLog(commandLine);
                    sortByTags();
                }
                else throw invalid_argument("command is wrong");
            }
            else throw invalid_argument("command is wrong"); //if command is invalid
        }
        catch(const invalid_argument& exception){
            ExceptionHandler::handle(exception.what());
            printToLog(commandLine);
        }
    }
    file.close();
}

template<typename T>
void Catalog<T>::printToLog(const string& message)const{ //open logfile and print message into it
    ofstream logfile(outputfilename,ios_base::app);
    logfile<<message<<endl;
    logfile.close();
}

template<typename T>
void Catalog<T>::searchByTitle(const string& title)const{ 
    //search all entries, if title of any entry includes the title string print that entry into logfile
    for(const auto& entry:entries)if(entry.getTitle().find(title)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByArtists(const string& artist)const{
    for(const auto& entry:entries) if(entry.getArtists().find(artist)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByYear(const string& year)const{
    for(const auto& entry:entries) if(entry.getYear().find(year)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByGenre(const string& genre)const{
    for(const auto& entry:entries) if(entry.getGenre().find(genre)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByDirector(const string& director)const{
    for(const auto& entry:entries) if(entry.getDirector().find(director)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByStarring(const string& starring)const{
    for(const auto& entry:entries) if(entry.getStarring().find(starring)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::searchByTags(const string& tags)const{
    for(const auto& entry:entries) if(entry.getTags().find(tags)!=string::npos) entry.print();
}

template<typename T>
void Catalog<T>::sortByTitle() { 
    //by using std::sort function and help of a lambda function sort entries vector according to titles of objects,
    //then print the sorted vector into logfile
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getTitle() < entry2.getTitle();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByArtists() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getArtists() < entry2.getArtists();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByYear() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getYear() < entry2.getYear();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByGenre() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getGenre() < entry2.getGenre();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByDirector() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getDirector() < entry2.getDirector();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByStarring() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getStarring() < entry2.getStarring();});
    for(const auto& entry:entries) entry.print();
}

template<typename T>
void Catalog<T>::sortByTags() {
    sort(entries.begin(), entries.end(), [](const T& entry1, const T& entry2) {return entry1.getTags() < entry2.getTags();});
    for(const auto& entry:entries) entry.print();
}

//virtual functions that will be overriden
string Entry::getAuthors()const {return "";}
string Entry::getTags()const {return "";}
string Entry::getArtists()const {return "";}
string Entry::getStarring()const {return "";}
string Entry::getDirector()const {return "";}
string Entry::getGenre()const {return "";}

//common getter and setters for book, movie and music classes
string Entry::getYear()const{return year;}
string Entry::getTitle()const{return title;}
string& Entry::getYear(){return year;} //get ref of year
string& Entry::getTitle(){return title;} //get ref of title

void ExceptionHandler::handle(string message){ //handler static function to print exception message into logfile
    ofstream logfile(outputfilename,ios_base::app);
    logfile<<"Exception: "<<message<<endl;
    logfile.close();
}

Book::Book(){}
bool Book::readData(const string& line){ 
    //while the file is being read line by line, every line is being sent to this function
    //in this function the line is being seperated into title, authors, year, tags
    try{
        int counter=0;
        for(const char& ch : line){ //
            //when encountered with " counter++
            if (ch=='"') counter++;
            //if counter is odd this means quotes are open
            //first quotes which is title
            else if (counter%2==1 && counter <2) getTitle()+=ch;
            //second quotes which is authors
            else if (counter%2==1 && counter <4) authors+=ch;
            //third quotes which is year
            else if (counter%2==1 && counter <6) getYear() +=ch;
            //fourth quotes which is tags
            else if (counter%2==1 && counter<8) tags +=ch;
        }
        //program understands argument amount with quotesamount/2, so 4 arguments for books.
        //if less than 4 then throw exception
        if (counter<8) throw invalid_argument("missing field"); 
    }
    catch(const invalid_argument& exception){
        ExceptionHandler::handle(exception.what());
        print();
        return false; //return false if has missing field
    }
    return true; //if has all fields return true
}
void Book::print()const{ //print the entry details into logfile
    ofstream logfile(outputfilename,ios_base::app);
    logfile<<'"'<<getTitle()<<'"'<<" "<<'"'<<authors<<'"'<<" "<<'"'<<getYear()<<'"'<<" "<<'"'<<tags<<'"'<<endl;
    logfile.close();
}
//getters
string Book::getAuthors()const {return authors;}
string Book::getTags()const {return tags;}

Music::Music(){}
bool Music::readData(const string& line){
    try{
        int counter=0;
        for(const char& ch : line){
            if (ch=='"') counter++;
            else if (counter%2==1 && counter <2) getTitle()+=ch;
            else if (counter%2==1 && counter <4) artists+=ch;
            else if (counter%2==1 && counter <6) getYear() +=ch;
            else if (counter%2==1 && counter<8) genre +=ch;
        }
        if (counter<8) throw invalid_argument("missing field");
    }
    catch(const invalid_argument& exception){
        ExceptionHandler::handle(exception.what());
        print();
        return false;
    }
    return true;
}
void Music::print()const{
    ofstream logfile(outputfilename,ios_base::app);
    logfile<<'"'<<getTitle()<<'"'<<" "<<'"'<<artists<<'"'<<" "<<'"'<<getYear()<<'"'<<" "<<'"'<<genre<<'"'<<endl;
    logfile.close();
}
string Music::getArtists()const {return artists;}
string Music::getGenre()const {return genre;}

Movie::Movie(){}
bool Movie::readData(const string& line){
    try{
        int counter=0;
        for(const char& ch : line){
            if (ch=='"') counter++;
            else if (counter%2==1 && counter <2) getTitle()+=ch;
            else if (counter%2==1 && counter <4) director+=ch;
            else if (counter%2==1 && counter <6) getYear() +=ch;
            else if (counter%2==1 && counter<8) genre +=ch;
            else if (counter %2==1 && counter<10) starring +=ch;
        }
        if (counter<10) throw invalid_argument("missing field");
    }
    catch(const invalid_argument& exception){
        ExceptionHandler::handle(exception.what());
        print();
        return false;
    }
    return true;
}
void Movie::print()const{
    ofstream logfile(outputfilename,ios_base::app);
    logfile<<'"'<<getTitle()<<'"'<<" "<<'"'<<director<<'"'<<" "<<'"'<<getYear()<<'"'<<" "<<'"'<<genre<<'"'<<" "<<'"'<<starring<<'"'<<endl;
    logfile.close();
}
string Movie::getDirector()const {return director;}
string Movie::getGenre()const {return genre;}
string Movie::getStarring()const {return starring;}