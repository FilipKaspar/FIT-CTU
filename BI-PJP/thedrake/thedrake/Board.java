package thedrake;

import java.io.PrintWriter;

public class Board implements JSONSerializable{

    private int dimension;
    private Board.TileAt[][] tiles;

    public Board(int dimension) {
        this.dimension = dimension;
        this.tiles = new Board.TileAt[dimension][dimension];
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                this.tiles[i][j] = new Board.TileAt(new BoardPos(dimension, i, j), BoardTile.EMPTY);
            }
        }
    }

    public int dimension() {
        return this.dimension;
    }

    public BoardTile at(TilePos pos) {
        return this.tiles[pos.i()][pos.j()].tile;
    }

    public Board withTiles(TileAt... ats) {
        Board new_board = new Board(this.dimension);

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                if(new_board.tiles[i][j].tile == BoardTile.EMPTY){
                    new_board.tiles[i][j] = this.tiles[i][j];
                }
            }
        }
        for (TileAt tile : ats) {
            new_board.tiles[tile.pos.i()][tile.pos.j()] = tile;
        }

        return new_board;
    }

    public PositionFactory positionFactory() {
        return new PositionFactory(dimension);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{");
        writer.printf("\"dimension\":%d,", this.dimension);
        writer.printf("\"tiles\":[");

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                this.tiles[j][i].tile.toJSON(writer);
                if(i < dimension - 1 || j < dimension -1) writer.printf(",");
            }
        }

        writer.printf("]}");
    }

    public static class TileAt {
        public final BoardPos pos;
        public final BoardTile tile;

        public TileAt(BoardPos pos, BoardTile tile) {
            this.pos = pos;
            this.tile = tile;
        }
    }
}

