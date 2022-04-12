#include <iostream>
#include "acquire-method.h"
#include "stanza.h"

static void printCapabilities()
{
	std::cout << "100 Capabilities" << std::endl;
	std::cout << "Version: 1.0" << std::endl;
	std::cout << "Single-Instance: true" << std::endl;
	std::cout << std::endl;
}

/*
 * Checking of the messages is based on the code used in the official APT helpers:
 * https://salsa.debian.org/apt-team/apt/-/blob/main/apt-pkg/acquire-method.cc#L89
 */
static void sendMessage(const std::string &message,
			const std::map<std::string, std::string> &fields)
{
	auto checkKey = [](const std::string &s) {
		 return std::all_of(s.begin(), s.end(), [](unsigned char c) -> bool {
		 	/* alphanumeric, hyphen or a space */
			return std::isalnum(c) || c == '-' || c == ' ';
		 });
	};

	auto checkValue = [](const std::string &s) {
		 return std::all_of(s.begin(), s.end(), [](unsigned char c) -> bool {
		 	/* unicode characters, printable characters or a tab */
			return c > 127 || (c > 31 && c < 127) || c == '\t';
		 });
	};

	for (const auto &[key, val] : fields) {
		if (!checkKey(key) || !checkValue(val)) {
			sendMessage("400 URI Failure", {
				{"URI", "<UNKNOWN>"},
				{"Message", "SECURITY: Message contains control characters, rejecting."}
			});
			abort();
		}
	}

	std::cout << message << std::endl;
	for (const auto &[key, val] : fields) {
		if (val.length() == 0)
			continue;

		std::cout << key << ": " << val << std::endl;
	}

	std::cout << std::endl;
}

void AcquireMethod::reportGeneralFailure(const std::string &message)
{
	sendMessage("401 General Failure", {{"Message", message}});
}

int AcquireMethod::acquire(std::istream &in)
{
	std::string uri;
	Stanza stanza(in);

	uri = stanza["URI"];

	/* TODO */

	sendMessage("201 URI Done", {{"URI", uri}});

	return 0;
}

int AcquireMethod::loop()
{
	std::string line;
	unsigned int code;
	int retval = 0, ret;

	printCapabilities();

	while (std::cin.good()) {
		try {
			getline(std::cin, line);
			if (line.length() == 0) {
				break;
			}

			code = std::stoi(line);

			switch (code) {
			case 600:
				ret = acquire(std::cin);
				if (ret != 0) {
					retval = ret;
				}
				break;
			default:
				throw std::invalid_argument("Unexpected command.");
				break;
			}

		} catch (const std::invalid_argument &e) {
			reportGeneralFailure(e.what());
			return -1;

		} catch (const std::exception &e) {
			reportGeneralFailure("Unexpected error.");
			return -1;
		}
	}

	return retval;
}
