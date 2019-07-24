#pragma once

#include "Message.h"
#include "tools/Singleton.h"

class GameEngine;
class GraphicEngine;
class SoundEngine;
class NetworkEngine;

class MessageSender : public Singleton<MessageSender>
{
public:

	void linkGameEngine(GameEngine* game);
	//void linkGraphicEngine(GraphicEngine* graphic){myGraphicEngine = graphic;}
	//void linkSoundEngine(SoundEngine* sound){mySoundEngine = sound;}
	//void linkNetworkEngine(NetworkEngine* network){myNetworkEngine = network;}

	//void sendMessage(const Message& msg);
	void sendMessageToGame(const Message& msg);
	void sendMessageToGraphic(const Message& msg);
	void sendMessageToSound(const Message& msg);
	void sendMessageToNetwork(const Message& msg);

protected:
	GameEngine* myGameEngine;
	//GraphicEngine* myGraphicEngine;
	//SoundEngine* mySoundEngine;
	//NetworkEngine* myNetworkEngine;
};

