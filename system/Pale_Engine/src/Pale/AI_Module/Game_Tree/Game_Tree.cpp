#include "palepch.h"
#include "Game_Tree.h"

namespace Pale::AI_Module {
	Game_Tree::Game_Tree(): _gameTreeDepth(0), _rootPlayerMove(OWNERS::WHITE) {
		Chess_Logic::Board_Representation<std::shared_ptr<Chess_Logic::Pieces>> startBoardArrangement(8, 8, false);
		_root = std::make_shared<Game_Tree_Node>(0, 0, std::make_shared<Chess_Logic::Board_Representation<std::shared_ptr<Chess_Logic::Pieces>>>(startBoardArrangement), "");

		PALE_ENGINE_INFO("Game_Tree.cpp->Game_Tree constructor [9]: New instance of game tree has been created! Created game tree consists only of root node!");
	}

	Game_Tree::Game_Tree(Chess_Logic::Board_Representation<std::shared_ptr<Chess_Logic::Pieces>> rootBoardArrangement, OWNERS rootPlayerMove): _gameTreeDepth(0), _rootPlayerMove(rootPlayerMove) {
		_root = std::make_shared<Game_Tree_Node>(0, 0, std::make_shared<Chess_Logic::Board_Representation<std::shared_ptr<Chess_Logic::Pieces>>>(rootBoardArrangement), "");

		PALE_ENGINE_INFO("Game_Tree.cpp->Game_Tree constructor [9]: New instance of game tree has been created! Created game tree consists only of root node!");
	}
}