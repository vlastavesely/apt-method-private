#ifndef __STANZA_H
#define __STANZA_H

#include <iostream>
#include <string>
#include <map>

class Stanza {
private:
	std::map<std::string, std::string> fields;
	int parseHeader(const std::string &line);
public:
	Stanza(std::istream &in);
};

#endif /* __STANZA_H */
