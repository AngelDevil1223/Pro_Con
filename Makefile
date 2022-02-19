CC := cc

all : producer consumer

producer : producer.c
	$(CC) producer.c -o producer

consumer : consumer.c
	$(CC) consumer.c -o consumer

clean:
	rm  consumer producer
