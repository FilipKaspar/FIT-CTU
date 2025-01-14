import argparse
from pathlib import Path

import torch
import torch.nn as nn
from handlers.DatasetHandlers import DatasetHandler
from handlers.TorchTextTokenizerHandler import TokenizerHandler
from models.lstm import LSTM
from static_variables import (
    BATCH_SIZE,
    HIDDEN_LAYER_SIZE,
    LEARNING_RATE,
    LSTM_NUM_HIDDEN_LAYERS,
    MODEL_DROPOUT_RATE,
    MODEL_STATS_END_PATH,
    MODELS_ROOT_FOLDER,
    NUM_EPOCHS,
    SEQUENCES_MAX_LENGTH,
    TRANSFORMER_NUM_HEADS,
    VOCAB_MIN_FREQUENCY_TO_ADD,
)
from tqdm import tqdm, trange


INDEX = -1  # 0 for transformer, -1 for LSTM


class TrainModels:
    model_name = None
    model_exist = True

    def __init__(self) -> None:
        self.device = torch.device(0 if torch.cuda.is_available() else "cpu")

        self.dataset_handler = DatasetHandler()

        self.tokenize_handler = TokenizerHandler()  # only basic english
        self.tokenize_handler.build_vocab(data=self.dataset_handler.train_df["raw_skill"])  # only basic english
        # self.tokenize_handler = BertTokenizer.from_pretrained('bert-base-uncased')

        self.dataset_handler.prepare_dataset_loaders()
        # self.model = NanoTransformer(
        #     num_emb=len(self.tokenize_handler.get_vocab()),
        #     output_size=len(self.dataset_handler.label_encoder.classes_),
        #     hidden_size=HIDDEN_LAYER_SIZE,
        #     num_heads=TRANSFORMER_NUM_HEADS
        # ).to(self.device)
        self.model = LSTM(
            num_emb=len(self.tokenize_handler.get_vocab()),
            output_size=len(self.dataset_handler.label_encoder.classes_),
            num_layers=LSTM_NUM_HIDDEN_LAYERS,
            hidden_size=HIDDEN_LAYER_SIZE,
        ).to(self.device)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=LEARNING_RATE)
        # Cosine annealing scheduler to decay the learning rate
        self.lr_scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(self.optimizer, T_max=NUM_EPOCHS, eta_min=0)
        self.loss_fn = nn.CrossEntropyLoss()

        self.cur_epoch = 0
        self.best_val_loss = float("inf")

        self.total_loss_test = 0
        self.total_loss_train = 0
        self.test_acc = 0
        self.train_acc = 0

        self.saved = ""

    def checked_parsed_options(self) -> None:
        if self.model_name:
            model_path = Path(MODELS_ROOT_FOLDER) / self.model_name
            if not model_path.exists():
                self.model_exist = False
                with open(Path(MODELS_ROOT_FOLDER) / (self.model_name + "_" + MODEL_STATS_END_PATH), "a") as file:
                    file.write(
                        f"SEQUENCES_MAX_LENGTH: {SEQUENCES_MAX_LENGTH}\n"
                        f"BATCH_SIZE: {BATCH_SIZE}\n"
                        f"NUM_EPOCHS: {NUM_EPOCHS}\n"
                        f"LEARNING_RATE: {LEARNING_RATE}\n"
                        f"HIDDEN_LAYER_SIZE: {HIDDEN_LAYER_SIZE}\n"
                        f"LSTM_NUM_HIDDEN_LAYERS: {LSTM_NUM_HIDDEN_LAYERS}\n"
                        f"TRANSFORMER_NUM_HEADS: {TRANSFORMER_NUM_HEADS}\n"
                        f"MODEL_DROPOUT_RATE: {MODEL_DROPOUT_RATE}\n"
                        f"VOCAB_MIN_FREQUENCY_TO_ADD: {VOCAB_MIN_FREQUENCY_TO_ADD}\n"
                    )
                # print("Model not found; new one is going to be created!")

    def parse_options(self) -> None:
        parser = argparse.ArgumentParser(description="Load a model file.")
        parser.add_argument(
            "model_file", type=str, help="Path to the model file (e.g., model.pth) or name of a new one"
        )

        args = parser.parse_args()
        self.model_name = args.model_file

        self.checked_parsed_options()

    def load_model(self) -> None:
        if self.model_exist:
            checkpoint = torch.load(Path(MODELS_ROOT_FOLDER) / self.model_name)

            self.cur_epoch = checkpoint["epoch"]
            self.best_val_loss = checkpoint["best_val_loss"]
            self.model.load_state_dict(checkpoint["model_state_dict"])
            self.optimizer.load_state_dict(checkpoint["optimizer_state_dict"])
            self.lr_scheduler.load_state_dict(checkpoint["scheduler_state_dict"])

    def train(self) -> None:
        self.load_model()

        pbar = trange(self.cur_epoch, NUM_EPOCHS, leave=True, desc="Epoch")
        for _ in pbar:
            # Update progress bar description with current accuracy
            self.model.train()
            self.total_loss_train = 0
            self.train_acc = 0
            for label, text in tqdm(self.dataset_handler.data_loader_train, desc="Training", leave=False):
                label = label.to(self.device)

                pred = self.forward_pass(text)
                loss = self.loss_fn(pred[:, INDEX, :], label)

                # Backpropagation and optimization
                self.optimizer.zero_grad()
                loss.backward()
                self.optimizer.step()

                self.total_loss_train += loss.item()
                self.train_acc += (pred[:, INDEX, :].argmax(1) == label).sum()

            self.train_acc = self.train_acc / (len(self.dataset_handler.data_loader_train) * BATCH_SIZE) * 100

            # Update learning rate
            self.lr_scheduler.step()

            self.model.eval()
            self.total_loss_test = 0
            self.test_acc = 0
            with torch.no_grad():
                for label, text in tqdm(self.dataset_handler.data_loader_test, desc="Testing", leave=False):
                    label = label.to(self.device)

                    pred = self.forward_pass(text)

                    self.total_loss_test += self.loss_fn(pred[:, INDEX, :], label)
                    self.test_acc += (pred[:, INDEX, :].argmax(1) == label).sum()

                # Calculate and append test accuracy for the epoch
                self.test_acc = self.test_acc / (len(self.dataset_handler.data_loader_test) * BATCH_SIZE) * 100

            self.cur_epoch += 1
            pbar.set_postfix_str("Accuracy: Train %.2f%%, Test %.2f%%" % (self.train_acc, self.test_acc))

            self.save_model()
            self.save_stats_to_file()

    def forward_pass(self, text):
        # Tokenize and transform text to tensor, move to device
        text_tokens = self.tokenize_handler.text_transform(self.tokenize_handler.text_tokenizer(text)).to(
            self.device
        )  # FOR BASIC ENGLISH
        # text_tokens = self.tokenize_handler(
        #         text,
        #         max_length=SEQUENCES_MAX_LENGTH,
        #         padding='max_length',
        #         truncation=True,
        #         return_tensors='pt',
        #     )['input_ids'].to(self.device) # FOR BERT

        # Initialize hidden and memory(cell) states
        hidden = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, BATCH_SIZE, HIDDEN_LAYER_SIZE, device=self.device)
        memory = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, BATCH_SIZE, HIDDEN_LAYER_SIZE, device=self.device)

        # Forward pass through the model
        pred = self.model(text_tokens, hidden, memory)  # FOR LSTM USE
        # pred = self.model(text_tokens) # FOR TRANSFORMER USE

        return pred

    def save_model(self):
        if self.total_loss_test < self.best_val_loss:
            # self.save_model_stats(total_loss, epoch)
            self.saved = " -- Saved"
            self.best_val_loss = self.total_loss_test
            torch.save(
                {
                    "epoch": self.cur_epoch,
                    "best_val_loss": self.best_val_loss,
                    "model_state_dict": self.model.state_dict(),
                    "optimizer_state_dict": self.optimizer.state_dict(),
                    "scheduler_state_dict": self.lr_scheduler.state_dict(),
                },
                Path(MODELS_ROOT_FOLDER) / self.model_name,
            )

    def save_stats_to_file(self) -> None:
        with open(Path(MODELS_ROOT_FOLDER) / (self.model_name + "_" + MODEL_STATS_END_PATH), "a") as file:
            file.write(
                f"Epoch {self.cur_epoch} -- Training_Accuracy: {self.train_acc:.2f} -- "
                f"Testing_Accuracy: {self.test_acc:.2f}"
                f" -- Training_Loss: {(self.total_loss_train / len(self.dataset_handler.data_loader_train)):.2f}"
                f" -- Testing_Loss: {(self.total_loss_test / len(self.dataset_handler.data_loader_test)):.2f}"
                f"{self.saved}\n"
            )


if __name__ == "__main__":
    train_models = TrainModels()

    train_models.parse_options()
    train_models.train()
