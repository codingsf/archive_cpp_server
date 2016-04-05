#last modify 2016-02-29
#author zengpw 2015-08-07

PROJECT_PATH = $(shell pwd)
PROJECT_NAME = common
PROJECT_TYPE = SO

SOURCE = $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.cc)
SOURCE_INCLUDE = -I $(PROJECT_PATH) -I $(PROJECT_PATH)/../include/common -I $(PROJECT_PATH)/../include -I $(ORACLE_SDK) -I /usr/local/include/python3.3m
SOURCE_PATH = $(PROJECT_PATH)

LINK_LIB = -levent -lpthread -lhiredis -llog4cplus -locci -lclntsh -lpython3 -lpython3.3m
LINK_LIB_PATH = -L $(PROJECT_PATH)/../output/lib -L $(ORACLE_LIB)

OUTPUT = $(PROJECT_PATH)/../output/lib
OUTPUT_TEMP = $(PROJECT_PATH)/../output/build_temp

INSTALL_FILE = $(wildcard *.py)
INSTALL_PATH = $(PROJECT_PATH)/../output/bin

include ../common.make