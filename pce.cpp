/*
 > PCE - Play eChess Engine
 > Code by SinisterIcy
 > Version INFDEV
 > Licensed under the GNU General Public License v3.0
 > Github: https://github.com/PCE-Engine/PCE
*/

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <Windows.h>

/**
 * @brief PlayeChessEngine is the namespace for the PCE engine who is made for the eChess project (playechess.com)
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
            /**
             * @brief The castle type (0 = no castle, 1 = short castle, 2 = long castle)
            */
            int castle;
        public:
            /**
             * @brief Construct a new Move object
             * 
             * @param start_square_x The x coordinate of the start square
             * @param start_square_y The y coordinate of the start square
             * @param end_square_x The x coordinate of the end square
             * @param end_square_y The y coordinate of the end square
            */
            Move(int start_square_x, int start_square_y, int end_square_x, int end_square_y) {
                this->start_square_x = start_square_x;
                this->start_square_y = start_square_y;
                this->end_square_x = end_square_x;
                this->end_square_y = end_square_y;
                this->castle = castle;
            };

            /**
             * @brief Construct a new Move object (for castling)
             * 
             * @param castle The castle type (0 = no castle, 1 = short castle, 2 = long castle)
             */
            Move(int castle) {
                this->castle = castle;
            }

            /**
             * @brief Shows the move in a readable format
             * 
             * @return Formatted move (std::string)
            */
            std::string show() {
                std::string files = "abcdefgh";
                if(castle == 1)
                    return "O-O";
                if(castle == 2)
                    return "O-O-O";
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
                for(int i = 0; i < moves.size(); i++) {
                    if(moves[i].get_end_coords()[0] == this->get_end_coords()[0] && moves[i].get_end_coords()[1] == this->get_end_coords()[1])
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
             * @brief The piece type enum (p = pawn, r = rook, n = knight, b = bishop, q = queen, k = king)
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
                     * @brief If the piece has moved (mainly used for castling)
                    */
                    bool has_moved = false;

                    /**
                     * @brief If the piece has moved two squares (mainly used for en passant)
                    */
                    bool has_moved_two = false;

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
                    virtual bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) { return false; };

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

                    /**
                     * @brief Get the type of the piece
                     * 
                     * @return The type (piece_type enum)
                     */
                    piece_type get_type() {
                        return this->type;
                    }

                    /**
                     * @brief Validates the move of the piece (checks if the landing square is valid)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the landing square is valid (bool)
                     */
                    bool validate_validation(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        if(board[x_final][y_final] == nullptr)
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
                    bool check_path(int x_final, int y_final, std::array<std::array<Piece*, 8>, 8> board) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if(x_diff == 0) {
                            if(y_diff > 0)
                                for(int i = 1; i < y_diff; i++) {
                                    if(board[this->coords[0]][this->coords[1] + i] != nullptr)
                                        return false;
                                }
                            else
                                for(int i = 1; i < abs(y_diff); i++) {
                                    if(board[this->coords[0]][this->coords[1] - i] != nullptr)
                                        return false;
                                }
                        } else if(y_diff == 0) {
                            if(x_diff > 0)
                                for(int i = 1; i < x_diff; i++) {
                                    if(board[this->coords[0] + i][this->coords[1]] != nullptr)
                                        return false;
                                }
                            else
                                for(int i = 1; i < abs(x_diff); i++) {
                                    if(board[this->coords[0] - i][this->coords[1]] != nullptr)
                                        return false;
                                }
                        } else if(abs(x_diff) == abs(y_diff)) {
                            if(x_diff > 0 && y_diff > 0) {
                                for(int i = 1; i < x_diff; i++) {
                                    if(board[this->coords[0] + i][this->coords[1] + i] != nullptr)
                                        return false;
                                }
                            } else if (x_diff < 0 && y_diff < 0) {
                                for(int i = 1; i < abs(x_diff); i++) {
                                    if(board[this->coords[0] - i][this->coords[1] - i] != nullptr)
                                        return false;
                                }
                            } else if (x_diff > 0 && y_diff < 0) {
                                for(int i = 1; i < x_diff; i++) {
                                    if(board[this->coords[0] + i][this->coords[1] - i] != nullptr)
                                        return false;
                                }
                            } else if (x_diff < 0 && y_diff > 0) {
                                for(int i = 1; i < abs(x_diff); i++) {
                                    if(board[this->coords[0] - i][this->coords[1] + i] != nullptr)
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
                     * @param is_white Whether the pawn is white or black (true = white, false = black)
                     * @param x The x coordinate of the pawn
                     * @param y The y coordinate of the pawn
                     */
                    Pawn(bool is_white, int x, int y) : Piece(p, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the pawn (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        // TODO Add pawn takes
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];

                        if (y_diff == 0 && x_diff != 0) {
                            if (this->is_white) {
                                if (x_diff == 1) {
                                    if (board[x_final][y_final] == nullptr)
                                        return true;
                                } else if (x_diff == 2 && !this->has_moved) {
                                    if (board[x_final][y_final] == nullptr && board[x_final][y_final - 1] == nullptr) {
                                        this->has_moved_two = true;
                                        return true;
                                    }
                                }
                            } else {
                                if (x_diff == -1) {
                                    if (board[x_final][y_final] == nullptr)
                                        return true;
                                } else if (x_diff == -2 && !has_moved) {
                                    if (board[x_final][y_final] == nullptr && board[x_final][y_final + 1] == nullptr) {
                                        this->has_moved_two = true;
                                        return true;
                                    }
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
                public:
                    /**
                     * @brief Construct a new Rook object
                     * 
                     * @param is_white Whether the rook is white or black (true = white, false = black)
                     * @param x The x coordinate of the rook
                     * @param y The y coordinate of the rook
                     */
                    Rook(bool is_white, int x, int y) : Piece(r, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the rook (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if (x_diff * y_diff == 0 && x_diff != y_diff) {
                            if(validate_validation(board, x_final, y_final))
                                return this->check_path(x_final, y_final, board);
                        }
                        return false;
                    };
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
                     * @param is_white Whether the knight is white or black (true = white, false = black)
                     * @param x The x coordinate of the knight
                     * @param y The y coordinate of the knight
                     */
                    Knight(bool is_white, int x, int y) : Piece(n, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the knight (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2))
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
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
                     * @param is_white Whether the bishop is white or black (true = white, false = black)
                     * @param x The x coordinate of the bishop
                     * @param y The y coordinate of the bishop
                     */
                    Bishop(bool is_white, int x, int y) : Piece(b, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the bishop (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if (abs(x_diff) == abs(y_diff) && x_diff != 0) {
                            if(validate_validation(board, x_final, y_final))
                                return this->check_path(x_final, y_final, board);
                        }
                        return false;
                    };
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
                     * @param is_white Whether the queen is white or black (true = white, false = black)
                     * @param x The x coordinate of the queen
                     * @param y The y coordinate of the queen
                     */
                    Queen(bool is_white, int x, int y) : Piece(q, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the queen (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if ((abs(x_diff) == abs(y_diff) && x_diff != 0) || ((x_diff == 0 || y_diff == 0) && x_diff != y_diff)) {
                            if(validate_validation(board, x_final, y_final))
                                return this->check_path(x_final, y_final, board);
                        }
                        return false;
                    };
            };

            /**
             * @brief The king piece (inherits from Piece)
             * 
             */
            class King : public Piece {
                public:
                    /**
                     * @brief Construct a new King object
                     * 
                     * @param is_white Whether the king is white or black (true = white, false = black)
                     * @param x The x coordinate of the king
                     * @param y The y coordinate of the king
                     */
                    King(bool is_white, int x, int y) : Piece(k, is_white, x, y) {};

                    /**
                     * @brief Validates the move of the king (combines the other validation functions)
                     * 
                     * @param board The board
                     * @param x_final The x coordinate of the landing square
                     * @param y_final The y coordinate of the landing square
                     * @return If the move is valid (bool)
                     */
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if (abs(x_diff) <= 1 && abs(y_diff) <= 1 && (x_diff != 0 || y_diff != 0))
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };
        }

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
                std::array<std::array<pieces::Piece*, 8>, 8> board = {{{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}}};

            public:
                // TODO Add the funcitonnality for w KQkq - 0 1
                /**
                 * @brief Construct a new Board object
                 * 
                 * @param fen The fen string
                 */
                Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
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
                    for(auto c : fen) {
                        if(c == '/') {
                            fen_split.push_back(buffer + '/');
                            buffer = "";
                        } else {
                            buffer += c;
                        }
                    }
                    fen_split.push_back(buffer + '/');
                    for(int i = fen_split.size() - 1; i >= 0; i--) {
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
                    for(auto c : fen) {
                        if(c == '/') {
                            y = 0;
                            x++;
                        } else if(c == '8') {
                            continue;
                        } else if(c >= '0' && c <= '9') {
                            y += c - '0';
                        } else {
                            bool is_white = c >= 'A' && c <= 'Z';
                            switch(c) {
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
                void print_board(std::vector<std::vector<int>> moves = {}, std::array<std::array<pieces::Piece*, 8>, 8> board = {}) {
                    if(board == std::array<std::array<pieces::Piece*, 8>, 8> {})
                        board = this->board;
                    std::cout << "  #-----------------#" << std::endl;
                    for (int i = 0; i < 8; i++) {
                        std::cout << 8 - i << " | ";
                        for (int j = 0; j < 8; j++) {
                            bool skip = false;
                            for(auto move : moves) {
                                if(move[0] == 7 - i && move[1] == j) {
                                    std::cout << "* ";
                                    skip = true;
                                    break;
                                }
                            }
                            if(skip)
                                continue;
                            if(board[7 - i][j] != nullptr) {
                                std::cout << board[7 - i][j]->show() << " ";
                            } else {
                                std::cout << "  ";
                            }
                        }
                        std::cout << "|" << std::endl;
                    }
                    std::cout << "  #-----------------#" << std::endl;
                    std::cout <<  "    a b c d e f g h" << std::endl;
                }

                /**
                 * @brief Gets the board
                 * 
                 * @return The board (std::array<std::array<pieces::Piece*, 8>, 8>)
                 */
                std::array<std::array<pieces::Piece*, 8>, 8> get_board() {
                    return this->board;
                };

                /**
                 * @brief Gets a piece from the board
                 * 
                 * @param x The x coordinate
                 * @param y The y coordinate
                 * @return The piece (pieces::Piece*)
                 */
                pieces::Piece* get_piece(int x, int y) {
                    return this->board[x][y];
                };

                /**
                 * @brief Gets the moves for a piece
                 * 
                 * @param x The x coordinate
                 * @param y The y coordinate
                 * @param from_premove If the function is called from fmove_check (to prevent infinite recursion)
                 * @return The moves (std::vector<PlayeChessEngine::Move>)
                 */
                std::vector<PlayeChessEngine::Move> get_moves(int x, int y, bool from_premove = false) {
                    std::vector<PlayeChessEngine::Move> moves;
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(this->board[x][y] == nullptr)
                                continue;
                            if(this->board[x][y]->validation_function(this->board, i, j)) {
                                PlayeChessEngine::Move move = PlayeChessEngine::Move(x, y, i, j);
                                if(from_premove) {
                                    moves.push_back(move);
                                    continue;
                                }
                                if(!this->fmove_check(move, this->board[x][y]->is_white)) {
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
                 * @param from_premove If the function is called from fmove_check (to prevent infinite recursion)
                 * @return The moves (std::vector<PlayeChessEngine::Move>)
                 */
                std::vector<PlayeChessEngine::Move> get_all_moves(std::array<std::array<pieces::Piece*, 8>, 8> brd, bool white, bool from_premove = false) {
                    std::vector<PlayeChessEngine::Move> moves;
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(brd[i][j] == nullptr)
                                continue;
                            if(brd[i][j]->is_white != white)
                                continue;
                            for(auto move : this->get_moves(i, j, from_premove)) {
                                moves.push_back(move);
                            }
                        }
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
                    for(auto move : this->get_all_moves(this->board, !white, true)) {
                        if(this->board[move.get_end_coords()[0]][move.get_end_coords()[1]] == nullptr)
                            continue;
                        if(this->board[move.get_end_coords()[0]][move.get_end_coords()[1]]->get_type() == pieces::piece_type::k)
                            return true;
                    }
                    return false;
                }

                /**
                 * @brief Transfer a piece from a position to another
                 * 
                 * @param start_x The x coordinate of the start position
                 * @param start_y The y coordinate of the start position
                 * @param end_x The x coordinate of the end position
                 * @param end_y The y coordinate of the end position
                 */
                void transfer(int start_x, int start_y, int end_x, int end_y) {
                    std::cout << "Transfering piece from " << start_x << ", " << start_y << " to " << end_x << ", " << end_y << std::endl;
                    std::string t;
                    std::cin >> t;
                    this->board[end_x][end_y] = this->board[start_x][start_y];
                    this->board[start_x][start_y] = nullptr;
                    this->board[end_x][end_y]->has_moved = true;
                    this->board[end_x][end_y]->update_coords(end_x, end_y);
                }

                /**
                 * @brief Plays one move in the future to check if it puts the king in check
                 * 
                 * @param move The move to check
                 * @param white If the color is white
                 * @return If the move is legal (bool)
                 */
                bool fmove_check(PlayeChessEngine::Move move, bool white) {
                    if(this->board[move.get_start_coords()[0]][move.get_start_coords()[1]] == nullptr)
                        throw std::invalid_argument("No piece at start coords");
                    std::array<std::array<pieces::Piece*, 8>, 8> backup = this->board;
                    this->transfer(move.get_start_coords()[0], move.get_start_coords()[1], move.get_end_coords()[0], move.get_end_coords()[1]);
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
                    if(this->board[move.get_start_coords()[0]][move.get_start_coords()[1]] == nullptr || this->board[move.get_start_coords()[0]][move.get_start_coords()[1]]->is_white != white)
                        return false;
                    if(move.am_in(this->get_moves(move.get_start_coords()[0], move.get_start_coords()[1]))) {
                        this->transfer(move.get_start_coords()[0], move.get_start_coords()[1], move.get_end_coords()[0], move.get_end_coords()[1]);
                        return true;
                    }
                    return false;
                }

                /**
                 * @brief Gets the status of the game
                 * 
                 * @param white 
                 * @return 0 if the game is not over, 1 if it is checkmate, 2 if it is stalemate (int)
                 */
                int status(bool white) {
                    if(this->get_all_moves(this->board, white).size() == 0) {
                        if(this->is_check(white)) {
                            return 1;
                        } else {
                            return 2;
                        }
                    }
                    return 0;
                }

                /**
                 * @brief Checks if a row can castle
                 * 
                 * @param row The row to check
                 * @param king_side If you want to check the king side
                 * @return Whether the row can castle (bool)
                 */
                bool can_castle_row(int row, bool king_side) {
                    if(king_side) {
                        if(this->board[row][4] != nullptr && this->board[row][5] == nullptr && this->board[row][6] == nullptr && this->board[row][7] != nullptr) {
                            if(this->board[row][4]->get_type() == pieces::piece_type::k && this->board[row][7]->get_type() == pieces::piece_type::r) {
                                if(!this->board[row][4]->has_moved && !this->board[row][7]->has_moved) {
                                    return true;
                                }
                                return false;
                            }
                            return false;
                        }
                        return false;
                    } else {
                        if(this->board[row][4] != nullptr && this->board[row][3] == nullptr && this->board[row][2] == nullptr && this->board[row][1] == nullptr && this->board[row][0] != nullptr) {
                            if(this->board[row][4]->get_type() == pieces::piece_type::k && this->board[row][0]->get_type() == pieces::piece_type::r) {
                                if(!this->board[row][4]->has_moved && !this->board[row][0]->has_moved) {
                                    return true;
                                }
                                return false;
                            }
                            return false;
                        }
                        return false;
                    }
                }

                /**
                 * @brief Checks if a player can castle
                 * 
                 * @param white If the player is white
                 * @param king_side If you want to check the king side
                 * @return Whether the player can castle (bool)
                 */
                bool can_castle(bool white, bool king_side) {
                    if(white) {
                        return this->can_castle_row(0, king_side);
                    }
                    return this->can_castle_row(7, king_side);
                }

                /**
                 * @brief Castles a row
                 * 
                 * @param row The row to castle
                 * @param king_side If you want to castle the king side
                 */
                void castle_row(int row, bool king_side) {
                    if(king_side) {
                        this->transfer(row, 7, row, 5);
                        this->transfer(row, 4, row, 6);

                        this->transfer(row, 0, row, 3);
                        this->transfer(row, 4, row, 2);
                    }
                }

                /**
                 * @brief Castles a player
                 * 
                 * @param white If the player is white
                 * @param king_side If you want to castle the king side
                 */
                void castle(bool white, bool king_side) {
                    if(white) {
                        this->castle_row(0, king_side);
                    } else {
                        this->castle_row(7, king_side);
                    }
                }

                /**
                 * @brief Moves en passant
                 * 
                 * @param row The row to move
                 * @param col The column to move
                 */
                void en_passant(int row, int col) {
                    if(this->board[row][col]->get_type() == pieces::piece_type::p) {
                        if(this->board[row][col]->is_white) {
                            this->board[row + 1][col] = nullptr;
                        } else {
                            this->board[row - 1][col] = nullptr;
                        }
                    }
                }
        };
    }

    // TODO Castle
        // Update castling rights (through check)

    // TODO Draw conditions
        // check_fifty_move_rule()
        // check_threefold_repetition()
        // check_insufficient_material()

    // TODO Promotion
        // check_promotion()
        // promote()
    
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
            // Checkmate fen : rnbqkbnr/2pp1ppp/pp6/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR
            // Stalemate fen : 4k3/8/8/8/8/8/8/3RKR1R
            // Castling fen : r3k2r/8/8/8/8/8/8/R3K2R
            // En passant fen : 8/7p/6P1/8/8/8/8/8
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
                system("cls");
                std::vector <Move> moves = this->board.get_all_moves(this->board.get_board(), white);
                std::string move;
                if(white)
                    std::cout << "White to play" << std::endl;
                else
                    std::cout << "Black to play" << std::endl;
                this->board.print_board();
                bool valid = false;
                while(!valid) {
                    std::cin >> move;
                    if(move == "exit")
                        return true;
                    if(move == "O-O") {
                        if(this->board.can_castle(white, true)) {
                            this->board.castle(white, true);
                            this->moves.push_back(Move(1));
                            return false;
                        } return false;
                    } if(move == "O-O-O") {
                        if(this->board.can_castle(white, false)) {
                            this->board.castle(white, false);
                            this->moves.push_back(Move(2));
                            return false;
                        } return false;
                    }
                    while(move.length() != 4) {
                        std::cout << "Invalid move" << std::endl;
                        std::cin >> move;
                    }
                    std::array<int, 2> start_coords = {{move[1] - '1', move[0] - 'a'}};
                    std::array<int, 2> end_coords = {{move[3] - '1', move[2] - 'a'}};
                    Move move_obj = Move(start_coords[0], start_coords[1], end_coords[0], end_coords[1]);
                    valid = this->board.move(move_obj, white);
                    if(valid)
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
                while(true) {
                    bool white = move_count % 2 == 0;
                    break_loop = this->move(white);
                    if(break_loop)
                        break;
                    if(this->board.status(!white) == 1) {
                        system("cls");
                        this->board.print_board();
                        if(white)
                            std::cout << "White wins (checkmate)" << std::endl;
                        else
                            std::cout << "Black wins (checkmate)" << std::endl;
                        break;
                    } else if(this->board.status(!white) == 2) {
                        system("cls");
                        this->board.print_board();
                        std::cout << "Draw (stalemate)" << std::endl;
                        break;
                    }
                    move_count++;
                }
                for(int i = 0; i < this->moves.size(); i+=2) {
                    std::cout << i / 2 + 1 << ".. " << this->moves[i].show();
                    if(i + 1 < this->moves.size())
                        std::cout << " " << this->moves[i + 1].show() << std::endl;
                    else
                        std::cout << std::endl;
                }
            }
    };
}