
.PHONY: clean all create_file

all:  Receiver Sender

clean:
	rm  -f *.o *.txt Receiver Sender

create_file:
	rm -f send.txt
	python3 create_file.py

Receiver: Receiver.o helper.o dinamic_array.o
	gcc -g Receiver.o helper.o dinamic_array.o -o $@

Sender: Sender.o helper.o create_file
	gcc -g Sender.o helper.o -o $@

Sender.o: Sender.c helper.h
	gcc -g -c Sender.c

Receiver.o: Receiver.c helper.h dinamic_array.h
	gcc -g -c Receiver.c

helper.o: helper.c
	gcc -g -c helper.c

dinamic_array.o: dinamic_array.c
	gcc -g -c dinamic_array.c

