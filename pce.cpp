/*
 > PCE - Play eChess Engine
 > Code by SinisterIcy
 > Version INFDEV
 > Licensed under the GNU General Public License v3.0
 > Github: https://github.com/PCE-Engine/PCE
*/

#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief PlayeChessEngine is the namespace for the PCE engine who is made for
 * the eChess project (playechess.com)
 */
namespace PlayeChessEngine {

/**
 * @brief Move is a class that represents a move (start square and end square)
 */
	class Move {
		private:
		/**
		 * @brief The x coordinate of the start square
		 */
		int start_square_x;
		/**
		 * @brief The y coordinate of the start square
		 */
		int start_square_y;
		/**
		 * @brief The x coordinate of the end square
		 */
		int end_square_x;
		/**
		 * @brief The y coordinate of the end square
		 */
		int end_square_y;

		public:
		/**
		 * @brief Construct a new Move object
		 *
		 * @param start_square_x The x coordinate of the start square
		 * @param start_square_y The y coordinate of the start square
		 * @param end_square_x The x coordinate of the end square
		 * @param end_square_y The y coordinate of the end square
		 */
		Move(int start_square_x, int start_square_y, int end_square_x,
			int end_square_y) {
			this->start_square_x = start_square_x;
			this->start_square_y = start_square_y;
			this->end_square_x = end_square_x;
			this->end_square_y = end_square_y;
		};

		/**
		 * @brief Shows the move in a readable format
		 *
		 * @return Formatted move (std::string)
		 */
		std::string show() {
			std::string files = "abcdefgh";
			return std::string(1, files[start_square_y]) + std::to_string(start_square_x + 1) + " -> " + std::string(1, files[end_square_y]) + std::to_string(end_square_x + 1);
		}

		/**
		 * @brief Get the start coords
		 *
		 * @return The start coords (std::vector<int>)
		 */
		std::vector<int> get_start_coords() {
			return {this->start_square_x, this->start_square_y};
		}
		/**
		 * @brief Get the end coords
		 *
		 * @return The end coords (std::vector<int>)
		 */
		std::vector<int> get_end_coords() {
			return {this->end_square_x, this->end_square_y};
		}

		/**
		 * @brief Checks if the move is in a vector of moves
		 *
		 * @param moves The vector of moves
		 * @return True if the move is in the vector, false if not (bool)
		 */
		bool am_in(std::vector<Move> moves) {
			for (int i = 0; i < moves.size(); i++) {
				if (moves[i].get_end_coords()[0] == this->get_end_coords()[0] && moves[i].get_end_coords()[1] == this->get_end_coords()[1])
					return true;
			}
			return false;
		}
	};

	/**
	 * @brief Namespace for things related to the board
	 */
	namespace board {

		/**
		 * @brief Namespace for things related to the pieces
		 */
		namespace pieces {

			/**
			 * @brief The piece type enum (p = pawn, r = rook, n = knight, b = bishop, q =
			 * queen, k = king)
			 */
			enum piece_type { p, r, n, b, q, k };

			/**
			 * @brief The piece class (abstract)
			 */
			class Piece {
				protected:
					/**
					 * @brief The type of the piece
					 */
					piece_type type;
					/**
					 * @brief The coordinates of the piece
					 */
					int coords[2] = {0, 0};

				public:
					/**
					 * @brief The color of the piece (true = white, false = black)
					 */
					bool is_white;
					/**
					 * @brief If the piece has moved (used for castling)
					 */
					bool has_moved = false;

					/**
					 * @brief Construct a new Piece object
					 *
					 * @param type The type of the piece (refer to the piece_type enum)
					 * @param is_white The color of the piece (true = white, false = black)
					 * @param x The x coordinate of the piece (0-7)
					 * @param y The y coordinate of the piece (0-7)
					 */
					Piece(piece_type type, bool is_white, int x, int y) {
						this->type = type;
						this->is_white = is_white;
						this->coords[0] = x;
						this->coords[1] = y;
					}

					/**
					 * @brief Validates the move of the piece (abstract)
					 *
					 * @return If the move is valid (bool)
					 */
					virtual bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						return false;
					}

