#pragma once

//--- Flag which specify if program should wait for user input after every AI move. (This option is available only when 2 AI instances are playing vs each others ---//
#define FULLY_AUTOMATED	false

//--- Errors codes ---//
#define VEC_OUT_OF_RANGE "Read attempt from out of vector range!"
#define FILE_OPEN_ERROR "Specified file cannot be opened!"
#define FIGURE_BAD_OWNER "If the figure was created it needs proper owning side!"
#define COPY_LIMIT_EXCEEDED "Attempt to create to many instances of the chess piece!"
#define BAD_TYPE_INSERTION "Attempt to insert value of the type different than board representation!"
#define BAD_TYPE_CONVERTION "Attempt to convert on incorrect type!"
#define WRONG_OWNER "Incorrect owner specify!"
#define MOVE_COMMAND__WRONG_PIECE "Specified piece do not exist!"
#define MOVE_COMMAND__COORDINATE_OUT_OF_RANGE "Specified coordinate was from out of range!"
#define MOVE_COMMAND__INVALID_MOVE_DIRECTION "Given move is not rectilinear!"
#define MOVE_COMMAND__NO_NEW_PIECE "Piece to change into wasn't specify!"
#define READ_FILE_ERROR "Unable to open file!"
#define TEST_CASE_SEARCH_FAIL "Type of test case that is searched for do not exist!"
#define TEST_TOO_FEW_ARGUMENTS "Not enough arguments has been specified to start test run."
#define UI_INVALID_AI_NUMBER "In case of chess game there can be only 2 players. Maximal number of AI instances is 2!"
#define KING_NOT_FOUND	"Application was unable to locate king piece on the board. It could mean it has been removed or not placed at all. Game cannot continue without king piece!"
#define MATH__MATRICES_DIMENTIONS_INCORRECT "Operation cannot be performed because some dimensions are incorrect!"
#define NN__PREVIOUS_LAYER_INVALID_SIZE "Calculation interrupted! Passed layer have different size than input weights vector!"
#define NN__INCORRECT_TOPOLOGY_SIZE "Specified topology size is incorrect! Neural network needs to have at least 1 hidden layer."
#define NN__INCOMPATIBLE_BIASES_NUMBER "Number of biases have to be the same as layer size!"
#define NN__INCORRECT_TARGET_SIZE "Vector of target data and output layer have different sizes!"
#define NN__DELTA_WEIGHTS_MATRIX_INVALID_SIZE "One of delta weights matrix dimensions is incorrect!"
#define NN__DELTA_BIASES_MATRIX_INVALID_SIZE "One of delta biases matrix dimensions is incorrect!"
#define NN__INVALID_DATA_SIZE "Passed data size is invalid!"
#define NN__INVALID_LAYER "Cannot perform this operation on specified layer type!"

//--- Warnings codes ---//
#define INVALID_PIECE_ID "No corresponding chess piece of the given ID! Insertion failed!"
#define MOVE_COMMAND__INVALID_OCCUPATION "Incorrect command! This plate is occupied by different piece!"
#define MOVE_COMMAND__INVALID_STARTING_POS "Given coordinates do not contains correct piece!"
#define MOVE_COMMAND__INCOMPATIBLE_CORDS "Given coordinates are different than in piece object!"
#define MOVE_COMMAND__NO_MOVE_NEEDED "Start and end coordinates are the same so no move is needed!"
#define MOVE_COMMAND__INVALID_SPECIAL_USE "Given piece cannot perform this special move!"

//--- Enum of available players ---//
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

//--- Hack function to handle custom assertion ---//
inline bool AssertionHandling(const bool assertion, std::string message) {
	if (!assertion)
		PALE_ASSERTION_ERROR(message);

	return assertion;
}