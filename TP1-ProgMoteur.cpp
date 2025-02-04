#include <fstream>
#include <iostream>
#include <json/json.h>
#include <windows.h>

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

            std::string command_line = ".\\Engine\\Build\\BatchFiles\\Build.bat " + project_name + "Editor Win64 Development " + project_path + " -waitmutex";

            std::cout << command_line << '\n';
            
            if (std::system(command_line.c_str()))
            {
                std::cout << "Command line error\n";
            }
            
        } else if (command == "package")
        {

            // -ScriptsForProject=C:/Users/Yvain/UE5.5/UnrealEngine/TestSource/TestSource.uproject BuildCookRun -project=C:/Users/Yvain/UE5.5/UnrealEngine/TestSource/TestSource.uproject -noP4 -clientconfig=Development -serverconfig=Development -nocompileeditor -unrealexe=C:\Users\Yvain\UE5.5\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Cmd.exe -utf8output -platform=Win64 -build -cook -map=ThirdPersonMap+ThirdPersonMap -CookCultures=fr -unversionedcookedcontent -stage -package -cmdline="ThirdPersonMap -Messaging" -addcmdline="-SessionId=DFC0C1C74948523AD87B90BC0F0230BD -SessionOwner='Yvain' -SessionName='Build1'   "
            std::string archive_path = argv[3];
            std::string command_line = ".\\Engine\\Build\\BatchFiles\\RunUAT.bat -ScriptsForProject="+ project_path +" BuildCookRun -project="+ project_path +" -noP4 -clientconfig=Development -serverconfig=Development -nocompileeditor -unrealexe=UnrealEditor-Cmd.exe -utf8output -platform=Win64 -build -cook -map=ThirdPersonMap+ThirdPersonMap -CookCultures=fr -unversionedcookedcontent -stage -package -archive -archivedirectory="+archive_path;

            if (std::system(command_line.c_str()))
            {
                std::cout << "Command line error\n";
            }
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
