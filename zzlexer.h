#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>
#include "token.h"

/* Tags are defined here */
const int
UNKNOWN = 256,
CREATE = 257, TABLE = 258, INT = 259,
DEFAULT = 260, PRIMARY = 261, KEY = 262,
INSERT = 263, INTO = 264, VALUES = 265,
DELETE = 266, FROM = 267, WHERE = 268,
SELECT = 269, NEQ = 270, EQ = 271,
LT = 272, GT = 273, LEQ = 274,
GEQ = 275, PLUS = 276, MINUS = 277,
STAR = 278, DIV = 279, AND = 280,
OR = 281, NOT = 282, ASN = 283,
ID = 284, NUM = 285, EEOS = 286,
COMMA = 287, LP = 288, RP = 289,
EEOF = 290;

/* */

class ZZlexer
{
private:
	int __zzline;
	int __zzcol;

	std::map< std::string, int > __words;   // store words
	char __peek;                            // store next character read
	bool __end;                             // flag check eof
	bool __fromFile;

	std::fstream fs;

public:
	int zzline() {
		return __zzline;
	}

	int zzcol() {
		return __zzcol;
	}

	void reserve(Token w) {
		__words.insert(std::pair< std::string, int >(w.lexeme, w.tag));
	}

	void readFromFile(const char* fn) {
		fs.open(fn, std::fstream::in);
		if (fs.is_open())
			__fromFile = true;
	}

	ZZlexer() {
		Token _create(CREATE, "CREATE");
		reserve(_create);
		Token _table(TABLE, "TABLE");
		reserve(_table);
		Token _int(INT, "INT");
		reserve(_int);
		Token _default(DEFAULT, "DEFAULT");
		reserve(_default);
		Token _primary(PRIMARY, "PRIMARY");
		reserve(_primary);
		Token _key(KEY, "KEY");
		reserve(_key);
		Token _insert(INSERT, "INSERT");
		reserve(_insert);
		Token _into(INTO, "INTO");
		reserve(_into);
		Token _values(VALUES, "VALUES");
		reserve(_values);
		Token _delete(DELETE, "DELETE");
		reserve(_delete);
		Token _from(FROM, "FROM");
		reserve(_from);
		Token _where(WHERE, "WHERE");
		reserve(_where);
		Token _select(SELECT, "SELECT");
		reserve(_select);
		__zzline = 1;
		__zzcol = 0;
		__end = false;
		__fromFile = false;

	}

	~ZZlexer() {
		if (fs.is_open())
			fs.close();
	}

	void peekNext() {
		__peek = __fromFile ? fs.peek() : std::cin.peek();
	}

	bool peekNext(char c) {
		peekNext();
		return (c == __peek);
	}

	void moveNext() {
		char c;
		__fromFile ? fs.get(c) : std::cin.get(c);
		__end = (fs.eof()) || __fromFile == false && c == '\n';
		__zzcol += 1;
	}

