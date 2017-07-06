#include "Configuration.h"
#include <stdio.h>
using namespace std;

int main(int argc, char ** argv)
{
	Configuration* config = new Configuration();

	const char* path = "/home/user/Desktop/Configuration.json";

	config->readConfiguration(path);

	printf("%f , %f , %f , %f , %f , %f", config->startX, config->startY, config->startYaw, config->destX, config->destY, config->resolution );

}
