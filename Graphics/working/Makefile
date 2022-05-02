INCFLAGS = -I.
LDFLAGS  = -L/usr/X11R6/lib
LIBS     = -lGL -lGLU -lglut -lX11 -lm

CC       = g++
CFLAGS   = $(INCFLAGS) $(LDFLAGS)

sources  = \
	Main.c \
	DotDevice.c \
	ScanConversion.c \

OBJS     = $(sources:.c=.o)

Main: $(OBJS)
	$(CC) $(CFLAGS) -o render $(OBJS) $(LIBS)

