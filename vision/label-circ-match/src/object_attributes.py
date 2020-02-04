#!/usr/bin/env python3
"""
Akash Chaurasia
akashc@jhu.edu
"""

import cv2
import numpy as np
import sys
import math

"""
Class for union find to hold equivalent labels
"""
class EQTable:

    def __init__(self):
        self.labels = [-1]

    def add_label(self, label):
        self.labels.append(label)

    # return label's parent
    # a label is its own parent if labels[label] = label
    def parent(self, label):

        l = self.labels
        while l[label] != label:
            label = l[label] = l[l[label]]

        return label

    # set parent of label1 equal to parent of label2
    def union(self, label1, label2):

        l = self.labels
        p1 = self.parent(label1)
        p2 = self.parent(label2)

        if p1 != p2:
            l[p1] = p2


"""
Turns grayscaled 8-bit image into binary
using a given threshold value
"""
def binarize(gray_image, thresh_val):
    binary_image = gray_image.copy()

    # set values >= thresh to 255, all others 0
    binary_image[binary_image >= thresh_val] = 255
    binary_image[binary_image < thresh_val] = 0
    return binary_image

"""
Get coordinatess of top, left, and top-left pixel
and their labels
"""
def get_neighbors(labeled_image, i, j):
    labels = []
    coords = []

    if i > 0:
        coords.append((i - 1, j))
        if j > 0:
            coords.append((i - 1, j - 1))

    if j > 0:
        coords.append((i, j - 1))

    for (x, y) in coords:
        val = labeled_image[x, y]
        if val != 0:
            labels.append(val)

    return labels, coords

"""
Sequential labeling algorithm

Takes in binary image, returns sequentially labeled image
"""
def label(binary_image):

    # initialize variables used for labeling
    dims = binary_image.shape
    im = np.zeros(dims, dtype=int)
    cur_label = 1

    # object used to store equivalent labels
    equivs = EQTable()

    # raster search
    for i in range(dims[0]):
        for j in range(dims[1]):
            if binary_image[i, j] != 0:

                # get coordinates & labels of valid neighbors
                n_labels, n_coords = get_neighbors(im, i, j)

                # need to create new label
                if len(n_labels) == 0:
                    im[i, j] = cur_label
                    equivs.add_label(cur_label)
                    cur_label += 1

                # use existing label, union equivalent labels
                else:
                    im[i, j] = min(n_labels)
                    for label in n_labels:
                        if label != im[i, j]:
                            equivs.union(label, im[i, j])

    # second pass, set all to parent label
    for i in range(dims[0]):
        for j in range(dims[1]):
            if im[i, j] != 0:
                im[i, j] = equivs.parent(im[i, j])

    return im

"""
Takes sequentially labeled image

Returns list of object attributes, describing
the properties of each object (label) in labeled image
"""
def get_attribute(labeled_image):

    dims = labeled_image.shape
    unique_labels, counts = np.unique(labeled_image, return_counts=True)

    # get rid of background label
    unique_labels = unique_labels[1:]
    counts = counts[1:]

    attribute_list = []


    for i, label in enumerate(unique_labels):
        area = counts[i]

        # find all coordinates of this label
        (y_vals, x_vals) = np.where(labeled_image == label)
        y_vals = (dims[1] - 1) - y_vals

        # get position coordinates
        x_bar = sum(x_vals) / area
        y_bar = sum(y_vals) / area

        # normalize all coordinates and format for ease of use
        n_xvals = x_vals - x_bar
        n_yvals = y_vals - y_bar
        coords = []

        for j in range(len(n_xvals)):
            coords.append((n_xvals[j], n_yvals[j]))

        a = 0
        b = 0
        c = 0

        # get second moment
        for (x, y) in coords:
            a += y ** 2
            b += 2 * x * y
            c += x ** 2

        # use second moment to calculate angles
        theta1 = 0.5 * np.arctan2(b, a - c)
        theta2 = theta1 + 0.5 * math.pi

        e_min = (a * (math.sin(theta1) ** 2) - (b / 2)*(math.sin(2 * theta1))
                    + c * (math.cos(theta1) ** 2))

        e_max = (a * (math.sin(theta2) ** 2) - (b / 2)*(math.sin(2 * theta2))
                    + c * (math.cos(theta2) ** 2))

        roundedness = e_min / e_max

        # place everything in output dictionary
        output = {}
        output['position'] = {'x': float(x_bar), 'y': float(y_bar)}
        output['orientation'] = float(theta1)
        output['roundedness'] = float(roundedness)

        attribute_list.append(output)

    return attribute_list

def main(argv):
    img_name = argv[0]
    thresh_val = int(argv[1])

    img = cv2.imread('data/' + img_name + '.png', cv2.IMREAD_COLOR)
    gray_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    binary_image = binarize(gray_image, thresh_val=thresh_val)
    labeled_image = label(binary_image)
    attribute_list = get_attribute(labeled_image)

    cv2.imwrite('output/' + img_name + "_gray.png", gray_image)
    cv2.imwrite('output/' + img_name + "_binary.png", binary_image)
    cv2.imwrite('output/' + img_name + "_labeled.png", labeled_image)
    print(attribute_list)


if __name__ == '__main__':
    main(sys.argv[1:])
