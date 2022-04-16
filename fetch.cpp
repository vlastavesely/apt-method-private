#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <curl/curl.h>
#include <string>
#include "fetch.h"

static int set_uri(CURL *curl, const char *uri)
{
	if (strncmp(uri, "ftpes://", 8) == 0) {
		char *dup = strdup(uri);
		memmove(dup + 3, dup + 5, strlen(dup) - 4);
		curl_easy_setopt(curl, CURLOPT_URL, dup);
		curl_easy_setopt(curl, CURLOPT_USE_SSL, 1);
		free(dup);
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, uri);

	return 0;
}

int fetchUri(const std::string &uri, const std::string &filename)
{
	CURL *curl;
	FILE *fp;
	int ret;

	curl = curl_easy_init();
	if (!curl)
		return CURLE_FAILED_INIT;

	fp = fopen(filename.c_str(), "w");
	if (fp == NULL)
		return -errno;

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
	set_uri(curl, uri.c_str());

	ret = curl_easy_perform(curl);
	if (ret == CURLE_HTTP_RETURNED_ERROR) {
		unsigned long code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
		ret = code;
	}

	curl_easy_cleanup(curl);
	fclose(fp);

	return ret;
}
