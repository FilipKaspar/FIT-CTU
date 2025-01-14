# Milestone

## List of Articles Read
1. [Technical Skill Assessment using Machine Learning and Artificial Intelligence Algorithm](https://www.researchgate.net/publication/342113805_Technical_Skill_Assessment_using_Machine_Learning_and_Artificial_Intelligence_Algorithm)
2. [SKILL: A System for Skill Identification and Normalization](https://www.researchgate.net/publication/361506924_SKILL_A_System_for_Skill_Identification_and_Normalization)
3. [Creating a Text Classifier With LSTM! PyTorch Deep Learning Tutorial](https://www.youtube.com/watch?v=mxj5eUY8FlY)
4. [LSTMs Explained: A Complete, Technically Accurate, Conceptual Guide with Keras](https://medium.com/analytics-vidhya/lstms-explained-a-complete-technically-accurate-conceptual-guide-with-keras-2a650327e8f2)
5. [LSTM from scratch](https://medium.com/@wangdk93/lstm-from-scratch-c8b4baf06a8b)

## Description of Current Workflow
The current workflow can be split into 3 steps:

- **Data Preprocessing:** Tokenization of raw skills using torchtexts _basic_english_ tokenizer, and converting it into embeddings for input to the model. With that the vocabulary for the model is also built.
- **Model Selection:** So far I am using LSTM RNN model for the training with Adam optimizer. The custom LSTM model takes token embedding as an input and outputs vector of logits with size being equal to the amount of all categories.
- **Training:** Experimenting with different hyperparameters to improve classification accuracy and reduce overfitting. The model is also saved each epoch if it performs better than in the previous best epoch.

The definition of the tokenizer can be found in `handlers/TokenizerHandler.py`

Similarly, the definition of the model can be found in `models/lstm.py`

## Current Results
- **Model Accuracy:** The current model achieves approximately **88%** accuracy on the validation dataset. The validation dataset is currently set to be **20%** of the whole dataset.
- **Challenges:** Some raw skills are ambiguous, or they don't appear that many times in the dataset.

The results for each model with the corresponding model instance can be found in folder `trained_models/`

## Future improvements

1. Experiment with different tokenizers
2. Adjust hyperparameters
3. Experiment with GRU RNN
4. Experiment with transformers