#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_LEN 10000

struct sm_region {
	int i;
	int n[MAX_LEN];
};

int main() {
	struct sm_region *sm_ptr;
	int fd;

	fd = shm_open("/collatz_region", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		printf("Erro ao abrir memoria compartilhada.\n");
		exit(1);
	}

	if(ftruncate(fd, sizeof(struct sm_region)) == -1) {
		printf("Erro ao alocar memoria compartilhada.\n");
		exit(1);
	}

	sm_ptr = mmap(NULL, sizeof(struct sm_region), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sm_ptr == MAP_FAILED) {
		printf("Erro ao mapear memoria compartilhada.\n");
		exit(1);
	}

	sm_ptr->i = 0;

	if(fork() == 0) {
		do {
			scanf("%d", &sm_ptr->n[sm_ptr->i]);
		} while(sm_ptr->n[sm_ptr->i] <= 0);
		
		while(sm_ptr->n[sm_ptr->i] != 1) {
			if(sm_ptr->n[sm_ptr->i] % 2 == 0) {
				sm_ptr->n[sm_ptr->i + 1] = sm_ptr->n[sm_ptr->i] / 2;
			}
			else {
				sm_ptr->n[sm_ptr->i + 1] = 3 * sm_ptr->n[sm_ptr->i] + 1;
			}
			sm_ptr->i++;
		}

		printf("Processo filho encerrado.");
	}
	else {
		wait();

		printf("\n\n");
		for(int i = 0; i < sm_ptr->i + 1; i++) {
			printf("%d\n", sm_ptr->n[i]);
		}

		if(shm_unlink("/collatz_region") == -1) {
			printf("Erro ao fechar memoria compartilhada.\n");
		}

		printf("Processo pai encerrado.");
	}

	return 0;
}