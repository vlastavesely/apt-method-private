#include <fstream>
#include "config.h"
#include "stanza.h"

int Config::load_from_file(const std::string &config_file)
{
	std::ifstream in(config_file);
	std::string origin;

	if (in.fail()) {
		throw std::invalid_argument("Failed to read the config.");
	}

	while (in.good()) {
		Stanza stanza(in);

		origin = stanza["Origin"];
		if (origin == "")
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
