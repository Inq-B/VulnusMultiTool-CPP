//some basic imports, fuck knows if these are used either.
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <regex>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>


using namespace std;
using json = nlohmann::json; //readability

//variable declarations, fuck knows if they're used
string VulnusMetaLoaded;
string VulnusMapLoaded;
string OsuMapLoaded;
string SoundSpaceMapLoaded;
string OsuMapData;
string SoundspaceMapData;

//no .split() so i have to do this nastiness.
boost::regex OsuTitleEx("Title:\\w+( \\w+)*$");
boost::regex OsuArtistEx("Artist:\\w*");
boost::smatch OsuTitleMatch;
boost::smatch OsuArtistMatch;

string command;
string metadatafield;
string metadatachange;
string osuArtist;
string osuBytes; 
ifstream VulnusMeta;
ifstream VulnusMap;
ifstream OsuMap;
json VulnusMetaJson;
json VulnusMapJson;

int OsuRegex(string OsuMapData) {
	//yup. that's all this function does; i couldn't find a way to have it return the match as a variable and output it otherwise. 
	//i fucking hate c++, and regex now that i think about it...
	if (boost::regex_search(OsuMapData, OsuArtistMatch, OsuArtistEx)) {
		cout << OsuArtistMatch << "\n";
	}
	if (boost::regex_search(OsuMapData, OsuTitleMatch, OsuTitleEx)) {
		cout << OsuTitleMatch;
	}
	return 0;

}

//json loads for the vulnus meta
json VulnusMetaParse() {
	try {
		VulnusMetaJson = json::parse(VulnusMeta);
		return VulnusMetaJson;
	}
	catch (json::parse_error) {
		cout << "An error occured in meta.json, check your file and try again."; //some basic error handling.
		return false;
	}

}

//vulnus map parsing
json VulnusMapParse() {
	try {
		VulnusMapJson = json::parse(VulnusMap);
		return VulnusMapJson;
	}
	catch (json::parse_error) {
		cout << "\n" << "An error occured in map.json, check your file and try again.\n\n";
		return "An error occured in map.json, check your file and try again.";
	}
}


//basic cli bc i have no idea how to write a proper one
string commandenum(string command) {
	if (command == "h") {
		cout << "\nHere's a (not so) comprehensive list of commands:\nmetadata: Allows you to change a map's metadata.\nexit: Exits the program.\nclear: Clears current output.";
		return "True";
	}
	else if (command == "metadata") {
		//limited to purely the meta.json for now until i can figure out a clean way to implement both functionalities
		if (VulnusMeta) {
			cout << "Enter which field you'd like to edit\n>>";
			cin >> metadatafield;
			boost::algorithm::to_lower(metadatafield);
			metadatafield = "_" + metadatafield;
			cout << "You have chosen: " << metadatafield;
			cout << "\nWhat would you like to change it to?\n>>";
			cin >> metadatachange;
			VulnusMetaJson[metadatafield] = metadatachange;
			ofstream file("meta.json");
			file << VulnusMetaJson;
			file.close();
			return "True";
		}
		else {
			cout << "No Meta file loaded.\n";
			return "False";
		}
	}
	else if (command == "clear") {
		cout << flush;
		system("CLS"); //wont work on linux for now bc i really cba write a separate program for it, sorry <3
	}
	else if (command == "exit") {
		return 0;
		exit;
	}
	else {
		cout << "Invalid command specified, type h for a list of commands \n";
		return "False";
	}
}


//used later, function not currently implemented.
string OsuBytesSubstr(string osuData) {
	size_t HitObjects = osuData.find("[HitObjects]");
	osuBytes = osuData.substr(HitObjects);
	size_t osuNewline = osuBytes.find("\n");
	osuBytes = osuBytes.substr(osuNewline);
	return osuBytes;
}

//initial loading. bully me for the string declarations if you want. its my first time writing anything substantial in c++ & i dont like 1, 0.
string LoadVulnusMeta() {
	try {
		VulnusMeta.open("meta.json");
		
		if (VulnusMeta.fail()) {
			throw false;
		}
		VulnusMetaLoaded = "True";
		VulnusMetaParse();
		return VulnusMetaLoaded;
	}
	catch (bool error) {
		VulnusMetaLoaded = "False";
		return VulnusMetaLoaded;
	}

}

string LoadVulnusMap() {
	try {
		VulnusMap.open("map.json");
		if (VulnusMap.fail()) {
			throw false;
		}
		VulnusMapParse();
		VulnusMapLoaded = "True";
		return VulnusMapLoaded;
	}
	catch (bool error) {
		VulnusMapLoaded = "False";
		return VulnusMapLoaded;
	}

}

string LoadOsuMap(){
	try {
		OsuMap.open("osumap.txt");
		if (OsuMap.fail()) {
			throw false;
		}
		OsuMapLoaded = "True";
		string str;
		ostringstream osuData;
		osuData << OsuMap.rdbuf();
		OsuMapData = osuData.str();
		//OsuBytesSubstr(osuData.str());
		return OsuMapLoaded, OsuMapData;
	}
	catch (bool error) {
		OsuMapLoaded = "False";
		return OsuMapLoaded;
	}
}

string LoadSoundSpaceMap(){
	try{
	ifstream SoundSpaceMap("map.txt");
	if (SoundSpaceMap.fail()) {
		throw false;
	}
	string str;
	ostringstream ss;
	ss << SoundSpaceMap.rdbuf();
	SoundspaceMapData = ss.str();
	SoundSpaceMapLoaded = "True";
	return SoundSpaceMapLoaded;
}
catch (bool error) {
	SoundSpaceMapLoaded = "False";
	return SoundSpaceMapLoaded;
	}
}

//initial loading
string initself() {
	LoadVulnusMeta();
	LoadVulnusMap();
	LoadOsuMap();
	LoadSoundSpaceMap();
	return VulnusMetaLoaded, VulnusMapLoaded, OsuMapLoaded, SoundSpaceMapLoaded;
}

//main
int main()
{
	initself();
	cout << "Vulnus Meta Loaded: " << VulnusMetaLoaded << "\nOsu Map Loaded: " << OsuMapLoaded << "\nSoundspace Map Loaded: " << SoundSpaceMapLoaded;
	cout << "\nVulnus Map Loaded: " << VulnusMapLoaded; //Newline for readability
	while (true) {
		cout << "\n\nEnter a command (h for help)\n>>";
		cin >> command;
		commandenum(command);
	}
}