
CC=g++
TARGET=bin/app
IDIR=-Iincludes/

LDIR=-Llib/

LIBS=-lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl


all: 
	$(CC) main.cpp includes/glad/glad.c -o $(TARGET) $(IDIR) $(LDIR) $(LIBS)

clean:
	rm -f $(TARGET)
