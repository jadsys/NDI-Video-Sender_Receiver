ifeq ($(NDILIB),)
	CC    ?= gcc
	NDILIB = $(shell $(CC) -dumpmachine)
endif

BMDSDK = NDIlib_Send_BMD/BMDSDK/Linux

PROJECTS = \
                   NDI-Video-Receiver \
                   NDI-Video-Sender \


.PHONY: all
all:
	@test -d ../../lib/$(NDILIB) || (echo NDILIB not found: $(NDILIB) >&2 ; false)
	-@for proj in $(PROJECTS); do \
		$(MAKE) -C $$proj -f ../Makefile.proj NDILIB=$(NDILIB); \
	done

.PHONY: clean
clean:
	-@for proj in $(PROJECTS); do \
		$(MAKE) -C $$proj -f ../Makefile.proj clean; \
	done

