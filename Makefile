
MAKEFLAGS += --silent

HOME_DIR = $(HOME)
LOC_FILE = $(HOME_DIR)/.loc.json

define ReadLoc
$(shell node -p "require('$(LOC_FILE)').$(1)")
endef

LIB = tokenizer

BUILD_DIR = build
BUILD_DEV_DIR = $(BUILD_DIR)/dev
LIB_DIR = library
CONFIG_DIR = config

# change to ON for building shared libs
SHARED = ON

.PHONY: build
build: clean
	echo "building release ..."
	if [[ "$(SHARED)" == "OFF" ]]; then \
		echo "compiling static library..."; \
		cmake -S . -B build -D CMAKE_BUILD_TYPE=Release; \
	elif [[ "$(SHARED)" == "ON" ]]; then \
		echo "compiling shared library"; \
		cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON; \
	else \
		echo "set SHARED variable to ON or OFF to select library type."; \
		exit 1; \
	fi
	cmake --build build

.PHONY: dev
dev:
	echo "building dev"
	cmake --preset=dev
	cmake --build --preset=dev

.PHONY: test
test:
	echo "running test"
	$(BUILD_DEV_DIR)/test/$(LIB)_test

.PHONY: clean
clean:
	echo "cleaning build dir ..."
	rm -rf $(BUILD_DIR)

# Initialize the project
.PHONY: init
init:
	echo "initializing project..."
	mkdir -p $(CONFIG_DIR)
	mkdir -p $(LIB_DIR)
	echo "done!"

# if install or uninstall is called, check if the directories exist
ifneq ($(filter install uninstall, $(MAKECMDGOALS)),)
$(if $(call ReadLoc, bin),,$(error ensure there is a .loc.json file in the home directory. if not run `make loc`))
endif

# PATH directories, where system wide binaries are stored
PATH_PREFIX = $(call ReadLoc,library)/libs
P_PATH_LIB_DIR = $(call ReadLoc,library)/$(LIB)
P_PATH_CONFIG_DIR = $(call ReadLoc,config)/$(LIB)

# Move all binaries to respective directories
.PHONY: install
install: uninstall
	echo "installing project library..."
	mkdir -p $(P_PATH_LIB_DIR)
	cp -r $(LIB_DIR)/* $(P_PATH_LIB_DIR) 2>/dev/null || :
	echo "installing lib..."
	cmake --install $(BUILD_DIR) --prefix $(PATH_PREFIX)
	echo "installing config files..."
	mkdir -p $(P_PATH_CONFIG_DIR)
	cp -r $(CONFIG_DIR)/* $(P_PATH_CONFIG_DIR) 2>/dev/null || :
	echo "removing unnessesary path directories..."
	rmdir $(P_PATH_LIB_DIR) 2>/dev/null || :
	rmdir $(P_PATH_CONFIG_DIR) 2>/dev/null || :
	echo "done!"

# Remove all project files from respective sys-wide directories
.PHONY: uninstall
uninstall:
	echo "uninstalling project library..."
	rm -rf $(P_PATH_LIB_DIR)
	echo "uninstalling config files..."
	rm -rf $(P_PATH_CONFIG_DIR)
	echo "uninstalling lib (if exists)"
	cat $(BUILD_DIR)/install_manifest.txt | xargs rm -f
	find $(PATH_PREFIX) -name "*$(LIB)*" -type d -exec rm -rf {} \; 2>/dev/null || :
	echo "Done!"

.PHONY: help
help:
	echo "Available targets:"
	echo "  build - Build the release version"
	echo "  dev   - Build the development version"
	echo "  test  - Run tests"
	echo "  clean - Clean the build directory"
	echo "  init  - Create supplemental development directories"
	echo "  install - Install the library"
	echo "  uninstall - Uninstall the library"
	echo "  help  - Display this help message"

TARGET_LIB = $(HOME)/dev/.library
TARGET_CONF = $(HOME)/dev/.config
TARGET_BIN = $(HOME)/dev/.bin
TARGET_SCRIPTS = $(HOME)/dev/.scripts
TARGET_WORK = $(HOME)/dev/workspace

.PHONY: loc
loc:
	echo "downloading loc-maker..."
	curl -L -O https://github.com/SPoovathumkadavil/loc-maker/releases/download/release/loc-maker
	chmod +x loc-maker
	echo "creating .loc.json file..."
	./loc-maker -n library=$(TARGET_LIB) bin=$(TARGET_BIN) config=$(TARGET_CONF) scripts=$(TARGET_SCRIPTS) workspace=$(TARGET_WORK)
	echo "cleaning up..."
	rm loc-maker
	echo "done."
