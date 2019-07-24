#include "MessageBuilder.h"

#include "CardDefManager.h"
#include "Game.h"

#include <sstream>

MessageBuilder::MessageBuilder(void)
{
}


MessageBuilder::~MessageBuilder(void)
{
}

Message MessageBuilder::clMovedToHand(int cardId)
{
	Message msg;
	msg.type = Message::MessageType::CL_CARD_MOVED_TO_HAND;
	msg.iData[Message::Key::ClCardMovedToHand::CARD_ID] = cardId;

	return msg;
}

Message MessageBuilder::SvNewPlayer(int id, int team, const std::string& name)
{
	Message msg;

	msg.type = Message::MessageType::SV_NEW_PLAYER;
	msg.iData[Message::Key::SvNewPlayer::ID] = id;
	msg.iData[Message::Key::SvNewPlayer::TEAM] = team;
	msg.sData[Message::Key::SvNewPlayer::NAME] = name;

	return msg;
}

Message MessageBuilder::SvPlayerList(int nbPlayers, const std::string& data)
{
	Message msg;

	msg.type = Message::MessageType::SV_PLAYER_LIST;
	msg.iData[Message::Key::SvPlayerList::NB_PLAYERS] = nbPlayers;
	msg.sData[Message::Key::SvPlayerList::LIST] = data;

	return msg;
}

Message MessageBuilder::SvPlayerYourId(int id)
{
	Message msg;

	msg.type = Message::MessageType::SV_PLAYER_YOUR_ID;
	msg.iData[Message::Key::SvPlayerYourId::ID] = id;

	return msg;
}

Message MessageBuilder::SvPlayerCreated(int id, int tmpId)
{
	Message msg;

	msg.type = Message::MessageType::SV_PLAYER_CREATED;
	msg.iData[Message::Key::SvPlayerCreated::ID] = id;
	msg.iData[Message::Key::SvPlayerCreated::TMP_ID] = tmpId;

	return msg;
}

Message MessageBuilder::SvConnectionResult(int result, int error)
{
	Message msg;

	msg.type = Message::MessageType::SV_CONNECTION_RESULT;
	msg.iData[Message::Key::SvConnectionResult::RESULT] = result;
	msg.iData[Message::Key::SvConnectionResult::ERROR_CONNEC] = error;

	return msg;
}

Message MessageBuilder::SvServerFull(const std::string& infos)
{
	Message msg;

	msg.type = Message::MessageType::SV_SERVER_FULL;
	msg.sData[Message::Key::SvServerFull::INFO] = infos;

	return msg;
}

Message MessageBuilder::ClConnection(const std::string& nick, bool isLocal)
{
	Message msg;

	msg.type = Message::MessageType::CL_CONNECTION;
	msg.sData[Message::Key::ClConnection::NAME] = nick;
	int val = isLocal ? 1 : 0;
	msg.iData[Message::Key::ClConnection::IS_LOCAL_CLIENT] = (int) val;
	return msg;
}

Message MessageBuilder::svCreateDeck(int nbCards, const std::string& data)
{
	Message msg;

	msg.type = Message::MessageType::SV_CREATE_DECK;
	msg.iData[Message::Key::SvCreateDeck::NB_CARDS] = nbCards;
	msg.sData[Message::Key::SvCreateDeck::DATA] = data;

	return msg;
}

Message MessageBuilder::svPlayerCards(int nbPlayers, int nbCards, const std::string& data)
{
	Message msg;

	msg.type = Message::MessageType::SV_PLAYER_CARDS;
	msg.iData[Message::Key::SvPlayerCards::NB_CARDS] = nbCards;
	msg.iData[Message::Key::SvPlayerCards::NB_PLAYERS] = nbPlayers;
	msg.sData[Message::Key::SvPlayerCards::DATA] = data;

	return msg;
}

