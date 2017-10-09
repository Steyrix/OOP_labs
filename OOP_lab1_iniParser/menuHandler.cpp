#include "menuHandler.hpp"
#include <iostream>
using namespace std;

MenuHandler::MenuHandler()
{
    helpInfoFile = "comList.txt";
    helpInfo.open(helpInfoFile);
    
    string line;
    while(getline(helpInfo,line))
        helpText = helpText + line + "\n";
    
    helpInfo.close();
}

MenuHandler::~MenuHandler()
{
}

void MenuHandler::startMenu()
{
    getCommand();
}

void MenuHandler::initParser(const string &fileName)
{
    parser.initialize(fileName);
    if(parser.isParsed())
    {
        cout << "File " << fileName << " is succesfully parsed." << endl;
        lastFile = fileName;
    }
    else
        cout << "File is not parsed :(" << endl;
}

void MenuHandler::exSection(const string &sectionName) const
{
    if(parser.existSection(sectionName))
        cout << "Section " << sectionName << " exists in the file" << endl;
    else
        cout << "Section " << sectionName << " doesn't exist in the file" << endl;
}

void MenuHandler::exParam(const string &sectionName, const string &paramName) const
{
    if(parser.existParameter(sectionName, paramName))
        cout << "Parameter " << sectionName << " exists in the file" << endl;
    else
        cout << "Parameter " << sectionName << " doesn't exist in the file" << endl;
}

void MenuHandler::getVal(const string &sectionName, const string &paramName, const string &type) const
{
    if(type == "STR")
        cout << parser.getValue<string>(sectionName, paramName) << endl;
    else if (type == "INT")
        cout << parser.getValue<int>(sectionName, paramName) << endl;
    else if (type == "FLT")
        cout << parser.getValue<float>(sectionName, paramName) << endl;
    else
        cout << "Unknown type of parameter's value! Please try INT,FLT,STR." << endl;
}

void MenuHandler::makeParam(const string &sectionName, const string &paramName, const string &value)
{
    parser.makeParameter(sectionName, paramName, value);
    cout << "Parameter " << paramName << "is succesfully added to section " << sectionName << endl;
}

void MenuHandler::getCommand()
{
    string com, arg1, arg2, arg3;
    
    cout << "Input command (type HELP for hints):" << endl;
    while(com != "exit" && com != "EXIT")
    {
        cin >> com;
        if (com == "HELP")
            cout << helpText << endl;
        else if (com == "LAST")
            cout << "Current file parsed is " << lastFile << endl;
        else if (com == "INIT")
        {
            cin >> arg1;
            initParser(arg1);
        }
        else if (com == "EXSEC")
        {
            cin >> arg1;
            exSection(arg1);
        }
        else if (com == "EXPAR")
        {
            cin >> arg1 >> arg2; //arg2 is parameter's name, arg1 is section's name
            exParam(arg1, arg2);
        }
        else if (com == "GETVAL")
        {
            cin >> arg1 >> arg2 >> arg3; //arg3 is typename
            getVal(arg1, arg2, arg3);
        }
        else if (com == "MAKE")
        {
            cin >> arg1 >> arg2 >> arg3;
            makeParam(arg1, arg2, arg3);
        }
        else if (com == "SHOW")
        {
            parser.show();
        }
        else if (com == "EXIT" || com == "exit")
            continue;
        else
            cout << "Unknown command. Type HELP for manual." << endl;
    }
    
}

