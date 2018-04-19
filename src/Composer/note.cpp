#include "note.h"

std::ostream &operator<<(std::ostream& s, const note& n) {
	switch(n) {
		case A:		s << "A";	break; case Ab:	s << "Ab";	break;
		case B:		s << "B";	break; case Bb:	s << "Bb";	break;
		case C:		s << "C";	break; case F:	s << "F";	break;
		case D:		s << "D";	break; case Db:	s << "Db";	break;
		case E:		s << "E";	break; case Eb:	s << "Eb";	break;
		case G:		s << "G";	break; case Gb:	s << "Gb";	break;
		default: s << static_cast<int>(n);
	}
	return s;
}

int random(int a, int b) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(a, b);
	return dis(gen);
}
bool chance(int percent) { return (random(0, 100) < percent); }
note random_note() { return static_cast<note>(random(0, 11)); }

note operator+(note lhs, const int& rhs) {
	lhs += rhs; return lhs;
}

note operator-(note lhs, const int& rhs) {
	lhs -= rhs; return lhs;
}

note &operator+=(note& n, const int num) {
	n = static_cast<note>( (static_cast<int>(n) + num) % 12);
	return n;
}

note &operator-=(note& n, const int num) {
	const int i = static_cast<int>(n) - num;
	if (i < 0) n = static_cast<note>( 12 - ( (-1 * i) % 12 ) );
	else n = static_cast<note>(i % 12);
	return n;
}

note &operator++(note& n) {
	n = static_cast<note>( (static_cast<int>(n) + 1) % 12);
	return n;
}
note &operator--(note& n) {
	const int i = static_cast<int>(n)-1;
	if (i < 0) n = static_cast<note>( 12 - ( (-1 * i) % 12 ) );
	else n = static_cast<note>(i % 12);
	return n;
}

int interval(note a, note b) {
	int i = 0;
	note search = a;
	while (search != b) { ++search; ++i; }
	return (i % 12);
}