#pragma once
#include "Message.h"

class Game;

class MessageBuilder
{
public:
	MessageBuilder(void);
	~MessageBuilder(void);

	static Message clMovedToHand(int cardId);
	static Message SvNewPlayer(int id, int team, const std::string& name);
	static Message SvPlayerList(int nbPlayers, const std::string& data);
	static Message SvPlayerYourId(int id);
	static Message SvPlayerCreated(int id, int tmpId);
	static Message SvConnectionResult(int result, int error);
	static Message SvServerFull(const std::string& infos);
	static Message ClConnection(const std::string& nick, bool isLocal);
	static Message svCreateDeck(int nbCards, const std::string& data);
	static Message svPlayerCards(int nbPlayers, int nbCards, const std::string& data);
	static Message svPlayerCards(int PlayerPosition, Game& game);
	static Message svPlayerToPlay(int id);
	static Message svFirstPlayerToPlay(int firstPlayerId);
	static Message clCardToPlay(int cardId);
	static Message svCardToTable(int playerId, int cardId);
	static Message svChangeCardTypeId(int playerId, int cardId, int cardTypeId);
	static Message svChangeTeam(int playerId, int team);
	static Message clNewBid(int value, int color);
	static Message svUpdateBidCombo(int minValue);
};