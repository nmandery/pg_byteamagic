EXTENSION 		= byteamagic
EXTVERSION 		= $(shell grep default_version $(EXTENSION).control | \
	sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")


MODULE_big 		= $(EXTENSION)
MODULES 		= $(patsubst %.c,%,$(wildcard src/*.c))
DATA			= $(filter-out $(wildcard sql/*--*.sql),$(wildcard sql/*.sql))
# link to libmagic
SHLIB_LINK		+= -lmagic
DOCS			= $(wildcard doc/*.md)
PG_CONFIG    	= pg_config
PG91 			= $(shell $(PG_CONFIG) --version | grep -qE " 8\.| 9\.0" && echo no || echo yes)


ifeq ($(PG91),yes)
all: sql/$(EXTENSION)--$(EXTVERSION).sql

sql/$(EXTENSION)--$(EXTVERSION).sql: sql/$(EXTENSION).sql
	cp $< $@

DATA 			= $(wildcard sql/*--*.sql) sql/$(EXTENSION)--$(EXTVERSION).sql
EXTRA_CLEAN 	= sql/$(EXTENSION)--$(EXTVERSION).sql
endif

PGXS 			:= $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
