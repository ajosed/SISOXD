#define VIDEO_SEGMENT 0xB000
#define LINE3_OFFSET 0x8000
#define BLACK 0x00

void executeProgram(char* name, int segment);
void readFile(char* fileName,char* buffer);
int strComp(char* buffer, char* fileName);
void readString(char* buffer);
void printString(char* chars);
char in;
char* buffer[80];
char* buffer2[512];

int main() {
		
	/*printString("Ingrese Texto: ");
	readString(buffer);
	printString(buffer);

	readSector(buffer2, 30);
	printString(buffer2);*/

	/*makeInterrupt21();
	loadProgram();*/

	/*char buffer [13312];

	clearScreen();
	readFile ("c.txt",buffer) ;
	printString (buffer) ;*/
	
	executeProgram("kn",1);
	executeProgram("kn",2);
		
	while(1);
	return 0;
}

void executeProgram(char* name, int segment){

	int i;
	int address;
	char readingBuffer[13312];
	readFile(name, readingBuffer);

	for (i=0; i<13312;i++){
		putInMemory(segment,i,readingBuffer[i]);
	}

	launchProgram(segment);
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


void readFile(char* fileName,char* buffer){
	
	char* prueba='H';
	int fileFound;
	int nameCt = 0;
	int index, k,h;
	int sectors[27];
	int j = 0;
	int i;
	int buffAddress = 0;

	printChar (prueba);
	readSector(buffer, 2);
	fileFound = strComp(buffer,fileName);

	if (fileFound!=0){

		index = fileFound*32+6;
		for (j=0;j<26;j++){
			sectors[j] = buffer[index+j];
		}
		sectors[26] = 0;
		k = 0;
		while(sectors[k]!=0x0){
			readSector(buffer+buffAddress,sectors[k]);
			buffAddress += 512;
			k++;
		}
	}
	else{
		printString("File not found :'(");
		return;
	}
}

int strComp(char* buffer, char* fileName){
	
	int i, j;
	int checkFound = 0;
	for (i = 0; i < 16; i++)
	{
		if (buffer[32*i] != 0x0){
			for (j=0; j < 6; j++){

				if (buffer[j+32*1] == 0x0 || buffer[j+32*1] == '\r' || buffer[j+32*1] == '\n' || fileName[j] == 0x0 || fileName[j] == '\r' || fileName[j] == '\n'){
					break;
				}
				else if (buffer[j+32*i] == fileName[j]){
					checkFound = 1;
				}
				else {
					checkFound = 0;
					break;
				}
			}
	
			if (checkFound == 1){
				return i;
			}
			else{
				/*printString("Next check");*/
			}
		}
	}

	if (checkFound == 0){
		for (i=0;i<13312;i++){
			buffer[i] = 0x0;
		}
		return 0;
	}
}

