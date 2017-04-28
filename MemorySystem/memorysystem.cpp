/*
 * MemorySystem.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: john
 */

#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include "memorysystem.h"

#define BUFFER_SIZE 256
#define MAX_ADDRESSES 100

const char * IN_FILE = "input.txt";
const char * DISK_FILE = "BACKING_STORE";

int main() {

	const char *delimiters = ", ";
	char buffer[BUFFER_SIZE];
	char *last_addr;
	u_int_t logic_addr[MAX_ADDRESSES];
	int i = 0;
	int num_addr;
	int result;
	printf("hi");
	FILE * input;
	input = fopen(IN_FILE, "r");

	if (input == NULL) {
		printf("Error: could not read %s", IN_FILE);
		return -1;
	}

	while (fgets(buffer, BUFFER_SIZE, input) != NULL) {

		last_addr = strtok(buffer, delimiters);
		while (last_addr != NULL) {
			logic_addr[i] = atoi(last_addr);
			//printf("%d", logic_adrr[i]);
			last_addr = strtok(NULL, delimiters);
			i++;
		}
;
	}
	num_addr = i;
	fclose(input);

	tlb_t * tlb;
	result = CreateTLB(tlb);

	page_table_t * page_table;
	result = CreatePageTable(page_table);
	if(result) {
		printf("Error: Could not create page table.");
		return -1;
	}

	for(i = 0; i < num_addr; i++) {
		bool *hit = (bool *) malloc(sizeof(bool));
		char ** frames = (char **) malloc(sizeof(char *));
		u_int_t page_num = ParsePageNum(logic_addr[i]);
		u_int_t offset= ParseOffset(logic_addr[i]);
		*hit = false;
		//SearchTLB(page_num, tlb, hit, frames);
		if(*hit == false) {
			printf("HI");
			SearchPageTable(page_num, page_table, hit, frames);
			if (*hit == false) {
				PageFaultHandler(page_num, DISK_FILE, page_table, tlb);
			}
		}
	}

	return 0;
}

int CreateTLB(tlb_t *tlb) {
	tlb = (tlb_t *) malloc(sizeof(tlb_t));

	if (tlb == NULL) {
		return -1;
	}

	int i;
	for (i = 0; i < TLB_SIZE; i++) {
		tlb->list[i] = NULL;
	}

	tlb->size=0;

	return 0;
}

int SearchTLB(u_int_t page_num, tlb_t * tlb, bool *is_tlb_hit,
		char **frames) {
	if(tlb->size>0){
	for(int i = 0; i < tlb->size; i++){
		if(tlb->list[i]->page_num == page_num){
			*is_tlb_hit = true;
			*frames = tlb->list[i]->frames;
		}
	}
	}
	*is_tlb_hit = false;
	*frames = NULL;

	return 0;
}

int CreatePageTable(page_table_t *page_table) {

	page_table = (page_table_t *) malloc(sizeof(page_table_t));

	if (page_table == NULL) {
		return -1;
	}

	int i;
	for (i = 0; i < NUM_PAGES; i++) {
		page_table->list[i] = NULL;
	}
	page_table->num_entries = 0;

	return 0;
}

int SearchPageTable(u_int_t page_num, page_table_t * page_table, bool *is_page_hit,
		char **frames) {

	if (page_table->list[page_num]->frames == NULL) {
		*frames = NULL;
		*is_page_hit = false;
		return 0;
	}

	*frames = page_table->list[page_num]->frames;

	return 0;
}
int PageFaultHandler(u_int_t page_num, const char * phys_mem_filename,
		page_table_t *page_table, tlb_t *tlb) {

	page_t * new_page = (page_t *) malloc(sizeof(page_t));

	if (new_page == NULL) {
		return -2;
	}

	new_page->frames = (char *) malloc(sizeof(char) * PAGE_SIZE);

	if (new_page->frames == NULL) {
		return -2;
	}

	FILE * disk;
	disk = fopen(phys_mem_filename, "rb");
	if (disk == NULL) {
		printf("Error: could not read %s", phys_mem_filename);
		return -1;
	}

	fseek(disk, page_num * PAGE_SIZE, SEEK_SET);

	fread(new_page->frames, 1, 256, disk);

	fclose(disk);

	return 0;
}

u_int_t ParsePageNum(u_int_t logical_addr) {
	return (logical_addr | PAGE_MASK) >> PAGE_SHIFT;
}

u_int_t ParseOffset(u_int_t logical_addr) {
	return (logical_addr | OFFSET_MASK);
}
