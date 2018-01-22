#include <php.h>
#include <sys/ioctl.h>
#include "ioctl.h"

// PHP ioctl extension
// by dj.thd <dj.thd@hotmail.com>
// UNSAFE! PHP developer is responsible of passing correct arguments to function

zend_function_entry ioctl_functions[] = {
            PHP_FE(ioctl, NULL)
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
	zend_string *data;
	zend_bool is_ptr;

	php_stream *stream;

	int fd;

	ZEND_PARSE_PARAMETERS_START(4, 4);
		Z_PARAM_RESOURCE(res);
		Z_PARAM_LONG(request);
		Z_PARAM_STR(data);
		Z_PARAM_BOOL(is_ptr);
	ZEND_PARSE_PARAMETERS_END_EX(return);

	php_stream_from_zval(stream, res);
	php_stdio_stream_data *stream_data = (php_stdio_stream_data*)stream->abstract;
	fd = stream_data->fd;

	int result = ioctl(fd, (int)request, is_ptr ? ZSTR_VAL(data) : *(void**)(ZSTR_VAL(data)));
	if(result == -1)
	{
		RETURN_FALSE;
	}
	else
	{
		RETURN_LONG(result);
	}
};
