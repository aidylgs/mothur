/*
 *  setoutdircommand.cpp
 *  Mothur
 *
 *  Created by westcott on 1/21/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "setdircommand.h"

//**********************************************************************************************************************
vector<string> SetDirectoryCommand::setParameters(){	
	try {
		CommandParameter ptempdefault("tempdefault", "String", "", "", "", "", "","",false,false); parameters.push_back(ptempdefault);
        CommandParameter pblast("blastdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pblast);
        CommandParameter ptools("tools", "String", "", "", "", "", "","",false,false); parameters.push_back(ptools);
        CommandParameter pdebug("debug", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(pdebug);
        CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pmodnames("modifynames", "Boolean", "", "T", "", "", "","",false,false); parameters.push_back(pmodnames);
		CommandParameter pinput("input", "String", "", "", "", "", "","",false,false,true); parameters.push_back(pinput);
		CommandParameter poutput("output", "String", "", "", "", "", "","",false,false,true); parameters.push_back(poutput);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
        
        abort = false; calledHelp = false;
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string SetDirectoryCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The set.dir command can be used to direct the output files generated by mothur to a specific place.\n";
		helpString += "The set.dir command can also be used to specify the directory where your input files are located, the directory must exist.\n";
		helpString += "The set.dir command can also be used to override or set the default location mothur will look for files if it is unable to find them, the directory must exist.\n";
        helpString += "The set.dir command can also be used to set the location of the blast directory if other than mothur's executable location, the directory must exist.\n";
        helpString += "The set.dir command can also be used to set the location of the directory containing mothur's external tools, i.e. vsearch, uchime, blast, fasterqdump, if other than mothur's executable location. You can set this to /usr/bin for example. \n";
        helpString += "The set.dir command can also be used to run mothur in debug mode.\n";
        helpString += "The set.dir command can also be used to seed random.\n";
        helpString += "The set.dir command can also be used to set the modifynames parameter. Default=t, meaning if your sequence names contain ':' change them to '_' to avoid issues while making trees.  modifynames=F will leave sequence names as they are.\n";
		helpString += "The set.dir command parameters are input, output, tempdefault and debug and one is required.\n";
        helpString += "To run mothur in debug mode set debug=true. Default debug=false.\n";
        helpString += "To seed random set seed=yourRandomValue. By default mothur seeds random with the start time.\n";
		helpString += "To return the output to the same directory as the input files you may enter: output=clear.\n";
		helpString += "To return the input to the current working directory you may enter: input=clear.\n";
        helpString += "To set the blast location to the directory where the blast executables are located you may enter: blastdir=yourBlastLocation.\n";
		helpString += "To set the output to the directory where mothur.exe is located you may enter: output=default.\n";
		helpString += "To set the input to the directory where mothur.exe is located you may enter: input=default.\n";
		helpString += "To return the tempdefault to the default you provided at compile time you may enter: tempdefault=clear.\n";
		helpString += "To set the tempdefault to the directory where mothur.exe is located you may enter: tempdefault=default.\n";
		helpString += "The set.dir command should be in the following format: set.dir(output=yourOutputDirectory, input=yourInputDirectory, tempdefault=yourTempDefault).\n";
		helpString += "Example set.outdir(output=/Users/lab/desktop/outputs, input=/Users/lab/desktop/inputs).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************

SetDirectoryCommand::SetDirectoryCommand(string option)  {
	try {
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
        else if(option == "category") {  abort = true; calledHelp = true;  }
		
		else {
			OptionParser parser(option, setParameters());
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			output = validParameter.valid(parameters, "output");
			if (output == "not found") {  output = "";  } 
			
			input = validParameter.valid(parameters, "input");
			if (input == "not found") {  input = "";  }
			
			tempdefault = validParameter.valid(parameters, "tempdefault");
			if (tempdefault == "not found") {  tempdefault = "";  }
            
            blastLocation = validParameter.valid(parameters, "blastdir");
            if (blastLocation == "not found") {  blastLocation = "";  }
            
            toolsLocation = validParameter.valid(parameters, "tools");
            if (toolsLocation == "not found") {  toolsLocation = "";  }
            
            bool debug = false;
            bool nodebug = false;
            debugorSeedOnly = false;
            string temp = validParameter.valid(parameters, "debug");
			if (temp == "not found") {  debug = false;  nodebug=true; }
            else {  debug = util.isTrue(temp); }
            m->setDebug(debug);
            
            bool nomod = false;
            temp = validParameter.valid(parameters, "modifynames");
			if (temp == "not found") {  modifyNames = true;  nomod=true; }
            else {  modifyNames = util.isTrue(temp); }
            m->setChangedSeqNames(modifyNames);
            
            bool seed = false;
            temp = validParameter.valid(parameters, "seed");
            if (temp == "not found") { random = 0; }
            else {
                if (util.isInteger(temp)) { util.mothurConvert(temp, random); seed = true; }
                else { m->mothurOut("[ERROR]: Seed must be an integer for the set.dir command.\n"); abort = true; }
            }
            
            if (debug) { m->mothurOut("Setting [DEBUG] flag.\n"); }
            if (seed)  {
                m->setRandomSeed(random);
                m->mothurOut("Setting random seed to " + toString(random) + ".\n\n");
            }
            
			if ((input == "") && (output == "") && (tempdefault == "") && (blastLocation == "") && (toolsLocation == "")&& nodebug && nomod && !seed) {
				m->mothurOut("[ERROR]: You must provide either an input, output, tempdefault, blastdir, tools, debug or modifynames for the set.dir command.\n");  abort = true;
			}else if((input == "") && (output == "") && (tempdefault == "") && (blastLocation == "") && (toolsLocation == "")) { debugorSeedOnly = true; }
		}
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "SetDirectoryCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int SetDirectoryCommand::execute(){
	try {
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
        if (debugorSeedOnly) {  }
        else {
            m->mothurOut("Mothur's directories:\n");
            
            //redirect output
            if ((output == "clear") || (output == "")) {  output = "";  current->setOutputDir(output);  }
            else if (output == "default") {
                string output = current->getProgramPath();
                
                m->mothurOut("outputDir=" + output + "\n");
                current->setOutputDir(output);
            }else {
                output = util.removeQuotes(output);
                if (util.mkDir(output)) {
                    m->mothurOut("outputDir=" + output + "\n");
                    current->setOutputDir(output);
                }
            }
            
            //redirect input
            if ((input == "clear") || (input == "")) {  input = "";  current->setInputDir(input);  }
            else if (input == "default") {
                string input = current->getProgramPath();
                //input = exepath.substr(0, (exepath.find_last_of('m')));
                
                m->mothurOut("inputDir=" + input+ "\n");
                current->setInputDir(input);
            }else {
                input = util.removeQuotes(input);
                if (util.dirCheck(input)) {
                    m->mothurOut("inputDir=" + input+ "\n");
                    current->setInputDir(input);
                }
            }
            
            //set default
            if (tempdefault == "clear") {
#ifdef MOTHUR_FILES
				string temp = MOTHUR_FILES;
				m->mothurOut("tempDefault=" + temp+ "\n");
				current->setDefaultPath(temp);
#else
				string temp = "";
				m->mothurOut("No default directory defined at compile time.\n"); 
				current->setDefaultPath(temp);
#endif
            }else if (tempdefault == "") {  //do nothing
            }else if (tempdefault == "default") {
                string tempdefault = current->getProgramPath();
                //tempdefault = exepath.substr(0, (exepath.find_last_of('m')));
                
                m->mothurOut("tempDefault=" + tempdefault+ "\n");
                current->setDefaultPath(tempdefault);
            }else {
                tempdefault = util.removeQuotes(tempdefault);
                if (util.mkDir(tempdefault)) {
                    m->mothurOut("tempDefault=" + tempdefault+ "\n");
                    current->setDefaultPath(tempdefault);
                }
            }
            
            //set default
            if ((toolsLocation == "default") || (toolsLocation == "clear")){
#ifdef MOTHUR_FILES
                string temp = MOTHUR_TOOLS;
                m->mothurOut("tools=" + temp+ "\n");
                current->setToolsPath(temp);
#else
                string temp = current->getProgramPath();
                m->mothurOut("tools=" + temp+ "\n");
                current->setToolsPath(temp);
#endif
            }else if (toolsLocation == "") {  //do nothing
            }else {
                toolsLocation = util.removeQuotes(toolsLocation);
                if (util.dirCheck(toolsLocation)) {
                    m->mothurOut("tools=" + toolsLocation+ "\n");
                    current->setToolsPath(toolsLocation);
                }
            }
            
            //set default
            if (blastLocation == "") {
            }else if ((blastLocation == "default") || (blastLocation == "clear")){
                string blastLocation = current->getProgramPath();
                                
                m->mothurOut("Blast Location=" + blastLocation+ "\n");
                current->setBlastPath(blastLocation);
            }else {
                blastLocation = util.removeQuotes(blastLocation);
                if (util.dirCheck(blastLocation)) {
                    m->mothurOut("Blast Location=" + blastLocation+ "\n");
                    current->setBlastPath(blastLocation);
                }
            }

        }
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SetDirectoryCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************/
