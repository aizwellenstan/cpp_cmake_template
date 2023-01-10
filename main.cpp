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
#include <algorithm>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1;
#include <experimental/filesystem> // or #include <filesystem> for C++17 and up
namespace fs = std::experimental::filesystem;

std::string NormalizePath(const std::string& messyPath) {
    std::filesystem::path path(messyPath);
    std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
    std::string npath = canonicalPath.make_preferred().string();
    return npath;
}

std::vector<std::string> AppendToVector(std::vector<std::string> v, std::string x) {
    if(!(std::find(v.begin(), v.end(), x) != v.end())) {
        v.push_back(x);
    }
    return v;
}

int CreateFolderIfNotExist(std::string folder) {
    if (!fs::is_directory(folder) || !fs::exists(folder)) { // Check if src folder exists
        std::ostringstream oss;
        oss << "mkdir " + folder; // prevent space in source path
        std::string command = oss.str();
        std::cout << command << std::endl;
        system(command.c_str());
    }
    return 0;
}

int CopyFileAndFolderCommand(std::string file, std::string destanation, bool copyRootFolder=false) {
    CreateFolderIfNotExist(destanation);
    std::replace(file.begin(), file.end(), '/', '\\');
    std::ostringstream oss;
    std::string folderName = "";
    if (copyRootFolder) {
        std::vector<std::string> result = {};
        std::stringstream  data(file);
        std::string line;
        while(std::getline(data,line,'\\'))
            result.push_back(line);
        folderName = result[result.size()-1].c_str();
    }
    
    if (fs::is_directory(file)) {
        if (!copyRootFolder)
            oss << "xcopy " <<"\""<< file << "\"" << " " << destanation << "\\" <<"*" << " /Y /I /S /J /A"; // prevent space in source path
        else
            oss << "xcopy " <<"\""<< file << "\"" << " " << destanation <<  "\\" << folderName << "\\" <<"*" << " /Y /I /S /J /A"; // prevent space in source path
    } else {
        oss << "xcopy " <<"\""<< file  << "\"" << " " << destanation << "\\" << "*" << " /Y /A"; // prevent space in source path
    } // is file
    std::string command = oss.str();
    std::cout << command << std::endl;
    system(command.c_str());
    return 0;
}

std::string getPath(std::string s) {
    std::vector<std::string> result = {};
    std::stringstream  data(s);
    std::string line;
    while(std::getline(data,line,' '))
        result.push_back(line);
    s = result[result.size()-1];
    s.erase(remove(s.begin(), s.end(), '\"'),s.end()); // remove double quotes
    s = NormalizePath(s);
    return s;
}

std::vector<std::string> getReference(std::string s) {
    std::vector<std::string> result = {};
    std::ifstream file(s);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line, ';')) {
            if (line.find("J:") != std::string::npos) {
                line = getPath(line);
                if (line == "") continue;
                result.push_back(line);
            }  
        }
        file.close();
    }
    return result;
}

std::string getOriginPath(std::string s) {
    std::vector<std::string> result = {};
    std::stringstream  data(s);
    std::string line;
    while(std::getline(data,line,' '))
        result.push_back(line);
    s = result[result.size()-1];
    s.erase(remove(s.begin(), s.end(), '\"'),s.end()); // remove double quotes
    return s;
}

std::vector<std::string> getOriginReference(std::string s) {
    std::vector<std::string> result = {};
    std::ifstream file(s);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line, ';')) {
            if (line.find("J:") != std::string::npos) {
                line = getOriginPath(line);
                if (line == "") continue;
                result.push_back(line);
            }  
        }
        file.close();
    }
    return result;
}

int writeSourceFileToTxt(std::string fPath, std::string shotFolder) {
    CreateFolderIfNotExist(shotFolder);
    std::ofstream myfile (shotFolder+"\\filePath.txt", std::ios_base::app);
    if (myfile.is_open())
    {
        myfile << fPath+"\n";
        myfile.close();
    }
    else std::cout << "Unable to open file";
    return 0;
}

std::string getFileName(std::string s) {
    std::vector<std::string> result = {};
    std::stringstream  data(s);
    std::string line;
    while(std::getline(data,line,'/'))
        result.push_back(line);
    s = result[result.size()-1];
    return s;
}

std::string getAssName(std::string s) {
    std::vector<std::string> result = {};
    std::stringstream  data(s);
    std::string line;
    while(std::getline(data,line,'.'))
        result.push_back(line);
    s = result[0];
    return s;
}

std::string ReplaceString(std::string line, std::string wordToReplace, std::string wordToReplaceWith) {
    size_t len = wordToReplace.length();
    size_t pos = line.find(wordToReplace);
    if (pos != std::string::npos)
        line.replace(pos, len, wordToReplaceWith);
    return line;
}

