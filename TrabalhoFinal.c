#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define TASK_SIZE 10

pthread_mutex_t mutex;	// Declaração do mutex

int tasks[TASK_SIZE];	//Declaração da lista de tarefas
int taskSize = TASK_SIZE;	//Declarando e igualando a variável do tamanho da lista ao valor total de tarefas a realizar, esta variável serve para decrementar pois não é possivel decrementar constantes

void listprint(int list []){	//Função imprimir as tarefas criadas
	printf("\nLista com %d Tarefas:\n", taskSize);
    for(int i = 0; i < taskSize; i++){	//ciclo para percorrer a lista e imprimir os seus elementos por ordem
		printf("\n-> Elemento %d : %d \n", i + 1, list[i]);
    }
}

void taskCreator(){	//Função para criação das tarefas
    for(int j = 0; j < TASK_SIZE; j++){
        tasks[j] = rand() % 1000;	 //Coloca a tarefa criada no array
    }
}

void *executeTask(long workerID){	//Função para cada thread executar paralelamente
	pthread_mutex_lock(&mutex);		//Bloqueio para impedir que duas threads entrem ao mesmo tempo
	printf("######Trabalhador #%ld está a executar a tarefa %d\n", workerID + 1, tasks[0]);		//Realiza a tarefa
	for(int z = 0;z < taskSize; z++){	//Reordena os elementos da array e elimina a tarefa que foi realizada
		tasks[z] = tasks[z+1];
	}
	taskSize--;		//Diminui o tamanho da array
	pthread_mutex_unlock(&mutex);
	return NULL;
}


void *threadFuncion(void *arg) {
    long threadID = (long)arg;
	executeTask(threadID);
    return NULL;
}

int main() {
    srand(time(NULL)); //Função que cria números aleatórios em função do tempo
    taskCreator();		//Função para criar as tarefas
	listprint(tasks);	//Imprime a lista das tarefas

    pthread_t threads[TASK_SIZE];	//Declara lista de armazenamento das threads
    pthread_mutex_init(&mutex, NULL); //Inicialização do mutex

    for(int t = 0; t < TASK_SIZE; t++) {	 // Criação e execução das threads
        pthread_create(&threads[t], NULL, threadFuncion, (void *)(intptr_t)t);
    }

    for(int s = 0; s < TASK_SIZE; s++) {	//Aguarda o termino das threads
        pthread_join(threads[s], NULL);
    }
	if(taskSize == 0){	//Vaida se todas as tarefas foram realizadas com sucesso caso contrário deixa uma menssagem de erro
		printf("As tarefas foram todas executadas com sucesso\n");
	} else{
		printf("Algo correu mal!! As tarefas não foram todas executadas");
	}
    pthread_mutex_destroy(&mutex); // Destruição do mutex
    return 0;
}