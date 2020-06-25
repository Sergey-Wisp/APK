#include <time.h>

#define BAUD 12
#define PACKET_END 0
#define MAX_BUF_SIZ 512
#define TIMEOUT 0.020

char com_receive(int base) {
	return inp(base);
}


void com_initializer(int base) {
	unsigned int lcr;

	lcr = inp(base + 0x03); 
	outp(base + 0x03, lcr | 0x80); //��������� ���� dlab � 1 � �������� lcr ����� ������� � ����� ��������� ������� (1000000)

	outp(base, 0x0C); //��������� �������� �������
	outp(base + 0x01, 0x00);

	outp(base + 0x03, lcr & 0x7f); //��������� ���� dlab � ���� (0111111)

	outp(base + 0x01, 0x00); // ��������� ����������

	outp(base + 0x03, 0x1b); // 0x1b = 0 | 0 (���� ��������� �������� ��������) | 011 (������) | 0 (1 �������� ���) | 11
	outp(base + 0x04, 0x00); // DTR - 0, RTS - 0 
}

int com_check_rhr(int base) {
	unsigned char lsr = inp(base + 0x05);
	return (lsr & 0x01) == 0x01; // �������� ���������� �� ����������
}

int com_check_tbe(int base) {
	unsigned char lsr = inp(base + 0x05);
	return ((lsr & 0x20) >> 5) == 0x01; //�������� ����������� �� ����������
}

void com_send(char symbol, int base) {
	unsigned char lsr;
	unsigned char mcr = inp(base + 0x04); /* modem control register */
	outp(base + 0x04, mcr | 0x02); // ��������� ���� rts

	// �������� ��������� ����� ����� ������� ������ � ����
	do lsr = inp(base + 0x05) & 0x40; // ������ �����������
	while (lsr != 0x40); // ���� ���������� ����

	outp(base, symbol); //��������� ������

	while (com_check_tbe(base)); // �������� ���������� �����������
	outp(base + 0x04, mcr | 0xfd); // ����� ���� rts
}

void com_send_string(char* string, int base) {
	do {
		com_initializer(base);
		com_send(*string, base);
	} while (*(string++));
}


char* com_receive_string(int base) {
	int i = 0;
	char buffer[MAX_BUF_SIZ];

	time_t control = 0, checker = 0;

	do {
		while (!com_check_rhr(base)) {
			checker = time(0);
			if (control) {
				if (difftime(checker, control) > TIMEOUT) {
					buffer[i] = '\0';
					return buffer;
				}
			}
		};
		buffer[i] = com_receive(base);
		control = time(0);
	} while (buffer[i++] != PACKET_END);

	return buffer;
}