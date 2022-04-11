#ifndef __ACQUIRE_METHOD_H
#define __ACQUIRE_METHOD_H

class AcquireMethod {
private:
	int acquire(std::istream &in);
public:
	int loop();
};

#endif /* __ACQUIRE_METHOD_H */
