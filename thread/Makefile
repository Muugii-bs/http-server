CC=gcc

default: $(FILE)

$(FILE).o: $(FILE).c
	$(CC) -c $(FILE).c -o $(FILE).o -lpthread

$(FILE): $(FILE).o 
	$(CC) $(FILE).o -o $(FILE) -lpthread

clean:
	-rm -f $(FILE).o
	-rm -f $(FILE)

rebuild: clean $(FILE)
