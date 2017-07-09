/*
 * @file	Configuration.cpp
 *
 * @author	Tomer Sulimany
 * @since	06/07/2017
 */

// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

#include "Configuration.h"
#include "JSON.h"

bool Configuration::readJSONFile(const char* path)
{
	FILE *fp;
	long lSize;

	fp = fopen ( path, "rb" );
	if( !fp )
		return false;

	fseek( fp , 0L , SEEK_END);
	lSize = ftell( fp );
	rewind( fp );

	/* allocate memory for entire content */
	json = reinterpret_cast<char*>(calloc( 1, lSize+1 ));
	if( !json )
	{
		fclose(fp);
		return false;
	}

	/* copy the file into the buffer */
	if( 1!=fread( json, lSize, 1 , fp) )
	{
		fclose(fp);
		free(json);
		return false;
	}

	/* do your work here, buffer is a string contains the whole text */

	fclose(fp);
	return true;
}

/**
 * Reads configuration file into configuration object.
 * Returns false if read failed or not all values present in configuration file.
 *
 * Param - char* path to the configuration file.
 */
bool Configuration::readConfiguration(const char* path)
{
	// Properties count
	int count = 0;

	if (!readJSONFile(path))
	{
		printf("Failed to read file\n");
		return false;
	}

	// Parse example data
	JSONValue *value = JSON::Parse(json);

	// Did it go wrong?
	if (value == NULL)
	{
		printf("Failed to parse file\n");
		return false;
	}


	// Retrieve the main object
	JSONObject root;
	if (value->IsObject() == false)
		return false;

	root = value->AsObject();

	if(root.find(L"startX") != root.end() && root[L"startX"]->IsNumber())
	{
		startX = root[L"startX"]->AsNumber();
		count++;
	}

	if(root.find(L"startY") != root.end() && root[L"startY"]->IsNumber())
	{
		startY = root[L"startY"]->AsNumber();
		count++;
	}

	if(root.find(L"startYaw") != root.end() && root[L"startYaw"]->IsNumber())
	{
		startYaw = root[L"startYaw"]->AsNumber();
		count++;
	}

	if(root.find(L"destX") != root.end() && root[L"destX"]->IsNumber())
	{
		destX = root[L"destX"]->AsNumber();
		count++;
	}

	if(root.find(L"destY") != root.end() && root[L"destY"]->IsNumber())
	{
		destY = root[L"destY"]->AsNumber();
		count++;
	}

	if(root.find(L"robotSize") != root.end() && root[L"robotSize"]->IsNumber())
	{
		size = root[L"robotSize"]->AsNumber();
		count++;
	}

	if(root.find(L"resolution") != root.end() && root[L"resolution"]->IsNumber())
	{
		resolution = root[L"resolution"]->AsNumber();
		count++;
	}

	if(count != NUMBER_OF_DEFINES)
		return false;

	return true;
}


