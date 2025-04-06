package thedrake.ui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import thedrake.Board;
import thedrake.BoardTile;
import thedrake.GameState;
import thedrake.PositionFactory;
import thedrake.StandardDrakeSetup;
import thedrake.ui.screens.MenuController;

public class TheDrakeApp extends Application {

    private Stage primaryStage;
    private BoardView boardView;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        this.primaryStage = primaryStage;

        this.boardView = new BoardView(createSampleGameState(), primaryStage);
        FXMLLoader loader = new FXMLLoader(getClass().getResource("screens/menu.fxml"));
        Parent root = loader.load();

        MenuController menuController = loader.getController();
        menuController.setMain(this);

        Scene menuScene = new Scene(root, 1400, 800);

        primaryStage.setMinWidth(700);
        primaryStage.setMinHeight(700);
        primaryStage.setMaxWidth(1400);
        primaryStage.setMaxHeight(800);

        primaryStage.setScene(menuScene);
        primaryStage.setTitle("The Drake");
        primaryStage.show();
    }

    public void switchToBoardView() {
        Scene boardScene = new Scene(boardView);
        primaryStage.setScene(boardScene);
        primaryStage.setTitle("The Drake Game");
    }

    public static GameState createSampleGameState() {
        Board board = new Board(4);
        PositionFactory positionFactory = board.positionFactory();
        board = board.withTiles(new Board.TileAt(positionFactory.pos(1, 1), BoardTile.MOUNTAIN));
        return new StandardDrakeSetup().startState(board);
    }

}
