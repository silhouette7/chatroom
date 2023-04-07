include rules.mk

SERVER_PATH := server/
SERVER_TARGET := server
CLIENT_PATH := client/
CLIENT_TARGET := client
COMMON_PATH := common/
OUTPUT := output/

all: install
	@echo --compile successfully--

.PHONY: install
install: server client common
	mkdir -p $(OUTPUT)
	mv $(SERVER_PATH)$(SERVER_TARGET) $(OUTPUT)
	mv $(CLIENT_PATH)$(CLIENT_TARGET) $(OUTPUT)

.PHONY: server
server: common
	$(MAKE) -C $(SERVER_PATH)

.PHONY: client
client: common
	$(MAKE) -C $(CLIENT_PATH)

.PHONY: common
common:
	$(MAKE) -C $(COMMON_PATH)

.PHONY: clean
clean:
	$(MAKE) clean -C $(SERVER_PATH)
	$(MAKE) clean -C $(CLIENT_PATH)
	$(MAKE) clean -C $(COMMON_PATH)
	rm -rf $(OUT)