include config.mk

.PHONY: clean all

all: $(APP_OUT)

clean:
	rm -rf $(BUILD)/*

$(APP_OUT): $(LIBC_OBJ_LIST) $(APP_OBJ_LIST)
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) -o $(APP_OUT) $(APP_OBJ_LIST) $(LIBC_OBJ_LIST)
	@echo "Program Compilled!"


$(APP_OBJ_LIST): $(APP_SRC_LIST) $(APP_ASM_LIST)
	$(CC) $(CFLAGS) -c $(APP_SRC_LIST)
	$(ASM) $(ASMFLAGS) $(APP_ASM_LIST)
	mv $(APP_SRC)/*.o $(BUILD)/

$(LIBC_OBJ_LIST): $(LIBC_SRC_LIST) $(LIBC_ASM_LIST)
	$(CC) $(CFLAGS) -c $(LIBC_SRC_LIST)
	$(ASM) $(ASMFLAGS) $(LIBC_ASM_LIST)
	mv $(LIBC_SRC)/*.o $(LIBC_OBJ)/

