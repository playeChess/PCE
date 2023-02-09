/*
 > PCE - Play eChess Engine
 > Code by SinisterIcy
 > Version INFDEV
 > Licensed under the GNU General Public License v3.0
 > Github: https://github.com/PCE-Engine/PCE-Cpp
*/

#include <iostream>
#include <array>
#include <vector>

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
                pieces::Rook R1 = pieces::Rook(true, 0, 0);
                pieces::Rook R2 = pieces::Rook(true, 0, 7);
                pieces::Knight N1 = pieces::Knight(true, 0, 1);
                pieces::Knight N2 = pieces::Knight(true, 0, 6);
                pieces::Bishop B1 = pieces::Bishop(true, 0, 2);
                pieces::Bishop B2 = pieces::Bishop(true, 0, 5);
                pieces::Queen Q = pieces::Queen(true, 0, 3);
                pieces::King K = pieces::King(true, 0, 4);
                pieces::Pawn P1 = pieces::Pawn(true, 1, 0);
                pieces::Pawn P2 = pieces::Pawn(true, 1, 1);
                pieces::Pawn P3 = pieces::Pawn(true, 1, 2);
                pieces::Pawn P4 = pieces::Pawn(true, 1, 3);
                pieces::Pawn P5 = pieces::Pawn(true, 1, 4);
                pieces::Pawn P6 = pieces::Pawn(true, 1, 5);
                pieces::Pawn P7 = pieces::Pawn(true, 1, 6);
                pieces::Pawn P8 = pieces::Pawn(true, 1, 7);
                pieces::Rook r1 = pieces::Rook(false, 7, 0);
                pieces::Rook r2 = pieces::Rook(false, 7, 7);
                pieces::Knight n1 = pieces::Knight(false, 7, 1);
                pieces::Knight n2 = pieces::Knight(false, 7, 6);
                pieces::Bishop b1 = pieces::Bishop(false, 7, 2);
                pieces::Bishop b2 = pieces::Bishop(false, 7, 5);
                pieces::Queen q = pieces::Queen(false, 7, 3);
                pieces::King k = pieces::King(false, 7, 4);
                pieces::Pawn p1 = pieces::Pawn(false, 6, 0);
                pieces::Pawn p2 = pieces::Pawn(false, 6, 1);
                pieces::Pawn p3 = pieces::Pawn(false, 6, 2);
                pieces::Pawn p4 = pieces::Pawn(false, 6, 3);
                pieces::Pawn p5 = pieces::Pawn(false, 6, 4);
                pieces::Pawn p6 = pieces::Pawn(false, 6, 5);
                pieces::Pawn p7 = pieces::Pawn(false, 6, 6);
                pieces::Pawn p8 = pieces::Pawn(false, 6, 7);

            public:
                Board() {
                    this->board[0][0] = &R1;
                    this->board[0][1] = &N1;
                    this->board[0][2] = &B1;
                    this->board[0][3] = &Q;
                    this->board[0][4] = &K;
                    this->board[0][5] = &B2;
                    this->board[0][6] = &N2;
                    this->board[0][7] = &R2;
                    this->board[1][0] = &P1;
                    this->board[1][1] = &P2;
                    this->board[1][2] = &P3;
                    this->board[1][3] = &P4;
                    this->board[1][4] = &P5;
                    this->board[1][5] = &P6;
                    this->board[1][6] = &P7;
                    this->board[1][7] = &P8;

                    this->board[7][0] = &r1;
                    this->board[7][1] = &n1;
                    this->board[7][2] = &b1;
                    this->board[7][3] = &q;
                    this->board[7][4] = &k;
                    this->board[7][5] = &b2;
                    this->board[7][6] = &n2;
                    this->board[7][7] = &r2;
                    this->board[6][0] = &p1;
                    this->board[6][1] = &p2;
                    this->board[6][2] = &p3;
                    this->board[6][3] = &p4;
                    this->board[6][4] = &p5;
                    this->board[6][5] = &p6;
                    this->board[6][6] = &p7;
                    this->board[6][7] = &p8;
                };
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