package thedrake.ui;

import thedrake.Move;
import thedrake.GameState;

public interface TileViewContext {

    void tileViewSelected(TileView tileView);

    void stackViewSelected(StackView stackView);

    void executeMove(Move move);

    GameState getGameState();

}
