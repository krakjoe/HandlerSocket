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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "module.h"

ZEND_DECLARE_MODULE_GLOBALS(handlersocket)

#include "src/host.h"
#include "src/connection.h"
#include "src/index.h"
#include "src/result.h"

/* {{{ php_handlersocket_init_globals
 */
static void php_handlersocket_init_globals(zend_handlersocket_globals *hsg)
{
	memset(hsg, 0, sizeof(zend_handlersocket_globals));
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(handlersocket)
{
	ZEND_INIT_MODULE_GLOBALS(handlersocket, php_handlersocket_init_globals, NULL);

	PHP_MINIT(host)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(connection)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(index)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(result)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(handlersocket)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(handlersocket)
{
#if defined(COMPILE_DL_HANDLERSOCKET) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	
	HSG(delim)	= zend_string_init(ZEND_STRL(","), 0);
	HSG(sep)	= zend_string_init(ZEND_STRL("\t"), 0);
	HSG(space)	= zend_string_init(ZEND_STRL(" "), 0);
	HSG(primary)= zend_string_init(ZEND_STRL("PRIMARY"), 0);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(handlersocket)
{
	zend_string_release(HSG(delim));
	zend_string_release(HSG(sep));
	zend_string_release(HSG(space));
	zend_string_release(HSG(primary));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(handlersocket)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "HandlerSocket support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ handlersocket_module_entry
 */
zend_module_entry handlersocket_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_HANDLERSOCKET_EXTNAME,
	NULL,
	PHP_MINIT(handlersocket),
	PHP_MSHUTDOWN(handlersocket),
	PHP_RINIT(handlersocket),
	PHP_RSHUTDOWN(handlersocket),
	PHP_MINFO(handlersocket),
	PHP_HANDLERSOCKET_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HANDLERSOCKET
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(handlersocket)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
