package thedrake;

import java.awt.image.AffineTransformOp;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class TroopTile implements Tile, JSONSerializable{
    private Troop troop;
    private PlayingSide side;
    private TroopFace face;


    public TroopTile(Troop troop, PlayingSide side, TroopFace face){
        this.troop = troop;
        this.side = side;
        this.face = face;
    }

    public PlayingSide side(){
        return this.side;
    }

    public TroopFace face(){
        return this.face;
    }

    public Troop troop(){
        return this.troop;
    }

    public boolean canStepOn(){
        return false;
    }

    public boolean hasTroop(){
        return true;
    }

    @Override
    public List<Move> movesFrom(BoardPos pos, GameState state) {
        List<Move> result = new ArrayList<>();

        List<TroopAction> lol = this.troop.actions(this.face);

        for(TroopAction action : lol){
            result.addAll(action.movesFrom(pos, this.side, state));
        }

        return result;
    }

    public TroopTile flipped(){
        TroopFace flippedFace;
        if(face == TroopFace.AVERS){
            flippedFace = TroopFace.REVERS;
        }
        else{
            flippedFace = TroopFace.AVERS;
        }
        return new TroopTile(troop, side, flippedFace);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{");
        writer.printf("\"troop\":\"%s\",", this.troop().name());
        writer.printf("\"side\":\"%s\",", this.side());
        writer.printf("\"face\":\"%s\"", this.face());
        writer.printf("}");
    }
}
