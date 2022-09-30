#include "Helper.h"



long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);

		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'c': //-c (Country)
				switch (argv[i][2])
				{
				case 's':
					lRet = glb.propertyBag.SetCountrySize(atof(&argv[i][4])); //-cs
					break;
				case 'p':
					lRet = glb.propertyBag.SetPopulationSize(atol(&argv[i][4])); //-cp
					break;
				case 'i':
					lRet = glb.propertyBag.SetStartSick(atol(&argv[i][4])); //-ci
					break;
				case 'c':
					lRet = glb.propertyBag.SetCycleTime(atol(&argv[i][4])); //-cc
					break;
				case 'f':
					lRet = glb.propertyBag.SetFamilyUnit(atol(&argv[i][4])); //-cf
					break;
				case 'r':
					lRet = glb.propertyBag.SetRateOfDifferentWorkAddress(atol(&argv[i][4])); //-cr
					break;
				case 'm':
					lRet = glb.propertyBag.SetMovingSpeed(atof(&argv[i][4])); //-cm
					break;
				default:
				{
					printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
					return PARSE_COMMAND_LINE;
				}
				}		
				break;
			case 's': //-s (City)
				switch (argv[i][2])
				{
				case 'a':
					lRet = glb.propertyBag.SetCitiesAmount(atol(&argv[i][4])); //-sa
					break;
				case 'n':
					lRet = glb.propertyBag.SetCitiesName(&argv[i][4]); //-sn
					break;
				case 's':
					lRet = glb.propertyBag.SetCitiesSizes(&argv[i][4]); //-ss
					break;
				default:
				{
					printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
					return PARSE_COMMAND_LINE;
				}
				}		
				break;
			case 'h'://-h (Human)
				switch (argv[i][2])
				{
				case 'i':
					lRet = glb.propertyBag.SetInfectionRate(atol(&argv[i][4])); //-hi 
					break;
				case 'd':
					lRet = glb.propertyBag.SetSickDuration(atol(&argv[i][4])); //-hd
					break;
				case 'k':
					lRet = glb.propertyBag.SetDeathRate(atol(&argv[i][4])); //-hk
				default:
				{
					printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
					return PARSE_COMMAND_LINE;
				}

				}
				break;
			case 'p':
				switch (argv[i][2])
				{
				case 't':
					lRet = glb.propertyBag.SetTreadBase(atol(&argv[i][4])); //-pt
					break;
				case 'c':
					lRet = glb.propertyBag.SetThreadOverlapCheck(atol(&argv[i][4])); //-pc
					break;
				case 'd':
					lRet = glb.propertyBag.SetSimulationDuration(atol(&argv[i][4])); //-pd
					break;
				default:
				{
					printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
					return PARSE_COMMAND_LINE;
				}
				}
				break;

			default:
			{
				printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
				return PARSE_COMMAND_LINE;
			}
			}
		}
		else if (*argv[i] == '?')
		{
			//print arguments: For the user to see
			printf_s(
				"This are the commands you can use within ZombieSim%s"
				"-cs <PlayfieldSize>\t\t Set the size of the country %s"
				"-cp <PopulationSize> \t\t Set the size of your human population%s"
				"-ci <AmountSickAtStart> \t\t Set the sick population at time 0 %s"
				"-cc <CycleTimeInSeconds> \t\t Set the CycleTime, aka how many seconds = 1 cycle %s"
				"\t\t\t Half the time humans are in work mode other half is in home mode %s"
				"-cf <FamilyUnitNumber> \t\t How Many humans in one family unit %s"
				"-cr <RateOfWorkInDifferentCity> \t\t How many humans work in a different city as they live %s"
				"-cm <MovingSpeed> \t\t How fast human objects move between Work and Home, independent on fps %s"
				"-sa <HowManyCities> \t\t Amount of cities in Country %s"
				"-sn <CityName1,CityName2> \t\t Names of the Cities comma delimited %s"
				"-ss <Citiessizes> \t\t Size of cities, comma delimeted %s"
				"-hi <InfectionRate> \t\t Infection Rate in full numbers aka between 1 and 100 will be translated to percentage e.g. 10 %s"
				"-hd <SickDuration> \t\t Full numbers based on how many cycles it takes to get healthy again. %s"
				"-pt <ThreadBase> \t\t How many threads are made divide the workload %s"
				"-pc <OverlapCheck> \t\t how many threads are made to divide workload of overlap check that the threadbase calls %s"
				"-pd <SimulationDuration> \t\t full numbers will be translated into minutes %s%s"//Extra line for clarity
				, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL, EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return PARSE_COMMAND_LINE;
	}
	return lRet;
}


long InitProgramVariables(int argc, char* argv[], char* envp[])
{
	long lReturn = ERR_NOERROR;
	lReturn = glb.propertyBag.ReadProperties("ZombieSim.conf");
	if (lReturn != ERR_NOERROR)
		return lReturn;

	lReturn = ParseCommandLine(argc, argv, envp);
	if (lReturn != ERR_NOERROR)
		return lReturn;

	return lReturn;
}