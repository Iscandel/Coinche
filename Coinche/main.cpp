#include "CommonInterface.h"
#include "Tools/Logger.h"

#ifdef _DEBUG 
#pragma comment(lib,"sfml-system-s-d.lib")
#pragma comment(lib,"sfml-graphics-s-d.lib")
#pragma comment(lib,"sfml-window-s-d.lib")
#pragma comment(lib,"sfml-main-d.lib")
#pragma comment(lib,"sfml-network-s-d.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"sfml-audio-s-d.lib")
#pragma comment(lib, "./Dependencies/Gaia-s-d.lib")
#pragma comment(lib, "./Dependencies/XMLLoader-s-d.lib")
#pragma comment(lib, "./Dependencies/SFMLRenderer-s-d.lib")
#else
#pragma comment(lib,"sfml-system-s.lib")
#pragma comment(lib,"sfml-graphics-s.lib")
#pragma comment(lib,"sfml-window-s.lib")
#pragma comment(lib,"sfml-network-s.lib")
#pragma comment(lib,"sfml-audio-s.lib")
//#pragma comment(lib,"tinyxml_STL.lib")
#pragma comment(lib,"sfml-main.lib")
#endif
#include "CreationFichierRessources.h"
int main()
{
	try
	{
		//std::ofstream out("cardsTest.txt");
		//out << "#nbCards" << std::endl;
		//out << 52 << std::endl;
		//out << "#nbJokers" << std::endl;
		//out << 2 << std::endl;
		//out << "#nbFakeCards" << std::endl;
		//out << 1;
		//out << "#always ID - color - value - imageID" << std::endl;
		//
		//int id = 0;
		//for(int i = 0; i < 13; i++)
		//{
		//	out << "#" << id << std::endl;
		//	out << id << std::endl; 
		//	out << 0 << std::endl;
		//	out << i + 1 << std::endl;
		//	out << 27 + i << std::endl;
		//	out << 55 << std::endl;
		//	id++;
		//}
		//for(int i = 0; i < 13; i++)
		//{
		//	out << "#" << id << std::endl;
		//	out << id << std::endl; 
		//	out << 1 << std::endl;
		//	out << i + 1 << std::endl;
		//	out << 40 + i << std::endl;
		//	out << 55 << std::endl;
		//	id++;
		//}
		//for(int i = 0; i < 13; i++)
		//{
		//	out << "#" << id << std::endl;
		//	out << id << std::endl; 
		//	out << 2 << std::endl;
		//	out << i + 1 << std::endl;
		//	out << 1 + i << std::endl;
		//	out << 55 << std::endl;
		//	id++;
		//}
		//for(int i = 0; i < 13; i++)
		//{
		//	out << "#" << id << std::endl;
		//	out << id << std::endl; 
		//	out << 3 << std::endl;
		//	out << i + 1 << std::endl;
		//	out << 14 + i << std::endl;
		//	out << 55 << std::endl;
		//	id++;
		//}

		//out.close();
		
		ILogger::setLogger(new ConsoleLog(std::cout));
		ILogger::setLogLevel(ILogger::ALL);
		//utiles::creerGuiDat();
		CommonInterface comInterface;
		comInterface.run();
	}
	catch(const std::exception& e) //sometimes there's a pb.Exception not caught in main...
	{
		ILogger::log(ILogger::ERRORS) << e.what() << "\n";
		std::cout << e.what() << "lol";
		getchar();
	}
	catch(...)
	{
		ILogger::log(ILogger::ERRORS) << "Unknown exception...\n";
		std::cout << "lol";
		getchar();
	}
	return 0;
}