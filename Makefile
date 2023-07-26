#########################################################################
# No need to change these                                               #
#########################################################################
TARGET=prosim

#########################################################################
# All C files should be added below, after main.c, separated by spaces. #
#########################################################################
SRC_FILES=main.c context.c prio_q.c process.c

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	gcc -Wall -g -o $(TARGET) $(SRC_FILES) -l pthread
