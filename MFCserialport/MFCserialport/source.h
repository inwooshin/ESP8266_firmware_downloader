#pragma once

#ifndef SOURCE_H
#define SOURCE_H

int sync[46] = { 0xC0, 0x00, 0x08, 
		0x24, 0x00, 0x78, 0x01, 0x3A, 0x00,
		0x07, 0x07, 0x12, 0x20, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xC0 };
//0x55 32�� �ؾߵ� ��

int flashBegin[] = { 0xC0, 0x00, 0x02, 
			0x10, 0x00, //Size 16byte
			0x00, 0x00, 0x00, 0x00, //checksum
			0x00, 0x08, 0x00, 0x00, //Size to erase
			0x00, 0x01, 0x00, 0x00, //number of data packets �� ������ ��Ŷ�� ����?
			0x08, 0x00, 0x00, 0x00, //Data Size in packets
			0x00, 0x00, 0x00, 0x00, 0xC0 //flash offset
};

int flashData[40] = {
	0xC0, 0x00, 0x03,
	0x08, 0x00, //Data Size 
	0x00, 0x00, 0x00, 0x00, //check sum
	0x00,					//���� ������
};

int flashEnd[] = {
	0xC0, 0x00, 0x04,
	0x04, 0x00, //Size
	0x00, 0x00, 0x00, 0x00, //check sum �Ⱦ�!
	0x01, 0x00, 0x00, 0x00, 0xC0 //enter add
};

//checksum �� ������ִ� �Լ�!
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

//size �� ����� �Լ�!
int SizeofChar(char* s) {
	int i = 0;
	while (s[i] != '\0')
		i++;

	return i;
}

//��Ŷ�� ������ ���ڿ� �߰��� �������ֱ� ���� �Լ�!
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