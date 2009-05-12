/*
 *  commandfactory.cpp
 *  
 *
 *  Created by Pat Schloss on 10/25/08.
 *  Copyright 2008 Patrick D. Schloss. All rights reserved.
 *
 */

#include "command.hpp"
#include "readdistcommand.h"
#include "readtreecommand.h"
#include "readotucommand.h"
#include "clustercommand.h"
#include "parselistcommand.h"
#include "collectcommand.h"
#include "collectsharedcommand.h"
#include "getgroupcommand.h"
#include "getlabelcommand.h"
#include "getlinecommand.h"
#include "rarefactcommand.h"
#include "summarycommand.h"
#include "summarysharedcommand.h"
#include "rarefactsharedcommand.h"
#include "quitcommand.h"
#include "helpcommand.h"
#include "commandfactory.hpp"
#include "deconvolutecommand.h"
#include "parsimonycommand.h"
#include "unifracunweightedcommand.h"
#include "unifracweightedcommand.h"
#include "libshuffcommand.h"
#include "heatmapcommand.h"
#include "filterseqscommand.h"
#include "mothur.h"
#include "venncommand.h"
#include "nocommands.h"
#include "binsequencecommand.h"
#include "getoturepcommand.h"
#include "treegroupscommand.h"
#include "bootstrapsharedcommand.h"
#include "concensuscommand.h"
#include "distancecommand.h"


/***********************************************************/

/***********************************************************/
CommandFactory::CommandFactory(){
	command = new NoCommand();
}
/***********************************************************/

/***********************************************************/
CommandFactory::~CommandFactory(){
	delete command;
}

/***********************************************************/

/***********************************************************/
//This function calls the appropriate command fucntions based on user input.
Command* CommandFactory::getCommand(string commandName){
	try {
		delete command;   //delete the old command

			 if(commandName == "read.dist")				{	command = new ReadDistCommand();			}
		else if(commandName == "read.otu")				{	command = new ReadOtuCommand();				}
		else if(commandName == "read.tree")				{	command = new ReadTreeCommand();			}
		else if(commandName == "cluster")				{	command = new ClusterCommand();				}
		else if(commandName == "deconvolute")			{	command = new DeconvoluteCommand();			}
		else if(commandName == "parsimony")				{	command = new ParsimonyCommand();			}
		else if(commandName == "help")					{	command = new HelpCommand();				}
		else if(commandName == "quit")					{	command = new QuitCommand();				}
		else if(commandName == "collect.single")		{	command = new CollectCommand();				}
		else if(commandName == "collect.shared")		{	command = new CollectSharedCommand();		}
		else if(commandName == "rarefaction.single")	{	command = new RareFactCommand();			}
		else if(commandName == "rarefaction.shared")	{	command = new RareFactSharedCommand();		}
		else if(commandName == "summary.single")		{	command = new SummaryCommand();				}
		else if(commandName == "summary.shared")		{	command = new SummarySharedCommand();		}
		else if(commandName == "unifrac.weighted")		{	command = new UnifracWeightedCommand();		}
		else if(commandName == "unifrac.unweighted")	{	command = new UnifracUnweightedCommand();	}
		else if(commandName == "get.group")             {   command = new GetgroupCommand();			}
		else if(commandName == "get.label")             {   command = new GetlabelCommand();			}
		else if(commandName == "get.line")              {   command = new GetlineCommand();				}
		else if(commandName == "libshuff")              {   command = new LibShuffCommand();			}
		else if(commandName == "heatmap")				{   command = new HeatMapCommand();				}
		else if(commandName == "filter.seqs")			{   command = new FilterSeqsCommand();          }
		else if(commandName == "venn")					{   command = new VennCommand();				}
		else if(commandName == "bin.seqs")				{   command = new BinSeqCommand();				}
		else if(commandName == "get.oturep")			{   command = new GetOTURepCommand();			}
		else if(commandName == "tree.shared")			{   command = new TreeGroupCommand();			}
		else if(commandName == "bootstrap.shared")		{   command = new BootSharedCommand();			}
		else if(commandName == "concensus")				{   command = new ConcensusCommand();			}
		else if(commandName == "distance")				{   command = new DistanceCommand();			}
		else											{	command = new NoCommand();					}

		return command;
	}
	catch(exception& e) {
		cout << "Standard Error: " << e.what() << " has occurred in the CommandFactory class Function getCommand. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}
	catch(...) {
		cout << "An unknown error has occurred in the CommandFactory class function getCommand. Please contact Pat Schloss at pschloss@microbio.umass.edu." << "\n";
		exit(1);
	}

}
/***********************************************************/

