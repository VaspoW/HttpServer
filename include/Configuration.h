
#ifndef GS_MICROGRID_HMI_Configuration_H_
#define GS_MICROGRID_HMI_Configuration_H_

#include <map>
#include <string>

class Configuration
{
public:
	typedef std::map<std::string,std::string> ConfigMap;
	Configuration(std::string);
	~Configuration();
	std::string getValue(const std::string&) const;
	std::string getValue(const char* key) const;
private:
	void TrimLine(std::string& str);
	void RemoveComment(std::string& str);
	void SaveToConfig(std::string& str);
	bool ValidateConfig();
	static const std::string _whitespace;
	static const std::string _comment;
	static const std::string _seperator;
//	static const std::map<std::string,std::string> _defaultConfig;
	ConfigMap _config;
	std::string _confPath;
};/*class Configuration*/

#endif /*GS_MICROGRID_HMI_Configuration_H_*/

