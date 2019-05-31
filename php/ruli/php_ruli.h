/*
  $Id: php_ruli.h,v 1.3 2004/10/13 23:31:12 evertonm Exp $ 
*/

#ifndef PHP_RULI_H
#define PHP_RULI_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ruli.h"

extern zend_module_entry ruli_module_entry;
#define phpext_ruli_ptr &ruli_module_entry

#ifdef PHP_WIN32
#define PHP_RULI_API __declspec(dllexport)
#else
#define PHP_RULI_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(ruli);
PHP_MSHUTDOWN_FUNCTION(ruli);
PHP_RINIT_FUNCTION(ruli);
PHP_RSHUTDOWN_FUNCTION(ruli);
PHP_MINFO_FUNCTION(ruli);

PHP_FUNCTION(confirm_ruli_compiled);	/* For testing, remove later. */
PHP_FUNCTION(ruli_sync_query);
PHP_FUNCTION(ruli_sync_smtp_query);
PHP_FUNCTION(ruli_sync_http_query);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(ruli)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(ruli)
*/

/* In every utility function you add that needs to use variables 
   in php_ruli_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as RULI_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define RULI_G(v) TSRMG(ruli_globals_id, zend_ruli_globals *, v)
#else
#define RULI_G(v) (ruli_globals.v)
#endif

#endif	/* PHP_RULI_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
