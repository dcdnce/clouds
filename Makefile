SRCS =	srcs/main.cpp \
		libs/glad/glad.cpp \
		srcs/Shader.cpp \
		srcs/Engine/Engine.cpp \
		srcs/Engine/Engine_Init.cpp \

OBJS = $(SRCS:%.cpp=%.o)

C = clang++

OS = $(shell uname)

# CFLAGS - Compiler flags (where to find headers)
# LFLAGS - Linking flags (where to find library to link at - -L & -l)

CFLAGS = -Wall -Wextra -Wno-deprecated -g
CFLAGS += -Iincludes

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