					/**
					 * @brief Updates the coordinates of the piece
					 *
					 * @param x The new x coordinate of the piece (0-7)
					 * @param y The new y coordinate of the piece (0-7)
					 */
					void update_coords(int x, int y) {
						this->coords[0] = x;
						this->coords[1] = y;
					}

					piece_type get_type() { return this->type; }

					/**
					 * @brief Validates the move of the piece (checks if the landing square is
					 * valid)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the landing square is valid (bool)
					 */
					bool validate_validation(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						if (board[x_final][y_final] == nullptr)
							return true;
						if (board[x_final][y_final]->is_white == this->is_white)
							return false;
						return true;
					}

					/**
					 * @brief Checks the path from the piece to the landing square
					 *
					 * @param x_final The final x coordinate
					 * @param y_final The final y coordinate
					 * @param board The board
					 * @return Whether the path is clear (bool)
					 */
					bool check_path(int x_final, int y_final, std::array<std::array<Piece *, 8>, 8> board) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if (x_diff == 0) {
							if (y_diff > 0) {
								for (int i = 1; i < y_diff; i++) {
									if (board[this->coords[0]][this->coords[1] + i] != nullptr)
										return false;
								}
							} else {
								for (int i = 1; i < abs(y_diff); i++) {
									if (board[this->coords[0]][this->coords[1] - i] != nullptr)
										return false;
								}
							}
						} else if (y_diff == 0) {
							if (x_diff > 0) {
								for (int i = 1; i < x_diff; i++) {
									if (board[this->coords[0] + i][this->coords[1]] != nullptr)
										return false;
								}
							} else {
								for (int i = 1; i < abs(x_diff); i++) {
									if (board[this->coords[0] - i][this->coords[1]] != nullptr)
										return false;
								}
							}
						} else if (abs(x_diff) == abs(y_diff)) {
							if (x_diff > 0 && y_diff > 0) {
								for (int i = 1; i < x_diff; i++) {
									if (board[this->coords[0] + i][this->coords[1] + i] != nullptr)
										return false;
								}
							} else if (x_diff < 0 && y_diff < 0) {
								for (int i = 1; i < abs(x_diff); i++) {
									if (board[this->coords[0] - i][this->coords[1] - i] != nullptr)
										return false;
								}
							} else if (x_diff > 0 && y_diff < 0) {
								for (int i = 1; i < x_diff; i++) {
									if (board[this->coords[0] + i][this->coords[1] - i] != nullptr)
										return false;
								}
							} else if (x_diff < 0 && y_diff > 0) {
								for (int i = 1; i < abs(x_diff); i++) {
									if (board[this->coords[0] - i][this->coords[1] + i] != nullptr)
										return false;
								}
							}
						}
						return true;
					}

					/**
					 * @brief Shows the piece in a readable format
					 *
					 * @return The formatted piece (std::string)
					 */
					std::string show() {
						if (this->is_white) {
							std::string piece_names[6] = {"P", "R", "N", "B", "Q", "K"};
							return piece_names[this->type];
						}
						std::string piece_names[6] = {"p", "r", "n", "b", "q", "k"};
						return piece_names[this->type];
					}
			};

			/**
			 * @brief The pawn piece (inherits from Piece)
			 *
			 */
			class Pawn : public Piece {
				private:
					/**
					 * @brief If the pawn has moved
					 *
					 */
					bool has_moved = false;

				public:
					/**
					 * @brief Construct a new Pawn object
					 *
					 * @param is_white Whether the pawn is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the pawn
					 * @param y The y coordinate of the pawn
					 */
					Pawn(bool is_white, int x, int y) : Piece(p, is_white, x, y){}

					/**
					 * @brief Validates the move of the pawn (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						// TODO Add pawn takes
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];

						if (y_diff == 0 && x_diff != 0) {
							if (this->is_white) {
								if (x_diff == 1) {
									if (board[x_final][y_final] == nullptr)
										return true;
								} else if (x_diff == 2 && !this->has_moved) {
									if (board[x_final][y_final] == nullptr && board[x_final][y_final - 1] == nullptr)
										return true;
								}
							} else {
								if (x_diff == -1) {
									if (board[x_final][y_final] == nullptr)
										return true;
								} else if (x_diff == -2 && !has_moved) {
									if (board[x_final][y_final] == nullptr && board[x_final][y_final + 1] == nullptr)
										return true;
								}
							}
						}
						return false;
					}
			};

			/**
			 * @brief The rook piece (inherits from Piece)
			 *
			 */
			class Rook : public Piece {
				private:
					/**
					 * @brief If the rook has moved
					 *
					 */
					bool has_moved = false;

