#ifndef __ACQUIRE_METHOD_H
#define __ACQUIRE_METHOD_H

#include "config.h"
#include "stanza.h"
#include "except.h"

class AcquireMethod {
private:
	int acquire(std::istream &in);
protected:
	Config config;
	void sendMessage(const std::string &message,
			 const std::map<std::string, std::string> &fields);
	void reportGeneralFailure(const std::string &message);
	void reportUriFailure(const uri_exception &e);
	virtual int fetchFile(Stanza &request) = 0;
public:
	int loop();
};

#endif /* __ACQUIRE_METHOD_H */
