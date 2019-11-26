# Hisilicon Hi35xx sample Makefile

include ../Makefile.param
#链接opencv库
CFLAGS += `pkg-config --cflags  --libs opencv`
CC = arm-himix200-linux-gcc

MPI_LIBS += /usr/local/lib/libpaho-mqtt3c.so.1
CFLAGS += -I/root/paho.mqtt.c/src

SMP_SRCS := $(wildcard *.c)
TARGET := $(SMP_SRCS:%.c=%)

TARGET_PATH := $(PWD)

# compile linux or HuaweiLite
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
