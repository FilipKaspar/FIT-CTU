import numpy as np
import scipy.io
import scipy.sparse as sp

TOLERANCE = 1e-7
MAX_ITERATIONS = 1000
MATRIX_PATH = "flickr.mtx"


# Defined convergence criteria
def crit_1(**kwargs):
    return np.linalg.norm(kwargs["x"] - kwargs["x_new"], 1).astype(np.float32) < TOLERANCE

def crit_2(**kwargs):
    return np.linalg.norm(kwargs["A"] @ kwargs["x_new"] - kwargs["lambda_new"] * kwargs["x_new"], 1).astype(np.float32) < TOLERANCE

CONVERGENCE_CRITERIA = {
    "crit_1": crit_1,
    "crit_2": crit_2,
}

# Function for loading the matrix
def load_sparse_matrix():
    try:
        matrix = scipy.io.mmread(MATRIX_PATH)
    except ValueError:
        raise ValueError("File is not in Matrix Market format!")

    if not sp.issparse(matrix):
        raise ValueError("Loaded matrix is not sparse.")

    return matrix

# Power method
def power_method(A, criteria):
    if criteria not in CONVERGENCE_CRITERIA:
        raise KeyError(f"Criteria {criteria} is not one of the allowed option!")

    m, n = A.shape
    if m != n:
        raise ValueError("Matrix must be square.")

    # Starting vector
    x = np.ones(m, dtype=np.float32) / m
    for k in range(MAX_ITERATIONS):
        Ax = (A @ x).astype(np.float32)
        # Dominant eigenvalue (Rayleigh quotient)
        lambda_new = np.sum(Ax).astype(np.float32) / np.sum(x).astype(np.float32)
        # Normalizing vector
        x_new = Ax / np.linalg.norm(Ax, 1).astype(np.float32)

        # Testing the criteria of convergences
        if CONVERGENCE_CRITERIA.get(criteria)(A=A, x=x, x_new=x_new, lambda_new=lambda_new):
                return x_new, lambda_new, k + 1

        # Update the starting vector
        x = x_new

    # In case convergence hasn't occurred, None is returned
    return None, None, MAX_ITERATIONS

if __name__ == "__main__":
    # Load matrix
    matrix = load_sparse_matrix()

    # Run Power Method
    result_vector, result_lambda, num_iterations = power_method(matrix, criteria="crit_1")

    # Results
    if result_vector is not None:
        result_vector = result_vector / np.linalg.norm(result_vector, 1)
        top_indices = np.argsort(result_vector)[-5:][::-1]
        top_values = result_vector[top_indices]

        print("Dominant eigenvalue (lambda1):", f"{result_lambda:.5f}")
        print("Number of iterations:", num_iterations)
        print("Top 5 components of the eigenvector:")
        for idx, val in zip(top_indices, top_values):
            print(f"Index: {idx}, Value: {val:.5f}")
    else:
        print("Convergence was not achieved within the maximum number of steps.")
