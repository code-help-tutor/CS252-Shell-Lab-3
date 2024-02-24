WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
//*

// * CS252: Shell project
 /*
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT 
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE 
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM  
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.

 */
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include "command.hh"
#include "shell.hh"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
Command::Command() {
    // Initialize a new vector of Simple Commands
    _simpleCommands = std::vector<SimpleCommand *>();


    _outFile = NULL;
    _inFile = NULL;
    _errFile = NULL;
    _background = false;
    _append = false;//add append in the .hh
    //
}

void Command::insertSimpleCommand( SimpleCommand * simpleCommand ) {
    // add the simple command to the vector
    //
    _simpleCommands.push_back(simpleCommand);
}

void Command::clear() {
    // deallocate all the simple commands in the command vector
    for (auto simpleCommand : _simpleCommands) {
        delete simpleCommand;
    }

    // remove all references to the simple commands we've deallocated
    // (basically just sets the size to 0)
    _simpleCommands.clear();

    if ( _outFile ) {
        delete _outFile;
    }
    _outFile = NULL;

    if ( _inFile ) {
        delete _inFile;
    }
    _inFile = NULL;

    if ( _errFile ) {
        delete _errFile;
    }
    _errFile = NULL;

    _background = false;
}

void Command::print() {
    printf("\n\n");
    printf("              COMMAND TABLE                \n");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ----------------------------------------------------------\n");

    int i = 0;
    // iterate over the simple commands and print them nicely
    for ( auto & simpleCommand : _simpleCommands ) {
        printf("  %-3d ", i++ );
        simpleCommand->print();
    }

    printf( "\n\n" );
    printf( "  Output       Input        Error        Background\n" );
    printf( "  ------------ ------------ ------------ ------------\n" );
//if (pipe(fdpipe)==-1) {
//perror("")

}

// end of PRINT; start of exe`

void Command::exe() {
    // Don't do anything if there are no simple commands
    if ( _simpleCommands.size() == 0 ) {
        Shell::prompt();
        return;
    }
    //int ret execute
int tmpin = dup(0);
int tmpout = dup(1);
int tmperr = dup(2);// maybe no need
 

// default input
int fdin;
int fdout; 
int fderror;

if (_inFile) {
fdin = open(_inFile->c_str(), O_RDONLY); // TODO: deal with error
if(fdin < 0)
perror("fdin error");
exit(1);
}
else 
fdin = dup(tmpin);
int pid;
for (int i = 0; i < _simpleCommands.size(); i ++) {// for every simcmd
dup2(fdin,0);
close(fdin);

// first simcmd for input?? < less?

if (i == _simpleCommands.size()-1) {// last simcmd
if (_outFile) {// do ambiguous output; a > b > c 
if (_append) {//TODO:  do ambiguous redirect
fdout = open(_outFile->c_str(), O_APPEND|O_CREAT);
if (fdout < 0)
perror("fdout error");
else fdout = open(_outFile->c_str(), O_TRUNC|O_CREAT);
}
else
fdout=dup(tmpout);

if (_errFile) {//_err same as to output
if (_append) 
fdout = open(_errFile->c_str(), O_APPEND|O_CREAT); 
else fdout = open(_errFile->c_str(), O_TRUNC|O_CREAT);
}

}
// place to write pipe code
else {// not last, pipe
int fdpipe[2];
pipe(fdpipe);
fdout = fdpipe[1];
fdin = fdpipe[0];
}
dup2(fdout,1);// now, 1 points to what pipe[1] points to; if last, 1 goes to outfi
close(fdout);
pid = fork();

if (pid == -1) {
perror("fork\n");
return;
}

if (pid == 0) {//child begin
//close(fdin);
//close(fdout);
//close(fderr);
//TODO: do I actually need fderr? 
//e.g. great must apply for last sicmd
//{

char ** cargs = new char* [_simpleCommands[i]->_arguments.size()];
for (int j = 0; j < _simpleCommands[i]->_arguments.size(); j++) {
cargs[j] =(char*)(_simpleCommands[i]->_arguments[j]->c_str());

cargs[_simpleCommands[i]->_arguments.size()] = NULL; // null terminate
}

execvp(_simpleCommands[i]->_arguments[0]->c_str(), cargs); // (args[0], args)  _arguments = std::vector<std::string *>()

perror("execvp");

_exit(1);
  }  // child end
}// end of for

// back to parent

// when for simple loop ends, temp back

dup2(tmpin, 0);
dup2(tmpout, 1);
dup2(tmperr, 2);
//restore 
close(tmpin);
close(tmpout);
close(tmperr);

if (!_background) {
waitpid(pid, NULL, 0);
}

// for every simcmd
//}// end of for
    // Print contents of Command data structure
    print();
    // Clear to prepare for next command
    clear();

    // Print new prompt
    Shell::prompt();
}//end of exe

SimpleCommand * Command::_currentSimpleCommand;



