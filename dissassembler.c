// Created by Daniel Richards
// argv[1] input is a text file with hex codes on each line which represesnts an LC-3 Instruction
// This program "dissassembles" the operations into LC-3 Assembly to be human readable by being printed out on the display. 


// Main iterates line by line through the file to read the hex. 
// Then passes into opcodePrinterBit which prints the whole line. This function calls other functions based off of the code to represent different LC-3 Opcodes.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


//FUNCTION AND GLOBAL VARIABLE DEFINITION

bool nBitChecker( int, unsigned int); // This function checks the Nth bit of a word passed into it. 
void opcodePrinterBit(unsigned int); // This function prints the opcode for the hex value. i.e AND 
void operandsPrinter(unsigned int, char[4]); // This is the function which controls how DR,SR1, SR2 etc are printed
void destinationRegister(unsigned int); // Prints the destination register
void statusRegister1(unsigned int); // Prints the Status Register 1
void statusRegister2(unsigned int); // Prints the Status Register 2
void imm5Print(unsigned int); // Prints the value used in immediate mode for AND / ADD
void addressPrintPCOffset9(unsigned int); // Prints the value of an address with an off set of 9 bits. 
unsigned int PC = 0;  // To be used as Global PC value for Part 4. Set to 0, so that if there is no passing in address it doesn't cause Runtime error. 

int main(int argc, char *argv[]){
	FILE *input;
	input = fopen(argv[1], "r");
	unsigned int line;
	if (argc == 3) { 

		PC = strtol(argv[2], NULL, 16); // Sees if the hex value of an address is passed into the function and assigns it to the PC.

	}
	if(input == NULL){
		printf("ERROR: \n");
		printf("There was an error in opening the file. Ensure you have passed in the obj file as an argument to the program. \n");
	}

	while (fscanf(input, "%x", &line) != EOF){

	PC = PC + 1;

	opcodePrinterBit(line);
	 // Adds to the PC count, pretending that we're actually going through the memory in LC-3.

	}
fclose(input);

}// End of Main Function


bool nBitChecker(int n, unsigned int word){
	return (1 & (word >> n));
}




// This is the major printer of
void operandsPrinter(unsigned int hex, char *opcode){

	if (opcode == "and") {

		if (nBitChecker(5, hex) == false) {
			//Register Mode
			destinationRegister(hex);
			statusRegister1(hex);
			statusRegister2(hex);
		}
		else {
			destinationRegister(hex);
			statusRegister1(hex);
			imm5Print(hex);
		} // in IMMU mode. For later dev


	} // END OF AND


// #######################################################################


	if (opcode == "add") {
		if (nBitChecker(5, hex) == false) {
			//Register Mode
			destinationRegister(hex);
			statusRegister1(hex);
			statusRegister2(hex);
		}
		else { // The opcode uses Imm5
			destinationRegister(hex);
			statusRegister1(hex);
			imm5Print(hex);
		} // End of IMM5 mode

	}// END OF ADD


// #######################################################################



	if (opcode == "br") {
		if (nBitChecker(11, hex) == true) {
			printf("n");
		}

		if (nBitChecker(10, hex) == true) {
			printf("z");
		}

		if (nBitChecker(9, hex) == true) {
			printf("p");
		}

		printf(" "); // Prints a space? 


		addressPrintPCOffset9(hex);




	}// END OF BR


// #######################################################################


	if (opcode ==  "jmp") {
		statusRegister1(hex); // This also prints the register used for JMP. - Isn't LC-3 smart?
	}// JMP 
	
	


}

