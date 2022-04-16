#include <filesystem>
#include <iostream>
#include <stdarg.h>

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

static int encode_entry(const std::filesystem::directory_entry &entry)
{
	std::string path = entry.path();

	std::cout << "Encoding: ‘" << path << "’ ..." << std::endl;

	return 0;
}

static void fatal(const char *fmt...)
{
	va_list params;
	char msg[4096];

	va_start(params, fmt);
	vsnprintf(msg, sizeof(msg), fmt, params);
	std::cerr << "fatal: " << msg << std::endl;
	va_end(params);

	exit(128);
}

int main(int argc, const char **argv)
{
	std::string origin, dest, src, conffile = CONFIG_FILE;
	int i, ret = 0;

	for (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case 'o':
				origin = argv[++i];
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

		fatal("unknown option '%s'.", arg);
	}

	if (origin.length() == 0)
		fatal("target origin was not specified.");

	if (src.length() == 0)
		fatal("source was not specified.");

	if (dest.length() == 0)
		fatal("destination was not specified.");

	std::filesystem::recursive_directory_iterator iter(src);

	for (const auto &entry : iter) {
		ret = encode_entry(entry);
		if (ret != 0) {
			return ret;
		}
	}

	return ret;
}
