#ifndef H_MESSAGE_150820130123_H
#define H_MESSAGE_150820130123_H

#include <iostream>
#include <map>
//#include <SFML/System.hpp>
#include <SFML/Network.hpp>

///////////////////////////////////////////////////////////////////////////////
/// \brief Class representing a message with different data types.
///////////////////////////////////////////////////////////////////////////////
class Message
{
public:
	Message(void);
	~Message(void);

	friend sf::Packet& operator << (sf::Packet& Packet, const Message& M); 
	friend sf::Packet& operator >> (sf::Packet& Packet, Message& M); 
	
	///////////////////////////////////////////////////////////////////////////
	// Vide le contenu du message
	///////////////////////////////////////////////////////////////////////////
	void clear();

	//Message data
	mutable std::map<int, int> iData;
	mutable std::map<int, float> fData;
	mutable std::map<int, std::string> sData;

protected:
	//////////////////////////////////////////////////////////////////////////////
	// Sérialise (?) le contenu d'un message dans un paquet 
	//////////////////////////////////////////////////////////////////////////////
	void toStream(sf::Packet& Packet)const;

	//////////////////////////////////////////////////////////////////////////////
	// Extrait le contenu d'un paquet vers un message
	//////////////////////////////////////////////////////////////////////////////
	void fromStream(sf::Packet& Packet, Message& M);
	
public:
	int type;

	//struct MessageType
	//{
	//	enum messageType
	//	{							
	//		EXAMPLE_1,
	//		EXAMPLE_2
	//	};
	//};
	struct MessageType
	{
		enum messageType
		{							
			CL_CARD_MOVED_TO_HAND, //i, SBidding
			SV_START_GAME,
			SV_INIT_CARDS,
			SV_NEW_PLAYER,
			SV_PLAYER_LIST,
			SV_PLAYER_YOUR_ID,
			SV_CHANGE_TEAM,
			CL_CONNECTION,
			SV_PLAYER_CREATED, //i, NetworkEngine
			SV_CONNECTION_RESULT,  //i, GameEngine
			SV_SERVER_FULL,
			SV_PLAYER_CARDS, 
			SV_GO_SPREBIDDING,
			SV_GO_SBIDDING,
			CL_SPREBIDDING_READY,
			CL_SBIDDING_READY,
			SV_CREATE_DECK,
			SV_PLAYER_DISTRIBUTE_CARDS,
			SV_PLAYER_TO_PLAY,
			SV_FIRST_PLAYER_TO_PLAY,
			CL_PLAY_CARD,
			SV_CARD_TO_TABLE,
			SV_GO_SGAME,
			SV_CHANGE_CARD_TYPE_ID,
			CARD_ARRIVED_ON_TABLE, //i, GameEngine
			SV_END_TRICK,
			CL_NEW_BID,
			SV_UPDATE_BID_COMBO,
		};
	};

	//All the keys associated to messages
	struct Key
	{
		struct ExampleLoadGame {
			//sData map key
			enum{
				GAME_NAME
			};
		};

		struct ClCardMovedToHand {
			enum {
				CARD_ID //iData
			};
		};

		struct SvPlayerYourId {
			enum {
				ID //iData
			};
		};
		
		struct SvNewPlayer {
			enum {
				ID,   //iData
				TEAM, //iData
				NAME  //sData
			};
		};

		struct SvPlayerList {
			enum {
				NB_PLAYERS, //iData
				LIST //sData (id, team, nicklength, nick)
			};
		};

		struct ClConnection {
			enum {
				TMP_ID, //iData
				NAME, //sData
				IS_LOCAL_CLIENT, //iData (0 or 1)
			};
		};

		struct SvPlayerCreated {
			enum {
				ID, //iData
				TMP_ID //iData
			};
		};

		struct SvConnectionResult {
			enum {
				RESULT, //iData
				ERROR_CONNEC, //iData
			};
		};

		struct SvServerFull {
			enum {
				INFO, //sData
			};
		};

		struct SvPlayerCards {
			enum {
				NB_PLAYERS, //iData
				NB_CARDS, //iData
				DATA, //sData (forAllPlayer id player, for(allPlayerCards) id, typeId endfor endfor
			};
		};

		struct SvCreateDeck {
			enum {
				NB_CARDS, //iData
				DATA, //sData (for(allCards) id, value, color endfor
			};
		};
		struct SvPlayerToPlay {
			enum {
				PLAYER_ID, //iData
			};
		};
		struct SvFirstPlayerToPlay {
			enum {
				FIRST_PLAYER_ID, //iData
			};
		};
		struct clCardToPlay {
			enum {
				ID, //iData (autoset)
				CARD_ID, //iData
			};
		};
		struct svCardToTable {
			enum {
				PLAYER_ID, //iData 
				CARD_ID, //iData
			};
		};
		struct svChangeCardTypeId {
			enum {
				PLAYER_ID, //iData 
				CARD_ID, //iData
				CARD_TYPE_ID, //iData
			};
		};
		struct svEndTrick {
			enum {
				FIRST_POSITION, //iData
			};
		};
		struct svChangeTeam {
			enum {
				ID_PLAYER, //iData
				TEAM_ID,
			};
		};
		struct clNewBid {
			enum {
				ID, //iData (autoset)
				VALUE, //iData
				COLOR  //iData
			};
		};
		struct svUpdateBidCombo {
			enum {
				MIN_VALUE, //iData
			};
		};
	};
};

struct MessageReseau
{
	MessageReseau(const Message& M, bool b) {MonMessage = M; destinataireUnique = b;}

	Message MonMessage;
	bool destinataireUnique;
};

struct MessageTemps
{
	Message M;
	float temps;
	int id;

	MessageTemps(const Message& M, int id, float delai)
	:M(M)
	,id(id)
	,temps(delai)
	{
	}

	friend bool operator < (const MessageTemps& Lui, const MessageTemps& Autre)
	{
		return (Lui.temps < Autre.temps);
	}
};

#endif