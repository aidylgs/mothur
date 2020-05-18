//
//  eachgapdist.cpp
//  Mothur
//
//  Created by Sarah Westcott on 4/21/20.
//  Copyright © 2020 Schloss Lab. All rights reserved.
//

#include "eachgapdist.h"

/***********************************************************************/

double eachGapDist::calcDist(Sequence A, Sequence B){
    try {
        string seqA = A.getAligned();
        string seqB = B.getAligned();
        
        int alignLength = (int)seqA.length();
        
        int start = setStart(seqA, seqB);
        int end = setEnd(seqA, seqB);
        
        int maxMinLength = end - start + 1;
        int diff = 0;
        
        for(int i=start;i<alignLength;i++){
            if(seqA[i] == '.' && seqB[i] == '.'){ //reached terminal gaps, so quit
                break;
            }
            else if((seqA[i] == '-' && seqB[i] == '-') || (seqA[i] == '-' && seqB[i] == '.') || (seqA[i] == '.' && seqB[i] == '-')){ maxMinLength--; } //comparing gaps, ignore
            else{
                if(seqA[i] != seqB[i]){
                    diff++;
                }
            }
            
            dist = (double)diff / maxMinLength;
            
            if (dist > cutoff) { return 1.0000; }
        }
        
        if(maxMinLength == 0)    {    dist = 1.0000;                                }
        else            {    dist = ((double)diff  / (double)maxMinLength);        }
        
        return dist;
    }
    catch(exception& e) {
        m->errorOut(e, "eachGapDist", "calcDist");
        exit(1);
    }
}
/***********************************************************************/
vector<double> eachGapDist::calcDist(Sequence A, classifierOTU otu, vector<int> cols){ //this function calcs the distance using only the columns provided
    try {
        vector<double> dists; dists.resize(otu.numSeqs, 0.0);
        
        //if you didn't select columns, use all columns
        if (cols.size() == 0) {
            for (int i = 0; i < otu.otuData.size(); i++) { cols.push_back(i); }
        }
        
        classifierOTU seq(A.getAligned());
        vector<int> starts = setStarts(seq, otu, cols);
        vector<int> ends = setEnds(seq, otu, cols);
        
        int alignLength = cols.size();

        for (int h = 0; h < otu.numSeqs; h++) {
            
            if (m->getControl_pressed()) { break; }
            
            int maxMinLength = ends[h] - starts[h] + 1;
            int difference = 0;
            
            for(int i=starts[h];i<alignLength;i++){
                
                char seqA = seq.otuData[cols[i]][0];
                vector<char> otuChars = otu.otuData[cols[i]];
                char seqB = otuChars[0]; //assume column if identical
                if (otuChars.size() == otu.numSeqs) {  seqB = otuChars[h]; }
                
                if(seqA == '.' && seqB == '.'){ i+=alignLength; } //reached terminal gaps, so quit
                
                else if((seqA == '-' && seqB == '-') || (seqA == '-' && seqB == '.') || (seqA == '.' && seqB == '-')){ maxMinLength--; } //comparing gaps, ignore
                else{
                    if(seqA != seqB){ difference++; }
                }
                
                double distance = 1.0;
                distance = (double)difference / maxMinLength;
                
                if (distance > cutoff) { dists[h] = 1.0000;    i+=alignLength;  } //break;
            }
            
            if(maxMinLength == 0)       {    dists[h] = 1.0000;                               }
            else if (dists[h] == 0.0)   {    dists[h] = (double)difference / maxMinLength;    } //not set
        }
    
        return dists;
    }
    catch(exception& e) {
        m->errorOut(e, "oneGapDist", "calcDist");
        exit(1);
    }
}
/***********************************************************************/

