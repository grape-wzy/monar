CMAKE ?= cmake
CTEST ?= ctest
HOST_PRESET ?= host-debug
STM32_PRESET ?= stm32f407-debug

.PHONY: test host-configure host-build host-test host-sync-compile-commands \
	stm32-configure stm32-build stm32-sync-compile-commands clean

test: host-test

host-sync-compile-commands: host-configure
	$(CMAKE) --build --preset $(HOST_PRESET) --target monar_sync_compile_commands

stm32-sync-compile-commands: stm32-configure
	$(CMAKE) --build --preset $(STM32_PRESET) --target monar_sync_compile_commands

host-configure:
	$(CMAKE) --preset $(HOST_PRESET)

host-build: host-configure
	$(CMAKE) --build --preset $(HOST_PRESET)

host-test: host-build
	$(CTEST) --preset $(HOST_PRESET)

stm32-configure:
	$(CMAKE) --preset $(STM32_PRESET)

stm32-build: stm32-configure
	$(CMAKE) --build --preset $(STM32_PRESET)

clean:
	$(CMAKE) -E rm -rf build
	$(CMAKE) -E rm -f compile_commands.json
