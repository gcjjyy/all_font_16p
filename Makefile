TARGET = all_font

LDFLAGS = -lsdl2

all: $(TARGET)

$(TARGET): all_font.c
	gcc -o $(TARGET) $(LDFLAGS) all_font.c

clean:
	@find . -name '$(TARGET)' -type f -delete

