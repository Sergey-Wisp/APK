#include <time.h>

#include <iostream>

using namespace std;



#define SIZE 4

#define COUNTER 1000000



void printMenu();

void printArray(int array[SIZE][SIZE]);



int main()

{

	printMenu();

	time_t random;

	srand((unsigned)time(&random));



	int intSize = sizeof(int);

	int doubleIntSize = sizeof(int) * 2;

	int MMXCounter = SIZE * SIZE * intSize * doubleIntSize / 64;

	int array1[SIZE][SIZE], array2[SIZE][SIZE], array3[SIZE][SIZE];



	for (int i = 0; i < SIZE; i++)

	{

		for (int j = 0; j < SIZE; j++)

		{

			array1[i][j] = rand() % 100;

			array2[i][j] = rand() % 100;

		}

	}



	printf("Array 1:\n");

	printArray(array1);

	printf("Array 2:\n");

	printArray(array2);



	int counter = COUNTER;



	// Computing using C

	clock_t begin_c = clock();

	while (--counter)

	{

		for (int i = 0; i < SIZE; i++)

		{

			for (int j = 0; j < SIZE; j++)

			{

				array3[i][j] = array1[i][j] & array2[i][j];

			}

		}

	}

	clock_t end_c = clock();



	printf("Result using C:\n");

	printArray(array3);

	printf("\n");



	counter = COUNTER;

	int asmCounter = SIZE * SIZE;



	// Computing using ASM

	clock_t begin_asm = clock();

	while (--counter)

	{

		__asm

		{
			xor esi, esi
			m_loop :
			cmp esi, SIZE * 4
				je loop_end
				mov eax, array1[esi]; 
				mov ebx, array2[esi]; 
				cmp eax, ebx
				jle continue
				mov eax, ebx
				continue:
			mov array3[esi], eax; 
				add esi, 4
				jmp m_loop
				loop_end :

		}

	}

	clock_t end_asm = clock();



	printf("Result using ASM:\n");

	printArray(array3);

	printf("\n");



	counter = COUNTER;



	// Computing using MMX

	clock_t begin_mmx = clock();

	while (--counter)

	{

		_asm

		{

			MOV ECX, MMXCounter				// ECX = MMXCounter - �������

			XOR ESI, ESI					// ����� ������, ������ ���������

			STARTM :

			MOVQ MM0, array1[ESI]	    // MM0 = array1[ESI]

				MOVQ MM1, array2[ESI]		// MM1 = array2[ESI]



				PAND MM0, MM1				// �������� ����������� �

				MOVQ array3[ESI], MM0		// array3[ESI] = MM0

				LOOP STARTM

				EMMS							// ������� ��������

		}

	}

	clock_t end_mmx = clock();



	printf("Result using MMX:\n");

	printArray(array3);

	printf("\n");



	printf("Computing using C\n");

	printf("time: %.6lf\n\n", (float)(end_c - begin_c) / CLOCKS_PER_SEC);



	printf("Computing using ASM\n");

	printf("time: %.6lf\n\n", (float)(end_asm - begin_asm) / CLOCKS_PER_SEC);



	printf("Computing using MMX\n");

	printf("time: %.6lf\n\n", (float)(end_mmx - begin_mmx) / CLOCKS_PER_SEC);



	system("pause");

	return 0;

}



void printMenu()

{

	printf("Logical operation AND\n");

	printf("Array size: %d x %d\n", SIZE, SIZE);

	printf("Counter value: %ld\n\n", COUNTER);

}



void printArray(int array[SIZE][SIZE])

{

	for (int i = 0; i < SIZE; i++)

	{

		printf("\t");

		for (int j = 0; j < SIZE; j++)

		{

			printf("\t %d ", array[i][j]);

		}

		printf("\n");

	}

	printf("\n");
}