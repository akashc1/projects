#!/usr/bin/env python3

"""
Akash Chaurasia (achaura1)
achaura1@jhu.edu
CS 461 Fall 2019
"""

import cv2
import numpy as np
import sys


def normxcorr2(template, image):
    """Do normalized cross-correlation on grayscale images.

    When dealing with image boundaries, the "valid" style is used. Calculation
    is performed only at locations where the template is fully inside the search
    image.

    Args:
    - template (2D float array): Grayscale template image.
    - image (2D float array): Grayscale search image.

    Return:
    - scores (2D float array): Heat map of matching scores.
    """
    # initialize dimensioning variables
    i_dims = image.shape
    t_dims = template.shape
    o_dims = ((i_dims[0] - t_dims[0] + 1), (i_dims[1] - t_dims[1] + 1))

    dy = int(t_dims[0] / 2)
    dx = int(t_dims[1] / 2)

    scores = np.zeros(o_dims, dtype=float)
    eps = 0.00001
    
    # fill kernel with appropriate range of image values
    for i in range(dy, (o_dims[0] + dy)):
        for j in range(dx, (o_dims[1] + dx)):
            i_kernel = image[i - dy:i + dy + 1, j - dx:j + dx + 1]
            score = i_kernel.flatten() @ template.flatten().T
            nrg = np.linalg.norm(i_kernel) * np.linalg.norm(template) + eps
            scores[i - dy, j - dx] = score / nrg

    return scores

def find_matches(template, image, thresh=None):
    """Find template in image using normalized cross-correlation.

    Args:
    - template (3D uint8 array): BGR template image.
    - image (3D uint8 array): BGR search image.

    Return:
    - coords (2-tuple or list of 2-tuples): When `thresh` is None, find the best
      match and return the (x, y) coordinates of the upper left corner of the
      matched region in the original image. When `thresh` is given (and valid),
      find all matches above the threshold and return a list of (x, y)
      coordinates.
    - match_image (3D uint8 array): A copy of the original search images where
      all matched regions are marked.
    """

    # make images grayscale and noralize to [0.0, 1.0]
    gray_img = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    gray_tpl = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

    gray_img = np.divide(gray_img, np.amax(gray_img))
    gray_tpl = np.divide(gray_tpl, np.amax(gray_tpl))

    dy = int(gray_tpl.shape[0] / 2)
    dx = int(gray_tpl.shape[1] / 2)

    scores = normxcorr2(gray_tpl, gray_img)
    coords = []

    # only want max score: find first instance of max score
    if not thresh:
        scores[scores != np.amax(scores)] = 0
        (y, x) = np.nonzero(scores)
        y = y[0]
        x = x[0]
        coords.append((x, (scores.shape[0] - 1 - y)))

    # if valid threshold, find where above threshold and append coordinate
    elif thresh >= 0 and thresh <= 1:
        (y_vals, x_vals) = np.where(scores >= thresh)
        for i in range(len(y_vals)):
            y = scores.shape[0] - 1 - y_vals[i]
            coords.append((x_vals[i], y))

    # add rectangle to image
    for (x, y) in coords:
        y = scores.shape[0] - 1 - y
        cv2.rectangle(image, (x, y), (x + gray_tpl.shape[1], y + gray_tpl.shape[0]), color=(0, 255, 0), thickness=2)

    # in case of no threshold, just return tuple instead of list
    if len(coords) == 1:
        coords = coords[0]

    return coords, image

def main(argv):
    img_name = argv[0]
    tpl_name = argv[1]
    
    img = cv2.imread('data/' + img_name + '.png', cv2.IMREAD_COLOR)
    tpl = cv2.imread('data/' + tpl_name + '.png', cv2.IMREAD_COLOR)

    coords, matched_img = find_matches(tpl, img, thresh=0.99)
    cv2.imwrite('output/' + img_name + '.png', matched_img)
    print(coords)

if __name__ == '__main__':
    main(sys.argv[1:])
