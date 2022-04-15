#ifndef __HASH_H
#define __HASH_H

#include <string>

std::string hashSha1(const std::string &salt, const std::string &path);

#endif /* __HASH_H */
