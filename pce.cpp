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
#include <algorithm>

namespace PlayeChessEngine {

    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }

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
            std::vector<int> get_start_coords() {
                return {this->start_square_x, this->start_square_y};
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
                    std::array<std::array<int, 8>, 8> test = {{{0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {32, 33, 34, 35, 36, 37, 38, 39}, {40, 41, 42, 43, 44, 45, 46, 47}, {48, 49, 50, 51, 52, 53, 54, 55}, {56, 57, 58, 59, 60, 61, 62, 63}}};
                public:
                    bool is_white;
                    bool is_king = false;
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
                        if (board[x_final][y_final]->is_white == this->is_white)
                            return false;
                        return true;
                    }

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

            class Rook : public Piece {
                private:
                    bool has_moved = false;
                public:
                    Rook(bool is_white, int x, int y) : Piece(r, is_white, x, y) {};
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
                        if (abs(x_diff) == abs(y_diff) && x_diff != 0) {
                            if(validate_validation(board, x_final, y_final))
                                return this->check_path(x_final, y_final, board);
                        }
                        return false;
                    };
            };

            class Queen : public Piece {
                public:
                    Queen(bool is_white, int x, int y) : Piece(q, is_white, x, y) {};
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

            class King : public Piece {
                private:
                    bool has_moved = false;
                public:
                    King(bool is_white, int x, int y) : Piece(k, is_white, x, y) {
                        this->is_king = true;
                    };
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
                // TODO Add the funcitonnality for w KQkq - 0 1
                Board(std::string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
                    fen = fen.substr(0, fen.find(" "));
                    load_fen(fen);
                };

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
                            x += std::stoi(std::string(1, c));
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

                std::array<std::array<pieces::Piece*, 8>, 8> get_board() {
                    return this->board;
                };

                pieces::Piece* get_piece(int x, int y) {
                    return this->board[x][y];
                };

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
                                if(!this->premove_check(move, this->board[x][y]->is_white))
                                    moves.push_back(move);
                            }
                        }
                    }
                    return moves;
                }

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

                bool is_check(bool white) {
                    for(auto move : this->get_all_moves(this->board, !white, true)) {
                        if(this->board[move.get_coords()[0]][move.get_coords()[1]] == nullptr)
                            continue;
                        if(this->board[move.get_coords()[0]][move.get_coords()[1]]->is_king)
                            return true;
                    }
                    return false;
                }

                std::array<std::array<pieces::Piece*, 8>, 8> transfer(std::array<std::array<pieces::Piece*, 8>, 8> board, int start_x, int start_y, int end_x, int end_y) {
                    board[end_x][end_y] = board[start_x][start_y];
                    board[start_x][start_y] = nullptr;
                    return board;
                }

                bool premove_check(PlayeChessEngine::Move move, bool white) {
                    if(this->board[move.get_start_coords()[0]][move.get_start_coords()[1]] == nullptr)
                        throw std::invalid_argument("No piece at start coords");
                    std::array<std::array<pieces::Piece*, 8>, 8> backup = this->board;
                    this->board = this->transfer(this->board, move.get_start_coords()[0], move.get_start_coords()[1], move.get_coords()[0], move.get_coords()[1]);
                    bool check = this->is_check(white);
                    this->board = backup;
                    return check;
                }
        };
    }
    
    // TODO anti_autocheck
        // predict_move()
        // king_eatable()
        // move_back()

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
            PlayeChessEngine::board::Board board = PlayeChessEngine::board::Board("rnbqkbnr/ppppqppp/8/8/8/8/PPPPQPPP/RNBQKBNR");
        public:
            PCE() {
                this->board.print_board();
                for(int i = 0; i < 8; i++) {
                    this->moves = board.get_moves(0, i);
                    std::vector<std::vector<int>> mvsc;
                    for(auto move : this->moves) {
                        mvsc.push_back(move.get_coords());
                    }
                    board.print_board(mvsc);
                }
                for(int i = 0; i < 8; i++) {
                    this->moves = board.get_moves(1, i);
                    std::vector<std::vector<int>> mvsc;
                    for(auto move : this->moves) {
                        mvsc.push_back(move.get_coords());
                    }
                    board.print_board(mvsc);
                }

                for(Move el : this->board.get_all_moves(this->board.get_board(), true)) {
                    el.show();
                }
            }
    };
}