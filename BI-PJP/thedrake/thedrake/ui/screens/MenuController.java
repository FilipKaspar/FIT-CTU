package thedrake.ui.screens;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.ImageView;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import thedrake.ui.TheDrakeApp;

public class MenuController {
    @FXML
    private StackPane stackPane;

    @FXML
    private ImageView imageView;

    @FXML
    private Button exitButton;

    @FXML
    private Button playButton;

    private TheDrakeApp main;

    public void setMain(TheDrakeApp main) {
        this.main = main;
    }

    @FXML
    public void initialize() {
        stackPane.widthProperty().addListener((obs, oldVal, newVal) -> resizeImage());
        stackPane.heightProperty().addListener((obs, oldVal, newVal) -> resizeImage());
        resizeImage();
    }

    @FXML
    public void resizeImage() {
        double width = stackPane.getWidth();
        double height = stackPane.getHeight();
        imageView.setFitWidth(width);
        imageView.setFitHeight(height);
    }

    public void handleExit(ActionEvent actionEvent) {
        ((Stage) exitButton.getScene().getWindow()).close();
    }

    public void startGame(){
        main.switchToBoardView();
    }
}
