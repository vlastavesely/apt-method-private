#ifndef __ACQUIRE_METHOD_H
#define __ACQUIRE_METHOD_H

class AcquireMethod {
private:
	void reportGeneralFailure(const std::string &message);
	int acquire(std::istream &in);
public:
	int loop();
};

#endif /* __ACQUIRE_METHOD_H */
