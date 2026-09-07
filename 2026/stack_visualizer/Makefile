CC      = gcc
CFLAGS  = -Wall -Wextra -O0 -fno-stack-protector -g
TARGET  = stack_visualizer

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)

.PHONY: all clean
