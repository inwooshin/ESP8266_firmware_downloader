#pragma once

#ifndef SOURCE_H
#define SOURCE_H

/*
char ESP_FLASH_BEGIN = 0x02;
char ESP_FLASH_DATA = 0x03;
char ESP_FLASH_END = 0x04;
char ESP_MEM_BEGIN = 0x05;
char ESP_MEM_END = 0x06;
char ESP_MEM_DATA = 0x07;
char ESP_SYNC = 0x08;
char ESP_WRITE_REG = 0x09;
char ESP_READ_REG = 0x0a;

char ESP_RAM_BLOCK = 0x1800;
char ESP_FLASH_BLOCK = 0x400;
char ESP_ROM_BAUD = 115200;
char ESP_IMAGE_MAGIC = 0xe9;
*/


int sync[50] = { 0xC0, 0x00, 0x08, 
		0x24, 0x00, 0x78, 0x01, 0x3A, 0x00,
		0x07, 0x07, 0x12, 0x20, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xC0 };
//0x55 32번 해야됨 ㅠ

int flashBegin[] = { 0xC0, 0x00, 0x02, 
			0x10, 0x00, //Size 16byte
			0x00, 0x00, 0x00, 0x00, //checksum
			0x00, 0x08, 0x00, 0x00, //Size to erase
			0x00, 0x01, 0x00, 0x00, //number of data packets 총 데이터 패킷의 개수?
			0x08, 0x00, 0x00, 0x00, //Data Size in packets
			0x00, 0x00, 0x00, 0x00, 0xC0 //flash offset
};

//memoffset?? 조금 나중!

int flashData[40] = {
	0xC0, 0x00, 0x03,
	0x08, 0x00, //Data Size 
	0x00, 0x00, 0x00, 0x00, //check sum
	0x00,					//보낼 데이터
};

int flashEnd[] = {
	0xC0, 0x00, 0x04,
	0x04, 0x00, //Size
	0x00, 0x00, 0x00, 0x00, //check sum 안씀!
	0x01, 0x00, 0x00, 0x00, 0xC0 //enter add
};

char* checksum(char * data, int length, unsigned int state = 0xef) {
	char tmp[4] = {0,};
	
	for (int i = 0; i < length; i++) {
		state ^= data[i];
	}

	for (int i = 0; i < 4; i++) {
		tmp[i] = state;

		state = state >> 8;
	}

	return tmp;
}

unsigned char* trans(int* in, int *length){
	unsigned char str[100] = "";

	for (int i = 0; i < sizeof(in) / sizeof(int); i++) {
		str[i] = (unsigned char)in[i];
	}

	return str;
}

int SizeofChar(char* s) {
	int i = 0;
	while (s[i] != '\0')
		i++;

	return i;
}

int SizeofChar(int* s) {
	int i = 0;
	while (s[i] != '\0')
		i++;

	return i;
}


int StrNinsert(char* s1, char* s2, int pos, int n) {
    int i = 0;
    int j;
    char tem[1024];
    if (pos<0 || n<0 || pos>SizeofChar(s1) || n>SizeofChar(s2))
        return 0;
    else {
        while (s1[i] != '\0') {
            tem[i] = s1[pos + i];
            i++;
        }
        tem[i] = '\0';
        for (i = 0; i < n; i++) {
            s1[pos + i] = s2[i];
        }
        i = 0;
        while (tem[i] != '\0') {
            s1[pos + n + i] = tem[i];
            i++;
        }
        s1[pos + n + i] = '\0';
    }
    return 1;
}

#endif //SOURCE_H