				public:
					/**
					 * @brief Construct a new Rook object
					 *
					 * @param is_white Whether the rook is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the rook
					 * @param y The y coordinate of the rook
					 */
					Rook(bool is_white, int x, int y) : Piece(r, is_white, x, y) {}

					/**
					 * @brief Validates the move of the rook (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if (x_diff * y_diff == 0 && x_diff != y_diff) {
							if (validate_validation(board, x_final, y_final))
								return this->check_path(x_final, y_final, board);
						}
						return false;
					}
			};

			/**
			 * @brief The knight piece (inherits from Piece)
			 *
			 */
			class Knight : public Piece {
				public:
					/**
					 * @brief Construct a new Knight object
					 *
					 * @param is_white Whether the knight is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the knight
					 * @param y The y coordinate of the knight
					 */
					Knight(bool is_white, int x, int y) : Piece(n, is_white, x, y) {}

					/**
					 * @brief Validates the move of the knight (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2))
							return validate_validation(board, x_final, y_final);
						return false;
					}
			};

			/**
			 * @brief The bishop piece (inherits from Piece)
			 *
			 */
			class Bishop : public Piece {
				public:
					/**
					 * @brief Construct a new Bishop object
					 *
					 * @param is_white Whether the bishop is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the bishop
					 * @param y The y coordinate of the bishop
					 */
					Bishop(bool is_white, int x, int y) : Piece(b, is_white, x, y) {}

					/**
					 * @brief Validates the move of the bishop (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if (abs(x_diff) == abs(y_diff) && x_diff != 0) {
							if (validate_validation(board, x_final, y_final))
								return this->check_path(x_final, y_final, board);
						}
						return false;
					}
			};

			/**
			 * @brief The queen piece (inherits from Piece)
			 *
			 */
			class Queen : public Piece {
				public:
					/**
					 * @brief Construct a new Queen object
					 *
					 * @param is_white Whether the queen is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the queen
					 * @param y The y coordinate of the queen
					 */
					Queen(bool is_white, int x, int y) : Piece(q, is_white, x, y) {}

					/**
					 * @brief Validates the move of the queen (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if ((abs(x_diff) == abs(y_diff) && x_diff != 0) || ((x_diff == 0 || y_diff == 0) && x_diff != y_diff)) {
							if (validate_validation(board, x_final, y_final))
								return this->check_path(x_final, y_final, board);
						}
						return false;
					}
			};

			/**
			 * @brief The king piece (inherits from Piece)
			 *
			 */
			class King : public Piece {
				private:
					/**
					 * @brief Whether the king has moved or not
					 *
					 */
					bool has_moved = false;

				public:
					/**
					 * @brief Construct a new King object
					 *
					 * @param is_white Whether the king is white or black (true = white, false =
					 * black)
					 * @param x The x coordinate of the king
					 * @param y The y coordinate of the king
					 */
					King(bool is_white, int x, int y) : Piece(k, is_white, x, y) {}

					/**
					 * @brief Validates the move of the king (combines the other validation
					 * functions)
					 *
					 * @param board The board
					 * @param x_final The x coordinate of the landing square
					 * @param y_final The y coordinate of the landing square
					 * @return If the move is valid (bool)
					 */
					bool validation_function(std::array<std::array<Piece *, 8>, 8> board, int x_final, int y_final) {
						int x_diff = x_final - this->coords[0];
						int y_diff = y_final - this->coords[1];
						if (abs(x_diff) <= 1 && abs(y_diff) <= 1 && (x_diff != 0 || y_diff != 0)) {
                            std::cout << "King validation " << this->coords[0] << " " << this->coords[1] << " - " << x_final << " " << y_final << std::endl;
							return validate_validation(board, x_final, y_final);
                        }
						return false;
					}
			};
		} // namespace pieces

