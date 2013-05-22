TARGET = struct_test
CFLAGS= -Wall -g
SOURCE = struct.c main.c
SOURCE_OBJ = $(SOURCE:.c=.o)

all: $(TARGET)

$(TARGET): $(SOURCE_OBJ)
	$(CC) -Wl -O2 $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(SOURCE_OBJ)