CC := gcc

CFLAGS := -no-pie

LDFLAGS := -z execstack

.PHONY: all clean run

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT) *.o

run: $(OUT)
	./$(OUT)

