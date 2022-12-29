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
#include <cstdlib>
#include <regex>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1;
#include <experimental/filesystem> // or #include <filesystem> for C++17 and up
    
namespace fs = std::experimental::filesystem;

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

int createFolderIfNotExist(std::string folder) {
    if (!fs::is_directory(folder) || !fs::exists(folder)) { // Check if src folder exists
        fs::create_directory(folder); // create src folder
    }
    return 0;
}

int sendCopyCommand(std::string file, std::string destanation) {
    std::replace(file.begin(), file.end(), '/', '\\');
    std::ostringstream oss;
    oss << "xcopy " <<"\""<< file  << "\"" << " " << destanation << " /Y"; // prevent space in source path
    std::string command = oss.str();
    std::cout << command << std::endl;
    // system(command.c_str());
    return 0;
}

int writeSourceFileToTxt(std::string fPath, std::string shotFolder) {
    std::ofstream myfile (shotFolder+"\\filePath.txt", std::ios_base::app);
    if (myfile.is_open())
    {
        myfile << fPath+"\n";
        myfile.close();
    }
    else std::cout << "Unable to open file";
    return 0;
}

int copyFile(std::string fPath, std::string bkpRootFolder) {
    std::vector<std::string> result = {};
    std::stringstream  data(fPath);
    std::string line;
    while(std::getline(data,line,'\\'))
    {
        result.push_back(line); // Note: You may get a couple of blank lines
                                // When multiple underscores are beside each other.
    }

    
    // for(std::string i : result) 
    //     std::cout << i << std::endl;
    
    // if (result.size() < 6)
    //     std::cout << "FAILED " << fPath << std::endl;
    //     return -1;
    std::cout << fPath << std::endl;
    std::string seq = result[5];
    std::string shot = result[6];

    std::string seqFolder = bkpRootFolder + "\\" + seq;
    std::string shotFolder = seqFolder + "\\" + shot;

    std::cout << fPath << std::endl;

    createFolderIfNotExist(seqFolder);
    createFolderIfNotExist(shotFolder);

    std::vector<std::string> refResult = getReference(fPath);
    for(std::string i : refResult) {
        std::cout << i << std::endl;
        sendCopyCommand(i, shotFolder);
        writeSourceFileToTxt(i, shotFolder);
    }
    

    return 0;
}

int getFileVersion(std::string fPath) {
    int version = 0;
    std::vector<std::string> result = {};
    std::stringstream  data(fPath);
    std::string line;
    while(std::getline(data,line,'_'))
    {
        result.push_back(line); // Note: You may get a couple of blank lines
                                // When multiple underscores are beside each other.
    }
    std::string res = result[result.size()-1].c_str();
    std::regex str_expr ("v([0-9])([0-9])([0-9])");
    if (regex_match (res,str_expr)) {
        res = res.erase(0,1);
        int ver = atoi(res.c_str());
        return ver;
    } else {
        // std::cout << "REGEX ERR "<< res << std::endl;
        return -1;
    }
}

int main(int argc, const char**argv) {
    std::string proj = "vd2";
    std::string rootPath = "J:\\"+proj+"\\work\\prod\\lig";
    // std::string bkpFolder = "J:\\"+proj+"\\work\\prod\\lig\\pkg";
    std::string bkpFolder = "J:\\vd2\\work\\ftp\\TEST\\pkg2";

    for (const auto& seq : fs::directory_iterator(rootPath)) {
        if (fs::is_directory(seq)) {
            const auto seqStr = seq.path().filename().string();
            if (seqStr == "CharacterTest") continue;
            for (const auto& shot : fs::directory_iterator(seq)) {
                if (fs::is_directory(shot)) {
                    const auto shotStr = shot.path().filename().string();
                    
                    int latestVersion = 0;
                    std::string latestFilePath = "";
                    for (const auto& scene : fs::directory_iterator(shot.path().string()+"\\scenes")) {
                        if (fs::is_regular_file(scene)) {
                            if (scene.path().extension().string() != ".ma") continue;
                            const auto sceneStr = scene.path().filename().string();
                            // std::cout << "seq:  " << seqStr << '\n';
                            // std::cout << "shot:  " << shotStr << '\n';
                            // std::cout << "scene:  " << sceneStr << '\n';
                            std::string fPath = scene.path().filename().string();
                            // std::cout << fPath << std::endl;

                            size_t lastindex = fPath.find_last_of("."); 
                            std::string rawname = fPath.substr(0, lastindex); 

                            int version = getFileVersion(rawname);
                            if (version >= latestVersion) {
                                latestVersion = version;
                                latestFilePath = scene.path().string();
                            }
                            // std::cout << scene.path().string() << " " << version << " " << latestVersion <<std::endl;
                            // std::cout << latestVersion << " " << latestFilePath << std::endl;
                        }
                    }
                    
                    if (latestFilePath != "") {
                        // std::cout << latestVersion << " " << latestFilePath << std::endl;
                        copyFile(latestFilePath, bkpFolder);
                    }
                        
                }
            } 
        }
    }
    // for (const auto& entry : recursive_directory_iterator(rootPath))
    // {
    //     if (entry.path().extension().string() != ".ma") continue;
    //     std::string fPath = entry.path().string();

    //     copyFile(fPath, bkpFolder);
    // }


    // // std::string fPath = "D:\\vd2_s062_280_lig_v004.ma";
    // std::string fPath = "D:\\vd2_s026b_010_lig_v009.ma";
    
    // std::vector<std::string> result = getReference(fPath);

    // // std::cout << result[0] << std::endl;
    // for(std::string i : result) 
    //     std::cout << i << std::endl;
    
	return 0;
    



    
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