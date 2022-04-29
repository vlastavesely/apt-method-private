#ifndef __FILTER_H
#define __FILTER_H

#include <string>

void encode_file(const std::string &in_file, const std::string &out_file,
		 const std::string &cipher, const unsigned char *key);
void decode_file(const std::string &in_file, const std::string &out_file,
		 const std::string &cipher, const unsigned char *key);

#endif /* __FILTER_H */
