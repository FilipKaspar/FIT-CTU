import math

import torch
import torch.nn as nn
from static_variables import MODEL_DROPOUT_RATE


class SinusoidalPosEmb(nn.Module):
    def __init__(self, dim):
        super().__init__()
        self.dim = dim

    def forward(self, x):
        device = x.device
        half_dim = self.dim // 2
        emb = math.log(10000) / (half_dim - 1)
        emb = torch.exp(torch.arange(half_dim, device=device) * -emb)
        emb = x[:, None] * emb[None, :]
        emb = torch.cat((emb.sin(), emb.cos()), dim=-1)
        return emb


# "Encoder-Only" Style Transformer
class NanoTransformer(nn.Module):
    """
    This class implements a simplified Transformer model for sequence classification.
    It uses an embedding layer for tokens, sinusoidal positional embeddings,
    a single Transformer block, and a final linear layer for prediction.

    Args:
      num_emb: The number of unique tokens in the vocabulary.
      output_size: The size of the output layer (number of classes).
      hidden_size: The dimension of the hidden layer in the Transformer block (default: 128).
      num_heads: The number of heads in the multi-head attention layer (default: 4).
    """

    def __init__(self, num_emb, output_size, hidden_size=256, num_heads=4):
        super(NanoTransformer, self).__init__()

        # Create an embedding for each token
        self.embedding = nn.Embedding(num_emb, hidden_size)
        self.embedding.weight.data = 0.001 * self.embedding.weight.data

        self.pos_emb = SinusoidalPosEmb(hidden_size)

        self.multihead_attn = nn.MultiheadAttention(
            hidden_size, num_heads=num_heads, batch_first=True, dropout=MODEL_DROPOUT_RATE
        )

        self.mlp = nn.Sequential(
            nn.Linear(hidden_size, hidden_size),
            nn.LayerNorm(hidden_size),
            nn.ELU(),
            nn.Linear(hidden_size, hidden_size),
        )

        self.fc_out = nn.Linear(hidden_size, output_size)

    def forward(self, input_seq):
        bs, length = input_seq.shape
        input_embs = self.embedding(input_seq)

        # Add a unique embedding to each token embedding depending on it's position in the sequence
        seq_indx = torch.arange(length, device=input_seq.device)
        pos_emb = self.pos_emb(seq_indx).reshape(1, length, -1).expand(bs, length, -1)
        embs = input_embs + pos_emb
        output, attn_map = self.multihead_attn(embs, embs, embs)

        output = self.mlp(output)

        return self.fc_out(output)  # , attn_map
