#ifndef OSDAB_GLOBAL_H
#define OSDAB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OSDAB_LIBRARY)
#  define OSDAB_BASE64SHARED_EXPORT Q_DECL_EXPORT
#else
#  define OSDAB_BASE64SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OSDAB_GLOBAL_H
