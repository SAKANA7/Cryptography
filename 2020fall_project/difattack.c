#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
unsigned short s_box[] = { 0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7 };
unsigned short decipher_s_box[] = { 0xe, 0x3, 0x4, 0x8, 0x1, 0xc, 0xa, 0xf, 0x7, 0xd, 0x9, 0x6, 0xb, 0x2, 0x0, 0x5 };
int s[0x10000], p[0x10000];
unsigned short Ruler_24 = 0x6;
unsigned short Ruler_13 = 0x6;
unsigned int Ruler = 0x0b00;
int count[256] = { 0 };
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
	unsigned int k[5];                      //ÃÜÔ¿·Ö¶Î
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
unsigned diffenrence(unsigned short* output) {
	int i, x1, x2, y1, y2, l1, l2, v11, v21;
	int v12, v14, v22, v24;
	unsigned int key;
	for (x1 = 0; x1 <= 0xffff; x1++) {
		x2 = x1 ^ 0xb00;
		y1 = output[x1];
		y2 = output[x2];
		if ((y1 & 0xf0f0) == (y2 & 0xf0f0)) {
			for (i = 0; i <= 0xff; i++) {
				l1 = (i & 0xf0) >> 4;
				l2 = i & 0xf;
				v12 = l1 ^ ((y1 & 0xf00) >> 8);
				v14 = l2 ^ (y1 & 0xf);
				v22 = l1 ^ ((y2 & 0xf00) >> 8);
				v24 = l2 ^ (y2 & 0xf);
				v12 = decipher_s_box[v12];
				v14 = decipher_s_box[v14];
				v22 = decipher_s_box[v22];
				v24 = decipher_s_box[v24];
				if (((v12 ^ v22) == 0b0110) && ((v14 ^ v24) == 0b0110))
					count[i]++;
			}
		}
	}
	int max24=-1;
	unsigned key24;
	for (unsigned j = 0; j < 16 * 16; j++) {
		if (count[j] > max24) {
			max24 = count[j];
			key24 = j;

		}
	}
	for (i = 0; i < 16 * 16; i++) {
		count[i] = 0;
	}
	for (x1 = 0; x1 <= 0xffff; x1++) {
		x2 = x1 ^ 0xd00;
		y1 = output[x1];
		y2 = output[x2];
		if ((y1 & 0xf0f) == (y2 & 0xf0f)) {
			for (i = 0; i <= 0xff; i++) {
				l1 = (i & 0xf0) >> 4;
				l2 = i & 0xf;
				v12 = l1 ^ ((y1 & 0xf000) >> 12);
				v14 = l2 ^ ((y1 & 0xf0) >> 4);
				v22 = l1 ^ ((y2 & 0xf000) >> 12);
				v24 = l2 ^ ((y2 & 0xf0) >> 4);
				v12 = decipher_s_box[v12];
				v14 = decipher_s_box[v14];
				v22 = decipher_s_box[v22];
				v24 = decipher_s_box[v24];
				if (((v12 ^ v22) == 0b0110) && ((v14 ^ v24) == 0b0110))
					count[i]++;
			}
		}
	}
	int max13 = -1;
	unsigned key13 = 0;
	for (unsigned j = 0; j < 16 * 16; j++) {
		if (count[j] > max13) {
			max13 = count[j];
			key13 = j;
		}
	}
	key = ((key13 & 0xf0) << 8) + ((key13 & 0xf) << 4) + ((key24 & 0xf0) << 4) + (key24 & 0xf);
	return key;
}

/*unsigned short difattack24(unsigned short* output) {
	
	short count_24[256] = { 0 };
	int i;
	int  y1,y2,x1,x2,l1,l2,v12,v14,v22,v24,key;
	for (x1 = 0; x1 <= 0xffff; x1++) {
		x2 = x1 ^ Ruler;
		y1 = output[x1];
		y2 = output[x2];
		if ((y1 & 0xf0f0) == (y2 & 0xf0f0)) {
			for (i = 0; i <= 0xff; i++) {
				l1 = (i & 0xf0) >> 4;
				l2 = i & 0xf;
				v12 = l1 ^ ((y1 & 0xf00) >> 8);
				v14 = l2 ^ (y1 & 0xf);
				v22 = l1 ^ ((y2 & 0xf00) >> 8);
				v24 = l2 ^ (y2 & 0xf);
				v12 = decipher_s_box[v12];
				v14 = decipher_s_box[v14];
				v22 = decipher_s_box[v22];
				v24 = decipher_s_box[v24];
				if (((v12 ^ v22) == Ruler_24) && ((v14 ^ v24) == Ruler_24))
					count_24[i]++;
			}
		}
	}
	short max24 = -1;
	for (i = 0; i < 256; i++) {	 
		if (count_24[i] > max24) {
			max24 = count_24[i];
			key = i;
		}
	}
	return key;
}

unsigned short difattack13(unsigned short* output) {
	short count_13[256] = { 0 };
	int i;
	int y1, y2, x1, x2, l1, l2, v12, v14, v22, v24, key;
	for (x1 = 0; x1 <= 0xffff; x1++) {
		x2 = x1 ^ 0x0d00;
		y1 = output[x1];
		y2 = output[x2];
		if ((y1 & 0xf0f0) == (y2 & 0xf0f0)) {
			for (i = 0; i <= 0xff; i++) {
				l1 = (i & 0xf0) >> 4;
				l2 = i & 0xf;
				v12 = l1 ^ ((y1 & 0xf000) >> 12);
				v14 = l2 ^ ((y1 & 0xf0) >> 4);
				v22 = l1 ^ ((y2 & 0xf000) >> 12);
				v24 = l2 ^ ((y2 & 0xf0) >> 4);
				v12 = decipher_s_box[v12];
				v14 = decipher_s_box[v14];
				v22 = decipher_s_box[v22];
				v24 = decipher_s_box[v24];
				if (((v12 ^ v22) == Ruler_13) && ((v14 ^ v24) == Ruler_13))
					count_13[i]++;
			}
		}
	}
	short max13 = -1;
	for (i = 0; i < 256; i++) {
		if (count_13[i] > max13) {
			max13 = count_13[i];
			key = i;
		}
	}
	return key;
}
*/
int main() {
	int i,j,k,n;
	unsigned short output[65600];
	unsigned short key24, key13,key,temp;
	unsigned int key_new;
	unsigned int key_l;
	int count = 0;
	int flag = 1;
	unsigned int ans[1000] = { 0x00000000 };
	creatSP();
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < 0x10000; j++) {
			scanf("%hx", &output[j]);
		}
		key = diffenrence(output);
		for (key_l = 0; key_l <= 0xffff; key_l++) {
			key_new = ((key_l << 16) | key);
			for (k = 0; k < 0x10000; k++) {
				temp = spn(k, key_new);
				if (temp != output[k]) {
					break;
				}
			}
			if (k == 0x10000) {
				ans[i] = key_new;
				flag = 0;
			}
		}
	}
	for (i = 0; i < n; i++) {
		printf("%08x\n", ans[i]);
	}
	return 0;
}