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

    namespace board {

        namespace pieces {

            enum piece_type { p, r, n, b, q, k, null };

            class Piece {
                private:
                    bool is_white;
                    piece_type type;
                public:
                    Piece(piece_type type, bool is_white) {
                        this->type = type;
                        this->is_white = is_white;
                    }

                    bool move(int x1, int y1, bool (*validation_function)(int, int)) {
                        return validation_function(x1, y1);
                    }

                    std::string show() {
                        if (is_white) {
                            switch (type) {
                                case p:
                                    return "P";
                                case r:
                                    return "R";
                                case n:
                                    return "N";
                                case b:
                                    return "B";
                                case q:
                                    return "Q";
                                case k:
                                    return "K";
                                default:
                                    return " ";
                            }
                        } else {
                            switch (type) {
                                case p:
                                    return "p";
                                case r:
                                    return "r";
                                case n:
                                    return "n";
                                case b:
                                    return "b";
                                case q:
                                    return "q";
                                case k:
                                    return "k";
                                default:
                                    return " ";
                            }
                        }
                    }
            };

            class NullPiece : public Piece {
                public:
                    NullPiece() : Piece(null, false) {};
            };

            class Pawn : public Piece {
                public:
                    Pawn(bool is_white) : Piece(p, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };

            class Rook : public Piece {
                public:
                    Rook(bool is_white) : Piece(r, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };

            class Knight : public Piece {
                public:
                    Knight(bool is_white) : Piece(n, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };

            class Bishop : public Piece {
                public:
                    Bishop(bool is_white) : Piece(b, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };

            class Queen : public Piece {
                public:
                    Queen(bool is_white) : Piece(q, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };

            class King : public Piece {
                public:
                    King(bool is_white) : Piece(k, is_white) {};
                    bool move(int x1, int y1) {
                        return true;
                    };
            };
        }

        class Board {
            private:
                std::array<std::array<pieces::Piece, 8>, 8> board = {{
                    {pieces::Rook(true), pieces::Knight(true), pieces::Bishop(true), pieces::Queen(true), pieces::King(true), pieces::Bishop(true), pieces::Knight(true), pieces::Rook(true)},
                    {pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true), pieces::Pawn(true)},
                    {pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece()},
                    {pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece()},
                    {pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece()},
                    {pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece(), pieces::NullPiece()},
                    {pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false), pieces::Pawn(false)},
                    {pieces::Rook(false), pieces::Knight(false), pieces::Bishop(false), pieces::Queen(false), pieces::King(false), pieces::Bishop(false), pieces::Knight(false), pieces::Rook(false)}
                }};
            public:
                Board() {};
                void print_board() {
                    for (int i = 0; i < 8; i++) {
                        for (int j = 0; j < 8; j++) {
                            std::cout << board[7 - i][7 - j].show() << " ";
                        }
                        std::cout << std::endl;
                    }
                }
        };
    }

    class Move {
        public:
            Move() {};
    };

    class PCE {
        private:
            std::vector<Move> moves;
            PlayeChessEngine::board::Board board;
        public:
            PCE() {
                board.print_board();
            }
            ~PCE() {
                std::cout << "PCE has been destroyed" << std::endl;
            }
    };
}