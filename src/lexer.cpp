
#include "editor.h"
#include "lexer.fdh"

#include "keywords_constant.h"

LTree *LTIdent;
char is_seperator[256];

inline char IsNumeric(char ch)
	{	return (ch >= '0' && ch <= '9');	}

/*
void c------------------------------() {}
*/

// return the next char and advance the read cursor
char next_char(LexInstance *li)
{
char ch;

	if (li->curchar > li->line_end)
	{
		li->curchar++;
		return 0;
	}

	ch = *(li->curchar++);

	if (li->curchar == li->gap_start)
		li->curchar = (li->gap_end + 1);

	return ch;
}

// decrement the read cursor
void back_char(LexInstance *li)
{
	li->curchar--;

	if (li->curchar == li->gap_end)
		li->curchar = (li->gap_start - 1);

	if (li->curchar < li->line)
		li->curchar = li->line;
}

// returns the index of the next character which would be read
int get_index(LexInstance *li)
{
char *i;

	i = li->curchar;
	if (i >= li->gap_start)
		i -= (li->gap_end - li->gap_start) + 1;

	return (i - li->line);
}

// sets the index of the next character which will be read
void set_index(LexInstance *li, int index)
{
	li->curchar = (li->line + index);

	if (li->curchar >= li->gap_start)
		li->curchar += (li->gap_end - li->gap_start);
}

/*
void c------------------------------() {}
*/

void lexer_update_line(clLine *line)
{
	int entry_state = line->prev ? line->prev->lexresult.exitstate : 0;
	lexer_parse_line(line, entry_state);
}

void lexer_parse_line(clLine *line, int entry_state)
{
LexInstance li;

	InitLexInstance(&li, line);
	line->lexresult.exitstate = ParseLine(&li, entry_state);
	//DumpLexResult(&line->lexresult);
}

/*
void c------------------------------() {}
*/

void InitLexInstance(LexInstance *li, clLine *line)
{
LexResult *output;

	// initilize line pointers
	li->line = line->text;
	li->line_end = (line->text + (line->length - 1));

	li->gap_start = (line->text + line->gap_start);
	li->gap_end = (line->text + line->gap_end);

	if (line->gap_start == 0)
		li->curchar = (li->gap_end + 1);
	else
		li->curchar = li->line;

	// initilize LexResult output buffer
	output = &line->lexresult;
	li->output = output;

	FreeLexResult(output);

	output->points = (LexPoint *)smal(128 * sizeof(LexPoint));
	output->alloc_size = 128;
}

void FreeLexResult(LexResult *lr)
{
	if (lr->points) frees(lr->points);
	memset(lr, 0, sizeof(*lr));
}

/*
void c------------------------------() {}
*/

