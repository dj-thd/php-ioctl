<?php

// Testing code for ioctl extension

// Defines taken from linux headers
define('KDGKBLED', 0x4B64);
define('KDSKBLED', 0x4B65);

define('K_CAPSLOCK', 0x04);

// Open tty device
$f = fopen('/dev/tty0', 'r');

echo "Get LED status\n";
// Fill 8 byte buffer to get status
$status = str_repeat("\x00", 8);
var_dump(ioctl($f, KDGKBLED, $status, true));
var_dump(bin2hex($status));
echo "\n";

sleep(1);

echo "Set caps lock LED\n";
// Pack as 64 bit little endian, to ensure it will work on both intel 32 and 64 bit systems
$status = pack('Q', K_CAPSLOCK);
var_dump(ioctl($f, KDSKBLED, $status, false));
echo "\n";

sleep(1);

echo "Get LED status\n";
// Fill 8 byte buffer to get status
$status = str_repeat("\x00", 8);
var_dump(ioctl($f, KDGKBLED, $status, true));
var_dump(bin2hex($status));
echo "\n";

sleep(1);

echo "Unset caps lock LED\n";
// Pack as 64 bit little endian, to ensure it will work on both intel 32 and 64 bit systems
// We set value 0 to switch off all leds
$status = str_repeat("\x00", 8);
var_dump(ioctl($f, KDSKBLED, $status, false));
echo "\n";

sleep(1);

echo "Get LED status\n";
// Fill 8 byte buffer to get status
$status = str_repeat("\x00", 8);
var_dump(ioctl($f, KDGKBLED, $status, true));
var_dump(bin2hex($status));
echo "\n";
