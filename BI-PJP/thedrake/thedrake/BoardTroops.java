package thedrake;

import java.io.PrintWriter;
import java.util.*;

public class BoardTroops implements JSONSerializable{
    private final PlayingSide playingSide;
    private final Map<BoardPos, TroopTile> troopMap;
    private final TilePos leaderPosition;
    private final int guards;

    public BoardTroops(PlayingSide playingSide) {
        this.playingSide = playingSide;
        this.troopMap = Collections.emptyMap();
        this.leaderPosition = BoardPos.OFF_BOARD;
        this.guards = 0;
    }

    public BoardTroops(
            PlayingSide playingSide,
            Map<BoardPos, TroopTile> troopMap,
            TilePos leaderPosition,
            int guards) {
        this.playingSide = playingSide;
        this.troopMap = new HashMap<>(troopMap);
        this.leaderPosition = leaderPosition;
        this.guards = guards;
    }

    public Optional<TroopTile> at(TilePos pos) {
        return Optional.ofNullable(this.troopMap.get(pos));
    }

    public PlayingSide playingSide() {
        return this.playingSide;
    }

    public TilePos leaderPosition() {
        return this.leaderPosition;
    }

    public int guards() {
        return this.guards;
    }

    public boolean isLeaderPlaced() {
        return this.leaderPosition != BoardPos.OFF_BOARD;
    }

    public boolean isPlacingGuards() {
        return this.isLeaderPlaced() && this.guards < 2;
    }

    public Set<BoardPos> troopPositions() {
        return this.troopMap.keySet();
    }

    public BoardTroops placeTroop(Troop troop, BoardPos target) {
        if (this.troopMap.containsKey(target)) {
            throw new IllegalArgumentException("Target position is not free.");
        }

        Map<BoardPos, TroopTile> newTroopMap = new HashMap<>(this.troopMap);
        newTroopMap.put(target, new TroopTile(troop, this.playingSide, TroopFace.AVERS));

        TilePos newLeaderPosition = this.leaderPosition;
        int newGuards = this.guards;

        if (!isLeaderPlaced()) {
            newLeaderPosition = target;
        } else if (isPlacingGuards()) {
            newGuards++;
        }

        return new BoardTroops(this.playingSide, newTroopMap, newLeaderPosition, newGuards);
    }

    public BoardTroops troopStep(BoardPos origin, BoardPos target) {
        if (!isLeaderPlaced() || isPlacingGuards()) {
            throw new IllegalStateException("Troop movement is not allowed in this phase.");
        }

        if (!this.troopMap.containsKey(origin)) {
            throw new IllegalArgumentException("Origin position doesn't contain any troop.");
        }

        if (this.troopMap.containsKey(target)) {
            throw new IllegalArgumentException("Target position is not free.");
        }

        TroopTile troopTile = this.troopMap.get(origin);
        Map<BoardPos, TroopTile> newTroopMap = new HashMap<>(this.troopMap);
        newTroopMap.remove(origin);
        newTroopMap.put(target, troopTile.flipped());

        TilePos newLeaderPosition = (origin.equals(this.leaderPosition)) ? target : this.leaderPosition;

        return new BoardTroops(this.playingSide, newTroopMap, newLeaderPosition, this.guards);
    }

    public BoardTroops troopFlip(BoardPos origin) {
        if (!isLeaderPlaced()) {
            throw new IllegalStateException(
                    "Cannot move troops before the leader is placed.");
        }

        if (isPlacingGuards()) {
            throw new IllegalStateException(
                    "Cannot move troops before guards are placed.");
        }

        if (!at(origin).isPresent())
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> newTroops = new HashMap<>(this.troopMap);
        TroopTile tile = newTroops.remove(origin);
        newTroops.put(origin, tile.flipped());

        return new BoardTroops(playingSide(), newTroops, this.leaderPosition, this.guards);
    }

    public BoardTroops removeTroop(BoardPos target) {
        if (!isLeaderPlaced() || isPlacingGuards()) {
            throw new IllegalStateException("Troop movement is not allowed in this phase.");
        }

        if (!this.troopMap.containsKey(target)) {
            throw new IllegalArgumentException("Target position doesn't contain any troop.");
        }

        Map<BoardPos, TroopTile> newTroopMap = new HashMap<>(this.troopMap);
        newTroopMap.remove(target);

        TilePos newLeaderPosition = (target.equals(this.leaderPosition)) ? BoardPos.OFF_BOARD : this.leaderPosition;

        return new BoardTroops(this.playingSide, newTroopMap, newLeaderPosition, this.guards);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{");
        writer.printf("\"side\":");
        this.playingSide.toJSON(writer);
        writer.printf(",\"leaderPosition\":\"%s\"", this.leaderPosition.toString());
        writer.printf(",\"guards\":%d", this.guards);

        writer.printf(",\"troopMap\":{");
        List<BoardPos> keyList = new ArrayList<>(troopMap.keySet());
        keyList.sort(new BoardPos.SortByPos());
        for (BoardPos pos : keyList) {
            TroopTile troop = troopMap.get(pos);
            writer.printf("\"%s\":", pos.toString());
            troop.toJSON(writer);
            if (!pos.equals(keyList.get(keyList.size() - 1))) {
                writer.printf(",");
            }
        }
        writer.printf("}");
        writer.printf("}");
    }
}
