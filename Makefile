INCLUDES_DIR = includes
LIBFT_DIR = $(INCLUDES_DIR)/libft

define compile_lib
	@echo "compiling library"
	$(MAKE) -C $(1) $(2)
endef

all: clib

clib:
	$(call compile_lib,$(LIBFT_DIR),all)
