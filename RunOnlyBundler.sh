#!/bin/bash
#
# RunBundler.sh
#   copyright (c) 2008-2013 Noah Snavely
#
# A script for preparing a set of image for use with the Bundler 
# structure-from-motion system.
#
# Usage: RunBundler.sh [config_file]
#
# config_file describes [optional] configuration options, including:
#
#   IMAGE_LIST=<list.txt> # file with input list of images / focal lengths
#   IMAGE_DIR=<dir> # directory containing the images you'd like to process
#   MATCH_WINDOW_RADIUS=<num> # only match images in a sliding 
#                             # window of size 2rad+1 
#   INIT_FOCAL=<num>   # value to use for initial focal length
#   FOCAL_WEIGHT=<num> # weight used to constrain focal length
#   TRUST_FOCAL=true   # tell bundler to trust the provided focal lengths
#   RAY_ANGLE_THRESHOLD=<num> # used to remove ill-conditioned points
#   USE_CERES=true # enables use of Ceres solver for bundle adjustment
#                  # (if this is enabled at compile time)
#   NUM_MATCHES_ADD_CAMERA=<num> # number of matches above which bundler
#                                # will definitely attempt to add a camera
#                                # (e.g., you might use 500)
#

BASE_PATH=$(dirname $(which $0));


OS=`uname -o`

if [ $OS == "Cygwin" ]
then
    MATCHKEYS=$BASE_PATH/bin/KeyMatchFull.exe
    BUNDLER=$BASE_PATH/bin/Bundler.exe
	echo "Using Cygwin"
else
    MATCHKEYS=$BASE_PATH/bin/KeyMatchFull
    BUNDLER=$BASE_PATH/bin/bundler
fi

IMAGE_LIST=list.txt


# Run Bundler!
echo "[- Running Only Bundler -]"
rm -f constraints.txt
rm -f pairwise_scores.txt
$BUNDLER $IMAGE_LIST --options_file options.txt > bundle/bundle.log

echo "[- Done -]"
