#ifndef INCLUDE_communication_H_
#define INCLUDE_communication_H_

void die(char *s);

void server(int port, int number_of_host, int *s);

void client(char *address, int port, int *s);

#endif