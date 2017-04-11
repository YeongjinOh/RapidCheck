import tensorflow as tf
import os
import dark
import numpy as np
from os import sep

def model_name(file_path):
    file_name = file_path.split(sep)[-1]
    ext = str()
    if '.' in file_name: # exclude extension
        file_name = file_name.split('.')
        ext = file_name[-1]
        file_name = '.'.join(file_name[:-1])
    if ext == str() or ext == 'meta': # ckpt file
        file_name = file_name.split('-')
        num = int(file_name[-1])
        return '-'.join(file_name[:-1])
    if ext == 'weights':
        return file_name