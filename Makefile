VPAHT = src
vpath %.h src
vpath %.c src
objects = main.o core.o fileUtil.o data.o

CFLAGS = -DDEBUG

mingjing : $(objects)
	cc -o mingjing $(CFLAGS) $(objects) -lssl -lcrypto

$(objects) : %.o : %.c core.h
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean : 
	-rm mingjing $(objects)
