#define VIDEO_SEGMENT 0xB000
#define LINE3_OFFSET 0x8000
#define BLACK 0x00

void readString(char* buffer);
void printString(char* chars);
void clearScreen();
char in;
char* buffer[80];
char* buffer2[512];

int main() {
		
	/*printString("Ingrese Texto: ");
	readString(buffer);
	printString(buffer);

	readSector(buffer2, 30);
	printString(buffer2);*/

	makeInterrupt21();
	loadProgram();
		
	while(1);
	return 0;
}

void readString(char* buffer)
{
	int index=0;
	char c=0x00;

	while (c!=0xd) /*Enter*/
	{
		c=readChar();
		if (c!=8) /*No es backspace*/
		{
			printChar(c);
			buffer[index]=c;
			index++;
		}
		else if (index>0)
		{
			printChar(c);
			printChar(' ');
			printChar(c);
			index--;
		}
	}

	buffer[index]=0xa;
	printChar(buffer[index]);
	buffer[index+1]=0;
}

void printString(char* chars){
	int i = 0;
	while(chars[i] != '\0'){
		printChar(chars[i]);
		i++;
	}
}

void clearScreen()
{
	int i=0;
	while(i < (80*25*2))
	{
		putInMemory(VIDEO_SEGMENT, LINE3_OFFSET + i, ' ');
		i++;
		putInMemory(VIDEO_SEGMENT, LINE3_OFFSET + i, BLACK);
		i++;
	};
};
