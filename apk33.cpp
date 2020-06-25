#include <dos.h> 

#define BUFF_WIDTH 80
#define CENTER_OFFSET 12
#define LEFT_OFFSET 20
#define REG_SCREEN_SIZE 9

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

int color = 0x5e;	

void print(int offset, int value);
void getRegisterValue();

void interrupt(*oldHandle08) (...);
void interrupt(*oldHandle09) (...);
void interrupt(*oldHandle0A) (...);
void interrupt(*oldHandle0B) (...);
void interrupt(*oldHandle0C) (...);
void interrupt(*oldHandle0D) (...);
void interrupt(*oldHandle0E) (...);
void interrupt(*oldHandle0F) (...);

void interrupt(*oldHandle70) (...);
void interrupt(*oldHandle71) (...);
void interrupt(*oldHandle72) (...);
void interrupt(*oldHandle73) (...);
void interrupt(*oldHandle74) (...);
void interrupt(*oldHandle75) (...);
void interrupt(*oldHandle76) (...);
void interrupt(*oldHandle77) (...);

void interrupt newHandle98(...) { getRegisterValue(); oldHandle08(); }
void interrupt newHandle99(...) { color++;  getRegisterValue(); oldHandle09(); }
void interrupt newHandle9A(...) { getRegisterValue(); oldHandle0A(); }
void interrupt newHandle9B(...) { getRegisterValue(); oldHandle0B(); }
void interrupt newHandle9C(...) { getRegisterValue(); oldHandle0C(); }
void interrupt newHandle9D(...) { getRegisterValue(); oldHandle0D(); }
void interrupt newHandle9E(...) { getRegisterValue(); oldHandle0E(); }
void interrupt newHandle9F(...) { getRegisterValue(); oldHandle0F(); }

void interrupt newHandle08(...) { getRegisterValue(); oldHandle70(); }
void interrupt newHandle09(...) { getRegisterValue(); oldHandle71(); }
void interrupt newHandle0A(...) { getRegisterValue(); oldHandle72(); }
void interrupt newHandle0B(...) { getRegisterValue(); oldHandle73(); }
void interrupt newHandle0C(...) { color++; getRegisterValue(); oldHandle74(); }
void interrupt newHandle0D(...) { getRegisterValue(); oldHandle75(); }
void interrupt newHandle0E(...) { getRegisterValue(); oldHandle76(); }
void interrupt newHandle0F(...) { getRegisterValue(); oldHandle77(); }

void print(int offset, int value)
{
	char temp;

	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);
	screen += CENTER_OFFSET * BUFF_WIDTH + offset;

	for (int i = 7; i >= 0; i--)
	{
		temp = value % 2;
		value /= 2;
		screen->symb = temp + '0';
		screen->attr = color;
		screen++;
	}
}

void getRegisterValue()
{
	print(0 + LEFT_OFFSET, inp(0x21));

	outp(0x20, 0x0B);
	print(REG_SCREEN_SIZE + LEFT_OFFSET, inp(0x20));

	outp(0x20, 0x0A);
	print(REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0x20));

	print(BUFF_WIDTH + LEFT_OFFSET, inp(0xA1));

	outp(0xA0, 0x0B);
	print(BUFF_WIDTH + REG_SCREEN_SIZE + LEFT_OFFSET, inp(0xA0));

	outp(0xA0, 0x0A);
	print(BUFF_WIDTH + REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0xA0));
}

void init()
{
	oldHandle08 = getvect(0x08);        
	oldHandle09 = getvect(0x09);        
	oldHandle0A = getvect(0x0A);		
	oldHandle0B = getvect(0x0B);	    
	oldHandle0C = getvect(0x0C);	    
	oldHandle0D = getvect(0x0D); 	   
	oldHandle0E = getvect(0x0E); 	   
	oldHandle0F = getvect(0x0F);	    

										
	oldHandle70 = getvect(0x70);		
	oldHandle71 = getvect(0x71);	    		
	oldHandle72 = getvect(0x72);	    
	oldHandle73 = getvect(0x73);		
	oldHandle74 = getvect(0x74);		
	oldHandle75 = getvect(0x75);		
	oldHandle76 = getvect(0x76);		
	oldHandle77 = getvect(0x77);		

	setvect(0x98, newHandle98);
	setvect(0x99, newHandle99);
	setvect(0x9A, newHandle9A);
	setvect(0x9B, newHandle9B);
	setvect(0x9C, newHandle9C);
	setvect(0x9D, newHandle9D);
	setvect(0x9E, newHandle9E);
	setvect(0x9F, newHandle9F);

	setvect(0x08, newHandle08);
	setvect(0x09, newHandle09);
	setvect(0x0A, newHandle0A);
	setvect(0x0B, newHandle0B);
	setvect(0x0C, newHandle0C);
	setvect(0x0D, newHandle0D);
	setvect(0x0E, newHandle0E);
	setvect(0x0F, newHandle0F);

	_disable(); 


	outp(0x20, 0x11); //icw1 инит текущ контроллера
	outp(0x21, 0x98); //icw2 базовый вектор
	outp(0x21, 0x04); //icw3 номер порта у слейва
	outp(0x21, 0x01); //icw4 'дефолт' - ввод символа

					
	outp(0xA0, 0x11); 
	outp(0xA1, 0x08); 
	outp(0xA1, 0x02); //номер входного порта мастера
	outp(0xA1, 0x01);

	_enable();
}

int main()
{
	unsigned far *fp;

	init();

	FP_SEG(fp) = _psp; // получение сегмента памяти
	FP_OFF(fp) = 0x2c; // смещение указателя
	_dos_freemem(*fp); // освобождение памяти

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); //сохранение резидентности программы
	return 0;
}