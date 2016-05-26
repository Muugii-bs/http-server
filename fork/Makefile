CC=gcc

default: $(FILE)

$(FILE).o: $(FILE).c
	$(CC) -c $(FILE).c -o $(FILE).o

$(FILE): $(FILE).o 
	$(CC) $(FILE).o -o $(FILE)

clean:
	-rm -f $(FILE).o
	-rm -f $(FILE)

rebuild: clean $(FILE)
