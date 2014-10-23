NAME    := psdthumb
HASH    := md5sum
BIN_DIR := /usr/local/bin

all: build

build: $(NAME).c
	gcc -Wall -Wextra -o $(NAME) $< -lm

test: build
	./$(NAME) in.psd out.jpg
	$(HASH) out.jpg cmp.jpg

memcheck:
	gcc -o $(NAME) $(NAME).c -lm -O0 -g
	valgrind ./$(NAME) in.psd out.jpg

install:
	install -m 0755 $(NAME) $(BIN_DIR)

uninstall:
	rm -f $(BINDIR)/$(NAME)

clean:
	rm $(NAME) out.jpg