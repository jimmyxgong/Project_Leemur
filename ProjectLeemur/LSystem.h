#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <random>
#include "Mesh.h"
#include "Turtle.h"


#define RULE_1	1
#define RULE_2	2
#define RULE_3	3
#define RULE_4  4
#define RULE_START_POS	3

class LSystem{
public:
	LSystem(std::string const & infile, long long seed);

	int iterations;
	int angle;
	long long seed;
	std::string axiom;
	std::string r1;
	std::string r2;
	std::string r3;
	std::string r4;
	std::vector<char> grammar;
	float posX, posY, posZ;

	SharedPointer<Turtle> turtle = std::make_shared<Turtle>();
	std::vector<std::string> var_list;
	void setIterations(int iteration);
	void setAngle(float angle);
	void setPosition(float x, float y, float z);
	void parse_file(std::string const & infile);
	void create_grammar();
	void drawRules();
	float randomize();
	int set_replace_grammar(int grammar_loop_pos, int r);
};