		/**
		 * @brief The board class
		 *
		 */
		class Board {
			private:
				/**
				 * @brief The board
				 *
				 */
				std::array<std::array<pieces::Piece *, 8>, 8> board = {{
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
					{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
				}};

			public:
				// TODO Add the funcitonnality for w KQkq - 0 1
				/**
				 * @brief Construct a new Board object
				 *
				 * @param fen The fen string
				 */
				Board(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
					fen = fen.substr(0, fen.find(" "));
					load_fen(fen);
				};

				/**
				 * @brief Reverses the fen string (for loading the board)
				 *
				 * @param fen The fen string
				 * @return The reversed fen (std::string)
				 */
				std::string reverse_fen(std::string fen) {
					std::vector<std::string> fen_split;
					std::string rt = "";
					std::string buffer = "";
					for (auto c : fen) {
						if (c == '/') {
							fen_split.push_back(buffer + '/');
							buffer = "";
						} else {
							buffer += c;
						}
					}
					fen_split.push_back(buffer + '/');
					for (int i = fen_split.size() - 1; i >= 0; i--) {
						rt += fen_split[i];
					}
					return rt.substr(0, rt.size() - 1);
				}

				/**
				 * @brief Loads the board from a fen string
				 *
				 * @param fen The fen string
				 */
				void load_fen(std::string fen) {
					fen = reverse_fen(fen);
					int x = 0;
					int y = 0;
					for (auto c : fen) {
					if (c == '/') {
						y = 0;
						x++;
					} else if (c == '8') {
						continue;
					} else if (c >= '0' && c <= '9') {
						y += c - '0';
					} else {
						bool is_white = c >= 'A' && c <= 'Z';
						switch (c) {
							case 'p':
							case 'P':
								this->board[x][y] = new pieces::Pawn(is_white, x, y);
								break;
							case 'r':
							case 'R':
								this->board[x][y] = new pieces::Rook(is_white, x, y);
								break;
							case 'n':
							case 'N':
								this->board[x][y] = new pieces::Knight(is_white, x, y);
								break;
							case 'b':
							case 'B':
								this->board[x][y] = new pieces::Bishop(is_white, x, y);
								break;
							case 'q':
							case 'Q':
								this->board[x][y] = new pieces::Queen(is_white, x, y);
								break;
							case 'k':
							case 'K':
								this->board[x][y] = new pieces::King(is_white, x, y);
								break;
						}
						y++;
					}
					}
				}

				/**
				 * @brief Prints the board (for console only)
				 *
				 * @param moves The moves to highlight
				 * @param board The board to print
				 */
				void print_board(std::vector<std::vector<int>> moves = {}, std::array<std::array<pieces::Piece *, 8>, 8> board = {}) {
					if (board == std::array<std::array<pieces::Piece *, 8>, 8>{})
						board = this->board;
					std::cout << "  #-----------------#" << std::endl;
					for (int i = 0; i < 8; i++) {
						std::cout << 8 - i << " | ";
						for (int j = 0; j < 8; j++) {
							bool skip = false;
							for (auto move : moves) {
								if (move[0] == 7 - i && move[1] == j) {
									std::cout << "* ";
									skip = true;
									break;
								}
							}
							if (skip)
								continue;
							if (board[7 - i][j] != nullptr)
								std::cout << board[7 - i][j]->show() << " ";
							else
							std::cout << "  ";
						}
						std::cout << "|" << std::endl;
					}
					std::cout << "  #-----------------#" << std::endl;
					std::cout << "    a b c d e f g h" << std::endl;
				}

				/**
				 * @brief Gets the board
				 *
				 * @return The board (std::array<std::array<pieces::Piece*, 8>, 8>)
				 */
				std::array<std::array<pieces::Piece *, 8>, 8> get_board() {
					return this->board;
				}

				/**
				 * @brief Gets a piece from the board
				 *
				 * @param x The x coordinate
				 * @param y The y coordinate
				 * @return The piece (pieces::Piece*)
				 */
				pieces::Piece *get_piece(int x, int y) { return this->board[x][y]; }

				/**
				 * @brief Gets the moves for a piece
				 *
				 * @param x The x coordinate
				 * @param y The y coordinate
				 * @param from_premove If the function is called from premove_check (to
				 * prevent infinite recursion)
				 * @return The moves (std::vector<PlayeChessEngine::Move>)
				 */
				std::vector<PlayeChessEngine::Move> get_moves(int x, int y, bool from_premove = false) {
					std::vector<PlayeChessEngine::Move> moves;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							if (this->board[x][y] == nullptr)
								continue;
							if (this->board[x][y]->validation_function(this->board, i, j)) {
								PlayeChessEngine::Move move = PlayeChessEngine::Move(x, y, i, j);
                                // std::cout << "Move of " << this->board[x][y]->show() << " from " << x << ", " << y << " to " << i << ", " << j << std::endl;
								if (from_premove) {
									moves.push_back(move);
									continue;
								}
								if (!this->premove_check(move, this->board[x][y]->is_white)) {
									moves.push_back(move);
								}
							}
						}
					}
					return moves;
				}

