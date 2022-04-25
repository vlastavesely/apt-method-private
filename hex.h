#ifndef __HEX_H
#define __HEX_H

#include <string>

std::string hex_encode(const unsigned char *bin, unsigned int len);
int hex_decode(unsigned char *bin, const std::string hex);

#endif /* __HEX_H */
