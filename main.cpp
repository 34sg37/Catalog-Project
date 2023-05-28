#include "catalog.cpp"

void trim(string& str) { //function to remove whitespace characters from catalogtype
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](int ch) {return !isspace(ch);}));
    str.erase(find_if(str.rbegin(), str.rend(), [](int ch) {return !isspace(ch);}).base(), str.end());
}

int main(){
    try{
        ifstream file(datafilename);
        if(!file) throw runtime_error("Failed to open file: "+datafilename);
        
        string catalogType;
        getline(file,catalogType);
        trim(catalogType);
        file.close();

        if(catalogType=="book"){
            Catalog<Book> bookCatalog("book");
            bookCatalog.readData();
            bookCatalog.readCommand();
        }
        else if(catalogType=="music"){
            Catalog<Music> musicCatalog("music");
            musicCatalog.readData();
            musicCatalog.readCommand();
        }
        else if(catalogType=="movie"){
            Catalog<Movie> movieCatalog("movie");
            movieCatalog.readData();
            movieCatalog.readCommand();
        }
        else throw invalid_argument("Invalid catalog type");
    }
    catch(const runtime_error& exception){
        ExceptionHandler::handle(exception.what());
        return 1;
    }
    catch(const invalid_argument& exception){
        ExceptionHandler::handle(exception.what());
        return 2;
    }
    return 0;
}