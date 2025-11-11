CC := gcc

# Compiler flags (m32 is a compiler option)
CFLAGS := -no-pie -m32 -fno-stack-protector

# Linker flags (z execstack is passed to the linker)
LDFLAGS := -z execstack

.PHONY: all clean run

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT) *.o

run: $(OUT)
	./$(OUT)

