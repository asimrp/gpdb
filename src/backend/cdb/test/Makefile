subdir=src/backend/cdb
top_builddir=../../../..
include $(top_builddir)/src/Makefile.global

TARGETS=cdbbufferedread \
	cdbbackup \
	cdbdisp \
	cdbfilerep \
	cdbgang

include $(top_builddir)/src/backend/mock.mk

cdbgang.t: \
	$(MOCK_DIR)/backend/catalog/namespace_mock.o \
	$(MOCK_DIR)/backend/storage/lmgr/proc_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/redzone_handler_mock.o

cdbdisp.t: \
	$(MOCK_DIR)/backend/cdb/cdbrelsize_mock.o \
	$(MOCK_DIR)/backend/cdb/cdbsrlz_mock.o \
	$(MOCK_DIR)/backend/utils/error/assert_mock.o \
	$(MOCK_DIR)/backend/executor/execUtils_mock.o

cdbfilerep.t: \
	$(MOCK_DIR)/backend/postmaster/fork_process_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/redzone_handler_mock.o
