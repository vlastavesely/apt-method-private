#ifndef __ACQUIRE_METHOD_H
#define __ACQUIRE_METHOD_H

#include "config-file.h"
#include "stanza.h"
#include "except.h"

class AcquireMethod {
private:
	int acquire(std::istream &in);
protected:
	Config config;
	void send_message(const std::string &message,
			  const std::map<std::string, std::string> &fields);
	void report_general_failure(const std::string &message);
	void report_uri_failure(const uri_exception &e);
	virtual void fetch_file(Stanza &request) = 0;
public:
	int loop();
};

#endif /* __ACQUIRE_METHOD_H */
