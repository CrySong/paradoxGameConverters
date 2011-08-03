#include <fstream>
#include "Parsers\Parser.h"
#include "Log.h"
#include "V2World.h"

static inline Object* doParseFile(const char* filename)
{
	ifstream	read;				// ifstream for reading files

	initParser();
	Object* obj = Parser::topLevel;
	read.open(filename); 
	if (!read.is_open())
	{
		log("Error: Could not open %s\n", filename);
		printf("Error: Could not open %s\n", filename);
		exit(1);
	}
	readFile(read);  
	read.close();
	read.clear();

	return obj;
}

static inline void AddCategoryToRegimentTypeMap(Object* obj, RegimentCategory category, string categoryName, RegimentTypeMap& rtm)
{
	vector<Object*> top = obj->getValue(categoryName);
	if (top.size() != 1)
	{
		log("Error: could not get regiment type map for %s", categoryName);
		printf("Error: could not get regiment type map for %s", categoryName);
		exit(1);
	}
	vector<Object*> types = top[0]->getLeaves();
	if (types.size() == 0)
	{
		log("Error: no regiment types to map for %s", categoryName);
		printf("Error: no regiment types to map for %s", categoryName);
		exit(1);
	}
	for (vector<Object*>::iterator itr = types.begin(); itr != types.end(); ++itr)
	{
		string type = (*itr)->getKey();
		string strength = (*itr)->getLeaf();
		rtm[type] = pair<RegimentCategory, int>(category, atoi(strength.c_str()));
	}
}

