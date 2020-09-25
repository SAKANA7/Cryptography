#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
unsigned short s_box[] = {0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7 };
unsigned short decipher_s_box[] = {0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 };
int s[0x10000],p[0x10000];
/*unsigned short spn(unsigned short x,unsigned int t) {
	unsigned int k[5];
	k[0] = t >> 16;
	k[1] = (t & 0x0ffff000) >> 12;
	k[2] = (t & 0xffff00) >> 8;
	k[3] = (t & 0xffff0) >> 4;
	k[4] = t & 0xffff;
	int a[4];
	for (int i = 0; i <= 2; i++) {
		x = x ^ k[i];
		//明文分段
		a[0] = (x & 0xf000) >> 12;
		a[1] = (x & 0xf00) >> 8;
		a[2] = (x & 0xf0) >> 4;
		a[3] = x & 0xf;
		//s_box
		for (int j = 0; j < 4; j++)
			a[j] = s_box[a[j]];
		//p_box
		x = ((a[0] & 0b1000) << 12) + ((a[1] & 0b1000) << 11) + ((a[2] & 0b1000) << 10) + ((a[3] & 0b1000) << 9) + ((a[0] & 0b100) << 9) + ((a[1] & 0b0100) << 8) + ((a[2] & 0b0100) << 7) + ((a[3] & 0b0100) << 6) + ((a[0] & 0b0010) << 6) + ((a[1] & 0b0010) << 5) + ((a[2] & 0b0010) << 4) + ((a[3] & 0b0010) << 3) + ((a[0] & 0b0001) << 3) + ((a[1] & 0b0001) << 2) + ((a[2] & 0b0001) << 1) + (a[3] & 0b0001);
	}
	x = x ^ k[3];
	a[0] = (x & 0xf000) >> 12;
	a[1] = (x & 0xf00) >> 8;
	a[2] = (x & 0xf0) >> 4;
	a[3] = x & 0xf;
	for (int j = 0; j < 4; j++)
		a[j] = s_box[a[j]];
	x = (a[0] << 12) + (a[1] << 8) + (a[2] << 4) + a[3];
	return (x ^ k[4]);
}*/

