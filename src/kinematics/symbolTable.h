#ifndef	_kinematics_symbolTable_H
#define _kinematics_symbolTable_H

#include <clasp/core/foundation.h>

namespace kinematics
{

#define	KinPkg_SYMBOLS
#define DO_SYMBOL(cname,idx,pkg,lispname,export) extern core::Symbol_sp cname;
#include <symbols_scraped_inc.h>
#undef DO_SYMBOL
#undef KinPkg_SYMBOLS

}; /* kinematics */


namespace kw
{
#define	KeywordPkg_SYMBOLS
#define DO_SYMBOL(cname,idx,pkg,lispname,export) extern core::Symbol_sp cname;
#include <symbols_scraped_inc.h>
#undef DO_SYMBOL
#undef KeywordPkg_SYMBOLS
}; /* kw */

#endif /* _kinematics_symbolTable_H */