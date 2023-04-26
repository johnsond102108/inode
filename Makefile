# build an executable named Bipartite from nodes.c
all: nodes.c
	gcc -g -Wall -o nodes nodes.c

clean:
	$(RM) nodes
