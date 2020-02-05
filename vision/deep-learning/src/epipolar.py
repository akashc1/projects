"""
Akash Chaurasia
achaura1@jhu.edu
JHU CS461 Fall 2019
Homework 3
"""

import numpy as np
from numpy import linalg
import cv2 as cv
import random as rnd


"""
Parses matches file to get corresponding features
in both images
"""
def get_matches(filename):

    f = open(filename)
    matches = []

    # parse match line by line
    for line in f:

        coords = line.strip().split()
        point1 = (float(coords[0]), float(coords[1]))
        point2 = (float(coords[2]), float(coords[3]))

        matches.append((point1, point2))

    f.close()

    # retun list of point tuples
    return matches

"""
Given the set of matches, uses RANSAC
to find the fundamental matrix that best fits
the matches.
"""
def get_fundamental_matrix(matches):

    # hyperparameters used to define RANSAC algorithm
    p = 0.9999
    e = 0.5
    # minimum number of matches needed to define fundamental matrix
    s = 8
    # threshold for a transformed point to be considered an inlier
    inlier_thresh = 0.01
    N = int(np.log(1.0 - p) / np.log(1 - (1 - e) ** s))

    xforms = []
    num_inliers = []

    # perform N iterations
    for i in range(N):
        # randomly get s pairs of points
        seed = rnd.sample(range(len(matches)), s)
        A = np.zeros((s, 9))

        # add randomly selected points to A and b matrices
        for j, p_ind in enumerate(seed):
            p1, p2 = np.array(matches[p_ind][0]), np.array(matches[p_ind][1])
            u1, v1 = p1[0], p1[1]
            u2, v2 = p2[0], p2[1]

            A[j] = [u1 * u2, u1 * v2, u1, v1 * u2, v1 * v2, v1, u2, v2, 1]


        # get transform coefficients using svd, convert to matrix
        Q, S, V = np.linalg.svd(A)
        f = np.reshape(V[8, :], (3, 3))

        Q, S, V = np.linalg.svd(f)
        f = Q @ np.diag([S[0], S[1], 0]) @ V

        # determine number of inliers
        inlier_count = 0
        for match in matches:

            p1, p2 = np.append(match[0], 1), np.append(match[1], 1)

            # get calculated x2, determine if inlier
            diff = p1 @ f @ p2.T

            if abs(diff) <= inlier_thresh:
                inlier_count += 1

        xforms.append(f)
        num_inliers.append(inlier_count)

    # best transform determined by max number of inliers
    best_ind = np.argmax(num_inliers)
    best_F = xforms[best_ind]

    # list of indices of inliers for sampling later
    inliers = []
    for i, match in enumerate(matches):
        p1, p2 = np.append(match[0], 1), np.append(match[1], 1)

        # get calculated diff, determine if inlier
        diff = p1 @ best_F @ p2

        if abs(diff) <= inlier_thresh:
            inliers.append(i)


    return best_F, inliers

def get_line_pts(F, l_pt, r_pt, dims):

    line_params = np.append(l_pt, 1) @ F
    ul, vl = l_pt[0], l_pt[1]
    a, b, c = line_params[0], line_params[1], line_params[2]

    a1 = F[0, 0] * ul + F[1, 0] * vl + F[2, 0]
    b1 = F[0, 1] * ul + F[1, 1] * vl + F[2, 1]
    c1 = F[0, 1] * ul + F[1, 2] * vl + F[2, 2]

    u1, u2 = 0, dims[1]

    v1 = int(-1 * c / b)
    v2 = int(-1 * (c + a * u2) / b)

    return (u1, v1), (u2, v2)


def draw_epi_lines(F, matches, inliers, im):

    seed = rnd.sample(inliers, 8)

    for ind in seed:

        pt1, pt2 = get_line_pts(F, matches[ind][0], matches[ind][1], im.shape)

        right_feat = matches[ind][1]
        right_pt = (int(right_feat[0]), int(right_feat[1]))

        cv.line(im, pt1, pt2, (0, 255, 0), 2)
        cv.circle(im, right_pt, 3, (0, 255, 255), -1)

    return im, seed

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

    return output

def main():

    matches = get_matches("data/matches.txt")
    F, inliers = get_fundamental_matrix(matches)

    im_l = cv.imread("data/hopkins1.jpg")
    im_r = cv.imread("data/hopkins2.jpg")

    im_r_epi, rand_inds = draw_epi_lines(F, matches, inliers, im_r)
    for ind in rand_inds:
        m1, m2 = matches[ind]
        pt = (int(m1[0]), int(m1[1]))
        cv.circle(im_l, pt, 2, (0, 255, 0), -1)

    both_images = stack_images(im_l, im_r_epi)
    cv.imwrite("output/hopkins_epi.jpg", both_images)

    print(F)


if __name__ == "__main__":
    main()
