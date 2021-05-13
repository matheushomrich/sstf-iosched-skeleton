/*
 * Simple disc I/O generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 512
#define DISK_SZ	1073741824

int main(){
	int ret, fd, pid, i;
	unsigned int pos;
	char buf[BUFFER_LENGTH];

	printf("Starting sector read example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches"); // limpa buffers e caches de disco

	printf("Configuring scheduling queues...\n");
	system("echo 2 > /sys/block/sdb/queue/nomerges"); // desabilita merges de i/o
	system("echo 4 > /sys/block/sdb/queue/max_sectors_kb"); //4Kb para o tamanho máximo de uma requisição e
	system("echo 0 > /sys/block/sdb/queue/read_ahead_kb"); //para que o sistema faça a leitura de mais conteúdo do que o que foi requisitado.

	srand(getpid());

	fd = open("/dev/sdb", O_RDWR);
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}

	strcpy(buf, "hello world!");

	for (i = 0; i < 50; i++){
		pos = (rand() % (DISK_SZ >> 9));
		/* Set position */
		lseek(fd, pos * 512, SEEK_SET);
		/* Peform read. */
		read(fd, buf, 100);
	}
	close(fd);

	return 0;
}
