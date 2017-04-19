import sys

sys.path.append('.\\')

import tensorflow as tf
import cv2
import numpy as np
from tools.preprocess_pascal_voc import voc_2007_classes_name


if __name__ == '__main__':
    common_params = {'image_size': 448, 'num_classes': 20,
                     'batch_size': 1}
    net_params = {'cell_size': 7, 'boxes_per_cell': 2, 'weight_decay': 0.0005}
