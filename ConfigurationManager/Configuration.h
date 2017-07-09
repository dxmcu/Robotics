/*
 * @file	Configuration.h
 *
 * @author	Tomer Sulimany
 * @since	06/07/2017
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

using namespace std;

class Configuration
{
	public:
		double startX;
		double startY;
		double startYaw;
		double destX;
		double destY;
		double size;
		double resolution;

		bool readConfiguration(const char* path);

	private:
		bool readJSONFile(const char* path);

		char* json;
		static const int NUMBER_OF_DEFINES = 7;
};

#endif /* CONFIGURATION_H_ */