// split the input into a series of WORDS, terminated by SEPERATING CHARS.
// then determine what each word is.
// returns: exit state
static int ParseLine(LexInstance *li, int entry_state)
{
uchar ch;
int word_start, word_end;
char word[2048];
char *word_ptr = word;
int wtype;
char have_chars = 0;
char have_non_blank = 0;

	word_start = 0;

	// block-comment carryover from previous line
	if (entry_state != 0)
	{
		AddLexPoint(li, 0, COLOR_CMT_BLOCK);

		word_start = FindEndOfBlockComment(li, entry_state);
		if (word_start < 0)
		{
			return -word_start;
		}

		AddLexPoint(li, word_start, COLOR_NORMAL);
		set_index(li, word_start);
	}

	rept
	{
		ch = next_char(li);
		//stat("read char %d/'%c'", ch);

		if (is_seperator[ch])
		{
			word_end = get_index(li);

			// identify the word and mark it with a special color if appropriate
			if (have_chars)
			{
				*word_ptr = 0;
				wtype = identify_word(word);

				if (wtype != COLOR_NORMAL)
				{
					AddLexPoint(li, word_start, wtype);
					AddLexPoint(li, (word_end - 1), COLOR_NORMAL);
				}
			}

			// check for some special chars
			switch(ch)
			{
				// comment opens
				case '/':
				{
					ch = next_char(li);

					if (ch == '/')
					{
						AddLexPoint(li, get_index(li)-2, COLOR_CMT_LINE);
						return 0;	// rest of line is taken up by comment
					}
					else if (ch == '*')
					{
						AddLexPoint(li, get_index(li)-2, COLOR_CMT_BLOCK);

						word_end = FindEndOfBlockComment(li, 1);
						if (word_end >= 0)	// comment ended
						{
							AddLexPoint(li, word_end, COLOR_NORMAL);
						}
						else
						{	// block comment stayed open at end of line.
							// return the nest level as the exit state
							return -word_end;
						}
					}
					else
					{
						back_char(li);
						goto mark_operator;
					}
				}
				break;

				// line-continuation
				case '\\':
					AddLexPoint(li, word_end - 1, COLOR_NUMBER);
					AddLexPoint(li, word_end, COLOR_NORMAL);
				break;

				// PP defines
				case '#':
				{
					if (!have_non_blank)
					{
						word_end = FindEndOfPPDefine(li);
						AddLexPoint(li, word_start, COLOR_PP);
						AddLexPoint(li, word_end, COLOR_NORMAL);
					}
				}
				break;

				// braces
				case '{': case '}':
				case '[': case ']':
				case '(': case ')':
					AddLexPoint(li, word_end - 1, COLOR_BRACE);
					AddLexPoint(li, word_end, COLOR_NORMAL);
				break;

				// operators
				case '=': case ',':
				case '.': case '>':
				case '*': case '<':
				case '+': case '-':
				case '&': case '|':
				case ';': case '!':
				case ':': case '~':
				case '%':
mark_operator: ;	// jump from "/*" comment detector ("/" operator)
					AddLexPoint(li, word_end - 1, COLOR_OPERATOR);
					AddLexPoint(li, word_end, COLOR_NORMAL);
				break;

				// string literals
				case '\"':
				case '\'':
				{
					int string_start = (word_end - 1);

					word_end = FindEndOfString(li, ch);
					if (word_end == -1)
					{
						AddLexPoint(li, string_start, COLOR_BROKEN_STRING);
					}
					else
					{
						AddLexPoint(li, string_start, (ch=='\"') ? COLOR_STRING:COLOR_SINGLE_STRING);
						AddLexPoint(li, word_end, COLOR_NORMAL);
					}
				}
				break;

				case 0: return 0;	// end of line, stop
			}

			if (ch != 9 && ch != ' ')
				have_non_blank = 1;

			word_start = word_end;
			word_ptr = word;
			have_chars = 0;
		}
		else
		{
			*(word_ptr++) = ch;
			have_chars = 1;
		}
	}

	return 0;
}

// reads chars from a string literal until it finds the end of a string "quotetype".
// if the string doesn't end before EOL, returns -1.
int FindEndOfString(LexInstance *li, char quotetype)
{
uchar ch;

	rept
	{
		switch(ch = next_char(li))
		{
			case 0: return -1;	// EOL
			case '\\': 			// escape char
				if (!next_char(li)) return -1;
			break;
		}

		if (ch == quotetype)
		{
			return get_index(li);
		}
	}
}


// reads chars until it finds the end of a preprocessor define.
// this is defined as:
//	- the end of the line,
//	- or the start of a comment
int FindEndOfPPDefine(LexInstance *li)
{
uchar ch;

	rept
	{
		switch(next_char(li))
		{
			case '/':
				ch = next_char(li);
				if (ch == '/' || ch == '*')
				{
					back_char(li);
					back_char(li);
					goto done;
				}
			break;

			case 0:
				back_char(li);
				goto done;
		}
	}

	done:	return get_index(li);
}

// reads chars until it finds the end of a block comment.
//
// entry_nesting is the initial nest level of the block comment.
// if the end is not found before the end of the line, returns a negative
// value which is the negation of the nest level.
int FindEndOfBlockComment(LexInstance *li, int entry_nesting)
{
int nest_level = entry_nesting;

	rept
	{
		switch(next_char(li))
		{
			/*case '/':	// check for another block comment opening
				if (next_char(li) == '*')
				{
					nest_level++;
				}
				else
				{
					back_char(li);
				}
			break;*/

			case '*':	// check for end of block comment
				if (next_char(li) == '/')
				{
					//if (nest_level <= 1)
					{
						return get_index(li);
					}
					/*else
					{
						nest_level--;
					}*/
				}
				else
				{
					back_char(li);
				}
			break;

			case 0:		// block comment did not end before line did
				return -nest_level;
			break;
		}
	}
}



int identify_word(char *word)
{
	//stat("identify_word... '%s'", word);

	// check if it is a number
	if (IsNumeric(*word))
	{
		return check_number(word);
	}

	// check for identifiers, comments, etc
	return check_identifier(word);
}


