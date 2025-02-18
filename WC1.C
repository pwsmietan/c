/* wc: count lines, words, chars */

#include <stdio.h>

main(argc, argv)
int   argc;
char  *argv[];
{
      int   c, i, inword;
      FILE  *fp, *fopen();
      long  linect, wordct, charct;
      long  tlinect = 0, twordct = 0, tcharct = 0;

      i = 1;
      fp = stdin;
      printf(" lines   words   chars   file\n");
      printf("======= ======= =======  =====\n");
      do {
	 if (argc > 1 && (fp = fopen(argv[i], "r")) == NULL) {
	    fprintf(stderr, "                        wc: can't open %s\n", argv[i]);
	    continue;
	 }
	 linect = wordct = charct = inword = 0;
	 while ((c = getc(fp)) != EOF) {
	    charct++;
	    if (c == '\n')
	       linect++;
	    if (c == ' ' || c == '\t' || c == '\n')
	       inword = 0;
	    else if (inword == 0) {
	       inword = 1;
	       wordct++;
	    }
	 }
	 printf("%7ld %7ld %7ld  ", linect, wordct, charct);
	 printf(argc > 1 ? "%s\n" : "\n", argv[i]);
	 fclose(fp);
	 tlinect += linect;
	 twordct += wordct;
	 tcharct += charct;
      } while (++i < argc);
      if (argc > 2)
	 printf("%7ld %7ld %7ld  total\n", tlinect, twordct, tcharct);
      exit(0);
}
