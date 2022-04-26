#ifndef __FILTER_H
#define __FILTER_H

#include <string>

void encode_file(std::ifstream &in, std::ofstream &out,
		 const std::string &cipher, const unsigned char *key);
void decode_file(std::ifstream &in, std::ofstream &out,
		 const std::string &cipher, const unsigned char *key);

#endif /* __FILTER_H */
