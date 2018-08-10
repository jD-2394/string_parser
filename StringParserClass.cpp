/*
 * StringParserClass.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: Joseph Doye
 */

#include <string>
#include <string.h>
#include <cstring>

#include <iostream>
#include "../327_proj3_test/includes/StringParserClass.h"
#include "../327_proj3_test/includes/constants.h";

KP_StringParserClass::StringParserClass::StringParserClass(void) { // initialize all variables
	areTagsSet = false;
	pStartTag = new char(0);
	pEndTag = new char (0);
}

//call cleanup to release any allocated memory
KP_StringParserClass::StringParserClass::~StringParserClass(void) {
	cleanup();
}

//these are the start tag and the end tags that we want to find,
//presumably the data of interest is between them, please make a
//COPY of what pStartTag and pEndTag point to.  In other words
//DO NOT SET pStartTag = pStart
//returns:
//SUCCESS
//ERROR_TAGS_NULL if either pStart or pEnd is null
int KP_StringParserClass::StringParserClass::setTags(const char *pStart,
		const char *pEnd) {
	if (pStart == NULL || pEnd == NULL) {// if either tag is null return error code ERROR_TAGS_NULL
		return ERROR_TAGS_NULL;
	}
	// create a copy of the Start and End Tags Respectively
	pStartTag = strdup(pStart);
	pEndTag = strdup(pEnd);
	areTagsSet = true;// set the flag that the tags are set so the other functions can execute
	return SUCCESS;

}

//First clears myVector
//going to search thru pDataToSearchThru, looking for info bracketed by
//pStartTag and pEndTag, will add that data to myStrings
//returns
//SUCCESS  finished searching for data between tags, results in myStrings (0 or more entries)
//ERROR_TAGS_NULL if either pStart or pEnd is null
//ERROR_DATA_NULL pDataToSearchThru is null
int KP_StringParserClass::StringParserClass::getDataBetweenTags(
		char *pDataToSearchThru, std::vector<std::string> &myVector) {
	myVector.clear(); // vector should be cleared each time function is called to avoid interference with previous outputs
	if (!areTagsSet) {// return error code ERROR_TAGS_NULL if either of the tags are not set before invoking this method
		return ERROR_TAGS_NULL;
	}
	if (pDataToSearchThru == NULL) { // return ERROR_DATA_NULL if there is no data to search through
		return ERROR_DATA_NULL;
	}
	bool areAllTagsFound = false;
	while (!areAllTagsFound) {// keeps the function finding more nonces until there are no more tags to find contents of
		std::string sNonce;
		char *pEnd = (pStartTag + strlen(pStartTag) - 1); // gets the end of the pStartTag
		char *pStart = 0;
		char* pNonceStart = 0;
		char* pNonceEnd = 0;
		int iret = findTag(pStartTag, pDataToSearchThru, pEnd); // this checks if first tag can be found

		if (iret == SUCCESS) {// if the first tag can be found, find the second, else break the loop same rule applies for the end tag
			pNonceStart = pEnd; // pNonceStart recieves the nonce (with the end still attached)
			pStart = pNonceStart;
			int iret = findTag(pEndTag, pStart, pEnd); // finds the end tag
			if(iret == FAIL){
				break;
			}
		} else {
			break;
		}

		pNonceEnd = pStart;
		sNonce.assign(pNonceStart, pNonceEnd - (pNonceStart));// assign the nonce with the tag and any remaining unwanted elements removed
		pDataToSearchThru = pStart + strlen(pEndTag);// set the current amount of data to the remaining amount of data to search through
		myVector.push_back(sNonce);// add the nonce to the end of the vector
		iret = findTag(pStartTag, pDataToSearchThru, pEnd);// check once more for any new tags
		if (iret != 0) {// if we can't find any tags we can exit the loop
			areAllTagsFound = true;
		}
	}

	return SUCCESS;
}
void KP_StringParserClass::StringParserClass::cleanup() { // clears all private variables in class
	delete pStartTag;
	delete pEndTag;
	areTagsSet = false;

}

//Searches a string starting at pStart for pTagToLookFor
//returns:
//SUCCESS  found pTagToLookFor, pStart points to beginning of tag and pEnd points to end of tag
//FAIL did not find pTagToLookFor and pEnd points to 0
//ERROR_TAGS_NULL if either pStart or pEnd is null
int KP_StringParserClass::StringParserClass::findTag(char *pTagToLookFor,
		char *&pStart, char *&pEnd) {
	if (!areTagsSet) {// return ERROR_TAGS_NULL if either tag not set (tags were already checked for null at set tags)
		return ERROR_TAGS_NULL;
	}
	pEnd = 0; // assume we wont find it.

	int iLen = strlen(pStart);// length of the start tag
	int lenTag = strlen(pTagToLookFor); // get the lengthof the tag we want
	for (int i = 0; i < iLen; i++) {// iterate through our data until we find a match of the tag we want
		if (*(pStart) == *(pTagToLookFor)) {
			if (strncmp(pStart, pTagToLookFor, lenTag) == 0) {// if the current set of characters match the tag we want we point to the
				pEnd = pStart + lenTag;// set pEnd to the end of the tag
				return SUCCESS;
			}
		}
		pStart++;
	}
	return FAIL;
}
