#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/brin
#
# IDENTIFICATION
#    src/backend/access/brin/Makefile
#
#-------------------------------------------------------------------------

subdir = src/backend/access/brin_ao
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = brin_ao.o brin_ao_pageops.o brin_ao_revmap.o brin_ao_tuple.o brin_ao_xlog.o \
       brin_ao_minmax.o brin_ao_inclusion.o brin_ao_validate.o

include $(top_srcdir)/src/backend/common.mk
