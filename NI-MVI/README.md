# My semestral work for class NI-MVI 2024
(Also my first AI related project hehe)


## Brief Summary of the Project
The goal of this project is to train a learning model capable of classifying raw skill data into predefined normalized skill categories. The dataset includes over 1.5 million entries of raw skills paired with their normalized equivalents. The objective is to improve the accuracy of extracting skills from job postings, in order to make it easier to summarize requirements from those job postings.

[More info can be found in the milestone](milestone.md)

## Installation

UV manager is needed for this project. If not already installed, follow these [Instructions](https://github.com/astral-sh/uv)

```shell
uv sync
```

- Don't forget to source the environment

## Usage

```shell
python train.py MODEL_NAME
```

**MODEL_NAME**: Either a name of a model in `trained_models` folder that we want to finetune or name of a new model, that we want to train from the beginning.

### Change model

2 models are currently implemented:
1. **LSTM**
2. **Encoder-only Transformer**

In order to change the model, few things has to be changed in the code:
1. `INDEX` static variable in `train.py` file
2. Variable `self.model` in train class
3. `Prediction` variables in forward_pass function

Instructions to all those variables are described in the `train.py` file

### Change the tokenizer

2 tokenizers are currently implemented:
1. **Basic English** using **torchtext** library
2. **Bert-base-uncased** using hugging face **transformers** library 

In order to change the tokenizer, few things has to be changed in the code:
1. Variable `self.tokenize_handler` in `__init__` needs to be changed in `train.py` file
2. Variable `text_tokens` in `forward_pass` function in `train.py` file needs to be changes

Instructions to all those variables are described in the `train.py` file as well

## Data description

- Example of the dataset can be seen in `skill.csv` file
- It consists only of 2 rows, where the first one is the `raw_skill` and the second column is the desired `normalized_skill`

- The full skills dataset has been provided by class tutor
