CC=g++

$(shell mkdir -p bin)
TARGET=bin/app

IDIR=-Iincludes/

INCLUDES += -Iincludes/src
LDIR=-Llib

LIBS=-lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lBulletCollision_gmake_x64_release -lBulletDynamics_gmake_x64_release -lLinearMath_gmake_x64_release
LDDEPS= -lBulletCollision_gmake_x64_release -lBulletDynamics_gmake_x64_release -lLinearMath_gmake_x64_release



all: 
	$(CC) -DBT_USE_DOUBLE_PRECISION  main.cpp includes/glad/glad.c $(INCLUDES) -o $(TARGET) $(IDIR) $(LDIR) $(LIBS) $(LDDEPS) 

clean:
	rm -f $(TARGET)
