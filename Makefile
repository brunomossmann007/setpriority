all:
	gcc -o thread_runner thread_runner.c -lpthread -Wall
	gcc -o deadline deadline.c -lpthread -Wall

clean:
	rm thread_runner deadline
