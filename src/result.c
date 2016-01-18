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
#ifndef HAVE_HS_RESULT
#define HAVE_HS_RESULT

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hs.h"

#include "../module.h"

#include "zend_operators.h"

ZEND_EXTERN_MODULE_GLOBALS(handlersocket);

zend_object_handlers HandlerSocket_Result_handlers;
zend_class_entry *HandlerSocket_Result_ce;

/* {{{ proto Result Result::__construct() */
PHP_METHOD(Result, __construct) {

}

ZEND_BEGIN_ARG_INFO_EX(php_hs_result_construct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
zend_function_entry php_hs_result_methods[] = {
	PHP_ME(Result, __construct, php_hs_result_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static inline zend_object* php_hs_result_create(zend_class_entry *ce) {
	php_hs_result_t *r = (php_hs_result_t*) 
		ecalloc(1, sizeof(php_hs_result_t) + zend_object_properties_size(ce));

	zend_object_std_init(&r->std, ce);
	object_properties_init(&r->std, ce);	

	r->std.handlers = &HandlerSocket_Result_handlers;

	return &r->std;	
}

static inline HashTable* php_hs_result_debug(zval *object, int *is_temp) {
	php_hs_result_t *r = php_hs_result_fetch(object);
	php_hs_index_t *i = php_hs_index_fetch(&r->index);
	HashTable *t;
	
	ALLOC_HASHTABLE(t);
	*is_temp = 1;
	zend_hash_init(t, 2, NULL, ZVAL_PTR_DTOR, 0);
	
	if (zend_hash_str_update(t, ":index:", sizeof(":index:"), &i->cols))
		Z_ADDREF(i->cols);

	if (zend_hash_str_update(t, ":fields:", sizeof(":fields:"), &r->fields))
		Z_ADDREF(r->fields);
	
	return t;
}

static inline void php_hs_result_free(zend_object *o) {
	php_hs_result_t *r = php_hs_result_fetch_from(o);

	if (Z_TYPE(r->fields) != IS_UNDEF) {
		zval_ptr_dtor(&r->fields);
	}

	if (Z_TYPE(r->index) != IS_UNDEF) {
		zval_ptr_dtor(&r->index);
	}

	zend_object_std_dtor(o);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(result) {
	zend_class_entry ce;

	memcpy(&HandlerSocket_Result_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	HandlerSocket_Result_handlers.offset = XtOffsetOf(php_hs_result_t, std);
	HandlerSocket_Result_handlers.free_obj = php_hs_result_free;
	HandlerSocket_Result_handlers.get_debug_info = php_hs_result_debug;

	INIT_NS_CLASS_ENTRY(ce, "HandlerSocket", "Result", php_hs_result_methods);
	HandlerSocket_Result_ce = zend_register_internal_class(&ce);
	HandlerSocket_Result_ce->create_object = php_hs_result_create;
} /* }}} */
#endif
