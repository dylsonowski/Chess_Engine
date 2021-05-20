#pragma once

//--- Errors codes ---//
#define VEC_OUT_OF_RANGE "Read attempt from out of vector range!"
#define FIGURE_BAD_OWNER "If the figure was created it needs proper owning side!"
#define COPY_LIMIT_EXCEEDED "Attempt to create to many instancies of the chess piece!"
#define BAD_TYPE_INSERTION "Attempt to insert value of the type different than board representaion!"
#define MOVE_COMMAND__WRONG_OWNER "Incorrect owner specify in move command!"
#define MOVE_COMMAND__WRONG_PIECE "Specified piece do not exist!"
#define MOVE_COMMAND__COORDINATE_OUT_OF_RANGE "Specified coordinate was from out of range!"

//--- Warnings codes ---//
#define INVALID_PIECE_ID "No coresponding chess piece of the given ID! Insertion failed!"
#define INVALID_OCCUPATION "Incorrect command! This plate is occupied by different piece!"