#!/usr/bin/env python3

"""
Akash Chaurasia
achaura1@jhu.edu
JHU CS461 Fall 2019
Homework 2
"""

import cv2 as cv
import numpy as np
from numpy import linalg
from scipy.ndimage.filters import maximum_filter
from scipy.signal import convolve2d
import random as rnd

"""
Gets patches of a given window size surrounding
the given corners in the given images

Returns a list of wsize x wsize patches from the image
"""
def get_patches(image, corners, wsize):

    # get radius of window
    patches = []
    rad = int(wsize / 2)

    # go through given corners
    for x, y in corners:

        # if too close to edge, ignore and append None
        if (y - rad < 0 or y + rad >= image.shape[0]
            or x - rad < 0 or x + rad >= image.shape[1]):
            patches.append(None)
            continue

        # append patch to list
        patches.append(image[y - rad:y + rad + 1, x - rad:x + rad + 1])

    return patches

"""
Calculates the zero-mean normalized cross-correlation
of two patches

returns score of correlation
"""
def zero_ncc(mat1, mat2):
    # normalization factor
    s_norm = np.std(mat1) * np.std(mat2)

    # compute unnormalized cross-correlation
    cross_corr = np.sum(np.multiply(mat1 - np.mean(mat1),
                                    mat2 - np.mean(mat2)))

    return (cross_corr / (s_norm * mat1.size))

"""
Calculates best fits for each patch in patches1

Returns a matrix with the zero_ncc score for 
each pair of patches
"""
def calculate_fit(res_mat, patches1, patches2):

    # iterate through patches in patches1
    for i, patch in enumerate(patches1):
        # if invalid patch set score to min score
        if patch is None:
            res_mat[i, :] = -1
            continue

        # find score for patch1 and patch2 and store
        for j, patch2 in enumerate(patches2):
            if patch2 is None:
                res_mat[i, j] = -1
                continue
            res_mat[i, j] = zero_ncc(patch, patch2)

    return res_mat

def detect_corners(image):
    """Harris corner detector.

    Args:
    - image (2D float64 array): A grayscale image.

    Returns:
    - corners (list of 2-tuples): A list of 2-tuples representing the locations
      of detected corners. Each tuple contains the (x, y) coordinates of a
      pixel, where y is row index and x is the column index, i.e. `image[y, x]`
      gives the corresponding pixel intensity.
    """
    # hyperparameters, sobel kernel size, window-size of corner and k-value
    w_diam = 7
    k = 0.05
    s_size = 3
    r_thresh = 0.01
    nms_window = w_diam

    # use sobel filter to get gradients
    x_grad = cv.Sobel(image, cv.CV_64F, 1, 0, ksize=s_size)
    y_grad = cv.Sobel(image, cv.CV_64F, 0, 1, ksize=s_size)

    # get second order terms
    I_xy = np.multiply(x_grad, y_grad)
    I_xx = np.square(x_grad)
    I_yy = np.square(y_grad)


    # kernel to use to sum over window
    sum_kernel = np.ones((w_diam, w_diam))

    # use kernel to convolve and sum over windows
    H_xx = convolve2d(I_xx, sum_kernel, mode='same')
    H_yy = convolve2d(I_yy, sum_kernel, mode='same')
    H_xy = convolve2d(I_xy, sum_kernel, mode='same')

    # construct determinant and trace squared matrices
    Dets = np.multiply(H_xx, H_yy) - np.square(H_xy)
    Trace_sq = np.square(H_xx + H_yy)

    # get R values based on determinants and traces
    R_map = Dets - k * Trace_sq
    R_map[R_map < r_thresh * np.amax(R_map)] = 0.0

    # NMS using given method
    R_map = nonmaxsup(R_map, nms_window)

    # get corner coordinates
    y_inds, x_inds = np.nonzero(R_map)
    corners = []

    # storing as (x, y) for ease of use later with cv
    for i, y in enumerate(y_inds):
        corners.append((x_inds[i], y))

    return corners

def nonmaxsup(scores, ksize):
  """Apply non-maximum suppression on a score map.

  Args:
  - scores (2D float64 array): A score map.
  - ksize (int): Kernel size of the maximum filter being used.

  Returns:
  - suppressed (2D float64 array): Suppressed version of `scores` where all
      elements except the local maxima are set to 0.
  """
  suppressed = np.copy(scores)
  filtered = maximum_filter(suppressed, (ksize, ksize))
  maxima = (suppressed == filtered)
  suppressed[np.logical_not(maxima)] = 0
  return suppressed


