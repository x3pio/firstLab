#include <iostream>
#include <vector>
#include <fstream>

std::ifstream::pos_type fileSize (std::ifstream& infile, std::string path){
    std::ifstream in (path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

int counterLines(std::ifstream& infile, std::string path){
    infile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try{
        infile.open(path);
    } catch(const std::exception& e) {
        std::cout << "ERROR" << std::endl << "file open error";
        std::cerr << e.what() << '\n';
    }
    
    std::string str;
    int counter = 0;
    while (!infile.eof()){
        std::getline(infile, str);
        counter += 1;
    }
    infile.close();
    return counter;
}

int counterWords(std::ifstream& infile, std::string path){
    infile.open(path);
     std::string word;
    int counter = 0;
    while (!infile.eof()){
        infile >> word;
        counter += 1;
    }
    infile.close();
    return counter;
}

int counterChar(std::ifstream& infile, std::string path){
    infile.open(path);
    char letter;
    int counter = 0;
    while (infile.get(letter)){
        counter += 1;
    }
    infile.close();
    return counter;
}


int main(int argc, char* argv[]){

    std::vector <std::string> shortCommandList;
    std::vector <std::string> longCommandList;
    std::vector <std::string> fileList;

    for (int i = 1; i < argc; i++){
        if (std::string(argv[i])[0] == '-'){
            if (std::string(argv[i])[1] == '-'){
                longCommandList.push_back(std::string(argv[i]));
            } else {
                shortCommandList.push_back(std::string(argv[i]));
            }
        } else {
            fileList.push_back(std::string(argv[i]));
        }
    }



    const int MAX_LENGTH_SHORT_COMMAND = 2;
    std::string lastCommand;
    // разделение нескольких комманд (пример -lwc) на отдельные (-l -w -c)
    for (int i = 0; i < shortCommandList.size(); i++){
        if (shortCommandList[i].size() > MAX_LENGTH_SHORT_COMMAND){
            while (shortCommandList[i].size() > MAX_LENGTH_SHORT_COMMAND){
                lastCommand = shortCommandList[i][shortCommandList[i].size() - 1];
                shortCommandList[i].pop_back();
                shortCommandList.push_back("-" + lastCommand);
            }        
        }
    }


    shortCommandList.insert(shortCommandList.end(), longCommandList.begin(), longCommandList.end());
    if (shortCommandList.size() == 0){
        for (int i = 0; i < fileList.size(); i++){
            std::cout << fileList[i];
        }
        
    }
    for (int j = 0; j < fileList.size(); j++){
        std::ifstream file;
        for (int i = 0; i < shortCommandList.size(); i++){
            if (shortCommandList[i] == "-c" || shortCommandList[i] == "--bytes") {
                std::cout << fileList[j] << ' ' << fileSize(file, fileList[j]) << " bytes" << std::endl;
            } else if (shortCommandList[i] == "-l" || shortCommandList[i] == "--lines"){
                std::cout << fileList[j] << ' ' << counterLines(file, fileList[j]) << " lines" << std::endl;
            } else if (shortCommandList[i] == "-w" || shortCommandList[i] == "--words") {
                std::cout << fileList[j] << ' ' << counterWords(file, fileList[j]) << " words" << std::endl;
            } else if (shortCommandList[i] == "-m" || shortCommandList[i] == "--chars") {
                std::cout << fileList[j] << ' ' << counterChar(file, fileList[j]) << " chars" << std::endl;
            } else {
                std::cout << "ERROR" << std::endl << "option <" << shortCommandList[i] << "> not exist";
                break;
            }
        }
    }
    
    return 0;
}