				/**
				 * @brief Gets all the moves for a color
				 *
				 * @param white If the color is white
				 * @param from_premove If the function is called from premove_check (to
				 * prevent infinite recursion)
				 * @return The moves (std::vector<PlayeChessEngine::Move>)
				 */
				std::vector<PlayeChessEngine::Move> get_all_moves(std::array<std::array<pieces::Piece *, 8>, 8> brd, bool white, bool from_premove = false) {
					std::vector<PlayeChessEngine::Move> moves;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							if (brd[i][j] == nullptr)
								continue;
							if (brd[i][j]->is_white != white)
								continue;
							for (auto move : this->get_moves(i, j, from_premove)) {
								moves.push_back(move);
							}
						}
					}
					return moves;
				}

				bool in(std::vector<std::vector<int>> vec, std::vector<int> val) {
					for (auto v : vec) {
						if (v == val)
							return true;
					}
					return false;
				}

				std::vector<std::vector<int>> get_all_landing_moves(std::array<std::array<pieces::Piece *, 8>, 8> brd, bool white, bool from_premove = false) {
					std::vector<std::vector<int>> moves;
					for(auto move : this->get_all_moves(brd, white, from_premove)) {
						moves.push_back(move.get_end_coords());
					}
					return moves;
				}

				/**
				 * @brief Checks if a color is in check
				 *
				 * @param white If the color is white
				 * @return If the color is in check (bool)
				 */
				bool is_check(bool white) {
					for (auto move : this->get_all_moves(this->board, !white, true)) {
						if (this->board[move.get_end_coords()[0]][move.get_end_coords()[1]] == nullptr)
							continue;
						if (this->board[move.get_end_coords()[0]][move.get_end_coords()[1]] ->get_type() == pieces::piece_type::k)
							return true;
					}
					return false;
				}

				/**
				 * @brief Transfer a piece from a position to another
				 *
				 * @param board The board to transfer the piece on
				 * @param start_x The x coordinate of the start position
				 * @param start_y The y coordinate of the start position
				 * @param end_x The x coordinate of the end position
				 * @param end_y The y coordinate of the end position
				 * @return Board with the piece positon updated
				 * (std::array<std::array<pieces::Piece*, 8>, 8>)
				 */
				std::array<std::array<pieces::Piece *, 8>, 8> transfer(std::array<std::array<pieces::Piece *, 8>, 8> board, int start_x, int start_y, int end_x, int end_y) {
					board[end_x][end_y] = board[start_x][start_y];
					board[start_x][start_y] = nullptr;
					return board;
				}

				/**
				 * @brief Plays one move in the future to check if it puts the king in check
				 *
				 * @param move The move to check
				 * @param white If the color is white
				 * @return If the move is legal (bool)
				 */
				bool premove_check(PlayeChessEngine::Move move, bool white) {
					if (this->board[move.get_start_coords()[0]][move.get_start_coords()[1]] == nullptr)
						throw std::invalid_argument("No piece at start coords");
					std::array<std::array<pieces::Piece *, 8>, 8> backup = this->board;
					this->board = this->transfer(this->board, move.get_start_coords()[0], move.get_start_coords()[1], move.get_end_coords()[0], move.get_end_coords()[1]);
					bool check = this->is_check(white);
					this->board = backup;
					return check;
				}