int main(int argc, char * argv[]) //changed from TCHAR, no use when everything else in program is in ASCII...
{
	initLog();

	Object*	obj;				// generic object
	ifstream	read;				// ifstream for reading files


	//Get V2 install location
	string V2Loc;
	if (argc >= 2)
	{
		V2Loc = argv[1];
		log("V2Installed at %s\n", V2Loc.c_str());
	}
	else
	{
		log("Need to specify V2 install location on command line.\n");
		printf("Need to specify V2 install location on command line.\n");
		return 1;
	}

	//Get Input EU3 save 
	string inputFilename("input.eu3");
	if (argc >= 3)
	{
		inputFilename = argv[2];
		log("Using input file %s.\n", inputFilename.c_str());
		printf("Using input file %s.\n", inputFilename.c_str());
	}
	else
	{
		log("No input file given, defaulting to input.eu3\n");
		printf("No input file given, defaulting to input.eu3\n");
	}


	// Parse EU3 Save
	log("Parsing EU3 save.\n");
	printf("Parsing EU3 save.\n");
	
	initParser();
	obj = Parser::topLevel;
	read.open(inputFilename.c_str());
	if (!read.is_open())
	{
		log("Error: Could not open EU3 save (%s).\n", inputFilename.c_str());
		printf("Error: Could not open EU3 save (%s).\n", inputFilename.c_str());
		return 1;
	}
	readFile(read);
	read.close();
	read.clear();
	EU3World sourceWorld;
	sourceWorld.init(obj);


	// Resolve unit types
	log("Resolving unit types.\n");
	printf("Resolving unit types.\n");
	RegimentTypeMap rtm;
	obj = doParseFile("unit_strength.txt");
	for (int i = 0; i < num_reg_categories; ++i)
	{
		AddCategoryToRegimentTypeMap(obj, (RegimentCategory)i, RegimentCategoryNames[i], rtm);
	}
	sourceWorld.resolveRegimentTypes(rtm);


	// Parse V2 input file
	log("Parsing Vicky2 data.\n");
	printf("Parsing Vicky2 data.\n");
	V2World destWorld;
	destWorld.init(V2Loc);


	// Parse province mappings
	log("Parsing province mappings.\n");
	printf("Parsing province mappings.\n");
	obj = doParseFile("province_mappings.txt");
	provinceMapping provinceMap = initProvinceMap(obj);


	// Get potential V2 countries
	log("Getting potential V2 nations.\n");
	printf("Getting potential V2 nations.\n");
	vector<string> V2Tags;
	ifstream V2CountriesInput;
	V2CountriesInput.open( (V2Loc + "\\common\\countries.txt").c_str() );
	if (!V2CountriesInput.is_open())
	{
		log("Error: Could not open countries.txt\n");
		printf("Error: Could not open countries.txt\n");
		return 1;
	}
	destWorld.addPotentialCountries(V2CountriesInput, V2Loc);
	V2CountriesInput.close();
	
	// Get list of blocked nations
	log("Getting blocked V2 nations.\n");
	printf("Getting blocked V2 nations.\n");
	obj = doParseFile("blocked_nations.txt");
	vector<string> blockedNations = processBlockedNations(obj);

	// Get country mappings
	log("Parsing country mappings.\n");
	printf("Parsing country mappings.\n");
	initParser();
	obj = doParseFile("country_mappings.txt");	

	// Map EU3 nations to V2 nations
	log("Mapping EU3 nations to V2 nations.\n");
	printf("Mapping EU3 nations to V2 nations.\n");
	uniteJapan(sourceWorld);
	removeEmptyNations(sourceWorld);
	removeDeadLandlessNations(sourceWorld);
	countryMapping countryMap;
	vector<string> EU3Tags = getEU3Tags(sourceWorld);
	int leftoverNations = initCountryMap(countryMap, EU3Tags, destWorld.getPotentialTags(), blockedNations, obj);
	if (leftoverNations == -1)
	{
		return 1;
	}
	else if (leftoverNations > 0)
	{
		log("Too many EU3 nations (%d). Removing older landless nations.\n", leftoverNations);
		printf("Too many EU3 nations (%d). Removing older landless nations.\n", leftoverNations);
		removeOlderLandlessNations(sourceWorld, leftoverNations);
		EU3Tags = getEU3Tags(sourceWorld);
		leftoverNations = initCountryMap(countryMap, EU3Tags, destWorld.getPotentialTags(), blockedNations, obj);
	}
	if (leftoverNations == -1)
	{
		return 1;
	}
	else if (leftoverNations > 0)
	{
		log("Too many EU3 nations (%d). Removing landless nations.\n", leftoverNations);
		printf("Too many EU3 nations (%d). Removing landless nations.\n", leftoverNations);
		removeLandlessNations(sourceWorld);
		EU3Tags = getEU3Tags(sourceWorld);
		leftoverNations = initCountryMap(countryMap, EU3Tags, destWorld.getPotentialTags(), blockedNations, obj);
	}
	
	log("Final Country maps:\n");
	for (countryMapping::iterator i = countryMap.begin(); i != countryMap.end(); i++)
	{
		log("\t%s -> %s\n", i->first.c_str(), i->second.c_str());
	}


	// Generate region mapping
	log("Parsing region structure.\n");
	printf("Parsing region structure.\n");

	obj = doParseFile( (V2Loc + "\\map\\region.txt").c_str() );
	if (obj->getLeaves().size() < 1)
	{
		log("This is where a TODO: error was. If you see this, bug a programmer so we can put in a useful message.\n");
		printf("This is where a TODO: error was. If you see this, bug a programmer so we can put in a useful message.\n");
		return 1;
	}
	stateMapping stateMap;
	stateMap = initStateMap(obj);


	// Parse Culture Mappings
	log("Parsing culture mappings.\n");
	printf("Parsing culture mappings.\n");

	obj = doParseFile("cultureMap.txt");
	if (obj->getLeaves().size() < 1)
	{
		log("Error: Failed to parse cultureMap.txt.\n");
		printf("Error: Failed to parse cultureMap.txt.\n");
		return 1;
	}
	cultureMapping cultureMap;
	cultureMap = initCultureMap(obj->getLeaves()[0]);

	// Parse Religion Mappings
	log("Parsing religion mappings.\n");
	printf("Parsing religion mappings.\n");

	obj = doParseFile("religionMap.txt");
	if (obj->getLeaves().size() < 1)
	{
		log("Error: Failed to parse religionMap.txt.\n");
		printf("Error: Failed to parse religionMap.txt.\n");
		return 1;
	}
	religionMapping religionMap;
	religionMap = initReligionMap(obj->getLeaves()[0]);


	//Parse unions mapping
	log("Parsing union mappings.\n");
	printf("Parsing union mappings.\n");
	obj = doParseFile("unions.txt");
	if (obj->getLeaves().size() < 1)
	{
		log("Error: Failed to parse unions.txt.\n");
		printf("Error: Failed to parse unions.txt.\n");
		return 1;
	}
	unionMapping unionMap;
	unionMap = initUnionMap(obj->getLeaves()[0]);


	//Parse government mapping
	log("Parsing governments mappings.\n");
	printf("Parsing governments mappings.\n");

	initParser();
	obj = doParseFile("governmentMapping.txt");
	governmentMapping governmentMap;
	governmentMap = initGovernmentMap(obj->getLeaves()[0]);

	// Convert
	printf("Converting countries.\n");
	log("Converting countries.\n");
	destWorld.convertCountries(sourceWorld, countryMap, cultureMap, religionMap, governmentMap);
	printf("Converting diplomacy.\n");
	log("Converting diplomacy.\n");
	destWorld.convertDiplomacy(sourceWorld, countryMap);
	printf("Converting provinces.\n");
	log("Converting provinces.\n");
	destWorld.convertProvinces(sourceWorld, provinceMap, countryMap, cultureMap, religionMap);
	printf("Adding unions.\n");
	log("Adding unions.\n");
	destWorld.addUnions(unionMap);
	printf("Converting capitals.\n");
	log("Converting capitals.\n");
	destWorld.convertCapitals(sourceWorld, provinceMap);
	printf("Converting armies and navies.\n");
	log("Converting armies and navies.\n");
	destWorld.convertArmies(sourceWorld, provinceMap);
	printf("Converting states.\n");
	log("Converting states.\n");
	destWorld.setupStates(stateMap);
	printf("Converting techs.\n");
	log("Converting techs.\n");
	destWorld.convertTechs(sourceWorld);
	  
	/*InstructionsParser insParser;
	InstructionsParser::Refresh();
	read.open(string("ins.txt").c_str());
	if (!read.is_open())
	{
		log("Error: Could not open ins.txt\n");
		printf("Error: Could not open ins.txt\n");
		return 1;
	}
	readInsFile(read);	
	read.close();
	read.clear();

	VariableCalculator::Instance()->SetWorlds(&euWorld, &vickyWorld);
	VariableCalculator::Instance()->ProcessVariables(InstructionsParser::GetProcessedVars());
	VariableCalculator::Instance()->ProcessRules(InstructionsParser::GetProcessedRulesets());
	InstructionsParser::Refresh();*/

	// Output results
	printf("Outputting save.\n");
	log("Outputting save.\n");
	FILE* output;
	if (fopen_s(&output, "output.v2", "w") != 0)
	{
		log("Error: could not open output.v2.\n");
		printf("Error: could not open output.v2.\n");
	}
	destWorld.output(output);
	fclose(output);

	closeLog();
	return 0;
}

