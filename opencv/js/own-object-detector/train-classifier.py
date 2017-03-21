# Import the required modules
from skimage.feature import local_binary_pattern
from sklearn.svm import LinearSVC
from sklearn import svm
from sklearn.linear_model import LogisticRegression
from sklearn.externals import joblib
import argparse as ap
import glob
import os
from config import *

# python3 train-classifier.py -p ./features/ColorCars10040/pos -n ./features/ColorCars10040/neg -m gray10040
# python3 train-classifier.py -p ./features/ColorCars150150/pos -n ./features/ColorCars150150/neg -m gray150150 -c RBF_SVM

if __name__ == "__main__":
    # Parse the command line arguments
    parser = ap.ArgumentParser()
    parser.add_argument('-p', "--posfeat", help="Path to the positive features directory", default='./features/ColorCars/pos')
    parser.add_argument('-n', "--negfeat", help="Path to the negative features directory", default='./features/ColorCars/neg')
    parser.add_argument('-c', "--classifier", help="Classifier to be used", default="LIN_SVM")
    parser.add_argument('-m', "--modelname", help="Define Model name in here", required=True)
    args = vars(parser.parse_args())

    pos_feat_path =  args["posfeat"]
    neg_feat_path = args["negfeat"]

    # Classifiers supported
    clf_type = args['classifier']

    fds = []
    labels = []
    # Load the positive features
    for feat_path in glob.glob(os.path.join(pos_feat_path,"*.feat")):
        fd = joblib.load(feat_path)
        fds.append(fd)
        labels.append(1)

    # Load the negative features
    for feat_path in glob.glob(os.path.join(neg_feat_path,"*.feat")):
        fd = joblib.load(feat_path)
        fds.append(fd)
        labels.append(0)

    if clf_type == "LIN_SVM":
        # clf = LinearSVC()
        clf = svm.SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
                    decision_function_shape=None, degree=3, gamma='auto', kernel='linear',
                    max_iter=-1, probability=False, random_state=None, shrinking=True,
                    tol=0.001, verbose=True)
        print("Training a Linear SVM Classifier")
        clf.fit(fds, labels)
        # If feature directories don't exist, create them
        
    elif clf_type == "POLY_SVM":
        print("Training a Polynomial SVM Classifier")
        clf = svm.SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
                    decision_function_shape=None, degree=3, gamma='auto', kernel='poly',
                    max_iter=-1, probability=False, random_state=None, shrinking=True,
                    tol=0.001, verbose=True)
        clf.fit(fds, labels)
    
    elif clf_type == "RBF_SVM":
        print("Training a RBF SVM Classifier")
        clf = svm.SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
                decision_function_shape=None, degree=3, gamma='auto', kernel='rbf',
                max_iter=-1, probability=False, random_state=None, shrinking=True,
                tol=0.001, verbose=True)
        clf.fit(fds, labels)
    else:
        print("clf type is not defined..")
    if not os.path.isdir(os.path.split(model_path)[0]):
        os.makedirs(os.path.split(model_path)[0])
        print(os.path.split(model_path)[0], " are created")
    model_path = model_path + clf_type + '_' +args['modelname']+ ".model"
    joblib.dump(clf, model_path)
    print("Classifier saved to {}".format(model_path))
