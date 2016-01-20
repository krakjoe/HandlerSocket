dnl $Id$
dnl config.m4 for extension HandlerSocket

PHP_ARG_ENABLE(handlersocket, whether to enable HandlerSocket support,
[  --enable-handlersocket           Enable HandlerSocket support])

if test "$PHP_HANDLERSOCKET" != "no"; then
  PHP_NEW_EXTENSION(handlersocket, module.c src/host.c src/connection.c src/index.c src/match.c src/result.c src/hs.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
