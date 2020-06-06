//Сформировать из двух массивов& quot; минимальный& quot; (результат[i] = min(массив1[i],
//	массив2[i])).* pminub, pminsb, pminuw, pminsw
#include <iostream>
#include <ctime>

#define SIZE 20
#define COUNTER 1000000

void generate(int* array);
void printArray(int* array);

int main()
{
	srand(time(NULL));
	int a1[SIZE], a2[SIZE], result[SIZE];
	time_t start, end;

	generate(a1);
	generate(a2);

	std::cout << "Array 1: "; printArray(a1);
	std::cout << "Array 2: "; printArray(a2);

	//ASM
	int sizex = SIZE;
	int counter = 1000000;
	

	start = clock();

	while (counter--)
	{
		_asm {
			mov ecx, sizex;
			xor esi, esi;

		begin:
			mov eax, a1[esi];
			mov ebx, a2[esi];

			cmp ebx, eax;
			jg isBiger;

			mov result[esi], ebx;
			jmp begin;

		isBiger:
			mov result[esi], eax;
			jmp begin;

		}
	}
	end = clock();

	std::cout << "\nResult ASM (" << (end - start) << " ms):	";
	for (size_t i = 0; i < SIZE; i++)
		std::cout << result[i] << " ";
