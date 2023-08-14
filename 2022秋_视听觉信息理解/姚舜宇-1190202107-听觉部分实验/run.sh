#!/usr/bin/env bash
# Copyright   2017   Johns Hopkins University (Author: Daniel Garcia-Romero)
#             2017   Johns Hopkins University (Author: Daniel Povey)
#        2017-2018   David Snyder
#             2018   Ewald Enzinger
# Apache 2.0.
#
# See ../README.txt for more info on data required.
# Results (mostly equal error-rates) are inline in comments below.

. ./cmd.sh
. ./path.sh
set -e
mfccdir=`pwd`/mfcc
vaddir=`pwd`/mfcc

#设定数据集以及模型等的路径

# The trials file is downloaded by local/make_voxceleb1_v2.pl.
voxceleb1_trials=data/voxceleb1_test/trials
voxceleb1_root=/export/corpora/VoxCeleb1
voxceleb2_root=/export/corpora/VoxCeleb2
nnet_dir=exp/xvector_nnet_1a
musan_root=/export/corpora/JHU/musan

stage=0

# 用mfcc提取测试集特征
# Make MFCCs and compute the energy-based VAD for each dataset
for name in voxceleb1_test; do
steps/make_mfcc.sh --write-utt2num-frames true --mfcc-config conf/mfcc.conf --nj 40 --cmd "$train_cmd" \
data/${name} exp/make_mfcc $mfccdir
utils/fix_data_dir.sh data/${name}
sid/compute_vad_decision.sh --nj 40 --cmd "$train_cmd" \
data/${name} exp/make_vad $vaddir
utils/fix_data_dir.sh data/${name}
done

# 训练测试集提取出来的mfcc特征，训练网络
# Extract x-vectors used in the evaluation.
sid/nnet3/xvector/extract_xvectors.sh --cmd "$train_cmd --mem 4G" --nj 40 \
$nnet_dir data/voxceleb1_test \
$nnet_dir/xvectors_voxceleb1_test

$train_cmd exp/scores/log/voxceleb1_test_scoring.log \
ivector-plda-scoring --normalize-length=true \
"ivector-copy-plda --smoothing=0.0 $nnet_dir/xvectors_train/plda - |" \
"ark:ivector-subtract-global-mean $nnet_dir/xvectors_train/mean.vec scp:$nnet_dir/xvectors_voxceleb1_test/xvector.scp ark:- | transform-vec $nnet_dir/xvectors_train/transform.mat ark:- ark:- | ivector-normalize-length ark:- ark:- |" \
"ark:ivector-subtract-global-mean $nnet_dir/xvectors_train/mean.vec scp:$nnet_dir/xvectors_voxceleb1_test/xvector.scp ark:- | transform-vec $nnet_dir/xvectors_train/transform.mat ark:- ark:- | ivector-normalize-length ark:- ark:- |" \
"cat '$voxceleb1_trials' | cut -d\  --fields=1,2 |" exp/scores_voxceleb1_test || exit 1;

# 输出测试结果
eer=`compute-eer <(local/prepare_for_eer.py $voxceleb1_trials exp/scores_voxceleb1_test) 2> /dev/null`
mindcf1=`sid/compute_min_dcf.py --p-target 0.01 exp/scores_voxceleb1_test $voxceleb1_trials 2> /dev/null`
mindcf2=`sid/compute_min_dcf.py --p-target 0.001 exp/scores_voxceleb1_test $voxceleb1_trials 2> /dev/null`
echo "EER: $eer%"
echo "minDCF(p-target=0.01): $mindcf1"
echo "minDCF(p-target=0.001): $mindcf2"
