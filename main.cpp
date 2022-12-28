#include <iostream>
// #include "adder.h"
#include <string>

//file
#include <filesystem>
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
#include <sstream>

#include <windows.h>
#include <shlobj.h>
#include <fstream>

std::string getPath(std::string s) {
    std::vector<std::string> result = {};
    std::stringstream  data(s);
    std::string line;
    while(std::getline(data,line,' '))
    {
        result.push_back(line); // Note: You may get a couple of blank lines
                                // When multiple underscores are beside each other.
    }
    return result[result.size()-1].c_str();
}

std::vector<std::string> getReference(std::string s) {
    std::vector<std::string> result = {};
    std::ifstream file(s);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line,';')) {
            if (line.find("J:") != std::string::npos) {
                line = getPath(line);
                result.push_back(line);
            }  
        }
        file.close();
    }
    return result;
}

int main() {
    // std::string fPath = "D:\\vd2_s062_280_lig_v004.ma";
    std::string fPath = "D:\\vd2_s026b_010_lig_v009.ma";
    
    std::vector<std::string> result = getReference(fPath);

    // std::cout << result[0] << std::endl;
    for(std::string i : result) 
        std::cout << i << std::endl;
    
	return 0;
    // std::string proj = "vd2";
    // std::string rootPath = "J:\\"+proj+"\\work\\prod\\lig";
    // for (const auto& entry : recursive_directory_iterator(rootPath))
    // {
    //     std::string fname = entry.path().stem().string();
    //     std::string fPath = entry.path().string();
    //     // std::cout << entry << std::endl;
    //     // std::cout << fname << std::endl;
    //     if (fPath.find("CharacterTest") != std::string::npos) continue;

    //     std::vector<std::string> result = {};
    //     std::stringstream  data(fPath);
    //     std::string line;
    //     while(std::getline(data,line,'\\'))
    //     {
    //         result.push_back(line); // Note: You may get a couple of blank lines
    //                                 // When multiple underscores are beside each other.
    //     }
        
    //     std::string seq = result[5];
    //     std::string shot = result[6];
    //     std::cout << seq << ", "<< shot<<std::endl;
    //     // std::cout << entry.path().string() << std::endl;
    //     // std::cout << seq << "," << shot << std::endl;
    // }



    
    // std::string sourcePath = "";
    // std::cout << "path: " << sourcePath << std::endl;
    // std::string basePath = "I:\\tiok23yt\\shot\\";

    // for (const auto& entry : recursive_directory_iterator(sourcePath))
    //     if (entry.path().extension().string() == ".mp4") {
    //         std::string fname = entry.path().stem().string();
    //         std::cout << entry << std::endl;
    //         std::cout << fname << std::endl;

    //         std::stringstream  data(fname);

    //         std::string line;
    //         std::vector<std::string> result = {};
    //         while(std::getline(data,line,'_'))
    //         {
    //             result.push_back(line); // Note: You may get a couple of blank lines
    //                                     // When multiple underscores are beside each other.
    //         }

    //         std::string destanation = basePath+result[0]+"\\"+result[1]+"\\"+result[2]+"\\04_comp\\finalimage\\02_anim\\"+entry.path().filename().string();
    //         // file = entry.path().filename().string();
            
    //         // std::cout << "xcopy " + entry.path().string() + " " + destanation << std::endl;
    //         // std::string command = "xcopy " +  +entry.path().string() + " " + destanation+"*";

    //         std::ostringstream oss;
    //         oss << "xcopy " <<"\""<<entry.path().string() << "\"" << " " << destanation << "*" << " /Y"; // prevent space in source path
    //         std::string command = oss.str();
    //         std::cout << command << std::endl;
    //         system(command.c_str());
    //     }

    return 0;
}