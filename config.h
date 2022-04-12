#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
#include <map>

class Config {
private:
	std::map<std::string, std::string> fields;
public:
	int loadFromFile(const std::string &configFile);
	std::string operator[](const std::string &key);
};

#endif /* __CONFIG_H */
