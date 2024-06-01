/* We built this program as part of an advanced C programming course.
The program implements functions on Pointers and Dynamic memory allocation, Matrices, and Linked Lists.
Above each function there is an explanation.

Made By: Tomer Biton & Maor Shalit */

#define _CRT_SECURE_NO_WARNINGS

/* Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Constant definitions */

#define N 3
#define COLS 4
#define ROWS 4

/* Structs definitions */

typedef unsigned long long extraint;

typedef struct number
{
	extraint num;
	int sum;
}Number;

typedef struct triad
{
	int i, j, value;
}Triad;

typedef struct item
{
	Triad data;
	struct item* next;
}Item;

/* Functions declarations */

void Ex1();
void Ex2();
void Ex3();

Number* primeSums(extraint n1, extraint n2, int* p_size);
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols);
void createThreeList(int** A, int rows, int cols, Item** pL1, Item** pL2);

/* Declarations of auxiliary functions */

int isPrime(int num);
int digitSum(extraint num);

int neighborMax(int A[][COLS], int rows, int cols, int i, int j);
int** allocMatrix(int rows, int cols);

Triad createThree(int i, int j, int value);
void insert(Item** head, Triad element);

void printArray(Number* arr, int size);
void inputMatrix(int A[][COLS], int rows, int cols);
void printMatrix(int A[][COLS], int rows, int cols);
void printDynamicMatrix(int** A, int rows, int cols);
void freeMatrix(int** A, int rows);
void inputDynamicMatrix(int** A, int rows, int cols);
void printList(Item* lst);
void freeList(Item* lst);

/* Main function */

int main()
{
	int select = 0, i, all_Ex_in_loop = 0;
	printf("Run menu once or cyclically?\n(Once - enter 0, cyclically - enter other number) ");
	if (scanf("%d", &all_Ex_in_loop) == 1)
		do
		{
			for (i = 1; i <= N; i++)
				printf("Ex%d--->%d\n", i, i);
			printf("EXIT-->0\n");
			do {
				select = 0;
				printf("please select 0-%d : ", N);
				scanf("%d", &select);
			} while ((select < 0) || (select > N));
			switch (select)
			{
			case 1: Ex1(); break;
			case 2: Ex2(); break;
			case 3: Ex3(); break; 
			}
		} while (all_Ex_in_loop && select);
	return 0;
}

/* Void functions to activate the program */

void Ex1()
{
	Number* prime_arr = NULL;
	extraint n1, n2;
	int* p_size = NULL;

	printf("\nEnter 2 positive numbers called n1, n2.\nAttention! n2 must be greater than n1 (n2 > n1): ");
	scanf("%llu %llu", &n1, &n2);

	prime_arr = primeSums(n1, n2, &p_size);

	printArray(prime_arr, p_size);

	free(prime_arr);
}

void Ex2()
{
	int** B = NULL;
	int A[ROWS][COLS];

	printf("\nEnter the numbers of the matrix: \n");
	inputMatrix(A,ROWS,COLS);

	printf("\nOriginal matrix:\n\n");
	printMatrix(A,ROWS,COLS);
	printf("\n\n");

	B = matrixMaxNeighbor(A,ROWS,COLS);
	printf("Matrix with max neighbors: \n\n");
	printDynamicMatrix(B,ROWS,COLS);
	printf("\n\n");

	freeMatrix(B,ROWS);
}

void Ex3()
{
	Item* pL1 = NULL, * pL2 = NULL;
	int** A = NULL;

	A = allocMatrix(ROWS,COLS);
	inputDynamicMatrix(A,ROWS,COLS);
	printf("\n\nThe matrix is:\n\n");
	printDynamicMatrix(A,ROWS,COLS);
	createThreeList(A,ROWS,COLS,&pL1,&pL2);

	printf("\nNow printing Linked List 1 (pL1):\n\n");
	printList(pL1);

	printf("\nNow printing Linked List 2 (pL2):\n\n");
	printList(pL2);

	freeMatrix(A, ROWS);
	freeList(pL1);
	freeList(pL2);
}

/* Ex1 */
/* This function checks if the given number is a prime number. */
int isPrime(int num)
{
	int i;
	if (num == 2)
		return 1;
	if (num < 2 || num % 2 == 0)
		return 0;
	for (i = 3; i * i <= num; i += 2)
	{
		if (num % i == 0)
			return 0;
	}
	return 1;
}

/* This function calculates the digits sum of the given number. */
int digitSum(extraint num)
{
	int sum = 0;
	while (num != 0)
	{
		sum += num % 10;
		num /= 10;
	}
	return sum;
}

/* This function get 2 natural numbers and calculate all of
   the prime numbers in the range between them. It returns a pointer
   to an array with all of the numbers. */
