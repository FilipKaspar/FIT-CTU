from pathlib import Path

import numpy as np
import torch
from handlers.DatasetHandlers import DatasetHandler
from handlers.TorchTextTokenizerHandler import TokenizerHandler
from models.lstm import LSTM
from sklearn.metrics import f1_score, precision_score, recall_score
from static_variables import BATCH_SIZE, HIDDEN_LAYER_SIZE, LSTM_NUM_HIDDEN_LAYERS, MODELS_ROOT_FOLDER
from tqdm import tqdm


MODEL_NAME = "bert_lstm_nodrop"


class Evaluator:
    def __init__(self):
        self.device = torch.device(0 if torch.cuda.is_available() else "cpu")

        self.dataset_handler = DatasetHandler()

        self.tokenize_handler = TokenizerHandler()  # only basic english
        self.tokenize_handler.build_vocab(data=self.dataset_handler.train_df["raw_skill"])  # only basic english
        self.dataset_handler.prepare_dataset_loaders()
        self.model = LSTM(
            num_emb=len(self.tokenize_handler.get_vocab()),
            output_size=len(self.dataset_handler.label_encoder.classes_),
            num_layers=LSTM_NUM_HIDDEN_LAYERS,
            hidden_size=HIDDEN_LAYER_SIZE,
        ).to(self.device)
        self.model.load_state_dict(torch.load(Path(MODELS_ROOT_FOLDER) / MODEL_NAME)["model_state_dict"])

    def evaluate(self):
        self.model.eval()
        all_predictions = []
        all_labels = []

        with torch.no_grad():
            for label, text in tqdm(self.dataset_handler.data_loader_test, desc="Testing", leave=True):
                label = label.to(self.device)
                text = self.tokenize_handler.text_transform(self.tokenize_handler.text_tokenizer(text)).to(self.device)

                hidden = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, BATCH_SIZE, HIDDEN_LAYER_SIZE, device=self.device)
                memory = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, BATCH_SIZE, HIDDEN_LAYER_SIZE, device=self.device)
                logits = self.model(text, hidden, memory)

                predictions = torch.argmax(logits[:, -1, :], dim=1)
                all_predictions.extend(predictions.cpu().numpy())
                all_labels.extend(label.cpu().numpy())

        all_predictions = np.array(all_predictions)
        all_labels = np.array(all_labels)

        # Calculate Scores
        print(f"Precision Score: {precision_score(all_labels, all_predictions, average='weighted', zero_division=0)}")
        print(f"Recall Score: {recall_score(all_labels, all_predictions, average='weighted', zero_division=0)}")
        print(f"F1 Score: {f1_score(all_labels, all_predictions, average='weighted')}")


Evaluator().evaluate()
