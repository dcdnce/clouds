SRCS =	srcs/main.cpp \
		libs/glad/glad.cpp \
		srcs/class_shader.cpp \
		srcs/class_camera.cpp \
		srcs/class_engine/class_engine.cpp \
		srcs/class_engine/class_engine_init.cpp \
		srcs/class_engine/class_engine_gui.cpp \
		srcs/class_skydome.cpp \
		srcs/class_debug_plane.cpp \
		srcs/class_terrain.cpp \
		$(wildcard libs/imgui/*.cpp) \
		libs/imgui/backends/imgui_impl_opengl3.cpp \
		libs/imgui/backends/imgui_impl_glfw.cpp	\

OBJS = $(SRCS:%.cpp=%.o)

C = clang++

OS = $(shell uname)

# CFLAGS - Compiler flags (where to find headers)
# LFLAGS - Linking flags (where to find library to link at - -L & -l)

CFLAGS = -Wall -Wextra -Wno-deprecated -Wno-writable-strings -g
CFLAGS += -Iincludes
CFLAGS += -Ilibs/imgui
CFLAGS += -Ilibs/imgui/backends

ifeq ($(OS), Darwin)
	LFLAGS 	= -framework OpenGL 
	LFLAGS += libs/glfw/libglfw.3.3.dylib
else
	CFLAGS += -I/usr/include
	LFLAGS = -L/usr/lib/x86_64-linux-gnu/
	LFLAGS += -lGL -lglfw -ldl
endif

NAME = clouds

%.o	:	%.cpp
	$(C) -g $(CFLAGS) -c $< -o $@

#######################################################################################
#######################################################################################

$(NAME):	$(OBJS)
	$(C) -o $@ $^ $(LFLAGS)

all:	$(NAME)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
