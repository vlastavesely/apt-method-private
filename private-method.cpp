#include <iostream>
#include "private-method.h"

int PrivateMethod::loop()
{
	std::string line;
	unsigned int code;
	int ret = 0;

	while (std::cin.good()) {
		try {
			getline(std::cin, line);
			if (line.length() == 0) {
				break;
			}

			code = std::stoi(line);

			switch (code) {
			case 600:
				/* TODO - acquire */
				break;
			default:
				throw std::invalid_argument("aa");
				break;
			}

		} catch (const std::exception &e) {
			std::cout << "ERROR: " << e.what() << std::endl; /* FIXME */
			return -1;
		}
	}

	return ret;
}
