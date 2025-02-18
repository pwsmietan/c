#include <stdio.h>

main() {
	int	EntryCount ;	/* looping variable--the entry number */
	int	NumEntries ;	/* total number of entries */
	int	StarCount  ;	/* counter of the current number of stars */
	int	Value	   ;	/* value of the specific entry */

	printf("How many entries will you make? ") ;
	scanf("%d", &NumEntries) ;
	for (EntryCount = 0 ; EntryCount < NumEntries ; EntryCount++) {
		printf("\nFor entry %d, type a value between 0 and 100: ",
			EntryCount + 1) ;
		scanf("%d", &Value) ;

		/* First, display the value, then a bar to represent the
		   base of the graph. */

		printf("%d | ", Value) ;

		/* To fit up to 100 stars on a single line, display half the
		   number of stars according to Value */

		for (StarCount = 0 ; StarCount < Value / 2 ; StarCount++)
			printf("*") ;

		printf("\n") ;
	}
}
