#last modify zengpw 2016-09-27
#author zengpw 2016-09-27

PROJECT_PATH = $(shell pwd)
PROJECT_NAME = upload_jzt
PROJECT_TYPE = BIN

# SOURCE = $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.cc)
SOURCE = db_interface.cpp tlv_handle.cpp upload.cpp upload_jzt_handle.cpp
SOURCE_INCLUDE = -I $(PROJECT_PATH) -I $(PROJECT_PATH)/../include -I $(ORACLE_SDK) -I /usr/local/include/python3.3m
SOURCE_PATH = $(PROJECT_PATH)

CXX_FLAGS = -DJZT
LINK_LIB = -lcommon -lprotocol -lsession
LINK_LIB_PATH = -L $(PROJECT_PATH)/../output/lib

OUTPUT = $(PROJECT_PATH)/../output/bin
OUTPUT_TEMP = $(PROJECT_PATH)/../output/build_temp

#INSTALL_FILE = $(wildcard *.h)
#INSTALL_PATH = $(PROJECT_PATH)/../include/$(PROJECT_NAME)

include ../common.make