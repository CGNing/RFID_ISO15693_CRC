#include <stdio.h>
#include <stdlib.h>

#define POLYNOMIAL 0x8408 // x^16 + x^12 + x^5 + 1
#define PRESET_VALUE 0xFFFF
#define CHECK_VALUE 0xF0B8
#define CALC_CRC 1
#define CHECK_CRC 0

void main()
{
	unsigned int current_crc_value;
	unsigned char* array_of_databytes;
	int number_of_databytes;
	int calculate_or_check_crc;
	int i, j;
	
	do {
		printf ("Please input calculate or check crc(0 is CHECK,1 is CALC):\n");
		scanf ("%d",&calculate_or_check_crc);
	} while (calculate_or_check_crc != 0 && calculate_or_check_crc != 1);
	
	do {
		printf ("Please input number of databytes:\n");
		scanf ("%d",&number_of_databytes);
	} while ((calculate_or_check_crc == 0&&number_of_databytes<1)||(calculate_or_check_crc == 1&&number_of_databytes<3));
	
	array_of_databytes = malloc(number_of_databytes);
	printf ("Please input the array of databytes(XX XX XX...):\n");
	for (i = 0; i < number_of_databytes; i++)
		scanf ("%02X",&array_of_databytes[i]);
	
	current_crc_value = PRESET_VALUE;
	for (i = 0; i < number_of_databytes; i++)
	{
		current_crc_value = current_crc_value^((unsigned int)array_of_databytes[i]);
	    for (j = 0; j < 8; j++)
		{
			if (current_crc_value & 0x0001)
			{
				current_crc_value = (current_crc_value >> 1) ^ POLYNOMIAL;
			}
			else
			{
				current_crc_value = (current_crc_value >> 1);
			}
		}
	}
	
	if (calculate_or_check_crc == CALC_CRC)
	{
	current_crc_value = ~current_crc_value;
	printf ("CRC-ISO/IEC 13239 of{ ");
	for (i = 0; i < number_of_databytes; i++)
		printf ("%02X ",array_of_databytes[i]);
	printf ("} is '%04X'\n",current_crc_value & 0xFFFF);
	printf ("Generated CRC is '%04X'\n", current_crc_value & 0xFFFF);
	printf ("The Least Significant Byte (transmitted first) is: '%02X'\n",(current_crc_value) & 0xFF);
	printf ("The Most Significant Byte (transmitted second) is: '%02X'\n",(current_crc_value >> 8) & 0xFF);
	printf ("Executing this program when CHECK_CRC generates: 'F0B8'\n");
// current_crc_value is now ready to be appended to the data stream
// (first LSByte, then MSByte)
	}
	else // check CRC
	{
		if (current_crc_value == CHECK_VALUE)
		{
			printf("Checked CRC is ok (0x%04X)\n", current_crc_value);
		}
		else
		{
			printf("Checked CRC is NOT ok (0x%04X)\n", current_crc_value);
		}
	}
	
	system("pause");
}
