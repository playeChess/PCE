/*
 > PCE - Play eChess Engine
 > Code by SinisterIcy
 > Version INFDEV
 > Licensed under the GNU General Public License v3.0
 > Github: https://github.com/PCE-Engine/PCE-Cpp
*/

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

namespace PlayeChessEngine {

    class Move {
        private:
            int start_square_x;
            int start_square_y;
            int end_square_x;
            int end_square_y;
        public:
            Move(int start_square_x, int start_square_y, int end_square_x, int end_square_y) {
                this->start_square_x = start_square_x;
                this->start_square_y = start_square_y;
                this->end_square_x = end_square_x;
                this->end_square_y = end_square_y;
            };
            void show() {
                std::string files = "abcdefgh";
                std::cout << "From " << files[start_square_y] << start_square_x + 1 << " to " << files[end_square_y] << end_square_x + 1 << std::endl;
            }
            std::vector<int> get_coords() {
                return {this->end_square_x, this->end_square_y};
            }
    };

    namespace board {

        namespace pieces {

            enum piece_type { p, r, n, b, q, k };

            class Piece {
                protected:
                    piece_type type;
                    int coords[2] = {0, 0};
                public:
                    bool is_white;
                    Piece(piece_type type, bool is_white, int x, int y) {
                        this->type = type;
                        this->is_white = is_white;
                        this->coords[0] = x;
                        this->coords[1] = y;
                    }

                    virtual bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) { return false; };

                    bool validate_validation(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        if(board[x_final][y_final] == nullptr)
                            return true;
                        Piece piece = *board[x_final][y_final];
                        if (piece.is_white == this->is_white)
                            return false;
                        return true;
                    }

                    bool check_path();

                    std::string show() {
                        if (this->is_white) {
                            std::string piece_names[6] = {"P", "R", "N", "B", "Q", "K"};
                            return piece_names[this->type];
                        }
                        std::string piece_names[6] = {"p", "r", "n", "b", "q", "k"};
                        return piece_names[this->type];
                    }
            };

            class Pawn : public Piece {
                private:
                    bool has_moved = false;
                public:
                    Pawn(bool is_white, int x, int y) : Piece(p, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if(y_diff == 0 && x_diff != 0) {
                            if(this->is_white) {
                                if (x_diff == 1 || (!this->has_moved && x_diff == 2))
                                    return validate_validation(board, x_final, y_final);
                                return false;
                            } else {
                                if (x_diff == -1 || (!this->has_moved && x_diff == -2))
                                    return validate_validation(board, x_final, y_final);
                                return false;
                            }
                        }
                        return false;
                    }
            };

            class Rook : public Piece {
                private:
                    bool has_moved = false;
                public:
                    Rook(bool is_white, int x, int y) : Piece(r, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if ((x_diff == 0 || y_diff == 0) && x_diff != y_diff)
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };

            class Knight : public Piece {
                public:
                    Knight(bool is_white, int x, int y) : Piece(n, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if ((abs(x_diff) == 2 && abs(y_diff) == 1) || (abs(x_diff) == 1 && abs(y_diff) == 2))
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };

            class Bishop : public Piece {
                public:
                    Bishop(bool is_white, int x, int y) : Piece(b, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if (abs(x_diff) == abs(y_diff) && x_diff != 0)
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };

            class Queen : public Piece {
                public:
                    Queen(bool is_white, int x, int y) : Piece(q, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if ((abs(x_diff) == abs(y_diff) && x_diff != 0) || ((x_diff == 0 || y_diff == 0) && x_diff != y_diff))
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };

            class King : public Piece {
                private:
                    bool has_moved = false;
                public:
                    King(bool is_white, int x, int y) : Piece(k, is_white, x, y) {};
                    bool validation_function(std::array<std::array<Piece*, 8>, 8> board, int x_final, int y_final) {
                        int x_diff = x_final - this->coords[0];
                        int y_diff = y_final - this->coords[1];
                        if (abs(x_diff) <= 1 && abs(y_diff) <= 1 && (x_diff != 0 || y_diff != 0))
                            return validate_validation(board, x_final, y_final);
                        return false;
                    };
            };
        }

