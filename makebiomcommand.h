#ifndef Mothur_makebiomcommand_h
#define Mothur_makebiomcommand_h

//
//  makebiomcommand.h
//  Mothur
//
//  Created by Sarah Westcott on 4/16/12.
//  Copyright (c) 2012 Schloss Lab. All rights reserved.
//


#include "command.hpp"
#include "sharedrabundvector.h"
#include "inputdata.h"


class MakeBiomCommand : public Command {
	
public:
	MakeBiomCommand(string);
	MakeBiomCommand();	
	~MakeBiomCommand(){}
	
	vector<string> setParameters();
	string getCommandName()			{ return "make.biom";	}
	string getCommandCategory()		{ return "General";		}
	string getHelpString();	
	string getCitation() { return "http://biom-format.org/documentation/biom_format.html, http://www.mothur.org/wiki/Make.biom"; }
	string getDescription()		{ return "creates a biom file"; }
    
	int execute(); 
	void help() { m->mothurOut(getHelpString()); }	
	
private:
    
	string sharedfile, contaxonomyfile, groups, outputDir, format, label;
	vector<string> outputNames, Groups;
	set<string> labels;
    
	bool abort, allLines;
    
    int getBiom(vector<SharedRAbundVector*>&);
    vector<string> getMetaData(vector<SharedRAbundVector*>&);
    vector<string> parseTax(string tax, vector<string>& scores);
};


#endif
