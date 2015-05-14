#define VIDEO_SEGMENT 0xB000
#define LINE3_OFFSET 0x8000
#define BLACK 0x00

void k_clear_screen(); 
int yScreen=0;
char *str="";
int indexString=0;

int main() {

		k_clear_screen();

		str = "Hello world!";

		for (yScreen=0; yScreen<12; yScreen++){
	
			int xScreen=0;
			
			for (indexString=0; indexString<12; indexString++){
				putInMemory(VIDEO_SEGMENT, LINE3_OFFSET + (xScreen+(yScreen*80)*2), str[indexString]);
				xScreen++;
				putInMemory(VIDEO_SEGMENT, LINE3_OFFSET + (xScreen+(yScreen*80)*2), yScreen);
				xScreen++;
			};
				
   		};
		
		while(1);
		return 0;
}

void k_clear_screen()
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
