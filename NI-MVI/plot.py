import csv
import re

import matplotlib.pyplot as plt


csv_files = [
    "trained_models/bert_lstm_nodrop_stats.txt",
    "trained_models/bert_transform_nodrop_stats.txt",
]

epochs = list(range(1, 21))
testing_accuracy = []
testing_loss = []

models = {
    "LSTM": [],
    "Transformer": [],
}

for i, model in enumerate(models):
    with open(csv_files[i], "r") as file:
        reader = csv.reader(file)
        for row in reader:
            line = "".join(row)

            accuracy_match = re.search(r"Testing_Accuracy: ([\d.]+)", line)
            loss_match = re.search(r"Testing_Loss: ([\d.]+)", line)

            if accuracy_match and loss_match:
                # models[model].append(float(accuracy_match.group(1)))
                models[model].append(float(loss_match.group(1)))

plt.figure(figsize=(10, 6))
plt.plot(epochs, models["LSTM"], label="LSTM Loss", color="blue")
plt.plot(epochs, models["Transformer"], label="Transformer Loss", color="orange")

plt.xlabel("Epoch")
plt.ylabel("Loss")
plt.title("Testing Loss With BERT Tokenizer")
plt.legend()
plt.grid()

max_epoch = max(epochs)
plt.xticks(range(0, max_epoch + 1, 1))
plt.xlim(0, max_epoch)

plt.show()
