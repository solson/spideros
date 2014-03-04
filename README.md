spideros
========
A hobby operating system written in C++11. A fun experiment to see
what use the new C++11 standard features might be in an operating
system kernel.


Building
--------
Just `make`.


Running in a VM
---------------
Install [qemu](http://wiki.qemu.org/) or [bochs](http://bochs.sourceforge.net/)
and run `make qemu` or`make bochs`.


Running on Real Hardware
------------------------
I haven't tried this yet... but feel free to give it a go. `make`
generates a `spideros.iso` which you should be able to burn to a
CD. Then just boot from the CD.


License
-------
spideros uses the [ISC license](http://en.wikipedia.org/wiki/ISC_license).
