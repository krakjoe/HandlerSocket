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
#ifndef HAVE_HS_CONNECTION
#define HAVE_HS_CONNECTION

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "hs.h"

#include "../module.h"

ZEND_EXTERN_MODULE_GLOBALS(handlersocket);

zend_object_handlers HandlerSocket_Connection_handlers;
zend_class_entry *HandlerSocket_Connection_ce;

/* {{{ proto Connection Connection::__construct(Host host [, int port = 9998]) */
PHP_METHOD(Connection, __construct) {
	zval *host = NULL;
	zend_long port = 9998;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|l", &host, HandlerSocket_Host_ce, &port) != SUCCESS) {
		return;
	}

	php_hs_connect(host, port, getThis());
}

ZEND_BEGIN_ARG_INFO_EX(php_hs_connection_construct_arginfo, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, host, HandlerSocket\\Host, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ proto Connection::openIndex(int id, string db, string tbl, array cols = [], string name = INDEX::PRIMARY) : Index */
PHP_METHOD(Connection, openIndex) {
	zend_long id = -1;
	zend_string *db = NULL;
	zend_string *tbl = NULL;
	zval *cols = NULL, _cols;
	zend_string *name = PHP_INDEX_PRIMARY;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lSSa|l", &id, &db, &tbl, &cols, &name) != SUCCESS) {
		return;
	}

	php_hs_index(getThis(), id, db, tbl, cols, name, return_value);
}

ZEND_BEGIN_ARG_INFO_EX(php_hs_connection_openIndex_arginfo, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tbl, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, cols, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 1)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ proto Connection::authenticate(string secret) : bool */
PHP_METHOD(Connection, authenticate) {
	zend_string *secret = NULL;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &secret) != SUCCESS) {
		return;
	}

	php_hs_authenticate(getThis(), secret, return_value);
}

ZEND_BEGIN_ARG_INFO_EX(php_hs_connection_authenticate_arginfo, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, db, IS_STRING, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
zend_function_entry php_hs_connection_methods[] = {
	PHP_ME(Connection, __construct, php_hs_connection_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Connection, authenticate, php_hs_connection_authenticate_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Connection, openIndex, php_hs_connection_openIndex_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static inline zend_object* php_hs_connection_create(zend_class_entry *ce) {
	php_hs_connection_t *c = (php_hs_connection_t*) 
		ecalloc(1, sizeof(php_hs_connection_t) + zend_object_properties_size(ce));

	zend_object_std_init(&c->std, ce);
	object_properties_init(&c->std, ce);	

	c->std.handlers = &HandlerSocket_Connection_handlers;
	
	return &c->std;	
}

static inline void php_hs_connection_free(zend_object *o) {
	php_hs_close(o);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(connection) {
	zend_class_entry ce;

	memcpy(&HandlerSocket_Connection_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	HandlerSocket_Connection_handlers.offset = XtOffsetOf(php_hs_connection_t, std);
	HandlerSocket_Connection_handlers.free_obj = php_hs_connection_free;

	INIT_NS_CLASS_ENTRY(ce, "HandlerSocket", "Connection", php_hs_connection_methods);
	HandlerSocket_Connection_ce = zend_register_internal_class(&ce);
	HandlerSocket_Connection_ce->create_object = php_hs_connection_create;
} /* }}} */
#endif
