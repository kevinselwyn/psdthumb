NAME   := psdthumb
HASH   := md5sum
BINDIR := /usr/local/bin

all: $(NAME)

$(NAME): $(NAME).c
	gcc -o $@ $< -lm

test: $(NAME)
	./$(NAME) in.psd out.jpg
	$(HASH) out.jpg cmp.jpg

memcheck:
	gcc -o $(NAME) $(NAME).c -lm -O0 -g
	valgrind ./$(NAME)

install: $(NAME)
	install -m 0755 $< $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(NAME)

clean:
	rm $(NAME) out.jpg
