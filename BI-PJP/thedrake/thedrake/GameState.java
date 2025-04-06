package thedrake;

import java.io.PrintWriter;
import java.util.Optional;

public class GameState implements JSONSerializable{
    private final Board board;
    private final PlayingSide sideOnTurn;
    private final Army blueArmy;
    private final Army orangeArmy;
    private final GameResult result;

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy) {
        this(board, blueArmy, orangeArmy, PlayingSide.BLUE, GameResult.IN_PLAY);
    }

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy,
            PlayingSide sideOnTurn,
            GameResult result) {
        this.board = board;
        this.sideOnTurn = sideOnTurn;
        this.blueArmy = blueArmy;
        this.orangeArmy = orangeArmy;
        this.result = result;
    }

    public Board board() {
        return board;
    }

    public PlayingSide sideOnTurn() {
        return sideOnTurn;
    }

    public GameResult result() {
        return result;
    }

    public Army army(PlayingSide side) {
        if (side == PlayingSide.BLUE) {
            return blueArmy;
        }

        return orangeArmy;
    }

    public Army armyOnTurn() {
        return army(sideOnTurn);
    }

    public Army armyNotOnTurn() {
        if (sideOnTurn == PlayingSide.BLUE)
            return orangeArmy;

        return blueArmy;
    }

    public Tile tileAt(TilePos pos) {
        // Place for your code
        if (this.blueArmy.boardTroops().troopPositions().contains(pos)) {
            if (this.blueArmy.boardTroops().at(pos).isPresent()) return this.blueArmy.boardTroops().at(pos).get();
        }
        if(this.orangeArmy.boardTroops().troopPositions().contains(pos)){
            if (this.orangeArmy.boardTroops().at(pos).isPresent()) return this.orangeArmy.boardTroops().at(pos).get();
        }

        return this.board.at(pos);
    }

    private boolean canStepFrom(TilePos origin) {
        // Place for your code
        if(this.result != GameResult.IN_PLAY  || origin == TilePos.OFF_BOARD) return false;

        if(!this.armyOnTurn().boardTroops().isLeaderPlaced() || this.armyOnTurn().boardTroops().isPlacingGuards()) return false;
        if(!this.armyOnTurn().boardTroops().troopPositions().contains(origin)) return false;

        return true;
    }

    private boolean canStepTo(TilePos target) {
        if(this.result != GameResult.IN_PLAY || target == TilePos.OFF_BOARD) return false;

        Tile troop = this.tileAt(target);

        if(troop == BoardTile.MOUNTAIN) return false;
        return troop.canStepOn();
    }

    private boolean canCaptureOn(TilePos target) {
        if(this.result != GameResult.IN_PLAY || target == TilePos.OFF_BOARD) return false;

        if(!this.armyNotOnTurn().boardTroops().troopPositions().contains(target)) return false;

        return true;
    }

    public boolean canStep(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canStepTo(target);
    }

    public boolean canCapture(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canCaptureOn(target);
    }

    public boolean canPlaceFromStack(TilePos target) {
        if(this.result != GameResult.IN_PLAY || target == TilePos.OFF_BOARD || this.armyOnTurn().stack().isEmpty() ||
                !this.tileAt(target).canStepOn()) return false;

        if(!this.armyOnTurn().boardTroops().isLeaderPlaced()){
            if (sideOnTurn.equals(PlayingSide.BLUE)) return target.row() == 1;
            return target.row() == this.board.dimension();
        }
        if(this.armyOnTurn().boardTroops().isPlacingGuards())
            return this.armyOnTurn().boardTroops().leaderPosition().isNextTo(target);
        for(BoardPos pos : this.armyOnTurn().boardTroops().troopPositions()){
            if(pos.isNextTo(target)) return true;
        }
        return false;
    }

    public GameState stepOnly(BoardPos origin, BoardPos target) {
        if (canStep(origin, target))
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().troopStep(origin, target), GameResult.IN_PLAY);

        throw new IllegalArgumentException();
    }

    public GameState stepAndCapture(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopStep(origin, target).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState captureOnly(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopFlip(origin).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState placeFromStack(BoardPos target) {
        if (canPlaceFromStack(target)) {
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().placeFromStack(target),
                    GameResult.IN_PLAY);
        }

        throw new IllegalArgumentException();
    }

    public GameState resign() {
        return createNewGameState(
                armyNotOnTurn(),
                armyOnTurn(),
                GameResult.VICTORY);
    }

    public GameState draw() {
        return createNewGameState(
                armyOnTurn(),
                armyNotOnTurn(),
                GameResult.DRAW);
    }

    private GameState createNewGameState(Army armyOnTurn, Army armyNotOnTurn, GameResult result) {
        if (armyOnTurn.side() == PlayingSide.BLUE) {
            return new GameState(board, armyOnTurn, armyNotOnTurn, PlayingSide.BLUE, result);
        }

        return new GameState(board, armyNotOnTurn, armyOnTurn, PlayingSide.ORANGE, result);
    }

    public Army getBlueArmy(){
        return this.blueArmy;
    }

    public Army getOrangeArmy(){
        return this.orangeArmy;
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{");
        writer.printf("\"result\":");
        this.result.toJSON(writer);
        writer.printf(",\"board\":");
        this.board.toJSON(writer);
        writer.printf(",\"blueArmy\":");
        this.blueArmy.toJSON(writer);
        writer.printf(",\"orangeArmy\":");
        this.orangeArmy.toJSON(writer);
        writer.printf("}");

    }
}
