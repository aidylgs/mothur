//
//  sharedclrvectors.cpp
//  Mothur
//
//  Created by Sarah Westcott on 1/21/20.
//  Copyright © 2020 Schloss Lab. All rights reserved.
//

#include "sharedclrvectors.hpp"

/***********************************************************************/
//reads a clr file
SharedCLRVectors::SharedCLRVectors(ifstream& f, vector<string>& userGroups, string& nextLabel, string& labelTag) : DataVector() {
    try {
        int num;
        string holdLabel, groupN;
        int numUserGroups = userGroups.size();
        
        for (int i = 0; i < lookup.size(); i++) {  if (lookup[i] != nullptr) { delete lookup[i];  lookup[i] = nullptr; } }  lookup.clear();
        
        //are we at the beginning of the file??
        if (nextLabel == "") {
            f >> label;
            
            //is this a shared file that has headers
            if (label == "label") {
                //gets "group"
                f >> label; util.gobble(f);
                
                //gets "numOtus"
                f >> label; util.gobble(f);
                
                //eat rest of line
                label = util.getline(f); util.gobble(f);
                
                //parse labels to save
                istringstream iStringStream(label);
                while(!iStringStream.eof()){
                    if (m->getControl_pressed()) { break; }
                    string temp;
                    iStringStream >> temp;  util.gobble(iStringStream);
                    
                    currentLabels.push_back(temp);
                }
                
                if (currentLabels.size() != 0) {
                    string binLabelTag = currentLabels[0];
                    labelTag = "";
                    for (int i = 0; i < binLabelTag.length(); i++) { if (isalpha(binLabelTag[i])){ labelTag += binLabelTag[i]; } }
                }
                f >> label >> groupN >> num;
            }else {
                //read in first row since you know there is at least 1 group.
                f >> groupN >> num;
                
                //make binlabels because we don't have any
                string snumBins = toString(num);
                if (labelTag == "") { labelTag = "Otu"; }
                for (int i = 0; i < num; i++) {
                    //if there is a bin label use it otherwise make one
                    string binLabel = labelTag;
                    string sbinNumber = toString(i+1);
                    if (sbinNumber.length() < snumBins.length()) {
                        int diff = snumBins.length() - sbinNumber.length();
                        for (int h = 0; h < diff; h++) { binLabel += "0"; }
                    }
                    binLabel += sbinNumber;
                    currentLabels.push_back(binLabel);
                }
            }
        }else {
            label = nextLabel;
            
            //read in first row since you know there is at least 1 group.
            f >> groupN >> num;
        }
        
        bool readData = false;
        bool remove = false;
        if (numUserGroups == 0) { //user has not specified groups, so we will use all of them
            userGroups.push_back(groupN);
            readData = true;
        }else{
            if (util.inUsersGroups(groupN, userGroups)) { readData = true; }
            else { remove = true; }// skipline because you are a group we dont care about
        }

        
        holdLabel = label;
        numBins = num;
        
        if (readData) {
            //add new vector to lookup
            SharedCLRVector* temp = new SharedCLRVector(f, label, groupN, numBins);
            push_back(temp);
        } else { util.getline(f); }
        util.gobble(f);
        
        if (!(f.eof())) { f >> nextLabel; }
        
        //read the rest of the groups info in
        while ((nextLabel == holdLabel) && (f.eof() != true)) {
            f >> groupN >> num;
            bool readData = false;
            if (numUserGroups == 0) { //user has not specified groups, so we will use all of them
                userGroups.push_back(groupN);
                readData = true;
            }else{
                if (util.inUsersGroups(groupN, userGroups)) { readData = true; }
                else { remove = true; }// skipline because you are a group we dont care about
            }
            
            if (readData) {
                SharedCLRVector* temp = new SharedCLRVector(f, label, groupN, numBins);
                push_back(temp);
            }else { util.getline(f); }
            util.gobble(f);
            
            if (f.eof() != true) { f >> nextLabel; }
        }
        
        otuTag = labelTag;
        
        //error in names of user inputted Groups
        if (lookup.size() < userGroups.size()) { m->mothurOut("[ERROR]: requesting groups not present in files, aborting.\n"); m->setControl_pressed(true); }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "SharedCLRVectors");
        exit(1);
    }
}
/***********************************************************************/
void SharedCLRVectors::print(ostream& output, bool& printOTUHeaders){
    try {
        printHeaders(output, printOTUHeaders);
        sort(lookup.begin(), lookup.end(), compareCLRVectors);
        for (int i = 0; i < lookup.size(); i++) {
            if (m->getControl_pressed()) { break; }
            lookup[i]->print(output);
        }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "print");
        exit(1);
    }
}
/***********************************************************************/
string SharedCLRVectors::getOTUName(int bin){
    try {
        if (currentLabels.size() > bin) {  }
        else { getOTUNames(); }
        return currentLabels[bin];
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getOTUName");
        exit(1);
    }
}
/***********************************************************************/
void SharedCLRVectors::setOTUName(int bin, string otuName){
    try {
        if (currentLabels.size() > bin) {  currentLabels[bin] = otuName; }
        else {
            getOTUNames(); //fills currentLabels if needed
            if (currentLabels.size() > bin) {  currentLabels[bin] = otuName; }
            else {
                m->setControl_pressed(true);
                m->mothurOut("[ERROR]: " + toString(bin) + " bin does not exist\n");
            }
        }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "setOTUName");
        exit(1);
    }
}
/***********************************************************************/
int SharedCLRVectors::push_back(vector<float> abunds, string binLabel){
    try {
        if (abunds.size() != lookup.size()) {  m->mothurOut("[ERROR]: you have provided " + toString(abunds.size()) + " abundances, but mothur was expecting " + toString(lookup.size()) + ", please correct.\n"); m->setControl_pressed(true); return 0; }
        
        for (int i = 0; i < lookup.size(); i ++) { lookup[i]->push_back(abunds[i]); }
        //vector<string> currentLabels = m->getCurrentSharedBinLabels();
        if (binLabel == "") { //create one
            int otuNum = 1; bool notDone = true;
            
            //find label prefix
            string prefix = "Otu";
            if (currentLabels.size() != 0) {
                if (currentLabels[currentLabels.size()-1][0] == 'P') { prefix = "PhyloType"; }
                
                string tempLabel = currentLabels[currentLabels.size()-1];
                string simpleLastLabel = util.getSimpleLabel(tempLabel);
                util.mothurConvert(simpleLastLabel, otuNum); otuNum++;
            }
            
            string potentialLabel = toString(otuNum);
            
            while (notDone) {
                if (m->getControl_pressed()) { notDone = false; break; }
                
                potentialLabel = toString(otuNum);
                vector<string>::iterator it = find(currentLabels.begin(), currentLabels.end(), potentialLabel);
                if (it == currentLabels.end()) {
                    potentialLabel = prefix + toString(otuNum);
                    it = find(currentLabels.begin(), currentLabels.end(), potentialLabel);
                    if (it == currentLabels.end()) {
                        notDone = false; break;
                    }
                }
                otuNum++;
            }
            
            binLabel = potentialLabel;
        }
        currentLabels.push_back(binLabel);
        numBins++;
        
        return lookup.size();
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "push_back");
        exit(1);
    }
}
/***********************************************************************/
int SharedCLRVectors::push_back(SharedCLRVector* thisLookup){
    try {
        if (numBins == 0) { numBins = thisLookup->getNumBins();  }
        lookup.push_back(thisLookup);
        sort(lookup.begin(), lookup.end(), compareCLRVectors);
        if (label == "") { label = thisLookup->getLabel(); }
        groupNames.clear();
        for (int i = 0; i < lookup.size(); i ++) { groupNames[lookup[i]->getGroup()] = i; }
        return lookup.size();
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "push_back");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::getOTUTotal(int bin){
    try {
        float totalOTUAbund = 0;
        for (int i = 0; i < lookup.size(); i++) { totalOTUAbund += lookup[i]->get(bin); }
        return totalOTUAbund;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getOTUTotal");
        exit(1);
    }
}
/***********************************************************************/
vector<float> SharedCLRVectors::getOTU(int bin){
    try {
        vector<float> abunds;
        for (int i = 0; i < lookup.size(); i++) { abunds.push_back(lookup[i]->get(bin)); }
        return abunds;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getOTU");
        exit(1);
    }
}

/***********************************************************************/
void SharedCLRVectors::setLabels(string l){
    try {
        label = l;
        for (int i = 0; i < lookup.size(); i++) { lookup[i]->setLabel(l); }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "setLabels");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::get(int bin, string group){
    try {
        float abund = 0;
        map<string, int>::iterator it = groupNames.find(group);
        if (it == groupNames.end()) { m->mothurOut("[ERROR]: can not find group " + group + ".\n"); m->setControl_pressed(true);  }
        else { abund = lookup[it->second]->get(bin); }
        return abund;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "get");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::getNumSeqs(string group){
    try {
        float numSeqs = 0;
        map<string, int>::iterator it = groupNames.find(group);
        if (it == groupNames.end()) { m->mothurOut("[ERROR]: can not find group " + group + ".\n"); m->setControl_pressed(true);  }
        else { numSeqs = lookup[it->second]->getNumSeqs(); }
        
        return numSeqs;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getNumSeqs");
        exit(1);
    }
}
/***********************************************************************/
void SharedCLRVectors::set(int bin, float binSize, string group){
    try {
        map<string, int>::iterator it = groupNames.find(group);
        if (it == groupNames.end()) { m->mothurOut("[ERROR]: can not find group " + group + ".\n"); m->setControl_pressed(true);  }
        else { lookup[it->second]->set(bin, binSize); }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "set");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::removeOTU(int bin){
    try {
        float totalOTUAbund = 0;
        for (int i = 0; i < lookup.size(); i ++) { totalOTUAbund += lookup[i]->remove(bin); }
        
        currentLabels.erase(currentLabels.begin()+bin);
        numBins--;
        
        return totalOTUAbund;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "removeOTU");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::removeOTUs(vector<int> bins, bool sorted){
    try {
        if (bins.size() == 0) { return 0; }
        
        if (!sorted) { sort(bins.begin(), bins.end()); }
        
        float totalOTUAbund = 0;
        for (int i = 0; i < lookup.size(); i ++) { totalOTUAbund += lookup[i]->remove(bins); }
        
        vector<string> newLabels; int binIndex = 0;
        for (int i = 0; i < currentLabels.size(); i++) {
            if (m->getControl_pressed()) { break; }
            
            if (i != bins[binIndex]) {
                newLabels.push_back(currentLabels[i]);
            }else if (i == bins[binIndex]) {
                binIndex++;
                
                if (binIndex > bins.size()) { //removed all bins
                    newLabels.insert(newLabels.end(), currentLabels.begin()+i, currentLabels.end()); //add rest of good bins
                    break;
                }
            }
        }
        
        currentLabels = newLabels;
        numBins = currentLabels.size();
        
        return totalOTUAbund;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "removeOTUs");
        exit(1);
    }
}

/***********************************************************************/
void SharedCLRVectors::setOTUNames(vector<string> names){
    try {
        currentLabels.clear();
        currentLabels = names;
        getOTUNames();
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "setOTUNames");
        exit(1);
    }
}
/***********************************************************************/
vector<string> SharedCLRVectors::getOTUNames(){
    try {
        util.getOTUNames(currentLabels, numBins, otuTag);
        return currentLabels;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getOTUNames");
        exit(1);
    }
}
/***********************************************************************/
void SharedCLRVectors::printHeaders(ostream& output, bool& printSharedHeaders){
    try {
        if (printSharedHeaders) {
            getOTUNames();
        
            output << "label\tGroup\tnumOtus";
            for (int i = 0; i < numBins; i++) { output  << '\t' << currentLabels[i]; } output << endl;
        
            printSharedHeaders = false;
        }
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "printHeaders");
        exit(1);
    }
}
/***********************************************************************/
vector<string> SharedCLRVectors::getNamesGroups(){
    try {
        vector<string> names;
        for (int i = 0; i < lookup.size(); i ++) { names.push_back(lookup[i]->getGroup()); }
        return names;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getNamesGroups");
        exit(1);
    }
}
/***********************************************************************/
float SharedCLRVectors::getNumSeqsSmallestGroup(){
    try {
        float smallest = MOTHURMAX;
        for (int i = 0; i < lookup.size(); i++) {
            if (lookup[i]->getNumSeqs() < smallest) { smallest = lookup[i]->getNumSeqs(); }
        }
        return smallest;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedRAbundVector", "getNumSeqsSmallestGroup");
        exit(1);
    }
}
/***********************************************************************/
vector<SharedCLRVector*> SharedCLRVectors::getSharedCLRVectors(){
    try {
        vector<SharedCLRVector*> newLookup;
        for (int i = 0; i < lookup.size(); i++) {
            SharedCLRVector* temp = new SharedCLRVector(*lookup[i]);
            newLookup.push_back(temp);
        }
        
        return newLookup;
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "getSharedCLRVectors");
        exit(1);
    }
}
/***********************************************************************/
void SharedCLRVectors::removeGroups(vector<string> g){
    try {
        bool remove = false;
        for (vector<SharedCLRVector*>::iterator it = lookup.begin(); it != lookup.end();) {
            //if this sharedrabund is not from a group the user wants then delete it.
            if (util.inUsersGroups((*it)->getGroup(), g) ) {
                remove = true;
                delete (*it); (*it) = nullptr;
                it = lookup.erase(it);
            }else { ++it; }
        }
        
        groupNames.clear();
        for (int i = 0; i < lookup.size(); i ++) { groupNames[lookup[i]->getGroup()] = i; }
        
    }
    catch(exception& e) {
        m->errorOut(e, "SharedRAbundFloatVectors", "removeGroups");
        exit(1);
    }
}
/***********************************************************************/
int SharedCLRVectors::removeGroups(int minSize, bool silent){
    try {
        vector<string> Groups;
        
        for (vector<SharedCLRVector*>::iterator it = lookup.begin(); it != lookup.end();) {
            if ((*it)->getNumSeqs() < minSize) {
                if (!silent) { m->mothurOut((*it)->getGroup() + " contains " + toString((*it)->getNumSeqs()) + ". Eliminating.\n"); }
                delete (*it); (*it) = nullptr;
                it = lookup.erase(it);
            }else {
                Groups.push_back((*it)->getGroup());
                ++it;
            }
        }
        
        groupNames.clear();
        for (int i = 0; i < lookup.size(); i ++) { groupNames[lookup[i]->getGroup()] = i; }
        
        return lookup.size();
    }
    catch(exception& e) {
        m->errorOut(e, "SharedCLRVectors", "removeGroups");
        exit(1);
    }
}
/***********************************************************************/

