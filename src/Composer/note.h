#pragma once

#include <iostream>
#include <random>

enum note {Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G};

int interval(note a, note b);
note random_note();
std::ostream &operator<<(std::ostream& s, const note& n);
note operator+(note lhs, const int& rhs);
note operator-(note lhs, const int& rhs);
note &operator+=(note& n, const int num);
note &operator-=(note& n, const int num);
note &operator++(note& n);
note &operator--(note& n);

int random(int a, int b);
bool chance(int percent);