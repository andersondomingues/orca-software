
CFLAGS_NOC = $(CFLAGS) -DNOC_HEIGHT=ORCA_NOC_HEIGHT -DNOC_WIDTH=ORCA_NOC_WIDTH

noc:
	$(Q)$(CC) $(CFLAGS_NOC) \
		$(SRC_DIR)/drivers/noc/ni_orca_dma.c \
		$(SRC_DIR)/drivers/noc/noc.c \
		$(SRC_DIR)/drivers/noc/noc_rpc.c
