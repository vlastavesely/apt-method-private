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

static void sendMessage(const std::string &message,
			const std::map<std::string, std::string> &fields)
{
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
