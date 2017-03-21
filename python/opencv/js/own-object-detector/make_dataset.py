import glob
import os
from shutil import copyfile, rmtree
import argparse as ap
from skimage.transform import resize
from skimage.io import imread, imsave
import cv2

# python3 -W ignore make_dataset.py -p ./dataset/ColorCars/TrainOrigins/ -n TrainResized
# python3 -W ignore make_dataset.py -p ./dataset/ColorCars/BackgroundOrigins/ -n BackgroundResized150150
if __name__ == '__main__':
	parser = ap.ArgumentParser()
	parser.add_argument('-p', "--dirpath", help="dir path to know how many files in there", required=True)
	parser.add_argument('-n', "--ndirpath", help="dir path to resized images", required=True)
	parser.add_argument('-d', "--debug", help="print debug message", default=False)
	args = vars(parser.parse_args())

	path = args['dirpath']
	
	for i, im_path in enumerate(glob.glob(os.path.join(path, "*"))):
		im_resized_path = "/".join( im_path.split('/')[:-2])+"/"+args['ndirpath']+'/'+im_path.split('/')[-1]
		img = imread(im_path)
		imgResized = resize(img, (150, 150))
		imsave(im_resized_path, imgResized)
		if args['debug']:
			print("orignal img path : ", im_path)
			print("orignal img info >> ", type(img), img.shape, img.dtype)
			print("resize img info >> ", type(imgResized), imgResized.shape, imgResized.dtype)
			print("resize img path : ", im_resized_path)
		if i % 100 == 0:
			print("{} image done..".format(i))

	print("Image Resize Done")
	print("Image Original file nums : ", len(os.listdir(path)))

# print(len(os.listdir(os.path.join(train_dir_path, 'pos'))))