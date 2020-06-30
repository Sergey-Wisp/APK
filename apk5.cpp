#include <dos.h>

#include <stdio.h>

#include <stdlib.h>

#include <conio.h>

#include <io.h>



/* ����� �������� */

int delayValue;



void getTime();

void setTime();

void setDelay();

void setAlarm();

void resetAlarm();

void inputTime();



int main()

{

		while (1) {

				printf("\n1.Show time\n");

				printf("2.Set time\n");

				printf("3.Enable alarm\n");

				printf("4.Delay time\n");

				printf("5.Exit\n");



				switch (getch()) {

					case '1':

							system("cls");

							getTime();

							break;

					case '2':

							setTime();

							break;

					case '3':

							setAlarm();

							break;

					case '4':

							setDelay();

							break;

					case '5':

						 system("cls");

						 printf("Exiting...");

						 delay(1200);

						 return 0;

					default:

							printf("Wrong value!\nTry again: ");

							system("cls");

							break;

					}

		}

}

// �������������� ����� �� ���������� ������� � BCD ������.

unsigned char toBcd(int val)

{

	return (unsigned char)((val / 10 * 16) + (val % 10));

}

// �������������� ����� �� ������� BCD � ���������� ������.

unsigned char fromBcd(int val)

{

	return (unsigned char)((val / 16 * 10) + (val % 16));

}



/* ��������� �������� ������� */

void getTime()

{

	char data[6];

	/* �������, ������, ���, ����, �����, ��� */

	unsigned int reg[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09 };

	unsigned char time[6];



	for (int i = 0; i < 6; i++)

	{

		/* �������� ������ ������� */

		outp(0x70, reg[i]);

		/* ��������� �������� �� ������� �������� � ������ */

		data[i] = inp(0x71);

	}



	/* ��������� ��������� �������� � ���������� ����� */

	for (i = 0; i < 6; i++)

	{

		time[i] = fromBcd(data[i]);

	}



	printf("Time: %2d:%2d:%2d\nDate: %2d.%2d.20%2d\n", time[2], time[1], time[0], time[3], time[4], time[5]);

}





void setTime(){



	char data[6];

	/* �������, ������, ���, ����, �����, ��� */

	unsigned int reg[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09 };

	int temp;

	//���� �����, ����� � ������

	do {

		fflush(stdin);

		printf("Enter hour: ");

		scanf("%i", &temp);

	} while ((temp > 23 || temp < 0));

	data[2] = toBcd(temp);



	do {

		fflush(stdin);

		printf("Enter minute: ");

		scanf("%i", &temp);

	} while (temp > 59 || temp < 0);

	data[1] = toBcd(temp);



	do {

		fflush(stdin);

		printf("Enter second: ");

		scanf("%i", &temp);

	} while (temp > 59 || temp < 0);

	data[0] = toBcd(temp);



	int res;

	do {

		outp(0x70, 0x0A);

		res=inp(0x71) & 0x80;

	} while (res);



  /* ��������� ���������� ����� ��������� ������� */

	outp(0x70, 0xB);

	outp(0x71, inp(0x71) | 0x80);



	for (int i = 0; i < 3; i++)

	{

		/* �������� ������ ������� � �������� registers[i]*/

		outp(0x70, reg[i]);

		/* ������ � ���� ������ �������� */

		outp(0x71, data[i]);

	}



	/* �������� ���������� ����� ������� ������� */

	outp(0x70, 0xB);

	outp(0x71, inp(0x71) & 0x7F);



}

/* ������ ���������� ���������� */

void interrupt(*oldHandler)(...);

/* ����� ����������� ���������� */

void interrupt newHandler(...)

{

	delayValue--;

	oldHandler();

}

void setDelay(){



	fflush(stdin);

	printf("Enter delay time: ");

	scanf("%d", &delayValue);

	_disable();

	/* ������������� ����� ����������� ���������� */

	oldHandler = getvect(0x70);

	setvect(0x70, newHandler);

	_enable();



	printf("This is your delay!");

	outp(0xA1, inp(0xA1) & 0xFE); 		// �������������� ���������� �� RTC 11111110



	  /* �������� ������� B */

	outp(0x70, 0x0B);

	outp(0x71, inp(0x71) | 0x40);		// ���������� ������������� ���������� 01000000



	while (delayValue > 0);



	outp(0x70, 0x0B);

	outp(0x71, inp(0x71) & 0xBF);	// ���������� ������������� ���������� 10111111



	outp(0xA1, inp(0xA1) | 0x01);//



	_disable();

	setvect(0x70, oldHandler);

	_enable();



	printf("\nOkey.You can go on!");



	return;

}



/* ������ ���������� ���������� */

void interrupt(*oldAlarmHandler) (...);

/* ����� ����������� ���������� */

void interrupt newAlarmHandler(...) {

	puts("\nAlarm! Alarm!\n");



	outp(0x70, 0xB);//��������� ���������� �� ����������

	outp(0x71, inp(0x71) & 0xDF);// (11011111)



	//oldAlarmHandler();



	resetAlarm();

}



void setAlarm(){



	char data[6];

	int temp;

	do {

		fflush(stdin);

		printf("Hours: ");

		scanf("%i", &temp);

	} while ((temp > 23 || temp < 0));

	data[2] = toBcd(temp);



	do {

		fflush(stdin);

		printf("Minutes: ");

		scanf("%i", &temp);

	} while (temp > 59 || temp < 0);

	data[1] = toBcd(temp);



	do {

		fflush(stdin);

		printf("Seconds: ");

		scanf("%i", &temp);

	} while (temp > 59 || temp < 0);



	data[0] = toBcd(temp);

	/* ������������� � �������� ���������� ������ ����� */

	outp(0x70, 0x05);

	outp(0x71, data[2]);

	outp(0x70, 0x03);

	outp(0x71, data[1]);

	outp(0x70, 0x01);

	outp(0x71, data[0]);



	_disable();

	oldAlarmHandler = getvect(0x4A);

	setvect(0x4A, newAlarmHandler);

	_enable();



	/* �������� ������� B */

	outp(0x70, 0x0B);

	outp(0x71, (inp(0x71) | 0x20)); // ��������� ���������� ��� ����������(00100000)



	outp(0xA1, (inp(0xA0) & 0xFE));//�������������� ���������� �� RTC(11111110)



	printf("%2d:%2d:%2d\n", fromBcd(data[2]), fromBcd(data[1]), fromBcd(data[0]));

}



void resetAlarm() {

	if (oldAlarmHandler == NULL)//�� ����������

	{

		return;

	}



	_disable();

	  /* ���������� ������ ���������� */

	setvect(0x4A, oldAlarmHandler);

	_enable();



	/* ���������� ������� ��������*/

	outp(0x70, 0x05);

	outp(0x71, 0);

	outp(0x70, 0x03);

	outp(0x71, 0);

	outp(0x70, 0x01);

	outp(0x71, 0);

}