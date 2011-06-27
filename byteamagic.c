/*
 * PostgreSQL extension to determinate the filetypes of bytea BLOBs
 *
 * Copyright (c) Nico Mandery 2011.
 * nico@nmandery.net
 */

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "magic.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/*#define BM_DEBUG*/

#ifdef BM_DEBUG
#define bm_debug(A)    ereport(NOTICE, (errmsg(A)));
#else
#define bm_debug(A)
#endif

static struct magic_set * magic_c = NULL;
static int m_flags = 0;

/* protoypes */
static text * magic_bytea_info(bytea*, int);
Datum byteamagic_mime(PG_FUNCTION_ARGS);
Datum byteamagic_text(PG_FUNCTION_ARGS);
void _PG_fini(void);

/* db functions */
PG_FUNCTION_INFO_V1(byteamagic_mime);
PG_FUNCTION_INFO_V1(byteamagic_text);


static text *
magic_bytea_info(bytea * b, int magic_flags)
{
    text *              ret_text;
    const char *        magic_result;
    char *              p_magic_err;
    const char *        magic_err;


    if (magic_c == NULL) {

        bm_debug("open the magic library");

        magic_c = magic_open(magic_flags);
        if (magic_c == NULL) {
            ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                    errmsg("could not open libmagic")));
        }

        // try to open the default magic database
        if (magic_load(magic_c, NULL) == -1) {
            magic_close(magic_c);
            magic_c = NULL;

             ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                    errmsg("could not load the default libmagic database")));
        }
        m_flags = magic_flags;
    }

    // change flags if they differ from last call
    if (magic_flags != m_flags) {
        magic_setflags(magic_c, magic_flags);
        m_flags = magic_flags;
    }

    magic_result = magic_buffer(magic_c, VARDATA_ANY(b),
            VARSIZE_ANY_EXHDR(b));
    if (magic_result == NULL) {

        magic_err = magic_error(magic_c);

        p_magic_err = palloc( (strlen(magic_err) + 1) * sizeof(char) );
        strcpy(p_magic_err, magic_err);

        magic_close(magic_c);

        ereport(ERROR,
            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                errmsg("libmagic error: %s", p_magic_err)));
    }

    ret_text = cstring_to_text(magic_result);

    return ret_text;
}

/**
 * unload callback
 *
 * close the magic library if it has been opened
 *
 * !! POSTGRES DOES NOT CALL THIS ... see PG docs
 */
void
_PG_fini(void)
{
    if (magic_c != NULL) {
        magic_close(magic_c);
        magic_c = NULL;
    }
}


/**
 * return the mimetype of the bytea data as text
 */
Datum
byteamagic_mime(PG_FUNCTION_ARGS)
{
    bytea * b = PG_GETARG_BYTEA_PP(0);

    // just return NULL if getting NULL
    if (b == NULL) {
        PG_RETURN_NULL();
    }

    PG_RETURN_TEXT_P(magic_bytea_info(b, MAGIC_MIME_TYPE));
}


/**
 * return a text describing the filetype of the bytea data
 */
Datum
byteamagic_text(PG_FUNCTION_ARGS)
{
    bytea * b = PG_GETARG_BYTEA_PP(0);

    // just return NULL if getting NULL
    if (b == NULL) {
        PG_RETURN_NULL();
    }

    PG_RETURN_TEXT_P(magic_bytea_info(b, MAGIC_NONE));
}


