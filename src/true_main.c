#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "../include/instrumental.h"
#include "../include/communication.h"
#include "../include/die.h"
#include "../include/mypthread.h"


// ↑キー入力の後、a,bなどを入力するとオクターブ上が出る
// ↓キー入力の後、a,bなどを入力するとオクターブ下が出る
// 0(zero)入力の後、サイン波が鳴る(デフォルト)
// 1入力の後、オルガンが鳴る

int main(int argc, char **argv) {

  int s = socket(PF_INET, SOCK_STREAM, 0);
  int *ss = NULL;
	int number_of_client = 0;
	
	int is_server = -1;
	if (argc == 3) is_server = 0;
	else if (argc == 4 && strcmp(argv[1], "l") == 0) is_server = 1;
	else {
		fprintf("usage as client: ./bin/main IPADDRESS PORT\n");
		fprintf("usage as server: ./bin/main -l PORT NUMBER_OF_CLIENT\n");
		exit(EXIT_FAILURE);
	}

	if (is_server) { // server
		int port = atoi(argv[2]);
		number_of_client = atoi(argv[3]);
		ss = (int*)calloc(number_of_client, sizeof(int));
    if (ss == NULL) die("calloc");
		server(port, number_of_client, )
	}
	else { // client
		char *address = argv[1];
		int port = atoi(argv[2]);
		client(address, port, *s);
	}

	/* ここで接続完了 */

	if (is_server) {
		// pthread_create;
		// join;
		sum_waves();
		// pthread_create;
		// join;
	}

	else {
		// send_music_from_client とrecv_music_by_clientをpthreadで並列に
	}
				

	close(s);
	if (ss != NULL) {
		for (int i = 0; i < number_of_client; i++) close(ss[i]);
		free(ss);
	}
	return 0;
}