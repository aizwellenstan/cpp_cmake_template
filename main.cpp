#include <iostream>
// #include "adder.h"
#include <string>

//file
#include <filesystem>
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
#include <sstream>

#include <windows.h>
#include <shlobj.h>

// std::string sSelectedFile;
// std::string sFilePath;

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{

    if(uMsg == BFFM_INITIALIZED)
    {
        std::string tmp = (const char *) lpData;
        std::cout << "path: " << tmp << std::endl;
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }

    return 0;
}

std::string BrowseFolder(std::string saved_path)
{
    TCHAR path[MAX_PATH];

    const char * path_param = saved_path.c_str();

    BROWSEINFO bi = { 0 };
    bi.lpszTitle  = ("Browse for folder...");
    bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn       = BrowseCallbackProc;
    bi.lParam     = (LPARAM) path_param;

    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

    if ( pidl != 0 )
    {
        //get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, path );

        //free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }

        return path;
    }

    return "";
}

// bool openFile()
// {
//     //  CREATE FILE OBJECT INSTANCE
//     HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
//     if (FAILED(f_SysHr))
//         return FALSE;

//     // CREATE FileOpenDialog OBJECT
//     IFileOpenDialog* f_FileSystem;
//     f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
//     if (FAILED(f_SysHr)) {
//         CoUninitialize();
//         return FALSE;
//     }

//     //  SHOW OPEN FILE DIALOG WINDOW
//     f_SysHr = f_FileSystem->Show(NULL);
//     if (FAILED(f_SysHr)) {
//         f_FileSystem->Release();
//         CoUninitialize();
//         return FALSE;
//     }

//     //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
//     IShellItem* f_Files;
//     f_SysHr = f_FileSystem->GetResult(&f_Files);
//     if (FAILED(f_SysHr)) {
//         f_FileSystem->Release();
//         CoUninitialize();
//         return FALSE;
//     }

//     //  STORE AND CONVERT THE FILE NAME
//     PWSTR f_Path;
//     f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
//     if (FAILED(f_SysHr)) {
//         f_Files->Release();
//         f_FileSystem->Release();
//         CoUninitialize();
//         return FALSE;
//     }

//     //  FORMAT AND STORE THE FILE PATH
//     std::wstring path(f_Path);
//     std::string c(path.begin(), path.end());
//     sFilePath = c;

//     //  FORMAT STRING FOR EXECUTABLE NAME
//     const size_t slash = sFilePath.find_last_of("/\\");
//     sSelectedFile = sFilePath.substr(slash + 1);

//     //  SUCCESS, CLEAN UP
//     CoTaskMemFree(f_Path);
//     f_Files->Release();
//     f_FileSystem->Release();
//     CoUninitialize();
//     return TRUE;
// }

int main() {
    // std::string path = BrowseFolder("I:/");
    // std::cout << path << std::endl;
    bool result = FALSE;
    result = openFile();
    switch (result) {
        case(TRUE): {
            printf("SELECTED FILE: %s\nFILE PATH: %s\n\n", sSelectedFile.c_str(), sFilePath.c_str());
            system("pause");
        }
        case(FALSE): {
            printf("ENCOUNTERED AN ERROR: (%d)\n", GetLastError());
            system("pause");
        }
    }
    // std::cout << "CMake\n";

    // std::cout << add(1.2f, 2.1f) << '\n';
    // MessageBox( nullptr, TEXT( "The driver is sleeping!!" ), TEXT( "Message" ), MB_OK );
    std::string sourcePath = "";
    // std::cout << "Enter copy path: ";
    // std::cin >> sourcePath;
    std::cout << "path: " << sourcePath << std::endl;
    std::string basePath = "I:\\tiok23yt\\shot\\";

    for (const auto& entry : recursive_directory_iterator(sourcePath))
        if (entry.path().extension().string() == ".mp4") {
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