void creatSP() {
	unsigned a[4];
	for (int x = 0; x <= 0xffff; x++) {
		a[0] = (x & 0xf000) >> 12;
		a[1] = (x & 0xf00) >> 8;
		a[2] = (x & 0xf0) >> 4;
		a[3] = x & 0xf;
		p[x] = ((a[0] & 0b1000) << 12) + ((a[1] & 0b1000) << 11) + ((a[2] & 0b1000) << 10) + ((a[3] & 0b1000) << 9) + ((a[0] & 0b100) << 9) + ((a[1] & 0b0100) << 8) + ((a[2] & 0b0100) << 7) + ((a[3] & 0b0100) << 6) + ((a[0] & 0b0010) << 6) + ((a[1] & 0b0010) << 5) + ((a[2] & 0b0010) << 4) + ((a[3] & 0b0010) << 3) + ((a[0] & 0b0001) << 3) + ((a[1] & 0b0001) << 2) + ((a[2] & 0b0001) << 1) + (a[3] & 0b0001);
		for (int j = 0; j < 4; j++)
			a[j] = s_box[a[j]];
		s[x] = (a[0] << 12) + (a[1] << 8) + (a[2] << 4) + a[3];
	}
}
unsigned spn(unsigned short x, unsigned int t) {
	unsigned int k[5];                      //密钥分段
	k[0] = t >> 16;
	k[1] = (t & 0x0ffff000) >> 12;
	k[2] = (t & 0xffff00) >> 8;
	k[3] = (t & 0xffff0) >> 4;
	k[4] = t & 0xffff;
	for (int i = 0; i < 3; i++) {
		x = x ^ k[i];
		x = s[x];
		x = p[x];
	}
	x = x ^ k[3];
	x = s[x];
	x = x ^ k[4];
	return x;
}
//返回key24[13]
unsigned short* LinerAttack24(unsigned short* input, unsigned short* output) {
	int count24[16][16] = { 0 };
	int i, j, k;
	static unsigned short key[13];//
	unsigned short y2, y4,  v42, v44, u42,  u44,  L2,  L4, z24= 0x0;
	for (i = 0; i < 8000; i++) {
		y2 = (output[i] & 0x0f00) >> 8;
		y4 = (output[i] & 0x000f);
		for (j = 0; j < 16; j++) {
			L4 = j;
			for (k = 0; k < 16; k++) {
				L2 = k;	
				v42 = L4 ^ y2;
				v44 = L2 ^ y4;
				u42 = decipher_s_box[v42];
				u44 = decipher_s_box[v44];
				z24 = ((input[i] & 0x0800) >> 11) ^ ((input[i] & 0x0200) >> 9) ^ ((input[i] & 0x0100) >> 8) ^
					((u42 & 0x4) >> 2) ^ ((u42 & 0x1)) ^ ((u44 & 0x4) >> 2) ^ ((u44 & 0x1));
				if (z24 == 0) {
					count24[j][k]++;
				}
			}
		}
	}
	short max24 = -1;
	short max24_array[13] = { 0 };//
	unsigned short  max2[13] = {0x0}, max4[13] = { 0x0 };//
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			count24[i][j] = abs(count24[i][j] - 4000);
			}
		}
	//排序取topk
	for (i = 0; i < 13; i++) {//
		max24_array[i] = 0;
		for (j = 0; j < 16; j++) {
			for (k = 0; k < 16; k++) {
				if (count24[j][k] > max24_array[i]) {
					max24_array[i] = count24[j][k];
					max2[i] = j;
					max4[i] = k;
				}
			}
		}
		j = max2[i];
		k = max4[i];
		count24[j][k] = 0;
		key[i] = (max2[i] << 8) | max4[i];
	}
	return key;//右16位A4,但只有盒子2和4有
}
unsigned short LinerAttack13(unsigned short key24,unsigned short* input, unsigned short* output) {
	int count13[16][16] = { 0 };
	int i, j, k;
	unsigned short key;
	unsigned short y1, y2, y3, v41,v42, v43, u41,u42, u43, L1, L3, z13 = 0x0;
	for (i = 0; i < 8000; i++) {
		y1 = (output[i] & 0xf000) >> 12;
		y2 = (output[i] & 0x0f00) >> 8;
		y3 = (output[i] & 0x00f0) >> 4;
		for (j = 0; j < 16; j++) {
			L3 = j;
			for (k = 0; k < 16; k++) {
				L1 = k;
				v41 = L3 ^ y1;
				v42 = ((key24 & 0x0f00) >> 8) ^ y2;
				v43 = L1 ^ y3;
				u41 = decipher_s_box[v41];
				u42 = decipher_s_box[v42];
				u43 = decipher_s_box[v43];
				z13 = ((input[i] & 0x0800) >> 11) ^ ((input[i] & 0x0400) >> 10)^((input[i]&0x0200)>>9)^((input[i]&0x0100)>>8) ^
					((u42&0x4)>>2)^((u42&0x1))^((u43&0x4)>>2)^((u43&0x1))^((u41&0x4)>>2)^((u41&0x1));
				if (z13 == 0) {
					count13[j][k]++;
				}
			}
		}
	}
	short max13 = -1;
	unsigned short  max1, max3 = 0x0;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			count13[i][j] = abs(count13[i][j] - 4000);
			if (count13[i][j] > max13) {
				max13 = count13[i][j];
				max1 = i;
				max3 = j;
			}
		}
	}
	key = (max1<< 12) | (max3<< 4) ;
	return key;//右4位
}
int main() {
	int i,j, n,k;
	unsigned short input[8010], output[8010];
	unsigned short key13[13];
	unsigned short *key24;
	unsigned short key[13];
	unsigned short  temp = 0x0000;
	unsigned int key_l = 0x0000;
	unsigned int key_new = 0x00000000;
	int flag = 1, count = 0;
	unsigned int ans[100] = { 0x00000000 };
	creatSP();
	scanf("%d", &n);
	for (j = 0; j < n; j++) {
		flag = 1;
		for (i = 0; i < 8000; i++) {
			scanf("%hx", &input[i]);
			scanf("%hx", &output[i]);
		}
		key24 = LinerAttack24(input, output);
		for (i = 0; i < 13; i++) {//
			key13[i] = LinerAttack13(*(key24 + i), input, output);
			key[i] = key24[i] | key13[i];
			for(key_l=0;key_l<=0xffff;key_l++) {
				key_new = ((key_l << 16) | key[i]);
				for (k = 0; k < 8000; k++) {
					temp = spn(input[k], key_new);
					if (temp != output[k]) {
						break;
					}
				}
				if (k == 8000) {
					ans[j] = key_new;
					flag = 0;
                    
				}
			}	
			if (flag == 0)
			{
				break;
			}
		}		
	}
	for (i = 0; i < n; i++) {
		printf("%08x\n", ans[i]);
	}
	return 0;
}