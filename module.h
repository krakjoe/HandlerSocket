/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe <krakjoe@php.net>                                    |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MODULE_H
#define PHP_MODULE_H

extern zend_module_entry handlersocket_module_entry;
#define phpext_handlersocket_ptr &handlersocket_module_entry

#define PHP_HANDLERSOCKET_EXTNAME "HandlerSocket"
#define PHP_HANDLERSOCKET_VERSION "0.0.1" 

#ifdef PHP_WIN32
#	define PHP_HANDLERSOCKET_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HANDLERSOCKET_API __attribute__ ((visibility("default")))
#else
#	define PHP_HANDLERSOCKET_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(handlersocket)
	zend_string *delim;
	zend_string *sep;
	zend_string *space;
	zend_string *primary;
ZEND_END_MODULE_GLOBALS(handlersocket)

#define HSG(v) ZEND_MODULE_GLOBALS_ACCESSOR(handlersocket, v)

#if defined(ZTS) && defined(COMPILE_DL_HANDLERSOCKET)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_MODULE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
