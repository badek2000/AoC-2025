CC      := gcc
CFLAGS  := -std=c17 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L -I./aoclib/include
LDLIBS  := -lm

AOC_LIBDIR := aoclib
AOC_LIB    := $(AOC_LIBDIR)/build/libaoc.a

BINDIR     := build

# All day directories that have main.c, e.g. day1/, day2/, ...
DAY_SRCS   := $(wildcard day*/main.c)
DAY_DIRS   := $(sort $(dir $(DAY_SRCS)))
DAY_NAMES  := $(notdir $(patsubst %/,%,$(DAY_DIRS)))      # day1, day2, ...
DAY_BINS   := $(addprefix $(BINDIR)/,$(DAY_NAMES))        # build/day1, build/day2, ...

.PHONY: all clean aoclib $(DAY_NAMES)

# Default: build all days
all: $(DAY_NAMES)

# Build only the library
aoclib: $(AOC_LIB)

# Make sure the library is built (delegates to aoclib/Makefile)
$(AOC_LIB):
	$(MAKE) -C $(AOC_LIBDIR)

# Ensure build/ exists
$(BINDIR):
	mkdir -p $(BINDIR)

# Compile each day's main.c -> dayX/main.o
%/main.o: %/main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Phony targets: "day1" -> depends on "build/day1"
$(DAY_NAMES): %: $(BINDIR)/%

# Link: build/dayX from dayX/main.o + libaoc
$(BINDIR)/%: %/main.o $(AOC_LIB) | $(BINDIR)
	$(CC) $(CFLAGS) $< $(AOC_LIB) $(LDLIBS) -o $@

clean:
	$(MAKE) -C $(AOC_LIBDIR) clean
	rm -f $(DAY_DIRS:%=%main.o) $(DAY_BINS)
