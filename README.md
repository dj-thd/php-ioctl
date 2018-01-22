# php-ioctl

Extension to enable ioctl() call support on PHP. Still in development as a Proof-Of-Concept.

## Installation

```
phpize
./configure --enable-ioctl
make && make install
```

You will need to add `extension=ioctl.so` into your `php.ini` file.

## Usage

`function ioctl(resource fd, int request, string data, boolean is_ptr) : int`

* **fd**: File descriptor resource, open with fopen()
* **request**: Request call, see [ioctl(2)](http://man7.org/linux/man-pages/man2/ioctl.2.html)
* **data**: Buffer to be passed to ioctl() call, this function does not do any bound checking
 as its impossible to determine which kind of data is required by the driver. This is left
 as responsability to PHP script developer.
* **is_ptr**: Whether the ioctl call expects a value (is_ptr = false) or a pointer (is_ptr = true)
 in which case the value will be the first bytes of the string that fit into a pointer data
 type (4 bytes for 32 bit systems, 8 bytes for 64 bit systems) and the pointer will be pointing
 to the string data itself. If the ioctl() call returns some value into a struct pointer,
 the PHP developer should supply a string with enough size to fit the data in.

### Return values

*  **FALSE**: the ioctl() call has failed.
*  **any other value**: the integer result of the ioctl() call.
