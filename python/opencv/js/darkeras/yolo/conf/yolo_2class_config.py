"""
only 2class classifier yolo model config
	person
	car
	source : VOC 2007 DataSet
	author : SSUHan (ljs93kr@gmail.com)
"""
# Global config variables
import os

model_name = 'yolo-2class'
classes_name = ["car", "person"]

dataset_abs_location = os.path.join('C:\\\\','Users','SoMa','myworkspace','voc_dataset','VOCdevkit', 'VOC2007')
# dataset_abs_location = os.path.join('C:\\\\','Users','Soma2','myworkspace','voc_dataset','VOCtrainval_11-2012','VOCdevkit','VOC2012')
ann_location = os.path.join(dataset_abs_location, 'Annotations')
imageset_location = os.path.join(dataset_abs_location, 'JPEGImages')

cell_size = 7
num_classes = 2
boxes_per_cell = 2

class_scale = 1
object_scale = 1
noobject_scale = 0.5
coord_scale = 5

inp_size = 448, 448, 3
batch_size = 32
epochs=100

lr=0.0001
trainer='adam'

image_dim_order = 'th'
norm_type = 'scale_down'