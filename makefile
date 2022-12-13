
.PHONY: clean all

all: Receiver Sender
	./Receiver

Receiver: Receiver.o
	gcc -g Receiver.o helper.c -o $@

Sender: Sender.o
	gcc -g Sender.o helper.c -o $@

clean:
	rm Receiver Sender

Sender.o: Sender.c helper.h
	gcc -g -c Sender.c

Receiver.o: Receiver.c helper.h
	gcc -g -c Receiver.c
