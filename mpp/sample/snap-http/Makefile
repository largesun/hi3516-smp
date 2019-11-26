# Hisilicon Hi35xx sample Makefile

include ../Makefile.param

SMP_SRCS := $(wildcard *.c)
#TARGET := $(SMP_SRCS:%.c=%)

TARGET := sample_snap
TARGET_PATH := $(PWD)

# compile linux or HuaweiLite
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
