#ifndef __ORIGIN_H
#define __ORIGIN_H

#include <string>
#include "config-file.h"

class Origin final {
public:
	std::string name;
	std::string uri;
	std::string salt;
	std::string hash;
	std::string cipher;
	unsigned char key[32];
	Origin(Config &config, const std::string &origin);
};

#endif /* __ORIGIN_H */
