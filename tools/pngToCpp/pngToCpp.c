/**
 *******************************************************************************
 * @file pngToCpp.cpp
 * @brief convert a binary file into a C source vector
 * @author Sandro Sigala
 * @date 26/01/2011
 * @par Project
 * ---
 *
 * @par Copyright
 * Copyright 2010 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *    You can not earn money with this Software (if the source extract from Edn
 *        represent less than 50% of original Sources)
 * Term of the licence in in the file licence.txt.
 * 
 * @compilation g++ pngToCpp -o pngToCpp
 * 
 *******************************************************************************
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

bool zeroTerminated = false;

int myfgetc(FILE *f)
{
	int c = fgetc(f);
	if (c == EOF && zeroTerminated)
	{
		zeroTerminated = 0;
		return 0;
	}
	return c;
}

void generateHeader(FILE *file)
{
	fprintf(file, "/**\n");
	fprintf(file, " ******************************************************************************* \n");
	fprintf(file, " * \n");
	fprintf(file, " * @par Project\n");
	fprintf(file, " * Edn\n");
	fprintf(file, " * \n");
	fprintf(file, " * Automatic generated file for Edn Software\n");
	fprintf(file, " * Please do not save this file on File configuration server\n");
	fprintf(file, " * \n");
	fprintf(file, " * @par Copyright\n");
	fprintf(file, " * \n");
	fprintf(file, " * Copyright 2010 Edouard DUPIN, all right reserved\n");
	fprintf(file, " * \n");
	fprintf(file, " * his software is distributed in the hope that it will be useful, but WITHOUT\n");
	fprintf(file, " * ANY WARRANTY.\n");
	fprintf(file, " * \n");
	fprintf(file, " * Licence summary : \n");
	fprintf(file, " *    You can modify and redistribute the sources code and binaries.\n");
	fprintf(file, " *    You can send me the bug-fix\n");
	fprintf(file, " *    You can not earn money with this Software (if the source extract from Edn\n");
	fprintf(file, " *        represent less than 50/100 of original Sources)\n");
	fprintf(file, " * Term of the licence in in the file licence.txt.\n");
	fprintf(file, " * \n");
	fprintf(file, " ******************************************************************************* \n");
	fprintf(file, " */\n\n");
}

FILE *ofile=NULL;
FILE *ofileH=NULL;

void process(const char *ifname)
{
	FILE *ifile=NULL;
	ifile = fopen(ifname, "rb");
	if (ifile == NULL)
	{
		fprintf(stderr, "cannot open %s for reading\n", ifname);
		exit(1);
	}
	char buf[PATH_MAX], *p;
	const char *cp;
	if ((cp = strrchr(ifname, '/')) != NULL)
	{
		++cp;
	} else {
		if ((cp = strrchr(ifname, '\\')) != NULL)
			++cp;
		else
			cp = ifname;
	}
	strcpy(buf, cp);
	for (p = buf; *p != '\0'; ++p)
	{
		if (!isalnum(*p))
			*p = '_';
	}
	fprintf(ofile, "unsigned char %s[] = {\n\t\t", buf);
	int n = 0;
	unsigned char c = 0;
	while(fread(&c, 1, 1, ifile) == 1)
	{
		if(n%100 == 0) {
			fprintf(ofile, "\n\t\t");
		}
		fprintf(ofile, "0x%02x,", c);
		n++;
	}
	fprintf(ofile, "\n};\n");
	fprintf(ofile, "unsigned long int %s_size = sizeof(%s);\n\n\n", buf, buf);
	
	fprintf(ofileH, "extern unsigned char %s[];\n", buf);
	fprintf(ofileH, "extern unsigned long int %s_size;\n", buf);
	
	fclose(ifile);
}
 
void usage(void)
{
	fprintf(stderr, "usage: pngToCpp <output_file>.xxx <input_file>\n");
	exit(1);
}
 
int main(int argc, char **argv)
{
	//zeroTerminated = true;
	if (argc < 3) {
		usage();
	}
	// dynamic output name : 
	char tmpOutput[PATH_MAX*2] = "";
	strcpy(tmpOutput, argv[1]);
	int32_t sizeName = strlen(tmpOutput);
	if (6>=sizeName) {
		usage();
	}
	if (tmpOutput[sizeName-1] == '.') {
		tmpOutput[sizeName-1] = '\0';
	} else if (tmpOutput[sizeName-2] == '.') {
		tmpOutput[sizeName-2] = '\0';
	} else if (tmpOutput[sizeName-3] == '.') {
		tmpOutput[sizeName-3] = '\0';
	} else if (tmpOutput[sizeName-4] == '.') {
		tmpOutput[sizeName-4] = '\0';
	} else if (tmpOutput[sizeName-5] == '.') {
		tmpOutput[sizeName-5] = '\0';
	} else if (tmpOutput[sizeName-6] == '.') {
		tmpOutput[sizeName-6] = '\0';
	}
	//fprintf(stderr, "writing to %s\n", tmpOutput);
	
	char tmpOutputCPP[PATH_MAX*2] = "";
	char tmpOutputH[PATH_MAX*2] = "";
	// Generate the output filename
	sprintf(tmpOutputCPP,"%s.cpp", tmpOutput);
	sprintf(tmpOutputH,"%s.h", tmpOutput);
	// open destination name : 
	ofile = fopen(tmpOutputCPP, "wb");
	if (ofile == NULL)
	{
		fprintf(stderr, "cannot open %s for writing\n", tmpOutputCPP);
		exit(1);
	}
	generateHeader(ofile);
	ofileH = fopen(tmpOutputH, "wb");
	if (ofileH == NULL)
	{
		fprintf(stderr, "cannot open %s for writing\n", tmpOutputH);
		exit(1);
	}
	generateHeader(ofileH);
	fprintf(ofileH, "#ifndef __INPUT_PNG_FILE_GENERATED_H__\n");
	fprintf(ofileH, "#define __INPUT_PNG_FILE_GENERATED_H__\n");
	//fprintf(ofileH, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n");
	//fprintf(ofile, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n");
	
	// Generate the output
	int32_t i;
	for(i=2; i<argc; i++) {
		process(argv[i]);
	}
	
	//fprintf(ofile, "#ifdef __cplusplus\n}\n#endif\n");
	//fprintf(ofileH, "#ifdef __cplusplus\n}\n#endif\n");
	
	fprintf(ofileH, "#endif\n\n");
	// Close destination files
	fclose(ofile);
	fclose(ofileH);
	
	return 0;
}
