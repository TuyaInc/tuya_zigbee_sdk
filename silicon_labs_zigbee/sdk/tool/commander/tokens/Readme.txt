
EFR32 Custom Tokens


======================================================================
 Introduction
======================================================================

The file "tokens-example-efr32.json" provides an example of the token
types and locations currently supported by Simplicity Commander.


======================================================================
 Creating custom token groups
======================================================================

In order to define a custom token group, copy
"tokens-example-efr32.json" to a new file in the same directory using
the naming convention "tokens-<groupname>-efr32.json", for example
"tokens-myapp-efr32.json".

To verify that Simplicity Commander sees the new file, run
"commander.exe tokendump --help". The name of your token group
(i.e. "myapp") should then be listed as a supported token group like
this:

--tokengroup <tokengroup>       Which set of tokens to use. Supported: myapp, znet


======================================================================
 Defining tokens
======================================================================

Each token in the JSON file has the following properties:

name

 The name of the token, which is used as an identifier when dumping or
 writing tokens.

page

 The named memory region to use for the token. See "Memory Regions"
 below.

offset

 The offset in number of bytes from the start of the memory region at
 which to place the token.

sizeB

 The size of the token in bytes.
 A token of size 1 is interpreted as an unsigned 8 bit integer.
 A token of size 2 is interpreted as an unsigned 16 bit integer.
 A token of size 4 is interpreted as an unsigned 32 bit integer.
 Any other size is interpreted as a byte array of the given size.

description

 A plain text description of the token. This property is currently only used as
 documentation of the JSON file.


======================================================================
 Memory Regions
======================================================================

The following values are valid data in the "page" option:

USERDATA

 The data in the user data page is NOT erased via a mass erase
 ("commander.exe flash --masserase", "commander.exe masserase" or when
 disabling debug lock). It can, however, be erased by a specific page
 erase.
 Located at address 0x0FE00000 with size 2 kB on EFR32 devices.

LOCKBITSDATA

 The lock bits page is used by the chip itself to configure page
 locks, debug lock, AAP lock, etc. However, the last 512 bytes of this
 page is unused, and has the important property that it is erased in a
 mass erase event.
 Located at address 0x0FE04000 with size 1 kB on EFR32 devices.
 Tokens in this page must use an offset of at least 0x200, since
 otherwise collisions with chip functionality can occur.


======================================================================
 Using custom token files
======================================================================

To use a custom token file, run Simplicity Commander with a
--tokengroup option corresponding with the name of the JSON file. If
the file was named "tokens-myapp-efr32.json", run commander with the
option "--tokengroup myapp".

In order to create a text file useful as input to the "flash" or
"convert" commands, the easiest way is to start by dumping the current
data from a device.
For example:
commander tokendump -s 440050148 --tokengroup myapp --outfile mytokens.txt

mytokens.txt can then modified to have the desired content, and then
used when flashing devices or creating images:
commander flash -s 440050148 --tokengroup myapp --tokenfile mytokens.txt

