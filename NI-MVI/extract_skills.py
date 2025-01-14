import re
from collections import Counter
from pathlib import Path

import pandas as pd
import torch
import torch.nn.functional as F
from handlers.DatasetHandlers import DatasetHandler
from handlers.TorchTextTokenizerHandler import TokenizerHandler
from models.lstm import LSTM
from sentence_transformers import SentenceTransformer
from static_variables import HIDDEN_LAYER_SIZE, LSTM_NUM_HIDDEN_LAYERS, MODELS_ROOT_FOLDER


MODEL_NAME = "bert_lstm_nodrop"
ACCEPTED_SKILL_THRESHOLD = 0.7
OUTPUT_FILENAME = "job_postings_skills.txt"
JOB_POSTINGS_FILENAME = "job_postings.csv"


class SkillExtractor:
    def __init__(self):
        self.job_posting = ""
        self.embedding_model = SentenceTransformer("all-MiniLM-L6-v2")
        self.device = torch.device(0 if torch.cuda.is_available() else "cpu")

        self.dataset_handler = DatasetHandler()
        self.dataset_handler.prepare_dataset_loaders()

        # self.tokenize_handler = BertTokenizer.from_pretrained("bert-base-uncased")
        self.tokenize_handler = TokenizerHandler()  # only basic english
        self.tokenize_handler.build_vocab(data=self.dataset_handler.train_df["raw_skill"])

        self.model = LSTM(
            num_emb=len(self.tokenize_handler.get_vocab()),
            output_size=len(self.dataset_handler.label_encoder.classes_),
            num_layers=LSTM_NUM_HIDDEN_LAYERS,
            hidden_size=HIDDEN_LAYER_SIZE,
        ).to(self.device)

        self.model.load_state_dict(torch.load(Path(MODELS_ROOT_FOLDER) / MODEL_NAME)["model_state_dict"])

    def prepare_job_posting(self, job_posting):
        job_posting = re.sub(r"<[^>]+>", "", job_posting).replace("\n", "")
        job_posting = re.sub(r"\s{2,}", " ", job_posting)
        self.job_posting = re.split(r"[;\-.?!]", job_posting)

    def extract_skills(self, job_posting):
        self.model.eval()
        self.prepare_job_posting(job_posting)

        # print("\n\nProcessing job posting...")

        skills = []
        with torch.no_grad():
            for sentence in self.job_posting:
                sentence = (sentence,)
                text = self.tokenize_handler.text_transform(self.tokenize_handler.text_tokenizer(sentence)).to(
                    self.device
                )

                hidden = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, 1, HIDDEN_LAYER_SIZE, device=self.device)
                memory = torch.zeros(LSTM_NUM_HIDDEN_LAYERS, 1, HIDDEN_LAYER_SIZE, device=self.device)

                pred = self.model(text, hidden, memory)
                predicted_index = pred[:, -1, :].argmax(1).item()

                predicted_prob = F.softmax(pred[:, -1, :], dim=1)[0, predicted_index].item()
                predicted_label = self.dataset_handler.label_encoder.inverse_transform([predicted_index])[0]

                # print(f"Sentence: {sentence}")
                # print(f"Predicted probability: {predicted_prob}")
                # print(f"Skill: {predicted_label}\n")

                with open(OUTPUT_FILENAME, "a+") as file:
                    file.write(f"Sentence: {sentence}\n")
                    file.write(f"Predicted probability: {predicted_prob}\n")
                    file.write(f"Skill: {predicted_label}\n\n")

                if predicted_prob >= ACCEPTED_SKILL_THRESHOLD:
                    skills.append(predicted_label)

        counter = Counter(skills)

        sorted_skills = [item for item, _ in counter.most_common()]

        # print(f"Predicted skills: {sorted_skills}")
        with open(OUTPUT_FILENAME, "a+") as file:
            file.write(f"Predicted skills: {sorted_skills}\n\n")


if __name__ == "__main__":
    df = pd.read_csv(JOB_POSTINGS_FILENAME, sep=",")
    job_descriptions = df["Job Description"].head(16).tolist()

    extractor = SkillExtractor()

    for i, job_post in enumerate(job_descriptions):
        with open(OUTPUT_FILENAME, "a+") as file:
            file.write(f"{i+1} record in {JOB_POSTINGS_FILENAME}\n")
            file.write("-------------------------------------------------------------------------\n")
        extractor.extract_skills(job_post)
