#include <fstream>
#include <tuple>
#include <string.h>
#include "filter.h"
#include "cipher.h"
#include "random.h"

#define CHUNKSIZE 16
#define MAX_BLOCKSIZE 16

static std::streampos fstream_size(std::ifstream &in)
{
	in.seekg(0, in.end);
	auto size = in.tellg();
	in.seekg(0, in.beg);

	return size;
}

static void encrypt_cbc_inplace(unsigned char *buf, unsigned int n,
				unsigned char *iv, Cipher &cipher)
{
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int done = 0, i;

	while (n - done) {
		for (i = 0; i < blocksize; i++)
			buf[i] ^= iv[i];

		cipher.encrypt_block(buf, buf);
		memcpy(iv, buf, blocksize);

		done += blocksize;
		buf += blocksize;
	}
}

static void decrypt_cbc_inplace(unsigned char *buf, unsigned int n,
				unsigned char *iv, Cipher &cipher)
{
	unsigned int blocksize = cipher.get_blocksize();
	unsigned char tmp[MAX_BLOCKSIZE];
	unsigned int done = 0, i;

	while (n - done) {
		memcpy(tmp, buf, blocksize);

		cipher.decrypt_block(buf, buf);
		for (i = 0; i < blocksize; i++)
			buf[i] ^= iv[i];

		memcpy(iv, tmp, blocksize);

		done += blocksize;
		buf += blocksize;
	}
}

static void encrypt_cbc_file(std::ifstream &in, std::ofstream &out,
			     Cipher &cipher)
{
	char chunk[CHUNKSIZE + MAX_BLOCKSIZE], iv[MAX_BLOCKSIZE];
	auto size = fstream_size(in);
	auto blocksize = cipher.get_blocksize();
	long int done = 0;

	randomise(iv, blocksize);
	out.write(iv, blocksize);

	while (1) {
		auto n = in.readsome(chunk, blocksize);
		done += n;

		if (done == size) {
			auto pad = blocksize - (n % blocksize);
			memset(chunk + n, 0x00, pad);
			chunk[n] = 0x80;
			n += pad;
		}

		encrypt_cbc_inplace((unsigned char *) chunk, n,
				    (unsigned char *) iv, cipher);

		out.write(chunk, n);
		memcpy(iv, chunk, blocksize);

		if (done == size) {
			break;
		}
	}
}

static void decrypt_cbc_file(std::ifstream &in, std::ofstream &out,
			     Cipher &cipher)
{
	char chunk[CHUNKSIZE], iv[MAX_BLOCKSIZE];
	auto size = fstream_size(in);
	auto blocksize = cipher.get_blocksize();
	long int done = 0;

	in.read(iv, blocksize);
	done = blocksize;

	while (1) {
		auto n = in.readsome(chunk, blocksize);
		done += n;

		decrypt_cbc_inplace((unsigned char *) chunk, n,
				    (unsigned char *) iv, cipher);

		if (done == size) {
			do {
				n--;
			} while (n >= 0 && ((unsigned char*) chunk)[n] != 0x80);
		}

		out.write(chunk, n);

		if (done == size) {
			break;
		}
	}
}

static std::tuple<std::string, std::string> parse_template_name(const std::string &tpl)
{
	char mode[64], cipher[64];
	int ret;

	ret = sscanf(tpl.data(), "%[a-zA-Z0-9](%[a-zA-Z0-9-])", mode, cipher);
	if (ret != 2) {
		throw std::invalid_argument(
			"Invalid cipher name '" + tpl + "'."
		);
	}

	return std::make_tuple(mode, cipher);
}

void encode_file(std::ifstream &in, std::ofstream &out,
		 const std::string &template_name, const unsigned char *key)
{
	const auto &[mode, cipher_name] = parse_template_name(template_name);
	Cipher cipher(cipher_name, key);

	if (mode == "cbc") {
		encrypt_cbc_file(in, out, cipher);
		return;
	}

	throw std::invalid_argument("Invalid cipher.");
}

void decode_file(std::ifstream &in, std::ofstream &out,
		 const std::string &template_name, const unsigned char *key)
{
	const auto &[mode, cipher_name] = parse_template_name(template_name);
	Cipher cipher(cipher_name, key);

	if (mode == "cbc") {
		decrypt_cbc_file(in, out, cipher);
		return;
	}

	throw std::invalid_argument("Invalid cipher.");
}
