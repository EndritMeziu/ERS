####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES: all				
.PHONY: clean all dox splint

####################################################################
# Path Definitions                                                 #
####################################################################
OUT_DIR     = ./output
OBJ_DIR     = $(OUT_DIR)/build
EXE_DIR     = $(OUT_DIR)/exe
DOX_DIR     = $(OUT_DIR)/docu
SRC_DIR     = ./src
INCLUDE_DIR = ./include

####################################################################
# Flag Definitions                                                 #
####################################################################
FLAGS = -Wall -c -ansi -g

SPLINTFLAGS = +skip-sys-headers +unixlib +standard -mustfreeonly -likelybool -temptrans \
-usedef -compdestroy -mustdefine -compdef +matchanyintegral -unrecog -dependenttrans \
-exportlocal +ignorequals -type -bufferoverflowhigh -nullstate
## -warnposix -preproc 
INCLUDEPATHS =-I$(INCLUDE_DIR)

####################################################################
# Create build and ouput directory			           #
####################################################################
$(OBJ_DIR):
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)
	@echo "Created build directory."

$(EXE_DIR):
	mkdir -p $(EXE_DIR)
	@echo "Created executable directory."

####################################################################
# SRCOBJ DEPENDENCIES				   #
####################################################################
CLIENT_DPN=$(OBJ_DIR)/client.o \
		   $(OBJ_DIR)/str_serialize.o \
           $(OBJ_DIR)/msg_queue.o \
		   $(OBJ_DIR)/named_pipe.o \
		   $(OBJ_DIR)/opt.o \
		   $(OBJ_DIR)/patient.o \
		   $(OBJ_DIR)/helperfunctions.o \
		   

SERVER_DPN=$(OBJ_DIR)/server.o \
		   $(OBJ_DIR)/str_serialize.o \
           $(OBJ_DIR)/msg_queue.o \
		   $(OBJ_DIR)/named_pipe.o \
		   $(OBJ_DIR)/sh_mem.o \
		   $(OBJ_DIR)/sh_sem.o \
		   $(OBJ_DIR)/opt.o \
		   $(OBJ_DIR)/patient.o \

####################################################################
# Build instructions			 			   #
####################################################################
all: $(OBJ_DIR) $(EXE_DIR) $(EXE_DIR)/server $(EXE_DIR)/client 

$(EXE_DIR)/client: $(CLIENT_DPN)
	gcc $(CLIENT_DPN) -o $(EXE_DIR)/client

$(EXE_DIR)/server: $(SERVER_DPN)
	gcc $(SERVER_DPN) -o $(EXE_DIR)/server
	

$(OBJ_DIR)/helperfunctions.o: $(SRC_DIR)/helperfunctions.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/helperfunctions.o $(SRC_DIR)/helperfunctions.c 

$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/client.o $(SRC_DIR)/client.c 

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/server.o $(SRC_DIR)/server.c 

$(OBJ_DIR)/str_serialize.o: $(SRC_DIR)/str_serialize.c $(INCLUDE_DIR)/str_serialize.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/str_serialize.o $(SRC_DIR)/str_serialize.c

$(OBJ_DIR)/opt.o: $(SRC_DIR)/opt.c $(INCLUDE_DIR)/opt.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/opt.o $(SRC_DIR)/opt.c

$(OBJ_DIR)/msg_queue.o: $(SRC_DIR)/msg_queue.c $(INCLUDE_DIR)/msg_queue.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/msg_queue.o $(SRC_DIR)/msg_queue.c

$(OBJ_DIR)/named_pipe.o: $(SRC_DIR)/named_pipe.c $(INCLUDE_DIR)/named_pipe.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/named_pipe.o $(SRC_DIR)/named_pipe.c

$(OBJ_DIR)/sh_mem.o: $(SRC_DIR)/sh_mem.c $(INCLUDE_DIR)/sh_mem.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/sh_mem.o $(SRC_DIR)/sh_mem.c

$(OBJ_DIR)/sh_sem.o: $(SRC_DIR)/sh_sem.c $(INCLUDE_DIR)/sh_sem.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/sh_sem.o $(SRC_DIR)/sh_sem.c

$(OBJ_DIR)/patient.o: $(SRC_DIR)/patient.c $(INCLUDE_DIR)/patient.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/patient.o $(SRC_DIR)/patient.c



dox:
	mkdir -p $(DOX_DIR)
	@echo "Created doxygen directory."

	doxygen ./doc/exc2.dox

splint:
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/sh_mem.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/sh_sem.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/helperfunctions.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/server.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/client.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/opt.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/msg_queue.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/patient.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/str_serialize.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/named_pipe.c

clean:
	rm -rf $(OBJ_DIR) $(DOX_DIR) $(EXE_DIR) $(OUT_DIR) 
