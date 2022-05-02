#include <iostream>
#include "private-method.h"
#include "hash.h"
#include "fetch.h"
#include "filter.h"
#include "origin.h"

std::string PrivateMethod::translate_uri(const URI &uri)
{
	Origin origin(config, uri.origin);
	std::string translated_uri, salt, hash;

	translated_uri = origin.uri;
	salt = origin.salt;
	hash = origin.hash;

	if (translated_uri.back() != '/')
		translated_uri += '/';

	if (hash == "sha1") {
		return translated_uri + hash_sha1(salt, uri.path);
	}

	throw std::invalid_argument("Invalid hash algorithm.");
}

void PrivateMethod::process_file(const URI &uri, const std::string src_file,
				 const std::string dest_file)
{
	Origin origin(config, uri.origin);
	decode_file(src_file, dest_file, origin.cipher, origin.key);
}

void PrivateMethod::fetch_file(Stanza &request)
{
	URI uri(request["URI"]);
	std::string filename, filename_part, real_uri;
	int ret;

	if (uri.is_protocol("private") != true)
		throw uri_exception(uri, "Invalid protocol.");

	real_uri = translate_uri(uri);
	filename = request["Filename"];
	filename_part = request["Filename"] + ".part";

	try {
		ret = fetch_uri(real_uri, filename_part);
		if (ret != 0) {
			throw std::runtime_error("Failed to fetch the URI");
		}

		process_file(uri, filename_part, filename);

	} catch (const std::exception &e) {
		remove(filename_part.data());
		throw; /* rethrow */
	}

	remove(filename_part.data());
}
