#include <stdexcept>
#include "origin.h"
#include "hex.h"

Origin::Origin(Config &config, const std::string &origin)
{
	std::string hexKey;

	name = origin;

	uri = config["Private::" + origin + "::URI"];
	if (uri == "") {
		throw std::invalid_argument("No URI defined for origin ‘" + origin + "’.");
	}

	salt = config["Private::" + origin + "::Salt"];
	if (salt == "") {
		throw std::invalid_argument("No salt defined for origin ‘" + origin + "’.");
	}

	hexKey = config["Private::" + origin + "::Key"];
	if (hexKey == "") {
		throw std::invalid_argument("No key defined for origin ‘" + origin + "’.");
	}

	hash = config["Private::" + origin + "::Hash"];
	if (hash == "") {
		hash = "sha1";
	}

	cipher = config["Private::" + origin + "::Cipher"];
	if (cipher == "") {
		cipher = "cbc(aes256)";
	}

	if (hexDecode(key, hexKey) != 0) {
		throw std::invalid_argument("Failed to parse the encryption key.");
	}
}
