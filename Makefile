CMAKE ?= cmake
CTEST ?= ctest
HOST_PRESET ?= host-debug
STM32_PRESET ?= stm32f407-debug
HOST_BUILD_DIR ?= build/host-debug
STM32_BUILD_DIR ?= build/stm32f407-debug

.PHONY: test host-configure host-build host-test stm32-configure stm32-build clean

test: host-test

host-sync-compile-commands:
	$(CMAKE) -E copy_if_different $(HOST_BUILD_DIR)/compile_commands.json compile_commands.json

stm32-sync-compile-commands:
	$(CMAKE) -E copy_if_different $(STM32_BUILD_DIR)/compile_commands.json compile_commands.json

host-configure:
	$(CMAKE) --preset $(HOST_PRESET)
	$(MAKE) host-sync-compile-commands

host-build: host-configure
	$(CMAKE) --build --preset $(HOST_PRESET)
	$(MAKE) host-sync-compile-commands

host-test: host-build
	$(CTEST) --preset $(HOST_PRESET)

stm32-configure:
	$(CMAKE) --preset $(STM32_PRESET)
	$(MAKE) stm32-sync-compile-commands

stm32-build: stm32-configure
	$(CMAKE) --build --preset $(STM32_PRESET)
	$(MAKE) stm32-sync-compile-commands

clean:
	$(CMAKE) -E rm -rf build
	$(CMAKE) -E rm -f compile_commands.json
