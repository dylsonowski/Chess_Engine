#pragma once

namespace Pale::AI_Module {
	class Game_Tree {

	public:

	private:
		unsigned short int _gameTreeDepth;
		std::shared_ptr<Game_Tree_Node> _root;
		OWNERS _rootMovePlayer;
	};

	class Game_Tree_Node {

	public:
		Game_Tree_Node() = delete;
		Game_Tree_Node(unsigned short int layerId, unsigned short int nodeId, std::shared_ptr<Chess_Logic::Board_Representation<int>> boardArrangement, std::string move, double evaluation = 0) : _layerId(layerId), _nodeId(nodeId), _boardArrangement(boardArrangement), _move(move), _evaluation(evaluation) { PALE_ENGINE_INFO("Game_Tree.h->Game_Tree_Node constructor [15]: New game tree node has been created! Arrangement move: {0}. Evaluation: {1}. Number of children: {2}.", _move, _evaluation, _childrenList.size()); }
		~Game_Tree_Node() = default;

		//--- Getters & Setters ---//
		inline std::string GetMove() const { return _move; }
		inline void SetMove(std::string value) { _move = value; }
		inline double GetEvaluation() const { return _evaluation; }
		inline void SetEvaluation(double value) { _evaluation = value; }
		inline unsigned short int GetChildrenListSize() const { return _childrenList.size(); }

		//--- Void functions ---//
		void AddBranch(const Game_Tree_Node& newNode) { _childrenList.emplace_back(std::make_shared<Game_Tree_Node>(newNode)); PALE_ENGINE_INFO("Game_Tree.h->AddBranch() [26]: Game tree node {0}node:{1}layer got new child node added! Children list size: {2}.", _nodeId, _layerId, _childrenList.size()); }
		void RemoveBranch(const std::vector<std::shared_ptr<Game_Tree_Node>>::iterator childrenListIterator) { _childrenList.erase(childrenListIterator); }

	private:
		unsigned short int _layerId, _nodeId;
		std::shared_ptr<Chess_Logic::Board_Representation<int>> _boardArrangement;
		std::string _move;
		double _evaluation;
		std::vector<std::shared_ptr<Game_Tree_Node>> _childrenList;
	};
}