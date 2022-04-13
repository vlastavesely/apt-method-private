#ifndef __URI_H
#define __URI_H

#include <string>

class URI final {
public:
	std::string whole_uri;
	std::string protocol;
	std::string origin;
	std::string path;
	URI(std::string uri);
	bool isProtocol(const std::string &proto);

	operator std::string() const {
		return whole_uri;
	}
};

#endif /* __URI_H */
