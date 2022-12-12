
.PHONY: clean run

all: Receiver Sender run

Receiver: Receiver.o
	gcc Receiver.o helper.c -o $@

Sender: Sender.o
	gcc Sender.o helper.c -o $@

clean:
	rm Receiver Sender

Sender.o: Sender.c helper.h
	gcc -c Sender.c

Receiver.o: Receiver.c helper.h
	gcc -c Receiver.c
