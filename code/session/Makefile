#last modify zengpw 2016-02-29
#author zengpw 2015-09-16

PROJECT_PATH = $(shell pwd)
PROJECT_NAME = session
PROJECT_TYPE = SO

SOURCE = $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.cc)
SOURCE_INCLUDE = -I $(PROJECT_PATH) -I $(PROJECT_PATH)/../include/session -I $(PROJECT_PATH)/../include -I $(ORACLE_SDK) -I /usr/local/include/python3.3m
SOURCE_PATH = $(PROJECT_PATH)

LINK_LIB = -lcommon -lprotocol
LINK_LIB_PATH = -L $(PROJECT_PATH)/../output/lib

OUTPUT = $(PROJECT_PATH)/../output/lib
OUTPUT_TEMP = $(PROJECT_PATH)/../output/build_temp

#INSTALL_FILE = $(wildcard *.h)
#INSTALL_PATH = $(PROJECT_PATH)/../include/$(PROJECT_NAME)

include ../common.make