def match_corners(img1, img2, corners1, corners2):
    """Match corners using mutual marriages.

    Args:
    - image1 (2D float64 array): A grayscale image.
    - image2 (2D float64 array): A grayscale image.
    - corners1 (list of 2-tuples): Corners in image1.
    - corners2 (list of 2-tuples): Corners in image2.

    Returns:
    - matches (list of 2-tuples): A list of 2-tuples representing the matching
      indices. Each tuple contains two integer indices. For example, tuple
      (0, 42) indicates that corners1[0] is matched to corners2[42].
    """

    # set window size and get patches of wsize for each image
    wsize = 9
    patches1 = get_patches(img1, corners1, wsize)
    patches2 = get_patches(img2, corners2, wsize)

    # calculate score for each pair of patches
    patches1_fit = np.zeros((len(patches1), len(patches2)))
    patches2_fit = np.zeros((len(patches2), len(patches1)))
    patches1_fit = calculate_fit(patches1_fit, patches1, patches2)
    patches2_fit = calculate_fit(patches2_fit, patches2, patches1)

    # get best fits for each set of patches
    patches1_matches = np.argmax(patches1_fit, axis=1)
    patches2_matches = np.argmax(patches2_fit, axis=1)

    matches = []

    # only add two-way matches
    for i, match_ind in enumerate(patches1_matches):
        for j, reverse_ind in enumerate(patches2_matches):
            if i == reverse_ind and match_ind == j:
                matches.append((i, j))

    return matches

"""
Stacks the two given images, figuring out dimensionality accordingly

returns the two images stacked horizontally
"""
def stack_images(image1, image2):

    # get output dimensions and initalize zero matrix
    max_h = max(image1.shape[0], image2.shape[0])
    width = image1.shape[1] + image2.shape[1]
    output = np.zeros((max_h, width, 3), dtype=np.uint8)

    # offset for second image
    offset = image1.shape[1]

    # copy image values
    output[:image1.shape[0], :image1.shape[1], :] = image1
    output[:image2.shape[0], offset:, :] = image2

    return offset, output

def draw_matches(image1, image2, corners1, corners2, matches,
    outlier_labels=None):
    """Draw matched corners between images.

    Args:
    - matches (list of 2-tuples)
    - image1 (3D uint8 array): A color image having shape (H1, W1, 3).
    - image2 (3D uint8 array): A color image having shape (H2, W2, 3).
    - corners1 (list of 2-tuples)
    - corners2 (list of 2-tuples)
    - outlier_labels (list of bool)

    Returns:
    - match_image (3D uint8 array): A color image having shape
      (max(H1, H2), W1 + W2, 3).
    """
    # get offset and stacked images
    offx, stack = stack_images(image1, image2)
    
    # define colors for ease of use
    red = (0, 0, 255)
    blue = (255, 0, 0)
    green = (0, 255, 0)

    # readjust corner locations based on offset
    for i, (ind1, ind2) in enumerate(matches):
        c1 = corners1[ind1]
        c2 = corners2[ind2]
        c2 = (offx + c2[0], c2[1])

        # draw the corners and line between them
        cv.circle(stack, c1, 2, green, -1)
        cv.circle(stack, c2, 2, green, -1)

        # if RANSAC matches then color appropriately, otherwise just blue
        if outlier_labels is not None and outlier_labels[i]:
            cv.line(stack, c1, c2, red)
        else:
            cv.line(stack, c1, c2, blue)

    return stack

def compute_affine_xform(corners1, corners2, matches):
    """Compute affine transformation given matched feature locations.

    Args:
    - corners1 (list of 2-tuples)
    - corners1 (list of 2-tuples)
    - matches (list of 2-tuples)

    Returns:
    - xform (2D float64 array): A 3x3 matrix representing the affine
      transformation that maps coordinates in image1 to the corresponding
      coordinates in image2.
    - outlier_labels (list of bool): A list of Boolean values indicating whether
      the corresponding match in `matches` is an outlier or not. For example,
      if `matches[42]` is determined as an outlier match after RANSAC, then
      `outlier_labels[42]` should have value `True`.
    """
    # hyperparameters used to define RANSAC algorithm
    p = 0.999
    e = 0.5
    # minimum number of matches needed to define affine transform
    s = 3
    # threshold for a transformed point to be considered an inlier
    inlier_thresh = 20.0
    N = int(np.log(1.0 - p) / np.log(1 - (1 - e) ** s))

    xforms = []
    num_inliers = []


    # perform N iterations
    for i in range(N):
        # randomly get three pairs of points
        seed = rnd.sample(range(len(matches)), s)
        A = np.zeros((2 * s, 6))
        b = np.zeros(2 * s)

        # add randomly selected points to A and b matrices
        for j, p_ind in enumerate(seed):
            x1, x2 = corners1[matches[p_ind][0]], corners2[matches[p_ind][1]]
            A[2 * j, :3] = [x1[0], x1[1], 1]
            A[2 * j + 1, 3:] = [x1[0], x1[1], 1]
            b[2 * j:2 * j + 2] = [x2[0], x2[1]]

        # get transform coefficients using least squares, convert to matrix
        t = np.linalg.lstsq(A, b, rcond=None)[0]

        t = np.array([[t[0], t[1], t[2]],
                      [t[3], t[4], t[5]],
                      [0, 0, 1]], dtype=float)

        # determine number of inliers
        inlier_count = 0
        for match in matches:
            x1 = np.array([corners1[match[0]][0], corners1[match[0]][1], 1])
            x2 = np.array([corners2[match[1]][0], corners2[match[1]][1], 1])

            # get calculated x2, determine if inlier
            x2_calc = t @ x1

            if np.sum(np.square(x2_calc - x2)) <= inlier_thresh:
                inlier_count += 1

        xforms.append(t)
        num_inliers.append(inlier_count)

    # best transform determined by max number of inliers
    best_ind = np.argmax(num_inliers)
    best_t = xforms[best_ind]

    # get outlier labels using same method as earlier
    outliers = []
    for match in matches:
        x1 = np.array([corners1[match[0]][0], corners1[match[0]][1], 1])
        x2 = np.array([corners2[match[1]][0], corners2[match[1]][1], 1])

        x2_calc = best_t @ x1

        if np.sum(np.square(x2_calc - x2)) <= inlier_thresh:
            outliers.append(False)
        else:
            outliers.append(True)


    return best_t, outliers


