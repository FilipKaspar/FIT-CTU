module thedrake {
    requires javafx.fxml;
    requires javafx.controls;
    requires junit;
    requires java.desktop;

    opens thedrake.ui;
    opens thedrake.ui.screens;
}