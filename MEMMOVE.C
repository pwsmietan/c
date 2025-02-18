/*****************************************************************
 |  memmove - move in memory with attention to order and overlap
 |----------------------------------------------------------------
 |  Arguments:
 |   1) destination: char *
 |   2) source: char *
 |   3) length: int
 |  Returns: none
 ****************************************************************/

void memmove (to, from, length)
    char *to, *from;
    int  length;
{
    register char *TO, *FROM;

    if (to < from) {
    /* move left to right */
	TO = to;
	FROM = from;
	while (length--)
	    *(TO++) = *(FROM++);
    }
    else {
    /* move right to left */
	TO = to + length - 1;
	FROM = from + length - 1;
	while (length--)
	    *(TO--) = *(FROM--);
    }
}