Message MessageBuilder::svPlayerCards(int playerPosition, Game& game)
{
	std::ostringstream oss;

	std::vector<std::ostringstream> tmp;
	tmp.resize(game.getNumberPlayers());

	std::vector<std::ostringstream> fakeTmp;
	fakeTmp.resize(game.getNumberPlayers());

	for(int i = 0; i < game.getNumberPlayers(); i++)
	{
		PtrPlayer player = game.getPlayerByPosition(i);
		std::vector<PtrCard>& cards = player->getCards();

		int fakeCardId = CardDefManager::getInstance()->getFakeCardTypeId();

		//For all the player, a correct version of their cards + a fake version
		for(int j = 0; j < cards.size(); j++)
		{
			tmp[i] << cards[j]->getId() << " " << cards[j]->getTypeId() << " ";
			fakeTmp[i] << cards[j]->getId() << " "<< fakeCardId << " ";
		}
	}

	for(int j = 0; j < game.getNumberPlayers(); j++)
	{
		PtrPlayer player = game.getPlayerByPosition(j);
		oss << player->getId() << " "; 

		if(playerPosition == j)
			oss << tmp[j].str();
		else
			oss << fakeTmp[j].str();
	}

	Message msg;

	msg.type = Message::MessageType::SV_PLAYER_CARDS;
	msg.iData[Message::Key::SvPlayerCards::NB_CARDS] = game.getPlayerByPosition(0)->getCards().size();
	msg.iData[Message::Key::SvPlayerCards::NB_PLAYERS] = game.getNumberPlayers();;
	msg.sData[Message::Key::SvPlayerCards::DATA] = oss.str();

	return msg;
}

Message MessageBuilder::svPlayerToPlay(int id)
{
	Message msg;
	msg.type = Message::MessageType::SV_PLAYER_TO_PLAY;
	msg.iData[Message::Key::SvPlayerToPlay::PLAYER_ID] = id;

	return msg;
}

Message MessageBuilder::svFirstPlayerToPlay(int firstPlayerId)
{
	Message msg;
	msg.type = Message::MessageType::SV_FIRST_PLAYER_TO_PLAY;
	msg.iData[Message::Key::SvFirstPlayerToPlay::FIRST_PLAYER_ID] = firstPlayerId;

	return msg;
}
	static Message svFirstPlayerToPlay(int firstPlayerId);
Message MessageBuilder::clCardToPlay(int cardId)
	{
	Message msg;
	msg.type = Message::MessageType::CL_PLAY_CARD;
	msg.iData[Message::Key::clCardToPlay::CARD_ID] = cardId;

	return msg;
}

Message MessageBuilder::svCardToTable(int playerId, int cardId)
{
	Message msg;
	msg.type = Message::MessageType::SV_CARD_TO_TABLE;
	msg.iData[Message::Key::svCardToTable::PLAYER_ID] = playerId;
	msg.iData[Message::Key::svCardToTable::CARD_ID] = cardId;

	return msg;
}

Message MessageBuilder::svChangeCardTypeId(int playerId, int cardId, int cardTypeId)
{
	Message msg;
	msg.type = Message::MessageType::SV_CHANGE_CARD_TYPE_ID;
	msg.iData[Message::Key::svChangeCardTypeId::PLAYER_ID] = playerId;
	msg.iData[Message::Key::svChangeCardTypeId::CARD_ID] = cardId;
	msg.iData[Message::Key::svChangeCardTypeId::CARD_TYPE_ID] = cardTypeId;

	return msg;
}

Message MessageBuilder::svChangeTeam(int playerId, int team)
{
	Message msg;
	msg.type = Message::MessageType::SV_CHANGE_TEAM;
	msg.iData[Message::Key::svChangeTeam::ID_PLAYER] = playerId;
	msg.iData[Message::Key::svChangeTeam::TEAM_ID] = team;

	return msg;
}

Message MessageBuilder::clNewBid(int value, int color)
{
	Message msg;
	msg.type = Message::MessageType::CL_NEW_BID;
	msg.iData[Message::Key::clNewBid::VALUE] = value;
	msg.iData[Message::Key::clNewBid::COLOR] = color;

	return msg;
}

Message MessageBuilder::svUpdateBidCombo(int minValue)
{
	Message msg;
	msg.type = Message::MessageType::SV_UPDATE_BID_COMBO;
	msg.iData[Message::Key::svUpdateBidCombo::MIN_VALUE] = minValue;

	return msg;
}