        class Board {
            private:
                std::array<std::array<pieces::Piece*, 8>, 8> board = {{{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}}};

            public:
                Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
                    std::string fen_board = fen.substr(0, fen.find(" "));
                    std::reverse(fen_board.begin(), fen_board.end());
                    load_fen(fen);
                };

                void load_fen(std::string fen) {
                    std::string fen_board = fen.substr(0, fen.find(" "));
                    std::reverse(fen_board.begin(), fen_board.end());
                    int x = 0;
                    int y = 0;
                    for (int i = 0; i < fen_board.length(); i++) {
                        if (fen_board[i] == '/') {
                            x = 0;
                            y++;
                            continue;
                        }
                        if (fen_board[i] >= '0' && fen_board[i] <= '9') {
                            x += fen_board[i] - '0';
                            continue;
                        }
                        bool is_white = true;
                        if (fen_board[i] >= 'a' && fen_board[i] <= 'z') {
                            is_white = false;
                            fen_board[i] = fen_board[i] - 'a' + 'A';
                        }
                        switch (fen_board[i]) {
                            case 'P':
                                this->board[y][x] = new pieces::Pawn(is_white, x, y);
                                break;
                            case 'R':
                                this->board[y][x] = new pieces::Rook(is_white, x, y);
                                break;
                            case 'N':
                                this->board[y][x] = new pieces::Knight(is_white, x, y);
                                break;
                            case 'B':
                                this->board[y][x] = new pieces::Bishop(is_white, x, y);
                                break;
                            case 'Q':
                                this->board[y][x] = new pieces::Queen(is_white, x, y);
                                break;
                            case 'K':
                                this->board[y][x] = new pieces::King(is_white, x, y);
                                break;
                        }
                        x++;
                    }
                }

                void print_board(std::vector<std::vector<int>> moves = {}) {
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
                            if(this->board[7 - i][7 - j] != nullptr) {
                                std::cout << this->board[7 - i][j]->show() << " ";
                            } else {
                                std::cout << "  ";
                            }
                        }
                        std::cout << "|" << std::endl;
                    }
                    std::cout << "  #-----------------#" << std::endl;
                    std::cout <<  "    a b c d e f g h" << std::endl;
                }

                pieces::Piece* get_piece(int x, int y) {
                    return this->board[x][y];
                };

                /*void show_moves(int x, int y) {
                    std::vector<PlayeChessEngine::Move> moves;
                    std::cout << "Piece: " << this->board[x][y]->show() << std::endl;
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            // std::cout << "Checking: " << i << " " << j << std::endl;
                            if(this->board[x][y]->validation_function(this->board, i - x, j - y)) {
                                PlayeChessEngine::Move(x, y, i, j).show();
                                // std::cout << "Move: " << x << " " << y << " " << i << " " << j << std::endl;
                            }
                        }
                    }
                }*/

                std::vector<PlayeChessEngine::Move> get_moves(int x, int y) {
                    std::vector<PlayeChessEngine::Move> moves;
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(this->board[x][y]->validation_function(this->board, i, j)) {
                                moves.push_back(PlayeChessEngine::Move(x, y, i, j));
                            }
                        }
                    }
                    return moves;
                }
        };
    }
    
    // TODO anti_autocheck
        // predict_move()
        // king_eatable()
        // move_back

    // TODO check_check
    // TODO check_checkmate
    // TODO Draw conditions
        // check_stalemate()
        // check_fifty_move_rule()
        // check_threefold_repetition()
        // check_insufficient_material()

    // TODO Promotion
        // check_promotion()
        // promote()

    // TODO Castling
        // check_castling()
        // castle()

    // TODO En passant
        // check_en_passant()
        // en_passant()
    
    class PCE {
        private:
            std::vector<Move> moves;
            PlayeChessEngine::board::Board board;
        public:
            PCE() {
                for(int i = 0; i < 8; i++) {
                    this->moves = board.get_moves(0, i);
                    std::vector<std::vector<int>> mvsc;
                    for(auto move : this->moves) {
                        mvsc.push_back(move.get_coords());
                    }
                    board.print_board(mvsc);
                }
            }
    };
}