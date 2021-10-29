#ifndef UTILS_H_
#define UTILS_H_



#define swap(a, b)                                                             \
  ({                                                                           \
    __typeof__(a) _a   = (a);                                                  \
    __typeof__(b) _b   = (b);                                                  \
	__typeof__(a) _aux = _a;                                                   \
	_a = _b;                                                                   \
	_b = _aux;                                                                 \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#endif  // UTILS_H_
