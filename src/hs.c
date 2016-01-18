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

#ifndef PHP_HS
#define PHP_HS

#include "php.h"
#include "hs.h"

#include "../module.h"

ZEND_EXTERN_MODULE_GLOBALS(handlersocket)

#include "../module.h"

#define php_hs_write(c, f, ...) php_stream_printf((c)->stream, f, __VA_ARGS__)
#define php_hs_ok(c) do { \
	char buffer[128]; \
	\
	if (!php_stream_gets((c)->stream, buffer, sizeof(buffer))) { \
		zend_throw_exception_ex(NULL, 0, \
			"failed to read from connection"); \
		return; \
	} \
	\
	if (sscanf(buffer, "0\t1\n") != SUCCESS) { \
		zend_throw_exception_ex(NULL, 0, \
		 	"protocol error"); \
		return; \
	} \
} while (0)

php_hs_host_t* php_hs_host(zend_string *address, zval *result) {
	php_hs_host_t *h = php_hs_host_fetch(result);

	if (!address) {
		return NULL;		
	}

	h->address = zend_string_copy(address);

	return h;
}

php_hs_connection_t* php_hs_connect(zval *host, zend_long port, zval *result) {
	zend_string *name = strpprintf(0, "%s:%ld", php_hs_host_string(host), port),
				*errstr = NULL;
	struct timeval tv;
	int _errno = SUCCESS;
	php_hs_connection_t *c = php_hs_connection_fetch(result);

	c->stream = php_stream_xport_create(ZSTR_VAL(name), ZSTR_LEN(name), 
			REPORT_ERRORS, STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT, 
			NULL, /* persistent key (char*) */
			&tv, NULL, &errstr, &_errno);
	zend_string_release(name);

	if (_errno == SUCCESS && c->stream) {
		ZVAL_COPY(&c->host, host);
		c->port = port;
		return c;
	}

	zend_throw_exception_ex(NULL, 0, 
		"error occured while negotiating connection: %d: %s", _errno, ZSTR_VAL(errstr));
	zend_string_release(errstr);
	return NULL;
}

void php_hs_authenticate(zval *connection, zend_string *secret, zval *result) {
	php_hs_connection_t *c = php_hs_connection_fetch(connection);
	
	ZVAL_FALSE(result);

	php_hs_write(c, 
		"A\t1\t%s\n", ZSTR_VAL(secret));
	php_hs_ok(c);

	ZVAL_TRUE(result);
}

php_hs_index_t *php_hs_index(zval *connection, zend_long id, zend_string *db, zend_string *tbl, zval *cols, zend_string *name, zval *result) {
	php_hs_index_t *i = NULL;
	php_hs_connection_t *c = php_hs_connection_fetch(connection);
	zval _cols;

	object_init_ex(result, HandlerSocket_Index_ce);

	i = php_hs_index_fetch(result);
	
	php_implode(HSG(delim), cols, &_cols);

	if (name == PHP_INDEX_PRIMARY)
		name = HSG(primary);

	php_hs_write(c,
		"P\t%ld\t%s\t%s\t%s\t%s\n",
		id, 
		ZSTR_VAL(db),
		ZSTR_VAL(tbl),
		ZSTR_VAL(name),
		Z_STRVAL(_cols));

	zval_ptr_dtor(&_cols);

	php_hs_ok(c);

	ZVAL_COPY(&i->connection, connection);
	i->id = id;
	i->db = zend_string_copy(db);
	i->tbl = zend_string_copy(tbl);
	i->name = zend_string_copy(name);
	ZVAL_COPY(&i->cols, cols);

	return i;
}

static inline php_hs_result_t* php_hs_result_from(zval *index, char *buffer, size_t length, zval *result) {
	php_hs_result_t *r = php_hs_result_fetch(result);
	long limit = 0;
	zend_string *response;

	limit = atol(&buffer[2]);

	if (!limit) {
		return;
	}

	response = zend_string_init(
		&buffer[4], length-5, 0);
	array_init_size(&r->fields, limit);
	php_explode(
		HSG(sep), response, &r->fields);
	zend_string_release(response);
	
	ZVAL_COPY(&r->index, index);

	if (Z_ARRVAL(r->fields)->nTableSize) {
		zval *member;
		zend_long idx;
		zend_string *k;
		php_hs_index_t *i = php_hs_index_fetch(&r->index);

		rebuild_object_properties(&r->std);

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(r->fields), idx, k, member) {
			zval *key = zend_hash_index_find(
				Z_ARRVAL(i->cols), idx);			

			if (Z_TYPE_P(key) != IS_STRING)
				continue;

			if (zend_hash_update(r->std.properties, Z_STR_P(key), member))
				Z_TRY_ADDREF_P(member);
		} ZEND_HASH_FOREACH_END();
	}

	return r;
}

php_hs_result_t* php_hs_find(zval *index, php_hs_op_t op, zend_string *cmp, zval *result) {
	php_hs_index_t *i = php_hs_index_fetch(index);
	php_hs_connection_t *r = php_hs_connection_fetch(&i->connection);
	char buffer[8192];
	size_t length = 0;

	object_init_ex(result, HandlerSocket_Result_ce);	

	php_stream_printf(
		r->stream,
		"%ld\t%s\t1\t%s\n",
		i->id, 
		php_hs_op(op), 
		ZSTR_VAL(cmp));

	if (!php_stream_get_line(r->stream, buffer, sizeof(buffer), &length)) {
		return NULL;
	}

	if (buffer[0] != '0') {
		return;
	}

	return php_hs_result_from(index, buffer, length, result);
}

void php_hs_close(zend_object *o) {
	php_hs_connection_t *c = php_hs_connection_fetch_from(o);

	if (Z_TYPE(c->host) != IS_UNDEF) {
		zval_ptr_dtor(&c->host);
	}

	if (c->stream) {
		php_stream_close(c->stream);
	}
	
	zend_object_std_dtor(o);
}

#endif	/* PHP_HS */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
