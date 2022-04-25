#include <stdexcept>
#include "uri.h"

/*
 * Obviously, this could be done using a regex, but it would be slooooow...
 */
URI::URI(std::string uri)
{
	size_t pos;

	whole_uri = uri;

	pos = uri.find("://");
	if (pos == std::string::npos)
		throw std::invalid_argument("Invalid URI.");

	protocol = uri.substr(0, pos);
	uri.erase(0, pos + 3);

	pos = uri.find('/');
	if (pos == std::string::npos)
		throw std::invalid_argument("Invalid URI.");

	origin = uri.substr(0, pos);
	path = uri.substr(pos);
}

bool URI::is_protocol(const std::string &proto)
{
	return protocol == proto;
}
