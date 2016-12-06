#last modify zengpw 2016-09-27
#author zengpw 2016-09-27

SUBMK = $(wildcard *.mk)

.PHONY: all

all:
	for mk in $(SUBMK);	\
	do $(MAKE) -f $$mk cm || exit 1;	\
	done