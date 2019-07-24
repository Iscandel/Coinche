#include "MessageSender.h"

#include "GameEngine.h"
#include "GraphicEngine.h"
#include "SoundEngine.h"
#include "NetworkEngine.h"

void MessageSender::linkGameEngine(GameEngine* game)
{
	myGameEngine = game;
}

//void MessageSender::linkGraphicEngine(GraphicEngine* graphic)
//{
//	myGraphicEngine = graphic;
//}
//
//void MessageSender::linkSoundEngine(SoundEngine* sound)
//{
//	mySoundEngine = sound;
//}
//
//void MessageSender::linkNetworkEngine(NetworkEngine* network)
//{
//	myNetworkEngine = network;
//}

//void MessageSender::sendMessage(const Message& msg)
//{
//}

void MessageSender::sendMessageToGame(const Message& msg)
{
	myGameEngine->sendMessageToGame(msg);
}

void MessageSender::sendMessageToGraphic(const Message& msg)
{
	myGameEngine->sendMessageToGraphic(msg);
}

void MessageSender::sendMessageToSound(const Message& msg)
{
	myGameEngine->sendMessageToSound(msg);
}

void MessageSender::sendMessageToNetwork(const Message& msg)
{
	myGameEngine->sendMessageToNetwork(msg);
}
