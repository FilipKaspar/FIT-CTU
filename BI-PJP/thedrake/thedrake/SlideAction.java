package thedrake;

import java.util.ArrayList;
import java.util.List;

public class SlideAction extends TroopAction{

    public SlideAction(Offset2D offset) {
        super(offset);
    }

    public SlideAction(int offsetX, int offsetY) {
        super(offsetX, offsetY);
    }

    @Override
    public List<Move> movesFrom(BoardPos origin, PlayingSide side, GameState state) {
        List<Move> result = new ArrayList<>();
        TilePos target = origin.stepByPlayingSide(offset(), side);

        while(target != TilePos.OFF_BOARD){
            if(state.canStep(origin, target)) {
                result.add(new StepOnly(origin, (BoardPos) target));
            } else if(state.canCapture(origin, target)) {
                result.add(new StepAndCapture(origin, (BoardPos) target));
                break;
            }
            // Mountain
            else {
                break;
            }
            target = target.stepByPlayingSide(offset(), side);
        }

        return result;
    }

}
