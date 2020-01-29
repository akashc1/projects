"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np

class Model(object):
    """ Abstract model object.

    Contains a helper function which can help with some datasets.
    """

    def __init__(self, nfeatures):
        self.num_input_features = nfeatures

    def fit(self, *, X, y, lr):
        """ Fit the model.

        Args:
            X: A compressed sparse row matrix of floats with shape
                [num_examples, num_features].
            y: A dense array of ints with shape [num_examples].
            lr: A float, the learning rate of this fit step.
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

    def _fix_test_feats(self, X):
        """ Fixes some feature disparities between datasets.
            Makes sure the X features matches the weights
        """
        num_examples, num_input_features = X.shape
        if num_input_features < self.num_input_features:
            X = X.copy()
            X._shape = (num_examples, self.num_input_features)
        if num_input_features > self.num_input_features:
            X = X[:, :self.num_input_features]
        return X


class MCModel(Model):
    """ A multiclass model abstraction.
    It wants to know, up front:
        - How many features in the data
        - How many classes in the data
    """

    def __init__(self, *, nfeatures, nclasses):
        super().__init__(nfeatures)
        self.num_classes = nclasses


class MCPerceptron(MCModel):

    def __init__(self, *, nfeatures, nclasses):
        super().__init__(nfeatures=nfeatures, nclasses=nclasses)
        self.W = np.zeros((nclasses, nfeatures), dtype=np.float)

    """
    Overridden fit method for MC perceptron.
    """
    def fit(self, *, X, y, lr):

        num_examples = X.shape[0]

        for i in range(num_examples):

            # get prediction
            y_out = self.W @ np.transpose(X[i, :])
            y_hat = np.argmax(y_out)

            # apply learning rule if wrong prediction
            if y_hat != y[i]:
                self.W[y_hat, :] -= lr * X[i, :]
                self.W[y[i], :] += lr * X[i, :]


    """
    Overridden predict method for MC perceptron.
    """
    def predict(self, X):

        num_examples = X.shape[0] 
        
        # vector of predictions, one per example
        y = np.zeros((num_examples, 1))
        for i in range(num_examples): 

            # find class with highest score for each example
            y_out = self.W @ np.transpose(X[i, :]) 
            y[i] = np.argmax(y_out)
        
        return y
        
    """
    returns the score of this model's prediction for y == 1
    used in the 1vA model
    """
    def score(self, X):
        return self.W[1] @ np.transpose(X)

"""
Model that performs multiclass logistic regression
"""
class MCLogistic(MCModel):

    def __init__(self, *, nfeatures, nclasses):
        super().__init__(nfeatures=nfeatures, nclasses=nclasses)
        self.W = np.zeros((nclasses, nfeatures), dtype=np.float)

    def fit(self, *, X, y, lr):
        
        num_examples = X.shape[0]
        num_classes = self.W.shape[0]

        for i in range(num_examples):
            
            # get logits and apply softmax to make distribution
            logits = self.W @ np.transpose(X[i, :])
            s_maxes = self.softmax(logits)

            # gradient ascent rule for learning
            for j in range(num_classes):
                if j == y[i]:
                    self.W[j, :] += lr * (1 - s_maxes[j]) * X[i, :]
                else:
                    self.W[j, :] -= lr * s_maxes[j] * X[i, :]

    def predict(self, X):

        # get X, y ready
        X = self._fix_test_feats(X)
        num_examples = X.shape[0]
        y = np.zeros((num_examples, 1))

        for i in range(num_examples):
            # find class with highest score for each example
            y_out = self.W @ np.transpose(X[i, :])
            y[i] = np.argmax(y_out)

        return y

    """
    Applies softmax to a set of logits to create
    a valid probability distribution
    """
    def softmax(self, logits):
        # take exponent of all logits minus the max logit
        logits -= max(logits)
        s_max = np.exp(logits)
        # divide all elements by the sum across k's
        s_max /= sum(s_max)

        return s_max

    """
    returns the score of this model's prediction for y == 1
    used in the 1vA model
    """
    def score(self, X):
        return self.W[1] @ np.transpose(X)

"""
1vA Model using either perceptron or
logistic regression depending on what is given
for multiclassification
"""
class OneVsAll(Model):

    def __init__(self, *, nfeatures, nclasses, model_class):
        super().__init__(nfeatures)
        self.num_classes = nclasses
        self.model_class = model_class
        self.models = [model_class(nfeatures=nfeatures, nclasses=2) for _ in range(nclasses)]

    def fit(self, *, X, y, lr):
        
        for i in range(self.num_classes):

            # make class labels binary and specific to each model
            y_i = y.copy()
            y_i[y_i != i] = -1
            y_i[y_i == i] = 1
            y_i[y_i == -1] = 0

            # train each model
            self.models[i].fit(X=X, y=y_i, lr=lr)

    def predict(self, X):
        X = self._fix_test_feats(X)

        num_examples = X.shape[0]
        y = np.zeros((num_examples, 1))

        for i in range(num_examples):
            scores = np.zeros((self.num_classes, 1))

            # find model with highest score given class & example
            for j in range(self.num_classes):
                scores[j] = self.models[j].score(X[i, :])

            y[i] = np.argmax(scores)

        return y

