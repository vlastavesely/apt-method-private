#ifndef __HASH_H
#define __HASH_H

#include <string>

std::string hash_sha1(const std::string &salt, const std::string &path);

#endif /* __HASH_H */
