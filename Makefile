CC = gcc
CFLAGS = -g -Wall #-o2
#VPATH = src obj
vpath %.c src
vpath %.h inc
vpath %.o obj


BDIR = bin 
IDIR = inc
ODIR = obj
SDIR = src 
INC = -I$(IDIR) 
LIBS =  -lm

# OBJS_ = read_gmsh.o \
# 	mesh_print.o \
# 	face_interpolation.o\
# 	equation.o \
# 	error.o\
# 	ssms.o\
# 	field.o\
# 	sim.o\
# 	bicgstab.o\
# 	hashTable.o\
# 	vector.o\
# 	test_bicgstab.o

OBJS_ = bicgstab.o\
	vector.o\
	test_bicgstab.o\
	ssms.o\
	error.o


OBJS = $(patsubst %,$(ODIR)/%,$(OBJS_))

all: test_bicgstab

test_read_gmsh: test_read_gmsh.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

test_ssms: test_ssms.o $(OBJS) 
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

test_field: test_field.o $(OBJS) 
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

test_bicgstab:  $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

test_hashTable: test_hashTable.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

fs : main.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@

$(ODIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -o $@ $< 	

.PHONY: clean

clean: 
	rm -rf *.o $(ODIR)/*.o 
