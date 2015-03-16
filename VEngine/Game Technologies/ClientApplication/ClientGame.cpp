#include "ClientGame.h" 
#include "..\..\nclgl\Vector3.h"

bool ClientGame::PLAY_GAME = false;
bool ClientGame::NEED_UPDATE = false;
bool ClientGame::UPDATE_PLAYER_STATUS = false;

ClientGame::ClientGame(void) {
	justOnceInitPacket = true;

	PlayerInformationNetworking::Initialise();
}

void ClientGame::sendActionPackets(PacketTypes TYPE) {
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = TYPE;

	packet.serialize(packet_data);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


int ClientGame::InitializeWindow() {
	//game initialization
	if (!Window::Initialise("Game Technologies", 1280, 800, false)) {
		return Quit(true, "Window failed to initialise!");
	}

	if (!Renderer::Initialise()) {
		return Quit(true, "Renderer failed to initialise!");
	}
}

void ClientGame::InitializeGameSinglePlayer() {
	PhysicsSystem::Initialise();
	SoundSystem::Initialise();

	//get the type of game from server
	game = new MyGame();

	Window::GetWindow().LockMouseToWindow(true);
	Window::GetWindow().ShowOSPointer(false);

	running = true;
	physics = std::thread{ [this](){ this->physicsLoop(game, std::ref(running)); } };
}

void ClientGame::UpdateSinglePlayer() {
	float msec = Window::GetWindow().GetTimer()->GetTimedMS();	//How many milliseconds since last update?

	game->UpdateRendering(msec);	//Update our 'sybsystem' logic (renderer and physics!)
	game->UpdateGame(msec);	//Update our game logic
}

void ClientGame::RunGame() {
	//check to see if it needs an update from the server
	if (game->UPDATE_SERVER) {
		sendActionPackets(SERVER_TO_CLIENT_UPDATE_TERRAIN_DATA);
		NEED_UPDATE = true;
		game->UPDATE_SERVER = false;
	}

	float msec = Window::GetWindow().GetTimer()->GetTimedMS();	//How many milliseconds since last update?

	game->UpdateRendering(msec);	//Update our 'sybsystem' logic (renderer and physics!)
	game->UpdateGame(msec);	//Update our game logic
	UPDATE_PLAYER_STATUS = true;
}

int ClientGame::TerminateGame() {
	running = false;
	physics.join();

	if (game) {
		delete game;	//Done with our game now...bye bye!
	}
	return Quit();
}

int ClientGame::Quit(bool pause, const string &reason) {
	PhysicsSystem::Destroy();
	Sound::DeleteSounds();
	SoundSystem::Destroy();
	Window::Destroy();
	Renderer::Destroy();

	if (pause) {
		std::cout << reason << std::endl;
		system("PAUSE");
	}

	return 0;
}

void ClientGame::physicsLoop(GameClass* game, bool& running) {
	GameTimer timer;
	while (running) {
		float msec = timer.GetTimedMS();
		game->UpdatePhysics(msec);
	}
}


void ClientGame::DecodeInitSToCData(const Packet& packet, TerrainGenerationData* terrainData)
{
	if (!terrainData)
		terrainData = new TerrainGenerationData();
	memcpy(terrainData, packet.data, sizeof(TerrainGenerationData));
	//then initialize all the pointers inside the terrainGenerationData
	{
		const int numTrees = (const int)(terrainData->getNumTrees());
		numOfTrees = numTrees;
		//initialize the treepositions
		Vector3* currentTreePostion = new Vector3[numTrees];
		void* currentPtr = ((char*)(packet.data) + sizeof(TerrainGenerationData));
		memcpy(currentTreePostion, currentPtr, numTrees * sizeof(Vector3));
		terrainData->setTreePostions(currentTreePostion);
		//then intialize the treesize
		int* treeSizes = new int[numTrees];
		currentPtr = ((char*)currentPtr + (numTrees * sizeof(Vector3)));
		cout << (numTrees * sizeof(Vector3)) << endl;
		memcpy(treeSizes, currentPtr, numTrees * sizeof(int));
		terrainData->setTreeSizes(treeSizes);

		//initialize game
		PhysicsSystem::Initialise();
		SoundSystem::Initialise();

		game = new MyGame(true, terrainData);

		Window::GetWindow().LockMouseToWindow(true);
		Window::GetWindow().ShowOSPointer(false);

		running = true;
		physics = std::thread{ [this](){ this->physicsLoop(game, std::ref(running)); } };
	}
}

void ClientGame::DecodeSToCUpdateData(const Packet& packet, Vector3* treePositions, Vector3& spawnerPosition, const int& numTrees)
{
	if (!treePositions)
		treePositions = new Vector3[numTrees];
	memcpy(treePositions, packet.data, sizeof(Vector3) * numTrees);
	void* currentPtr = ((char*)(packet.data) + sizeof(Vector3)* numTrees);
	memcpy(&spawnerPosition, currentPtr, sizeof(Vector3));

	terrainData = new TerrainGenerationData();
	terrainData->setTreePostions(treePositions);
	terrainData->setSpawnerPosition(spawnerPosition);

	game->UPDATE_TERRAIN_DATA = true;
	game->setTerrainGenerationData(terrainData);
}


Packet* ClientGame::EncodeCToSPlayerData(const PlayerDataNetworking* playerData)
{
	Packet* currentPacket = new Packet();
	currentPacket->packet_type = PLAYER_STATUS;
	currentPacket->packet_length = sizeof(PlayerDataNetworking);
	memcpy(currentPacket->data, playerData, sizeof(PlayerDataNetworking));

	return currentPacket;
}

void ClientGame::DecodeSToCPlayerInfoData(const Packet& packet)
{
	memcpy(&PlayerInformationNetworking::GetPlayerInformationNetworking(), packet.data, sizeof(int));
	PlayerDataNetworking* pTemp = new PlayerDataNetworking[PlayerInformationNetworking::GetPlayerInformationNetworking().GetNumPlayers()];
	memcpy(pTemp, (char*)packet.data + sizeof(int), PlayerInformationNetworking::GetPlayerInformationNetworking().GetNumPlayers() * sizeof(PlayerDataNetworking));
	PlayerInformationNetworking::GetPlayerInformationNetworking().setPlayerDataNetworking(pTemp);
}

void ClientGame::InitializeGameMultiPlayer() {
	network = new ClientNetwork();
	//connect to server
	// send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
	// ------------------------------------------------------------- //
}

void ClientGame::UpdateMultiPlayer() {
	if (UPDATE_PLAYER_STATUS) {
		Packet* packetToSend;
		//send the update of player position to server
		//get it from mygame
		pDNetworking = game->getPlayerDataNetworking();
		packetToSend = EncodeCToSPlayerData(pDNetworking);
		NetworkServices::sendMessage(network->ConnectSocket, (char*)packetToSend, sizeof(Packet));
		UPDATE_PLAYER_STATUS = false;
	}

	Packet packet;
	int data_length = network->receivePackets(network_data);

	if (PLAY_GAME && !NEED_UPDATE) {
		RunGame();
	}

	if (data_length <= 0 && !NEED_UPDATE) {
		return;
	}

	if (NEED_UPDATE) {
		data_length = network->receivePackets(network_data);
	}

	int i = 0;
	while (i < (unsigned int)data_length) {
		packet.deserialize(&(network_data[i]));
		i += sizeof(Packet);

		switch (packet.packet_type) {
		case INIT_SERVER_TO_CLIENT_DATA: {
			if (justOnceInitPacket) {
				printf("Initial packet received from server, run game. (on update)\n");
				terrainData = NULL;
				//decode the packet data and initialize the terrain data with it.
				DecodeInitSToCData(packet, terrainData);
				PLAY_GAME = true;
				justOnceInitPacket = false;
			}
			break;
		}
		case NOT_ALL_CLIENTS_CONNECTED: {
			printf("Waiting all clients to connect, packet received from server. (on update)\n");
			break;
		}
		case SERVER_TO_CLIENT_UPDATE_TERRAIN_DATA: {
			//update the tree positions and spawner position based on the data received from client
			Vector3 spawnPos;
			Vector3* treePos = NULL;
			DecodeSToCUpdateData(packet, treePos, spawnPos, numOfTrees);
			NEED_UPDATE = false;
			break;
		}
		case PLAYER_INFO: {
			DecodeSToCPlayerInfoData(packet);
			printf("Server has sent packets with player info. (on update) %d \n", PlayerInformationNetworking::GetPlayerInformationNetworking().GetNumPlayers());
			break;
		}
		default:
			printf("error in packet types (on update) %d \n", packet.packet_type);
			break;
		}
	}
}