Number* primeSums(extraint n1, extraint n2, int* p_size)
{
	extraint i;
	int size = 2, number_sum = 0;
	Number* prime_arr = NULL, * temp = NULL;
	*p_size = 0;

	prime_arr = (Number*)malloc(size * sizeof(Number));
	if (!prime_arr) return NULL;

	for (i = n1; i <= n2; i++)
	{
		number_sum = digitSum(i);
		if (isPrime(number_sum) == 1)
		{
			if ((*p_size) == size)
			{
				size *= 2;
				temp = (Number*)realloc(prime_arr, size * sizeof(Number));
				if (!temp) return NULL;
				prime_arr = temp;
			}
			prime_arr[(*p_size)].num = i;
			prime_arr[(*p_size)++].sum = number_sum;
		}
	}

	temp = (Number*)realloc(prime_arr, (*p_size) * sizeof(Number));
	if (!temp || *p_size == 0)
	{
		prime_arr = NULL;
		return prime_arr;
	}

	prime_arr = temp;
	return prime_arr;
}

/* Ex2 */
/* This function get the size of the new matrix and allocate dynamic memory for it.
It returns a pointer to the adress of the matrix. */
int** allocMatrix(int rows, int cols)
{
	int** matrixB = NULL, i;
	matrixB = (int**)calloc(rows, sizeof(int*));
	if (!matrixB) return NULL;

	for (i = 0; i < rows; i++)
	{
		matrixB[i] = (int*)calloc(cols, sizeof(int));
		if (!matrixB) return NULL;
	}
	return matrixB;
}

/* This function recieve 2D array, num of cols, num of rows, and index of a number in the matrix A.
It returns the maximum neighbor of the index. */
int neighborMax(int A[][COLS], int rows, int cols, int i, int j)
{
	int max = INT_MIN;
	if (i - 1 >= 0 && A[i - 1][j] > max)
		max = A[i-1][j];
	if (i + 1 < rows && A[i + 1][j] > max)
		max = A[i+1][j];
	if (j + 1 < cols && A[i][j+1] > max)
		max = A[i][j+1];
	if (j - 1 >= 0 && A[i][j-1] > max)
		max = A[i][j-1];
	return max;
}

/* This function create a new matrix 'B' and return a pointer to the adress of the new matrix. */
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols)
{
	int** B = NULL, i, j;
	B = allocMatrix(rows, cols);

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			B[i][j] = neighborMax(A, rows, cols, i, j);
		}
	}
	return B;
}

/* Ex3 */
/* This function get 3 parameters, insert them to the parameter 'element' and return the parameter. */
Triad createThree(int i, int j, int value)
{
	Triad element;
	element.i = i;
	element.j = j;
	element.value = value;
	return element;
}

/* This function insert a new element to the linked list. */
void insert(Item** head, Triad element)
{
	if (*head == NULL)
	{
		*head = (Item*)malloc(sizeof(Item));
		(*head)->data = element;
		(*head)->next = NULL;
	}
	else
	{
		Item* last = *head;
		Item* temp = (Item*)malloc(sizeof(Item));
		temp->data = element;
		temp->next = NULL;

		while (last->next)
		{
			last = last->next;
		}
		last->next = temp;
	}
}

/* This function recieve a matrix, and seperate it into two groups.
It return by reference 2 linked lists - 1 for each group.
Group 1 - The elements in the matrix are equal to the coordinate values.
Group 2 - The members of the matrix whose values constitutes an inovice series. */
void createThreeList(int** A, int rows, int cols, Item** pL1, Item** pL2)
{
	int i, j;
	Triad element;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i + j == A[i][j])
			{
				element = createThree(i, j, A[i][j]);
				insert(pL1, element);
			}
			if (i - j == j - A[i][j])
			{
				element = createThree(i, j, A[i][j]);
				insert(pL2, element);
			}
		}
	}
}

/* Auxiliary functions */

void printArray(Number* arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("\nThe number is: %llu\n", arr[i].num);
		printf("The sum is: %d\n", arr[i].sum);
	}
}

void inputMatrix(int A[][COLS], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("Enter number: ");
			scanf("%d", &A[i][j]);
		}
	}
}

void printMatrix(int A[][COLS], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}

void printDynamicMatrix(int** A, int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
}

void freeMatrix(int** A, int rows)
{
	int i;
	for (i = 0; i < rows; i++)
		free(A[i]);
	free(A);
}

void inputDynamicMatrix(int** matrix, int rows, int cols)
{
	printf("\nEnter the elements of the matrix:\n");
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			printf("Element [%d][%d]: ", i, j);
			scanf("%d", &matrix[i][j]);
		}
	}
}

void printList(Item* lst)
{
	Item* temp = lst;

	while (temp)
	{
		printf("-> i = %d j = %d value = %d \n", temp->data.i, temp->data.j, temp->data.value);
		temp = temp->next;
	}
	printf("-> NULL\n\nEnd of list\n");
}

void freeList(Item* lst)
{
	Item* temp = lst;
	
	while (temp)
	{
		temp = lst->next;
		free(lst);
		lst = temp;
	}
}