MODULE_big = byteamagic
OBJS = byteamagic.o
DATA_built = byteamagic.sql
DOCS = README.byteamagic

# link to libmagic
SHLIB_LINK += -lmagic

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
