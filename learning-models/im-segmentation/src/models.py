"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np

def initialize_variational_parameters(num_rows_of_image, num_cols_of_image, K):
    """ Helper function to initialize variational distributions before each E-step.
    Args:
                num_rows_of_image: Integer representing the number of rows in the image
                num_cols_of_image: Integer representing the number of columns in the image
                K: The number of latent states in the MRF
    Returns:
                q: 3-dimensional numpy matrix with shape [num_rows_of_image, num_cols_of_image, K]
     """
    q = np.random.random((num_rows_of_image, num_cols_of_image, K))
    for row_num in range(num_rows_of_image):
        for col_num in range(num_cols_of_image):
            q[row_num, col_num, :] = q[row_num, col_num, :]/sum(q[row_num ,col_num, :])
    return q

def initialize_theta_parameters(K):
    """ Helper function to initialize theta before begining of EM.
    Args:
                K: The number of latent states in the MRF
    Returns:
                mu: A numpy vector of dimension [K] representing the mean for each of the K classes
                sigma: A numpy vector of dimension [K] representing the standard deviation for each of the K classes
    """
    mu = np.zeros(K)
    sigma = np.zeros(K) + 10
    for k in range(K):
        mu[k] = np.random.randint(10,240)
    return mu, sigma


class MRF(object):
    def __init__(self, J, K, n_em_iter, n_vi_iter):
        self.J = J
        self.K = K
        self.n_em_iter = n_em_iter
        self.n_vi_iter = n_vi_iter


    """
    Calculates the pdf of each pixel for each state given the x values,
    the mu, and sigmas for each state.

    Returns a (r, c, K) matrix
    """
    def n_dist(self, x, mu, sig):

        res = np.zeros((x.shape[0], x.shape[1], self.K))

        # calculate normal pdf values array-wise per state
        for k in range(self.K):
            var = sig[k] ** 2
            z = np.square(x - mu[k]) / var
            z *= -0.5
            res[:, :, k] = np.exp(z) / np.sqrt(2 * np.pi * var)

        return res


    def fit(self, *, X):
        """ Fit the model.
                Args:
                X: A matrix of floats with shape [num_rows_of_image, num_cols_of_image]
        """

        mu, sig = initialize_theta_parameters(self.K)

        r_bound, c_bound = X.shape[0] - 1, X.shape[1] - 1

        # perform N iterations of EM
        for n in range(self.n_em_iter):

            # E step = calculate probabilities of a pixel being in each state

            # initialize parameters
            q = initialize_variational_parameters(X.shape[0], X.shape[1], self.K)
            norms = self.n_dist(X, mu, sig)

            # Perform M iterations of variational inference
            for v in range(self.n_vi_iter):

                # iterate through pixels
                for r in range(X.shape[0]):
                    for c in range(X.shape[1]):

                        # get normal distribution values for this pixel
                        ns = norms[r, c, :]
                        exp_sum = np.zeros(self.K)

                        # sum q values across valid neighbors
                        if r > 0:
                            exp_sum += q[r - 1, c, :]
                        if r < r_bound:
                            exp_sum += q[r + 1, c, :]
                        if c > 0:
                            exp_sum += q[r, c - 1, :]
                        if c < c_bound:
                            exp_sum += q[r, c + 1, :]

                        # calculate q values and softmax
                        q[r, c, :] = np.multiply(ns, np.exp(self.J * exp_sum))
                        q[r, c, :] /= np.sum(q[r, c, :])

            # Maximization step, calculate new mu/sigma
            for k in range(self.K):
                mu[k] = np.sum(np.multiply(q[:, :, k], X)) / np.sum(q[:, :, k])
                s = np.square(X - mu[k])
                sig[k] = np.sqrt(np.sum(np.multiply(q[:, :, k], s)) / np.sum(q[:, :, k]))

        # store for predictions
        self.mu = mu
        self.sig = sig



    def predict(self, X):
        """ Predict.
        Args:
                X: A matrix of floats with shape [num_rows_of_image, num_cols_of_image]

        Returns:
                A matrix of ints with shape [num_rows_of_image, num_cols_of_image].
                    - Each element of this matrix should be the most likely state according to the trained model for the pixel corresponding to that row and column
                    - States should be encoded as {0,..,K-1}
        """

        # initial parameters based on initialization or last round of EM
        mu, sig = self.mu, self.sig
        r_bound, c_bound = X.shape[0] - 1, X.shape[1] - 1
        q = initialize_variational_parameters(X.shape[0], X.shape[1], self.K)
        norms = self.n_dist(X, mu, sig)

        # final round of variational inference using final parameters
        for v in range(self.n_vi_iter):

            # iterate through pixels
            for r in range(X.shape[0]):
                for c in range(X.shape[1]):

                    # get normal distribution values for this pixel
                    ns = norms[r, c, :]
                    exp_sum = np.zeros(self.K)

                    # sum across valid neighbors
                    if r > 0:
                        exp_sum += q[r - 1, c, :]
                    if r < r_bound:
                        exp_sum += q[r + 1, c, :]
                    if c > 0:
                        exp_sum += q[r, c - 1, :]
                    if c < c_bound:
                        exp_sum += q[r, c + 1, :]

                    # get final q value and softmax across states
                    q[r, c, :] = np.multiply(ns, np.exp(self.J * exp_sum))
                    q[r, c, :] /= np.sum(q[r, c, :])

        # final label is argmax across states
        return np.argmax(q, axis=2)
