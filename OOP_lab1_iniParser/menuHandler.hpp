#ifndef menuHandler_hpp
#define menuHandler_hpp
#include <fstream>
#include <map>
#include <stdio.h>
#include "iniParser.hpp"

class MenuHandler
{
public:
    MenuHandler();
    ~MenuHandler();
    void startMenu();
    void getCommand();
private:
    iniParser parser;
    std::ifstream helpInfo;
    std::string helpInfoFile;
    std::string helpText;
    std::string lastFile;
    void showHelp() const;
    void exSection(const std::string&sectionName) const;
    void exParam(const std::string &sectionName, const std::string &paramName) const;
    void getVal(const std::string &sectionName, const std::string &paramName, const std::string &type) const;
    void makeParam(const std::string &sectionName, const std::string &paramName, const std::string &value);
    void initParser(const std::string &fileName);
};

#endif /* menuHandler_hpp */
