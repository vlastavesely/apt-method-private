#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <stdexcept>

class uri_exception final : std::exception {
private:
	std::string _message = "";
	std::string _reason = "";
public:
	uri_exception(const std::string &m, const std::string &r) {
		_message = m;
		_reason = r;
	}
	uri_exception(const std::string &m) {
		_message = m;
	}
	const char *what() const noexcept {
		return _message.c_str();
	}
	const char *reason() const noexcept {
		return _reason.c_str();
	}
};

#endif /* __EXCEPTIONS_H */
