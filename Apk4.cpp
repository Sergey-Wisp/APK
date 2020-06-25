#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define COUNT 39
#define DELAY 10
#define TIME_RUN 65536

void SoundGeneration1();
void SoundGeneration2();
void DivisionCoefficientComputation();
void StatusWord();
void RandomNumber();

void main()
{
	system("CLS");
	char choice;
	do
	{
		printf("\n\n1. Generate sound (usual).\n2. Generate sound (unusual).\n3. Division coefficient.\n4. Status word.\n5. Random number.\n0. Exit.\n");
		rewind(stdin);
		printf("\nYour choise: ");
		scanf("%c", &choice);

		switch (choice)
		{
		case '1': SoundGeneration1(); break;
		case '2': SoundGeneration2(); break;
		case '3': DivisionCoefficientComputation(); break;
		case '4': StatusWord(); break;
		case '5': RandomNumber(); break;
		}
	} while (choice != '0');
}

void SoundGeneration1()
{

	int frequency[8] = { 329, 246, 246, 261, 293, 329, 392, 349 };
	int durability[8] = { 400, 400, 400, 400, 400, 400, 400, 400 };

	long unsigned base = 1193180;
	int frequencyCounter;
	int divisionCoefficient;

	for (frequencyCounter = 0; frequencyCounter < 8; frequencyCounter++)
	{
		outp(0x43, 0xB6);
		divisionCoefficient = base / frequency[frequencyCounter];
		outp(0x42, divisionCoefficient % 256);
		divisionCoefficient /= 256;
		outp(0x42, divisionCoefficient);

		outp(0x61, inp(0x61) | 3);
		delay(durability[frequencyCounter]);
		outp(0x61, inp(0x61) & 0xFC);
		delay(100);
	}
}

void SoundGeneration2()
{

	int frequency[COUNT] = { 392, 392, 392, 311, 466, 392, 311, 466, 392,
							 587, 587, 587, 622, 466, 369, 311, 466, 392,
							 784, 392, 392, 784, 739, 698, 659, 622, 659,
							 415, 554, 523, 493, 466, 440, 466,
							 311, 369, 311, 466, 392 };
	int durability[COUNT] = { 350, 350, 350, 250, 100, 350, 250, 100, 700,
							  350, 350, 350, 250, 100, 350, 250, 100, 700,
							  350, 250, 100, 350, 250, 100, 100, 100, 200,
							  100, 350, 250, 100, 100, 100, 200,
							  100, 350, 250, 100, 750 };

	long unsigned base = 1193180;
	int frequencyCounter;
	int divisionCoefficient;

	for (frequencyCounter = 0; frequencyCounter < COUNT; frequencyCounter++)
	{
		outp(0x43, 0xB6);
		divisionCoefficient = base / frequency[frequencyCounter];
		outp(0x42, divisionCoefficient % 256);
		divisionCoefficient /= 256;
		outp(0x42, divisionCoefficient);

		outp(0x61, inp(0x61) | 3);
		delay(durability[frequencyCounter]);
		outp(0x61, inp(0x61) & 0xFC);
		delay(durability[frequencyCounter]);
	}
}

void DivisionCoefficientComputation()
{
	unsigned long j;
	int Channel;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 0, 64, 128 };
	unsigned divisionCoefficientLow, divisionCoefficientHigh, divisionCoefficient, max;

	printf("\n\nCoefficient of division: \n");

	for (Channel = 0; Channel < 3; Channel++)
	{
		divisionCoefficient = 0;
		max = 0;

		if (Channel == 2)
		{
			outp(0x61, inp(0x61) | 3);
		}

		for (j = 0; j < TIME_RUN; j++)
		{
			outp(0x43, controlWords[Channel]);
			divisionCoefficientLow = inp(ports[Channel]);
			divisionCoefficientHigh = inp(ports[Channel]);
			divisionCoefficient = divisionCoefficientHigh * 256 + divisionCoefficientLow;

			if (divisionCoefficient > max)
			{
				max = divisionCoefficient;
			}
		}
		if (Channel == 2)
		{
			outp(0x61, inp(0x61) & 0xFC);
		}
		printf("\nChannel %d: %4X\n", Channel, max);
	}
}

void StatusWord()
{
	printf("\n\nStatus word: \n");
	char stateWord[8];
	int Channel, state;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 226, 228, 232 };		//command words,
												//acording to the rules

	for (Channel = 0; Channel < 3; Channel++)
	{
		outp(0x43, controlWords[Channel]);
		state = inp(ports[Channel]);

		for (int i = 7; i >= 0; i--)
		{
			stateWord[i] = state % 2 + '0';
			state /= 2;

		}
		printf("\nChannel %d: ", Channel);
		for (i = 0; i < 8; i++)
		{
			printf("%c", stateWord[i]);
		}
		printf("\n");
	}
}

void RandomNumber()
{
	char choice;
	unsigned int limit = TIME_RUN - 1, numberLow, numberHigh, number;

	do
	{
		printf("\n\n1. Set a limit.\n2. Get a number.\n0. Exit\n");
		fflush(stdin);
		printf("\nYour choise: ");
		scanf("%s", &choice);

		switch (choice)
		{
		case '1':
		{
			do
			{
				printf("\nEnter a limit in range [1...65635].\n");
				fflush(stdin);
			} while (!scanf("%d", &limit) || limit < 1);

			outp(0x43, 0xB4);
			outp(0x42, limit % 256);
			limit /= 256;
			outp(0x42, limit);
			outp(0x61, inp(0x61) | 1);
			break;
		}
		case '2':
		{
			outp(0x43, 128);
			numberLow = inp(0x42);
			numberHigh = inp(0x42);
			number = numberHigh * 256 + numberLow;

			printf("\nRandom number: %u\n", number);

			break;
		}
		}
	} while (choice != '0');
	outp(0x61, inp(0x61) & 0xFC);
}