# Extra Credit
def compute_proj_xform(corners1, corners2, matches):
  """Compute projective transformation given matched feature locations.

  Args:
  - corners1 (list of 2-tuples)
  - corners1 (list of 2-tuples)
  - matches (list of 2-tuples)

  Returns:
  - xform (2D float64 array): A 3x3 matrix representing the projective
      transformation that maps coordinates in image1 to the corresponding
      coordinates in image2.
  - outlier_labels (list of bool)
  """
  raise NotImplementedError


def stitch_images(image1, image2, xform):
  """Stitch two matched images given the transformation between them.

  Args:
  - image1 (3D uint8 array): A color image.
  - image2 (3D uint8 array): A color image.
  - xform (2D float64 array): A 3x3 matrix representing the transformation
      between image1 and image2. This transformation should map coordinates
      in image1 to the corresponding coordinates in image2.

  Returns:
  - image_stitched (3D uint8 array)
  """
  # apply affine transformation
  t_image1 = cv.warpAffine(image1, xform[:2, :], (image2.shape[1], image2.shape[0]))
  # stitch by calculating weighted sum of transformed im1 and image2
  output = cv.addWeighted(t_image1, 0.5, image2, 0.5, 0)

  return output


# Extra Credit
def detect_blobs(image):
  """Laplacian blob detector.

  Args:
  - image (2D float64 array): A grayscale image.

  Returns:
  - corners (list of 2-tuples): A list of 2-tuples representing the locations
      of detected blobs. Each tuple contains the (x, y) coordinates of a
      pixel, which can be indexed by image[y, x].
  - scales (list of floats): A list of floats representing the scales of
      detected blobs. Has the same length as `corners`.
  - orientations (list of floats): A list of floats representing the dominant
      orientation of the blobs.
  """
  raise NotImplementedError


# Extra Credit
def compute_descriptors(image, corners, scales, orientations):
  """Compute descriptors for corners at specified scales.

  Args:
  - image (2d float64 array): A grayscale image.
  - corners (list of 2-tuples): A list of (x, y) coordinates.
  - scales (list of floats): A list of scales corresponding to the corners.
      Must have the same length as `corners`.
  - orientations (list of floats): A list of floats representing the dominant
      orientation of the blobs.

  Returns:
  - descriptors (list of 1d array): A list of desciptors for each corner.
      Each element is an 1d array of length 128.
  """
  if len(corners) != len(scales) or len(corners) != len(orientations):
    raise ValueError(
        '`corners`, `scales` and `orientations` must all have the same length.')

  raise NotImplementedError


# Extra Credit
def match_descriptors(descriptors1, descriptors2):
  """Match descriptors based on their L2-distance and the "ratio test".

  Args:
  - descriptors1 (list of 1d arrays):
  - descriptors2 (list of 1d arrays):

  Returns:
  - matches (list of 2-tuples): A list of 2-tuples representing the matching
      indices. Each tuple contains two integer indices. For example, tuple
      (0, 42) indicates that corners1[0] is matched to corners2[42].
  """
  raise NotImplementedError


def baseline_main():

    im_name1 = 'graf1'
    im_name2 = 'graf3'
    img_path1 = 'data/' + im_name1 + '.png'
    img_path2 = 'data/' + im_name2 + '.png'

    img1 = cv.imread(img_path1, cv.IMREAD_COLOR)
    img2 = cv.imread(img_path2, cv.IMREAD_COLOR)

    gray1 = cv.cvtColor(img1, cv.COLOR_BGR2GRAY) / 255.0
    gray2 = cv.cvtColor(img2, cv.COLOR_BGR2GRAY) / 255.0

    corners1 = detect_corners(gray1)
    corners2 = detect_corners(gray2)

    output_dir = 'output/'

    matches = match_corners(gray1, gray2, corners1, corners2)

    stack = draw_matches(img1, img2, corners1, corners2, matches)

    cv.imwrite(output_dir + im_name1[:-1] + '_stacked.png', stack)
    affine_xform, outlier_labels = compute_affine_xform(corners1, corners2, matches)

    new_stack = draw_matches(img1, img2, corners1, corners2, matches, outlier_labels)
    cv.imwrite(output_dir + im_name1[:-1] + '_RANSACstacked.png', new_stack)

    stitched_image = stitch_images(img1, img2, affine_xform)
    cv.imwrite(output_dir + im_name1[:-1] + '_stitched.png', stitched_image)

if __name__ == '__main__':
  baseline_main()

