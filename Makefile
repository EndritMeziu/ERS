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
# SPLINTFLAGS = +standard -skipposixheaders -mustfreeonly -likelybool -temptrans -usedef

SPLINTFLAGS = +posixlib +standard -mustfreeonly -likelybool \
			  -temptrans -nullstate -mustdefine -compdef -compdestroy \
			  -dependenttrans -noeffect
## -warnposix -preproc 
INCLUDEPATHS =-I$(INCLUDE_DIR)
SPLINCLUDEPATHS = -I$(INCLUDE_DIR) -I/usr/include/x86_64-linux-gnu

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
           $(OBJ_DIR)/msg_queue.o \
		   $(OBJ_DIR)/named_pipe.o \

SERVER_DPN=$(OBJ_DIR)/server.o \
           $(OBJ_DIR)/msg_queue.o \
		   $(OBJ_DIR)/named_pipe.o \

####################################################################
# Build instructions			 			   #
####################################################################
all: $(OBJ_DIR) $(EXE_DIR) $(EXE_DIR)/server $(EXE_DIR)/client 

$(EXE_DIR)/client: $(CLIENT_DPN)
	gcc $(CLIENT_DPN) -o $(EXE_DIR)/client

$(EXE_DIR)/server: $(SERVER_DPN)
	gcc $(SERVER_DPN) -o $(EXE_DIR)/server

$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/client.o $(SRC_DIR)/client.c 

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/server.o $(SRC_DIR)/server.c 

$(OBJ_DIR)/msg_queue.o: $(SRC_DIR)/msg_queue.c $(INCLUDE_DIR)/msg_queue.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/msg_queue.o $(SRC_DIR)/msg_queue.c

$(OBJ_DIR)/named_pipe.o: $(SRC_DIR)/named_pipe.c $(INCLUDE_DIR)/named_pipe.h
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/named_pipe.o $(SRC_DIR)/named_pipe.c


dox:
	mkdir -p $(DOX_DIR)
	@echo "Created doxygen directory."

	doxygen ./doc/exc2.dox

splint:
	splint $(SPLINTFLAGS) -unreachable $(INCLUDEPATHS) $(SPLINCLUDEPATHS) $(SRC_DIR)/server.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SPLINCLUDEPATHS) $(SRC_DIR)/client.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SPLINCLUDEPATHS) $(SRC_DIR)/msg_queue.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SPLINCLUDEPATHS) $(SRC_DIR)/named_pipe.c

clean:
	rm -rf $(OBJ_DIR) $(DOX_DIR) $(EXE_DIR) $(OUT_DIR) 
