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

#ifndef PHP_HS_H
#define PHP_HS_H

#include "host.h"
#include "connection.h"
#include "index.h"
#include "match.h"
#include "result.h"

php_hs_host_t* php_hs_host(zend_string *address, zval *result);

php_hs_connection_t* php_hs_connect(zval *host, zend_long port, zval *result);

void php_hs_authenticate(zval *connection, zend_string *secret, zval *result);

php_hs_index_t* php_hs_index(zval *connection, zend_long id, zend_string *db, zend_string *tbl, zval *cols, zend_string *name, zval *result);

php_hs_result_t* php_hs_find(zval *index, zval *match, zval *result);

void php_hs_close(zend_object *object);

#endif	/* PHP_HS_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
