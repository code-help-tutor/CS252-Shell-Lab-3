WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "simpleCommand.hh"

SimpleCommand::SimpleCommand() {
// vector functionalities: 1. vec[i]; 2. push_back. 
//3. vectors work as 
  _arguments = std::vector<std::string *>();
}

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _arguments) {
    delete arg;
  }
}

void SimpleCommand::insertArgument( std::string * argument ) {
  // simply add the argument to the vector at end 
  _arguments.push_back(argument);
}

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _arguments) {
    std::cout << "\"" << *arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
