#include <stdio.h>
#include <string.h>


typedef struct list_elt_tag{
	char *name;
	int num;
	struct list_elt_tag *prev, *next; //Pointers to next and prev elements in list
}list_elt;


/*
* lookup- finds a person by name in the list, returns NULL if not found
*/

list_elt *lookup (list_elt *first_elt, char *name)
{
  list_elt *elt = first_elt;

  //Iterate down linked list
  while(elt != NULL) {
    //Compare the name using strcmp
    if (strcmp (name, elt->name) == 0)
      return elt;
    elt = elt->next;
  }
  return NULL; //If no matches found return null
}


/*
 * Creates a new element and pushes it to the front of the list
 * returns the pointer of the newly created element.
*/

list_elt *push (list_elt *first_elt, char *name, int num)
{
  list_elt *elt;

  //Allocates memory for new list element
  elt = malloc(sizeof(*elt));

  //if malloc fails => end 
  if(elt == NULL){
    printf("Could not allocate sufficient memory for new list element\n");
    exit(1);
  }
  
  //Allocates memory for name
  elt->name = malloc(strlen(name) + 1);

  //if malloc fails => end
  if(elt->name == NULL){
    printf("Could not allocated sufficient memory for new name\n");
    exit(1);
  }

  //Copy name to elt
  strcpy (elt->name, name);

  elt->num = num;

  elt->prev = NULL;
  elt->next = first_elt;

  if(first_elt != NULL)
    first_elt->prev = elt;

  //return the pointer of new element
  return elt;
}

/*
 * delete removes an element in the list and returns new first elet
 */
list_elt *delete (list_elt *first_elt, list_elt *elt)
{
  list_elt *prev, *next;

  if (elt == NULL || first_elt == NULL)
    return first_elt;
	
  prev = elt->prev;
  next = elt->next;

  //Fix the prev and next pointers for list elements
  if(prev)
    prev->next = elt->next;
  if(next)
    next->prev = elt->prev;

  //Free up space allocated for name and list element;
  free(elt->name);
  free(elt);

  //Check if it was the first element
  if (elt == first_elt)
    return next;
  else
    return first_elt;
}
/*
 * Frees any elements that have num <= 0
 */
list_elt *eradicate (list_elt *first_elt)
{
  list_elt *elt = first_elt;
  list_elt *next;
  while(elt != NULL){
    next = elt->next;
	if(elt->num < 1)
      first_elt = delete(first_elt, elt);
    elt = next;
  }
  return first_elt;
}

/*
 * Frees all space used by list
 */
list_elt *empty (list_elt *first_elt)
{
  list_elt *next, *elt;
  
  elt = first_elt;
  next = first_elt->next;
  while(next != NULL){
    free(elt->name);
    free(elt);
    elt = next;
    next = elt->next;
  }
  return NULL;
}

int main(int argc, char *argv[])
{
	int i, t, num, set = 1;
	char name[30], line[30];
	FILE *fp;
	list_elt *elt, *temp;
	
	//Opens up a file for reading
	fp = fopen(argv[1], "r");
	
	if(fp == NULL){
		printf("Couldn't open file\n");
		return 1;
	}
	
	while (fgets(line, 30, fp)) {
		sscanf(line, "%s %d", name, &num);
//		printf("%s, %d\n", name, num);
		for (i = 0; name[i] != '\0'; i++)
			name[i] = toupper(name[i]);
		if (set) {
			elt = push(NULL, name, num);			
			set = 0;
		}
		else{
			temp = lookup(elt, name);
			if (temp == NULL) {
				elt = push(elt, name, num);
//				printf("New val: %s, %d\n", elt->name, elt->num);
			}
			else {
				t = temp->num;
				num = t + num;
				temp->num = num;
			}
		}
		elt = eradicate(elt);
	}
	temp = elt;
	//Prints out all list values
	printf("Final Order:\n");
	while (temp != NULL) {
		printf("%s %d\n", temp->name, temp->num);
		temp = temp->next;
	}
	
	//Frees all space used by list
	if(elt != NULL)
		empty(elt);
	//Closes the file
	fclose(fp);
	return 0;
}
