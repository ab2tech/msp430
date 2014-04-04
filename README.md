msp430
======

Please visit our site for more awesome stuff!
http://www.ab2tech.com

This repository includes our msp430 libraries and tests/examples. The [include](include) directory contains the libraries, and the [src](src) directory contains the examples.

In the include directory, files included in the msp directory are intended for any msp430 device. These will only be tested for the microcontrollers we are supporting (currently msp430f5510 and msp430f2553). The device-specific libraries that we provide will be posted in the respective device folder inside of the [include](include) folder.

The same is true for the [src](src) directory. There may be also sub-directories inside the [src](src) directory indicating an example was intended for a specific platform (e.g. Launchpad).

The way the repository is structured, we are very dependent on Unix-style symbolic links. For use with Windows systems, the repository should ideally be placed on a Unix-based system that the Windows system can access via Samba or some other method. While this isn't ideal for some, it allows us a way to keep the libraries centralized and ensure there aren't multiple copies of a library floating around.

Enjoy! Please give us your feedback and report any issues you might encounter.
