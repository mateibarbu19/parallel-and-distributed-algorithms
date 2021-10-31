#ifndef UTILS_H_
#define UTILS_H_



#define swap(a, b)                                                             \
  ({                                                                           \
	__typeof__(a) _aux = a;                                                      \
	a = b;                                                                       \
	b = _aux;                                                                    \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define make_even(a)                                                           \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    (_a % 2) ? _a + 1 : _a;                                                    \
  })

#endif  // UTILS_H_
