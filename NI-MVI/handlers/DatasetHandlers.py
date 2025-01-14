import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from static_variables import BATCH_SIZE, DATA_TEST_SIZE, DATASET_PATH, NUM_CPU_THREADS, TRAIN_TEST_SPLIT_RANDOM_STATE
from torch.utils.data import DataLoader, Dataset


class DatasetHandler:
    def __init__(self) -> None:
        self.data_loader_test = None
        self.data_loader_train = None

        self.label_encoder = LabelEncoder()
        self.df = pd.read_csv(DATASET_PATH, sep=",")

        self.train_df, self.test_df = train_test_split(
            self.df, test_size=DATA_TEST_SIZE, random_state=TRAIN_TEST_SPLIT_RANDOM_STATE
        )

    def prepare_dataset_loaders(self) -> None:
        self.label_encoder.fit_transform(self.df["normalized_skill"])

        self.train_df["normalized_skill"] = self.label_encoder.transform(self.train_df["normalized_skill"])
        self.test_df["normalized_skill"] = self.label_encoder.transform(self.test_df["normalized_skill"])

        self.data_loader_train = DataLoader(
            SkillDataset(self.train_df),
            batch_size=BATCH_SIZE,
            shuffle=True,
            num_workers=NUM_CPU_THREADS,
            drop_last=True,
        )
        self.data_loader_test = DataLoader(
            SkillDataset(self.test_df), batch_size=BATCH_SIZE, shuffle=True, num_workers=NUM_CPU_THREADS, drop_last=True
        )


class SkillDataset(Dataset):
    def __init__(self, dataframe):
        self.raw = dataframe["raw_skill"].tolist()  # Text data
        self.normalized = dataframe["normalized_skill"].tolist()  # Label data

    def __len__(self):
        return len(self.raw)

    def __getitem__(self, idx):
        return self.normalized[idx], self.raw[idx]  # Return label and text
