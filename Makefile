XBE_TITLE = nxdkSol
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c
NXDK_DIR = /opt/nxdk
NXDK_SDL = y
include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/card01.jpg
$(GEN_XISO): $(OUTPUT_DIR)/card01.jpg
$(OUTPUT_DIR)/card01.jpg: $(CURDIR)/card01.jpg $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'
