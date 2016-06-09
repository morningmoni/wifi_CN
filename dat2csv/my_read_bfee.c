#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>

void read_bfee(unsigned char *inBytes, FILE *out) {
	unsigned int timestamp_low = inBytes[0] + (inBytes[1] << 8) +
	                             (inBytes[2] << 16) + (inBytes[3] << 24);
	fprintf(out, "%d,", timestamp_low);
	unsigned short bfee_count = inBytes[4] + (inBytes[5] << 8);
	fprintf(out, "%d,", bfee_count);
	unsigned int Nrx = inBytes[8];
	fprintf(out, "%d,", Nrx);
	unsigned int Ntx = inBytes[9];
	fprintf(out, "%d,", Ntx);
	unsigned int rssi_a = inBytes[10];
	fprintf(out, "%d,", rssi_a);
	unsigned int rssi_b = inBytes[11];
	fprintf(out, "%d,", rssi_b);
	unsigned int rssi_c = inBytes[12];
	fprintf(out, "%d,", rssi_c);
	char noise = inBytes[13];
	fprintf(out, "%d,", noise);
	unsigned int agc = inBytes[14];
	fprintf(out, "%d,", agc);
	unsigned int antenna_sel = inBytes[15];
	fprintf(out, "%d,", antenna_sel);
	unsigned int len = inBytes[16] + (inBytes[17] << 8);
	fprintf(out, "%d,", len);
	unsigned int fake_rate_n_flags = inBytes[18] + (inBytes[19] << 8);
	fprintf(out, "%d,", fake_rate_n_flags);


	unsigned int calc_len = (30 * (Nrx * Ntx * 8 * 2 + 3) + 7) / 8;
	unsigned int i, j;
	unsigned int index = 0, remainder;
	unsigned char *payload = &inBytes[20];
	char tmp;
	/* Check that length matches what it should */
	if (len != calc_len) {
		printf("MIMOToolbox:read_bfee_new:size, Wrong beamforming matrix size.");
		exit(-1);
	}

	/* Compute CSI from all this crap :) */
	for (i = 0; i < 30; ++i) {
		index += 3;
		remainder = index % 8;
		double csi = 0, csi_r, csi_i;
		for (j = 0; j < Nrx * Ntx; ++j) {
			tmp = (payload[index / 8] >> remainder) |
			      (payload[index / 8 + 1] << (8 - remainder));
			csi_r = (double) tmp;
			tmp = (payload[index / 8 + 1] >> remainder) |
			      (payload[index / 8 + 2] << (8 - remainder));
			csi_i = (double) tmp;
			index += 16;
			csi += sqrt(csi_r * csi_r + csi_i * csi_i);
		}
		if (i < 29)
			fprintf(out, "%lf,", csi / (Nrx * Ntx));
		else
			fprintf(out, "%lf\n", csi / (Nrx * Ntx));
	}
}


unsigned char buffer[1000000];

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		printf("usage: read_bfee in.dat out.csv\n");
		return 0;
	}
	FILE * fp = fopen(argv[1], "rb");
	FILE * out = fopen(argv[2], "w");
	unsigned short l, l2;
	unsigned char code;
	fprintf(out, "timestamp_low,bfee_count,Nrx,Ntx,rssi_a,rssi_b,rssi_c,noise,agc,antenna_sel,len,fake_rate_n_flags,csi01,csi02,csi03,csi04,csi05,csi06,csi07,csi08,csi09,csi10,csi11,csi12,csi13,csi14,csi15,csi16,csi17,csi18,csi19,csi20,csi21,csi22,csi23,csi24,csi25,csi26,csi27,csi28,csi29,csi30\n");
	while (!feof(fp)) {
		fread(&l2, 1, sizeof(unsigned short), fp);
		l = ntohs(l2);
		fread(&code, 1, sizeof(unsigned char), fp);
		if (code == 187) {
			int get = fread(&buffer, 1, l - 1, fp);
			if (get != l - 1) {
				printf("need %d, get %d", l - 1, get);
				printf("file end unexpectly\n");
				fclose(fp);
				return -1;
			}
		} else {
			fseek(fp, l - 1, SEEK_CUR);
			continue;
		}
		read_bfee(buffer, out);
	}
	return 0;
}
