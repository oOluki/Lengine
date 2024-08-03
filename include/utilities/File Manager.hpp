#pragma once
#include "../../dependencies/STD_dependencies.hpp"

#define LE_PATH_OPENING_ERROR_CHECK(boolean, path) if(boolean) std::cout << "Not able to open" << path << std::endl
#define LE_PROMPTCOMMAND(x) system(x)

#if (defined(__linux__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__unix__))
#define LE_FILEMANAGER


#define LE_OPENFILECOMMAND(x) system((std::string("open ") + x).data())
#define LE_OPENDIRECTORYCOMMAND(x) LE_OPENFILECOMMAND(x)
#define LE_MAKEFILECOMMAND(x) system((std::string("> ") + x).data())
#define LE_MAKEDIRECTORYCOMMAND(x) system((std::string("mkdir ") + x).data())
#define LE_DELETEFILECOMMAND(x) system((std::string("rm ") + x).data())
#define LE_DELETEDIRECTORYCOMMAND(x) system((std::string("rm -r ") + x).data())
#define LE_RENAMECOMMAND(x, y) system((std::string("mv ") + x + " " + y).data())
#define LE_MOVECOMMAND(x, y) LE_RENAMECOMMAND(x, y)
#define LE_COPYFILECOMMAND(x, y) system((std::string("cp ") + x + " " + y).data())
#define LE_COPYDIRECTORYCOMMAND(x) system((std::string("cp -r ") + x + " " + y).data())


#elif defined(_WIN32)
#define LE_FILEMANAGER

#define LE_OPENFILECOMMAND(x) system((std::string("start ") + x).data())
#define LE_MAKEFILECOMMAND(x) system((std::string("type nul > ") + x).data())
#define LE_MAKEDIRECTORYCOMMAND(x) system((std::string("mkdir ") + x).data())
#define LE_DELETEFILECOMMAND(x) system((std::string("del ") + x).data())
#define LE_DELETEDIRECTORYCOMMAND(x) system((std::string("rmdir /s /q ") + x).data())
#define LE_RENAMECOMMAND(x, y) system((std::string("ren ") + x + " " + y).data())
#define LE_MOVECOMMAND(x, y) system((std::string("move ") + x + " " + y).data())
#define LE_COPYFILECOMMAND(x, y) system((std::string("copy ") + x + " " + y).data())
#define LE_COPYDIRECTORYCOMMAND(x, y) system((std::string("xcopy ") + x + " " + y).data())

#else
#warning "Unsuported OS"
#endif

namespace Le{


std::string readfile(const char* path, int firstLine = 0, int finalLine = -1){
    std::ifstream file;
    file.open(path);
    std::string str, dummy;
    LE_PATH_OPENING_ERROR_CHECK(!file.is_open() || file.fail(), path);
    int line = 0;
    if(finalLine > -1){
        while(std::getline(file, dummy)){
            if(line <= finalLine && line++ >= firstLine){
                str += dummy + "\n";
            }
        }
        return str;
    }
    while(std::getline(file, dummy)){
        if(line++ >= firstLine){
            str += dummy + "\n";
        }
    }
    return str;
}

void writefile(const char* path, const char* data){
    std::ofstream file(path);
    LE_PATH_OPENING_ERROR_CHECK(!file.is_open() || file.fail(), path);
    file << data;
}

unsigned int char_count_to_line(const char* path, unsigned int line){
    std::ifstream file(path);
    LE_PATH_OPENING_ERROR_CHECK(!file.is_open() || file.fail(), path);
    std::string dummy;
    while(std::getline(file, dummy) && line-- > 0);
    return file.gcount();
}

std::string getextension(const char* path){
    if(std::filesystem::is_directory(path))
        return "";
    std::filesystem::path file(path);
    return file.has_extension()? file.extension().string() : "";
}

void write_dir_contents(const char* path, std::vector<std::string>& vec){
    LE_PATH_OPENING_ERROR_CHECK(!std::filesystem::is_directory(path), path);
    vec.clear();
    std::vector<std::string> dummy;
    for(const auto& obj: std::filesystem::directory_iterator(path)){
        obj.is_directory() ? vec.push_back(obj.path().filename().string()) :
        dummy.push_back(obj.path().filename().string());
    } std::sort(vec.begin(), vec.end());
    std::sort(dummy.begin(), dummy.end());
    for(const std::string& str: dummy) vec.push_back(str);
}


};