int replaceReference(std::string inPath, std::string outPath, std::vector<std::string> refResult, std::string wordToReplaceWith) {
    std::string wordToReplace = "";
    std::string fileName = "";
    std::vector<std::vector<std::string>> v;
    for(std::string i : refResult) {
        std::filesystem::path fPath = NormalizePath(i);
        if (fPath.extension().string() == ".ocio") continue;
        if (fPath.extension().string() == ".ass") {
            wordToReplace = i;
            fileName = getFileName(i);
            fileName = getAssName(fileName) + "/" + fileName;
            wordToReplace = regex_replace(wordToReplace, std::regex(fileName), "");
            wordToReplaceWith = "./cache/ass/";
        } else if (fPath.extension().string() == ".abc") {
            wordToReplace = i;
            wordToReplace = regex_replace(wordToReplace, std::regex(getFileName(i)), "");
            wordToReplaceWith = "./cache/alembic/";
        } else if (fPath.extension().string() == ".exr" || fPath.extension().string() == ".tif" || fPath.extension().string() == ".jpg") {
            wordToReplace = i;
            wordToReplace = regex_replace(wordToReplace, std::regex(getFileName(i)), "");
            wordToReplaceWith = "./sourceimages/";
        } else if (fPath.extension().string() == ".vdb") {
            wordToReplace = i;
            wordToReplace = regex_replace(wordToReplace, std::regex(getFileName(i)), "");
            wordToReplaceWith = "./cache/vdb/";
        }
        if(wordToReplace == "") continue;

        v.push_back({wordToReplace, wordToReplaceWith});
    }
    std::ifstream in(inPath);
    std::ofstream out(outPath);
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)) {
            if (line.find("J:") != std::string::npos) {
                if (line.find("ocio") != std::string::npos) continue;
                for(int i=0;i<v.size();i++){
                    wordToReplace = v[i][0];
                    wordToReplaceWith = v[i][1];
                    std::cout << line << " "<< wordToReplace << " " << wordToReplaceWith << std::endl;
                    line = ReplaceString(line, wordToReplace, wordToReplaceWith);
                }
            }
            out << line << '\n';
        }
        in.close();
    }
    return 0;
}

int writeRelativePathMa(std::vector<std::string> refResult, std::string fPath, std::string shotFolder, std::string originalPath) {
    CreateFolderIfNotExist(shotFolder);
    std::vector<std::string> result = {};
    std::stringstream  data(fPath);
    std::string line;
    while(std::getline(data,line,'\\'))
        result.push_back(line);
    std::string res = result[result.size()-1].c_str();
    std::string finalPath = shotFolder + "\\" + res;
    replaceReference(fPath, finalPath, refResult, ".");
    return 0;
}

int copyFile(std::string fPath, std::string bkpRootFolder) {
    std::vector<std::string> result = {};
    std::stringstream  data(fPath);
    std::string line;
    while(std::getline(data,line,'\\'))
        result.push_back(line);

    // WARNING PLEASE CHANGE HERE WITH ACTUAL FOLDER PATH
    // result.size()-1 vd2_ms025_010a_lig_v001.ma
    // result.size()-2 scenes
    std::string seq = result[result.size()-4];
    std::string shot = result[result.size()-3];

    std::string seqFolder = bkpRootFolder + "\\" + seq;
    std::string shotFolder = seqFolder + "\\" + shot;
    std::string assFolder = shotFolder + "\\cache\\ass";
    std::string abcFolder = shotFolder + "\\cache\\alembic";
    std::string imgFolder = shotFolder + "\\sourceimages";
    std::string vdbFolder = shotFolder + "\\cache\\vdb";

    std::vector<std::string> sceneList = {};
    std::vector<std::string> assList = {};
    std::vector<std::string> abcList = {};
    std::vector<std::string> imgList = {};
    std::vector<std::string> vdbList = {};

    std::vector<std::string> refResult = getReference(fPath);

    for(std::string i : refResult) {
        std::filesystem::path rfPath = i;
        if (rfPath.extension().string() == ".ocio") continue;
        if (rfPath.extension().string() == ".ass") {
            assList = AppendToVector(assList, rfPath.parent_path().string());
        } else if (rfPath.extension().string() == ".abc") {
            abcList = AppendToVector(abcList, rfPath.parent_path().string());
        } else if (rfPath.extension().string() == ".exr" || rfPath.extension().string() == ".tif" || rfPath.extension().string() == ".jpg") {
            imgList = AppendToVector(imgList, rfPath.string());
        } else if (rfPath.extension().string() == ".vdb") {
            vdbList = AppendToVector(vdbList, rfPath.parent_path().string());
        } else {
            sceneList = AppendToVector(sceneList, rfPath.string());
        }
        writeSourceFileToTxt(i, shotFolder);
    }

    std::vector<std::string> originRefResult = getOriginReference(fPath);
    writeRelativePathMa(originRefResult, fPath, shotFolder, fPath);

    // return 0;

    for(std::string i : sceneList)
        CopyFileAndFolderCommand(i, shotFolder);
    for(std::string i : assList)
        CopyFileAndFolderCommand(i, assFolder, true);
    for(std::string i : abcList)
        CopyFileAndFolderCommand(i, abcFolder);
    for(std::string i : imgList)
        CopyFileAndFolderCommand(i, imgFolder);
    for(std::string i : vdbList)
        CopyFileAndFolderCommand(i, vdbFolder);
    return 0;
}

