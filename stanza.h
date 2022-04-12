#ifndef __STANZA_H
#define __STANZA_H

#include <iostream>
#include <string>
#include <map>
#include <iterator>

class Stanza {
private:
	std::map<std::string, std::string> fields;
	int parseHeader(const std::string &line);
public:
	Stanza(std::istream &in);
	std::string operator[](const std::string &key);
	std::map<std::string, std::string>::iterator begin();
	std::map<std::string, std::string>::iterator end();
};

#endif /* __STANZA_H */
