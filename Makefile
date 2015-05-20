
NAME  = firmware

OBJS += main.o
OBJS += protob/messages.pb.o
#OBJS += protob/storage.pb.o
OBJS += protob/types.pb.o
#OBJS += protob/config.pb.o

include Makefile.include


