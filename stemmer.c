#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "php.h"
#include "php_stemmer.h"
#include "libstemmer_c/include/libstemmer.h"

static zend_function_entry stemmer_functions[] = {
    PHP_FE(stemword, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry stemmer_module_entry = {
#   if ZEND_MODULE_API_NO >= 20010901
        STANDARD_MODULE_HEADER,
#   endif
    PHP_STEMMER_EXTNAME,
    stemmer_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#   if ZEND_MODULE_API_NO >= 20010901
        PHP_STEMMER_VERSION,
#   endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_STEMMER
    ZEND_GET_MODULE(stemmer)
#endif

PHP_FUNCTION(stemword)
{
    zval *lang, *enc, *arg;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &arg, &lang, &enc) == FAILURE) {
        RETURN_NULL();
    }

    convert_to_string(lang);
    convert_to_string(enc);

    struct sb_stemmer * stemmer;

    stemmer = sb_stemmer_new(Z_STRVAL_P(lang), Z_STRVAL_P(enc));
    if (!stemmer) {
        RETURN_NULL();
    }

    convert_to_string(arg);    
    const sb_symbol *stemmed = sb_stemmer_stem(stemmer, Z_STRVAL_P(arg), Z_STRLEN_P(arg));
    if (stemmed) {
        ZVAL_STRING(return_value, stemmed, 1);
    }

    sb_stemmer_delete(stemmer);
}
