PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SDL_LIBS = $(shell sdl2-config --libs)
SDL_LIBS += -lSDL2_image
SDL_LIBS += -lSDL2_ttf

SDL_INCLUDES = $(shell sdl2-config --cflags)

BOOST_LIBS = -lboost_random

OBJECTSDIR += obj/

SRCDIR += src/

OBJECTS += $(OBJECTSDIR)Sim.o
OBJECTS += $(OBJECTSDIR)random_generator.o
OBJECTS += $(OBJECTSDIR)OnInit.o
OBJECTS += $(OBJECTSDIR)OnRender.o
OBJECTS += $(OBJECTSDIR)CEvent.o
OBJECTS += $(OBJECTSDIR)main.o
OBJECTS += $(OBJECTSDIR)OnEvent.o
OBJECTS += $(OBJECTSDIR)OnLoop.o
OBJECTS += $(OBJECTSDIR)OnCleanup.o
OBJECTS += $(OBJECTSDIR)OnExecute.o

CFLAGS += -std=c++17

all:	virus

virus:	$(OBJECTS)
	$(CXX) -o $@ $^ $(SDL_LIBS) $(BOOST_LIBS)

$(OBJECTSDIR)%.o:	$(PROJECT_ROOT)$(SRCDIR)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(SDL_INCLUDES) $(BOOST_LIBS) -o $@ $<

# %.o:	$(PROJECT_ROOT)%.c
# 	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(SDL_INCLUDES) $(BOOST_LIBS) -o $@ $<

clean:
	rm -fr virus $(OBJECTS)
