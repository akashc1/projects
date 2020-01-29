"""
Akash Chaurasia
akash@jhu.edu
"""

import numpy as np

'''
Class used to hold the tree object. Recursively splits
in its own split method and makes predictions
'''
class Node:
    # standard initializer
    def __init__(self, max_depth, cur_depth):

        self.dim = None
        self.thresh = None
        self.is_leaf = False
        self.pred = None
        self.left, self.right = None, None
        self.depth = cur_depth

    # finds the best feature and threshold to split data on
    # and splits into left and right nodes, recurses
    def split(self, X, y, max_depth, cur_depth):

        # depth base case
        if cur_depth == max_depth:
            self.is_leaf = True
            self.pred = np.mean(y)
            return self

        best_dim = 0
        best_ind = 0

        # get best feature dimension & threshold
        best_dim, thresh = self.optimal_params(X, y)

        # split data
        self.dim = best_dim
        self.thresh = thresh
        left_inds = np.argwhere(X[:, best_dim] < thresh).flatten()
        right_inds = np.argwhere(X[:, best_dim] >= thresh).flatten()

        left_X, right_X = X[left_inds, :], X[right_inds, :]
        left_y, right_y = y[left_inds], y[right_inds]

        left = Node(max_depth, cur_depth + 1)
        right = Node(max_depth, cur_depth + 1)

        # check for left and right base cases to avoid recursion
        if np.sum(np.var(left_X, axis=0)) == 0.0 or left_X.shape[0] <=1:
            left.is_leaf = True
            left.pred = np.mean(left_y)
            self.left = left
        else:
            self.left = left.split(left_X, left_y, max_depth, cur_depth + 1)


        if np.sum(np.var(right_X, axis=0)) == 0.0 or right_X.shape[0] <= 1:
            right.is_leaf = True
            right.pred = np.mean(right_y)
            self.right = right
        else:
            self.right = right.split(right_X, right_y, max_depth, cur_depth + 1)

        return self


    # iterate through unique features and find best dimension, threshold
    def optimal_params(self, X, y):
        
        dim, thresh = 0, 0.0

        min_sse = float("inf")

        # go through all dimensions
        for d in range(X.shape[1]):

            # only look at unique thresholds in this dimension
            unique_thetas = np.unique(X[:, d])
            for t in unique_thetas:

                left_inds = np.argwhere(X[:, d] < t).flatten()
                right_inds = np.argwhere(X[:, d] >= t).flatten()

                # skip if not really a split
                if len(left_inds) == 0 or len(right_inds) == 0:
                    continue

                left_y = y[left_inds]
                right_y = y[right_inds]
                left_y = left_y - np.mean(left_y)
                right_y = right_y - np.mean(right_y)

                # get updated sse, update values if lower
                cur_error = np.dot(left_y, left_y) + np.dot(right_y, right_y)

                if cur_error < min_sse:
                    dim = d
                    thresh = t
                    min_sse = cur_error

        return dim, thresh


class RegressionTree(object):
    def __init__(self, nfeatures, max_depth):
        self.num_input_features = nfeatures
        self.max_depth = max_depth
    def fit(self, *, X, y):
        """ Fit the model.
                Args:
                X: A of floats with shape [num_examples, num_features].
                y: An array of floats with shape [num_examples].
                max_depth: An int representing the maximum depth of the tree
        """

        # initialize root and split, recurse
        root = Node(self.max_depth, 0)
        self.root = root.split(X, y, self.max_depth, 0)


    def predict(self, X):

        """ Predict.
        Args:
                X: A  matrix of floats with shape [num_examples, num_features].

        Returns:
                An array of floats with shape [num_examples].
        """
        y = np.zeros(X.shape[0])

        # starting from root, follow thresholds to best leaf
        for i in range(X.shape[0]):
            cur_node = self.root
            while not (cur_node.is_leaf):
                thresh, dim = cur_node.thresh, cur_node.dim
                if X[i, dim] < thresh:
                    cur_node = cur_node.left
                else:
                    cur_node = cur_node.right

            y[i] = cur_node.pred

        return y



class GradientBoostedRegressionTree(object):
    def __init__(self, nfeatures, max_depth, n_estimators, regularization_parameter):
        self.num_input_features = nfeatures
        self.max_depth = max_depth
        self.n_estimators = n_estimators
        self.regularization_parameter = regularization_parameter
    def fit(self, *, X, y):
        """ Fit the model.
                Args:
                X: A of floats with shape [num_examples, num_features].
                y: An array of floats with shape [num_examples].
                max_depth: An int representing the maximum depth of the tree
                n_estimators: An int representing the number of regression trees to iteratively fit
        """

        # initialize as mean
        self.F_0 = np.mean(y)
        self.h_list = []

        F = self.F_0 * np.ones(y.shape[0])

        # fit to residuals, add predictions to F
        for m in range(self.n_estimators):
            new_tree = RegressionTree(self.num_input_features,
                                        self.max_depth)

            g = y - F

            new_tree.fit(X=X, y=g)
            h = new_tree.predict(X=X)

            F += h * self.regularization_parameter
            self.h_list.append(new_tree)


    def predict(self, X):
        """ Predict.
        Args:
                X: A  matrix of floats with shape [num_examples, num_features].

        Returns:
                An array of floats with shape [num_examples].
        """
        # sum of all predicitions (add residuals)
        y = self.F_0 * np.ones(X.shape[0])

        for tree in self.h_list:
            y_m = tree.predict(X=X)

            y += self.regularization_parameter * y_m

        return y
