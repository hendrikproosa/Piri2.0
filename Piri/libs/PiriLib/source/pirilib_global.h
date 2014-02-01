#ifndef PIRILIB_GLOBAL_H
#define PIRILIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PIRILIB_LIBRARY)
#  define PIRILIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PIRILIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PIRILIB_GLOBAL_H