int check_identifier(char *word)
{
LTNode *node = LTIdent;
unsigned char ch;

	rept
	{
		ch = *(word++);

		if (node->branches[ch])
		{
			node = node->branches[ch];
		}
		else	// there is never a branch for '\0' so this catches end-of-word as well
		{
			// hit end of node tree: are we at a terminator?
			// if so we have found an identifier
			if (!ch && node->terminator_type)
			{
				return node->terminator_type;
			}

			return COLOR_NORMAL;
		}
	}
}


int check_number(char *word)
{
char hex_enable = 0;
char ch;

	// read first char of number
	ch = *(word++);

	if (ch == '0')
	{	// check for hex
		ch = *word;
		if (ch == 'x' || ch == 'X')
		{
			hex_enable = 1;
			word++;
		}
		else if (ch && !IsNumeric(ch))
		{
			if (ch == 'b')
			{
				word++;
			}
			else
			{
				return COLOR_NORMAL;
			}
		}
	}
	else if (ch > '9' || ch < '0')
	{	// first char not numeric
		return COLOR_NORMAL;
	}

	// ensure that all remaining chars of word are numeric
	rept
	{
		ch = *(word++);
		if (!ch) return COLOR_NUMBER;

		if (!IsNumeric(ch))
		{
			if (hex_enable)
			{
				if (ch >= 'a' && ch <= 'f') continue;
				if (ch >= 'A' && ch <= 'F') continue;
				if (ch == 'l' && *word == 0) continue;	// e.g. "0x80000000l"
			}
			else
			{
				if (ch == 'f' && *word == 0) continue;	// eg. "1.50f"
				if (ch == '.') continue;
			}

			return COLOR_NORMAL;
		}
	}
}

/*
void c------------------------------() {}
*/

void AddLexPoint(LexInstance *li, int index, int type)
{
LexPoint *lp;

	if (li->output->npoints >= li->output->alloc_size)
	{
		li->output->alloc_size += 128;
		li->output->points = (LexPoint *)resmal(li->output->points,
											li->output->alloc_size * sizeof(LexPoint));
	}

	lp = &li->output->points[li->output->npoints - 1];

	if (!li->output->npoints || lp->index != index)
		lp = &li->output->points[li->output->npoints++];

	lp->index = index;
	lp->type = type;
	//stat("  LexPoint #%d added: type %d at index %d", li->output->npoints-1, type, index);
}

void lexeme_add(const char *str, int color)
{
int i;
uchar ch;
LTree *tree = LTIdent;

//stat("lexeme_add: '%s'", str);
	for(i=0;;i++)
	{
		ch = str[i];

		if (!ch)
		{
			tree->terminator_type = color;
			return;
		}

		if (!tree->branches[ch])
		{
			tree->branches[ch] = CreateLTNode();
			if (ch < tree->minbranch) tree->minbranch = ch;
			if (ch > tree->maxbranch) tree->maxbranch = ch;
		}

		tree = tree->branches[ch];
	}
}

LTNode *CreateLTNode()
{
LTNode *t = (LTNode *)smalz(sizeof(LTNode));

	t->minbranch = 255;
	return t;
}

void FreeLTNode(LTNode *t)
{
int i;

	for(i=t->minbranch;i<=t->maxbranch;i++)
	{
		if (t->branches[i]) FreeLTNode(t->branches[i]);
	}

	frees(t);
}

/*
void c------------------------------() {}
*/

