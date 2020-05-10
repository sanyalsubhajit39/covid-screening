
# ################################################ CROWD SCREENING WITH DEEP LEARNING #########################

# GreenHolics- This CNN -CC algorithm will be integrated in the edge nodes of the CCTV cameras to detect realtime gathering and report to thr authourity
# 2 nd-part

## YOU must edit Your .jsons to point to thhe respective locations on your downloaded datasets
## if using any web based gpu then Fulfil as per their formalities of dataset location.

## Datasets
ShanghaiTech Dataset: [Google Drive](https://drive.google.com/open?id=16dhJn7k4FWVwByRsQAEpl9lwjuV03jVI)
Now, move the dataset into the repository you cloned above and unzip it.
In the dataset above change the folders Ground_truth to Ground-truth in both part_A_final and part_B_final. 
Now, open make_dataset.ipynb in Greenholics-Ct Master folder, execute it.	TO TRAIN THE DEEP LEARNING MODEL

## Prerequisites
We strongly recommend Anaconda as the environment.

Python: 3.8.2

PyTorch: 0.4.0

CUDA: 10.1
## Ground Truth

Please follow the `make_dataset.ipynb ` to generate the ground truth. It shall take some time to generate the dynamic ground truth. Note you need to generate your own json file.

## Training Process

Try `python train.py train.json val.json 0 0` to start training process.

## Validation

Follow the `val.ipynb` to try the validation. You can try to modify the notebook and see the output of each image.
## Results

ShanghaiA MAE: 66.4 [Google Drive](https://drive.google.com/open?id=1Z-atzS5Y2pOd-nEWqZRVBDMYJDreGWHH)
ShanghaiB MAE: 10.6 [Google Drive](https://drive.google.com/open?id=1zKn6YlLW3Z9ocgPbP99oz7r2nC7_TBXK)
## Performance Evaluation of CNN-CC Algorithms
In this section, our main goal was to evaluate the selected existing CNN-CC algorithms.
For evaluation purposes, we considered a common performance metric: MAE, where N is the number
of test samples, yi is used for ground-truth count, and y0
i is the estimated count of i th sample.
MAE =
1
N
Nå
i=1
jyi 􀀀 y0
i j (

## References

Inspired by CNN_CC reaseach paper by Zing zou.
```
Please cite the Shanghai datasets and other works if you use them.


