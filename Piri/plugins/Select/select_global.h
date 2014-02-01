#ifndef SELECT_GLOBAL_H
#define SELECT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SELECT_LIBRARY)
#  define SELECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SELECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SELECT_GLOBAL_H
