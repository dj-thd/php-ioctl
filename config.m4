PHP_ARG_ENABLE(ioctl, Whether to enable the ioctl extension, [ --enable-ioctl   Enable ioctl])

if test "$PHP_IOCTL" != "no"; then
    PHP_NEW_EXTENSION(ioctl, src/ioctl.c, $ext_shared)
fi
