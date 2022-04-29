#ifndef __PRIVATE_METHOD_H
#define __PRIVATE_METHOD_H

#include "acquire-method.h"
#include "uri.h"

class PrivateMethod : public AcquireMethod {
private:
	std::string translate_uri(const URI &uri);
	void process_file(const URI &uri, const std::string src_file,
			  const std::string dest_file);
	void fetch_file(Stanza &request);
};

#endif /* __PRIVATE_METHOD_H */