int getFileVersion(std::string fPath) {
    int version = 0;
    std::vector<std::string> result = {};
    std::stringstream  data(fPath);
    std::string line;
    while(std::getline(data,line,'_'))
        result.push_back(line);
    std::string res = result[result.size()-1].c_str();
    std::regex str_expr ("v([0-9])([0-9])([0-9])");
    if (regex_match (res,str_expr)) {
        res = res.erase(0,1);
        int ver = atoi(res.c_str());
        return ver;
    } else return -1;
}

std::vector<std::string> WordToVector(std::string s) {
    std::string tmp; 
    std::stringstream ss(s);
    std::vector<std::string> words;

    while(getline(ss, tmp, ',')){
        words.push_back(tmp);
    }
    return words;
}

int main(int argc, const char**argv) {
    std::string projPath;
    std::string bkpFolder;
    std::string seqListStr;
    std::string shotListStr;

    std::ifstream fin("config.txt");
    std::string line;
    std::istringstream sin;

    while (std::getline(fin, line)) {
        sin.str(line.substr(line.find("=")+1));
        if (line.find("projPath") != std::string::npos) {
            sin >> projPath;
        }
        sin.clear();

        if (line.find("bkpFolder") != std::string::npos) {
            sin >> bkpFolder;
        }

        if (line.find("seqList") != std::string::npos) {
            sin >> seqListStr;
        }

        if (line.find("shotList") != std::string::npos) {
            sin >> shotListStr;
        }
        sin.clear();
    }
    fin.close();

    std::cout << projPath << std::endl;
    std::cout << bkpFolder << std::endl;
    std::cout << seqListStr << std::endl;
    std::cout << shotListStr << std::endl;

    // how to read data
    // int iw = lconf["image_width"].as<int>();
    // std::string model = lconf["distortion_model"].as<std::string>();
    // std::vector<double> distdata = lconf["distortion_coefficients"]["data"].as<std::vector<double>>();
    
    // std::string proj = "vd2";
    // std::string projPath = "J:\\"+proj+"\\work\\prod\\lig";
    // \\isilon-nl\archive\packet\vd2\work\prod\lig2
    // std::string bkpFolder = "\\\\isilon-nl\\archive\\packet\\vd2\\work\\prod\\lig";
    // std::string bkpFolder = "\\\\isilon-nl\\archive\\packet\\vd2\\work\\prod\\lig2";
    // \\10.95.20.193\d$\lig
    // std::string bkpFolder = "\\\\10.95.20.193\\d$\\lig";

    // std::vector<std::string> seqList = {"ms025","s002","s009","s010","s011","s012","s013","s014","s015","s016","s019","s020",
    // "s025","s026","s026a"};
    // std::vector<std::string> seqList = {"s026b","s026c","s026d","s026e","s028","s029","s030","s032","s034","s036","s039","s040",
    // "s041","s052","s055"};
    // std::vector<std::string> seqList = {"s056","s059","s062","s063","s064","s066","s067","s067a","s068","s069","s070","s998","s999"};
    // std::vector<std::string> seqList = {"s016"};
    // std::vector<std::string> shotList = {"010","020"};

    std::vector<std::string> seqList = WordToVector(seqListStr);
    std::vector<std::string> shotList = WordToVector(shotListStr);

    for(std::string i : seqList)
        std::cout << i << std::endl;

    for(std::string i : shotList)
        std::cout << i << std::endl;

    for (const auto& seq : fs::directory_iterator(projPath)) {
        if (fs::is_directory(seq)) {
            const auto seqStr = seq.path().filename().string();
            if (seqStr == "CharacterTest") continue;
            if (seqList.size() > 0)
                if (!(std::find(seqList.begin(), seqList.end(), seqStr) != seqList.end())) continue;
            for (const auto& shot : fs::directory_iterator(seq)) {
                if (fs::is_directory(shot)) {
                    const auto shotStr = shot.path().filename().string();
                    if (shotList.size() > 0)
                        if (!(std::find(shotList.begin(), shotList.end(), shotStr) != shotList.end())) continue;
                    int latestVersion = 0;
                    std::string latestFilePath = "";
                    for (const auto& scene : fs::directory_iterator(shot.path().string()+"\\scenes")) {
                        if (fs::is_regular_file(scene)) {
                            if (scene.path().extension().string() != ".ma") continue;
                            const auto sceneStr = scene.path().filename().string();
                            std::string fPath = scene.path().filename().string();
                            size_t lastindex = fPath.find_last_of(".");
                            std::string rawname = fPath.substr(0, lastindex);
                            int version = getFileVersion(rawname);
                            if (version >= latestVersion) {
                                latestVersion = version;
                                latestFilePath = scene.path().string();
                            }
                        }
                    }
                    
                    if (latestFilePath != "")
                        copyFile(latestFilePath, bkpFolder);
                }
            }
        }
    }
    return 0;
}