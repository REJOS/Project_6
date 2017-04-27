/*
 * memorysystem.h
 *
 *  Created on: Apr 17, 2017
 *      Author: john
 */

#ifndef MEMORYSYSTEM_H_
#define MEMORYSYSTEM_H_

#define NUM_PAGES 256
#define NUM_FRAMES 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define PAGE_MASK 0b00000000000000001111111100000000
#define OFFSET_MASK 0b00000000000000000000000011111111
#define PAGE_SHIFT 8

typedef unsigned int u_int_t;

typedef struct {
	u_int_t page_num;
	char * frame;
	u_int_t age;
	//bool valid;
} tlb_entry_t;

typedef struct {
	tlb_entry_t * list[TLB_SIZE];
	u_int_t size;
	//u_int_t rem_next;
} tlb_t;

typedef struct {
	char * frame;
} page_t;

typedef struct {
	page_t * list[NUM_PAGES];
	u_int_t num_entries;
} page_table_t;

u_int_t ParsePageNum(u_int_t logical_addr);
u_int_t ParseOffset(u_int_t logical_addr);

int CreateTLB(tlb_t *tlb);
int SearchTLB(u_int_t page_num, tlb_t tlb, bool *is_tlb_hit, u_int_t *frame_num);

int CreatePageTable(page_table_t *page_table);
int SearchPageTable(u_int_t page_num, page_table_t * page_table, bool *is_page_hit, u_int_t *frame_num);
int pageFaultHandler(u_int_t page_num, const char * phys_mem_filename, page_table_t *page_table, tlb_t *tlb);



#endif /* MEMORYSYSTEM_H_ */
