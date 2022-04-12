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
	void reportGeneralFailure(const std::string &message);
	void reportUriFailure(const std::string &uri, const uri_exception &e);
	virtual int fetchFile(Stanza &request) = 0;
public:
	int loop();
};

#endif /* __ACQUIRE_METHOD_H */
