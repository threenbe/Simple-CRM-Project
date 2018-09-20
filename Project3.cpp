/* 
 * EE312 Simple CRM Project
 * Name: Raiyan Chowdhury
 * EE312 Spring 2016
 * CRM
 */

#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS];
int num_customers = 0;

Customer catalog = {0};

/* you'll probably need several more global variables */
/* and of course, you have a few functions to write */


/************************reset************************
* clear the inventory and reset the customer database to empty 
* [INPUTS] none
* [OUTPUTs] none
*/
void reset(void) {
    for (int i = 0; i < num_customers; i++) {
        StringDestroy(&customers[i].name);
        customers[i].rattles = customers[i].bottles = customers[i].diapers = 0;
    }
    catalog.bottles = catalog.rattles = catalog.diapers = 0;
    num_customers = 0;
}

/************************updateCustomers************************
* This function goes through the customer array to find the name
* of whoever made the last valid purchase in order to update their
* purchase history. If the name isn't found, a new entry for the customer
* is created.
* [INPUTS] name of the customer so that customer array can be updated with name,
* the item type so that the function knows which item the customer purchased, and
* the number of items that the customer purchased 
* [OUTPUTS] none
*/
void updateCustomers(String name, String item, int num) {
    String bottles = StringCreate("Bottles");
    String rattles = StringCreate("Rattles");
    String diapers = StringCreate("Diapers");
    int exists = 0;//flag used to see if customer was found in array or not

    //searches for customer name in customers array, creates new entry if not found
    for (int i = 0; i < num_customers; i++) {
        if (StringIsEqualTo(&name, &customers[i].name)) {

            if (StringIsEqualTo(&item, &bottles)) {
                customers[i].bottles += num; 
            }
            else if (StringIsEqualTo(&item, &rattles)) {
                customers[i].rattles += num; 
            }
            else if (StringIsEqualTo(&item, &diapers)) {
                customers[i].diapers += num; 
            }

            exists = 1;
            break;
        }
    }

    if (exists) {
        exists = 0;
    }
    else {
        customers[num_customers].name = StringDup(&name);

        if (StringIsEqualTo(&item, &bottles)) { 
        	customers[num_customers].bottles = num; 
        	customers[num_customers].rattles = customers[num_customers].diapers = 0;
        }
        else if (StringIsEqualTo(&item, &rattles)) { 
        	customers[num_customers].rattles = num; 
        	customers[num_customers].bottles = customers[num_customers].diapers = 0;
        }
        else if (StringIsEqualTo(&item, &diapers)) { 
        	customers[num_customers].diapers = num; 
        	customers[num_customers].bottles = customers[num_customers].rattles = 0; 
        }

        num_customers++;
    }

    StringDestroy(&bottles);
    StringDestroy(&rattles);
    StringDestroy(&diapers);
}

/************************mostDiapers************************
* This function is called by processSummarize and goes through
* the customers array to find out which customer bought the highest
* number of diapers. If two or more bought the highest number, then
* this function will count the customer whose data was created first.
* [INPUTS] none
* [OUTPUTS] index indicating place in customers array with the customer who bought
* the most diapers
 */
int mostDiapers() {
	int i, most = 0;
	for (i = 1; i < num_customers; i++) {
		if (customers[i].diapers > customers[most].diapers) most = i;
	}
	if (customers[most].diapers == 0) most = -1;
	return most;
}

/************************mostBottles************************
* This function is called by processSummarize. Does the same as 
* mostRattles and mostDiapers, but for bottles.
* [INPUTS] none
* [OUTPUTS] index indicating customer who bought most bottles in customers array
*/
int mostBottles() {
    int i, most = 0;
    for (i = 1; i < num_customers; i++) {
        if (customers[i].bottles > customers[most].bottles) most = i;
    }
    if (customers[most].bottles == 0) most = -1;
    return most;
}

/************************mostRattles************************
* This function is called by processSummarize. Serves the same purpose
* as mostDiapers and mostBottles, but for rattles.
* [INPUTS] none
* [OUTPUTS] index indicating customer who bought most rattles in customers array
*/
int mostRattles() {
	int i, most = 0;
	for (i = 1; i < num_customers; i++) {
		if (customers[i].rattles > customers[most].rattles) most = i;
	}
    if (customers[most].rattles == 0) most = -1;
    return most;
}

