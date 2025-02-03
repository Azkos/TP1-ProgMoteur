#include <fstream>
#include <iostream>
#include <json/json.h>
#include <windows.h>
#include <filesystem>

std::string get_file_name(const std::string& file_path)
{
    std::string file_full_name = file_path.substr(file_path.find_last_of("\\/") + 1);
    std::string::size_type const p(file_full_name.find_last_of('.'));
    return file_full_name.substr(0, p);
}

int main(int argc, char* argv[])
{
    
    if (argc >= 3)
    {
        std::string project_path = argv[1];
        std::string command = argv[2];
        std::string project_name = get_file_name(project_path);
        
        if (command == "show-infos")
        {
            std::ifstream uproject_infos(project_path, std::ifstream::binary);
            Json::Value infos;
            uproject_infos >> infos;
            std::cout << "Project name : " << project_name << '\n';
            if (infos["EngineAssociation"])
            {
                std::cout << "Unreal Version : " << infos["EngineAssociation"].asString() << '\n';
            } else
            {
                std::cout << "From Source" << '\n';
            }
            std::cout << "Plugins : " << '\n';
            for (auto& plugin : infos["Plugins"])
            {
                std::cout << " - " << plugin["Name"].asString() << '\n';
            }
        } else if (command == "build")
        {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            std::string command_line = ".\\Engine\\Build\\BatchFiles\\Build.bat " + project_name + " Win64 Development " + project_path + " -waitmutex";

            std::cout << command_line << '\n';

            if (std::system(command_line.c_str()))
            {
                std::cout << "Command line error\n";
            }
            
        } else if (command == "package")
        {
            
        } else
        {
            std::cout << argc << '\n';
            std::cout << "Command not recongized. " << '\n'
                    << "Availabe commands:" << '\n'
                    << " - MyTool [UPROJECT PATH] show-infos" << '\n'
                    << " - MyTool [UPROJECT PATH] build" << '\n'
                    << " - MyTool [UPROJECT PATH] package [PACKAGE PATH]" << '\n';
        }
    }
}
