/*
 *	diff.c ->	Source file comparator
 *
 *	Conforms to conventions of standard Unix library with the exception
 *	 of movmem()
 */

 /*
 To paraphrase the Unix (trademark Bell Laboratories) manual:

 NAME
	  diff - differential file comparator

 SYNOPSIS
	  diff file1 file2

 DESCRIPTION
	  Diff  tells  which  lines differ between  two  files.   Diff
	  outputs  to the standard output all affected lines from  the
	  first file preceeded by '<',  a line containing three dashes
	  ("---"),  then  all  affected  lines from  the  second  file
	  preceeded by '>'.

	  Diff maintains a "synchronization window" of 50  lines,  and
	  generates an error message if the input files differ by more
	  than 50 lines.   Error messages are written to the  standard
	  error output.

	  Diff ignores all carriage-return characters (0x0D),  so that
	  a	missing  carriage-return is not considered to be  a  dif-
	  ference.	For  backward compatability,  diff does recognize
	  control-Z (0x1A) as the end-of-file marker.

 DIAGNOSTICS
	  Exit  status  is 0 for no differences,  1 for  some,  2  for
	  trouble.
  */

#include <stdio.h>
#include <fcntl.h>

#define EOFCHR		0x1A
#define MAXLINES	50
#define BUFFSIZ 	(MAXLINES * 128)

typedef struct
{
	char	*lptr;		/*  -> text line in memory	*/
	int	lhash;		/*  hash of line's contents     */
}
LBUF;

typedef struct
{
	int	fd;		/*  file descriptor		*/
	char	*eptr,		/*  ptrs into buffer		*/
		*mptr;
	char	buff[BUFFSIZ];	/*  the buffer			*/
	LBUF	line[MAXLINES]; /*  the lines			*/
}
FBUF;

FBUF	*f1,			/*  the input files		*/
*f2;

int	differences = 0;	/*  found differences ??	*/

extern char	*malloc();

/*
 *	MatchLines() -> compare two lines for a match
 */

int MatchLines(int lev1, int lev2) {
	return ((f1->line[lev1].lhash == f2->line[lev2].lhash) && !strcmp(f1->line[lev1].lptr, f2->line[lev2].lptr));
}

/*
 *	Fatal() ->	fatal error
 */

Fatal(char *mesg) {
	fprintf(stderr, "diff : ");
	fprintf(stderr, mesg);
	fprintf(stderr, "\r\n");
	exit(2);
}

/*
 *	ReadMore() ->	read in more lines, preserving lines in use
 */

char *ReadMore(FBUF *fdesc, int last) {
	LBUF	*slin, *elin;
	int	size, dist, ramt;

	size = fdesc->mptr - fdesc->line[0].lptr;

	/*  move text to beginning of buffer  */
	movmem(fdesc->line[0].lptr, fdesc->buff, size);
	slin = fdesc->line;
	elin = &fdesc->line[last];
	dist = fdesc->line[0].lptr - fdesc->buff;

	/*  fix up existing line pointers  */
	while(slin <= elin) {
		slin->lptr -= dist;
		++slin;
	}

	/*  read in some more  */
	ramt = read(fdesc->fd, fdesc->mptr - dist, size = (BUFFSIZ - size));
	if(ramt < 0)
		Fatal("file read error");

	fdesc->mptr += (ramt - dist);

	/*  check for EOF  */
	if(ramt < size)
		*fdesc->mptr = EOFCHR;

	return (fdesc->mptr - ramt);
}

/*
 *	HashMore() ->	read/hash a number of lines of text
 */

HashMore(int file, int amt) {
	FBUF	*fdesc;
	int	thash;
	char	*cptr;

	for(fdesc = (file ? f2 : f1); amt > 0; --amt) {
		fdesc->line[MAXLINES - amt].lptr = cptr = fdesc->eptr;

		/*  check for EOF  */
		if(!cptr)
			fdesc->line[MAXLINES - amt].lhash = 0;
		else {
			/*  calculate hash value  */
			thash = 0;
			for(;;) {
				/*  don't use up the buffer  */
				if(cptr >= fdesc->mptr)
					cptr = ReadMore(fdesc, MAXLINES - amt);

				if((*cptr == EOFCHR) || (*cptr == '\n'))
					break;

				if(*cptr == '\r')      /*  ignore CRs  */
					++cptr;
				else
					thash += *cptr++;
			}

			/*  terminate line correctly  */
			if(*cptr == EOFCHR) {
				*cptr = 0;
				cptr = NULL;
			}
			else
				*cptr++ = 0;

			/*  save the hash value  */
			fdesc->line[MAXLINES - amt].lhash = thash;
		}

		fdesc->eptr = cptr;
	}
}

