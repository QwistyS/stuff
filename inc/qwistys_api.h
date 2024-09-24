#ifndef QWISTYS_API_H
#define QWISTYS_API_H


#ifdef STATIC_API
#define API_IMPL static
#else 
#define API_IMPL extern
#endif

#endif // QWISTYS_API_H
