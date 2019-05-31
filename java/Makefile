#
# $Id: Makefile,v 1.5 2004/10/05 03:50:43 evertonm Exp $
#


JAVAC = javac
JAVAH = javah
JAR   = jar

SRC_DIR     = src
BUILD_DIR   = build
INSTALL_DIR = install

JNI_DIR      = $(SRC_DIR)/jni
BUILT_STAMP  = $(BUILD_DIR)/built
SRC_FILES    = $(shell find $(SRC_DIR) -name "*.java")
JAVA_RULI_SO = $(JNI_DIR)/libjava-ruli.so

.PHONY: default
.PHONY: clean
.PHONY: build
.PHONY: install


default: $(BUILT_STAMP) $(JAVA_RULI_SO)

clean:
	$(MAKE) -C $(JNI_DIR) clean
	rm -rf $(BUILD_DIR)
	rm -f `find . -name '*~'`

build: clean default


$(BUILT_STAMP): $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(JAVAC) -d $(BUILD_DIR) $(SRC_FILES)
	touch $@

$(JNI_DIR)/ruli_RuliSyncImp.h:
	$(JAVAH) -classpath $(BUILD_DIR) -d $(JNI_DIR) ruli.RuliSyncImp

$(JAVA_RULI_SO): $(JNI_DIR)/ruli_RuliSyncImp.h
	$(MAKE) -C $(JNI_DIR)


install: $(BUILT_STAMP) $(JAVA_RULI_SO)
	mkdir -p $(INSTALL_DIR)/lib
	cp src/jni/libjava-ruli.so $(INSTALL_DIR)/lib
	mkdir -p $(INSTALL_DIR)/classes
	$(JAR) cf $(INSTALL_DIR)/classes/java-ruli.jar -C $(BUILD_DIR) ruli
	@echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(INSTALL_DIR)/lib'
	@echo 'export CLASSPATH=$$CLASSPATH:$(INSTALL_DIR)/classes/java-ruli.jar'

