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
		double resolution;

		bool readConfiguration(const char* path);

	protected:

	private:
		void readJSONFile(const char* path);

		char* json;
};

#endif /* CONFIGURATION_H_ */
