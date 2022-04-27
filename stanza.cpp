#include <iostream>
#include "stanza.h"

int Stanza::parse_header(const std::string &line)
{
	std::string key, val;
	size_t pos;

	pos = line.find(':');
	if (pos == std::string::npos)
		return -1;

	key = line.substr(0, pos);
	val = line.substr(pos + 2);

	fields[key] = val;

	return 0;
}

std::string Stanza::operator[](const std::string &key)
{
	return fields[key.data()];
}

std::map<std::string, std::string>::iterator Stanza::begin() {
	return fields.begin();
};

std::map<std::string, std::string>::iterator Stanza::end() {
	return fields.end();
};

Stanza::Stanza(std::istream &in)
{
	std::string line;
	int ret;

	if (in.fail()) {
		throw std::invalid_argument("Failed to read a stanza.");
	}

	while (in.good()) {
		std::getline(in, line);

		if (line.length() == 0)
			break;

		ret = parse_header(line);
		if (ret != 0) {
			throw std::invalid_argument("Invalid input.");
		}
	}
}
