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

int AcquireMethod::acquire(std::istream &in)
{
	Stanza stanza(in);

	/* TODO */

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

		} catch (const std::exception &e) {
			std::cout << "ERROR: " << e.what() << std::endl; /* FIXME */
			return -1;
		}
	}

	return retval;
}
