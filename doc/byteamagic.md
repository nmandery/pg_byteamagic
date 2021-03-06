PostgreSQL extension to determinate the filetypes of bytea BLOBs
----------------------------------------------------------------

This extensions basically allows to use [libmagic](http://www.darwinsys.com/file/) - the library behind the "file"
command - on bytea values in the database.

Although it is not really recommended to store files in a database it occasionally
might be the case in practice. This extension might come in handy when the type
of data inside the BLOB has to be identified.

Installation
------------

To build this module you need to have the Postgresql development headers installed
as well as the development package for libmagic [1] (libmagic-dev on debian).

The extension can be build by calling

    make
    make install

"make install" probably needs elevated privileges on your system.

The extension can be loaded in a database by running

    CREATE EXTENSION byteamagic;

on the psql command line.


Functions
---------

byteamagic_mime(bytea)

returns the mimetype of the bytea data as text

This is essentially the same as
> file --mime-type <file>


byteamagic_text(bytea)
returns a text describing the filetype of the bytea data

The output matches
> file <file>


License
-------

Simplified BSD License -- see the COPYING file

nico@nmandery.net 2011
