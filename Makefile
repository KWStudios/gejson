CC = gcc
CFLAGS = -I ./include -Wall -std=c99 -ggdb
LDFLAGS = 

SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

DEP = $(OBJ:.o=.d)

libgejson.a: $(DEP) $(OBJ)
	$(info Making $@)
	ar -rcs lib/libgejson.a $(OBJ)
	cp src/gejson.h include/gejson.h

$(OBJDIR)/%.d: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -E -MM -MF $@ -c $^

include $(DEP)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(info Making $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f lib/libgejson.a
	rm -f include/gejson.h
	rm -f $(OBJDIR)/*
