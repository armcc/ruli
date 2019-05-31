/*
  $Id: php_ruli.c,v 1.6 2004/10/25 05:47:16 evertonm Exp $ 
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ruli.h"

/* If you declare any globals in php_ruli.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ruli)
*/

#if 0
/* True global resources - no need for thread safety here */
static int le_ruli;
#endif

/* {{{ ruli_functions[]
 *
 * Every user visible function must have an entry in ruli_functions[].
 */
function_entry ruli_functions[] = {
	PHP_FE(confirm_ruli_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(ruli_sync_query,	NULL)
	PHP_FE(ruli_sync_smtp_query, NULL)
	PHP_FE(ruli_sync_http_query, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in ruli_functions[] */
};
/* }}} */

/* {{{ ruli_module_entry
 */
zend_module_entry ruli_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ruli",
	ruli_functions,
	PHP_MINIT(ruli),
	PHP_MSHUTDOWN(ruli),
	PHP_RINIT(ruli),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ruli),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ruli),
#if ZEND_MODULE_API_NO >= 20010901
	"0.2", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RULI
ZEND_GET_MODULE(ruli)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ruli.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_ruli_globals, ruli_globals)
    STD_PHP_INI_ENTRY("ruli.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ruli_globals, ruli_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_ruli_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ruli_init_globals(zend_ruli_globals *ruli_globals)
{
	ruli_globals->global_value = 0;
	ruli_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ruli)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(ruli, php_ruli_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ruli)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ruli)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ruli)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ruli)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ruli support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_ruli_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_ruli_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "ruli", arg);
	RETURN_STRINGL(string, len, 1);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

static int scan_srv_list(zval *return_value, ruli_sync_t *sync_query)
{
	int srv_code;
	ruli_list_t *srv_list;
	int srv_list_size;
	int i;

	/*
	 * Underlying SRV query failure?
	 */
	srv_code = ruli_sync_srv_code(sync_query);
	if (srv_code == RULI_SRV_CODE_ALARM)
		return -1;

	/*
	 * Service provided?
	 */
	if (srv_code == RULI_SRV_CODE_UNAVAILABLE)
		return -1;

	/*
	 * Server RCODE?
	 */
	if (srv_code) {
		int rcode = ruli_sync_rcode(sync_query);
		if (rcode)
			return -1;

		return -1;
	}

	srv_list = ruli_sync_srv_list(sync_query);
	srv_list_size = ruli_list_size(srv_list);

	/*
	 * Scan list of SRV records
	 */
	array_init(return_value);
	for (i = 0; i < srv_list_size; ++i) {
		ruli_srv_entry_t *entry         = ruli_list_get(srv_list, i);
		ruli_list_t      *addr_list     = &entry->addr_list;
		int              addr_list_size = ruli_list_size(addr_list);
		char             txt_dname_buf[RULI_LIMIT_DNAME_TEXT_BUFSZ];
		int              txt_dname_len;
		int              j;
		zval             *srv;
		zval		     *srv_addrs;

		if (ruli_dname_decode(txt_dname_buf, RULI_LIMIT_DNAME_TEXT_BUFSZ,
							  &txt_dname_len,
							  entry->target, entry->target_len))
			continue;

		MAKE_STD_ZVAL(srv);
		array_init(srv);
		add_index_zval(return_value, i, srv);

		add_assoc_string(srv, "target", txt_dname_buf, 1);
		add_assoc_long(srv, "priority", entry->priority);
		add_assoc_long(srv, "weight", entry->weight);
		add_assoc_long(srv, "port", entry->port);

		MAKE_STD_ZVAL(srv_addrs);
		array_init(srv_addrs);
		add_assoc_zval(srv, "addr_list", srv_addrs);

		for (j = 0; j < addr_list_size; ++j) {
			ruli_addr_t *addr = ruli_list_get(addr_list, j);
			char buf[40];
			if (ruli_addr_snprint(buf, 40, addr) < 0)
				continue;
			add_index_string(srv_addrs, j, buf, 1);
		}
	}

	return 0;
}

/* {{{ proto array ruli_sync_query(string service, string domain, int fallback_port, long options)
   query SRV records synchronously */
PHP_FUNCTION(ruli_sync_query)
{
	char *service = NULL;
	char *domain = NULL;
	int argc = ZEND_NUM_ARGS();
	int service_len;
	int domain_len;
	long fallback_port;
	long options;

	ruli_sync_t *sync_query;

	if (zend_parse_parameters(argc TSRMLS_CC, "ssll", &service, &service_len, &domain, &domain_len, &fallback_port, &options) == FAILURE) 
		return;

	/*
	 * Submit query
	 */
	sync_query = ruli_sync_query(service, domain, fallback_port, options);
	if (!sync_query) {
		RETURN_FALSE;
	}

	{
		int result;
		result = scan_srv_list(return_value, sync_query);

		ruli_sync_delete(sync_query);

		if (result) {
			RETURN_FALSE;
		}
	}
}
/* }}} */

/* {{{ proto array ruli_sync_smtp_query(string domain, long options)
   query SRV records synchronously for _smtp._tcp service */
PHP_FUNCTION(ruli_sync_smtp_query)
{
	char *domain = NULL;
	int argc = ZEND_NUM_ARGS();
	int domain_len;
	long options;

	ruli_sync_t *sync_query;

	if (zend_parse_parameters(argc TSRMLS_CC, "sl", &domain, &domain_len, &options) == FAILURE) 
		return;

	/*
	 * Submit query
	 */
	sync_query = ruli_sync_smtp_query(domain, options);
	if (!sync_query) {
		RETURN_FALSE;
	}

	{
		int result;
		result = scan_srv_list(return_value, sync_query);

		ruli_sync_delete(sync_query);
		
		if (result) {
			RETURN_FALSE;
		}
	}
}
/* }}} */

/* {{{ proto array ruli_sync_http_query(string domain, int force_port, long options)
   query SRV records synchronously for _http._tcp service */
PHP_FUNCTION(ruli_sync_http_query)
{
	char *domain = NULL;
	int argc = ZEND_NUM_ARGS();
	int domain_len;
	long force_port;
	long options;

	ruli_sync_t *sync_query;

	if (zend_parse_parameters(argc TSRMLS_CC, "sll", &domain, &domain_len, &force_port, &options) == FAILURE) 
		return;

	/*
	 * Submit query
	 */
	sync_query = ruli_sync_http_query(domain, force_port, options);
	if (!sync_query) {
		RETURN_FALSE;
	}

	{
		int result;
		result = scan_srv_list(return_value, sync_query);
		
		ruli_sync_delete(sync_query);
		
		if (result) {
			RETURN_FALSE;
		}
	}
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
