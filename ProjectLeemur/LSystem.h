#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

#define RULE_1	1
#define RULE_2	2
#define RULE_3	3
#define RULE_START_POS	3

class LSystem {
public:
	LSystem(std::string const & infile);

	int iterations;
	int angle;
	std::string axiom;
	std::string r1;
	std::string r2;
	std::string r3;
	std::vector<char> grammar;

	std::vector<std::string> var_list;
	void setVariables();
	void setIterations(int iteration);
	void setAngle(float angle);
	void parse_file(std::string const & infile);
	void create_grammar();
	int set_replace_grammar(int grammar_loop_pos, int r);
};
