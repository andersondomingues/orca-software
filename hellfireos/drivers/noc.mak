noc:
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/drivers/noc/ni_orca_dma.c \
		$(SRC_DIR)/drivers/noc/noc.c \
		$(SRC_DIR)/drivers/noc/noc_rpc.c
