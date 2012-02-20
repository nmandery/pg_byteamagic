EXTENSION = byteamagic
MODULE_big = byteamagic--0.2
OBJS = byteamagic.o
DATA_built = byteamagic--0.2.sql
DOCS = README.byteamagic

# link to libmagic
SHLIB_LINK += -lmagic

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
