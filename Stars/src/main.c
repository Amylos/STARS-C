/*
 ============================================================================
 Name        : main.c
 Author      : <author>
 Version     : <date-version>
 Copyright   : <Your copyright notice>
 Description : StarsExpansion in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <app.h>

int main(void) {
	printf("==============ENTER APPLICATION=============\n");
	t_app*pApp=AppNew();
	int ret=AppRun(pApp);
	pApp=AppDel(pApp);
	printf("==============EXIT  APPLICATION=============\n");
	return ret;
}
