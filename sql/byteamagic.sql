
CREATE FUNCTION byteamagic_mime(bytea) RETURNS text
AS 'byteamagic', 'byteamagic_mime'
IMMUTABLE LANGUAGE C STRICT;

comment on function byteamagic_mime(bytea) is
    'return the mimetype of the bytea data as text';



CREATE FUNCTION byteamagic_text(bytea) RETURNS text
AS 'byteamagic', 'byteamagic_text'
IMMUTABLE LANGUAGE C STRICT;

comment on function byteamagic_text(bytea) is
    'return a text describing the filetype of the bytea data';
