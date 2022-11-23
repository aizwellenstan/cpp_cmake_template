#include <iostream>
// #include "adder.h"
#include<string>

//file
#include <filesystem>
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
#include <sstream>

#include "lib\dirent.h"
#include <sys\stat.h>
#include <conio.h>

int main() {
    // std::cout << "CMake\n";

    // std::cout << add(1.2f, 2.1f) << '\n';

    std::string sourcePath = "";
    std::cout << "Enter copy path: ";
    std::cin >> sourcePath;
    std::cout << "path: " << sourcePath << std::endl;
    std::string basePath = "I:\\tiok23yt\\shot\\";

    for (const auto& entry : recursive_directory_iterator(sourcePath))
        // std::cout << dirEntry << std::endl;
        if (entry.path().extension().string() == ".mp4") {
            // std::cout << entry << std::endl;

            std::string fname = entry.path().stem().string();
            std::cout << entry << std::endl;
            std::cout << fname << std::endl;

            std::stringstream  data(fname);

            std::string line;
            std::vector<std::string> result = {};
            while(std::getline(data,line,'_'))
            {
                result.push_back(line); // Note: You may get a couple of blank lines
                                        // When multiple underscores are beside each other.
            }

            std::string destanation = basePath+result[0]+"\\"+result[1]+"\\"+result[2]+"\\04_comp\\finalimage\\02_anim\\"+entry.path().filename().string();
            // file = entry.path().filename().string();
            
            // std::cout << "xcopy " + entry.path().string() + " " + destanation << std::endl;
            // std::string command = "xcopy " +  +entry.path().string() + " " + destanation+"*";

            std::ostringstream oss;
            oss << "xcopy " <<"\""<<entry.path().string() << "\"" << " " << destanation << "*" << " /Y"; // prevent space in source path
            std::string command = oss.str();
            std::cout << command << std::endl;
            system(command.c_str());

            // system("DATE");

            // std::vector<std::string> result = explode(fname, '_');
            // for (size_t i = 0; i < result.size(); i++) {
            //     std::cout << "\"" << result[i] << "\"" << std::endl;
            // }
        }

    // struct dirent *d;
    // struct stat dst;

    // DIR *dr;

    // dr = opendir(sourcePath.c_str());

    // if (dr != NULL) 
    // {
    //     for (d = readdir(dr); d != NULL; d = readdir(dr)) 
    //     {
    //         std::string type = d->d_name;
    //         type = sourcePath + type;
    //         if (stat(type.c_str(), &dst) ==  0) 
    //         {
    //             if (dst.st_mode & S_IFDIR) 
    //             {
    //                 type = " is a FOLDER.";
    //             } 
    //             else if (dst.st_mode & S_IFREG) 
    //             {
    //                 type = " is a FILE.";
    //             }
    //         }
    //         std::cout << d->d_name + type << std::endl;
    //     }
    //     closedir(dr);
    // }

    return 0;
}

// #include<iostream>
// #include<string>
// using namespace std;
// int main() {
//     string s1 = "testtest";
//     string s2 = "";
//     string s3 = "TEST";

//     cout << "s1=" << s1 << ", s2=" << s2 << ", s3=" << s3 << endl;
//     return 0;
// }