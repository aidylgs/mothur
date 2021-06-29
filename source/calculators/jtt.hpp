//
//  jtt.hpp
//  Mothur
//
//  Created by Sarah Westcott on 5/26/21.
//  Copyright © 2021 Schloss Lab. All rights reserved.
//

#ifndef jtt_hpp
#define jtt_hpp

#include "calculator.h"

/* this jtt matrix decomposition due to Elisabeth  Tillier */

static double jtteigs[] =
{+0.00000000000000,-1.81721720738768,-1.87965834528616,-1.61403121885431,
-1.53896608443751,-1.40486966367848,-1.30995061286931,-1.24668414819041,
-1.17179756521289,-0.31033320987464,-0.34602837857034,-1.06031718484613,
-0.99900602987105,-0.45576774888948,-0.86014403434677,-0.54569432735296,
-0.76866956571861,-0.60593589295327,-0.65119724379348,-0.70249806480753};

static double jttprobs[20][20] =
{{+0.07686196156903,+0.05105697447152,+0.04254597872702,+0.05126897436552,
 +0.02027898986051,+0.04106097946952,+0.06181996909002,+0.07471396264303,
 +0.02298298850851,+0.05256897371552,+0.09111095444453,+0.05949797025102,
 +0.02341398829301,+0.04052997973502,+0.05053197473402,+0.06822496588753,
 +0.05851797074102,+0.01433599283201,+0.03230298384851,+0.06637396681302},
{-0.04445795120462,-0.01557336502860,-0.09314817363516,+0.04411372100382,
 -0.00511178725134,+0.00188472427522,-0.02176250428454,-0.01330231089224,
 +0.01004072641973,+0.02707838224285,-0.00785039050721,+0.02238829876349,
 +0.00257470703483,-0.00510311699563,-0.01727154263346,+0.20074235330882,
 -0.07236268502973,-0.00012690116016,-0.00215974664431,-0.01059243778174},
{+0.09480046389131,+0.00082658405814,+0.01530023104155,-0.00639909042723,
 +0.00160605602061,+0.00035896642912,+0.00199161318384,-0.00220482855717,
 -0.00112601328033,+0.14840201765438,-0.00344295714983,-0.00123976286718,
 -0.00439399942758,+0.00032478785709,-0.00104270266394,-0.02596605592109,
 -0.05645800566901,+0.00022319903170,-0.00022792271829,-0.16133258048606},
{-0.06924141195400,-0.01816245289173,-0.08104005811201,+0.08985697111009,
 +0.00279659017898,+0.01083740322821,-0.06449599336038,+0.01794514261221,
 +0.01036809141699,+0.04283504450449,+0.00634472273784,+0.02339134834111,
 -0.01748667848380,+0.00161859106290,+0.00622486432503,-0.05854130195643,
 +0.15083728660504,+0.00030733757661,-0.00143739522173,-0.05295810171941},
{-0.14637948915627,+0.02029296323583,+0.02615316895036,-0.10311538564943,
 -0.00183412744544,-0.02589124656591,+0.11073673851935,+0.00848581728407,
 +0.00106057791901,+0.05530240732939,-0.00031533506946,-0.03124002869407,
 -0.01533984125301,-0.00288717337278,+0.00272787410643,+0.06300929916280,
 +0.07920438311152,-0.00041335282410,-0.00011648873397,-0.03944076085434},
{-0.05558229086909,+0.08935293782491,+0.04869509588770,+0.04856877988810,
 -0.00253836047720,+0.07651693957635,-0.06342453535092,-0.00777376246014,
 -0.08570270266807,+0.01943016473512,-0.00599516526932,-0.09157595008575,
 -0.00397735155663,-0.00440093863690,-0.00232998056918,+0.02979967701162,
 -0.00477299485901,-0.00144011795333,+0.01795114942404,-0.00080059359232},
{+0.05807741644682,+0.14654292420341,-0.06724975334073,+0.02159062346633,
 -0.00339085518294,-0.06829036785575,+0.03520631903157,-0.02766062718318,
 +0.03485632707432,-0.02436836692465,-0.00397566003573,-0.10095488644404,
 +0.02456887654357,+0.00381764117077,-0.00906261340247,-0.01043058066362,
 +0.01651199513994,-0.00210417220821,-0.00872508520963,-0.01495915462580},
{+0.02564617106907,+0.02960554611436,-0.00052356748770,+0.00989267817318,
 -0.00044034172141,-0.02279910634723,-0.00363768356471,-0.01086345665971,
 +0.01229721799572,+0.02633650142592,+0.06282966783922,-0.00734486499924,
 -0.13863936313277,-0.00993891943390,-0.00655309682350,-0.00245191788287,
 -0.02431633805559,-0.00068554031525,-0.00121383858869,+0.06280025239509},
{+0.11362428251792,-0.02080375718488,-0.08802750967213,-0.06531316372189,
 -0.00166626058292,+0.06846081717224,+0.07007301248407,-0.01713112936632,
 -0.05900588794853,-0.04497159138485,+0.04222484636983,+0.00129043178508,
 -0.01550337251561,-0.01553102163852,-0.04363429852047,+0.01600063777880,
 +0.05787328925647,-0.00008265841118,+0.02870014572813,-0.02657681214523},
{+0.01840541226842,+0.00610159018805,+0.01368080422265,+0.02383751807012,
 -0.00923516894192,+0.01209943150832,+0.02906782189141,+0.01992384905334,
 +0.00197323568330,+0.00017531415423,-0.01796698381949,+0.01887083962858,
 -0.00063335886734,-0.02365277334702,+0.01209445088200,+0.01308086447947,
 +0.01286727242301,-0.11420358975688,-0.01886991700613,+0.00238338728588},
{-0.01100105031759,-0.04250695864938,-0.02554356700969,-0.05473632078607,
 +0.00725906469946,-0.03003724918191,-0.07051526125013,-0.06939439879112,
 -0.00285883056088,+0.05334304124753,+0.12839241846919,-0.05883473754222,
 +0.02424304967487,+0.09134510778469,-0.00226003347193,-0.01280041778462,
 -0.00207988305627,-0.02957493909199,+0.05290385686789,+0.05465710875015},
{-0.01421274522011,+0.02074863337778,-0.01006411985628,+0.03319995456446,
 -0.00005371699269,-0.12266046460835,+0.02419847062899,-0.00441168706583,
 -0.08299118738167,-0.00323230913482,+0.02954035119881,+0.09212856795583,
 +0.00718635627257,-0.02706936115539,+0.04473173279913,-0.01274357634785,
 -0.01395862740618,-0.00071538848681,+0.04767640012830,-0.00729728326990},
{-0.03797680968123,+0.01280286509478,-0.08614616553187,-0.01781049963160,
 +0.00674319990083,+0.04208667754694,+0.05991325707583,+0.03581015660092,
 -0.01529816709967,+0.06885987924922,-0.11719120476535,-0.00014333663810,
 +0.00074336784254,+0.02893416406249,+0.07466151360134,-0.08182016471377,
 -0.06581536577662,-0.00018195976501,+0.00167443595008,+0.09015415667825},
{+0.03577726799591,-0.02139253448219,-0.01137813538175,-0.01954939202830,
 -0.04028242801611,-0.01777500032351,-0.02106862264440,+0.00465199658293,
 -0.02824805812709,+0.06618860061778,+0.08437791757537,-0.02533125946051,
 +0.02806344654855,-0.06970805797879,+0.02328376968627,+0.00692992333282,
 +0.02751392122018,+0.01148722812804,-0.11130404325078,+0.07776346000559},
{-0.06014297925310,-0.00711674355952,-0.02424493472566,+0.00032464353156,
 +0.00321221847573,+0.03257969053884,+0.01072805771161,+0.06892027923996,
 +0.03326534127710,-0.01558838623875,+0.13794237677194,-0.04292623056646,
 +0.01375763233229,-0.11125153774789,+0.03510076081639,-0.04531670712549,
 -0.06170413486351,-0.00182023682123,+0.05979891871679,-0.02551802851059},
{-0.03515069991501,+0.02310847227710,+0.00474493548551,+0.02787717003457,
 -0.12038329679812,+0.03178473522077,+0.04445111601130,-0.05334957493090,
 +0.01290386678474,-0.00376064171612,+0.03996642737967,+0.04777677295520,
 +0.00233689200639,+0.03917715404594,-0.01755598277531,-0.03389088626433,
 -0.02180780263389,+0.00473402043911,+0.01964539477020,-0.01260807237680},
{-0.04120428254254,+0.00062717164978,-0.01688703578637,+0.01685776910152,
 +0.02102702093943,+0.01295781834163,+0.03541815979495,+0.03968150445315,
 -0.02073122710938,-0.06932247350110,+0.11696314241296,-0.00322523765776,
 -0.01280515661402,+0.08717664266126,+0.06297225078802,-0.01290501780488,
 -0.04693925076877,-0.00177653675449,-0.08407812137852,-0.08380714022487},
{+0.03138655228534,-0.09052573757196,+0.00874202219428,+0.06060593729292,
 -0.03426076652151,-0.04832468257386,+0.04735628794421,+0.14504653737383,
 -0.01709111334001,-0.00278794215381,-0.03513813820550,-0.11690294831883,
 -0.00836264902624,+0.03270980973180,-0.02587764129811,+0.01638786059073,
 +0.00485499822497,+0.00305477087025,+0.02295754527195,+0.00616929722958},
{-0.04898722042023,-0.01460879656586,+0.00508708857036,+0.07730497806331,
 +0.04252420017435,+0.00484232580349,+0.09861807969412,-0.05169447907187,
 -0.00917820907880,+0.03679081047330,+0.04998537112655,+0.00769330211980,
 +0.01805447683564,-0.00498723245027,-0.14148416183376,-0.05170281760262,
 -0.03230723310784,-0.00032890672639,-0.02363523071957,+0.03801365471627},
{-0.02047562162108,+0.06933781779590,-0.02101117884731,-0.06841945874842,
 -0.00860967572716,-0.00886650271590,-0.07185241332269,+0.16703684361030,
 -0.00635847581692,+0.00811478913823,+0.01847205842216,+0.06700967948643,
 +0.00596607376199,+0.02318239240593,-0.10552958537847,-0.01980199747773,
 -0.02003785382406,-0.00593392430159,-0.00965391033612,+0.00743094349652}};


/**************************************************************************************************/
//Jones-Taylor-Thornton matrix

class JTT : public DistCalc {
    
public:
    
    JTT(double c) : DistCalc(c) { name = "JTT (Jones-Taylor-Thornton matrix)"; }

    double calcDist(Protein A, Protein B); //calc distance between 2 seqeunces
    string getCitation() { return "https://evolution.gs.washington.edu/phylip/doc/protdist.html, https://evolution.genetics.washington.edu/phylip/credits.html"; }
    
private:
    
    
};

/**************************************************************************************************/
#endif /* jtt_hpp */
