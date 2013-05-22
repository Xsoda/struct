TARGET = struct_test
CFLAGS= -Wall -g
SOURCE = struct.c main.c
SOURCE_OBJ = $(addsuffix .o, $(basename $(SOURCE)))

all: $(TARGET)

$(TARGET): $(SOURCE_OBJ)
	$(CC) -W1 -O2 $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(SOURCE_OBJ)