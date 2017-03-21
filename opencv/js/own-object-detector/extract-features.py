# Import the functions to calculate feature descriptors
from skimage.feature import local_binary_pattern
from skimage.feature import hog
from skimage.io import imread
from sklearn.externals import joblib
# To read file names
import argparse as ap
import glob
import os
from config import *

if __name__ == "__main__":
    # Argument Parser
    parser = ap.ArgumentParser()
    parser.add_argument('-i', "--imgpath", help="Path to Train Images",
            default="./dataset/CarData/TrainImages/")
    parser.add_argument('-d', "--descriptor", help="Descriptor to be used -- HOG",
            default="HOG")
    args = vars(parser.parse_args())

    train_img_path = args["imgpath"]
	
    des_type = args["descriptor"]

    pos_feat_ph = './features/ColorCars/pos'
    neg_feat_ph = './features/ColorCars/neg'

    # If feature directories don't exist, create them
    if not os.path.isdir(pos_feat_ph):
        os.makedirs(pos_feat_ph)
        print("Making positive feature directory %s" % pos_feat_ph)

    # If feature directories don't exist, create them
    if not os.path.isdir(neg_feat_ph):
        os.makedirs(neg_feat_ph)
        print("Making negative feature directory %s" % neg_feat_ph)

    print("Calculating the descriptors for the positive/negative samples and saving them")
    
    for im_path in glob.glob(os.path.join(train_img_path, "*")):
        img_name = im_path.split('/')[-1]
        class_ = img_name.split('-')[0]
        if class_ == 'pos':
            im = imread(im_path, as_grey=True)
            if des_type == "HOG":
                fd = hog(im, orientations, pixels_per_cell, cells_per_block, visualize, normalize)
            fd_name = os.path.split(im_path)[1].split(".")[0] + ".feat"
            print(">> fd_name : ", fd_name)
            fd_path = os.path.join(pos_feat_ph, fd_name)
            joblib.dump(fd, fd_path)
        elif class_ == 'neg':
            im = imread(im_path, as_grey=True)
            if des_type == "HOG":
                fd = hog(im,  orientations, pixels_per_cell, cells_per_block, visualize, normalize)
            fd_name = os.path.split(im_path)[1].split(".")[0] + ".feat"
            print(">> fd_name : ", fd_name)
            fd_path = os.path.join(neg_feat_ph, fd_name)
            joblib.dump(fd, fd_path)

    print("Positive features saved in {}".format(pos_feat_ph))
    
    print("Negative features saved in {}".format(neg_feat_ph))
    
    print("Completed calculating features from training images")
