#pragma once

//--- Errors codes ---//
#define VEC_OUT_OF_RANGE "Read attempt from out of vector range!"
#define FIGURE_BAD_OWNER "If the figure was created it needs proper owning side!"
#define COPY_LIMIT_EXCEEDED "Attempt to create to many instancies of the chess piece!"
#define BAD_TYPE_INSERTION "Attempt to insert value of the type different than board representaion!"
#define BAD_TYPE_CONVERTION "Attempt to convert on incorrect type!"
#define MOVE_COMMAND__WRONG_OWNER "Incorrect owner specify in move command!"
#define MOVE_COMMAND__WRONG_PIECE "Specified piece do not exist!"
#define MOVE_COMMAND__COORDINATE_OUT_OF_RANGE "Specified coordinate was from out of range!"

//--- Warnings codes ---//
#define INVALID_PIECE_ID "No coresponding chess piece of the given ID! Insertion failed!"
#define MOVE_COMMAND__INVALID_OCCUPATION "Incorrect command! This plate is occupied by different piece!"
#define MOVE_COMMAND__INVALID_STARTING_POS "Given coordinates do not contains correct piece!"

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