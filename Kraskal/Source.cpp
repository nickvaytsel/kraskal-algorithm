#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

FILE* input;
struct graph {
	int firstPoint;
	int secondPoint;
	int weight;
	struct graph* next;
};

//Граф вводится и обрабатывается с помощью списка рёбер 

void init(struct graph **, int, int, int);      //Создание списка
void insert(struct graph *, int, int, int);     //Добавление элемента в список
void inputGraph(struct graph **);               //Ввод графа
void sortEdges(struct graph *);                 //Сортировка рёбер по весу
void buildMinOstTree(struct graph *);           //Алгоритм Краскала
void printGraph(int **);                        //Печать графа
void rewriteConnections(int fP, int sP, int n); //"Совмещение" компонент связности
int* compConnections;                           //Массив определяющий компоненты связности
int Arcs, Points;                               //Переменные, где хранится кол-во вершин и рёбер

int main() {
	struct graph * Graph;
	inputGraph(&Graph);
	sortEdges(Graph);
	buildMinOstTree(Graph);
	_getch();
}

void init(struct graph ** Graph, int firstPoint, int secondPoint, int weight){
	if (Graph == NULL) return;
	*Graph = (struct graph*)malloc(sizeof(struct graph));
	(*Graph)->next = NULL;
	(*Graph)->firstPoint = firstPoint;
	(*Graph)->secondPoint = secondPoint;
	(*Graph)->weight = weight;
}

void insert(struct graph* Graph, int firstPoint, int secondPoint, int weight){
	struct graph* elem = NULL;
	struct graph* q = Graph;
	if (Graph == NULL) return;
	elem = (struct graph*)malloc(sizeof(struct graph));
	elem->next = NULL;
	elem->firstPoint = firstPoint;
	elem->secondPoint = secondPoint;
	elem->weight = weight;
	while (q->next != NULL) {
		q = q->next;
	}
	q->next = elem;
	return;
}

void inputGraph(struct graph** Graph) {
	if ((input = fopen("input.txt", "r")) == NULL) {
		printf("file error");
		return;
	}
	int data[3];
	fscanf(input, "%i", &Arcs);
	fscanf(input, "%i", &Points);
	for (int i = 0; i < 3; i++) {
		fscanf(input, "%i", &data[i]);
	}
	init(Graph, data[0], data[1], data[2]);
	int l = 1;
	while (l < Arcs) {
		for (int i = 0; i < 3; i++) {
			fscanf(input, "%i", &data[i]);
		}
		insert(*Graph,data[0], data[1], data[2]);
		l++;
	}
	fclose(input);
 }

void sortEdges(struct graph* Graph) {
	struct graph* i = Graph;
	struct graph* j = Graph;

	while (i != NULL) {
		j = i;
		while (j != NULL) {
			if (i->weight > j->weight) {
				int temp[3];
				temp[0] = j->firstPoint;
				temp[1] = j->secondPoint;
				temp[2] = j->weight;
				j->firstPoint = i->firstPoint;
				i->firstPoint = temp[0];
				j->secondPoint = i->secondPoint;
				i->secondPoint = temp[1];
				j->weight = i->weight;
				i->weight = temp[2];
			}
			j = j->next;
		}
		i = i->next;
	}
}

void printGraph(int ** graph) {
	int sumWeight = 0;
	for (int i = 0; i < Arcs; i++) {
		sumWeight += graph[2][i];
		for (int j = 0; j < 3; j++) {
			if (graph[j][i] != -1) {
				printf("%i ", graph[j][i]);
				if(j==2){
					printf("\n");
				}
			}
			else {
				break;
			}
		}
	}
	printf("%i", sumWeight);
}

void rewriteConnections(int fP, int sP) {
	int max;
	if (compConnections[fP] < compConnections[sP]) {
		max = compConnections[sP];
		for (int i = 0; i < Points; i++) {
			if (compConnections[i] == max) {
				compConnections[i] = compConnections[fP];
			}
		}
	}
	else {
		for (int i = 0; i < Points; i++) {
			max = compConnections[fP];
			if (compConnections[i] == compConnections[fP]) {
				compConnections[i] = compConnections[sP];
			}
		}
	}
}

void buildMinOstTree(struct graph* Graph) {
	int* ostTree[3];
	compConnections = (int*)malloc(Points * sizeof(int));
	for (int i = 0; i < 3; i++) {
		ostTree[i] = (int*)malloc(Arcs*sizeof(int));
	}
	for (int i = 0; i < Points; i++) {
		compConnections[i] = i;
		for (int j = 0; j < 3; j++) {
			if (i < Arcs) {
				ostTree[j][i] = -1;
			}
		}
 	}
	struct graph* p = Graph;
	int l = 0;
	int k = 0;
	while (l < Arcs-k && p!=NULL) {
		if (compConnections[p->firstPoint]==compConnections[p->secondPoint]){
			p = p->next;
			k++;
		}
		else {
			ostTree[0][l] = p->firstPoint;
			ostTree[1][l] = p->secondPoint;
			ostTree[2][l] = p->weight;
			p = p->next;
			rewriteConnections(ostTree[0][l], ostTree[1][l]);
			l++;
		}
	}
	printGraph(ostTree);
}