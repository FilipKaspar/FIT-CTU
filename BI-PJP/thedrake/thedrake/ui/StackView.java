package thedrake.ui;

import javafx.scene.control.Label;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import thedrake.*;

public class StackView extends Pane {
    private GameState gameState;
    private PlayingSide side;
    private Tile tile;
    private TileBackgrounds backgrounds = new TileBackgrounds();
    private Border selectBorder = new Border(
            new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(3)));
    private TileViewContext tileViewContext;

    public StackView(TileViewContext tileViewContext, PlayingSide side) {
        this.tileViewContext = tileViewContext;
        this.side = side;

        setPrefSize(100, 100);

        setOnMouseClicked(e -> onClick());
        update();
    }

    private void onClick() {
        if (this.tileViewContext.getGameState().armyOnTurn().side() != this.side ||
            this.tileViewContext.getGameState().result() != GameResult.IN_PLAY){
            return;
        }
        select();
    }

    public void select() {
        setBorder(selectBorder);
        tileViewContext.stackViewSelected(this);
    }

    public void unselect() {
        setBorder(null);
    }

    public void update() {
        gameState = this.tileViewContext.getGameState();
        if(this.side == PlayingSide.BLUE){
            this.tile = !gameState.getBlueArmy().stack().isEmpty() ? new TroopTile(gameState.getBlueArmy().stack().getFirst(), this.side, TroopFace.AVERS) : null;
        } else {
            this.tile = !gameState.getOrangeArmy().stack().isEmpty() ? new TroopTile(gameState.getOrangeArmy().stack().getFirst(), this.side, TroopFace.AVERS) : null;
        }

        this.setBackground(backgrounds.get(this.tile));
    }
}