	Token scan() {
		while (__end == false) {
			peekNext();
			moveNext();

			// newline
			if (__peek == '\n') {
				__zzline += 1;
				__zzcol = 0;
				continue;
			}

			if (!(__peek == ' ' || __peek == '\t')) {

				// deal with operators
				switch (__peek) {
				case '<':
				{
					if (peekNext('>')) {
						moveNext();
						Token neq(NEQ, "<>");
						neq.setLocation(__zzline, __zzcol);
						return neq;
					}
					else if (peekNext('=')) {
						moveNext();
						Token leq(LEQ, "<=");
						leq.setLocation(__zzline, __zzcol);
						return leq;
					}
					else {
						Token lt(LT, "<");
						lt.setLocation(__zzline, __zzcol);
						return lt;
					}
				}
				case '>':
				{
					if (peekNext('=')) {
						moveNext();
						Token geq(GEQ, ">=");
						geq.setLocation(__zzline, __zzcol);
						return geq;
					}
					else {
						Token gt(GT, ">");
						gt.setLocation(__zzline, __zzcol);
						return gt;
					}
				}
				case '=':
				{
					if (peekNext('=')) {
						moveNext();
						Token eq(EQ, "==");
						eq.setLocation(__zzline, __zzcol);
						return eq;
					}
					else {
						Token asn(ASN, "=");
						asn.setLocation(__zzline, __zzcol);
						return asn;
					}
				}
				case '+':
				{
					Token plus(PLUS, "+");
					plus.setLocation(__zzline, __zzcol);
					return plus;
				}
				case '-':
				{
					Token minus(MINUS, "-");
					minus.setLocation(__zzline, __zzcol);
					return minus;
				}
				case '*':
				{
					Token star(STAR, "*");
					star.setLocation(__zzline, __zzcol);
					return star;
				}
				case '/':
				{
					Token div(DIV, "/");
					div.setLocation(__zzline, __zzcol);
					return div;
				}
				case '&':
				{
					if (peekNext('&')) {
						moveNext();
						Token aand(AND, "&&");
						aand.setLocation(__zzline, __zzcol);
						return aand;
					}
					else {
						Token u('&');
						u.setLocation(__zzline, __zzcol);
						return u;
					}
				}
				case '|':
				{
					if (peekNext('|')) {
						moveNext();
						Token oor(OR, "||");
						oor.setLocation(__zzline, __zzcol);
						return oor;
					}
					else {
						Token u('|');
						u.setLocation(__zzline, __zzcol);
						return u;
					}
				}
				case '!':
				{
					Token nnot(NOT, "!");
					nnot.setLocation(__zzline, __zzcol);
					return nnot;
				}
				case '(':
				{
					Token lp(LP, "(");
					lp.setLocation(__zzline, __zzcol);
					return lp;
				}
				case ')':
				{
					Token rp(RP, ")");
					rp.setLocation(__zzline, __zzcol);
					return rp;
				}
				case ';':
				{
					Token eeos(EEOS, ";");
					eeos.setLocation(__zzline, __zzcol);
					return eeos;
				}
				case ',':
				{
					Token comma(COMMA, ",");
					comma.setLocation(__zzline, __zzcol);
					return comma;
				}
				default:
					;

				}

				// deal with numbers
				if (isdigit(__peek)) {
					int num = 0;

					do {
						num = 10 * num + (int)(__peek - '0');
						peekNext();
						if (isdigit(__peek))
							moveNext();
					} while (isdigit(__peek));

					Token number(num);
					number.setLocation(__zzline, __zzcol);
					return number;
				}

				// deal with words
				if (isalpha(__peek) || __peek == '_') {
					std::string stringBuffer;

					do {
						std::string tmp;
						tmp = __peek;
						stringBuffer += tmp;
						peekNext();
						if (isalnum(__peek) || __peek == '_')
							moveNext();
					} while (isalnum(__peek) || __peek == '_');

					std::map< std::string, int >::iterator it;

					std::string tmp = stringBuffer;
					int i = 0;
					while (tmp[i]) {
						tmp[i] = toupper(tmp[i]);
						i++;
					}
					it = __words.find(tmp);

					// reserved words are in the map
					if (it != __words.end()) {
						int t = __words[tmp];
						Token reservedWords(t, tmp);
						reservedWords.setLocation(__zzline, __zzcol);
						return reservedWords;
					}

					// otherwise ids
					Token id(ID, stringBuffer);
					id.setLocation(__zzline, __zzcol);
					return id;
				}

				// unknown character (excluding EOF)
				if (__end == false && __peek != '\r') {
					Token u(__peek);
					u.setLocation(__zzline, __zzcol);
					return u;
				}
			}

		}

		// EOF
		Token eof(EEOF, "EOF");
		eof.setLocation(__zzline, __zzcol);
		__end = false;
		return eof;
	}
};
