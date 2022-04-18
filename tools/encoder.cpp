#include <filesystem>
#include <iostream>
#include <stdarg.h>
#include "../config.h"
#include "../origin.h"
#include "../hash.h"
#include "../filter.h"

#define PROGNAME "apt-private-prepare"
#define VERSION "1.0"

#define CONFIG_FILE "config" // FIXME

static const char *usage_str =
	PROGNAME ", an encoder for encryption of APT repositories.\n"
	"\n"
	"usage:\n"
	"  " PROGNAME " [-o ORIGIN] [-s DIR] [-d DIR]\n"
	"\n"
	"options:\n"
	"  -o     target origin\n"
	"  -d     destination directory\n"
	"  -s     source directory\n"
	"  -c     configuration file\n"
	"  -h     shows this help and exits\n"
	"  -v     shows version number and exits\n";

static void print_usage()
{
	std::cout << usage_str << std::endl;
}

static void print_version()
{
	std::cout << PROGNAME " v" VERSION  << std::endl;
}

static int encode_entry(const std::filesystem::directory_entry &entry,
			const std::string &root, const std::string &dest,
			Origin &origin)
{
	std::string entry_path = entry.path();
	std::string fragment, dest_file;
	std::string salt, hash;
	int ret;

	fragment = entry_path.substr(root.length());
	hash = origin.hash;
	salt = origin.salt;

	if (hash == "sha1") {
		dest_file = dest + "/" + hashSha1(origin.salt, fragment);
	} else {
		throw std::invalid_argument("Invalid hash.");
	}

	std::cout << "Encoding: ‘" << entry_path << "’ ... " << std::endl;

	/* FIXME */
	ret = encode_file(entry_path, dest_file, origin.key);

	return ret;
}

static int encode_directory(const std::string &src, const std::string &dest,
			    Origin &origin)
{
	std::filesystem::directory_entry dest_dir(dest);
	std::filesystem::recursive_directory_iterator iter(src);
	int ret = 0;

	if (!dest_dir.is_directory()) {
		std::filesystem::create_directories(dest);
	}

	for (const auto &entry : iter) {
		if (!entry.is_regular_file() && !entry.is_symlink())
			continue;

		ret = encode_entry(entry, src, dest, origin);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

int main(int argc, const char **argv)
{
	std::string originName, dest, src, conffile = CONFIG_FILE;
	int i, ret = 0;

	for (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case 'o':
				originName = argv[++i];
				continue;
			case 'd':
				dest = argv[++i];
				continue;
			case 's':
				src = argv[++i];
				continue;
			case 'c':
				conffile = argv[++i];
				continue;
			case 'h':
				print_usage();
				return 0;
			case 'v':
				print_version();
				return 0;
			}
		}

		std::cerr << "unknown option '" << arg << "'." << std::endl;
		return 1;
	}

	if (originName == "") {
		std::cerr << "target origin was not specified." << std::endl;
		return 1;
	}

	if (src == "") {
		std::cerr << "source directory was not specified." << std::endl;
		return 1;
	}

	if (dest == "") {
		std::cerr << "destination directory was not specified." << std::endl;
		return 1;
	}

	if (src.back() == '/')
		src.pop_back();

	try {
		Config config;
		config.loadFromFile(conffile);
		Origin origin(config, originName);

		ret = encode_directory(src, dest, origin);

	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return ret;
}
