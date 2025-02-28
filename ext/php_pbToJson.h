/* pbtojson extension for PHP */

#ifndef PHP_PBTOJSON_H
# define PHP_PBTOJSON_H

extern zend_module_entry pbtojson_module_entry;
# define phpext_pbtojson_ptr &pbtojson_module_entry

# define PHP_PBTOJSON_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_PBTOJSON)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_PBTOJSON_H */