				/**
				 * @brief Plays a move
				 *
				 * @param move The move to play
				 * @param white If the color is white
				 * @return If the move was played (bool)
				 */
				bool move(PlayeChessEngine::Move move, bool white) {
					if (this->board[move.get_start_coords()[0]][move.get_start_coords()[1]] == nullptr)
						return false;
                    if(this->board[move.get_start_coords()[0]][move.get_start_coords()[1]]->is_white != white) 
                        return false;
					if (move.am_in(this->get_moves(move.get_start_coords()[0], move.get_start_coords()[1]))) {
						this->board = this->transfer(this->board, move.get_start_coords()[0], move.get_start_coords()[1], move.get_end_coords()[0], move.get_end_coords()[1]);
						this->board[move.get_end_coords()[0]][move.get_end_coords()[1]]->update_coords(move.get_end_coords()[0], move.get_end_coords()[1]);
						return true;
					}
					return false;
				}

				/**
				 * @brief Gets the status of the game
				 *
				 * @param white
				 * @return 0 if the game is not over, 1 if it is checkmate, 2 if it is
				 * stalemate (int)
				 */
				int status(bool white) {
					if (this->get_all_moves(this->board, white).size() == 0) {
						if (this->is_check(white))
							return 1;
						else
							return 2;
					}
					return 0;
				}

				bool can_castle_row(int row, bool kingside) {
					if (kingside) {
						if (this->board[row][4] != nullptr && this->board[row][7] != nullptr && this->board[row][5] == nullptr && this->board[row][6] == nullptr) {
							if (this->board[row][4]->get_type() == pieces::piece_type::k && this->board[row][7]->get_type() == pieces::piece_type::r && !this->board[row][4]->has_moved && !this->board[row][7]->has_moved) {
								if (!this->is_check(this->board[row][4]->is_white)) {
									if(!in(this->get_all_landing_moves(this->board, row != 0), {row, 5}) && !in(this->get_all_landing_moves(this->board, row != 0), {row, 6}))
										return true;
									return false;
								}
								return false;
							}
							return false;
						}
						return false;
					}
					if (this->board[row][4] != nullptr && this->board[row][0] != nullptr && this->board[row][1] == nullptr && this->board[row][2] == nullptr && this->board[row][3] == nullptr) {
						if (this->board[row][4]->get_type() == pieces::piece_type::k && this->board[row][0]->get_type() == pieces::piece_type::r && !this->board[row][4]->has_moved && !this->board[row][0]->has_moved) {
							if (!this->is_check(this->board[row][4]->is_white)) {
								if(!in(this->get_all_landing_moves(this->board, row != 0), {row, 1}) && !in(this->get_all_landing_moves(this->board, row != 0), {row, 2}) && !in(this->get_all_landing_moves(this->board, row != 0), {row, 3}))
									return true;
								return false;
							}
							return false;
						}
						return false;
					}
					return false;
				}

				bool can_castle(bool white, bool kingside) {
					if (white)
						return this->can_castle_row(0, kingside);
					return this->can_castle_row(7, kingside);
				}

				void castle(bool is_white, bool kingside) {
					if (is_white) {
						if (kingside) {
							this->board[0][6] = this->board[0][4];
							this->board[0][5] = this->board[0][7];
							this->board[0][4] = nullptr;
							this->board[0][7] = nullptr;
							this->board[0][6]->update_coords(0, 6);
							this->board[0][5]->update_coords(0, 5);
						} else {
							this->board[0][2] = this->board[0][4];
							this->board[0][3] = this->board[0][0];
							this->board[0][4] = nullptr;
							this->board[0][0] = nullptr;
							this->board[0][2]->update_coords(0, 2);
							this->board[0][3]->update_coords(0, 3);
						}
					} else {
						if (kingside) {
							this->board[7][6] = this->board[7][4];
							this->board[7][5] = this->board[7][7];
							this->board[7][4] = nullptr;
							this->board[7][7] = nullptr;
							this->board[7][6]->update_coords(7, 6);
							this->board[7][5]->update_coords(7, 5);
						} else {
							this->board[7][2] = this->board[7][4];
							this->board[7][3] = this->board[7][0];
							this->board[7][4] = nullptr;
							this->board[7][0] = nullptr;
							this->board[7][2]->update_coords(7, 2);
							this->board[7][3]->update_coords(7, 3);
						}
					}
				}
		};
	} // namespace board

