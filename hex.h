#ifndef __HEX_H
#define __HEX_H

#include <string>

std::string hexEncode(const unsigned char *bin, unsigned int len);
int hexDecode(unsigned char *bin, const std::string hex);

#endif /* __HEX_H */
