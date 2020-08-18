#include <stdio.h>

int main() {
	int n, fd;

	fd = fork();

	if(fd == 0) {
		do {
			scanf("%d", &n);
		} while(n <= 0);
		
		printf("\n%d\n", n);
		while(n != 1) {
			if(n % 2 == 0) {
				n = n / 2;
			}
			else {
				n = 3 * n + 1;
			}
			printf("%d\n", n);
		}

		printf("Processo filho encerrado.\n");
	}
	else {
		wait();

		printf("Processo pai encerrado.\n");
	}

	return 0;
}