	// TODO Draw conditions
	// check_fifty_move_rule()
	// check_threefold_repetition()
	// check_insufficient_material()

	// TODO Promotion
	// check_promotion()
	// promote()

	// TODO En passant
	// check_en_passant()
	// en_passant()

	/**
	* @brief PCE is the actual chess engine
	*
	*/
	class PCE {
		private:
			/**
			* @brief The moves played
			*
			*/
			std::vector<Move> moves;
			/**
			* @brief The board
			*
			*/
			PlayeChessEngine::board::Board board = PlayeChessEngine::board::Board("r3k2r/8/8/8/8/8/8/R3K2R");
			// Checkmate fen : 7k/Q7/6K1/8/8/8/8/8
			// Stalemate fen : 7k/8/8/8/8/8/8/R5RK
			// Castling fen : r3k2r/8/8/8/8/8/8/R3K2R
			// En passant fen : 7k/7p/6P1/8/8/8/8/7K
			// King fen : 5k4/8/8/8/8/8/8/5K4

			void clear_screen() {
				#ifdef _WIN32
					std::system("cls");
				#else
					std::system("clear");
				#endif
			}

		public:
			/**
			* @brief Construct a new PCE object
			*
			*/
			PCE() {}

			/**
			* @brief Plays a move
			*
			* @param white If the color is white
			* @return If the move was played (bool)
			*/
			bool move(bool white) {
				this->clear_screen();
				std::vector<Move> moves = this->board.get_all_moves(this->board.get_board(), white);
				for (auto move : moves) {
					std::cout << move.show() << std::endl;
				}
				std::string move;
				for(auto i : this->board.get_all_landing_moves(this->board.get_board(), !white)){
					std::cout << i[0] << " - " << i[1] << std::endl;
				}
				if (white)
					std::cout << "White to play" << std::endl;
				else
					std::cout << "Black to play" << std::endl;
				this->board.print_board(this->board.get_all_landing_moves(this->board.get_board(), !white));
				bool valid = false;
				while (!valid) {
					do {
						std::cin >> move;
					} while (move.length() != 4 && move != "exit" && move != "O-O" && move != "O-O-O");
					if (move == "exit")
						return true;
					if (move == "O-O") {
						if (this->board.can_castle(white, true)) {
							this->board.castle(white, true);
							return false;
						}
					}
					if (move == "O-O-O") {
						if (this->board.can_castle(white, false)) {
							this->board.castle(white, false);
							return false;
						}
					}
                    /*for(auto move : this->board.get_moves(move[1] - '1', move[0] - 'a')) {
                        std::cout << move.show() << std::endl;
                    }*/
					Move move_obj = Move(move[1] - '1', move[0] - 'a', move[3] - '1', move[2] - 'a');
					valid = this->board.move(move_obj, white);
					if (valid)
						this->moves.push_back(move_obj);
				}
				return false;
			}

			/**
			* @brief Starts the game
			*
			*/
			void main() {
				int move_count = 0;
				bool break_loop = false;
				while (true) {
					bool white = move_count % 2 == 0;
					break_loop = this->move(white);
					if (break_loop)
						break;
					if (this->board.status(!white) == 1) {
						this->clear_screen();
						this->board.print_board();
						if (white)
							std::cout << "White wins (checkmate)" << std::endl;
						else
							std::cout << "Black wins (checkmate)" << std::endl;
						break;
					} else if (this->board.status(!white) == 2) {
						this->clear_screen();
						this->board.print_board();
						std::cout << "Draw (stalemate)" << std::endl;
						break;
					}
					move_count++;
				}
				for (int i = 0; i < this->moves.size(); i += 2) {
					std::cout << i / 2 + 1 << ".. " << this->moves[i].show();
					if (i + 1 < this->moves.size())
						std::cout << " " << this->moves[i + 1].show() << std::endl;
					else
						std::cout << std::endl;
				}
			}
	};
} // namespace PlayeChessEngine
