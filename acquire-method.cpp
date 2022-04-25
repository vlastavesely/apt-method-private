/*
 * More information about the APT helpers:
 * http://www.fifi.org/doc/libapt-pkg-doc/method.html/ch2.html
 */
#include <iostream>
#include "acquire-method.h"
#include "stanza.h"

static void print_capabilities()
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
void AcquireMethod::send_message(const std::string &message,
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
			send_message("400 URI Failure", {
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

void AcquireMethod::report_general_failure(const std::string &message)
{
	send_message("401 General Failure", {{"Message", message}});
}

void AcquireMethod::report_uri_failure(const uri_exception &e)
{
	send_message("400 URI Failure", {
		{"URI", e.uri()},
		{"FailReason", e.reason()},
		{"Message", e.what()}
	});
}

int AcquireMethod::acquire(std::istream &in)
{
	Stanza stanza(in);
	int ret = 0;

	try {
		/* TODO handle the return code...? */
		ret = fetch_file(stanza);
		if (ret != 0) {
			throw std::invalid_argument("XXX"); // FIXME
		}

	} catch (const uri_exception &e) {
		report_uri_failure(e);
		return 100;
	}

	send_message("201 URI Done", {{"URI", stanza["URI"]}});

	return ret;
}

int AcquireMethod::loop()
{
	std::string line;
	unsigned int code;
	int retval = 0, ret;

	print_capabilities();

	config.load_from_file("config"); /* FIXME */

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
			report_general_failure(e.what());
			return -1;

		} catch (const std::exception &e) {
			report_general_failure("Unexpected error.");
			return -1;
		}
	}

	return retval;
}
