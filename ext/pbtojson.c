/* pbtojson extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"
#include "php_pbtojson.h"
#include "libgopbtojson.h"
/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void pbtojson_test1()
 */
PHP_METHOD(pbtojson,pbCompile) {
    char *jsonData;
    size_t jsonData_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &jsonData, &jsonData_len) == FAILURE) {
        RETURN_NULL();
    }
	int resLen=pbCompile(jsonData);
    RETURN_LONG(resLen);
}


PHP_METHOD(pbtojson,toJson) {
    char *data,*pbFile,*typeUrl;
    size_t data_len,pbFile_len,typeUrl_len;
    size_t topic_len, channel_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sss", &data, &data_len,&pbFile, &pbFile_len,&typeUrl,&typeUrl_len) == FAILURE) {
        RETURN_NULL();
    }
    size_t buf_len=data_len*3.5;

    char* buf = (char*)malloc(buf_len+1);
    if (buf == NULL) {
		zend_throw_exception(zend_ce_exception, "memory allocation failed", 0);
        RETURN_NULL();
    }
	memset(buf, 0,buf_len+1); 

	int resLen=pbToJsonBuf(data,data_len,pbFile,typeUrl,buf , buf_len);
    if(resLen > 0) {
        RETURN_STRING(buf);
		free(buf);
    } else {
		free(buf);
		if(resLen==-2){
			//php_error_docref0(NULL TSRMLS_CC, E_ERROR, "Fatal error: buf_len is not long enough");
			zend_throw_exception(zend_ce_exception, "buf_len is not long enough", 0);
		}
        RETURN_NULL();
    }
}

PHP_METHOD(pbtojson,anyToJson) {
    char *data;
    size_t data_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &data, &data_len) == FAILURE) {
        RETURN_NULL();
    }
    size_t buf_len=data_len*3.5;

    char* buf = (char*)malloc(buf_len+1);
    if (buf == NULL) {
		zend_throw_exception(zend_ce_exception, "memory allocation failed", 0);
        RETURN_NULL();
    }
	memset(buf, 0,buf_len+1); 

	int resLen=anypbToJsonBuf(data,data_len, buf , buf_len);
    if(resLen > 0) {
        RETURN_STRING(buf);
		free(buf);
    } else {
		free(buf);
		if(resLen==-2){
			//php_error_docref0(NULL TSRMLS_CC, E_ERROR, "Fatal error: buf_len is not long enough");
			zend_throw_exception(zend_ce_exception, "buf_len is not long enough", 0);
		}
        RETURN_NULL();
    }
}


/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pbtojson)
{
#if defined(ZTS) && defined(COMPILE_DL_PBTOJSON)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pbtojson)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pbtojson support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
// 为 anyToJson 函数定义参数信息
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_anyToJson, 0, 1, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, topic, IS_STRING, 0)
ZEND_END_ARG_INFO();

// 为 confirmMessage 定义参数信息
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_toJson, 0, 3, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pbFile, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, typeUrl, IS_STRING, 0)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_pbCompile, 0, 1, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO();
/* }}} */

/* {{{ pbtojson_functions[]
 */
zend_class_entry *pbToJson_ce; // 类入口指针

static const zend_function_entry pbToJson_methods[] = {
    PHP_ME(pbtojson, anyToJson, arginfo_anyToJson, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(pbtojson, toJson, arginfo_toJson, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(pbtojson, pbCompile, arginfo_pbCompile, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_FE_END
};


PHP_MINIT_FUNCTION(pbtojson) {
    zend_class_entry ce;
    // 定义类名和构造函数（可选）
    INIT_CLASS_ENTRY(ce, "PbToJson", pbToJson_methods);
    pbToJson_ce = zend_register_internal_class(&ce);
    return SUCCESS;
}


/* }}} */

/* {{{ pbtojson_module_entry
 */
zend_module_entry pbtojson_module_entry = {
	STANDARD_MODULE_HEADER,
	"pbtojson",					/* Extension name */
	NULL,			/* zend_function_entry */
	PHP_MINIT(pbtojson),							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(pbtojson),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(pbtojson),			/* PHP_MINFO - Module info */
	PHP_PBTOJSON_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

/* }}} */

#ifdef COMPILE_DL_PBTOJSON
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pbtojson)
#endif
