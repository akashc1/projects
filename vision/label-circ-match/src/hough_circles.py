#!/usr/bin/env python3

"""
Akash Chaurasia
akashc@jhu.edu
"""

import cv2
import numpy as np
import sys
import math


def detect_edges(image):
    """Find edge points in a grayscale image.

    Args:
    - image (2D uint8 array): A grayscale image.

    Return:
    - edge_image (2D float array): A heat map where the intensity at each point
      is proportional to the edge magnitude.
    """
    im = image.copy()
    dims = image.shape
    edge_image = np.zeros(dims, dtype=float)
    
    # sobel filters
    x_g = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype=np.single)
    y_g = -1 * np.transpose(x_g)

    for i in range(dims[0]):
        for j in range(dims[1]):
            im_kernel = np.zeros((3, 3))
            
            # modify image kernel according to boundry constraints
            if i == 0:
                if j == 0:
                    im_kernel[1:3, 1:3] = im[i:i + 2, j:j + 2]
                elif j == (dims[1] - 1):
                    im_kernel[1:3, 0:2] = im[i:i + 2, j - 1:j + 1]
                else:
                    im_kernel[1:3, 0:3] = im[i:i + 2, j - 1:j + 2]
            elif j == 0:
                if i == (dims[0] - 1):
                    im_kernel[0:2, 1:3] = im[i - 1:i + 1, j:j + 2]
                else:
                    im_kernel[0:3, 1:3] = im[i - 1:i + 2, j:j + 2]

            elif i == (dims[0] - 1):
                if j == (dims[1] - 1):
                    im_kernel[0:2, 0:2] = im[i - 1:i + 1, j - 1:j + 1]
                else:
                    im_kernel[0:2, 0:3] = im[i - 1:i + 1, j - 1:j + 2]

            elif j == (dims[1] - 1):
                im_kernel[0:3, 0:2] = im[i - 1:i + 2, j - 1:j + 1]
                
            else:
                im_kernel[0:3, 0:3] = im[i - 1:i + 2, j - 1:j + 2]

            # get x and y magnitudes, get overall magnitude
            x_mag = im_kernel.flatten() @ x_g.flatten().T
            y_mag = im_kernel.flatten() @ y_g.flatten().T

            edge_image[i, j] = np.linalg.norm(np.array([x_mag, y_mag]))

    return edge_image

def hough_circles(edge_image, edge_thresh, radius_values):
    """Threshold edge image and calculate the Hough transform accumulator array.

    Args:
    - edge_image (2D float array): An H x W heat map where the intensity at each
      point is proportional to the edge magnitude.
    - edge_thresh (float): A threshold on the edge magnitude values.
    - radius_values (1D int array): An array of R possible radius values.

    Return:
    - thresh_edge_image (2D bool array): Thresholded edge image indicating
      whether each pixel is an edge point or not.
    - accum_array (3D int array): Hough transform accumulator array. Should have
      shape R x H x W.
    """

    # threshold, get range of angles, initialize acc array
    e_thresh = (edge_image >= edge_thresh)
    dims = e_thresh.shape
    theta = np.arange(0, 2*math.pi, 2*math.pi / 100)
    acc = np.zeros((len(radius_values), dims[0], dims[1]), dtype=int)

    offsets = []
  
    # add x- and y-offsets at each radius and angle
    for i, r in enumerate(radius_values):
        for t in theta:
            offsets.append((i, int(r*np.cos(t)), int(r*np.sin(t))))

    # add to acc array in positions at a radius from this point
    for i in range(dims[0]):
        for j in range(dims[1]):
            if e_thresh[i, j]:
                for (r_ind, dx, dy) in offsets:
                    x = j + dx
                    y = (dims[0] - 1 - i) + dy
                    if (x >= 0 and x < dims[1] and
                        y >= 0 and y < dims[0]):
                        acc[r_ind, y, x] += 1

    return e_thresh, acc

def find_circles(image, accum_array, radius_values, hough_thresh):

    """Find circles in an image using output from Hough transform.

    Args:
    - image (3D uint8 array): An H x W x 3 BGR color image. Here we use the
      original color image instead of its grayscale version so the circles
      can be drawn in color.
    - accum_array (3D int array): Hough transform accumulator array having shape
      R x H x W.
    - radius_values (1D int array): An array of R radius values.
    - hough_thresh (int): A threshold of votes in the accumulator array.

    Return:
    - circles (list of 3-tuples): A list of circle parameters. Each element
      (r, y, x) represents the radius and the center coordinates of a circle
      found by the program.
    - circle_image (3D uint8 array): A copy of the original image with detected
      circles drawn in color.
    """
    # threshold, find non-zero indices
    acc = accum_array
    acc[acc < hough_thresh] = 0
    (r_inds, y_vals, x_vals) = np.where(acc > 0)
    y_vals = (image.shape[0] - 1) - y_vals

    circ_list = []
    c_img = image.copy()

    # add to circle list, add circle in image
    for i in range(len(r_inds)):
        circ_list.append((radius_values[r_inds[i]], y_vals[i], x_vals[i]))
        cv2.circle(c_img, (x_vals[i], y_vals[i]), radius_values[r_inds[i]], (0, 255, 0), thickness=2)

    return circ_list, c_img


def main(argv):
    img_name = argv[0]

    img = cv2.imread('data/' + img_name + '.png', cv2.IMREAD_COLOR)
    gray_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    edges = detect_edges(gray_image)

    r_range = np.arange(25, 40)

    e_thresh, acc = hough_circles(edges, 0.6*np.amax(edges), r_range)
    cv2.imwrite('output/' + img_name + "_edges.png", e_thresh * 255)
    h_thresh = np.amax(acc) * 0.5
    c_list, c_img = find_circles(img, acc, r_range, h_thresh)
    print(c_list)
    cv2.imwrite('output/' + img_name + '_circles.png', c_img)

if __name__ == '__main__':
  main(sys.argv[1:])
