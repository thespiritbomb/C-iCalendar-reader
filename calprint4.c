/*
 * calprint4.c
 *
 * Starter file provided to students for Assignment #4, SENG 265,
 * Summer 2019.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "ics.h"
#include "listy.h"


void print_event(node_t *n, void *arg) {
    assert(n != NULL);

    event_t *event = n->val;

    if (strcmp(event->rrule, "") == 0) {
        printf("EVENT: %s %s '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location);
    } else {
        printf("EVENT: %s %s '%.10s' '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location,
            event->rrule);
    }
}

node_t *createEventsLL(char *filename){
	FILE *ics;
	char buffer[BUFLEN];
	event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;
    bool calendarEnd;
	ics=fopen(filename,"r");
	
	if(ics==NULL){
		printf("File not found");
		exit(0);
	}
	
	while (fgets(buffer, sizeof(char) * BUFLEN, ics)) {
		if(strncmp(buffer,"BEGIN:VCALENDAR",14)==0){
			calendarEnd=0;
		}
		else if(strncmp(buffer,"END:VCALENDAR",12)==0){
			calendarEnd=1;
		}
		else if(strncmp(buffer,"BEGIN:VEVENT",11)==0){
			temp_event = emalloc(sizeof(event_t));
			temp_event->rrule[0] = '\0';
		}
		else if(strncmp(buffer,"END:VEVENT",9)==0){
			if(head!=NULL){
				temp_node = new_node(temp_event);
    			head = add_front(head, temp_node);
    		}
    		else{
    			head=new_node(temp_event);
    		}
		}
		else if(starts_with(buffer,"DTSTART:")){
			buffer[strcspn(buffer, "\n")] = '\0';
			strncpy(temp_event->dtstart,buffer+8, 17);
		}
		else if(starts_with(buffer,"DTEND:")){
			buffer[strcspn(buffer, "\n")] = '\0';
			strncpy(temp_event->dtend,buffer+6, 17);
		}
		else if(starts_with(buffer,"RRULE:")){
			buffer[strcspn(buffer, "\n")] = '\0';
			strncpy(temp_event->rrule, buffer+32, 15);
		}  
		else if(starts_with(buffer,"LOCATION:")){
			buffer[strcspn(buffer, "\n")] = '\0';
			strncpy(temp_event->location, buffer+9, LOCATION_LEN);
		}
		else if(starts_with(buffer,"SUMMARY:")){
			buffer[strcspn(buffer, "\n")] = '\0';
			strncpy(temp_event->summary, buffer+8, SUMMARY_LEN);
		}
	}

	fclose(ics);
	return head;
}
#ifdef DEBUG

/*
 * Just showing the use of the linked-list routines.
 */

void _demo() {
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;

    /* Add one event, without an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190706T120000", 17);
    strncpy(temp_event->dtend, "20190706T160000", 17);
    strncpy(temp_event->summary, "summary 1", SUMMARY_LEN);
    strncpy(temp_event->location, "location 1", LOCATION_LEN);
    temp_event->rrule[0] = '\0';
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Add a second event, with an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190707T093000", 17);
    strncpy(temp_event->dtend, "20190707T102000", 17);
    strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
    strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
    strncpy(temp_event->rrule, "yada RULE yada UNTIL yada", RRULE_LEN);
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Print the list of events. */

    apply(head, print_event, NULL);

    /* Free up the memory. This is done rather deliberately
     * and manually. A full-featured function might better
     * serve for this. Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     */

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);
   
    assert(head == NULL); 
    
}

#endif



int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 7) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--end=", 5) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s --start=dd/mm/yyyy --end=dd/mm/yyyy --file=icsfile\n",
            argv[0]);
        exit(1);
    }


/* 
 * Showing some simple usage of the linked-list routines.
 */

#ifdef DEBUG
    _demo();
#endif

    exit(0);
}
