#include <tuple>
#include <map>
#include <stdexcept>
#include <functional>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "filter.h"
#include "random.h"
#include "macros.h"
#include "cipher.h"

#define CHUNKSIZE 1024
#define MAX_BLOCKSIZE 16

static off_t fd_size(int fd)
{
	off_t offset, size;

	offset = lseek(fd, 0, SEEK_CUR);
	size = lseek(fd, 0, SEEK_END);
	lseek(fd, offset, SEEK_SET);

	return size;
}

static int read_iv(int in, unsigned char *iv, unsigned int len)
{
	int n;

	n = read(in, iv, len);
	if (n != (int) len)
		return -errno;

	return 0;
}

static int generate_and_write_iv(int out, unsigned char *iv, unsigned int len)
{
	int ret, n;

	ret = randomise(iv, len);
	if (ret != 0)
		return ret;

	n = write(out, iv, len);
	if (n != (int) len)
		return -errno;

	return 0;
}

static void encrypt_cbc_inplace(unsigned char *buf, unsigned int len,
				unsigned char *iv, Cipher &cipher)
{
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int done = 0, i;

	while (done < len) {
		for (i = 0; i < blocksize; i++)
			buf[done + i] ^= iv[i];

		cipher.encrypt_block(buf + done, buf + done);
		memcpy(iv, buf + done, blocksize);

		done += blocksize;
	}
}

static void decrypt_cbc_inplace(unsigned char *buf, unsigned int len,
				unsigned char *iv, Cipher &cipher)
{
	unsigned char tmp[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int done = 0, i;

	while (done < len) {
		memcpy(tmp, buf + done, blocksize);

		cipher.decrypt_block(buf + done, buf + done);
		for (i = 0; i < blocksize; i++)
			buf[done + i] ^= iv[i];

		memcpy(iv, tmp, blocksize);

		done += blocksize;
	}
}

static int encrypt_cbc_fd(int in, int out, Cipher &cipher)
{
	unsigned char chunk[CHUNKSIZE + MAX_BLOCKSIZE], iv[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	off_t size = fd_size(in), done = 0;
	int ret, n;

	ret = generate_and_write_iv(out, iv, blocksize);
	if (ret != 0)
		return  ret;

	while (1) {
		n = read(in, chunk, CHUNKSIZE);
		if (n == -1)
			return -errno;

		done += n;
		if (done == size) {
			auto pad = blocksize - (n % blocksize);
			memset(chunk + n, 0x00, pad);
			chunk[n] = 0x80;
			n += pad;
		}

		encrypt_cbc_inplace(chunk, n, iv, cipher);

		n = write(out, chunk, n);
		if (n == -1) {
			return -errno;
		}

		if (done == size) {
			break;
		}
	}

	return 0;
}

static int decrypt_cbc_fd(int in, int out, Cipher &cipher)
{
	unsigned char chunk[CHUNKSIZE], iv[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	off_t size = fd_size(in), done = 0;
	int ret, n;

	ret = read_iv(in, iv, blocksize);
	if (ret != 0)
		return ret;

	done = blocksize; /* skip the IV */

	while (1) {
		n = read(in, chunk, CHUNKSIZE);
		if (n == -1)
			return -errno;

		done += n;
		decrypt_cbc_inplace(chunk, n, iv, cipher);

		if (done == size) {
			do {
				n--;
			} while (n >= 0 && chunk[n] != 0x80);
		}

		n = write(out, chunk, n);
		if (n == -1) {
			return -errno;
		}

		if (done == size) {
			break;
		}
	}

	return 0;
}

static void crypt_ctr_inplace(unsigned char *buf, unsigned int len,
			      unsigned char *iv, Cipher &cipher,
			      unsigned long long *blockno)
{
	unsigned char block[MAX_BLOCKSIZE], out[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int done = 0, i;

	memcpy(block, iv, blocksize / 2);

	while (1) {
		switch (blocksize) {
		case 8:
			PUTU32_BE(block + 4, *blockno);
			break;
		case 16:
			PUTU64_BE(block + 8, *blockno);
			break;
		}

		cipher.encrypt_block(out, block);

		auto n = len - done > blocksize ? blocksize : len - done;
		for (i = 0; i < n; i++)
			buf[done + i] ^= out[i];

		done += n;
		if (done == len) {
			break;
		}
	}
}

static int crypt_ctr_fd(int in, int out, unsigned char *iv, Cipher &cipher)
{
	unsigned char chunk[CHUNKSIZE];
	unsigned long long blockno = 0;
	int n;

	while (1) {
		n = read(in, chunk, sizeof(chunk));
		if (n == -1)
			return -errno;
		if (n == 0)
			break;

		crypt_ctr_inplace(chunk, n, iv, cipher, &blockno);

		if (write(out, chunk, n) != n) {
			return -errno;
		}
	}

	return 0;
}

static int encrypt_ctr_fd(int in, int out, Cipher &cipher)
{
	unsigned char iv[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int ivlen = blocksize / 2;
	int ret;

	ret = generate_and_write_iv(out, iv, ivlen);
	if (ret != 0)
		return  ret;

	return crypt_ctr_fd(in, out, iv, cipher);
}

static int decrypt_ctr_fd(int in, int out, Cipher &cipher)
{
	unsigned char iv[MAX_BLOCKSIZE];
	unsigned int blocksize = cipher.get_blocksize();
	unsigned int ivlen = blocksize / 2;
	int ret;

	ret = read_iv(in, iv, ivlen);
	if (ret != 0)
		return ret;

	return crypt_ctr_fd(in, out, iv, cipher);
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

typedef std::function<int(int, int, Cipher &cipher)> transform_callback;

static std::map<const std::string, transform_callback> encrypt_callbacks = {
	{"cbc", encrypt_cbc_fd},
	{"ctr", encrypt_ctr_fd}
};

static std::map<const std::string, transform_callback> decrypt_callbacks = {
	{"cbc", decrypt_cbc_fd},
	{"ctr", decrypt_ctr_fd}
};

static int transform_file(const std::string &in_file, const std::string &out_file,
			  Cipher &cipher, transform_callback transform)
{
	int in, out, ret;

	in = open(in_file.data(), O_RDONLY);
	if (in == -1)
		return -1;

	out = open(out_file.data(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out == -1) {
		close(in);
		return -1;
	}

	ret = transform(in, out, cipher);

	close(in);
	close(out);

	return ret;
}

void encode_file(const std::string &in_file, const std::string &out_file,
		 const std::string &template_name, const unsigned char *key)
{
	const auto &[mode, cipher_name] = parse_template_name(template_name);
	Cipher cipher(cipher_name, key);
	int ret;

	auto transform = encrypt_callbacks[mode];
	if (transform == nullptr) {
		throw std::invalid_argument("Invalid encryption mode.");
	}

	ret = transform_file(in_file, out_file, cipher, transform);
	if (ret != 0) {
		throw std::runtime_error("Failed to encrypt the file.");
	}
}

void decode_file(const std::string &in_file, const std::string &out_file,
		 const std::string &template_name, const unsigned char *key)
{
	const auto &[mode, cipher_name] = parse_template_name(template_name);
	Cipher cipher(cipher_name, key);
	int ret;

	auto transform = decrypt_callbacks[mode];
	if (transform == nullptr) {
		throw std::invalid_argument("Invalid decryption mode.");
	}

	ret = transform_file(in_file, out_file, cipher, transform);
	if (ret != 0) {
		throw std::runtime_error("Failed to decrypt the file.");
	}
}
