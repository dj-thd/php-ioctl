#include <php.h>
#include <sys/ioctl.h>
#include "ioctl.h"
#define HAVE_SOCKETS 1
#include <php_network.h>
#include <ext/sockets/php_sockets.h>
#include <stdio.h>
#if PHP_VERSION_ID >= 70000 && PHP_VERSION_ID < 80000
#include <Zend/zend_list.h>
#endif


// PHP ioctl extension
// by dj.thd <dj.thd@hotmail.com>
// UNSAFE! PHP developer is responsible of passing correct arguments to function

/* {{{ argument information */
#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_ioctl, 0, 3, MAY_BE_LONG | MAY_BE_FALSE)
    ZEND_ARG_OBJ_INFO(0, res, Socket, 0)
#elif PHP_VERSION_ID >= 70000 && PHP_VERSION_ID < 80000
ZEND_BEGIN_ARG_INFO_EX(arginfo_ioctl, 0, 0, 4)
    ZEND_ARG_INFO(0, socket)
#endif
    ZEND_ARG_TYPE_INFO(0, request, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, is_ptr, _IS_BOOL, 0)
ZEND_END_ARG_INFO()
/* }}} */

zend_function_entry ioctl_functions[] = {
            PHP_FE(ioctl, arginfo_ioctl)
        PHP_FE_END
};

zend_module_entry ioctl_module_entry = {
            STANDARD_MODULE_HEADER,
        PHP_IOCTL_EXTNAME,
        ioctl_functions,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_IOCTL_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(ioctl);

// From main/streams/plain_wrapper.c
typedef struct {
	FILE *file;
	int fd;					/* underlying file descriptor */
	unsigned is_process_pipe:1;	/* use pclose instead of fclose */
	unsigned is_pipe:1;			/* don't try and seek */
	unsigned cached_fstat:1;	/* sb is valid */
	unsigned is_pipe_blocking:1; /* allow blocking read() on pipes, currently Windows only */
	unsigned _reserved:28;

	int lock_flag;			/* stores the lock state */
	zend_string *temp_name;	/* if non-null, this is the path to a temporary file that
							 * is to be deleted when the stream is closed */
#if HAVE_FLUSHIO
	char last_op;
#endif

#if HAVE_MMAP
	char *last_mapped_addr;
	size_t last_mapped_len;
#endif
#ifdef PHP_WIN32
	char *last_mapped_addr;
	HANDLE file_mapping;
#endif

	zend_stat_t sb;
} php_stdio_stream_data;


PHP_FUNCTION(ioctl) {

	zval *res;
	zend_long request;
	zend_bool is_ptr;

	php_socket *php_sock;
	char *data;
	size_t data_len;

#if PHP_VERSION_ID >= 80000
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Olsb", &res, socket_ce, &request, &data, &data_len, &is_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	php_sock = Z_SOCKET_P(res);
	ENSURE_SOCKET_VALID(php_sock);
#elif PHP_VERSION_ID >= 70000 && PHP_VERSION_ID < 80000
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rlsb", &res, &request, &data, &data_len, &is_ptr) == FAILURE) {
		RETURN_FALSE;
	}

	if ((php_sock = (php_socket *)zend_fetch_resource(Z_RES_P(res), php_sockets_le_socket_name, php_sockets_le_socket())) == NULL) {
		RETURN_FALSE;
	}
#endif

	int result = ioctl(php_sock->bsd_socket, (int)request, is_ptr ? data : *(void**)(&data));
	if(result == -1)
	{
		RETURN_FALSE;
	}
	else
	{
		RETURN_LONG(result);
	}
};
