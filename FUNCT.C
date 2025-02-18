
/*    funct.c  `C' library functions */
/*    be sure to #include <ascii.h> in source */

strcpy(dest, src)		       /* string copy routine */
char  *dest, *src;
{
      while (*dest++ = *src++)
	 ;
}

isodigit(c)			       /* check if octal digit */
char  c;
{
      return (c >= '0' && c <= '7');
}

isxdigit(c)			       /* check if hexidecimal digit */
char  c;
{
      return (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

iscntrl(c)
char  c;
{
      return ((c >= NULL && c < SPACE) || c == DEL);
}

isprint(c)
char  c;
{
      return (c >= SPACE && c < DEL);
}

ispunct(c)
char  c;
{
      return (isascii(c) && !(isalnum(c) || iscntrl(c)));
}

iswhite(c)
char  c;
{
      return (c == SPACE || c == TAB || c == EOL || c == NL);
}

tolower(c)
char  c;
{
      return (isupper(c) ? (c | '\x20') : c);
}

toupper(c)
char  c;
{
      return (islower(c) ? (c & ~'\x20') : c);
}

char  *gets(string)
char  *string;
{
      char  *str;
      int   c;

      str = string;

      while ((c = getchar()) != EOL && c != EOF)
	 *string++ = c;
      *string = EOS;

      if ( c == EOF && str == string)
	 return (NULL);

      return (line);
}

strlen(string)
char  *string;
{
      int   length = 0;

      while (*string++)
	 ++length;

      return (length);
}

getstr(string)
char  *string;
{
      int   c, length = 0;

      while ((c = getchar()) != EOL && c != EOF) {
	 *string++ = c;
	 ++length;
      }
      *string = EOS;

      return (c == EOF ? (-1 * length) : length);
}

char  *stolower(string)
char  *string;
{
      char  *str;

      str = string;
      while (*string = tolower(*string))
	 string++;

      return (str);
}

char  *stoupper(string)
char  *string;
{
      char  *str;

      str = string;
      while (*string = toupper(*string))
	 string++;

      return (str);
}

char  *strcat(string1, string2)
char  *string1, *string2;
{
      char  *ptr = string1;

      while (*string1)
	 ++string1;
      while (*string1++ = *string2++)
	 ;

      return (ptr);
}

strcmp(string1, string2)
char  *string1, *string2;
{
      while (*string1 == *string2) {
	 if (!*string1)
	    return (0);
	 ++string1;
	 ++string2;
      }

      if (*string1 < *string2)
	 return (-1);
      else
	 return (1);
 }
