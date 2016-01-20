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
#ifndef HAVE_HS_INDEX
#define HAVE_HS_INDEX

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hs.h"

#include "../module.h"


#include "zend_operators.h"

ZEND_EXTERN_MODULE_GLOBALS(handlersocket);

zend_object_handlers HandlerSocket_Index_handlers;
zend_class_entry *HandlerSocket_Index_ce;

/* {{{ proto Index Index::__construct() */
PHP_METHOD(Index, __construct) {

}

ZEND_BEGIN_ARG_INFO_EX(php_hs_index_construct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ proto Index Index::find(Match match, int op = INDEX::EQ) */
PHP_METHOD(Index, find) {
	zval *match = NULL;

	zend_long op = HS_EQ;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &match, HandlerSocket_Match_ce) != SUCCESS) {
		return;
	}

	php_hs_find(getThis(), match, return_value);
}

ZEND_BEGIN_ARG_INFO_EX(php_hs_index_find_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_TYPE_INFO(0, op, IS_LONG, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
zend_function_entry php_hs_index_methods[] = {
	PHP_ME(Index, __construct, php_hs_index_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Index, find, php_hs_index_find_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static inline zend_object* php_hs_index_create(zend_class_entry *ce) {
	php_hs_index_t *i = (php_hs_index_t*) 
		ecalloc(1, sizeof(php_hs_index_t) + zend_object_properties_size(ce));

	zend_object_std_init(&i->std, ce);
	object_properties_init(&i->std, ce);	

	i->std.handlers = &HandlerSocket_Index_handlers;

	return &i->std;	
}

static inline void php_hs_index_free(zend_object *o) {
	php_hs_index_t *i = php_hs_index_fetch_from(o);

	if (i->db) {
		zend_string_release(i->db);
	}

	if (i->tbl) {
		zend_string_release(i->tbl);
	}
	
	if (i->name) {
		zend_string_release(i->name);
	}

	if (Z_TYPE(i->cols) != IS_UNDEF) {
		zval_ptr_dtor(&i->cols);
	}

	if (Z_TYPE(i->connection) != IS_UNDEF) {
		zval_ptr_dtor(&i->connection);
	}

	zend_object_std_dtor(o);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(index) {
	zend_class_entry ce;

	memcpy(&HandlerSocket_Index_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	HandlerSocket_Index_handlers.offset = XtOffsetOf(php_hs_index_t, std);
	HandlerSocket_Index_handlers.free_obj = php_hs_index_free;

	INIT_NS_CLASS_ENTRY(ce, "HandlerSocket", "Index", php_hs_index_methods);
	HandlerSocket_Index_ce = zend_register_internal_class(&ce);
	HandlerSocket_Index_ce->create_object = php_hs_index_create;

	zend_declare_class_constant_string(HandlerSocket_Index_ce, ZEND_STRL("PRIMARY"), "PRIMARY");
} /* }}} */
#endif