/************************processSummarize************************
* Prints out a summary of the inventory and purchases at the time it 
* was called. First prints quantity of everything left in inventory,
* then number of customers who visited the store (not counting those 
* who attempted to make an invalid purchase), and then it prints which
* customer bought the most of each item (unless no one bought a particular
* item, in which case this is indicated instead).
* [INPUTS] none
* [OUTPUTS] none
*/
void processSummarize() {
	int most_bottles, most_rattles, most_diapers;
    printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", catalog.bottles, catalog.diapers, catalog.rattles);
    printf("we have had a total of %d different customers\n", num_customers);

    most_bottles = mostBottles();
    most_rattles = mostRattles();
    most_diapers = mostDiapers();

    if (most_bottles == -1) {
    	printf("no one has purchased any Bottles\n");
    }
    else {
    	StringPrint(&customers[most_bottles].name);
    	printf(" has purchased the most Bottles (%d)\n", customers[most_bottles].bottles);
    }

    if (most_diapers == -1) {
        printf("no one has purchased any Diapers\n");
    }
    else {
    	StringPrint(&customers[most_diapers].name);
    	printf(" has purchased the most Diapers (%d)\n", customers[most_diapers].diapers);
    }

    if (most_rattles == -1) {
    	printf("no one has purchased any Rattles\n");
    }
    else {
    	StringPrint(&customers[most_rattles].name);
    	printf(" has purchased the most Rattles (%d)\n", customers[most_rattles].rattles);
    }
}

/************************processPurchase************************
* This function reads the customer's name, item type, and quantity
* of said item. It then updates the customer array appropriately 
* (updating entry for customer if it already exists, creating new 
* entry if it doesn't). Prints an error message if customer tries
* to request more than what's available in the inventory.
* [INPUTS] none
* [OUTPUTS] none
*/
void processPurchase() {
    String bottles = StringCreate("Bottles");
    String rattles = StringCreate("Rattles");
    String diapers = StringCreate("Diapers");
    String name;
    String item;
    int num;

    //read name of person purchasing, item they're purchasing, and the # of items they want
    readString(&name);
    readString(&item);
    //assert statement below makes sure item read matches correct format
    assert(StringIsEqualTo(&item, &bottles) || StringIsEqualTo(&item, &rattles) || StringIsEqualTo(&item, &diapers));
    readNum(&num);
    //assert makes sure that customer doesn't try to purchase negative # of items (which is nonsense)
    assert(num > 0);

    //update items in inventory based on purchase
    if (StringIsEqualTo(&item, &bottles)) {
        if (num <= catalog.bottles && num != 0) {
            catalog.bottles -= num;
            //general function that updates the customers array
            updateCustomers(name, item, num);
        }
        //if customer tried to purchase more than what was available in inventory, print error message
        else {
        	printf("Sorry ");
        	StringPrint(&name);
            printf(", we only have %d Bottles\n", catalog.bottles);
        }
    }

    else if (StringIsEqualTo(&item, &rattles)) {
        if (num <= catalog.rattles && num != 0) {
            catalog.rattles -= num;
            updateCustomers(name, item, num);
        }
        else {
        	printf("Sorry ");
        	StringPrint(&name);
            printf(", we only have %d Rattles\n", catalog.rattles);
        }
    }

    else if (StringIsEqualTo(&item, &diapers)) {
        if (num <= catalog.diapers && num != 0) {
            catalog.diapers -= num;
            updateCustomers(name, item, num);
        }
        else {
        	printf("Sorry ");
        	StringPrint(&name);
            printf(", we only have %d Diapers\n", catalog.diapers);
        }
    }

    StringDestroy(&bottles);
    StringDestroy(&rattles);
    StringDestroy(&diapers);
    StringDestroy(&name);
    StringDestroy(&item);
}

/************************processInventory************************
* This function reads the item type and quantity using readString 
* and readNum, respectively, and updates the inventory appropriately.
* Prints an error if an incorrect item type is given.
* [INPUTS] none
* [OUTPUTS] none
*/
void processInventory() {
    String bottles = StringCreate("Bottles");
    String rattles = StringCreate("Rattles");
    String diapers = StringCreate("Diapers");
    String next_command;
    int num;

    //read next command to check for item, then read quantity of said item
    readString(&next_command);
    //assert statement below makes sure item read matches correct format
    assert(StringIsEqualTo(&next_command, &bottles) || StringIsEqualTo(&next_command, &rattles) || StringIsEqualTo(&next_command, &diapers));
    readNum(&num);
    //assert statement below makes sure inventory quantity isn't negative
    assert(num >= 0);
    if (num == 0) return;

    //check to see which item was read to update inventory
    //print error message if invalid item type was read
    if (StringIsEqualTo(&next_command, &bottles)) {
        catalog.bottles += num;
    }
    else if (StringIsEqualTo(&next_command, &rattles)) {
        catalog.rattles += num;
    }
    else if (StringIsEqualTo(&next_command, &diapers)) {
        catalog.diapers += num;
    }

    StringDestroy(&bottles);
    StringDestroy(&rattles);
    StringDestroy(&diapers);
    StringDestroy(&next_command);
}
