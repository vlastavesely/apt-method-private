#ifndef __PRIVATE_METHOD_H
#define __PRIVATE_METHOD_H

#include "acquire-method.h"

class PrivateMethod : public AcquireMethod {
private:
	 int fetchFile(Stanza &request);
};

#endif /* __PRIVATE_METHOD_H */
