#ifndef menuHandler_hpp
#define menuHandler_hpp
#include <fstream>
#include <map>
#include <stdio.h>
#include "iniParser.hpp"

class MenuHandler
{
private:
    std::unique_ptr<iniParser> parser;
    std::ifstream helpInfo;
    std::string helpInfoFile;
    std::string helpText;
    std::string lastFile;
    void showHelp() const;
    void exSection(std::string const &sectionName) const;
    void exParam(std::string const &sectionName, std::string const &paramName) const;
    void getVal(std::string const &sectionName, std::string const &paramName, std::string const &type) const;
    void makeParam(std::string const &sectionName, std::string const &paramName, std::string const &value);
    void initParser(std::string const &fileName);
public:
    MenuHandler();
    ~MenuHandler();
    void startMenu();
    void getCommand();
};

#endif /* menuHandler_hpp */
