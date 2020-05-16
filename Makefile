
DEST_PKG=NVSAVR_AD567x
DEST_PKG_VERSION="1.0.0"

DEST_PKG_PATH=$(DEST_PKG)_$(DEST_PKG_VERSION).zip
DEST_PKG_FILES=NVSAVR_AD567x.h keywords.txt LICENSE.txt Makefile README.md examples

all: release

release: $(DEST_PKG_FILES)
	@echo "packing " $(DEST_PKG) "(v$(DEST_PKG_VERSION)): "
	@rm -f $(DEST_PKG_PATH)
	@zip -r $(DEST_PKG_PATH) $(DEST_PKG_FILES)
	@echo "done"

clean:
	@rm -f $(DEST_PKG_PATH)

clean-all:
	@rm -f $(DEST_PKG)_*.zip