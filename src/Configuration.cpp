
#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include <sys/stat.h>
const std::string Configuration::_whitespace = " \t";
const std::string Configuration::_comment = "#";
const std::string Configuration::_seperator = ":";
Configuration::Configuration(std::string confPath) : _confPath(confPath)
{
	struct stat buffer;   
  	if(stat (confPath.c_str(), &buffer) != 0)
  		throw std::runtime_error("config file '"+confPath+"' not found");
  	std::ifstream fis(confPath.c_str());
  	if(!fis.is_open())
  		throw std::runtime_error("Unable to open file:'"+confPath+"'");
  	std::string line;
	while(std::getline(fis,line))
	{
		TrimLine(line);
		if(line.empty() || line.at(0) == '#')
			continue;
		RemoveComment(line);
		TrimLine(line);
		SaveToConfig(line);
		std::cout<<line<<std::endl;
	}
	if(!ValidateConfig())
		throw std::runtime_error("Some configuration parameter are not provided");
}
Configuration::~Configuration()
{
}
void Configuration::TrimLine(std::string& str)
{
    std::size_t charFirst=str.find_first_not_of(_whitespace);
    if (charFirst == std::string::npos)
    {
        str = "";
        return;
    }
    std::size_t charLast=str.find_last_not_of(_whitespace);
    str=str.substr(charFirst, charLast-charFirst+1);
}
void Configuration::RemoveComment(std::string& str)
{
    std::size_t charFirst=str.find_first_of(_comment);
    if (charFirst == std::string::npos)
    	return;
    str = str.substr(0,charFirst);
}
void Configuration::SaveToConfig(std::string& str)
{
	std::size_t charFirst=str.find_first_of(_seperator);
    if (charFirst == std::string::npos)//nothing to do.can warn if required
    	return;
    _config[str.substr(0,charFirst)]=str.substr(charFirst+1,str.length()-1);
//    std::cout<<"--"<<str.substr(0,charFirst)<<"::"<<str.substr(charFirst+1,str.length()-1)<<"__"<<std::endl;
}
bool Configuration::ValidateConfig()
{
	return true;
}
std::string Configuration::getValue(const std::string& key) const
{
	ConfigMap::const_iterator valueIt = _config.find(key);
	if(valueIt == _config.end())
		throw std::invalid_argument(key+" not found in configuration");
	return valueIt->second;
}
std::string Configuration::getValue(const char* key) const
{
//	ConfigMap::iterator valueIt = _config.find(key);
//	if(valueIt == _config.end())
//		throw std::invalid_argument(std::string(key)+" not found in configuration");
	return getValue(key);
}

