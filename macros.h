#ifndef __MACROS_H
#define __MACROS_H

#ifndef PUTU32_BE
#define PUTU32_BE(a, i) {              \
	(a)[0] = (uint8_t)((i) >> 24); \
	(a)[1] = (uint8_t)((i) >> 16); \
	(a)[2] = (uint8_t)((i) >>  8); \
	(a)[3] = (uint8_t)((i));       \
}
#endif

#ifndef PUTU64_BE
#define PUTU64_BE(a, i) {              \
	(a)[0] = (uint8_t)((i) >> 56); \
	(a)[1] = (uint8_t)((i) >> 48); \
	(a)[2] = (uint8_t)((i) >> 40); \
	(a)[3] = (uint8_t)((i) >> 32); \
	(a)[4] = (uint8_t)((i) >> 24); \
	(a)[5] = (uint8_t)((i) >> 16); \
	(a)[6] = (uint8_t)((i) >>  8); \
	(a)[7] = (uint8_t)((i));       \
}
#endif

#endif /* __MACROS_H */