void addressPrintPCOffset9(unsigned int hex) {
	int offSet = 0;
	if (nBitChecker(8, hex) == false) {
		if (nBitChecker(0, hex) == true) {
			offSet = offSet + 1;
		}
		if (nBitChecker(1, hex) == true) {
			offSet = offSet + 2;
		}
		if (nBitChecker(2, hex) == true) {
			offSet = offSet + 4;
		}
		if (nBitChecker(3, hex) == true) {
			offSet = offSet + 8;
		}
		if (nBitChecker(4, hex) == true) {
			offSet = offSet + 16;
		}
		if (nBitChecker(5, hex) == true) {
			offSet = offSet + 32;
		}
		if (nBitChecker(6, hex) == true) {
			offSet = offSet + 64;
		}
		if (nBitChecker(7, hex) == true) {
			offSet = offSet + 128;
		}
		
	
	}
	if (nBitChecker(8, hex) == true) { // The value is a negative number. 

		if (nBitChecker(0, hex) == false) {
			offSet = offSet + 1;
		}
		if (nBitChecker(1, hex) == false) {
			offSet = offSet + 2;
		}
		if (nBitChecker(2, hex) == false) {
			offSet = offSet + 4;
		}
		if (nBitChecker(3, hex) == false) {
			offSet = offSet + 8;
		}
		if (nBitChecker(4, hex) == false) {
			offSet = offSet + 16;
		}
		if (nBitChecker(5, hex) == false) {
			offSet = offSet + 32;
		}
		if (nBitChecker(6, hex) == false) {
			offSet = offSet + 64;
		}
		if (nBitChecker(7, hex) == false) {
			offSet = offSet + 128;
		}
		offSet = offSet + 1;
		offSet = offSet * -1;
		

	}

	int tempPC = PC;
	tempPC = tempPC + offSet;
	printf("0x%x", tempPC);
	// Might want to put the PC + offset here and printing here. 

}


// OpCodePrinterBit - This Prints the op code of the hex value in the file. Gets called from main();
void opcodePrinterBit(unsigned int hex){

	if((nBitChecker(15,hex) == false) & (nBitChecker(14,hex) == false) & (nBitChecker(13,hex) == false) & (nBitChecker(12,hex) == true)){
		printf("add ");
		operandsPrinter(hex, "add");
	} // END OF ADD

	if((nBitChecker(15,hex) == false) & (nBitChecker(14,hex) == true) & (nBitChecker(13,hex) == false) & (nBitChecker(12,hex) == true)){
		printf("and ");
		operandsPrinter(hex, "and" );
	} // END OF AND

	if((nBitChecker(15,hex) == false) & (nBitChecker(14,hex) == false) & (nBitChecker(13,hex) == false) & (nBitChecker(12,hex) == false)){
		printf("br", hex);
		operandsPrinter(hex, "br");
		printf("\n"); 
	} // END OF BR

	if((nBitChecker(15,hex) == true) & (nBitChecker(14,hex) == true) & (nBitChecker(13,hex) == false) & (nBitChecker(12,hex) == false)){
		printf("jmp ", hex);
		operandsPrinter(hex, "jmp");
		printf("\n"); // Because JMP uses code for SR1 Print function, you need to have \n at the end to go to a new line.
	} // END OF JMP

}


void destinationRegister(unsigned int hex) {

	int reg = 0;

	if (nBitChecker(9, hex) == true) {
		reg = reg + 1;
	}
	if (nBitChecker(10, hex) == true) {
		reg = reg + 2;
	}
	if (nBitChecker(11, hex) == true) {
		reg = reg + 4;
	}
	printf("r%d,", reg);

}// End of destinationRegister

void statusRegister1(unsigned int hex) {

	int reg = 0;

	if (nBitChecker(6, hex) == true) {
		reg = reg + 1;
	}
	if (nBitChecker(7, hex) == true) {
		reg = reg + 2;
	}
	if (nBitChecker(8, hex) == true) {
		reg = reg + 4;
	}
	printf("r%d", reg);

}

void statusRegister2(unsigned int hex) {
	int reg = 0;

	if (nBitChecker(0, hex) == true) {
		reg = reg + 1;
	}
	if (nBitChecker(1, hex) == true) {
		reg = reg + 2;
	}
	if (nBitChecker(2, hex) == true) {
		reg = reg + 4;
	}
	printf(",r%d\n", reg);
	
}

void imm5Print(unsigned int hex){

	 int reg = 0;
	if (nBitChecker(4, hex) == false) {
		if (nBitChecker(0, hex) == true) {
			reg = reg + 1;
		}
		if (nBitChecker(1, hex) == true) {
			reg = reg + 2;
		}
		if (nBitChecker(2, hex) == true) {
			reg = reg + 4;
		}
		if (nBitChecker(3, hex) == true) {
			reg = reg + 8;
		}
		
		printf(",%d \n", reg);
	}
	else { // The value is a negative number. 

		if (nBitChecker(0, hex) == false) {
			reg = reg + 1;
		}
		if (nBitChecker(1, hex) == false) {
			reg = reg + 2;
		}
		if (nBitChecker(2, hex) == false) {
			reg = reg + 4;
		}
		if (nBitChecker(3, hex) == false) {
			reg = reg + 8;
		}
		
		reg = reg + 1;
		printf(",-%d \n", reg);


	}

}
