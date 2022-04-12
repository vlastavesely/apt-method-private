#include <fstream>
#include "config.h"
#include "stanza.h"

int Config::loadFromFile(const std::string &configFile)
{
	std::fstream in(configFile);
	std::string origin;

	while (in.good()) {
		Stanza stanza(in);

		origin = stanza["Origin"];
		if (origin.length() == 0)
			continue;

		for (auto &[key, val] : stanza) {
			fields["Private::" + origin + "::" + key] = val;
		}
	}

	return 0;
}

std::string Config::operator[](const std::string &key)
{
	return fields[key];
}
