package thedrake.ui;

import java.util.List;

import javafx.fxml.FXMLLoader;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import thedrake.*;

public class BoardView extends GridPane implements TileViewContext {
    private Stage primaryStage;

    private GameState gameState;

    private ValidMoves validMoves;

    private TileView selected;
    private StackView selectedStackViewBlue;
    private StackView selectedStackViewOrange;

    private VBox header;
    private Label headerLabel;

    private Label orangeCaptured;
    private Label blueCaptured;

    public BoardView(GameState gameState, Stage primaryStage) {
        this.primaryStage = primaryStage;
        this.gameState = gameState;
        this.validMoves = new ValidMoves(gameState);
        this.header = new VBox();
        this.headerLabel = new Label("");
        this.blueCaptured = new Label("");
        this.orangeCaptured = new Label("");

        PositionFactory positionFactory = gameState.board().positionFactory();
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                BoardPos boardPos = positionFactory.pos(x, 3 - y);
                add(new TileView(boardPos, gameState.tileAt(boardPos), this), x+1, y+1);
            }
        }

        setHgap(5);
        setVgap(5);
        setPadding(new Insets(15));
        setAlignment(Pos.CENTER);

        this.setCaptured();

        this.setHeader();
        header.getChildren().add(headerLabel);
        add(header, 0, 0);

        this.selectedStackViewOrange = new StackView(this, PlayingSide.ORANGE);

        VBox stackFrameOrange = new VBox();
        stackFrameOrange.getChildren().add(new Label("Orange Stack:"));
        stackFrameOrange.getChildren().add(this.selectedStackViewOrange);
        stackFrameOrange.getChildren().add(this.blueCaptured);
        add(stackFrameOrange, 5, 1, 1, 4);
        this.selectedStackViewBlue = new StackView(this, PlayingSide.BLUE);

        VBox stackFrameBlue = new VBox();
        stackFrameBlue.getChildren().add(new Label("Blue Stack:"));
        stackFrameBlue.getChildren().add(this.selectedStackViewBlue);
        stackFrameBlue.getChildren().add(this.orangeCaptured);
        add(stackFrameBlue, 0, 1, 1, 4);
    }

    public void setHeader(){
        headerLabel.setText("On turn: " + gameState.sideOnTurn());
    }

    public void setCaptured(){
        StringBuilder troopsb = new StringBuilder("Yoinked (" + gameState.getBlueArmy().captured().size() + "):");
        for(Troop troop : gameState.getBlueArmy().captured()){
            troopsb.append("\n").append(troop.name());
        }

        StringBuilder troopso = new StringBuilder("Yoinked (" + gameState.getOrangeArmy().captured().size() + "):");
        for(Troop troop : gameState.getOrangeArmy().captured()){
            troopso.append("\n").append(troop.name());
        }

        this.orangeCaptured.setText(String.valueOf(troopsb));
        this.blueCaptured.setText(String.valueOf(troopso));
    }

    @Override
    public void tileViewSelected(TileView tileView) {
        if (selected != null && selected != tileView)
            selected.unselect();
        this.unselectOther();

        selected = tileView;

        clearMoves();
        showMoves(validMoves.boardMoves(tileView.position()));
    }

    @Override
    public void stackViewSelected(StackView stackView) {
        if (selected != null) {
            selected.unselect();
        }

        if(gameState.armyOnTurn().side() == PlayingSide.BLUE){
            this.selectedStackViewBlue = stackView;
        } else {
            this.selectedStackViewOrange = stackView;
        }

        clearMoves();

        showMoves(validMoves.movesFromStack());
    }

    public void unselectOther(){
        if(selectedStackViewBlue != null){
            selectedStackViewBlue.unselect();
        }
        if(selectedStackViewOrange != null ){
            selectedStackViewOrange.unselect();
        }
    }

    @Override
    public void executeMove(Move move) {
        if (selected != null){
            selected.unselect();
        }
        this.unselectOther();
        selected = null;
        clearMoves();
        gameState = move.execute(gameState);
        validMoves = new ValidMoves(gameState);
        updateTiles();
        this.setHeader();
        this.setCaptured();
        if(gameState.result() != GameResult.IN_PLAY){
            this.handleEndGameScreen();
        }
    }

    public void handleEndGameScreen(){
        VBox result = new VBox();
        Label resultLabel = new Label("");
        result.getChildren().add(resultLabel);
        add(result, 2, 5);
        header.getChildren().clear();

        if(gameState.result() == GameResult.DRAW){
            resultLabel.setText("DRAW!");
        } else {
            resultLabel.setText(gameState.armyNotOnTurn().side() + " WON!");
        }

        Button nextGame = new Button("Next Game");
        Button mainMenu = new Button("Main Menu");

        add(nextGame, 0,5);
        add(mainMenu, 5,5);

        nextGame.setOnAction(e -> {
            BoardView boardView = new BoardView(TheDrakeApp.createSampleGameState(), primaryStage);
            primaryStage.setScene(new Scene(boardView));
            primaryStage.setTitle("The Drake Game");
        });

        mainMenu.setOnAction(e -> {
            try {
                new TheDrakeApp().start(this.primaryStage);
            } catch (Exception ex) {
                throw new RuntimeException(ex);
            }
        });
    }

    private void updateTiles() {
        boolean possible_move = false;
        for (Node node : getChildren()) {
            if (node instanceof TileView) {
                TileView tileView = (TileView) node;
                tileView.setTile(gameState.tileAt(tileView.position()));
                tileView.update();
                selectedStackViewBlue.update();
                selectedStackViewOrange.update();
                if(!possible_move && !validMoves.boardMoves(tileView.position()).isEmpty()){
                    possible_move = true;
                }
            }
        }
        if(!possible_move && validMoves.movesFromStack().isEmpty()){
            this.handleEndGameScreen();
        }
    }

    private void clearMoves() {
        for (Node node : getChildren()) {
            if (node instanceof TileView) {
                TileView tileView = (TileView) node;
                tileView.clearMove();
            }
        }
    }

    private void showMoves(List<Move> moveList) {
        for (Move move : moveList)
            tileViewAt(move.target()).setMove(move);
    }

    private TileView tileViewAt(BoardPos target) {
        int index = (3 - target.j()) * 4 + target.i();
        return (TileView) getChildren().get(index);
    }

    public GameState getGameState(){
        return this.gameState;
    }
}
