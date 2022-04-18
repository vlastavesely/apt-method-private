#ifndef __FILTER_H
#define __FILTER_H

int encode_file(const std::string inFile, const std::string outFile,
		const unsigned char *key);
int decode_file(const std::string inFile, const std::string outFile,
		const unsigned char *key);

#endif /* __FILTER_H */
