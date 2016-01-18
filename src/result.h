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
#ifndef HAVE_HS_RESULT_H
#define HAVE_HS_RESULT_H

typedef struct _hs_result_t {
	zend_long count;
	zval fields;
	zval index;
	zend_object std;
} php_hs_result_t;

#define php_hs_result_fetch_from(o) (php_hs_result_t*) ((char*) o - XtOffsetOf(php_hs_result_t, std))
#define php_hs_result_fetch(z) php_hs_result_fetch_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(result);

extern zend_class_entry *HandlerSocket_Result_ce;
#endif