/*
 *	MoveDown() ->	move down a number of lines in each file
 */

MoveDown(int amt1, int amt2) {
	movmem(&f1->line[amt1], &f1->line[0],
		sizeof(LBUF) * (MAXLINES - amt1));
	HashMore(0, amt1);

	movmem(&f2->line[amt2], &f2->line[0],
		sizeof(LBUF) * (MAXLINES - amt2));
	HashMore(1, amt2);
}

/*
 *	PrintLine() ->	print the specified line
 */

PrintLine(int file, int lev) {
	FBUF	*fdesc;

	fdesc = (file ? f2 : f1);

	printf("%c %s\r\n", (file ? '>' : '<'), fdesc->line[lev].lptr);
}

/*
 *	AtEOF() ->	check for both files at EOF
 */

int AtEOF() {
	return (!f1->line[0].lptr && !f2->line[0].lptr);
}

/*
 *	CloseUp() ->	close the files
 */

CloseUp() {
	close(f1->fd);
	free(f1);
	close(f2->fd);
	free(f2);
}

/*
 *	CmpRegion() ->	compare a region
 */

void CmpRegion() {
	int	depth, level, tmp;

	/*  move over matching portion	*/
	while(!AtEOF() && MatchLines(0, 0))
		MoveDown(1, 1);

	if(AtEOF())
		return;
	else
		differences = 1;

	/*  for each line in each file, compare it and next line with
	 *	every previous line in the other file, looking for a
	 *	two line match
	 */
	depth = level = 0;

	for(;;) {
		if(depth >= (MAXLINES - 1)) {
			char msg[100];
			sprintf(msg, "files differ by more than %d lines", MAXLINES);
			Fatal(msg);
		}

		if(MatchLines(level, depth) && MatchLines(level + 1, depth + 1))
			break;
		if(MatchLines(depth, level) && MatchLines(depth + 1, level + 1))
			break;

		if(level < depth)
			++level;
		else {
			++depth;
			level = 0;
		}
	}

	/*  make tmp be the length in the first file  */
	if(MatchLines(level, depth) && MatchLines(level + 1, depth + 1)) {
		tmp = level;
		level = depth;
		depth = tmp;
	}

	/*  print the region of difference  */
	tmp = 0;
	while(tmp < depth)
		PrintLine(0, tmp++);
	printf("---\r\n");
	tmp = 0;
	while(tmp < level)
		PrintLine(1, tmp++);

	MoveDown(depth, level);
}

/*
 *	Init() ->	initialize the input files
 */

Init(char *name1, char *name2) {
	char msg[100];

	if(!(f1 = (FBUF *)malloc(sizeof(FBUF))))
		Fatal("insufficient memory");

	if(!(f2 = (FBUF *)malloc(sizeof(FBUF))))
		Fatal("insufficient memory");



	if((f1->fd = open(name1, O_RDWR)) < 0) {
		sprintf(msg, "Unable to open: %s", name1);
		Fatal(msg);
	}

	if((f2->fd = open(name2, O_RDWR)) < 0) {
		sprintf(msg, "Unable to open: %s", name2);
		Fatal(msg);
	}

	f1->mptr = f1->eptr = f1->line[0].lptr = f1->buff;
	f2->mptr = f2->eptr = f2->line[0].lptr = f2->buff;
	HashMore(0, MAXLINES);
	HashMore(1, MAXLINES);
}

/*
 *	good old main
 */

int main(argc, argv)
int	argc;
char	*argv[];
{
	if(argc != 3)
		Fatal("usage: diff file1 file2");

	Init(argv[1], argv[2]);

	while(!AtEOF())
		CmpRegion();

	CloseUp();

	return (differences);
}


/*
 *	END of diff.c
 */
