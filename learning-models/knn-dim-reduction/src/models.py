"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np
import numpy.matlib
from collections import defaultdict
from scipy.stats import mode
from scipy.linalg import eig
from scipy.sparse.linalg import eigsh


"""
Defines a generic model to be overridden
"""
class Model(object):

    def __init__(self):
        self.num_input_features = None

    def fit(self, X, y):
        """ Fit the model.

        Args:
            X: A compressed sparse row matrix of floats with shape
                [num_examples, num_features].
            y: A dense array of ints with shape [num_examples].
        """
        raise NotImplementedError()

    def predict(self, X):
        """ Predict.

        Args:
            X: A compressed sparse row matrix of floats with shape
                [num_examples, num_features].

        Returns:
            A dense array of ints with shape [num_examples].
        """
        raise NotImplementedError()

"""
Principal Component Analysis
Dimensionality reduction that defines a basis that most heavily accounts
for the variability in the data
"""
class PCA(Model):

    def __init__(self, X, target_dim):
        self.num_x = X.shape[0]
        self.x_dim = X.shape[1]
        self.target_dim = target_dim
        self.W = None

    def fit(self, X):

        # making each dimension zero-mean
        means = np.mean(X, axis=0)
        big_means = np.matlib.repmat(means, X.shape[0], 1)
        X -= big_means

        # making each dimension have unit variance
        vars = np.std(X, axis=0)

        for i, var in enumerate(vars):
            if var != 0:
                X[:, i] /= var

        # construct covariance matrix and find eigenvals/vectors
        S = X.T @ X / (X.shape[0] - 1)
        w, v = eig(S, left=True, right=False)

        # need to sort eigenvalues and eigenvectors in the same way
        sorted_inds = w.argsort()[::-1]
        w = w[sorted_inds]
        v = v[:, sorted_inds]

        # only keep the desired number of prinicipal directions
        v = v[:, :self.target_dim]

        # transform in to prinicipal component space
        X = X @ v

        return X


"""
Locally linear encoding
Reduces dimensionality while preserving neighborhood structure
in high dimensional data
"""
class LLE(Model):

    def __init__(self, X, target_dim, lle_k):
        self.num_x = X.shape[0]
        self.x_dim = X.shape[1]

        self.target_dim = target_dim
        self.k = lle_k

    def fit(self, X):

        # making each dimension zero-mean
        means = np.mean(X, axis=0)
        big_means = np.matlib.repmat(means, X.shape[0], 1)
        X -= big_means

        # making each dimension have unit variance
        variances = np.std(X, axis=0)

        for i, var in enumerate(variances):
            if var != 0:
                X[:, i] /= var

        # using KNN sneakily to get nearest neighbors
        knn = KNN(self.k)
        knn.fit(X, None)
        k_neighbors = knn.get_nearest(X)

        # initialize weight matrix
        W = np.zeros((self.num_x, self.num_x))

        for i in range(self.num_x):

            x_i = X[i]

            # get distances from neighbors and covariances
            neighbors = X[k_neighbors[i]]
            Z = (neighbors - x_i).T
            C = Z.T @ Z

            # add regularization term
            C += 0.001 * np.trace(C) * np.eye(C.shape[0])

            # solve for weights and normalize, put in big matrix
            w = np.linalg.lstsq(C, np.ones(C.shape[0]), rcond=None)[0]
            w /= w.sum()
            W[i, k_neighbors[i]] = w

        # construct sparse matrix, find eigenvalues/vectors
        I = np.eye(self.num_x)
        M = (I - W).T @ (I - W)
        w, v = eig(M, left=True, right=False)

        # need to sort eigenvalues and eigenvectors in the same way
        sorted_inds = w.argsort()
        w = w[sorted_inds]
        v = v[:, sorted_inds]

        # get rid of first and extra vectors
        v = v[:, 1:self.target_dim + 1]

        return v

"""
Simple k-nearest neighbor algorithm
"""
class KNN(Model):

    def __init__(self, k):
        self.k = k
        self.data = None
        self.labels = None

    """
    Trivial to fit, just store data
    """
    def fit(self, X, y):
        self.data = X
        self.labels = y

    def predict(self, X):

        k = self.k
        predictions = []

        # go point by point
        for i in range(X.shape[0]):

            # use this to store indices to track later
            indices = np.arange(self.data.shape[0])

            # calculate distances and use them to sort
            distances = np.sqrt(np.sum(np.square(self.data - X[i]), axis=1))
            sorted_distances = sorted(zip(distances, indices))

            # get labels of k nearest neighbors and set mode as prediction
            closest_labels = [self.labels[y[1]] for y in sorted_distances[:k]]
            predictions.append(mode(closest_labels)[0][0])

        return predictions

    def get_nearest(self, X):

        k = self.k
        predictions = np.zeros((X.shape[0], k), dtype=int)

        for i in range(X.shape[0]):

            # sort by distance
            indices = np.arange(self.data.shape[0])
            distances = np.sqrt(np.sum(np.square(self.data - X[i]), axis=1))
            sorted_distances = sorted(zip(distances, indices))

            # store indices of k nearest neighbors
            closest_labels = [y[1] for y in sorted_distances[1:k + 1]]
            predictions[i] = closest_labels

        return predictions

