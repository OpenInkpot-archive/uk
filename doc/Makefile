CFLAGS=$(shell pkg-config --cflags eina-0 ecore ecore-con)
LDFLAGS=$(shell pkg-config --libs eina-0 ecore ecore-con)

sampleclient: sampleclient.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

clean:
	-rm sampleclient