#pragma once

//--- Flag which specify if program should wait for user input after every AI move. (This option is avaliable only when 2 AI instances are playing vs each others ---//
#define FULLY_AUTOMATED	false

//--- Errors codes ---//
#define VEC_OUT_OF_RANGE "Read attempt from out of vector range!"
#define FIGURE_BAD_OWNER "If the figure was created it needs proper owning side!"
#define COPY_LIMIT_EXCEEDED "Attempt to create to many instancies of the chess piece!"
#define BAD_TYPE_INSERTION "Attempt to insert value of the type different than board representaion!"
#define BAD_TYPE_CONVERTION "Attempt to convert on incorrect type!"
#define MOVE_COMMAND__WRONG_OWNER "Incorrect owner specify in move command!"
#define MOVE_COMMAND__WRONG_PIECE "Specified piece do not exist!"
#define MOVE_COMMAND__COORDINATE_OUT_OF_RANGE "Specified coordinate was from out of range!"
#define MOVE_COMMAND__NO_KING_ON_BOARD "Unable to find correct king piece on given board!"
#define MOVE_COMMAND__INVALID_MOVE_DIRECTION "Given move is not rectilinear!"
#define MOVE_COMMAND__NO_NEW_PIECE "Piece to change into wasn't specify!"
#define READ_FILE_ERROR "Unable to open file!"
#define TEST_CASE_SEARCH_FAIL "Type of test case that is searched for do not exist!"
#define TEST_TOO_FEW_ARGUMENTS "Not enought arguments has been specified to start test run."
#define UI_INVALID_AI_NUMBER "In case of chess game there can be only 2 players. Maximal number of AI instances is 2!"

//--- Warnings codes ---//
#define INVALID_PIECE_ID "No coresponding chess piece of the given ID! Insertion failed!"
#define MOVE_COMMAND__INVALID_OCCUPATION "Incorrect command! This plate is occupied by different piece!"
#define MOVE_COMMAND__INVALID_STARTING_POS "Given coordinates do not contains correct piece!"
#define MOVE_COMMAND__INCOMPATIBLE_CORDS "Given coordinats are different than in piece object!"
#define MOVE_COMMAND__NO_MOVE_NEEDED "Start and end coordinates are the same so no move is needed!"
#define MOVE_COMMAND__INVALID_SPECIAL_USE "Given piece cannot perform this speciam move!"

//--- Enum of avaliable players ---//
enum class OWNERS {
	NONE = 0,
	WHITE,
	BLACK
};


//--- Enum of move types ---//
enum class MOVE_TYPES {
	BASIC = 0,
	PROMOTION,
	CASTLING,
	EN_PASSANT
};

//-- Enum of UI types ---//
enum class UI_TYPE {
	COMMAND_LINE = 0,
	GRAPHICAL
};

enum class APP_STATES { // TODO: Temporary solution! Remove when state machine will be implemented!
	MAIN_MENU_STATE = 0,
	GAME_STATE,
	END_STATE
};

//--- Structure containing data passed by move command ---//
struct Move_Command {
	MOVE_TYPES m_moveType = MOVE_TYPES::BASIC;
	char m_piece = 'x';
	OWNERS m_owner = OWNERS::NONE;
	bool m_capture = false;
	std::pair<unsigned int, unsigned int> m_startPos, m_endPos;
	std::optional<char> m_newPiece;

	Move_Command() = default;
	Move_Command(MOVE_TYPES moveType, char piece, OWNERS owner, bool capture, std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos)
		: m_moveType(moveType), m_piece(piece), m_owner(owner), m_capture(capture), m_startPos(startPos), m_endPos(endPos) {}
};

//--- Exception class ---//
class PaleEngineException : public std::exception {

public:
	PaleEngineException(const char* msg, char type, std::string file, int line, std::string function, std::string info)
		: std::exception(msg), _type(type), _file(file), _line(line), _function(function), _info(info) {}

	char GetType() const { return _type; }
	std::string GetFile() const { return _file; }
	int GetLine() const { return _line; }
	std::string GetFunction() const { return _function; }
	std::string GetInfo() const { return _info; }

private:
	char _type;
	int _line;
	std::string _file, _function, _info;
};