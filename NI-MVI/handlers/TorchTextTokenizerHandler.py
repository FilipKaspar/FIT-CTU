import torchtext.transforms as T
from static_variables import SEQUENCES_MAX_LENGTH, VOCAB_MIN_FREQUENCY_TO_ADD
from torchtext.data.utils import get_tokenizer
from torchtext.vocab import build_vocab_from_iterator


class TokenizerHandler:
    def __init__(self) -> None:
        self.text_transform = None
        self.vocab = None

        self.tokenizer = get_tokenizer("basic_english")
        self.text_tokenizer = lambda batch: [self.tokenizer(x) for x in batch]

    def yield_tokens(self, data_iter):
        for text in data_iter:
            yield self.tokenizer(text)

    def build_vocab(self, data) -> None:
        self.vocab = build_vocab_from_iterator(
            self.yield_tokens(data),  # Tokenized data iterator
            min_freq=VOCAB_MIN_FREQUENCY_TO_ADD,  # Minimum frequency threshold for token inclusion
            specials=["<pad>", "<sos>", "<eos>", "<unk>"],  # Special case tokens
            special_first=True,  # Place special tokens first in the vocabulary
        )
        self.vocab.set_default_index(self.vocab["<unk>"])

        self.set_transform_function()

    def set_transform_function(self) -> None:
        self.text_transform = T.Sequential(
            # Convert the sentences to indices based on the given vocabulary
            T.VocabTransform(vocab=self.vocab),
            # Add <sos> at the beginning of each sentence. 1 is used because the index for <sos> in the vocabulary is 1.
            T.AddToken(1, begin=True),
            # Crop the sentence if it is longer than the max length
            T.Truncate(max_seq_len=SEQUENCES_MAX_LENGTH),
            # Add <eos> at the end of each sentence. 2 is used because the index for <eos> in the vocabulary is 2.
            T.AddToken(2, begin=False),
            # Convert the list of lists to a tensor.
            # This also pads a sentence with the <pad> token if it is shorter than the max length,
            # ensuring that all sentences are the same length.
            T.ToTensor(padding_value=0),
        )

    def get_vocab(self):
        return self.vocab
