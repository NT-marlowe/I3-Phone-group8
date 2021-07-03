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

#define N 1024

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
		fprintf(stderr, "usage as client: ./bin/main IPADDRESS PORT\n");
		fprintf(stderr, "usage as server: ./bin/main -l PORT NUMBER_OF_CLIENT\n");
		exit(EXIT_FAILURE);
	}

	if (is_server) { // server
		int port = atoi(argv[2]);
		number_of_client = atoi(argv[3]);
		ss = (int*)calloc(number_of_client, sizeof(int));
    if (ss == NULL) die("calloc");
		server(port, number_of_client, ss);
	}
	else { // client
		char *address = argv[1];
		int port = atoi(argv[2]);
		client(address, port, &s);
	}

	/* ここで接続完了 */

	if (is_server) { // server
		Args_pthread arg[number_of_client];
		pthread_t tid[number_of_client]; // スレッド識別変数
		signed short **waves = (signed short**)calloc(number_of_client, sizeof(signed short *));
		signed short *base_waves = (signed short*)calloc(N * number_of_client, sizeof(signed short));

		for (int i = 0; i < number_of_client; ++i){
				// tid[i] = i;
				arg[i].s = ss[i];
				waves[i] = base_waves + i * N;
				arg[i].buf = waves[i];
		}

		while(1){
			for (int i = 0; i < number_of_client; ++i){
				pthread_create(&tid[i], NULL, receive_data_from_client, (void*)&arg[i]);
			}

			for (int i = 0; i < number_of_client; ++i){
				pthread_join(tid[i], NULL);
			}

			signed short *result = (signed short*)calloc(N, sizeof(signed short));
			sum_waves(waves, result, number_of_client);

			for (int i = 0; i < number_of_client; ++i){
				arg[i].buf = result;
				pthread_create(&tid[i], NULL, send_data_to_client, (void*)&arg[i]);
			}

			for (int i = 0; i < number_of_client; ++i){
				pthread_join(tid[i], NULL);
			}

			free(result);
		}

		free(base_waves);
		free(waves);
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