void lexer_init()
{
int i;

	// initilize identifier tables
	LTIdent = CreateLTNode();
	lexeme_add("int", COLOR_IDENTIFIER);
	lexeme_add("short", COLOR_IDENTIFIER);
	lexeme_add("char", COLOR_IDENTIFIER);
	lexeme_add("long", COLOR_IDENTIFIER);
	lexeme_add("bool", COLOR_IDENTIFIER);
	lexeme_add("uchar", COLOR_IDENTIFIER);
	lexeme_add("uint", COLOR_IDENTIFIER);
	lexeme_add("ushort", COLOR_IDENTIFIER);
	lexeme_add("uint8", COLOR_IDENTIFIER);
	lexeme_add("uint16", COLOR_IDENTIFIER);
	lexeme_add("uint32", COLOR_IDENTIFIER);
	lexeme_add("int8", COLOR_IDENTIFIER);
	lexeme_add("int16", COLOR_IDENTIFIER);
	lexeme_add("int32", COLOR_IDENTIFIER);
	lexeme_add("void", COLOR_IDENTIFIER);
	lexeme_add("static", COLOR_IDENTIFIER);
	lexeme_add("struct", COLOR_IDENTIFIER);
	lexeme_add("class", COLOR_IDENTIFIER);
	lexeme_add("union", COLOR_IDENTIFIER);
	lexeme_add("enum", COLOR_IDENTIFIER);
	lexeme_add("double", COLOR_IDENTIFIER);
	lexeme_add("float", COLOR_IDENTIFIER);
	lexeme_add("signed", COLOR_IDENTIFIER);
	lexeme_add("unsigned", COLOR_IDENTIFIER);
	lexeme_add("extern", COLOR_IDENTIFIER);
	lexeme_add("const", COLOR_IDENTIFIER);
	lexeme_add("virtual", COLOR_IDENTIFIER);
	lexeme_add("try", COLOR_IDENTIFIER);
	lexeme_add("catch", COLOR_IDENTIFIER);
	lexeme_add("inline", COLOR_IDENTIFIER);
	lexeme_add("true", COLOR_IDENTIFIER);
	lexeme_add("false", COLOR_IDENTIFIER);

	load_builtin_keywords(keywords_constant, COLOR_SYSTEM_CONSTANT);
	//load_keywords("keywords_constant", COLOR_SYSTEM_CONSTANT);

	//lexeme_add(LTIdent, "public");
	//lexeme_add(LTIdent, "private");

	// initilize word-seperator table
	memset(is_seperator, 1, sizeof(is_seperator));
	for(i='A';i<='Z';i++) is_seperator[i] = 0;
	for(i='a';i<='z';i++) is_seperator[i] = 0;
	for(i='0';i<='9';i++) is_seperator[i] = 0;
	is_seperator['_'] = 0;
}

void lexer_close()
{
	FreeLTNode(LTIdent);
}

void load_builtin_keywords(const char *array[], int color)
{
int i;

	for(i=0;array[i];i++)
		lexeme_add(array[i], color);
}

char load_keywords(char *fname, int color)
{
FILE *fp;
char line[256];

	fp = fopen(fname, "rb");
	if (!fp) return 1;

	while(!feof(fp))
	{
		fgetline(fp, line, sizeof(line));
		if (!line[0]) continue;

		lexeme_add(line, color);
	}

	fclose(fp);
	return 0;
}

/*
void c------------------------------() {}
*/

/*
void lexer_test(char *line)
{
clLine ln;

	//line = "public int j = 37;";

	memset(&ln, 0, sizeof(ln));
	ln.gap_start = ln.gap_end = 20000;
	ln.text = line;
	ln.length = strlen(line);

	//ln.gap_start = 0;
	//ln.gap_end = 0;

	lexer_parse_line(&ln, 0);
	testdraw(&ln, &ln.lexresult);
	stat("exit state %d", ln.lexresult.exitstate);
	DumpLexResult(&ln.lexresult);

	getch();
}

void DumpLexResult(LexResult *lr)
{
int i;

	stat("");
	for(i=0;i<lr->npoints;i++)
	{
		stat("$%02x: change to color %d", lr->points[i].index, lr->points[i].type);
	}
	stat("exit state %d", lr->exitstate);
}

void testdraw(clLine *line, LexResult *lr)
{
int i, index;
int p = 0;

	conshow(1);
	gotoxy(0, 0);

	index = i = 0;
	rept
	{
		if (i == line->gap_start)
			i = line->gap_end + 1;
		if (i >= line->length)
			break;

		if (index == lr->points[p].index && p < lr->npoints)
		{
			textbg(0);

			switch(lr->points[p].type)
			{
				case COLOR_NORMAL: textcolor(7); break;
				case COLOR_IDENTIFIER: textcolor(14); textbg(1); break;
				case COLOR_PP: textcolor(6); break;
				case COLOR_NUMBER: textcolor(12); break;
				case COLOR_CMT_LINE: textcolor(3); break;
				case COLOR_CMT_BLOCK: textcolor(2); break;
				case COLOR_STRING_DOUBLE: textcolor(10); break;
				case COLOR_STRING_SINGLE: textcolor(9); break;
				case COLOR_BROKEN_STRING: textcolor(4); break;
				default: textcolor(12); textbg(10); break;
			}

			p++;
		}

		putch(line->text[i]);
		i++;
		index++;
	}

	stat("");
	textcolor(7);
	textbg(0);
}
*/
