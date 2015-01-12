
NAME  = firmware

OBJS += firmware.o
OBJS += messages.o
OBJS += protob/pb_decode.o
OBJS += protob/pb_encode.o
OBJS += protob/messages.pb.o
OBJS += protob/storage.pb.o
OBJS += protob/types.pb.o

include Makefile.include

CFLAGS += -Iprotob -DPB_FIELD_16BIT=1
