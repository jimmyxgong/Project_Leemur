#include "LSystem.h"

LSystem::LSystem(std::string const & infile) {
	parse_file(infile);
	//now create the grammar used to draw the L-system
	create_grammar();
	
}

void LSystem::setIterations(int iteration) {
	this->iterations = iteration;
}

void LSystem::setAngle(float angle) {
	this->angle = angle;
}

void LSystem::setPosition(float x, float y, float z) {
	this->posX = x;
	this->posY = y;
	this->posZ = z;
}

void LSystem::parse_file(std::string const & infile) {
	std::ifstream settings;
	settings.open(infile);

	std::string vars;
	std::string iter;
	std::string ang;

	//getting settings for the L-System of the object
	std::getline(settings, vars);	
	std::getline(settings, this->axiom);	
	std::getline(settings, iter);
	std::getline(settings, ang);	
	//for now lets limit to only 3 rules
	std::getline(settings, this->r1);
	std::getline(settings, this->r2);
	std::getline(settings, this->r3);

	//create variable vector
	std::istringstream buffer(vars);
	std::string s;
	while (std::getline(buffer, s, ' ')) {
		this->var_list.push_back(s);
	}

	this->iterations = std::stoi(iter, nullptr);
	this->angle = std::stoi(ang, nullptr);

	settings.close();

	/** DEBUGGING STATEMENTS **/
	//std::cout << "vars: " << vars << std::endl;
	//std::cout << "axiom: " << axiom << std::endl;
	//std::cout << "iter: " << iter << std::endl;
	//std::cout << "ang: " << ang << std::endl;
	//std::cout << "r1: " << r1 << std::endl;
	//std::cout << "r2: " << r2 << std::endl;
	//std::cout << "r3: " << r3 << std::endl;
}

void LSystem::create_grammar() {
	//initial state of grammar is its axiom (starting variable)
	grammar.push_back(axiom[0]);

	//increasing iterations increate complexity of the grammar
	for (int i = 0; i < iterations; i++) {
		for (int j = 0; j < grammar.size(); j++) {
			if (grammar[j] == this->r1[0]) {		//rule 1
				//utilize rule_1's grammar instructions and apply them to our grammar 
				j = set_replace_grammar(j, RULE_1);
			}
			else if (grammar[j] == this->r2[0]) {	//rule 2
				j = set_replace_grammar(j, RULE_2);
			}
			else if (grammar[j] == this->r3[0]) {	//rule 3
				j = set_replace_grammar(j, RULE_3);
			}
		}
	}

	/* USE THIS TO CHECK OUTPUT OF THE FINAL GRAMMAR*/
	/*
	for (std::vector<char>::iterator iz = grammar.begin(); iz != grammar.end(); iz++) {
		std::cout << *iz;
	}
	std::cout << std::endl;
	*/
}

int LSystem::set_replace_grammar(int grammar_loop_pos, int r) {
	int iter_pos = grammar_loop_pos;
	std::string rule;

	//determine which rule we are using
	switch (r) {
	case 1: rule = this->r1; break;
	case 2: rule = this->r2; break;
	case 3: rule = this->r3; break;
	}

	//start from index 3 because of the "F->" syntax at start of the string
	//RULE_START_POS == 3
	for (int i = RULE_START_POS; i < rule.length(); i++) {
		std::vector<char>::iterator it = grammar.begin();
		//replace the variable with the start of its own rule
		if (i == RULE_START_POS) {
			grammar[grammar_loop_pos] = rule[RULE_START_POS];
			continue;
		}
		else {
			//last variable of the grammar, put rules to end of grammar
			if (iter_pos == grammar.size() - 1) {	
				grammar.insert(grammar.end(), rule[i]);
				iter_pos++;
			}
			//insert in middle of vector
			else {	
				iter_pos++;
				grammar.insert(it + iter_pos, rule[i]);
			}
		}
	}
	//now that we have added more items to our grammar, update the position to go the next original variable to evaluate
	return iter_pos;
}

void LSystem::drawRules() {
	for (int i = 0; i < grammar.size(); i++) {
		char rule = grammar[i];
		switch (rule) {
		case 'F': {
			//draw forward
			turtle->drawForward(2.0f);
			break;
		}
		case 'X': {
			//move turtle
			turtle->moveForward(2.0f);
			break;
		}
		case '+': {
			//turn right angle
			turtle->rotateRight(this->angle);
			break;
		}
		case '-': {
			//turn left angle
			turtle->rotateLeft(this->angle);
			break;
		}
		case '&': {
			//pitch down
			break;
		}
		case '^': {
			//pitch up
			break;
		}
	   case '\\' : {
		   //roll left
		   break;
		}
	   case '/': {
		   //roll right
		   break;
	   }
	   case '|': {
		   //turn 180
		   break;
	   }
	   case '[': {
			//save matrix state
		   turtle->saveState();
			break;
		}
	    case ']': {
			//restore matrix state
			turtle->restoreState();
			break;
		}

		}
	}
}